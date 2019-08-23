// @(#)$Id: hpidcandidatebase.h,v 1.1 2002-10-30 10:55:33 halo Exp $

#ifndef HPIDCANDIDATE_AAAAAA_PPPPPP_H
#define HPIDCANDIDATE_AAAAAA_PPPPPP_H

// -----------------------------------------------------------------------------

#include "hpidcandidate.h"

////////////////////////////////////////////////////////////////////////////////
//
// HPidCandidate_AAAAAA_PPPPPP
//
// HPidCandidate for AAAAAA algorithms and PPPPPP particle spices
//
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------

#define NUM_ALGORITHMS AAAAAA
#define NUM_PARTICLES  PPPPPP

// -----------------------------------------------------------------------------

class HPidCandidate_AAAAAA_PPPPPP : public HPidCandidate
{
public:
                        HPidCandidate_AAAAAA_PPPPPP(void) { Clear(); };
                       ~HPidCandidate_AAAAAA_PPPPPP(void) {};


    // -------------------------------------------------------------------------

private:

    Int_t               aAlgorithms[NUM_ALGORITHMS];
    Short_t             aPidObjIds[NUM_ALGORITHMS];
    Short_t             aParticles[NUM_PARTICLES];
    Float_t             aValues[NUM_ALGORITHMS][NUM_PARTICLES];

    // -------------------------------------------------------------------------

public:

    // -------------------------------------------------------------------------

    virtual UInt_t      getNParticles(void) const   { return NUM_PARTICLES;    }
    virtual UInt_t      getNAlgorithms(void) const  { return NUM_ALGORITHMS;   }

    // -------------------------------------------------------------------------

    virtual Int_t*      getAlgorithms(void) const
                                { return (Int_t *) aAlgorithms;                }

    virtual Short_t*    getPidObjIds(void) const
                                { return (Short_t *) aPidObjIds;               }

    virtual Short_t*    getParticles(void) const
                                { return (Short_t *) aParticles;               }

    virtual Float_t*    getValues(void) const
                                { return (Float_t *) aValues;                  }

    // -------------------------------------------------------------------------

    ClassDef(HPidCandidate_AAAAAA_PPPPPP, 1)
};

// -----------------------------------------------------------------------------

#undef NUM_ALGORITHMS
#undef NUM_PARTICLES

#endif // HPIDCANDIDATE_AAAAAA_PPPPPP_H
