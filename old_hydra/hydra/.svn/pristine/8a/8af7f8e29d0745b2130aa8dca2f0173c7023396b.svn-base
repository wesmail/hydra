// $Id: hpidsingleleptoneff.h,v 1.11 2008-05-09 16:15:22 halo Exp $
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

#include "hreconstructor.h"



class HCategory;
class HIterator;
class HPairSim;
class HPidParticleSim;
class HGeantKine;
class HPairCutPar;
class HEventHeader;

class TFile;
class TNtuple;

class HPidSingleLeptonEff: public  HReconstructor
{

protected:


    Int_t maxNParticle;

    TFile*   out;
    TNtuple* simYieldEle;
    TNtuple* simYieldPosi;
    TNtuple* effEle;
    TNtuple* effPosi;
    TNtuple* acceEle;
    TNtuple* accePosi;

    HCategory*    pPairCat;       // category of HPairSim
    HIterator*    pitPairCat;     // iterator on the pInputCat category
    HIterator*    pitPairCat1;    // iterator on the pInputCat category
    HCategory*    pPartCat;       // category of HPidParticle
    HIterator*    pitPartCat;     // iterator on the pInputCat category

    HCategory*    pGeantCat;      // category of HGeantKine
    HIterator*    pitGeant;       // iterator on catGeantKine
    HIterator*    pitGeant1;      // iterator on catGeantKine
    HEventHeader* pEvtHeader;

    HPairCutPar*  pC;             // pair cut container

    Int_t         findFirstHitInShower(Int_t numTrack);
    void          checkEff(HGeantKine *p1);
    void          removecloseTracks(Int_t *p);
    Int_t         checkSingleTrack(HPidParticleSim* part,Int_t lepTrackNb,Bool_t& isHiddenLepton);
    Int_t         checkTrackId (HPairSim *p,Int_t lepTrackNb,Int_t& whichPart,Bool_t& isHiddenLepton);
    void          calculateTrackProperties(HPairSim *p,Float_t& th,Float_t& ph,Float_t& p,Int_t& cpF,Int_t whichPart);
    Bool_t        isGeantTrackInAcceptance(HGeantKine *p1);
    Float_t       calcOpeningAngleKine(HGeantKine *kine1,HGeantKine *kine2);
    void          bookNTuple(void);

public:
    HPidSingleLeptonEff(Char_t * file);
    HPidSingleLeptonEff(Text_t name[], Text_t title[],Char_t *file);
    HPidSingleLeptonEff(void){;};
    ~HPidSingleLeptonEff(void);
    void setMaxNParticle(Int_t n){ maxNParticle = n;}

    virtual Int_t       execute (void);
    virtual Bool_t      reinit  (void){ return kTRUE; };
    virtual Bool_t      init    (void);
    virtual Bool_t      finalize(void);

    ClassDef(HPidSingleLeptonEff, 0)
};

#endif /* !HPIDSINGLELEPTONEFF_H */








