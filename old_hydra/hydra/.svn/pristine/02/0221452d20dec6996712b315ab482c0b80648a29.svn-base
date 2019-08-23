// @(#)$Id: hpidparticlesim.cc,v 1.2 2002-12-03 14:45:01 jaskula Exp $
//*-- Author : Marcin Jaskula 11/11/2002
//  Modified : Marcin Jaskula 01/12/2002
//             new method getGeantTrackSet()
//             getGeantKine() rewritten

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidParticleSim                                                            //
//                                                                            //
// Identified particle for sim data                                           //
// All members and methods from HPidParticle + :                              //
//                                                                            //
// nGeantTrack - Geant track which was found in Meta, MDC and RICH (if  used).//
//               -1 - track was not matched in all used detectors             //
// nGeantId    - Geant pid for the matched track.                             //
//               -1 - track was not matched                                   //
//               -2 - track nGeantTrack was not found in KGeantKine cat.      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "hpidparticlesim.h"

#include "hades.h"
#include "hevent.h"
#include "hlinearcategory.h"

#include <TError.h>

#include "piddef.h"
#include "hpidphysicsconstants.h"

#include "hpidgeanttrackset.h"

// -----------------------------------------------------------------------------

ClassImp(HPidParticleSim)

// -----------------------------------------------------------------------------

HPidParticleSim::HPidParticleSim(Double_t dM0, Double_t dPx,
                            Double_t dPy, Double_t dPz)
                : HPidParticle(dM0, dPx, dPy, dPz)
{
// Constuctor with kinematic parameters of the particle
// dM0 - mass (MeV/c^2), dPx, dPy, dPz (MeV/c),

    setDefault();
}

// -----------------------------------------------------------------------------

HPidParticleSim::HPidParticleSim(Short_t nPid, Double_t dPx,
                            Double_t dPy, Double_t dPz)
                : HPidParticle(nPid, dPx, dPy, dPz)
{
// Constuctor with kinematic parameters of the particle and its id
// Particle id, Px, Py, Pz (MeV/c)

    setDefault();
}

// -----------------------------------------------------------------------------

HPidParticleSim::HPidParticleSim(const Char_t *sPid, Double_t dPx,
                            Double_t dPy, Double_t dPz)
                : HPidParticle(sPid, dPx, dPy, dPz)
{
// Constuctor with kinematic parameters of the particle and its name
// Particle id, Px, Py, Pz (MeV/c)

    setDefault();
}

// -----------------------------------------------------------------------------

void HPidParticleSim::setDefault(void)
{
    nGeantId    = -1;
    nGeantTrack = -1;
}

// -----------------------------------------------------------------------------

void HPidParticleSim::Clear(Option_t *)
{
// Clear all variables

    HPidParticle::Clear();
    setDefault();
}

// -----------------------------------------------------------------------------

void HPidParticleSim::print(void) const
{
// Print info about the particle

    HPidParticle::print();

    printf("Geant Id : %d \"%s\"\n", nGeantId,
                (nGeantId > 0) ?  HPidPhysicsConstants::pid(nGeantId) : "");
    printf("Geant Trk: %d\n", nGeantTrack);
}

// -----------------------------------------------------------------------------

HGeantKine* HPidParticleSim::getGeantKine(HCategory *pCat) const
{
// Returns HGeantKine object corresponding to nGeantTrack (if exists)
// Works when pCat is set or gHades->getCurrentEvent() is accessible

    if(nGeantTrack <= 0)
        return NULL;

    return HPidGeantTrackSet::getGeantKine(nGeantTrack, pCat);
}

// -----------------------------------------------------------------------------

HGeantKine* HPidParticleSim::getGeantKine(Short_t nTrack,
                                HCategory *pCat = NULL)
{
// Static function which returns HGeantKine object corresponding to nTrack.
// Works when pCat is set or gHades->getCurrentEvent() is accessible

    return HPidGeantTrackSet::getGeantKine(nTrack, pCat);
}

// -----------------------------------------------------------------------------

HCategory* HPidParticleSim::buildPidParticleSimCategory(void)
{
// Static function for making the category HPidParticleSim

HCategory    *pCat;
HEvent       *pEvent;

    if((gHades == NULL) || ((pEvent = gHades->getCurrentEvent()) == NULL))
    {
        ::Error("HPidParticleSim::buildPidParticleSimCategory",
                    "Cannot access current event");

        return NULL;
    }

    if((pCat = pEvent->getCategory(catPidPart)) != NULL)
        return pCat;

    if((pCat = new HLinearCategory("HPidParticleSim", 1000)) == NULL)
    {
        ::Error("HPidParticleSim::buildPidParticleSimCategory",
                    "Cannot create new category");

        return NULL;
    }

    pEvent->addCategory(catPidPart, pCat, "Pid");

    return pCat;
}

// -----------------------------------------------------------------------------

const HPidGeantTrackSet* HPidParticleSim::getGeantTrackSet(void) const
{
// Return HPidGeantTrackSet from tracks stored in the corresponding
// HKickTrackSim and HRichHitSim (if exist and are accessible).
// The returned pointer points to one common static HPidGeantTrackSet for
// all HPidParticleSim, so one needs to store the output they needs to copy it.
// For warning info see HPidTrackCand::getGeantTrackSet.
// If the correspondig HPidTrackCand is not accessible a warning is printed
// and NULL is returned.

HPidCandidate *pCand;
HPidTrackCand *pTrack;

    if(((pCand = getPidCandidate()) != NULL)
            && ((pTrack = pCand->getCandidate()) != NULL))
    {
        return pTrack->getGeantTrackSet();
    }

    Warning("getGeantTrackSet", "Cannot access HPidTrackCand");

    return NULL;
}
