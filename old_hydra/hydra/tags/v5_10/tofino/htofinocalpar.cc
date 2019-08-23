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
   Float_t fSlope, fOffset;
   Float_t fTime, fDist;

   fLoc[0] = nSector;
   fLoc[1] = nCell;

   fOffset = getOffset(fLoc);
   fSlope = getSlope(fLoc);

   HTofinoCalParCell* pCell;
   pCell = getObject(fLoc);

   fDist = pCell->calcDistance(fRow);
 
   fTime = fOffset + fDist*fSlope;

   return channelToTime(nSector, nCell, fTime);
}

Float_t HTofinoCalPar::
     calcLightPropTime(Int_t nSector, Int_t nCell, Float_t fY){

   Float_t fSlope, fOffset;
   Float_t fTime, fDist;

   fLoc[0] = nSector;
   fLoc[1] = nCell;

   fOffset = getOffset(fLoc);
   fSlope = getSlope(fLoc);
   
   HTofinoCalParCell* pCell;
   pCell = getObject(fLoc);

   fDist = pCell->calcDistPMT(fY);
 
   fTime = fOffset + fDist*fSlope;   // to be consistent with calcDriftTime()
//   fTime = fDist*fSlope;

   return channelToTime(nSector, nCell, fTime);
}

Float_t HTofinoCalPar::
     calcDeltaLightPropTime(Int_t nSector, Int_t nCell, Float_t fTime){

   Float_t fSlope, fOffset;
   Float_t fDeltaSlope, fDeltaOffset;
   Float_t fTimeResol;

   fLoc[0] = nSector;
   fLoc[1] = nCell;

   fOffset = getOffset(fLoc);
   fSlope = getSlope(fLoc);
   fDeltaOffset = getDeltaOffset(fLoc);
   fDeltaSlope = getDeltaSlope(fLoc);
   
   HTofinoCalParCell* pCell;
   pCell = getObject(fLoc);

   fTimeResol = 
     //  fTime * sqrt(pow(fDeltaOffset/fOffset,2) + pow(fDeltaSlope/fSlope,2));
     fTime * fDeltaSlope ;

   return fTimeResol;
}

Float_t HTofinoCalPar::channelToTime(Int_t nSector, Int_t nCell, Float_t fTime) {
   fLoc[0] = nSector;
   fLoc[1] = nCell;

   HTofinoCalParCell* pCell;
   pCell = getObject(fLoc);

   return pCell->getTimeChannel()*(fTime-pCell->getOffset());
}

Float_t HTofinoCalPar::
   timeToChannel(Int_t nSector, Int_t nCell, Float_t fTime) {

   Float_t fOffset;
   fLoc[0] = nSector;
   fLoc[1] = nCell;

   fOffset = getOffset(fLoc);
   HTofinoCalParCell* pCell;
   pCell = getObject(fLoc);

   return (fOffset + fTime/(pCell->getTimeChannel()));
}

Float_t HTofinoCalPar::getSlope(HLocation &loc)
{
  //return slope for the pad at position loc
  return getObject(loc)->getSlope();
}

Float_t HTofinoCalPar::getOffset(HLocation &loc)
{
  //return offset for the pad at position loc
  return getObject(loc)->getOffset();
}

Float_t HTofinoCalPar::getDeltaSlope(HLocation &loc)
{
  //return error in slope for the pad at position loc
  return getObject(loc)->getDeltaSlope();
}

Float_t HTofinoCalPar::getDeltaOffset(HLocation &loc)
{
  //return error in slope for the pad at position loc
  return getObject(loc)->getDeltaOffset();
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

void HTofinoCalPar::setOffset(HLocation &loc, Float_t fOffset)
{
  //set offset for the pad at position loc
  getObject(loc)->setOffset(fOffset);
}

void HTofinoCalPar::setDeltaSlope(HLocation &loc, Float_t fDeltaSlope)
{
  //set slope for the pad at position loc
  getObject(loc)->setDeltaSlope(fDeltaSlope);
}

void HTofinoCalPar::setDeltaOffset(HLocation &loc, Float_t fDeltaOffset)
{
  //set slope for the pad at position loc
  getObject(loc)->setDeltaOffset(fDeltaOffset);
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
  Float_t timechannel;
  Float_t coordpos;

  int sect, cell;
  float slope, offset, deltaslope, deltaoffset;
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
             slope = mydata.ReadFloat(keyname, 0); 
             offset = mydata.ReadFloat(keyname, 1); 

             sprintf(keyname, "Cell %d Distance Params", cell);
             mydata.ReadFloat(distparam, keyname);
             
             sprintf(keyname, "Cell %d Time Channel", cell);
             timechannel = mydata.ReadFloat(keyname);

             sprintf(keyname, "Cell %d Coordinate System Position", cell);
             coordpos = mydata.ReadFloat(keyname);

             sprintf(keyname, "Cell %d Delta Params", cell);
             deltaslope = mydata.ReadFloat(keyname, 0); 
             deltaoffset = mydata.ReadFloat(keyname, 1); 

             loc[1] = cell;
             pCell = (HTofinoCalParCell*) getSlot(loc);
             if (pCell) {
                 pCell = new(pCell) HTofinoCalParCell;
                 pCell->setSlope(slope);
                 pCell->setOffset(offset);
                 pCell->setTimeChannel(timechannel); 
                 pCell->setDistanceParams(distparam);
                 pCell->setCoordPosition(coordpos);
                 pCell->setDeltaSlope(deltaslope);
                 pCell->setDeltaOffset(deltaoffset);
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
  Float_t distparam[3];
  Float_t timechannel;
  Float_t coordpos, deltaslope, deltaoffset;

  int sect, cell;
  float slope, offset;
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
               offset = pCell->getOffset();
               timechannel = pCell->getTimeChannel();              
               pCell->getDistanceParams(distparam);
               coordpos = pCell->getCoordPosition();
               deltaslope = pCell->getDeltaSlope();
               deltaoffset = pCell->getDeltaOffset();

               sprintf(keyname, "Cell %d Params", cell);
	       mydata.WriteNFloat(keyname, 2, slope, offset); 
               sprintf(keyname, "Cell %d Distance Params", cell);
               mydata.WriteFloat(keyname, 3, distparam);
               sprintf(keyname, "Cell %d Time Channel", cell);
               mydata.WriteFloat(keyname, timechannel);
               sprintf(keyname, "Cell %d Coordinate System Position", cell);
               mydata.WriteFloat(keyname, coordpos);
               sprintf(keyname, "Cell %d Delta Params", cell);
               mydata.WriteNFloat(keyname, 2, deltaslope, deltaoffset); 
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
  //values of slope==1 and offset==0

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
                 pCell->setOffset(0.0);
           }
         }
 
  printf("Tofino CalPar container default initialization\n");
  return kTRUE;
}

void HTofinoCalPar::Print() {
  int i, j;
  float slope, offset, coordpos;

  HLocation loc;

  loc.setNIndex(2);
  for (i = 0; i < m_nSectors; i++)
     for (j = 0; j < m_nCells; j++) {
       loc[0] = i;
       loc[1] = j;
       
       slope = getSlope(loc);
       offset = getOffset(loc);
       coordpos = getCoordPosition(loc);

       printf("%d:%d %f %f\n", i, j, slope, offset);
       cout <<"KS = " <<coordpos <<endl;
   }
}








