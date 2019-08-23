// File: hstartunpacker.h
//
// Author: D.B
// Last update: 98/12/13 17:43:23
//

#ifndef HSTARTUNPACKER_H
#define HSTARTUNPACKER_H

#include "hldunpack.h"

class HStartHeader : public TObject {
public:
    UShort_t adc1, adc2, adc3, adc4, adc5, adc6, adc7, adc8;
    UShort_t tdc1, tdc2, tdc3, tdc4, tdc5, tdc6, tdc7, tdc8;
public:
    HStartHeader(void) {}
    ~HStartHeader(void) {}
   void  set(UShort_t* adc, UShort_t* tdc){
 // channel number    
    this->adc1= *adc;this->adc2= *(adc+1);this->adc3= *(adc+2); 
    this->adc4= *(adc+3);this->adc5= *(adc+4);this->adc6= *(adc+5); 
    this->adc7= *(adc+6);this->adc8= *(adc+7);   
// channel data
    this->tdc1= *tdc;this->tdc2= *(tdc+1);this->tdc3= *(tdc+2); 
    this->tdc4= *(tdc+3);this->tdc5= *(tdc+4);this->tdc6= *(tdc+5); 
    this->tdc7= *(tdc+6);this->tdc8= *(tdc+7);   
   }
   void  clear ( UShort_t* adc, UShort_t* tdc){ 
     for(Int_t i=0;i<8;i++) {*(adc+i)=0;*(tdc+i)=0;}
   }
   void cleardata(){
   // channel adc    
    this->adc1= 0;this->adc2= 0;this->adc3= 0;this->adc4= 0; 
    this->adc5= 0;this->adc6= 0;this->adc7= 0;this->adc8= 0;   
    // channel tdc
    this->tdc1= 0;this->tdc2= 0;this->tdc3= 0;this->tdc4= 0; 
    this->tdc5= 0;this->tdc6= 0;this->tdc7= 0;this->tdc8= 0;   
   } 

    ClassDef(HStartHeader,0)  // START detector partial event header
};

class HStartUnpacker: public HldUnpack {
public:
  //HStartUnpacker(void) {header=0;}
  HStartUnpacker(Int_t id = 501) { subEvtId = id; }
  HStartUnpacker(const HStartUnpacker& start) { subEvtId = start.subEvtId; }
  ~HStartUnpacker(void) {  }
  Int_t getSubEvtId() const { return subEvtId; }
  
  Bool_t init(void);
  Int_t execute(void);

  //  decoding of CAEN ADCs and TDCs
  Int_t getData(const UShort_t data) const { return data & 0xfff; }

protected:
    Int_t subEvtId;
    HStartHeader* header;

    ClassDef(HStartUnpacker, 1)	// unpack START data 12/98
};

#endif /* ! HSTARTUNPACKER_H */



