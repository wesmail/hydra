#include "hdetpario.h"
#include "hparhadasciifileio.h"
#include "hrichdigitisationpar.h"
#include "hparamlist.h"
#include "hdetparasciifileio.h"
#include <fstream.h>
#include <iostream.h>
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
 fChargePerChannel = 0.;
 fQupper = 0.0;
 isActiveNoise = 0;


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

 cout<<" fElectronsNr "<<fElectronsNr <<endl;
 cout<<" fSigmaValue "<< fSigmaValue<<endl;
 cout<<" fChargePerChannel "<< fChargePerChannel<<endl;
 cout<<" fQupper "<< fQupper<<endl;
 cout<<" fParameter1 "<<fParameter1 <<endl;
 cout<<" fParameter2 "<< fParameter2<<endl;
 cout<<" fFactor1 "<< fFactor1<<endl;
 cout<<" fFactor2 "<< fFactor2<<endl;
 cout<<" fExp1Nor "<<fExp1Nor <<endl;
 cout<<" fExp1exp "<< fExp1exp<<endl;
 cout<<" fExp2Nor "<<fExp2Nor <<endl;
 cout<<" fExp2exp "<< fExp2exp<<endl;
 cout<<" isActiveNoise "<< isActiveNoise<<endl;
 cout<<" fBinsNr "<<fBinsNr <<endl;
 //cout<<" fPhotonLen"<< fPhotonLen<<endl;
 //cout<<" fPhotonEff"<< fPhotonEff<<endl;
}
//============================================================================ 

void HRichDigitisationPar::putParams(HParamList* l) {
  if (!l) return;
  l->add("fElectronsNr",fElectronsNr);
  l->add("fSigmaValue",fSigmaValue);
  l->add("fChargePerChannel",fChargePerChannel);
  l->add("fQupper",fQupper);
  l->add("fParameter1",fParameter1);
  l->add("fParameter2",fParameter2,6);
  l->add("fFactor1",fFactor1);
  l->add("fFactor2",fFactor2);
  l->add("fExp1Nor",fExp1Nor);
  l->add("fExp1exp",fExp1exp,6);
  l->add("fExp2Nor",fExp2Nor);
  l->add("fExp2exp",fExp2exp,6);
  l->add("isActiveNoise",isActiveNoise);
  l->add("fBinsNr",fBinsNr);
  l->add("fPhotonLen",fPhotonLen);
  l->add("fPhotonEff",fPhotonEff,6);
}
//============================================================================

Bool_t HRichDigitisationPar::getParams(HParamList* l) {
  if (!l) return kFALSE;
  if (!l->fill("fElectronsNr",&fElectronsNr)) return kFALSE;
  if (!l->fill("fSigmaValue",&fSigmaValue)) return kFALSE;
  if (!l->fill("fChargePerChannel",&fChargePerChannel)) return kFALSE;
  if (!l->fill("fQupper",&fQupper)) return kFALSE;
  if (!l->fill("fParameter1",&fParameter1)) return kFALSE;
  if (!l->fill("fParameter2",&fParameter2)) return kFALSE;
  if (!l->fill("fFactor1",&fFactor1)) return kFALSE;
  if (!l->fill("fFactor2",&fFactor2)) return kFALSE;
  if (!l->fill("fExp1Nor",&fExp1Nor)) return kFALSE;
  if (!l->fill("fExp1exp",&fExp1exp)) return kFALSE;
  if (!l->fill("fExp2Nor",&fExp2Nor)) return kFALSE;
  if (!l->fill("fExp2exp",&fExp2exp)) return kFALSE;
  if (!l->fill("isActiveNoise",&isActiveNoise)) return kFALSE;
  if (!l->fill("fBinsNr",&fBinsNr)) return kFALSE;
  if (!l->fill("fPhotonLen",&fPhotonLen)) return kFALSE;
  if (!l->fill("fPhotonEff",&fPhotonEff)) return kFALSE;
  return kTRUE;
}





