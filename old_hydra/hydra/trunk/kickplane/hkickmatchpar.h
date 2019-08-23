#ifndef HKICKMATCHPAR_H
#define HKICKMATCHPAR_H

#include "hsurface.h"
#include "hkickgrid.h"
#include "hparset.h"

class HKickMatchPar : public HParSet {
protected:
  friend class HKickParOraIo;
  Double_t fScaleFactor;    //Field scaling
  HKickGrid fGridA,fGridB,fGridC; //Parameterization for positive particles
  HKickGrid fGridAN,fGridBN,fGridCN; //Parameterization for negative particles

public:
  HKickMatchPar(const Char_t *name="KickMatchPar",
		const Char_t *title="Matching parameters for kickplane v2",
		const Char_t *context="KickMatchProduction");
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
  Float_t getDeflection(HGeomVector &position,Float_t p,Float_t &err,
			const Int_t polarity=+1);
  ClassDef(HKickMatchPar,2)
};

#endif
