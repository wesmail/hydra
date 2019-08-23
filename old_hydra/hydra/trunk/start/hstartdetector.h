#ifndef HSTARTDETECTOR_H
#define HSTARTDETECTOR_H

#include "hdetector.h"
#include "haddef.h"
#include "hstartdef.h"

#include "TArrayI.h"

class HStartDetector : public HDetector {
public:
  HStartDetector();
  ~HStartDetector();
  Bool_t init(void);
  void activateParIo(HParIo* io);
  Bool_t write(HParIo* io);
  HCategory* buildMatrixCategory(const Text_t* name,Float_t fill=1.0);
  HCategory* buildLinearCategory(const Text_t* name);
  HCategory* buildCategory(Cat_t);
  Int_t getMaxModInSetup(void);

  ClassDef(HStartDetector,1) // Start detector class
};

#endif  /* !HSTARTDETECTOR_H */
                                                            














