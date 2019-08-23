#include <iostream.h>
#include "hades.h"
#include "hparhadasciifileio.h"
#include "hspectrometer.h"
#include "hrichparset.h"
#include "hrichdetector.h"
#include "hrichcalpar.h"

ClassImp(HRichCalPar)

////////// **********RichCalPar defintion ****************/////////////77
HRichCalPar::HRichCalPar() {
  m_pReadParam = NULL;

  setSetup(6, 32, 32); //default setup 
  setCellClassName("HRichCalParCell");

  SetName("RichCalPar");
}

HRichCalPar::~HRichCalPar()
{ 
  m_ParamsTable.deleteTab();
}

void HRichCalPar::setSetup(Short_t nSectors, Short_t  nRows, Short_t nColumns)
{
  fSectors = nSectors;
  fRows = nRows;
  fColumns = nColumns;
} 

void HRichCalPar::setReadParam(HRichReadParam* pReadParam)
{
  m_pReadParam = pReadParam;
}

HRichReadParam* HRichCalPar::getReadParam()
{
  return m_pReadParam;
}

void HRichCalPar::setCellClassName(Char_t* pszName)
{
  strncpy(m_szClassName, pszName, sizeof(m_szClassName));
}

Char_t* HRichCalPar::getCellClassName()
{
  return m_szClassName;
}


HRichCalParCell* HRichCalPar::getSlot(HLocation &loc)
{
  return (HRichCalParCell*) m_ParamsTable.getSlot(loc);
}

HRichCalParCell* HRichCalPar::getObject(HLocation &loc)
{
  return (HRichCalParCell*) m_ParamsTable.getObject(loc);
}

Float_t HRichCalPar::getSlope(HLocation &loc)
{
  return getObject(loc)->getSlope();
}

Float_t HRichCalPar::getOffset(HLocation &loc)
{
  return getObject(loc)->getOffset();
}

void HRichCalPar::setSlope(HLocation &loc, Float_t fSlope)
{
  getObject(loc)->setSlope(fSlope);
}

void HRichCalPar::setOffset(HLocation &loc, Float_t fOffset)
{
  getObject(loc)->setOffset(fOffset);
}


Bool_t HRichCalPar::initAscii(HParHadAsciiFileIo* pHadAsciiFile) {
  if (!pHadAsciiFile) return kFALSE;

  char sectname[60];
  int sect, row, col, row1, col1;
  float slope, offset;
  HLocation loc;
  HRichCalParCell *pCell;
 
  loc.setNIndex(3);
 
  HAsciiRaw* pHAscii = pHadAsciiFile->GetRawAscii();
  HAsciiRaw &mydata = *pHAscii;

  if (!mydata.SetActiveSection("Rich Calibrater Parameters Setup"))
     return kFALSE;

  mydata.SetReadKeyTypes('i', 'i', 'i');
  mydata.ReadRawLine(0,&fSectors,&fRows,&fColumns);

  setSetup(fSectors, fRows, fColumns);
  m_ParamsTable.set(3, fSectors, fRows, fColumns);
  m_ParamsTable.setCellClassName(getCellClassName());
  m_ParamsTable.makeObjTable();


  for(sect = 0; sect < fSectors; sect++) {
       sprintf(sectname, "Rich Calibrater Parameters: Sector %d", sect); 
       if (!mydata.SetActiveSection(sectname))
          return kFALSE;

       mydata.SetReadKeyTypes('i', 'i', 'f', 'f');

       loc[0] = sect;

       int line = 0;
       for(row = 0; row < fRows; row++)
         for(col = 0; col < fColumns; col++) {
            loc[1] = row;
            loc[2] = col;
            mydata.ReadRawLine(line, &row1, &col1, &slope, &offset);
            if(row!=row1 || col!=col1) {
             printf("error");
             return kFALSE;
            }

            pCell = (HRichCalParCell*) getSlot(loc);
            if (pCell) {
              pCell = new(pCell) HRichCalParCell;
              pCell->setSlope(slope);
              pCell->setOffset(offset); 
            }
            line++;
       }
  }

 return kTRUE;
}


