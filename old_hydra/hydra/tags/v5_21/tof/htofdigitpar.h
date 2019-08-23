#ifndef HTOFDIGITPAR_H
#define HTOFDIGITPAR_H

#include <TObject.h>
#include "hlocation.h"
#include "hparset.h"

class HTofDigitParCell : public TObject {

private:

  Float_t effiQuant;   // PMT quantum efficiency
  Float_t yieldPhot;   // photon yield in scintillator [phot/MeV]
  Float_t halfLen;     // half length of the strip [mm]
  Float_t velGroup;    // group velocity [mm/ns]
  Float_t angleRef;    // total internal reflection angle [deg]
  Float_t attenLen;    // attenuation length [mm]
  Float_t timeResZero; // time resolution in the centre of the strip [ns]

public:

  HTofDigitParCell(void) {}
  ~HTofDigitParCell(void) {}
  Float_t getEffiQuant() { return effiQuant; }
  Float_t getYieldPhot() { return yieldPhot; }
  Float_t getHalfLen() { return halfLen; }
  Float_t getVelGroup() { return velGroup; }
  Float_t getAngleRef() { return angleRef; }
  Float_t getAttenLen() { return attenLen; }
  Float_t getTimeResZero() { return timeResZero; }

  void setEffiQuant(Float_t f) { effiQuant = f; }
  void setYieldPhot(Float_t f) { yieldPhot = f; }
  void setHalfLen(Float_t f) { halfLen = f; }
  void setVelGroup(Float_t f) { velGroup = f; }
  void setAngleRef(Float_t f) { angleRef = f; }
  void setAttenLen(Float_t f) { attenLen = f; }
  void setTimeResZero(Float_t f) { timeResZero = f; }

  ClassDef(HTofDigitParCell,1)  // Cell level of the TOF digit parameters

}; 


class HTofDigitPar : public HParSet {

private:

  void printHeader(void);
  void printPause(void);

protected:

  HTofDigitParCell fCells[6][22][8]; //!

public:

  HTofDigitPar(void) { SetName("TofDigitPar"); }
  ~HTofDigitPar(void) {}
  Bool_t check(void) {return kFALSE;}
  Bool_t init(void);
  Int_t update(Int_t evtNum) { return (evtNum>0)?kMaxInt:0; }
  Int_t write(void) {return kFALSE;}
  void clear(void) {}
  void print(void);
  void printParam();
  HTofDigitParCell *getCell(HLocation &l) { return &fCells[l[0]][l[1]][l[2]];}

  ClassDef(HTofDigitPar,1)  // Container for the TOF digit parameters

};

#endif /* !HTOFDIGITPAR_H */










