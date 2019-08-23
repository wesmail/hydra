#ifndef HKICKMATCHPAR_H
#define HKICKMATCHPAR_H

#include "hsurface.h"
#include "hkickgrid.h"
#include "hparset.h"

class HKickMatchPar : public HParSet {
protected:
  Double_t fScaleFactor;    //Field scaling
  HKickGrid fPtPhiGrid;     //Grid for Pt for phi deflection
  HKickGrid fPtPhiGridNeg;  //Grid for Pt for phi deflection and neg. parts

public:
  HKickMatchPar(const char *name="KickPlane2",
		const char *title="Matching parameters for kickplane v2",
		const char *context="");
  HKickMatchPar(const HKickMatchPar &pl);
  virtual ~HKickMatchPar(void);
  static HKickMatchPar *getMeta(void);
  void clear(void);
  void readFrom(const HKickMatchPar &pl);
  Double_t getFieldStrength(void) { return fScaleFactor;}
  Bool_t init(HParIo *inp, Int_t *set);
  Int_t write(HParIo *io);
  Int_t writeFile(fstream *pFile);
  Bool_t read(fstream *pFile,Int_t* set,Int_t inputNumber);
  Double_t getPtPhi(const HGeomVector &pos,Double_t &err,const Int_t polarity=+1);
  ClassDef(HKickMatchPar,1)
};

#endif
