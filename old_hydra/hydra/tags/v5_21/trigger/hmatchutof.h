#ifndef HMATCHUTOF_H
#define HMATCHUTOF_H

#include "hdataobject.h"

class HMatchUTof : public HDataObject {
 protected:
  UInt_t fData[512];
  UInt_t fTriggerTag;
 public:
  HMatchUTof(void) {}
  ~HMatchUTof(void) {}
  UInt_t getDataWord(Int_t i) { return fData[i]; }
  void setDataWord(Int_t i,UInt_t val) { fData[i]=val; }
  void setTriggerTag(UInt_t t) { fTriggerTag = t; }
  UInt_t getTriggerTag(void) { return fTriggerTag; }
  ClassDef(HMatchUTof,0)
};

#endif
