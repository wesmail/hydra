#ifndef HTRIGGERDETECTOR_H
#define HTRIGGERDETECTOR_H

#pragma interface

#include "hdetector.h" 
#include "haddef.h"
#include <TArrayI.h>

class HParIo;
class HTask;
class HTaskSet;
class HParSet;



class HTriggerDetector : public HDetector {

private:


public:
  HCategory *buildLinearCat(Text_t *classname);
  HCategory *buildMatrixCat(Text_t *classname, Float_t fillRate);
  HCategory *buildSimMatrixCat(Text_t *classname,Float_t fillRate, Int_t size=200);

public:
  HTriggerDetector();
  ~HTriggerDetector();
  HTask *buildTask(Text_t name[], Option_t *opt="");
  HCategory *buildCategory(Cat_t cat);
  void activateParIo(HParIo* io);
  Bool_t write(HParIo* io);

  Bool_t init(void) { return kTRUE; }

  ClassDef(HTriggerDetector,1) // 
};


#endif // HTRIGGERDETECTOR_H








