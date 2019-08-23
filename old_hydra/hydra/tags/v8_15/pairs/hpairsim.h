// $Id: hpairsim.h,v 1.3 2006-08-16 15:42:18 halo Exp $
// Author: Thomas.Eberl@ph.tum.de, last modified : 2006-04-20 15:44:30
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
