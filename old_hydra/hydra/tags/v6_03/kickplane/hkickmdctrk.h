#ifndef HKICKMDCTRK_H
#define HKICKMDCTRK_H

#include <TObject.h>
#include "hgeomvector.h"

class HKickMdcTrk : public TObject {
 protected:
  Float_t x0,y0,z0; // Target position
  Float_t xKick,yKick,zKick; //Position in kickplane
  Float_t xOut,yOut,zOut; //Position in outer chambers
  Int_t charge; 
  Float_t p; // Momentum
  Float_t pull; // Correlation test variable
  Float_t kickDist;
  Float_t dist;
  Int_t flag;
  Char_t sector;
 public:
  HKickMdcTrk(void) { reset(); }
  ~HKickMdcTrk(void) {}
  Int_t getFlag(void) { return flag; }
  void setFlag(Int_t f) { flag = f; }
  void setSector(Char_t s) {sector = s;}
  inline void setZeroPos(const HGeomVector &v);
  inline void setKickPos(const HGeomVector &v);
  inline void setOuterPos(const HGeomVector &v);
  inline void setCharge(Int_t c);
  inline void setMomentum(Float_t aP);
  inline void setPull(Float_t aPull);
  void setKickDist(Float_t f) { kickDist = f; }
  void setDist(Float_t f) { dist = f;} 
  inline HGeomVector getZeroPos(void);
  inline HGeomVector getKickPos(void);
  inline HGeomVector getOuterPos(void);
  inline HGeomVector getInnerAlpha(void);
  inline HGeomVector getOuterAlpha(void);
  inline Int_t getCharge(void);
  inline Float_t getMomentum(void);
  inline Float_t getPull(void);
  inline Float_t getPath(void);
  Float_t getKickDist(void) { return kickDist; }
  Float_t getDist(void) { return dist;} 
  Char_t getSector(void) { return sector; }
  inline void reset(void);
  ClassDef(HKickMdcTrk,0) //Result of matching segments in mag. field
};

//----- Inline functions
void HKickMdcTrk::setZeroPos(const HGeomVector &v) {
  x0=v.getX();
  y0=v.getY();
  z0=v.getZ();
}   

void HKickMdcTrk::setKickPos(const HGeomVector &v) {
  xKick=v.getX();
  yKick=v.getY();
  zKick=v.getZ();
}

void HKickMdcTrk::setOuterPos(const HGeomVector &v) {
  xOut=v.getX();
  yOut=v.getY();
  zOut=v.getZ();
}

void HKickMdcTrk::setCharge(Int_t c) {
  charge=c;
}

void HKickMdcTrk::setMomentum(Float_t aP) {
  p=aP;
}

void HKickMdcTrk::setPull(Float_t aPull) {
  pull=aPull;
}

HGeomVector HKickMdcTrk::getZeroPos(void) {
  return HGeomVector(x0,y0,z0);
}

HGeomVector HKickMdcTrk::getKickPos(void) {
  return HGeomVector(xKick,yKick,zKick);
}

HGeomVector HKickMdcTrk::getOuterPos(void) {
  return HGeomVector(xOut,yOut,zOut);
}

HGeomVector HKickMdcTrk::getInnerAlpha(void) {
  HGeomVector v(xKick - x0, yKick - y0, zKick - z0);
  v /= v.length();
  return v;
}

HGeomVector HKickMdcTrk::getOuterAlpha(void) {
  HGeomVector v(xOut - xKick - x0, yOut - yKick, zOut - zKick);
  v /= v.length();
  return v;
}

Int_t HKickMdcTrk::getCharge(void) {
  return charge;
}

Float_t HKickMdcTrk::getMomentum(void) {
  return p;
}

Float_t HKickMdcTrk::getPull(void) {
  return pull;
}

Float_t HKickMdcTrk::getPath(void) {
  Float_t d = TMath::Sqrt( (xKick - x0)*(xKick - x0) +
			   (yKick - y0)*(yKick - y0) +
			   (zKick - z0)*(zKick - z0) );

  d += TMath::Sqrt( (xOut - xKick)*(xOut - xKick) +
		    (yOut - yKick)*(yOut - yKick) +
		    (zOut - zKick)*(zOut - zKick) );
  return d;
}

void HKickMdcTrk::reset(void) {
  x0 = y0 = z0 = 0.; // Target position
  xKick = yKick = zKick = 0.; //Position in kickplane
  xOut = yOut = zOut = 0.; //Position in outer chambers
  charge = 0; 
  p = 0.; // Momentum
  pull = 0.; // Correlation test variable
}

#endif
