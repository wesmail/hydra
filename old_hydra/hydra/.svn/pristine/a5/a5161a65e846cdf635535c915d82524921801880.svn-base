// File: hmdcunpacker.cc
//
// Last update: 98/12/09 19:15:51 d.b
// Read Mdc data according to HAMOT techn. spec.

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





Int_t HMdcUnpacker::getMbo(const int roc, const int pos) const {
  //  return 2*roc + pos;	// 2 RC, 4 Mbo
  // Only one MBo per LVL1 Bus (Beam test 12/98) 
      return roc + 0*pos;      	// 4 RC, 4 Mbo
}

Bool_t HMdcUnpacker::init() {
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
  // print(); return 0;
    return fill();
}

Int_t HMdcUnpacker::fill(){
if(pSubEvt) {
   HLocation loc;
   loc.set(4, 0, 0, 0, 0);

   UInt_t* data=NULL;
   for (UShort_t* cursor = (UShort_t*)  pSubEvt->getData(),
        *end    = (UShort_t*)  pSubEvt->getEnd(); 
         cursor < end; cursor+=2){
//- packing 2 16-bit word in a 32-bit word    
          data= (UInt_t*) cursor;
          UInt_t tmp1 = *cursor;       
          UInt_t tmp2 = *(cursor+1);
 
          *data  = *data | (tmp2 << 16);
          *data  = *data & 0xffff0000; 
	  *data  = *data | tmp1;     
  	 
          Int_t mbo = getMbo(getRoc(*data), getRocPos(*data));
	  Int_t tdc = getTdc(getChip(*data), getChannel(*data));
       	  loc[2] = mbo;
	  loc[3] = tdc;
           HMdcRaw *pCell= NULL;
	   pCell = (HMdcRaw*) pRawCat->getObject(loc);

	   if (!pCell) {
	       pCell = (HMdcRaw*) pRawCat->getSlot(loc);
	       if (pCell) {
	  	 pCell = new (pCell) HMdcRaw(); 
		 pCell->setMbo(mbo);
		 pCell->setTdc(tdc);
	       }
	       else {
	 	Error("HMdcUnpacker::execute", "Can't get slot");
                printf(" in location loc0:%i loc1:%i loc2:%i loc3:%i \n",
                       loc[0],loc[1],loc[2],loc[3]); 
		return kFALSE;
	      }
	 }
	    pCell->setTime(getTime(*data));
   }
 
  }
    return kTRUE;
}


void HMdcUnpacker::print() {
    if (pSubEvt) {


UInt_t *data=NULL;
   for (UShort_t* cursor = (UShort_t*)  pSubEvt->getData(),
        *end    = (UShort_t*)  pSubEvt->getEnd(); 
        cursor < end; cursor+=2){

//- convert 2 16-bit words in a 32-bit word         
        data= (UInt_t*) cursor;
        UInt_t tmp1 = *cursor;       
        UInt_t tmp2 = *(cursor+1);

        *data  = *data | (tmp2 << 16);
	*data  = *data & 0xffff0000; 
	*data  = *data | tmp1; 

        Int_t mbo = getMbo(getRoc(*data), getRocPos(*data));
	Int_t tdc = getTdc(getChip(*data), getChannel(*data));
	/*
          printf("Mbo = %2d; Tdc = %2d; Time = %4d\n",
		   getMbo(getRoc(*data), getRocPos(*data)),
		   getTdc(getChip(*data), getChannel(*data)),
         	   getTime(*data));
	*/
         printf(" chip :%5i Chan:%5i \n", 
                getChip(*data),getChannel(*data)); 

   }
 }
}


ClassImp(HMdcUnpacker)







