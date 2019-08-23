// File: hstartunpacker.cc
//
// Author: Walter Karig <W.Karig@gsi.de>
// updated 10/12/98 d.b 
// Last update: 14/12/98 r. holzmann
//

#include "hstartunpacker.h"
#include "hldsubevt.h"
#include "hades.h"
#include "hrecevent.h"
#include "hpartialevent.h"
#include "hdebug.h"
#include <iostream.h>

Bool_t HStartUnpacker::init(void) {
  HRecEvent *event=0;
  HPartialEvent *par=0;

  event=(HRecEvent *)gHades->getCurrentEvent();
  if (!event) return kFALSE;
  par=event->getPartialEvent(catStart);
  if (!par) {
    event->addPartialEvent(catStart,"Start","The start event");
    par=event->getPartialEvent(catStart);
    if (!par) return kFALSE;
  }

  header = (HStartHeader *)par->getSubHeader();
  if (!header) {
    header=new HStartHeader;
    par->setSubHeader(header);
  }
  
  return kTRUE;
}

int HStartUnpacker::execute(void) {
UInt_t startCh;
UShort_t adcData[8];
UShort_t tdcData[8]; 

 if (pSubEvt) {
    header->cleardata();
    UShort_t* data =  (UShort_t*) pSubEvt->getData();
    UShort_t* first = data; 
    UShort_t* end  =  (UShort_t*) pSubEvt->getEnd(); 
   
    header->clear(adcData,tdcData);
 
    while (data < end){
      //      printf("data = %x\n",*data);
      if((*data++ == 0x2005) && ((*data >> 15) & 0x1 == 1)) { // this is the TDC
	//      printf("data = %x\n",*data);
        startCh = (*data >> 12 & 0x7) + 1;
	//        printf("TDC chan = %d\n",startCh);
        data++;
        for(UInt_t i=0;i<startCh;i++){
          tdcData[(*data >> 12) & 0x7] = (*data & 0xfff) % 0xfff;
	  //          printf("i=%d, TDCdata = %x\n",i,*data);
          data++;
        }
      }
      //      printf("data = %x\n",*data);
      if((*data++ == 0x0000) && ((*data >> 15) & 0x1 == 1) && (data<end)) { // this is the ADC
	//      printf("data = %x\n",*data);
        startCh = (*data >> 12 & 0x7) + 1;         
	//        printf("ADC chan = %d\n",startCh);
        data++;   
        for(UInt_t i=0;i<startCh;i++){
          adcData[(*data >> 12) & 0x7] = (*data & 0xfff) % 0xfff;
	  //          printf("i=%d, ADCdata = %x\n",i,*data);
          data++;   
        }
      }
    }
  header->set(adcData,tdcData);    
  }
  return kTRUE;
}

ClassImp(HStartUnpacker)
ClassImp(HStartHeader)



