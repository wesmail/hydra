#ifndef HKICKTASK_H
#define HKICKTASK_H

#include "hreconstructor.h"

class HRuntimeDb;

class HKickTask : public HReconstructor {
private:
  static Bool_t fParIoInitialized;
public:
  HKickTask(void) {}
  HKickTask(Text_t name[], Text_t title[]) : HReconstructor(name,title) {} 
  static void setKickParIo(HRuntimeDb *rtdb);
  ClassDef(HKickTask,0)
};

#endif
