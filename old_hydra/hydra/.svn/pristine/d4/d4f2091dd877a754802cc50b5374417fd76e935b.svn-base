#include "hmdchitindexcorr.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hspectrometer.h"
#include "hades.h"
#include "hevent.h"
#include "hreconstructor.h"
#include "hiterator.h"
#include "hmdctrackddef.h"
#include "hmdcseg.h"
#include <iostream>
using namespace std;

//*-- Author : J.Bielcik
//*-- Modified :02.04.2003
//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////
// HMdcHitIndexCorr                                      ///  
// Reconstructor loop over segments and puting correct   ///
// indexes of corresponding hits. Up to 02.04.2003       ///
// indexes was not correlated correctly for case where   ///
// fitting was in setup.                                 ///
////////////////////////////////////////////////////////////

HMdcHitIndexCorr::HMdcHitIndexCorr(void):HReconstructor (){
  ;
}

HMdcHitIndexCorr::HMdcHitIndexCorr(const Text_t *name,const Text_t *title) :HReconstructor (name,title) {
  ;  
}
HMdcHitIndexCorr::~HMdcHitIndexCorr(void){;}

Bool_t HMdcHitIndexCorr::init(void){
  
  fMdcSegCat=gHades->getCurrentEvent()->getCategory(catMdcSeg);
  if (!fMdcSegCat) {
    cout<<"there is no catMdcHit !"<<endl;
    return kFALSE;
  }

  fMdcSegIter =(HIterator*) fMdcSegCat->MakeIterator("native");
  return kTRUE; 
}

Bool_t HMdcHitIndexCorr::finalize(void){
  return kTRUE;
}

Int_t HMdcHitIndexCorr::execute(void){
  //Iterating over segments in MdcSeg and seting correct index of
  //the corresponding  
  HMdcSeg* pMdcSeg;
  fMdcSegIter->Reset();
  Int_t hitIndex=0;
  while( (pMdcSeg=(HMdcSeg*)fMdcSegIter->Next()) !=0) {
    for(Int_t mod=0;mod<2;mod++) {
      Int_t ind=pMdcSeg->getHitInd(mod);
      if(ind<0) continue;
      if(ind!=hitIndex) pMdcSeg->setHitInd(mod,hitIndex);
      hitIndex++;
    }
  }
  return 0;
}
ClassImp(HMdcHitIndexCorr) 
