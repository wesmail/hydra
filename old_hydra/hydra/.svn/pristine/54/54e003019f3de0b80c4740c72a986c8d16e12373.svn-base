#ifndef HMATCHUTOFHEADER_H
#define HMATCHUTOFHEADER_H
#include "hdataobject.h"

class HMatchUTofHeader : public HDataObject {
 private:
  Int_t fTriggerTag; 
  Int_t fID; 
  Int_t fTriggerCode;
  Int_t fVeto;
 public:
  HMatchUTofHeader(){}
  ~HMatchUTofHeader(){}
  void setTriggers(Int_t i, Int_t j){
    fTriggerTag  = i;
    fTriggerCode = j;
  }

  void setID( Int_t i){ fID=i;}
  void setVeto( Int_t i){fVeto=i;}  
  Int_t getTriggerTag(){ return fTriggerTag;}
  Int_t getTriggerCode(){return fTriggerCode;}
  Int_t getVeto(){return fVeto;}
  Int_t getID(){return fID;}
                    
  ClassDef(HMatchUTofHeader,0)
};

#endif
