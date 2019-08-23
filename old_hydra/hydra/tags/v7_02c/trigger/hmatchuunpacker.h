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
#include "hmuscalers.h"
#include "hrichpad.h"
#include "hrichgeometrypar.h"
#include "hspecgeompar.h"
#include "hgeomvector.h"
#include "hgeomvector2.h"
#include "hgeomvolume.h"
#include "hshowergeometry.h"
#include "hmatchushower.h"

class HCategory;
class HTriggerParShowerMap;
class HTriggerParRichMap;

class HMatchUSubEvtHdr {
public:
  Int_t length;              //Length of the subevent
  Int_t bigEndianFlag;       //=0x1 distinguish little and big endian 
  Int_t muId;                //non-ambiguous Matching Unit ID. Set to 0x200
  Int_t triggerTag;          //triggerTag of event
  Int_t triggerCode;         //trigger code of subevent
};


typedef struct {
  Int_t dataw1;
  Int_t dataw2;
} EXPAND; 


class HMatchUUnpacker : public HldUnpack {
protected:
  static const Int_t kSubEvtId;
  HLocation fLoc;
  HCategory *fCatRich;    //Output category for Rich
  HCategory *fCatShower;  //Output category for Shower  
  HCategory *fCatTof;     //Output category for Tof
  HCategory *fCatLeptons;
  HCategory *fCatDiLeptons; 
  HCategory *fCatScalers; 

  HMUData* muData; 
  HMUScalers *muScalers; 

  TIterator *iterLepton;

  HShowerUnpackParam* fShowerParam;  //shower unpacker parameters 
  HTriggerParShowerMap *fTrigParSho;  //shower mapping parameters
  HTriggerParRichMap *fTrigParRich;  //rich mapping parameters


  HRecEvent* event; 
  Int_t fUnit;  //Conversion flag: 0-DEG 1-RAD
  FILE* fMU_logfile;  //log file
  Int_t fLog;  //log flag   
  Bool_t fDebugMode;  //global  debug mode
  Bool_t fRichDebugMode;  //rich spec. debug mode
  Bool_t fShowerDebugMode;  //shower spec. debug mode
  Int_t richc;  //count for rich empty content
  Bool_t isCompressed;  //flag for the new format
  Int_t version;        // switch different beamtimes

  Int_t hitpersec[6];
  Int_t nShower[6];
  Int_t nTof[6];
  Int_t sec;
  Int_t sec_old;

  UInt_t RichSecPatternMask;
  UInt_t ShowerSecPatternMask;
  UInt_t TofSecPatternMask;


  UInt_t MU_sec_pattern_Rich;
  UInt_t MU_sec_pattern_Shower;
  UInt_t MU_sec_pattern_Tof;

  Int_t mapNrTof[6][100];
  Int_t mapNrShower[6][100];
  Int_t mapNrRich[6][100];

  // new IPU format
  EXPAND expand;
  UInt_t *data;
  Int_t triggerCode;

  //Int_t Rich_Sector_Map[6];//={0,1,2,3,4,5}; // sep03 configuration
  UInt_t sec_count;

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
  UInt_t getRichSecHeader(UInt_t d) { return ( (d>>10) & 0x7); }

  // Shower decoding
  Int_t getShowColumn( Int_t d) { return (( d >> 16 ) & 0x1F); }
  Int_t getShowRowPattern( Int_t d) { return ( d & 0xFFFF); }
  Int_t getShowBuildID( Int_t  d) {return ( (d >> 16) &  0xfff ) ; }
  Bool_t isShowTrailer(UInt_t d) { return ( d ==0x0000A511)?kTRUE:kFALSE;}
  Int_t getShowFrameCount( Int_t d) { return ( d & 0x1f ); }
  Int_t storeShowElements(Int_t buildID, Int_t rowPattern, Int_t col); 
  HShowerSubIdPar* showerParamConfigure( HShowerUnpackParam* param, Int_t subEvtId);
 
 // tof decoding
  Int_t  getTofTriggerTag(UInt_t d){ return (d & 0x7f); }
  Int_t  getTofTriggerCode(UInt_t d){return ( (d>>8) & 0xf); }
  Int_t  getTofId(UInt_t d){ return ( (d>>13) & 0x1);  }
  Int_t  getTofVeto(UInt_t d){return ( (d>>12) & 0x1); }
  Int_t fillTofData(HMatchUTof *mtof, UInt_t dataWord);

  // Lepton decoding
  Int_t getLeptonSector(Int_t d) { return (d & 0xF);} 
  Int_t getLeptonRichNr(Int_t d) { return ( (d >> 4) & 0x7f ); } 
  Int_t getLeptonMetaNr(Int_t d) { return ( (d >> 12) & 0xff ); }  
  Int_t getLeptonDetBit(Int_t d) { return ( ( d >> 20) & 0x1 ); }   
  Int_t getLeptonFlag(Int_t d) { return ( ( d >> 21) & 0x1 ); }   
  Float_t getLeptonMom(Int_t d) { return ( (d >> 24) & 0xff);}  
 
  //Scalers decoding 
  Int_t getScaler(UInt_t d) {  return (d & 0xffffffff);}

  // Angle matching
  Int_t mapAngles(void);


  // new IPU format 
  void getExpanded( Int_t d) {
       Int_t data1 = d>>16 & 0xff; 
       Int_t data2 = d&0xff; 
       expand.dataw2=data1;  
       expand.dataw1=data2;
  }


  UInt_t*  expansion(UInt_t* d, Int_t longueur){
    if (data) delete[] data; 
    data = new UInt_t[2*longueur]; 
    memset(data,0,2*longueur*sizeof(UInt_t));
    Int_t j=0;
    for(Int_t i=0;i<longueur;i++){
      data[j]=   d[i] & 0xffff;
      data[j+1]=  d[i] >> 16 & 0xffff;
      j+=2; 
    }
    return data;
  }


public:
  Int_t evt;
  Int_t tofNr,tofLep;
  Int_t showerNr,showerLep;

  HMatchUUnpacker(Int_t unites=0,Bool_t compression=kFALSE, Int_t flog=0,TString vers="sep03");
  ~HMatchUUnpacker(void);
  Int_t getSubEvtId(void) const { //return kSubEvtId; }
    return 512;}
  Bool_t init(void); 
  Int_t execute(void);
  Bool_t isDebugMode(void) { return fDebugMode; }


  void setDebugMode(Bool_t dm) { fDebugMode = dm; }
  void setRichDebugMode(Bool_t dm) { fRichDebugMode = dm; }
  void setShowerDebugMode(Bool_t dm) { fShowerDebugMode = dm; }
  Int_t getRichEmptyCount(){ return richc+1; }
  
  ClassDef(HMatchUUnpacker,0)
};

#endif











