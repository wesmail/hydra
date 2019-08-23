using namespace std;
#include "hdetpario.h"
#include "hparhadasciifileio.h"
#include "hrichanalysispar.h"
#include "hparamlist.h"
#include "hdetparasciifileio.h"
#include <fstream> 
#include <iostream> 
#include <iomanip>
#include <string.h>

ClassImp(HRichAnalysisPar)


//---------------------------------------------------------------------------- 
HRichAnalysisPar::HRichAnalysisPar(const Char_t* name,const Char_t* title,
                                   const Char_t* context)
                : HParCond(name,title,context) {
  strcpy(detName,"Rich");
  clear();
}
//============================================================================

//---------------------------------------------------------------------------- 
Bool_t HRichAnalysisPar::init(HParIo* inp,Int_t* set) {
  // initializes the container from an input using the RICH interface class
  HDetParIo* input=inp->getDetParIo("HRichParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}
//============================================================================

//---------------------------------------------------------------------------- 
Int_t HRichAnalysisPar::write(HParIo* output) {
  // writes the container to an output using the RICH interface class 
  HDetParIo* out=output->getDetParIo("HRichParIo");
  if (out) return out->write(this);
  return kFALSE;
}
//============================================================================

//---------------------------------------------------------------------------- 
HRichAnalysisPar::~HRichAnalysisPar() {
 clear();
}
//============================================================================

//---------------------------------------------------------------------------- 
void HRichAnalysisPar::clear() {

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

 maxFiredSectorPads = 0;
 isActiveBorderAmplitReduction = 0;

 isActiveLabelPads = 0;
 iLabeledPadsDistance = 0;
 isActiveTestAsymmetry = 0;

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

 isActiveTestCharge = 0;
 fRingMinCharge = 0.;
 fRingMaxCharge = 0.;

 isActiveFakesRejection = 0;
 fFakeQualityRatio = 0.;
 fFakeCentroidCut = 0.;

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
//============================================================================

//---------------------------------------------------------------------------- 
void HRichAnalysisPar::defaultInit() {

// so far dummy
 clear();
}
//============================================================================

//---------------------------------------------------------------------------- 
HRichAnalysisPar::HRichAnalysisPar(const HRichAnalysisPar& source) {
 cerr << "HRichAnalysisPar object can not be initialized with values of another object!\n";
 cerr << "Default constructor will be called.\n";
 HRichAnalysisPar();
}
//============================================================================

//---------------------------------------------------------------------------- 
HRichAnalysisPar& HRichAnalysisPar::operator=(const HRichAnalysisPar& source) {
 if (this != &source) {
  cerr << "HRichAnalysisPar object can not be assigned!\n";
  cerr << "Default constructor will be called.\n";
  HRichAnalysisPar();
 }
return *this;
}
//============================================================================

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
 
  maxFiredSectorPads = mydata.ReadInt("Maximal number of fired pads per sector");
  isActiveBorderAmplitReduction = mydata.ReadInt("Amplitude reduction at the border Status");
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
  isActiveTestCharge = mydata.ReadInt("Test Ring Charge");
  fRingMinCharge = mydata.ReadFloat("Minimal Ring Charge");
  fRingMaxCharge = mydata.ReadFloat("Maximal Ring Charge");
  isActiveFakesRejection = mydata.ReadInt("Close Fakes Rejection");
  fFakeQualityRatio = mydata.ReadFloat("Minimal Neighbour Quality");
  fFakeCentroidCut = mydata.ReadFloat("Minimal Centroid Shift");
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
void HRichAnalysisPar::printParam() {

 using namespace HadAsciiAuxiliaryFunctions;

 Char_t buf[5280], buf2[2280];

sprintf(buf,"$RING PARAMETERS$ \
             Ring radius = %d$ \
             Ring radius error = %d$$",
             iRingRadius,
             iRingRadiusError);

sprintf(buf2,"PAD CLEANING$ \
             Clean Alone Pad lower threshold = %d$ \
             Clean High Pulse upper threshold = %d$$ \
             PATTERN MATRIX$", 
             iCleanAlonePadLowerThreshold, iCleanHighPulseUpperThreshold);

 strconcat(buf,buf2);
 buf2[0] = '\0';

if (isActiveRingFindFitMatrix) {
 sprintf(buf2,"Minimal Ring Quality in Pattern Matrix = %d$ \
               Minimal Rings Distances in Pattern Matrix = %d$ \
               Number of Rings to be sought by Pattern Matrix = %d$$ \
               HOUGH TRANSFORM$",
               iMinimalFitMatrixRingQuality,
               iMinimalFitMatrixRingDistance,
               iHowManyFitMatrixRings);
} else sprintf(buf2,"(not active)$$HOUGH TRANSFORM$");

 strconcat(buf,buf2);
 buf2[0] = '\0';

if (isActiveRingHoughTransf) {
 sprintf(buf2,"Minimal Ring Quality in Hough Transform = %d$ \
               Minimal Rings Distances in Hough Transform = %d$ \
               Number of Rings to be sought by Hough Transform = %d$$ \
               RING TESTS$",
               iMinimalHoughTransfRingQuality,
               iMinimalHoughTransfRingDistance,
               iHowManyHoughTransfRings);
} else sprintf(buf2,"(not active)$$RING TESTS$");

 strconcat(buf,buf2);
 buf2[0] = '\0';

 sprintf(buf2,"Ring Test Density  = ");
 strconcat(buf,buf2);
 buf2[0] = '\0';
 if (isActiveTestDensity) sprintf(buf2,"yes$Amplitude reduction at the border = ");
  else sprintf(buf2,"no$Amplitude reduction at the border = ");
 strconcat(buf,buf2);
 buf2[0] = '\0';
 if (isActiveBorderAmplitReduction) sprintf(buf2,"yes$Dynamic Amplitude Threshold = ");
  else sprintf(buf2,"no$Dynamic Amplitude Threshold = ");
 strconcat(buf,buf2);
 buf2[0] = '\0';
 if (isActiveDynamicThrAmplitude) sprintf(buf2,"yes$Fired Pads Ratio in ring = ");
  else sprintf(buf2,"no$Fired Pads Ratio in ring = ");
 strconcat(buf,buf2);
 buf2[0] = '\0';
 if (isActiveFiredRingPadsRatio) sprintf(buf2,"yes$Test Ring Asymmetry = ");
  else sprintf(buf2,"no$Test Ring Asymmetry = ");
 strconcat(buf,buf2);
 buf2[0] = '\0';
 if (isActiveTestAsymmetry) sprintf(buf2,"yes$Test Ring Charge =");
  else sprintf(buf2,"no$Test Ring Charge =");
 strconcat(buf,buf2);
 buf2[0] = '\0';
if (isActiveTestCharge) sprintf(buf2,"yes$Close Fakes Rejection =");
  else sprintf(buf2,"no$Fakes Rejection = ");
 strconcat(buf,buf2);
buf2[0] = '\0';
if (isActiveFakesRejection) sprintf(buf2,"yes");
  else sprintf(buf2,"no");
 strconcat(buf,buf2);
buf2[0] = '\0';
 ErrorMsg(0,"RICH ANALYSIS PARAMETERS",1,buf);
 cout<<" iCutOffThresheold	"<< iCutOffThresheold<<endl;
 cout<<" iRingRadius	"<< iRingRadius<<endl;
 cout<<" iRingMatrixSize	"<< iRingMatrixSize<<endl;
 cout<<" iRingMaskSize	"<< iRingMaskSize<<endl;
 cout<<" isActiveCleanAlonePad	"<< isActiveCleanAlonePad<<endl;
 cout<<" iCleanAlonePadBorder	"<< iCleanAlonePadBorder<<endl;
 cout<<" iCleanAlonePadLowerThreshold	"<< iCleanAlonePadLowerThreshold<<endl;
 cout<<" isActiveCleanHighPulse	"<< isActiveCleanHighPulse<<endl;
 cout<<" iCleanHighPulseBorder	"<< iCleanHighPulseBorder<<endl;
 cout<<" iCleanHighPulseUpperThreshold	"<< iCleanHighPulseUpperThreshold<<endl;
 cout<<" isActiveLabelPads	"<< isActiveLabelPads<<endl;
 cout<<" iLabeledPadsDistance	"<< iLabeledPadsDistance<<endl;
 cout<<" maxFiredSectorPads	"<< maxFiredSectorPads<<endl;
 cout<<" isActiveBorderAmplitReduction	"<< isActiveBorderAmplitReduction<<endl;
 cout<<" isActiveTestDensity	"<< isActiveTestDensity<<endl;
 cout<<" fThresholdDensity	"<< fThresholdDensity<<endl;
 cout<<" fSurfaceArea	"<< fSurfaceArea<<endl;
 cout<<" isActiveDynamicThrAmplitude	"<< isActiveDynamicThrAmplitude<<endl;
 cout<<" fLowerAmplFactor	"<< fLowerAmplFactor<<endl;
 cout<<" fFormulaParam1	"<< fFormulaParam1<<endl;
 cout<<" fFormulaParam2	"<< fFormulaParam2<<endl;
 cout<<" fFormulaParam3	"<< fFormulaParam3<<endl;
 cout<<" isActiveFiredRingPadsRatio	"<< isActiveFiredRingPadsRatio<<endl;
 cout<<" fFiredRingPadsRatio	"<< fFiredRingPadsRatio<<endl;
 cout<<" isActiveTestAsymmetry	"<< isActiveTestAsymmetry<<endl;
 cout<<" isActiveTestCharge	"<< isActiveTestCharge<<endl;
 cout<<" fRingMinCharge	"<< fRingMinCharge<<endl;
 cout<<" fRingMaxCharge	"<< fRingMaxCharge<<endl;
 cout<<" isActiveFakesRejection	"<< isActiveFakesRejection<<endl;
 cout<<" fFakeQualityRatio	"<< fFakeQualityRatio<<endl;
 cout<<" fFakeCentroidCut	"<< fFakeCentroidCut<<endl;
 cout<<" iSuperiorAlgorithmID	"<< iSuperiorAlgorithmID<<endl;
 cout<<" isActiveRingFindFitMatrix	"<< isActiveRingFindFitMatrix<<endl;
 cout<<" iMinimalFitMatrixRingQuality	"<< iMinimalFitMatrixRingQuality<<endl;
 cout<<" iMinimalFitMatrixRingDistance	"<< iMinimalFitMatrixRingDistance<<endl;
 cout<<" iHowManyFitMatrixRings	"<< iHowManyFitMatrixRings<<endl;
 cout<<" isActiveRingHoughTransf	"<< isActiveRingHoughTransf<<endl;
 cout<<" iMinimalHoughTransfRingQuality	"<< iMinimalHoughTransfRingQuality<<endl;
 cout<<" iMinimalHoughTransfRingDistance	"<< iMinimalHoughTransfRingDistance<<endl;
 cout<<" iHowManyHoughTransfRings	"<< iHowManyHoughTransfRings<<endl;

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

  mydata.WriteInt("Maximal number of fired pads per sector",maxFiredSectorPads);
  mydata.WriteLine("\n");
  mydata.WriteInt("Amplitude reduction at the border Status",isActiveBorderAmplitReduction);
  mydata.WriteLine("\n");
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
  mydata.WriteInt("Test Ring Charge",isActiveTestCharge);
  mydata.WriteFloat("Minimal Ring Charge",fRingMinCharge);
  mydata.WriteFloat("Maximal Ring Charge",fRingMaxCharge);
  mydata.WriteLine("\n");
  mydata.WriteInt("Close Fakes Rejection",isActiveFakesRejection);
  mydata.WriteFloat("Minimal Neighbour Quality",fFakeQualityRatio);
  mydata.WriteFloat("Minimal Centroid Shift",fFakeCentroidCut);
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

//============================================================================

void HRichAnalysisPar::putParams(HParamList* l) {
  if (!l) return;
  l->add("iCutOffThresheold",iCutOffThresheold);
  l->add("iRingRadius",iRingRadius);
  l->add("iRingRadiusError",iRingRadiusError);
  l->add("iRingMatrixSize",iRingMatrixSize);
  l->add("iRingMatrix",iRingMatrix);
  l->add("iRingMaskSize",iRingMaskSize);
  l->add("iRingMask",iRingMask);
  l->add("isActiveCleanAlonePad",isActiveCleanAlonePad);
  l->add("iCleanAlonePadBorder",iCleanAlonePadBorder);
  l->add("iCleanAlonePadLowerThreshold",iCleanAlonePadLowerThreshold);
  l->add("isActiveCleanHighPulse",isActiveCleanHighPulse);
  l->add("iCleanHighPulseBorder",iCleanHighPulseBorder);
  l->add("iCleanHighPulseUpperThreshold",iCleanHighPulseUpperThreshold);
  l->add("isActiveLabelPads",isActiveLabelPads);
  l->add("iLabeledPadsDistance",iLabeledPadsDistance);
  l->add("maxFiredSectorPads",maxFiredSectorPads);
  l->add("isActiveBorderAmplitReduction",isActiveBorderAmplitReduction);
  l->add("isActiveTestDensity",isActiveTestDensity);
  l->add("fThresholdDensity",fThresholdDensity);
  l->add("fSurfaceArea",fSurfaceArea);
  l->add("isActiveDynamicThrAmplitude",isActiveDynamicThrAmplitude);
  l->add("fLowerAmplFactor",fLowerAmplFactor);
  l->add("fFormulaParam1",fFormulaParam1);
  l->add("fFormulaParam2",fFormulaParam2);
  l->add("fFormulaParam3",fFormulaParam3);
  l->add("isActiveFiredRingPadsRatio",isActiveFiredRingPadsRatio);
  l->add("fFiredRingPadsRatio",fFiredRingPadsRatio);
  l->add("isActiveTestAsymmetry",isActiveTestAsymmetry);
  l->add("isActiveTestCharge",isActiveTestCharge);
  l->add("fRingMinCharge",fRingMinCharge);
  l->add("fRingMaxCharge",fRingMaxCharge);
  l->add("isActiveFakesRejection",isActiveFakesRejection);
  l->add("fFakeQualityRatio",fFakeQualityRatio);
  l->add("fFakeCentroidCut",fFakeCentroidCut);
  l->add("iSuperiorAlgorithmID",iSuperiorAlgorithmID);
  l->add("isActiveRingFindFitMatrix",isActiveRingFindFitMatrix);
  l->add("iMinimalFitMatrixRingQuality",iMinimalFitMatrixRingQuality);
  l->add("iMinimalFitMatrixRingDistance",iMinimalFitMatrixRingDistance);
  l->add("iHowManyFitMatrixRings",iHowManyFitMatrixRings);
  l->add("isActiveRingHoughTransf",isActiveRingHoughTransf);
  l->add("iMinimalHoughTransfRingQuality",iMinimalHoughTransfRingQuality);
  l->add("iMinimalHoughTransfRingDistance",iMinimalHoughTransfRingDistance);
  l->add("iHowManyHoughTransfRings",iHowManyHoughTransfRings);
}
//============================================================================

Bool_t HRichAnalysisPar::getParams(HParamList* l) {
  if (!l) return kFALSE;
  if (!l->fill("iCutOffThresheold",&iCutOffThresheold)) return kFALSE;
  if (!l->fill("iRingRadius",&iRingRadius)) return kFALSE;
  if (!l->fill("iRingRadiusError",&iRingRadiusError)) return kFALSE;
  if (!l->fill("iRingMatrixSize",&iRingMatrixSize)) return kFALSE;
  if (!l->fill("iRingMatrix",&iRingMatrix)) return kFALSE;
  if (!l->fill("iRingMaskSize",&iRingMaskSize)) return kFALSE;
  if (!l->fill("iRingMask",&iRingMask)) return kFALSE;
  if (!l->fill("isActiveCleanAlonePad",&isActiveCleanAlonePad)) return kFALSE;
  if (!l->fill("iCleanAlonePadBorder",&iCleanAlonePadBorder)) return kFALSE;
  if (!l->fill("iCleanAlonePadLowerThreshold",&iCleanAlonePadLowerThreshold)) return kFALSE;
  if (!l->fill("isActiveCleanHighPulse",&isActiveCleanHighPulse)) return kFALSE;
  if (!l->fill("iCleanHighPulseBorder",&iCleanHighPulseBorder)) return kFALSE;
  if (!l->fill("iCleanHighPulseUpperThreshold",&iCleanHighPulseUpperThreshold)) return kFALSE;
  if (!l->fill("isActiveLabelPads",&isActiveLabelPads)) return kFALSE;
  if (!l->fill("iLabeledPadsDistance",&iLabeledPadsDistance)) return kFALSE;
  if (!l->fill("maxFiredSectorPads",&maxFiredSectorPads)) return kFALSE;
  if (!l->fill("isActiveBorderAmplitReduction",&isActiveBorderAmplitReduction)) return kFALSE;
  if (!l->fill("isActiveTestDensity",&isActiveTestDensity)) return kFALSE;
  if (!l->fill("fThresholdDensity",&fThresholdDensity)) return kFALSE;
  if (!l->fill("fSurfaceArea",&fSurfaceArea)) return kFALSE;
  if (!l->fill("isActiveDynamicThrAmplitude",&isActiveDynamicThrAmplitude)) return kFALSE;
  if (!l->fill("fLowerAmplFactor",&fLowerAmplFactor)) return kFALSE;
  if (!l->fill("fFormulaParam1",&fFormulaParam1)) return kFALSE;
  if (!l->fill("fFormulaParam2",&fFormulaParam2)) return kFALSE;
  if (!l->fill("fFormulaParam3",&fFormulaParam3)) return kFALSE;
  if (!l->fill("isActiveFiredRingPadsRatio",&isActiveFiredRingPadsRatio)) return kFALSE;
  if (!l->fill("fFiredRingPadsRatio",&fFiredRingPadsRatio)) return kFALSE;
  if (!l->fill("isActiveTestAsymmetry",&isActiveTestAsymmetry)) return kFALSE;
  if (!l->fill("isActiveTestCharge",&isActiveTestCharge)) return kFALSE;
  if (!l->fill("fRingMinCharge",&fRingMinCharge)) return kFALSE;
  if (!l->fill("fRingMaxCharge",&fRingMaxCharge)) return kFALSE;
  if (!l->fill("isActiveFakesRejection",&isActiveFakesRejection)) return kFALSE;
  if (!l->fill("fFakeQualityRatio",&fFakeQualityRatio)) return kFALSE;
  if (!l->fill("fFakeCentroidCut",&fFakeCentroidCut)) return kFALSE;
  if (!l->fill("iSuperiorAlgorithmID",&iSuperiorAlgorithmID)) return kFALSE;
  if (!l->fill("isActiveRingFindFitMatrix",&isActiveRingFindFitMatrix)) return kFALSE;
  if (!l->fill("iMinimalFitMatrixRingQuality",&iMinimalFitMatrixRingQuality)) return kFALSE;
  if (!l->fill("iMinimalFitMatrixRingDistance",&iMinimalFitMatrixRingDistance)) return kFALSE;
  if (!l->fill("iHowManyFitMatrixRings",&iHowManyFitMatrixRings)) return kFALSE;
  if (!l->fill("isActiveRingHoughTransf",&isActiveRingHoughTransf)) return kFALSE;
  if (!l->fill("iMinimalHoughTransfRingQuality",&iMinimalHoughTransfRingQuality)) return kFALSE;
  if (!l->fill("iMinimalHoughTransfRingDistance",&iMinimalHoughTransfRingDistance)) return kFALSE;
  if (!l->fill("iHowManyHoughTransfRings",&iHowManyHoughTransfRings)) return kFALSE;
  return kTRUE;
}

Bool_t HRichAnalysisPar::readAscii(HDetParAsciiFileIo* io) {
  return io->readCond(this);
}  

Int_t HRichAnalysisPar::writeAscii(HDetParAsciiFileIo* io) {
  return io->writeCond(this);
}
