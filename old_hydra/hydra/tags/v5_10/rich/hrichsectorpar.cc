// File: hrichsectorpar.cc
// ***************************************************************************

//*-- Author : Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
//*-- Modified : 1999/12/04 by Witold Przygoda (przygoda@psja1.if.uj.edu.pl)


#include <iostream.h>
#include "hrichsectorpar.h"

// ***************************************************************************
//////////////////////////////////////////////////////////////////////////////
//
//  HRichSectorPar
//
//  Sector (module) parameters.
//
//////////////////////////////////////////////////////////////////////////////
// ***************************************************************************


ClassImp(HRichSectorPar)

//----------------------------------------------------------------------------
HRichSectorPar::HRichSectorPar() {
  reset();
} // eof constructor
//============================================================================

//----------------------------------------------------------------------------
HRichSectorPar::~HRichSectorPar() {
  reset();
} // eof destructor
//============================================================================

//----------------------------------------------------------------------------
void HRichSectorPar::reset()
{
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichSectorPar::initAscii(HParHadAsciiFileIo* pHadAsciiFile) {

  if (!pHadAsciiFile) return kFALSE;

  char buf[80];

  sprintf(buf, "Rich Sector %d parameters", fSectorId);

  HAsciiKey &mydata = *pHadAsciiFile->GetKeyAscii();

  mydata.SetActiveSection(buf);

  fRotationAngle = mydata.ReadFloat("Rotation angle");
  mydata.ReadFloat((Float_t*)fRotationMatrix, "Rotation matrix");
  mydata.ReadFloat(fSectorCentre, "Sector centre");

  transform.setRotMatrix(fRotationMatrix);
  transform.setTransVector(fSectorCentre);


  return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichSectorPar::writeAscii(HParHadAsciiFileIo* pHadAsciiFile) {

  if (!pHadAsciiFile) return kFALSE;

  char buf[80];

  sprintf(buf, "Rich Sector %d parameters", fSectorId);

  HAsciiKey &mydata = *pHadAsciiFile->GetKeyAscii();

  mydata.WriteSection(buf);

  mydata.WriteFloat("Rotation angle",fRotationAngle);
  mydata.SetNumInLine(3);
  mydata.WriteFloat("Rotation matrix*",9,fRotationMatrix);
  mydata.WriteFloat("Sector centre",3,fSectorCentre);


 return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
//void HShowerModuleGeom::transformModule(HGeomTransform& t) {
//  transform.transTo(t);
//}
//============================================================================

//----------------------------------------------------------------------------
void HRichSectorPar::transformVector(HGeomTransform& t) {
 HGeomVector v;
 HGeomVector vT;

 v = transform.getTransVector();
 vT = t.transFrom(v);
 transform.setTransVector(vT);
}
//============================================================================

//----------------------------------------------------------------------------
//void HShowerSectorGeom::translateModules() {
//  int i;
//  for(i = 0; i < m_nModules; i++)
//     getModule(i)->transformVector(transform);
//}
//============================================================================

//----------------------------------------------------------------------------
void HRichSectorPar::rotateSector() {
  //int i;
  HGeomTransform tt;
  HGeomRotation rot;

  rot.setEulerAngles(0, 0, getRotationAngle());
  tt.setRotMatrix(rot);
  transformSector(tt);
  //for(i = 0; i < m_nModules; i++)
  //    getModule(i)->transformModule(tt);
}
//============================================================================

//----------------------------------------------------------------------------
void HRichSectorPar::transformSector(HGeomTransform& tt) {
  transform.transTo(tt);
}
//============================================================================



