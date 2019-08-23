// File: hmdcunpacker.cc
//
// Author: Walter Karig <W.Karig@gsi.de>
// Modified by D.B <D.Bertini@gsi.de>
// Last update: 99/17/10 19:15:51
//

#include <stdio.h>
#include "hmdcunpacker.h"
#include "hades.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hldsubevt.h"
#include "hevent.h"
#include "hmdcraw.h"
#include "hspectrometer.h"
#include "hmdcdetector.h"
#include "hruntimedb.h"

Bool_t HMdcDataWord::decode( const UInt_t data ){

    //***************************************************** 
    // decoding of 32 bit data word from right->left 
    //
    //  bits        sz  mask           comments 
    // ----------------------------------------------------
    //  0 - 10      11  0x7ff          time value
    //      11       1  0x1            hit #0..1
    // 12 - 14       3  0x7            Channel of TDC Chip#0..7
    // 15 - 18       4  0xf            TDC Chip#1..12
    // 19 - 20       2  0x3            MBO#1..3 
    //      21       1  0x1            LVL1 Bus Pos (0,1) 
    // 22 - 24       3  0x7            RoC#1..7 
    //      25       1  0x1            SAM# Pos  
    // 26 - 29       4  0xf            SAM#1..12
    // 30 - 31       2   -             not used
    //*******************************************************  
   
    sam = (data >> 26 & m4)-1; 
    samP = data >> 25 & m1;
    roc = (data >> 22 & m3); 
    rocP = data >> 21 & m1;
    mbo = (data >> 19 & m2); 
    tdc = (data >> 15 & m4)-1;
    chan = data >> 12 & m3;
    hit =  data >> 11 & m1;
    time = data & m11;   
return  kTRUE;}




Int_t HMdcUnpacker::getMbo(const Int_t nsamN, const Int_t nroc, 
      const Int_t nrocP, const Int_t nmbo) const {
  
 if(pReadout){ 
    HMdcEvReadoutBus& bus =(*pReadout)[nsamN][nroc][nrocP][nmbo];
    return bus.getMbo();
 
   }
 return -999;
}

Bool_t HMdcUnpacker::init() {
    
   pReadout = (HMdcEvReadout*) 
                gHades->getRuntimeDb()->getContainer("MdcEvReadout");
    if(!pReadout){
             pReadout = new HMdcEvReadout();
             gHades->getRuntimeDb()->addContainer(pReadout); 
    }  
    
    pRawCat = gHades->getCurrentEvent()->getCategory(catMdcRaw);
    if (pRawCat){
	return kTRUE;
    } else {
 	pRawCat = gHades->getSetup()->getDetector("Mdc")
 	    ->buildCategory(catMdcRaw);
 	if (pRawCat) {
 	    return gHades->getCurrentEvent()
		->addCategory(catMdcRaw, pRawCat, "Mdc");
	} else{
	    return kFALSE;
	}
    }
}


Int_t HMdcUnpacker::execute() {
  // print();
    return fill();
}

Int_t HMdcUnpacker::fill() {
  Int_t ncons=0;
    if (pSubEvt) {
	HLocation loc;
	loc.set(4, 0, 0, 0, 0);
       

	for (UInt_t* data = pSubEvt->getData(), *end = pSubEvt->getEnd();
	     data < end; data++)
	  {
	    
	    w->clear(); 
            if(w->decode(*data)){
                      
            Int_t nsam = w->getSam()*2+w->getSamP();
	    Int_t mbo = getMbo(nsam,w->getRoc(),w->getRocP(),w->getBus());
	    Int_t tdc = getTdc(w->getTdc(),w->getChan());
	    
            loc[2] = mbo;
	    loc[3] = tdc;
                      
            HMdcRaw *pCell= NULL;
            pCell = (HMdcRaw*) pRawCat->getObject(loc);
	    if (!pCell) {
	       pCell = (HMdcRaw*) pRawCat->getSlot(loc);
	       if (pCell) {
		 pCell = new (pCell) HMdcRaw(); 
		 ncons++;
		 pCell->setMbo(mbo);
		 pCell->setTdc(tdc);
	       }
	      else {
		Error("HMdcUnpacker::execute", "can't get slot\n");
		return 2;
	      }
	    }
	    pCell->setTime(w->getTime());	  
	  }else { 
        Error("HMdcUnpacker::fill()"," error while decoding the data-word \n");
	return -1;
	}
      }
    }
  return 1; 
}

void HMdcUnpacker::print() {
    if (pSubEvt) {
        w->clear();
	for (UInt_t* data = pSubEvt->getData(), *end = pSubEvt->getEnd();
 	     data < end; data++){
        if(w->decode(*data)){
           }else{
         printf("problem with data decoding !\n");
   	}
    }
}
}
ClassImp(HMdcDataWord)
ClassImp(HMdcUnpacker)




