// @(#)$Id: hpidparticlesimexthit.cc,v 1.1 2003-03-04 17:54:12 jaskula Exp $
//*-- Author : Marcin Jaskula 03/03/2003

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidParticleSimExtHit                                                      //
//                                                                            //
// Identified particle for SIM data with Geant track ids                      //
// and data from hit level.                                                   //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "hpidparticlesimexthit.h"

// -----------------------------------------------------------------------------

ClassImp(HPidParticleSimExtHit)

// -----------------------------------------------------------------------------

HPidParticleSimExtHit::HPidParticleSimExtHit(Double_t dM0, Double_t dPx,
                            Double_t dPy, Double_t dPz)
                : HPidParticleSimExt(dM0, dPx, dPy, dPz)
{
// Constuctor with kinematic parameters of the particle
// dM0 - mass (MeV/c^2), dPx, dPy, dPz (MeV/c),
}

// -----------------------------------------------------------------------------

HPidParticleSimExtHit::HPidParticleSimExtHit(Short_t nPid, Double_t dPx,
                            Double_t dPy, Double_t dPz)
                : HPidParticleSimExt(nPid, dPx, dPy, dPz)
{
// Constuctor with kinematic parameters of the particle and its id
// Particle id, Px, Py, Pz (MeV/c)
}

// -----------------------------------------------------------------------------

HPidParticleSimExtHit::HPidParticleSimExtHit(const Char_t *sPid, Double_t dPx,
                            Double_t dPy, Double_t dPz)
                : HPidParticleSimExt(sPid, dPx, dPy, dPz)
{
// Constuctor with kinematic parameters of the particle and its name
// Particle id, Px, Py, Pz (MeV/c)
}

// -----------------------------------------------------------------------------

void HPidParticleSimExtHit::Clear(Option_t *)
{
// Clear all variables

    HPidParticleSimExt::Clear();
    cHitData.reset();
}

// -----------------------------------------------------------------------------

void HPidParticleSimExtHit::print(void) const
{
// Print info about the particle

    HPidParticleSimExt::print();
    cHitData.print();
}
