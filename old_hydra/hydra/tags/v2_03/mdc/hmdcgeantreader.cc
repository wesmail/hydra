#include "hmdcgeantreader.h"
#include "hmdcdef.h"
#include "hdebug.h"
#include "hades.h"
#include "hmdcgeantraw.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hevent.h"
#include "hcategory.h"
#include "hlocation.h"
#include "hmdcdigitgeompar.h"
#include "hmdclookupgeom.h"
#include <iostream.h>

//*-- Author : Alexander Nekhaev
//*-- Modified : 05/03/99 by 

////////////////////////////////////////////////////////////////
//
//  HMdcGeantReader reads geant ntupls after h2root conversion,  
//  makes preliminary transformation before digitization procedure
//  and stores information in MdcGeantRaw category.
//  
////////////////////////////////////////////////////////////////

ClassImp(HMdcGeantReader)



HMdcGeantReader::HMdcGeantReader(void) {
  fGeantRawCat=0;
  fDigitGeomPar=0;
  setParContainers();
}

HMdcGeantReader::~HMdcGeantReader(void) {
  if (fDigitGeomPar) gHades->getRuntimeDb()->removeContainer("MdcDigitGeomPar");
}

void HMdcGeantReader::setParContainers() {
  fDigitGeomPar=(HMdcDigitGeomPar*)gHades->getRuntimeDb()->getContainer("MdcDigitGeomPar");
  if (!fDigitGeomPar) {
    fDigitGeomPar=new HMdcDigitGeomPar;
    gHades->getRuntimeDb()->addContainer(fDigitGeomPar);
  }
}

struct mdc_tuple{
  Int_t idevt, nhit, mdctrk[100];
  Float_t mdcx[100][6],mdcy[100][6],mdctheta[100][6];
  Float_t mdcphi[100][6],mdctof[100][6];
} mdc;

Bool_t HMdcGeantReader::init(void) {
  fGeantRawCat=gHades->getCurrentEvent()->getCategory(catMdcGeantRaw);
  if (!fGeantRawCat) {
    fGeantRawCat=gHades->getSetup()->getDetector("Mdc")->buildCategory(catMdcGeantRaw);
    if (!fGeantRawCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catMdcGeantRaw,fGeantRawCat,"Mdc");
  }
  loc.set(4,0,0,0,0);
  //  fActive=kTRUE;

  pi=acos(-1);
  HLocation loc;
  char* trname[24] = { "h3011","h3012","h3013","h3014",
		       "h3021","h3022","h3023","h3024",
		       "h3031","h3032","h3033","h3034",
		       "h3041","h3042","h3043","h3044",
		       "h3051","h3052","h3053","h3054",
		       "h3061","h3062","h3063","h3064" };
    
  loc.set(4,0,0,0,0);
  Int_t i;
  for(i=0;i<24;i++) tr[i]=(TTree*)fInputFile->Get(trname[i]);
  for(i=0;i<24;i++) {
    tr[i]->SetBranchAddress("Idevt",&mdc.idevt);
    tr[i]->SetBranchAddress("Nhit",&mdc.nhit);
    tr[i]->SetBranchAddress("Mdctrk",mdc.mdctrk);
    tr[i]->SetBranchAddress("Mdcx",mdc.mdcx);
    tr[i]->SetBranchAddress("Mdcy",mdc.mdcy);
    tr[i]->SetBranchAddress("Mdctheta",mdc.mdctheta);
    tr[i]->SetBranchAddress("Mdcphi",mdc.mdcphi);
    tr[i]->SetBranchAddress("Mdctof",mdc.mdctof);
  }

  return kTRUE;
}


