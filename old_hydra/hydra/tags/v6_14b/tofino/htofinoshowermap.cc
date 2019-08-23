#pragma implementation
using namespace std;
#include <iostream> 
#include <iomanip>
#include "hades.h"
#include "hparhadasciifileio.h"
#include "hspectrometer.h"
#include "htofinoparset.h"
#include "htofinodetector.h"
#include "htofinoshowermap.h"
#include "htofinoshowermapcell.h"

ClassImp(HTofinoShowerMap)

///////////////////////////////////////////////////////////////////////
// HTofinoShowerMap
//
// HTofinoShowerMap is used to manage of all parameters tied tof and
// shower detectors
// This class contains structure HObjTable, which contains all 
// HTofinoShowerMapCell objects. One HTofinoShowerMapCell object contains
// inforamtions attached position of shower's pad relative to tofino cell
//
//////////////////////////////////////////////////////////////////////

HTofinoShowerMap::HTofinoShowerMap(const char* name,const char* title,
                                   const char* context)
                 : HTofinoParSet(name,title,context) {
  setSetup(32, 32); //default setup 
  setCellClassName("HTofinoShowerMapCell");
  defaultInit();
  fLoc.set(2, 0, 0);
}

HTofinoShowerMap::~HTofinoShowerMap()
{ 
  m_ParamsTable.deleteTab();
}

void HTofinoShowerMap::setSetup(Int_t nRows, Int_t nColumns)
{
//set sizes of internal structure
  m_nRows = nRows;
  m_nColumns = nColumns;
} 

void HTofinoShowerMap::setCellClassName(Char_t* pszName)
//set name of class of calibration parameter container for one pad
{
  strncpy(m_szClassName, pszName, sizeof(m_szClassName));
}

Char_t* HTofinoShowerMap::getCellClassName()
{
//return  name of class of calibration parameter container for one pad

  return m_szClassName;
}


HTofinoShowerMapCell*& HTofinoShowerMap::getSlot(HLocation &loc)
{
  //return references address to place new object at loc position
  return (HTofinoShowerMapCell*&) m_ParamsTable.getSlot(loc);
}

HTofinoShowerMapCell* HTofinoShowerMap::getObject(HLocation &loc)
{
  //return pointer to object, which contains parameter for pad
  //at loc position
  return (HTofinoShowerMapCell*) m_ParamsTable.getObject(loc);
}

Int_t HTofinoShowerMap::getCellNumber(Int_t nRow, Int_t nColumn) {
  fLoc[0] = nRow;
  fLoc[1] = nColumn;

  return getCellNumber(fLoc);
}

Int_t HTofinoShowerMap::getCellNumber(HLocation &loc)
{
  //return tofino cell number for the pad at postion loc
  return getObject(loc)->getCellNumber();
}

void HTofinoShowerMap::setCellNumber(HLocation &loc, Int_t nCell)
{
  //set tofino cell number for the pad at postion loc
  getObject(loc)->setCellNumber(nCell);
}

Bool_t HTofinoShowerMap::initAscii(HParHadAsciiFileIo* pHadAsciiFile) {
//reading and initializing  calibration parameters from ascii file (HAsciiKey format)
  Bool_t status = kTRUE;
  if (!pHadAsciiFile) return kFALSE;

  char keyname[60];
  int row, col, cell[32], cell1;

  HLocation loc;
  HTofinoShowerMapCell *pCell;
 
  loc.setNIndex(2);

  try {
 
     HAsciiKey* pHAscii = pHadAsciiFile->GetKeyAscii();
     HAsciiKey &mydata = *pHAscii;
   
     setSetup(32, 32);  //row, columns
     m_ParamsTable.set(2, m_nRows, m_nColumns);
     m_ParamsTable.setCellClassName(getCellClassName());
     m_ParamsTable.makeObjTable();
    
     mydata.SetActiveSection("Tofino-Shower Map");
     for(row = 0; row < m_nRows; row++) {
        sprintf(keyname, "row%02d", row);
        mydata.ReadInt(cell, keyname);

        loc[0] = row;
        for(col = 0; col < 32; col++) {
             loc[1] = col;

             pCell = (HTofinoShowerMapCell*) getSlot(loc);
             if (pCell) {
                 pCell = new(pCell) HTofinoShowerMapCell;
//                 pCell->setCellNumber(cell[col]);
//                   cell1 = (cell[col]==3) ? 9 : cell[col];
//                   cell1 = (cell1==-1) ? 9 : cell1; 
                   cell1 = cell[col];   
                   pCell->setCellNumber(cell1);
             }
        }
      }

  }
  catch (Bool_t ret) {
    status = ret;
  }

  return status;
}

Bool_t HTofinoShowerMap::writeAscii(HParHadAsciiFileIo* pHadAsciiFile) {
//writing calibration parameters into ascii file (HAsciiKey format)
  Bool_t status = kTRUE;
  if (!pHadAsciiFile) return kFALSE;

  char keyname[60];
  Int_t row, col;
  Int_t cell[m_nColumns];
  
  HLocation loc;

  loc.setNIndex(2);

  try {
     HAsciiKey* pHAscii = pHadAsciiFile->GetKeyAscii();
     HAsciiKey &mydata = *pHAscii;

     mydata.WriteSection("Tofino-Shower Map");
     mydata.SetSeparator(' ');
     mydata.SetNumInLine(32);
 
     for(row = 0; row < m_nRows; row++) {
          loc[0] = row;

          for(col = 0; col < 32; col++) {
               loc[1] = col;
               cell[col] = getCellNumber(loc);
          }
          sprintf(keyname, "row%02d", row);
          mydata.WriteInt(keyname, 32, cell);
     }
  } 
  catch (Bool_t ret) {
    status = ret;
  }

  return status;
}


Bool_t HTofinoShowerMap::defaultInit()
{
  //initialization of calibration parameters for full setup
  //of tofino detector. Default calibration using
  //value cellNumber=9 

  printf("initialization of TofinoShowerMap\n");
  Int_t row, col;
  Int_t tofcell;

  HTofinoDetector *pTofinoDet = (HTofinoDetector*)gHades->getSetup()
                                                  ->getDetector("Tofino");
  if (!pTofinoDet) return kFALSE;
 
  m_nRows = 32;
  m_nColumns = 32;

  printf("%d - %d\n", m_nRows, m_nColumns);
  m_ParamsTable.set(2, m_nRows, m_nColumns);

  m_ParamsTable.setCellClassName(getCellClassName());
  m_ParamsTable.makeObjTable();

  HLocation loc;
  HTofinoShowerMapCell *pCell;
  loc.setNIndex(2);

  for(row = 0; row < m_nRows; row++)
    for(col = 0; col < m_nColumns; col++) {
           loc[0] = row;
           loc[1] = col;

           pCell = (HTofinoShowerMapCell*) getSlot(loc);
           if (pCell) {
                 pCell = new(pCell) HTofinoShowerMapCell;
                 tofcell = (col < 8) ? 9 : 3 -  col/8; 
                 pCell->setCellNumber(tofcell);
           }
         }
 
  printf("Tofino/Shower MapPar container default initialization\n");
  return kTRUE;
}

void HTofinoShowerMap::Print() {
  int i, j;
  int cell;

  HLocation loc;

  loc.setNIndex(2);
  for (i = 0; i < m_nRows; i++)
     for (j = 0; j < m_nColumns; j++) {
       loc[0] = i;
       loc[1] = j;
       
       cell = getCellNumber(loc);
             
       printf("%d:%d %d\n", i, j, cell);
   }
}

