//*-- Author : D. Bertini
//*-- Modified: 02/06/2000 by R. Holzmann
//*-- Modified: 24/01/2000 by R. Schicker
//*-- Modified: 18/06/99 by Ilse Koenig
//
// File: hstartunpacker.cc
//
#include "hstartunpacker.h"
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

HStartUnpacker::HStartUnpacker(Int_t id) {
  subEvtId = id;
  pRawCat=0;
  lookup=0;
  tboxCat=0;
  tofinoRawCat=0;
}
HStartUnpacker::HStartUnpacker(const HStartUnpacker& start) {
  subEvtId = start.subEvtId;
  pRawCat=start.pRawCat;
  lookup=start.lookup;
  tboxCat=start.tboxCat;
  tofinoRawCat=start.tofinoRawCat;
}

Bool_t HStartUnpacker::init(void) {
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
      pRawCat=det->buildCategory(catStartRaw);
      if (pRawCat) par->addCategory(catStartRaw,pRawCat);
      else return kFALSE;
    } 
    lookup=(HStartLookup*)(gHades->getRuntimeDb()
                                 ->getContainer("StartLookup"));
    if(!lookup) {
      lookup=new HStartLookup(1,16);  // must be changed when 2 TDCs are used
      gHades->getRuntimeDb()->addContainer(lookup); 
    }
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
      tofinoRawCat=det->buildCategory(catTofinoRaw);
      if (tofinoRawCat) par->addCategory(catTofinoRaw,tofinoRawCat);
      else return kFALSE;
    }  
  }
  return kTRUE;
}

int HStartUnpacker::execute(void) {
  HRecEvent *event=0;
  HEventHeader *eventh=0;
  HStartRaw* startRaw=0;
  HTofinoRaw* tofinoRaw=0;
  HTBoxChan* ptboxchan=0;
  Int_t tofinoSec[16]={1,1,1,1,2,2,2,2,5,5,5,5,0,0,0,0};
  Int_t tofinoCell[16]={0,1,2,3,0,1,2,3,0,1,2,3,0,1,2,3};
  Int_t i1, i2, i3, i4, tb0, tb1, tb2, tb3, adc1, adc2;

  if (pSubEvt) {
    Int_t tdc=0, mod=0, strip=0, slot=-1, crate=-1, count=0;
    UInt_t* data =  (UInt_t*) pSubEvt->getData();
    UInt_t* end  =  (UInt_t*) pSubEvt->getEnd(); 
   
    clearAdcData();  // clear data arrays
    clearTdcData();
    clearScalData();

    UInt_t stHead = *data++;
    if ((stHead & 0xff000000) != 0xc8000000) return 0; // not a START subevent
    if (stHead & 0x00010000) {
      cout << "Trigger tag mismatch!" << endl;
      return 0;
    }

    while (data < end && *data != 0x0) {
      if (*data == 0xc8fffff0) break;   // START subevent trailer

      if ((*data & 0xff000000) == 0xd1000000) {  // this is a TDC module 
        data++;
        while (*data != 0xd1fffff0) {
          if ((*data & 0x07000000) != 0x02000000) printf("No TDC header\n");
          crate = (*data & 0x00ff0000) >> 16;
          slot = (*data & 0xf8000000) >> 27;
          count = (*data & 0x00003f00) >> 8;
          printf("crate=%d  TDC slot=%d  count=%d\n",crate,slot,count);
          data++;
          for (Int_t i=0;i<count;i++) {
            if ((*data & 0x07000000) != 0) printf("No TDC data\n");
            if(slot==9) tdcData[(*data&0x003f0000) >> 16] = (*data & 0xfff);
            if(slot==8) adcData[(*data&0x003f0000) >> 16] = (*data & 0xfff);
            data++;
          }
          if ((*data & 0x07000000) != 0x04000000) printf("No TDC trailer\n");
          data++;  // skip data trailer
        } 
      }

      if ((*data & 0xff000000) == 0xd2000000) {  // this is a ADC module 
        data++;
        while (*data != 0xd1fffff0) {
          if ((*data & 0x07000000) != 0x02000000) printf("No ADC header\n");
          crate = (*data & 0x00ff0000) >> 16;
          slot = (*data & 0xf8000000) >> 27;
          count = (*data & 0x00003f00) >> 8;
          printf("crate=%d  ADC slot=%d  count=%d\n",crate,slot,count);
          data++;
          for (Int_t i=0;i<count;i++) {
            if ((*data & 0x07000000) != 0) printf("No ADC data\n");
            adcData[(*data&0x003f0000) >> 16] = (*data & 0xfff);
            data++;
          }
          if ((*data & 0x07000000) != 0x04000000) printf("No ADC trailer\n");
          data++;  // skip data trailer
        } 
      }

      if ((*data & 0xff000000) == 0xd4000000) {  // this is a scaler module
        data++;
        UInt_t modId = *data++;  // module ID
        for(Int_t i=0;i<32;i++) {
 	  scalData[i] = *data++;
        }
        data++; // skip data trailer
      }

      data++;
    } // while (data<end)


    if (tofinoRawCat) {  // fill now TOFINO raw category
      for (Int_t i=0;i<16;i++) {
        if (adcData[i]>=0 || tdcData[i]>=0) {
          tofinoRaw=(HTofinoRaw *)tofinoRawCat->getNewSlot(loc);
          if (tofinoRaw) tofinoRaw=new(tofinoRaw) HTofinoRaw;
          else return -1;
          // The addresses are here hardcoded but should be provided in the
          // future via a lookup table
          tofinoRaw->fill(tdcData[i],adcData[i],tofinoSec[i],0,tofinoCell[i]);
        }
      }
    }

    tdc = 0;
    if (pRawCat) { // fill now START raw category
      for (Int_t i=16;i<32;i++) {
        if (tdcData[i]>=0) {
          startRaw=(HStartRaw *)pRawCat->getNewSlot(loc);
          if (startRaw) startRaw=new(startRaw) HStartRaw;
          else return -1;
//          (*lookup)[tdc][i-16].getAddress(mod,strip); 
          mod = (i-16)/8;
          strip = (i-16)%8;
          startRaw->fill(tdcData[i],mod,strip);
        }
      }
    }

    if(tboxCat) { // fill scaler data
      for (Int_t i=0; i<8; i++){
        i1=scalData[i];
        i2=scalData[i+8];
        i3=scalData[i+16];
        i4=scalData[i+24];
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

    adc1=adcData[16];
    adc2=adcData[17];

    tb1=0; tb2=0; tb3=0; 

    if (adc1 > 300 && adc1 < 330){
      tb1=0; tb2=0; tb3=1;
    }  
    else if (adc1 > 490 && adc1 < 550) {
      tb1=0; tb2=1; tb3=0;
    }
    else if (adc1 > 700 && adc1 < 750) {
      tb1=0; tb2=1; tb3=1;
    }
    else if (adc1 > 920 && adc1 < 970) {
      tb1=1; tb2=0; tb3=0;
    }
    else if (adc1 > 1120 && adc1 < 1180) {
      tb1=1; tb2=0; tb3=1;
    }
    else if (adc1 > 1320 && adc1 < 1390) {
      tb1=1; tb2=1; tb3=0;
    }

    tb0=tb1+4*tb2+16*tb3;

  event=(HRecEvent *)gHades->getCurrentEvent();
  eventh=(HEventHeader*)event->getHeader();
  eventh->setTBit(tb0);

  }  // start subevent
  return 1;
}  

ClassImp(HStartUnpacker)
















