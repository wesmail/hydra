// @(#)$Id: hpidparticlesimext.cc,v 1.1 2002-12-03 14:46:10 jaskula Exp $
//*-- Author : Marcin Jaskula 01/12/2002

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidParticleSimExt                                                         //
//                                                                            //
// Identified particle for SIM data with Geant track ids                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "hpidparticlesimext.h"

#include "hades.h"
#include "hevent.h"
#include "hlinearcategory.h"

#include <TError.h>

#include "piddef.h"
#include "hpidgeanttrackset.h"

// -----------------------------------------------------------------------------

ClassImp(HPidParticleSimExt)

// -----------------------------------------------------------------------------

HPidParticleSimExt::HPidParticleSimExt(Double_t dM0, Double_t dPx,
                            Double_t dPy, Double_t dPz)
                : HPidParticleSim(dM0, dPx, dPy, dPz)
{
// Constuctor with kinematic parameters of the particle
// dM0 - mass (MeV/c^2), dPx, dPy, dPz (MeV/c),
}

// -----------------------------------------------------------------------------

HPidParticleSimExt::HPidParticleSimExt(Short_t nPid, Double_t dPx,
                            Double_t dPy, Double_t dPz)
                : HPidParticleSim(nPid, dPx, dPy, dPz)
{
// Constuctor with kinematic parameters of the particle and its id
// Particle id, Px, Py, Pz (MeV/c)
}

// -----------------------------------------------------------------------------

HPidParticleSimExt::HPidParticleSimExt(const Char_t *sPid, Double_t dPx,
                            Double_t dPy, Double_t dPz)
                : HPidParticleSim(sPid, dPx, dPy, dPz)
{
// Constuctor with kinematic parameters of the particle and its name
// Particle id, Px, Py, Pz (MeV/c)
}

// -----------------------------------------------------------------------------

void HPidParticleSimExt::Clear(Option_t *)
{
// Clear all variables

    HPidParticleSim::Clear();
    cGeantTracks.reset();
}

// -----------------------------------------------------------------------------

void HPidParticleSimExt::print(void) const
{
// Print info about the particle

    HPidParticleSim::print();
    cGeantTracks.print();
}

// -----------------------------------------------------------------------------

void HPidParticleSimExt::setGeantTrackSet(const HPidGeantTrackSet *pSet)
{
    if(pSet == NULL)
        return;

    cGeantTracks = *pSet;
}

// -----------------------------------------------------------------------------

HCategory* HPidParticleSimExt::buildPidParticleSimExtCategory(void)
{
// Static function for making the category HPidParticleSimExt

HCategory    *pCat;
HEvent       *pEvent;

    if((gHades == NULL) || ((pEvent = gHades->getCurrentEvent()) == NULL))
    {
        ::Error("HPidParticleSimExt::buildPidParticleSimExtCategory",
                    "Cannot access current event");

        return NULL;
    }

    if((pCat = pEvent->getCategory(catPidPart)) != NULL)
        return pCat;

    if((pCat = new HLinearCategory("HPidParticleSimExt", 1000)) == NULL)
    {
        ::Error("HPidParticleSimExt::buildPidParticleSimExtCategory",
                    "Cannot create new category");

        return NULL;
    }

    pEvent->addCategory(catPidPart, pCat, "Pid");

    return pCat;
}
