#ifndef HRICHCORRELATORPAR_H
#define HRICHCORRELATORPAR_H
#include "TObject.h"
#include "TArrayI.h"
#include "hparcond.h"
#include "hlocation.h"

class HParamList;
class HParHadAsciiFileIo;
class HDetParAsciiFileIo;
 
class HRichCorrelatorPar : public HParCond  {
  
 public:
  // switches 

 Int_t kShowerCond;// =1 if we ask for a shower in the PreShower det.
 Int_t kIsNoCutOnTOFtof;
 Int_t kIsNoCutOnTOFINOtof;
 Int_t kNoSectorSelected;
 Int_t kSwitchOffRich;
 Int_t kRingQualityCut;
 Int_t kTofinoMultCut;
 Int_t kMagnet;


  // Theta Phi cuts

  Float_t fRichMdcThetaCut; 
  Float_t fRichMdcThetaMinCut; 
  Float_t fRichMdcThetaMaxCut; 
  Float_t fRichTofThetaCut; 
  Float_t fRichTofThetaMinCut; 
  Float_t fRichTofThetaMaxCut;
  Float_t fRichShowerThetaCut; 
  Float_t fRichShowerThetaMinCut;
  Float_t fRichShowerThetaMaxCut;
  Float_t fMdcTofThetaCut; 
  Float_t fMdcTofThetaMinCut; 
  Float_t fMdcTofThetaMaxCut; 
  Float_t fMdcShowerThetaCut; 
  Float_t fMdcShowerThetaMinCut; 
  Float_t fMdcShowerThetaMaxCut; 
  Float_t fTofTofMinCut; 
  Float_t fTofinoTofMinCut; 
  Float_t fTofTofMaxCut; 
  Float_t fTofinoTofMaxCut; 
  Float_t fShowerAmplification;
  Float_t fRichMdcPhiCut; 
  Float_t fRichTofPhiCut; 
  Float_t fRichShowerPhiCut; 
  Float_t fMdcTofPhiCut; 
  Float_t fMdcShowerPhiCut; 
  Int_t   nTofinoMultCut;

  // MDC Theta Correction factor

  Float_t fMdcThetaCorrection;

  // Ring Quality cuts
  
  Int_t nPatMatCut;
  Int_t nHouTraCut;
  Int_t nPadNrCut;
  Int_t nRingAmplCut;
  Int_t nLocMax4Cut;

  //SECTOR SELECTION

  Int_t nSelectedSector; // from 0 to 5

 public:

  HRichCorrelatorPar(const char* name="RichCorrelatorParameters",
                  const char* title="Rich Correlator Parameters",
                  const char* context="RichCorrSharpCut");
  virtual ~HRichCorrelatorPar();
  // HRichCorrelatorPar(const HRichCorrelatorPar& source);
  //HRichCorrelatorPar& operator=(const HRichCorrelatorPar& source);
  
  Bool_t init(HParIo*,Int_t*);
  Int_t write(HParIo*);

  virtual Bool_t initAscii(HParHadAsciiFileIo* pHadAsciiFile);
  virtual Bool_t writeAscii(HParHadAsciiFileIo* pHadAsciiFile);
  Bool_t readAscii(HDetParAsciiFileIo*);
  Int_t writeAscii(HDetParAsciiFileIo*);

  void clear();
  void printParam();

  void putParams(HParamList*);
  Bool_t getParams(HParamList*);

    // Getters and Setters for the parameters

    Int_t getTofinoMultCut(){return nTofinoMultCut;}
    void setTofinoMultCut(Int_t m){nTofinoMultCut=m;}

    void setSelectedSector(Int_t s){nSelectedSector=s;}
    Int_t getSelectedSector(void){return nSelectedSector;}

    void setShowerCond(Int_t m){kShowerCond=m;}

