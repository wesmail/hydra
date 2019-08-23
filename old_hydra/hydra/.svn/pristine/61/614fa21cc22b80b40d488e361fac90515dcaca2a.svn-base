#pragma implementation
#include "TString.h"
#include "fstream.h"
#include "hshowergeometry.h"
#include "hparhadasciifileio.h"
ClassImp(HShowerGeometry)

/////////////////////////////////////////////////////////////
// HShowerGeometry
//
//  There is all information about shower detctor coordinates
//  in HShowerGeometry.
//
/////////////////////////////////////////////////////////////

HShowerGeometry::HShowerGeometry(const char* name,const char* title,
                                 const char* context)
                : HShowerParSet(name,title,context) {
//Create containers system for default parameters of shower geometry

  m_nModules = 3;
  m_nSectors = 6;
  m_nRows = 32;
  m_nColumns = 32;
  m_nLab = 0;
//  m_pReadParam = NULL;

  for(int i = 0; i < 6; i++)
    sectors[i].setSectorID(i);
}

HShowerGeometry::~HShowerGeometry()
{
  clear(); 
}

void HShowerGeometry::attachTables() {
//Make connections between local coordinates of frame, wires, pads
//to containers which contained inforamation about module position
//in labolatory 

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
//Reading and initializating data from ASCII file (HadAsciiKey format)

 Bool_t status = kTRUE;
 if (!pHadAsciiFile) return kFALSE;

 try {

    HAsciiKey* pHAscii = pHadAsciiFile->GetKeyAscii();
    HAsciiKey &mydata = *pHAscii;

    mydata.SetActiveSection(" Shower General Geometry Data ");

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
  }
  catch (Bool_t ret) {
    status = ret;
  }

  return status;
}

Bool_t HShowerGeometry::writeAscii(HParHadAsciiFileIo* pHadAsciiFile)
{
//Writing to ASCII file (HadAsciiKey format)

 Bool_t status = kTRUE;
 if (!pHadAsciiFile) return kFALSE;

 try {

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
        sectors[i].writeAscii(pHadAsciiFile);  //writing lab coord information

    for(int i = 0; i < 3; i++) 
        frames[i].writeAscii(pHadAsciiFile);  //writing local coord inforamtion

    for(int i = 0; i < 3; i++)
        wires[i].writeAscii(pHadAsciiFile);   

    for(int i = 0; i < 3; i++)
        pads[i].writeAscii(pHadAsciiFile);
  }
  catch (Bool_t ret) {
    status = ret;
  }

  return status;
}

void HShowerGeometry::clear()
{
}

const HGeomTransform&  HShowerGeometry::getTransform(Int_t sect) {
//return transformation data of sector sect
  return sectors[sect].getTransform(); 
}

const HGeomTransform&  HShowerGeometry::getTransform(Int_t sect, Int_t mod) {
//return transformation data of module mod in sector sect
  return sectors[sect].getModule(mod)->getTransform(); 
}

void HShowerGeometry::getLocalCoord(HLocation& loc, HGeomVector& v) {
//retrieve local coordinates of center of pads in position loc

  float fX, fY;
  float scale = 10.0; //transform cm to mm

  HShowerPadTab* pPads = getPadParam(loc[1]);
  pPads->getPad(loc[2], loc[3])->getPadCenter(&fX, &fY);
  
  v.setX(scale*fX);
  v.setY(scale*fY);
  v.setZ(0.0);  //z==0 local coordinates are in plain
}

void HShowerGeometry::getLabCoord(HLocation& loc, HGeomVector2& v) {
//retrieve lab coordinates of center of pads in position loc
// Local coordinates are transformed to lab using HGeomTransform class
// There is data for transformation this in HShowerModuleGeom object
// addressed by loc[0], loc[1]

  HGeomVector vLoc;
  HGeomTransform tt;

  getLocalCoord(loc, vLoc);
  tt = getTransform(loc[0], loc[1]);
  
  v = tt.transFrom(vLoc);
}

void HShowerGeometry::getSphereCoord(HLocation& loc, HGeomVector2& v){
//Sphere coordinate are calculated from lab coordinates
  float r,theta,phi;
  HGeomVector2 vLoc;

  getLabCoord(loc,vLoc);
  vLoc.sphereCoord(r,theta,phi); //calculation of sphere coord
 
  v.setRad(r);
  v.setTheta(theta);
  v.setPhi(phi);
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


// this function I used to transform pads from  loc. to lab-sphere coord. system only 
Bool_t HShowerGeometry::transPadToLab() {
   HGeomTransform tt;
   HGeomVector2 vLoc,vLab;
   float fX,fY,r,theta,phi;
   float scale = 10.0; //transform cm to mm
   FILE *fp;
   fp = fopen("test.txt","w");

   for(int i=0;i<m_nSectors;i++) {
   for(int j=0;j<sectors[i].getModules();j++) {
    for (int k=0;k < m_nRows; k++) {
    for (int l=0;l < m_nColumns; l++) {

    HShowerPadTab* pPads = getPadParam(j);
    pPads->getPad(k,l)->getPadCenter(&fX, &fY);
    vLoc.setX(scale*fX);
    vLoc.setY(scale*fY);
    vLoc.setZ(0.0);

    tt = getTransform(i,j);
    vLab = tt.transFrom(vLoc);

    vLab.sphereCoord(r,theta,phi);
    if(!j) fprintf(fp,"%3d%3d%3d %10.3f%10.3f\n",i,k,l,phi,theta);

   }
  }
 }
}
 fclose(fp);
 return kTRUE;
}

