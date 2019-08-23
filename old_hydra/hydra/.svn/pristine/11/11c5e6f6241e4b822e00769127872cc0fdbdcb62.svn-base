//*-- Author : Walter Karig
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
#include <stdlib.h>

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
#if DEBUG_LEVEL>4
    gDebuger->enterFunc("HStartUnpacker::execute\n");
#endif
    HStartRaw* pRaw    = 0;
    Bool_t bIsOld      = kFALSE;
    Int_t nCrate       = 0;
    Int_t nSlot        = 0;
    Int_t nChannel     = 0;
    Int_t nData        = 0;
    Int_t nMod         = 0;
    Int_t nStrip       = 0;
    Int_t nEvtId       = 0; //Evt Id
    Int_t nEvt         = 0; //Evt number
    UInt_t uOldCalType = 0;
    UInt_t uWordCount  = 0; //
    UInt_t uTrigbits   = 0;
    UInt_t uBlockSize  = 0; //Number of data words in one block
    UInt_t uBoardType  = 0; // 1 - Struck, 0 - TDC
    UInt_t uCode       = 0; //
    UInt_t uCtrl        = 0; //
    UInt_t uStruck     = 0; // scaler or latch
    UInt_t *uTmp       = NULL;
  // trigbits = (gHades->getCurrentEvent()->getHeader()->getTBit() & 0xfffffe00);