Bool_t HRichCalPar::writeAscii(HParHadAsciiFileIo* pHadAsciiFile) {
  if (!pHadAsciiFile) return kFALSE;

  char sectname[60];
  int sect, row, col;
  float slope, offset;
  HLocation loc;

  loc.setNIndex(3);

  HAsciiRaw* pHAscii = pHadAsciiFile->GetRawAscii();
  HAsciiRaw &mydata = *pHAscii;

  mydata.SetSeparator('\t');
  mydata.WriteSection("Rich Calibrater Parameters Setup");
  mydata.WriteKeywords(3, "Sector", "Row", "Column");
  mydata.SetWriteKeyTypes('i', 'i', 'i');
  mydata.WriteRawLine(fSectors, fRows, fColumns);
 
  for(sect = 0; sect < fSectors; sect++) {
       sprintf(sectname, "Rich Calibrater Parameters: Sector %d", sect);
       mydata.WriteSection(sectname);
       mydata.WriteKeywords(4, "Row", "Column", "Slope", "Offset");
       mydata.SetWriteKeyTypes('i', 'i', 'f', 'f');

       loc[0] = sect;

       for(row = 0; row < fRows; row++)
         for(col = 0; col < fColumns; col++) {
            loc[1] = row;
            loc[2] = col;
            slope = getSlope(loc);
            offset = getOffset(loc);

            mydata.WriteRawLine(row, col, slope, offset);
       }
  }
 
  return kTRUE;
}


Bool_t HRichCalPar::initRP()
{
  printf("initialization of RichCalPar\n");

  HRichDetector *pRichDet = (HRichDetector*)gHades->getSetup()->getDetector("Rich");
  if (!pRichDet) return kFALSE;

  fSectors = pRichDet->getSectors(); 
  fRows = pRichDet->getRows(); 
  fColumns = pRichDet->getColumns(); 
 
  printf("%d - %d - %d\n", fSectors, fRows, fColumns); 
  m_ParamsTable.set(3, fSectors, fRows, fColumns);
  
  m_ParamsTable.setCellClassName(getCellClassName());
  m_ParamsTable.makeObjTable();

  if (m_pReadParam) {
    m_pReadParam->setSetup(fSectors);
    m_pReadParam->setCalPar(this);

    return m_pReadParam->readParams();
  }

  printf("Rich calpar default initialization\n");
  return kTRUE;
}

Bool_t HRichCalPar::defaultInit()
{
  printf("initialization of RichCalPar\n");

  HRichDetector *pRichDet = (HRichDetector*)gHades->getSetup()->getDetector("Rich");
  if (!pRichDet) return kFALSE;
 
  fSectors = pRichDet->getSectors();
  fRows = pRichDet->getRows();
  fColumns = pRichDet->getColumns();

  printf("%d - %d - %d\n", fSectors, fRows, fColumns);
  m_ParamsTable.set(3, fSectors, fRows, fColumns);

  m_ParamsTable.setCellClassName(getCellClassName());
  m_ParamsTable.makeObjTable();

  printf("Rich calpar default initialization\n");
  return kTRUE;
}

void HRichCalPar::PrintCalPar() {
  int i, j, k, l;
  float slope, offset;

  HLocation loc;

  loc.setNIndex(3);
  for (i = 0; i < fSectors; i++)
      for (k = 0; k < fRows; k++)
          for (l = 0; l < fColumns; l++) {
             loc[0] = i;
             loc[1] = k;
             loc[2] = l;
             slope = getSlope(loc);
             offset = getOffset(loc);
             
             if (offset < 0) 
             printf("%d:%d:%d %f %f\n", i, k, l, slope, offset);
          }
    
}
