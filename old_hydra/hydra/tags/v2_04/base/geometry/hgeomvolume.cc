//*-- AUTHOR : Ilse Koenig
//*-- Modified : 27/05/99

/////////////////////////////////////////////////////////////
// HGeomVolume
//
// Class to hold the basic geometry properties of a volume
// Each volume has a name, a pointer to a reference volume and
// a lab transformation.
// The reference volume defines the shape, the mother, the size
// and the transformation relative to the mother volume which
// is either the cave (for modules) or the detector (for the
// inner parts). 
// As an example:
// The Mdc modules built at GSI (plane 1 type) are all identical
// independent where the sit in the cave. This module type has a
// fixed coordinate system. The first layers in all these modules
// are identical and they have the same position in the module
// coordinate system.   
//
/////////////////////////////////////////////////////////////

#include "hgeomvolume.h"

ClassImp(HGeomVolume)

HGeomVolume::HGeomVolume(HGeomVolume& r) {
  // copy constructor
  fName=r.GetName();
  setVolumePar(r);
}

void HGeomVolume::setVolumePar(HGeomVolume& r) {
  // copies all volume parameters except the name
  shape=r.getShape();
  mother=r.getMother();
  nPoints=r.getNumPoints();
  createPoints(nPoints);
  for (Int_t i=0;i<nPoints;i++)
      (*points)[i]=new HGeomVector(*(r.getPoint(i)));
  transform=r.getTransform();
}

void HGeomVolume::createPoints(const Int_t n) {
  // creates n Points (objects of class HGeomVector)
  // if the array exists already the size is adjusted
  // if n==0 the points are deleted
  nPoints=n;
  if (n>0) {
    if (points==0) points=new TObjArray(n);
    else points->Expand(n);
    for(Int_t i=0;i<n;i++) (*points)[i]=new HGeomVector();
  }
  else {
    if (points) points->Delete();
    delete points;
    points=0;
  }
}

void HGeomVolume::setPoint(const Int_t n,const Double_t x,
                           const Double_t y,const Double_t z) {
  // set the 3 values of the point with index n
  if (points && n<nPoints) {
    HGeomVector* v=(HGeomVector*)points->At(n);
    v->setX(x);
    v->setY(y);
    v->setZ(z);
  }
}

void HGeomVolume::setPoint(const Int_t n,const HGeomVector& p) {
  // sets point with index n by copying the 3 components of point p 
  if (points && n<nPoints) {
    HGeomVector& v=*((HGeomVector*)points->At(n));
    v=p;
  }
}

void HGeomVolume::clear() {
  // clears the volume
  // deletes the points
  shape="";
  mother="";
  if (points) points->Delete();
  delete points;
  points=0;
  nPoints=0;
  transform.clear();
}

void HGeomVolume::print() {
  // prints all parameters of a volume
  cout<<((const char*)fName)<<"  "<<((const char*)shape)<<"  "
      <<((const char*)mother)<<'\n';
  if (points) {
    for (Int_t i=0;i<nPoints;i++)
        cout<<(*((HGeomVector*)points->At(i)));
  }
  transform.print();
  cout<<'\n';
}





