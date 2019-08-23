// File: hstartunpacker.cc
// Last update: 8/12/99 r. holzmann
// Modified 12/99 by Denis Bertini <D.Bertini@gsi.de> 

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
        startCh = (*data >> 12 & 0x7) + 1;

        data++;   
        for(Int_t i=0;i<startCh;i++){
           adcData[(*data >> 12) & 0x7] = (*data & 0xfff) % 0xfff;
           data++;   
 	} 
      }
    
     // unpack for  the TDC1  part   
      if((*data++ == 0x2005) && ((*data >> 15) & 0x1 == 1)) { 
         startCh = (*data >> 12 & 0x7) + 1;
         data++;
         for(Int_t i=0;i<startCh;i++){
           tdcData[(*data >> 12) & 0x7] = (*data & 0xfff) % 0xfff;
           data++;
         }
      }

     // unpack for  the  TDC2 part   
      if((*data++ == 0x2006) && ((*data >> 15) & 0x1 == 1)) { 
         startCh = (*data >> 12 & 0x7) + 1;
         data++;
         for(Int_t i=0;i<startCh;i++){
           tdcData[((*data >> 12) & 0x7) +8] = (*data & 0xfff) % 0xfff;
           data++;
         }
      }


      /*       
     //unpack the scaler part
    UInt_t *sdata= (UInt_t*) data;   
    
    if((*sdata++ == 0x3801)) {
    sdata++; 
    for(UInt_t i=0;i<32;i++){
 	   scalData[i] = *sdata;
           sdata++;
         } 
    } 
     */
 
  } //while (data<end)
  end:
  header->set(adcData,tdcData,scalData);    
 }
  return kTRUE;
}

ClassImp(HStartUnpacker)
ClassImp(HStartHeader)



