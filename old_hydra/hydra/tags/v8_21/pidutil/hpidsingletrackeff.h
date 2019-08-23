#ifndef HPIDSINGLETRACKEFF_H
#define HPIDSINGLETRACKEFF_H

#include "hcategory.h"
#include "hiterator.h"
#include "hreconstructor.h"
#include "TNtuple.h"
#include "TFile.h"
#include "hgeantkine.h"
#include "hpairsim.h"
#include "hgeanttof.h"
#include "hgeantshower.h"
#include "hgeantmdc.h"
#include "TString.h"
#include "hpidfl.h"
#include "hpaircutpar.h"
#include "heventheader.h"

class HPidSingleTrackEff: public  HReconstructor
{
public:
    HPidSingleTrackEff(const Char_t * file);
    HPidSingleTrackEff(const Text_t name[],const  Text_t title[],const Char_t *file);
    HPidSingleTrackEff(void) {}
    ~HPidSingleTrackEff(void);

    virtual Int_t       execute(void);
    virtual Bool_t      reinit(void){ return kTRUE; };
    virtual Bool_t      init(void);
    virtual Bool_t      finalize(void);                 
    void                checkEff(HGeantKine *p1);
    void                removecloseTracks(Int_t *p);
    Bool_t              isGeantTrackInAcceptance(HGeantKine *p1);
    Float_t             calcOpeningAngleKine(HGeantKine *kine1,HGeantKine *kine2);

    void bookNTuple(void);
    //++++++++++++++++++++++++++++++++++++++++++++++++++

    HCategory          *pInputCat;      // category of HPairSim
    HIterator          *pidPartInput;       // iterator on the pInputCat category

    HCategory          *pGeantCat;      // category of HGeantKine
    HCategory          *pMdcGeantCat;
    HCategory          *pTofGeantCat;
    HCategory          *pShowerGeantCat;
    HIterator          *pitGeant; 
    HIterator          *pitGeant1;  
    HIterator          *pitMdcGeant;
    HIterator          *pitTofGeant;
    HIterator          *pitShowerGeant;
    HEventHeader       *pEvtHeader;
   
protected:
    
    Int_t lTrack, lId;
    Int_t nStatMDC1, nStatMDC2, nStatMDC3, nStatMDC4;
    Int_t nStatTof, nStatShower;

    TNtuple *simYield, *acc, *eff;
    TFile *out; 
   
ClassDef(HPidSingleTrackEff, 0) 
};

#endif /* !HPIDSINGLETRACKEFF_H */

