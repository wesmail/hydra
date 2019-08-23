// @(#)$Id: hpidparticlehit.cc,v 1.1 2003-03-04 17:53:42 jaskula Exp $
//*-- Author : Marcin Jaskula 03/03/2003

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidParticleHit                                                            //
//                                                                            //
// Identified particle with data from hit level                               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "hpidparticlehit.h"

// -----------------------------------------------------------------------------

ClassImp(HPidParticleHit)

// -----------------------------------------------------------------------------

HPidParticleHit::HPidParticleHit(Double_t dM0, Double_t dPx,
                            Double_t dPy, Double_t dPz)
                : HPidParticle(dM0, dPx, dPy, dPz)
{
// Constuctor with kinematic parameters of the particle
// dM0 - mass (MeV/c^2), dPx, dPy, dPz (MeV/c),
}

// -----------------------------------------------------------------------------

HPidParticleHit::HPidParticleHit(Short_t nPid, Double_t dPx,
                            Double_t dPy, Double_t dPz)
                : HPidParticle(nPid, dPx, dPy, dPz)
{
// Constuctor with kinematic parameters of the particle and its id
// Particle id, Px, Py, Pz (MeV/c)
}

// -----------------------------------------------------------------------------

HPidParticleHit::HPidParticleHit(const Char_t *sPid, Double_t dPx,
                            Double_t dPy, Double_t dPz)
                : HPidParticle(sPid, dPx, dPy, dPz)
{
// Constuctor with kinematic parameters of the particle and its name
// Particle id, Px, Py, Pz (MeV/c)
}

// -----------------------------------------------------------------------------

void HPidParticleHit::Clear(Option_t *)
{
// Clear all variables

    HPidParticle::Clear();
    cHitData.reset();
}

// -----------------------------------------------------------------------------

void HPidParticleHit::print(void) const
{
// Print info about the particle

    HPidParticle::print();
    cHitData.print();
}
