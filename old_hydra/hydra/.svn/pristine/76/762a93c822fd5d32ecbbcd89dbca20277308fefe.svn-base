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

//#pragma implementation
#include "hades.h"
#include "hspectrometer.h"
#include "hshowerdetector.h"
#include "hshowercalpar.h"
ClassImp(HShowerCalPar)

////////// **********ShowerCalPar defintion ****************/////////////77
HShowerCalPar::HShowerCalPar() {
  m_pReadParam = NULL;

  setSetup(6, 3, 32, 32); //default setup 
  setCellClassName("HShowerCalParCell");
}

HShowerCalPar::~HShowerCalPar()
{ 
  m_ParamsTable.deleteTab();
}

void HShowerCalPar::setSetup(Int_t nSectors, Int_t nModules,
   Int_t  nRows, Int_t nColumns)
{
  m_nSectors = nSectors;
  m_nModules = nModules;
  m_nRows = nRows;
  m_nColumns = nColumns;
} 

void HShowerCalPar::setReadParam(HShowerReadParam* pReadParam)
{
  m_pReadParam = pReadParam;
}

HShowerReadParam* HShowerCalPar::getReadParam()
{
  return m_pReadParam;
}

void HShowerCalPar::setCellClassName(Char_t* pszName)
{
  strncpy(m_szClassName, pszName, sizeof(m_szClassName));
}

Char_t* HShowerCalPar::getCellClassName()
{
  return m_szClassName;
}


HShowerCalParCell* HShowerCalPar::getSlot(HLocation &loc)
{
  return (HShowerCalParCell*) m_ParamsTable.getSlot(loc);
}

HShowerCalParCell* HShowerCalPar::getObject(HLocation &loc)
{
  return (HShowerCalParCell*) m_ParamsTable.getObject(loc);
}

Float_t HShowerCalPar::getSlope(HLocation &loc)
{
  return getObject(loc)->getSlope();
}

Float_t HShowerCalPar::getOffset(HLocation &loc)
{
  return getObject(loc)->getOffset();
}

void HShowerCalPar::setSlope(HLocation &loc, Float_t fSlope)
{
  getObject(loc)->setSlope(fSlope);
}

void HShowerCalPar::setOffset(HLocation &loc, Float_t fOffset)
{
  getObject(loc)->setOffset(fOffset);
}

Bool_t HShowerCalPar::init()
{
  HShowerDetector *pShowerDet = (HShowerDetector*)gHades->getSetup()
                                                  ->getDetector("Shower");
  if (!pShowerDet) return kFALSE;

  m_nSectors = pShowerDet->getShowerSectors(); 
  m_nModules = pShowerDet->getShowerModules(); 
  m_nRows = pShowerDet->getRows(); 
  m_nColumns = pShowerDet->getColumns(); 
  
  m_ParamsTable.set(4, m_nSectors, m_nModules, m_nRows, m_nColumns);
  
  m_ParamsTable.setCellClassName(getCellClassName());
  m_ParamsTable.makeObjTable();

  if (m_pReadParam) {
    m_pReadParam->setSetup(m_nSectors, m_nModules);
    m_pReadParam->setCalPar(this);

    return m_pReadParam->readParams();
  }

  printf("Shower calpar default initialization\n");
  return kTRUE;
}

