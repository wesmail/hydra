// File: hrichcorrelator.h
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 02/03/27 12:57:28
//
#ifndef HRICHCORRELATOR_H
#define HRICHCORRELATOR_H
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TString.h>
#include <TStyle.h>
#include <TProfile.h>
#include <TObjArray.h>
#include "hreconstructor.h"
#include "hparset.h"

class HCategory;
class HIterator;
class HRichHit;
class HMdcSeg;
class HMdcHit;
class HShowerHitTof;
class HTofHit;
class HKickTrack;
class HHitMatch;
class HHitMatchHeader;
class HRichUtilFunc;
class HSpecGeomPar;
class HRichCorrelator : public HReconstructor {
public:

    HRichCorrelator();
    HRichCorrelator(Text_t *name,Text_t *title);
    HRichCorrelator(Text_t *name,Text_t *title,char* filename);
    ~HRichCorrelator();
    
    Bool_t init();
    Bool_t reinit();
    Bool_t finalize();
    Int_t execute();
    Bool_t delRichHit(TObjArray *);
    Bool_t isCorrelated(HRichHit*,HMdcSeg*);
    Bool_t isCorrelated(HRichHit*,HHitMatch*);
    Bool_t isCorrelated(HRichHit*,HTofHit*);
    Bool_t isCorrelated(HRichHit*,HShowerHitTof*);
    Bool_t isCorrelated(HMdcSeg*,HShowerHitTof*);
    Bool_t isCorrelated(HMdcSeg*,HTofHit*);
    Bool_t isGoodRing(HRichHit*);
    Bool_t isSelectedSector(Int_t);
    Bool_t isShower(HShowerHitTof*);
    void fillControlHistos(void);
    void matchKickTracks(void);
    void fillALLRingHistos(HRichHit*);
    //    void iniCorrelationPar();
    void iniCuts(void);
    void iniControlHistos(void);
    virtual void iniCounters(void);
    void iniSwitches(void);
    void fillHitMatch(HHitMatch*,HRichHit*,HMdcSeg*);
    void fillHitMatch(HHitMatch*,HRichHit*,HTofHit*);
    void fillHitMatch(HHitMatch*,HRichHit*,HShowerHitTof*);
    void fillHitMatch(HHitMatch*, HMdcSeg*, HTofHit*);
    void fillHitMatch(HHitMatch*, HMdcSeg*, HShowerHitTof*);
    Int_t fillUniqueIndex(Int_t,Int_t,Int_t*);
    Int_t countDiscreteIndexes(Int_t*,Int_t);
    void setRich(HHitMatch*,HRichHit*);
    void setMdc(HHitMatch*,HMdcSeg*);
    void setTof(HHitMatch*, HTofHit*);
    void setShower(HHitMatch*,HShowerHitTof*);
    void dumpCorrelationStatus();
    void incrementRingCount(HRichHit *pRichHit);
    TCanvas* makeCanvas1();
    TCanvas* makeCanvas2();
    Float_t correctMdcSegPhi(Int_t);
    Float_t correctMdcSegTheta(Float_t);
    HHitMatch* createHitMatch(Int_t);
    HHitMatchHeader* createHitMatchHeader();
    void fillHitMatchHeader(HHitMatchHeader*);
    HCategory* getTofHitCat(){return fTofHits;};
    HCategory* getShowerTofHitCat(){return fShowerTofHits;};
    HCategory* getMdcSegCat() { return fMdcSeg ; };
    HCategory* getMdcHitCat() { return fMdcHit ; };
    HCategory* getHitMatchCat(){return pHitMatchCat;};
    HCategory* getHitMatchHeaderCat(){return pHitMatchHeaderCat;};
    HCategory* getRichHitCat() { return fRichPID; };
    HCategory* getKickCat() {return fKickTrackCat;};
    void setCorrelationPar(HParSet* pPar){pCorrPar = pPar;};
    HParSet* getCorrelationPar(){return pCorrPar;};
    void setSpecGeomPar(HSpecGeomPar* pPar){pSpecGeomPar = pPar;};
    HSpecGeomPar* getSpecGeomPar(){return pSpecGeomPar;};
    void rotateTrackToLab(Int_t,Float_t,Float_t,Float_t &,Float_t &);
    ClassDef(HRichCorrelator,1) //ROOT extension
	
	
	protected:
    HCategory *fTofHits; //!
    HCategory *fShowerTofHits; //!
    HCategory *fRichPID; //!
    HCategory *fMdcSeg; //!
    HCategory *fMdcHit; //!
    HCategory *pHitMatchCat; //!
    HCategory *pHitMatchHeaderCat; //!
    HCategory* fKickTrackCat;     //! Pointer to KickTrack data category
    HIterator* iterTracks;        //! Iterator over KickTrack category

