#include "hparhadasciifileio.h"
#include "hrichdigitisationpar.h"
#include <fstream.h>
#include <iostream.h>
#include <string.h>

ClassImp(HRichDigitisationPar)



HRichDigitisationPar::HRichDigitisationPar() {

 SetName("RichDigitisationParameters");
 clear();
}


HRichDigitisationPar::~HRichDigitisationPar() {
 clear();
}



void HRichDigitisationPar::clear() {
 fElectronsNr = 0.0;
 fRichPadRange = 0;
 fMatrixPadRange = 0;
 fMatrixPadSize = 0.0;
 fFactorIndex = 0.0;
 fVoltage = 0;
 fSigmaValue = 0.;
 fChargePerChannel = 0.;
 fQlower = 0.0;
 fQupper = 0.0;

 fFactor1 = 0.;
 fParameter1 = 0.;
 fParameter2 = 0.;
 fFactor2 = 0.;
 fFactor3 = 0.;
 fFactor4 = 0.;
 fFactor5 = 0.;
 fFactor6 = 0.;
 fFactor7 = 0.;
 fFactor8 = 0.;

 fBinsNr = 0;
 fPhotonLen.Reset();
 fPhotonEff.Reset();
}


void HRichDigitisationPar::defaultInit() {
// so far dummy
 clear();
}



HRichDigitisationPar::HRichDigitisationPar(const HRichDigitisationPar& source) {
 cerr << "HRichDigitisationPar object can not be initialized"
         " with values of another object!\n";
 cerr << "Default constructor will be called.\n";
 HRichDigitisationPar();
}


HRichDigitisationPar& HRichDigitisationPar::operator=(const HRichDigitisationPar& source) {
 if (this != &source) {
  cerr << "HRichDigitisationPar object can not be assigned!\n";
  cerr << "Default constructor will be called.\n";
  HRichDigitisationPar();
 }
return *this;
}




Bool_t HRichDigitisationPar::initAscii(HParHadAsciiFileIo* pHadAsciiFile)
{
 // Int_t i, j;

 if (!pHadAsciiFile) return kFALSE;

 HAsciiKey* pHAscii = pHadAsciiFile->GetKeyAscii();
 HAsciiKey &mydata = *pHAscii;

 if (mydata.SetActiveSection("Rich Digitisation Parameters") == kFALSE)
  return kFALSE;

  fElectronsNr = mydata.ReadFloat("Mean number of electrons / keV");
  fRichPadRange = mydata.ReadInt("Pad range for digitisation");
  fMatrixPadRange = mydata.ReadInt("Matrix pad range for digitisation");
  fMatrixPadSize = mydata.ReadFloat("Matrix pad size in pattern matrix [cm]");
  fFactorIndex = mydata.ReadFloat("Factor index of charge coupling to pads");
  fSigmaValue = mydata.ReadFloat("Noise sigma value for gauss [e]");
  fChargePerChannel = mydata.ReadFloat("Charge [e] per one channel");
  fVoltage = mydata.ReadInt("Voltage on anode wire [V]");
  if (fVoltage < 2100 || fVoltage > 2600) {
   cerr << "Error in <HRichDigitisationPar::initAscii>: "
           " parameter Voltaga is out of range (2100-2600 V)!\n";
   exit(1);
  }
  fQlower = mydata.ReadFloat("Q lower limit");
  fQupper = mydata.ReadFloat("Q upper limit");

  fFactor1 = mydata.ReadFloat("Pad 4 factor");
  fParameter1 = mydata.ReadFloat("Pad 1 and 7 parameter a");
  fParameter2 = mydata.ReadFloat("Pad 1 and 7 parameter b");
  fFactor2 = mydata.ReadFloat("Pad 1 and 7 factor");
  fFactor3 = mydata.ReadFloat("Pad 3 and 5 factor 1");
  fFactor4 = mydata.ReadFloat("Pad 3 and 5 factor 2");
  fFactor5 = mydata.ReadFloat("Pad 0 and 2 and 6 and 8 factor 1");
  fFactor6 = mydata.ReadFloat("Pad 0 and 2 and 6 and 8 factor 2");
  fFactor7 = mydata.ReadFloat("Pad 0 and 2 and 6 and 8 factor 3");
  fFactor8 = mydata.ReadFloat("Pad 0 and 2 and 6 and 8 factor 4");


 if (mydata.SetActiveSection("CsI quantum efficiency for photons") == kFALSE)
  return kFALSE;

  fBinsNr = mydata.ReadInt("Number of bins");
  if (fBinsNr > 0) {
   fPhotonLen.Set(fBinsNr);
   fPhotonEff.Set(fBinsNr);
   mydata.ReadFloat(&fPhotonLen[0], "Cherenkov photon length [eV]");
   mydata.ReadFloat(&fPhotonEff[0], "CsI photoelectron extraction efficiency");
  }

return kTRUE;
}

