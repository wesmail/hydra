//*-- Author : Walter Karig
//*-- Modified : 03/05/2005 by Ilse Koenig
//*-- Modified : 16/09/2003 by T.Wojcik
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
#include "hspectrometer.h"
#include "hdetector.h"
#include "hruntimedb.h"
#include "hstartlookup.h"
#include "hdebug.h"
#include "heventheader.h"
#include "hdatasource.h"
#include <iostream>
#include <iomanip>
#include <stdlib.h>

ClassImp(HStartUnpacker)

//_HADES_CLASS_DESCRIPTION 
//////////////////////////////////////////////////////////////////////
//
// HStartUnpacker
//
// Unpacker used to read Start data from .hld files
//
// The scaler data and latch data are unpacked by separate unpackers
// HTBoxUnpacker and HLatchUnpacker. 
//
/////////////////////////////////////////////////////////////////////

HStartUnpacker::HStartUnpacker(Int_t id) {
  // Constructor
  // Input: subevent id
  subEvtId=id;
  pRawCat=0;
  lookup=0;
}

Bool_t HStartUnpacker::init(void) {
  // Creates the raw category for the START, if not yet existing
  // Gets the lookup table from the runtime database
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
  return kTRUE;
}

Int_t HStartUnpacker::execute() {
  // Unpacks the data of the start detector and fills the raw data category
#if DEBUG_LEVEL>4
  gDebuger->enterFunc("HStartUnpacker::execute\n");
#endif
  if (gHades->isCalibration()) {  // calibration event, nothing to be done for START
    return 1;
  }
  if (pSubEvt) {
    UInt_t* data=pSubEvt->getData();
    UInt_t* end=pSubEvt->getEnd();
    UInt_t uBlockSize = 0;    //Number of data words in one block  
    Int_t nEvt=gHades->getCurrentEvent()->getHeader()->getEventSeqNumber();
    #if DEBUG_LEVEL>4
       printf("Next SubEvt. data between: %p and %p\n",data,end);
       pSubEvt->dumpIt();
    #endif
    // Loop over all data in this subevt
    while (++data<end && *data!=0x0) {
      #if DEBUG_LEVEL>2
         printf("Data word: %p\n",*data);
      #endif
      // Scan for TIP-block header ( TYPE 0 )
      // TIP-block appears only in the new data structure
      if (((*data >>24) & 0x6)==0) {
        uBlockSize = (*data) & 0x0000003ff;
        UInt_t uBoardType = (*data >> 12 ) & 0x1;
        if(uBoardType==1) {  // latch or scaler
          data+=uBlockSize;
          continue;
        } else if (uBoardType==0) {  // tdc header
          //Do a normal TDC scanning
          #if DEBUG_LEVEL>2
             printf("TIP-block 0x%08x Size %i Board  %i\n",
                    *data,uBlockSize,uBoardType);
          #endif
          continue;
        } else {
          Error("execute","Unknown BoardType=%i",uBoardType);
        }
      } else if ( ((*data>>24) & 0x6)==2 ) {
        uBlockSize=((*data) >>  8) & 0xff;
        Int_t nCrate    =((*data) >> 16) & 0xff;
        Int_t nSlot     =((*data) >> 27) & 0x1f;
        // Check if also trailer exists
        if ((*(data + uBlockSize+1) >>24 & 0x6)!=4) {
          Error("execute","Header found but no trailer!!");
          continue;
        }
        // Check trigger tag in the trailer
        if ((*(data+uBlockSize+1)&0xff)!=((UInt_t)pSubEvt->getTrigNr()&0xFF)) {
	    Error("execute","Trigger tag mismatch: EventSeqNumber= %i! Skip event...",nEvt);
            if(!gHades->getForceNoSkip()) return kDsSkip;
	}
        // Scan all data words
        #if DEBUG_LEVEL>4
           printf("TDC header 0x%08x.Size: %i.Crate: %i.Slot: %i.\n",
                  *data,uBlockSize,nCrate,nSlot);
        #endif
        UInt_t* uTmp=data;
        HStartLookupTdc* pTdc=lookup->getTdc(nCrate-1,nSlot-1);
        if (pTdc) {
	  UInt_t uWordCount=0;
          while (++data && data<(uTmp + uBlockSize+1) && *data!=0x0) {
            uWordCount++;
            Int_t nChannel=(*(data) >> 16) & 0x003f;
            Int_t nData   =*data  & 0xfff;
            HStartLookupChan* tc=pTdc->getChannel(nChannel);
            if (tc) {
              Int_t nMod=-1;
              Int_t nStrip=-1;
              tc->getAddress(nMod,nStrip);
              if (nMod>=0 && nStrip>=0) {
                loc[0]=nMod;
                loc[1]=nStrip;
                HStartRaw* pRaw=(HStartRaw*)pRawCat->getObject(loc);
                if (!pRaw) {
                  pRaw=(HStartRaw*)pRawCat->getSlot(loc);
                  if (pRaw) {
                    pRaw=new (pRaw) HStartRaw;
                  } else {
                    Error("execute()","Can't get slot\n");
                    return EXIT_FAILURE;
                  }
                } else {
                  #if DEBUG_LEVEL>2
                     Info("execute()","Slot already exists!\n");
                  #endif
                }
                char cType='\0';
                Int_t nType=-1;
                cType = tc->getType();
                if (cType !='\0') {
                  switch(cType) {
                    case 'R':
                      nType = TYPE_RIGHT;
                      break;
                    case 'L':
                      nType = TYPE_LEFT;
                      break;
                    case 'M':
                      nType = TYPE_MEANTIMER;
                      break;
                    default:
                      break;
                  }
                  pRaw->fill(nData,nMod,nStrip,nType);
                } else {
                  pRaw->fill(nData,nMod,nStrip);
                }
                #if DEBUG_LEVEL>2
                   printf("Evt:%d Mod:%d Str:%d Typ:%d Data:%d\n",
                          nEvt,nMod,nStrip,nType,nData);
                #endif
              } else {
                #if DEBUG_LEVEL>4
                   Error("execute()","Wrong address! Mod: %i Strip; %i",
                         nMod,nStrip);
                #endif
              }
            } else {
              #if DEBUG_LEVEL>2
                 Info("execute()","TDC channel: %i not found",nChannel);
              #endif
            }
          }
          if (uWordCount!=uBlockSize) {
            Error("execute()","Found %i but expecting % words!!!",
                  uWordCount,uBlockSize);
          }
        } else {
          #if DEBUG_LEVEL>4
             Error("execute()","Can not get TDC for create %i and slot %i",
                   nCrate-1,nSlot-1);
          #endif
          data+=uBlockSize+1; // Skip this block
        }
      } else if (((*data>>24) & 0x6)==6) {
        #if DEBUG_LEVEL>2
           Error("execute()","Evt: %i, Invalid Data Word! Type: %d,Skipping 0x%08x",
                 nEvt,((*data>>24) & 0x6),*data);
           pSubEvt->dumpIt();
        #endif
      } else if (((*data>>24) & 0x6)==0){
          Error("execute()","Evt: %i, Data follow, but no header",nEvt);
      } else {
        Error("execute()","Evt: %i, Unknow raw data type %i",
              nEvt,((*data>>24) & 0x6));
      }
    } // end data
  } 
  #if DEBUG_LEVEL>4
     gDebuger->leaveFunc("HStartUnpacker::execute\n");
  #endif
  return 1;
}
