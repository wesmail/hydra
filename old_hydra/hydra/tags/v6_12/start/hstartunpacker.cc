//*-- Author : Walter Karig
//*-- Modified : 24/11/2002 by Dusan Zovinec
//*-- Modified : 18/12/2001 by Ilse Koenig
//*-- Modified : 29/11/2000 by Ilse Koenig 
//*-- Modified : 13/11/2000 by M. Golubeva
//*-- Modified : 05/06/98 by Manuel Sanchez
using namespace std;
#include "hstartunpacker.h"
#include "hstartdef.h"
#include "hcategory.h"
#include "hstartraw.h"
#include "hevent.h"
#include "hldsubevt.h"
#include "hades.h"
#include "htboxchan.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hruntimedb.h"
#include "hstartlookup.h"
#include "hdebug.h"
#include "heventheader.h"
#include "hiterator.h"
#include <iostream> 
#include <iomanip>

//////////////////////////////////////////////////////////////////
// HStartUnpacker
//
// This is the HUnpacker used to read Start data from .hld files
//
//////////////////////////////////////////////////////////////////

HStartUnpacker::HStartUnpacker(Int_t id) {
  subEvtId=id;
  pRawCat=0;
  tboxCat=0;
  lookup=0;
}

Bool_t HStartUnpacker::init(void) {

  pRawCat=(HCategory*)gHades->getCurrentEvent()->getCategory(catStartRaw); 
  if (!pRawCat) {
    pRawCat=gHades->getSetup()->getDetector("Start")->buildCategory(catStartRaw);
    if (!pRawCat) return kFALSE;
    gHades->getCurrentEvent()
          ->addCategory(catStartRaw,pRawCat,"Start");
  }
  loc.set(2,0,0);
  lookup=(HStartLookup*)(gHades->getRuntimeDb()
                         ->getContainer("StartLookup"));
  if(!lookup) return kFALSE;

  tboxCat=(HCategory*)gHades->getCurrentEvent()->getCategory(catTBoxChan);
  if (!tboxCat) {
    tboxCat=gHades->getSetup()->getDetector("TBox")->buildCategory(catTBoxChan);
    if (tboxCat) gHades->getCurrentEvent()->addCategory(catTBoxChan,tboxCat,"TBox");
    else return kFALSE;
  }

  return kTRUE;
}

Int_t HStartUnpacker::execute() {
  HStartRaw* pRaw=0;
  HTBoxChan* ptboxchan=0;
  Int_t crate=-1, slot=-1, nChan=0, chan=-1, schan=-1, sval=-1, mod=-1, strip=-1;
  Int_t i1,i2,i3,i4;
  Int_t count_tbox;

  Int_t trigbits=0;
  // trigbits = (gHades->getCurrentEvent()->getHeader()->getTBit() & 0xfffffe00);

  if(pSubEvt) {
    UInt_t* data=pSubEvt->getData();
    UInt_t* end=pSubEvt->getEnd();

    if((*data & 0xf00)==0x900){   // calibration event

      // set 9th bit to 1 when calibration event
      trigbits = trigbits | 0x100;
//      UInt_t *mydata= (UInt_t*) data;
//      printf("new event\n");
//      for(Int_t i=0; i<35; i++){
//        printf(" %x \n", *mydata);
//        mydata++;
//      }
      clearScalData();
      data++;
      //check if header
      if(*data == 0x8000000){
        data++;    // header found.
      } else {
//        printf("%d: scaler subevt -> no header\n",gHades->getCurrentEvent()->getHeader()->getEventSeqNumber());
      }
      count_tbox=0;
      while((*data >> 8) != 0x80000){    // read scalers data
        if(count_tbox < 32){
          if((*data & 0xff000000) != 0x0){  // correction for TDC data words in scaler
            scalData[count_tbox] = 0x0;
          } else {
            scalData[count_tbox] = (*data & 0xffffff);
          }
          data++;
          count_tbox++;
        } else {
//          printf("%d: scaler subevt -> 32 data words passed and no trailer follows\n",gHades->getCurrentEvent()->getHeader()->getEventSeqNumber());
          clearScalData();
          break;
        }
      }
      if(count_tbox != 32){
//        printf("\n%d: scaler subevt -> number of data words: %d %d\n",
//          gHades->getCurrentEvent()->getHeader()->getEventSeqNumber(),
//          count_tbox,((*data & 0xff)/4));
      }

      if(tboxCat) {    // fill scalers data
        for (Int_t i=0; i<8; i++){
          i1=scalData[i];
          i2=scalData[i+8];
          i3=scalData[i+16];
          i4=scalData[i+24];
          ptboxchan=(HTBoxChan*)tboxCat->getNewSlot(loc);
          if(!ptboxchan) continue;
          ptboxchan = new(ptboxchan) HTBoxChan;
          ptboxchan->set(i,i1,i2,i3,i4);
        }
      }

    } else {
      while (++data<end && *data!=0x0) {
        slot=(*data >> 27 & 0x1f);
        crate=(*data >> 16 & 0xff);
        nChan=(*data >> 8 & 0xff);

        UInt_t *sdata= (UInt_t*) data;  // trigger particular stuff
        for(Int_t i=0;i<nChan;i++) {
          sdata++;
          schan = (*sdata & 0x001f0000) >> 16;
          sval = ((*sdata & 0xfff) % 0xfff);
          if((slot==11) && (crate==2) && (schan>=24) && (schan<32)){
            // read only data from last 8 channels of crate 2 slot 11 (trigger info)
            if(schan==24) trigbits = trigbits | 0x1;
            if(schan==25) trigbits = trigbits | 0x2;
            if(schan==26) trigbits = trigbits | 0x4;
            if(schan==27) trigbits = trigbits | 0x8;
            if(schan==28) trigbits = trigbits | 0x10;
            if(schan==29) trigbits = trigbits | 0x20;
            if(schan==30) trigbits = trigbits | 0x40;
            if(schan==31) trigbits = trigbits | 0x80;
          }
        }
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
  }
  gHades->getCurrentEvent()->getHeader()->setTBit(trigbits);  // store trigger info into the EventHeader::fTBit

  return 1;
}

ClassImp(HStartUnpacker) 