    HParSet* pCorrPar;//!
    HSpecGeomPar *pSpecGeomPar ;//!
    HIterator* fMdcSegIter;    //!
    HIterator* fMdcHitIter;    //!
    HIterator* fRichIter;    //!
    HIterator* fTofIter;    //!
    HIterator* fShowerTofIter;    //!
    HIterator* pIterMatchHit; //!
    HIterator* pIterMatchHitHeader; //!
    TFile *pFileOut; //!
    
    TH1D *pHdiffThetaRichMdc;//!
    // C U T S
    Float_t fRichMdcThetaCut; //!
    Float_t fRichMdcThetaMinCut; //!
    Float_t fRichMdcThetaMaxCut; //!
    Float_t fRichTofThetaCut; //!
    Float_t fRichTofThetaMinCut; //!
    Float_t fRichTofThetaMaxCut;//!
    Float_t fRichShowerThetaCut; //!
    Float_t fRichShowerThetaMinCut;//!
    Float_t fRichShowerThetaMaxCut;//!
    Float_t fMdcTofThetaCut; //!
    Float_t fMdcTofThetaMinCut; //!
    Float_t fMdcTofThetaMaxCut; //!
    Float_t fMdcShowerThetaCut; //!
    Float_t fMdcShowerThetaMinCut; //!
    Float_t fMdcShowerThetaMaxCut; //!
    Float_t fTofTofMinCut; //!
    Float_t fTofinoTofMinCut; //!
    Float_t fTofTofMaxCut; //!
    Float_t fTofinoTofMaxCut; //!
    Float_t fShowerAmplification;
    Float_t fRichMdcPhiCut; //!
    Float_t fRichTofPhiCut; //!
    Float_t fRichShowerPhiCut; //!
    Float_t fMdcTofPhiCut; //!
    Float_t fMdcShowerPhiCut; //!
    Int_t   nTofinoMultCut;//!
    // MDC Theta Correction factor
    Float_t fMdcThetaCorrection;//!
    //    HRichUtilFunc *pUtil;//!
    // Control Histos
    TObjArray *pHistArray;//!
    TH2D *pHistRMTheta;//!
    TH2D *pHistRMPhi;//!
    
    TH1F *pHistDiffRMTheta1;//!
    TH1F *pHistDiffRMPhi1;//!
    TH1F *pHistDiffRMTheta2;//!
    TH1F *pHistDiffRMPhi2;//!
    TH1F *pHistDiffRMTheta3;//!
    TH1F *pHistDiffRMPhi3;//!
    TH1F *pHistDiffRMTheta4;//!
    TH1F *pHistDiffRMPhi4;//!
    TH1F *pHistDiffRMTheta5;//!
    TH1F *pHistDiffRMPhi5;//!
    TH1F *pHistDiffRMTheta6;//!
    TH1F *pHistDiffRMPhi6;//!
    
    TH2D *pHistRSTheta;//!
    TH2D *pHistRSPhi;//!
    
