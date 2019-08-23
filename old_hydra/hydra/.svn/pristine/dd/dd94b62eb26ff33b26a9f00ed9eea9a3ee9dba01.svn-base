// @(#)$Id: hpidparticle.cc,v 1.2 2002-11-04 23:21:07 jaskula Exp $
//*-- Author : Marcin Jaskula 30/11/2002

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidParticle                                                               //
//                                                                            //
// Identified particle                                                        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "hpidparticle.h"
#include "hpidphysicsconstants.h"

#include "hades.h"
#include "hevent.h"
#include "hlinearcategory.h"

#include <TError.h>

#include "piddef.h"

// -----------------------------------------------------------------------------

ClassImp(HPidParticle)

// -----------------------------------------------------------------------------

HPidParticle::HPidParticle(Double_t dM0, Double_t dPx,
                            Double_t dPy, Double_t dPz)
{
// Constuctor with kinematic parameters of the particle
// dM0 - mass (MeV/c^2), dPx, dPy, dPz (MeV/c),

    setDefault();
    SetXYZM(dPx, dPy, dPz, dM0);
}

// -----------------------------------------------------------------------------

HPidParticle::HPidParticle(Short_t nPid, Double_t dPx,
                            Double_t dPy, Double_t dPz)
{
// Constuctor with kinematic parameters of the particle and its id
// Particle id, Px, Py, Pz (MeV/c)

    setDefault();

Double_t dM;

    if((dM = HPidPhysicsConstants::mass(nPid)) < 0.0)
        dM = 0.0;

    SetXYZM(dPx, dPy, dPz, dM);
    setPid(nPid);
}

// -----------------------------------------------------------------------------

HPidParticle::HPidParticle(const Char_t *sPid, Double_t dPx,
                            Double_t dPy, Double_t dPz)
{
// Constuctor with kinematic parameters of the particle and its name
// Particle id, Px, Py, Pz (MeV/c)

    setDefault();

Short_t  nPid = HPidPhysicsConstants::pid(sPid);
Double_t dM;

    if((nPid == 0) || ((dM = HPidPhysicsConstants::mass(nPid)) < 0.0))
        dM = 0.0;

    SetXYZM(dPx, dPy, dPz, dM);
    setPid(nPid);
}

// -----------------------------------------------------------------------------

void HPidParticle::setDefault(void)
{
    nPidCandidate = -1;

    fBetaExp = 0.0f;
    fR       = 0.0f;
    fZ       = 0.0f;
    nSystem  = 0;

    nPid     = 0;
    fPdf     = 0.0f;
    fBayes   = 0.0f;
    fCL      = 0.0f;
}

// -----------------------------------------------------------------------------

void HPidParticle::Clear(Option_t *)
{
// Clear all variables

    setDefault();
    SetXYZT(0.0, 0.0, 0.0, 0.0);
}

// -----------------------------------------------------------------------------

void HPidParticle::print(void) const
{
// Print info about the particle

    printf("Particle : %d \"%s\"\n", nPid, HPidPhysicsConstants::pid(nPid));
    printf("4Momentum: (%8.4f, %8.4f, %8.4f, %8.4f)\nMom. mag.: %8.4f MeV/c\n",
                X(), Y(), Z(), T(), P());
    printf("Theta/Phi: %8.4f  %8.4f\n", thetaDeg(), phiDeg());
    printf("Sect./Sys: %d %d\n", sector(), nSystem);
    printf("R / Z    : %8.4f  %8.4f\n", fR, fZ);
    printf("Beta calc: %8.4f  Exp: %8.4f\n", Beta(), getBetaExp());
    printf("PDF      : %8.4f  Intens: %8.4f  CL: %8.4f\n", fPdf, fBayes, fCL);
}

// -----------------------------------------------------------------------------

HPidCandidate* HPidParticle::getPidCandidate(HCategory *pCat) const
{
// Returns HPidCandidate object corresponding to nPidCandidate (if exists)
// Works when pCat is set or gHades->getCurrentEvent() is accessable

    if(nPidCandidate < 0)
        return NULL;

    if(pCat == NULL)
    {
    HEvent *pEvent;

        if((gHades == NULL) || ((pEvent = gHades->getCurrentEvent()) == NULL))
        {
            Error("getPidCandidate", "Cannot access current event");
            return NULL;
        }

        if((pCat = pEvent->getCategory(catPidCandidate)) == NULL)
            Error("getPidCandidate", "No catPidTrackCand category");
    }

    return (HPidCandidate *) pCat->getObject(nPidCandidate);
}

// -----------------------------------------------------------------------------


HCategory* HPidParticle::buildPidParticleCategory(void)
{
// Static function for making the category HPidParticle

HCategory    *pCat;
HEvent       *pEvent;

    if((gHades == NULL) || ((pEvent = gHades->getCurrentEvent()) == NULL))
    {
        ::Error("HPidParticle::buildPidParticleCategory",
                    "Cannot access current event");

        return NULL;
    }

    if((pCat = pEvent->getCategory(catPidPart)) != NULL)
        return pCat;

    if((pCat = new HLinearCategory("HPidParticle", 1000)) == NULL)
    {
        ::Error("HPidParticle::buildPidParticleCategory",
                    "Cannot create new category");

        return NULL;
    }

    pEvent->addCategory(catPidPart, pCat, "Pid");

    return pCat;
}
