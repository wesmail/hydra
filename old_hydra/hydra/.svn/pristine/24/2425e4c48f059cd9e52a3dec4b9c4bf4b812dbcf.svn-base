#ifndef HMATCHUUNPACKER_H
#define HMATCHUUNPACKER_H

#include "hldunpack.h"
#include "hlocation.h"

class HCategory;

class HMatchUSubEvtHdr {
public:
  Int_t length;              //Length of the subevent
  Int_t bigEndianFlag;       //=0x1 distinguish little and big endian 
  Int_t muId;                //non-ambiguous Matching Unit ID. Set to 0x200
  Int_t triggerTag;          //triggerTag of event
  Int_t triggerCode;         //trigger code of subevent
};

class HMatchUUnpacker : public HldUnpack {
protected:
  static const Int_t kSubEvtId;
  HLocation fLoc;
  HCategory *fCatRich;   //Output category for Rich
  HCategory *fCatShower; //Output category for Shower  
  HCategory *fCatTof;    //Output category for Tof
  Bool_t fDebugMode;
  //the following methods return the number of 
  //words consumed by the decoder or -1 for error
  Int_t decodeRich(UInt_t *pData,Int_t maxLength); 
  Int_t decodeShower(UInt_t *pData,Int_t maxLength);
  Int_t decodeTof(UInt_t *pData,Int_t maxLength);
  //rich decoding
  
  Int_t getRichSize(UInt_t d) {return ((d >> 8) & 0x00ff);}
  Bool_t checkRichMSW(UInt_t d) {return ((d & 0x0F00) == 0)?kTRUE:kFALSE; }
  Bool_t checkRichLSW(UInt_t d) {return ((d & 0xE380) == 0)?kTRUE:kFALSE; }
  Int_t getRichFifoNb(UInt_t d) {return (d & 0xF000) >> 12; }
  Int_t getRichColumPattern(UInt_t d) { return (d & 0x00FF); }
  Int_t getRichSegId(UInt_t d) { return ((d >> 10 ) & 0x7); }
  Int_t getRichRow(UInt_t d) { return (d & 0x7F); }
  Int_t getShowColumn(UInt_t d) { return (d & 0x001F); }
  Int_t getShowRowPattern(UInt_t d) { return (d & 0xFFFF); }
  Bool_t isShowTrailer(UInt_t d) { 
    // return ((d & 0xFFFFFF00)==0x0000A5)?kTRUE:kFALSE;
       return ( d ==0x0000A511)?kTRUE:kFALSE;
  }  //FIXME
public:
  HMatchUUnpacker(void);
  ~HMatchUUnpacker(void);
  Int_t getSubEvtId(void) const { //return kSubEvtId; }
    return 512;}
  Bool_t init(void);
  Int_t execute(void);
  Bool_t isDebugMode(void) { return fDebugMode; }
  void setDebugMode(Bool_t dm) { fDebugMode = dm; }
  ClassDef(HMatchUUnpacker,0)
};

#endif