    TH1F *pHistDiffRSTheta1;//!
    TH1F *pHistDiffRSPhi1;//!
    TH1F *pHistDiffRSTheta2;//!
    TH1F *pHistDiffRSPhi2;//!
    TH1F *pHistDiffRSTheta3;//!
    TH1F *pHistDiffRSPhi3;//!
    TH1F *pHistDiffRSTheta4;//!
    TH1F *pHistDiffRSPhi4;//!
    TH1F *pHistDiffRSTheta5;//!
    TH1F *pHistDiffRSPhi5;//!
    TH1F *pHistDiffRSTheta6;//!
    TH1F *pHistDiffRSPhi6;//!
    
    
    TH2D *pHistRTTheta;//!
    TH2D *pHistRTPhi;//!
    
    TH1F *pHistDiffRTTheta1;//!
    TH1F *pHistDiffRTPhi1;//!
    TH1F *pHistDiffRTTheta2;//!
    TH1F *pHistDiffRTPhi2;//!
    TH1F *pHistDiffRTTheta3;//!
    TH1F *pHistDiffRTPhi3;//!
    TH1F *pHistDiffRTTheta4;//!
    TH1F *pHistDiffRTPhi4;//!
    TH1F *pHistDiffRTTheta5;//!
    TH1F *pHistDiffRTPhi5;//!
    TH1F *pHistDiffRTTheta6;//!
    TH1F *pHistDiffRTPhi6;//!
    
    TH2D *pHistMTTheta;//!
    TH2D *pHistMTPhi;//!
    
    TH1F *pHistDiffMTTheta1;//!
    TH1F *pHistDiffMTPhi1;//!
    TH1F *pHistDiffMTTheta2;//!
    TH1F *pHistDiffMTPhi2;//!
    TH1F *pHistDiffMTTheta3;//!
    TH1F *pHistDiffMTPhi3;//!
    TH1F *pHistDiffMTTheta4;//!
    TH1F *pHistDiffMTPhi4;//!
    TH1F *pHistDiffMTTheta5;//!
    TH1F *pHistDiffMTPhi5;//!
    TH1F *pHistDiffMTTheta6;//!
    TH1F *pHistDiffMTPhi6;//!
    
    TH2D *pHistMSTheta;//!
    TH2D *pHistMSPhi;//!
    
    TH1F *pHistDiffMSTheta1;//!
    TH1F *pHistDiffMSPhi1;//!
    TH1F *pHistDiffMSTheta2;//!
    TH1F *pHistDiffMSPhi2;//!
    TH1F *pHistDiffMSTheta3;//!
    TH1F *pHistDiffMSPhi3;//!
    TH1F *pHistDiffMSTheta4;//!
    TH1F *pHistDiffMSPhi4;//!
    TH1F *pHistDiffMSTheta5;//!
    TH1F *pHistDiffMSPhi5;//!
    TH1F *pHistDiffMSTheta6;//!
    TH1F *pHistDiffMSPhi6;//!
    
    TH1F *pHistRMTRingHouTra;//!
    TH1F *pHistRMTRingPatMat;//!
    TH1F *pHistRMTRingAmplitude;//!
    TH1F *pHistRMTRingPadNr;//!
    TH1F *pHistRMTRingLocMax4;//!
    TH1F *pHistRMTRingRadius;//!
    TH1F *pHistRMTRingCenterX;//!
    TH1F *pHistRMTRingCenterY;//!
    TH1F *pHistRMTRingTheta;//!
    TH1F *pHistRMTRingPhi;//!
    TH2D *pHistRMTRingHouTraTheta;//!
    TH2D *pHistRMTRingPatMatTheta;//!
    TH2D *pHistRMTRingAmplitudeTheta;//!
    TH2D *pHistRMTRingPadNrTheta;//!
    TH2D *pHistRMTRingLocMax4Theta;//!
    
