// @(#)$Id: hpidparticle.cc,v 1.8 2003-10-23 18:55:42 jaskula Exp $
//*-- Author : Marcin Jaskula 30/11/2002
//  Modified : Marcin Jaskula 11/11/2002
//             fExpMass added
//  Modified : Marcin Jaskula 01/12/2002
//             new methods getIndex(), getPidParticle()
//             new interface in getPidCandidate()
//  Modified : Marcin Jaskula 25/02/2003
//             new methods getTrackCand() and getKickTrack()
//  Modified : Marcin Jaskula 24/06/2003
//             fillMultiUsed method added

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidParticle                                                               //
//                                                                            //
// Identified particle                                                        //
//                                                                            //
// fMassExp member defines the mass calulated from experimental parameters    //
// momentum and beta. The value of this memeber may have two artifical values://
// -1 (kMassExpInTLV) - means that fMassExp was used to build TLorentzVector  //
// -2 (kMassExpNoPhy) - the experimental beta was not physical one.           //
//                                                                            //
// Proper values of both masses: experimental and ideal can be got by methods://
// getMassExp(), getMassIdeal() which returns propper mass independently      //
// on the mass used to make TLorentzVector                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "hpidparticlesim.h"
#include "hpidhitdata.h"

#include "hpidcandidate.h"
#include "hpidtrackcand.h"

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
    fMassExp = Float_t(kMassExpNoPhy);
    fR       = 0.0f;
    fZ       = 0.0f;
    fPull    = 0.0f;
    nSystem  = 0;

    nPid     = 0;
    fTestVal = 0.0f;

    fWeight  = 1.0f;

    fMomRescal = 1.0f;
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

    printf("Particle  : %d \"%s\"\n", nPid, HPidPhysicsConstants::pid(nPid));
    printf("4Momentum : (%8.4f, %8.4f, %8.4f, %8.4f)\nMom. mag.: %8.4f MeV/c\n",
                X(), Y(), Z(), T(), P());
    printf("Theta/Phi : %8.4f  %8.4f\n", thetaDeg(), phiDeg());
    printf("Sect./Sys : %d %d\n", sector(), nSystem);
    printf("R, Z, Pull: %8.4f  %8.4f  %8.4f\n", fR, fZ, fPull);
    printf("Beta calc : %8.4f  Exp: %8.4f\n", Beta(), getBetaExp());
    printf("Mass ideal: %8.4f  Exp: %8.4f\n", getMassIdeal(), getMassExp());
    printf("TestVal   : %8.4f\n", fTestVal);
    printf("Weight    : %8.4f\n", fWeight);
}

// -----------------------------------------------------------------------------

