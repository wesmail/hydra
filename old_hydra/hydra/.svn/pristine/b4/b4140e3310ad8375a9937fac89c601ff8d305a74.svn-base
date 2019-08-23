#pragma implementation
#include <iostream.h>
#include "hades.h"
#include "hparhadasciifileio.h"
#include "hspectrometer.h"
#include "htofinoparset.h"
#include "htofinodetector.h"
#include "htofinocalpar.h"
#include "htofinocalparcell.h"

ClassImp(HTofinoCalPar)

///////////////////////////////////////////////////////////////////////
// HTofinoCalPar
//
// HTofinoCalPar is used to manage of all parameters for calibration
// This class contains structure HObjTable, which contains all 
// HTofinoCalParCell objects. One HTofinoCalParCell object contains
// calibration parameters of one pad: slope and offset
//
//////////////////////////////////////////////////////////////////////

HTofinoCalPar::HTofinoCalPar() {
  setSetup(6, 4); //default setup 
  setCellClassName("HTofinoCalParCell");

  SetName("TofinoCalPar");
  fLoc.set(2, 0, 0);
}

HTofinoCalPar::~HTofinoCalPar()
{ 
  m_ParamsTable.deleteTab();
}

void HTofinoCalPar::setSetup(Int_t nSectors, Int_t nCells)
{
//set sizes of internal structure
  m_nSectors = nSectors;
  m_nCells = nCells;
} 

void HTofinoCalPar::setCellClassName(Char_t* pszName)
//set name of class of calibration parameter container for one pad
{
  strncpy(m_szClassName, pszName, sizeof(m_szClassName));
}

Char_t* HTofinoCalPar::getCellClassName()
{
//return  name of class of calibration parameter container for one pad

  return m_szClassName;
}


HTofinoCalParCell*& HTofinoCalPar::getSlot(HLocation &loc)
{
  //return references address to place new object at loc position
  return (HTofinoCalParCell*&) m_ParamsTable.getSlot(loc);
}

HTofinoCalParCell* HTofinoCalPar::getObject(HLocation &loc)
{
  //return pointer to object, which contains parameter for pad
  //at loc position
  return (HTofinoCalParCell*) m_ParamsTable.getObject(loc);
}

Float_t HTofinoCalPar::calcDriftTime(Int_t nSector, Int_t nCell, Float_t fRow){
   Float_t fSlope, fDist;

   fLoc[0] = nSector;
   fLoc[1] = nCell;

   fSlope = getSlope(fLoc);

   HTofinoCalParCell* pCell;
   pCell = getObject(fLoc);

   fDist = pCell->calcDistance(fRow);
 
   return fDist*fSlope;
}

Float_t HTofinoCalPar::
     calcLightPropTime(Int_t nSector, Int_t nCell, Float_t fY){

   Float_t fSlope, fDist;

   fLoc[0] = nSector;
   fLoc[1] = nCell;

   fSlope = getSlope(fLoc);
   
   HTofinoCalParCell* pCell;
   pCell = getObject(fLoc);

   fDist = pCell->calcDistPMT(fY);
 
   return fDist*fSlope; 
}

Float_t HTofinoCalPar::
     calcDeltaLightPropTime(Int_t nSector, Int_t nCell, Float_t fTime){

   Float_t fSlope, fDeltaSlope;

   fLoc[0] = nSector;
   fLoc[1] = nCell;

   fSlope = getSlope(fLoc);
   fDeltaSlope = getDeltaSlope(fLoc);
   
   HTofinoCalParCell* pCell;
   pCell = getObject(fLoc);

   return fTime * fDeltaSlope;
}

Float_t HTofinoCalPar::getSlope(HLocation &loc)
{
  //return slope for the pad at position loc
  return getObject(loc)->getSlope();
}

Float_t HTofinoCalPar::getDeltaSlope(HLocation &loc)
{
  //return error in slope for the pad at position loc
  return getObject(loc)->getDeltaSlope();
}

Float_t HTofinoCalPar::getCoordPosition(HLocation &loc)
{
  //return Y position of the coordinate system for the pad at position loc
  return getObject(loc)->getCoordPosition();
}

void HTofinoCalPar::setSlope(HLocation &loc, Float_t fSlope)
{
  //set slope for the pad at position loc
  getObject(loc)->setSlope(fSlope);
}

void HTofinoCalPar::setDeltaSlope(HLocation &loc, Float_t fDeltaSlope)
{
  //set slope for the pad at position loc
  getObject(loc)->setDeltaSlope(fDeltaSlope);
}

void HTofinoCalPar::setCoordPosition(HLocation &loc, Float_t fCoordPos)
{
  //set Y position of the coordinate system for the pad at position loc
  getObject(loc)->setCoordPosition(fCoordPos);
}

