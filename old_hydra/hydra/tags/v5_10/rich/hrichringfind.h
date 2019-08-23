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
#include "hrichhit.h"


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
 TArrayI iClusterSize, iClusterLMax4, iClusterLMax8;

 Int_t iRingImageSize;
 TArrayI iRingTempImage;
 Double_t HomogenDistr(Double_t left, Double_t right);

public:

 TArrayI iPadPlane;

 Int_t iAlgorithm1RingNr;
 Int_t iAlgorithm2RingNr;

 HRichHit *pAlgorithm1Ring;
 HRichHit *pAlgorithm2Ring;
 
 Int_t iRingNr;
 HRichHit *pRings;

 HRichRingFind();
 virtual ~HRichRingFind();


 Bool_t init(HRichAnalysis*);
 
 HRichRingFind(const HRichRingFind& source);
 HRichRingFind& operator=(const HRichRingFind& source);

 Int_t GetAlgorithmNr(HRichAnalysis *showMe);
 Float_t CalcDistance(const HRichHit& ring1, const HRichHit& ring2);
 Float_t CalcDistance(Int_t x, Int_t y, const HRichHit& ring);
 Float_t CalcDistance(Int_t x1, Int_t y1, Int_t x2, Int_t y2);
 Int_t TestRing(HRichAnalysis *showYou, const HRichHit& ring, Int_t amplit);

 virtual void CalcRingParameters(HRichAnalysis *showMe);
 void CalcRingClusters(HRichAnalysis *showYou, Int_t *dumpArr, Int_t ringNr, Int_t nowX, Int_t nowY);

 void RingFindFitMatrix(HRichAnalysis *showMe, Int_t minampl, Int_t distance, Int_t howmanyrings = 5);
 void RingFindHoughTransf(HRichAnalysis *showMe, Int_t minampl, Int_t distance, Int_t howmanyrings = 5);

 Int_t Execute(HRichAnalysis *giveMe);

 
 ClassDef(HRichRingFind,1)
};

//============================================================================


#endif // HRICHRINGFIND_H
