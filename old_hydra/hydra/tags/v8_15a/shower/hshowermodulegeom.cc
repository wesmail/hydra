#pragma implementation
using namespace std;
#include <iostream> 
#include <iomanip>
#include "hshowermodulegeom.h"

ClassImp(HShowerModuleGeom)

//_HADES_CLASS_DESCRIPTION 
/////////////////////////////////////////////////////////////
// HShowerModuleGeom
//
// Class contains information about lab coordinates of one module
// There are information of geometrical transformation to lab coordinates
//
/////////////////////////////////////////////////////////////


HShowerModuleGeom::HShowerModuleGeom() {
//default ctor
  reset();
} // eof constructor


HShowerModuleGeom::~HShowerModuleGeom() {
  reset();
} // eof destructor

void HShowerModuleGeom::reset()
//clearing data
{
  m_nModuleID = -1;

  m_pFrame = NULL;
  m_pWires = NULL;
  m_pPads = NULL;

}

Bool_t HShowerModuleGeom::initAscii(HParHadAsciiFileIo* pHadAsciiFile) {
//reading and initializing lab coordinates of one module from ascii file (HAscii Key format)

  Bool_t status = kTRUE;
  if (!pHadAsciiFile)
     return kFALSE;

  char buf[80];
  Float_t matrixRot[9];
  Float_t transVect[3];

  try {

     HAsciiKey* pHAscii = pHadAsciiFile->GetKeyAscii();
     HAsciiKey &mydata = *pHAscii;

     mydata.ReadFloat((Float_t*)matrixRot, "Rotation Matrix");

     sprintf(buf, "Center Module %d", m_nModuleID);
     mydata.ReadFloat((Float_t*)transVect, buf);
 
     transform.setRotMatrix(matrixRot);
     transform.setTransVector(transVect);
  }
  catch (Bool_t ret) {
    status = ret;
  }

  return status;
}

Bool_t HShowerModuleGeom::writeAscii(HParHadAsciiFileIo* pHadAsciiFileIo) {
//writing lab coordinates of one module into ascii file (HAsciiKey format)

  Bool_t status = kTRUE;
  if (!pHadAsciiFileIo)
     return kFALSE;

  char buf[80];
  Float_t transVect[3];

  try { 

     HAsciiKey* pHAscii = pHadAsciiFileIo->GetKeyAscii();
     HAsciiKey &mydata = *pHAscii;

     transVect[0]=transform.getTransVector().getX();
     transVect[1]=transform.getTransVector().getY();
     transVect[2]=transform.getTransVector().getZ();

     sprintf(buf, "Center Module %d", m_nModuleID);
     mydata.WriteFloat(buf, 3, (Float_t*)transVect);
  }
  catch (Bool_t ret) {
    status = ret;
  }

  return status;
}

//______________________________________________________________________________
void HShowerModuleGeom::Streamer(TBuffer &R__b)
{
   // Stream an object of class HShowerModuleGeom.

   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(); if (R__v) { }
      HParSet::Streamer(R__b);
      transform.Streamer(R__b);
      R__b >> m_nModuleID;
   } else {
      R__b.WriteVersion(HShowerModuleGeom::IsA());
      HParSet::Streamer(R__b);
      transform.Streamer(R__b);
      R__b << m_nModuleID;
      m_pFrame = NULL;   //It must to be initializated 
      m_pWires = NULL;   //after reading all geometrical data
      m_pPads = NULL;    //function attachTable is called in 
                         //HShowerParRootFile::read(HShowerGeometry* pPar,Int_t* set) function
   }
}

void HShowerModuleGeom::transformModule(HGeomTransform& t) {
  transform.transTo(t);
}

void HShowerModuleGeom::transformVector(HGeomTransform& t) {
 HGeomVector v;
 HGeomVector vT;

 v = transform.getTransVector();
 vT = t.transFrom(v);
 transform.setTransVector(vT);
}


