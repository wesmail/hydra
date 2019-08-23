#ifndef  HTOFHITF_H
#define  HTOFHITF_H

#include "hreconstructor.h"
class HCategory;

class HTofHitF : public HReconstructor {
private:
  HCategory *fCalCat; //!Calibrated data category
  HCategory *fHitCat; //!Pointer to the hits category
  HLocation fLoc;
public:
  HTofHitF(void);
  HTofHitF(Text_t *name,Text_t *title);
  ~HTofHitF(void) {}
  Bool_t init(void);
  Bool_t finalize(void) {return kTRUE;}
  Int_t execute(void);
  ClassDef(HTofHitF,1) //Finds TOF hits
};

#endif /* !HTOFHITF_H */
