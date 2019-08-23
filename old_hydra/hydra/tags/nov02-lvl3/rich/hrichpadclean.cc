// File: hrichpadclean.cc
// ***************************************************************************

//*-- Author : Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
//*-- Modified : 1999/12/04 by Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
//*-- Modified : Oct. 2000 W. Koenig
using namespace std;
#include <stdlib.h>
#include <iostream> 
#include <iomanip>
#include "hrichpadclean.h"

// ***************************************************************************
//////////////////////////////////////////////////////////////////////////////
//
//  HRichPadClean
//
//  This class cleans pad plane provided by HRichAnalysis.
//
//////////////////////////////////////////////////////////////////////////////
// ***************************************************************************


ClassImp(HRichPadClean)

//----------------------------------------------------------------------------
HRichPadClean::HRichPadClean() {
 iCount = 0;
 iTempCluster.Set(100);
}
//============================================================================

//----------------------------------------------------------------------------
HRichPadClean::~HRichPadClean() {}
//============================================================================

//----------------------------------------------------------------------------
HRichPadClean::HRichPadClean(const HRichPadClean& source) {
 iCount = source.iCount;
 iTempCluster = source.iTempCluster;
}
//============================================================================

//----------------------------------------------------------------------------
HRichPadClean& HRichPadClean::operator=(const HRichPadClean& source) {
 if (this != &source) {
  iCount = source.iCount;
  iTempCluster = source.iTempCluster;
 }
return *this;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichPadClean::CleanAlonePad(HRichAnalysis *showMe, Int_t border, Int_t lowerThr) {
    // this method deletes all single pad under threshold (lowerThr).
    // A single pad is defined as pad whose first fired neighbour
    // has at least a distance of "border" from the pad (4 pads at the moment).

Int_t k,l;
iCount = 0;

if (!border) {
  cerr << "Argument \'border\' in function CleanAlonePad must be larger than 0!\n";
  exit(1);
}

#ifdef HRICH_DEBUGMODE
 cout << "RICH DEBUG MODE: CleanAlonePad cleans the following pads: \n";
#endif
 
 HRichPadSignal * pad;
 Int_t offset;
 Int_t maxCols = showMe->GetPadsXNr();
 Int_t maxRows = showMe->GetPadsYNr();

 for(int row=0;row<maxRows;++row) {
  Int_t leftBorder=showMe->pLeftBorder[row];
  Int_t rightBorder=showMe->pRightBorder[row];
  pad = showMe->GetPad(leftBorder+row*maxCols);
  for(int col=leftBorder; col<=rightBorder;++col) {
   if (pad->getAmplitude() > 0) {
    for (k = row-border; k <= row+border; ++k) {
      offset = k*maxCols;
      for (l = col-border; l <= col+border; ++l)
        if (!(l == col && k == row))
          if (!showMe->IsOut(l,k))
            if (showMe->GetPad(l+offset)->getAmplitude() > 0) goto nextPad;
    }

    if (pad->getAmplitude() < lowerThr) {
      pad->setAmplitude(0);
      iCount++;

#ifdef HRICH_DEBUGMODE
      // cout << " (" << colStart+border << "," << rowStart+border << ")... ";
#endif

    }
   }
   nextPad:
   ++pad;
  }
 }

#ifdef HRICH_DEBUGMODE
 if (!iCount) cout << "None. \n";
 else cout << "\nRICH DEBUG MODE: Total number of pads cleaned by "
              "CleanAlonePad = " << iCount << "\n";
#endif

return iCount;
}
//============================================================================

//----------------------------------------------------------------------------
void HRichPadClean::DeletePulse(HRichAnalysis *showYou, Int_t border, Int_t col, Int_t row) {
    // this method deletes a given pad and all the neighbours within
    //  a distance of "border"( 4 pads).
 Int_t maxCols = showYou->GetPadsXNr();
 showYou->GetPad(col+maxCols*row)->setAmplitude(0);

 iCount++;

#ifdef HRICH_DEBUGMODE
 // cout << " (" << colStart+border << "," << rowStart+border << ")... ";
#endif

 Int_t j;

 for (Int_t i = row-border; i <= row+border; ++i) {
  Int_t offset = maxCols*i;
  for (j = col-border; j <= col+border; ++j)
   if (!showYou->IsOut(j,i))
    if(!(i == row && j == col))
     if (showYou->GetPad(j + offset)->getAmplitude() > 0)
      DeletePulse(showYou, border, j, i);
 }
 return;
}

//============================================================================

//----------------------------------------------------------------------------
Int_t HRichPadClean::CleanHighPulse(HRichAnalysis *showMe, Int_t border, Int_t upperThr) {
    //
    // this method deletes all the pads above upper threshold (upperThr)
    // and the cluster connected to each pad.
    //
 Int_t iCountBefore = 0;

 iCount = 0;

#ifdef HRICH_DEBUGMODE
 cout << "RICH DEBUG MODE: DeletePulse cleans the following pads: \n";
#endif
 Int_t maxRows = showMe->GetPadsYNr();
 Int_t maxCols = showMe->GetPadsXNr();
 Int_t nCleaned = 0;

 for(int row=0; row<maxRows; ++row) {
  Int_t offset=row*maxCols;
  Int_t rightBorder=showMe->pRightBorder[row];
  for(int col=showMe->pLeftBorder[row]; col<=rightBorder; ++col) {
   if (showMe->GetPad(col+offset)->getAmplitude() > upperThr) {
    iCountBefore = iCount;
    DeletePulse(showMe,border,col, row);
    if ((iCount-iCountBefore) > 0 && nCleaned < 100) {
     iTempCluster[nCleaned++] = iCount-iCountBefore;
    }
   }
  }
 }
 showMe->iClusterCleanedNr = nCleaned;
 showMe->iClustersCleaned.Set(nCleaned);
 for (int i = 0; i < nCleaned; i++)
 showMe->iClustersCleaned[i] = iTempCluster[i];

#ifdef HRICH_DEBUGMODE
 if (!iCount) cout << "None. \n" << "RICH DEBUG MODE: Total number "
                      "of pads cleaned by CleanHighPulse = 0\n";
 else cout << "\nRICH DEBUG MODE: Total number of pads cleaned "
              "by CleanHighPulse = " << iCount << "\n";
#endif

 return iCount;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichPadClean::Execute(HRichAnalysis *giveMe) {

if(giveMe->iPadFiredNr >2) {
  Int_t iReducedNr = 0;
  HRichAnalysisPar *analParams = giveMe->getParams(); 

  if (analParams->isActiveCleanAlonePad) 
   iReducedNr += CleanAlonePad(giveMe,analParams->iCleanAlonePadBorder,
                               analParams->iCleanAlonePadLowerThreshold);
  if (analParams->isActiveCleanHighPulse) 
   iReducedNr += CleanHighPulse(giveMe,analParams->iCleanHighPulseBorder,
                                analParams->iCleanHighPulseUpperThreshold);
  return (iReducedNr);
} else return giveMe->iPadFiredNr; //either 0, 1 or 2

}
//============================================================================

