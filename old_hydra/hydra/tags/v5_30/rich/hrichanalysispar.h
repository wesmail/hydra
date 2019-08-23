#ifndef HRICHANALYSISPAR_H
#define HRICHANALYSISPAR_H

#include "TObject.h"
#include "TArrayI.h"
#include "hparset.h"
#include "hrichpad.h"
#include "hrichparset.h"
#include "hlocation.h"


class HRichAnalysisPar: public HRichParSet {

public:
 
 TArrayI iSectorsNr;
 TArrayI iPadsNr;
 TArrayI iPadsXNr;
 TArrayI iPadsYNr;
 
 Int_t iCutOffThresheold;

 Int_t iRingRadius;
 Int_t iRingRadiusError;

 Int_t iRingMatrixSize;
 TArrayI iRingMatrix;
 Int_t iRingMaskSize;
 TArrayI iRingMask;

 // ---- parameters for pad cleaning ----

 Int_t isActiveCleanAlonePad;
 Int_t iCleanAlonePadBorder;
 Int_t iCleanAlonePadLowerThreshold;

 Int_t isActiveCleanHighPulse;
 Int_t iCleanHighPulseBorder;
 Int_t iCleanHighPulseUpperThreshold;

 // ---- parameters for pad labeling ----

 Int_t isActiveLabelPads;
 Int_t iLabeledPadsDistance;

 // ---- parameters for ring finding ----

 Int_t isActiveTestDensity;
 Float_t fThresholdDensity;
 Float_t fSurfaceArea;



 Int_t isActiveDynamicThrAmplitude;
 Float_t fLowerAmplFactor;
 Float_t fFormulaParam1;
 Float_t fFormulaParam2;
 Float_t fFormulaParam3;

 Int_t isActiveFiredRingPadsRatio;
 Float_t fFiredRingPadsRatio;

    // Int_t isActiveTestAsymmetry;

 Int_t iSuperiorAlgorithmID;

 Int_t isActiveRingFindFitMatrix;
 Int_t iMinimalFitMatrixRingQuality;
 Int_t iMinimalFitMatrixRingDistance;
 Int_t iHowManyFitMatrixRings;

 Int_t isActiveRingHoughTransf;
 Int_t iMinimalHoughTransfRingQuality;
 Int_t iMinimalHoughTransfRingDistance;
 Int_t iHowManyHoughTransfRings;


    Float_t fThresholdRed; //reduction factor for the quality check of those rings that are close to the detector edges
    Float_t fFakeQualRatio;//minimal quality for a fake ring found in the nearhodd of a bright rone.
    Float_t fFakeCentroidCut; //minimal shift of the centroid of the fake ring respect to teh centroid of the bright one.
    Float_t fFakePadAmplitude; //minimal average pad amplitude for fake rings produced by noise 

    Float_t fRingRadius; // maximal deviation centroid-ring center
    Float_t fMeanRadiusError; //maximal error for ring radius test
 
    Int_t isActiveCentroidCheck;
    Int_t isActiveRadiusCheck; 
private:

 char filename[40];

public:

 HRichAnalysisPar();
 virtual ~HRichAnalysisPar();
 
 HRichAnalysisPar(const HRichAnalysisPar& source);
 HRichAnalysisPar& operator=(const HRichAnalysisPar& source);

 virtual Bool_t initAscii(HParHadAsciiFileIo* pHadAsciiFile);
 virtual Bool_t writeAscii(HParHadAsciiFileIo* pHadAsciiFile);
 void clear();
 void defaultInit();
 void printParam();

 void setFileName(char* fn){strncpy(filename, fn, 40);}


 ClassDef(HRichAnalysisPar,1)
};


#endif // HRICHANALYSISPAR_H


