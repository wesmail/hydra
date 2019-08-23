//*-- Author : D. Bertini
//*-- Modified: 18/12/2001 by Ilse Koenig 
//*-- Modified: 28/05/2000 by R. Holzmann
//*-- Modified: 24/01/2000 by R. Schicker
//*-- Modified: 18/06/99 by Ilse Koenig
//
// File: hstartunpacker99.cc
//
#include "hstartunpacker99.h"
#include "hstartlookup.h"
#include "hstartraw.h"
#include "htboxchan.h"
#include "htofinoraw.h"
#include "hldsubevt.h"
#include "hades.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hruntimedb.h"
#include "hrecevent.h"
#include "hpartialevent.h"
#include "heventheader.h"
#include "hdebug.h"
#include "hlinearcategory.h"
#include "hiterator.h"
#include "hcategory.h"
#include <iostream.h>

HStartUnpacker99::HStartUnpacker99(Int_t id) {
  subEvtId = id;
  pRawCat=0;
  lookup=0;
  tboxCat=0;
  tofinoRawCat=0;
}

HStartUnpacker99::HStartUnpacker99(const HStartUnpacker99& start) {
  subEvtId = start.subEvtId;
  pRawCat=start.pRawCat;
  lookup=start.lookup;
  tboxCat=start.tboxCat;
  tofinoRawCat=start.tofinoRawCat;
}

Bool_t HStartUnpacker99::init(void) {
  HRecEvent *event=0;
  HPartialEvent *par=0;
  loc.set(0);
  event=(HRecEvent *)gHades->getCurrentEvent();
  if (!event) return kFALSE;
  HDetector* det=gHades->getSetup()->getDetector("Start");
  if (det) {
    par=event->getPartialEvent(catStart);
    if (!par) { 
      event->addPartialEvent(catStart,"Start","The Start subevent");
      par=event->getPartialEvent(catStart);
      if (!par) return kFALSE;
    }
    pRawCat=(HCategory*)par->getCategory(catStartRaw); 
    if (!pRawCat) {
      //cout << "Creating a category \n";
      pRawCat=det->buildCategory(catStartRaw);
      if (pRawCat) par->addCategory(catStartRaw,pRawCat);
      else return kFALSE;
    } 
    lookup=(HStartLookup*)(gHades->getRuntimeDb()
                                 ->getContainer("StartLookup"));
    if(!lookup) return kFALSE;
  }

  det=gHades->getSetup()->getDetector("TBox");
  if(det) {
    par=event->getPartialEvent(catStart);
    if (!par) { 
      event->addPartialEvent(catStart,"Start","The Start subevent");
      par=event->getPartialEvent(catStart);
      if (!par) return kFALSE;
    }
    tboxCat=(HCategory*)par->getCategory(catTBoxChan); 
    if (!tboxCat) {
      tboxCat=det->buildCategory(catTBoxChan);
      if (tboxCat) par->addCategory(catTBoxChan,tboxCat);
      else return kFALSE;
    }
  }

  det=gHades->getSetup()->getDetector("Tofino"); 
  if (det) {
    par=event->getPartialEvent(catTofino);
    if (!par) { 
      event->addPartialEvent(catTofino,"Tofino","The Tofino subevent");
      par=event->getPartialEvent(catTofino);
      if (!par) return kFALSE;
    }
    tofinoRawCat=(HCategory*)par->getCategory(catTofinoRaw); 
    if (!tofinoRawCat) {
      //cout << "Creating a category \n";
      tofinoRawCat=det->buildCategory(catTofinoRaw);
      if (tofinoRawCat) par->addCategory(catTofinoRaw,tofinoRawCat);
      else return kFALSE;
    }  
  }
  return kTRUE;
}

