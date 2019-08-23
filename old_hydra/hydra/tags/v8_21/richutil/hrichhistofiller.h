// $Id: hrichhistofiller.h,v 1.9 2009-07-15 11:39:22 halo Exp $
// Last update by Thomas Eberl: 03/12/17 14:56:17
//
#ifndef HRICHHISTOFILLER_H
#define HRICHHISTOFILLER_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hparset.h"
#include "TString.h"

class HCategory;
class HIterator;
class HDiHitMatch;
class HHitMatch;
class HHitMatchSim;
class TObjArray;
class TFile;
class TTree;
class HRichHistoFiller : public HReconstructor {
public:

	HRichHistoFiller();
        HRichHistoFiller(const Text_t *name,const Text_t *title,const Char_t*,const Char_t*,const Char_t*);
	~HRichHistoFiller();

        Bool_t init();
        Bool_t finalize();
        Int_t execute();
        
        void setTrackletCutObjectName(const Char_t *t)     {TString tt(t);if (!tt.CompareTo("default"))
	    trackletcut = new TString("trackcut"); else trackletcut = new TString(t);};
        void setPairCutObjectName(const Char_t *p)         {TString pp(p);if (!pp.CompareTo("default"))
	    paircut = new TString("paircut"); else paircut = new TString(p);};
        void setTrackletCuts(const Char_t* cuts)           {setCuts(cuts,cutIdArrT,",");};
        void setPairCuts(const Char_t* cuts)               {setCuts(cuts,cutIdArrP,",");};

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
        void setCuts(const Char_t*,TObjArray*,const Char_t*);
        void makeCutCombinatorics(TObjArray*,Int_t,const Char_t*);
        TObjArray* splitSubsetName(TString&);
        TString* buildArrName(const Char_t*,TObjArray*,const Char_t*);
        Bool_t isNewSubset(TString&,TObjArray*);
        Int_t countCutsInSubset(const Char_t*);
        TObjArray* selectTracklets(const Char_t*,TObjArray*,Bool_t bar=kFALSE);
        TObjArray* selectPairs(const Char_t*,TObjArray*,Bool_t bar=kFALSE);
        TObjArray* filterPairs(TObjArray*, Int_t*);// filter pairs due to track list
        TString* buildCutStateId(TObjArray*,const Char_t*);
        void fillHistos(TObjArray*,const Char_t *);
        Int_t fillTrackHistosFromTracklets(TObjArray*,const Char_t*);
        Int_t fillDetHistosFromTracklets(TObjArray*,const Char_t*);
        Int_t fillPairHistosFromPairs(TObjArray*,const Char_t*);
        Int_t fillTrackHistosFromPairs(TObjArray*,const Char_t*);
        void fillTH1F(const Char_t*,const Char_t*,Float_t,Float_t weight=1.);
        void fillTH2F(const Char_t*,const Char_t*,Float_t,Float_t,Float_t weight1=1.);
        void setHistNameAddition(TString&,const Char_t*,const Char_t*);
        void appendName(TString&,const Char_t*);
        void prependName(TString&,const Char_t*);
        TObjArray* getSet(const TObjArray*, const Char_t*);
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
        TString pFileName;//!
        TString pInFileName;//!
        //FILE
        TFile *pFileOut;//! histogram file
        TFile *pFileCutIn;//! 
    TTree * tree;//! 
    HHitMatch *testhit;//!
    HHitMatchSim *testhitsim;//!

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

