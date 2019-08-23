
//#
//#       
//#                      FRAMEWORK
//#
//#       Authors:                           W.Koenig
//#       adoption to framework & ROOT       W.Schoen
//#    
//#       last mod. W.Schoen 19.1.98
// ###################################################

//#pragma implementation
#include <iostream.h>
#include "hades.h"
#include "hparhadasciifileio.h"
#include "hspectrometer.h"
#include "hshowerparset.h"
#include "hshowerdetector.h"
#include "hshowerlookupasic.h"
ClassImp(HShowerLookupAsic)

////////// **********ShowerLookupAsic defintion ****************/////////////77
HShowerLookupAsic::HShowerLookupAsic() {

  setSetup(6, 3, 32); //default setup 
  setCellClassName("HShowerLookupAsicTab");

  SetName("ShowerLookupAsic");
}

HShowerLookupAsic::~HShowerLookupAsic()
{ 
  m_ParamsTable.deleteTab();
}

void HShowerLookupAsic::setSetup(Int_t nSectors, Int_t nModules,
   Int_t  nRows)
{
  m_nSectors = nSectors;
  m_nModules = nModules;
  m_nRows = nRows;
} 


void HShowerLookupAsic::setCellClassName(Char_t* pszName)
{
  strncpy(m_szClassName, pszName, sizeof(m_szClassName));
}

Char_t* HShowerLookupAsic::getCellClassName()
{
  return m_szClassName;
}


HShowerLookupAsicTab* HShowerLookupAsic::getSlot(HLocation &loc)
{
  return (HShowerLookupAsicTab*) m_ParamsTable.getSlot(loc);
}

HShowerLookupAsicTab* HShowerLookupAsic::getObject(HLocation &loc)
{
  return (HShowerLookupAsicTab*) m_ParamsTable.getObject(loc);
}

Int_t HShowerLookupAsic::getAsicNr(HLocation &loc)
{
  HLocation l1;
  HShowerLookupAsicTab *pAsicArr;

  l1.setNIndex(2);
  l1[0] = loc[0];
  l1[1] = loc[1];
  pAsicArr = getObject(l1);

  return (*pAsicArr)[loc[2]];
}

void HShowerLookupAsic::setAsicNr(HLocation &loc, Int_t nAsicNr)
{
  HLocation l1;
  HShowerLookupAsicTab *pAsicArr;

  l1.setNIndex(2);
  l1[0] = loc[0];
  l1[1] = loc[1];
  pAsicArr = getObject(l1);

  (*pAsicArr)[loc[2]] = nAsicNr;
}

Bool_t HShowerLookupAsic::initAscii(HParHadAsciiFileIo* pHadAsciiFile) {
  Bool_t status = kTRUE;
  if (!pHadAsciiFile) return kFALSE;

  char namebuf[60];
  int asics[32];
  int sect, mod, row;
  HLocation loc;
  loc.setNIndex(3);

  try {

     HAsciiKey* pHAscii = pHadAsciiFile->GetKeyAscii();
     HAsciiKey &mydata = *pHAscii;

     mydata.SetActiveSection("Shower Lookup Asic Setup");

     m_nSectors = mydata.ReadInt("Sectors");
     m_nModules = mydata.ReadInt("Modules");
     m_nRows = mydata.ReadInt("Rows");

     setSetup(m_nSectors, m_nModules, m_nRows);

     printf("%d - %d - %d\n", m_nSectors, m_nModules, m_nRows);
     allocateTables();

     for(sect = 0; sect < m_nSectors; sect++) {
       sprintf(namebuf, "Shower Lookup Asic Table: Sector %d", sect);
       mydata.SetActiveSection(namebuf);
       for(mod = 0; mod < m_nModules; mod++) {
          loc[0] = sect;
          loc[1] = mod;
   
          sprintf(namebuf, "Module %d", mod);
          mydata.ReadInt(asics, namebuf, m_nRows);

          for(row = 0; row < m_nRows; row++) {
               loc[2] = row;
               setAsicNr(loc, asics[row]);
          }
       }
     }
  }
  catch (Bool_t ret) {
    status = ret;
  }

  return status;
}

