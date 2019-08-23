#include "hdilepfilter.h"
#include "hreconstructor.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hspectrometer.h"
#include "hades.h"
#include "hevent.h"
#include "piddef.h"
#include "hlinearcategory.h"
#include <iostream.h>
//*-- Author : J.Bielcik
//*-- Modified :29.11.2002
////////////////////////////////////////////////////////////
// HDiLeptonFilter                                          //  
// Should filter HPidDileptons puting some cuts            //
////////////////////////////////////////////////////////////



HDiLepFilter::HDiLepFilter(void):HReconstructor (){
  ; 
}

HDiLepFilter::HDiLepFilter(Text_t *name,Text_t *title) :HReconstructor (name,title) {
  ; 
}

HDiLepFilter::~HDiLepFilter(void){;}

Bool_t HDiLepFilter::init(void) {
  
  fPidDilepton=gHades->getCurrentEvent()->getCategory(catPidDilepton);
  if (!fPidDilepton) {
    Error("init()","no HPidParticle hit category in input");
  }
  
  fPidDileptonIter =(HIterator*) fPidDilepton->MakeIterator();
  return kTRUE;
}



Bool_t HDiLepFilter::finalize(void){

  return kTRUE;
}




Int_t HDiLepFilter::execute(void) { 
  
  if(fPidDilepton->getEntries()<1) return kSkipEvent;
  
   //maybe later one can apply here some filter
  return 0;
}


ClassImp(HDiLepFilter)
