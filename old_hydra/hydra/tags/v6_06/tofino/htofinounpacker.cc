//*-- Author : Ilse Koenig
//*-- Created: 01/12/2000

#include "htofinounpacker.h"
#include "tofinodef.h"
#include "hcategory.h"
#include "htofinoraw.h"
#include "hevent.h"
#include "hldsubevt.h"
#include "hades.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hruntimedb.h"
#include "htofinolookup.h"
#include "hdebug.h"
#include <iostream.h>

ClassImp(HTofinoUnpacker)
 
//////////////////////////////////////////////////////////////////
// HTofinoUnpacker
//
// This is the HUnpacker used to read Tofino data from .hld files
//
//////////////////////////////////////////////////////////////////

HTofinoUnpacker::HTofinoUnpacker(Int_t id) {
  subEvtId=id;
  pRawCat=0;
  lookup=0;
}

Bool_t HTofinoUnpacker::init(void) {
  pRawCat=(HCategory*)gHades->getCurrentEvent()->getCategory(catTofinoRaw); 
  if (!pRawCat) {
    pRawCat=gHades->getSetup()->getDetector("Tofino")
            ->buildCategory(catTofinoRaw);
    if (!pRawCat) return kFALSE;
    gHades->getCurrentEvent()
          ->addCategory(catTofinoRaw,pRawCat,"Tofino");
  }
  loc.set(3,0,0,0);
  lookup=(HTofinoLookup*)(gHades->getRuntimeDb()
                                 ->getContainer("TofinoLookup"));
  return kTRUE;
}

Int_t HTofinoUnpacker::execute() {
  HTofinoRaw* pRaw=0;
  Int_t crate=-1, slot=-1, nChan=0, chan=-1, sec=-1, mod=-1, cell=-1;
  Char_t dcType='U';
  if(pSubEvt) {
    UInt_t* data=pSubEvt->getData();
    UInt_t* end=pSubEvt->getEnd();
    //cout<<"----------------  Tofino subevent  ----------------"<<endl;
    while (++data<end && *data!=0x0) {
      slot=(*data >> 27 & 0x1f);
      crate=(*data >> 16 & 0xff);
      nChan=(*data >> 8 & 0xff);
      //cout <<"crate,slot,nChan: "
      //     <<crate<<" "<<slot<<" "<<nChan<<endl;
      HTofinoLookupDConv* dc=lookup->getDConv(crate-1,slot-1);
      if (dc) {
        dcType=dc->getType();
        for(Int_t i=0;i<nChan;i++) {
          data++;
          chan = (*data & 0x001f0000) >> 16;
          HTofinoLookupChan* dcs=dc->getChannel(chan);
          if (dcs) {
            dcs->getAddress(sec,mod,cell);
            if (sec>=0) {
              loc[0]=sec;
              loc[1]=mod;
              loc[2]=cell;
              pRaw=(HTofinoRaw *)pRawCat->getObject(loc);
              if (!pRaw) {
                pRaw=(HTofinoRaw *)pRawCat->getSlot(loc);
                if (pRaw) {
                  pRaw=new (pRaw) HTofinoRaw;
                  pRaw->setAddress(sec,mod,cell);
                } else {
                  Error("HTofinoUnpacker::execute()", "can't get slot\n");
                  return -2;
                }
	      }
              if (dcType=='T') pRaw->setTime(((*data & 0xfff) % 0xfff));
              else  pRaw->setCharge(((*data & 0xfff) % 0xfff));
              //cout <<"filled: crate,slot,type,chan,sec,mod,cell,data = "
              //     <<crate<<" "<<slot<<" "<<dcType<<" "
              //     <<chan<<" "<<sec<<" "<<mod<<" "<<cell<<" "
              //     <<((*data & 0xfff) % 0xfff)<<endl;
            }
          }
        }
      } else  data+=nChan;
      data++;
    }
  }
  return 1;
}
