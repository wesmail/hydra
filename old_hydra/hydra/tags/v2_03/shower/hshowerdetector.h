#ifndef HSHOWERDETECTOR_H
#define HSHOWERDETECTOR_H

#pragma interface

#include "hdetector.h" 
#include "haddef.h"
#include <TArrayI.h>

class HShowerDetector : public HDetector {
private:
  Int_t maxRows;
  Int_t maxColumns;

  void getMaxValues(int *, int *);
protected:
  HCategory *buildLinearCat(Text_t *classname);
  HCategory *buildMatrixCat(Text_t *classname,Float_t fillRate);
  HCategory *buildCategorySplitS(Text_t *className,Float_t fillRate);
  HCategory *buildCategorySplitSM(Text_t *className,Float_t fillRate);
  HCategory *buildCategorySplitSMR(Text_t *className,Float_t fillRate);
public:
  HShowerDetector(void);
  ~HShowerDetector(void);
  HTask *buildTask(Text_t name[],Option_t *opt="");
  HCategory *buildCategory(Cat_t cat);

  Bool_t init(void) {return kTRUE;}
  Int_t getShowerSectors();
  Int_t getShowerModules();
  Int_t getRows(){return maxRows;}
  Int_t getColumns(){return maxColumns;}

  ClassDef(HShowerDetector,1)
};


#endif /* !HSHOWERDETECTOR_H */
