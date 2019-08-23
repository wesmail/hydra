#ifndef  HTOFHITF_H
#define  HTOFHITF_H

#include "hreconstructor.h"


class HCategory;
class HTofCalPar;

class HTofHitF : public HReconstructor {
private:
  HCategory *fRawCat; //!Raw data category
  HCategory *fHitCat; //!Pointer to the hits category
//  HIterator *fIter; //!Iterator on raw data
  HTofCalPar *fCalPar; //!Calibration parameters
  HLocation fLoc;
public:
  HTofHitF(void);
  HTofHitF(Text_t *name,Text_t *title);
  ~HTofHitF(void) {}
  Bool_t init(void);
  void initParContainer();
  Bool_t finalize(void) {return kTRUE;}
  Int_t execute(void);
  ClassDef(HTofHitF,1) //Finds TOF hits
};

#endif /* !HTOFHITF_H */






