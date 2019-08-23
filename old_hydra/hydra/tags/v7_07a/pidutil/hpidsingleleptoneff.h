#ifndef HPIDSINGLELEPTONEFF_H
#define HPIDSINGLELEPTONEFF_H

#include "hcategory.h"
#include "hiterator.h"
#include "hreconstructor.h"
#include "TNtuple.h"
#include "TFile.h"
#include "hgeantkine.h"
#include "hpairsim.h"
#include "TString.h"
#include "hpidfl.h"
#include "hpaircutpar.h"

class HPidSingleLeptonEff: public  HReconstructor
{
public:
    HPidSingleLeptonEff(char * file);
    HPidSingleLeptonEff(Text_t name[], Text_t title[],Char_t *file);
    HPidSingleLeptonEff(void){;};
    ~HPidSingleLeptonEff(void);

    virtual Int_t       execute(void);
    virtual Bool_t      reinit(void){ return kTRUE; };
    virtual Bool_t      init(void);
    virtual Bool_t      finalize(void);                 
    void                checkEff(HGeantKine *p1);
    void               removecloseTracks(Int_t *p);
    //    void                pairLoopToCompareMom();
    Bool_t              checkTrackId(HPairSim *p,Int_t lepTrackNb);
    void                calculateTrackProperties(HPairSim *p,Float_t& th,Float_t& ph,Float_t& p,Int_t cpF);
    Bool_t              isGeantTrackInAcceptance(HGeantKine *p1);
    Float_t             calcOpeningAngleKine(HGeantKine *kine1,HGeantKine *kine2);
   
    //++++++++++++++++++++++++++++++++++++++++++++++++++

    HCategory          *pInputCat;      // category of HPairSim
    HIterator          *pitInput;       // iterator on the pInputCat category
    HCategory          *pInputCat1;      // category of HPairSim
    HIterator          *pidPartInput;       // iterator on the pInputCat category

    HCategory          *pGeantCat;      // category of HGeantKine
    HIterator          *pitGeant;  
    HIterator          *pitGeant1;  
   
    HPairCutPar        * pC;           // pair cut container
   
protected:

    
    
    Float_t            errP1,errP2;
    Int_t              lep1TrackNb,lep2TrackNb;
    Int_t              lep1ID,lep2ID; 
    
    TNtuple *simYieldEle, *simYieldPosi, *effEle, *effPosi, *acceEle, *accePosi;
    TFile *out; 
   
ClassDef(HPidSingleLeptonEff, 0) 
};

#endif /* !HPIDSINGLELEPTONEFF_H */








