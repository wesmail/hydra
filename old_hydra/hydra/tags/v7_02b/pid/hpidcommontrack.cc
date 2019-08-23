// @(#)$Id: hpidcommontrack.cc,v 1.3 2003-10-23 18:54:25 jaskula Exp $
//*-- Author : Marcin Jaskula 25/02/2003
//  Modified : Marcin Jaskula 29/05/2003
//             getLorentzVector added
//  Modified : Marcin Jaskula 22/06/2003
//             static method getLorentzVector from HGeantKine added

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidCommonTrack                                                            //
//                                                                            //
// Geant tracks matched in HPidTrackCand                                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "hpidcommontrack.h"
#include "hpidgeanttrackset.h"
#include "hpidtrackcand.h"

#include <TError.h>
#include "hgeantkine.h"

#include "hpidphysicsconstants.h"

// -----------------------------------------------------------------------------

ClassImp(HPidCommonTrack)

// -----------------------------------------------------------------------------

HGeantKine* HPidCommonTrack::getGeantKine(Int_t iTrack, HCategory *pCat)
{
// Return HGeantKine object corresponding to nTrack from pCat.
// If the pCat is not set (NULL) catGeantKine category from gHades is used.

    return HPidGeantTrackSet::getGeantKine(iTrack, pCat);
}

// -----------------------------------------------------------------------------

void HPidCommonTrack::setDefault(void)
{
// Clear all members

    cDetectors = kTrackNotSet;
    iProcess   = 0;
    iMask      = 0;
    nTrackId   = 0;
    nGeantId   = 0;

    vMomentum.SetXYZ(0., 0., 0.);
}

// -----------------------------------------------------------------------------

void HPidCommonTrack::set(const HPidCommonTrack *pSet)
{
// Copy parmeters fro pSet. If pSet == NULL setDefault() is called to clear
// the class memebers

    if(pSet == NULL)
        setDefault();
    else
    {
        cDetectors = pSet->cDetectors;
        iProcess   = pSet->iProcess;
        iMask      = pSet->iMask;
        nTrackId   = pSet->nTrackId;
        nGeantId   = pSet->nGeantId;

        vMomentum  = pSet->vMomentum;
    }
}

// -----------------------------------------------------------------------------

Bool_t HPidCommonTrack::fillKine(UInt_t iMask)
{
// Fill parameters of the track from HGenatKine object.
// If iMask != 0 HPidPhysicsConstants::fillProcesses method is called to
// fill iProcess member.
// Returns status of the filling.

    if(nTrackId <= 0)
        return kFALSE;

HGeantKine *pKine;

    if((pKine = getGeantKine(nTrackId)) == NULL)
    {
        ::Error("HPidCommonTrack::fillKine",
                "Cannot get kine object: %d", nTrackId);

        return kFALSE;
    }

Float_t fX, fY, fZ;
Int_t   iTrack, iId;

    pKine->getParticle(iTrack, iId);
    nGeantId = iId;

    pKine->getMomentum(fX, fY, fZ);
    vMomentum.SetXYZ(fX, fY, fZ);

    this->iMask = iMask;
    if(iMask != 0)
        iProcess = HPidPhysicsConstants::getProcess(pKine, iMask);
    else
        iProcess = 0;

    return kTRUE;
}

// -----------------------------------------------------------------------------

TLorentzVector HPidCommonTrack::getLorentzVector(void) const
{
// Bulit TLorentzVector from the momentum and the particle mass from the track
TLorentzVector v;

    if(cDetectors != HPidCommonTrack::kTrackNotSet)
        v.SetVectM(vMomentum, HPidPhysicsConstants::mass(nGeantId));

    return v;

}

// -----------------------------------------------------------------------------

TLorentzVector HPidCommonTrack::getLorentzVector(HGeantKine *pKine)
{
// Bulit TLorentzVector from the HGeantKine
TLorentzVector v;

    if(pKine == NULL)
    {
        ::Error("HPidCommonTrack::getLorentzVector", "pKine == NULL");
        return v;
    }

Float_t fX, fY, fZ;
Int_t   iT, iId;

    pKine->getParticle(iT, iId);
    pKine->getMomentum(fX, fY, fZ);

    v.SetXYZM(fX, fY, fZ, HPidPhysicsConstants::mass(iId));

    return v;
}

// -----------------------------------------------------------------------------

TLorentzVector HPidCommonTrack::getLorentzVector(Short_t nTrack,
                                HCategory *pCat)
{
// Bulit TLorentzVector from the HGeantKine taken from the category

    return getLorentzVector(getGeantKine(nTrack, pCat));
}


// -----------------------------------------------------------------------------

void HPidCommonTrack::print(void) const
{
// Print all the members

    printf("HPidCommonTrack parameters:\n");
    printf("Detectors    : %5d\n", cDetectors);
    printf("Geant TrackId: %5d\n", nTrackId);
    printf("Geant Pid    : %5d (%s)\n", nGeantId,
                                    HPidPhysicsConstants::pid(nGeantId));
    printf("Process      : 0x%08X\n", iProcess);
    printf("Momentum     : %8.3f %8.3f %8.3f (%8.3f)\n", vMomentum.X(),
            vMomentum.Y(), vMomentum.Z(), vMomentum.Mag());
}