HPidCandidate* HPidParticle::getPidCandidate(HCategory *pCat) const
{
// Returns HPidCandidate object corresponding to nPidCandidate (if exists)
// Works when pCat is set or gHades->getCurrentEvent() is accessible

    if(nPidCandidate < 0)
        return NULL;

    return HPidCandidate::getPidCandidate(nPidCandidate, pCat);
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

// -----------------------------------------------------------------------------

Float_t HPidParticle::getMassExp(void) const
{
// Return experimental value of the mass of the particle.
// Negative value means that the beta was not proper one.

    return (getTLVUseExpMass()) ? (Float_t)M() : fMassExp;
}

// -----------------------------------------------------------------------------

Float_t HPidParticle::getMassIdeal(void) const
{
// Return value of the ideal mass of the particle.

    return (getTLVUseExpMass())
                ? HPidPhysicsConstants::mass(nPid) : (Float_t)M();
}

// -----------------------------------------------------------------------------

HPidParticle* HPidParticle::getPidParticle(Int_t iIdx, HCategory *pCat)
{
// Return HPidParticle object corresponding to iIdx from pCat.
// If the pCat is not set (NULL) catPidPart category from gHades is used.

    if(iIdx < 0)
        return NULL;

    if((pCat == NULL) && ((pCat = HPidTrackCand::getCategory(
                            catPidPart, kFALSE)) == NULL))
    {
        ::Error("HPidParticle::getPidParticle", "No catPidPart category");

        return NULL;
    }

    return (HPidParticle *) pCat->getObject(iIdx);
}

// -----------------------------------------------------------------------------

Int_t HPidParticle::getIndex(HCategory *pCat)
{
// Return index of the HPidParticle in the pCat or catPidPart if pCat == NULL

    if((pCat == NULL) && ((pCat = HPidTrackCand::getCategory(
                            catPidPart, kFALSE)) == NULL))
    {
        Error("getIndex", "Cannot get category");
        return -2;
    }

    return pCat->getIndex(this);
}

// -----------------------------------------------------------------------------

HPidTrackCand* HPidParticle::getTrackCand(void) const
{
// This method returns HPidTrackCand object from the corresponding
// HPidCandidate (when nPidCandidate >= 0) or directly from catPidTrackCand
// when nPidCandidate <= -1000
// (interface for HPidParticleFillerFromTrack)

    if(nPidCandidate >= 0)
    {
    HPidCandidate *pCand;

        if((pCand = getPidCandidate()) == NULL)
        {
            Error("getTrackCand", "Cannot get HPidCandidate");
            return NULL;
        }

        if(pCand->getCandidateId() < 0)
            return NULL;

        return pCand->getCandidate();
    }

    if(nPidCandidate <= kTrackCandOffset)
        return HPidTrackCand::getTrackCand(kTrackCandOffset - nPidCandidate);

    return NULL;
}

// -----------------------------------------------------------------------------

HKickTrack* HPidParticle::getKickTrack(void) const
{
// This method returns HKickTrack object from the corresponding
// HPidCandidate -> HPidTrackCand (where nPidCandidate >= 0)
// or directly from HPidTrackCand when nPidCandidate <= -1000
// (interface for HPidParticleFillerFromTrack)

HPidTrackCand *pCand;

    if((pCand = getTrackCand()) != NULL)
        return pCand->getKickTrack();

    return NULL;
}

// -----------------------------------------------------------------------------

TLorentzVector HPidParticle::getRescaledVector(void) const
{
TLorentzVector v = *this;

    if(fMomRescal != 1.0f)
    {
    TVector3 v3 = v.Vect();

        v3.SetMag(fMomRescal * v3.Mag());
        v.SetVectM(v3, M());
    }

    return v;
}

// -----------------------------------------------------------------------------

void HPidParticle::fillMultiUsed(const HPidTrackCand *pCand)
{
// Fill the *MultiUsed flags from HPidTrackCand

    if(pCand == NULL)
    {
        Error("fillMultiUsed", "pCand == NULL");
        return;
    }

    setRichMultiUsed(pCand->getRingCount() > 1);
    setMdcMultiUsed(pCand->getInnerMdcCount(0) > 1);
    setKickTrackMultiUsed(pCand->getKickTrackCount() > 1);
    setMetaMultiUsed(pCand->getMetaCount() > 1);
}

// -----------------------------------------------------------------------------

void HPidParticle::Streamer(TBuffer &R__b)
{
// Stream an object of class HPidParticle.

UInt_t R__s, R__c;

    if(R__b.IsReading())
    {
    Version_t R__v = R__b.ReadVersion(&R__s, &R__c);

        TLorentzVector::Streamer(R__b);
        R__b >> nPidCandidate;
        R__b >> fBetaExp;

        if(R__v > 1)
            R__b >> fMassExp;
        else
            fMassExp = Float_t(kMassExpNoPhy);

        R__b >> fR;
        R__b >> fZ;
        R__b >> nSystem;
        R__b >> nPid;

        if(R__v < 4)
        {
        Float_t f;

            R__b >> f;
            R__b >> f;
            R__b >> f;

            fMomRescal = 1.0f;
            fPull = 0.0f;
        }
        else
        {
            R__b >> fMomRescal;
            R__b >> fPull;
        }

        if(R__v > 2)
            R__b >> fWeight;
        else
            fWeight = 1.0f;

        R__b.CheckByteCount(R__s, R__c, HPidParticle::IsA());
    }
    else
    {
        R__c = R__b.WriteVersion(HPidParticle::IsA(), kTRUE);
        TLorentzVector::Streamer(R__b);
        R__b << nPidCandidate;
        R__b << fBetaExp;
        R__b << fMassExp;
        R__b << fR;
        R__b << fZ;
        R__b << nSystem;
        R__b << nPid;
        R__b << fMomRescal;
        R__b << fPull;
        R__b << fWeight;
        R__b.SetByteCount(R__c, kTRUE);
    }
}
