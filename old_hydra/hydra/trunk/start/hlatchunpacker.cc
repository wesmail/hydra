//*-- Author : Walter Karig
//*-- Modified : 03/05/2005 by Ilse Koenig
//*-- Modified : 16/09/2003 by T.Wojcik
//*-- Modified : 24/11/2002 by Dusan Zovinec
//*-- Modified : 18/12/2001 by Ilse Koenig
//*-- Modified : 29/11/2000 by Ilse Koenig
//*-- Modified : 13/11/2000 by M. Golubeva
//*-- Modified : 05/06/98 by Manuel Sanchez
using namespace std;
#include "hlatchunpacker.h"
#include "hevent.h"
#include "hldsubevt.h"
#include "heventheader.h"
#include "hades.h"
#include "hdebug.h"
#include "hdatasource.h"
#include <iostream>
#include <iomanip>
#include <stdlib.h>

ClassImp(HLatchUnpacker)

//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////
// 
// HLatchUnpacker
//
// Unpacker used to read the LATCH data from .hld files
// Sets EventHeader::fTBit.
//
////////////////////////////////////////////////////////////////////

HLatchUnpacker::HLatchUnpacker(Int_t id) {
  // Constructor
  // Input: subevent id
  subEvtId=id;
}

Int_t HLatchUnpacker::execute() {
  // Unpacks the latch data and sets EventHeader::fTBit
#if DEBUG_LEVEL>4
  gDebuger->enterFunc("HLatchUnpacker::execute\n");
#endif
  Bool_t bIsOld      = kFALSE;
  Int_t nCrate       = 0;
  Int_t nSlot        = 0;
  Int_t nChannel     = 0;
  Int_t nEvtId       = 0; //Evt Id
  Int_t nEvt         = 0; //Evt number
  UInt_t uOldCalType = 0;
  UInt_t uWordCount  = 0; //
  UInt_t uTrigbits   = 0;
  UInt_t uBlockSize  = 0; //Number of data words in one block
  UInt_t uBoardType  = 0; // 1 - Struck, 0 - TDC
  UInt_t uStruck     = 0; // scaler or latch
  UInt_t* uTmp       = NULL;
  if (subEvtId==412) {
    if (pSubEvt) {
      UInt_t* data = pSubEvt->getData();
      UInt_t* end  = pSubEvt->getEnd();
      #if DEBUG_LEVEL>4
         printf("Next SubEvt. data between: %p and %p\n",data,end);
         pSubEvt->dumpIt();
      #endif
      nEvtId=gHades->getCurrentEvent()->getHeader()->getId();
      nEvt=gHades->getCurrentEvent()->getHeader()->getEventSeqNumber();
      // only in old data
      uOldCalType=(*data & 0xf00);
      uTmp=data;
      bIsOld=gHades->getCurrentEvent()->getHeader()->isOldDAQ();
      // Loop over all data words
      while (++data<end && *data!=0x0) {
        #if DEBUG_LEVEL>2
           printf("Data word: %p\n",*data);
        #endif
        // nEvtId==5 (calibration event) and no TIP-block header indicate
        // old data scheme
        if (nEvtId==5 && (*(data) == 0x8000000) && bIsOld && uOldCalType==0x900) {
          // There is no TIP header - go back one word
          data--;
          data = skipScaler(data,kTRUE);
        } else {
          // Scan for TIP-block header ( TYPE 0 )
          if (((*data >>24) & 0x6)==0) {
            uBlockSize=(*data) & 0x0000003ff;
            uBoardType=(*data >> 12 ) & 0x1;
            uStruck   =(*data >> 10 ) & 0x3f;
            #if DEBUG_LEVEL>4
              cout<<"Struck "<<uStruck <<" BoardType:"<< uBoardType<<endl;
            #endif
            // latch or scaler
            if (uBoardType==1) {
              if (uStruck == SCALER){
                uTrigbits=uTrigbits | 0x100;
                data=skipScaler(data,kFALSE,SCALER);
              } else if (uStruck == SCALER_JAN04){
                data=skipScaler(data,kFALSE,SCALER_JAN04);
              } else if ( uStruck == LATCH ){
                data=fillLatch(data);
              } else if (uStruck == DEBUG){
                Info("execute","DEBUG block, size: %i",uBlockSize);
                data+=uBlockSize;
                continue;
              } else{
                Error("execute","Uknown type=%i",uStruck);
              }
            } else if (uBoardType==0) {  
              // next should be tdc header
              #if DEBUG_LEVEL>2
                 printf("TIP-block 0x%08x Size %i Board  %i\n",
                        *data,uBlockSize,uBoardType);
              #endif
            } else{
              Error("execute","Uknown BoardType=%i",uBoardType);
            }
          } else if (((*data>>24) & 0x6)==2) {  // Scan for TDC header
            // Found valid TDC header
            uBlockSize=((*data) >>  8) & 0xff;
            nCrate    =((*data) >> 16) & 0xff;
            nSlot     =((*data) >> 27) & 0x1f;
            // Check if also trailer exists
            if ((*(data + uBlockSize+1) >>24 & 0x6)!=4) {
              Error("execute","Header found but no trailer!!");
              continue;
            }
            // Check trigger tag in the trailer
	    if ((*(data+uBlockSize+1)&0xff)!=((UInt_t)pSubEvt->getTrigNr()&0xFF)) {
		Error("execute","Trigger tag mismatch: EventSeqNumber= %i! Skip event...",nEvt);
            }
            // Scan all data words
            #if DEBUG_LEVEL>4
               printf("TDC header 0x%08x.Size: %i.Crate: %i.Slot: %i.\n",
                      *data,uBlockSize,nCrate,nSlot);
            #endif
            uTmp=data;
            uWordCount=0;
            while (++data && data<(uTmp + uBlockSize+1) && *data!=0x0) {
              uWordCount++;
              nChannel=(*(data) >> 16) & 0x003f;
              // trigger particular stuff in old data
              // read only data from last 8 channels of crate 2
              // slot 11 (trigger info)
              if (bIsOld &&  (nSlot==11) && (nCrate==2)) {
                if (nChannel==24) uTrigbits = uTrigbits | 0x1;
                if (nChannel==25) uTrigbits = uTrigbits | 0x2;
                if (nChannel==26) uTrigbits = uTrigbits | 0x4;
                if (nChannel==27) uTrigbits = uTrigbits | 0x8;
                if (nChannel==28) uTrigbits = uTrigbits | 0x10;
                if (nChannel==29) uTrigbits = uTrigbits | 0x20;
                if (nChannel==30) uTrigbits = uTrigbits | 0x40;
                if (nChannel==31) uTrigbits = uTrigbits | 0x80;
	      }
            }
            if (uWordCount!=uBlockSize) {
              Error("execute()","Found %i but expecting %i words!!!",
                    uWordCount,uBlockSize);
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
        }
      } // end data
    } // pSubEvt
    // store trigger info into the EventHeader::fTBit
    if (bIsOld) gHades->getCurrentEvent()->getHeader()->setTBit(uTrigbits);
  } else if(subEvtId==416) {   // New data structures fince aug04
    if (pSubEvt) {
      UInt_t* data = pSubEvt->getData();
      UInt_t* end  = pSubEvt->getEnd();
      #if DEBUG_LEVEL>4
         printf("Next SubEvt. data between: %p and %p\n",data,end);
         pSubEvt->dumpIt();
      #endif
      nEvtId=gHades->getCurrentEvent()->getHeader()->getId();
      nEvt=gHades->getCurrentEvent()->getHeader()->getEventSeqNumber();
      //Loop over all data words
      while (++data<end && *data!=0x0) {
        #if DEBUG_LEVEL>2
           printf("Data word: %p\n",*data);
        #endif
        //Scan for TIP-block header Scalers have always TIP-Block
        if (((*data >>24) & 0x6)==0 && (((*data) &  0x3fffff )!=0)) {
          uBlockSize = (*data) & 0x0000003ff;
          uBoardType = (*data >> 10 ) & 0x3f;
          #if DEBUG_LEVEL>4
             printf("BoardType: %d\n ",uBoardType);
          #endif
          //new scaler aug04
          if (uBoardType==SCALER_AUG04) {
            #if DEBUG_LEVEL>4
               printf("Total scalers block size is :%d \n",uBlockSize);
            #endif
            UInt_t* pScalerTrailer=0;
            UInt_t uScalerSize=0;
            UInt_t uScalerTotalSize=0;
            UInt_t* pScalerHeader = 0;
            while( uScalerTotalSize < uBlockSize ) {
              pScalerTrailer = data+uBlockSize-uScalerTotalSize;
              uScalerSize=((*pScalerTrailer) & 0x7fffff)/4;
              pScalerHeader =  pScalerTrailer-uScalerSize+1;
              uScalerTotalSize = uScalerTotalSize+uScalerSize;
              if ((*pScalerHeader &  0x3fffff )!=0) {
                data+=uBlockSize;
                break; //return to main loop
              }
            }
            data+=uBlockSize;
          } else if (uBoardType==0) {
            //next should be tdc header
            //Do a normal TDC/latch scanning
            #if DEBUG_LEVEL>2
               printf("TIP-block 0x%08x Size %i Board  %i Next is tdc header\n",
                      *data,uBlockSize,uBoardType);
            #endif
          } else {
            Error("execute","Uknown BoardType=%i",uBoardType);
          }
        } else if (((*data>>24) & 0x6)==2) { // TDC
          uBlockSize = ( (*data) >> 8   ) & 0xff;
          //Check if also trailer exists
          if ((*(data + uBlockSize+1) >>24 & 0x6)!=4) {
            Error("execute","Header found but no trailer!!");
            continue;
          }
          //Check trigger tag in the trailer
          if ((*(data+uBlockSize+1)&0xff)!=((UInt_t)pSubEvt->getTrigNr()&0xFF)) {
	    Error("execute","Trigger tag mismatch: EventSeqNumber= %i! Skip event...",nEvt);
	    if(!gHades->getForceNoSkip()) return kDsSkip;
          }
          uTmp=data;
          uWordCount=0;
          while( ++data && data<(uTmp + uBlockSize+1) && *data!=0x0 ){
            uWordCount++;
          }
          if (uWordCount!=uBlockSize) {
            Error("execute()","Found %i but expecting %i words!!!",
                  uWordCount,uBlockSize);
          }
        } else if (((*data) &  0x3fffff )==0) {
          //Check for SIS3829 header (defined by SIS) (latch only)
          UInt_t uGeo = (*data)>>27 &0x1F;
          //Latch should have one data word, zero is also possible
          #if DEBUG_LEVEL>4
             printf("Trying to find the size \n");
          #endif
          UInt_t* pLatchTmp=data;
          if (++pLatchTmp && ((*pLatchTmp)>>27 &0x1F)==uGeo) { //trailer has geo
            //no data word. size should be zero
            Warning("execute()","Empty LATCH !!!!!!");
          } else if (++pLatchTmp && ((*pLatchTmp)>>27 &0x1F)==uGeo){
            #if DEBUG_LEVEL>4
               printf("Size of latch  0x%08x block is :%d\n",
                      (*pLatchTmp),((*pLatchTmp) & 0x7fffff)/4 );
               printf("Latch data is 0x%08x\n",*(pLatchTmp-1));
            #endif
             gHades->getCurrentEvent()->getHeader()->setTBit(*(pLatchTmp-1));
          } else {
            Error("execute()","Wrong latch data!");
          }
          data=pLatchTmp;
        } else if ( ((*data>>24) & 0x6)==6 ){
          #if DEBUG_LEVEL>2
            Error("execute()","Evt: %i, Invalid Data Word! Type: %d,Skipping 0x%08x",
                  nEvt,((*data>>24) & 0x6),*data);
            pSubEvt->dumpIt();
          #endif
        } else if (((*data>>24) & 0x6)==0) {
          Error("execute()","Evt: %i, Data follow, but no header",nEvt);
        } else {
          Error("execute()","Evt: %i, Unknow raw data type %i",
                nEvt,((*data>>24) & 0x6));
        }
      }  //end data
    }
  } //end new data structures
  #if DEBUG_LEVEL>4
     gDebuger->leaveFunc("HLatchUnpacker::execute\n");
  #endif
  return 1;
}

UInt_t* HLatchUnpacker::fillLatch(UInt_t* pData) {
  //Function for filling latch structure. (zero or one data word)
  //Latch appears only in the new data scheme
  //returns pointer to the next data word  in subevt.
  UInt_t uWordCount    = 0; //
  UInt_t* uTmp         = NULL;
  UInt_t uBlockSize    = 0; //Number of data words in one block.
  uBlockSize = (*pData) & 0x0000003ff;
  //Check header
  #if DEBUG_LEVEL>2
     printf("Latch data 0x%08x Size: %d\n",*pData,uBlockSize);
  #endif
  if (*++pData == 0x8000000) {
    //check trailer
#warning FIXME  latch trailer !!!!
    if ((*(pData+uBlockSize-1)>>24)!=0x8) {
      Error("fillLatch","Header found but no trailer!!");
    } else {
      //read latch
      uTmp=pData;
      uWordCount=0;
      while (++pData && pData<uTmp+uBlockSize-1 && *pData!=0x0) {
        uWordCount++;
        if (uWordCount== 1){  //Only one word for latch!
          //printf("Latch data 0x%08x \n",*pData);
          gHades->getCurrentEvent()->getHeader()->setTBit(*pData);
        } else {
          Error("fillLatch","Too many data words for Latch!!!!");
          return pSubEvt->getEnd();
        }
      }
      if (uWordCount!=(uBlockSize-2)) {  //-header -trailer
        Error("fillLatch()",
              "Found %i but expecting %i words!!!", uWordCount,uBlockSize);
        return pSubEvt->getEnd();
      }
    }
  } else{
    Error("fillLatch",
          "Header for latch data not found!");
    return pData;
  }
  return pData;
}

UInt_t* HLatchUnpacker::skipScaler(UInt_t* pData,Bool_t bOld,Int_t nType){
  // Skips the scaler data in old readout schema used before aug04
  // Returns the pointer to the next data word
  UInt_t uWordCount    = 0;
  UInt_t* uTmp         = NULL;
  UInt_t uBlockSize    = 0; //Number of data words in one block
  Int_t nCountBox      = 0;
  // set 9th bit to 1 when calibration event
  uBlockSize = (*pData) & 0x0000003ff;
  //Check header
  if (bOld) {
    //In old data no TIP-header and no size information
    //header+32 data words + trailer = 34
    //printf("Old data\n");
    uBlockSize=34; //MAX VALUE
  }
  #if DEBUG_LEVEL>2
     printf("Scaler data 0x%08x Size: %d\n",*pData,uBlockSize);
  #endif
  if (*++pData == 0x08000000 || nType==SCALER_JAN04) {
    //Header exists, check trailer
    //since nov04  no header/trailer
#warning FIXME scaler trailer !!!!
    if (!bOld && (*(pData+uBlockSize-1)>>24)!=0x8 && nType!=SCALER_JAN04) {
      Error("skipScaler","Header found but no trailer!!");
      return pSubEvt->getEnd();
    } else {
      //read scaler
      uTmp=pData;
      if (nType==SCALER_JAN04) { //No header!!!
        uTmp=pData;
        pData--;
      }
      uWordCount=0;
      nCountBox=0;
      while (++pData && pData<uTmp+uBlockSize && (*pData>>8) != 0x80000) {
        uWordCount++;
        if(nCountBox < 32) {
          nCountBox++;
        } else {
          Error("skipScaler","Evt %d. Too many data words for scaler",
                gHades->getCurrentEvent()->getHeader()->getEventSeqNumber());
          return pSubEvt->getEnd();
        }
      }
      if (!bOld && ((nType==SCALER_JAN04 &&  uWordCount!=uBlockSize) || (nType!=SCALER_JAN04 && uWordCount!=(uBlockSize-2)))) {
        Error("skipScaler",
              "Found %i but expecting %i words!!!", uWordCount,uBlockSize);
        return pSubEvt->getEnd();
      }
      if (nType==SCALER_JAN04) pData--; //no trailer
    }
  } else {
    Error("skipScaler",
          "Header for scaler data %p not found!",pData);
  }
  return pData;
}
