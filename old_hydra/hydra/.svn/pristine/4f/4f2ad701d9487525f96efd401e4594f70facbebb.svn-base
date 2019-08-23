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
#include "hmdclayergeompar.h"
#include "hdatasource.h"
#include <iostream.h>

//*-- Author : A.Nekhaev
//*-- Modified: 28/05/99 by Ilse Koenig

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


void HMdcGeantRootReader::setParContainers() {
  fDigitGeomPar=(HMdcLayerGeomPar*)gHades->getRuntimeDb()->getContainer("MdcLayerGeomPar");
  if (!fDigitGeomPar) {
    fDigitGeomPar=new HMdcLayerGeomPar;
    gHades->getRuntimeDb()->addContainer(fDigitGeomPar);
  }
}


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

  fprintf(stderr,"\rProcessing event: %d", fEventId+1);   // event counter

  //*********** the return type is a Bool_t !!!!
  //if (t->GetEvent(fEventId)==0) return kDsEndData;  // end of tree reached
  if (t->GetEvent(fEventId)==0) return kFALSE;  // end of tree reached
  if (mdc_root.fNDataObjs>28800) {
    printf("\n**** Number of tracks too large\n");
    //    return kDsError;
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

    HMdcLayerGeomParLay& layer = (*fDigitGeomPar)[loc[0]][loc[1]][loc[2]];
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
	    
    if ((catDistForw>0.5*layer.getPitch()) &&
        (catDistBack<0.5*layer.getPitch())) {
      numFiredCell = long((catDistForw-0.5*layer.getPitch()) /
                     layer.getPitch())+2;
      Float_t cosalpha = cos(theta)/(sqrt(1-sin(theta)*sin(theta)*cos(phi)
                                     *cos(phi)));
      cosalpha < 0 ? cosalpha*=-1 : cosalpha;
      Float_t sinalpha = sqrt(1-cosalpha*cosalpha);
      Float_t minDist = (layer.getPitch() - yDist)*cosalpha;
// Only for 2 neighbouring cells!
      if(numFiredCell ==2) {
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
    if((catDistBack>0.5*layer.getPitch()) &&
       (catDistForw<0.5*layer.getPitch())) {
      numFiredCell = long((catDistBack-0.5*layer.getPitch())/
                           layer.getPitch())+2;
      Float_t cosalpha = cos(theta)/(sqrt(1-sin(theta)*sin(theta)*cos(phi)
                         *cos(phi)));
      cosalpha < 0 ? cosalpha*=-1 : cosalpha;
      Float_t sinalpha = sqrt(1-cosalpha*cosalpha);
      Float_t minDist = (layer.getPitch() - yDist)*cosalpha;
		
// Only for 2 neigbhouring cells!!!
      if(numFiredCell ==2) {
	if (minDist*cosalpha>0.5*layer.getPitch())
	    minDist = sqrt(0.5*layer.getPitch()*0.5*layer.getPitch() +
		     ((.5*layer.getPitch()-yDist)*(0.5*layer.getPitch()-yDist))
                     /tanalpha);
        if (minDist*sinalpha>0.5*layer.getCatDist())
	    minDist =sqrt((0.25*layer.getCatDist()*layer.getCatDist()) +
		     (((numFiredCell-1)*layer.getPitch() - catDistBack) *
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
    if((catDistForw>0.5*layer.getPitch()) &&
           (catDistBack>0.5*layer.getPitch())) {
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

//return kDsOk;
  return kTRUE;  // return type is Bool_t
}

Int_t HMdcGeantRootReader::cellNum(Int_t nmod, Int_t nlay, Float_t xcor, 
Float_t ycor) {

// Calculate the number of fired cell
// y - Y-coordinate of particle in current layer
// nmod - number of module
// nlay - number of layer
  HMdcLayerGeomParLay& layer = (*fDigitGeomPar)[0][nmod][nlay];

  Int_t nCell=-999;   
  Float_t Dist;
  
// Transformation from MDC's coordinate system to layer's coord. system:
  Float_t y_wire = ycor*cos(layer.getWireOrient()*pi/180.) -
                   xcor*sin(layer.getWireOrient()*pi/180.);
  
  Dist = (layer.getCentralWireNr()-1)*layer.getPitch()+y_wire+0.5*layer.getPitch();
  if(Dist>=0&&Dist<=((layer.getNumWires()-0.5)*layer.getPitch())) 
           nCell=(Int_t)(Dist/layer.getPitch());  

  return nCell;
}

Float_t HMdcGeantRootReader::perp(Int_t nmod, Int_t nlay, Int_t nWire, Float_t xcor, Float_t ycor, 
			  Float_t theta, Float_t phi) {
// Returns the shortest distance between particle trajectory and sense wire
// xcor - X-coordinate of particle in MDC.
// ycor - Y-coordinate of particle in MDC.
// theta - incident angle
// phi - azimuthal angle
  HMdcLayerGeomParLay& layer = (*fDigitGeomPar)[0][nmod][nlay];

// Transformation from MDC's coordinate system to layer's coord. system:

   Float_t y_wire = ycor*cos(layer.getWireOrient()*pi/180.) -
                  xcor*sin(layer.getWireOrient()*pi/180.);

// Distance to the nearest wire

   yDist = (layer.getCentralWireNr()-1)*layer.getPitch()+y_wire - 
           nWire*layer.getPitch();  //distance to the nearest wire
   yDist>0 ? yDist=yDist : yDist=-yDist;

// MDCTHETA is the angle of incidence (=polar angle with respect
// to the normal), MDCPHI is the angle in the x-y plane with respect
// to the x axis.
   
   Float_t theta_r = theta*pi/180.;
   Float_t phi_r = (phi-layer.getWireOrient())*pi/180.;
   Float_t perp = yDist*sqrt(1-sin(theta_r)*sin(theta_r)*cos(phi_r)*cos(phi_r));
   return perp;
  }
    



