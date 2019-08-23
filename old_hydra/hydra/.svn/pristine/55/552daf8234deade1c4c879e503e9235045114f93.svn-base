using namespace std;
#include "hdetpario.h"
#include "hparhadasciifileio.h"
#include "hrichdigitisationpar.h"
#include "hparamlist.h"
#include "hdetparasciifileio.h"
#include <fstream> 
#include <iostream> 
#include <iomanip>
#include <string.h>

ClassImp(HRichDigitisationPar)


HRichDigitisationPar::HRichDigitisationPar(const char* name,const char* title,
                                   const char* context)
                    : HParCond(name,title,context) {
  strcpy(detName,"Rich");
  clear();
}


HRichDigitisationPar::~HRichDigitisationPar() {
 clear();
}


void HRichDigitisationPar::clear() {
 fElectronsNr = 0.0;
 fSigmaValue = 0.;
 fIncreaseNoise = 0.;
 fChargePerChannel = 0.;
 fQupper = 0.0;


 fFactor1 = 0.;
 fFactor2 = 0.;
 fFactor1Sig = 0.;
 fFactor2Sig = 0.;

 fParameter1 = 0.;
 fParameter2 = 0.;

 
 fExpSlope.Reset();
 

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


Bool_t HRichDigitisationPar::init(HParIo* inp,Int_t* set) {
  // initializes the container from an input using the RICH interface class
  HDetParIo* input=inp->getDetParIo("HRichParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}


Int_t HRichDigitisationPar::write(HParIo* output) {
  // writes the container to an output using the RICH interface class
  HDetParIo* out=output->getDetParIo("HRichParIo");
  if (out) return out->write(this);
  return kFALSE;
}

Bool_t HRichDigitisationPar::readAscii(HDetParAsciiFileIo* io) {
  return io->readCond(this);
}

Int_t HRichDigitisationPar::writeAscii(HDetParAsciiFileIo* io) {
  return io->writeCond(this);
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
  fSigmaValue = mydata.ReadFloat("Sigma Value used for Threshold [ADC chan]");
  fIncreaseNoise = mydata.ReadFloat("Multiplication factor for noise");
   fChargePerChannel = mydata.ReadFloat("Charge [e] per one channel");
  fQupper = mydata.ReadFloat("Q upper limit");

  fFactor1 = mydata.ReadFloat("strong side coupling");
  fFactor2= mydata.ReadFloat("weak side coupling");
  fFactor1Sig = mydata.ReadFloat("sigma for strong Coupling");
  fFactor2Sig = mydata.ReadFloat("sigma for weak Coupling");

  fParameter1 = mydata.ReadFloat("vertical coupling : a");
  fParameter2 = mydata.ReadFloat("vertical coupling : b");

  mydata.SetActiveSection("Gas Amplification for the 6 sectors");
 
  fExpSlope.Set(6);
 
  mydata.ReadFloat(&fExpSlope[0],"Slope exp");

 

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
  mydata.WriteFloat("Sigma Value used for Threshold [ADC chan]",fSigmaValue);
 mydata.WriteFloat("Multiplication factor for noise",fIncreaseNoise);
  mydata.WriteFloat("Charge [e] per one channel",fChargePerChannel);
  mydata.WriteFloat("Q upper limit",fQupper);
  mydata.WriteRemark("these are charge limits used in Polya function",'#');
  mydata.WriteLine("\n");
  mydata.WriteFloat("strong side coupling",fFactor1);
  mydata.WriteFloat("weak side coupling",fFactor2);
  mydata.WriteFloat("sigma for strong Coupling",fFactor1Sig);
  mydata.WriteFloat("sigma for weak Coupling",fFactor2Sig);
  mydata.WriteFloat("vertical coupling : a",fParameter1);
  mydata.WriteFloat("vertical coupling : b",fParameter2);

  mydata.WriteSection("Gas Amplification for the 6 sectors");
  mydata.SetNumInLine(6);

 
  mydata.WriteFloat("Slope exp*",&fExpSlope);

  
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

sprintf(buf,"Mean number of electrons / keV = %f$ \
             Charge [e] per one channel = %f$ \
             Sigma Value used for Threshold [ADC chan]= %f$",
             fElectronsNr,
             fChargePerChannel,
             fSigmaValue);
 

 strconcat(buf,buf2);
 buf2[0] = '\0';
 
 ErrorMsg(0,"RICH DIGITISATION PARAMETERS",1,buf);

 cout<<" fElectronsNr "<<fElectronsNr <<endl;
 cout<<" fSigmaValue "<< fSigmaValue<<endl;
 cout<<" fIncreaseNoise "<<fIncreaseNoise<<endl;
 cout<<" fChargePerChannel "<< fChargePerChannel<<endl;
 cout<<" fQupper "<< fQupper<<endl;
 cout<<" fParameter1 "<<fParameter1 <<endl;
 cout<<" fParameter2 "<< fParameter2<<endl;
 cout<<" fFactor1 "<< fFactor1<<endl;
 cout<<" fFactor2 "<< fFactor2<<endl;
 cout<<" fFactor1Sig "<< fFactor1Sig<<endl;
 cout<<" fFactor2Sig "<< fFactor2Sig<<endl;
 cout<<" fBinsNr "<<fBinsNr <<endl;



cout<<" fExpSlope "<<fExpSlope[0]<<"  "<<fExpSlope[1]<<"  "<<fExpSlope[2]<<"  "<<fExpSlope[3]<<"  "<<fExpSlope[4]<<"  "<<fExpSlope[5]<<endl;
     //cout<<" fPhotonLen"<< fPhotonLen<<endl;
 //cout<<" fPhotonEff"<< fPhotonEff<<endl;
}
//============================================================================ 

void HRichDigitisationPar::putParams(HParamList* l) {
  if (!l) return;
  l->add("fElectronsNr",fElectronsNr);
  l->add("fSigmaValue",fSigmaValue);
  l->add("fIncreaseNoise",fIncreaseNoise);
  l->add("fChargePerChannel",fChargePerChannel);
  l->add("fQupper",fQupper);
  l->add("fParameter1",fParameter1);
  l->add("fParameter2",fParameter2,6);
  l->add("fFactor1",fFactor1);
  l->add("fFactor2",fFactor2);
  l->add("fFactor1Sig",fFactor1Sig);
  l->add("fFactor2Sig",fFactor2Sig);
  l->add("fExpSlope",fExpSlope,6);
  l->add("fBinsNr",fBinsNr);
  l->add("fPhotonLen",fPhotonLen);
  l->add("fPhotonEff",fPhotonEff,6);
}
//============================================================================

Bool_t HRichDigitisationPar::getParams(HParamList* l) {
  if (!l) return kFALSE;
  if (!l->fill("fElectronsNr",&fElectronsNr)) return kFALSE;
  if (!l->fill("fSigmaValue",&fSigmaValue)) return kFALSE;
  if (!l->fill("fIncreaseNoise",&fIncreaseNoise)) return kFALSE;
  if (!l->fill("fChargePerChannel",&fChargePerChannel)) return kFALSE;
  if (!l->fill("fQupper",&fQupper)) return kFALSE;
  if (!l->fill("fParameter1",&fParameter1)) return kFALSE;
  if (!l->fill("fParameter2",&fParameter2)) return kFALSE;
  if (!l->fill("fFactor1",&fFactor1)) return kFALSE;
  if (!l->fill("fFactor2",&fFactor2)) return kFALSE;
  if (!l->fill("fFactor1Sig",&fFactor1Sig)) return kFALSE;
  if (!l->fill("fFactor2Sig",&fFactor2Sig)) return kFALSE;
  if (!l->fill("fExpSlope",&fExpSlope)) return kFALSE;
  if (!l->fill("fBinsNr",&fBinsNr)) return kFALSE;
  if (!l->fill("fPhotonLen",&fPhotonLen)) return kFALSE;
  if (!l->fill("fPhotonEff",&fPhotonEff)) return kFALSE;
  return kTRUE;
}





