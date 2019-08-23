#ifndef  HTOFHITF_H
#define  HTOFHITF_H

#include "hreconstructor.h"

class HCategory;
class HTofCalPar;
class HIterator;

class HTofHitF : public HReconstructor {
protected:
  HCategory *fRawCat;  //!Raw data category
  HCategory *fHitCat;  //!Pointer to the hits category
  HTofCalPar *fCalPar; //!Calibration parameters
  HLocation fLoc;      //!
  HIterator *iter;     //!
public:
  HTofHitF(void);
  HTofHitF(Text_t *name,Text_t *title);
  ~HTofHitF(void);
  Bool_t init(void);
  void initParContainer();
  Bool_t finalize(void) {return kTRUE;}
  Int_t execute(void);
  ClassDef(HTofHitF,1) //Finds TOF hits
};

#endif /* !HTOFHITF_H */






