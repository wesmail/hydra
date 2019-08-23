#pragma implementation
#include "hshowerhitfpar.h"
#include "hparhadasciifileio.h"
ClassImp(HShowerHitFPar)

/////////////////////////////////////////////////////////
//
// HShowerHitFPar 
// It contains parameters for hit finding process
//
/////////////////////////////////////////////////////////

HShowerHitFPar::HShowerHitFPar() {
  SetName("ShowerHitFinderParams");

  clear();
  m_fGainPost1 = 1.9;
  m_fThresholdPost1 = 0.0;

  m_fGainPost2 = 1.9;
  m_fThresholdPost2 = 0.0;

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
//reading and initializing analysis parameters from ascii file (HAsciiKey format)
{
  int nLower, nUpper, nInd;
  int status=kTRUE;

  if (!pHadAsciiFile) return kFALSE;

  try {

    HAsciiKey* pHAscii = pHadAsciiFile->GetKeyAscii();
    HAsciiKey &mydata = *pHAscii;

    mydata.SetActiveSection("Shower Hit Finder Parameters");

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

    mydata.SetActiveSection("Shower Criterium - Post 1");
    m_fGainPost1 = mydata.ReadFloat("gain"); 
    m_fThresholdPost1 = mydata.ReadFloat("threshold"); 
   
    mydata.SetActiveSection("Shower Criterium - Post 2");
    m_fGainPost2 = mydata.ReadFloat("gain"); 
    m_fThresholdPost2 = mydata.ReadFloat("threshold"); 
  }
  catch (Bool_t ret) {
    status = ret;
  }

  return status;
}

Bool_t HShowerHitFPar::writeAscii(HParHadAsciiFileIo* pHadAsciiFile)
{
//writing analysis parameters into ascii file (HAsciiKey format)

  Bool_t status = kTRUE;
  int nLower, nUpper, nInd;
  int nTab[36];  //max. size of  table

  if (!pHadAsciiFile) return kFALSE;
 
  try {
     HAsciiKey* pHAscii = pHadAsciiFile->GetKeyAscii();
     HAsciiKey &mydata = *pHAscii;

     mydata.WriteSection("Shower Hit Finder Parameters");

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

     mydata.WriteSection("Shower Criterium - Post 1");
     mydata.WriteFloat("gain", m_fGainPost1); 
     mydata.WriteFloat("threshold", m_fThresholdPost1); 
   
     mydata.WriteSection("Shower Criterium - Post 2");
     mydata.WriteFloat("gain", m_fGainPost2); 
     mydata.WriteFloat("threshold", m_fThresholdPost2); 
  }
  catch(Bool_t ret) {
    status = ret;
  }

  return status;
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
      R__b >> m_fGainPost1;
      R__b >> m_fThresholdPost1;
      R__b >> m_fGainPost2;
      R__b >> m_fThresholdPost2;
   } else {
      R__b.WriteVersion(HShowerHitFPar::IsA());
      HParSet::Streamer(R__b);
      R__b << nSectors;
      R__b << nModules;
      nSize = nSectors*nModules*2;
      R__b.WriteFastArray(pRowBorder, nSize);
      R__b.WriteFastArray(pColBorder, nSize);
      R__b << nThreshold;
      R__b << m_fGainPost1;
      R__b << m_fThresholdPost1;
      R__b << m_fGainPost2;
      R__b << m_fThresholdPost2;
   }
}


