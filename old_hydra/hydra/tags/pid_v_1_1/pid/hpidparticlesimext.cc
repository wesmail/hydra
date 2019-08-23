// @(#)$Id: hpidparticlesimext.cc,v 1.2 2003-03-04 17:53:23 jaskula Exp $
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

    setDefault();
}

// -----------------------------------------------------------------------------

HPidParticleSimExt::HPidParticleSimExt(Short_t nPid, Double_t dPx,
                            Double_t dPy, Double_t dPz)
                : HPidParticleSim(nPid, dPx, dPy, dPz)
{
// Constuctor with kinematic parameters of the particle and its id
// Particle id, Px, Py, Pz (MeV/c)

    setDefault();
}

// -----------------------------------------------------------------------------

HPidParticleSimExt::HPidParticleSimExt(const Char_t *sPid, Double_t dPx,
                            Double_t dPy, Double_t dPz)
                : HPidParticleSim(sPid, dPx, dPy, dPz)
{
// Constuctor with kinematic parameters of the particle and its name
// Particle id, Px, Py, Pz (MeV/c)

    setDefault();
}

// -----------------------------------------------------------------------------

void HPidParticleSimExt::setDefault(void)
{
// Called in the constructors.
}

// -----------------------------------------------------------------------------

void HPidParticleSimExt::Clear(Option_t *)
{
// Clear all variables

    HPidParticleSim::Clear();
    cTrackSet.reset();
}

// -----------------------------------------------------------------------------

void HPidParticleSimExt::print(void) const
{
// Print info about the particle

    HPidParticleSim::print();
    cTrackSet.print();
}

// -----------------------------------------------------------------------------

const HPidGeantTrackSet* HPidParticleSimExt::getGeantTrackSet(void) const
{
    return (const HPidGeantTrackSet *) &cTrackSet;
}

// -----------------------------------------------------------------------------

void HPidParticleSimExt::setGeantTrackSet(const HPidGeantTrackSet *pSet)
{
    if(pSet == NULL)
        return;

    memcpy(&cTrackSet, pSet, sizeof(HPidGeantTrackSet));
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

// -----------------------------------------------------------------------------

void HPidParticleSimExt::Streamer(TBuffer &R__b)
{
// Stream an object of class HPidParticleSimExt.

UInt_t R__s, R__c;

    if (R__b.IsReading())
    {
    Version_t R__v = R__b.ReadVersion(&R__s, &R__c);

        HPidParticleSim::Streamer(R__b);
        cTrackSet.Streamer(R__b);

        if(R__v > 1)
        {
            cTrack1.Streamer(R__b);
            cTrack2.Streamer(R__b);
            cTrack3.Streamer(R__b);
            cTrack4.Streamer(R__b);
        }

        R__b.CheckByteCount(R__s, R__c, HPidParticleSimExt::IsA());
    }
    else
    {
        R__c = R__b.WriteVersion(HPidParticleSimExt::IsA(), kTRUE);
        HPidParticleSim::Streamer(R__b);
        cTrackSet.Streamer(R__b);
        cTrack1.Streamer(R__b);
        cTrack2.Streamer(R__b);
        cTrack3.Streamer(R__b);
        cTrack4.Streamer(R__b);
        R__b.SetByteCount(R__c, kTRUE);
    }
}
