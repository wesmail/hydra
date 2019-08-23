// File: $RCSfile: hmdcdataword.h,v $
//
// Former Author: Walter Karig <W.Karig@gsi.de>
// Meanwhile Peter Zumbruch <P.Zumbruch@gsi.de>
//
// Version: $Revision: 1.16 $
// Modified by $Author: zumbruch $ on $Date: 2006-05-12 16:42:17 $
///////////////////////////////////////////////////////////////////////////////////////////
#ifndef HMDCDATAWORD_H
#define HMDCDATAWORD_H

#include "hmdcdef.h"

class HMdcDataWord : public TObject {

 private:
  enum { m1=0x1, m2=0x3, m3=0x7, m4=0xf, m5=0x1f, m11=0x7ff, m16=0xffff, m31=0x7fffffff}; //! mask used for decoding

  //sub-header
  Int_t evNb;             // event number
  Int_t rocNb;            // roc number
  Int_t trigType;         // trigger type
  Int_t errorFlag;        // error flag
  Int_t subevtSize;       // subevent size
 
  //Mdc data-word
  Int_t sam;              // sam number
  Int_t samP;             // sam position (x/y)
  Int_t roc;              // roc number
  Int_t rocP;             // roc position (LVL1 port 0/1)
  Int_t mboNb;            // mbo position
  Int_t tdcNb;            // tdc number
  Int_t chan;             // channel number
  Int_t hit;              // hit number (modulo 2)
  Int_t time;             // time
    
  //Adress
  Int_t sector;           // MdcRaw container coordinate: sector
  Int_t module;           // MdcRaw container coordinate: module
  Int_t mbo;              // MdcRaw container coordinate: mbo
  Int_t tdc;              // MdcRaw container coordinate: tdc (tdc*8 + channel)

  Int_t entry;            // Number of entry
  static Bool_t  noComment; //! switch all prints of;

public:
  HMdcDataWord();
  ~HMdcDataWord(){;};

  Bool_t checkConsistency(UInt_t version=0, UInt_t data=0);
  Bool_t checkConsistencyRocHeader(UInt_t version=0);

  void clear(){sam=samP=roc=rocP=mboNb=tdcNb=chan=hit=time=-1;}
  void clearAll(){clear();clearHeader();clearAddress();};
  void clearAddress(){sector=module=mbo=tdc=-1;};
  void clearHeader(){evNb=subevtSize=trigType=rocNb=errorFlag=-1;};
    
  Bool_t decode(const UInt_t data, UInt_t version, Bool_t consisCheck=kFALSE);
  Bool_t decode(const UInt_t data, Bool_t consisCheck=kFALSE)
    {
      return decode(data, 0, consisCheck);
    };
  void dump(UInt_t version=1);
    
  // selectors sub-header
  Int_t getEvNb(){ return evNb;}; 
  Int_t getTrigType(){ return trigType;};
  Int_t getErrorFlag(){ return errorFlag;};
  Int_t getSizeStatus(){ return errorFlag;}; // obsolete function
  Int_t getSubEvtSize(){ return subevtSize;};
  Int_t getRocNb(){ return rocNb;};
  
  // selectors data-word
  Int_t getSam(){ return sam;};
  Int_t getSamP(){ return samP;};
  Int_t getRoc(){ return roc;};
  Int_t getRocP(){ return rocP;};
  Int_t getBus(){ return mboNb;};
  Int_t getMbo(){ return mboNb;};
  Int_t getMboNb(){ return mboNb;};
  Int_t getTdc(){ return tdcNb;};
  Int_t getTdcNb(){ return tdcNb;};
  Int_t getChan(){ return chan;};
  Int_t getHit(){ return hit;};
  Int_t getTime(){ return time;};
  Int_t getVirtualSamNr() {return ((sam*2 + samP) -1);}; 

  UInt_t getCodedDataWord( UInt_t version = 1);
  Bool_t getQuietMode(){return noComment;};

  void print(UInt_t version);
  void print(void){print(1);};

  void setAdress(Int_t s, Int_t m, Int_t mb, Int_t t, Int_t e){setAddress(s,m,mb,t,e);};
  void setAddress(Int_t s, Int_t m, Int_t mb, Int_t t, Int_t e){sector=s;module=m;mbo=mb;tdc=t;entry=e;};
  // set selectors sub-header
  void setEvNb(Int_t i){ evNb = i;}; 
  void setTrigType(Int_t i){ trigType = i;};
  void setErrorFlag(Int_t i){ errorFlag = i;};
  void setSizeStatus(Int_t i){ errorFlag = i;}; // obsolete function
  void setSubEvtSize(Int_t i){ subevtSize = i;};
  void setRocNb(Int_t i){ rocNb = i;};

  // set selectors data-word
  void setSam(Int_t i){ sam = i;};
  void setSamP(Int_t i){ samP = i;};
  void setRoc(Int_t i){ roc = i;};
  void setRocP(Int_t i){ rocP = i;};
  void setBus(Int_t i){ mboNb = i;};
  void setMbo(Int_t i){ mboNb = i;};
  void setMboNb(Int_t i){ mboNb = i;};
  void setTdc(Int_t i){ tdcNb = i;};
  void setTdcNb(Int_t i){ tdcNb = i;};
  void setChan(Int_t i){ chan = i;};
  void setHit(Int_t i){ hit = i;};
  void setTime(Int_t i){ time = i;};

  void setQuietMode(Bool_t b=kTRUE){noComment=b;};

  Bool_t subHeader(const UInt_t* data, UInt_t version, Bool_t consisCheck=kFALSE);
  Bool_t subHeader(const UInt_t* data, Bool_t consisCheck=kFALSE)
  {
      return subHeader(data, 0, consisCheck);
  };

  ClassDef(HMdcDataWord, 2)	// a mdc data word  
};

#endif /* ! HMDCDATAWORD */
