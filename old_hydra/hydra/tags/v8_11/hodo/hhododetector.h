#ifndef HHODODETECTOR_H
#define HHODODETECTOR_H

#include "hdetector.h" 

class HHodoDetector : public HDetector {
public:
  HHodoDetector(void);
  ~HHodoDetector(void);
  HCategory* buildCategory(Cat_t);
  HCategory* buildLinearCategory(Text_t*,Float_t);
  HCategory* buildMatrixCategory(Text_t*,Float_t);
  Bool_t init(void) {return kTRUE;}
  void activateParIo(HParIo*);
  Bool_t write(HParIo*);
  Int_t getMaxModInSetup(void);
  
  ClassDef(HHodoDetector,1) // Detector class for the Pion Hodoscopes
};


#endif /* !HHODODETECTOR_H */









