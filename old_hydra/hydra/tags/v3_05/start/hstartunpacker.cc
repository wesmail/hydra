// File: hstartunpacker.cc
// updated 10/12/98 d.b 
// Last update: 14/12/98 r. holzmann
// Modified 06/08/99 by Denis Bertini <D.Bertini@gsi.de> 

#include "hstartunpacker.h"
#include "hldsubevt.h"
#include "hades.h"
#include "hrecevent.h"
#include "hpartialevent.h"
#include "hdebug.h"
#include <iostream.h>

UShort_t HStartUnpacker::adcData[8];
UShort_t HStartUnpacker::tdcData[8]; 
UInt_t   HStartUnpacker::scalData[10];   


void  HStartHeader::clear ( UShort_t* adc, UShort_t* tdc,UInt_t* scal){ 
          for(Int_t i=0;i<8;i++) {*(adc+i)=0;*(tdc+i)=0;}
          for(Int_t j=0;j<10;j++) {scal[j]=0;}  
 }


Bool_t HStartUnpacker::init(void) {
  HRecEvent *event=0;
  HPartialEvent *par=0;

  event=(HRecEvent *)gHades->getCurrentEvent();
  if (!event) return kFALSE;
  par=event->getPartialEvent(catStart);
  if (!par) {
    event->addPartialEvent(catStart,"Start","The Start subevent");
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


 if (pSubEvt) {
    header->cleardata();
    UShort_t* data =  (UShort_t*) pSubEvt->getData();
    UShort_t* end  =  (UShort_t*) pSubEvt->getEnd(); 
   
    header->clear(adcData,tdcData,scalData);
    
    while (data < end){
     if(*data == 0x0) goto end;
       // unpack for the ADC's part
     if((*data++ == 0x1007) && ((*data >> 15) & 0x1 == 1) ) {
         //	printf("data_adc = %x\n",*data);
         startCh = (*data >> 12 & 0x7) + 1;         
 	//	printf("ADC chan = %d\n",startCh);
         data++;   
         for(UInt_t i=0;i<startCh;i++){
           adcData[(*data >> 12) & 0x7] = (*data & 0xfff) % 0xfff;
 	  //	printf("i=%d, ADCdata = %x\n",i,*data);
           data++;   
 	} 
      }
    
     // unpack for  the TDC's part   
      if((*data++ == 0x2005) && ((*data >> 15) & 0x1 == 1)) { 
        //printf("datatdc = %x\n",*data);
         startCh = (*data >> 12 & 0x7) + 1;
  	 //printf("TDC chan = %d\n",startCh);
         data++;
         for(UInt_t i=0;i<startCh;i++){
 	   // printf(" channel touched:%i  time:%i \n",(*data >> 12) & 0x7
	   //  ,(*data & 0xfff) % 0xfff); 
           tdcData[(*data >> 12) & 0x7] = (*data & 0xfff) % 0xfff;
	   // printf("i=%d, TDCdata = %x\n",i,*data);
           data++;
         }
      }
       
     //unpack the scaler part
    UInt_t *sdata= (UInt_t*) data;   
    //   printf("scaler %x \n",*sdata);
    
    if((*sdata++ == 0x3801)) {
    sdata++; 
    for(UInt_t i=0;i<10;i++){
 	   scalData[i] = *sdata;
	   //  printf("i=%d, scaldata = %x\n",i,*sdata);
           sdata++;
         } 
    } 
  
 
  } //while (data<end)
  end:
  header->set(adcData,tdcData,scalData);    
 }
  return kTRUE;
}

ClassImp(HStartUnpacker)
ClassImp(HStartHeader)



