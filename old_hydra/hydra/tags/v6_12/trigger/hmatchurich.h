#ifndef HMATCHURICH_H
#define HMATCHURICH_H

#include <TObject.h>

class HMatchURich : public TObject {
protected:
  Int_t fFifoNb;
  Int_t fSegmentId; // sector number

  // The expressions "row" and "column" below refer to the hardware 
  // notation. The standard HADES "row" and "column" are the other
  // way round
  Int_t fColumnPattern;
  Int_t fRowNb; 
  Int_t fColumn;
  Int_t fRow; 

  // The following "X", "Y", "Theta" and "Phi" are consistent with
  // the standard HADES notation.
  Float_t fTheta;
  Float_t fPhi;
  Int_t iRingX;   
  Int_t iRingY;

public:
  HMatchURich(void);
  ~HMatchURich(void);
  Int_t getFifo(void) { return fFifoNb; }
  Int_t getColumnPattern(void) { return fColumnPattern; }
  Int_t getSegmentId(void) { return fSegmentId; }
  Int_t getSector(void) {return fSegmentId;}
  Int_t getRowNb(void) {return fRowNb; }
  Int_t getRow(void) {return fRow; }
  Int_t getColumn(void) { return fColumn; }
  Float_t getTheta(void) { return fTheta; }
  Float_t getPhi(void) { return fPhi; }
  Int_t getX(void) {return iRingX;}
  Int_t getY(void) {return iRingY;}

  void setFifo(Int_t fifo) { fFifoNb=fifo; }
  void setColumnPattern(Int_t colPat) { fColumnPattern=colPat; }
  void setSegmentId(Int_t segId) { fSegmentId=segId; }
  void setRowNb(Int_t row) { fRowNb=row; }
  void setColumn(Int_t col) { fColumn=col; }
  void setRow(Int_t row) { fRow=row; }
  void setTheta(Float_t th) { fTheta=th; }
  void setPhi(Float_t ph) { fPhi=ph; }
  void setX(Int_t px) { iRingX = px; }
  void setY(Int_t py) { iRingY = py; }

  ClassDef(HMatchURich,1)
};

#endif
