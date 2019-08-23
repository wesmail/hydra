#ifndef HMUDATA_H
#define HMUDATA_H

#include <TObject.h>


class HMUData : public TObject
{
private:
  UInt_t version;      //
  UInt_t triggerCode;  //  
  UInt_t downscaling;  //
  UInt_t downscalingFlag; //
  UInt_t triggerDecision; //
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

public:
  HMUData(void);
  ~HMUData(void) {};

  Int_t decode (UInt_t *ptr, Int_t maxLength); 

  void clear(); 

  Bool_t isTriggerCodeFirstFormat(void) { return fTriggerCodeFirstFormat; }
  Bool_t getTriggerCodeFirstFormat() { return fTriggerCodeFirstFormat; };

  UInt_t getVersion(){return version;}
  UInt_t getTriggerCode(){return triggerCode;}
  UInt_t getDownscaling(){return downscaling;}
  UInt_t getDownscalingFlag(){return downscalingFlag;}
  UInt_t getTriggerDecision() { return triggerDecision;}

  UInt_t getNHitsRich() { return nHitsRich; }
  UInt_t getNHitsShower() { return nHitsShower; }
  UInt_t getNHitsTof() { return nHitsTof; }
  UInt_t getNLeptons(){return nLeptons;}
  UInt_t getNDiLeptons(){return nDiLeptons;}

  void setTriggerCodeFirstFormat(Bool_t tcff) { fTriggerCodeFirstFormat = tcff; }
  void setVersion(Int_t v){version=v;}
  void setTriggerCode(Int_t tc){triggerCode=tc;}
  void setDownscaling(Int_t d){downscaling=d;}
  void setDownscalingFlag(Int_t df){downscalingFlag=df;}
  void setTriggerDecision(Int_t td) {triggerDecision=td;}

  void setNHitsRich(Int_t hr) { nHitsRich=hr; }
  void setNHitsShower(Int_t hs) { nHitsShower=hs; }
  void setNHitsTof(Int_t ht) { nHitsTof=ht; }
  void setNLeptons(Int_t nl){ nLeptons = nl ;}
  void setNDiLeptons(Int_t nl){ nDiLeptons = nl;}
//  void setTriggerCode(Int_t tc){ triggerCode = tc;}

 

  ClassDef(HMUData,1) // MUData
};

#endif /* !HMUDATA_H */













