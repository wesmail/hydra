#ifndef HKICKPLANE2_H
#define HKICKPLANE2_H

#include "hsurface.h"
#include "hkickgrid.h"
#include "hparset.h"

class HKickPlane2 : public HParSet {
protected:
  friend class HKickParOraIo;
  HSurface *fSurface; //Kick plane's surface.
  Double_t fScaleFactor;    //Field scaling
  HKickGrid fGridA;        //Grid for Pt
  HKickGrid fGridB;        //Grid for Pt
  HKickGrid fGridC;
  HKickGrid fGridANeg;     //Grid for Pt and neg. parts
  HKickGrid fGridBNeg;     //Grid for Pt and neg. parts
  HKickGrid fGridCNeg;     //Grid for Pt and neg. parts

public:
  HKickPlane2(const char *name="KickPlane2",
	      const char *title="Parameter container for kickplane v2",
	      const char *context="KickPlane2Production");
  HKickPlane2(const HKickPlane2 &pl);
  virtual ~HKickPlane2(void);
  static HKickPlane2 *getMDC3(void);
  static HKickPlane2 *getMeta(void);
  void clear(void);
  void readFrom(const HKickPlane2 &pl);
  Bool_t init(HParIo *inp, Int_t *set);
  Int_t write(HParIo *io);
  Int_t writeFile(fstream *pFile);
  Bool_t read(fstream *pFile,Int_t* set,Int_t inputNumber);
  inline Double_t distanceTo(const HGeomVector &v);
  Float_t getFieldStrength() const { return fScaleFactor; }
  inline Bool_t calcIntersection(const HGeomVector &r,const HGeomVector &alpha,
				 HGeomVector &out) const;
  Double_t getP(const HGeomVector &pos, Double_t theta,const Int_t polarity);
  HSurface *getSurface(void) { return fSurface; }

  ClassDef(HKickPlane2,1)
};

//*********** Inlines
inline Bool_t HKickPlane2::calcIntersection(const HGeomVector &r,const HGeomVector &alpha,
				    HGeomVector &out) const {
  //FIXME: Devolver kFALSE para trazas que no cortan al plano.
  fSurface->calcIntersection(r,alpha,out);
  return kTRUE;
}

inline Double_t  HKickPlane2::distanceTo(const HGeomVector &v) {
  return fSurface->distanceTo( v );
}

#endif
