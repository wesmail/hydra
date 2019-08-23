// @(#)$Id: hpidparticlefillersim.cc,v 1.3 2002-12-03 14:49:19 jaskula Exp $
//*-- Author : Marcin Jaskula 13/11/2002
//  Modified : Marcin Jaskula 01/12/2002
//             support for HPidParticleSimExt class
//             function for searching Geant tracks uses HPidGeantTrackSet

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
#include "hpidparticlesimext.h"

#include "hades.h"
#include "hevent.h"

#include "hlinearcategory.h"
#include "piddef.h"

#include "hgeantkine.h"

#include "hpidgeanttrackset.h"

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
    setParameters(par);
}

// -----------------------------------------------------------------------------

HPidParticleFillerSim::HPidParticleFillerSim(Text_t name[], Text_t title[],
                                        Option_t par[])
            : HPidParticleFiller(name, title, par)
{
// Constructor with names
// par parameters is used in constructor of HPidParticleFiller class.

    setDefault();
    setParameters(par);
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
    bExt    = kFALSE;
}

// -----------------------------------------------------------------------------

Bool_t HPidParticleFillerSim::buildOutputCategory(void)
{
// Build all categories

    // output category
    if(bExt)
    {
        if((pOutCat = HPidParticleSimExt::buildPidParticleSimExtCategory())
                        == NULL)
        {
            Error("init", "Cannot build HPidParticleSimExt category");
            return kFALSE;
        }
    }
    else
    {
        if((pOutCat = HPidParticleSim::buildPidParticleSimCategory()) == NULL)
        {
            Error("init", "Cannot build HPidParticleSim category");
            return kFALSE;
        }
    }

    return kTRUE;
}

// -----------------------------------------------------------------------------

void HPidParticleFillerSim::setParameters(Option_t par[])
{
// Set parameters by names. See parameters in HPidParticleFiller::setParameters
// + options:
// NORICH - do not use Geant Tracks from RICH in the calculations
// EXT    - HPidParticleSimExt is used instead of HPidParticleSim

    HPidParticleFiller::setParameters(par);

TString s = par;

    s.ToUpper();

    bNoRich = (strstr(s.Data(), "NORICH") != NULL);
    bExt    = (strstr(s.Data(), "EXT") != NULL);
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

    if(bExt)
        pOut = new(pOut) HPidParticleSimExt();
    else
        pOut = new(pOut) HPidParticleSim();

    return pOut;
}

// -----------------------------------------------------------------------------

HPidParticle* HPidParticleFillerSim::makeParticle(const HPidCandidate *pCand,
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

HPidTrackCand *pSimCand;
HGeantKine    *pKine;
Int_t          iTrack, iId;
const HPidGeantTrackSet *pSet = NULL;

    // find the ring matched in HPidTrackCand (if exists)
    if(((pSimCand = pCand->getCandidate()) != NULL)
            && ((pSet = pSimCand->getGeantTrackSet( ! bNoRich)) != NULL))
    {
        pParticle->setGeantTrackSet(pSet);

        if((iTrack = pSet->getCommonTrack( ! bNoRich)) > 0)
        {
            if((pKine = HPidGeantTrackSet::getGeantKine(iTrack)) == NULL)
            {
                Error("makeParticle", "Cannot get HGeantKine %d", iTrack);
                iId = -2;
            }
            else
                pKine->getParticle(iTrack, iId);

            pParticle->setGeantParams(iTrack, iId);
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
    printf("Use SimExt        : %s\n", (bExt) ? "ON" : "OFF");
}