Bool_t HMdcGeantReader::execute(void) {
  fprintf(stderr,"\rEvents processed: %d", fEventId+1); 
  for(loc[0]=0;loc[0]<6;loc[0]++) {
    for(loc[1]=0;loc[1]<4;loc[1]++) {
      Int_t n = loc[0]*4+loc[1];
      tr[n]->GetEvent(fEventId);
      Int_t j;
      for(j=0;j<mdc.nhit;j++) {
	for(loc[2]=0;loc[2]<6;loc[2]++) {
// loc[0] - sector 
// loc[1] - module
// loc[2] - layer
// loc[3] - cell


	  HMdcDigitGeomParLayer& layer = (*fDigitGeomPar)[loc[0]][loc[1]][loc[2]];

                
// The bug in Geant ???
	  if(mdc.mdctof[j][loc[2]]==0.0) continue;

	  loc[3] = cellNum(loc[1],loc[2],mdc.mdcx[j][loc[2]], mdc.mdcy[j][loc[2]]);
	  if(loc[3]==-999) continue;
	  
// Calculate the shortest distance to the sense wire.
	  Float_t per = perp(loc[1],loc[2],loc[3],mdc.mdcx[j][loc[2]],
			     mdc.mdcy[j][loc[2]],mdc.mdctheta[j][loc[2]],
			     mdc.mdcphi[j][loc[2]]);
	  
	  hit=(HMdcGeantRaw *)fGeantRawCat->getObject(loc);

	  if (!hit) {
	    hit=(HMdcGeantRaw *)fGeantRawCat->getNewSlot(loc);
	    hit = new(hit) HMdcGeantRaw;
	  }
	  nHit=hit->getNumHits();
          hit->setSector(loc[0]);
          hit->setModule(loc[1]);
          hit->setLayer(loc[2]);
          hit->setCell(loc[3]);
	  hit->setNumHits(nHit+1);
	  hit->setMinDist(per, nHit);
	  hit->setTimeFlight(mdc.mdctof[j][loc[2]], nHit);
	  
	  //	  if(r==2) {
// Double fired cells, at this momet only particles which go through 2 neighbouring cells!
	    Float_t theta = mdc.mdctheta[j][loc[2]]*pi/180.;
	    Float_t phi = (mdc.mdcphi[j][loc[2]]-layer.getWireOrient())*pi/180.;
	    Float_t tanalpha =  sin(theta)*sin(phi)/cos(theta);
	    Float_t catDistBack = (yDist + 0.5*layer.getCatDist()*tanalpha);
	    Float_t catDistForw = (yDist - 0.5*layer.getCatDist()*tanalpha);
	    catDistBack<0 ? catDistBack*=-1. : catDistBack;
	    catDistForw<0 ? catDistForw*=-1 : catDistForw;
	    Int_t numFiredCell = 1;
	    
	    if((catDistForw>0.5*layer.getPitch())&&(catDistBack<0.5*layer.getPitch()))
	      {
		numFiredCell = long((catDistForw-0.5*layer.getPitch())/layer.getPitch())+2;
		Float_t cosalpha = cos(theta)/(sqrt(1-sin(theta)*sin(theta)*cos(phi)*cos(phi)));
		cosalpha < 0 ? cosalpha*=-1 : cosalpha;
		Float_t sinalpha = sqrt(1-cosalpha*cosalpha);
		Float_t minDist = (layer.getPitch() - yDist)*cosalpha;
// Only for 2 neighbouring cells!
		if(numFiredCell ==2)
		  {
		    if (minDist*cosalpha>0.5*layer.getPitch())
		      minDist = sqrt(0.5*layer.getPitch()*0.5*layer.getPitch() +
				     ((0.5*layer.getPitch()-yDist)*(0.5*layer.getPitch()))/tanalpha);
		    if (minDist*sinalpha>0.5*layer.getCatDist())
		      minDist =sqrt((0.5*layer.getCatDist()*0.5*layer.getCatDist()) +
				    (((numFiredCell-1)*layer.getPitch() - catDistForw)*
				     ((numFiredCell-1)*layer.getPitch() - catDistForw)));
		    Int_t numCellNeigh;
		    tanalpha<0 ? numCellNeigh=loc[3]+1 : numCellNeigh=loc[3]-1;
		    if(numCellNeigh>=layer.getNumWires()||numCellNeigh<0) continue;
		    loc[3] = numCellNeigh;
		    hitNeigh = (HMdcGeantRaw*) fGeantRawCat->getObject(loc);
		    if (!hitNeigh) {
		      hitNeigh = (HMdcGeantRaw*) fGeantRawCat->getSlot(loc);
		      hitNeigh = new(hitNeigh) HMdcGeantRaw;
		    } 
		    nHit = hitNeigh->getNumHits();
		    hitNeigh->setSector(loc[0]);
		    hitNeigh->setModule(loc[1]);
		    hitNeigh->setLayer(loc[2]);
		    hitNeigh->setCell(loc[3]);
		    hitNeigh->setNumHits(nHit+1);
		    hitNeigh->setMinDist(minDist, nHit);
		    hitNeigh->setTimeFlight(mdc.mdctof[j][loc[2]], nHit);
		  }
	      }
	    if((catDistBack>0.5*layer.getPitch())&&(catDistForw<0.5*layer.getPitch()))
	      {
		numFiredCell = long((catDistBack-0.5*layer.getPitch())/layer.getPitch())+2;
		Float_t cosalpha = cos(theta)/(sqrt(1-sin(theta)*sin(theta)*cos(phi)*cos(phi)));
		cosalpha < 0 ? cosalpha*=-1 : cosalpha;
		Float_t sinalpha = sqrt(1-cosalpha*cosalpha);
		Float_t minDist = (layer.getPitch() - yDist)*cosalpha;
		
// Only for 2 neigbhouring cells!!!
		if(numFiredCell ==2)
		  {
		    if (minDist*cosalpha>0.5*layer.getPitch())
		      minDist = sqrt(0.5*layer.getPitch()*0.5*layer.getPitch() +
				     ((0.5*layer.getPitch()-yDist)*(0.5*layer.getPitch()-yDist))/tanalpha);
		    if (minDist*sinalpha>0.5*layer.getCatDist())
		      minDist =sqrt((0.25*layer.getCatDist()*layer.getCatDist()) +
				    (((numFiredCell-1)*layer.getPitch() - catDistBack)*
				     ((numFiredCell-1)*layer.getPitch() - catDistBack)));
		    Int_t numCellNeigh;
		    tanalpha<0 ? numCellNeigh=loc[3]-1 : numCellNeigh=loc[3]+1;
		    if(numCellNeigh>=layer.getNumWires()||numCellNeigh<0) continue;
		    loc[3] = numCellNeigh;
		    hitNeigh = (HMdcGeantRaw*) fGeantRawCat->getObject(loc);
		    if (!hitNeigh) {
		      hitNeigh = (HMdcGeantRaw*) fGeantRawCat->getSlot(loc);
		      hitNeigh = new(hitNeigh) HMdcGeantRaw;
		    } 
		    nHit = hitNeigh->getNumHits();
		    hitNeigh->setSector(loc[0]);
		    hitNeigh->setModule(loc[1]);
		    hitNeigh->setLayer(loc[2]);
		    hitNeigh->setCell(loc[3]);
		    hitNeigh->setNumHits(nHit+1);
		    hitNeigh->setMinDist(minDist, nHit);
		    hitNeigh->setTimeFlight(mdc.mdctof[j][loc[2]], nHit);
		  }
	      }
	    if((catDistForw>0.5*layer.getPitch())&&(catDistBack>0.5*layer.getPitch()))
	      {
		numFiredCell = long((catDistBack-0.5*layer.getPitch())/layer.getPitch())+
		  long((catDistForw-0.5*layer.getPitch())/layer.getPitch())+3;
	      }
	    //	  }
// Loop over neighbouring cells are closed

	}
      }
    }
  }
  
  fEventId++;

#if DEBUG_LEVEL>2
	gDebuger->leaveFunc("HMdcGReader::execute");
#endif

	return kTRUE;
}

