// File: hrichringfind.h
// ****************************************************************************
//
// HRichRingFind
//
// Author: Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
// Modified: 1999/12/04 by Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
// ----------------------------------------------------------------------------
//
// ****************************************************************************


#ifndef HRICHRINGFIND_H
#define HRICHRINGFIND_H

#include "TObject.h"
#include "TArrayI.h"
#include "TList.h"
#include "hrichhit.h"
#include "hrichhitcandidate.h"


class HRichAnalysis;
class HRichAnalysisPar;
class HRichGeometryPar;

// ****************************************************************************

class HRichRingFind: public TObject {

// ****************************************************************************

protected:

 HRichAnalysisPar *pAnalysisParams;
 HRichGeometryPar *pGeometryParams;

 Int_t iInnerCount, iInnerPhot4, iInnerPhot8;
 Float_t fClusterSize, fClusterLMax4, fClusterLMax8;

 Int_t iRingImageSize;
 TArrayI iRingTempImage;
 Double_t HomogenDistr(Double_t left, Double_t right);

 Float_t xMeanMax, yMeanMax, xPadMeanMax, yPadMeanMax, thetaMeanMax, phiMeanMax;
 Int_t fMaxClusterSize, fMaxClusterSum, fMaxThrClusterSize;
 Int_t iCount;
 Int_t iHitCount;
 Int_t maxCols;
 Int_t maxRows;

 Int_t iMatrixSize, iMatrixHalfSize;

 Int_t ly_from, ly_to, lx_from, lx_to;
 Int_t d_col_ij,d_row_ij,d_col_jk,d_row_jk,d2_colrow_jk,d2_colrow_ij;

 TArrayI iPadActive;

public:

 TArrayI iPadPlane;
 TArrayI iPadPlaneCopy;

 TArrayI iPadCol;
 TArrayI iPadRow;

 Int_t iRingNr;
 HRichHit *pRings;

 TList fHitList1;
 TList fHitList2;
 TList fHitCandidate;

 HRichRingFind();
 virtual ~HRichRingFind();


 Bool_t init(HRichAnalysis*);
 
 HRichRingFind(const HRichRingFind& source);
 HRichRingFind& operator=(const HRichRingFind& source);

 Int_t GetAlgorithmNr(HRichAnalysis *showMe);
 Float_t CalcDistance(const HRichHit& ring1, const HRichHit& ring2);
 Float_t CalcDistance(Int_t x, Int_t y, const HRichHit& ring);
 Float_t CalcDistance(Int_t x1, Int_t y1, Int_t x2, Int_t y2);

 Bool_t TestDensity(HRichAnalysis *showYou, HRichHit *pHit);
 Bool_t TestBorder(HRichAnalysis *showYou, HRichHit *pHit, Int_t amplit);
 Bool_t TestDynamic(HRichAnalysis *showYou, HRichHit *pHit, Int_t amplit);
 Bool_t TestRatio(HRichAnalysis *showYou, HRichHit *pHit);
 Bool_t TestAsymmetry(HRichAnalysis *showYou, HRichHit *pHit, Int_t amplit);
 Bool_t TestRingCharge(HRichHit *hit);
 Int_t  TestRing(HRichAnalysis *showYou,HRichHit *hit, Int_t amplit);


 void MaxFinding(HRichAnalysis *showYou, TList *hitList, TArrayI *in, TArrayI *out,
                 Int_t ringnr, Float_t distance);

 void MaxAnalysis(HRichAnalysis *showMe, TList *hitList, TArrayI *in, TArrayI *out,
                  Int_t minAmpl);
 void MaxCluster(HRichAnalysis *showYou, TArrayI *in, TArrayI *out,
                    Int_t nowPad, Int_t maxCode, Int_t minAmpl);

 Int_t MaxLabAmpl(TList *hitList, Int_t maxCode);
 void MaxSelector(HRichAnalysis *showMe, TList *hitList, TArrayI *in, TArrayI *out);
 void MaxMarker(HRichAnalysis *showYou, TArrayI *in, TArrayI *out,
                    Int_t nowPad, Int_t maxCode);
 void CloseMaxRejection(TList *hitList);


 virtual void CalcRingParameters(HRichAnalysis *showMe,HRichHit *pHit);
 virtual void CalcFakeContribution(HRichAnalysis *showMe);
 void CalcRingClusters(HRichAnalysis *showYou, Int_t *dumpArr, Int_t ringNr, Int_t nowX, Int_t nowY);

 void RingFindFitMatrix(HRichAnalysis *showMe, Int_t minampl, Int_t distance, Int_t howmanyrings = 5);
 void RingFindHoughTransf(HRichAnalysis *showMe, Int_t minampl, Int_t distance, Int_t howmanyrings = 5);

 Int_t MatchRings(HRichAnalysis *showMe, TList *hitList1, TList *hitList2);

 Int_t Execute(HRichAnalysis *giveMe);

 
 ClassDef(HRichRingFind,1)
};

//============================================================================


#endif // HRICHRINGFIND_H
