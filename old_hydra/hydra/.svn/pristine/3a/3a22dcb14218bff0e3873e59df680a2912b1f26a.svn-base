#ifndef HKICKPLANE_H
#define HKICKPLANE_H

#include "hsurface.h"
#include "hkickgrid.h"
#include "hparset.h"

class HKickPlane : public HParSet {
protected:
  HSurface *fSurface; //Kick plane's surface.
  Double_t fScaleFactor;    //Field scaling
  HKickGrid fPtGrid;        //Grid for Pt
  HKickGrid fPtPhiGrid;     //Grid for Pt for phi deflection
  HKickGrid fPtGridNeg;     //Grid for Pt and neg. parts
  HKickGrid fPtPhiGridNeg;  //Grid for Pt for phi deflection and neg. parts
 public:
  void initParams(void);
public:
  HKickPlane(Text_t name[]="KickPlane");
  HKickPlane(const HKickPlane &pl);
  virtual ~HKickPlane(void);
  static HKickPlane *getMDC3(void);
  static HKickPlane *getMeta(void);
  void clear(void);
  void readFrom(const HKickPlane &pl);
  Double_t getFieldStrength(void) { return fScaleFactor;}
  Bool_t init(HParIo *inp, Int_t *set);
  Int_t write(HParIo *io);
  Int_t writeFile(fstream *pFile);
  Bool_t read(fstream *pFile,Int_t* set,Int_t inputNumber);
  inline Double_t distanceTo(const HGeomVector &v);
  inline Bool_t calcIntersection(const HGeomVector &r,const HGeomVector &alpha,
				 HGeomVector &out) const;
  Double_t getPt(const HGeomVector &pos,Double_t &err,
		 const Int_t polarity=+1);
  Double_t getPtPhi(const HGeomVector &pos,Double_t &err,const Int_t polarity=+1);
  ClassDef(HKickPlane,1)
};

//*********** Inlines
Bool_t HKickPlane::calcIntersection(const HGeomVector &r,const HGeomVector &alpha,
				    HGeomVector &out) const {
  //FIXME: Devolver kFALSE para trazas que no cortan al plano.
  fSurface->calcIntersection(r,alpha,out);
  return kTRUE;
}

Double_t  HKickPlane::distanceTo(const HGeomVector &v) {
  return fSurface->distanceTo( v );
}

#endif
