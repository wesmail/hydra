//*-- Author : Walter Karig
//*-- Modified : 03/05/2005 by Ilse Koenig
//*-- Modified : 16/09/2003 by T.Wojcik
//*-- Modified : 24/11/2002 by Dusan Zovinec
//*-- Modified : 18/12/2001 by Ilse Koenig
//*-- Modified : 29/11/2000 by Ilse Koenig
//*-- Modified : 13/11/2000 by M. Golubeva
//*-- Modified : 05/06/98 by Manuel Sanchez

using namespace std;
#include "htboxunpacker.h"
#include "hstartdef.h"
#include "hcategory.h"
#include "hevent.h"
#include "hldsubevt.h"
#include "hades.h"
#include "htboxchan.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hdebug.h"
#include "heventheader.h"
#include <iostream>
#include <iomanip>
#include <stdlib.h>

ClassImp(HTBoxUnpacker)

//////////////////////////////////////////////////////////////////
//
// HTBoxUnpacker
//
// Unpacker used to read Scalers data from .hld files
// Fills the TBox category.
//
//////////////////////////////////////////////////////////////////

HTBoxUnpacker::HTBoxUnpacker(Int_t id) {
  // Constructor
  // Input: subevent id
  subEvtId=id;
  tboxCat=0;
}

Bool_t HTBoxUnpacker::init(void) {
  // Creates the TBox category for the scalers, if not yet existing
  tboxCat=(HCategory*)gHades->getCurrentEvent()->getCategory(catTBoxChan);
  if (!tboxCat) {
    tboxCat=gHades->getSetup()->getDetector("TBox")->buildCategory(catTBoxChan);
    if (tboxCat) gHades->getCurrentEvent()->addCategory(catTBoxChan,tboxCat,"TBox");
    else return kFALSE;
  }
  loc.set(2,0,0);
  return kTRUE;
}

