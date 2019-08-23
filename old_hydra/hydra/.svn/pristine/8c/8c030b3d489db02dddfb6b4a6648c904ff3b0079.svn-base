// $Id: hrichhistofiller.h,v 1.5 2003-05-22 17:30:21 eberl Exp $
// Last update by Thomas Eberl: 03/05/17 20:27:14
//
#ifndef HRICHHISTOFILLER_H
#define HRICHHISTOFILLER_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hparset.h"

class HCategory;
class HIterator;
class HDiHitMatch;
class HHitMatch;
class TObjArray;
class TFile;

class HRichHistoFiller : public HReconstructor {
public:

	HRichHistoFiller();
        HRichHistoFiller(Text_t *name,Text_t *title,Char_t*,Char_t*,Char_t*);
	~HRichHistoFiller();

        Bool_t init();
        Bool_t finalize();
        Int_t execute();
        
        void setTrackletCutObjectName(Char_t *t)     {TString tt(t);if (!tt.CompareTo("default")) 
	    trackletcut = new TString("trackcut"); else trackletcut = new TString(t);};
        void setPairCutObjectName(Char_t *p)         {TString pp(p);if (!pp.CompareTo("default")) 
	    paircut = new TString("paircut"); else paircut = new TString(p);};
        void setTrackletCuts(Char_t* cuts)           {setCuts(cuts,cutIdArrT,",");};
        void setPairCuts(Char_t* cuts)               {setCuts(cuts,cutIdArrP,",");};

        void setTrackletCutLvlCombiDepth(Int_t level){userDefTrackletCutLevelDepth = level;};
        void setPairCutLvlCombiDepth(Int_t level)    {userDefPairCutLevelDepth     = level;};
        void setHistoMinCutLvl(Int_t level)          {userDefHistoMinCutLvl        = level;
	if (userDefTrackletCutLevelDepth<level) userDefTrackletCutLevelDepth=level; 
	if (userDefPairCutLevelDepth<level) userDefPairCutLevelDepth = level;};
        void setDataLvlRecursionDepth(Int_t d)       {userDefDataLvlRecDepth       = d;};
        void setDetHitsFullSectorSplit(Int_t s)      {kSplitSector                 = (Bool_t) s;
	if (kSplitSector) userDefDataLvlRecDepth = 3;};
        void setStrictPairCutP(Int_t s)            {   kStrictPairCutP   = (Bool_t) s;};
        void setStrictPairCutT(Int_t s)            {   kStrictPairCutT   = (Bool_t) s;};
        void setCutCombinatorics(Int_t s)            {userDefCutCombinatorics      = (Bool_t) s;};

        void setTrackletCombiCut(Int_t s)            {kCombiCutT                   = (Bool_t) s;
	if (kCombiCutT) setCutCombinatorics(0);};
        void setPairCombiCut(Int_t s)                {kCombiCutP                   = (Bool_t) s;
	if (kCombiCutP) setCutCombinatorics(0);};


        void setHistogramCycling(Int_t s)            {if (s>0) {kMakeSave          = (Bool_t) s;
	nMakeSave = s;}};
        
        ClassDef(HRichHistoFiller,1) //ROOT extension

protected:
	Int_t nCounterProcessedNbEvents; 
	void initHistos();
        void initCuts();
        void setCuts(Char_t*,TObjArray*,Char_t*);
        void makeCutCombinatorics(TObjArray*,Int_t,char*);
        TObjArray* splitSubsetName(TString&);
        TString* buildArrName(const char*,TObjArray*,const char*);
        Bool_t isNewSubset(TString&,TObjArray*);
        Int_t countCutsInSubset(const char*);
        TObjArray* selectTracklets(const char*,TObjArray*,Bool_t bar=kFALSE);
        TObjArray* selectPairs(const char*,TObjArray*,Bool_t bar=kFALSE);
        TObjArray* filterPairs(TObjArray*, Int_t*);// filter pairs due to track list
        TString* buildCutStateId(TObjArray*,Char_t*);
        void fillHistos(TObjArray*,char *);
        Int_t fillTrackHistosFromTracklets(TObjArray*,const Char_t*);
        Int_t fillDetHistosFromTracklets(TObjArray*,const Char_t*);
        Int_t fillPairHistosFromPairs(TObjArray*,const Char_t*);
        Int_t fillTrackHistosFromPairs(TObjArray*,const Char_t*);
        void fillTH1F(const char*,const char*,Float_t);
        void fillTH2F(const char*,const char*,Float_t,Float_t);
        void setHistNameAddition(TString&,const Char_t*,const Char_t*);
        void appendName(TString&,const char*);
        void prependName(TString&,const char*);
        TObjArray* getSet(const TObjArray*, const char*);
        HCategory* getHitMatchCat(){return pHitMatchCat;};
        HCategory* getHitDiMatchCat(){return pHitDiMatchCat;};
        void reconPi0Mass(void);
    // data members
	HCategory* pHitMatchCat; //!
        HIterator* pIterMatchHit; //!
	HCategory* pHitDiMatchCat; //!
        HIterator* pIterDiMatchHit; //!
        TObjArray* pHistArray;//! array with histograms
        TObjArray* cuts;//! array with cut objects
        TObjArray* cutIdArrT;//! TObjStrings with cut names
        TObjArray* cutIdArrP;//!
        //FILENAME
        char *pFileName;//!
        char *pInFileName;//!
        //FILE
        TFile *pFileOut;//! histogram file
        TFile *pFileCutIn;//! 
    // CUT Object NAMES
    TString *trackletcut;//! name for used cut object 
    TString *paircut;//!     default or from file
    // USER DEFINED CUTNAMES //!
//     TString *userDefTrackcuts; //!
//     TString *userDefPaircuts; //!
    // SWITCH
    Bool_t kMakeSave;//! auto save histograms every n evts
    Bool_t kDetHPairs;//! auto creation of det hit histos from pairs
    Bool_t kTrackHPairs;//! auto creation of tracklet histos from pairs
    Bool_t kDetHTracks;//! auto creation of det hit histos from tracks 
    Bool_t kSplitSector;//! auto split detector histos sector-wise
    Bool_t isSim;//! simulation or real data world
    Bool_t kCombiCutT;//! process a list of cut tracklets as one 
    Bool_t kCombiCutP;//! process a list of pair tracklets as one
    Int_t userDefTrackletCutLevelDepth;//! iteratively combine cuts up to n cuts per histo
    Int_t userDefPairCutLevelDepth;//!
    Int_t userDefDataLvlRecDepth;//! steers auto creation of lower lvl histograms
    Bool_t userDefCutCombinatorics;//! switch on/off iterative combining of cuts
    Int_t userDefHistoMinCutLvl;//! min number of cuts for histo to be filled
    Int_t nMakeSave;//! save histograms every n evts
    Bool_t kAntiHistos;//!
    Bool_t kStrictPairCutP;//! discard all pairs w/ tracks cont in pair discarded due to pair cut
    Bool_t kStrictPairCutT;//! discard all pairs w/ tracks cont in pair discarded due to track cut
};

#endif // HRICHPAIRCREATOR_H

