// @(#)$Id: hpidparticlesim.cc,v 1.9 2006-09-06 14:36:16 christ Exp $
//*-- Author : Marcin Jaskula 11/11/2002
//  Modified : Marcin Jaskula 01/12/2002
//             new method getGeantTrackSet()
//             getGeantKine() rewritten
//  Modified : Marcin Jaskula 26/02/2003
//             nGeantId & nGeantTrack -> cCommonTrack structure

//_HADES_CLASS_DESCRIPTION
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidParticleSim                                                            //
//                                                                            //
// Identified particle for sim data                                           //
// All members and methods from HPidParticle are still valid but functions to //
// handle simulation information are added                                    //
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
    : HPidParticle(pCandidate, assignedWeights, useMassIdeal, userDefSpecies, userWeight),
      itsGeantTrackSet(*(( (HPidTrackCandSim*)pCandidate->getTrackCandidate() )->getGeantTrackSet()))
{
  //itsGeantTrackSet = new HPidGeantTrackSet(*(source->getGeantTrackSet()));
  //cout << "Calling copy constructor for HPidGeantTrackSet" << endl;
  //cout << "Address of GeantTrackSet: " << itsGeantTrackSet << endl;
  //itsGeantTrackSet->print();

  //setDefault();
}

HPidParticleSim::HPidParticleSim(const HPidParticleSim& source):
  HPidParticle(source),
  itsGeantTrackSet(*(source.getGeantTrackSet()))
{
  
}


void HPidParticleSim::Clear(Option_t *opt)
{
// Clear all variables

  HPidParticle::Clear(opt);
  itsGeantTrackSet.Clear();

}

// -----------------------------------------------------------------------------

void HPidParticleSim::print(void) const
{
// Print info about the particle

    HPidParticle::print();
    getGeantTrackSet()->print();
}


HCategory* HPidParticleSim::buildPidParticleSimCategory(void) //OK
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
HPidTrackCandSim* HPidParticleSim::getTrackCandSim(void) const //OK
{
  //cast the base class to the derived one
  return (HPidTrackCandSim*) HPidParticle::getTrackCand();
}

// -----------------------------------------------------------------------------



