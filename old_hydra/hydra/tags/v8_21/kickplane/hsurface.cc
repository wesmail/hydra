#include "hsurface.h"
#include "hgeomvector.h"

//*-- Author : M. Sanchez (14/04/2000)

//_HADES_CLASS_DESCRIPTION 
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

///////////////////////////////////////////////////////////////////
// HSurfPlaneTriV
///////////////////////////////////////////////////////////////////
HSurfPlaneTriV::HSurfPlaneTriV(void) {
  fDzLimit[0] = 400;
  fDzLimit[1] = 1100;
  fDzLimit[2] = 1e6;
}

HSurfPlaneTriV::HSurfPlaneTriV(const HSurfPlaneTriV &pl) {
  fDx = pl.fDx; 
  for (Int_t i=0;i<3;i++) {
    fDz[i] = pl.fDz[i];
    fDzLimit[i] = pl.fDzLimit[i]; 
  }
  fC[0]  = pl.fC[0];
  calcC();
}

HSurface *HSurfPlaneTriV::clone(void) {
  return new HSurfPlaneTriV(*this);
}

void HSurfPlaneTriV::printTo(ostream &out) { 
  out << fDx << " " << fDz[0] << " " << fDz[1] << " " << fDz[2] << " "<< fC[0] << endl; 
}

void HSurfPlaneTriV::readFrom(istream &in) { 
  in >> fDx >> fDz[0] >> fDz[1] >> fDz[2] >> fC[0];
  calcC();
}

void HSurfPlaneTriV::calcC(void) {
  fC[1] = fC[0] + (fDz[0]-fDz[1])*fDzLimit[0];
  fC[2] = fC[1] + (fDz[1]-fDz[2])*fDzLimit[1];
}

Double_t HSurfPlaneTriV::distanceTo(const HGeomVector &v) {
  //This code needs a bit of explanation. 
  // For each subplane the point of closest approach is computed
  // if this point is in the subplane, the corresponding distance is used
  // If not a check is run to ensure the point is in the intersection 
  // between a plane and the previous, if so. The point of closest approach
  // is recalculated and the distance computed

  Double_t d,normod;
  Double_t xc,yc,zc; // z of closest approach

  //FIXME: Can be optimized by taking a likely first guess from sign of v.x
  //Let's supose the point of closes approach has x>0. Then
  for (Int_t i=0;i<3;i++) {
    normod = TMath::Sqrt( 1 + fDx*fDx + fDz[i]*fDz[i]);
    d = v.getY() - fDx * v.getX() - fDz[i] * v.getZ() - fC[i];
    d /= normod;
    if ((v.getX() - fDx * d / normod)<0) {
      //The assumption was wrong. The x of closest approach<0  ==> recalculate
      d = v.getY() + fDx * v.getX() - fDz[i] * v.getZ() - fC[i];
      d /= normod;
    }
    zc = v.getZ() - fDz[i] * d /normod;
    if (zc<fDzLimit[i]) {
      if (i==0 || zc>fDzLimit[i-1]) {
	break;
      } else { //Point of closest approach must be in the intersection of the 2 planes
	zc = fDzLimit[i];
	xc = (v.getX() - fDx * d / normod);
	yc = fDz[i]*zc+fC[i];
	yc += (xc>0)?fDx:-fDx;
	
	d = sqrt( (v.getX()-xc)*(v.getX()-xc)  + (v.getY()-yc)*(v.getY()-yc) + 
		  (v.getZ()-zc)*(v.getZ()-zc) );
	break;
      }
    }
  }

  return TMath::Abs( d );
}



void HSurfPlaneTriV::calcIntersection(const HGeomVector &r,const HGeomVector &alpha,
				  HGeomVector &out) const {
  //Calculates the intersenction of a straight line with this plane and puts the result
  //in "out"
  Double_t bx=alpha.getX()/alpha.getZ();
  Double_t by=alpha.getY()/alpha.getZ();

  Double_t numconst = by*r.getZ() - r.getY();
  Double_t numdx =  fDx*r.getX() - fDx*bx*r.getZ();
  
  for (Int_t i=0;i<3;i++) {
    out.setZ( (numconst + fC[i] + numdx)/ (by - fDz[i] - fDx * bx));
    out.setX(r.getX() + bx * (out.getZ() - r.getZ()));
    if (out.getX() < 0.) {
      out.setZ( (numconst + fC[i] - numdx)/(by - fDz[i] + fDx * bx));   
      out.setX(r.getX() + bx * (out.getZ() - r.getZ()));
    }
    if (out.getZ()<fDzLimit[i]) break;
  }
  out.setY(r.getY() + by * (out.getZ() - r.getZ()));
}



ClassImp(HSurface)
ClassImp(HSurfPlane)
ClassImp(HSurfPlaneV)
ClassImp(HSurfPlaneTriV)
ClassImp(HSurfXCilinder)
