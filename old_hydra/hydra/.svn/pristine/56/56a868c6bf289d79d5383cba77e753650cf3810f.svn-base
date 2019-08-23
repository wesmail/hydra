#include "hrtextractor.h"
#include "hcategory.h"
#include "hades.h"
#include "hiterator.h"
#include "hgeantmdc.h"
#include "hgeantkine.h"

HRtExtractor::HRtExtractor(Text_t name[],Text_t title[]) : 
  HReconstructor(name,title) {
  fCatMdc = 0;
  fCatKine = 0;
  fMdcIter = 0;
  fKineIter = 0;
  fData = 0;
}

HRtExtractor::~HRtExtractor(void) {
}

Bool_t HRtExtractor::init(void) {
  HEvent *ev = 0;
  if (!gHades) return kFALSE;
  if ( (ev=gHades->getCurrentEvent())==0 ) return kFALSE;

  fCatMdc = ev->getCategory(catMdcGeantRaw);
  if (!fCatMdc) {
    Error("init","Geant MDC category not found!");
    return kFALSE;
  }
  fMdcIter = (HIterator *)fCatMdc->MakeIterator();

  fCatKine = ev->getCategory(catGeantKine);
  if (!fCatKine) {
    Error("init","Kine category not found in input");
    return kFALSE;
  }
  fKineIter = (HIterator *)fCatKine->MakeIterator();

  if (gHades->getOutputFile()) {
    gHades->getOutputFile()->cd();
    fData = new TNtuple("data","data",
			"p:r:z:theta:phi:x1:y1:x2:y2:x3:y3:x4:y4");
  }  else {
    Warning("init","No output file found!");
    fData = 0;
  }
  
  return kTRUE;
}

Bool_t HRtExtractor::finalize(void) {
 /* if (fData) {
    gHades->getOutputFile()->cd();
    fData->Write();
  }*/
  return kTRUE;
}

Int_t HRtExtractor::execute(void) {
  const Int_t theLayer = 6;
  HGeantMdc *geMdc = 0;
  HGeantKine *geKine = 0;
  Float_t x[4],y[4];
  Float_t tof,pMdc[4];
  Float_t px,py,pz,p;
  Float_t theta,phi;
  Int_t module;
  Int_t count = 0;
  Int_t trackN,pid;
  Float_t momentum;

  fKineIter->Reset();
  while ( (geKine = (HGeantKine *)fKineIter->Next()) != 0) {
    geKine->resetMdcIter();

    for (Int_t i=0;i<4;i++) {x[i]=y[i]=pMdc[i]=10000.;}
    count = 0;
    while ( (geMdc = (HGeantMdc *)geKine->nextMdcHit()) != 0) {
      if (geMdc->getLayer() == theLayer) {
	module = geMdc->getModule();
	geMdc->getHit(x[module],y[module],tof,pMdc[module]);
	count++;
      }
    }
   
    if (count>0) { //Store only tracks with at least one hit in MDC
      //FIXME: For the moment r=z=0 always. Translate to sector coord.
      geKine->getMomentum(px,py,pz);
      geKine->getParticle(trackN,pid);
      p = TMath::Sqrt(px*px + py*py + pz*pz);
      theta = TMath::ACos(pz/p);
      phi = TMath::ATan2(py,px);
      momentum = (pMdc[0]<9000.)?pMdc[0]:p;
      if (pid == 3) { //Electrons
	momentum = -momentum;
      } else if (pid!=2) Error("execute","Unknown PID");
      fData->Fill(momentum,0,0,theta,phi,
	  x[0],y[0],x[1],y[1],x[2],y[2],x[3],y[3]);
    } 
  }  
  
  return 0;
}


ClassImp(HRtExtractor)
