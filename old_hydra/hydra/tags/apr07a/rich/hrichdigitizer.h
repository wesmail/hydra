#ifndef HRICHDIGITISER_H
#define HRICHDIGITISER_H

#include "TList.h"
#include "TH1.h"
#include "TFile.h"
#include "hreconstructor.h"
#include "hlocation.h"
#include "hparset.h"
#include "hcategory.h"
#include <TVector.h>
#include <TRandom.h>
#include <TF1.h>
class HCategory;
class HIterator;
class HGeantRichPhoton;
class HGeantRichDirect;
class HRichPad;
class HRichTrack;
class HRichCalSim; // change ETLF 02/23/00

class HRichDigitizer : public HReconstructor {

public:

    HRichDigitizer();
    HRichDigitizer(Text_t *name,Text_t *title, Bool_t kNoise=kFALSE,Bool_t oem=kFALSE);
    ~HRichDigitizer();
    
    HRichDigitizer(const HRichDigitizer& source);
    HRichDigitizer &operator=(HRichDigitizer &source);
    
    Bool_t init();
    Bool_t reinit();
    Int_t execute();
    //Bool_t finalize() { return kTRUE; };
    Bool_t finalize();
  
    
    HCategory* getCerInputCat() { return fCerInputCat; }
    HCategory* getDirInputCat() { return fDirInputCat; }
    HCategory* getOutputCat() { return fOutputCat; }
    HCategory* getTrackOutputCat() { return fTrackOutputCat;}

    HParSet* getRichCalPar() { return fCalPar;}
    HParSet* getDigitisationPar() { return fDigitisationPar; }
    HParSet* getGeometryPar() { return fGeometryPar; }
    
    
    void setCerInputCat(HCategory* pInCat) { fCerInputCat = pInCat; }
    void setDirInputCat(HCategory* pInCat) { fDirInputCat = pInCat; }
    void setOutputCat(HCategory* pOutCat) { fOutputCat = pOutCat; }
    void setTrackOutputCat(HCategory* pOutCat){ fTrackOutputCat =pOutCat;} 
    
    
    void setDigitisationPar(HParSet* pPar) { fDigitisationPar = pPar; }
    void setGeometryPar(HParSet* pPar) { fGeometryPar = pPar; }
    void setCalPar(HParSet* pPar) { fCalPar = pPar; } 
    
    Int_t checkPad(HRichCalSim *calSim);
    
    ClassDef(HRichDigitizer,0) // Rich digitizer
	
    Int_t lNrEvent;
    
private:
    
    HCategory *fCerInputCat; //Pointer to the raw data category
    HCategory *fDirInputCat; //Pointer to the raw data category
    HCategory *fOutputCat;   //Pointer to the cal data category
    HCategory *fTrackOutputCat;//Pointer to the richtrack category        
    
    HParSet* fDigitisationPar; //Pointer to digitisation parameters
    HParSet* fGeometryPar; //Pointer to geometry parameters
    HParSet* fCalPar;//Pointer to calibration parameters

    HIterator *fCerIter;     //!
    HIterator *fDirIter;     //!
    HIterator *fTrackIter;   //!           
    HIterator *fCalIter;     //!

    Float_t GaussFun(Float_t mean, Float_t sigma);
 
    Float_t calcIndCharge(float yCharge,float q4,int iPdaIndex,int iWireNr);
    Float_t qX(float pos);
    Float_t q4Calc(float charge,float pos,float par1,float par2);

    Bool_t calcQE(const Float_t photlen, Int_t sec);
    Int_t getWireNr(Float_t xhit);
    Float_t calcChargeOnWire(Int_t sector, Float_t xhit, Float_t yhit,
			  Float_t nTrack,Float_t nFlag,Float_t ene);
    // void digitiseCherenkovHits(HGeantRichPhoton *pCerHit);
    void digitiseCherenkovHits(HGeantRichPhoton *pCerHit,Int_t count);
    void digitiseDirectHits(HGeantRichDirect *pDirHit);

     Bool_t calcFeedBack(Int_t sec,Float_t xhit, Float_t yhit,Float_t &ene,Float_t &xhittFB,Float_t &yhittFB,Float_t charge);
    void processPhoton(Float_t ene,Float_t xPos,Float_t yPos,Int_t track,Int_t sector);
    Float_t feedBackProb(Float_t x);
    HRichPad* translateCorners(HRichPad *pPad, Float_t dx, Float_t dy);
    void makeNoiseOnPads();
    void addNoiseToCharge(HRichCalSim* calSim);
    Float_t calcNoiseOnPad(Float_t sigma,Float_t floatMean);
    void digitisePads();
    void updateCharge(Int_t sector, HRichPad* pPad, Float_t charge,
		      TVector * rTrack, Float_t ene);
    
    void updateTrack(HRichCalSim *pCalSim,HLocation & loc,
		     TVector * rTrack);
    
 
    Float_t fQupper;//!
    Float_t fChargePerChannel;//!
    Float_t fChargeScaling;//!
    Float_t fDigitPadMatrix[9];//!
    Float_t noiseCharge[1000];//!
    TList fChargeOnWireList; //!//holds charge on a wire
    TList fTrackNumberList;  //!// holds track number and flag for a hit
    HLocation loc;
    
    Int_t isActiveNoise; // flag to switch on/off the electronic noise
    Int_t isOEM; //flag to select oem analysis.
    Int_t countNoisePad; // number of pads with a noise cont. above threshold.
    Float_t fYShift;     // shift on the y pad plane coordinate necessary to get the correct pad position and the correct theta.
    Float_t distWirePads; // distance between wires and pad plane. in cm
    Float_t fIncreaseNoise;
    Float_t fSigmaValue;
    
    Float_t fMeanPad; // mean value of the noise distribution 
    Float_t fSigmaPad; //sigma of the noise distribution 
    Float_t fFloatMean; //float part of the mean value
    Float_t fCharge;

    Float_t fFactor1, fFactor2;
    Float_t fFactor1Sig, fFactor2Sig;
    Float_t fParam1, fParam2;
    Float_t param21;
    Float_t param11;
    Float_t fInt ;
    Float_t a1,a2,b1,b2;
    TFile *output;
    TH1F *position;
    TF1 *ga;//! Gauss function
    Int_t cont,countFBphot;

    Int_t countPhotSec[6];// to be removed!
};

#endif // HRICHDIGITISER_H 







