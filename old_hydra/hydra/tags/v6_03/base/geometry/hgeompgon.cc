//*-- AUTHOR : Ilse Koenig
//*-- Modified : 26/11/2001 by Ilse Koenig

///////////////////////////////////////////////////////////////////////////////
//
// HGeomPgon
//
// class for the ROOT shape TPGON which is identical with the GEANT shape PGON
// 
// The size of a pgon is defined by a variable number of 'points'.
//   point 0:   number of planes perpendicular to the z axis where the
//              dimension of the section is given;
//   point 1:   azimutal angle phi at which the volume begins,
//              opening angle dphi of the volume,
//              number of sides of the cross section between the phi-limits;
//   point 2ff: z coordinate of the section,
//              inner radius at position z,
//              outer radius at position z;
//
// The intrinsic coordinate system of a PGON, which sits in the CAVE and is
// not rotated, is identical with the laboratory system.
//
///////////////////////////////////////////////////////////////////////////////

#include "hgeompgon.h"
#include "hgeomvolume.h"
#include "hgeomvector.h"
#include "TPGON.h"

ClassImp(HGeomPgon)

HGeomPgon::HGeomPgon() {
  // constructor
  fName="PGON";
  nPoints=0;
  nParam=0;
}


HGeomPgon::~HGeomPgon() {
  // default destructor
  if (param) { delete param; param=0; }
  if (intrinsicRot) { delete intrinsicRot; intrinsicRot=0; }
  if (center) { delete center; center=0; }
  if (position) { delete position; position=0; }
}


Int_t HGeomPgon::readPoints(fstream* pFile,HGeomVolume* volu) {
  // reads the 'points' decribed above from ascii file and stores them in the
  // array 'points' of the volume
  // returns the number of points
  if (!pFile) return 0;
  Double_t x,y,z;
  const Int_t maxbuf=155;
  Text_t buf[maxbuf];
  pFile->getline(buf,maxbuf);
  Int_t n;
  sscanf(buf,"%i",&n);
  if (n<=2) return 0;
  nPoints=n+2;
  if (volu->getNumPoints()!=nPoints) volu->createPoints(nPoints);
  volu->setPoint(0,(Double_t)n,0.0,0.0);
  for(Int_t i=1;i<nPoints;i++) {
    pFile->getline(buf,maxbuf);
    sscanf(buf,"%lf%lf%lf",&x,&y,&z);
    volu->setPoint(i,x,y,z);
  }
  return nPoints;
}


Bool_t HGeomPgon::writePoints(fstream* pFile,HGeomVolume* volu) {
  // writes the 'points' decribed above to ascii file
  if (!pFile) return kFALSE;  
  Text_t buf[155];
  for(Int_t i=0;i<volu->getNumPoints();i++) {
    HGeomVector& v=*(volu->getPoint(i));
    if (i!=0) sprintf(buf,"%9.3f%10.3f%10.3f\n",v(0),v(1),v(2));
    else sprintf(buf,"%3i\n",(Int_t)v(0));
    pFile->write(buf,strlen(buf));
  }
  return kTRUE;
}


void HGeomPgon::calcVoluParam(HGeomVolume* volu) {
  // calculates the parameters needed to create the ROOT shape TPGON 
  Double_t fac=10.;
  nPoints=volu->getNumPoints();
  nParam=nPoints*3-2;
  if (param && param->GetSize()!=nParam) {
    delete param;
    param=0;
  }
  if (!param) param=new TArrayD(nParam);
  HGeomVector& v1=*(volu->getPoint(1));
  Int_t k=0;
  param->AddAt(v1(0),k++);
  param->AddAt(v1(1),k++);
  param->AddAt(v1(2),k++);
  param->AddAt((nPoints-2),k++);
  for(Int_t i=2;i<nPoints;i++) {
    HGeomVector& v=*(volu->getPoint(i));
    param->AddAt(v(1)/fac,k++);
    param->AddAt(v(2)/fac,k++);
    param->AddAt(v(0)/fac,k++);
  }
} 


TShape* HGeomPgon::createRootVolume(HGeomVolume* volu) {
  // creates a volume with ROOT shape TPGON
  calcVoluParam(volu);
  Text_t* name=(char*)volu->GetName();
  TArrayD& p=*param;
  Int_t nz=(Int_t) p[3];
  TShape* s=new TPGON(name,name,"",(Float_t)(p[0]),(Float_t)(p[1]),
                      (Int_t)(p[2]),nz);
  Int_t k=3;
  for(Int_t i=0;i<nz;i++) {
    ((TPGON*)s)->DefineSection(i,(Float_t)(p[++k]),(Float_t)(p[++k]),
                              (Float_t)(p[++k]));
  }
  return s;
}


void HGeomPgon::calcVoluPosition(HGeomVolume*,
          const HGeomTransform& dTC,const HGeomTransform& mTR) {
  // calls the function posInMother(...) to calculate the position of the ROOT
  // volume in its mother 
  if (!intrinsicRot) intrinsicRot=new HGeomRotation();
  if (!center) center=new HGeomTransform();
  if (!position) position=new HGeomTransform();
  center->clear();
  posInMother(dTC,mTR);
}