    void setRichMdcThetaCut(Float_t c){fRichMdcThetaCut=c;};
    void setRichMdcPhiCut(Float_t c){fRichMdcPhiCut=c;};
    void setRichTofThetaCut(Float_t c){fRichTofThetaCut=c;};
    void setRichTofPhiCut(Float_t c){fRichTofPhiCut=c;};
    void setRichShowerThetaCut(Float_t c){fRichShowerThetaCut=c;};
    void setRichShowerPhiCut(Float_t c){fRichShowerPhiCut=c;};
    void setMdcShowerThetaCut(Float_t c){fMdcShowerThetaCut=c;};
    void setMdcShowerPhiCut(Float_t c){fMdcShowerPhiCut=c;};
    void setMdcTofThetaCut(Float_t c){fMdcTofThetaCut=c;};
    void setMdcTofPhiCut(Float_t c){fMdcTofPhiCut=c;};

    Float_t getRichMdcThetaCut(void){return fRichMdcThetaCut;};
    Float_t getRichMdcThetaMinCut(void){return fRichMdcThetaMinCut;};
    Float_t getRichMdcThetaMaxCut(void){return fRichMdcThetaMaxCut;};
    Float_t getRichMdcPhiCut(void){return fRichMdcPhiCut;};
    Float_t getRichTofThetaCut(void){return fRichTofThetaCut;};
    Float_t getRichTofThetaMinCut(void){return fRichTofThetaMinCut;};
    Float_t getRichTofThetaMaxCut(void){return fRichTofThetaMaxCut;};
    Float_t getRichTofPhiCut(void){return fRichTofPhiCut;};
    Float_t getRichShowerThetaCut(void){return fRichShowerThetaCut;};
    Float_t getRichShowerThetaMinCut(void){return fRichShowerThetaMinCut;};
    Float_t getRichShowerThetaMaxCut(void){return fRichShowerThetaMaxCut;};
    Float_t getRichShowerPhiCut(void){return fRichShowerPhiCut;};
    Float_t getMdcShowerThetaCut(void){return fMdcShowerThetaCut;};
    Float_t getMdcShowerThetaMinCut(void){return fMdcShowerThetaMinCut;};
    Float_t getMdcShowerThetaMaxCut(void){return fMdcShowerThetaMaxCut;};
    Float_t getMdcShowerPhiCut(void){return fMdcShowerPhiCut;};
    Float_t getMdcTofThetaCut(void){return fMdcTofThetaCut;};
    Float_t getMdcTofThetaMinCut(void){return fMdcTofThetaMinCut;};
    Float_t getMdcTofThetaMaxCut(void){return fMdcTofThetaMaxCut;};
    Float_t getMdcTofPhiCut(void){return fMdcTofPhiCut;};

    Int_t getRingPatMatCut(void){return nPatMatCut;};
    Int_t getRingHouTraCut(void){return nHouTraCut;};
    Int_t getRingPadNrCut(void){return nPadNrCut;};
    Int_t getRingAmplCut(void){return nRingAmplCut;};
    Int_t getRingLocMax4Cut(void){return nLocMax4Cut;};

    Float_t getMdcThetaCorrection(void){return fMdcThetaCorrection;};

    Int_t getShowerCond(void){return kShowerCond;};
    Int_t getTofSwitch(void){return kIsNoCutOnTOFtof;};
    Int_t getTofinoSwitch(void){return kIsNoCutOnTOFINOtof;};
    Int_t getSectorSelectionSwitch(void){return kNoSectorSelected;};
    Int_t getRichSwitch(void){return kSwitchOffRich;};
    Int_t getRingQualitySwitch(void){return kRingQualityCut;};
    Int_t getTofinoMultSwitch(void){return kTofinoMultCut;};
    Int_t getMagnetSwitch(void){return kMagnet;};

    Float_t getTofTofMinCut(void){return fTofTofMinCut;};
    Float_t getTofTofMaxCut(void){return fTofTofMaxCut;};
    Float_t getTofinoTofMinCut(void){return fTofinoTofMinCut;};
    Float_t getTofinoTofMaxCut(void){return fTofinoTofMaxCut;};
    
    Float_t getShowerAmplification(void){return fShowerAmplification;};
  ClassDef(HRichCorrelatorPar,1)
    };

#endif // HRICHCORRELATORPAR_H




