#include "hparhadasciifileio.h"
#include "hrichanalysispar.h"
#include <fstream.h>
#include <iostream.h>
#include <string.h>

ClassImp(HRichAnalysisPar)



HRichAnalysisPar::HRichAnalysisPar() {

 SetName("RichAnalysisParameters");
 clear();
}


HRichAnalysisPar::~HRichAnalysisPar() {

 if (pPads) {
  for (Int_t i = 0; i < 6; i++)
   if (pPads[i]) delete [] pPads[i];
  delete [] pPads;
  pPads = NULL;
 }
 clear();
}



void HRichAnalysisPar::clear() {

 iSectorsNr.Set(6);
 iPadsNr.Set(6);
 iPadsXNr.Set(6);
 iPadsYNr.Set(6);

 iCutOffThresheold = 0;
 iRingMatrixSize = 0;
 iRingMaskSize = 0;
 iRingRadius = 0;
 iRingRadiusError = 0;
 
 pPads = NULL;

 isActiveCleanAlonePad = 0;
 iCleanAlonePadBorder = 0;
 iCleanAlonePadLowerThreshold = 0;

 isActiveCleanHighPulse = 0;
 iCleanHighPulseBorder = 0;
 iCleanHighPulseUpperThreshold = 0;

 isActiveLabelPads = 0;
 iLabeledPadsDistance = 0;

 isActiveTestDensity = 0;
 fThresholdDensity = 0.;
 fSurfaceArea = 0.;

 isActiveDynamicThrAmplitude = 0;
 fLowerAmplFactor = 0.;
 fFormulaParam1 = 0.;
 fFormulaParam2 = 0.;
 fFormulaParam3 = 0.;

 isActiveFiredRingPadsRatio = 0;
 fFiredRingPadsRatio = 0.;

 isActiveTestAsymmetry = 0;

 iSuperiorAlgorithmID = 0;

 isActiveRingFindFitMatrix = 0;
 iMinimalFitMatrixRingQuality = 0;
 iMinimalFitMatrixRingDistance = 0;
 iHowManyFitMatrixRings = 0;

 isActiveRingHoughTransf = 0;
 iMinimalHoughTransfRingQuality = 0;
 iMinimalHoughTransfRingDistance = 0;
 iHowManyHoughTransfRings = 0;
}


void HRichAnalysisPar::defaultInit() {

// so far dummy
 clear();
}



HRichAnalysisPar::HRichAnalysisPar(const HRichAnalysisPar& source) {
 cerr << "HRichAnalysisPar object can not be initialized with values of another object!\n";
 cerr << "Default constructor will be called.\n";
 HRichAnalysisPar();
}


HRichAnalysisPar& HRichAnalysisPar::operator=(const HRichAnalysisPar& source) {
 if (this != &source) {
  cerr << "HRichAnalysisPar object can not be assigned!\n";
  cerr << "Default constructor will be called.\n";
  HRichAnalysisPar();
 }
return *this;
}


