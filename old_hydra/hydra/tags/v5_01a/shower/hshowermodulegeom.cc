#pragma implementation
#include <iostream.h>
#include "hshowermodulegeom.h"

ClassImp(HShowerModuleGeom)


HShowerModuleGeom::HShowerModuleGeom() {
  reset();
} // eof constructor


HShowerModuleGeom::~HShowerModuleGeom() {
  reset();
} // eof destructor

void HShowerModuleGeom::reset()
{
  m_nModuleID = -1;

  m_pFrame = NULL;
  m_pWires = NULL;
  m_pPads = NULL;

  m_pReadParam = NULL;

}

Bool_t HShowerModuleGeom::initAscii(HParHadAsciiFileIo* pHadAsciiFile) {
  if (!pHadAsciiFile)
     return kFALSE;

  char buf[80];
  Float_t matrixRot[9];
  Float_t transVect[3];

  HAsciiKey* pHAscii = pHadAsciiFile->GetKeyAscii();
  HAsciiKey &mydata = *pHAscii;

  mydata.ReadFloat((Float_t*)matrixRot, "Rotation Matrix");

  sprintf(buf, "Center Module %d", m_nModuleID);
  mydata.ReadFloat((Float_t*)transVect, buf);
 
  transform.setRotMatrix(matrixRot);
  transform.setTransVector(transVect);
 
  return kTRUE;
}

Bool_t HShowerModuleGeom::writeAscii(HParHadAsciiFileIo* pHadAsciiFileIo) {
  if (!pHadAsciiFileIo)
     return kFALSE;

  char buf[80];
  Float_t transVect[3];

  HAsciiKey* pHAscii = pHadAsciiFileIo->GetKeyAscii();
  HAsciiKey &mydata = *pHAscii;

  transVect[0]=transform.getTransVector().getX();
  transVect[1]=transform.getTransVector().getY();
  transVect[2]=transform.getTransVector().getZ();

  sprintf(buf, "Center Module %d", m_nModuleID);
  mydata.WriteFloat(buf, 3, (Float_t*)transVect);

  return kTRUE;
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
      m_pFrame = NULL;
      m_pWires = NULL;
      m_pPads = NULL;
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


