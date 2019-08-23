//*-- created : 011/03/2000 by R.Schicker
#include "htboxchan.h"
#include "htboxchansum.h"
#include "htboxsum.h"
#include "hdebug.h"
#include "hades.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hevent.h"
#include "hcategory.h"
#include "hlinearcategory.h"

#include <iostream.h>

////////////////////////////////////////////////////////////////
//
//  HTBoxSum: sums TBox data over many events
//  
////////////////////////////////////////////////////////////////


HTBoxSum::HTBoxSum(void) {
  tboxCat=0;
  tboxsCat=0;
  iter=0;
  iters=0;
  evsum=0;
}

HTBoxSum::HTBoxSum(Text_t *name,Text_t *title) : 
               HReconstructor(name,title) {
  tboxCat=0;
  tboxsCat=0;
  iter=0;
  iters=0;
  evsum=0;
}

HTBoxSum::~HTBoxSum(void) {
  if (iter) delete iter;
  if (iters) delete iters;
  iter=0;
  iters=0;
}

Bool_t HTBoxSum::init(void) {

  evsum=0;

  tboxCat=gHades->getCurrentEvent()->getCategory(catTBoxChan);

  if (!tboxCat) {
    tboxCat=gHades->getSetup()->getDetector("TBox")
                              ->buildCategory(catTBoxChan);
    if (!tboxCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catTBoxChan,tboxCat,"TBox");
  }

  tboxsCat=gHades->getCurrentEvent()->getCategory(catTBoxChanSum);
  if (!tboxsCat) {
    tboxsCat=gHades->getSetup()->getDetector("TBox")
                               ->buildCategory(catTBoxChanSum);
    if (!tboxsCat) return kFALSE;
    else 
        gHades->getCurrentEvent()->addCategory(catTBoxChanSum,tboxsCat,"TBox");
  }

  iter = (TIterator*)tboxCat->MakeIterator("native");
  iters = (TIterator*)tboxsCat->MakeIterator("native");
  
  Int_t nChan = ((HLinearCategory*)tboxCat)->getCollection()->GetSize();  
  HTBoxChanSum* chans;
  for(Int_t i=0;i<nChan;i++) {
    loc.set(1,i);
    chans = (HTBoxChanSum*) ((HLinearCategory*)tboxsCat)->getNewSlot(loc); 
    if (chans) chans = new(chans) HTBoxChanSum(i);
  }

  fActive=kTRUE;
  return kTRUE;
}

Int_t HTBoxSum::execute(void) {

  evsum+=1;
  HTBoxChan *tboxchan;
  HTBoxChanSum *tboxchans;
  
  Int_t chl=0, inl=0, dtl=0, redl=0;

  iter->Reset();
  while ((tboxchan=(HTBoxChan*)iter->Next())!=0) {
    tboxchan->get(chl,inl,dtl,redl);
    loc[0] = chl;
    tboxchans = (HTBoxChanSum*) tboxsCat->getSlot(loc); 
    tboxchans->add(chl,inl,dtl,redl);
  }
 
  return 0;
}

Bool_t HTBoxSum::finalize(void) {

  Int_t i1=0, i2=0, i3=0, i4=0;

  printf("\n--------------------------------------------------------------\n");
  printf("TBox scaler values for %i events: \n",evsum); 
 
  HTBoxChanSum *tboxchans=0;
  iters->Reset();
  while ((tboxchans=(HTBoxChanSum *)iters->Next())!=0) {
    tboxchans->get(i1,i2,i3,i4);
    printf("Chan %2i:  Raw= %10i   Inh= %10i   Red= %10i \n",
            i1,i2,i3,i4);
  }
  printf("\n--------------------------------------------------------------\n");

  return kTRUE;
}

ClassImp(HTBoxSum)















