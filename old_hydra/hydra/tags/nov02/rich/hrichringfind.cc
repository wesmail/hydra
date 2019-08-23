// File: hrichringfind.cc
// ***************************************************************************

//*-- Author : Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
//*-- Modified : 2000/10/xx by Wolfgang Koenig 
//*-- Modified : 2001/01/xx by Laura Fabbietti
//*-- Modified : 2001/09/06 by Witold Przygoda


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
 fClusterSize = 0.;
 fClusterLMax4 = 0.;
 fClusterLMax8 = 0.;
 iRingImageSize = 16;
 iRingTempImage.Set(256);
 iPadPlane.Set(16384); 
 iPadPlaneCopy.Set(16384); 
 iPadCol.Set(1000);
 iPadRow.Set(1000);
 iRingNr = 0;
 maxCols = 0;
 maxRows = 0;
 iMatrixSize = 0;
 iMatrixHalfSize = 0;
 ly_from = ly_to = lx_from = lx_to = 0;
 fMaxClusterSize = 0;
 fMaxClusterSum = 0;
 fMaxThrClusterSize = 0;
 pRings = NULL;

 pAnalysisParams = NULL;
 pGeometryParams = NULL;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichRingFind::init(HRichAnalysis* showMe) {

 pAnalysisParams = showMe->getParams();
 pGeometryParams = showMe->getGeometryPar();

 iInnerCount = 0;
 iInnerPhot4 = 0;
 iInnerPhot8 = 0;
 fClusterSize = 0.;
 fClusterLMax4 = 0.;
 fClusterLMax8 = 0.;
 iRingImageSize = pAnalysisParams->iRingMaskSize;
 iRingTempImage.Set(iRingImageSize * iRingImageSize);
 iPadPlane.Set(pGeometryParams->getPadsNr());
 iPadPlaneCopy.Set(pGeometryParams->getPadsNr());
 iPadCol.Set(pAnalysisParams->maxFiredSectorPads);
 iPadRow.Set(pAnalysisParams->maxFiredSectorPads);
 iPadActive.Set(pGeometryParams->getPadsNr());
 for (int i=0 ; i<pGeometryParams->getPadsNr(); i++)
  if (pGeometryParams->getPadsPar()->getPad(i)->getPadActive()>0) 
   iPadActive[i] = 1; else iPadActive[i] = 0;

 iRingNr = 0;
 maxCols = showMe->GetPadsXNr();
 maxRows = showMe->GetPadsYNr();

 iMatrixSize = pAnalysisParams->iRingMatrixSize;
 iMatrixHalfSize = iMatrixSize/2;
 fMaxClusterSize = 0;
 fMaxClusterSum = 0;
 fMaxThrClusterSize = 0;


 fHitList1.Delete();
 fHitList2.Delete();
 fHitCandidate.Delete();

 pRings = new HRichHit[10];
 showMe->pRings = pRings;

 return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
HRichRingFind::~HRichRingFind() {
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
 Int_t dx,dy;
 dx = x-ring.iRingX;
 dy = y-ring.iRingY;
 return sqrt(dx*dx + dy*dy);
}
//============================================================================

//----------------------------------------------------------------------------
Float_t HRichRingFind::CalcDistance(Int_t x1, Int_t y1, Int_t x2, Int_t y2) {
 Int_t dx,dy;
 dx = x1-x2;
 dy = y1-y2;
 return sqrt(dx*dx + dy*dy);
}
//============================================================================

//----------------------------------------------------------------------------
Double_t HRichRingFind::HomogenDistr(Double_t left, Double_t right) {
 return rand() / (double)RAND_MAX * (right - left) + left;
}
//============================================================================


//----------------------------------------------------------------------------
Int_t HRichRingFind::MaxLabAmpl(TList *hitList, Int_t maxCode) {
//
// This is an auxiliary function called by MaxSelector only.
// It returns the amplitude of a local maximum labeled with "maxCode".
//
Int_t m = 0;
HRichHit *pHit;

 do {
  pHit = (HRichHit*)(hitList->At(m));
  m++;
 } while (pHit->iRingMaxLabel != maxCode);

return pHit->iRingQuality;
}
//============================================================================

//----------------------------------------------------------------------------
void HRichRingFind::MaxSelector(HRichAnalysis *showMe, TList *hitList,
                                TArrayI *in, TArrayI *out) {
//
// This function is used to label clusters of local maxima.
// It propagates the label assigned to a hit from "hitList"
// to all connected pads if they are reachable from a maximum (hit)
// in a descending monotonious way. If a pad belongs to more than
// one hit it is marked with '-2'. At the end the 8-connected fired pads
// of a local maximum are marked with the label of this maximum (even
// if they are '-2'. In case of a conflict (possible only if two
// maxima are separated by one pad) the label of maximum with 
// higher amplitude is assigned to the pads.
// Propagation of a label is done in a similar way as in the case
// of cleaning algorithm (high amplitude clusters).
//

Int_t i,j,k,l,m,pad,padnear;
Int_t fMaxCode;
HRichHit *pHit = NULL;

 for (m = 0; m < hitList->GetSize(); m++) {
  pHit = (HRichHit*)(hitList->At(m));
  pad = pHit->iRingX + maxCols*pHit->iRingY;
  fMaxCode = pHit->iRingMaxLabel;
  MaxMarker(showMe,in,out,pad,fMaxCode);

 } // eof loop over all local maxima


 // this part (below) is to mark also direct neighboring 
 // pads of a local maximum even if they are common pads

 for (m = 0; m < hitList->GetSize(); m++) {
  pHit = (HRichHit*)(hitList->At(m));
  i = pHit->iRingX;
  j = pHit->iRingY;
  pad = i + maxCols*j;
  fMaxCode = pHit->iRingMaxLabel;

  ly_from = ((j-1 < 0) ? 0 : j-1);
  ly_to   = ((j+1 >= maxRows) ? maxRows-1 : j+1);
  lx_from = ((i-1 < 0) ? 0 : i-1);
  lx_to   = ((i+1 >= maxCols) ? maxCols-1 : i+1);

  for (k = ly_from; k <= ly_to; k++)
   for (l = lx_from; l <= lx_to; l++) {
    padnear = l + maxCols*k;
    if (iPadActive[padnear] && !(l==i && k==j))
     if ((*out)[padnear] == -2) {
      (*out)[padnear] = fMaxCode;
     } else
        if ((*out)[padnear] != 0 &&
            (*out)[padnear] != fMaxCode &&
            MaxLabAmpl(hitList,(*out)[padnear]) < pHit->iRingQuality) {
         (*out)[padnear] = fMaxCode;
        }    
   }

 } // eof second loop over all local maxima

}
//============================================================================


//----------------------------------------------------------------------------
void HRichRingFind::MaxMarker(HRichAnalysis *showYou,
                                  TArrayI *in, TArrayI *out,
                                  Int_t nowPad, Int_t maxCode) {
//
// This function is called by MaxSelector recursively to propagate
// given label "maxCode" to all pads in "out" array which are marked with '-1'
// If the pad marked earlier by a label from another maximum is reached
// it is marked as a common pad with '-2'.
//

Int_t i,j,k,l, padnear, x_from, x_to, y_from, y_to;
TArrayI iTempMatrix(9);

 i = nowPad % maxCols;
 j = nowPad / maxCols;

 (*out)[nowPad] = maxCode;

 y_from = ((j-1 < 0) ? 0 : j-1);
 y_to   = ((j+1 >= maxRows) ? maxRows-1 : j+1);
 x_from = ((i-1 < 0) ? 0 : i-1);
 x_to   = ((i+1 >= maxCols) ? maxCols-1 : i+1);

 for (k = y_from; k <= y_to; k++)
  for (l = x_from; l <= x_to; l++) {
   padnear = l + maxCols*k;
   if (iPadActive[padnear] && !(l==i && k==j)) 
    if ((*in)[padnear] <= (*in)[nowPad]) {
     if ((*out)[padnear] == -1) {
      (*out)[padnear] = maxCode;
      iTempMatrix[l-i+1 + 3*(k-j+1)] = maxCode;
     } else
       if ((*out)[padnear] != 0 &&
           (*out)[padnear] != maxCode) {
      (*out)[padnear] = -2;
      iTempMatrix[l-i+1 + 3*(k-j+1)] = -2;
     }
    }
  }

 for (k = 0; k < 3; k++)
  for (l = 0; l < 3; l++)
   if (iTempMatrix[l + 3*k] != 0) 
    MaxMarker(&(*showYou), &(*in), &(*out), 
              nowPad+l-1 + maxCols*(k-1), iTempMatrix[l + 3*k]);

}
//============================================================================


//----------------------------------------------------------------------------
void HRichRingFind::MaxAnalysis(HRichAnalysis *showMe, TList *hitList, 
                                TArrayI *in, TArrayI *out, Int_t minAmpl) {
//
// The function analyses local maximum cluster, calculating the mean 
// position of a maximum, weighted by the amplitude of pads in cluster, 
// also cluster size and the amplitude sum of the pads belonging to a cluster. 
// The data are stored in HRichHit structure.
//

Int_t m,pad;
Int_t fMaxCode;
HRichHit *pHit = NULL;

 for (m = 0; m < hitList->GetSize(); m++) {
  pHit = (HRichHit*)(hitList->At(m));
  pad = pHit->iRingX + maxCols*pHit->iRingY;
  fMaxCode = pHit->iRingMaxLabel;
  xMeanMax = 0.;
  yMeanMax = 0.;
  xPadMeanMax = 0.;
  yPadMeanMax = 0.;
  thetaMeanMax = 0.;
  phiMeanMax = 0.;
  fMaxClusterSize = 0;
  fMaxClusterSum = 0;
  fMaxThrClusterSize = 0;

  MaxCluster(showMe,in,out,pad,fMaxCode, minAmpl);

  xMeanMax /= fMaxClusterSum;
  yMeanMax /= fMaxClusterSum;
  xPadMeanMax /= fMaxClusterSum;
  yPadMeanMax /= fMaxClusterSum;
  thetaMeanMax /= fMaxClusterSum;
  phiMeanMax /= fMaxClusterSum;
  pHit->fX = xMeanMax;
  pHit->fY = yMeanMax;
  pHit->fPadX = xPadMeanMax;
  pHit->fPadY = yPadMeanMax;
  pHit->fMeanTheta = thetaMeanMax;
  pHit->fMeanPhi = phiMeanMax;
  pHit->fMaxClusterSize = fMaxClusterSize;
  pHit->fMaxClusterSum = fMaxClusterSum;
  pHit->fMaxThrClusterSize = fMaxThrClusterSize;

 } // eof loop over all local maxima

}          
//============================================================================

//----------------------------------------------------------------------------
void HRichRingFind::MaxCluster(HRichAnalysis *showYou, 
                                  TArrayI *in, TArrayI *out, 
                                  Int_t nowPad, Int_t maxCode, Int_t minAmpl) {
//
// Function called recursively from MaxAnalysis to analyse features of local
// maxima clusters; similar to MaxMarker.
//

Int_t i,j,k,l, padnear, x_from, x_to, y_from, y_to;
TArrayI iTempMatrix(9);
HRichPad *pPad = showYou->getGeometryPar()->getPadsPar()->getPad(nowPad);

 xMeanMax += ((*in)[nowPad])*(pPad->getX());
 yMeanMax += ((*in)[nowPad])*(pPad->getY());
 // variables below are in pad and pad fraction units
 xPadMeanMax += ((*in)[nowPad])*((Float_t)(nowPad % maxCols));
 yPadMeanMax += ((*in)[nowPad])*((Float_t)(nowPad / maxCols));
 // variables below are angles
 thetaMeanMax += ((*in)[nowPad])*(pPad->getTheta());
 phiMeanMax += ((*in)[nowPad])*(pPad->getPhi(showYou->GetActiveSector()));
 fMaxClusterSize++;
 fMaxClusterSum += (*in)[nowPad]; 
 if ((*in)[nowPad] > minAmpl) fMaxThrClusterSize++;
 (*out)[nowPad] = 0;

 i = nowPad % maxCols;
 j = nowPad / maxCols;

 y_from = ((j-1 < 0) ? 0 : j-1);
 y_to   = ((j+1 >= maxRows) ? maxRows-1 : j+1);
 x_from = ((i-1 < 0) ? 0 : i-1);
 x_to   = ((i+1 >= maxCols) ? maxCols-1 : i+1);

 for (k = y_from; k <= y_to; k++)
  for (l = x_from; l <= x_to; l++) {
   padnear = l + maxCols*k;
   if (iPadActive[padnear] && !(l==i && k==j))
    if ((*out)[padnear] == maxCode) {
     (*out)[padnear] = 0;
     iTempMatrix[l-i+1 + 3*(k-j+1)] = 1;
    } 
  }

 for (k = 0; k < 3; k++)
  for (l = 0; l < 3; l++)
   if (iTempMatrix[l + 3*k] > 0)
    MaxCluster(&(*showYou), &(*in), &(*out),
               nowPad+l-1 + maxCols*(k-1), maxCode, minAmpl);
}                    
//============================================================================

//----------------------------------------------------------------------------
void HRichRingFind::MaxFinding(HRichAnalysis *showYou, TList *hitList,
                               TArrayI *in, TArrayI *out, Int_t ringnr, Float_t distance) {
//
// This function looks for the 8-connected local maxima and fills
// the output "out" array with the values. The "out" array is created here
// in such a way that the amplitude of a local maximum is copied from
// input "in" array, the other pads (not local maxima) are marked with '-1'.
// Not fired pads are marked with '0'. 
// Parameters of local maxima are first put to the list fHitCandidate
// of HRichHitCandidate objects. The list is sorted descending and "ringnr" 
// or less of hits wich are in a distance of "distance" from each other are 
// stored in a "hitList".
//
 
 Int_t i,j,k,l,m,n,pad,padnear;
 Int_t x1,y1,x2,y2;
 Int_t iHitCount = 0;
 Bool_t fMax = kTRUE;
 Int_t iHit = 0;
 HRichLabel *pLabel = NULL;
 

 fHitCandidate.Delete();

 for (m = 0; m < showYou->GetLabelNr(); m++) {
  pLabel = showYou->GetLabel(m);
  for (j = pLabel->iLowerY; j <= pLabel->iUpperY; j++) {
   ly_from = ((j-1 < 0) ? 0 : j-1);
   ly_to   = ((j+1 >= maxRows) ? maxRows-1 : j+1);

   for (i = pLabel->iLeftX; i <= pLabel->iRightX; i++) {
    lx_from = ((i-1 < 0) ? 0 : i-1);
    lx_to   = ((i+1 >= maxCols) ? maxCols-1 : i+1);

    pad = i + maxCols*j;
    if ((*in)[pad]==0) (*out)[pad] = 0;
     else {

      fMax = kTRUE;

      for (k = ly_from; k <= ly_to; k++)
       for (l = lx_from; l <= lx_to; l++) {
        padnear = l + maxCols*k;
        if (iPadActive[padnear] && !(l==i && k==j))
         if ((*in)[padnear] > (*in)[pad])
          fMax = kFALSE;
       }

      if (fMax) {
       (*out)[pad] = (*in)[pad];
       fHitCandidate.Add(new HRichHitCandidate(i,j,(*in)[pad],m,++iHitCount));
      } else (*out)[pad] = -1;

     }
   }
  }  // end of loops over all pads of given label
 } // end of loops over all labels

 // now selection of ringnr of the highest maxima to be processed
 // checking the distance between candidates

 fHitCandidate.Sort(kSortDescending);
 if (iHitCount>=1) 
  for (j=0; j<iHitCount; j++) {
   if (iHit<ringnr && ((HRichHitCandidate*)(fHitCandidate.At(j)))->getA() > 0) {
    iHit++;
    for (i=j+1; i<iHitCount; i++) {
     if (iHit < ringnr)
      if (((HRichHitCandidate*)(fHitCandidate.At(j)))->getA() > 0 &&
          ((HRichHitCandidate*)(fHitCandidate.At(i)))->getA() > 0) {
        x1 = ((HRichHitCandidate*)(fHitCandidate.At(j)))->getX();
        y1 = ((HRichHitCandidate*)(fHitCandidate.At(j)))->getY();
        x2 = ((HRichHitCandidate*)(fHitCandidate.At(i)))->getX();
        y2 = ((HRichHitCandidate*)(fHitCandidate.At(i)))->getY();
        if (CalcDistance(x1,y1,x2,y2) <= distance) 
         ((HRichHitCandidate*)(fHitCandidate.At(i)))->setA(0);
      }
 
    }
   }
  }
 

  for (i=0; i<iHitCount; i++)
   if (iHit>0 && ((HRichHitCandidate*)(fHitCandidate.At(i)))->getA() > 0) {
    j = ((HRichHitCandidate*)(fHitCandidate.At(i)))->getX();
    k = ((HRichHitCandidate*)(fHitCandidate.At(i)))->getY();
    l = ((HRichHitCandidate*)(fHitCandidate.At(i)))->getA();
    m = ((HRichHitCandidate*)(fHitCandidate.At(i)))->getPadLabel();
    n = ((HRichHitCandidate*)(fHitCandidate.At(i)))->getMaxLabel();
    hitList->Add(new HRichHit(j,k,l,m,n));
    iHit--;
   }

}
//============================================================================

//----------------------------------------------------------------------------
void HRichRingFind::RingFindFitMatrix(HRichAnalysis *showMe, 
                    Int_t minampl, Int_t distance, Int_t howmanyrings) {

//
// Ring recognition algorithm: Pattern Matrix
//

Int_t i,j,m, lx,ly, pad;
Int_t iRingQuality;
Int_t iLabelNr = showMe->GetLabelNr();
HRichLabel *pLabel = NULL;
HRichHit *pHit = NULL;


iHitCount = 0; 

if (howmanyrings < 1) howmanyrings = 1;


iPadPlane.Reset();
iPadPlaneCopy.Reset();
fHitList1.Delete();


 for (m = 0; m < iLabelNr; m++) {
  pLabel = showMe->GetLabel(m);
  for (j = pLabel->iLowerY; j <= pLabel->iUpperY; j++) {
   ly_from = ((j-iMatrixHalfSize < 0) ? 0 : j-iMatrixHalfSize);
   ly_to   = ((j+iMatrixHalfSize >= maxRows) ? maxRows-1 : j+iMatrixHalfSize);

   for (i = pLabel->iLeftX; i <= pLabel->iRightX; i++) {
    lx_from = ((i-iMatrixHalfSize < 0) ? 0 : i-iMatrixHalfSize);
    lx_to   = ((i+iMatrixHalfSize >= maxCols) ? maxCols-1 : i+iMatrixHalfSize);

    iRingQuality = 0;
   

    for (ly = ly_from; ly <= ly_to; ly++)
    for (lx = lx_from; lx <= lx_to; lx++) {

      pad = lx + maxCols*ly;
      if (iPadActive[pad])
       if (showMe->GetPad(pad)->getAmplitude() > 0 && 
           showMe->GetPad(pad)->getLabel() == pLabel->iSignature)
        iRingQuality += pAnalysisParams->
         iRingMatrix[lx-i+iMatrixHalfSize + iMatrixSize*(ly-j+iMatrixHalfSize)];
    }  // end of loop over all pattern matrix pads

    if (iRingQuality > 0) 
     iPadPlane[i + maxCols*j] += iRingQuality;
   }
  }  // end of loops over all pads of given label
 } // end of loops over all labels 


 MaxFinding(showMe,&fHitList1,&iPadPlane,&iPadPlaneCopy,howmanyrings,distance);
 MaxSelector(showMe,&fHitList1,&iPadPlane,&iPadPlaneCopy);
 MaxAnalysis(showMe,&fHitList1,&iPadPlane,&iPadPlaneCopy,minampl);


 for (m = 0; m < fHitList1.GetSize(); m++) {
  pHit = (HRichHit*)(fHitList1.At(m));
  CalcRingParameters(showMe,&(*pHit));
  pHit->fTests = TestRing(showMe,&(*pHit),minampl);
 }


} // eof RingFindFitMatrix
//============================================================================

//----------------------------------------------------------------------------
void HRichRingFind::RingFindHoughTransf(HRichAnalysis *showMe, 
                    Int_t minampl, Int_t distance, Int_t howmanyrings) {

//
// Ring recognition algorithm: Hough Transform
//

Int_t i,j,k,m,nrFired;
Float_t fDistance;
Float_t fRingX, fRingY, fRingR, fDiv;
HRichHit *pHit = NULL;
HRichLabel *pLabel = NULL;
HRichPadSignal *pPad = NULL;

iHitCount = 0;

if (howmanyrings < 1) howmanyrings = 1; 

iPadPlane.Reset();
iPadPlaneCopy.Reset();
fHitList2.Delete();

for (m = 0; m < showMe->GetLabelNr(); m++) {

 pLabel = showMe->GetLabel(m);
 nrFired = pLabel->iFiredPadsNr;

 k = 0;
 for (j = pLabel->iLowerY; j <= pLabel->iUpperY; j++)
  for (i = pLabel->iLeftX; i <= pLabel->iRightX; i++) {
   pPad = showMe->GetPad(i,j);
   if (pPad->getAmplitude() > 0  && pPad->getLabel() == pLabel->iSignature) {
    iPadCol[k] = i;
    iPadRow[k] = j;
    k++;
    if (k >  nrFired) exit(1); 
   }
  }


 for (i = 0; i < nrFired-2; i++)
  for (j = i+1; j < nrFired-1; j++) {

    d_col_ij = iPadCol[i]-iPadCol[j];
    d_row_ij = iPadRow[i]-iPadRow[j];
    fDistance = sqrt(d_col_ij*d_col_ij + d_row_ij*d_row_ij);

    if (fDistance > pAnalysisParams->iRingRadius/2 && 
        fDistance < pAnalysisParams->iRingMatrixSize)

     for (k = j+1; k < nrFired; k++) {

       d_col_jk = iPadCol[j]-iPadCol[k];
       d_row_jk = iPadRow[j]-iPadRow[k];
       fDistance = sqrt(d_col_jk*d_col_jk + d_row_jk*d_row_jk);

       if (fDistance > pAnalysisParams->iRingRadius/2 && 
           fDistance < pAnalysisParams->iRingMatrixSize) {
        
 // ................. create  map .........

         fDiv = d_col_jk*d_row_ij - d_col_ij*d_row_jk;

         if (TMath::Abs(fDiv) >= 2.0) { 

          d2_colrow_jk = iPadCol[j]*iPadCol[j] - iPadCol[k]*iPadCol[k] +
                         iPadRow[j]*iPadRow[j] - iPadRow[k]*iPadRow[k];
          d2_colrow_ij = iPadCol[i]*iPadCol[i] - iPadCol[j]*iPadCol[j] +
                         iPadRow[i]*iPadRow[i] - iPadRow[j]*iPadRow[j];

          fRingX = 0.5*((Float_t)(d2_colrow_jk*d_row_ij - 
                                  d2_colrow_ij*d_row_jk)) / fDiv;

          fRingY = 0.5*((Float_t)(d2_colrow_ij*d_col_jk -
                                  d2_colrow_jk*d_col_ij)) / fDiv;

          fRingR = sqrt((iPadCol[i]-fRingX)*(iPadCol[i]-fRingX) +
                        (iPadRow[i]-fRingY)*(iPadRow[i]-fRingY));
  
          if (fRingR < (0.5 + pAnalysisParams->iRingRadius + pAnalysisParams->iRingRadiusError) &&
              fRingR > (0.5 + pAnalysisParams->iRingRadius - pAnalysisParams->iRingRadiusError) &&
              pLabel->iLeftX <= (Int_t)fRingX && 
              pLabel->iRightX >= (Int_t)fRingX &&
              pLabel->iLowerY <= (Int_t)fRingY && 
              pLabel->iUpperY >= (Int_t)fRingY)
           iPadPlane[(Int_t)fRingX + maxCols*(Int_t)fRingY] += 1;
         }

// eof create map ...............

       }
     }
   
  }

}  // end of loop over all labels

// ..................................... find MAX ..............
// 



 MaxFinding(showMe,&fHitList2,&iPadPlane,&iPadPlaneCopy,howmanyrings,distance);
 MaxSelector(showMe,&fHitList2,&iPadPlane,&iPadPlaneCopy);
 MaxAnalysis(showMe,&fHitList2,&iPadPlane,&iPadPlaneCopy,minampl);


 for (m = 0; m < fHitList2.GetSize(); m++) {
  pHit = (HRichHit*)(fHitList2.At(m));
  CalcRingParameters(showMe,&(*pHit));
  pHit->fTests = TestRing(showMe,&(*pHit),minampl);
 }


} // eof RingFindHoughTransf
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichRingFind::TestDensity(HRichAnalysis *showYou, HRichHit *pHit) {

//
// If density (ratio: fired pads / all active pads) in label larger
// than 2*Sm - see below - exceeds 40%, ring recognition is not performed.
// For small labels like < 2*(pattern matrix surface) this test is not performed.
//

if (pAnalysisParams->isActiveTestDensity) {

Int_t iLabelNr = 0, iActivePads = 0, iSurface = 0,
      iActiveSurface = 0, iMatrixSurface = 0;

  iLabelNr = pHit->iRingFreeParam;
  iActivePads = showYou->GetLabel(iLabelNr)->iFiredPadsNr;
  iSurface = (showYou->GetLabel(iLabelNr)->iUpperY -
             showYou->GetLabel(iLabelNr)->iLowerY + 1)*
             (showYou->GetLabel(iLabelNr)->iRightX -
             showYou->GetLabel(iLabelNr)->iLeftX + 1);
  iActiveSurface = showYou->GetLabel(iLabelNr)->iLabeledPadsNr;
  iMatrixSurface = pAnalysisParams->iRingMaskSize * pAnalysisParams->iRingMaskSize;
  if (iActiveSurface==0 || iMatrixSurface==0) Error("HRichRingFind::TestDensity","possible division by zero");
  if ((Float_t)iActivePads/iActiveSurface > pAnalysisParams->fThresholdDensity &&
      (Float_t)iActiveSurface/(2*iMatrixSurface) > pAnalysisParams->fSurfaceArea) 
   return kFALSE;

 pHit->setTestDens(kTRUE);

}
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichRingFind::TestBorder(HRichAnalysis *showYou, HRichHit *pHit, Int_t amplit) {

//
// Basic check for the ring amplitude. 
// Reduction of the threshold amplitude at the borders of the detector
// is applied, proportionally to the part of a ring which is cut (outside).
// Maximum reduction is by 50% (it means half or more of a ring can be still
// recognised. If there is more than a half of a ring outside it must have
// iRingQuality >= amplit (without any threshold reduction) to be accepted.
// Additionally in this test fBorderFactor is written to a hit !
//


  Float_t fraction = pGeometryParams->getPadsPar()->
    getPad(pHit->iRingX,pHit->iRingY)->getAmplitFraction();
  
  pHit->fBorderFactor = fraction;
  
  if (!pAnalysisParams->isActiveBorderAmplitReduction && fraction < 0.95)
    if (pHit->iRingQuality < amplit) return kFALSE;
  
  if (pAnalysisParams->isActiveBorderAmplitReduction && amplit && fraction < 0.95) {
    
    if (fraction < 0.5) {
      if (pHit->iRingQuality < amplit)  return kFALSE; 
    } else if (pHit->iRingQuality < (amplit*fraction)) return kFALSE;
    
  } 
  
  pHit->setTestBord(kTRUE);
  
  return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichRingFind::TestDynamic(HRichAnalysis *showYou, HRichHit *pHit, Int_t amplit) {

//
// The dynamic minimal (threshold) amplitude (quality) is estimated here
// if called with amplit > 0 and for rings that are mainly not outside.
// It is calculated due to highly fenomenological formula:
//
//      threshold Amplitude = amplit * e^( P1*(S/Sm - 1) + P2*(D/P3 - 1) )
//  where
//      P1 - parameter (0.055)
//      P2 - parameter (0.5)
//      Sm = ring pattern matrix surface (in number of pads units)
//      S  = number of pads in given labeled area
//      P3 = mean density (ratio: pads fired / all pads) for ring matrix area ~0.15
//      D  = density of fired pads for given labeled area
//


 Float_t fraction = pGeometryParams->getPadsPar()->
                    getPad(pHit->iRingX,pHit->iRingY)->getAmplitFraction();

 if (!pAnalysisParams->isActiveDynamicThrAmplitude && fraction >= 0.95)
  if (pHit->iRingQuality < amplit) return kFALSE;


 if (pAnalysisParams->isActiveDynamicThrAmplitude && amplit && fraction >= 0.95) {
     
  Int_t iDynamicAmplit = 0, iLabelNr = 0;
  Int_t iActivePads = 0, iActiveSurface = 0, iMatrixSurface = 0;

  iLabelNr = pHit->iRingFreeParam;
  iActivePads = showYou->GetLabel(iLabelNr)->iFiredPadsNr;
  iActiveSurface = showYou->GetLabel(iLabelNr)->iLabeledPadsNr;
  iMatrixSurface = pAnalysisParams->iRingMaskSize * pAnalysisParams->iRingMaskSize;
  if (iActiveSurface==0 || iMatrixSurface==0) Error("HRichRingFind::TestDynamic","possible division by zero");
  Float_t fSurfRatio = (Float_t)iActiveSurface/iMatrixSurface;
  Float_t fDensRatio = (Float_t)iActivePads/iActiveSurface;

   if (fSurfRatio <= 1.34 && fDensRatio <= 1.34*pAnalysisParams->fFormulaParam3) {
    iDynamicAmplit = (Int_t)(amplit * pAnalysisParams->fLowerAmplFactor);
   } else {
    iDynamicAmplit = (Int_t)(amplit * exp(pAnalysisParams->fFormulaParam1 * 
                     (fSurfRatio - 1.) +
                     pAnalysisParams->fFormulaParam2 * 
                     (fDensRatio/pAnalysisParams->fFormulaParam3 - 1.)));
    if (iDynamicAmplit < amplit) iDynamicAmplit = amplit;
   }

   if (iDynamicAmplit > pHit->iRingQuality) return kFALSE;
 } 

 pHit->setTestDyna(kTRUE);

return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichRingFind::TestRatio(HRichAnalysis *showYou, HRichHit *pHit) {

//
// This part calculates how many fired pads belong to ring in comparison
// with the number of fired pads outside and inside. The area of ringMask
// is scanned. In it the fired pads outside/inside of ring must
// not to exceed i.e.  33% of total number of fired pads in this area -
// at least 67% has to be inside of ring border.
// If ring is outside the test is not done (always positive).
//

 if (pAnalysisParams->isActiveFiredRingPadsRatio &&
     pGeometryParams->getPadsPar()->
          getPad(pHit->iRingX,pHit->iRingY)->getAmplitFraction() >= 0.95) {

  Int_t k,m,n;
  Int_t iMatrixSurface, iOutRing = 0, iOnRing = 0, iInRing = 0, iAllRing = 0;
  iMatrixSurface = pAnalysisParams->iRingMaskSize * pAnalysisParams->iRingMaskSize;

  for (k = 0; k < iMatrixSurface; k++) {
   m = (k % pAnalysisParams->iRingMaskSize) - pAnalysisParams->iRingMaskSize/2;
   n = (k / pAnalysisParams->iRingMaskSize) - pAnalysisParams->iRingMaskSize/2;
   if (!showYou->IsOut(pHit->iRingX,pHit->iRingY,m,n) &&
       showYou->GetPad(pHit->iRingX+m,pHit->iRingY+n)->getAmplitude() > 0)
    if (pAnalysisParams->iRingMask[k] == 0) iOutRing++;
    else
     if (pAnalysisParams->iRingMask[k] == 1) iOnRing++;
     else
      if (pAnalysisParams->iRingMask[k] == 2) iInRing++;
  }
 
  iAllRing = iOutRing + iOnRing + iInRing;
  if (iOutRing+iInRing >= pAnalysisParams->fFiredRingPadsRatio*iAllRing) return kFALSE;

 } 

 pHit->setTestRati(kTRUE);

return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichRingFind::TestAsymmetry(HRichAnalysis *showYou, HRichHit *pHit, Int_t amplit) {
  
  //
  // Asymmetry of ring is investigated. First mean value of fired pads for x
  // and y is calculated. It can not be larger than iRingRadiusError given in input data.
  // Then a ring radius is calculated and it also has to fit
  // in the value iRingRadius +/- iRingRadiusError.
  // If a ring is clearly outside the border the test is not done (always positive).
  // REMARK: ring radius and ring centroid is calculated ONLY for rings
  // inside the frame! If the ring candidate is placed partially outside
  // zero values for its radius and centroid are stored in HRichHit.
  //
  
  if (pAnalysisParams->isActiveTestAsymmetry &&
      pGeometryParams->getPadsPar()->
      getPad(pHit->iRingX,pHit->iRingY)->getAmplitFraction() >= 0.95) {
    
    Int_t i,j;
    
    Int_t iHalfRingMatrix = pAnalysisParams->iRingMatrixSize/2;
    Float_t iPosX = 0., iPosY = 0.; 
    Int_t iHowManyPads = 0;
    Float_t dx = 0., dy = 0., fRingRadius = 0., fRadCalc = 0.;
    Float_t fRingCentroid = 0.;
    
    // 
    // calculation of ring centroid
    //
    
    for (j = 0; j < pAnalysisParams->iRingMatrixSize; j++)
      for (i = 0; i < pAnalysisParams->iRingMatrixSize; i++)
	if (!showYou->IsOut(pHit->iRingX,pHit->iRingY,i-iHalfRingMatrix,j-iHalfRingMatrix))
	  if (showYou->GetPad(pHit->iRingX+i-iHalfRingMatrix,
			      pHit->iRingY+j-iHalfRingMatrix)->getAmplitude() > 0) {
	    iPosX += i - iHalfRingMatrix;
	    iPosY += j - iHalfRingMatrix;
	    iHowManyPads++;
	  }
    if (iHowManyPads==0) Error("HRichRingFind::TestAsymmetry","possible division by zero");
    iPosX /= iHowManyPads;
    iPosY /= iHowManyPads;
    
    fRingCentroid = sqrt(iPosX*iPosX + iPosY*iPosY);
    pHit->fRingCentroid = fRingCentroid;
    
    //
    // calculation of ring radius
    //
    
    iHowManyPads = 0;
    
    for (j = 0; j < pAnalysisParams->iRingMatrixSize; j++)
      for (i = 0; i < pAnalysisParams->iRingMatrixSize; i++)
	if (!showYou->IsOut(pHit->iRingX,pHit->iRingY,i-iHalfRingMatrix,j-iHalfRingMatrix))
	  if (showYou->GetPad(pHit->iRingX+i-iHalfRingMatrix,
			      pHit->iRingY+j-iHalfRingMatrix)->getAmplitude() > 0) { 
	    
	    // ring radius is calculated as harmonic mean here
	    
	    dx = i-iHalfRingMatrix;
	    dy = j-iHalfRingMatrix;
	    fRadCalc = sqrt(dx*dx + dy*dy);
	    if (fRadCalc == 0) fRadCalc = 1.;
	    else iHowManyPads++;
	    fRingRadius += 1./fRadCalc;
	  }

    fRingRadius = iHowManyPads/fRingRadius;
    pHit->fRingRadius = fRingRadius;
    
    if (fRingCentroid > pAnalysisParams->iRingRadiusError || 
	TMath::Abs(fRingRadius - pAnalysisParams->iRingRadius) >
	pAnalysisParams->iRingRadiusError) return kFALSE;
    
  } 
  
  pHit->setTestAsym(kTRUE);
  
  return kTRUE;
}
//============================================================================
Bool_t HRichRingFind::TestRingCharge(HRichHit *hit) {
  // Average charge of ring candidate is checked, minimum and maximum
  //value are set in the richanalysispar.txt. 
    //cout<<" in test charge "<<endl;
  Float_t ringMinCharge = pAnalysisParams->fRingMinCharge;
  Float_t ringMaxCharge = pAnalysisParams->fRingMaxCharge;

  if(pAnalysisParams->isActiveTestCharge && ringMinCharge && ringMaxCharge ){
      //cout<<" looking for the cut "<<endl;
      //cout<<" average ampli "<<Eit->iRingAmplitude <<" compared with "<<(Int_t)(ringMinCharge*(Float_t)hit->iRingPadNr) + 5<<endl;
      if (hit->iRingPadNr < 1) return kFALSE; 
      if(hit->iRingAmplitude < (Int_t)(ringMinCharge*(Float_t)hit->iRingPadNr) + 5 || hit->iRingAmplitude/hit->iRingPadNr > ringMaxCharge) {
      return kFALSE;
      }
  }
  hit->setTestCharge(kTRUE);
  return kTRUE;
} 
//----------------------------------------------------------------------------
Int_t HRichRingFind::TestRing(HRichAnalysis *showIt, HRichHit *hit, Int_t amplit) {

//
// There are five tests at the moment and they are active if their 
// Status is set to 1 or 2. If Status == 1 a given test is
// performed and the result is stored in the HRichHit structure.
// If Status == 2, the test result must be positive to store
// the hit candidate in the output file. If Status == 0 the test
// is not performed and the information stored in HRichHit can
// be false or true, depending on the test (see tests for details).
//

 Int_t test = 0, result = 0;

 result = (Int_t)TestDensity(&(*showIt), &(*hit));
 if (pAnalysisParams->isActiveTestDensity == 2 && result == 0) 
  return ((test = 3));
 test += 1*result;

 result = (Int_t)TestBorder(&(*showIt), &(*hit), amplit);
 if (pAnalysisParams->isActiveBorderAmplitReduction == 2 && result == 0)
  return ((test = 3));
 test += 10*result;

 result = (Int_t)TestDynamic(&(*showIt), &(*hit), amplit);
 if (pAnalysisParams->isActiveDynamicThrAmplitude == 2 && result == 0)
  return ((test = 3));
 test += 100*result;

 result = (Int_t)TestRatio(&(*showIt), &(*hit));
 if (pAnalysisParams->isActiveFiredRingPadsRatio == 2 && result == 0)
  return ((test = 3));
 test += 1000*result;

 result = (Int_t)TestAsymmetry(&(*showIt), &(*hit), amplit);
 if (pAnalysisParams->isActiveTestAsymmetry == 2 && result == 0)
   return ((test = 3));
 test += 10000*result;
 
 result = (Int_t)TestRingCharge(&(*hit));
 if (pAnalysisParams->isActiveTestCharge == 2 && result == 0)
   return ((test = 3));
 test += 100000*result;
 
return test;
}
//============================================================================

//----------------------------------------------------------------------------
void HRichRingFind::CalcRingParameters(HRichAnalysis *showMe,HRichHit *pHit){
Int_t i,j,k,l,m,
       iIsPhot4, iIsPhot8, iPhot4Nr, iPhot8Nr, iPad;
 Int_t iNowX, iNowY, iShift;

 iPhot4Nr = iPhot8Nr = iPad = 0;
 iShift = pAnalysisParams->iRingMaskSize/2;


  iRingTempImage.Reset();
  for (j = 0; j < pAnalysisParams->iRingMaskSize; j++) 
      for (i = 0; i < pAnalysisParams->iRingMaskSize; i++) {
	  if (!showMe->IsOut(pHit->iRingX,pHit->iRingY,i-iShift,j-iShift)) {
	      pHit->iRingImage[i + pAnalysisParams->iRingMaskSize*j] =
		  showMe->GetPad(pHit->iRingX+i-iShift, pHit->iRingY+j-iShift)->getAmplitude();
	  } else pHit->iRingImage[i + pAnalysisParams->iRingMaskSize*j] = 0;
      }
  
  iPhot4Nr = iPhot8Nr = 0;
  iNowX = pHit->iRingX;
  iNowY = pHit->iRingY;
  for (j = 0; j < pAnalysisParams->iRingMaskSize; j++)
      for (i = 0; i < pAnalysisParams->iRingMaskSize; i++)
	  if (!showMe->IsOut(iNowX,iNowY,i-iShift,j-iShift)) {
	      iIsPhot4 = iIsPhot8 = 0;
	      m = iNowX+i-iShift + maxCols*(iNowY+j-iShift);
	      if (showMe->GetPad(m)->getAmplitude() > 0 && 
		  pAnalysisParams->iRingMask[i + (pAnalysisParams->iRingMaskSize)*j] == 1) {
		  pHit->iRingPadNr++;
		  pHit->iRingAmplitude += showMe->GetPad(m)->getAmplitude();
		  for (k = -1; k < 2; k++)
		      for (l = -1; l < 2; l++)
			  if (((l == 0 && abs(k)) || (k == 0 && abs(l))) && !(l == 0 && k == 0) &&
			      !showMe->IsOut(m,l,k) && 
			      showMe->GetPad(m+l,k)->getAmplitude() >= 
			      showMe->GetPad(m)->getAmplitude()) {
			      iIsPhot4++;
			  }
		  if (iIsPhot4 == 0) {
		      iPhot4Nr++;
		      iRingTempImage[i + (pAnalysisParams->iRingMaskSize)*j] += 1;
		  }
		  for (k = -1; k < 2; k++)
		      for (l = -1; l < 2; l++)
			  if (abs(l) && abs(k) && !showMe->IsOut(m,l,k) && 
			      showMe->GetPad(m+l,k)->getAmplitude() >= 
			      showMe->GetPad(m)->getAmplitude()) {
			      iIsPhot8++;
			  }
		  if (iIsPhot4 == 0 && iIsPhot8 == 0) {
		      iPhot8Nr++;
		      iRingTempImage[i + (pAnalysisParams->iRingMaskSize)*j] += 1;
		  }
	      }
	  }
  
  pHit->iRingLocalMax4 = iPhot4Nr;
  pHit->iRingLocalMax8 = iPhot8Nr;
  
 
 
}
//============================================================================

//----------------------------------------------------------------------------
void HRichRingFind::CalcFakeContribution(HRichAnalysis *showMe) {
    
    Int_t i,j,k,l,m,n,
	iIsPhot4, iIsPhot8, iPhot4Nr, iPhot8Nr, iPad;
    Int_t iNowX, iNowY, iShift;
    
    iPhot4Nr = iPhot8Nr = iPad = 0;
    iShift = pAnalysisParams->iRingMaskSize/2;
    
    if (iRingNr == 0 && 
	pGeometryParams->getPadsPar()->getActivePadsNr() > iShift*iShift*4) {
	
	// If any ring is not found a random position iNowX, iNowY is drawn and
	// treated as if it were a ring centre. Local maxima (4- and 8-connected)
	// are calculated as well as the number of fired pads. This gives
	// the estimation of fakes contribution in real rings (number of fake
	// fired pads and fake local maxima). 
	
	do {
	    iNowX = (Int_t)HomogenDistr(pAnalysisParams->iRingRadius, 
					maxCols - pAnalysisParams->iRingRadius);
	    iNowY = (Int_t)HomogenDistr(pAnalysisParams->iRingRadius, 
					maxRows - pAnalysisParams->iRingRadius);
	} while (showMe->IsOut(iNowX,iNowY,0,0));
	
	for (j = 0; j < pAnalysisParams->iRingMaskSize; j++)
	    for (i = 0; i < pAnalysisParams->iRingMaskSize; i++)
		if (!showMe->IsOut(iNowX,iNowY,i-iShift,j-iShift)) {
		    iIsPhot4 = iIsPhot8 = 0;
		    m = iNowX+i-iShift + maxCols*(iNowY+j-iShift);
		    if (showMe->GetPad(m)->getAmplitude() > 0 && 
			pAnalysisParams->iRingMask[i + (pAnalysisParams
							->iRingMaskSize)*j] == 1) {
			iPad++;
			for (k = -1; k < 2; k++)
			    for (l = -1; l < 2; l++)
				if (((l == 0 && abs(k)) || 
				     (k == 0 && abs(l))) && 
				    !(l == 0 && k == 0) &&
				    !showMe->IsOut(m,l,k) && 
				    showMe->GetPad(m+l,k)->getAmplitude() >= 
                                    showMe->GetPad(m)->getAmplitude())
				    iIsPhot4++;
			if (iIsPhot4 == 0) iPhot4Nr++;
			for (k = -1; k < 2; k++)
			    for (l = -1; l < 2; l++)
				if (abs(l) && abs(k) && 
				    !showMe->IsOut(m,l,k) && 
				    showMe->GetPad(m+l,k)->getAmplitude() 
				    >= showMe->GetPad(m)->getAmplitude())
				    iIsPhot8++;
			if (iIsPhot4 == 0 && iIsPhot8 == 0) iPhot8Nr++;
		    }
		}
	
	showMe->iFakePad = iPad;
	showMe->iFakeLocalMax4 = iPhot4Nr;
	showMe->iFakeLocalMax8 = iPhot8Nr;
	
    } else {
	
	for (n = 0; n < iRingNr; n++) {
	    iCount = 0;
	    fClusterSize = 0.;
	    fClusterLMax4 = 0.;
	    fClusterLMax8 = 0.;
	    for (j = 0; j < iRingImageSize; j++)
		for (i = 0; i < iRingImageSize; i++)
		    if (pRings[n].iRingImage[i + iRingImageSize*j] > 0) {
			iInnerCount = iInnerPhot4 = iInnerPhot8 = 0;
			CalcRingClusters(showMe, &iRingTempImage[0], n, i, j);
			if (iInnerCount) {
			    fClusterSize += iInnerCount;
			    fClusterLMax4 += iInnerPhot4;
			    fClusterLMax8 += iInnerPhot8;
			    iCount++;
			}
		    }
	    pRings[n].iRingClusterNr = iCount;
	    if (iCount > 0) {
		pRings[n].fRingClusterSize = fClusterSize/iCount;
		pRings[n].fRingClusterLMax4 = fClusterLMax4/iCount;
		pRings[n].fRingClusterLMax8 = fClusterLMax8/iCount; 
	    } 
	    
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
          if (pAnalysisParams->iRingMask[nowX+a-1 + 
                (pAnalysisParams->iRingMaskSize)*(nowY+b-1)] == 1) iInnerCount++;
          dumpArr[nowX+a-1 + iRingImageSize*(nowY+b-1)] = 3;
          iTempMatrix[a][b] = 1;

     }

 for (b = 0; b < 3; b++)
  for (a = 0; a < 3; a++)
   if (iTempMatrix[a][b] > 0) 
    CalcRingClusters(showYou,dumpArr,ringNr,nowX+a-1,nowY+b-1);

} // eof CalcRingClusters
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichRingFind::GetAlgorithmNr(HRichAnalysis *showMe) {
 return (pAnalysisParams->isActiveRingFindFitMatrix + 
         pAnalysisParams->isActiveRingHoughTransf);
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichRingFind::MatchRings(HRichAnalysis *showMe, TList *hitList1,
                                                       TList *hitList2) {
//
// If both algorithms are active and iSuperiorAlgorithmID = 3 then
// hit with iRingAlgorithmIndex = 3 is Pattern Matrix information
// hit with iRingAlgorithmIndex = 4 is Hough Transform information
// iSuperiorAlgorithmID = 3 means all rings found independently by
// two algorithms are stored.
// 
// If both algorithms are active and iSuperiorAlgorithmID = 1 or 2 then
// only rings found by both algorithms are stored. If iSuperiorAlgorithmID = 1
// information is taken from Pattern Matrix and iRingAlgorithmIndex = 5.
// If iSuperiorAlgorithmID = 2 information is taken from Hough Transform 
// and iRingAlgorithmIndex = 6. This distinction is made because the
// position of a ring found by both algorithms may slightly differ and
// therefore also ring parameters may differ.
//
// If only one algorithm is active then iSuperiorAlgorithmID flag is
// ignored and rings found by the active algorithm are stored. 
// If it is Pattern Matrix iRingAlgorithmIndex = 1 and in the case
// of Hough Transform iRingAlgorithmIndex = 2.
// 
// In general if iRingAlgorithmIndexis is odd (1,3,5) information on a ring 
// found by Pattern Matrix algorithm is stored and if iRingAlgorithmIndexis 
// is even (2,4,6) information on a ring found by Hough Transform algorithm 
// is stored.
// 

Int_t i,j,m,iChosen = 0;
HRichHit *pHit1 = NULL;
HRichHit *pHit2 = NULL;
Int_t listSize1, listSize2;
iRingNr = 0;
listSize1 = hitList1->GetSize();
listSize2 = hitList2->GetSize();

//
// --- all rings found by all algorithms will be stored ---
//
if (GetAlgorithmNr(showMe) == 2 && pAnalysisParams->iSuperiorAlgorithmID == 3) {
 for (m = 0; m < listSize1; m++) {
  pHit1 = (HRichHit*)(hitList1->At(m));
  if (pHit1->fTests != 3) {
   pRings[iRingNr] = *pHit1;
   pRings[iRingNr].iRingAlgorithmIndex = 3;
   pRings[iRingNr].iRingPatMat = pHit1->iRingQuality;
   pRings[iRingNr].iRingHouTra = 0;
   iRingNr++;
  }
 }
 for (m = 0; m < listSize2; m++) {
  pHit2 = (HRichHit*)(hitList2->At(m));
  if (pHit2->fTests != 3) {
   pRings[iRingNr] = *pHit2;
   pRings[iRingNr].iRingAlgorithmIndex = 4;
   pRings[iRingNr].iRingPatMat = 0;
   pRings[iRingNr].iRingHouTra = pHit2->iRingQuality;
   iRingNr++;
  }
 }
CalcFakeContribution(showMe);
return (showMe->iRingNr = iRingNr);
}
//
//
//

//
// --- only rings found by both algorithms stored ---
//
if (GetAlgorithmNr(showMe) == 2) {

 for (i = 0; i < listSize1; i++) {
  pHit1 = (HRichHit*)(hitList1->At(i));
  if (pHit1->fTests != 3 && pHit1->iRingQuality > 0) {
   for (j = 0; j < listSize2; j++) {
    pHit2 = (HRichHit*)(hitList2->At(j));
    iChosen = 0;
    if (pHit2->fTests != 3 && pHit2->iRingQuality > 0) {
     if (CalcDistance(*pHit1,*pHit2) <= pAnalysisParams->iRingRadius/2) {
      if (pAnalysisParams->iSuperiorAlgorithmID == 1) {
       pRings[iRingNr] = *pHit1;
       pRings[iRingNr].iRingPatMat = pHit1->iRingQuality;
       pRings[iRingNr].iRingHouTra = pHit2->iRingQuality;
       pRings[iRingNr].iRingAlgorithmIndex = 5;
       iRingNr++;
       iChosen = 1;
      } else if (pAnalysisParams->iSuperiorAlgorithmID == 2) {
       pRings[iRingNr] = *pHit2;
       pRings[iRingNr].iRingPatMat = pHit1->iRingQuality;
       pRings[iRingNr].iRingHouTra = pHit2->iRingQuality;
       pRings[iRingNr].iRingAlgorithmIndex = 6;
       iRingNr++;
       iChosen = 1;
      }
     }
    }
    if (iChosen) break;
   }
  }
 }
CalcFakeContribution(showMe);

return (showMe->iRingNr = iRingNr);
}
//
//
//

//
// --- rings found by selected algorithm ---
//
if (GetAlgorithmNr(showMe) == 1) {
 if (pAnalysisParams->isActiveRingFindFitMatrix) {
  for (m = 0; m < listSize1; m++) {
   pHit1 = (HRichHit*)(hitList1->At(m));
   if (pHit1->fTests != 3) {
    pRings[m] = *pHit1;
    pRings[m].iRingAlgorithmIndex = 1;
    pRings[m].iRingPatMat = pHit1->iRingQuality;
    pRings[m].iRingHouTra = 0;
    iRingNr++;
   }
  }
 
CalcFakeContribution(showMe);
return (showMe->iRingNr = iRingNr);
 }
 if (pAnalysisParams->isActiveRingHoughTransf) {
  for (m = 0; m < listSize2; m++) {
   pHit2 = (HRichHit*)(hitList2->At(m));
   if (pHit2->fTests != 3) {
    pRings[m] = *pHit2;
    pRings[m].iRingAlgorithmIndex = 2;
    pRings[m].iRingHouTra = pHit2->iRingQuality;
    pRings[m].iRingPatMat = 0;
    iRingNr++;
   }
  }
  CalcFakeContribution(showMe);
  return (showMe->iRingNr = iRingNr);
 }
}
//
//
//

return (showMe->iRingNr = 0); // this should never happen
}
//============================================================================


//----------------------------------------------------------------------------
void HRichRingFind::CloseMaxRejection(TList *hitList){

  // this method tests the quality of candidates close
  // to bright rings. Picking up some intensity of the
  // bright ring fakes could be identified.
  // If the quality of these "fakes" candidates is too
  // low compared with the quality of the bright ring
  // the candidates are rejected ( if the isActiveFakesRejection
  // is equal 2) or stored and labeled with a flag.
  // If the quality of the 2 compared rings is similar
  // the centroid of both is checked, if one of the two is 
  // very assymmetric is rejected.
 
  Int_t listSize = hitList->GetSize();
  HRichHit *pHit1 = NULL;
  HRichHit *pHit2 = NULL;

  Float_t maxFakeDistSquared = pAnalysisParams->iRingRadius * pAnalysisParams->iRingRadius * 4.2F;
  Float_t fakeQualityRatio = pAnalysisParams->fFakeQualityRatio;
  Float_t fakeCentroidCut = pAnalysisParams->fFakeCentroidCut;

  for (int i = 0; i <listSize ; i++) {
    pHit1 = (HRichHit*)(hitList->At(i));
    if (pHit1->fTests != 3) {
	//cout<<" i "<< i<<endl;
      for (int j = 0; j <listSize ; j++) {
	pHit2 = (HRichHit*)(hitList->At(j));
	if (pHit2->fTests != 3) {
	    //  cout<<" j "<<j<<endl;
	  Int_t dx = pHit1->iRingX - pHit2->iRingX;
	  Int_t dy = pHit1->iRingY - pHit2->iRingY;
	  Float_t distSquared=dx*dx+dy*dy;
	  //cout<<" distSquared "<<distSquared <<" maxFakeDistSquared "<< maxFakeDistSquared<<endl;
	  if (distSquared<maxFakeDistSquared && i!=j) {
	      // only candidates whose distance is lower then 
	      // ring diameter are compared
	      //  cout<<" pHit1->iRingQuality "<<pHit1->iRingQuality<<endl;
	      //cout<<" pHit2->iRingQuality "<<pHit2->iRingQuality<<endl;
	      if (pHit1->iRingQuality+pHit2->iRingQuality==0) Error("HRichRingFind::CloseMaxRejection","possible division by zero");
	    Float_t dQ =(Float_t)(pHit1->iRingQuality-pHit2->iRingQuality)
	      /(Float_t)(pHit1->iRingQuality+pHit2->iRingQuality);
	    //cout <<" dQ "<<dQ <<"  fakeQualityRatio "<<fakeQualityRatio<<endl;
	    if(dQ>fakeQualityRatio) pHit2->setRejFake(0); 
	    else if(dQ<-fakeQualityRatio) pHit1->setRejFake(0);

	    else {
	      if(dQ > 0.15 && pHit2->getCentroid()>=fakeCentroidCut)
		pHit2-> setRejFake(0); //ring rejected
	      else if(dQ < -0.15 && pHit1->getCentroid()>=fakeCentroidCut)
		pHit1->setRejFake(0) ;
	    }

	  }

	}
      }// end second loop on the ring list
    }
  }//end first loop on the ring list

  for (int i = 0; i <listSize ; i++) {
    pHit1 = (HRichHit*)(hitList->At(i));
    if ( pAnalysisParams-> isActiveFakesRejection ==2 
	 && pHit1->getRejFake()==0)
      pHit1-> fTests = 3;
    else pHit1-> fTests += pHit1->getRejFake()*1000000;
  }

}    
//============================================================================

//----------------------------------------------------------------------------

Int_t HRichRingFind::Execute(HRichAnalysis *giveMe) {


//if (giveMe->pRings) {
// giveMe->iRingNr = 0;
// delete [] giveMe->pRings;
// giveMe->pRings = NULL;
//}

if (giveMe->GetLabelNr() == 0 || GetAlgorithmNr(giveMe) == 0) {
 iRingNr = 0;
 CalcFakeContribution(giveMe);
return (giveMe->iRingNr = iRingNr);
}

iRingNr = 0;

if (pAnalysisParams->isActiveRingFindFitMatrix)
 RingFindFitMatrix(giveMe,pAnalysisParams->iMinimalFitMatrixRingQuality,
                   pAnalysisParams->iMinimalFitMatrixRingDistance,
                   pAnalysisParams->iHowManyFitMatrixRings);


if (pAnalysisParams->isActiveRingHoughTransf)
 RingFindHoughTransf(giveMe,pAnalysisParams->iMinimalHoughTransfRingQuality,
                     pAnalysisParams->iMinimalHoughTransfRingDistance,
                     pAnalysisParams->iHowManyHoughTransfRings);


 CloseMaxRejection(&fHitList1);
 CloseMaxRejection(&fHitList2);

 return MatchRings(giveMe,&fHitList1,&fHitList2);
 // fHitList1 contains the candidates found by the pattern matrix
 // fHitList2 contains the candidates found by the Hough trasformation.

} // eof
//============================================================================

