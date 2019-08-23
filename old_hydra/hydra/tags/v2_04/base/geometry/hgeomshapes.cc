//*-- AUTHOR : Ilse Koenig
//*-- Modified : 14/05/99 by Ilse Koenig

/////////////////////////////////////////////////////////////
// HGeomShapes
//
// Class to manage the ROOT geometry shapes
//
/////////////////////////////////////////////////////////////       

#include"hgeomshapes.h"
#include "hgeomvolume.h"
#include "hgeombasicshape.h"
#include "hgeombrik.h"
#include "hgeomtrap.h"
#include "hgeomtrd1.h"

ClassImp(HGeomShapes) 

HGeomShapes::HGeomShapes() {
  // constructor craetes empty list of shapes
  shapes=new TList();
}


HGeomShapes::~HGeomShapes() {
  // destructor deletes all shapes
  shapes->Delete();
  delete shapes;
}


HGeomBasicShape* HGeomShapes::selectShape(HGeomVolume * volu) {
  // returns a pointer to the shape used in given volume
  // calls selectShape(TString&) with the name of the shape
  const TString& name(volu->getShape());
  return selectShape(name);
}


HGeomBasicShape* HGeomShapes::selectShape(const TString& name) {
  // returns a pointer to the shape given by name
  // creates a shape object and adds it to the list of shapes if
  // not existing
  // return 0 if the corresponding shape class is not implemented 
  TString allShapes[3]={"BOX ","TRAP","TRD1"};  // later all possible shapes
  HGeomBasicShape* s=(HGeomBasicShape*)shapes->FindObject(name);
  if (s) return s;
  Int_t no=-1;
  for(Int_t i=0;i<3;i++) {if (name.CompareTo(allShapes[i])==0) no=i;}
  switch(no) {
    case 0:  {s= new HGeomBrik();  break;}
    case 1:  {s= new HGeomTrap();  break;}
    case 2:  {s= new HGeomTrd1();  break;}
    default: {
      cerr<<"shape "<<name<<" not yet implemented"<<endl;
    }
  }
  if (s) shapes->Add(s);
  return s;
}
 
 
Int_t HGeomShapes::readPoints(fstream* pFile,HGeomVolume* volu) {
  // reads the points of the given volume from the Ascii file
  // returns the number of points read
  // returns 0 if if the corresponding shape class is not implemented
  HGeomBasicShape* s=selectShape(volu);
  if (s) return s->readPoints(pFile,volu);
  else return 0;
}

  
Bool_t HGeomShapes::writePoints(fstream* pFile,HGeomVolume* volu) {
  // writes the points of the given volume to the Ascii file
  // return kFALSE if the corresponding shape class is not implemented
  HGeomBasicShape* s=selectShape(volu);
  if (s) return s->writePoints(pFile,volu);
  else return kFALSE;
}

  
HGeomBasicShape* HGeomShapes::calcVoluParam(HGeomVolume* volu) {
  // calculates the parameters needed to create the corresponding ROOT shape
  // and returns a pointer to the shape containing the parameters as data
  // elements
  // returns 0 if the corresponding shape class is not implemented
  HGeomBasicShape* s=selectShape(volu);
  if (s) return s->calcVoluParam(volu);
  else return 0;
}

 
HGeomBasicShape* HGeomShapes::calcVoluPosition(HGeomVolume* volu,
		 const HGeomTransform& mTR) {
  // calculates the parameters needed to position a ROOT volume in its
  // ROOT mother volume
  // mTR is the ROOT coordinate system of the mother volume relative to the
  //     technical coordinate system of the mother;
  // The function returns a pointer to the shape class and returns 0 if
  // the corresponding shape class is not implemented. 
  HGeomBasicShape* s=selectShape(volu);
  HGeomTransform& dTC=volu->getTransform();
  if (s) return s->calcVoluPosition(volu,dTC,mTR);
  return 0; 
}

HGeomBasicShape* HGeomShapes::calcVoluPosition(HGeomVolume* volu,
	     const HGeomTransform& dTC,const HGeomTransform& mTR) {
  // calculates the parameters needed to position a ROOT volume in its
  // ROOT mother volume
  // dTC is the technical daughter coordinate system relative to the
  //     technical coordinate system of its mother; this transformation,
  //     not the one in the volume, is used to calculate the position  
  //     parameters; used e.g. for modules where the volume decribes the
  //     module type which created only once but several copies of this
  //     volume are positioned individually 
  // mTR is the ROOT coordinate system of the mother volume relative to the
  //     technical coordinate system of the mother;
  // The function returns a pointer to the shape class and returns 0 if
  // the corresponding shape class is not implemented. 
  HGeomBasicShape* s=selectShape(volu); 
  if (s) return s->calcVoluPosition(volu,dTC,mTR);
  return 0; 
}

