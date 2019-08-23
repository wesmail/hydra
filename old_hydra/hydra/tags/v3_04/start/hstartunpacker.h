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
    UInt_t  scal1, scal2, scal3, scal4, scal5;
    UInt_t  scal6, scal7, scal8, scal9, scal10;
       

public:
    HStartHeader(void) {}
    ~HStartHeader(void) {}
   void  set(UShort_t* adc, UShort_t* tdc, UInt_t* scal){
// channel number
    this->adc1= *adc;this->adc2= *(adc+1);this->adc3= *(adc+2); 
    this->adc4= *(adc+3);this->adc5= *(adc+4);this->adc6= *(adc+5); 
    this->adc7= *(adc+6);this->adc8= *(adc+7);   
// channel data
    tdc1= *tdc;this->tdc2= *(tdc+1);this->tdc3= *(tdc+2); 
    this->tdc4= *(tdc+3);this->tdc5= *(tdc+4);this->tdc6= *(tdc+5); 
    this->tdc7= *(tdc+6);this->tdc8= *(tdc+7);  
// scaler data 
    scal1=scal[0];scal2=scal[1];scal3=scal[2];scal4=scal[3];scal5=scal[4];
    scal6=scal[5];scal7=scal[6];scal8=scal[7];scal9=scal[8];scal10=scal[9];       }
   void  clear ( UShort_t* adc, UShort_t* tdc, UInt_t* scal);
   void cleardata(){
   // channel adc    
    this->adc1= 0;this->adc2= 0;this->adc3= 0;this->adc4= 0; 
    this->adc5= 0;this->adc6= 0;this->adc7= 0;this->adc8= 0;   
    // channel tdc
    this->tdc1= 0;this->tdc2= 0;this->tdc3= 0;this->tdc4= 0; 
    this->tdc5= 0;this->tdc6= 0;this->tdc7= 0;this->tdc8= 0;   
    // scalers 
    scal1=0;scal2=0;scal3=0;scal4=0;scal5=0;
    scal6=0;scal7=0;scal8=0;scal9=0;scal10=0;
    } 
   

    ClassDef(HStartHeader,0)  // START detector partial event header
};

class HStartUnpacker: public HldUnpack {
friend class  HStartMonitor;

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
public:
    static UShort_t adcData[8];//!
    static UShort_t tdcData[8];//!
    static UInt_t   scalData[10];//!
 
    ClassDef(HStartUnpacker, 1)	// unpack START data 12/98
};

#endif /* ! HSTARTUNPACKER_H */



