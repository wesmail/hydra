#pragma implementation
using namespace std;
#include <iostream> 
#include <iomanip>
#include "hshowermodulegeom.h"
#include "hshowersectorgeom.h"

ClassImp(HShowerSectorGeom)

/////////////////////////////////////////////////////////////
// HShowerSectorGeom
//
// Class contains information about lab coordinates 
// of set of shower detectors.
// There are informations about each modules in one sector 
// and information of geometrical transformation to lab coordinates
// 
/////////////////////////////////////////////////////////////


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
//reading and initializing lab coordinates of one sector from ascii file (HAsciiKey format)

  Bool_t status = kTRUE;
  if (!pHadAsciiFile)
     return kFALSE;

  char buf[80];
  Float_t matrixRot[9];
  Float_t transVect[3];
  int mod;

  try {
 
     sprintf(buf, "Shower Sector%d Geometry Data", m_nSectorID);

     HAsciiKey* pHAscii = pHadAsciiFile->GetKeyAscii();
     HAsciiKey &mydata = *pHAscii;

     mydata.SetActiveSection(buf);

     mydata.ReadFloat((Float_t*)matrixRot, "Rotation Matrix");
     mydata.ReadFloat((Float_t*)transVect, "Center Sector");

     mydata.ReadInt(&mod, "Modules");
     mydata.ReadFloat(&angleRot, "Rotation Angle");
   
     transform.setRotMatrix(matrixRot);
     transform.setTransVector(transVect);

     for(int i = 0; i < mod; i++) {
       HShowerModuleGeom *pModGeom = new HShowerModuleGeom;
       pModGeom->setModuleID(i);
       if (!pModGeom->initAscii(pHadAsciiFile))
         return kFALSE;
       addModule(pModGeom);
     }
  }
  catch (Bool_t ret) {
    status = ret;
  }

  return status;
}

Bool_t HShowerSectorGeom::writeAscii(HParHadAsciiFileIo* pHadAsciiFileIo) {
//writing lab coordinates of one sector into ascii file (HAsciiKey format)
  Bool_t status = kTRUE;
  char buf[80];
  Float_t matrixRot[9];
  Float_t transVect[3];

  try {

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
  }
  catch (Bool_t ret) {
    status = ret;
  }

  return status;
}

HShowerModuleGeom* HShowerSectorGeom::getModule(Int_t n)
{
//return information about module n
   if ((n < 0) || (n >= m_nModules))
      return NULL;

   return (HShowerModuleGeom*)m_ModuleArr.At(n);
}


Int_t HShowerSectorGeom::setModule(HShowerModuleGeom* pModule, Int_t n)
{
//set information about module n

   if ((n < 0) || (n >= m_nModules))
      return 0;

   if (m_ModuleArr.At(n))
      delete m_ModuleArr.At(n);
   m_ModuleArr.AddAt(pModule, n);

   return 1;
}

Int_t HShowerSectorGeom::addModule(HShowerModuleGeom* pModule)
{
//add new information about module
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
