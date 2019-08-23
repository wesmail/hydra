#ifndef HRICHDETECTOR_H
#define HRICHDETECTOR_H

#pragma interface

#include "hdetector.h" 
#include "haddef.h"
#include <TArrayI.h>

class HParIo;
class HTask;
class HTaskSet;
class HParSet;



class HRichDetector : public HDetector {

private:
  Int_t maxColumns;
  Int_t maxRows;
  HParSet *fGeometry; //!

  void getMaxValues(int *);

public:
  HCategory *buildLinearCat(Text_t *classname);
  HCategory *buildMatrixCat(Text_t *classname, Float_t fillRate);
  HCategory *buildSimMatrixCat(Text_t *classname,Float_t fillRate, Int_t size=200);
//  HCategory *buildCategorySplit(Text_t *className, Float_t fillRate);

public:
  HRichDetector();
  ~HRichDetector();
  HTask *buildTask(Text_t name[], Option_t *opt="");
  HCategory *buildCategory(Cat_t cat);

  void activateParIo(HParIo* io);
  Bool_t write(HParIo* io);

  Bool_t init(void) { return kTRUE; }
  Int_t getSectors();
  Int_t getColumns() { return maxColumns; }
  Int_t getRows() { return maxRows; }

  HParSet* getGeometry() { return fGeometry; }
  void setGeometry(HParSet* pGeometry) { fGeometry = pGeometry; }

  ClassDef(HRichDetector,1) // RICH detector class
};


#endif // HRICHDETECTOR_H
