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
 fSigmaValue = 0.;
 fChargePerChannel = 0.;
 fQupper = 0.0;
 isActiveNoise = 0.0;


 fFactor1 = 0.;
 fFactor2 = 0.;
 fParameter1 = 0.;
 fParameter2 = 0.;
 
 fExp1Nor = 0.;
 fExp1exp = 0.;
 fExp2Nor = 0.;
 fExp2exp = 0.;

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



//----------------------------------------------------------------------------
Bool_t HRichDigitisationPar::initAscii(HParHadAsciiFileIo* pHadAsciiFile) {

 using namespace HadAsciiAuxiliaryFunctions;
 if (!pHadAsciiFile) return kFALSE;

 Bool_t status = kTRUE;

 try {

  HAsciiKey &mydata = *pHadAsciiFile->GetKeyAscii();

  mydata.SetActiveSection("Rich Digitisation Parameters");

  fElectronsNr = mydata.ReadFloat("Mean number of electrons / keV");
  fSigmaValue = mydata.ReadFloat("Noise sigma value for gauss [e]");
  isActiveNoise = mydata.ReadInt("Electronic Noise Status");
  fChargePerChannel = mydata.ReadFloat("Charge [e] per one channel");
  fQupper = mydata.ReadFloat("Q upper limit");

  fFactor1 = mydata.ReadFloat("strong side coupling");
  fFactor2= mydata.ReadFloat("weak side coupling");
  fParameter1 = mydata.ReadFloat("vertical coupling : a");
  fParameter2 = mydata.ReadFloat("vertical coupling : b");
  
  fExp1Nor= mydata.ReadFloat("parameter 1 first exp");
  fExp1exp = mydata.ReadFloat("parameter 2 first exp");
  fExp2Nor = mydata.ReadFloat("parameter 1 second exp");
  fExp2exp = mydata.ReadFloat("parameter 2 second exp");
  
  mydata.SetActiveSection("CsI quantum efficiency for photons");

  fBinsNr = mydata.ReadInt("Number of bins");
  if (fBinsNr > 0) {
   fPhotonLen.Set(fBinsNr);
   fPhotonEff.Set(fBinsNr);
   mydata.ReadFloat(&fPhotonLen[0], "Cherenkov photon length [eV]");
   mydata.ReadFloat(&fPhotonEff[0], "CsI photoelectron extraction efficiency");
  }

 } // eof try block

 catch (Bool_t ret) {
  status = ret;
 }
 catch (...) {
  status = kFALSE;
  ErrorMsg(2,"HRichDigitisationPar::initAscii",1,"Unidentified exception catched.");
 }

 if (status)
  ErrorMsg(0,"HRichDigitisationPar::initAscii",
   1,"Container \'RichDigitisationParameters\' has been read from ASCII file.");

return status;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichDigitisationPar::writeAscii(HParHadAsciiFileIo* pHadAsciiFile) {

 using namespace HadAsciiAuxiliaryFunctions;
 if (!pHadAsciiFile) return kFALSE;

 Bool_t status = kTRUE;

 try {

  HAsciiKey &mydata = *pHadAsciiFile->GetKeyAscii();

  mydata.WriteSection("Rich Digitisation Parameters"); 

  mydata.WriteFloat("Mean number of electrons / keV",fElectronsNr);
  mydata.WriteFloat("Noise sigma value for gauss [e]",fSigmaValue);
  mydata.WriteInt("Electronic Noise Status",isActiveNoise);
  mydata.WriteFloat("Charge [e] per one channel",fChargePerChannel);
  mydata.WriteFloat("Q upper limit",fQupper);
  mydata.WriteRemark("these are charge limits used in Polya function",'#');
  mydata.WriteLine("\n");
  mydata.WriteFloat("strong side coupling",fFactor1);
  mydata.WriteFloat("weak side coupling",fFactor2);
  mydata.WriteFloat("vertical coupling : a",fParameter1);
  mydata.WriteFloat("vertical coupling : b",fParameter2);

  mydata.WriteFloat("parameter 1 first exp",fExp1Nor);
  mydata.WriteFloat("parameter 2 first exp",fExp1exp);
  mydata.WriteFloat("parameter 1 second exp",fExp2Nor);
  mydata.WriteFloat("parameter 2 second exp",fExp2exp);
  
  mydata.WriteSection("CsI quantum efficiency for photons");

  mydata.WriteInt("Number of bins",fBinsNr);
  mydata.SetNumInLine(6);
  mydata.WriteFloat("Cherenkov photon length [eV]*",&fPhotonLen);
  mydata.WriteFloat("CsI photoelectron extraction efficiency*",&fPhotonEff);

 } // eof try block

 catch (Bool_t ret) {
  status = ret;
 }
 catch (...) {
  status = kFALSE;
  ErrorMsg(2,"HRichDigitisationPar::writeAscii",1,"Unidentified exception catched.");
 }

 if (status)
  ErrorMsg(0,"HRichDigitisationPar::writeAscii",
   1,"Container \'RichDigitisationParameters\' has been written to ASCII file.");

 return status;
}
//============================================================================ 

void HRichDigitisationPar::printParam(){

 using namespace HadAsciiAuxiliaryFunctions;

 Char_t buf[5280], buf2[2280];

sprintf(buf,"Mean number of electrons / keV = %f$
             Charge [e] per one channel = %f$
             Noise sigma value for gauss [e] = %f$",
             fElectronsNr,
             fChargePerChannel,
             fSigmaValue);

 if (isActiveNoise) {
  sprintf(buf2,"Electronic Noise is switched ON"); 
 } else {
  sprintf(buf2,"Electronic Noise is switched OFF"); 
 }

 strconcat(buf,buf2);
 buf2[0] = '\0';

 ErrorMsg(0,"RICH DIGITISATION PARAMETERS",1,buf);
}






