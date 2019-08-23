//###################################################
//#
//#       
//#                      FRAMEWORK
//#
//#       Authors:                           W.Koenig
//#       adoption to framework & ROOT       W.Schoen
//#    
//#       last mod. W.Schoen 19.1.98
// ###################################################

#pragma implementation
#include "hrichdigidetpar.h"
#include "hparhadasciifileio.h"

ClassImp(HRichDigiDetPar)

////////// ********** HRichDigiDetPar defintion ****************/////////////77
HRichDigiDetPar::HRichDigiDetPar() {
 SetName("RichDigiPar");
}

HRichDigiDetPar::~HRichDigiDetPar()
{
  clear(); 
}

Bool_t HRichDigiDetPar::initAscii(HParHadAsciiFileIo* pHadAsciiFile)
{
 if (!pHadAsciiFile) return kFALSE;

 HAsciiKey* pHAscii = pHadAsciiFile->GetKeyAscii();
 HAsciiKey &mydata = *pHAscii;
/* =============== this MUST be modified
 if (!mydata.SetActiveSection("Rich Digitalization Parameters"))
   return kFALSE;

 fThickDet = mydata.ReadFloat("thickDet");
 fFwhmResponse = mydata.ReadFloat("fwhmResponse");
 nMatrixRange = mydata.ReadInt("matrixRange");
 fBoxSize = mydata.ReadFloat("boxSize");
 fPlaneDist = mydata.ReadFloat("planeDist");
 nPadRange = mydata.ReadInt("padRange");
 fFactorInd = mydata.ReadFloat("factorInd");
 nNumElec = mydata.ReadInt("numElec");
 fVoltage = mydata.ReadFloat("voltage");
 fTrackThreshold = mydata.ReadFloat("trackThreshold");
 fRawThreshold = mydata.ReadFloat("rawThreshold");

  initChargeMatrix();
========================================*/
  return 1;
}


void HRichDigiDetPar::clear()
{
detNr = 0;
numElec = 0.;
fFactorInd = 0.;
if (chargeMatrix) {
 delete [] chargeMatrix;
 chargeMatrix = NULL;
}

binNr = 0;

if (photlen) {
 delete [] photlen;
 photlen = NULL;
}

if (photeff) {
 delete [] photeff;
 photeff = NULL;
}

} //end of destructor


float HRichDigiDetPar::calcCharge(float charge, float dist, 
  float Xd, float Yd, float Xu , float Yu) {

const float twoPI = 6.28318530718;

return ((fFactorInd*charge/twoPI)*(atan(Xd*Yd/(dist*sqrt(dist*dist+Xd*Xd+Yd*Yd)))
 - atan(Xd*Yu/(dist*sqrt(dist*dist+Xd*Xd+Yu*Yu))) 
 + atan(Xu*Yu/(dist*sqrt(dist*dist+Xu*Xu+Yu*Yu))) 
 - atan(Xu*Yd/(dist*sqrt(dist*dist+Xu*Xu+Yd*Yd)))));
} // eof


void HRichDigiDetPar::initChargeMatrix() {
  int i,j;
  float cornerX = -1*(nMatrixRange*fBoxSize + 0.5*fBoxSize);
  float cornerY = cornerX;

  chargeMatrix = new float[(2*nMatrixRange+1)*(2*nMatrixRange+1)];
  for (j=0; j<2*nMatrixRange+1; j++)
   for (i=0; i<2*nMatrixRange+1; i++)
        chargeMatrix[i+j*(2*nMatrixRange+1)] = calcCharge(1.,fPlaneDist,
        cornerX+i*fBoxSize, cornerY+j*fBoxSize,
        cornerX+(i+1)*fBoxSize, cornerY+(j+1)*fBoxSize);
}



void HRichDigiDetPar::Streamer(TBuffer &R__b)
{

// ============================ this MUST be changed
   // Stream an object of class HRichDigiDetPar.

   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(); if (R__v) { }
      HRichParSet::Streamer(R__b);
      // R__b >> fThickDet;
      // R__b >> fFwhmResponse;
      // R__b >> fFactorInd;
      // R__b >> nMatrixRange;

      // if (chargeMatrix) delete chargeMatrix;
      // chargeMatrix = new float[(2*nMatrixRange+1)*(2*nMatrixRange+1)];
      // R__b.ReadFastArray(chargeMatrix, (2*nMatrixRange+1)*(2*nMatrixRange+1));

      // R__b >> fPlaneDist;
      // R__b >> fBoxSize;
      // R__b >> nPadRange;
      // R__b >> nNumElec;
      // R__b >> fVoltage;
      // R__b >> fTrackThreshold;
      // R__b >> fRawThreshold;
   } else {
      R__b.WriteVersion(HRichDigiDetPar::IsA());
      HRichParSet::Streamer(R__b);
      // R__b << fThickDet;
      // R__b << fFwhmResponse;
      // R__b << fFactorInd;
      // R__b << nMatrixRange;
// 
      // R__b.WriteFastArray(chargeMatrix, (2*nMatrixRange+1)*(2*nMatrixRange+1));

      // R__b << fPlaneDist;
      // R__b << fBoxSize;
      // R__b << nPadRange;
      // R__b << nNumElec;
      // R__b << fVoltage;
      // R__b << fTrackThreshold;
      // R__b << fRawThreshold;
   }
}

