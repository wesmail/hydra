// $Id: hpairfiller.h,v 1.4 2009-07-15 11:41:59 halo Exp $
// Author: Thomas.Eberl@ph.tum.de, last modified : 2006-11-13 18:14:38
//
// ---------------------------------------------------------------------------
#ifndef HPAIRFILLER_H
#define HPAIRFILLER_H
// ---------------------------------------------------------------------------
#include <hreconstructor.h>
// ---------------------------------------------------------------------------
class HCategory;
class HIterator;
class TNtuple;
class TObjArray;
class TString;
class TTree;
// ---------------------------------------------------------------------------
class HPair;
class HPairQA;
class HPairSim;
class HPidParticle;
class HPidParticleSim;
// ---------------------------------------------------------------------------

class HPairFiller : public HReconstructor {
public:

	HPairFiller();
        HPairFiller(const Text_t *name,const Text_t *title);
	~HPairFiller();

        Bool_t init();
        Bool_t finalize();
        Int_t execute();

        void setQAFileName(const Char_t* c){cQAFileName=c;};
        TString& getQAFileName(void){return cQAFileName;};


protected:
        void diag();
        Bool_t fillPairCategory(TObjArray*);
	Bool_t fillPair(HPair*);
	Bool_t fillPair(HPairSim*);
        Bool_t deletePairArray(TObjArray*);
        Bool_t fillControlData(TObjArray*);
        void selectPairs(TObjArray*);
        TObjArray* createAllPossiblePairs(void);
        HPair* createPair(HPidParticle*,HPidParticle*);
        HPairSim* createPair(HPidParticleSim*,HPidParticleSim*);
        HCategory* getPidParticleCat(){return pCatPidParticle;};
        HCategory* getPairCat(){return pCatPair;};
    //
        HCategory* pCatPidParticle; //!
        HIterator* pIterPidParticle; //!
	HCategory* pCatPair; //!
        HIterator* pIterPair; //!
        Bool_t kIsReal; //!

        TString cQAFileName; //!
        HPairQA* pQA;//!

ClassDef(HPairFiller,1)
};

#endif // HPAIRFILLER_H

