// File: hcreatecorrpar.h
//
// Author: Laura Fabbietti <L.Fabbietti@physik.tu-muenchen.de>
// Last update by Laura Fabbietti: 03/02/05 18:01:36
//
#ifndef HCREATECORRPAR_H
#define HCREATECORRPAR_H
#include "hreconstructor.h"
#include "hlocation.h"
#include "hparset.h"
#include "TH1.h"
#include "TH2.h"
#include "TString.h"
#include "TF1.h"
#include "TObjArray.h"
#include "TFile.h"
class HCategory;
class HIterator;
class HHitMatchHeader;
class HHitMatch;

class HCreateCorrPar: public HReconstructor {
public:
    HCreateCorrPar(Text_t *name,Text_t *title);
    HCreateCorrPar(Text_t *name,Text_t *title, Char_t* filename, Int_t iRichQual);
    ~HCreateCorrPar(void);
    Bool_t init();
    Int_t execute();
    void iniHistos();
    Bool_t isGoodRing(HHitMatch *pHM);
    Bool_t isTOFBetween(Float_t tof,Float_t low,Float_t high);
    void fitLinHistos(TH1F* histo,TH1F *histoFit);
    void fitGausHistos(TH1F* histo,TH1F *histoFit);
    Bool_t finalize();
    HCategory* getHitMatchCat(void){return pHitMatchCat;}
   
    ClassDef(HCreateCorrPar,1) //ROOT extension
protected:
    HCategory* pHitMatchCat; //!
    HIterator* pIterMatchHit; //!
    TFile* pFileOut;//!
    TObjArray *pHistArray;//!
    
    Char_t* pFileName;//!
    Int_t iRichQualThreshold;
    Int_t bin1;
    Float_t fPhiCut;
    TH2F * pH_RichMdcdThetadPhi;
    TH2F * pH_RichMdcdPhiTheta;
    TH1F* pH_RichMdcdTheta;
    TH1F* pH_RichMdcdThetaPhiCut;
    TH1F* pH_RichMdcdThetaTofCorr;
    TH1F* pH_RichMdcdThetaTof1Hit;
    TH1F* pH_RichMdcdThetaTof1HitPhiCut;
    TH1F* pH_RichMdcdThetaTof2Hit;
    TH1F* pH_RichMdcdPhi;
    TH1F* pH_RichMdcdPhiTofCorr; 
    TH1F* pH_RichMdcdPhiTof1Hit ;
    TH1F* pH_RichMdcdPhiTof2Hit;

    TH1F* pH_RichMdcdThetaFit;
    TH1F* pH_RichMdcdThetaPhiCutFit;
    TH1F* pH_RichMdcdThetaTofCorrFit;
    TH1F* pH_RichMdcdThetaTof1HitFit;
    TH1F* pH_RichMdcdThetaTof1HitPhiCutFit;
    TH1F* pH_RichMdcdThetaTof2HitFit;
    TH1F* pH_RichMdcdPhiFit;
    TH1F* pH_RichMdcdPhiTofCorrFit; 
    TH1F* pH_RichMdcdPhiTof1HitFit ; 
    TH1F* pH_RichMdcdPhiTof2HitFit;

    Int_t count;

};
#endif /* !HCREATECORRPAR_H */

