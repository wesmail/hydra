#pragma implementation
#include <iostream.h>
#include "hades.h"
#include "hparhadasciifileio.h"
#include "hspectrometer.h"
#include "hshowerparset.h"
#include "hshowerdetector.h"
#include "hshowercalpar.h"
#include "hshowercalparcell.h"
#include "hshowerasic.h"
#include "hshowercalasic.h"
#include "hshowerlookupasic.h"

ClassImp(HShowerCalPar)

///////////////////////////////////////////////////////////////////////
// HShowerCalPar
//
// HShowerCalPar is used to manage of all parameters for calibration
// This class contains structure HObjTable, which contains all 
// HShowerCalParCell objects. One HShowerCalParCell object contains
// calibration parameters of one pad: slope and offset
//
//////////////////////////////////////////////////////////////////////

HShowerCalPar::HShowerCalPar() {
  m_pLookupAsic = NULL;
  m_pCalAsic = NULL;

  setSetup(6, 3, 32, 32); //default setup 
  setCellClassName("HShowerCalParCell");

  SetName("ShowerCalPar");
}

HShowerCalPar::~HShowerCalPar()
{ 
  m_ParamsTable.deleteTab();
}

void HShowerCalPar::setSetup(Int_t nSectors, Int_t nModules,
   Int_t  nRows, Int_t nColumns)
{
//set sizes of internal structure
  m_nSectors = nSectors;
  m_nModules = nModules;
  m_nRows = nRows;
  m_nColumns = nColumns;
} 

void HShowerCalPar::setAsicTables(HShowerLookupAsic *pLA, HShowerCalAsic *pCA) {
//set references to HShowerLookupAsic and HShowerCalAsic structures
  m_pLookupAsic = pLA;
  m_pCalAsic = pCA;
}

void HShowerCalPar::setCellClassName(Char_t* pszName)
//set name of class of calibration parameter container for one pad
{
  strncpy(m_szClassName, pszName, sizeof(m_szClassName));
}

Char_t* HShowerCalPar::getCellClassName()
{
//return  name of class of calibration parameter container for one pad

  return m_szClassName;
}


HShowerCalParCell*& HShowerCalPar::getSlot(HLocation &loc)
{
  //return references address to place new object at loc position
  return (HShowerCalParCell*&) m_ParamsTable.getSlot(loc);
}

HShowerCalParCell* HShowerCalPar::getObject(HLocation &loc)
{
  //return pointer to object, which contains parameter for pad
  //at loc position
  return (HShowerCalParCell*) m_ParamsTable.getObject(loc);
}

Float_t HShowerCalPar::getSlope(HLocation &loc)
{
  //return slope for the pad at postion loc
  return getObject(loc)->getSlope();
}

Float_t HShowerCalPar::getOffset(HLocation &loc)
{
  //return offset for the pad at postion loc
  return getObject(loc)->getOffset();
}

Int_t HShowerCalPar::getAsicNr(HLocation &loc)
{
  return getObject(loc)->getAsicNr();
}

void HShowerCalPar::setSlope(HLocation &loc, Float_t fSlope)
{
  //set slope for the pad at postion loc
  getObject(loc)->setSlope(fSlope);
}

void HShowerCalPar::setOffset(HLocation &loc, Float_t fOffset)
{
  //set offset for the pad at postion loc
  getObject(loc)->setOffset(fOffset);
}

void HShowerCalPar::setAsicNr(HLocation &loc, Int_t nAsicNr)
{
  getObject(loc)->setAsicNr(nAsicNr);
}