Bool_t HShowerLookupAsic::writeAscii(HParHadAsciiFileIo* pHadAsciiFile) {
  Bool_t status = kTRUE;
  if (!pHadAsciiFile) return kFALSE;

  char namebuf[60];
  int asics[32];
  int sect, mod, row;
  HLocation loc;

  loc.setNIndex(3);

  try {

     HAsciiKey* pHAscii = pHadAsciiFile->GetKeyAscii();
     HAsciiKey &mydata = *pHAscii;
   
     mydata.WriteSection("Shower Lookup Asic Setup");
     mydata.WriteInt("Sectors", m_nSectors);
     mydata.WriteInt("Modules", m_nModules);
     mydata.WriteInt("Rows", m_nRows);
   
     mydata.SetNumInLine(16);
    
     for(sect = 0; sect < m_nSectors; sect++) {
       sprintf(namebuf, "Shower Lookup Asic Table: Sector %d", sect);
       mydata.WriteSection(namebuf);
       for(mod = 0; mod < m_nModules; mod++) {
          loc[0] = sect;
          loc[1] = mod;
          
          for(row = 0; row < m_nRows; row++) {
            loc[2] = row;
            asics[row] = getAsicNr(loc);
          }
          sprintf(namebuf, "Module %d*", mod);
          mydata.WriteInt(namebuf, m_nRows, asics);
          mydata.WriteRemark("---------------------------------", '#');
       }
     }
  }
  catch (Bool_t ret) {
    status = ret;
  }

  return status;
}

Bool_t HShowerLookupAsic::defaultInit() {
  printf("initialization of ShowerLookupAsic\n");
  HShowerDetector *pShowerDet = (HShowerDetector*)gHades->getSetup()
                                                  ->getDetector("Shower");
  if (!pShowerDet) return kFALSE;

  m_nSectors = pShowerDet->getShowerSectors();
  m_nModules = pShowerDet->getShowerModules();
  m_nRows = pShowerDet->getRows();
 
  setSetup(m_nSectors, m_nModules, m_nRows);

  printf("%d - %d - %d\n", m_nSectors, m_nModules, m_nRows);
  allocateTables(); 

  printf("Shower lookup asic default initialization\n");

  return kTRUE;
}

void HShowerLookupAsic::allocateTables() {
  int sect, mod;
  HShowerLookupAsicTab *pAsicArr;
  HLocation loc;
  loc.setNIndex(2);

  m_ParamsTable.set(2, m_nSectors, m_nModules);

  m_ParamsTable.setCellClassName(getCellClassName());
  m_ParamsTable.makeObjTable();
  
  for(sect = 0; sect < m_nSectors; sect++)
    for(mod = 0; mod < m_nModules; mod++) {
      loc[0] = sect;
      loc[1] = mod;
      pAsicArr = (HShowerLookupAsicTab*) getSlot(loc);
      if (pAsicArr) {
         pAsicArr = new(pAsicArr) HShowerLookupAsicTab;
         pAsicArr->setNIndex(m_nRows);
         pAsicArr->fill(-1);
      }
    }              
}

void HShowerLookupAsic::Print() {
  int i, j, k;
  int asic;

  HLocation loc;

  loc.setNIndex(3);
  for (i = 0; i < m_nSectors; i++)
     for (j = 0; j < m_nModules; j++) {
        printf("Sector: %d Module: %d", i, j);
        for (k = 0; k < m_nRows; k++) {
             loc[0] = i;
             loc[1] = j;
             loc[2] = k;
             asic  = getAsicNr(loc);
             if(k % 16 == 0) printf("\n");
             printf("%d ", asic);
           }
        printf("\n");
     }
}

