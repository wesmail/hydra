//*-- Author : Ilse Koenig
//*-- Created: 01/12/2000
//*-- Modified: 08/08/2003 T.Wojcik
using namespace std;
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
#include "heventheader.h"
#include <iostream>
#include <iomanip>
#include <stdlib.h>

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
    HTofinoRaw* pRaw = 0;
    Int_t nCrate=-1, nSlot=-1, nChannel=0,  nSector=-1, nMod=-1, nCell=-1;
    Int_t nData       = 0;
	Int_t nId         = 0; //trigger type
	Int_t nEvt        = 0; //Evt Id
    Char_t dcType     ='U';
    UInt_t uWordCount = 0; //
    UInt_t uBlockSize = 0; //Number of data words in one block
    UInt_t uBoardType = 0; // 1 - Struck, 0 - TDC
    UInt_t *uTmp      = NULL;

	nId = gHades->getCurrentEvent()->getHeader()->getId();
	if( gHades->isCalibration() ){
        //calibration event
        //Nothing to do for tof
        return 1;
    }
    if(pSubEvt) {
        UInt_t* data=pSubEvt->getData();
        UInt_t* end=pSubEvt->getEnd();
        nEvt=gHades->getCurrentEvent()->getHeader()->getEventSeqNumber();
        while( ++data<end && *data!=0x0 ){
            #if DEBUG_LEVEL>4
                printf("Data word: %p\n",*data);
            #endif
            //Scan for TIP-block header ( TYPE 0 )
            if( ((*data >>24) & 0x6)==0 ){
                uBlockSize = (*data) & 0x0000003ff;
                uBoardType = (*data >> 12 ) & 0x1;
                //latch or scaler
                if(uBoardType==1){
                    data+=uBlockSize;
                    continue;
                }
                //next Should be tdc header
                else if(uBoardType==0){
                    //Do a normal TDC scanning
                    #if DEBUG_LEVEL>2
                        printf("TIP-block 0x%08x Size %i Board  %i\n",
                                *data,uBlockSize,uBoardType);
                    #endif
                    continue;
                }
                else{
                    Warning("execute","Uknown BoardType=%i",uBoardType);
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
                    Warning("execute","Header found but no trailer!!");
                    continue;
                }
                //Check trigger tag in the trailer
                if ((*(data+uBlockSize+1)&0xff)!=( ((UInt_t)pSubEvt->getTrigNr()
)&0xff ) ){
                    Error("execute","Trigger tag mismatch!");
                    exit(EXIT_FAILURE);
                }
                //Scan all data words
                #if DEBUG_LEVEL>4
                    printf("TDC header 0x%08x.Size: %i.Crate: %i.Slot: %i.\n",
                        *data,uBlockSize,nCrate,nSlot);
                #endif
                uTmp=data;
                HTofinoLookupDConv* dc=lookup->getDConv(nCrate-1,nSlot-1);
                if (dc) {
                    dcType=dc->getType();
                    uWordCount=0;
                    while( ++data && data<(uTmp + uBlockSize+1) && *data!=0x0 ){
                        uWordCount++;
                        if(nSlot!=(*data>>27)&0x1f){
                            Warning("execute",
                                "Slot (%d) different that in header(%d)",
                                nSlot,(*data>>27)&0x1f);
                            continue;
                        }
                        nChannel = (*(data) >> 16 ) & 0x003f;
                        nData    = *data  & 0xfff;
                        HTofinoLookupChan* dcs=dc->getChannel(nChannel);
                        if (dcs){
                            dcs->getAddress(nSector,nMod,nCell);
                            if (nSector>=0 && nSector<6) {
                                loc[0]=nSector;
                                loc[1]=nMod;
                                loc[2]=nCell;
                                pRaw=(HTofinoRaw *)pRawCat->getObject(loc);
                                if (!pRaw){
                                    pRaw=(HTofinoRaw *)pRawCat->getSlot(loc);
                                    if (pRaw) {
                                        pRaw=new (pRaw) HTofinoRaw;
                                        pRaw->setAddress(nSector,nMod,nCell);
                                    }
                                    else {
                                        Error("execute()",
                                                "Can't get slot\n");
                                        return -1;
                                    }
                                }
                                else {
                                    #if DEBUG_LEVEL>2
                                        Info("execute()",
                                            "Slot already exists!\n");
                                    #endif
                                }
                                if (dcType=='T'){
                                    pRaw->setTime(((*data & 0xfff) % 0xfff));
                                }
                                else {
                                    pRaw->setCharge(((*data & 0xfff) % 0xfff));
                                }
                            }
                            else {
                                #if DEBUG_LEVEL>4
                                    Warning("execute()",
                                        "Wrong address! Sector %i",
                                        nSector);
                                #endif
                            }
                        }//end dcs
                        else {
                            #if DEBUG_LEVEL>2
                            Warning("execute()",
                                    "TDC channel: %i not found",nChannel);
                            #endif

                        }
                    }
                    if(uWordCount!=uBlockSize){
                        Warning("execute()",
                                "Found %i but expecting % words!!!",
                                uWordCount,uBlockSize);
                    }
                }//TDC
                else{
                    //Skip this block
                    data+=uBlockSize+1;
                    #if DEBUG_LEVEL>4
                        Warning("execute()",
                            "Can not get TDC for create %i and slot %i",
                            nCrate-1,nSlot-1);
                    #endif
                }
            }
            else if ( ((*data>>24) & 0x6)==6 ){
                #if DEBUG_LEVEL>2
                    Warning("execute()",
                        "Evt: %i, Invalid Data Word! Type: %d,Skipping 0x%08x",
                        nEvt,((*data>>24) & 0x6),*data);
                    #if DEBUG_LEVEL>4
                        pSubEvt->dumpIt();
                    #endif
                #endif
            }
            else if ( ((*data>>24) & 0x6)==0 ){
                Warning("execute()",
                        "Evt: %i, Data follow, but no header",nEvt);
            }
            else {
                Warning("execute()",
                        "Evt: %i, Unknow raw data type %i",
                        nEvt,((*data>>24) & 0x6));
            }
        }//end data
    }
    return 1;
}
