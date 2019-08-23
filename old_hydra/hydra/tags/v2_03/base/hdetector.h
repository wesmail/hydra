#ifndef HDETECTOR_H
#define HDETECTOR_H

#include <TNamed.h>
#include <TArrayI.h>
#include "haddef.h"

class HTask;
class HCategory;
class HParIo;

class HDetector : public TNamed {
protected:
  Int_t maxModules; //maximun number of modules per sector
  TArrayI* modules; //Module's array.
public:
  HDetector(void);
  HDetector(Text_t* name, Text_t* title);
  virtual ~HDetector(void);
  Int_t getMaxModules(void) {return maxModules;}
  Int_t getMaxSectors(void) {return 6;}
  virtual void setModules(Int_t sec,Int_t* modules);
  virtual Int_t* getModules(void);
  virtual Int_t getModule(Int_t sector,Int_t mod) ;
  virtual Bool_t init(Text_t* ) {return kTRUE;} //Text_t level="raw"
  virtual Bool_t write(HParIo* ) {return kFALSE;}
  virtual void activateParIo(HParIo* ) {return;}
  void print();

  virtual HTask *buildTask(Text_t *,Option_t *) {return 0;}
  virtual HCategory *buildCategory(Cat_t ) {return 0;}
  ClassDef(HDetector,1) //Detector's descriptor.
};

#endif /* !HDETECTOR_H */
