#ifndef HRTEXTRACTOR_H
#define HRTEXTRACTOR_H

#include "hreconstructor.h"
#include "TNtuple.h"

class HCategory;
class HIterator;

class HRtExtractor : public HReconstructor {
public:
  HRtExtractor(Text_t name[],Text_t title[]);
  ~HRtExtractor(void);
  Bool_t init(void);
  Int_t execute(void);
  Bool_t finalize(void);
protected:
  TNtuple *fData; //!
  HCategory *fCatMdc; //! Geant MDC data
  HIterator *fMdcIter; //! Iterator on MDC data
  HCategory *fCatKine; //! Geant Tracks
  HIterator *fKineIter; //! Iterator on Geant Tracks
public:
  ClassDef(HRtExtractor,1)
};

#endif
