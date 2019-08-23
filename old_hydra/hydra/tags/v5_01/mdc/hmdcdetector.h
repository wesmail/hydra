#ifndef HMDCDETECTOR_H
#define HMDCDETECTOR_H

#include "hdetector.h"
#include "haddef.h"
#include <TArrayI.h>

class HParIo;
class HDetGeomPar;
class HGeomShapes;

class HMdcDetector : public HDetector {
public:
  HMdcDetector();
  ~HMdcDetector();

  Bool_t init(void);
  void activateParIo(HParIo* io);
  Bool_t write(HParIo* io);
  HCategory* buildLinearCategory(Text_t* className);
  HCategory* buildMatrixCategory(Text_t* className,Float_t fillRate);
  HCategory* buildCategorySplit(Text_t* className,Float_t fillRate);
  HCategory* buildCategory(Cat_t cat);
  HDetGeomPar* createDetGeomPar(HGeomShapes*);

  ClassDef(HMdcDetector,1) // Mdc detector class
};

#endif  /* !HMDCDETECTOR_H */
                                                            
