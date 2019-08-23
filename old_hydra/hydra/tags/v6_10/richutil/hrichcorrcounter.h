// File: hrichcorrcounter.h
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 02/03/08 16:33:39
//
#ifndef HRICHCORRCOUNTER_H
#define HRICHCORRCOUNTER_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hparset.h"
#include <TH1.h>
#include <TH2.h>
#include <TObjArray.h>
#include <TFile.h>
class HCategory;
class HIterator;
class HHitMatchHeader;
class HHitMatch;
class HKickTrack;
class HRichCorrCounter : public HReconstructor {
public:

	HRichCorrCounter();
        HRichCorrCounter(Text_t *name,Text_t *title);
    HRichCorrCounter(Text_t *name,Text_t *title, char* filename);
	~HRichCorrCounter();

        Bool_t init();
        Bool_t finalize();
        Int_t execute();
        void dumpCorrelationStatus();
        void iniCounters();
        void iniHistos();
        Int_t fillHistos();
        Int_t fillMdcTofThetadTheta(HHitMatch*,Int_t,Int_t*,Int_t*);
        Int_t fillMdcShowerThetadTheta(HHitMatch*,Int_t,Int_t*,Int_t*);
        Int_t fillRichMdcThetadTheta(HHitMatch*,Int_t,Int_t*,Int_t*);
        Int_t fillUniqueIndex(Int_t,Int_t,Int_t*);
        Int_t countDiscreteIndexes(Int_t*,Int_t);
        Int_t countDiscreteIndexesF(Float_t*,Int_t);
        Int_t checkCombination(Int_t,Int_t,Int_t *,Int_t *,Int_t);
        Int_t checkCombinationSameCat(Int_t,Int_t,Int_t *,Int_t *,Int_t);
        Int_t isStored(Int_t, Int_t*, Int_t);
        Int_t isStoredWithOffset(Int_t, Int_t*, Int_t, Int_t);
        Int_t selectObj(TObjArray*,HHitMatch*);
        void  calcOpeningAngleMDC(TObjArray*,TH1F*);
        void  calcOpeningAngleTOF(TObjArray*,TH1F*);
        void  calcOpeningAngleMETA(TObjArray*,TH1F*,TH2F*,TH2F*);
        HKickTrack* getKickTrack(HHitMatch*);
        HCategory* getHitMatchCat(){return pHitMatchCat;};
        HCategory* getHitMatchHeaderCat(){return pHitMatchHeaderCat;};
        ClassDef(HRichCorrCounter,1) //ROOT extension

protected:

 	HCategory* pHitMatchCat; //!
        HCategory* pHitMatchHeaderCat; //!

        HIterator* pIterMatchHit; //!
        HIterator* pIterMatchHitHeader; //!

    HCategory* fKickTrackCat;     //! Pointer to KickTrack data category
    HIterator* iterTracks;        //! Iterator over KickTrack category

    TFile* pFileOut;//!
    TObjArray *pHistArray;//!
    // HISTOGRAMS
    TH2F *pH_MdcTofThetadTheta;//!
    TH2F *pH_MdcShowerThetadTheta;//!
    TH2F *pH_RichMdcThetadTheta;//!
    TH1F *pH_RichMdcdTheta;//!
    TH2F *pH_MdcTofToftofdTheta;//!
    TH2F *pH_MdcShowerTofinotofdTheta;//!
    TH1F *pH_opangleMDC;//!
    TH1F *pH_opangleTOF;//!
    TH1F *pH_opangleMETA;//!
    TH2F *pH_opangleMETAdThetaMDCMETA;//!
    TH2F *pH_opangleMETAdPhiMDCMETA;//!
    TH1F *pH_MdcTofdTheta;//!
    TH1F *pH_MdcShowerdTheta;//!
    TH1F *pH_MdcMETAdTheta;//!
    TH2F *pH_opangleTrksinRing;//!
    TH1D *pH_RichDblPadNr;//!
    TH1D *pH_RichDblAmpl;//!
    TH1D *pH_RichDblPatMat;//!
    TH1D *pH_RichDblHouTra;//!
    TH1D *pH_RichSnglPadNr;//!
    TH1D *pH_RichSnglAmpl;//!
    TH1D *pH_RichSnglPatMat;//!
    TH1D *pH_RichSnglHouTra;//!
    //-------------------------------
    char* pFileName;//!
    Int_t nCounterProcessedNbEvents; //total nb of processed evts
    Int_t ngNbCorrObjs; //!
    
    Int_t ngNb_RM; //!only
    Int_t ngNb_RT; //!
    Int_t ngNb_RS; //!
    Int_t ngNb_MT; //!
    Int_t ngNb_MS; //!
    Int_t ngNb_RcMcT; //!//  RM MT
    Int_t ngNb_RcMcTcR; //!//RM MT RT
    Int_t ngNb_RMcTcR; //!//    MT RT
    Int_t ngNb_RcMTcR; //!// RM    RT
    Int_t ngNb_RcMcS; //!//  RM MS
    Int_t ngNb_RcMcScR; //!//RM MS RS
    Int_t ngNb_RMcScR; //!//    MS RS
    Int_t ngNb_RcMScR; //!// RM    RS

    Int_t ngRTM; //!//3 indexes in obj
    Int_t ngRSM; //!
    Int_t ngRichCnt; //!
    Int_t ngMdcSegCnt; //!
    Int_t ngShowerCnt; //!
    Int_t ngTofCnt; //!
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
    
    Int_t mdcmult[10];
    Int_t tofmult[10];
    Int_t shomult[10];
};

#endif // HRICHCORRCOUNTER_H

