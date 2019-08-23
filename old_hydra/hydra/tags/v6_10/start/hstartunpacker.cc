//*-- Author : Walter Karig
//*-- Modified : 18/12/2001 by Ilse Koenig 
//*-- Modified : 29/11/2000 by Ilse Koenig 
//*-- Modified : 13/11/2000 by M. Golubeva
//*-- Modified : 05/06/98 by Manuel Sanchez

#include "hstartunpacker.h"
#include "hstartdef.h"
#include "hcategory.h"
#include "hstartraw.h"
#include "hevent.h"
#include "hldsubevt.h"
#include "hades.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hruntimedb.h"
#include "hstartlookup.h"
#include "hdebug.h"
#include <iostream.h>

//////////////////////////////////////////////////////////////////
// HStartUnpacker
//
// This is the HUnpacker used to read Start data from .hld files
//
//////////////////////////////////////////////////////////////////

HStartUnpacker::HStartUnpacker(Int_t id) {
  subEvtId=id;
  pRawCat=0;
  lookup=0;
}

Bool_t HStartUnpacker::init(void) {
  pRawCat=(HCategory*)gHades->getCurrentEvent()->getCategory(catStartRaw); 
  if (!pRawCat) {
    pRawCat=gHades->getSetup()->getDetector("Start")
            ->buildCategory(catStartRaw);
    if (!pRawCat) return kFALSE;
    gHades->getCurrentEvent()
          ->addCategory(catStartRaw,pRawCat,"Start");
  }
  loc.set(2,0,0);
  lookup=(HStartLookup*)(gHades->getRuntimeDb()
                         ->getContainer("StartLookup"));
  if(!lookup) return kFALSE;
  return kTRUE;
}

Int_t HStartUnpacker::execute() {
  HStartRaw* pRaw=0;
  Int_t crate=-1, slot=-1, nChan=0, chan=-1, mod=-1, strip=-1;
  if(pSubEvt) {
    UInt_t* data=pSubEvt->getData();
    UInt_t* end=pSubEvt->getEnd();
    // cout<<"----------------------------------------------"<<endl;
    while (++data<end && *data!=0x0) {
      slot=(*data >> 27 & 0x1f);
      crate=(*data >> 16 & 0xff);
      nChan=(*data >> 8 & 0xff);
      // cout <<"crate,slot,nChan: "
      //      <<crate<<" "<<slot<<" "<<nChan<<endl;
      HStartLookupTdc* pTdc=lookup->getTdc(crate-1,slot-1);
      if (pTdc) {
        for(Int_t i=0;i<nChan;i++) {
          data++;
	  chan = (*data & 0x001f0000) >> 16;
          HStartLookupChan* tc=pTdc->getChannel(chan);
          if (tc) {
            tc->getAddress(mod,strip);
            if (mod>=0) {
              loc[0]=mod;
              loc[1]=strip;
              pRaw=(HStartRaw*)pRawCat->getObject(loc);
              if (!pRaw) {
                pRaw=(HStartRaw*)pRawCat->getSlot(loc);
                if (pRaw) {
                  pRaw=new (pRaw) HStartRaw;
                  pRaw->fill(((*data & 0xfff) % 0xfff),mod,strip);
                  // cout <<"chan,mod,strip,data = "
                  //      <<chan<<" "<<mod<<" "<<strip<<" "
                  //      <<((*data & 0xfff) % 0xfff)<<endl;
                }
                else {
                  Error("HStartUnpacker::execute()", "can't get slot\n");
                  return -2;
                }
              }
            }
          }
        }
      } else data+=nChan;
      data++;
    }
  }
  return 1;
}

ClassImp(HStartUnpacker) 










