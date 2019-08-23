//*-- created : 011/03/2000 by R.Schicker
using namespace std;
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
#include "heventheader.h"

#include <iostream> 
#include <iomanip>

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

  nChan = ((HLinearCategory*)tboxCat)->getCollection()->GetSize();
  for (Int_t i=0;i<nChan;i++) {
    scaler1lsum[i] = 0;
    scaler2lsum[i] = 0;
    scaler3lsum[i] = 0;
    scaler4lsum[i] = 0;
  }
  evsum=0;
  timeInSpill=0;
  spill = kFALSE;
  gHades->getCurrentEvent()->getHeader()->SetBit(0x10000,spill);

  iter = (TIterator*)tboxCat->MakeIterator("native");
  iters = (TIterator*)tboxsCat->MakeIterator("native");

  fActive=kTRUE;
  return kTRUE;
}

Int_t HTBoxSum::execute(void) {

  evsum+=1;
  HTBoxChan *tboxchan;
  HTBoxChanSum *tboxchans;
  
  Float_t raw[8];
  Int_t save;
  Int_t chNl=0, scalD1l=0, scalD2l=0, scalD3l=0, scalD4l=0;

  iter->Reset();
  save = 0;
  while ((tboxchan=(HTBoxChan*)iter->Next())!=0) {
    tboxchan->get(chNl,scalD1l,scalD2l,scalD3l,scalD4l);
    loc.set(1,chNl);
    tboxchans = (HTBoxChanSum*) tboxsCat->getNewSlot(loc);
    if (tboxchans) tboxchans = new(tboxchans) HTBoxChanSum(chNl);
    scaler1lsum[chNl]+=scalD1l;
    scaler2lsum[chNl]+=scalD2l;
    scaler3lsum[chNl]+=scalD3l;
    scaler4lsum[chNl]+=scalD4l;
    raw[chNl]=scalD1l;
    if (chNl==5) { timeInSpill+=scalD1l; save=scalD1l;}
    tboxchans->set(chNl,scaler1lsum[chNl],scaler2lsum[chNl],scaler3lsum[chNl],scaler4lsum[chNl]);
    tboxchans->get(chNl,scalD1l,scalD2l,scalD3l,scalD4l);
  }

  if (gHades->getCurrentEvent()->getHeader()->getTBit() & 0x100) { // scaler ev
    spill = (raw[1]/raw[5] > 10.);  // spill on or off?
    if (spill && !gHades->getCurrentEvent()->getHeader()->TestBit(0x10000) ) {
      timeInSpill = save;  // if spill comes on, reset counter
    }

  }
  gHades->getCurrentEvent()->getHeader()->setTimeInSpill(timeInSpill);
  gHades->getCurrentEvent()->getHeader()->SetBit(0x10000,spill);

  return 0;
}

Bool_t HTBoxSum::finalize(void) {

  printf("\n----------------------------------------------------------------\n");
  printf("TBox scaler values for %i events: \n",evsum); 
 
  printf("               scaler1       scaler2       scaler3       scaler4 \n");
  for(Int_t i=0;i<nChan;i++) {
    printf("Chan %2i:   %11i   %11i   %11i   %11i \n",
            i,scaler1lsum[i],scaler2lsum[i],scaler3lsum[i],scaler4lsum[i]);
  }
  printf("\n----------------------------------------------------------------\n");

  return kTRUE;
}

ClassImp(HTBoxSum)