Bool_t HShowerCalPar::initAscii(HParHadAsciiFileIo* pHadAsciiFile) {
//reading and initializing  calibration parameters from ascii file (HAsciiRaw format)
  Bool_t status = kTRUE;
  if (!pHadAsciiFile) return kFALSE;

  char sectname[60];
  int sect, mod, row, col, row1, col1;
  float slope, offset;
  HLocation loc;
  HShowerCalParCell *pCell;
 
  loc.setNIndex(4);

  try {
 
     HAsciiRaw* pHAscii = pHadAsciiFile->GetRawAscii();
     HAsciiRaw &mydata = *pHAscii;
   
     mydata.SetActiveSection("Shower Calibrater Parameters Setup");
   
     mydata.SetReadKeyTypes('i', 'i', 'i', 'i');
     mydata.ReadRawLine(0,&m_nSectors,&m_nModules,&m_nRows,&m_nColumns);
   
     setSetup(m_nSectors, m_nModules, m_nRows, m_nColumns);
     m_ParamsTable.set(4, m_nSectors, m_nModules, m_nRows, m_nColumns);
     m_ParamsTable.setCellClassName(getCellClassName());
     m_ParamsTable.makeObjTable();

     for(sect = 0; sect < m_nSectors; sect++)
       for(mod = 0; mod < m_nModules; mod++) {
          sprintf(sectname, 
               "Shower Calibrater Parameters: Sector %d Module %d",
                                                             sect, mod); 
          if (!mydata.SetActiveSection(sectname))
             return kFALSE;
   
          mydata.SetReadKeyTypes('i', 'i', 'f', 'f');
   
          loc[0] = sect;
          loc[1] = mod;
   
          int line = 0;
          for(row = 0; row < m_nRows; row++)
            for(col = 0; col < m_nColumns; col++) {
               loc[2] = row;
               loc[3] = col;
               mydata.ReadRawLine(line, &row1, &col1, &slope, &offset);
               if(row!=row1 || col!=col1) {
                   printf("error");
                   return kFALSE;
               }

               pCell = (HShowerCalParCell*) getSlot(loc);
               if (pCell) {
                    pCell = new(pCell) HShowerCalParCell;
                    pCell->setSlope(slope);
                    pCell->setOffset(offset); 
               }
               line++;
          }
     }
  }
  catch (Bool_t ret) {
    status = ret;
  }

  return status;
}

Bool_t HShowerCalPar::writeAscii(HParHadAsciiFileIo* pHadAsciiFile) {
//writing calibration parameters into ascii file (HAsciiRaw format)
  Bool_t status = kTRUE;
  if (!pHadAsciiFile) return kFALSE;

  char sectname[60];
  int sect, mod, row, col;
  float slope, offset;
  HLocation loc;

  loc.setNIndex(4);

  try {

     HAsciiRaw* pHAscii = pHadAsciiFile->GetRawAscii();
     HAsciiRaw &mydata = *pHAscii;

     mydata.SetSeparator('\t');
     mydata.WriteSection("Shower Calibrater Parameters Setup");
     mydata.WriteKeywords(4, "Sector", "Module", "Row", "Column");
     mydata.SetWriteKeyTypes('i', 'i', 'i', 'i');
     mydata.WriteRawLine(m_nSectors, m_nModules, m_nRows, m_nColumns);
 
     for(sect = 0; sect < m_nSectors; sect++)
       for(mod = 0; mod < m_nModules; mod++) {
          sprintf(sectname, "Shower Calibrater Parameters: Sector %d Module %d",
                                                             sect, mod);
          mydata.WriteSection(sectname);
          mydata.WriteKeywords(4, "Row", "Column", "Slope", "Offset");
          mydata.SetWriteKeyTypes('i', 'i', 'f', 'f');

          loc[0] = sect;
          loc[1] = mod;

          for(row = 0; row < m_nRows; row++)
            for(col = 0; col < m_nColumns; col++) {
               loc[2] = row;
               loc[3] = col;
               slope = getSlope(loc);
               offset = getOffset(loc);
   
               mydata.WriteRawLine(row, col, slope, offset);
          }
     }
  } 
  catch (Bool_t ret) {
    status = ret;
  }

  return status;
}


