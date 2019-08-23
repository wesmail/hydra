// File: hthreseval.h
//
// Author: Laura Fabbietti <L.Fabbietti@physik.tu-muenchen.de>
// Last update by Laura Fabbietti: 03/01/13 17:26:27
//
#ifndef HTHRESEVAL_H
#define HTHRESEVAL_H

#include <TObjArray.h>
#include "hreconstructor.h"
#include "hparset.h"
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TObjArray.h>
#include <TCanvas.h>
#include <TNtuple.h>
#include <TString.h>
#include "hrichgeometrypar.h"
#include "hrichanalysispar.h"
class HCategory;
class HIterator;
class HRichHit;
class HHitMatch;
class HRichUtilFunc;

class HThreseval : public HReconstructor {
public:
    HThreseval();
    HThreseval(Text_t *name,Text_t *title,Option_t *opt,Option_t *opt1);
    ~HThreseval(void);
    Bool_t init();
    Bool_t reinit();
    Bool_t finalize();
    Int_t execute();
    Bool_t isRingInArray(HHitMatch *  pM);
    void fillHistoPM(Float_t pm,Float_t dt,Int_t histoInd);
    void fillHistoHt(Float_t ht,Float_t dt,Int_t histoInd);
    Float_t fitAndSubtractBG(TH1F* histo);
    Bool_t isTestDensityTrue(TString st);
    Bool_t isTestBoarderTrue(TString st);
    Bool_t isTestDynamicTrue(TString st);
    Bool_t  isTestRatioTrue(TString st);
    Bool_t isTestAssymerTrue(TString st);
    Bool_t isTestChargeTrue(TString st);
    Bool_t isClosePairRejectes(TString st);

    Float_t testBorder(HRichHit *pHit);
    Float_t testRatio(HRichHit *pHit);
    void testCloseRej(HRichHit *pHit1,Float_t &dq);
   

    Int_t IsOut(Int_t x, Int_t y, Int_t dx, Int_t dy);
   
    HParSet *fpAnalysisPar;
    HRichAnalysisPar *getAnalysisParams() { return ((HRichAnalysisPar*)fpAnalysisPar); }
    void setAnalysisPar(HParSet* pPar) { fpAnalysisPar = pPar; }

    HParSet *fpGeomPar;
    HRichGeometryPar* getGeometryPar() { return ((HRichGeometryPar*)fpGeomPar); }
    void setGeomPar(HParSet* pPar) { fpGeomPar = pPar; }

    HCategory* getHitMatchCat(){return pHitMatchCat;};
    HCategory* getRichCalCat(){return pRichCalCat;}
    HCategory* getRichHitCat(){return pRichHitCat;}
private:
    TArrayI iPadActive;
    TFile * out;
    TObjArray *pHistArray;//!
    TH1F ***thetaDiffPM, ***thetaDiffHT;
    TH2F * hTestBorder,*hTestClose;
    TH1F * hTestRatio, *hTestRadius, *hTestCentroid,*hTestCharge;
    TH2F * hTestBorderCorr,*hTestCloseCorr;
    TH1F * hTestRatioCorr, *hTestRadiusCorr, *hTestCentroidCorr,*hTestChargeCorr;
    TH2F *effPM,*effHT,*effPM1,*effHT1,*effPM2,*effHT2,*effPM3,*effHT3,*effPM4,*effHT4,*effPM5,*effHT5,*effPM6,*effHT6,*effPM7,*effHT7;
    TH2F *pMhT;
    Int_t richIndArray[100];
    Int_t countHitMatchObj;
    TString isLepton;
    TString whichData;
    Float_t maxPadCharge;

    TNtuple * allRingPar;
    TNtuple * corrRingPar;

    HCategory *pHitMatchCat; //!
    HCategory * pRichCalCat; //!
    HCategory *pRichHitCat; //!

    HIterator* pIterMatchHit; //!
    HIterator* pIterMatchHit1; //!
    HIterator* pIterRichCal; //!
    HIterator* pIterRichHit1; //!
    HIterator* pIterRichHit2; //!

    ClassDef(HThreseval,1) //ROOT extension
};
#endif /* !HTHRESEVAL_H */