if(subEvtId==412){
    if(pSubEvt) {
        UInt_t* data = pSubEvt->getData();
        UInt_t* end  = pSubEvt->getEnd();
        #if DEBUG_LEVEL>4
            printf("Next SubEvt. data between: %p and %p\n",data,end);
            pSubEvt->dumpIt();
        #endif
        nEvtId = gHades->getCurrentEvent()->getHeader()->getId();
        nEvt=gHades->getCurrentEvent()->getHeader()->getEventSeqNumber();
        //only in old data
        uOldCalType  = (*data & 0xf00);
        uTmp = data;
        bIsOld = gHades->getCurrentEvent()->getHeader()->isOldDAQ();
        //Loop over all data words
        while( ++data<end && *data!=0x0 ){
            #if DEBUG_LEVEL>2
                printf("Data word: %p\n",*data);
            #endif
            //nEvtId==5 (calibration event) and no TIP-block header indicate
            //old data scheme
            if(nEvtId==5 && (*(data) == 0x8000000) && bIsOld && uOldCalType==0x900){
                //There is no TIP header - go back one word
                data--;
                data = fillScaler(data,&uTrigbits,kTRUE);
            }
            else{
                //Scan for TIP-block header ( TYPE 0 )
                if( ((*data >>24) & 0x6)==0 ){
                    uBlockSize = (*data) & 0x0000003ff;
                    uBoardType = (*data >> 12 ) & 0x1;
                    uStruck    = (*data >> 10 ) & 0x3f;
                    #if DEBUG_LEVEL>4
                        cout<<"Struck "<<uStruck <<" BoardType:"<< uBoardType<<endl;
                    #endif
                    //latch or scaler
                    if(uBoardType==1){
                        if( uStruck == SCALER ){
                            uTrigbits = uTrigbits | 0x100;
                            data = fillScaler(data,&uTrigbits,kFALSE,SCALER);
                        }
                        else if ( uStruck == SCALER_JAN04 ){
                            data = fillScaler(data,&uTrigbits,kFALSE,SCALER_JAN04);
                        }
                        else if ( uStruck == LATCH ){
                            data = fillLatch(data);
                        }
                        else if ( uStruck == DEBUG ){
                            Info("execute","DEBUG block, size: %i",uBlockSize);
                            data +=uBlockSize;
                            continue;
                        }
                        else{
                            Error("execute","Uknown type=%i",uStruck);
                        }

                    }
                    //next Should be tdc header
                    else if(uBoardType==0){
                        //Do a normal TDC scanning
                        #if DEBUG_LEVEL>2
                            printf("TIP-block 0x%08x Size %i Board  %i\n",
                                    *data,uBlockSize,uBoardType);
                        #endif
                    }
                    else{
                        Error("execute","Uknown BoardType=%i",uBoardType);
                    }
                }
                ///Scan for TDC header
                else if ( ((*data>>24) & 0x6)==2 ){
                    //Found valid TDC header
                    uBlockSize = ( (*data) >> 8   ) & 0xff;
                    nCrate     = ( (*data) >> 16  ) & 0xff;
                    nSlot      = ( (*data) >> 27  ) & 0x1f;
                    //Check if also trailer exists
                    if ((*(data + uBlockSize+1) >>24 & 0x6)!=4){
                        Error("execute","Header found but no trailer!!");
                        continue;
                    }
                    //Check trigger tag in the trailer
                    if ((*(data+uBlockSize+1)&0xff)!=(UInt_t)pSubEvt->getTrigNr() ){
                        Error("execute","Evt= %d Trigger tag mismatch!",nEvt);
                        //Error("execute","Trigger tag mismatch!");
                        //exit(EXIT_FAILURE);
                    }
                    //Scan all data words
                    #if DEBUG_LEVEL>4
                        printf("TDC header 0x%08x.Size: %i.Crate: %i.Slot: %i.\n",
                            *data,uBlockSize,nCrate,nSlot);
                    #endif
                    uTmp=data;
                    HStartLookupTdc* pTdc=lookup->getTdc(nCrate-1,nSlot-1);
                    if (pTdc) {
                        uWordCount=0;
                        while( ++data && data<(uTmp + uBlockSize+1) && *data!=0x0 ){
                            uWordCount++;
                            nChannel = (*(data) >> 16 ) & 0x003f;
                            nData    = *data  & 0xfff;
                            // trigger particular stuff in old data
                            // read only data from last 8 channels of crate 2
                            // slot 11 (trigger info)
                            if(bIsOld &&  (nSlot==11) && (nCrate==2) ){
                                if(nChannel==24) uTrigbits = uTrigbits | 0x1;
                                if(nChannel==25) uTrigbits = uTrigbits | 0x2;
                                if(nChannel==26) uTrigbits = uTrigbits | 0x4;
                                if(nChannel==27) uTrigbits = uTrigbits | 0x8;
                                if(nChannel==28) uTrigbits = uTrigbits | 0x10;
                                if(nChannel==29) uTrigbits = uTrigbits | 0x20;
                                if(nChannel==30) uTrigbits = uTrigbits | 0x40;
                                if(nChannel==31) uTrigbits = uTrigbits | 0x80;
                            }
                            HStartLookupChan* tc=pTdc->getChannel(nChannel);
                            if (tc){
                                tc->getAddress(nMod,nStrip);
                                if (nMod>=0 && nStrip>=0){
                                    loc[0]=nMod;
                                    loc[1]=nStrip;
                                    pRaw=(HStartRaw*)pRawCat->getObject(loc);
                                    if (!pRaw){
                                        pRaw=(HStartRaw*)pRawCat->getSlot(loc);
                                        if (pRaw) {
                                            pRaw=new (pRaw) HStartRaw;
                                        }
                                        else {
                                            Error("execute()",
                                                    "Can't get slot\n");
                                            return EXIT_FAILURE;
                                        }
                                    }
                                    else {
                                        #if DEBUG_LEVEL>2
                                            Info("execute()",
                                                    "Slot already exists!\n");
                                        #endif
                                    }
                                    char cType='\0';
                                    Int_t  nType=-1;
                                    cType = tc->getType();
                                    if(cType !='\0') {
                                        switch(cType){
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
                                    }
                                    else{
                                        pRaw->fill(nData, nMod,nStrip);
                                    }
                                    #if DEBUG_LEVEL>2
                                    printf("Evt:%d Mod:%d Str:%d Typ:%d Data:%d\n",
                                            nEvt,nMod,nStrip,nType,nData);
                                    #endif
                                }
                                else {
                                    #if DEBUG_LEVEL>4
                                        Error("execute()",
                                            "Wrong address! Mod: %i Strip; %i",
                                            nMod,nStrip);
                                    #endif
                                }
                            }
                            else {
                                #if DEBUG_LEVEL>2
                                    Info("execute()",
                                        "TDC channel: %i not found",nChannel);
                                #endif
                            }
                        }
                        if(uWordCount!=uBlockSize){
                            Error("execute()",
                                    "Found %i but expecting % words!!!",
                                    uWordCount,uBlockSize);
                        }
                    }//TDC
                    else{
                        //Skip this block
                        data+=uBlockSize+1;
                        #if DEBUG_LEVEL>4
                            Error("execute()",
                                "Can not get TDC for create %i and slot %i",
                                nCrate-1,nSlot-1);
                        #endif
                    }

                }
                else if ( ((*data>>24) & 0x6)==6 ){
                    #if DEBUG_LEVEL>2
                        Error("execute()",
                            "Evt: %i, Invalid Data Word! Type: %d,Skipping 0x%08x",
                            nEvt,((*data>>24) & 0x6),*data);
                            pSubEvt->dumpIt();
                    #endif
                }
                else if ( ((*data>>24) & 0x6)==0 ){
                    Error("execute()",
                            "Evt: %i, Data follow, but no header",nEvt);
                }
                else {
                    Error("execute()",
                            "Evt: %i, Unknow raw data type %i",
                            nEvt,((*data>>24) & 0x6));
                }
            }
        }//end data
    }//
    // store trigger info into the EventHeader::fTBit
    if(bIsOld)
        gHades->getCurrentEvent()->getHeader()->setTBit(uTrigbits);
}
else if(subEvtId==416){ //New data structures fince aug04

	
    if(pSubEvt) {
	    UInt_t* data = pSubEvt->getData();
        UInt_t* end  = pSubEvt->getEnd();
        //pSubEvt->dumpIt();
        #if DEBUG_LEVEL>4
            printf("Next SubEvt. data between: %p and %p\n",data,end);
            pSubEvt->dumpIt();
        #endif
        nEvtId = gHades->getCurrentEvent()->getHeader()->getId();
        nEvt=gHades->getCurrentEvent()->getHeader()->getEventSeqNumber();
        //Loop over all data words
        while( ++data<end && *data!=0x0 ){
            #if DEBUG_LEVEL>2
                printf("Data word: %p\n",*data);
            #endif
            //Scan for TIP-block header Scalers have always TIP-Block
            if( ((*data >>24) & 0x6)==0 && (((*data) &  0x3fffff )!=0)){
                uBlockSize = (*data) & 0x0000003ff;
                uBoardType = (*data >> 10 ) & 0x3f;
                uCode      = (*data >>20 )  & 0xf;
                uCtrl      = (*data >>20 )  & 0x7;
                #if DEBUG_LEVEL>4
                    printf("BoardType: %d\n ",uBoardType);
                #endif
                //new scaler aug04
                if( uBoardType==SCALER_AUG04 ){
                    #if DEBUG_LEVEL>4
                        printf("Total scalers block size is :%d \n",uBlockSize);
                    #endif
                    UInt_t *pScalerTrailer=0;
                    UInt_t uScalerSize=0;
                    UInt_t uScalerTotalSize=0;
                    UInt_t *pScalerHeader = 0;
                    Int_t nScalerCounts=0;
                    Int_t nScalerCountsTotal=0;

                    //Count valid scaler blocks, for correct scaler numbering
                    while( uScalerTotalSize < uBlockSize ){
                        //Here should be trailer
                        pScalerTrailer = data+uBlockSize-uScalerTotalSize;
                        uScalerSize=((*pScalerTrailer) & 0x7fffff)/4;
                        //and header
                        pScalerHeader =  pScalerTrailer-uScalerSize+1;
                        uScalerTotalSize = uScalerTotalSize+uScalerSize;
                        if((*pScalerHeader &  0x3fffff )==0){
                            nScalerCounts++;
                        }
                        else{
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
                            //printf("Scaler header 0x%08x trailer 0x%08x\n",
                              //                      *pScalerHeader,*pScalerTrailer);
                        #if DEBUG_LEVEL>4
                            printf("Scaler header 0x%08x trailer 0x%08x\n",
                                                    *pScalerHeader,*pScalerTrailer);
                        #endif
                        if((*pScalerHeader &  0x3fffff )==0){
                            //printf("Size of scaler is %d\n",
                              //                  ((*pScalerTrailer) & 0x7fffff)/4-2);
                            #if DEBUG_LEVEL>4
                            printf("Found valid scaler header and trailer\n");
                            printf("Size of scaler is %d\n",
                                                ((*pScalerTrailer) & 0x7fffff)/4-2);
                            #endif
								
                            fillScalerData(nScalerCountsTotal-nScalerCounts-1,pScalerHeader,uScalerSize-2);
                            nScalerCounts++;
                        }
                        else{
                            printf("No valid SIS header for scalers found. Data may be corrupted. Skipping it.\n");
                            data+=uBlockSize;
                            break; //return to main loop
                        }
                    }

                    //Fill tboxchan with all scaler data
                    fillScalers(nScalerCounts);
                    data+=uBlockSize;
                }
                //next Should be tdc header
                else if(uBoardType==0){
                    //Do a normal TDC/latch scanning
                    //exit from this statement
                    #if DEBUG_LEVEL>2
                        printf("TIP-block 0x%08x Size %i Board  %i Next is tdc header\n",
                                *data,uBlockSize,uBoardType);
                    #endif
                }
                else{
                    Error("execute","Uknown BoardType=%i",uBoardType);
                }
            }//end of TIP-block search
            ///Now Scan for TDC header
            else if ( ((*data>>24) & 0x6)==2 ){
                //Found valid TDC header
                uBlockSize = ( (*data) >> 8   ) & 0xff;
                nCrate     = ( (*data) >> 16  ) & 0xff;
                nSlot      = ( (*data) >> 27  ) & 0x1f;
                //Check if also trailer exists
                if ((*(data + uBlockSize+1) >>24 & 0x6)!=4){
                    Error("execute","Header found but no trailer!!");
                    continue;
                }
                //Check trigger tag in the trailer
                if ((*(data+uBlockSize+1)&0xff)!=(UInt_t)pSubEvt->getTrigNr() ){
                    Error("execute","Evt= %d Trigger tag mismatch!",nEvt);
                    //Error("execute","Trigger tag mismatch!");
                    //exit(EXIT_FAILURE);
                }
                //Scan all data words
                #if DEBUG_LEVEL>4
                    printf("TDC header 0x%08x.Size: %i.Crate: %i.Slot: %i.\n",
                        *data,uBlockSize,nCrate,nSlot);
                #endif
                //printf("%p %p\n",data,data+uBlockSize+1);
                uTmp=data;
                HStartLookupTdc* pTdc=lookup->getTdc(nCrate-1,nSlot-1);
                    if (pTdc) {
                        uWordCount=0;
                        while( ++data && data<(uTmp + uBlockSize+1) && *data!=0x0 ){
                            uWordCount++;
                            nChannel = (*(data) >> 16 ) & 0x003f;
                            nData    = *data  & 0xfff;
                            // trigger particular stuff in old data
                            // read only data from last 8 channels of crate 2
                            // slot 11 (trigger info)
                            HStartLookupChan* tc=pTdc->getChannel(nChannel);
                            if (tc){
                                tc->getAddress(nMod,nStrip);
                                if (nMod>=0 && nStrip>=0){
                                    loc[0]=nMod;
                                    loc[1]=nStrip;
                                    pRaw=(HStartRaw*)pRawCat->getObject(loc);
                                    if (!pRaw){
                                        pRaw=(HStartRaw*)pRawCat->getSlot(loc);
                                        if (pRaw) {
                                            pRaw=new (pRaw) HStartRaw;
                                        }
                                        else {
                                            Error("execute()",
                                                    "Can't get slot\n");
                                            return EXIT_FAILURE;
                                        }
                                    }
                                    else {
                                        #if DEBUG_LEVEL>2
                                            Info("execute()",
                                                    "Slot already exists!\n");
                                        #endif
                                    }
                                    char cType='\0';
                                    Int_t  nType=-1;
                                    cType = tc->getType();
                                    if(cType !='\0') {
                                        switch(cType){
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
                                        //printf("Evt:%d Mod:%d Str:%d Typ:%d Data:%d\n",
                                        //  nEvt,nMod,nStrip,nType,nData);
                                    }
                                    else{
                                        pRaw->fill(nData, nMod,nStrip);
                                    }
                                    #if DEBUG_LEVEL>2
                                    printf("Evt:%d Mod:%d Str:%d Typ:%d Data:%d\n",
                                            nEvt,nMod,nStrip,nType,nData);
                                    #endif
                                }
                                else {
                                    #if DEBUG_LEVEL>4
                                        Error("execute()",
                                            "Wrong address! Mod: %i Strip; %i",
                                            nMod,nStrip);
                                    #endif
                                }
                            }
                            else {
                                #if DEBUG_LEVEL>2
                                    Info("execute()",
                                        "TDC channel: %i not found",nChannel);
                                #endif
                            }
                        }
                        if(uWordCount!=uBlockSize){
                            Error("execute()",
                                    "Found %i but expecting % words!!!",
                                    uWordCount,uBlockSize);
                        }

                }
                else{
                    //Skip this block
                    data+=uBlockSize+1;
                    #if DEBUG_LEVEL>4
                        Error("execute()",
                            "Can not get TDC for create %i and slot %i",
                            nCrate-1,nSlot-1);
                    #endif
                }
            }
            //Check for SIS3829 header (defined by SIS) (latch only)
            else if(((*data) &  0x3fffff )==0){ //
                UInt_t uGeo = (*data)>>27 &0x1F;
                //Latch should have one data word, zero is also possible
                #if DEBUG_LEVEL>4
                    printf("Trying to find the size \n");
                #endif
                UInt_t *pLatchTmp=data;
                if(++pLatchTmp && ((*pLatchTmp)>>27 &0x1F)==uGeo){ //trailer has geo
                    //no data word. size should be zero
                    Warning("execute()","Empty LATCH !!!!!!");
                }
                else if (++pLatchTmp && ((*pLatchTmp)>>27 &0x1F)==uGeo){
                    #if DEBUG_LEVEL>4
                    printf("Size of latch  0x%08x block is :%d\n",
                                                (*pLatchTmp),((*pLatchTmp) & 0x7fffff)/4 );

                    printf("Latch data is 0x%08x\n",*(pLatchTmp-1));
                    #endif
                    gHades->getCurrentEvent()->getHeader()->setTBit(*(pLatchTmp-1));
                }
                else{
                         Error("execute()",
                        "Wrong latch data!");
                        //printf("Hmm 0x%08x\n",*pLatchTmp);

                }//return address
                data=pLatchTmp;
            }
            else if ( ((*data>>24) & 0x6)==6 ){
                #if DEBUG_LEVEL>2
                    Error("execute()",
                        "Evt: %i, Invalid Data Word! Type: %d,Skipping 0x%08x",
                        nEvt,((*data>>24) & 0x6),*data);
                        pSubEvt->dumpIt();
                #endif
            }
            else if ( ((*data>>24) & 0x6)==0 ){
                Error("execute()",
                        "Evt: %i, Data follow, but no header",nEvt);
            }
            else {
                Error("execute()",
                        "Evt: %i, Unknow raw data type %i",
                        nEvt,((*data>>24) & 0x6));
            }
        }//end data
    }//
}//end new data structures
#if DEBUG_LEVEL>4
    gDebuger->leaveFunc("HStartUnpacker::execute\n");
#endif

  return 1;
}

UInt_t* HStartUnpacker::fillScaler(UInt_t *pData,UInt_t *uTrigbits,Bool_t bOld,Int_t nType){
    //Fuction for filling scaler data
    //returns the pointer to the next data word

    HTBoxChan* ptboxchan = 0;
    UInt_t uWordCount    = 0; //
    UInt_t *uTmp         = NULL;
    UInt_t uBlockSize    = 0; //Number of data words in one block
    Int_t nCountBox      = 0;
    // set 9th bit to 1 when calibration event
    *uTrigbits = *uTrigbits | 0x100;
    uBlockSize = (*pData) & 0x0000003ff;
    clearScalData();
    //Check header
    if(bOld){
        //In old data no TIP-header and no size information
        //header+32 data words + trailer = 34
        //printf("Old data\n");
        uBlockSize=34; //MAX VALUE
    }
    #if DEBUG_LEVEL>2
        printf("Scaler data 0x%08x Size: %d\n",*pData,uBlockSize);
    #endif
    if(*++pData == 0x08000000 || nType==SCALER_JAN04){
        //Header exists, check trailer
        //new data structure ???
        //if( !bOld && (*(pData+uBlockSize-1))!=0x08000008 ){
        //since nov04  no header/trailer
#warning FIXME scaler trailer !!!!
        if( !bOld && (*(pData+uBlockSize-1)>>24)!=0x8 && nType!=SCALER_JAN04){
            Error("fillScaler","Header found but no trailer!!");
            clearScalData();
            return pSubEvt->getEnd();
        }
        else{
            //read scaler
            uTmp=pData;
            if(nType==SCALER_JAN04){ //No header!!!
                uTmp=pData;
                pData--;
            }
            uWordCount=0;
            nCountBox=0;
            while(++pData && pData<uTmp+uBlockSize && (*pData>>8) != 0x80000){
                uWordCount++;
                if(nCountBox < 32){
                    if((*pData & 0xff000000) != 0x0){
                        scalData[nCountBox] = 0x0;
                    }
                    else {
                        //printf("scalData %d\n",(*pData & 0xffffff));
                        scalData[nCountBox] = (*pData & 0xffffff);
                    }
                    nCountBox++;
                }
                else{
                    Error("fillScaler",
                        "Evt %d. Too many data words for scaler",
                        gHades->getCurrentEvent()->
                            getHeader()->getEventSeqNumber());
                    clearScalData();
                    return pSubEvt->getEnd();
                }
            }
            if( !bOld && ((nType==SCALER_JAN04 &&  uWordCount!=uBlockSize) || (nType!=SCALER_JAN04 && uWordCount!=(uBlockSize-2)))){
                Error("fillScaler",
                "Found %i but expecting %i words!!!", uWordCount,uBlockSize);
                return pSubEvt->getEnd();
            }
            if (nType==SCALER_JAN04) //no trailer
                pData--;
        }
        if(tboxCat) {
            for (Int_t i=0; i<8; i++){
                ptboxchan=(HTBoxChan*)tboxCat->getNewSlot(loc);
                if(!ptboxchan) continue;
                ptboxchan = new(ptboxchan) HTBoxChan;
                ptboxchan->set(i,scalData[i],scalData[i+8],scalData[i+16],
                                scalData[i+24]);
            }
        }
    }
    else{
        Error("fillScaler",
            "Header for scaler data %p not found!",*pData);
    }
    return pData;
}

UInt_t *HStartUnpacker::fillLatch(UInt_t *pData){
    //Fuction for filling latch structure. (zero or one data word)
    //Latch appears only in the new data scheme
    //returns pointer to the next data word  in subevt.
    UInt_t uWordCount    = 0; //
    UInt_t *uTmp         = NULL;
    UInt_t uBlockSize    = 0; //Number of data words in one block.
    uBlockSize = (*pData) & 0x0000003ff;
    //Check header
    #if DEBUG_LEVEL>2
        printf("Latch data 0x%08x Size: %d\n",*pData,uBlockSize);
    #endif
    if(*++pData == 0x8000000){
        //check trailer
#warning FIXME  latch trailer !!!!
        if( (*(pData+uBlockSize-1)>>24)!=0x8 ){
            Error("fillLatch","Header found but no trailer!!");
            clearScalData();
        }
        else{
            //read latch
            uTmp=pData;
            uWordCount=0;
            while(++pData && pData<uTmp+uBlockSize-1 && *pData!=0x0){
                uWordCount++;
                if(uWordCount  == 1){//Only one word for latch!
                    //printf("Latch data 0x%08x \n",*pData);
                    gHades->getCurrentEvent()->getHeader()->setTBit(*pData);
                }
                else{
                    Error("fillLatch","Too many data words for Latch!!!!");
                    return pSubEvt->getEnd();
                }
            }
            if(uWordCount!=(uBlockSize-2) ){//-header -trailer
                Error("fillLatch()",
                "Found %i but expecting %i words!!!", uWordCount,uBlockSize);
                return pSubEvt->getEnd();
            }
        }
    }
    else{
        Error("fillLatch",
            "Header for latch data not found!");
        return pData;
    }

    return pData;
}
//These functions are used since aug04 data
Int_t HStartUnpacker::fillScalerData(Int_t n,UInt_t *pScalerData,Int_t nSize){
    HTBoxChan* ptboxchan = NULL;
    for(Int_t i=0;i<nSize;i++){
        pScalerData++;
        scalData[n*32 + i] = (*pScalerData & 0xffffffff);
    }
    return 1;


}

Int_t HStartUnpacker::fillScalers(Int_t nScalerCounts){
    //Function for filling TBoxChan category for given number of scalers
    HTBoxChan* ptboxchan = NULL;
    if(tboxCat) {
        for (Int_t i=0; i<8; i++){
            ptboxchan=(HTBoxChan*)tboxCat->getNewSlot(loc);
            if(!ptboxchan) {
                printf("Can not get new slot for HTBoxChan\n");
                continue;
            }
            else{
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

ClassImp(HStartUnpacker)










