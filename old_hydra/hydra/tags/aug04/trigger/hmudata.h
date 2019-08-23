#ifndef HMUDATA_H
#define HMUDATA_H

#include "hades.h"
#include <TObject.h>

class HEventHeader;
class HMUData : public TObject
{
private:
  UInt_t muLength;

  UInt_t version;      //
  UInt_t triggerCode;  //  
  UInt_t nHitsRich;    // 
  UInt_t nHitsShower;  //
  UInt_t nHitsTof;  //
  UInt_t scalers[20]; //
  
  UInt_t secHitPatternRich; // 
  UInt_t secHitPatternShower; // 
  UInt_t secHitPatternTof; //
  UInt_t nLeptons; // 
  UInt_t nDiLeptons; // 
  Bool_t fTriggerCodeFirstFormat; // flag for format of MU data (old or new)
  HEventHeader *eventhead; //!  pointer to header
public:
  HMUData(void);
  ~HMUData(void) {};

  Int_t decode (UInt_t *ptr, Int_t maxLength); 

  void clear(); 

  void setMULength(Int_t mul){muLength=mul;}
  UInt_t getMULength(){return muLength;}

  Bool_t isTriggerCodeFirstFormat(void) { return fTriggerCodeFirstFormat; }
  Bool_t getTriggerCodeFirstFormat() { return fTriggerCodeFirstFormat; };

  UInt_t getVersion(){return version;}
  UInt_t getTriggerCode(){return triggerCode;}
  UInt_t getNHitsRich() { return nHitsRich; }
  UInt_t getNHitsShower() { return nHitsShower; }
  UInt_t getNHitsTof() { return nHitsTof; }
  UInt_t getSecHitPatternRich() {return secHitPatternRich;}
  UInt_t getSecHitPatternShower() {return secHitPatternShower;}
  UInt_t getSecHitPatternTof() {return secHitPatternTof;}
  UInt_t getNLeptons(){return nLeptons;}
  UInt_t getNDiLeptons(){return nDiLeptons;}

  void setTriggerCodeFirstFormat(Bool_t tcff) { fTriggerCodeFirstFormat = tcff; }
  void setVersion(Int_t v){version=v;}
  void setTriggerCode(Int_t tc){triggerCode=tc;}

  void setNHitsRich(Int_t hr) { nHitsRich=hr; }
  void setNHitsShower(Int_t hs) { nHitsShower=hs; }
  void setNHitsTof(Int_t ht) { nHitsTof=ht; }
  void setSecHitPatternRich(Int_t pr) {secHitPatternRich=pr;}
  void setSecHitPatternShower(Int_t pr) {secHitPatternShower=pr;}
  void setSecHitPatternTof(Int_t pr) {secHitPatternTof=pr;}
  void setNLeptons(Int_t nl){ nLeptons = nl ;}
  void setNDiLeptons(Int_t nl){ nDiLeptons = nl;}

  ClassDef(HMUData,2) // MUData
};

#endif /* !HMUDATA_H */













