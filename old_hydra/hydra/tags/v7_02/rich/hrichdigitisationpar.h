#ifndef HRICHDIGITISATIONPAR_H
#define HRICHDIGITISATIONPAR_H

#include "TObject.h"
#include "TArrayI.h"
#include "hparcond.h"
#include "hlocation.h"

class HParamList;
class HParHadAsciiFileIo;
class HDetParAsciiFileIo;
 
class HRichDigitisationPar: public HParCond {

public:

 // ---- general parameters ----
 
    Float_t fElectronsNr;//number of electron ionized in c4f10 for keV
    Float_t fSigmaValue;//sigma value for electronic noise
    Float_t fIncreaseNoise;//factor the noise has to be increased of
    Float_t fChargePerChannel;//number of electron for ADC channel
    Float_t fQupper;//upper limit for Polya function in ADC channel
    
    
    Float_t fParameter1;// parameters that describe the coupling of the charge on the wire to the pad plane.
    Float_t fParameter2;
    Float_t fFactor1;
    Float_t fFactor2;
    Float_t fFactor1Sig;
    Float_t fFactor2Sig;

   
    TArrayF fExpSlope;//slope of the exponential functions used to describe the photon charge distribution, 1 slope for each sector

   
 // ---- quantum efficiency ----

    Int_t fBinsNr;
    TArrayF fPhotonLen;
    TArrayF fPhotonEff;
    TArrayF corrSec[6];
    //    Float_t **corrSec;
    

private:

 char filename[40];

public:

    HRichDigitisationPar(const char* name="RichDigitisationParameters",
			 const char* title="Rich Digitisation Parameters",
			 const char* context="RichDigiNoiseOn");
    virtual ~HRichDigitisationPar();
    
    HRichDigitisationPar(const HRichDigitisationPar& source);
    HRichDigitisationPar& operator=(const HRichDigitisationPar& source);
    
    Bool_t init(HParIo*,Int_t*);
    Int_t write(HParIo*);
    virtual Bool_t initAscii(HParHadAsciiFileIo* pHadAsciiFile);
    virtual Bool_t writeAscii(HParHadAsciiFileIo* pHadAsciiFile);
    Bool_t readAscii(HDetParAsciiFileIo*);
    Int_t writeAscii(HDetParAsciiFileIo*);
    void putParams(HParamList*);
    Bool_t getParams(HParamList*);
    void clear();
    void printParam();
    
    void defaultInit();
    void setFileName(char* fn) { strncpy(filename, fn, 40); }
    
    
    Float_t getElectronsNr() { return fElectronsNr; }
    Float_t getChargePerChannel() { return fChargePerChannel; }
    Float_t getSigmaValue() { return fSigmaValue; }
    Float_t getIncreaseNoise() { return fIncreaseNoise; }
    Int_t getQEBinsNr() { return fBinsNr; }
    Float_t* getPhotonLenArray() { return &fPhotonLen[0]; }
    Float_t* getPhotonEffArray() { return &fPhotonEff[0]; }
    Float_t* getCorrectionParams(Int_t sector) {return &(corrSec[sector][0]);}

    Float_t getQupper() { return fQupper; }
    
    Float_t getFactor1() { return fFactor1; }
    Float_t getFactor2() { return fFactor2; }
    Float_t getFactor1Sig() { return fFactor1Sig; }
    Float_t getFactor2Sig() { return fFactor2Sig; }

    Float_t getParameter1() { return fParameter1; }
    Float_t getParameter2() { return fParameter2; }
    
   
    Float_t * getExpSlope() { return &fExpSlope[0];}
    
 ClassDef(HRichDigitisationPar,1)
};


#endif // HRICHDIGITISATIONPAR_H