    TH1F *pHistRMRingHouTra;//!
    TH1F *pHistRMRingPatMat;//!
    TH1F *pHistRMRingAmplitude;//!
    TH1F *pHistRMRingPadNr;//!
    TH1F *pHistRMRingLocMax4;//!
    TH1F *pHistRMRingRadius;//!
    TH1F *pHistRMRingCenterX;//!
    TH1F *pHistRMRingCenterY;//!
    TH1F *pHistRMRingTheta;//!
    TH1F *pHistRMRingPhi;//!
    TH2D *pHistRMRingHouTraTheta;//!
    TH2D *pHistRMRingPatMatTheta;//!
    TH2D *pHistRMRingAmplitudeTheta;//!
    TH2D *pHistRMRingPadNrTheta;//!
    TH2D *pHistRMRingLocMax4Theta;//!
    
    TH1F *pHistRMSRingHouTra;//!
    TH1F *pHistRMSRingPatMat;//!
    TH1F *pHistRMSRingAmplitude;//!
    TH1F *pHistRMSRingPadNr;//!
    TH1F *pHistRMSRingLocMax4;//!
    TH1F *pHistRMSRingRadius;//!
    TH1F *pHistRMSRingCenterX;//!
    TH1F *pHistRMSRingCenterY;//!
    TH1F *pHistRMSRingTheta;//!
    TH1F *pHistRMSRingPhi;//!
    TH2D *pHistRMSRingHouTraTheta;//!
    TH2D *pHistRMSRingPatMatTheta;//!
    TH2D *pHistRMSRingAmplitudeTheta;//!
    TH2D *pHistRMSRingPadNrTheta;//!
    TH2D *pHistRMSRingLocMax4Theta;//!
    
    TH1F *pHistALLRingHouTra;//!
    TH1F *pHistALLRingPatMat;//!
    TH1F *pHistALLRingAmplitude;//!
    TH1F *pHistALLRingPadNr;//!
    TH1F *pHistALLRingLocMax4;//!
    TH1F *pHistALLRingRadius;//!
    TH1F *pHistALLRingCenterX;//!
    TH1F *pHistALLRingCenterY;//!
    TH1F *pHistALLRingTheta;//!
    TH1F *pHistALLRingPhi;//!
    TH2D *pHistALLRingHouTraTheta;//!
    TH2D *pHistALLRingPatMatTheta;//!
    TH2D *pHistALLRingAmplitudeTheta;//!
    TH2D *pHistALLRingPadNrTheta;//!
    TH2D *pHistALLRingLocMax4Theta;//!
    
    TProfile *pProfRMTHTtheta;//!
    TProfile *pProfRMSHTtheta;//!
    TProfile *pProfRMTPMtheta;//!
    TProfile *pProfRMHTtheta;//!
    TProfile *pProfRMPMtheta;//!
    TProfile *pProfRMSPMtheta;//!
    TProfile *pProfALLHTtheta;//!
    TProfile *pProfALLPMtheta;//!
    TProfile *pProfALLPadNrTheta;//!
    TProfile *pProfRMPadNrTheta;//!
    TProfile *pProfRMSPadNrTheta;//!
    TProfile *pProfRMTPadNrTheta;//!

    TProfile *pProfALLLocMax4Theta;//!
    TProfile *pProfRMLocMax4Theta;//!
    TProfile *pProfRMSLocMax4Theta;//!
    TProfile *pProfRMTLocMax4Theta;//!

