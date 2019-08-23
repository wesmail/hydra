#ifndef HMUDATA_H
#define HMUDATA_H

#include <TObject.h>


class HMUData : public TObject
{
private:
  UInt_t version;      //
  UInt_t triggerCode;  //  
  UInt_t downscaling;  //
  UInt_t nHitsRich;    // 
  UInt_t nHitsShower;  //
  UInt_t nHitsTof;  //

  UInt_t secHitPatternRich; // 
  UInt_t secHitPatternShower; // 
  UInt_t secHitPatternTof; //
  UInt_t nLeptons; // 
  UInt_t nDiLeptons; // 
  Bool_t fTriggerCodeFirstFormat; // flag for format of shower data (old or new)

public:
  HMUData(void);
  ~HMUData(void) {;}
  Int_t decode (UInt_t *ptr, Int_t maxLength); 
  void clear(); 
  UInt_t getNLeptons(){return nLeptons;}
  UInt_t getNDiLeptons(){return nDiLeptons;}
  void setNLeptons(Int_t nl){ nLeptons = nl ;}
  void setNDiLeptons(Int_t nl){ nDiLeptons = nl;}
  void setTriggerCode(Int_t tc){ triggerCode = tc;}
  Bool_t isTriggerCodeFirstFormat(void) { return fTriggerCodeFirstFormat; }
  void setTriggerCodeFirstFormat(Bool_t tcff) { fTriggerCodeFirstFormat = tcff; } 
  Bool_t getTriggerCodeFirstFormat() { return fTriggerCodeFirstFormat; }  

  ClassDef(HMUData,1) // MUData
};

#endif /* !HMUDATA_H */













