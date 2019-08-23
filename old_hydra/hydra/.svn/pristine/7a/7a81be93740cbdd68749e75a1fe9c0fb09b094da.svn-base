#ifndef HTOFDETECTOR_H
#define HTOFDETECTOR_H

#include "hdetector.h" 
#include "haddef.h"
#include <TArrayI.h>

class HTofDetector : public HDetector {
protected:
  HCategory *buildMatrixCat(Text_t *classname,Float_t fillRate);
  HCategory *buildCategorySplit(Text_t *className,Float_t fillRate);
public:
  HTofDetector(void);
  ~HTofDetector(void);
  HTask *buildTask(Text_t name[],Option_t *opt="");
  HCategory *buildCategory(Cat_t cat);
  Bool_t init(void) {return kTRUE;}

  ClassDef(HTofDetector,1)
};


#endif /* !HTOFDETECTOR_H */