Int_t HMdcGeantReader::cellNum(Int_t nmod, Int_t nlay, Float_t xcor, 
Float_t ycor) {
// Calculate the number of fired cell
// y - Y-coordinate of particle in current layer
// nmod - number of module
// nlay - number of layer
  HMdcDigitGeomParLayer& layer = (*fDigitGeomPar)[0][nmod][nlay];

  Int_t nCell;   
  Float_t Dist;
  
// Transformation from MDC's coordinate system to layer's coord. system:
  Float_t y_wire = ycor*cos(layer.getWireOrient()*pi/180.) -
    xcor*sin(layer.getWireOrient()*pi/180.);
  
  Dist = (layer.getCentralWireNr()-1)*layer.getPitch()+
    y_wire+0.5*layer.getPitch();
  if(Dist>=0&&Dist<=((layer.getNumWires()-0.5)*layer.getPitch())) 
    nCell=(Int_t)(Dist/layer.getPitch());
  else 
    nCell=-999;
  return nCell;
}

Float_t HMdcGeantReader::perp(Int_t nmod, Int_t nlay, Int_t nWire, Float_t xcor, Float_t ycor, 
			  Float_t theta, Float_t phi) {
// Returns the shortest distance between particle trajectory and sense wire
// xcor - X-coordinate of particle in MDC.
// ycor - Y-coordinate of particle in MDC.
// theta - incident angle
// phi - azimuthal angle
  HMdcDigitGeomParLayer& layer = (*fDigitGeomPar)[0][nmod][nlay];

// Transformation from MDC's coordinate system to layer's coord. system:

  Float_t y_wire = ycor*cos(layer.getWireOrient()*pi/180.) -
    xcor*sin(layer.getWireOrient()*pi/180.);

// Distance to the nearest wire

  yDist = (layer.getCentralWireNr()-1)*layer.getPitch()+y_wire - 
	   nWire*layer.getPitch();  //distance to the nearest wire
  yDist>0 ? : yDist=-yDist;

// MDCTHETA is the angle of incidence (=polar angle with respect
// to the normal), MDCPHI is the angle in the x-y plane with respect
// to the x axis.
   
  Float_t theta_r = theta*pi/180.;
  Float_t phi_r = (phi-layer.getWireOrient())*pi/180.;
  Float_t perp = yDist*sqrt(1-sin(theta_r)*sin(theta_r)*cos(phi_r)*cos(phi_r));

   return perp;
  }
    




