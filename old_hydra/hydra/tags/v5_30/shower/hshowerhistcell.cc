#pragma implementation
#include "TH2.h"
#include "TH3.h"
#include "hshowerhistcell.h"

ClassImp(HShowerHistCell)

////////// **********ShowerHistCell defintion ****************/////////////77
HShowerHistCell::HShowerHistCell() {
   m_pHist = NULL;
   m_pSlopeHist = NULL;
   m_pOffsetHist = NULL;

   setSector(-1);
   setModule(-1);
   setRows(0);
   setCols(0);

   m_nDataSize = 0;
   m_pData = NULL;

   m_fChannel10pC = 50.0;
}

HShowerHistCell::HShowerHistCell(Int_t nSect, Int_t nMod, 
                                                  Int_t nRows, Int_t nCols) {
   m_pHist = NULL;
   m_pSlopeHist = NULL;
   m_pOffsetHist = NULL;

   setSector(nSect);
   setModule(nMod);
   setRows(nRows);
   setCols(nCols);

   m_nDataSize = 0;
   m_pData = NULL;

   m_fChannel10pC = 50.0;
}

HShowerHistCell::~HShowerHistCell() {
   deleteHist();
}

Bool_t HShowerHistCell::reset()
{
   m_pHist->Reset();
   m_pSlopeHist->Reset();
   m_pOffsetHist->Reset();
   return kTRUE;
}


Bool_t HShowerHistCell::book() {
   if (m_nRows*m_nCols==0) return kFALSE;

   char name[60];
   char title[60];
   char name2[60];
   char title2[60];

   deleteHist();

   getName(name, sizeof(name));
   getTitle(title, sizeof(title));

   m_pHist = new TH3S(name, title, 
                           m_nRows, 0, m_nRows,
                           m_nCols, 0, m_nCols,
                           256, 0, 256);

   m_nDataSize = m_pHist->GetNbinsZ();
   m_pData = new Int_t[m_nDataSize];

   sprintf(name2, "%sSlope", name);
   sprintf(title2, "%s - Slope", title);
   
   m_pSlopeHist = new TH2F(name2, title, 
                           m_nRows, 0, m_nRows,
                           m_nCols, 0, m_nCols);
   m_pSlopeHist->GetXaxis()->SetTitle("columns");
   m_pSlopeHist->GetYaxis()->SetTitle("rows");


   sprintf(name2, "%sOffset", name);
   sprintf(title2, "%s - Offset", title);
  
   m_pOffsetHist = new TH2F(name2, title, 
                           m_nRows, 0, m_nRows,
                           m_nCols, 0, m_nCols);

   m_pOffsetHist->GetXaxis()->SetTitle("columns");
   m_pOffsetHist->GetYaxis()->SetTitle("rows");


   return kTRUE;
}


Bool_t HShowerHistCell::book(Int_t nRows, Int_t nCols) {
   if (nRows * nCols == 0) return kFALSE;

   setRows(nRows);
   setCols(nCols);

   return book();
}

Bool_t HShowerHistCell::fill(Int_t nRow, Int_t nCol, Int_t nVal) {
   if (!m_pHist) return kFALSE;
    
   m_pHist->Fill(nRow, nCol, nVal);

   return kTRUE;
}

Bool_t HShowerHistCell::draw(Int_t nRow, Int_t nCol, Option_t* opt) {
   if (!m_pHist) return kFALSE;

//   m_pHist->Draw(opt);
   return kTRUE;
}


void HShowerHistCell::deleteHist() {
   if (m_pHist) {
//     delete m_pHist;
     m_pHist = NULL;
   }
   if (m_pSlopeHist) {
//     delete m_pSlopeHist;
     m_pSlopeHist = NULL;
   }
   if (m_pOffsetHist) {
//     delete m_pOffsetHist;
     m_pOffsetHist = NULL;
   }

   if (m_pData) {
      delete [] m_pData;
      m_pData = NULL;
      m_nDataSize = 0;
   }
}

void HShowerHistCell::getName(Char_t* name, Int_t nSize) {
  char buf[40];

  sprintf(buf, "S%d%d", m_nSector, m_nModule);
  strncpy(name, buf, nSize);
} 

