//###################################################
//#
//#       
//#                      FRAMEWORK
//#
//#       Authors:                           W.Koenig
//#       adoption to framework & ROOT       W.Schoen
//#    
//#       last mod. W.Schoen 19.1.98
// ###################################################

#pragma implementation
#include "TString.h"
#include "fstream.h"
#include "hshowergeometry.h"
#include "hparhadasciifileio.h"
ClassImp(HShowerGeometry)

////////// ********** HShowerGeometry defintion ****************/////////////77
HShowerGeometry::HShowerGeometry() {
  SetName("ShowerGeometry");

  m_nModules = 3;
  m_nSectors = 6;
  m_nRows = 32;
  m_nColumns = 32;
  m_nLab = 0;
  m_pReadParam = NULL;

  for(int i = 0; i < 6; i++)
    sectors[i].setSectorID(i);

}

HShowerGeometry::~HShowerGeometry()
{
  clear(); 
}

void HShowerGeometry::attachTables() {
   int i,j;
  
   for(i = 0; i < 6; i++) 
     for(j = 0; j < 3; j++) {
       HShowerModuleGeom* pModGeom = sectors[i].getModule(j);
       pModGeom->setFrame(frames + j);
       pModGeom->setWires(wires + j);
       pModGeom->setPads(pads + j);
     }
 
   for(int i = 0; i < 3; i++) 
       pads[i].setFrame(&frames[i]);
}

Bool_t HShowerGeometry::initAscii(HParHadAsciiFileIo* pHadAsciiFile)
{
 if (!pHadAsciiFile) return kFALSE;

 HAsciiKey* pHAscii = pHadAsciiFile->GetKeyAscii();
 HAsciiKey &mydata = *pHAscii;

 if(!mydata.SetActiveSection(" Shower General Geometry Data "))
    return kFALSE;

 mydata.ReadInt(&m_nSectors, "Sectors");
 mydata.ReadInt(&m_nModules, "Modules");
 mydata.ReadInt(&m_nRows, "Rows");
 mydata.ReadInt(&m_nColumns, "Columns");
 mydata.ReadInt(&m_nLab, "Labolatory");

 for(int i = 0; i < 6; i++) 
     sectors[i].initAscii(pHadAsciiFile);

 for(int i = 0; i < 3; i++) {
     frames[i].setModuleID(i);
     frames[i].initAscii(pHadAsciiFile);
 }

 for(int i = 0; i < 3; i++) { 
     wires[i].setModuleID(i);
     wires[i].initAscii(pHadAsciiFile);
 }

 for(int i = 0; i < 3; i++) {
     pads[i].setModuleID(i);
     pads[i].setFrame(&frames[i]);
     pads[i].initAscii(pHadAsciiFile);
 }

 attachTables();
 return kTRUE;
}

Bool_t HShowerGeometry::writeAscii(HParHadAsciiFileIo* pHadAsciiFile)
{
 if (!pHadAsciiFile) return kFALSE;

 HAsciiKey* pHAscii = pHadAsciiFile->GetKeyAscii();
 HAsciiKey &mydata = *pHAscii;

 if(!mydata.WriteSection(" Shower General Geometry Data "))
    return kFALSE;

 mydata.WriteInt("Sectors", m_nSectors);
 mydata.WriteInt("Modules", m_nModules);
 mydata.WriteInt("Rows", m_nRows);
 mydata.WriteInt("Columns", m_nColumns);
 mydata.WriteInt("Labolatory", m_nLab);

 for(int i = 0; i < 6; i++) 
     sectors[i].writeAscii(pHadAsciiFile);

 for(int i = 0; i < 3; i++) 
     frames[i].writeAscii(pHadAsciiFile);

 for(int i = 0; i < 3; i++)
     wires[i].writeAscii(pHadAsciiFile);

 for(int i = 0; i < 3; i++)
     pads[i].writeAscii(pHadAsciiFile);

 return kTRUE;
}

void HShowerGeometry::clear()
{
}

const HGeomTransform&  HShowerGeometry::getTransform(Int_t sect) {
  return sectors[sect].getTransform(); 
}

const HGeomTransform&  HShowerGeometry::getTransform(Int_t sect, Int_t mod) {
  return sectors[sect].getModule(mod)->getTransform(); 
}

void HShowerGeometry::getLocalCoord(HLocation& loc, HGeomVector& v) {
  float fX, fY;
  float scale = 10.0; //transform cm to mm

  HShowerPadTab* pPads = getPadParam(loc[1]);
  pPads->getPad(loc[2], loc[3])->getPadCenter(&fX, &fY);
  
  v.setX(scale*fX);
  v.setY(scale*fY);
  v.setZ(0.0);
}

void HShowerGeometry::getLabCoord(HLocation& loc, HGeomVector& v) {
  HGeomVector vLoc;
  HGeomTransform tt;

  getLocalCoord(loc, vLoc);
  tt = getTransform(loc[0], loc[1]);
  
  v = tt.transFrom(vLoc);
}

void HShowerGeometry::translModules() {
  sectors[0].translateModules();  //Only in sector 0
 				  //Sector 0 is reference to others
}

void HShowerGeometry::rotateSectors() {
  int i, j;

  HGeomTransform modt[3];
  HGeomTransform st;

  st = sectors[0].getTransform();
  for (j = 0; j < sectors[0].getModules(); j++)
    modt[j] = sectors[0].getModule(j)->getTransform();

  for(i = 1; i < m_nSectors; i++) {
    sectors[i].setTransform(st);
    for(j = 0; j < sectors[i].getModules(); j++)
        sectors[i].getModule(j)->setTransform(modt[j]);

    sectors[i].rotateSector();
  }
}

Bool_t HShowerGeometry::transToLab() {
  if (IsLab()) {
     printf("Geometry has been transformed to labolatory coordinates already\n");
     return kFALSE;
  }

  printf("Geometry is being transformed to labolatory coordinates now\n");

  translModules();
  rotateSectors();
  setLab();
  setChanged();

  return kTRUE;
}


