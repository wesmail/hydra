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

 public:
  HMdcDataWord(){clearAll();};
  ~HMdcDataWord(){;};

  Bool_t checkConsistency(UInt_t version=0);
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
  void dump(); 
    
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

  void print(UInt_t version);
  void print(void){print(0);};

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

  Bool_t subHeader(const UInt_t* data, UInt_t version, Bool_t consisCheck=kFALSE);
  Bool_t subHeader(const UInt_t* data, Bool_t consisCheck=kFALSE)
    {
      return subHeader(data, 0, consisCheck);
    };

  ClassDef(HMdcDataWord, 2)	// a mdc data word  
    };


    inline Bool_t HMdcDataWord::checkConsistency(UInt_t version)
{
  if (version==1) version--; // checks for version 1 same as version 0 
  // checks for the consistency of the data word
  switch(version)
    {
    case 0:
      {
	//             sam in [ 0,   31]
	//    sam position in [ 0,    1]
	//             roc in [ 1,    7]
	//    roc position in [ 0,    1]
	//           mboNb in [ 1,    3]
	//           tdcNb in [ 1,   12]
	//        channels in [ 0,    7]
	// hits (module 2) in [ 0,    1]
	//            time in [ 0,0x7ff]
	
	Int_t samMin = 0;
	Int_t samMax = 31;
	Int_t samPMin = 0;
	Int_t samPMax = 1;
	Int_t rocMin = 1;
	Int_t rocMax = 5; // 5
	Int_t rocPMin = 0;
	Int_t rocPMax = 1;
	Int_t mboNbMin = 1;
	Int_t mboNbMax = 3;
	Int_t tdcNbMin = 1;
	Int_t tdcNbMax = 12;
	tdcNbMin --; // adaption to 
	tdcNbMax --; // some communication problems
	Int_t chanMin = 0;
	Int_t chanMax = 7;
	Int_t hitMin = 0;
	Int_t hitMax = 1;
	Int_t timeMin = 0;
	Int_t timeMax = 0x7ff;
	
	if (sam < samMin || sam > samMax ) 
	  {
	    Error("checkConsistency()","%s = %i out of range [%i,%i]", "SAM number", sam, samMin, samMax );
	    return kFALSE; 
	  }
	if (samP < samPMin || samP > samPMax ) 
	  {
	    Error("checkConsistency()","%s = %i out of range [%i,%i]", "SAM Position", samP, samPMin, samPMax );
	    return kFALSE; 
	  }
	if (roc < rocMin || roc > rocMax ) 
	  {
	    Error("checkConsistency()","%s = %i out of range [%i,%i]", "ROC Number", roc, rocMin, rocMax );
	    return kFALSE; 
	  }
	if (rocP < rocPMin || rocP > rocPMax ) 
	  {
	    Error("checkConsistency()","%s = %i out of range [%i,%i]", "LVL1 Slot", rocP, rocPMin, rocPMax );
	    return kFALSE; 
	  }
	if (mboNb < mboNbMin || mboNb > mboNbMax ) 
	  {
	    Error("checkConsistency()","%s = %i out of range [%i,%i]", "MBO Position", mboNb, mboNbMin, mboNbMax );
	    return kFALSE; 
	  }
	if (tdcNb < tdcNbMin || tdcNb > tdcNbMax ) 
	  {
	    Error("checkConsistency()","%s = %i out of range [%i,%i]", "TDC Number", tdcNb, tdcNbMin, tdcNbMax );
	    return kFALSE; 
	  }
	if (chan < chanMin || chan > chanMax ) 
	  {
	    Error("checkConsistency()","%s = %i out of range [%i,%i]", "Channel Number", chan, chanMin, chanMax );
	    return kFALSE; 
	  }
	if (hit < hitMin || hit > hitMax ) 
	  {
	    Error("checkConsistency()","%s = %i out of range [%i,%i]", "Hit Number (modulo 2)", hit, hitMin, hitMax );
	    return kFALSE; 
	  }
	if (time < timeMin || time > timeMax ) 
	  {
	    Error("checkConsistency()","%s = %i out of range [%i,%x]", "Time", time, timeMin, timeMax );
	    return kFALSE; 
	  }
      }
      break;
    default:
      Error("checkConsistency()","decoding version %i not valid",version);
      return kFALSE;
    }
  return kTRUE;
};

