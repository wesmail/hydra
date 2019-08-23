#ifndef  HTOFHITF_H
#define  HTOFHITF_H

#include "hreconstructor.h"

class HCategory;
class HTofCalPar;
class HIterator;
class HTofGeomPar;
class HSpecGeomPar;
class HSpectrometer;
class HRuntimeDb;
class HTofHit;

class HTofHitF : public HReconstructor {
protected:
  HCategory *fRawCat;  //!Raw data category
  HCategory *fHitCat;  //!Pointer to the hits category
  HCategory *fStartHitCat; //! Category with start hits
  HTofCalPar *fCalPar; //!Calibration parameters
  HLocation fLoc;      //!
  HIterator *iter;     //!
  HTofGeomPar *fTofGeometry; //!Container for TOF geometry
  HSpecGeomPar *fSpecGeometry;

  void fillGeometry(HTofHit *);
public:
  HTofHitF(void);
  HTofHitF(Text_t *name,Text_t *title);
  ~HTofHitF(void);
  Bool_t init(void);
  void initParContainer(HSpectrometer *,HRuntimeDb *);
  Bool_t finalize(void) {return kTRUE;}
  Int_t execute(void);
  ClassDef(HTofHitF,0) //Finds TOF hits
};

#endif /* !HTOFHITF_H */






