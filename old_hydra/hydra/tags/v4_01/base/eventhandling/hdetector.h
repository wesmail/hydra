#ifndef HDETECTOR_H
#define HDETECTOR_H

#include <TNamed.h>
#include <TArrayI.h>
#include "haddef.h"

class HTask;
class HCategory;
class HParIo;
class HGeomShapes;
class HDetGeomPar;

class HDetector : public TNamed {
protected:
  Int_t maxModules;   //maximum number of modules per sector
  Int_t maxComponents; //maximum number of components per module
  TArrayI* modules;   //Module's array.
public:
  HDetector(void);
  HDetector(Text_t* name, Text_t* title);
  virtual ~HDetector(void);
  Int_t getMaxSectors(void) {return 6;}
  Int_t getMaxModules(void) {return maxModules;}
  Int_t getMaxComponents(void) {return maxComponents;}
  virtual void setModules(Int_t sec,Int_t* modules);
  virtual Int_t* getModules(void);
  virtual Int_t getModule(Int_t sector,Int_t mod) ;
  virtual Bool_t init(void) {return kTRUE;} 
  virtual Bool_t write(HParIo* ) {return kFALSE;}
  virtual HDetGeomPar* createDetGeomPar(HGeomShapes*) {return 0;}
  virtual void activateParIo(HParIo* ) {return;}
  void print();

  virtual HTask *buildTask(Text_t *,Option_t *) {return 0;}
  virtual HCategory *buildCategory(Cat_t ) {return 0;}
  ClassDef(HDetector,1) //Detector's descriptor.
};

#endif /* !HDETECTOR_H */
