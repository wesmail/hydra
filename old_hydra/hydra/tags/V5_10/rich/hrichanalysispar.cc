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



//----------------------------------------------------------------------------
Bool_t HRichAnalysisPar::initAscii(HParHadAsciiFileIo* pHadAsciiFile) {

 using namespace HadAsciiAuxiliaryFunctions;
 if (!pHadAsciiFile) return kFALSE;

 Bool_t status = kTRUE;

 try {

  HAsciiKey &mydata = *pHadAsciiFile->GetKeyAscii();

  mydata.SetActiveSection("RICH Analysis Initial Data");

  iCutOffThresheold = mydata.ReadInt("Signal cut-off threshold");
  iRingRadius = mydata.ReadInt("Ring radius");
  iRingRadiusError = mydata.ReadInt("Ring radius error");

  mydata.SetActiveSection("RICH Analysis Algorithms");

  isActiveCleanAlonePad = mydata.ReadInt("Clean Alone Pad Status");
  iCleanAlonePadBorder = mydata.ReadInt("Clean Alone Pad border");
  iCleanAlonePadLowerThreshold = mydata.ReadInt("Clean Alone Pad lower threshold");
  isActiveCleanHighPulse = mydata.ReadInt("Clean High Pulse Status");
  iCleanHighPulseBorder = mydata.ReadInt("Clean High Pulse border");
  iCleanHighPulseUpperThreshold = mydata.ReadInt("Clean High Pulse upper threshold");
  isActiveLabelPads = mydata.ReadInt("Label Pads Status");
  iLabeledPadsDistance = mydata.ReadInt("Label Pads distance");

  mydata.SetActiveSection("RICH Ring Recognition Algorithms");
 
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

  mydata.SetActiveSection("RICH Pattern Matrixes");

  iRingMatrixSize = mydata.ReadInt("Ring Pattern Matrix Size");
  iRingMatrix.Set(iRingMatrixSize*iRingMatrixSize);
  mydata.ReadInt(&iRingMatrix[0],"Ring Pattern Matrix");
  iRingMaskSize = mydata.ReadInt("Ring Mask Matrix Size");
  iRingMask.Set(iRingMaskSize*iRingMaskSize);
  mydata.ReadInt(&iRingMask[0],"Ring Mask Matrix");

 } // eof try block

 catch (Bool_t ret) {
  status = ret;
 }
 catch (...) {
  status = kFALSE;
  ErrorMsg(2,"HRichAnalysisPar::initAscii",1,"Unidentified exception catched.");
 }

 if (status) 
  ErrorMsg(0,"HRichAnalysisPar::initAscii",
   1,"Container \'RichAnalysisParameters\' has been read from ASCII file.");

 return status;
}
//============================================================================

//---------------------------------------------------------------------------- 
Bool_t HRichAnalysisPar::writeAscii(HParHadAsciiFileIo* pHadAsciiFile) {

 using namespace HadAsciiAuxiliaryFunctions;
 if (!pHadAsciiFile) return kFALSE;

 Bool_t status = kTRUE;

 try {

  HAsciiKey &mydata = *pHadAsciiFile->GetKeyAscii();

  mydata.WriteSection("RICH Analysis Initial Data");

  mydata.WriteInt("Signal cut-off threshold",iCutOffThresheold);
  mydata.WriteInt("Ring radius",iRingRadius);
  mydata.WriteInt("Ring radius error",iRingRadiusError);

  mydata.WriteSection("RICH Analysis Algorithms");

  mydata.WriteInt("Clean Alone Pad Status",isActiveCleanAlonePad);
  mydata.WriteInt("Clean Alone Pad border",iCleanAlonePadBorder);
  mydata.WriteInt("Clean Alone Pad lower threshold",iCleanAlonePadLowerThreshold);
  mydata.WriteLine("\n");
  mydata.WriteInt("Clean High Pulse Status",isActiveCleanHighPulse);
  mydata.WriteInt("Clean High Pulse border",iCleanHighPulseBorder);
  mydata.WriteInt("Clean High Pulse upper threshold",iCleanHighPulseUpperThreshold);
  mydata.WriteLine("\n");
  mydata.WriteInt("Label Pads Status",isActiveLabelPads);
  mydata.WriteInt("Label Pads distance",iLabeledPadsDistance);

  mydata.WriteSection("RICH Ring Recognition Algorithms");

  mydata.WriteInt("Ring Test Density Status",isActiveTestDensity);
  mydata.WriteFloat("Pads Density threshold",fThresholdDensity);
  mydata.WriteFloat("Surface Area (in ring units)",fSurfaceArea);
  mydata.WriteLine("\n");
  mydata.WriteInt("Dynamic Amplitude Threshold Status",isActiveDynamicThrAmplitude);
  mydata.WriteFloat("Minimal Amplitude fraction",fLowerAmplFactor);
  mydata.WriteFloat("Formula Parameter 1",fFormulaParam1);
  mydata.WriteFloat("Formula Parameter 2",fFormulaParam2);
  mydata.WriteFloat("Formula Parameter 3",fFormulaParam3);
  mydata.WriteLine("\n");
  mydata.WriteInt("Fired Pads Ratio in ring Status",isActiveFiredRingPadsRatio);
  mydata.WriteFloat("Fired Pads Out of Ring/In Ring Ratio",fFiredRingPadsRatio);
  mydata.WriteLine("\n");
  mydata.WriteInt("Test Ring Asymmetry Status",isActiveTestAsymmetry);
  mydata.WriteLine("\n");
  mydata.WriteInt("Superior Algorithm ID",iSuperiorAlgorithmID);
  mydata.WriteLine("\n");
  mydata.WriteInt("Ring Find Fit Pattern Matrix Status",isActiveRingFindFitMatrix);
  mydata.WriteInt("Minimal Ring Quality in Fit Matrix",iMinimalFitMatrixRingQuality);
  mydata.WriteInt("Minimal Rings Distances in Fit Matrix",iMinimalFitMatrixRingDistance);
  mydata.WriteInt("Number of Rings to be sought by Fit Matrix",iHowManyFitMatrixRings);
  mydata.WriteLine("\n");
  mydata.WriteInt("Ring Find Hough Transformation Status",isActiveRingHoughTransf);
  mydata.WriteInt("Minimal Ring Quality in Hough Transformation",iMinimalHoughTransfRingQuality);
  mydata.WriteInt("Minimal Rings Distances in Hough Transformation",iMinimalHoughTransfRingDistance);
  mydata.WriteInt("Number of Rings to be sought by Hough Transformation",iHowManyHoughTransfRings);

  mydata.WriteSection("RICH Pattern Matrixes");

  mydata.WriteInt("Ring Pattern Matrix Size",iRingMatrixSize);
  mydata.SetNumInLine(11);
  mydata.WriteInt("Ring Pattern Matrix*",&iRingMatrix);
  mydata.WriteInt("Ring Mask Matrix Size",iRingMaskSize);
  mydata.SetNumInLine(13);
  mydata.WriteInt("Ring Mask Matrix*",&iRingMask);

 } // eof try block

 catch (Bool_t ret) {
  status = ret;
 }
 catch (...) {
  status = kFALSE;
  ErrorMsg(1,"HRichAnalysisPar::writeAscii",1,"Unidentified exception catched.");
 }

 if (status)         
  ErrorMsg(0,"HRichAnalysisPar::writeAscii",
   1,"Container \'RichAnalysisParameters\' has been written to ASCII file.");

 return status;
}
//============================================================================



