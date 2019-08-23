#ifndef HKICKTASK_H
#define HKICKTASK_H

#include "hreconstructor.h"

class HRuntimeDb;
class HParIo;

class HKickTask : public HReconstructor {
public:
  HKickTask(void) {}
  HKickTask(Text_t name[], Text_t title[]) : HReconstructor(name,title) {} 
  static void setKickParInput(HParIo *io,Int_t n);
  static void setKickParIo(HRuntimeDb *rtdb);
  static void setKickParOutput(HParIo *io);
  ClassDef(HKickTask,0)
};

#endif
