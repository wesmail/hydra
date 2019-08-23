// File: hrichringfind.cc
// ***************************************************************************

//*-- Author : Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
//*-- Modified : 1999/12/04 by Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
//*-- Modified Wolfgang Koenig Oct. 2000


#include <stdlib.h>
#include <math.h>
#include <iostream.h>
#include "hrichringfind.h"
#include "hruntimedb.h"
#include "hrichanalysis.h"
#include "TMath.h"
// ***************************************************************************
//////////////////////////////////////////////////////////////////////////////
//
//  HRichRingFind
//
//  Ring recognition algorithms.
//
//////////////////////////////////////////////////////////////////////////////
// ***************************************************************************
//
//  This class is entitled for the ring finding.
//  Its execute function is called in the HRichAnalysis::execute.
//  The rngs are sought by means of 2 algorithms : Hough Transformation,
//  Pattern Matrix. All found rings whose qualilty is sufficiently high
//  (cfr. comments in RingFindFitMatrix,RingFindHoughTransf fn.)
//  are additionaly tested (TestRing); the ring quality is tested, the fired
//  pads distribution and the assymetry of the ring can be tested as well
//  ( flags in the richanalysispar file). For each "good" candidate 
//  the corresponding paramaters are calculated (cfr CalcRingParameters).
//  Additional tests for fake rings are executed ( cfr deleteFakeRings,
//  rejectFakes) before storing the rings.  
//  The Hit Container is then filled up in the HRichAnalysis:updateHits fn.
//
//
//////////////////////////////////////////////////////////////////////////////

ClassImp(HRichRingFind)

