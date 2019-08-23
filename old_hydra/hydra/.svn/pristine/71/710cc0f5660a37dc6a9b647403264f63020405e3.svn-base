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

  Bool_t init(Text_t* level="raw");
  void activateParIo(HParIo* io);
  Bool_t write(HParIo* io);
  HCategory* buildMatrixCategory(Text_t* className,Float_t fillRate);
  HCategory* buildCategorySplit(Text_t* className,Float_t fillRate);
  HCategory* buildCategory(Cat_t cat);
  HDetGeomPar* createDetGeomPar(HGeomShapes*);

  ClassDef(HMdcDetector,1) // Mdc detector class
};

#endif  /* !HMDCDETECTOR_H */
                                                            