//----------------------------------------------------------------------------
Bool_t HRichDigitisationPar::writeAscii(HParHadAsciiFileIo* pHadAsciiFile)
{
 if (!pHadAsciiFile) return kFALSE;

 HAsciiKey* pHAscii = pHadAsciiFile->GetKeyAscii();
 HAsciiKey &mydata = *pHAscii;

 if (!mydata.WriteSection("Rich Digitisation Parameters")) 
     return kFALSE;

 mydata.WriteInt("Mean number of electrons / keV",fElectronsNr);
 mydata.WriteInt("Pad range for digitisation",fRichPadRange);
 mydata.WriteRemark("this pad range means that the area of 2*n+1 vs 2*n+1 is digitised",'#');
 mydata.WriteInt("Matrix pad range for digitisation",fMatrixPadRange);
 mydata.WriteFloat("Matrix pad size in pattern matrix [cm]",fMatrixPadSize);
 mydata.WriteFloat("Factor index of charge coupling to pads",fFactorIndex);
 mydata.WriteFloat("Noise sigma value for gauss [e]",fSigmaValue);
 mydata.WriteFloat("Charge [e] per one channel",fChargePerChannel);
 mydata.WriteInt("Voltage on anode wire [V]",fVoltage);
 mydata.WriteFloat("Q lower limit",fQlower);
 mydata.WriteFloat("Q upper limit",fQupper);
 mydata.WriteRemark("these are charge limits used in Polya function",'#');
 mydata.WriteLine("\n");
 mydata.WriteFloat("Pad 4 factor",fFactor1);
 mydata.WriteFloat("Pad 1 and 7 parameter a",fParameter1);
 mydata.WriteFloat("Pad 1 and 7 parameter b",fParameter2);
 mydata.WriteFloat("Pad 1 and 7 factor",fFactor2);
 mydata.WriteFloat("Pad 3 and 5 factor 1",fFactor3);
 mydata.WriteFloat("Pad 3 and 5 factor 2",fFactor4);
 mydata.WriteFloat("Pad 0 and 2 and 6 and 8 factor 1",fFactor5);
 mydata.WriteFloat("Pad 0 and 2 and 6 and 8 factor 2",fFactor6);
 mydata.WriteFloat("Pad 0 and 2 and 6 and 8 factor 3",fFactor7);
 mydata.WriteFloat("Pad 0 and 2 and 6 and 8 factor 4",fFactor8);

 if (!mydata.WriteSection("CsI quantum efficiency for photons"))
     return kFALSE;

 mydata.WriteInt("Number of bins",fBinsNr);
 mydata.SetNumInLine(6);
 mydata.WriteFloat("Cherenkov photon length [eV]*",&fPhotonLen);
 mydata.WriteFloat("CsI photoelectron extraction efficiency*",&fPhotonEff);

 return kTRUE;
}
//============================================================================ 
