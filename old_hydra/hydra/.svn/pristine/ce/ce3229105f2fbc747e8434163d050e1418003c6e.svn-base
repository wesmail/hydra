#ifndef HRICHDETECTOR_H
#define HRICHDETECTOR_H

#pragma interface

#include "hdetector.h" 
#include "haddef.h"
#include "TArrayI.h"

class HParIo;
class HTask;
class HTaskSet;
class HParSet;



class HRichDetector : public HDetector {

private:

  Int_t    maxColumns;
  Int_t    maxRows;
  HParSet* fGeometry; //!

  void getMaxValues(Int_t *);

public:
  HCategory *buildLinearCat   (const Text_t* classname, Int_t   size = 1000);
  HCategory *buildMatrixCat   (const Text_t* classname, Float_t fillRate);
  HCategory *buildSimMatrixCat(const Text_t* classname, Float_t fillRate, Int_t size = 200);

public:
  HRichDetector();
  ~HRichDetector();

  HTask*     buildTask(const Text_t name[],const Option_t* opt = "");
  HCategory* buildCategory(Cat_t cat);

  void     setGeometry(HParSet* pGeometry) { fGeometry = pGeometry; }
  void     activateParIo(HParIo* io);
  Bool_t   write(HParIo* io);
  Bool_t   init(void) { return kTRUE; }
  Int_t    getSectors();
  Int_t    getColumns()  { return maxColumns; }
  Int_t    getRows()     { return maxRows;    }
  HParSet* getGeometry() { return fGeometry;  }


  ClassDef(HRichDetector,1) // RICH detector class
};


#endif // HRICHDETECTOR_H
