#ifndef HHODODUMMY_H
#define HHODODUMMY_H

#include "hreconstructor.h"
#include "hlocation.h"

class HCategory;
class HIterator;

class HHodoDummy : public HReconstructor {
protected:
  HCategory *rawCat;          //! pointer to the raw data 
  HIterator *iter;            //! iterator on raw data.
  HLocation loc;              //! location for new cal object
 public: 
  HHodoDummy(void);
  HHodoDummy(Text_t* name,Text_t* title);
  ~HHodoDummy(void);
  Bool_t init(void);
  Bool_t finalize(void) { return kTRUE; }
  Int_t execute(void);
public:
  ClassDef(HHodoDummy,0) // Dummy raw->X for Hodo data
};

#endif /* !HHODODUMMY_H */
















