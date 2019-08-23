//*-- AUTHOR : Ilse Koenig
//*-- Modified : 14/05/99 by Ilse Koenig

/////////////////////////////////////////////////////////////
// HGeomBasicShape
//
// Base class of the all shapes
//
/////////////////////////////////////////////////////////////

#include "hgeombasicshape.h"
#include "hgeomvolume.h"
#include "hgeomvector.h"
#include <iostream.h>

ClassImp(HGeomBasicShape)

HGeomBasicShape::HGeomBasicShape() {
  // default constructor
  nPoints=0;
  nParam=0;
  param=0;
}


HGeomBasicShape::~HGeomBasicShape() {
  // destructor
  if (param) { delete param; param=0; }
  if (intrinsicRot) { delete intrinsicRot; param=0; }
  if (center) { delete center; param=0; }
  if (position) { delete position; param=0; }
}


Int_t HGeomBasicShape::readPoints(fstream* pFile,HGeomVolume* volu) {
  // reads nPoints with 3 components from Ascii file
  // if the array of points is not existing in the volume it is created and
  // the values are stored inside
  // returns the number of points
  if (!pFile) return 0;
  Bool_t store=kFALSE;
  if (volu->getNumPoints()==0) {
    volu->createPoints(nPoints);
    store=kTRUE;
  }
  Double_t x,y,z;
  const Int_t maxbuf=155;
  Text_t buf[maxbuf];
  for(Int_t i=0;i<nPoints;i++) {
    pFile->getline(buf,maxbuf);
    sscanf(buf,"%lf%lf%lf",&x,&y,&z);
    if (store) volu->setPoint(i,x,y,z);
  }  
  return nPoints;
}
   

Bool_t HGeomBasicShape::writePoints(fstream* pFile,HGeomVolume* volu) {
  // writes nPoints 3 components to Ascii file
  if (!pFile) return kFALSE;  
  Text_t buf[155];
  for(Int_t i=0;i<volu->getNumPoints();i++) {
    HGeomVector& v=*(volu->getPoint(i));
    sprintf(buf,"%9.3f%10.3f%10.3f\n",v(0),v(1),v(2));
    pFile->write(buf,strlen(buf));
  }
  return kTRUE;
}


void HGeomBasicShape::posInMother(const HGeomTransform& dTC,
                                  const HGeomTransform& mTR) {
  position->setRotMatrix(center->getRotMatrix());
  position->setTransVector(center->getTransVector());
  position->transFrom(dTC);
  position->transTo(mTR);
  HGeomVector t(position->getTransVector());
  position->setTransVector(t*=0.1);
}


void HGeomBasicShape::printParam() {
  if (param) {
    for (Int_t i=0;i<nParam;i++) cout<<param->At(i)<<" ";
    cout<<'\n';
  }
}