Int_t HTBoxUnpacker::execute() {
  // Unpacks the scaler data and fills the TBox data category
#if DEBUG_LEVEL>4
  gDebuger->enterFunc("HTBoxUnpacker::execute\n");
#endif
  if (!(gHades->isCalibration())) {  // no calibration event, nothing to be done for scalers
    return 1;
  }
  Bool_t bIsOld      = kFALSE;
  Int_t nEvtId       = 0; //Evt Id
  Int_t nEvt         = 0; //Evt number
  UInt_t uOldCalType = 0;
  UInt_t uTrigbits   = 0;
  UInt_t uBlockSize  = 0; //Number of data words in one block
  UInt_t uBoardType  = 0; // 1 - Struck, 0 - TDC
  UInt_t uStruck     = 0; // scaler or latch
  UInt_t* uTmp       = NULL;
  UInt_t uWordCount  = 0;
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
          data = fillScaler(data,&uTrigbits,kTRUE);
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
                data=fillScaler(data,&uTrigbits,kFALSE,SCALER);
              } else if (uStruck == SCALER_JAN04){
                data=fillScaler(data,&uTrigbits,kFALSE,SCALER_JAN04);
              } else if (uStruck == LATCH) {
                data = skipLatch(data);
              } else if (uStruck == DEBUG) {
                data+=uBlockSize;
                continue;
              } else {
                Error("execute","Unknown type=%i",uStruck);
              }
            } else if (uBoardType!=0) {  // 0 = tdc header
              Error("execute","Unknown BoardType=%i",uBoardType);
            }
          } else if (((*data>>24) & 0x6)==2) {  // Scan for TDC header
            // Found valid TDC header
            uBlockSize=((*data) >>  8) & 0xff;
            // Check if also trailer exists
            if ((*(data + uBlockSize+1) >>24 & 0x6)!=4) {
              Error("execute","Header found but no trailer!!");
              continue;
            }
            // Check trigger tag in the trailer
            if ((*(data+uBlockSize+1)&0xff)!=(UInt_t)pSubEvt->getTrigNr()) {
              Error("execute","Evt= %d Trigger tag mismatch!",nEvt);
            }
            uTmp=data;
            uWordCount=0;
            while (++data && data<(uTmp + uBlockSize+1) && *data!=0x0) {
              uWordCount++;
            }
            if (uWordCount!=uBlockSize) {
              Error("execute()","Found %i but expecting % words!!!",
                    uWordCount,uBlockSize);
            }
          } else if (((*data>>24) & 0x6)==6) {
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
        }
      } // end data
    } // pSubEvt
  } else if(subEvtId==416) {   // New data structures since aug04
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
            Int_t nScalerCounts=0;
            Int_t nScalerCountsTotal=0;
            //Count valid scaler blocks, for correct scaler numbering
            while( uScalerTotalSize < uBlockSize ) {
              //Here should be trailer
              pScalerTrailer = data+uBlockSize-uScalerTotalSize;
              uScalerSize=((*pScalerTrailer) & 0x7fffff)/4;
              //and header
              pScalerHeader =  pScalerTrailer-uScalerSize+1;
              uScalerTotalSize = uScalerTotalSize+uScalerSize;
              if ((*pScalerHeader &  0x3fffff )==0) {
                nScalerCounts++;
              } else {
                data+=uBlockSize;
                break; //return to main loop
              }
            }
            nScalerCountsTotal=nScalerCounts;
            //printf("Found %d scaler(s)\n",nScalerCounts);
            #if DEBUG_LEVEL>2
               printf("Found %d scaler(s)\n",nScalerCounts);
            #endif
            //Clear variables and fill scalers
            pScalerTrailer=0;
            uScalerSize=0;
            uScalerTotalSize=0;
            pScalerHeader = 0;
            nScalerCounts=0;
            //Loop over all scalers data
            clearScalData();
            while( uScalerTotalSize < uBlockSize ){
              //Here should be trailer
              pScalerTrailer = data+uBlockSize-uScalerTotalSize;
              uScalerSize=((*pScalerTrailer) & 0x7fffff)/4;
              //and header
              pScalerHeader =  pScalerTrailer-uScalerSize+1;
              uScalerTotalSize = uScalerTotalSize+uScalerSize;
              #if DEBUG_LEVEL>4
                 printf("Scaler header 0x%08x trailer 0x%08x\n",
                        *pScalerHeader,*pScalerTrailer);
              #endif
              if ((*pScalerHeader &  0x3fffff )==0) {
                #if DEBUG_LEVEL>4
                   printf("Found valid scaler header and trailer\n");
                   printf("Size of scaler is %d\n",
                          ((*pScalerTrailer) & 0x7fffff)/4-2);
                #endif						
                fillScalerData(nScalerCountsTotal-nScalerCounts-1,pScalerHeader,uScalerSize-2);
                nScalerCounts++;
              } else {
                printf("No valid SIS header for scalers found. Data may be corrupted. Skipping it.\n");
                data+=uBlockSize;
                break; //return to main loop
              }
            }
            //Fill tboxchan with all scaler data
            fillScalers(nScalerCounts);
            data+=uBlockSize;
          } else if (uBoardType!=0) {
            Error("execute","Unknown BoardType=%i",uBoardType);
          }
        } else if (((*data>>24) & 0x6)==2) {  // tdc
          uBlockSize = ( (*data) >> 8   ) & 0xff;
          //Check if also trailer exists
          if ((*(data + uBlockSize+1) >>24 & 0x6)!=4) {
            Error("execute","Header found but no trailer!!");
            continue;
          }
          //Check trigger tag in the trailer
          if ((*(data+uBlockSize+1)&0xff)!=(UInt_t)pSubEvt->getTrigNr()) {
            Error("execute","Evt= %d Trigger tag mismatch!",nEvt);
            //Error("execute","Trigger tag mismatch!");
            //exit(EXIT_FAILURE);
          }
          uTmp=data;
          uWordCount=0;
          while( ++data && data<(uTmp + uBlockSize+1) && *data!=0x0 ){
            uWordCount++;
          }
          if (uWordCount!=uBlockSize) {
            Error("execute()","Found %i but expecting % words!!!",
                  uWordCount,uBlockSize);
          }
        } else if (((*data) &  0x3fffff )==0) {  // SIS3829 header for latch
          UInt_t uGeo = (*data)>>27 &0x1F;
          //Latch should have one data word, zero is also possible
          #if DEBUG_LEVEL>4
             printf("Trying to find the size \n");
          #endif
          UInt_t* pLatchTmp=data;
          if (++pLatchTmp && ((*pLatchTmp)>>27 &0x1F)==uGeo) {
            Warning("execute()","Empty LATCH !!!!!!");
          } else if (!(++pLatchTmp && ((*pLatchTmp)>>27 &0x1F)==uGeo) ) {
            Error("execute()","Wrong latch data!");
          }
          data=pLatchTmp;
        } else if ( ((*data>>24) & 0x6)==6 ) {
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
      }  // end data
    }
  } //end new data structures
  #if DEBUG_LEVEL>4
     gDebuger->leaveFunc("HTBoxUnpacker::execute\n");
  #endif
  return 1;
}

