// File: hrichringfind.cc
// ***************************************************************************

//*-- Author : Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
//*-- Modified : 2000/10/xx by Wolfgang Koenig 
//*-- Modified : 2001/01/xx by Laura Fabbietti
//*-- Modified : 2001/09/06 by Witold Przygoda

using namespace std;
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <iomanip>
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
// this Class isnt a task but it is called every event in the execute fn. of hrichanalysis

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
Float_t HRichRingFind::CalcDistance(Int_t x, Int_t y, const HRichHit& ring) {
 Int_t dx,dy;
 dx = x-ring.iRingX;
 dy = y-ring.iRingY;
 return sqrt((Float_t)(dx*dx + dy*dy));
}
//============================================================================

//----------------------------------------------------------------------------
Float_t HRichRingFind::CalcDistance(Int_t x1, Int_t y1, Int_t x2, Int_t y2) {
 Int_t dx,dy;
 dx = x1-x2;
 dy = y1-y2;
 return sqrt((Float_t)(dx*dx + dy*dy));
}
//============================================================================
//----------------------------------------------------------------------------
Float_t HRichRingFind::CalcDistance(const HRichHit& ring1, const HRichHit& ring2) {
 return sqrt((Float_t)(ring1.iRingX-ring2.iRingX)*(ring1.iRingX-ring2.iRingX) + 
             (ring1.iRingY-ring2.iRingY)*(ring1.iRingY-ring2.iRingY));
}
//============================================================================