    TProfile *pProfALLRingAmplitudeTheta;//!
    TProfile *pProfRMRingAmplitudeTheta;//!
    TProfile *pProfRMSRingAmplitudeTheta;//!
    TProfile *pProfRMTRingAmplitudeTheta;//!
    // HELPER
    Int_t nCounterProcessedNbEvents; 
    //total nb of processed evts
    Float_t nCounterMin2Rings; 
    //nb of events with min 2 rings 
    Float_t nCounterTotalNbRings; 
    //total nb of processed rings
    Float_t nCounterRingsAbove45; 
    //total nb of rings above 45 degrees pol angle
    Float_t nCounterRingsBelow45; 
    //total nb of rings below 45 degrees pol angle
    Float_t nCounterEvtWritten; 
    //total nb of events written to output after filter
    Float_t nCounterRingsWritten; 
    //total nb of rings written to output after filter
    Float_t nCounterNbRejectedRings; 
    //nb of (Mdc uncorr) rings deleted from written events 
    Float_t nCounterRingsUnresMultHit; 
    //number of rings for which more than one Mdc
    //hit was found (double rings)
    Float_t nCounterEventsUnresMultHit; 
    //number of events in which rings 
    //with multiple Mdc hits were found
    Float_t nCounterRingsCorrMdc; 
    //nb of rings corr with Mdc Segment
    Float_t nCounterRingsCorrShower; 
    //nb of rings corr with Mdc and Shower (lep id)
    Float_t nCounterRingsCorrTof; 
    //nb of rings corr with Mdc and Tof (lep id)
    Int_t ngNbCorrObjs;
    // local (for this evt) counter variables
    Int_t ngNb_RM;//only
    Int_t ngNb_RT;
    Int_t ngNb_RS;
    Int_t ngNb_MT;
    Int_t ngNb_MS;
    Int_t ngNb_RcMcT;//  RM MT
    Int_t ngNb_RcMcTcR;//RM MT RT
    Int_t ngNb_RMcTcR;//    MT RT
    Int_t ngNb_RcMTcR;// RM    RT
    Int_t ngNb_RcMcS;//  RM MS
    Int_t ngNb_RcMcScR;//RM MS RS
    Int_t ngNb_RMcScR;//    MS RS
    Int_t ngNb_RcMScR;// RM    RS

    Int_t ngRTM;//3 indexes in obj
    Int_t ngRSM;
    Int_t ngRichCnt;
    Int_t ngMdcSegCnt;
    Int_t ngShowerCnt;
    Int_t ngTofCnt;
    Int_t ngDRind;
    Int_t ngDRichminRMind;
    Int_t ngDRichminRTind;
    Int_t ngDRichminRSind;
    Int_t ngDRichminRMTind;
    Int_t ngDRichminRMSind;
    Int_t ngDRcMcTcRind;
    Int_t ngDRcMcScRind;
    Int_t ngDRcMTcRind;
    Int_t ngDRcMScRind;
    Int_t ngDRcMcTind;
    Int_t ngDRcMcSind;
    Int_t ngDRMcTcRind;
    Int_t ngDRMcScRind;
    Int_t ngDRMind;
    Int_t ngDRTind;
    Int_t ngDRSind;
    Int_t ngDMTind;
    Int_t ngDMSind;
    Int_t ngDMind;
    Int_t ngDTind;
    Int_t ngDSind;
    
    
    //HISTOGRAM DIMENSIONS
    Int_t nNbBinsForPatMat;
    Int_t nNbBinsForHouTra;
    Int_t nHouTraMax;

    //SWITCHES
    Bool_t kShowerCond;
    Bool_t kIsNoCutOnTOFtof;
    Bool_t kIsNoCutOnTOFINOtof;
    Bool_t kNoSectorSelected;
    Bool_t kSwitchOffRich;
    Bool_t kRingQualityCut;
    Bool_t kTofinoMultCut;
    Bool_t kMagnet;
    Bool_t kStandRing;
    // RICH RING QUALITY
    Int_t nPatMatCut;
    Int_t nHouTraCut;
    Int_t nPadNrCut;
    Int_t nRingAmplCut;
    Int_t nLocMax4Cut;
    //SECTOR SELECTION
    Int_t nSelectedSector; // from 0 to 5

    //FILENAME
    char *pFileName;//!
};

#endif //HRICHCORRELATOR_H



