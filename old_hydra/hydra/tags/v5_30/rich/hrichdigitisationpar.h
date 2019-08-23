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
 
 Float_t fElectronsNr;
 Int_t fRichPadRange;
 Int_t fMatrixPadRange;
 Float_t fMatrixPadSize;
 Float_t fFactorIndex;
 Float_t fSigmaValue;
 Float_t fChargePerChannel;
 Int_t fVoltage;
 Float_t fQlower;
 Float_t fQupper;

 Float_t fFactor1;
 Float_t fParameter1;
 Float_t fParameter2;
 Float_t fFactor2;
 Float_t fFactor3;
 Float_t fFactor4;
 Float_t fFactor5;
 Float_t fFactor6;
 Float_t fFactor7;
 Float_t fFactor8;

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

 void defaultInit();
 void setFileName(char* fn) { strncpy(filename, fn, 40); }

 
 Float_t getElectronsNr() { return fElectronsNr; }
 Int_t getPadRange() { return fRichPadRange; }
 Int_t getVoltage() { return fVoltage; }
 Float_t getChargePerChannel() { return fChargePerChannel; }
 Float_t getCouplingFactor() { return fFactorIndex; }
 Float_t getSigmaValue() { return fSigmaValue; }
 Int_t getMatrixPadRange() { return fMatrixPadRange; }
 Float_t getMatrixPadSize() { return fMatrixPadSize; }
 Int_t getQEBinsNr() { return fBinsNr; }
 Float_t* getPhotonLenArray() { return &fPhotonLen[0]; }
 Float_t* getPhotonEffArray() { return &fPhotonEff[0]; }
 Float_t getQlower() { return fQlower; }
 Float_t getQupper() { return fQupper; }

 Float_t getFactor1() { return fFactor1; }
 Float_t getFactor2() { return fFactor2; }
 Float_t getFactor3() { return fFactor3; }
 Float_t getFactor4() { return fFactor4; }
 Float_t getFactor5() { return fFactor5; }
 Float_t getFactor6() { return fFactor6; }
 Float_t getFactor7() { return fFactor7; }
 Float_t getFactor8() { return fFactor8; }
 Float_t getParameter1() { return fParameter1; }
 Float_t getParameter2() { return fParameter2; }

 ClassDef(HRichDigitisationPar,1)
};


#endif // HRICHDIGITISATIONPAR_H
