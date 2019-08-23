// File: hmdcunpacker.h
//
// Author: Walter Karig <W.Karig@gsi.de>
// Modified by D.B
// Last update: 98/12/12 18:36:14
//

#ifndef HMDCUNPACKER_H
#define HMDCUNPACKER_H

#include "hldunpack.h"

class HMdcUnpacker: public HldUnpack {
protected:
   Int_t  subEvtId;
public:
    // Range for MDC subevents is 200 - 299
    HMdcUnpacker(Int_t id = 1) { subEvtId = id;}
    HMdcUnpacker(const HMdcUnpacker &mdc) { subEvtId = mdc.subEvtId; }
    Int_t getSubEvtId() const { return subEvtId; }

    Bool_t init();
    Int_t execute();


// decoding of 32 bit data word
// using T. Specif. of Hamot control logic
//   bits       sz      mask        word-name             valid range      
// 24 - 31       8       -           not used                -   
// 21 - 23       3      0x7          HArOc number          (1-7)
// 19 - 20       2      0x3          MBo nb. at LV1 bus    (1,2,3)
// 15 - 18       4      0xf          TDC chip nb (MB64)    (1-8) 
// 11 - 14       4      0xf          TDC register nb       (nw?)  
//  0 - 10      11      0x7ff        TDC data (time)       (0-2047)

    Int_t getTime(const UInt_t data) const { return data & 0x7ff;}
    Int_t getChannel(const UInt_t data) const { return data>>12 & 0xf;}
    Int_t getChip(const UInt_t data) const { return data>>16 & 0xf;}
    Int_t getRocPos(const UInt_t data) const { return (data>>20 & 0x3) - 1;}
    Int_t getRoc(const UInt_t data) const { return (data>>22 & 0x7) - 1;}

    Int_t getMbo(const Int_t roc, const Int_t pos) const;
    Int_t getTdc(const Int_t chip, const Int_t chan) const{
	return 8*chip + chan;
    }

protected:
    Int_t fill();
    void print();
public:
    ClassDef(HMdcUnpacker,1)	// unpack MDC data 12/98
};

#endif /* ! HMDCUNPACKER_H */




