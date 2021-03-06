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

Short_t HStartUnpacker::adcData[8];
Short_t HStartUnpacker::tdcData[16]; 
Int_t   HStartUnpacker::scalData[32];   


void  HStartHeader::clear ( Short_t* adc, Short_t* tdc,
                          Int_t* scal){ 
          for(Int_t i=0;i<8;i++) {*(adc+i)=-1*(i+1);}
          for(Int_t i=0;i<16;i++) {*(tdc+i)=-10*(i+1);}
          for(Int_t j=0;j<32;j++) {scal[j]=-100*(j+1);}  
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

UInt_t startCh=0;


 if (pSubEvt) {
    header->cleardata();
    Short_t* data =  (Short_t*) pSubEvt->getData();
    Short_t* end  =  (Short_t*) pSubEvt->getEnd(); 
   
    header->clear(adcData,tdcData,scalData);
    
    while (data < end){
     if(*data == 0x0) goto end;
       // unpack for the ADC's part
     if((*data++ == 0x1007) && ((*data >> 15) & 0x1 == 1) ) {
         //	printf("data_adc = %x\n",*data);
         startCh = (*data >> 12 & 0x7) + 1;
                  
 	if (startCh==1)	printf("ADC chan = %d\n",startCh);
         data++;   
         for(Int_t i=0;i<startCh;i++){
           adcData[(*data >> 12) & 0x7] = (*data & 0xfff) % 0xfff;
 	if(startCh==1) 	printf("i=%d, ADCdata = %x \n",i,((*data & 0xfff) % 0xfff));
           data++;   
 	} 
      }
    
     // unpack for  the TDC1  part   
      if((*data++ == 0x2005) && ((*data >> 15) & 0x1 == 1)) { 
        //printf("datatdc = %x\n",*data);
         startCh = (*data >> 12 & 0x7) + 1;
  	 //printf("TDC chan = %d\n",startCh);
         data++;
         for(Int_t i=0;i<startCh;i++){
 	   // printf(" channel touched:%i  time:%i \n",(*data >> 12) & 0x7
	   //  ,(*data & 0xfff) % 0xfff); 
           tdcData[(*data >> 12) & 0x7] = (*data & 0xfff) % 0xfff;
	   // printf("i=%d, TDC1 data = %x\n",i,*data);
           data++;
         }
      }

     // unpack for  the  TDC2 part   
      if((*data++ == 0x2006) && ((*data >> 15) & 0x1 == 1)) { 
        //printf("tofino tdc = %x\n",*data);
         startCh = (*data >> 12 & 0x7) + 1;
	 // printf("Tofino TDC chan = %d\n",startCh);
         data++;
         for(Int_t i=0;i<startCh;i++){
	   //   printf(" channel touched:%i  time:%i \n",(*data >> 12) & 0x7
	   //   ,(*data & 0xfff) % 0xfff); 
           tdcData[((*data >> 12) & 0x7) +8] = (*data & 0xfff) % 0xfff;
	   //  printf("i=%d, TDC2 data = %x\n",i,*data);
           data++;
         }
      }


      /*       
     //unpack the scaler part
    UInt_t *sdata= (UInt_t*) data;   
    //   printf("scaler %x \n",*sdata);
    
    if((*sdata++ == 0x3801)) {
    sdata++; 
    for(UInt_t i=0;i<32;i++){
 	   scalData[i] = *sdata;
	   //  printf("i=%d, scaldata = %x\n",i,*sdata);
           sdata++;
         } 
    } 
     */
 
  } //while (data<end)
  end:
    /////     for (Int_t i=0;i<8;i++) printf(" adcData %i \n", adcData[i]);
  header->set(adcData,tdcData,scalData);    
 }
  return kTRUE;
}

ClassImp(HStartUnpacker)
ClassImp(HStartHeader)



