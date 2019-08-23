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
#include "hshowerhitfpar.h"
ClassImp(HShowerHitFPar)

////////// ********** HShowerHitFPar defintion ****************/////////////77

HShowerHitFPar::HShowerHitFPar() {
  m_pReadParam = NULL;
  clear();
}

HShowerHitFPar::~HShowerHitFPar()
{
  clear(); 
}

void HShowerHitFPar::setHadIoAscii(HadIoAscii* pReadParam)
{
  m_pReadParam = pReadParam;
}

HadIoAscii* HShowerHitFPar::getHadIoAscii()
{
  return m_pReadParam;
}

void HShowerHitFPar::defaultInit() {
  int nLower = 0;
  int nUpper = 31;

  nSectors = 6;
  nModules = 3;
  nThreshold = 0;

  for(int i = 0; i < nSectors; i++)
    for(int j = 0; j < nModules; j++) {
       setColBord(i, j, nLower, nUpper);
       setRowBord(i, j, nLower, nUpper);
  }
 

}

Bool_t HShowerHitFPar::init()
{
  int nLower, nUpper, nInd;
  if (!m_pReadParam) {
    defaultInit();
    return kTRUE;
  }

  HadIoAscii &mydata = *m_pReadParam;

  clear();
  mydata.echo = 0;

  mydata.open();
 
  nSectors = mydata.getIntData("sectors"); 
  nModules = mydata.getIntData("modules"); 
  nThreshold = mydata.getIntData("threshold"); 

  pColBorder = new int[nSectors * nModules * 2];
  pRowBorder = new int[nSectors * nModules * 2];

  nInd = 0;
  for(int i = 0; i < nSectors; i++) 
    for(int j = 0; j < nModules; j++) {
       nLower = mydata.getIntData("cols", nInd++); 
       nUpper = mydata.getIntData("cols", nInd++); 

       setColBord(i, j, nLower, nUpper);
  }

  nInd = 0;
  for(int i = 0; i < nSectors; i++)
    for(int j = 0; j < nModules; j++) {
       nLower = mydata.getIntData("rows", nInd++);
       nUpper = mydata.getIntData("rows", nInd++);

       setRowBord(i, j, nLower, nUpper);
  }


  mydata.close();

  return 1;
}

void HShowerHitFPar::clear()
{
  nModules = 0;
  nSectors = 0;

  if (pColBorder) 
     delete pColBorder;
  pColBorder = NULL;

  if (pRowBorder) 
     delete pRowBorder;
  pRowBorder = NULL;
}

Int_t HShowerHitFPar::getIndex(Int_t nSect, Int_t nMod) {
  return (nMod + nSect * nModules) * 2;
}

Int_t HShowerHitFPar::setColBord(Int_t nSect, Int_t nMod, 
                             Int_t nLowerCol, Int_t nUpperCol) {
  if ((nSect < 0) || (nSect >= nSectors)) return 0;
  if ((nMod < 0) || (nMod >= nModules)) return 0;

  if (pColBorder) {
      int nIdx = getIndex(nSect, nMod);
      pColBorder[nIdx] = nLowerCol;
      pColBorder[nIdx + 1] =  nUpperCol;
  }

  return 1;
}

Int_t HShowerHitFPar::setRowBord(Int_t nSect, Int_t nMod, 
                             Int_t nLowerRow, Int_t nUpperRow) {
  if ((nSect < 0) || (nSect >= nSectors)) return 0;
  if ((nMod < 0) || (nMod >= nModules)) return 0;

  if (pRowBorder) {
      int nIdx = getIndex(nSect, nMod);
      pRowBorder[nIdx] = nLowerRow;
      pRowBorder[nIdx + 1] = nUpperRow;
  }

  return 1;
}

Int_t HShowerHitFPar::getColBord(Int_t nSect, Int_t nMod, 
                             Int_t* nLowerCol, Int_t* nUpperCol) {
  if ((nSect < 0) || (nSect >= nSectors)) return 0;
  if ((nMod < 0) || (nMod >= nModules)) return 0;

  if (pColBorder) {
      int nIdx = getIndex(nSect, nMod);
      *nLowerCol = pColBorder[nIdx];
      *nUpperCol = pColBorder[nIdx + 1];
  }

  return 1;
}

Int_t HShowerHitFPar::getRowBord(Int_t nSect, Int_t nMod, 
                             Int_t* nLowerRow, Int_t* nUpperRow) {
  if ((nSect < 0) || (nSect >= nSectors)) return 0;
  if ((nMod < 0) || (nMod >= nModules)) return 0;

  if (pRowBorder) {
      int nIdx = getIndex(nSect, nMod);
      *nLowerRow = pRowBorder[nIdx];
      *nUpperRow = pRowBorder[nIdx + 1];
  }

  return 1;
}


