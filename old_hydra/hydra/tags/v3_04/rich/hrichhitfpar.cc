//###################################################

#pragma implementation
#include "hrichhitfpar.h"
#include "hparhadasciifileio.h"

ClassImp(HRichHitFPar)


HRichHitFPar::HRichHitFPar() {
  SetName("RichHitFinderParams");

  clear();
}

HRichHitFPar::~HRichHitFPar()
{
  clear(); 
}

void HRichHitFPar::defaultInit() {
  int nLower = 0;
  int nUpper = 31;

  nSectors = 6;
  nThreshold = 0;

  for(int i = 0; i < nSectors; i++) {
    setColBord(i, nLower, nUpper);
    setRowBord(i, nLower, nUpper);
  }
 

}

Bool_t HRichHitFPar::initAscii(HParHadAsciiFileIo* pHadAsciiFile)
{
  int nLower, nUpper, nInd;

  if (!pHadAsciiFile) return kFALSE;

  HAsciiKey* pHAscii = pHadAsciiFile->GetKeyAscii();
  HAsciiKey &mydata = *pHAscii;
/* ========================== this MUST be changed 
  if (!mydata.SetActiveSection("Rich Hit Finder Parameters"))
      return kFALSE;

  nSectors = mydata.ReadInt("sectors"); 
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
========================================== */
  return 1;
}

void HRichHitFPar::clear()
{
  nSectors = 0;

  if (pColBorder) delete pColBorder;
  pColBorder = NULL;

  if (pRowBorder) delete pRowBorder;
  pRowBorder = NULL;
}

Int_t HRichHitFPar::getIndex(Int_t nSect) {
// ===================== MUST check it
  return (nSect * 2);
}

Int_t HRichHitFPar::setColBord(Int_t nSect,  
                             Int_t nLowerCol, Int_t nUpperCol) {
  if ((nSect < 0) || (nSect >= nSectors)) return 0;

  if (pColBorder) {
      int nIdx = getIndex(nSect);
      pColBorder[nIdx] = nLowerCol;
      pColBorder[nIdx + 1] =  nUpperCol;
  }

  return 1;
}

Int_t HRichHitFPar::setRowBord(Int_t nSect, 
                             Int_t nLowerRow, Int_t nUpperRow) {
  if ((nSect < 0) || (nSect >= nSectors)) return 0;

  if (pRowBorder) {
      int nIdx = getIndex(nSect);
      pRowBorder[nIdx] = nLowerRow;
      pRowBorder[nIdx + 1] = nUpperRow;
  }

  return 1;
}

Int_t HRichHitFPar::getColBord(Int_t nSect, 
                             Int_t* nLowerCol, Int_t* nUpperCol) {
  if ((nSect < 0) || (nSect >= nSectors)) return 0;

  if (pColBorder) {
      int nIdx = getIndex(nSect);
      *nLowerCol = pColBorder[nIdx];
      *nUpperCol = pColBorder[nIdx + 1];
  }

  return 1;
}

Int_t HRichHitFPar::getRowBord(Int_t nSect, 
                             Int_t* nLowerRow, Int_t* nUpperRow) {
  if ((nSect < 0) || (nSect >= nSectors)) return 0;

  if (pRowBorder) {
      int nIdx = getIndex(nSect);
      *nLowerRow = pRowBorder[nIdx];
      *nUpperRow = pRowBorder[nIdx + 1];
  }

  return 1;
}

//______________________________________________________________________________
void HRichHitFPar::Streamer(TBuffer &R__b)
{
   // Stream an object of class HRichHitFPar.
   Int_t nSize;
/* ================================ MUST be changed
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(); if (R__v) { }
      HParSet::Streamer(R__b);
      R__b >> nSectors;

      nSize = nSectors*2;
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
      R__b.WriteVersion(HRichHitFPar::IsA());
      HParSet::Streamer(R__b);
      R__b << nSectors;
      nSize = nSectors*2;
      R__b.WriteFastArray(pRowBorder, nSize);
      R__b.WriteFastArray(pColBorder, nSize);
      R__b << nThreshold;
//      R__b << m_pReadParam;
   }
==============================================*/
}


