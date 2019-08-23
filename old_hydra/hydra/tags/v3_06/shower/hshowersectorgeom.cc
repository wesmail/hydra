#pragma implementation
#include <iostream.h>
#include "hshowermodulegeom.h"
#include "hshowersectorgeom.h"

ClassImp(HShowerSectorGeom)


HShowerSectorGeom::HShowerSectorGeom() {
  reset();
} // eof constructor


HShowerSectorGeom::~HShowerSectorGeom() {
  reset();
} // eof destructor

void HShowerSectorGeom::reset()
{
  m_nModules = 0;
  m_ModuleArr.Delete();
}

Bool_t HShowerSectorGeom::initAscii(HParHadAsciiFileIo* pHadAsciiFile) {
  if (!pHadAsciiFile)
     return kFALSE;

  char buf[80];
  Float_t matrixRot[9];
  Float_t transVect[3];
  int mod;

  sprintf(buf, "Shower Sector%d Geometry Data", m_nSectorID);

  HAsciiKey* pHAscii = pHadAsciiFile->GetKeyAscii();
  HAsciiKey &mydata = *pHAscii;

  if (!mydata.SetActiveSection(buf))
     return kFALSE;

  mydata.ReadFloat((Float_t*)matrixRot, "Rotation Matrix");
  mydata.ReadFloat((Float_t*)transVect, "Center Sector");

  mydata.ReadInt(&mod, "Modules");
  mydata.ReadFloat(&angleRot, "Rotation Angle");

  transform.setRotMatrix(matrixRot);
  transform.setTransVector(transVect);

  for(int i = 0; i < mod; i++) {
    HShowerModuleGeom *pModGeom = new HShowerModuleGeom;
    pModGeom->setModuleID(i);
    pModGeom->initAscii(pHadAsciiFile);
    addModule(pModGeom);
  }

  return kTRUE;
}

Bool_t HShowerSectorGeom::writeAscii(HParHadAsciiFileIo* pHadAsciiFileIo) {
  char buf[80];
  Float_t matrixRot[9];
  Float_t transVect[3];

  HAsciiKey* pHAscii = pHadAsciiFileIo->GetKeyAscii();
  HAsciiKey &mydata = *pHAscii;

  sprintf(buf, "Shower Sector%d Geometry Data", m_nSectorID);
  mydata.WriteSection(buf);
  mydata.SetNumInLine(3);

  mydata.WriteInt("Modules", m_nModules);
  mydata.WriteFloat("Rotation Angle", angleRot);

  for(int i=0; i < 9; i++)
    matrixRot[i]=transform.getRotMatrix()(i);

  transVect[0]=transform.getTransVector().getX();
  transVect[1]=transform.getTransVector().getY();
  transVect[2]=transform.getTransVector().getZ();

  mydata.WriteFloat("Rotation Matrix*", 9, (Float_t*)matrixRot);
  mydata.WriteFloat("Center Sector", 3, (Float_t*)transVect);

  for(int i = 0; i < getModules(); i++)
    getModule(i)->writeAscii(pHadAsciiFileIo);
  

  return kTRUE;
}

HShowerModuleGeom* HShowerSectorGeom::getModule(Int_t n)
{
   if ((n < 0) || (n >= m_nModules))
      return NULL;

   return (HShowerModuleGeom*)m_ModuleArr.At(n);
}


Int_t HShowerSectorGeom::setModule(HShowerModuleGeom* pModule, Int_t n)
{
   if ((n < 0) || (n >= m_nModules))
      return 0;

   if (m_ModuleArr.At(n))
      delete m_ModuleArr.At(n);
   m_ModuleArr.AddAt(pModule, n);

   return 1;
}

Int_t HShowerSectorGeom::addModule(HShowerModuleGeom* pModule)
{
  m_ModuleArr.Add(pModule);
  return m_nModules++;
}

void HShowerSectorGeom::translateModules() {
  int i;
  for(i = 0; i < m_nModules; i++)
     getModule(i)->transformVector(transform);
}

void HShowerSectorGeom::rotateSector() {
  int i;
  HGeomTransform tt;
  HGeomRotation rot;

  rot.setEulerAngles(0, 0, getAngleRot()); 
  tt.setRotMatrix(rot);
  transformSector(tt);
  for(i = 0; i < m_nModules; i++)
      getModule(i)->transformModule(tt);
}

void HShowerSectorGeom::transformSector(HGeomTransform& tt) {
  transform.transTo(tt);
}
