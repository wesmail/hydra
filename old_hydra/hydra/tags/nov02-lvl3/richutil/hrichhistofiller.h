// $Id: hrichhistofiller.h,v 1.3 2003-01-24 16:07:34 eberl Exp $
// Last update by Thomas Eberl: 03/01/17 17:25:52
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
        HRichHistoFiller(Text_t *name,Text_t *title,Char_t*,Char_t*);
	~HRichHistoFiller();

        Bool_t init();
        Bool_t finalize();
        Int_t execute();
        TObjArray* selectTracklets(char*,TObjArray*);
        TObjArray* selectPairs(char*,TObjArray*);
        void fillPairHistos(TObjArray*);
        void fillTrackletHistos(TObjArray*);
        Int_t fillTrackHistosFromTracklets(TObjArray*,const Char_t*);
        Int_t fillDetHistosFromTracklets(TObjArray*,const Char_t*);
        Int_t fillPairHistosFromPairs(TObjArray*,const Char_t*);
        Int_t fillTrackHistosFromPairs(TObjArray*,const Char_t*);
        void fillTH1F(const char*,const char*,Float_t);
        void fillTH2F(const char*,const char*,Float_t,Float_t);
        void setHistNameAddition(TString&,const Char_t*,const Char_t*);
        void appendName(TString&,const char*);
        void prependName(TString&,const char*);
        HCategory* getHitMatchCat(){return pHitMatchCat;};
        HCategory* getHitDiMatchCat(){return pHitDiMatchCat;};
        ClassDef(HRichHistoFiller,1) //ROOT extension

protected:
	Int_t nCounterProcessedNbEvents; 
	void initHistos();
        void initCuts();
	HCategory* pHitMatchCat; //!
        HIterator* pIterMatchHit; //!
	HCategory* pHitDiMatchCat; //!
        HIterator* pIterDiMatchHit; //!
        TObjArray* pHistArray;//!
        TObjArray* cuts;//!
        //FILENAME
        char *pFileName;//!
        char *pInFileName;//!
        //FILE
        TFile *pFileOut;//!
        TFile *pFileCutIn;//!
    // CUT NAMES
    TString *trackletcut;//!
    TString *paircut;//!
    // SWITCH
    Bool_t kMakeSave;//!
    Bool_t kDetHPairs;//!
    Bool_t kTrackHPairs;//!
    Bool_t kDetHTracks;//!
};

#endif // HRICHPAIRCREATOR_H

