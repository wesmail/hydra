#ifndef HTBOXDETECTOR_H
#define HTBOXDETECTOR_H

#include "hdetector.h"
#include "haddef.h"
#include "hstartdef.h"

#include <TArrayI.h>

class HParIo;
class HDetGeomPar;
class HGeomShapes;
class HCategory;

class HTBoxDetector : public HDetector {
public:
  HTBoxDetector();
  HTBoxDetector(Int_t mod, Int_t chan);
  ~HTBoxDetector();
  Bool_t init(void);
//  void activateParIo(HParIo* io);
  Bool_t write(HParIo* io) {return kTRUE;}
  HCategory* buildMatrixCategory(Text_t*);
  HCategory* buildLinearCategory(Text_t*);
  HCategory* buildCategory(Cat_t);
  Int_t getMaxModInSetup(void);
//  HDetGeomPar* createDetGeomPar(HGeomShapes*);

  ClassDef(HTBoxDetector,1) // Trigger Box detector class
};

#endif  /* !HTBOXDETECTOR_H */
                                                            