Bool_t HShowerCalPar::defaultInit()
{
  //initialization of calibration parameters for full setup
  //of pre-SHOWER detector. Default calibration using
  //values of slope==1 and offset==0

  printf("initialization of ShowerCalPar\n");
  int sect, mod, row, col;

  HShowerDetector *pShowerDet = (HShowerDetector*)gHades->getSetup()
                                                  ->getDetector("Shower");
  if (!pShowerDet) return kFALSE;
 
  m_nSectors = pShowerDet->getShowerSectors();
  m_nModules = pShowerDet->getShowerModules();
  m_nRows = pShowerDet->getRows();
  m_nColumns = pShowerDet->getColumns();

  printf("%d - %d - %d - %d\n", m_nSectors, m_nModules, m_nRows, m_nColumns);
  m_ParamsTable.set(4, m_nSectors, m_nModules, m_nRows, m_nColumns);

  m_ParamsTable.setCellClassName(getCellClassName());
  m_ParamsTable.makeObjTable();

  HLocation loc;
  HShowerCalParCell *pCell;
  loc.setNIndex(4);

  for(sect = 0; sect < m_nSectors; sect++)
    for(mod = 0; mod < m_nModules; mod++) 
      for(row = 0; row < m_nRows; row++)
         for(col = 0; col < m_nColumns; col++) {
           loc[0] = sect;
           loc[1] = mod;
           loc[2] = row;
           loc[3] = col;

           pCell = (HShowerCalParCell*) getSlot(loc);
           if (pCell) {
                 pCell = new(pCell) HShowerCalParCell;
                 pCell->setSlope(1.0);
                 pCell->setOffset(0.0);
           }
         }
 
  printf("Shower calpar default initialization\n");
  return kTRUE;
}

void HShowerCalPar::takeDataFromAsic() {
  if ((m_pLookupAsic == NULL) || (m_pCalAsic == NULL))
     return;

  HLocation loc;
  HShowerAsic* pAsic;
  Float_t fSlope=1.0, fOffset=0.0;
  Int_t asicNr;
  loc.setNIndex(4);
 
  
  for(loc[0] = 0; loc[0] < m_nSectors; loc[0]++)
    for(loc[1] = 0; loc[1] < m_nModules; loc[1]++)
      for(loc[2] = 0; loc[2] < m_nRows; loc[2]++) {
          asicNr = m_pLookupAsic->getAsicNr(loc);
          if (asicNr==-1) 
                continue;
       
           pAsic = m_pCalAsic->getAsic(asicNr);
           if (pAsic) 
                for(loc[3] = 0; loc[3] < m_nColumns; loc[3]++) {
                   setAsicNr(loc, asicNr);
                   setSlope(loc, fSlope);
                   setOffset(loc, fOffset);
                   fSlope = pAsic->getSlope(loc[3]);
                   fOffset = pAsic->getOffset(loc[3]);
                }
           else 
                for(loc[3] = 0; loc[3] < m_nColumns; loc[3]++) {
                   setAsicNr(loc, asicNr);
                   setSlope(loc, 1.0);
                   setOffset(loc, 0.0);
                }
      }
}
 
void HShowerCalPar::propagateDataToAsic() {
  if ((m_pLookupAsic == NULL) || (m_pCalAsic == NULL))
     return;

  HLocation loc;
  HShowerAsic* pAsic;
  Float_t fSlope, fOffset;
  Int_t asicNr;
  loc.setNIndex(4);

  for(loc[0] = 0; loc[0] < m_nSectors; loc[0]++)
    for(loc[1] = 0; loc[1] < m_nModules; loc[1]++)
      for(loc[2] = 0; loc[2] < m_nRows; loc[2]++) {
          asicNr = m_pLookupAsic->getAsicNr(loc);
          if (asicNr==-1)
                continue;

           pAsic = m_pCalAsic->getAsic(asicNr);
           if (!pAsic) {
              pAsic = new HShowerAsic(asicNr);
              m_pCalAsic->addAsic(pAsic);
           }
           if (pAsic)
                for(loc[3] = 0; loc[3] < m_nColumns; loc[3]++) {
                   fSlope = getSlope(loc);
                   fOffset = getOffset(loc);
                   pAsic->setSlope(loc[3], fSlope);
                   pAsic->setOffset(loc[3], fOffset);
                }
      }
}



void HShowerCalPar::Print() {
  int i, j, k, l;
  float slope, offset;

  HLocation loc;

  loc.setNIndex(4);
  for (i = 0; i < m_nSectors; i++)
     for (j = 0; j < m_nModules; j++)
        for (k = 0; k < m_nRows; k++)
           for (l = 0; l < m_nColumns; l++) {
             loc[0] = i;
             loc[1] = j;
             loc[2] = k;
             loc[3] = l;
             slope = getSlope(loc);
             offset = getOffset(loc);
             
             if (offset < 0) 
             printf("%d:%d:%d:%d %f %f\n", i, j, k, l, slope, offset);
           }
}

