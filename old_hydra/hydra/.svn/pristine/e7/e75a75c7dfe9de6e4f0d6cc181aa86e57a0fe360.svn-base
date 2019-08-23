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
  Int_t count=0;
  HTBoxChanSum *tboxchans=0;
  tboxCat=gHades->getCurrentEvent()->getCategory(catTBoxChan);

  if (!tboxCat) {
   tboxCat=gHades->getSetup()->getDetector("TBox")->buildCategory(catTBoxChan);
    if (!tboxCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catTBoxChan,tboxCat,"TBox");
  }

  tboxsCat=gHades->getCurrentEvent()->getCategory(catTBoxChanSum);
  if (!tboxsCat) {
    tboxsCat=gHades->getSetup()->getDetector("TBox")->buildCategory(catTBoxChanSum);

   if (!tboxsCat) return kFALSE;
   else gHades->getCurrentEvent()->addCategory(catTBoxChanSum,tboxsCat,"TBox");
  }

  iter=(TIterator *)tboxCat->MakeIterator("native");
  iters=(TIterator *)tboxsCat->MakeIterator("native");
  
  Int_t entries;  
  entries = (TClonesArray*) 
          ((HLinearCategory*) tboxCat)->getCollection()->GetSize();
  
  HTBoxChan* chan =0;
  HTBoxChanSum* chans =0;

  for(Int_t i=0;i<entries;i++){
  chan = (HTBoxChan*) ((HLinearCategory*)tboxCat)->getSlot( i ); 
  if (!chan) chan = new(chan) HTBoxChan();
  }  

  for(Int_t i=0;i<entries;i++){
  chans = (HTBoxChanSum*) ((HLinearCategory*)tboxsCat)->getSlot( i ); 
  if (!chans) chans = new(chans) HTBoxChanSum();
  }  

  iters->Reset();  
  while ((tboxchans=(HTBoxChanSum *)iters->Next())!=0) {
    tboxchans->set(count,0,0,0);
    count+=1;
  }

  // create one index and set it to zero
  loc.set(1,0);

  fActive=kTRUE;
  return kTRUE;
}

Int_t HTBoxSum::execute(void) {

  evsum+=1;
  HTBoxChan *tboxchan=0;
  HTBoxChanSum *tboxchans=0;
  
  Int_t count=0, chl=0, inl=0, dtl=0, redl=0;

  iter->Reset();

  while ((tboxchan=(HTBoxChan *)iter->Next())!=0) {
    loc[0] = count++;
    tboxchan->get(chl,inl,dtl,redl);
    tboxchans = (HTBoxChanSum*) tboxsCat->getSlot(loc); 
    tboxchans->add(chl,inl,dtl,redl);
  }
 
  return 0;
}

Bool_t HTBoxSum::finalize(void) {

  Int_t i1=0, i2=0, i3=0, i4=0;

  printf("    \n");
  printf(" SCALER VALUES FOR %8.1i EVENTS \n",evsum); 
 
  HTBoxChanSum *tboxchans=0;
  iters->Reset();
  while ((tboxchans=(HTBoxChanSum *)iters->Next())!=0) {
    tboxchans->get(i1,i2,i3,i4);
    printf(" channel %2.1i      input=%10.1i     deadtime=%10.1i       reduced=%6.1i \n",i1,i2,i3,i4);
  }

  return kTRUE;
}

ClassImp(HTBoxSum)















