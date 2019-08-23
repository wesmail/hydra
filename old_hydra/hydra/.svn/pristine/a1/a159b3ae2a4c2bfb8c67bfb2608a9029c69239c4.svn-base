#ifndef HMDCDETECTOR_H
#define HMDCDETECTOR_H

#include "hdetector.h"
#include "haddef.h"
#include <TArrayI.h>

class HParIo;

class HMdcDetector : public HDetector {
protected:
  Int_t maxLayers;  // maximum number of layers
public:
  HMdcDetector();
  ~HMdcDetector();

  Int_t getMaxLayers();
  Bool_t init(Text_t* level="raw");
  void activateParIo(HParIo* io);
  Bool_t write(HParIo* io);
  HCategory* buildMatrixCategory(Text_t* className,Float_t fillRate);
  HCategory* buildCategorySplit(Text_t* className,Float_t fillRate);
  HCategory* buildCategory(Cat_t cat);

  ClassDef(HMdcDetector,1) // Mdc detector class
};

#endif  /* !HMDCDETECTOR_H */
                                                            
