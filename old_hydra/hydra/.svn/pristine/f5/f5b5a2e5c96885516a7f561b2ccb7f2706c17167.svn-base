//####################################################
//#
//#
//#       Author:                           L. Kidon
//#    
//#       last modified by L. Kidon 23.02.2000
// ###################################################

#pragma implementation
#include "hshowerhitfpar.h"
#include "hparhadasciifileio.h"
ClassImp(HShowerHitFPar)

////////// ********** HShowerHitFPar defintion ****************/////////////77

HShowerHitFPar::HShowerHitFPar() {
  SetName("ShowerHitFinderParams");

  clear();
}

HShowerHitFPar::~HShowerHitFPar()
{
  clear(); 
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


Bool_t HShowerHitFPar::initAscii(HParHadAsciiFileIo* pHadAsciiFile)
{
  int nLower, nUpper, nInd;

  if (!pHadAsciiFile) return kFALSE;

  HAsciiKey* pHAscii = pHadAsciiFile->GetKeyAscii();
  HAsciiKey &mydata = *pHAscii;

  if (!mydata.SetActiveSection("Shower Hit Finder Parameters"))
      return kFALSE;

  nSectors = mydata.ReadInt("sectors"); 
  nModules = mydata.ReadInt("modules"); 
  nThreshold = mydata.ReadInt("threshold"); 

  pColBorder = new int[nSectors * nModules * 2];
  pRowBorder = new int[nSectors * nModules * 2];

  nInd = 0;
  for(int i = 0; i < nSectors; i++) 
    for(int j = 0; j < nModules; j++) {
       nLower = mydata.ReadInt("cols", nInd++); 
       nUpper = mydata.ReadInt("cols", nInd++); 

       setColBord(i, j, nLower, nUpper);
  }

  nInd = 0;
  for(int i = 0; i < nSectors; i++)
    for(int j = 0; j < nModules; j++) {
       nLower = mydata.ReadInt("rows", nInd++);
       nUpper = mydata.ReadInt("rows", nInd++);

       setRowBord(i, j, nLower, nUpper);
  }

  return 1;
}

Bool_t HShowerHitFPar::writeAscii(HParHadAsciiFileIo* pHadAsciiFile)
{
  int nLower, nUpper, nInd;
  int nTab[36];  //max. size of  table

  if (!pHadAsciiFile) return kFALSE;

  HAsciiKey* pHAscii = pHadAsciiFile->GetKeyAscii();
  HAsciiKey &mydata = *pHAscii;

  if (!mydata.WriteSection("Shower Hit Finder Parameters"))
      return kFALSE;

  mydata.WriteInt("sectors", nSectors); 
  mydata.WriteInt("modules", nModules); 
  mydata.WriteInt("threshold", nThreshold); 

  mydata.SetNumInLine(2);
  nInd = 0;
  for(int i = 0; i < nSectors; i++)
    for(int j = 0; j < nModules; j++) {
       getColBord(i, j, &nLower, &nUpper);
       nTab[nInd++]=nLower;
       nTab[nInd++]=nUpper;
       
  }
  mydata.WriteInt("cols*", nInd, nTab);

  nInd = 0;
  for(int i = 0; i < nSectors; i++)
    for(int j = 0; j < nModules; j++) {
       getRowBord(i, j, &nLower, &nUpper);
       nTab[nInd++]=nLower;
       nTab[nInd++]=nUpper;
  }
  mydata.WriteInt("rows*", nInd, nTab);

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

//______________________________________________________________________________
void HShowerHitFPar::Streamer(TBuffer &R__b)
{
   // Stream an object of class HShowerHitFPar.
   Int_t nSize;

   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(); if (R__v) { }
      HParSet::Streamer(R__b);
      R__b >> nSectors;
      R__b >> nModules;

      nSize = nSectors*nModules*2;
      if (pRowBorder) delete[] pRowBorder;
      pRowBorder = new int[nSize];
      R__b.ReadFastArray(pRowBorder, nSize);

      if (pColBorder) delete[] pColBorder;
      pColBorder = new int[nSize];
      R__b.ReadFastArray(pColBorder, nSize);

      R__b >> nThreshold;
//      R__b >> m_pReadParam;
      m_pReadParam = NULL;
   } else {
      R__b.WriteVersion(HShowerHitFPar::IsA());
      HParSet::Streamer(R__b);
      R__b << nSectors;
      R__b << nModules;
      nSize = nSectors*nModules*2;
      R__b.WriteFastArray(pRowBorder, nSize);
      R__b.WriteFastArray(pColBorder, nSize);
      R__b << nThreshold;
//      R__b << m_pReadParam;
   }
}