void HRichAnalysisPar::Streamer(TBuffer &R__b) {
// Stream an object of class HRichAnalysisPar.
Int_t i,j;

   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(); if (R__v) { }
      TObject::Streamer(R__b);
      iSectorsNr.Streamer(R__b);
      iPadsNr.Streamer(R__b);
      iPadsXNr.Streamer(R__b);
      iPadsYNr.Streamer(R__b);
      R__b >> iCutOffThresheold;
      R__b >> iRingRadius;
      R__b >> iRingRadiusError;
      pPads = new (HRichPad*)[6];
      for (i = 0; i < 6; i++) {
       if (iSectorsNr[i] > 0 && iPadsNr[i] > 0) {
        pPads[i] = new HRichPad[iPadsNr[i]];
        for (j = 0; j < iPadsNr[i]; j++) pPads[i][j].Streamer(R__b);
       } else pPads[i] = NULL;
      }
      //R__b.ReadArray(pPads);
      R__b >> iRingMatrixSize;
      iRingMatrix.Streamer(R__b);
      R__b >> iRingMaskSize;
      iRingMask.Streamer(R__b);
      R__b >> isActiveCleanAlonePad;
      R__b >> iCleanAlonePadBorder;
      R__b >> iCleanAlonePadLowerThreshold;
      R__b >> isActiveCleanHighPulse;
      R__b >> iCleanHighPulseBorder;
      R__b >> iCleanHighPulseUpperThreshold;
      R__b >> isActiveLabelPads;
      R__b >> iLabeledPadsDistance;
      R__b >> isActiveTestDensity;
      R__b >> fThresholdDensity;
      R__b >> fSurfaceArea;
      R__b >> isActiveDynamicThrAmplitude;
      R__b >> fLowerAmplFactor;
      R__b >> fFormulaParam1;
      R__b >> fFormulaParam2;
      R__b >> fFormulaParam3;
      R__b >> isActiveFiredRingPadsRatio;
      R__b >> fFiredRingPadsRatio;
      R__b >> isActiveTestAsymmetry;
      R__b >> iSuperiorAlgorithmID;
      R__b >> isActiveRingFindFitMatrix;
      R__b >> iMinimalFitMatrixRingQuality;
      R__b >> iMinimalFitMatrixRingDistance;
      R__b >> iHowManyFitMatrixRings;
      R__b >> isActiveRingHoughTransf;
      R__b >> iMinimalHoughTransfRingQuality;
      R__b >> iMinimalHoughTransfRingDistance;
      R__b >> iHowManyHoughTransfRings;
   } else {
      R__b.WriteVersion(HRichAnalysisPar::IsA());
      TObject::Streamer(R__b);
      iSectorsNr.Streamer(R__b);
      iPadsNr.Streamer(R__b);
      iPadsXNr.Streamer(R__b);
      iPadsYNr.Streamer(R__b);
      R__b << iCutOffThresheold;
      R__b << iRingRadius;
      R__b << iRingRadiusError;
      for (i = 0; i < 6; i++) 
       if (iSectorsNr[i] > 0 && iPadsNr[i] > 0)
        for (j = 0; j < iPadsNr[i]; j++) pPads[i][j].Streamer(R__b);
      //R__b.WriteArray(pPads, __COUNTER__);
      R__b << iRingMatrixSize;
      iRingMatrix.Streamer(R__b);
      R__b << iRingMaskSize;
      iRingMask.Streamer(R__b);
      R__b << isActiveCleanAlonePad;
      R__b << iCleanAlonePadBorder;
      R__b << iCleanAlonePadLowerThreshold;
      R__b << isActiveCleanHighPulse;
      R__b << iCleanHighPulseBorder;
      R__b << iCleanHighPulseUpperThreshold;
      R__b << isActiveLabelPads;
      R__b << iLabeledPadsDistance;
      R__b << isActiveTestDensity;
      R__b << fThresholdDensity;
      R__b << fSurfaceArea;
      R__b << isActiveDynamicThrAmplitude;
      R__b << fLowerAmplFactor;
      R__b << fFormulaParam1;
      R__b << fFormulaParam2;
      R__b << fFormulaParam3;
      R__b << isActiveFiredRingPadsRatio;
      R__b << fFiredRingPadsRatio;
      R__b << isActiveTestAsymmetry;
      R__b << iSuperiorAlgorithmID;
      R__b << isActiveRingFindFitMatrix;
      R__b << iMinimalFitMatrixRingQuality;
      R__b << iMinimalFitMatrixRingDistance;
      R__b << iHowManyFitMatrixRings;
      R__b << isActiveRingHoughTransf;
      R__b << iMinimalHoughTransfRingQuality;
      R__b << iMinimalHoughTransfRingDistance;
      R__b << iHowManyHoughTransfRings;
   }
}