inline Bool_t HMdcDataWord::checkConsistencyRocHeader(UInt_t version)
{
  // checks for the consistency of the roc header
  switch(version)
    {
    case 0:
      {
	//  event number in [ 0,0xffff]
	// subevent size in [ 0,0xffff]
	//  trigger type in [ 1,0xf]
	//    roc number in [ 0,0xf]
	//     errorFlag in [ 0,0xf]
      
	Int_t evNbMin = 0;
	Int_t evNbMax = 0xffff;
	Int_t subevtSizeMin = 0;
	Int_t subevtSizeMax = 0xffff;
	Int_t trigTypeMin = 0;
	Int_t trigTypeMax = 0xf;
	Int_t rocNbMin = 1;
	Int_t rocNbMax = 0xf;
	Int_t errorFlagMin = 0;
	Int_t errorFlagMax = 0xf;
      
	if (evNb < evNbMin || evNb > evNbMax ) 
	  {
	    Error("checkConsistencyRocHeader()","%s = %i out of range [%i,%x]", "Event Number number", evNb, evNbMin, evNbMax );
	    return kFALSE; 
	  }
	if (subevtSize < subevtSizeMin || subevtSize > subevtSizeMax ) 
	  {
	    Error("checkConsistencyRocHeader()","%s = %i out of range [%i,%x]", "Subevent size number", subevtSize, subevtSizeMin, subevtSizeMax );
	    return kFALSE; 
	  }
	if (trigType < trigTypeMin || trigType > trigTypeMax ) 
	  {
	    Error("checkConsistencyRocHeader()","%s = %i out of range [%i,%x]", "Trigger Type", trigType, trigTypeMin, trigTypeMax );
	    return kFALSE; 
	  }
	if (rocNb < rocNbMin || rocNb > rocNbMax ) 
	  {
	    Error("checkConsistencyRocHeader()","%s = %i out of range [%i,%i]", "ROC Number", rocNb, rocNbMin, rocNbMax );
	    return kFALSE; 
	  }
	if (errorFlag < errorFlagMin || errorFlag > errorFlagMax ) 
	  {
	    Error("checkConsistencyRocHeader()","%s = %i out of range [%i,%x]", "Error Flag", errorFlag, errorFlagMin, errorFlagMax );
	    return kFALSE; 
	  }
      }
      break;
    default:
      Error("checkConsistency()","decoding version %i not valid",version);
      return kFALSE;
    }
  return kTRUE;
};

inline void HMdcDataWord::print(UInt_t version)
{
  switch(version)
    {
    case 0:
      printf("\t%s: ",ClassName());
      printf("\n\t>>>>> DataWord   : ");    
      printf("%s: %2i | ","sam" ,sam);    
      printf("%s: %2i | ","samP",samP);   
      printf("%s: %2i | ","roc" ,roc);    
      printf("%s: %2i | ","rocP",rocP);   
      printf("%s: %2i | ","mboNb" ,mboNb);    
      printf("%s: %2i | ","tdcNb" ,tdcNb);    
      printf("%s: %2i | ","chan",chan);   
      printf("%s: %2i | ","hit" ,hit);    
      printf("%s: %4i   ","time",time);   
      printf("\n\t  >>> ROC-Header : ");    
      printf("%s: %4i | ","event number" ,evNb);      
      printf("%s: %2i | ","roc number"   ,rocNb);     
      printf("%s: %2i | ","trigger type" ,trigType);  
      printf("%s: %2i | ","error flag"   ,errorFlag); 
      printf("%s: %4i   ","subevent size",subevtSize);
      printf("\n\t  >>> RAW-Address: ");    
      printf("%s: %2i | ","sector" ,sector);
      printf("%s: %2i | ","module" ,module);   
      printf("%s: %2i | ","mbo"    ,mbo);
      printf("%s: %2i | ","tdc"    ,tdc);
      printf("%s: %2i   ","entry# ",entry);
      printf("\n");
      break;
    case 1:
      printf("\t%s: ",ClassName());
      printf("\n\t>>>>> DataWord   : ");    
      printf("%s: %2i | ","virtualSamNr" ,getVirtualSamNr());    
      printf("%s: %2i | ","sam" ,sam);    
      printf("%s: %2i | ","samP",samP);   
      printf("%s: %2i | ","roc" ,roc);    
      printf("%s: %2i | ","rocP",rocP);   
      printf("%s: %2i | ","mboNb" ,mboNb);    
      printf("%s: %2i | ","tdcNb" ,tdcNb);    
      printf("%s: %2i | ","chan",chan);   
      printf("%s: %2i | ","hit" ,hit);    
      printf("%s: %4i   ","time",time);   
      printf("\n\t  >>> ROC-Header : ");    
      printf("%s: %4i | ","event number" ,evNb);      
      printf("%s: %2i | ","roc number"   ,rocNb);     
      printf("%s: %2i | ","trigger type" ,trigType);  
      printf("%s: %2i | ","error flag"   ,errorFlag); 
      printf("%s: %4i   ","subevent size",subevtSize);
      printf("\n\t  >>> RAW-Address: ");    
      printf("%s: %2i | ","sector" ,sector);
      printf("%s: %2i | ","module" ,module);   
      printf("%s: %2i | ","mbo"    ,mbo);
      printf("%s: %2i | ","tdc"    ,tdc);
      printf("%s: %2i   ","entry# ",entry);
      printf("\n");
      break;
    default:
      Error("print()","decoding version %i not valid",version);
    }

};

#endif /* ! HMDCDATAWORD */
