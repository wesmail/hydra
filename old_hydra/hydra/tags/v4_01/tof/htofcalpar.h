#ifndef HTOFCALPAR_H
#define HTOFCALPAR_H

#include <TObject.h>
#include "hlocation.h"
#include "hparset.h"

class HTofCalParCell : public TObject {
private:

  Float_t leftK; // left side TDC width channel
  Float_t rightK; // right side TDC width channel
  Float_t vGroup;  // group velocity
  Float_t posK; // position offset
  Float_t timK; // timing offset

  /*  Float_t fSlopeTL;
  Float_t fSlopeTR;
  Float_t fOffsetTL;
  Float_t fOffsetTR;
  Float_t fSlopeQL;
  Float_t fSlopeQR;
  Float_t fOffsetQL;
  Float_t fOffsetQR;*/
public:
  HTofCalParCell(void) {}
  ~HTofCalParCell(void) {}
  Float_t getLeftK() { return leftK; }
  Float_t getRightK() { return rightK; }
  Float_t getVGroup() { return vGroup; }
  Float_t getPosK() { return posK; }
  Float_t getTimK() { return timK; }
  void setLeftK(Float_t f) { leftK = f; }
  void setRightK(Float_t f) { rightK = f; }
  void setVGroup(Float_t f) { vGroup = f; }
  void setPosK(Float_t f) { posK = f; }
  void setTimK(Float_t f) { timK = f; }
//
  /*  Float_t getSlopeTimeLeft(void) {return fSlopeTL;}
  Float_t getSlopeTimeRight(void) {return fSlopeTR;}
  Float_t getOffsetTimeLeft(void) {return fOffsetTL;}
  Float_t getOffsetTimeRight(void) {return fOffsetTR;}
  Float_t getSlopeChargeLeft(void) {return fSlopeQL;}
  Float_t getSlopeChargeRight(void) {return fSlopeQR;}
  Float_t getOffsetChargeLeft(void) {return fOffsetQL;}
  Float_t getOffsetChargeRight(void) {return fOffsetQR;}
  void setSlopeTimeLeft(Float_t f) {fSlopeTL=f;}
  void setSlopeTimeRight(Float_t f) {fSlopeTR=f;}
  void setOffsetTimeLeft(Float_t f) {fOffsetTL=f;}
  void setOffsetTimeRight(Float_t f) {fOffsetTR=f;}
  void setSlopeChargeLeft(Float_t f) {fSlopeQL=f;}
  void setSlopeChargeRight(Float_t f) {fSlopeQR=f;}
  void setOffsetChargeLeft(Float_t f) {fOffsetQL=f;}
  void setOffsetChargeRight(Float_t f) {fOffsetQR=f;}  */
  ClassDef(HTofCalParCell,1)  // Cell level of the TOF calibration parameters
}; 

class HTofCalPar : public HParSet {
private:
  void printHeader(void);
  void printPause(void);
protected:
  HTofCalParCell fCells[6][22][8]; //!
public:
  HTofCalPar(void) {SetName("TofCalPar");}
  ~HTofCalPar(void) {}
  Bool_t check(void) {return kFALSE;}
  Bool_t init(void);
  Int_t update(Int_t evtNum) {return (evtNum>0)?kMaxInt:0;}
  Int_t write(void) {return kFALSE;}
  void clear(void) {}
  void print(void);
  void printParam();
  HTofCalParCell *getCell(HLocation &l) {return &fCells[l[0]][l[1]][l[2]];}
  ClassDef(HTofCalPar,1)  // Container for the TOF calibration parameters
};

#endif /* !HTOFCALPAR_H */










