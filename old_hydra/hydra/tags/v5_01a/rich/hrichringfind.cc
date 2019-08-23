// File: hrichringfind.cc
// ***************************************************************************

//*-- Author : Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
//*-- Modified : 1999/12/04 by Witold Przygoda (przygoda@psja1.if.uj.edu.pl)


#include <stdlib.h>
#include <math.h>
#include <iostream.h>
#include "hrichringfind.h"
#include "hruntimedb.h"
#include "hrichanalysis.h"

// ***************************************************************************
//////////////////////////////////////////////////////////////////////////////
//
//  HRichRingFind
//
//  Ring recognition algorithms.
//
//////////////////////////////////////////////////////////////////////////////
// ***************************************************************************


ClassImp(HRichRingFind)

//----------------------------------------------------------------------------
HRichRingFind::HRichRingFind() { 
 iInnerCount = 0;
 iInnerPhot4 = 0;
 iInnerPhot8 = 0;
 iClusterSize.Set(50);
 iClusterLMax4.Set(50);
 iClusterLMax8.Set(50);
 iRingImageSize = 16;
 iRingTempImage.Set(256);
 iPadPlane.Set(16384); 
 iRingNr = 0;
 iAlgorithm1RingNr = 0;
 iAlgorithm2RingNr = 0;
 pRings = new HRichHit[10];
 pAlgorithm1Ring = new HRichHit[5];
 pAlgorithm2Ring = new HRichHit[5];

 pAnalysisParams = NULL;
 pGeometryParams = NULL;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichRingFind::init(HRichAnalysis* showMe) {

 pAnalysisParams = showMe->getParams();
 pGeometryParams = (HRichGeometryPar*)showMe->getGeomPar();

 iInnerCount = 0;
 iInnerPhot4 = 0;
 iInnerPhot8 = 0;
 iClusterSize.Set(50);
 iClusterLMax4.Set(50);
 iClusterLMax8.Set(50);
 iRingImageSize = pAnalysisParams->iRingMaskSize;
 iRingTempImage.Set(pAnalysisParams->iRingMaskSize * pAnalysisParams->iRingMaskSize);
 iPadPlane.Set(((HRichGeometryPar*)showMe->getGeomPar())->getPadsNr());
 iRingNr = 0;
 iAlgorithm1RingNr = 0;
 iAlgorithm2RingNr = 0;

 if (pRings) delete [] pRings;
 pRings = NULL;
 if (pAlgorithm1Ring) delete [] pAlgorithm1Ring;
 if (pAlgorithm2Ring) delete [] pAlgorithm2Ring;

 if (pAnalysisParams->iHowManyFitMatrixRings > 0)
  pAlgorithm1Ring = new HRichHit[pAnalysisParams->iHowManyFitMatrixRings];
 else pAlgorithm1Ring = NULL;
 if (pAnalysisParams->iHowManyHoughTransfRings > 0)
  pAlgorithm2Ring = new HRichHit[pAnalysisParams->iHowManyHoughTransfRings];
 else pAlgorithm2Ring = NULL;

 return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
HRichRingFind::~HRichRingFind() {
 if (pAlgorithm1Ring) delete [] pAlgorithm1Ring;
 if (pAlgorithm2Ring) delete [] pAlgorithm2Ring;
 if (pRings) delete [] pRings;
}
//============================================================================

//----------------------------------------------------------------------------
HRichRingFind::HRichRingFind(const HRichRingFind& source) {
 cerr << "HRichRingFind object can not be initialized with values of another object!\n";
 cerr << "Default constructor will be called.\n";
 HRichRingFind();
}
//============================================================================

//----------------------------------------------------------------------------
HRichRingFind& HRichRingFind::operator=(const HRichRingFind& source) {
 if (this != &source) {
  cerr << "HRichRingFind object can not be assigned!\n";
  cerr << "Default constructor will be called.\n";
  HRichRingFind();
 }
return *this;
}
//============================================================================

//----------------------------------------------------------------------------
Float_t HRichRingFind::CalcDistance(const HRichHit& ring1, const HRichHit& ring2) {
 return sqrt((ring1.iRingX-ring2.iRingX)*(ring1.iRingX-ring2.iRingX) + 
             (ring1.iRingY-ring2.iRingY)*(ring1.iRingY-ring2.iRingY));
}
//============================================================================

//----------------------------------------------------------------------------
Float_t HRichRingFind::CalcDistance(Int_t x, Int_t y, const HRichHit& ring) {
 return sqrt((x-ring.iRingX)*(x-ring.iRingX) + (y-ring.iRingY)*(y-ring.iRingY));
}
//============================================================================

//----------------------------------------------------------------------------
Float_t HRichRingFind::CalcDistance(Int_t x1, Int_t y1, Int_t x2, Int_t y2) {
 return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}
//============================================================================

//----------------------------------------------------------------------------
Double_t HRichRingFind::HomogenDistr(Double_t left, Double_t right) {
 return rand() / (double)RAND_MAX * (right - left) + left;
}
//============================================================================

//----------------------------------------------------------------------------
void HRichRingFind::RingFindFitMatrix(HRichAnalysis *showMe, 
                    Int_t minampl, Int_t distance, Int_t howmanyrings) {

Int_t i,j,k,m,n, lx,ly;
Int_t iRingQuality;
Int_t iRingDistance;

if (howmanyrings < 1) {
 howmanyrings = 1;
}

iAlgorithm1RingNr = 0;
for (i = 0; i < howmanyrings; i++) pAlgorithm1Ring[i].Reset();


for (n = 0; n < howmanyrings; n++) {
 for (m = 0; m < showMe->GetLabelNr(); m++) {
  for (j = showMe->GetLabel(m)->iLowerY; j <= showMe->GetLabel(m)->iUpperY; j++)
   for (i = showMe->GetLabel(m)->iLeftX; i <= showMe->GetLabel(m)->iRightX; i++) {

    iRingQuality = 0;
    iRingDistance = 1;
 
    for (k = 0; k < iAlgorithm1RingNr; k++) 
     if (CalcDistance(i,j,pAlgorithm1Ring[k]) <= distance) iRingDistance = 0;
   
    if (iRingDistance) {
     for (k = 0; k < (pAnalysisParams->iRingMatrixSize)*(pAnalysisParams->iRingMatrixSize); k++) {
      lx = (k % pAnalysisParams->iRingMatrixSize) - (pAnalysisParams->iRingMatrixSize / 2);
      ly = (k / pAnalysisParams->iRingMatrixSize) - (pAnalysisParams->iRingMatrixSize / 2);

      if (!showMe->IsOut(i,j,lx,ly))
       if (showMe->GetPad(i+lx,j+ly)->getAmplitude() > 0 && 
           showMe->GetPad(i+lx,j+ly)->getLabel() == showMe->GetLabel(m)->iSignature)
        iRingQuality += pAnalysisParams->iRingMatrix[k];

     }  // end of loop over all pattern matrix pads

     if (iRingQuality > pAlgorithm1Ring[n].iRingQuality) {
      pAlgorithm1Ring[n].iRingX = i;
      pAlgorithm1Ring[n].iRingY = j;
      pAlgorithm1Ring[n].iRingQuality = iRingQuality;
      pAlgorithm1Ring[n].iRingFreeParam = m;
     }
    }

   }  // end of loops over all pads of given label
 } // end of loops over all labels for the first ring

#ifdef HRICH_DEBUGMODE
 cout << "RICH DEBUG MODE: RichFindFitMatrix " << n+1 << ". ring candidate | x = " 
      << pAlgorithm1Ring[n].iRingX << " | y = " << pAlgorithm1Ring[n].iRingY 
      << " | quality = " << pAlgorithm1Ring[n].iRingQuality 
      << " | label nr = " << pAlgorithm1Ring[n].iRingFreeParam << "\n";
#endif

 if (!TestRing(showMe,pAlgorithm1Ring[n],minampl)) pAlgorithm1Ring[n].Reset();
 else {
  iAlgorithm1RingNr++; 
  pAlgorithm1Ring[n].iRingAlgorithmIndex = 1;
 }


#ifdef HRICH_DEBUGMODE
 if (pAlgorithm1Ring[n].iRingQuality)
  cout << n+1 << ". ring candidate stored for further processing\n";
 else cout << n+1 << ". ring candidate not accepted\n";
#endif

 if (pAlgorithm1Ring[n].iRingQuality == 0) return;
} // end of loop over all rings sought


return;
} // eof RingFindFitMatrix
//============================================================================

//----------------------------------------------------------------------------
void HRichRingFind::RingFindHoughTransf(HRichAnalysis *showMe, 
                    Int_t minampl, Int_t distance, Int_t howmanyrings) {


Int_t i,j,k,m,n,nrFired;
Float_t fDistance;
Float_t fRingX, fRingY, fRingR, fDivX, fDivY;
Int_t iRingDistance;

if (howmanyrings < 1) {
 howmanyrings = 1;
}

iAlgorithm2RingNr = 0;
for (i = 0; i < howmanyrings; i++) pAlgorithm2Ring[i].Reset();

iPadPlane.Reset();

for (m = 0; m < showMe->GetLabelNr(); m++) {

 nrFired = showMe->GetLabel(m)->iFiredPadsNr;
 HRichPadSignal *pPadsFired = NULL;
 if (nrFired) pPadsFired = new HRichPadSignal[nrFired];

 k = 0;
 for (j = showMe->GetLabel(m)->iLowerY; j <= showMe->GetLabel(m)->iUpperY; j++)
  for (i = showMe->GetLabel(m)->iLeftX; i <= showMe->GetLabel(m)->iRightX; i++)
   if (showMe->GetPad(i,j)->getAmplitude() > 0  && 
       showMe->GetPad(i,j)->getLabel() == showMe->GetLabel(m)->iSignature) {
    pPadsFired[k].setCol(i);
    pPadsFired[k].setRow(j);
    pPadsFired[k].setAmplitude(showMe->GetPad(i,j)->getAmplitude());
    k++;
    if (k >  nrFired) {
     exit(1);
    }
   }


 for (i = 0; i < nrFired; i++)
  for (j = i+1; j < nrFired; j++)
   if ((pPadsFired[i].getCol()-pPadsFired[j].getCol()) && 
       (pPadsFired[i].getRow()-pPadsFired[j].getRow())) {
    fDistance = sqrt((pPadsFired[i].getCol()-pPadsFired[j].getCol())*
                (pPadsFired[i].getCol()-pPadsFired[j].getCol()) +
                (pPadsFired[i].getRow()-pPadsFired[j].getRow())*
                (pPadsFired[i].getRow()-pPadsFired[j].getRow()));
    if (fDistance > pAnalysisParams->iRingRadius/2 && fDistance < pAnalysisParams->iRingMatrixSize)
     for (k = j+1; k < nrFired; k++)
      if ((pPadsFired[j].getCol()-pPadsFired[k].getCol()) && 
          (pPadsFired[j].getRow()-pPadsFired[k].getRow())) {
       fDistance = sqrt((pPadsFired[j].getCol()-pPadsFired[k].getCol())*
                   (pPadsFired[j].getCol()-pPadsFired[k].getCol()) +
                   (pPadsFired[j].getRow()-pPadsFired[k].getRow())*
                   (pPadsFired[j].getRow()-pPadsFired[k].getRow()));
       if (fDistance > pAnalysisParams->iRingRadius/2 && fDistance < pAnalysisParams->iRingMatrixSize) {
        
 // ................. create  map .........

         fDivX = (pPadsFired[j].getCol()-pPadsFired[k].getCol())*
                (pPadsFired[i].getRow()-pPadsFired[j].getRow())-
                (pPadsFired[i].getCol()-pPadsFired[j].getCol())*
                (pPadsFired[j].getRow()-pPadsFired[k].getRow());
         fDivY = (pPadsFired[j].getCol()-pPadsFired[k].getCol())*
                (pPadsFired[i].getRow()-pPadsFired[j].getRow())-
                (pPadsFired[i].getCol()-pPadsFired[j].getCol())*
                (pPadsFired[j].getRow()-pPadsFired[k].getRow());

         if (fDivX == 0.0 || fDivY == 0.0) { }
         else {

          fRingX = 0.5*((pPadsFired[j].getCol()*pPadsFired[j].getCol()-
                        pPadsFired[k].getCol()*pPadsFired[k].getCol()+
                        pPadsFired[j].getRow()*pPadsFired[j].getRow()-
                        pPadsFired[k].getRow()*pPadsFired[k].getRow())*
                       (pPadsFired[i].getRow()-pPadsFired[j].getRow()) - 
                       (pPadsFired[i].getCol()*pPadsFired[i].getCol()-
                        pPadsFired[j].getCol()*pPadsFired[j].getCol()+
                        pPadsFired[i].getRow()*pPadsFired[i].getRow()-
                        pPadsFired[j].getRow()*pPadsFired[j].getRow())*
                       (pPadsFired[j].getRow()-pPadsFired[k].getRow())) / fDivX;

          fRingY = 0.5*((pPadsFired[i].getCol()*pPadsFired[i].getCol()-
                        pPadsFired[j].getCol()*pPadsFired[j].getCol()+
                        pPadsFired[i].getRow()*pPadsFired[i].getRow()-
                        pPadsFired[j].getRow()*pPadsFired[j].getRow())*
                       (pPadsFired[j].getCol()-pPadsFired[k].getCol()) - 
                       (pPadsFired[j].getCol()*pPadsFired[j].getCol()-
                        pPadsFired[k].getCol()*pPadsFired[k].getCol()+
                        pPadsFired[j].getRow()*pPadsFired[j].getRow()-
                        pPadsFired[k].getRow()*pPadsFired[k].getRow())*
                       (pPadsFired[i].getCol()-pPadsFired[j].getCol())) / fDivY;

          fRingR = sqrt((pPadsFired[i].getCol()-fRingX)*
                       (pPadsFired[i].getCol()-fRingX)+
                       (pPadsFired[i].getRow()-fRingY)*
                       (pPadsFired[i].getRow()-fRingY));

          if (fRingR < (0.5 + pAnalysisParams->iRingRadius + pAnalysisParams->iRingRadiusError) &&
              fRingR > (0.5 + pAnalysisParams->iRingRadius - pAnalysisParams->iRingRadiusError) &&
              showMe->GetLabel(m)->iLeftX <= (Int_t)fRingX && 
              showMe->GetLabel(m)->iRightX >= (Int_t)fRingX &&
              showMe->GetLabel(m)->iLowerY <= (Int_t)fRingY && 
              showMe->GetLabel(m)->iUpperY >= (Int_t)fRingY)
           iPadPlane[(Int_t)fRingX + showMe->GetPadsXNr()*(Int_t)fRingY] += 1;
         }

// eof create map ...............

       }
      }
   }

 if (pPadsFired) delete [] pPadsFired;
}  // end of loop over all labels

// ..................................... find MAX ..............
// return findMax(showMe,minAmpl,areaMax);


for (n = 0; n < howmanyrings; n++) {
 for (m = 0; m < showMe->GetLabelNr(); m++) {
  for (j = showMe->GetLabel(m)->iLowerY; j <= showMe->GetLabel(m)->iUpperY; j++)
   for (i = showMe->GetLabel(m)->iLeftX; i <= showMe->GetLabel(m)->iRightX; i++) {

    iRingDistance = 1;

    for (k = 0; k < iAlgorithm2RingNr; k++)
     if (CalcDistance(i,j,pAlgorithm2Ring[k]) <= distance) iRingDistance = 0;

    if (iRingDistance) {
     if (iPadPlane[i + showMe->GetPadsXNr()*j] > pAlgorithm2Ring[n].iRingQuality) {
      pAlgorithm2Ring[n].iRingX = i;
      pAlgorithm2Ring[n].iRingY = j;
      pAlgorithm2Ring[n].iRingQuality = iPadPlane[i + showMe->GetPadsXNr()*j];
      pAlgorithm2Ring[n].iRingFreeParam = m;
     }
    }

   }  // end of loops over all pads of given label
 } // end of loops over all labels for the first ring

#ifdef HRICH_DEBUGMODE
 cout << "RICH DEBUG MODE: RingFindHoughTransf " << n+1 << ". ring candidate | x = "
      << pAlgorithm2Ring[n].iRingX << " | y = " << pAlgorithm2Ring[n].iRingY
      << " | quality = " << pAlgorithm2Ring[n].iRingQuality
      << " | label nr = " << pAlgorithm2Ring[n].iRingFreeParam << "\n";
#endif

 if (!TestRing(showMe,pAlgorithm2Ring[n],minampl)) pAlgorithm2Ring[n].Reset();
 else {
  iAlgorithm2RingNr++;
  pAlgorithm2Ring[n].iRingAlgorithmIndex = 2;
 }

#ifdef HRICH_DEBUGMODE
 if (pAlgorithm2Ring[n].iRingQuality)
  cout << n+1 << ". ring candidate stored for further processing\n";
 else cout << n+1 << ". ring candidate not accepted\n";
#endif

 if (pAlgorithm2Ring[n].iRingQuality == 0) return;
} // end of loop over all rings sought


return;
// ...... eof Find MAX .....................
} // eof RingFindHoughTransf
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichRingFind::TestRing(HRichAnalysis *showYou, 
                              const HRichHit& ring, Int_t amplit) {

 if (0.75*amplit > ring.iRingQuality) return 0;

 Int_t i, j, k, m, n;
 Int_t iLabelNr = 0, iActivePads = 0, iSurface = 0, 
       iActiveSurface = 0, iMatrixSurface = 0,
       iInRing = 0, iOnRing = 0, iOutRing = 0,
       iAllRing = 0, iDynamicAmplit = 0;

 iLabelNr = ring.iRingFreeParam;
 iActivePads = showYou->GetLabel(iLabelNr)->iFiredPadsNr;
 iSurface = (showYou->GetLabel(iLabelNr)->iUpperY - 
            showYou->GetLabel(iLabelNr)->iLowerY + 1)*
            (showYou->GetLabel(iLabelNr)->iRightX - 
            showYou->GetLabel(iLabelNr)->iLeftX + 1);
 iActiveSurface = showYou->GetLabel(iLabelNr)->iLabeledPadsNr;
 iMatrixSurface = pAnalysisParams->iRingMaskSize * pAnalysisParams->iRingMaskSize;

// -------------------------------------------------------------------------
// If density (ratio: fired pads / all active pads) in label larger 
// than 2*Sm - see below - exceeds 40%, ring recognition is not performed.

if (pAnalysisParams->isActiveTestDensity) {

#ifdef HRICH_DEBUGMODE
 cout << "Density test... (density = " << (Float_t)iActivePads/iActiveSurface 
      << " | area surface = " << (Float_t)iActiveSurface/(2*iMatrixSurface) << ") ";
#endif

 if ((Float_t)iActivePads/iActiveSurface > pAnalysisParams->fThresholdDensity && 
     (Float_t)iActiveSurface/(2*iMatrixSurface) > pAnalysisParams->fSurfaceArea) {
#ifdef HRICH_DEBUGMODE
  cout << "\n";
#endif
  return 0;
 }

#ifdef HRICH_DEBUGMODE
  cout << "...passed!\n";
#endif

} // eof isActiveTestDensity

// -------------------------------------------------------------------------
// The dynamic minimal (threshold) amplitude (quality) is estimated here 
// if function TestRing is called with amplit > 0.
// It is calculated due to empirical formula:
//                                      
//      threshold Amplitude = amplit * e^( P1*(S/Sm - 1) + P2*(D/P3 - 1) )
//  where
//      P1 - parameter (0.055)
//      P2 - parameter (0.5)
//      Sm = ring pattern matrix surface (in number of pads units)
//      S  = number of pads in given labeled area
//      P3 = mean density (ratio: pads fired / all pads) for ring matrix area ~0.15 
//      D  = density of fired pads for given labeled area

if (pAnalysisParams->isActiveDynamicThrAmplitude) {

 if (amplit) {

  Float_t fSurfRatio = (Float_t)iActiveSurface/iMatrixSurface;
  Float_t fDensRatio = (Float_t)iActivePads/iActiveSurface;

  if (fSurfRatio <= 1.34 && fDensRatio <= 1.34*pAnalysisParams->fFormulaParam3) {
   iDynamicAmplit = (Int_t)(amplit * pAnalysisParams->fLowerAmplFactor);
  } else {
   iDynamicAmplit = (Int_t)(amplit * exp(pAnalysisParams->fFormulaParam1 * (fSurfRatio - 1.) + 
                    pAnalysisParams->fFormulaParam2 * (fDensRatio/pAnalysisParams->fFormulaParam3 - 1.)));
   if (iDynamicAmplit < amplit) iDynamicAmplit = amplit;
  }

#ifdef HRICH_DEBUGMODE
  cout << "Dynamic amplitude (quality) test... (amplitude = " << iDynamicAmplit << ") ";
#endif

  if (iDynamicAmplit > ring.iRingQuality) {

#ifdef HRICH_DEBUGMODE
   cout << "\n";
#endif

   return 0;
  }

#ifdef HRICH_DEBUGMODE
  cout << "...passed!\n";
#endif

 }

} // eof isActiveDynamicThrAmplitude

// -------------------------------------------------------------------------
// This part calculates how many fired pads belong to ring in comparison 
// with the number of fired pads outside and inside. The area of ringMask 
// is scanned. In it the fired pads outside/inside of ring must
// not to exceed i.e.  33% of total number of fired pads in this area - 
// at least 67% has to be inside of ring border.

if (pAnalysisParams->isActiveFiredRingPadsRatio) {

 for (k = 0; k < iMatrixSurface; k++) {
  m = (k % pAnalysisParams->iRingMaskSize) - pAnalysisParams->iRingMaskSize/2;
  n = (k / pAnalysisParams->iRingMaskSize) - pAnalysisParams->iRingMaskSize/2;
  if (!showYou->IsOut(ring.iRingX,ring.iRingY,m,n) &&
      showYou->GetPad(ring.iRingX+m,ring.iRingY+n)->getAmplitude() > 0)
   if (pAnalysisParams->iRingMask[k] == 0) iOutRing++;
   else
    if (pAnalysisParams->iRingMask[k] == 1) iOnRing++;
    else
     if (pAnalysisParams->iRingMask[k] == 2) iInRing++;
 }

 iAllRing = iOutRing + iOnRing + iInRing;
#ifdef HRICH_DEBUGMODE
  cout << "Number of pads in ring and outside test... (ring = " 
       << 100.*iOnRing/iAllRing << "% | outside = "
       << 100.*(iOutRing+iInRing)/iAllRing << "%) ";
#endif
 
 if (iOutRing+iInRing >= pAnalysisParams->fFiredRingPadsRatio*iAllRing) {
#ifdef HRICH_DEBUGMODE
  cout << "\n";
#endif
  return 0;
 }

#ifdef HRICH_DEBUGMODE
  cout << "...passed!\n";
#endif

} // eof isActiveFiredRingPadsRatio

// -------------------------------------------------------------------------
// Asymmetry of ring is investigated. First mean value of fired pads for x 
// and y is calculated. It can not be larger than iRingRadiusError given in input data. 
// Then a ring radius is calculated and it also has to fit
// in the value iRingRadius +/- iRingRadiusError.

if (pAnalysisParams->isActiveTestAsymmetry) {

Int_t iHalfRingMatrix = pAnalysisParams->iRingMatrixSize/2;
Int_t  iXpos, iYpos, iPosX = 0, iPosY = 0, iHowManyPads = 0, iErrFlag = 0;
Float_t iRadCalc = 0.;

for (j = 0; j < pAnalysisParams->iRingMatrixSize; j++)
 for (i = 0; i < pAnalysisParams->iRingMatrixSize; i++)
  if (!showYou->IsOut(ring.iRingX,ring.iRingY,i-iHalfRingMatrix,j-iHalfRingMatrix))
   if (showYou->GetPad(ring.iRingX+i-iHalfRingMatrix,
                       ring.iRingY+j-iHalfRingMatrix)->getAmplitude() > 0) {
    iPosX += i - iHalfRingMatrix;
    iPosY += j - iHalfRingMatrix;
    iHowManyPads++;
   }
iXpos = iPosX/iHowManyPads;
iYpos = iPosY/iHowManyPads;

iHowManyPads = (Int_t)(sqrt((Float_t)iXpos*iXpos+(Float_t)iYpos*iYpos));

#ifdef HRICH_DEBUGMODE
 cout << "Assymetry test... (middle shift = " << iHowManyPads;
#endif

if (iHowManyPads > pAnalysisParams->iRingRadiusError) {
#ifdef HRICH_DEBUGMODE
 cout << ")\n";
#endif
 return 0;
} else {
 if (iHowManyPads > 0) iErrFlag++;
#ifdef HRICH_DEBUGMODE
 cout << " |";
#endif
}

iHowManyPads = 0;

for (j = 0; j < pAnalysisParams->iRingMatrixSize; j++)
 for (i = 0; i < pAnalysisParams->iRingMatrixSize; i++)
  if (!showYou->IsOut(ring.iRingX,ring.iRingY,i-iHalfRingMatrix,j-iHalfRingMatrix))
   if (showYou->GetPad(ring.iRingX+i-iHalfRingMatrix,
                       ring.iRingY+j-iHalfRingMatrix)->getAmplitude() > 0 &&
       (Int_t)(0.5+CalcDistance(iXpos,iYpos,i-iHalfRingMatrix,j-iHalfRingMatrix)) <= 
       (pAnalysisParams->iRingRadius+pAnalysisParams->iRingRadiusError)) {
    iRadCalc += sqrt((iXpos+i-iHalfRingMatrix)*(iXpos+i-iHalfRingMatrix) + 
                     (iYpos+j-iHalfRingMatrix)*(iYpos+j-iHalfRingMatrix));
    iHowManyPads++;
   }

#ifdef HRICH_DEBUGMODE
 cout << " radius = " << (Int_t)(iRadCalc/iHowManyPads) << ") ";
#endif

if (abs((Int_t)(iRadCalc/iHowManyPads) - pAnalysisParams->iRingRadius) > 
    pAnalysisParams->iRingRadiusError) {

#ifdef HRICH_DEBUGMODE
 cout << "\n";
#endif
 return 0;
}

if (abs((Int_t)(iRadCalc/iHowManyPads) - pAnalysisParams->iRingRadius) > 0) iErrFlag++;
if (iErrFlag<2) {
#ifdef HRICH_DEBUGMODE
 cout << "...passed!\n";
#endif
} else {
#ifdef HRICH_DEBUGMODE
 cout << "...ring distortion too big!\n";
#endif
 return 0;
}

} // eof isActiveTestAsymmetry

// -------------------------------------------------------------------------
//  Finally if all tests are passed function returns 1 (ring qualified for storing).

return 1;
}
//============================================================================

//----------------------------------------------------------------------------
void HRichRingFind::CalcRingParameters(HRichAnalysis *showMe) {

 Int_t i,j,k,l,m,n,
       iCount, iIsPhot4, iIsPhot8, iPhot4Nr, iPhot8Nr, iPad;
 Int_t iNowX, iNowY, iShift;

 iPhot4Nr = iPhot8Nr = iPad = 0;
 iShift = pAnalysisParams->iRingMaskSize/2;

if (iRingNr == 0 && 
     pGeometryParams->getPadsPar()->getActivePadsNr() > 
     iShift*iShift*4) {

 do {
  iNowX = (Int_t)HomogenDistr(pAnalysisParams->iRingRadius, showMe->GetPadsXNr() - pAnalysisParams->iRingRadius);
  iNowY = (Int_t)HomogenDistr(pAnalysisParams->iRingRadius, showMe->GetPadsYNr() - pAnalysisParams->iRingRadius);
 } while (showMe->IsOut(iNowX,iNowY,0,0));

 for (j = 0; j < pAnalysisParams->iRingMaskSize; j++)
  for (i = 0; i < pAnalysisParams->iRingMaskSize; i++)
   if (!showMe->IsOut(iNowX,iNowY,i-iShift,j-iShift)) {
    iIsPhot4 = iIsPhot8 = 0;
    m = iNowX+i-iShift + showMe->GetPadsXNr()*(iNowY+j-iShift);
    if (showMe->GetPad(m)->getAmplitude() > 0 && 
        pAnalysisParams->iRingMask[i + (pAnalysisParams->iRingMaskSize)*j] == 1) {
     iPad++;
     for (k = -1; k < 2; k++)
      for (l = -1; l < 2; l++)
       if (((l == 0 && abs(k)) || (k == 0 && abs(l))) && !(l == 0 && k == 0) &&
           !showMe->IsOut(m,l,k) && showMe->GetPad(m+l,k)->getAmplitude() >= showMe->GetPad(m)->getAmplitude())
        iIsPhot4++;
     if (iIsPhot4 == 0) iPhot4Nr++;
     for (k = -1; k < 2; k++)
      for (l = -1; l < 2; l++)
       if (abs(l) && abs(k) && !showMe->IsOut(m,l,k) && 
           showMe->GetPad(m+l,k)->getAmplitude() >= showMe->GetPad(m)->getAmplitude())
        iIsPhot8++;
     if (iIsPhot4 == 0 && iIsPhot8 == 0) iPhot8Nr++;
    }
   }

 showMe->iFakePad = iPad;
 showMe->iFakeLocalMax4 = iPhot4Nr;
 showMe->iFakeLocalMax8 = iPhot8Nr;

#ifdef HRICH_DEBUGMODE
 cout << "RICH DEBUG MODE: 0 rings found:\n";
 cout << "Background parameters | fake local maxima 4 = " << showMe->iFakeLocalMax4 
      << " | fake local maxima 8 = " << showMe->iFakeLocalMax8 << "\n";
#endif

} else {


 for (n = 0; n < iRingNr; n++) {
  iRingTempImage.Reset();
  for (j = 0; j < pAnalysisParams->iRingMaskSize; j++) 
   for (i = 0; i < pAnalysisParams->iRingMaskSize; i++) {
    if (!showMe->IsOut(pRings[n].iRingX,pRings[n].iRingY,i-iShift,j-iShift)) {
     pRings[n].iRingImage[i + pAnalysisParams->iRingMaskSize*j] =
      showMe->GetPad(pRings[n].iRingX+i-iShift, pRings[n].iRingY+j-iShift)->getAmplitude();
    } else pRings[n].iRingImage[i + pAnalysisParams->iRingMaskSize*j] = 0;
   }

  iPhot4Nr = iPhot8Nr = 0;
  iNowX = pRings[n].iRingX;
  iNowY = pRings[n].iRingY;
  for (j = 0; j < pAnalysisParams->iRingMaskSize; j++)
   for (i = 0; i < pAnalysisParams->iRingMaskSize; i++)
    if (!showMe->IsOut(iNowX,iNowY,i-iShift,j-iShift)) {
     iIsPhot4 = iIsPhot8 = 0;
     m = iNowX+i-iShift + showMe->GetPadsXNr()*(iNowY+j-iShift);
     if (showMe->GetPad(m)->getAmplitude() > 0 && 
         pAnalysisParams->iRingMask[i + (pAnalysisParams->iRingMaskSize)*j] == 1) {
      pRings[n].iRingPadNr++;
      pRings[n].iRingAmplitude += showMe->GetPad(m)->getAmplitude();
      // cout << "-------- pad amplitude --- " << m << " |  " << showMe->GetPad(m)->getAmplitude() << endl;
      for (k = -1; k < 2; k++)
       for (l = -1; l < 2; l++)
        if (((l == 0 && abs(k)) || (k == 0 && abs(l))) && !(l == 0 && k == 0) &&
            !showMe->IsOut(m,l,k) && showMe->GetPad(m+l,k)->getAmplitude() >= showMe->GetPad(m)->getAmplitude()) {
         iIsPhot4++;
        }
      if (iIsPhot4 == 0) {
       iPhot4Nr++;
       iRingTempImage[i + (pAnalysisParams->iRingMaskSize)*j] += 1;
      }
      for (k = -1; k < 2; k++)
       for (l = -1; l < 2; l++)
        if (abs(l) && abs(k) && !showMe->IsOut(m,l,k) && 
            showMe->GetPad(m+l,k)->getAmplitude() >= showMe->GetPad(m)->getAmplitude()) {
         iIsPhot8++;
        }
      if (iIsPhot4 == 0 && iIsPhot8 == 0) {
       iPhot8Nr++;
       iRingTempImage[i + (pAnalysisParams->iRingMaskSize)*j] += 1;
      }
     }
    }

 pRings[n].iRingLocalMax4 = iPhot4Nr;
 pRings[n].iRingLocalMax8 = iPhot8Nr;


 iCount = 0;
 iClusterSize.Reset();
 iClusterLMax4.Reset();
 iClusterLMax8.Reset();
 for (j = 0; j < iRingImageSize; j++)
  for (i = 0; i < iRingImageSize; i++)
   if (pRings[n].iRingImage[i + iRingImageSize*j] > 0) {
    iInnerCount = iInnerPhot4 = iInnerPhot8 = 0;
    CalcRingClusters(showMe, &iRingTempImage[0], n, i, j);
    if (iInnerCount) {
     iClusterSize[iCount] = iInnerCount;
     iClusterLMax4[iCount] = iInnerPhot4;
     iClusterLMax8[iCount] = iInnerPhot8;
     iCount++;
    }
   }
 pRings[n].iRingClusterNr = iCount;
 pRings[n].iRingClusterSize.Set(iCount);
 pRings[n].iRingClusterLMax4.Set(iCount);
 pRings[n].iRingClusterLMax8.Set(iCount);
 for (i = 0; i < iCount; i++) {
  pRings[n].iRingClusterSize[i] = iClusterSize[i];
  pRings[n].iRingClusterLMax4[i] = iClusterLMax4[i];
  pRings[n].iRingClusterLMax8[i] = iClusterLMax8[i]; 
 } 

#ifdef HRICH_DEBUGMODE
 cout << "RICH DEBUG MODE: " << iRingNr << " ring(s) found:\n";
 cout << "Ring nr " << n+1 << " fired = " << pRings[n].iRingPadNr 
      << " | x = " << pRings[n].iRingX
      << " | y = " << pRings[n].iRingY << " | quality = " << pRings[n].iRingQuality
      << " | amplitude = " << pRings[n].iRingAmplitude 
      << " | local max 4 = " << pRings[n].iRingLocalMax4 
      << " | local max 8 = " << pRings[n].iRingLocalMax8 << "\n";
 cout << " free parameter = " << pRings[n].iRingFreeParam << "\n";
 cout << "Clusters data (number of pads, local max 4, local max 8):\n";
 
 for (i = 0; i < pRings[n].iRingClusterNr; i++) {
  cout << "(" << pRings[n].iRingClusterSize[i]
       << ", " << pRings[n].iRingClusterLMax4[i]
       << ", " << pRings[n].iRingClusterLMax8[i] << ")\n"; 
 }
#endif

 } // end of loop over all rings


} // end of condition with iRingNr

} // eof CalcRingParameters
//============================================================================

//----------------------------------------------------------------------------
void HRichRingFind::CalcRingClusters(HRichAnalysis *showYou, Int_t *dumpArr, 
                                    Int_t ringNr, Int_t nowX, Int_t nowY) {

Int_t a,b, iTempMatrix[3][3] = {{0,0,0},{0,0,0},{0,0,0}};

 if (dumpArr[nowX + iRingImageSize*nowY] != 3) {
  if (dumpArr[nowX + iRingImageSize*nowY] == 1) iInnerPhot4++;
  else if (dumpArr[nowX + iRingImageSize*nowY] == 2) { 
   iInnerPhot4++;
   iInnerPhot8++; 
  }
  if (pAnalysisParams->iRingMask[nowX + iRingImageSize*nowY] == 1) iInnerCount++;
  dumpArr[nowX + iRingImageSize*nowY] = 3;
 }

 for (b = 0; b < 3; b++)
  for (a = 0; a < 3; a++)
   if (nowX+a-1 >= 0 && nowX+a-1 < iRingImageSize && 
       nowY+b-1 >= 0 && nowY+b-1 < iRingImageSize)
    if (!(a == 1 && b == 1))
     if (pRings[ringNr].iRingImage[nowX+a-1 + iRingImageSize*(nowY+b-1)] > 0 &&
         dumpArr[nowX+a-1 + iRingImageSize*(nowY+b-1)] != 3) {
          if (dumpArr[nowX+a-1 + iRingImageSize*(nowY+b-1)] == 1) iInnerPhot4++;
          else if (dumpArr[nowX+a-1 + iRingImageSize*(nowY+b-1)] == 2) { 
           iInnerPhot4++;
           iInnerPhot8++; 
          }
          if (pAnalysisParams->iRingMask[nowX+a-1 + (pAnalysisParams->iRingMaskSize)*(nowY+b-1)] == 1) iInnerCount++;
          dumpArr[nowX+a-1 + iRingImageSize*(nowY+b-1)] = 3;
          iTempMatrix[a][b] = 1;

#ifdef HRICH_DEBUGMODE0
 cout << "RICH DEBUG MODE: cluster in ring (pads fired = " << iInnerCount 
      << ", local max 4 = " << iInnerPhot4
      << ", local max 8 = " << iInnerPhot8 << ")\n";
#endif

     }


 for (b = 0; b < 3; b++)
  for (a = 0; a < 3; a++)
   if (iTempMatrix[a][b] > 0) CalcRingClusters(showYou,dumpArr,ringNr,nowX+a-1,nowY+b-1);

} // eof CalcRingClusters
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichRingFind::GetAlgorithmNr(HRichAnalysis *showMe) {
 return (pAnalysisParams->isActiveRingFindFitMatrix + pAnalysisParams->isActiveRingHoughTransf);
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichRingFind::Execute(HRichAnalysis *giveMe) {

Int_t i,j, iChosen;
if (giveMe->pRings) {
 giveMe->iRingNr = 0;
 delete [] giveMe->pRings;
 giveMe->pRings = NULL;
}

if (giveMe->GetLabelNr() == 0 || GetAlgorithmNr(giveMe) == 0) {
 iRingNr = 0;
 CalcRingParameters(giveMe);
return (giveMe->iRingNr = iRingNr);
}

iRingNr = 0;

if (pAnalysisParams->isActiveRingFindFitMatrix)
 RingFindFitMatrix(giveMe,pAnalysisParams->iMinimalFitMatrixRingQuality,
                   pAnalysisParams->iMinimalFitMatrixRingDistance,pAnalysisParams->iHowManyFitMatrixRings);

if (pAnalysisParams->isActiveRingHoughTransf)
 RingFindHoughTransf(giveMe,pAnalysisParams->iMinimalHoughTransfRingQuality,
                     pAnalysisParams->iMinimalHoughTransfRingDistance,pAnalysisParams->iHowManyHoughTransfRings);

// --- all rings found by all algorithms will be stored ---
if (GetAlgorithmNr(giveMe) == 2 && pAnalysisParams->iSuperiorAlgorithmID == 3) {
 pRings = new HRichHit[iAlgorithm1RingNr + iAlgorithm2RingNr];
 for (i = 0; i < iAlgorithm1RingNr; i++) {
  pRings[iRingNr].iRingX = pAlgorithm1Ring[i].iRingX;
  pRings[iRingNr].iRingY = pAlgorithm1Ring[i].iRingY;
  pRings[iRingNr].iRingQuality = pAlgorithm1Ring[i].iRingQuality;
  pRings[iRingNr].iRingAlgorithmIndex = pAlgorithm1Ring[i].iRingAlgorithmIndex;
  pRings[iRingNr].iRingFreeParam = pAlgorithm1Ring[i].iRingFreeParam;
  iRingNr++;
 }
 for (i = 0; i < iAlgorithm2RingNr; i++) {
  pRings[iRingNr].iRingX = pAlgorithm2Ring[i].iRingX;
  pRings[iRingNr].iRingY = pAlgorithm2Ring[i].iRingY;
  pRings[iRingNr].iRingQuality = pAlgorithm2Ring[i].iRingQuality;
  pRings[iRingNr].iRingAlgorithmIndex = pAlgorithm2Ring[i].iRingAlgorithmIndex;
  pRings[iRingNr].iRingFreeParam = pAlgorithm2Ring[i].iRingFreeParam;
  iRingNr++;
 }
 CalcRingParameters(giveMe);
 giveMe->pRings = pRings;
 pRings = NULL;
return (giveMe->iRingNr = iRingNr);
}

// --- only rings found by both algorithms ---
if (GetAlgorithmNr(giveMe) == 2) {
 pRings = new HRichHit[((iAlgorithm1RingNr > iAlgorithm2RingNr) ? iAlgorithm1RingNr : iAlgorithm2RingNr)];

#ifdef HRICH_DEBUGMODE
 cout << "Rings found by both algorithms matching... (maximum " 
      << ((iAlgorithm1RingNr > iAlgorithm2RingNr) ? iAlgorithm1RingNr : iAlgorithm2RingNr) << " candidates).\n";
#endif

 for (i = 0; i < iAlgorithm1RingNr; i++)
  if (pAlgorithm1Ring[i].iRingQuality > 0) {
   for (j = 0; j < iAlgorithm2RingNr; j++) {
    iChosen = 0;
    if (pAlgorithm2Ring[j].iRingQuality > 0) {
     if (CalcDistance(pAlgorithm1Ring[i],pAlgorithm2Ring[j]) <= pAnalysisParams->iRingRadius/2) {
      if (pAnalysisParams->iSuperiorAlgorithmID == 1) {
       pRings[iRingNr].iRingX = pAlgorithm1Ring[i].iRingX;
       pRings[iRingNr].iRingY = pAlgorithm1Ring[i].iRingY;
       pRings[iRingNr].iRingQuality = pAlgorithm1Ring[i].iRingQuality;
       pRings[iRingNr].iRingAlgorithmIndex = 3;
       pRings[iRingNr].iRingFreeParam = pAlgorithm1Ring[i].iRingFreeParam;
       iRingNr++;
       iChosen = 1;
      } else if (pAnalysisParams->iSuperiorAlgorithmID == 2) {
       pRings[iRingNr].iRingX = pAlgorithm2Ring[j].iRingX;
       pRings[iRingNr].iRingY = pAlgorithm2Ring[j].iRingY;
       pRings[iRingNr].iRingQuality = pAlgorithm2Ring[j].iRingQuality;
       pRings[iRingNr].iRingAlgorithmIndex = 3;
       pRings[iRingNr].iRingFreeParam = pAlgorithm2Ring[j].iRingFreeParam;
       iRingNr++;
       iChosen = 1;
      }
     }
    }
    if (iChosen) break;
   }
  }

#ifdef HRICH_DEBUGMODE
 cout << "Number of rings matched: " << iRingNr << endl;
#endif

 CalcRingParameters(giveMe);
 giveMe->pRings = pRings;
 pRings = NULL;
return (giveMe->iRingNr = iRingNr);
}

// --- rings found by selected algorithm ---
if (GetAlgorithmNr(giveMe) == 1) {
 if (pAnalysisParams->isActiveRingFindFitMatrix) {
  iRingNr = iAlgorithm1RingNr;
  pRings = pAlgorithm1Ring;
  pAlgorithm1Ring = new HRichHit[pAnalysisParams->iHowManyFitMatrixRings];
  CalcRingParameters(giveMe);
  giveMe->pRings = pRings;
  pRings = NULL;
return (giveMe->iRingNr = iRingNr);
 }
 if (pAnalysisParams->isActiveRingHoughTransf) {
  iRingNr = iAlgorithm2RingNr;
  pRings = pAlgorithm2Ring;
  pAlgorithm2Ring = new HRichHit[pAnalysisParams->iMinimalHoughTransfRingQuality];
  CalcRingParameters(giveMe);
  giveMe->pRings = pRings;
  pRings = NULL;
return (giveMe->iRingNr = iRingNr);
 }
}


return (giveMe->iRingNr = 0); // this should never happen
} // eof
//============================================================================

