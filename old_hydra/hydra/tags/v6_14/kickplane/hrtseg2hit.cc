#include "hrtseg2hit.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "hcategory.h"
#include "hmdcdetector.h"
#include "hmdcdef.h"
#include "hades.h"
#include "hgeomtransform.h"
#include "hmdcgeompar.h"
#include "hgeomvector.h"
#include "hmdcseg.h"
#include "hmdchit.h"
#include "hmdchitsim.h"
#include "hmdcsegsim.h"
#include "hspectrometer.h"

HRtSeg2Hit::HRtSeg2Hit(Text_t name[],Text_t title[]) : HReconstructor(name,title) {
  fSegCat = fHitCat = 0;
  fSegIter = 0;
  fMdcGeometry = 0;
  fLeverArm = 1; //Default is outer lever arm
}

HRtSeg2Hit::~HRtSeg2Hit(void) {
}

Bool_t HRtSeg2Hit::init(void) {
  HRuntimeDb *rtdb = Hades::instance()->getRuntimeDb();
  HEvent *ev = Hades::instance()->getCurrentEvent();
  HMdcDetector *mdc = (HMdcDetector *)Hades::instance()->getSetup()
    ->getDetector("Mdc");
  if (rtdb && ev) {
    if ( (fSegCat = ev->getCategory(catMdcSeg)) == 0) {
      Error("init","Segment category not found");
      return kFALSE;
    }
    fSegIter = (HIterator *)fSegCat->MakeIterator("native"); 
    //FIXME: Substitute native iteraros by unordered
	  
    if ( (fHitCat = ev->getCategory(catMdcHit)) == 0) {
      if ( (fHitCat = mdc->buildMatrixCategory("HMdcHitSim",0.5)) == 0) {
	Error("init","catMdcHit could not be built");
	return kFALSE;
      } else {
	ev->addCategory(catMdcHit,fHitCat,"Mdc");
      }
    } else Warning("init","Reusing previously created catMdcHit for output");
    
    fMdcGeometry = (HMdcGeomPar *)rtdb->getContainer("MdcGeomPar");
  }
  fHitLoc.set(2,0,2);
  return kTRUE;
}

Int_t HRtSeg2Hit::execute(void) {
  HMdcSeg *seg = 0;
  HGeomVector r,alpha,rLoc,alphaLoc;
  Float_t theta,phi,pi2 = TMath::Pi()/2.;

  fSegIter->Reset();
  while ( (seg = (HMdcSeg *)fSegIter->Next()) != 0) {
    if (seg->getIOSeg() == 1) { //Outer segment
      HModGeomPar *mod = fMdcGeometry->getModule(0,2);
      HGeomTransform &trans = mod->getLabTransform();

      // Obtain normal representation of the straight line.
      theta = seg->getTheta();
      phi = seg->getPhi();
      r.setZ(seg->getZ ());
      r.setX(seg->getR () * TMath::Cos (phi + pi2));
      r.setY(seg->getR () * TMath::Sin (phi + pi2));
      
      alpha.setZ(TMath::Cos (theta));
      alpha.setY(TMath::Sin (theta) * TMath::Sin (phi));
      alpha.setX(TMath::Sin (theta) * TMath::Cos (phi));

      //Go to local system
      rLoc = trans.transTo(r);
      alphaLoc = trans.getRotMatrix().inverse() * alpha;

      //Extrapolate to z=0
      rLoc.setXYZ(rLoc.getX() + 
		  (alphaLoc.getX()/alphaLoc.getZ()) * (0 - rLoc.getZ()),
		  rLoc.getY() + 
		  (alphaLoc.getY()/alphaLoc.getZ()) * (0 - rLoc.getZ()),
		  0.);

      //Fill to output
      fillData(seg,rLoc,alphaLoc);
    }
  }
  return 0;
}


HMdcHit *HRtSeg2Hit::fillData(HMdcSeg *seg,HGeomVector &r,HGeomVector &alpha) {
  HMdcHitSim *hit = 0;
  HMdcSegSim *segsim = 0;
  TObject *slot = 0;
  Int_t lsTracks[5];
  UChar_t lsTimes[5];
  Int_t nTracks;

  fHitLoc[0] = seg->getSec();
  fHitLoc[1] = 2; //MDC3

  slot = fHitCat->getNewSlot(fHitLoc);
  if (slot) {
    hit = new(slot) HMdcHitSim;
    hit->setXY(r.getX(),0.16,r.getY(),0.08);
    hit->setXYDir(alpha.getX(),0.01,alpha.getY(),0.01);
    hit->setChi2(0);
    hit->setSecMod(fHitLoc[0],fHitLoc[1]);

    segsim = dynamic_cast<HMdcSegSim *>(seg);
    if (segsim) {
      nTracks = segsim->getNTracks();
      for (int i=0; i < nTracks; i++) {
	lsTracks[i] = segsim->getTrack(i);
	lsTimes[i] = segsim->getNTimes(i);
      }
      hit->setNTracks(nTracks,lsTracks,lsTimes);
    }
  } else return 0;
  return hit;
}


ClassImp(HRtSeg2Hit)
