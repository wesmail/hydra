#ifndef HDETPARFILEIO_H
#define HDETPARFILEIO_H

#include "hdetpario.h"
#include "TFile.h"
#include "TArrayI.h"

class HParRootFile;
class HDetector;
class HParSet;

class HDetParFileIo : public HDetParIo {
protected:
  HParRootFile* pFile;    // pointer to parameter ROOT file
  TArrayI* modulesFound;  // modules found in Root file
  TArrayI* initModules;   // array of modules initialized from Root file
  Bool_t isActiv;         // flag is set kTrue by funtion readModules()
public:
  HDetParFileIo(HParRootFile* f);
  virtual ~HDetParFileIo();
  Int_t write(HParSet*);
  Bool_t write(HDetector*);
  void printInfo(Text_t* msg);
  void resetActivFlag(Bool_t f=kFALSE) {isActiv=f;}
protected:
  Int_t readModules(Text_t* detName);
  Int_t findInputVersion(Text_t* contName);
  Int_t getMaxVersion(Text_t* contName);
  TObject* findContainer(Text_t* contName, Int_t version);

  ClassDef(HDetParFileIo,0) // Base class for parameter I/O from ROOT files
};

#endif  /* !HDETPARFILEIO_H */