void HShowerHistCell::getTitle(Char_t* title, Int_t nSize) {
  char buf[60];
  char mods[3][6] = {"pre", "post", "post2"};

  sprintf(buf, "Shower Histogram: Sector %d %s", 
                        m_nSector, mods[m_nModule]);
  strncpy(title, buf, nSize);
}

void HShowerHistCell::writeHistogram() { 
//    m_pHist->Write();
    m_pSlopeHist->Write();
    m_pOffsetHist->Write();
}

/* obliczanie parametrow kalibracji z dwoch pikow */
void HShowerHistCell::calculate2(Int_t nRow, Int_t nCol,
                                     Float_t *pfSlope, Float_t *pfOffset) {

  int nBins = m_pHist->GetNbinsZ();
  int nMax; 
  float fMean, fMean1, fMean2;

  *pfOffset = -250.0;
  *pfSlope = 0.0;

  fillData(nRow, nCol, m_nDataSize, m_pData);
  nMax = getMax(m_nDataSize, m_pData);

  fMean = getMean(m_nDataSize, m_pData, 0, nBins, nMax/5);

  fMean1 = getMean(m_nDataSize, m_pData, 0, fMean, nMax/5);

  fMean2 = getMean(m_nDataSize, m_pData, fMean, nBins, nMax/5);

  if ((fMean > 0) && (fMean1 > 0) && (fMean2 > 0))
    if (fMean2 - fMean1 > m_fChannel10pC/4) {
      *pfOffset = -fMean1 - 3;
      *pfSlope = m_fChannel10pC / (fMean2 - fMean1);
    }

  m_pSlopeHist->Fill(nCol, nRow, *pfSlope);
  m_pOffsetHist->Fill(nCol, nRow, -*pfOffset);
}

/* obliczanie parametrow kalibracji z jednego piku */
/* oblicznie tylko offsetu - slope = 1             */
void HShowerHistCell::calculate1(Int_t nRow, Int_t nCol,
                                     Float_t *pfSlope, Float_t *pfOffset) {

  int nBins = m_pHist->GetNbinsZ();
  int nMax;
  float fMean, fMean1, fMean2;

  *pfOffset = 0.0;
  *pfSlope = 1.0;

  fillData(nRow, nCol, m_nDataSize, m_pData);
  nMax = getMax(m_nDataSize, m_pData);
  fMean = getMean(m_nDataSize, m_pData, 0, nBins, nMax/2);

  if (fMean > 0)  *pfOffset = -fMean - 1;

  m_pSlopeHist->Fill(nCol, nRow, *pfSlope);
  m_pOffsetHist->Fill(nCol, nRow, -*pfOffset);


}
void HShowerHistCell::fillData(Int_t nRow, Int_t nCol, 
                                             Int_t nBins, Int_t *pData) {
  int i;
  int bin;
  int binx, biny, binz;
  for(i = 0; i < nBins; i++) {
    binx = m_pHist->GetXaxis()->FindBin(nRow);
    biny = m_pHist->GetYaxis()->FindBin(nCol);
    binz = m_pHist->GetZaxis()->FindBin(i);
    bin = m_pHist->GetBin(binx, biny, binz);
    pData[i] = (Int_t)m_pHist->GetBinContent(bin);
  }
}

Int_t HShowerHistCell::getMax(Int_t nSize, Int_t* pData) {
  int i = 0; 
  Int_t nMax = 0;
  for (i = 0; i < nSize; i++) 
    if (nMax < pData[i])
       nMax =  pData[i];

  return nMax;
}

Float_t HShowerHistCell::getMean(Int_t nBins, Int_t * pData, Int_t nMin,  
                                             Int_t nMax,  Int_t nThreshold) {
   Int_t nSum = 0;
   Int_t nE = 0;

   Int_t nStart = (nMin < 0) ? 0 : nMin;
   Int_t nStop = (nMax > nBins) ? nBins : nMax;

   for(int i = nStart; i < nStop; i++) 
      if (pData[i] >= nThreshold) {
        nSum += pData[i];
        nE += i*pData[i];
      }
   
   return (nSum == 0) ? -1.0 : ((float)nE)/nSum;
}
      
   
