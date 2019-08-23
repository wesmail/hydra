#ifndef HMATCHUUNPACKER_H
#define HMATCHUUNPACKER_H

#include "hldunpack.h"
#include "hlocation.h"
#include "hmatchutof.h"
#include "hmuleptons.h"
#include "hshowerunpackparam.h"
#include "hshowersubidpar.h"
#include "hrecevent.h"
#include "hmudata.h"

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
  HCategory *fCatLeptons; 
  HCategory *fCatDiLeptons; 
  HMUData* muData; 

  
  HShowerUnpackParam* fShowerParam; // shower parameter container
  HRecEvent* event;
  Int_t fUnit; // Conversion flag: 0-DEG 1-RAD
  FILE* fMU_logfile; // log file
  Int_t fLog; //log flag   
  Bool_t fDebugMode; // global  debug mode
// Bool_t fTriggerCodeFirstFormat; // flag for format of shower data (old or new)
  Bool_t fRichDebugMode; // rich spec. debug mode
  Bool_t fShowerDebugMode; // shower spec. debug mode
  Int_t richc; // count for rich empty content

  //the following methods return the number of 
  //words consumed by the decoder or -1 for error
  Int_t decodeMUData(UInt_t *pData,Int_t maxLength);
  Int_t decodeRich(UInt_t *pData,Int_t maxLength);
  Int_t decodeIpuRich(UInt_t *pData,Int_t maxLength);
  Int_t decodeIpuShower(UInt_t *pData,Int_t maxLength);
  Int_t decodeShower(UInt_t *pData,Int_t maxLength);
  Int_t decodeShowerOld(UInt_t *pData,Int_t maxLength);
  Int_t decodeTof(UInt_t *pData,Int_t maxLength);
  Int_t decodeIpuTof(UInt_t *pData,Int_t maxLength); 
  
  //rich decoding
  Int_t getRichSize(UInt_t d) {return ((d >> 8) & 0x00ff);}
  Bool_t checkRichMSW(UInt_t d) {return ((d & 0x0F00) == 0)?kTRUE:kFALSE; }
  Bool_t checkRichLSW(UInt_t d) {return ((d & 0xE380) == 0)?kTRUE:kFALSE; }
  Int_t getRichFifoNb(UInt_t d) {return (d & 0xF00) >> 8; }
  Int_t getRichColumPattern(UInt_t d) { return (d & 0x00FF); }
  Int_t getRichSegId(UInt_t d) { return ((d >> 10 ) & 0x7); }
  Int_t getRichRow(UInt_t d) { return (d & 0x7F); }
  
  // Shower decoding
  Int_t getShowColumn( Int_t d) { return (( d >> 16 ) & 0x1F); }
  Int_t getShowRowPattern( Int_t d) { return ( d & 0x7FFF); }
  Int_t getShowBuildID( Int_t  d) {return ( (d >> 16) &  0xfff ) ; }
  Bool_t isShowTrailer(UInt_t d) { return ( d ==0x0000A511)?kTRUE:kFALSE;}
  Int_t getShowFrameCount( Int_t d) { return ( d & 0xf ); }
  Int_t storeShowElements(Int_t buildID, Int_t rowPattern, Int_t col); 
  HShowerSubIdPar* showerParamConfigure( HShowerUnpackParam* param, Int_t subEvtId);
  // tof decoding
  Int_t  getTofTriggerTag(UInt_t d){ return (d & 0x7f); }
  Int_t  getTofTriggerCode(UInt_t d){return ( (d>>8) & 0xf); }
  Int_t  getTofId(UInt_t d){ return ( (d>>13) & 0x1);  }
  Int_t  getTofVeto(UInt_t d){return ( (d>>12) & 0x1); }
  Bool_t fillTofData(HMatchUTof *mtof, UInt_t dataWord);

  Float_t getLeptonTheta(Int_t d) { return (d & 0xFF);} 
  Float_t getLeptonPhi(Int_t d) { return ( (d >> 8) & 0x7ff ); }  
  Float_t getLeptonFlag(Int_t d) { return ( ( d >> 20) & 0x1 ); }   
  Float_t getLeptonMom(Int_t d) { return ( (d >> 24) & 0xFF);}  


  

public:
  HMatchUUnpacker(Int_t unites=0,Int_t flog=0);
  ~HMatchUUnpacker(void);
  Int_t getSubEvtId(void) const { //return kSubEvtId; }
    return 512;}
  Bool_t init(void);
  Int_t execute(void);
  Bool_t isDebugMode(void) { return fDebugMode; }
 // Bool_t isTriggerCodeFirstFormat(void) { return fTriggerCodeFirstFormat; }
 // void setfTriggerCodeFirstFormat(Bool_t tcff) { fTriggerCodeFirstFormat = tcff; }
  void setDebugMode(Bool_t dm) { fDebugMode = dm; }
  void setRichDebugMode(Bool_t dm) { fRichDebugMode = dm; }
  void setShowerDebugMode(Bool_t dm) { fShowerDebugMode = dm; }
  Int_t getRichEmptyCount(){ return richc+1; }
  
  ClassDef(HMatchUUnpacker,0)
};

#endif


