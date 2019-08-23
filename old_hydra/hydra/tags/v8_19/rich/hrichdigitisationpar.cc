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


HRichDigitisationPar::HRichDigitisationPar(const Char_t* name,const Char_t* title,
                                   const Char_t* context)
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
 fChargeScaling = 1.0;
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

 
 for(Int_t i=0;i<6;i++){
     corrSec[i].Reset();
 }

 
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

  for(Int_t i=0;i<6;i++) corrSec[i].Set(fBinsNr);
   mydata.ReadFloat(&corrSec[0][0], "Efficiency correction sector 0");
   mydata.ReadFloat(&corrSec[1][0], "Efficiency correction sector 1");
   mydata.ReadFloat(&corrSec[2][0], "Efficiency correction sector 2");
   mydata.ReadFloat(&corrSec[3][0], "Efficiency correction sector 3");
   mydata.ReadFloat(&corrSec[4][0], "Efficiency correction sector 4");
   mydata.ReadFloat(&corrSec[5][0], "Efficiency correction sector 5");

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
  //Float_t * s1=(Float_t*) *corrSec;
  mydata.WriteFloat("Efficiency correction sector 0*",&corrSec[0]);
  mydata.WriteFloat("Efficiency correction sector 1",&corrSec[1]);
   mydata.WriteFloat("Efficiency correction sector 2",&corrSec[2]);
   mydata.WriteFloat("Efficiency correction sector 3",&corrSec[3]);
  mydata.WriteFloat("Efficiency correction sector 4",&corrSec[4]);
   mydata.WriteFloat("Efficiency correction sector 5",&corrSec[5]);
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

 Char_t buf[5280];

sprintf(buf,"Mean number of electrons / keV = %f$"
            "Charge [e] per one channel = %f$"
            "Sigma Value used for Threshold [ADC chan]= %f$",
             fElectronsNr,
             fChargePerChannel,
             fSigmaValue);
 
 
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
 cout<<" fChargeScaling "<<fChargeScaling<<endl;


cout<<" fExpSlope "<<fExpSlope[0]<<"  "<<fExpSlope[1]<<"  "<<fExpSlope[2]<<"  "<<fExpSlope[3]<<"  "<<fExpSlope[4]<<"  "<<fExpSlope[5]<<endl;
cout<<"  correction  sector 0:  ";
 for(Int_t i = 0;i<18;i++){
     cout<< corrSec[0][i]<<"  ";
}
 cout<<endl;
cout<<"  correction  sector 1:  ";
 for(Int_t i = 0;i<18;i++){
     cout<< corrSec[1][i]<<"  ";
}
 cout<<endl;
cout<<"  correction  sector 2:  ";
 for(Int_t i = 0;i<18;i++){
     cout<< corrSec[2][i]<<"  ";
}
 cout<<endl;
cout<<"  correction  sector 3:  ";
 for(Int_t i = 0;i<18;i++){
     cout<< corrSec[3][i]<<"  ";
}
 cout<<endl;
cout<<"  correction  sector 4:  ";
 for(Int_t i = 0;i<18;i++){
     cout<< corrSec[4][i]<<"  ";
}
 cout<<endl;
cout<<"  correction  sector 5:  ";
 for(Int_t i = 0;i<18;i++){
     cout<< corrSec[5][i]<<"  ";
}
 cout<<endl;
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
  if(this->Class_Version()>1)
    {
      l->add("fChargeScaling",fChargeScaling);
    }
  l->add("fBinsNr",fBinsNr);
  l->add("fPhotonLen",fPhotonLen);
  l->add("fPhotonEff",fPhotonEff,6);
 
  l->add("corrSec0",corrSec[0],6);
  
  l->add("corrSec1",corrSec[1],6);
  l->add("corrSec2",corrSec[2],6);
  l->add("corrSec3",corrSec[3],6);
  l->add("corrSec4",corrSec[4],6);
  l->add("corrSec5",corrSec[5],6);

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

  if (!l->fill("fChargeScaling",&fChargeScaling)) 
    {
      if(this->Class_Version()<2) //For classversions below 2 we use a reasonable default!
	{
	  fChargeScaling=1.0;
	}
      else //All classversions above 1 MUST supply a value for this parameter!
	{
	  return kFALSE;
	}
    }
  if (!l->fill("fBinsNr",&fBinsNr)) return kFALSE;
  if (!l->fill("fPhotonLen",&fPhotonLen)) return kFALSE;
  if (!l->fill("fPhotonEff",&fPhotonEff)) return kFALSE;
  if (!l->fill("corrSec0",&corrSec[0])) return kFALSE;
  if (!l->fill("corrSec1",&corrSec[1])) return kFALSE;
  if (!l->fill("corrSec2",&corrSec[2])) return kFALSE;
  if (!l->fill("corrSec3",&corrSec[3])) return kFALSE;
  if (!l->fill("corrSec4",&corrSec[4])) return kFALSE;
  if (!l->fill("corrSec5",&corrSec[5])) return kFALSE;
  return kTRUE;
}

void HRichDigitisationPar::Streamer(TBuffer &R__b)
{
   // Stream an object of class HRichDigitisationPar.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      HParCond::Streamer(R__b);
      R__b >> fElectronsNr;
      R__b >> fSigmaValue;
      R__b >> fIncreaseNoise;
      R__b >> fChargePerChannel;
      R__b >> fQupper;
      R__b >> fParameter1;
      R__b >> fParameter2;
      R__b >> fFactor1;
      R__b >> fFactor2;
      R__b >> fFactor1Sig;
      R__b >> fFactor2Sig;
      fExpSlope.Streamer(R__b);
      R__b >> fBinsNr;
      fPhotonLen.Streamer(R__b);
      fPhotonEff.Streamer(R__b);
      Int_t R__i;
      for (R__i = 0; R__i < 6; R__i++)
         corrSec[R__i].Streamer(R__b);
      if (R__v>1)
	{
	  R__b >> fChargeScaling;
	}
      R__b.ReadStaticArray((Char_t*)filename);
      R__b.CheckByteCount(R__s, R__c, HRichDigitisationPar::IsA());
   } else {
      R__c = R__b.WriteVersion(HRichDigitisationPar::IsA(), kTRUE);
      HParCond::Streamer(R__b);
      R__b << fElectronsNr;
      R__b << fSigmaValue;
      R__b << fIncreaseNoise;
      R__b << fChargePerChannel;
      R__b << fQupper;
      R__b << fParameter1;
      R__b << fParameter2;
      R__b << fFactor1;
      R__b << fFactor2;
      R__b << fFactor1Sig;
      R__b << fFactor2Sig;
      fExpSlope.Streamer(R__b);
      R__b << fBinsNr;
      fPhotonLen.Streamer(R__b);
      fPhotonEff.Streamer(R__b);
      Int_t R__i;
      for (R__i = 0; R__i < 6; R__i++)
         corrSec[R__i].Streamer(R__b);
      //cout << "Streaming: " << fChargeScaling << endl;
      R__b << fChargeScaling;
      R__b.WriteArray(filename, 40);
      R__b.SetByteCount(R__c, kTRUE);
   }
}

