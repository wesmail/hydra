#ifndef HPARCOND_H
#define HPARCOND_H

#include "hparset.h"

class HParamList;

class HParCond : public HParSet {
public:
  HParCond(const char* name,const char* title,const char* context)
         : HParSet(name,title,context) {}
  virtual ~HParCond() {}
  virtual void putParams(HParamList*)=0;
  virtual Bool_t getParams(HParamList*)=0;
  virtual void printParams();
protected:
  HParCond() {};
  ClassDef(HParCond,1) // Base class for condition containers
};

#endif  /* !HPARCOND_H */
