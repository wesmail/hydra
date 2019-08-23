// File: hstartunpacker.h
//
// Author: D.B
// Last update: 8/12/99
//

#ifndef HSTARTUNPACKER_H
#define HSTARTUNPACKER_H

#include "hldunpack.h"

class HStartHeader : public TObject {
public:
    Short_t adc1, adc2, adc3, adc4, adc5, adc6, adc7, adc8;
    Short_t tdc1, tdc2, tdc3, tdc4, tdc5, tdc6, tdc7, tdc8;
    Short_t tofdet1, tofdet2, tofdet3, tofsmall, vtdc1, vtdc2, vtdc3, vtdc4;
    //UInt_t  scal1, scal2, scal3, scal4, scal5;
    //UInt_t  scal6, scal7, scal8, scal9, scal10;
    Int_t scaler[32];   

public:
    HStartHeader(void) {}
    ~HStartHeader(void) {}
    void  set(Short_t* adc, Short_t* tdc, Int_t* scal){
// channel number
    this->adc1= *adc; this->adc2= *(adc+1); this->adc3= *(adc+2); 
  
    this->adc4= *(adc+3); this->adc5= *(adc+4); this->adc6= *(adc+5); 
    this->adc7= *(adc+6); this->adc8= *(adc+7);   
// TDC1 data
    tdc1= *tdc;this->tdc2= *(tdc+1); this->tdc3= *(tdc+2); 
    this->tdc4= *(tdc+3); this->tdc5= *(tdc+4); this->tdc6= *(tdc+5); 
    this->tdc7= *(tdc+6); this->tdc8= *(tdc+7);  
//  TDC2 data
    tofdet1= *(tdc+8);
    tofdet2= *(tdc+9);
    tofdet3= *(tdc+10); 
    tofsmall= *(tdc+11);
    vtdc1= *(tdc+12); vtdc2= *(tdc+13); 
    vtdc3= *(tdc+14); vtdc4= *(tdc+15);  


// scaler data 
   for(Int_t i=0;i<32;i++) scaler[i] = scal[i];   
 //scal1=scal[0];scal2=scal[1];scal3=scal[2];scal4=scal[3];scal5=scal[4];
 //scal6=scal[5];scal7=scal[6];scal8=scal[7];scal9=scal[8];scal10=scal[9]; 
   }
   void  clear ( Short_t* adc, Short_t* tdc, Int_t* scal);
   void cleardata(){
  
    // channel adc    
    adc1= -1; adc2= -2; adc3= -3; adc4= -4; 
    adc5= -5; adc6= -6; adc7= -7; adc8= -8;   
  
    // channel tdc
    tdc1= -10; tdc2= -11; tdc3= -13; tdc4= -14; 
    tdc5= -15; tdc6= -16; tdc7= -17; tdc8= -18;   
  
    // tofino tdc
    tofdet1= -21;
    tofdet2= -22;
    tofdet3= -23;
    tofsmall= -24;
    vtdc1= -31; vtdc2= -32; 
    vtdc3= -33; vtdc4= -34;  

    // scalers 
    for(Int_t i=0;i<32;i++) scaler[i] = 100+i;   
    // scal1=0;scal2=0;scal3=0;scal4=0;scal5=0;
    // scal6=0;scal7=0;scal8=0;scal9=0;scal10=0;
    } 
   

    ClassDef(HStartHeader,1)  // START detector partial event header
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
  Int_t getData(const Short_t data) const { return data & 0xfff; }

protected:
    Int_t subEvtId;
    HStartHeader* header;
public:
    static Short_t adcData[8];//!
    static Short_t tdcData[16];//!
    static Int_t   scalData[32];//!
 
    ClassDef(HStartUnpacker, 1)	// unpack START data 12/99
};

#endif /* ! HSTARTUNPACKER_H */



