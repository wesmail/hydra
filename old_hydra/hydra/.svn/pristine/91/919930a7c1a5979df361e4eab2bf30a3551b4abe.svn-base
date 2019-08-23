#ifndef HKICKPLANE_H
#define HKICKPLANE_H

#include "hsurface.h"
#include "hkickgrid.h"
#include "hparset.h"

class HKickPlane : public HParSet {
protected:
  HSurface *fSurface; //Kick plane's surface.
  HKickGrid fPtGrid;        //Grid for Pt
  HKickGrid fPtPhiGrid;     //Grid for Pt for phi deflection
  HKickGrid fPtGridNeg;     //Grid for Pt and neg. parts
  HKickGrid fPtPhiGridNeg;  //Grid for Pt for phi deflection and neg. parts
 public:
  void initParams(void);
public:
  HKickPlane(void);
  virtual ~HKickPlane(void);
  void clear(void);
  Bool_t init(HParIo *inp, Int_t *set);
  Int_t write(HParIo *io);
  Int_t writeFile(fstream *pFile);
  Bool_t read(fstream *pFile,Int_t* set,Int_t inputNumber);
  inline Bool_t calcIntersection(const HGeomVector &r,const HGeomVector &alpha,
				 HGeomVector &out) const;
  Double_t getPt(const HGeomVector &pos,Double_t &err,
		 const Int_t polarity=+1);
  Double_t getPtPhi(const HGeomVector &pos,Double_t &err,const Int_t polarity=+1);
  ClassDef(HKickPlane,0)
};

//*********** Inlines
Bool_t HKickPlane::calcIntersection(const HGeomVector &r,const HGeomVector &alpha,
				    HGeomVector &out) const {
  //FIXME: Devolver kFALSE para trazas que no cortan al plano.
  fSurface->calcIntersection(r,alpha,out);
  return kTRUE;
}

#endif