//----------------------------------------------------------------------------
HRichRingFind::HRichRingFind() { 
 iInnerCount = 0;
 iInnerPhot4 = 0;
 iInnerPhot8 = 0;
 iClusterSize=0;
 iClusterLMax4=0;
 iClusterLMax8=0;
 iRingImageSize = 16;
 iRingNr = 0;

 thrReductionFac = 0;
 fakeQualityRatio= 0;
 fakeCentroidCut= 0;
 fakePadAmplitude= 0;
 iAlgorithm1RingNr = 0;
 iAlgorithm2RingNr = 0;
 iRingRadius = 4;
 iRingRadiusError = 1;
 iRingMatrixSize = 13;
 pRings = NULL;
 pAlgorithm1Ring = NULL;
 pAlgorithm2Ring = NULL;

 pAnalysisParams = NULL;
 pGeometryParams = NULL;
 pPadsFiredCol = NULL;
 pPadsFiredRow = NULL;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichRingFind::init(HRichAnalysis* showMe) {
 initParameters(showMe);

 if (pRings) delete [] pRings;
 Int_t maxRings = 0;
 if (pAnalysisParams->isActiveRingFindFitMatrix)
   maxRings += pAnalysisParams->iHowManyFitMatrixRings;
 if (pAnalysisParams->isActiveRingHoughTransf)
   maxRings += pAnalysisParams->iHowManyHoughTransfRings;
 pRings=new HRichHit[maxRings];
 showMe->pRings = pRings;
 return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
void HRichRingFind::initParameters(HRichAnalysis* showMe) {
 pAnalysisParams = showMe->getParams();
 pGeometryParams = (HRichGeometryPar*)showMe->getGeomPar();

 iInnerCount = 0;
 iInnerPhot4 = 0;
 iInnerPhot8 = 0;
 iRingImageSize = pAnalysisParams->iRingMaskSize;
 iRingTempImage.Set(iRingImageSize * iRingImageSize);
 iRingMatrixSize = pAnalysisParams->iRingMatrixSize;
 iRingRadius = pAnalysisParams->iRingRadius;
 iRingRadiusError = pAnalysisParams->iRingRadiusError;
 iRingNr = 0;
 iAlgorithm1RingNr = 0;
 iAlgorithm2RingNr = 0;

 thrReductionFac = pAnalysisParams->fThresholdRed;

 // thrReductionFac = 0.1F;

 //min ring quality ratio of a fake ring produced in the neighborhood of a bright ring
 fakeQualityRatio= pAnalysisParams->fFakeQualRatio;
 // max distance between a bright ring and an induced fake
 maxFakeDistSquared= (Float_t)(iRingRadius*iRingRadius)*4.2F;
 // min centroid shift of an induced fake (if fakeQualityRatio is missed)
 fakeCentroidCut= pAnalysisParams->fFakeCentroidCut;
 // mean amplidude of a pad in a fake ring produced by noise
 fakePadAmplitude = pAnalysisParams->fFakePadAmplitude;;

 if (pAlgorithm1Ring) delete [] pAlgorithm1Ring;
 if (pAlgorithm2Ring) delete [] pAlgorithm2Ring;
 if (pPadsFiredCol) delete [] pPadsFiredCol;
 if (pPadsFiredRow) delete [] pPadsFiredRow;


 if (pAnalysisParams->isActiveRingFindFitMatrix)
  pAlgorithm1Ring = new HRichHit[pAnalysisParams->iHowManyFitMatrixRings];
 else pAlgorithm1Ring = NULL;
 if (pAnalysisParams->isActiveRingHoughTransf) {
  pAlgorithm2Ring = new HRichHit[pAnalysisParams->iHowManyHoughTransfRings];
  pPadsFiredCol = new char[showMe->maxFiredSectorPads];
  pPadsFiredRow = new char[showMe->maxFiredSectorPads];

  iPadPlane.Set(showMe->GetPadNr());
  iPadPlaneShifted.Set(showMe->GetPadNr()+showMe->GetPadsXNr());

 } else {
     pAlgorithm2Ring = NULL;
     pPadsFiredCol = NULL;
     pPadsFiredRow = NULL;
 }
}
//============================================================================

//----------------------------------------------------------------------------
HRichRingFind::~HRichRingFind() {
 if (pAlgorithm1Ring) delete [] pAlgorithm1Ring;
 if (pAlgorithm2Ring) delete [] pAlgorithm2Ring;
 if (pPadsFiredCol) delete [] pPadsFiredCol;
 if (pPadsFiredRow) delete [] pPadsFiredRow;
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
 return sqrt(double((ring1.iRingX-ring2.iRingX)*(ring1.iRingX-ring2.iRingX) + 
             (ring1.iRingY-ring2.iRingY)*(ring1.iRingY-ring2.iRingY)));
}
//============================================================================

//----------------------------------------------------------------------------
Float_t HRichRingFind::CalcDistance(Int_t x, Int_t y, const HRichHit& ring) {
 x -= ring.iRingX;
 y -= ring.iRingY;
 return sqrt(double(x*x + y*y));
}
//============================================================================

//----------------------------------------------------------------------------
Float_t HRichRingFind::CalcDistance(Int_t x1, Int_t y1, Int_t x2, Int_t y2) {
 x1-=x2;
 y1-=y2;
 return sqrt(double(x1*x1 + y1*y1));
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
    //
    // this method looks for rings using the ring pattern matris assigend
    // in the richanalysyspar.txt input file.
    //
Int_t i,j,k,m,n, lx,ly;
Int_t iRingQuality;
Int_t iRingDistance;

if (howmanyrings < 1) howmanyrings = 1;

iAlgorithm1RingNr = 0;

//helper variables
Int_t iRingMatrixHalfSize = iRingMatrixSize / 2;
Int_t minX,maxX,minY,maxY,padOffset;
HRichPadSignal * pad;

//This loop over 'requested' number of rings causes double-counting
//which must be explicitely checked (time consuming).
//There is no need for such a loop. Try to get all candidates in one shot. (not implemented)

Int_t threshold = (Int_t) (0.75F*minampl);
Int_t threshold0 = (Int_t) (0.55F*minampl);
Int_t deltaThreshold = (Int_t) (thrReductionFac*(float)threshold+0.5F);
Int_t maxCols = showMe->GetPadsXNr();
Int_t maxRows = showMe->GetPadsYNr();

for (n = 0; n < howmanyrings; n++) {
 HRichHit & ring = pAlgorithm1Ring[n];
 ring.iRingQuality=threshold0;

 // all the pads belonging to all the found labels ( labeling -> cfr HRichAnalysis::execute() are scanned and the centre of the pattern matrix is overlapped
 // to each of them. The fired pads sorrounding the centre are weighted
 // by the correspondet pattern matrix value, and the weights are accumulated 
 // in iRingQuality.

 for (m = 0; m < showMe->GetLabelNr(); m++) {
  HRichLabel * label = showMe->GetLabel(m);
  Int_t labelSignature = label->iSignature;

  for (j = label->iLowerY; j <= label->iUpperY; j++) {
   minY = j-iRingMatrixHalfSize;
   if(minY<0) minY=0;
   maxY = j + iRingMatrixHalfSize;
   if(maxY>=maxRows) maxY=maxRows-1;
   Int_t matrixOffset = (j-iRingMatrixHalfSize)*iRingMatrixSize - iRingMatrixHalfSize;

   for (i = label->iLeftX; i <= label->iRightX; i++) {

    iRingQuality = 0;
    iRingDistance = 1;

    // this check on the distance betweeb the found rings and the currently
    // analyzed pad avoids a ring to be found twice.
 
    for (k = 0; k < n; k++) {
     if (CalcDistance(i,j,pAlgorithm1Ring[k]) <= distance) {
       iRingDistance = 0;
       break;
     }
    }
   
    if (iRingDistance) {
     minX = i - iRingMatrixHalfSize;
	 if(minX<0) minX=0;
     maxX = i + iRingMatrixHalfSize;
	 if(maxX>=maxCols) maxX=maxCols-1;
     for (ly = minY; ly <= maxY; ++ly) {
      padOffset=ly*maxCols;
      for (lx = minX; lx <= maxX; ++lx) {

       pad = showMe->GetPad(lx+padOffset);
       if (pad->getAmplitude() > 0 && 
           pad->getLabel() == labelSignature)
        iRingQuality += pAnalysisParams->iRingMatrix[lx-i+iRingMatrixSize*ly-matrixOffset];
      }
     }  // end of loop over all pattern matrix pads

     //Find ring with highest quality first.
     //Ignore all other rings with lower quality and search for them again.
     //Very time consuming.
     if (iRingQuality > ring.iRingQuality) {
      ring.iRingX = i;
      ring.iRingY = j;
      ring.iRingQuality = iRingQuality;
      ring.iRingFreeParam = m;
     }
    }
   }
   }  // end of loops over all pads of given label
 } // end of loops over all labels for the first ring

#ifdef HRICH_DEBUGMODE
 cout << "RICH DEBUG MODE: RichFindFitMatrix " << n+1 << ". ring candidate | x = " 
      << ring.iRingX << " | y = " << ring.iRingY 
      << " | quality = " << ring.iRingQuality 
      << " | label nr = " << ring.iRingFreeParam << "\n";
#endif

 if (ring.iRingQuality <= threshold0) return;

 // if a ring lies in the nearhood of the detector edges the minimal
 // ring quality threshold is reduced of maximally deltaThreshold 
 // (richanalysispar.txt)

 Int_t varThreshold=threshold;
 Int_t leftBorder= showMe->pLeftBorder[ring.iRingY];
 Int_t rightBorder= showMe->pRightBorder[ring.iRingY];
 if(ring.iRingX-leftBorder <= iRingRadius)
   varThreshold-=(iRingRadius-ring.iRingX+leftBorder+1)*deltaThreshold;
 else if(rightBorder-ring.iRingX <= iRingRadius)
   varThreshold-=(iRingRadius-rightBorder+ring.iRingX+1)*deltaThreshold;
 if(ring.iRingY <= iRingRadius) {
   Int_t yThr=threshold-(iRingRadius-ring.iRingY+1)*deltaThreshold;
   if(yThr < varThreshold) varThreshold = yThr;
 }

 if (ring.iRingQuality <= varThreshold) return;

// Keep ring even if it fails the test. Otherwise it will be found again.
 iAlgorithm1RingNr++; 
 if (TestRing(showMe,ring,minampl*varThreshold/threshold)) {
  ring.iRingAlgorithmIndex = 1;
 } else  ring.iRingAlgorithmIndex = 0;


#ifdef HRICH_DEBUGMODE
 if (ring.iRingQuality>threshold)
  cout << n+1 << ". ring candidate stored for further processing\n";
 else cout << n+1 << ". ring candidate not accepted\n";
#endif

} // end of loop over all rings sought


return;
} // eof RingFindFitMatrix
//============================================================================

//----------------------------------------------------------------------------
void HRichRingFind::RingFindHoughTransf(HRichAnalysis *showMe, 
                    Int_t minampl, Int_t distance, Int_t howmanyrings) {


Int_t i,j,k,m,n,nrFired;

if (howmanyrings < 1) {
 howmanyrings = 1;
}

iAlgorithm2RingNr = 0;
//for (i = 0; i < howmanyrings; i++) pAlgorithm2Ring[i].Reset();

iPadPlane.Reset();
iPadPlaneShifted.Reset();

//helper variables below could go into init part
Int_t maxCols = showMe->GetPadsXNr();
Int_t ringHalfRadiusSquared =iRingRadius * iRingRadius/4;
Int_t ringMatrixSizeSquared = iRingMatrixSize * iRingMatrixSize;
Float_t ringMaxRadiusSquared =0.5F + iRingRadius + iRingRadiusError;
ringMaxRadiusSquared *= ringMaxRadiusSquared;
Float_t ringMinRadiusSquared =0.5F + iRingRadius - iRingRadiusError;
ringMinRadiusSquared *= ringMinRadiusSquared;

HRichPadSignal* pad;

for (m = 0; m < showMe->GetLabelNr(); m++) {

 HRichLabel * label =  showMe->GetLabel(m);
 Int_t labelSignature = label->iSignature;
 nrFired = label->iFiredPadsNr;

 // storing fired pads in all the labels

 k = 0;
 for (j = label->iLowerY; j <= label->iUpperY; j++) {
  for (i = label->iLeftX; i <= label->iRightX; i++) {
   pad=showMe->GetPad(i,j);
   if (pad->getAmplitude() > 0  && pad->getLabel() == labelSignature) {
    pPadsFiredCol[k]=i;
    pPadsFiredRow[k]=j;
    k++;
    if (k >  nrFired) exit(1);
   }
  }
 }

//temporary helper variables for innermost loop
 Int_t col_k, row_k, deltaCol23, deltaRow23, squaredDifference23, iDiv;

 Float_t distanceSquared;
 Float_t fRingX, fRingY, fRingRSquared, fDiv;
 //
 //  loop on all fired pads inside each label. All the combinations of 
 //  3 pads are tested, the distance of at least 2 couple of pads 
 //  has to greater then ringsradius/2. Furthermore the 3 pads
 //  haven't to be aligned. If these conditions are satisfied
 //  the ring passing through  the 3 pads is calculated, the radius is
 //  tested ( should be 4 pads) and the centre coordiantes are stored.
 // 
 for (i = 0; i < nrFired-2; i++) {
  Int_t col_i= pPadsFiredCol[i];
  Int_t row_i = pPadsFiredRow[i];

  for (j = i+1; j < nrFired-1; j++) {
   Int_t col_j=pPadsFiredCol[j];
   Int_t row_j = pPadsFiredRow[j];
   Int_t deltaCol12=col_i-col_j;
   Int_t deltaRow12=row_i-row_j;

   distanceSquared = deltaCol12*deltaCol12 + deltaRow12*deltaRow12;
   if (distanceSquared > ringHalfRadiusSquared && distanceSquared < ringMatrixSizeSquared) {
     Int_t squaredDifference12 = col_i*col_i - col_j*col_j + row_i*row_i - row_j*row_j;

     for (k = j+1; k < nrFired; k++) {
      col_k = pPadsFiredCol[k];
      row_k = pPadsFiredRow[k];
      deltaCol23 = col_j - col_k;
      deltaRow23 = row_j - row_k;

      distanceSquared = deltaCol23*deltaCol23 + deltaRow23*deltaRow23;
      if (distanceSquared > ringHalfRadiusSquared && distanceSquared < ringMatrixSizeSquared) {

// ................. create  map .........
	 iDiv = deltaCol23*deltaRow12 - deltaCol12*deltaRow23;

// iDiv != 0 is the only relevant test to check gap between rows and cols of the 3 points.
	 if(iDiv > 2 || iDiv < -2) {

          fDiv = float(iDiv);
          squaredDifference23 = col_j*col_j - col_k*col_k + row_j*row_j - row_k*row_k;

          fRingX = 0.5F*float(squaredDifference23*deltaRow12
                            - squaredDifference12*deltaRow23) / fDiv;

          fRingY = 0.5F*float(squaredDifference12*deltaCol23
	                    - squaredDifference23*deltaCol12) / fDiv;

          fRingRSquared = (col_i-fRingX)*(col_i-fRingX) + (row_i-fRingY)*(row_i-fRingY);

          if (fRingRSquared < ringMaxRadiusSquared &&
              fRingRSquared > ringMinRadiusSquared &&
              label->iLeftX <= (Int_t)fRingX && //not necessary
              label->iRightX >= (Int_t)fRingX && //not necessary
              label->iLowerY <= (Int_t)fRingY && //not necessary
              label->iUpperY >= (Int_t)fRingY) { //not necessary
	      // 
	      // iPadPlane contains the centres coordinates,
	      // iPadPlaneShifted the coordinates of the centres shifted
	      // of an half pad unit up and to the right.
	      //
           iPadPlane[(Int_t)fRingX + maxCols*(Int_t)fRingY] += 1;
           iPadPlaneShifted[(Int_t)(fRingX+0.5F) + maxCols*(Int_t)(fRingY+0.5F)] += 1;
		  }
         }

// eof create map ...............

       }
//      }
     }
    }
//   }
  }
 }

}  // end of loop over all labels

// ..................................... find MAX ..............
// return findMax(showMe,minAmpl,areaMax);

Int_t threshold0 = (Int_t) (0.40F*minampl);
Int_t threshold = (Int_t) (0.75F*minampl);
Int_t deltaThreshold = (Int_t) (thrReductionFac*(float)threshold*2.0F + 0.5F);

Int_t iRingDistance;

//
// again a loop on all the pads of a label to look for maxima in the iPadPlane 
// array.
//

for (n = 0; n < howmanyrings; n++) {
 HRichHit & ring = pAlgorithm2Ring[n];
 ring.iRingQuality=threshold0;
 for (m = 0; m < showMe->GetLabelNr(); m++) {
  HRichLabel * label = showMe->GetLabel(m);

  for (j = label->iLowerY; j <= label->iUpperY; j++)
   for (i = label->iLeftX; i <= label->iRightX; i++) {

    iRingDistance = 1;

    for (k = 0; k < n; k++) {
	//**
     if (CalcDistance(i,j,pAlgorithm2Ring[k]) <= distance) {
       iRingDistance = 0;
       break;
     }
    }

    if (iRingDistance) {
     if (iPadPlane[i + maxCols*j] > ring.iRingQuality) {

	 // if the currently analyzed pad corresponds to ring centre 
	 // where enough hits were accumulated, the ring is accepted
	 // as a valid candidate.
	 // The previous condition selects the highest of the rings centres.
	 // The condition (**) avoids the same ring to be found twice. 

      ring.iRingX = i;
      ring.iRingY = j;
      ring.iRingQuality = iPadPlane[i + maxCols*j];
      ring.iRingFreeParam = m;
     }
    }

   }  // end of loops over all pads of given label
 } // end of loops over all labels for the first ring

#ifdef HRICH_DEBUGMODE
 cout << "RICH DEBUG MODE: RingFindHoughTransf " << n+1 << ". ring candidate | x = "
      << ring.iRingX << " | y = " << ring.iRingY
      << " | quality = " << ring.iRingQuality
      << " | label nr = " << ring.iRingFreeParam << "\n";
#endif

 if (ring.iRingQuality <= threshold0) return;
 for (j=0;j<2;++j) {
   int offset =ring.iRingX + maxCols*(ring.iRingY+j);
   for (i=0;i<2;++i) {
	 if(iPadPlaneShifted[i + offset] > ring.iRingQuality) ring.iRingQuality = iPadPlaneShifted[i + offset];
   }
 }
 Int_t varThreshold=threshold;
 Int_t leftBorder= showMe->pLeftBorder[ring.iRingY];
 Int_t rightBorder= showMe->pRightBorder[ring.iRingY];
 //
 // if the distance of the ring centre from the detector edge is smaller
 // then the ring radius ( ring image not complete!) the quality 
 // threshold is reduced of a factor proportional to the distance.
 // The maximal reduction factor si given by the parameter  deltaThreshold.
 // (see ringanalysispar.txt)
 //  
 if(ring.iRingX-leftBorder <= iRingRadius)
   varThreshold-=(iRingRadius-ring.iRingX + leftBorder+1)*deltaThreshold;
 else if(rightBorder-ring.iRingX <= iRingRadius)
   varThreshold-=(iRingRadius-rightBorder+ring.iRingX+1)*deltaThreshold;
 if(ring.iRingY <= iRingRadius) {
   Int_t yThr = threshold-(iRingRadius-ring.iRingY+1)*deltaThreshold;
   if(yThr < varThreshold) varThreshold = yThr;
 }
 if (ring.iRingQuality <= varThreshold) return;

// Keep ring even if it fails the test. Otherwise it will be found again.
 iAlgorithm2RingNr++;
 if (TestRing(showMe,ring,minampl*varThreshold/threshold)) {
  ring.iRingAlgorithmIndex = 2;
 } else ring.iRingAlgorithmIndex = 0;

#ifdef HRICH_DEBUGMODE
 if (ring.iRingQuality>threshold)
  cout << n+1 << ". ring candidate stored for further processing\n";
 else cout << n+1 << ". ring candidate not accepted\n";
#endif

} // end of loop over all rings sought


return;
// ...... eof Find MAX .....................
} // eof RingFindHoughTransf
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichRingFind::TestRing(HRichAnalysis *showYou, HRichHit& ring, Int_t amplit) {

// if (0.75F*amplit > ring.iRingQuality) return 0; // already checked

 Int_t i, j, k, m, n;
 Int_t iLabelNr = 0, iActivePads = 0, iSurface = 0, 
       iActiveSurface = 0, iMatrixSurface = 0,
       iInRing = 0, iOnRing = 0, iOutRing = 0,
       iAllRing = 0, iDynamicAmplit = 0;

 iLabelNr = ring.iRingFreeParam;

 HRichLabel * label = showYou->GetLabel(iLabelNr);

 iActivePads = label->iFiredPadsNr;
 iSurface = (label->iUpperY - label->iLowerY + 1)*
            (label->iRightX - label->iLeftX + 1);
 iActiveSurface = label->iLabeledPadsNr;
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

  if (fSurfRatio <= 1.34F && fDensRatio <= 1.34F*pAnalysisParams->fFormulaParam3) {
   iDynamicAmplit = (Int_t)(amplit * pAnalysisParams->fLowerAmplFactor);
  } else {
   iDynamicAmplit = (Int_t)(amplit * exp(pAnalysisParams->fFormulaParam1 * (fSurfRatio - 1.0F) + 
                    pAnalysisParams->fFormulaParam2 * (fDensRatio/pAnalysisParams->fFormulaParam3 - 1.0F)));
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
 Int_t iRingMaskHalfSize = pAnalysisParams->iRingMaskSize/2;
 for (k = 0; k < iMatrixSurface; k++) {
  m = (k % pAnalysisParams->iRingMaskSize) - iRingMaskHalfSize;
  n = (k / pAnalysisParams->iRingMaskSize) - iRingMaskHalfSize;
  if (!showYou->IsOut(ring.iRingX+m,ring.iRingY+n) &&
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
       << 100.0F*iOnRing/iAllRing << "% | outside = "
       << 100.0F*(iOutRing+iInRing)/iAllRing << "%) ";
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


Int_t iHalfRingMatrix = iRingMatrixSize/2;
Int_t  iPosX = 0, iPosY = 0, iHowManyPads = 0, iErrFlag = 0;
Float_t fXpos, fYpos;
Int_t dX, dY, row;

for (j = 0; j < iRingMatrixSize; j++) {
 dY = j-iHalfRingMatrix;
 row = ring.iRingY+dY;
 for (i = 0; i < iRingMatrixSize; i++) {
  dX = i-iHalfRingMatrix;
  if (!showYou->IsOut(ring.iRingX+dX, row)) {
   if (showYou->GetPad(ring.iRingX+dX, row)->getAmplitude() > 0) {
    iPosX += dX;
    iPosY += dY;
    iHowManyPads++;
   }
  }
 }
}
fXpos = (Float_t)iPosX/(Float_t)iHowManyPads;
fYpos = (Float_t)iPosY/(Float_t)iHowManyPads;

ring.centroid = (Float_t)(sqrt((double)(fXpos*fXpos + fYpos*fYpos)));
ring.iCentX = ring.iRingX+(Int_t)fXpos;
ring.iCentY = ring.iRingY+(Int_t)fYpos;
//cout<<" centroid X "<<ring.iCentX<<" centroid y  "<<ring.iCentY<<endl;
// cout<<" ringX "<<ring.iRingX<<" ringY  "<<ring.iRingY<<endl;
//check for unphysical centroids
if(ring.centroid > pAnalysisParams->fRingRadius) {
  ring.radius = 0.0F;
  return 0;
}

#ifdef HRICH_DEBUGMODE
 cout << "Assymetry test... (middle shift = " << iHowManyPads;
#endif

if (pAnalysisParams->isActiveCentroidCheck) {
 iHowManyPads = (Int_t)ring.centroid;
 if(iHowManyPads > iRingRadiusError) {
  #ifdef HRICH_DEBUGMODE
   cout << ")\n";
  #endif
  ring.radius=0.0F;
  return 0;
 } else {
  if (iHowManyPads > 0) iErrFlag++;
  #ifdef HRICH_DEBUGMODE
   cout << " |";
  #endif
 }
}

iHowManyPads = 0;
Float_t fRadCalc = 0.0F;
Int_t iRadSquared;

for (j = 0; j < iRingMatrixSize; j++) {
 dY = j-iHalfRingMatrix;
 for (i = 0; i < iRingMatrixSize; i++) {
  dX = i-iHalfRingMatrix;
  if (!showYou->IsOut(ring.iRingX+dX,ring.iRingY+dY)) {
   if (showYou->GetPad(ring.iRingX+dX, ring.iRingY+dY)->getAmplitude() > 0) {
//radius calculation modified. Old formula looks wrong to me. W. Koenig
// old: fRadCalc += sqrt((fXpos+dX)*(fXpos+dX) + (fYpos+dY)*(fYpos+dY));
// The old formula tried to use the calculated centroid as ring-center but signs were wrong.
// Area available for fired pads at given radius increases proportional to radius.
// Thus mean value is obtained by sum(radii/weight) / sum(weights) with weight=radius
// If radius is 0 (central pad) weight is set to 1. Next best to nothing approach.
	 iRadSquared=dX*dX+dY*dY;
	 if(iRadSquared==0) iRadSquared=1;
	 else iHowManyPads++;
	 fRadCalc+=1.0F/(Float_t)sqrt(double(iRadSquared));
   }
  }
 }
}
ring.radius=(Float_t)iHowManyPads/fRadCalc;
#ifdef HRICH_DEBUGMODE
 cout << " radius = " << ring.radius << ") ";
#endif

if (pAnalysisParams->isActiveRadiusCheck) {
if(TMath::Abs(ring.radius - iRingRadius) > pAnalysisParams->fMeanRadiusError ) {

 #ifdef HRICH_DEBUGMODE
  cout << "\n";
 #endif
 return 0;
}

if (abs((Int_t)ring.radius - iRingRadius) > 0) iErrFlag++;
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
void HRichRingFind::CalcRingParameters(HRichAnalysis *showMe,HRichHit *pRings) {

    //
    // this method calculates the number of local maxima for each ring
    // looking both at 4 neighbours (iPhot4Nr) and at 8 neighbours (iPhot8Nr)
    // The number of local maxima in a ring correponds to the number
    // of Cherenkov Photons.
    // Furthermore the average size of the clusters the ring is composed of
    // is calculated.
    //

 Int_t i,j,k,l,n,
       iCount, iIsPhot4, iIsPhot8, iPhot4Nr=0, iPhot8Nr=0;
 Int_t iNowX, iNowY;

 Int_t ringSize = iRingImageSize;
 Int_t ringHalfSize = ringSize/2;
 Int_t maxCols = showMe->GetPadsXNr();

 
 for (n = 0; n < iRingNr; n++) {
  iRingTempImage.Reset();
  HRichHit & ring=pRings[n];
  ring.iRingAmplitude = 0;
  ring.iRingPadNr = 0;
  iNowX = ring.iRingX;
  iNowY = ring.iRingY;
  for (j = 0; j < ringSize; j++) {
   Int_t row = iNowY+j-ringHalfSize;
   for (i = 0; i < ringSize; i++) {
    if (!showMe->IsOut(iNowX+i-ringHalfSize, row)) {
     ring.iRingImage[i + ringSize*j] =
      showMe->GetPad(iNowX+i-ringHalfSize, row)->getAmplitude();
    } else ring.iRingImage[i + ringSize*j] = 0;
   }
  }

  iPhot4Nr = iPhot8Nr = 0;
  for (j = 0; j < ringSize; j++) {
   Int_t row = iNowY+j-ringHalfSize;
   Int_t offset= iNowX-ringHalfSize + maxCols*row;
   for (i = 0; i < ringSize; i++) {
    Int_t col = iNowX+i-ringHalfSize;
    if (!showMe->IsOut(col,row)) {
     iIsPhot4 = iIsPhot8 = 0;
     Int_t amplitude = showMe->GetPad(i+offset)->getAmplitude();
     if (amplitude > 0 && 
         pAnalysisParams->iRingMask[i + ringSize*j] == 1) {
      ring.iRingPadNr++;
      ring.iRingAmplitude += amplitude;
//    cout << "- pad amplitude - "<<col <<", "<< row <<"="<< amplitude << endl;
      for (k = -1; k < 2; k++)
       for (l = -1; l < 2; l++)
        if (((l == 0 && abs(k)) || (k == 0 && abs(l))) && !(l == 0 && k == 0) &&
          !showMe->IsOut(col+l,row+k) &&
           showMe->GetPad(col+l,row+k)->getAmplitude() >= amplitude) {
         iIsPhot4++;
        }
      if (iIsPhot4 == 0) {
       iPhot4Nr++;
       iRingTempImage[i + ringSize*j] += 1;
      }
      for (k = -1; k < 2; k++)
       for (l = -1; l < 2; l++)
        if (abs(l) && abs(k) && !showMe->IsOut(col+l,row+k) && 
            showMe->GetPad(col+l,row+k)->getAmplitude() >= amplitude) {
         iIsPhot8++;
        }
      if (iIsPhot4 == 0 && iIsPhot8 == 0) {
       iPhot8Nr++;
       iRingTempImage[i + ringSize*j] += 1;
      }
     }
    }
   }
  }

 ring.iRingLocalMax4 = iPhot4Nr;
 ring.iRingLocalMax8 = iPhot8Nr;


 iCount = 0;
 iClusterSize=0;
 iClusterLMax4=0;
 iClusterLMax8=0;
 for (j = 0; j < iRingImageSize; j++)
  for (i = 0; i < iRingImageSize; i++)
   if (ring.iRingImage[i + iRingImageSize*j] > 0) {
    iInnerCount = iInnerPhot4 = iInnerPhot8 = 0;
    CalcRingClusters(showMe, &iRingTempImage[0], ring, i, j);
    if (iInnerCount) {
     iClusterSize += iInnerCount;
     iClusterLMax4 += iInnerPhot4;
     iClusterLMax8 += iInnerPhot8;
     iCount++;
    }
   }
 ring.iRingClusterNr = iCount;
 if(iCount>0) {
  ring.iRingClusterSize = (Float_t)iClusterSize/(Float_t)iCount;
  ring.iRingClusterLMax4 = (Float_t)iClusterLMax4/(Float_t)iCount;
  ring.iRingClusterLMax8 = (Float_t)iClusterLMax8/(Float_t)iCount; 
 } 

#ifdef HRICH_DEBUGMODE
 cout << "RICH DEBUG MODE: " << iRingNr << " ring(s) found:\n";
 cout << "Ring nr " << n+1 << " fired = " << ring.iRingPadNr 
      << " | x = " << ring.iRingX
      << " | y = " << ring.iRingY << " | quality = " << ring.iRingQuality
      << " | amplitude = " << ring.iRingAmplitude 
      << " | local max 4 = " << ring.iRingLocalMax4 
      << " | local max 8 = " << ring.iRingLocalMax8 << "\n";
 cout << " free parameter = " << ring.iRingFreeParam << "\n";
 cout << "Clusters data (number of pads, local max 4, local max 8):\n";
 
 cout << "(" << ring.iRingClusterSize
       << ", " << ring.iRingClusterLMax4
       << ", " << ring.iRingClusterLMax8 << ")\n"; 
#endif

 } // end of loop over all rings

} // eof CalcRingParameters
//============================================================================

//----------------------------------------------------------------------------
void HRichRingFind::deleteFakeRings(HRichHit * pRingArray) {

    //
    // This method tests the average pad amplitude for a ring candidate. 
    // If the average amplitude is under threshold ( fakePadAmplitude; cfr.
    // ringanalysispar.txt) the ring is deleted.
    //

    //cout<<" ring numer "<<iRingNr<<endl;
 for (Int_t n = 0; n < iRingNr; ++n) {
   HRichHit & ring=pRingArray[n];
   if(ring.iRingAmplitude < (Int_t)(fakePadAmplitude*(Float_t)ring.iRingPadNr) + 5 || ring.iRingAmplitude/ring.iRingPadNr > 35) {
     --iRingNr;
     //  cout<<" ring number after check on amplitude "<<iRingNr<<endl;
     for(Int_t k = n; k<iRingNr;++k) {
	 // cout<<" copying Ring nr :  "<<k<<endl;

	 pRingArray[k]=pRingArray[k+1];
     }
   }
 }
}
//============================================================================


//----------------------------------------------------------------------------
void HRichRingFind::CalcRingClusters(HRichAnalysis *showYou, Short_t *dumpArr, 
                                    HRichHit & ring, Int_t nowX, Int_t nowY) {
    //
    // this method appends to the  fired pads all the fired neigbours
    // to compose a cluster.
    // 
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
 for (b = 0; b < 3; b++) {
   if(nowY+b-1 >= 0 && nowY+b-1 < iRingImageSize) {
    Int_t offset=nowX-1+iRingImageSize*(nowY+b-1);
    for (a = 0; a < 3; a++)
     if (nowX+a-1 >= 0 && nowX+a-1 < iRingImageSize) 
      if (!(a == 1 && b == 1))
       if (ring.iRingImage[a + offset] > 0 && dumpArr[a + offset] != 3) {
          if (dumpArr[a + offset] == 1) iInnerPhot4++;
          else if (dumpArr[a + offset] == 2) { 
           iInnerPhot4++;
           iInnerPhot8++; 
          }
          if (pAnalysisParams->iRingMask[a + offset] == 1) iInnerCount++;
          dumpArr[a + offset] = 3;
          iTempMatrix[a][b] = 1;

#ifdef HRICH_DEBUGMODE0
 cout << "RICH DEBUG MODE: cluster in ring (pads fired = " << iInnerCount 
      << ", local max 4 = " << iInnerPhot4
      << ", local max 8 = " << iInnerPhot8 << ")\n";
#endif
       }
   }
 }


 for (b = 0; b < 3; b++)
  for (a = 0; a < 3; a++)
   if (iTempMatrix[a][b] > 0) CalcRingClusters(showYou,dumpArr,ring,nowX+a-1,nowY+b-1);
} // eof CalcRingClusters
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichRingFind::GetAlgorithmNr() {
 return (pAnalysisParams->isActiveRingFindFitMatrix + pAnalysisParams->isActiveRingHoughTransf);
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichRingFind::Execute(HRichAnalysis *giveMe) {

if (giveMe->GetLabelNr()==0) return 0;


if (pAnalysisParams->isActiveRingHoughTransf) {
  RingFindHoughTransf(giveMe,pAnalysisParams->iMinimalHoughTransfRingQuality,
    pAnalysisParams->iMinimalHoughTransfRingDistance,pAnalysisParams->iHowManyHoughTransfRings);
  if (iAlgorithm2RingNr==0 && pAnalysisParams->isActiveRingFindFitMatrix
    && pAnalysisParams->iSuperiorAlgorithmID < 3) return 0;
  rejectFakes(iAlgorithm2RingNr, pAlgorithm2Ring);
} 

if (pAnalysisParams->isActiveRingFindFitMatrix) {
  RingFindFitMatrix(giveMe,pAnalysisParams->iMinimalFitMatrixRingQuality,
    pAnalysisParams->iMinimalFitMatrixRingDistance,pAnalysisParams->iHowManyFitMatrixRings);
  rejectFakes(iAlgorithm1RingNr, pAlgorithm1Ring);
}

iRingNr = 0;

// --- all rings found by all algorithms will be stored ---
if (GetAlgorithmNr() == 2) {
 if(pAnalysisParams->iSuperiorAlgorithmID == 3) {
   storeRings(pRings,iAlgorithm1RingNr,pAlgorithm1Ring,1);
   storeRings(pRings,iAlgorithm2RingNr,pAlgorithm2Ring,2);
 } else {

// --- only rings found by both algorithms ---
 Int_t i, j, iChosen;

 #ifdef HRICH_DEBUGMODE
  cout << "Rings found by both algorithms matching... (maximum " 
       << ((iAlgorithm1RingNr > iAlgorithm2RingNr) ? iAlgorithm1RingNr : iAlgorithm2RingNr) << " candidates).\n";
 #endif

 for (i = 0; i < iAlgorithm1RingNr; ++i) {
  if (pAlgorithm1Ring[i].iRingAlgorithmIndex > 0) { //not set, if ring was rejected
   for (j = 0; j < iAlgorithm2RingNr; ++j) {
    iChosen = 0;
    if (pAlgorithm2Ring[j].iRingAlgorithmIndex > 0) { //not set, if ring was rejected
     if (CalcDistance(pAlgorithm1Ring[i],pAlgorithm2Ring[j]) <= iRingRadius/2) {
      HRichHit & ring = pRings[iRingNr];
      if (pAnalysisParams->iSuperiorAlgorithmID == 0) {
       ring.iRingX = (pAlgorithm1Ring[i].iRingX + pAlgorithm2Ring[j].iRingX)/2;
       ring.iRingY = (pAlgorithm1Ring[i].iRingY + pAlgorithm2Ring[j].iRingY)/2;
       ring.iRingQuality = (pAlgorithm1Ring[i].iRingQuality + pAlgorithm2Ring[j].iRingQuality)/2;
       ring.centroid = (pAlgorithm1Ring[i].centroid + pAlgorithm2Ring[j].centroid)/2.0F;
       ring.iCentX = (pAlgorithm1Ring[i].iCentX + pAlgorithm2Ring[j].iCentX)/2.0F;
      ring.iCentY = (pAlgorithm1Ring[i].iCentY + pAlgorithm2Ring[j].iCentY)/2.0F;
       ring.radius = (pAlgorithm1Ring[i].radius + pAlgorithm2Ring[j].radius)/2.0F;
       ring.iRingFreeParam = pAlgorithm1Ring[i].iRingFreeParam; //no sense to average
      } else {
       HRichHit * algorithmRing;
       if (pAnalysisParams->iSuperiorAlgorithmID == 1)
            algorithmRing=pAlgorithm1Ring + i;
       else algorithmRing=pAlgorithm2Ring + j;

       ring.iRingX = algorithmRing->iRingX;
       ring.iRingY = algorithmRing->iRingY;
       ring.iRingQuality = algorithmRing->iRingQuality;
       ring.centroid = algorithmRing->centroid;
       ring.iCentX= algorithmRing->iCentX;
       ring.iCentY= algorithmRing->iCentY;
      
       ring.radius = algorithmRing->radius;
       ring.iRingFreeParam = algorithmRing->iRingFreeParam;
      }
      ring.iRingHouTra = pAlgorithm2Ring[j].iRingQuality;
      ring.iRingPatMat = pAlgorithm1Ring[i].iRingQuality;
      ring.iRingAlgorithmIndex = 3;
      ++iRingNr;
      iChosen = 1;
     }
    }
    if (iChosen) break;
   }
  }
 }

 #ifdef HRICH_DEBUGMODE
  cout << "Number of rings matched: " << iRingNr << endl;
 #endif
 }
 if(iRingNr>0) {
	 CalcRingParameters(giveMe,pRings);
	 deleteFakeRings(pRings);
 }
 return (iRingNr);
}

// --- rings found by selected algorithm ---
if (GetAlgorithmNr() == 1) {
 if (pAnalysisParams->isActiveRingFindFitMatrix) storeRings(pRings,iAlgorithm1RingNr,pAlgorithm1Ring,1);
 else if (pAnalysisParams->isActiveRingHoughTransf) storeRings(pRings,iAlgorithm2RingNr,pAlgorithm2Ring,2);

 if(iRingNr>0) {
	 CalcRingParameters(giveMe,pRings);
	 deleteFakeRings(pRings);
 }
 return (iRingNr);
}

return (0); // this should never happen
} // eof

void HRichRingFind::storeRings(HRichHit * ringArray, Int_t ringCandidateNr, HRichHit * pAlgorithmRing, Int_t algorithmIndex) {
  for (int i = 0; i < ringCandidateNr; ++i) {
   if(pAlgorithmRing[i].iRingAlgorithmIndex) { //not set, if ring was rejected
     HRichHit & ring = ringArray[iRingNr];
     ring.iRingX = pAlgorithmRing[i].iRingX;
     ring.iRingY = pAlgorithmRing[i].iRingY;
	 ring.iRingQuality = pAlgorithmRing[i].iRingQuality;
	 if(algorithmIndex==1) {
       ring.iRingPatMat = ring.iRingQuality;
       ring.iRingHouTra = 0;
	 } else {
       ring.iRingHouTra = ring.iRingQuality;
       ring.iRingPatMat = 0;
	 }
     ring.iRingAlgorithmIndex = algorithmIndex;
     ring.centroid = pAlgorithmRing[i].centroid;
     ring.iCentX = pAlgorithmRing[i].iCentX;
     ring.iCentY = pAlgorithmRing[i].iCentY;
     ring.radius = pAlgorithmRing[i].radius;
     ring.iRingFreeParam = pAlgorithmRing[i].iRingFreeParam;
     ++iRingNr; // real ring, not a rejected fake
   }
  }
}

//============================================================================

//----------------------------------------------------------------------------
void HRichRingFind::rejectFakes(Int_t nRings, HRichHit * pRingArray) {
    //
    // this method rejects fakes produced by picking up intensity 
    // from neighboured ring.
    // If 2 rings closed to each other are found the quality factor
    // of the 2 candidates are compared, and they differ too much
    // (crf. fakeQualityRatio)
    // the worse ring is deleted.
    // 
  for (int i = 0; i < nRings-1; ++i) {
   HRichHit & ring1 = pRingArray[i];
   if(ring1.iRingAlgorithmIndex) { //not set, if ring was rejected
     for (int k = i+1; k < nRings; ++k) {
       HRichHit & ring2 = pRingArray[k];
       if(ring2.iRingAlgorithmIndex) { //not set, if ring was rejected
         Int_t dx=ring1.iRingX-ring2.iRingX;
         Int_t dy=ring1.iRingY-ring2.iRingY;
         Float_t distSquared=dx*dx+dy*dy;
         if(distSquared<maxFakeDistSquared) {
           Float_t dQ =(Float_t)(ring1.iRingQuality-ring2.iRingQuality)
                      /(Float_t)(ring1.iRingQuality+ring2.iRingQuality);
           if(dQ>fakeQualityRatio) ring2.iRingAlgorithmIndex=0; //ring rejected
           else if(dQ<-fakeQualityRatio) ring1.iRingAlgorithmIndex=0;
	   else {
             if(dQ > 0.15 && ring2.centroid>=fakeCentroidCut)
	       ring2.iRingAlgorithmIndex=0; //ring rejected
             else if(dQ < -0.15 && ring1.centroid>=fakeCentroidCut)
	       ring1.iRingAlgorithmIndex=0;
	   }
         }
       }
     }
   }
  }
}







