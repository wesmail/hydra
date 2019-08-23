// $Id: hpairevtfilter.h,v 1.2 2006-11-24 16:14:03 eberl Exp $
// Author: Thomas.Eberl@ph.tum.de, last modified : 2006-11-10 14:57:49
//
// --------------------------------------------------------------------------  
#ifndef HPAIREVTFILTER_H
#define HPAIREVTFILTER_H
// --------------------------------------------------------------------------  
#include <hreconstructor.h>
// --------------------------------------------------------------------------  
class TString;
// --------------------------------------------------------------------------  
class HCategory;
class HIterator;
// --------------------------------------------------------------------------  

class HPairEvtFilter : public HReconstructor {
public:

	HPairEvtFilter();
	HPairEvtFilter(Text_t *name,Text_t *title,
		       TString opt, Int_t nLower=0, Int_t nUpper=10000);
	HPairEvtFilter(Text_t *name,Text_t *title, TString opt,
			       Double_t zMin, Double_t zMax);
	~HPairEvtFilter();

        Bool_t init();
        Bool_t finalize();
        Int_t execute();



protected:

    void resetCounters();
    Bool_t isDiscardedEvt(TString);
    Bool_t isSkipNextTask(Int_t);


    // event selectors
    Bool_t isParticleMultRange();
    Bool_t isTarget(Int_t);
	Bool_t isTargetZ(Double_t zMin, Double_t zMax);

    // counters
    Int_t nCntProcessedEvents;//!
    Int_t nCounterNbRejectedEvts;//!

    // input categories
    HCategory* pCatPidParticle; //!

    // category getters
    HCategory* getPidParticleCat() { return pCatPidParticle; };

    // category iterators
    HIterator* pIterPidParticle; //!

    //
    Bool_t kIsReal;//!

    // CUT PARAMETERS
    // multiplicities 
    Int_t nLowerMult;//!
    Int_t nUpperMult;//!
    // vertex range
    Double_t zVertMin;//!
    Double_t zVertMax;//!

    // string to select
    TString filterOption; //!


    ClassDef(HPairEvtFilter,1) //Event filter of the pair module
    
};

#endif // HPAIREVTFILTER_H
