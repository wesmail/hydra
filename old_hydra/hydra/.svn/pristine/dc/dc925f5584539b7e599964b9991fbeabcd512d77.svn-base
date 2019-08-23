// @(#)$Id: hpiddilepton.cc,v 1.1 2002-12-03 14:50:09 jaskula Exp $
//*-- Author : Marcin Jaskula 01/12/2002

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidDilepton                                                               //
//                                                                            //
// Dilepton parameters                                                        //
//                                                                            //
// This is a modified HDilepton class from libPhyAna                          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "hpiddilepton.h"
#include "hpidparticle.h"
#include "hpidtrackcand.h"

#include "piddef.h"

#include "hades.h"
#include "hevent.h"
#include "hlinearcategory.h"

#include <TError.h>

// -----------------------------------------------------------------------------

ClassImp(HPidDilepton)

// -----------------------------------------------------------------------------

HPidDilepton::HPidDilepton(Short_t nId1, Short_t nId2)
{
// Construct HPidDileption from two HPidParticles found by their ids
// in catPidPart category. Both particles must be accessible, or the
// HPidDileption will not be construced in a propper way.
// If nId1 <= 0 a not initialized instance of HPidDilepton is created
// (default constructor)

    setDefault();

    if((nId1 >= 0) && (nId2 >= 0))
        setParticles(nId1, nId2);
}

// -----------------------------------------------------------------------------

HPidDilepton::HPidDilepton(const HPidParticle *pPart1,
                           const HPidParticle *pPart2)
{
// Construct HPidDileption from two HPidParticles.
// Both particles must exist in catPidPart category.

    setDefault();
    setParticles(pPart1, pPart2);
}

// -----------------------------------------------------------------------------

void HPidDilepton::setDefault(void)
{
// Set default parameters of the class

    SetPxPyPzE(0., 0., 0., 0.);

    nParticleId1 = -1;
    nParticleId2 = -1;

    cCharge = 0;
    fAngle  = 0.0f;
}

// -----------------------------------------------------------------------------

Bool_t HPidDilepton::setParticles(Short_t nId1, Short_t nId2)
{
    setDefault();

    if((nId1 <= 0) || (nId2 <= 0))
    {
        Error("setParticles", "Wrong ids %d %d", nId1, nId2);
        return kFALSE;
    }

const HPidParticle *p1 = HPidParticle::getPidParticle(nId1);
const HPidParticle *p2 = HPidParticle::getPidParticle(nId2);

    if((p1 == NULL) || (p2 == NULL))
    {
        Error("setParticles", "Cannot find particles %d (%p) %d (%p)",
                        nId1, p1, nId2, p2);

        return kFALSE;
    }

    nParticleId1 = nId1;
    nParticleId2 = nId2;

    calculate(p1, p2);

    return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HPidDilepton::setParticles(const HPidParticle *pPart1,
                                 const HPidParticle *pPart2)
{
    setDefault();

    if((pPart1 == NULL) || (pPart2 == NULL))
    {
        Error("setParticles", "Wrong pointers %p %p", pPart1, pPart2);
        return kFALSE;
    }

Short_t nId1 = ((HPidParticle *)pPart1)->getIndex();
Short_t nId2 = ((HPidParticle *)pPart2)->getIndex();

    if((nId1 < 0) || (nId2 < 0))
    {
        Error("setParticles", "Cannot find particle ids %d (%p) %d (%p)",
                        nId1, pPart1, nId2, pPart2);

        return kFALSE;
    }

    nParticleId1 = nId1;
    nParticleId2 = nId2;

    calculate(pPart1, pPart2);

    return kTRUE;
}

// -----------------------------------------------------------------------------

const HPidParticle* HPidDilepton::getParticle1(void) const
{
// Return HPidParticle corresponding to the nParticleId1

    return (nParticleId1 < 0) ? NULL
                : HPidParticle::getPidParticle(nParticleId1);
}

// -----------------------------------------------------------------------------

const HPidParticle* HPidDilepton::getParticle2(void) const
{
// Return HPidParticle corresponding to the nParticleId2

    return (nParticleId2 < 0) ? NULL
                : HPidParticle::getPidParticle(nParticleId2);
}

// -----------------------------------------------------------------------------

HCategory* HPidDilepton::buildPidDileptonCategory(void)
{
// Static function for making the category HPidDilepton

HCategory    *pCat;
HEvent       *pEvent;

    if((gHades == NULL) || ((pEvent = gHades->getCurrentEvent()) == NULL))
    {
        ::Error("HPidDilepton::buildPidParticleCategory",
                    "Cannot access current event");

        return NULL;
    }

    if((pCat = pEvent->getCategory(catPidDileption)) != NULL)
        return pCat;

    if((pCat = new HLinearCategory("HPidDilepton", 1000)) == NULL)
    {
        ::Error("HPidDilepton::buildPidDileptonCategory",
                    "Cannot create new category");

        return NULL;
    }

    pEvent->addCategory(catPidDileption, pCat, "Pid");

    return pCat;
}

// -----------------------------------------------------------------------------

HPidDilepton* HPidDilepton::getPidDilepton(Int_t iIdx, HCategory *pCat)
{
// Return HPidDilepton object corresponding to iIdx from pCat.
// If the pCat is not set (NULL) catPidDileption category from gHades is used.

    if(iIdx < 0)
        return NULL;

    if((pCat == NULL) && ((pCat = HPidTrackCand::getCategory(
                            catPidDileption, kFALSE)) == NULL))
    {
        ::Error("HPidDilepton::getPidDilepton", "No catPidDileption category");
        return NULL;
    }

    return (HPidDilepton *) pCat->getObject(iIdx);
}

// -----------------------------------------------------------------------------

void HPidDilepton::print(void) const
{
// Print info about the particle

    printf("HPidDilepton : ");
    if( ! isOk())
    {
        printf("not ok !!!\n");
        return;
    }

    printf("Invariant mass: %8.4f\nCharge    : %2d\nOpening angle : %8.4f\n"
            "Theta         : %8.4f    Phi : %8.4f\n",
            M(), cCharge, fAngle, thetaDeg(), phiDeg());
}

// -----------------------------------------------------------------------------

void HPidDilepton::calculate(const HPidParticle *pPart1,
                             const HPidParticle *pPart2)
{
// Internal function for calculations of the TLorenzVector, the charge and
// the opening angle

TLorentzVector v = *pPart1 + *pPart2;

    *(TLorentzVector *)this = v;
    cCharge = pPart1->charge() + pPart2->charge();
    fAngle = TMath::RadToDeg() * pPart1->Angle(pPart2->Vect());
}
