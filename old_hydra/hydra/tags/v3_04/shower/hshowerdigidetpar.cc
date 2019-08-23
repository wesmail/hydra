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
#include "hshowerdigidetpar.h"
#include "hparhadasciifileio.h"

ClassImp(HShowerDigiDetPar)

////////// ********** HShowerDigiDetPar defintion ****************/////////////
HShowerDigiDetPar::HShowerDigiDetPar() {
  SetName("ShowerDigiPar");
}

HShowerDigiDetPar::~HShowerDigiDetPar()
{
  clear(); 
}

Bool_t HShowerDigiDetPar::initAscii(HParHadAsciiFileIo* pHadAsciiFile)
{
 if (!pHadAsciiFile) return kFALSE;

 HAsciiKey* pHAscii = pHadAsciiFile->GetKeyAscii();
 HAsciiKey &mydata = *pHAscii;

 if (!mydata.SetActiveSection("Shower Digitalization Parameters"))
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

  return 1;
}

Bool_t HShowerDigiDetPar::writeAscii(HParHadAsciiFileIo* pHadAsciiFile)
{
  if (!pHadAsciiFile) return kFALSE;

  HAsciiKey* pHAscii = pHadAsciiFile->GetKeyAscii();
  HAsciiKey &mydata = *pHAscii;

  if (!mydata.WriteSection("Shower Digitalization Parameters"))
    return kFALSE;
 
  mydata.WriteFloat("thickDet", fThickDet);
  mydata.WriteFloat("fwhmResponse", fFwhmResponse);
  mydata.WriteInt("matrixRange", nMatrixRange);
  mydata.WriteFloat("boxSize", fBoxSize);
  mydata.WriteFloat("planeDist", fPlaneDist);
  mydata.WriteInt("padRange", nPadRange);
  mydata.WriteFloat("factorInd", fFactorInd);
  mydata.WriteInt("numElec", nNumElec);
  mydata.WriteFloat("voltage", fVoltage);
  mydata.WriteFloat("trackThreshold", fTrackThreshold);
  mydata.WriteFloat("rawThreshold", fRawThreshold);

  return 1;
}


void HShowerDigiDetPar::clear()
{
fFwhmResponse = 0.;
fThickDet = 0.;
fFactorInd = 0.;
fVoltage = 0.;
if (chargeMatrix) {
 delete [] chargeMatrix;
 chargeMatrix = NULL;
}
  nMatrixRange = 0;
  fPlaneDist = 0.0;
  fBoxSize = 0.0;
  nPadRange = 0;
  nNumElec = 0;
  fVoltage = 0.0;
  fTrackThreshold = 0.0;
  fRawThreshold = 0.0;

} //end of destructor

float HShowerDigiDetPar::calcCharge(float charge, float dist, 
  float Xd, float Yd, float Xu , float Yu) {

const float twoPI = 6.28318530718;

return ((fFactorInd*charge/twoPI)*(atan(Xd*Yd/(dist*sqrt(dist*dist+Xd*Xd+Yd*Yd)))
 - atan(Xd*Yu/(dist*sqrt(dist*dist+Xd*Xd+Yu*Yu))) 
 + atan(Xu*Yu/(dist*sqrt(dist*dist+Xu*Xu+Yu*Yu))) 
 - atan(Xu*Yd/(dist*sqrt(dist*dist+Xu*Xu+Yd*Yd)))));
} // eof

void HShowerDigiDetPar::initChargeMatrix() {
  int i,j;
  float cornerX = -1*(nMatrixRange*fBoxSize + 0.5*fBoxSize);
  float cornerY = cornerX;

  if (chargeMatrix) delete chargeMatrix;
  chargeMatrix = new float[(2*nMatrixRange+1)*(2*nMatrixRange+1)];
  for (j=0; j<2*nMatrixRange+1; j++)
   for (i=0; i<2*nMatrixRange+1; i++)
        chargeMatrix[i+j*(2*nMatrixRange+1)] = calcCharge(0.1, fPlaneDist,
        cornerX+i*fBoxSize, cornerY+j*fBoxSize,
        cornerX+(i+1)*fBoxSize, cornerY+(j+1)*fBoxSize);
}


//______________________________________________________________________________
void HShowerDigiDetPar::Streamer(TBuffer &R__b)
{
   // Stream an object of class HShowerDigiDetPar.

   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(); if (R__v) { }
      HShowerParSet::Streamer(R__b);
      R__b >> fThickDet;
      R__b >> fFwhmResponse;
      R__b >> fFactorInd;
      R__b >> nMatrixRange;
      
      if (chargeMatrix) delete chargeMatrix;
      chargeMatrix = new float[(2*nMatrixRange+1)*(2*nMatrixRange+1)];
      R__b.ReadFastArray(chargeMatrix, (2*nMatrixRange+1)*(2*nMatrixRange+1));
      
      R__b >> fPlaneDist;
      R__b >> fBoxSize;
      R__b >> nPadRange;
      R__b >> nNumElec;
      R__b >> fVoltage;
      R__b >> fTrackThreshold;
      R__b >> fRawThreshold;
   } else {
      R__b.WriteVersion(HShowerDigiDetPar::IsA());
      HShowerParSet::Streamer(R__b);
      R__b << fThickDet;
      R__b << fFwhmResponse;
      R__b << fFactorInd;
      R__b << nMatrixRange;

      R__b.WriteFastArray(chargeMatrix, (2*nMatrixRange+1)*(2*nMatrixRange+1));

      R__b << fPlaneDist;
      R__b << fBoxSize;
      R__b << nPadRange;
      R__b << nNumElec;
      R__b << fVoltage;
      R__b << fTrackThreshold;
      R__b << fRawThreshold;
   }
}