//----------------------------------------------------------------------------
Double_t HRichRingFind::HomogenDistr(Double_t left, Double_t right) {
 return rand() / (double)RAND_MAX * (right - left) + left;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichRingFind::GetAlgorithmNr(HRichAnalysis *showMe) {
 return (pAnalysisParams->isActiveRingFindFitMatrix + 
         pAnalysisParams->isActiveRingHoughTransf);
}//============================================================================

//----------------------------------------------------------------------------

Int_t HRichRingFind::Execute(HRichAnalysis *giveMe) {
    
    
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
    // the ring candidates found by the pattern matrix are stored in the list fHitList1
    
    if (pAnalysisParams->isActiveRingHoughTransf)
	RingFindHoughTransf(giveMe,pAnalysisParams->iMinimalHoughTransfRingQuality,
			    pAnalysisParams->iMinimalHoughTransfRingDistance,
			    pAnalysisParams->iHowManyHoughTransfRings);
    // the ring candidates found by the hough trasfomation are stored in the list fHitList2
    
    
    CloseMaxRejection(&fHitList1);
    CloseMaxRejection(&fHitList2);
    //the two list of candidates are merged according to the analysis parameter (see comment in MatchRings)
    return MatchRings(giveMe,&fHitList1,&fHitList2);
    
} // eof
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
    
    // loop over all the labels ( the labeling procedure is executed 
    // in the hrichanalysis execute fn.)

    for (m = 0; m < iLabelNr; m++) {
	pLabel = showMe->GetLabel(m);
	//loop over all pads belonging to a given label
	for (j = pLabel->iLowerY; j <= pLabel->iUpperY; j++) {
	    ly_from = ((j-iMatrixHalfSize < 0) ? 0 : j-iMatrixHalfSize);
	    ly_to   = ((j+iMatrixHalfSize >= maxRows) ? maxRows-1 : j+iMatrixHalfSize);
	    
	    for (i = pLabel->iLeftX; i <= pLabel->iRightX; i++) {
		lx_from = ((i-iMatrixHalfSize < 0) ? 0 : i-iMatrixHalfSize);
		lx_to   = ((i+iMatrixHalfSize >= maxCols) ? maxCols-1 : i+iMatrixHalfSize);
		
		iRingQuality = 0;
		
		// the center of the pattern matrix is overlapped to each pad.
		// If the pads in the pattern matrix range belong to the label
		// the corresponding patter matrix quality is summed up
		// to the total quality of this ring candidate.
		
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
    
    //loop over alla the labels
    
    for (m = 0; m < showMe->GetLabelNr(); m++) {
	
	pLabel = showMe->GetLabel(m);
	nrFired = pLabel->iFiredPadsNr;
	
	k = 0;
	//loop over all the label pads
	for (j = pLabel->iLowerY; j <= pLabel->iUpperY; j++)
	    for (i = pLabel->iLeftX; i <= pLabel->iRightX; i++) {
		pPad = showMe->GetPad(i,j);
		if (pPad->getAmplitude() > 0  && pPad->getLabel() == pLabel->iSignature) {
		    // the position of the fired pads in the label are stored in the arrays
		    // iPadCol[i] iPadROw[i]
		    iPadCol[k] = i;
		    iPadRow[k] = j;
		    k++;
		    if (k >  nrFired) exit(1); 
		}
	    }
	
	// 3 loops to obtain all the combinations of three pads.
	// two pads in the triplet must have a distance bigger than
	// half a ring radius and lower than the matrix dimension (diameter)
	
	for (i = 0; i < nrFired-2; i++)
	    for (j = i+1; j < nrFired-1; j++) {
		
		d_col_ij = iPadCol[i]-iPadCol[j];
		d_row_ij = iPadRow[i]-iPadRow[j];
		fDistance = sqrt((Float_t)(d_col_ij*d_col_ij + d_row_ij*d_row_ij));
		
		if (fDistance > pAnalysisParams->iRingRadius/2 && 
		    fDistance < pAnalysisParams->iRingMatrixSize)
		    
		    for (k = j+1; k < nrFired; k++) {
			
			d_col_jk = iPadCol[j]-iPadCol[k];
			d_row_jk = iPadRow[j]-iPadRow[k];
			fDistance = sqrt((Float_t)(d_col_jk*d_col_jk + d_row_jk*d_row_jk));
			
			if (fDistance > pAnalysisParams->iRingRadius/2 && 
			    fDistance < pAnalysisParams->iRingMatrixSize) {
			    
			    // ................. create  map .........
			    // for each combination of three pads the center of the ring
			    // that goes trough the pads is calculated
			    
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
				
				// this array contains all the centers of the rings each with its 
				// weight ( how many time this pad did correspond to a ring center 
				// for a given three  pads combination.
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
void HRichRingFind::MaxFinding(HRichAnalysis *showYou, TList *hitList,
                               TArrayI *in, TArrayI *out, Int_t ringnr, Float_t distance) {
    //
    // This function looks for local maxima among 8 neighbouring pads in 
    // the "in"  array (iPadPlabe) "and fills  the output "out" array. 
    // The iPadPlane contains the pads that correpond to the ring center
    // each with its specific weight. Among them the Local Maxima is sought.
    // The "out" array is created here in such a way that the amplitude 
    // of a local maximum is copied from input "in" array (iPdaPlane), 
    // the pads that are not local maxima are marked with '-1'.
    // The pads that havent been fired are marked with '0'. 
    // The parameters of local maxima are first put to the list fHitCandidate
    // of HRichHitCandidate objects. The list is sorted by descending 
    // ring quality and only the rings that are at a minimal distance 
    // of "distance" from each other are stored in a "hitList".
    //
 
    Int_t i,j,k,l,m,n,pad,padnear;
    Int_t x1,y1,x2,y2;
    Int_t iHitCount = 0;
    Bool_t fMax = kTRUE;
    Int_t iHit = 0;
    HRichLabel *pLabel = NULL;
    

    fHitCandidate.Delete();
    // loop over all labels
    for (m = 0; m < showYou->GetLabelNr(); m++) {
	pLabel = showYou->GetLabel(m);
	// loop over all pads in a given label.
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
		    
		    // for each pad the weight in the "in" array is compared 
		    // with the weights on the 8 neighbouring and local maxima
		    // are defined
		    
		    for (k = ly_from; k <= ly_to; k++)
       for (l = lx_from; l <= lx_to; l++) {
	   padnear = l + maxCols*k;
	   if (iPadActive[padnear] && !(l==i && k==j))
	       if ((*in)[padnear] > (*in)[pad])
		   fMax = kFALSE;
       }
		    
		    if (fMax) {
			// for pad that is a L.M. the weight contained
			// in the "in" array  is assigned to the "out" array
			// and the l.m. is added to the temporary list (fHitCandidate).
			(*out)[pad] = (*in)[pad];
			fHitCandidate.Add(new HRichHitCandidate(i,j,(*in)[pad],m,++iHitCount));
      } else (*out)[pad] = -1;
		    
		}
	    }
	}  // end of loops over all pads of given label
    } // end of loops over all labels
    
 // now selection of ringnr of the highest maxima to be processed
    // checking the distance between candidates
    
    // The l. m. are sorteb by descending quality and the distance
    // between the different candidate is checked. It has to be
    // higher than the threshold.

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
    
    // the selected l.m. are added to the list hitList    
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
void HRichRingFind::MaxSelector(HRichAnalysis *showMe, TList *hitList,
                                TArrayI *in, TArrayI *out) {
    // 
    // This function is used to label the clusters of the local maxima.
    // The pads corresponding to a L.M. were labeled in the MaxFinding
    // function and their values stored in "out" array. 
    // A label is assigned to each L.M. (hit) belonging to the list "hitList"
    // (this list has been filled at the end of the member function
    // MaxFinding)
    // and this label must be now propagated to all pads belonging 
    // to the L.M cluster. The condition to be fullfilled in order to belong
    // to a cluster is that the pads are reachable from the maximum (L.M.)
    // in a descending monotonous way. If a pad belongs to more than
    // one clusters it is marked with '-2'. At the end all the connected fired pads
    // of a local maximum are marked with the label of this maximum (even
    // if they are '-2'. In case of a conflict (possible only if two
    // maxima are separated by one pad) the label of maximum with 
    // higher amplitude is assigned to the pads.
    // The propagation of a label is done in a similar way as in the case
    // of cleaning algorithm (high amplitude clusters).
    //
    
    Int_t i,j,k,l,m,pad,padnear;
    Int_t fMaxCode;
    HRichHit *pHit = NULL;
    //loop over all the hits    
    for (m = 0; m < hitList->GetSize(); m++) {
	pHit = (HRichHit*)(hitList->At(m));
	pad = pHit->iRingX + maxCols*pHit->iRingY;
	fMaxCode = pHit->iRingMaxLabel;
	// the following function labels the pads belonging
	// to the cluster of each l.m..
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
// given label "maxCode" of a l.m. to all pads beloning to the l.m cluster.
// All these pads are marked with '-1' in the "out" array.
// If the pad marked earlier by a label from another maximum is reached
// it is marked as a common pad with '-2'.
// The label is propagated as long as the value of the weight of the
// concatenated pads is monotonous.

Int_t i,j,k,l, padnear, x_from, x_to, y_from, y_to;
TArrayI iTempMatrix(9);

 i = nowPad % maxCols;
 j = nowPad / maxCols;

 (*out)[nowPad] = maxCode;

 y_from = ((j-1 < 0) ? 0 : j-1);
 y_to   = ((j+1 >= maxRows) ? maxRows-1 : j+1);
 x_from = ((i-1 < 0) ? 0 : i-1);
 x_to   = ((i+1 >= maxCols) ? maxCols-1 : i+1);
 // loop on the 8-connected pads around the pad "nowPad".
 for (k = y_from; k <= y_to; k++)
  for (l = x_from; l <= x_to; l++) {
   padnear = l + maxCols*k;
   if (iPadActive[padnear] && !(l==i && k==j)) 
       if ((*in)[padnear] <= (*in)[nowPad]) {// if the neighbouring pad has a weight lower than the l.m. it gets the same label as l.m.
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
 // this label procedure is reapeated for all the pads that have
 // been already labeled, as long as they are sorrounded by pads
 // with a lower l.m. weight.

 for (k = 0; k < 3; k++)
  for (l = 0; l < 3; l++)
   if (iTempMatrix[l + 3*k] != 0) 
    MaxMarker(&(*showYou), &(*in), &(*out), 
              nowPad+l-1 + maxCols*(k-1), iTempMatrix[l + 3*k]);

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
 // here the function MaxCluster is called recursevely
 // in order to take into account all the pads beloning to
 // the cluster in the calculation of the cluster properties.
 for (k = 0; k < 3; k++)
  for (l = 0; l < 3; l++)
   if (iTempMatrix[l + 3*k] > 0)
    MaxCluster(&(*showYou), &(*in), &(*out),
               nowPad+l-1 + maxCols*(k-1), maxCode, minAmpl);
}                    

         

//----------------------------------------------------------------------------
Int_t HRichRingFind::TestRing(HRichAnalysis *showIt, HRichHit *hit, Int_t amplit) {

// All the rings that have been found by the 2 algorhtms and then
// further analyzed, are tested.
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
  // The asymmetry of ring is investigated. The centroid of the ring is 
  // defined by the difference in X and Y coordinates between the ring center
    // and the center of gravity of the ring. In order to get a positive
    // decision from the test  the distance must be lower than iRingRadiusError given in input data.
  // Then a ring radius is calculated and it also has to fit
  // in the value iRingRadius +/- iRingRadiusError.
  // If a ring is clearly outside the border the test is not done (always positive).
  // REMARK: ring radius and ring centroid are calculated ONLY for rings
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
  else
    {
      pHit->fRingCentroid = -1.0;
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

//============================================================================

//----------------------------------------------------------------------------
void HRichRingFind::CalcRingParameters(HRichAnalysis *showMe,HRichHit *pHit){
    //
    // This method calculates for each ring candidate:
    // 1) the number of fired pads that belong to the ring (all fired
    // pads in the region 13X13 centered in the ring center)
    // 2) the total charge of the ring obtained summing up all
    // the charges of the pads that belong to the ring.
    // 3) the number of photon local maxima among 5 pads, that should correspond
    // to the number of photon in one ring
    // 4) the number of photon local maxima among 9 pads.
    // It is called at the end of the RingFindHoughTransf and RingFindFitMatrix
    // function, after the lists of candidate have been filled and all properties
    // calculated
Int_t i,j,k,l,m,
       iIsPhot4, iIsPhot8, iPhot4Nr, iPhot8Nr, iPad;
 Int_t iNowX, iNowY, iShift;

 iPhot4Nr = iPhot8Nr = iPad = 0;
 iShift = pAnalysisParams->iRingMaskSize/2;


  iRingTempImage.Reset();
  // loop on all the pads
  for (j = 0; j < pAnalysisParams->iRingMaskSize; j++) 
      for (i = 0; i < pAnalysisParams->iRingMaskSize; i++) {
	  if (!showMe->IsOut(pHit->iRingX,pHit->iRingY,i-iShift,j-iShift)) {
	      pHit->iRingImage[i + pAnalysisParams->iRingMaskSize*j] =
		  showMe->GetPad(pHit->iRingX+i-iShift, pHit->iRingY+j-iShift)->getAmplitude();
	  } else pHit->iRingImage[i + pAnalysisParams->iRingMaskSize*j] = 0;
      }
  // iRingImage contains the amplitudes of all the fired pads in 
  // the 13X13 Mask centered in the ring center. 
  iPhot4Nr = iPhot8Nr = 0;
  iNowX = pHit->iRingX;
  iNowY = pHit->iRingY;
  for (j = 0; j < pAnalysisParams->iRingMaskSize; j++)// loop on all the pads
      for (i = 0; i < pAnalysisParams->iRingMaskSize; i++)
	  if (!showMe->IsOut(iNowX,iNowY,i-iShift,j-iShift)) {
	      iIsPhot4 = iIsPhot8 = 0;
	      m = iNowX+i-iShift + maxCols*(iNowY+j-iShift);
	      if (showMe->GetPad(m)->getAmplitude() > 0 && 
		  pAnalysisParams->iRingMask[i + (pAnalysisParams->iRingMaskSize)*j] == 1) {
		  pHit->iRingPadNr++;
		  pHit->iRingAmplitude += showMe->GetPad(m)->getAmplitude();

		  //loop on the pad neighbour to determine if the pad
		  // is a photon local maxima among the 4 or 8-connected pads.
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
		      // "iRingTempImage" contains the label of the photon
		      // local maxima, if a photon local maxima belongs
		      // only to 1 ring it becomes 1 as label, 2 if it
		      // belongs to more than 1 rings 

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
    // cout<<" in match rings "<<endl;
    // cout<<"  GetAlgorithmNr(showMe)  "<<  GetAlgorithmNr(showMe) <<endl;
    // cout<<" pAnalysisParams->iSuperiorAlgorithmID  "<< pAnalysisParams->iSuperiorAlgorithmID  <<endl;
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
	//if the two rings are so closed to each other
	// to be the same.
     if (CalcDistance(*pHit1,*pHit2) <= pAnalysisParams->iRingRadius/2) {
      if (pAnalysisParams->iSuperiorAlgorithmID == 1) {
       pRings[iRingNr] = *pHit1;
       pRings[iRingNr].iRingPatMat = pHit1->iRingQuality;
       pRings[iRingNr].iRingHouTra = pHit2->iRingQuality;
       pRings[iRingNr].iRingAlgorithmIndex = 5;
       iRingNr++;
       iChosen = 1;
       //  cout<<" ring Number :"<<iRingNr<<" pat mat "<<pHit1->iRingQuality<<" Pad X Float"<<pHit1->getPadX()<<"  Pad Y Float"<<pHit1->getPadY()<<" X Int  "<<pHit1->getRingCenterX()<<" Y Int  "<< pHit1->getRingCenterY() <<" sector  "<<pHit1->getSector() <<endl;
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
	
	do {// center of the fake ring
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
	// if there are rings the number of Clusters the ring is composed
	// of is calculated together with some average variables.
	for (n = 0; n < iRingNr; n++) {
	    iCount = 0;
	    fClusterSize = 0.;
	    fClusterLMax4 = 0.;
	    fClusterLMax8 = 0.;
	    for (j = 0; j < iRingImageSize; j++)// loop over all pads
		//belonging to the ring mask
		for (i = 0; i < iRingImageSize; i++)
		    if (pRings[n].iRingImage[i + iRingImageSize*j] > 0) {
			iInnerCount = iInnerPhot4 = iInnerPhot8 = 0;
			// the following function calculates the cluster
			// properties
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

    // the function calculates the total number of pads and photon
    // local maxima (4 and 8-connected pads), for all the clusters
    // that belong to a ring.

Int_t a,b, iTempMatrix[3][3] = {{0,0,0},{0,0,0},{0,0,0}};

// "dumpArr" contains the positions of the photon local maxima (it is
// nothing else than the "iRingTempImage" filled in the CalcRingParameter 
// function.
 if (dumpArr[nowX + iRingImageSize*nowY] != 3) {
  if (dumpArr[nowX + iRingImageSize*nowY] == 1) iInnerPhot4++;
  else if (dumpArr[nowX + iRingImageSize*nowY] == 2) { 
   iInnerPhot4++;
   iInnerPhot8++; 
  }
  if (pAnalysisParams->iRingMask[nowX + iRingImageSize*nowY] == 1) iInnerCount++;
  dumpArr[nowX + iRingImageSize*nowY] = 3;// each pad already taken into account is marked with the label 3
 }

 for (b = 0; b < 3; b++)// loop over the neighbouring pads 
     for (a = 0; a < 3; a++)
	 if (nowX+a-1 >= 0 && nowX+a-1 < iRingImageSize && 
	     nowY+b-1 >= 0 && nowY+b-1 < iRingImageSize)
	     if (!(a == 1 && b == 1))// we look for other local maxima
		 if (pRings[ringNr].iRingImage[nowX+a-1 + iRingImageSize*(nowY+b-1)] > 0 &&
		     dumpArr[nowX+a-1 + iRingImageSize*(nowY+b-1)] != 3) {
		     if (dumpArr[nowX+a-1 + iRingImageSize*(nowY+b-1)] == 1) iInnerPhot4++;
		     else if (dumpArr[nowX+a-1 + iRingImageSize*(nowY+b-1)] == 2) { 
			 iInnerPhot4++;// number of photon local maxima in the cluster
			 iInnerPhot8++; 
		     }
		     if (pAnalysisParams->iRingMask[nowX+a-1 + 
						   (pAnalysisParams->iRingMaskSize)*(nowY+b-1)] == 1) iInnerCount++;// number of pads in the cluster
		     dumpArr[nowX+a-1 + iRingImageSize*(nowY+b-1)] = 3;
		     iTempMatrix[a][b] = 1;
		     
		 }
 // CalcRingClusters is called for all the pads connected to 
 // the neighbours. (See how the pads for the direct hits clusters
 // are connected.
 
 for (b = 0; b < 3; b++)
     for (a = 0; a < 3; a++)
	 if (iTempMatrix[a][b] > 0) 
	     CalcRingClusters(showYou,dumpArr,ringNr,nowX+a-1,nowY+b-1);

} // eof CalcRingClusters
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
	 && pHit1->getRejFake()==0 || pHit1-> fTests == 3)
      pHit1-> fTests = 3;
    else pHit1-> fTests += pHit1->getRejFake()*1000000;
  }

}    
//============================================================================

