// @(#)$Id: hpidparticlefillersim.cc,v 1.1 2002-11-20 17:27:37 jaskula Exp $
//*-- Author : Marcin Jaskula 13/11/2002

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidParticleFillerSim                                                      //
//                                                                            //
// Fill HPidParticleSim like HPidParticleFiller does for HPidParticle.        //
// Then matching of Geant tracks before and after magnetic field is done.     //
// The same track is searched in fMetaTracks and fMdcTracks from HKickTrackSim//
// and (if bNoRich flag is not set - NORICH flag not set in the constructor)  //
// in corresponding HRichHitSim as well.                                      //
// If several tracks match then the first is used.                            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "hcategory.h"
#include "hiterator.h"

#include "hpidparticlefillersim.h"

#include "hpidcandidate.h"
#include "hpidtrackcand.h"
#include "hkicktracksim.h"
#include "hpidparticlesim.h"

#include "hades.h"
#include "hevent.h"

#include "hlinearcategory.h"
#include "piddef.h"

#include "hgeantkine.h"
#include "hrichhitsim.h"

// -----------------------------------------------------------------------------

ClassImp(HPidParticleFillerSim)

// -----------------------------------------------------------------------------

#define SAFE_DELETE(A) { if(A) { delete (A); A = NULL; }}

// -----------------------------------------------------------------------------

HPidParticleFillerSim::HPidParticleFillerSim(Option_t par[])
            : HPidParticleFiller("PidPartFillerSim",
                                 "Filler of HPidParticleSim cat.", par)
{
// Default constructor.
// par parameters is used in constructor of HPidParticleFiller class.

    setDefault();
}

// -----------------------------------------------------------------------------

HPidParticleFillerSim::HPidParticleFillerSim(Text_t name[], Text_t title[],
                                        Option_t par[])
            : HPidParticleFiller(name, title, par)
{
// Constructor with names
// par parameters is used in constructor of HPidParticleFiller class.

    setDefault();
}

// -----------------------------------------------------------------------------

HPidParticleFillerSim::~HPidParticleFillerSim(void)
{
// Destructor
}

// -----------------------------------------------------------------------------

void HPidParticleFillerSim::setDefault(void)
{
// Called in constructors to set default valuest to the members

    bNoRich = kFALSE;
}

// -----------------------------------------------------------------------------

Bool_t HPidParticleFillerSim::buildOutputCategory(void)
{
// Build all categories

    // output category
    if((pOutCat = HPidParticleSim::buildPidParticleSimCategory()) == NULL)
    {
        Error("init", "Cannot build HPidParticleSim category");
        return kFALSE;
    }

    return kTRUE;
}

// -----------------------------------------------------------------------------

void HPidParticleFillerSim::setParameters(Option_t par[])
{
// Set parameters by names. See parameters in HPidParticleFiller::setParameters
// + options:
// NORICH - do not use Geant Tracks from RICH in the calculations

    HPidParticleFiller::setParameters(par);

TString s = par;

    s.ToUpper();

    bNoRich = kFALSE;
    if(strstr(s.Data(), "NORICH") != NULL)
        bNoRich = kTRUE;
}

// -----------------------------------------------------------------------------

HPidParticle* HPidParticleFillerSim::getNextSlot(void)
{
// Get next slot from the output category

HPidParticleSim  *pOut = NULL;
static HLocation  locDummy;

    if(pOutCat == NULL)
    {
        Error("getNextSlot", "Output category not set: use init/reinit");
        return NULL;
    }

    if((pOut = (HPidParticleSim *) pOutCat->getNewSlot(locDummy)) == NULL)
    {
        Error("getNextSlot", "No new slot");
        return NULL;
    }

    pOut = new(pOut) HPidParticleSim();

    return pOut;
}

// -----------------------------------------------------------------------------

HPidParticle* HPidParticleFillerSim::makeParticle(HPidCandidate *pCand,
                                HKickTrack *pKickTrack, Int_t iIdx)
{
// Make a new HPidParticleSim from the input data and returns pointer on it.
// Parameters from HPidParticle are filled by HPidParticleFiller::makeParticle()
// Then the searching for geant tracks before and after magnetic field
// is done. If bNoRich flag is not set, then track from RICH is NOT used
// in matching. For more info see the class description.

HPidParticleSim* pParticle;

    if((pParticle = (HPidParticleSim *)HPidParticleFiller::makeParticle(
                    pCand, pKickTrack, iIdx)) == NULL)
    {
        return NULL;
    }

    if(pKickTrack->InheritsFrom(HKickTrackSim::Class()) == kFALSE)
    {
        Error("makeParticle", "pKickTrack doesn't inherits from HKickTrackSim");
        return pParticle;
    }

HKickTrackSim *pSimTrack = (HKickTrackSim *)pKickTrack;
HPidTrackCand *pSimCand;
HRichHitSim   *pSimRich  = NULL;
HGeantKine    *pKine;
Int_t          iMeta, iMdc, iTrack, iId;

    // find the ring matched in HPidTrackCand (if exists)
    if(( ! bNoRich) && ((pSimCand = pCand->getCandidate()) != NULL))
        pSimRich = pSimCand->getRichHitSim();

    for(iMeta = 0; iMeta < pSimTrack->getNMetaTracks(); iMeta++)
    {
        if((iTrack = pSimTrack->getMetaTrack(iMeta)) <= 0)
            continue;

        for(iMdc = 0; iMdc < pSimTrack->getNMdcTracks(); iMdc++)
        {
            if(pSimTrack->getMdcTrack(iMeta) != iTrack)
                continue;

            if(pSimRich != NULL)
            {
                if(((pSimRich->weigTrack1 > 0) && (pSimRich->track1 == iTrack))
                || ((pSimRich->weigTrack2 > 0) && (pSimRich->track2 == iTrack))
                || ((pSimRich->weigTrack3 > 0) && (pSimRich->track3 == iTrack)))
                {
                    // here one track is ok
                }
                else
                {
                    // no track matches, so do not fill output
                    continue;
                }
            }

            if((pKine = HPidParticleSim::getGeantKine(iTrack)) == NULL)
            {
                Error("makeParticle", "Cannot get HGeantKine %d", iTrack);
                iId = -2;
            }
            else
                pKine->getParticle(iTrack, iId);

            pParticle->setGeantParams(iTrack, iId);

            return pParticle;
        }
    }

    return pParticle;
}

// -----------------------------------------------------------------------------

void HPidParticleFillerSim::print(void) const
{
// Print the list of all algorithms

    HPidParticleFiller::print();

    printf("No use RICH       : %s\n", (bNoRich) ? "ON" : "OFF");
}
