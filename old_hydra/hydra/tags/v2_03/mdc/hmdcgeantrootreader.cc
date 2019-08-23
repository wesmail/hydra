#include "hmdcgeantrootreader.h"
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
#include "hmatrixcategory.h"
#include "hindextable.h"
#include <iostream.h>

////////////////////////////////
//  HMdcGeantRootReader
//
////////////////////////////////

ClassImp(HMdcGeantRootReader)



HMdcGeantRootReader::HMdcGeantRootReader(void) {
  fGeantRawCat=0;
  fDigitGeomPar=0;
  setParContainers();
}


HMdcGeantRootReader::~HMdcGeantRootReader(void) {
  if (fDigitGeomPar) gHades->getRuntimeDb()->removeContainer("MdcDigitGeomPar");
}

void HMdcGeantRootReader::setParContainers() {
  fDigitGeomPar=(HMdcDigitGeomPar*)gHades->getRuntimeDb()->getContainer("MdcDigitGeomPar");
  if (!fDigitGeomPar) {
    fDigitGeomPar=new HMdcDigitGeomPar;
    gHades->getRuntimeDb()->addContainer(fDigitGeomPar);
  }
}

struct geant_root {
  HMatrixCategory oHMatrixCategory;
  HIndexTable     oHIndexTable; 
  Int_t           fNDataObjs;
  Int_t   trackNumber[28800];            // GEANT track number
  Float_t xHit[28800];                   // x of hit  (in cm)
  Float_t yHit[28800];                   // y of hit  (in cm)
  Float_t thetaHit[28800];               // theta of hit  (0-180 deg)
  Float_t phiHit[28800];                 // phi of hit  (0-360 deg)
  Float_t tofHit[28800];                 // time of flight of hit  (in ns)
  Char_t  sector[28800];                 // sector number  (0-5)
  Char_t  module[28800];                 // module number  (0-3)
  Char_t  layer[28800];                  // layer number  (0-5)
} mdc_root;

Bool_t HMdcGeantRootReader::init(void) {
  fGeantRawCat=gHades->getCurrentEvent()->getCategory(catMdcGeantRaw);
  if (!fGeantRawCat) {
    fGeantRawCat=gHades->getSetup()->getDetector("Mdc")->buildCategory(catMdcGeantRaw);
    if (!fGeantRawCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catMdcGeantRaw,fGeantRawCat,"Mdc");
  }


  pi=acos(-1);
  HLocation loc;

  t=(TTree*)fInputFile->Get("T");
  t->SetBranchAddress("HGeantMdc",&mdc_root.oHMatrixCategory);
  t->SetBranchAddress("GeantMdc.*fIndexTable",&mdc_root.oHIndexTable);
  t->SetBranchAddress("HGeantMdc.fNDataObjs",&mdc_root.fNDataObjs);
  t->SetBranchAddress("HGeantMdc.fData.trackNumber", mdc_root.trackNumber);
  t->SetBranchAddress("HGeantMdc.fData.xHit", mdc_root.xHit);
  t->SetBranchAddress("HGeantMdc.fData.yHit", mdc_root.yHit);
  t->SetBranchAddress("HGeantMdc.fData.thetaHit", mdc_root.thetaHit);
  t->SetBranchAddress("HGeantMdc.fData.phiHit", mdc_root.phiHit);
  t->SetBranchAddress("HGeantMdc.fData.tofHit", mdc_root.tofHit);
  t->SetBranchAddress("HGeantMdc.fData.sector", mdc_root.sector);
  t->SetBranchAddress("HGeantMdc.fData.module", mdc_root.module);
  t->SetBranchAddress("HGeantMdc.fData.layer", mdc_root.layer);
  return kTRUE;
}

Bool_t HMdcGeantRootReader::execute(void) {


  fprintf(stderr,"\rEvents processed: %d", fEventId+1); 
    t->GetEvent(fEventId);
    if (mdc_root.fNDataObjs>28800) {
      printf("Number of tracks too large\n");
      return kFALSE;
    }

  for (Int_t j=0;j<mdc_root.fNDataObjs;j++ ) {
// loc[0] - sector 
// loc[1] - module
// loc[2] - layer
// loc[3] - cell

    loc.set(4,0,0,0,0);
    loc[0] = (Int_t)mdc_root.sector[j];
    loc[1] = (Int_t)mdc_root.module[j]; 
    loc[2] = (Int_t)mdc_root.layer[j];

    HMdcDigitGeomParLayer& layer = (*fDigitGeomPar)[loc[0]][loc[1]][loc[2]];
// The bug in Geant ???
	  if(mdc_root.tofHit[j]==0.0) continue;

	  loc[3] = cellNum(loc[1],loc[2],mdc_root.xHit[j], mdc_root.yHit[j]);
	  if(loc[3]==-999) continue;
  
// Calculate the shortest distance to the sense wire.
	  Float_t per = perp(loc[1],loc[2],loc[3],mdc_root.xHit[j],
			     mdc_root.yHit[j],mdc_root.thetaHit[j],mdc_root.phiHit[j]);
	  
	  hit=(HMdcGeantRaw *)fGeantRawCat->getObject(loc);

	  if (!hit) {
	    hit=(HMdcGeantRaw *)fGeantRawCat->getSlot(loc);
	    hit = new(hit) HMdcGeantRaw;
	  }
          nHit=hit->getNumHits();
          hit->setSector(loc[0]);
          hit->setModule(loc[1]);
          hit->setLayer(loc[2]);
          hit->setCell(loc[3]);
	  hit->setNumHits(nHit+1);
	  hit->setMinDist(per, nHit);
	  hit->setTimeFlight(mdc_root.tofHit[j], nHit);
  
	  //	  if(r==2) {
// Double fired cells, at this momet only particles which go through 2 neighbouring cells!
	    Float_t theta = mdc_root.thetaHit[j]*pi/180.;
	    Float_t phi = (mdc_root.phiHit[j]-layer.getWireOrient())*pi/180.;
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
		    hitNeigh->setTimeFlight(mdc_root.tofHit[j], nHit);
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
		    hitNeigh->setTimeFlight(mdc_root.tofHit[j], nHit);
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
   
  
  fEventId++;

#if DEBUG_LEVEL>2
	gDebuger->leaveFunc("HMdcGReader::execute");
#endif

	return kTRUE;
}

Int_t HMdcGeantRootReader::cellNum(Int_t nmod, Int_t nlay, Float_t xcor, 
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
  
  Dist = (layer.getCentralWireNr()-1)*layer.getPitch()+y_wire+
    0.5*layer.getPitch();
  if(Dist>=0&&Dist<=((layer.getNumWires()-0.5)*layer.getPitch())) 
    nCell=(Int_t)(Dist/layer.getPitch());  
  else 
    nCell=-999;

  return nCell;
}

Float_t HMdcGeantRootReader::perp(Int_t nmod, Int_t nlay, Int_t nWire, Float_t xcor, Float_t ycor, 
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
    
