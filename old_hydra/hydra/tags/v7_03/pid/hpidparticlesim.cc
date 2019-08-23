// @(#)$Id: hpidparticlesim.cc,v 1.6 2005-01-11 11:06:46 halo Exp $
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

  HPidParticleSim::HPidParticleSim(HPidCandidate* pCandidate, Float_t* assignedWeights,
				   Bool_t useMassIdeal,  Int_t userDefSpecies, Float_t userWeight)
    : HPidParticle(pCandidate, assignedWeights, useMassIdeal, userDefSpecies, userWeight)
{
  //setDefault();
}

void HPidParticleSim::Clear(Option_t *)
{
// Clear all variables

    HPidParticle::Clear();
}

// -----------------------------------------------------------------------------

void HPidParticleSim::print(void) const
{
// Print info about the particle

    HPidParticle::print();
    getGeantTrackSet()->print();
}


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
      {
	((HLinearCategory*)pCat)->setDynamicObjects(kTRUE);
        return pCat;
      }

    if((pCat = new HLinearCategory("HPidParticleSim", 1000)) == NULL)
    {
        ::Error("HPidParticleSim::buildPidParticleSimCategory",
                    "Cannot create new category");

        return NULL;
    }
    ((HLinearCategory*)pCat)->setDynamicObjects(kTRUE);
    pEvent->addCategory(catPidPart, pCat, "Pid");

    return pCat;
}

// -----------------------------------------------------------------------------
HPidTrackCandSim* HPidParticleSim::getTrackCandSim(void) const
{
  return (HPidTrackCandSim*) HPidParticle::getTrackCand();
}

// -----------------------------------------------------------------------------



