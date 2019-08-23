#ifndef HRICHDIGITISATIONPAR_H
#define HRICHDIGITISATIONPAR_H

#include "TObject.h"
#include "TArrayI.h"
#include "hparset.h"
#include "hrichpad.h"
#include "hrichparset.h"
#include "hlocation.h"


class HRichDigitisationPar : public HRichParSet {

public:

 // ---- general parameters ----
 
    Float_t fElectronsNr;//number of electron ionized in c4f10 for keV
    Float_t fSigmaValue;//sigma value for electronic noise
    Float_t fChargePerChannel;//number of electron for ADC channel
    Float_t fQupper;//upper limit for Polya function in ADC channel
    
    
    Float_t fParameter1;// parameters that describe the coupling of the charge on the wire to the pad plane.
    Float_t fParameter2;
    Float_t fFactor1;
    Float_t fFactor2;

    Float_t fExp1Nor;//coefficients of the 2 exponential functions used to describe the photon charge distribution
    Float_t fExp1exp;
    Float_t fExp2Nor;
    Float_t fExp2exp;

    Int_t isActiveNoise; //flag to switch on, off the electronic noise 

 // ---- quantum efficiecy ----

    Int_t fBinsNr;
    TArrayF fPhotonLen;
    TArrayF fPhotonEff;

private:

 char filename[40];

public:

 HRichDigitisationPar();
 virtual ~HRichDigitisationPar();
 
 HRichDigitisationPar(const HRichDigitisationPar& source);
 HRichDigitisationPar& operator=(const HRichDigitisationPar& source);

 virtual Bool_t initAscii(HParHadAsciiFileIo* pHadAsciiFile);
 virtual Bool_t writeAscii(HParHadAsciiFileIo* pHadAsciiFile);
 void clear();
 void printParam();

 void defaultInit();
 void setFileName(char* fn) { strncpy(filename, fn, 40); }

 
 Float_t getElectronsNr() { return fElectronsNr; }
 Float_t getChargePerChannel() { return fChargePerChannel; }
 Float_t getSigmaValue() { return fSigmaValue; }
    Int_t getActiveNoise() { return isActiveNoise; }
 Int_t getQEBinsNr() { return fBinsNr; }
 Float_t* getPhotonLenArray() { return &fPhotonLen[0]; }
 Float_t* getPhotonEffArray() { return &fPhotonEff[0]; }
 
 Float_t getQupper() { return fQupper; }

 Float_t getFactor1() { return fFactor1; }
 Float_t getFactor2() { return fFactor2; }
 
 Float_t getParameter1() { return fParameter1; }
 Float_t getParameter2() { return fParameter2; }

    Float_t  getExp1Nor() { return fExp1Nor; }
    Float_t  getExp1exp() { return fExp1exp; }
    Float_t  getExp2Nor() { return fExp2Nor; }
    Float_t  getExp2exp() { return fExp2exp;}
				
 ClassDef(HRichDigitisationPar,1)
};


#endif // HRICHDIGITISATIONPAR_H
