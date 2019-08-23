//*-- AUTHOR : Ilse Koenig
//*-- Modified : 28/06/99 by Ilse Koenig

///////////////////////////////////////////////////////////////////////////////
//
// HGeomTubs
//
// class for the ROOT shape TELTU which is identical with the
// GEANT shape ELTU
// 
// The size of a tubs is defined by 3 'points'.
//   point 0:   origin of starting ellipse of the eltu;
//   point 1:   semi-axis of the ellipse along x,
//              semi-axis of the ellipse along y;
//              (z-component not used)
//   point 2:   origin of ending ellipse of the eltu;
// Warning: The x- and y-values of point 0 and 2 have to be the same!!!!
//          A rotation has to be desribed via the rotation matrix.
//
// The intrinsic coordinate system of a TUBS, which sits in the CAVE and is
// not rotated, is identical with the laboratory system.
//
///////////////////////////////////////////////////////////////////////////////

#include "hgeomeltu.h"
#include "hgeomvolume.h"
#include "hgeomvector.h"
#include "TELTU.h"

ClassImp(HGeomEltu)

HGeomEltu::HGeomEltu() {
  // constructor
  fName="ELTU";
  nPoints=3;
  nParam=3;
}


HGeomEltu::~HGeomEltu() {
  // default destructor
  if (param) { delete param; param=0; }
  if (intrinsicRot) { delete intrinsicRot; intrinsicRot=0; }
  if (center) { delete center; center=0; }
  if (position) { delete position; position=0; }
}


Int_t HGeomEltu::readPoints(fstream* pFile,HGeomVolume* volu) {
  // reads the 3 'points' decribed above from ascii file
  // if the array of points is not existing in the volume it is created and
  // the values are stored inside
  // returns the number of points
  if (!pFile) return 0;
  if (volu->getNumPoints()!=nPoints) volu->createPoints(nPoints);
  Double_t x,y,z;
  const Int_t maxbuf=155;
  Text_t buf[maxbuf];
  for(Int_t i=0;i<nPoints;i++) {
    pFile->getline(buf,maxbuf);
    if (i!=1) {
      sscanf(buf,"%lf%lf%lf",&x,&y,&z);
      volu->setPoint(i,x,y,z);
    } else {
      sscanf(buf,"%lf%lf",&x,&y);
      volu->setPoint(i,x,y,0.0);
    }
  }
  return nPoints;
}


Bool_t HGeomEltu::writePoints(fstream* pFile,HGeomVolume* volu) {
  // writes the 3 'points' decribed above to ascii file
  if (!pFile) return kFALSE;  
  Text_t buf[155];
  for(Int_t i=0;i<nPoints;i++) {
    HGeomVector& v=*(volu->getPoint(i));
    if (i!=1) sprintf(buf,"%9.3f%10.3f%10.3f\n",v(0),v(1),v(2));
    else sprintf(buf,"%9.3f%10.3f\n",v(0),v(1));
    pFile->write(buf,strlen(buf));
  }
  return kTRUE;
}


void HGeomEltu::calcVoluParam(HGeomVolume* volu) {
  // calculates the parameters needed to create the ROOT shape TELTU
  Double_t fac=10.;
  if (param && param->GetSize()!=nParam) {
    delete param;
    param=0;
  }
  if (!param) param=new TArrayD(nParam);
  HGeomVector& v1=*(volu->getPoint(1));
  param->AddAt(v1(0)/fac,0);
  param->AddAt(v1(1)/fac,1);
  HGeomVector v=*(volu->getPoint(2)) - *(volu->getPoint(0));
  param->AddAt(TMath::Abs(v(2))/fac/2.,2);
} 


TShape* HGeomEltu::createRootVolume(HGeomVolume* volu) {
  // creates a volume with ROOT shape TELTU
  calcVoluParam(volu);
  Text_t* name=(char*)volu->GetName();
  TArrayD& p=*param;
  TShape* s=new TELTU(name,name,"",(Float_t)(p[0]),(Float_t)(p[1]),
                      (Float_t)(p[2]));
  return s;
}


void HGeomEltu::calcVoluPosition(HGeomVolume* volu,
          const HGeomTransform& dTC,const HGeomTransform& mTR) {
  // calculates the position of the center of the ROOT volume in the intrinsic
  // coordinate system and stores it in the data element 'center'
  // calls the function posInMother(...) to calculate the position of the ROOT
  // volume in its mother 
  if (!intrinsicRot) intrinsicRot=new HGeomRotation();
  if (!center) center=new HGeomTransform();
  if (!position) position=new HGeomTransform();
  Double_t t[3]={0.,0.,0.};
  HGeomVector v=*(volu->getPoint(2)) + *(volu->getPoint(0));
  t[2]=v(2)/2.;  
  center->setTransVector(t);
  center->setRotMatrix(*intrinsicRot);
  posInMother(dTC,mTR);
}
