// @(#)$Id: hpidparticlesim.cc,v 1.4 2003-03-04 17:52:33 jaskula Exp $
//*-- Author : Marcin Jaskula 11/11/2002
//  Modified : Marcin Jaskula 01/12/2002
//             new method getGeantTrackSet()
//             getGeantKine() rewritten
//  Modified : Marcin Jaskula 26/02/2003
//             nGeantId & nGeantTrack -> cCommonTrack structure

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidParticleSim                                                            //
//                                                                            //
// Identified particle for sim data                                           //
// All members and methods from HPidParticle                                  //
// and the first HPidCommonTrack from the corresponding HPidGeantTrackSet     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "hpidparticlesim.h"

#include "hades.h"
#include "hevent.h"
#include "hlinearcategory.h"

#include <TError.h>
#include <TVector3.h>

#include "piddef.h"
#include "hpidphysicsconstants.h"

#include "hpidgeanttrackset.h"

#include "hpidtrackcand.h"
#include "hpidcandidate.h"

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
// Called in the constructor
}

// -----------------------------------------------------------------------------

void HPidParticleSim::Clear(Option_t *)
{
// Clear all variables

    HPidParticle::Clear();
    clearTracks();
}

// -----------------------------------------------------------------------------

void HPidParticleSim::print(void) const
{
// Print info about the particle

    HPidParticle::print();
    printTracks();
}

// -----------------------------------------------------------------------------

HGeantKine* HPidParticleSim::getGeantKine(HCategory *pCat) const
{
// Returns HGeantKine object corresponding to nGeantTrack (if exists)
// Works when pCat is set or gHades->getCurrentEvent() is accessible

    if(getGeantTrack() <= 0)
        return NULL;

    return HPidGeantTrackSet::getGeantKine(getGeantTrack(), pCat);
}

// -----------------------------------------------------------------------------

HGeantKine* HPidParticleSim::getGeantKine(Short_t nTrack,
                                HCategory *pCat)
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

// -----------------------------------------------------------------------------

Int_t HPidParticleSim::getValidCommonTracks(void) const
{
// Return number of the filled "common tracks" - the tracks for which
// getDetectors() != HPidCommonTrack::kTrackNotSet

UInt_t i;
UInt_t iMax = getMaxCommonTracks();

    for(i = 0; i < iMax; i++)
    {
        if(getTrack(i)->getDetectors() == HPidCommonTrack::kTrackNotSet)
            return i;
    }

    return iMax;
}

// -----------------------------------------------------------------------------

void HPidParticleSim::clearTracks(void)
{
// Clear all common tracks

UInt_t i;
UInt_t iMax = getMaxCommonTracks();

    for(i = 0; i < iMax; i++)
        getTrackFullAccess(i)->Clear();
}

// -----------------------------------------------------------------------------

Bool_t HPidParticleSim::setTrack(const HPidCommonTrack *pSet, UInt_t iPos)
{
// Set the parameters of the selected HPidCommonTrack

HPidCommonTrack *p;

    if((p = getTrackFullAccess(iPos)) != NULL)
    {
        p->set(pSet);
        return kTRUE;
    }

    return kFALSE;
}

// -----------------------------------------------------------------------------

void HPidParticleSim::printTracks(Bool_t bAll) const
{
// Print all common tracks. If bAll == kFALSE only the filled (valid) tracks
// are printed.

UInt_t   i, iMax;

    iMax = (bAll) ? getMaxCommonTracks() : getValidCommonTracks();
    printf("Common tracks: %d / %d\n", iMax, getMaxCommonTracks());

    for(i = 0; i < iMax; i++)
    {
        printf("Track: %d\n", i);
        getTrack(i)->print();
    }
}

// -----------------------------------------------------------------------------

void HPidParticleSim::Streamer(TBuffer &R__b)
{
// Stream an object of class HPidParticleSim.

UInt_t R__s, R__c;

    if(R__b.IsReading())
    {
    Version_t R__v = R__b.ReadVersion(&R__s, &R__c);

        HPidParticle::Streamer(R__b);

        if(R__v == 1)
        {
        Short_t n;

            R__b >> n;
            cTrack0.setGeantId(n);
            R__b >> n;
            cTrack0.setTrackId(n);
        }
        else
        {
            cTrack0.Streamer(R__b);
        }

        R__b.CheckByteCount(R__s, R__c, HPidParticleSim::IsA());
    }
    else
    {
        R__c = R__b.WriteVersion(HPidParticleSim::IsA(), kTRUE);
        HPidParticle::Streamer(R__b);
        cTrack0.Streamer(R__b);
        R__b.SetByteCount(R__c, kTRUE);
    }
}