Bool_t HRichAnalysisPar::initAscii(HParHadAsciiFileIo* pHadAsciiFile)
{
 // Int_t i, j;

 if (!pHadAsciiFile) return kFALSE;

 HAsciiKey* pHAscii = pHadAsciiFile->GetKeyAscii();
 HAsciiKey &mydata = *pHAscii;

 if (mydata.SetActiveSection("RICH Analysis Initial Data") == kFALSE)
  return kFALSE;

  iCutOffThresheold = mydata.ReadInt("Signal cut-off threshold");
  iRingRadius = mydata.ReadInt("Ring radius");
  iRingRadiusError = mydata.ReadInt("Ring radius error");

 if (mydata.SetActiveSection("RICH Analysis Algorithms") == kFALSE)
  return kFALSE;

  isActiveCleanAlonePad = mydata.ReadInt("Clean Alone Pad Status");
  iCleanAlonePadBorder = mydata.ReadInt("Clean Alone Pad border");
  iCleanAlonePadLowerThreshold = mydata.ReadInt("Clean Alone Pad lower threshold");
  isActiveCleanHighPulse = mydata.ReadInt("Clean High Pulse Status");
  iCleanHighPulseBorder = mydata.ReadInt("Clean High Pulse border");
  iCleanHighPulseUpperThreshold = mydata.ReadInt("Clean High Pulse upper threshold");
  isActiveLabelPads = mydata.ReadInt("Label Pads Status");
  iLabeledPadsDistance = mydata.ReadInt("Label Pads distance");

 if (mydata.SetActiveSection("RICH Ring Recognition Algorithms") == kFALSE)
  return kFALSE;
 
  isActiveTestDensity = mydata.ReadInt("Ring Test Density Status");
  fThresholdDensity = mydata.ReadFloat("Pads Density threshold");
  fSurfaceArea = mydata.ReadFloat("Surface Area (in ring units)");
  isActiveDynamicThrAmplitude = mydata.ReadInt("Dynamic Amplitude Threshold Status");
  fLowerAmplFactor = mydata.ReadFloat("Minimal Amplitude fraction");
  fFormulaParam1 = mydata.ReadFloat("Formula Parameter 1");
  fFormulaParam2 = mydata.ReadFloat("Formula Parameter 2");
  fFormulaParam3 = mydata.ReadFloat("Formula Parameter 3");
  isActiveFiredRingPadsRatio = mydata.ReadInt("Fired Pads Ratio in ring Status");
  fFiredRingPadsRatio = mydata.ReadFloat("Fired Pads Out of Ring/In Ring Ratio");
  isActiveTestAsymmetry = mydata.ReadInt("Test Ring Asymmetry Status");
  iSuperiorAlgorithmID = mydata.ReadInt("Superior Algorithm ID");
  isActiveRingFindFitMatrix = mydata.ReadInt("Ring Find Fit Pattern Matrix Status");
  iMinimalFitMatrixRingQuality = mydata.ReadInt("Minimal Ring Quality in Fit Matrix");
  iMinimalFitMatrixRingDistance = mydata.ReadInt("Minimal Rings Distances in Fit Matrix");
  iHowManyFitMatrixRings = mydata.ReadInt("Number of Rings to be sought by Fit Matrix");
  isActiveRingHoughTransf = mydata.ReadInt("Ring Find Hough Transformation Status");
  iMinimalHoughTransfRingQuality = mydata.ReadInt("Minimal Ring Quality in Hough Transformation");
  iMinimalHoughTransfRingDistance = mydata.ReadInt("Minimal Rings Distances in Hough Transformation");
  iHowManyHoughTransfRings = mydata.ReadInt("Number of Rings to be sought by Hough Transformation");

 if (mydata.SetActiveSection("RICH Pattern Matrixes") == kFALSE)
  return kFALSE;

  iRingMatrixSize = mydata.ReadInt("Ring Pattern Matrix Size");
  iRingMatrix.Set(iRingMatrixSize*iRingMatrixSize);
  mydata.ReadInt(&iRingMatrix[0],"Ring Pattern Matrix");
  iRingMaskSize = mydata.ReadInt("Ring Mask Matrix Size");
  iRingMask.Set(iRingMaskSize*iRingMaskSize);
  mydata.ReadInt(&iRingMask[0],"Ring Mask Matrix");

return kTRUE;
}

 




