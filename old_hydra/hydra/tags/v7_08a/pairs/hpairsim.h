// $Id: hpairsim.h,v 1.2 2005-04-27 18:04:11 eberl Exp $
// Author: Thomas.Eberl@ph.tum.de, last modified : 2005-04-27 18:53:13
//
// ---------------------------------------------------------------------------
#ifndef HPAIRSIM_H
#define HPAIRSIM_H
// ---------------------------------------------------------------------------
#include "hpair.h"
// ---------------------------------------------------------------------------
class HPidParticleSim;
// ---------------------------------------------------------------------------

class HPairSim: public HPair {


public:

    HPairSim();
    HPairSim(HPidParticleSim*, HPidParticleSim*);
    virtual ~HPairSim();

    HPairSim(const HPairSim& source);
    virtual void print(void);

// ---------------------------------------------------------------------------

protected:

    virtual void reset(void);
    Bool_t calcPairPropertiesSim(HPidParticleSim*, HPidParticleSim*);
    //
    TLorentzVector GLorentzVector1;
    TLorentzVector GLorentzVector2;
    Int_t nGPid1;
    Int_t nGPid2;
    Float_t fGOpang;
    Float_t fGInvMass;
    Char_t cIsTrue;
    Char_t cGCharge;

// ---------------------------------------------------------------------------
    ClassDef(HPairSim,1)

};
#endif // HPAIRSIM_H
