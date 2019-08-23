// File: hrichringfind.h
// ****************************************************************************
//
// HRichRingFind
//
// Author: Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
// Modified: 1999/12/04 by Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
// Updated Oct. 2000 by Wolfgang Koenig.
// ----------------------------------------------------------------------------
//
// ****************************************************************************


#ifndef HRICHRINGFIND_H
#define HRICHRINGFIND_H

#include "TObject.h"
#include "TArrayI.h"
#include "TArrayS.h"
#include "hrichhit.h"
#include "hrichpadsignal.h"


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
 Int_t iClusterSize, iClusterLMax4, iClusterLMax8;

 Int_t iRingRadius;
 Int_t iRingRadiusError;
 Int_t iRingMatrixSize;
 Int_t iRingImageSize;
 Float_t thrReductionFac;
 Float_t fakeQualityRatio;
 Float_t maxFakeDistSquared;
 Float_t fakeCentroidCut;
 Float_t fakePadAmplitude;
 TArrayS iRingTempImage;
 char * pPadsFiredCol;
 char * pPadsFiredRow;
 Double_t HomogenDistr(Double_t left, Double_t right);

 TArrayS iPadPlane;
 TArrayS iPadPlaneShifted;

public:

 Int_t iAlgorithm1RingNr;
 Int_t iAlgorithm2RingNr;

 HRichHit *pAlgorithm1Ring;
 HRichHit *pAlgorithm2Ring;
 
 Int_t iRingNr;
 HRichHit *pRings;

 HRichRingFind();
 virtual ~HRichRingFind();


 Bool_t init(HRichAnalysis*);
 void initParameters(HRichAnalysis*);
 
 HRichRingFind(const HRichRingFind& source);
 HRichRingFind& operator=(const HRichRingFind& source);

 Int_t GetAlgorithmNr();
 Float_t CalcDistance(const HRichHit& ring1, const HRichHit& ring2);
 Float_t CalcDistance(Int_t x, Int_t y, const HRichHit& ring);
 Float_t CalcDistance(Int_t x1, Int_t y1, Int_t x2, Int_t y2);
 Int_t TestRing(HRichAnalysis *showYou, HRichHit& ring, Int_t amplit);

 virtual void CalcRingParameters(HRichAnalysis *showMe,HRichHit *pRings);
 void CalcRingClusters(HRichAnalysis *showYou, Short_t *dumpArr, HRichHit & ring, Int_t nowX, Int_t nowY);

 void RingFindFitMatrix(HRichAnalysis *showMe, Int_t minampl, Int_t distance, Int_t howmanyrings = 5);
 void RingFindHoughTransf(HRichAnalysis *showMe, Int_t minampl, Int_t distance, Int_t howmanyrings = 5);
 void rejectFakes(Int_t nRings, HRichHit * pRingArray); //called directly after ring finding algorithm was applied
 void storeRings(HRichHit * pRings, Int_t ringCandidateNr, HRichHit * pAlgorithRing, Int_t algorithmId);
 void deleteFakeRings(HRichHit * pRingArray); //called after all ring parameters are known

 Int_t Execute(HRichAnalysis *giveMe);
 
 ClassDef(HRichRingFind,1)
};

//============================================================================


#endif // HRICHRINGFIND_H






