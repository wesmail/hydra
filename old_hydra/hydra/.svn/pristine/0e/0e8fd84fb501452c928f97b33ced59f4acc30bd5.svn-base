#ifndef HSTARTDETECTOR_H
#define HSTARTDETECTOR_H

#include "hdetector.h"
#include "haddef.h"
#include "hstartdef.h"

#include <TArrayI.h>

class HParIo;
class HDetGeomPar;
class HGeomShapes;
class HCategory;

class HStartDetector : public HDetector {
public:
  HStartDetector();
  ~HStartDetector();
  Bool_t init(void);
  void activateParIo(HParIo* io);
  Bool_t write(HParIo* io);
  HCategory* buildMatrixCategory(Text_t*,Float_t);
  HCategory* buildLinearCategory(Text_t*);
  HCategory* buildCategory(Cat_t);
  Int_t getMaxModInSetup(void);
  HDetGeomPar* createDetGeomPar(HGeomShapes*);

  ClassDef(HStartDetector,1) // Start detector class
};

#endif  /* !HSTARTDETECTOR_H */
                                                            