int HStartUnpacker99::execute(void) {
  HRecEvent *event=0;
  HEventHeader *eventh=0;
  HStartRaw* startRaw=0;
  HTofinoRaw* tofinoRaw=0;
  HTBoxChan* ptboxchan=0;
  Int_t i1,i2,i3,tb0,tb1,tb2,tb3,adc;

  if (pSubEvt) {
    Int_t startCh=0, tdc=0, chan=0, mod=0, strip=0;
    Short_t* data =  (Short_t*) pSubEvt->getData();
    Short_t* end  =  (Short_t*) pSubEvt->getEnd(); 
   
    clearAdcData();
    clearTdcData();      // needed for Tofino
    clearScalData();
    while (data < end && *data != 0x0) {

      // unpack the ADC's part:  Tofino + Triggerbox ADC
      if((*data++ == 0x1007) && ((*data >> 15) & 0x1 == 1) ) {
        startCh = (*data >> 12 & 0x7) + 1;
        data++;   
        for(Int_t i=0;i<startCh;i++) {
          adcData[(*data >> 12) & 0x7] = (*data & 0xfff) % 0xfff;
          data++;   
        } 
      }

      // unpack the TDC  part of Start/Veto
      if((*data++ =0x2005) && ((*data >> 15) & 0x1 == 1) ) {
        tdc=0;    // only one TDC in Nov99 for Start/Veto
        startCh = (*data >> 12 & 0x7) + 1;
        data++;
        for(Int_t i=0;i<startCh;i++){
          if (pRawCat) {
            chan=(*data >> 12) & 0x7;
            (*lookup)[tdc][chan].getAddress(mod,strip);
            if (mod>=0) {
              startRaw=(HStartRaw *)pRawCat->getNewSlot(loc);
              if (startRaw) startRaw=new (startRaw) HStartRaw;
              if (!startRaw) return -1;
              startRaw->fill(((*data & 0xfff) % 0xfff),mod,strip);
            }
            data++;
          }
        }
      }

      // unpack the TDC  part of Tofino
      if((*data++ =0x2006) && ((*data >> 15) & 0x1 == 1) ) {
        startCh = (*data >> 12 & 0x7) + 1;
        data++;
        for(Int_t i=0;i<startCh;i++){
          tdcData[((*data >> 12) & 0x7)] = (*data & 0xfff) % 0xfff;
          data++;
        }
      }

      // unpack the scaler part
      UInt_t *sdata= (UInt_t*) data;   
      if (*sdata++ == 0x3801) {
        for(UInt_t i=0;i<32;i++) {
 	  scalData[i] = *sdata;
          sdata++;
        }
      } 
    } //while (data<end)

    if (tofinoRawCat) {
      for (Int_t i=0;i<3;i++) {
        if (adcData[i]>=0 || tdcData[i]>=0) {
          tofinoRaw=(HTofinoRaw *)tofinoRawCat->getNewSlot(loc);
          if (tofinoRaw) tofinoRaw=new (tofinoRaw) HTofinoRaw;
          if (!tofinoRaw) return -1;
          // The addresses are here hardcoded but should be provided in the
          // future via a lookup table
          tofinoRaw->fill(tdcData[i],adcData[i],2,0,i);
        }
      }
      if (tdcData[3]>=0) {
        tofinoRaw=(HTofinoRaw *)tofinoRawCat->getNewSlot(loc);
        if (tofinoRaw) tofinoRaw=new (tofinoRaw) HTofinoRaw;
        if (!tofinoRaw) return -1;
        tofinoRaw->fill(tdcData[3],-1,2,0,3);
      }
    }

    if(tboxCat) {    // scaler data
      for (Int_t i=0; i<8; i++){
        i1=scalData[i];
        i2=scalData[i+8];
        i3=scalData[i+16];
        ptboxchan=(HTBoxChan*)tboxCat->getNewSlot(loc);
        ptboxchan->set(i,i1,i2,i3);
      }

/*
      HIterator *iter= (HIterator*) tboxCat->MakeIterator();
      while ( (ptboxchan =(HTBoxChan*) iter->Next() ) != 0 ) {
        ptboxchan->print();
      }

      delete iter;
*/
    }

    adc=adcData[3];

    tb1=0; tb2=0; tb3=0; 

    if(adc > 300 && adc < 330){
      tb1=0; tb2=0; tb3=1;
    }  
    else {if(adc > 490 && adc < 550) {
      tb1=0; tb2=1; tb3=0;
    }
    else {if(adc > 700 && adc < 750) {
      tb1=0; tb2=1; tb3=1;
    }
    else {if(adc > 920 && adc < 970) {
      tb1=1; tb2=0; tb3=0;
    }
    else {if(adc > 1120 && adc < 1180) {
      tb1=1; tb2=0; tb3=1;
    }
    else {if(adc > 1320 && adc < 1390) {
      tb1=1; tb2=1; tb3=0;
    }}}}}};

    tb0=tb1+4*tb2+16*tb3;

  event=(HRecEvent *)gHades->getCurrentEvent();
  eventh=(HEventHeader*)event->getHeader();
  eventh->setTBit(tb0);

  }  // start subevent
  return 1;
}  

ClassImp(HStartUnpacker99)
















