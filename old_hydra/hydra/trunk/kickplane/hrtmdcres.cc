#include "hrtmdcres.h"
#include "hcategory.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "hiterator.h"
#include "hmdcgeompar.h"
#include "hades.h"
#include "hmdcdef.h"
#include "hspecgeompar.h"
#include "hmdcseg.h"
#include "hmdchit.h"
#include "hgeomvolume.h"


HRtMdcRes::HRtMdcRes(const Text_t name[],const Text_t title[], Int_t sec,Int_t mod) :
  HReconstructor(name,title) {
  fCatHit = fCatSeg = 0;
  fHitIter = fSegIter = 0;
  fLoc.set(2,sec,mod);
  fSegLoc.set(2,sec,0);
}


HRtMdcRes::~HRtMdcRes(void) {
  
}

Bool_t HRtMdcRes::init(void) {
  HRuntimeDb *rtdb = HRuntimeDb::instance();
  HEvent *ev = gHades->getCurrentEvent();

  if (ev) {
    fMdcGeometry = (HMdcGeomPar *)rtdb->getContainer("MdcGeomPar");
    fSpecGeometry = (HSpecGeomPar *)rtdb->getContainer("SpecGeomPar");
    
    fCatHit = ev->getCategory(catMdcHit);
    fCatSeg = ev->getCategory(catMdcSeg);
    if (!(fCatHit && fCatSeg)) return kFALSE;
    
    fHitIter = (HIterator *)fCatHit->MakeIterator();
    fSegIter = (HIterator *)fCatSeg->MakeIterator();
  }

  if (gHades->getOutputFile()) {
    gHades->getOutputFile()->cd();
    fData=new TNtuple("data","data","x:y:xex:yex");
  }
  return kTRUE;
}

Bool_t HRtMdcRes::finalize(void) {
  if (fData) {
    gHades->getOutputFile()->cd();
    fData->Write();
  }
  return kTRUE;
}

Bool_t HRtMdcRes::reinit(void) {
  HGeomTransform &TA = 
    fMdcGeometry->getModule(fLoc[0],fLoc[1])->getLabTransform();
  HGeomTransform &TS = fSpecGeometry->getSector(fLoc[0])->getTransform();
  
  // Transformation from module to sector system.
  fModTrans = TA;
  fModTrans.transTo(TS);
  fModTrans.invert();
  
  return kTRUE;
}

Int_t HRtMdcRes::execute(void) {
  HMdcSeg *seg;
  HMdcHit *hit;
  HGeomVector r0,alpha;
  HGeomVector r0Local;
  HGeomVector alphaLocal;
  Float_t r,z,theta,phi;
  Double_t pi2 = TMath::Pi() / 2.;
  Float_t x_ex,y_ex,x,y;

  fSegIter->Reset();
  fSegIter->gotoLocation(fSegLoc);
  while ( (seg =(HMdcSeg *)fSegIter->Next()) != 0) {
    r = seg->getR(); z=seg->getZ(); theta=seg->getTheta(); phi=seg->getPhi();
    r0.setXYZ( r*cos(phi+pi2), r*sin(phi+pi2), z);
    alpha.setXYZ( sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta) );
    
//     Warning("execute","target %.2f, %.2f, %.2f",r0.getX(),r0.getY(),r0.getZ());

    r0Local = fModTrans.transFrom(r0);
    alphaLocal = fModTrans.getRotMatrix() * alpha;
    
    x_ex = r0Local.getX()-(alphaLocal.getX()/alphaLocal.getZ())*r0Local.getZ();
    y_ex = r0Local.getY()-(alphaLocal.getY()/alphaLocal.getZ())*r0Local.getZ();
    
    fHitIter->Reset();
    fHitIter->gotoLocation(fLoc);
    while ( (hit = (HMdcHit *)fHitIter->Next()) != 0) {
      x = hit->getX(); y=hit->getY();
      if ( fabs(x-x_ex) < 100 && fabs(y-y_ex) < 100) {
	fData->Fill(x,y,x_ex,y_ex);
      }
    }
  }

  return 0;
}

ClassImp(HRtMdcRes)
