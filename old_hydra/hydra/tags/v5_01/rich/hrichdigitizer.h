#ifndef HRICHDIGITISER_H
#define HRICHDIGITISER_H

#include "TList.h"
#include "hreconstructor.h"
#include "hlocation.h"
#include "hparset.h"
#include "hdataobject.h"
#include <TVector.h>
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
        HRichDigitizer(Text_t *name,Text_t *title);
	~HRichDigitizer();

        HRichDigitizer(const HRichDigitizer& source);
        HRichDigitizer &operator=(HRichDigitizer &source);

        Bool_t init();
        Int_t execute();
        Bool_t finalize() { return kTRUE; };

        HCategory* getCerInputCat() { return fCerInputCat; }
        HCategory* getDirInputCat() { return fDirInputCat; }
        HCategory* getOutputCat() { return fOutputCat; }
        HCategory* getTrackOutputCat() { return fTrackOutputCat;}
        
        HParSet* getDigitisationPar() { return fDigitisationPar; }
        HParSet* getGeometryPar() { return fGeometryPar; }


        void setCerInputCat(HCategory* pInCat) { fCerInputCat = pInCat; }
        void setDirInputCat(HCategory* pInCat) { fDirInputCat = pInCat; }
        void setOutputCat(HCategory* pOutCat) { fOutputCat = pOutCat; }
        void setTrackOutputCat(HCategory* pOutCat){ fTrackOutputCat =pOutCat;} 
        
          
        void setDigitisationPar(HParSet* pPar) { fDigitisationPar = pPar; }
        void setGeometryPar(HParSet* pPar) { fGeometryPar = pPar; }
        

        Float_t getQmean() { return fQmean; }
        void setQmean(Float_t q) { fQmean = q; }


        ClassDef(HRichDigitizer,0) //ROOT extension

        Int_t lNrEvent;

private:

        HCategory *fCerInputCat; //Pointer to the raw data category
        HCategory *fDirInputCat; //Pointer to the raw data category
        HCategory *fOutputCat; //Pointer to the cal data category
        HCategory *fTrackOutputCat;//Pointer to the richtrack category        

        HParSet* fDigitisationPar; //Pointer to digitisation parameters
        HParSet* fGeometryPar; //Pointer to geometry parameters

        HIterator *fCerIter;     //!
        HIterator *fDirIter;     //!
        HIterator *fTrackIter;   //!           
        HIterator *fCalIter;     //!

        void calcQmean();
        Float_t GaussFun(Float_t mean, Float_t sigma);
        Float_t calcCharge(Float_t sourceCharge, Float_t distance, 
                           Float_t Xd, Float_t Yd, Float_t Xu, Float_t Yu);
        void initChargePatternMatrix();
        Bool_t calcQE(const Float_t photlen);
        Int_t getWireNr(Float_t xhit);
        void calcChargeOnWire(Int_t sector, Float_t xhit, Float_t yhit,
                              Float_t nTrack,Float_t nFlag);
        void digitiseCherenkovHits(HGeantRichPhoton *pCerHit);
        void digitiseDirectHits(HGeantRichDirect *pDirHit);
        void calcLimits(HRichPad *pPad, Int_t *Xd, Int_t *Yd, Int_t *Xu,
                        Int_t *Yu);
        HRichPad* translateCorners(HRichPad *pPad, Float_t dx, Float_t dy);
        void translateCorners(HRichPad *pPad, Float_t dx, Float_t dy,
                         Float_t *Xd, Float_t *Yd, Float_t *Xu, Float_t *Yu);
        void makeNoiseOnPads();
        void digitisePads();
        Float_t analyticalCalc(HRichPad* pPad, Float_t wirecharge);
        Float_t numericalCalc(HRichPad* pPad, Float_t wirecharge);
        void updateCharge(Int_t sector, HRichPad* pPad, Float_t charge,
                          TVector * rTrack);
        
        void updateTrack(HRichCalSim *pCalSim,HLocation & loc,
                         TVector * rTrack);

        Float_t fQmean; // mean Q charge value for Polya function
        Float_t fQlower, fQupper;
        Float_t fChargePerChannel;
        Float_t *fpChargePatternMatrix;
        Float_t fDigitPadMatrix[9];
        TList fChargeOnWireList; //holds charge on a wire
        TList fTrackNumberList; // holds track number and flag for a hit
        HLocation loc;

        Float_t fSigmaValue;
        Float_t fFactor1, fFactor2, fFactor3, fFactor4, fFactor5;
        Float_t fFactor6, fFactor7, fFactor8, fParam1, fParam2;

};

#endif // HRICHDIGITISER_H 


