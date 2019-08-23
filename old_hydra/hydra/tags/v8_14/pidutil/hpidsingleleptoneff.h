// $Id: hpidsingleleptoneff.h,v 1.7 2007-05-24 10:39:54 jacek Exp $
// Author: Laura Fabbietti <L.Fabbietti@physik.tu-muenchen.de>
// Last update by Laura Fabbietti: 05/06/05 21:47:04
// Modified in March 2006 by W. Przygoda / T. Wojcik (Cracow)
// Thomas.Eberl@ph.tum.de, last modified : 2006-11-08 10:42:49
// 
// This task is used to generate ntuples of embedded leptons 
// with white distributions in 1/p, theta, phi.
// There are ntuples with such leptons in the acceptance and 
// additionally identified and accepted leptons.
// These ntuples are later divided by each other (external code)
// to get (p,theta,phi) distributions of reconstruction efficiency
// 
#ifndef HPIDSINGLELEPTONEFF_H
#define HPIDSINGLELEPTONEFF_H

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
    //void                pairLoopToCompareMom();
    Int_t              checkTrackId(HPairSim *p,Int_t lepTrackNb);
    Bool_t             checkLeptonId(HPairSim *pair);
    void                calculateTrackProperties(HPairSim *p,Float_t& th,Float_t& ph,Float_t& p,Int_t& cpF);
    Bool_t              isGeantTrackInAcceptance(HGeantKine *p1);
    Float_t             calcOpeningAngleKine(HGeantKine *kine1,HGeantKine *kine2);

    void bookNTuple(void);
    //++++++++++++++++++++++++++++++++++++++++++++++++++

    HCategory          *pInputCat;      // category of HPairSim
    HIterator          *pitInput;       // iterator on the pInputCat category
    HIterator          *pitInput1;       // iterator on the pInputCat category
    HCategory          *pInputCat1;      // category of HPairSim
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
   
    HPairCutPar        * pC;           // pair cut container
   
protected:

    
    
    Float_t            errP1,errP2;
    Int_t              lep1TrackNb,lep2TrackNb;
    Int_t              lep1ID,lep2ID; 
   
    Int_t lTrack, lId;
    Int_t nStatMDC1, nStatMDC2, nStatMDC3, nStatMDC4;
    Int_t nStatTof, nStatShower;

    TNtuple *simYieldEle, *simYieldPosi, *effEle, *effPosi;
    TNtuple *fakeEle, *fakePosi, *acceEle, *accePosi;
    TFile *out; 
   
ClassDef(HPidSingleLeptonEff, 0) 
};

#endif /* !HPIDSINGLELEPTONEFF_H */