UInt_t* HTBoxUnpacker::fillScaler(UInt_t* pData,UInt_t* uTrigbits,Bool_t bOld,Int_t nType){
  // Function for filling scaler data
  // returns the pointer to the next data word
  HTBoxChan* ptboxchan = 0;
  UInt_t uWordCount    = 0;
  UInt_t* uTmp         = NULL;
  UInt_t uBlockSize    = 0; //Number of data words in one block
  Int_t nCountBox      = 0;
  // set 9th bit to 1 when calibration event
  *uTrigbits = *uTrigbits | 0x100;
  uBlockSize = (*pData) & 0x0000003ff;
  clearScalData();
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
    //new data structure ???
    //since nov04  no header/trailer
#warning FIXME scaler trailer !!!!
    if (!bOld && (*(pData+uBlockSize-1)>>24)!=0x8 && nType!=SCALER_JAN04) {
      Error("fillScaler","Header found but no trailer!!");
      clearScalData();
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
          if ((*pData & 0xff000000) != 0x0) {
             scalData[nCountBox] = 0x0;
          } else {
            //printf("scalData %d\n",(*pData & 0xffffff));
            scalData[nCountBox] = (*pData & 0xffffff);
          }
          nCountBox++;
        } else {
          Error("fillScaler","Evt %d. Too many data words for scaler",
                gHades->getCurrentEvent()->getHeader()->getEventSeqNumber());
          clearScalData();
          return pSubEvt->getEnd();
        }
      }
      if (!bOld && ((nType==SCALER_JAN04 &&  uWordCount!=uBlockSize) || (nType!=SCALER_JAN04 && uWordCount!=(uBlockSize-2)))) {
        Error("fillScaler",
              "Found %i but expecting %i words!!!", uWordCount,uBlockSize);
        return pSubEvt->getEnd();
      }
      if (nType==SCALER_JAN04) pData--; //no trailer
    }
    if(tboxCat) {
printf("******* scaler filled\n");
      for (Int_t i=0; i<8; i++) {
        ptboxchan=(HTBoxChan*)tboxCat->getNewSlot(loc);
        if(!ptboxchan) continue;
        ptboxchan = new(ptboxchan) HTBoxChan;
        ptboxchan->set(i,scalData[i],scalData[i+8],scalData[i+16],scalData[i+24]);
      }
    }
  } else {
    Error("fillScaler",
          "Header for scaler data %p not found!",*pData);
  }
  return pData;
}

void HTBoxUnpacker::fillScalerData(Int_t n,UInt_t* pScalerData,Int_t nSize){
  // Used since aug04 data
  for (Int_t i=0;i<nSize;i++) {
    pScalerData++;
    scalData[n*32 + i] = (*pScalerData & 0xffffffff);
  }
}

void HTBoxUnpacker::fillScalers(Int_t nScalerCounts) {
  //Function for filling TBoxChan category for given number of scalers
  HTBoxChan* ptboxchan = NULL;
  if (tboxCat) {
    for (Int_t i=0;i<8;i++) {
      ptboxchan=(HTBoxChan*)tboxCat->getNewSlot(loc);
      if (!ptboxchan) {
        printf("Can not get new slot for HTBoxChan\n");
        continue;
      } else {
        ptboxchan = new(ptboxchan) HTBoxChan;
        for (Int_t s=0;s<nScalerCounts;s++){
          ptboxchan->set(i,scalData[i+s*32],
          scalData[i+s*32+8],
          scalData[i+s*32+16],
          scalData[i+s*32+24],s);
        }
      }
    }
  }
}

UInt_t* HTBoxUnpacker::skipLatch(UInt_t* pData) {
  // Skips the latch data in old readout schema used before aug04
  // Returns the pointer to the next data word
  UInt_t uWordCount    = 0;
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
      Error("skipLatch","Header found but no trailer!!");
      clearScalData();
    } else {
      //read latch
      uTmp=pData;
      uWordCount=0;
      while (++pData && pData<uTmp+uBlockSize-1 && *pData!=0x0) {
        uWordCount++;
        if (uWordCount== 1){  //Only one word for latch!
          //printf("Latch data 0x%08x \n",*pData);
        } else {
          Error("skipLatch","Too many data words for Latch!!!!");
          return pSubEvt->getEnd();
        }
      }
      if (uWordCount!=(uBlockSize-2)) {  //-header -trailer
        Error("skipLatch()",
              "Found %i but expecting %i words!!!", uWordCount,uBlockSize);
        return pSubEvt->getEnd();
      }
    }
  } else{
    Error("skipLatch",
          "Header for latch data not found!");
    return pData;
  }
  return pData;
}
