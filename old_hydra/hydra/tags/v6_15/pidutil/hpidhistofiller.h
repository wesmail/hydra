// $Id: hpidhistofiller.h,v 1.1 2003-02-26 08:54:39 eberl Exp $
// Last update by Thomas Eberl: 03/02/25 14:51:26
//
#ifndef HPIDHISTOFILLER_H
#define HPIDHISTOFILLER_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hparset.h"

class HCategory;
class HIterator;
class TObjArray;
class TFile;

class HPidHistoFiller : public HReconstructor {
public:

	HPidHistoFiller();
        HPidHistoFiller(Text_t *name,Text_t *title,Char_t*,Char_t*);
	~HPidHistoFiller();

        Bool_t init();
        Bool_t finalize();
        Int_t execute();
        TObjArray* selectPidParticles(char*,TObjArray*);
        TObjArray* selectPidDileptons(char*,TObjArray*);
        void fillPidDileptonHistos(TObjArray*);
        void fillPidParticleHistos(TObjArray*);
        Int_t fillParticleHistosFromParticles(TObjArray*,const Char_t*);
        Int_t fillDetHistosFromParticles(TObjArray*,const Char_t*);
        Int_t fillDileptonHistosFromDileptons(TObjArray*,const Char_t*);
        Int_t fillParticleHistosFromDileptons(TObjArray*,const Char_t*);
        void fillTH1F(const char*,const char*,Float_t);
        void fillTH2F(const char*,const char*,Float_t,Float_t);
        void setHistNameAddition(TString&,const Char_t*,const Char_t*);
        void appendName(TString&,const char*);
        void prependName(TString&,const char*);
        HCategory* getPidParticleCat(){return pPidParticleCat;};
        HCategory* getPidDileptonCat(){return pPidDileptonCat;};
        ClassDef(HPidHistoFiller,1) //ROOT extension

protected:
	Int_t nCounterProcessedNbEvents; 
        void initCuts();
        void initSwitches();
	HCategory* pPidParticleCat; //!
        HIterator* pIterPidParticle; //!
	HCategory* pPidDileptonCat; //!
        HIterator* pIterPidDilepton; //!
        TObjArray* pHistArray;//!
        TObjArray* cuts;//!
        //FILENAME
        char *pFileName;//!
        char *pInFileName;//!
        //FILE
        TFile *pFileOut;//!
        TFile *pFileCutIn;//!
    // CUT NAMES
    TString *particlecut;//!
    TString *dileptoncut;//!
    // SWITCH
    Bool_t kMakeSave;//!
    Bool_t kDetHDileptons;//!
    Bool_t kParticleHDileptons;//!
    Bool_t kDetHParticles;//!
};

#endif // HPIDHISTOFILLER_H

