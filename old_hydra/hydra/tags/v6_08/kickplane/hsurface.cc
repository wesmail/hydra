#include "hsurface.h"
#include "hgeomvector.h"

//*-- Author : M. Sanchez (14/04/2000)

//////////////////////////////////////////////
// HSurface
//  The HSurface classes are used by the kickplane tracking to abstract
// the actual shape of the kickplane. This class declares a essential method
// calcIntersection which calculates the intersection of a straight line with the 
// given surface.
//////////////////
HSurfPlane::HSurfPlane(const HSurfPlane &pl) {
  fDx = pl.fDx; 
  fDz = pl.fDz;
  fC  = pl.fC;
}

HSurface *HSurfPlane::clone(void) {
  return new HSurfPlane(*this);
}

Double_t HSurfPlane::distanceTo(const HGeomVector &v) {
  Double_t d;
  d = v.getY() - fDx * v.getX() - fDz * v.getZ() - fC;
  d /= TMath::Sqrt( 1 + fDx*fDx + fDz*fDz);
  return TMath::Abs( d );
}



void HSurfPlane::calcIntersection(const HGeomVector &r,const HGeomVector &alpha,
				  HGeomVector &out) const {
  //Calculates the intersenction of a straight line with this plane and puts the result
  //in "out"
  Double_t bx=alpha.getX()/alpha.getZ();
  Double_t by=alpha.getY()/alpha.getZ();

  Double_t dx = fDx;

  out.setZ( (by*r.getZ() - r.getY() + fC + dx*r.getX() - dx*bx*r.getZ()) /
	    (by - fDz - dx*bx));
  out.setX(r.getX() + bx*(out.getZ() - r.getZ()));
  out.setY(r.getY() + by*(out.getZ() - r.getZ()));
}

////////////////////////////////////////////////////////////
// HSurfPlaneV
///////////////////////////////////////////////////////////

HSurfPlaneV::HSurfPlaneV(const HSurfPlaneV &pl) {
  fDx = pl.fDx; 
  fDz = pl.fDz;
  fC  = pl.fC;
}

HSurface *HSurfPlaneV::clone(void) {
  return new HSurfPlaneV(*this);
}

Double_t HSurfPlaneV::distanceTo(const HGeomVector &v) {
  Double_t d,normod;

  normod = TMath::Sqrt( 1 + fDx*fDx + fDz*fDz);

  //FIXME: Can be optimized by taking a likely first guess from sign of v.x
  //Let's supose the point of closes approach has x>0. Then
  d = v.getY() - fDx * v.getX() - fDz * v.getZ() - fC;
  d /= normod;
  if ((v.getX() - fDx * d / normod)<0) {
    //The assumption was wrong. The x of closest approach<0  ==> recalculate
    d = v.getY() + fDx * v.getX() - fDz * v.getZ() - fC;
    d /= normod;
  }

  return TMath::Abs( d );
}



void HSurfPlaneV::calcIntersection(const HGeomVector &r,const HGeomVector &alpha,
				  HGeomVector &out) const {
  //Calculates the intersenction of a straight line with this plane and puts the result
  //in "out"
  Double_t bx=alpha.getX()/alpha.getZ();
  Double_t by=alpha.getY()/alpha.getZ();

  out.setZ( (by*r.getZ() - r.getY() + fC + fDx*r.getX() - fDx*bx*r.getZ())/
	    (by - fDz - fDx * bx));
  out.setX(r.getX() + bx * (out.getZ() - r.getZ()));
  if (out.getX() < 0.) {
    out.setZ( (by*r.getZ() - r.getY() + fC - fDx*r.getX() + fDx*bx*r.getZ())/
	      (by - fDz + fDx * bx));   
    out.setX(r.getX() + bx * (out.getZ() - r.getZ()));
  }
  out.setY(r.getY() + by * (out.getZ() - r.getZ()));
}

////////////////////////////////////////////////////////////
// HSurfXCilinder
///////////////////////////////////////////////////////////
Double_t HSurfXCilinder::distanceTo(const HGeomVector &v) {
  cerr << "HSurfXCilinder::distanceTo not implemented" << endl;
  return 0.;
}

HSurfXCilinder::HSurfXCilinder(const HSurfXCilinder &ci) {
  fDx = ci.fDx; 
  fDz = ci.fDz;
  fC  = ci.fC;
}

HSurface *HSurfXCilinder::clone(void) {
  return new HSurfXCilinder(*this);
}

void HSurfXCilinder::calcIntersection(const HGeomVector &r,
				      const HGeomVector &alpha,
				      HGeomVector &out) const {
  Double_t bx = alpha.getX() / alpha.getZ();
  Double_t by = alpha.getY() / alpha.getZ();
  Double_t A = fDx * bx*bx;
  Double_t delta;
  if (A == 0.) {
    delta = fDx*r.getX()*r.getX() + fDz*r.getZ() + fC - r.getY();
    delta /= by - fDz;
  } else {
    Double_t B = 2 * fDx * r.getX() * bx - by + fDz;
    Double_t C = fC + fDz * r.getZ() + fDx * r.getX()*r.getX() - r.getY();
    Double_t rad = TMath::Sqrt(B*B - 4 * A * C);
    delta = (-B - rad) / (2 * A);
    // cout << r.getZ() + (-B + rad) / (2 * A) << "  \t<-->\t";
    // cout << r.getZ() + (-B - rad) / (2 * A) << endl;
  }
  out.setZ(r.getZ() + delta);
  out.setX(r.getX() + bx * delta);
  out.setY(r.getY() + by * delta);
}


ClassImp(HSurface)
ClassImp(HSurfPlane)
ClassImp(HSurfPlaneV)
ClassImp(HSurfXCilinder)
