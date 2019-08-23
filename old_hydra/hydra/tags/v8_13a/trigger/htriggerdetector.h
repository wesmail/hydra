//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 07/14/2005
//*-- Version: $Id: htriggerdetector.h,v 1.6 2005-07-15 09:00:25 wuestenf Exp $

#ifndef HTRIGGERDETECTOR_H
#define HTRIGGERDETECTOR_H

#pragma interface

#include "hdetector.h"
#include "haddef.h"

class HParIo;
class HTask;
class HTaskSet;
class HParSet;

class HTriggerDetector : public HDetector {

public:
  HCategory *buildLinearCat(Text_t *classname);
  HCategory *buildMatrixCat(Text_t *classname, Float_t fillRate);
  HCategory *buildSimMatrixCat(Text_t *classname,Float_t fillRate, Int_t size=200);

  HTriggerDetector();
  ~HTriggerDetector();
  HTask *buildTask(Text_t name[], Option_t *opt="");
  HCategory *buildCategory(Cat_t cat);
  void activateParIo(HParIo* io);
  Bool_t write(HParIo* io);

  Bool_t init(void) { return kTRUE; }

  ClassDef(HTriggerDetector,1) //  Detector class for the trigger system
};


#endif // HTRIGGERDETECTOR_H