Bool_t HTofinoCalPar::initAscii(HParHadAsciiFileIo* pHadAsciiFile) {
//reading and initializing  calibration parameters from ascii file (HAsciiKey format)
  Bool_t status = kTRUE;
  if (!pHadAsciiFile) return kFALSE;

  char sectname[60];
  char keyname[60];
  Float_t distparam[3];
  Float_t coordpos;

  int sect, cell;
  float slope, deltaslope;
  HLocation loc;
  HTofinoCalParCell *pCell;
 
  loc.setNIndex(2);

  try {
 
     HAsciiKey* pHAscii = pHadAsciiFile->GetKeyAscii();
     HAsciiKey &mydata = *pHAscii;
   
     mydata.SetActiveSection("Tofino Calibrater Parameters Setup");
     m_nSectors = mydata.ReadInt("Sectors"); 
     m_nCells = mydata.ReadInt("Cells"); 
   
     setSetup(m_nSectors, m_nCells);
     m_ParamsTable.set(2, m_nSectors, m_nCells);
     m_ParamsTable.setCellClassName(getCellClassName());
     m_ParamsTable.makeObjTable();

     for(sect = 0; sect < m_nSectors; sect++) {
          sprintf(sectname, 
               "Tofino Calibrater Parameters: Sector %d", sect); 
          if (!mydata.SetActiveSection(sectname))
             return kFALSE;
   
          loc[0] = sect;
   
          for(cell = 0; cell < m_nCells; cell++) {
             sprintf(keyname, "Cell %d Params", cell);
             slope = mydata.ReadFloat(keyname); 

             sprintf(keyname, "Cell %d Distance Params", cell);
             mydata.ReadFloat(distparam, keyname);
             
             sprintf(keyname, "Cell %d Coordinate System Position", cell);
             coordpos = mydata.ReadFloat(keyname);

             sprintf(keyname, "Cell %d Delta Params", cell);
             deltaslope = mydata.ReadFloat(keyname); 

             loc[1] = cell;
             pCell = (HTofinoCalParCell*) getSlot(loc);
             if (pCell) {
                 pCell = new(pCell) HTofinoCalParCell;
                 pCell->setSlope(slope);
                 pCell->setDistanceParams(distparam);
                 pCell->setCoordPosition(coordpos);
                 pCell->setDeltaSlope(deltaslope);
             }
          }
     }
  }
  catch (Bool_t ret) {
    status = ret;
  }

  return status;
}

Bool_t HTofinoCalPar::writeAscii(HParHadAsciiFileIo* pHadAsciiFile) {
//writing calibration parameters into ascii file (HAsciiKey format)
  Bool_t status = kTRUE;
  if (!pHadAsciiFile) return kFALSE;

  char sectname[60];
  char keyname[60];
  Float_t distparam[3], coordpos, deltaslope;

  int sect, cell;
  float slope;
  HLocation loc;
  HTofinoCalParCell* pCell;

  loc.setNIndex(2);

  try {
     HAsciiKey* pHAscii = pHadAsciiFile->GetKeyAscii();
     HAsciiKey &mydata = *pHAscii;

     mydata.WriteSection("Tofino Calibrater Parameters Setup");
     mydata.WriteInt("Sectors", m_nSectors); 
     mydata.WriteInt("Cells", m_nCells); 

     mydata.SetSeparator('\t');    
     for(sect = 0; sect < m_nSectors; sect++) {
          sprintf(sectname, "Tofino Calibrater Parameters: Sector %d", sect);
          mydata.WriteSection(sectname);

          loc[0] = sect;

          for(cell = 0; cell < m_nCells; cell++) {
               loc[1] = cell;
               pCell = getObject(loc);
               slope = pCell->getSlope();
               pCell->getDistanceParams(distparam);
               coordpos = pCell->getCoordPosition();
               deltaslope = pCell->getDeltaSlope();

               sprintf(keyname, "Cell %d Params", cell);
	       mydata.WriteFloat(keyname, slope); 
               sprintf(keyname, "Cell %d Distance Params", cell);
               mydata.WriteFloat(keyname, 3, distparam);
               sprintf(keyname, "Cell %d Coordinate System Position", cell);
               mydata.WriteFloat(keyname, coordpos);
               sprintf(keyname, "Cell %d Delta Params", cell);
               mydata.WriteFloat(keyname, deltaslope); 
               mydata.WriteLine("\n");
          }
     }
  } 
  catch (Bool_t ret) {
    status = ret;
  }

  return status;
}


Bool_t HTofinoCalPar::defaultInit()
{
  //initialization of calibration parameters for full setup
  //of tofino detector. Default calibration using
  //values of slope==1 

  printf("initialization of TofinoCalPar\n");
  Int_t sect, cell;

  HTofinoDetector *pTofinoDet = (HTofinoDetector*)gHades->getSetup()
                                                  ->getDetector("Tofino");
  if (!pTofinoDet) return kFALSE;
 
  m_nSectors = 6;
  m_nCells = 4;

  printf("%d - %d\n", m_nSectors, m_nCells);
  m_ParamsTable.set(2, m_nSectors, m_nCells);

  m_ParamsTable.setCellClassName(getCellClassName());
  m_ParamsTable.makeObjTable();

  HLocation loc;
  HTofinoCalParCell *pCell;
  loc.setNIndex(2);

  for(sect = 0; sect < m_nSectors; sect++)
    for(cell = 0; cell < m_nCells; cell++) {
           loc[0] = sect;
           loc[1] = cell;

           pCell = (HTofinoCalParCell*) getSlot(loc);
           if (pCell) {
                 pCell = new(pCell) HTofinoCalParCell;
                 pCell->setSlope(1.0);
           }
         }
 
  printf("Tofino CalPar container default initialization\n");
  return kTRUE;
}

void HTofinoCalPar::Print() {
  int i, j;
  float slope, coordpos;

  HLocation loc;

  loc.setNIndex(2);
  for (i = 0; i < m_nSectors; i++)
     for (j = 0; j < m_nCells; j++) {
       loc[0] = i;
       loc[1] = j;
       
       slope = getSlope(loc);
       coordpos = getCoordPosition(loc);

       printf("%d:%d %f \n", i, j, slope);
       cout <<"KS = " <<coordpos <<endl;
   }
}















































































