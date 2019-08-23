// $Id: hpair.cc,v 1.6 2006-11-10 13:20:59 eberl Exp $
// Author: Thomas.Eberl@ph.tum.de, last modified : 2006-11-10 14:19:48
//
// ---------------------------------------------------------------------------
//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////////////
//                                                                           
// HPair                                                                       
//                                                                           
///////////////////////////////////////////////////////////////////////////////
//
using namespace std;
// ---------------------------------------------------------------------------
#include <hades.h>
#include <hcategory.h>
#include <hpidparticle.h>
// ---------------------------------------------------------------------------
#include "hpairfl.h"
#include "pairsdef.h"
// ---------------------------------------------------------------------------
#include "hpair.h"
// ---------------------------------------------------------------------------
ClassImp(HPair)
// ---------------------------------------------------------------------------
HPair::HPair() 
{
    reset();
}

// ---------------------------------------------------------------------------
HPair::HPair(HPidParticle *p1, HPidParticle *p2  ) 
{
    reset();
    calcPairProperties(p1,p2);
    //if (!calcPairProperties(p1,p2)) Warning("ctor","pair could not be calculated.");
}

// ---------------------------------------------------------------------------
HPair::~HPair() 
{

}

// ---------------------------------------------------------------------------
HPair::HPair(const HPair& source) 
{

}

// ---------------------------------------------------------------------------
void HPair::print()
{
    printf("--- EXP -----------------------------------------------------\n");
    printf("Pair: 4Momentum : (px=%5.1f, py=%5.1f, pz=%5.1f, E=%5.1f)\nMom. mag.: p=%5.1f MeV/c\n",
	   X(), Y(), Z(), T(), P());
    printf("Pair:  inv. mass (MeV/c2) = %5.1f, pt = %5.1f, rapidity = %5.1f\n", fInvMass, Pt(), Rapidity());
    printf("Pair:  opening angle (deg) = %5.1f, charge = %2i\nindex1 = %2i, index2 = %2i\npid1 = %2i, prob1 = %3.2f, pid2 = %2i, prob2 = %3.2f\n",
	   fOpang, (Int_t)getCharge(), sIdxP1, sIdxP2, nPid1, fProbability1, nPid2, fProbability2);
}

// ---------------------------------------------------------------------------
Bool_t HPair::calcPairProperties(HPidParticle *p1, HPidParticle *p2  ) 
{
    if (p1 && p2)
    {

	// What to do with the particles which have pid = -99 ??
	if (p1->getMostProbablePID() < 0 || p2->getMostProbablePID() < 0) 
	{
	    *(TLorentzVector *)this = *p1 + *p2;
	    setOpeningAngleDeg(TMath::RadToDeg() * p1->Angle(p2->Vect()));
	    setIndexParticle1((Short_t) p1->getIndex());
	    setIndexParticle2((Short_t) p2->getIndex());
	    setPid1(p1->getMostProbablePID());
	    setPid2(p2->getMostProbablePID());
	    setPidProbability1(p1->getWeightForPID(p1->getMostProbablePID()));
	    setPidProbability2(p2->getWeightForPID(p2->getMostProbablePID()));
	    setInvariantMass(this->M());
	    return kFALSE;
	}


	//p1->print();
	//p2->print();


	// TEMPORARY FIX to repair the mass in the lorentz vector 
	TLorentzVector pair;
	if (gHades->isReal())
	{
	    TVector3 p13 = p1->Vect();
	    TVector3 p23 = p2->Vect();
	    
	    TLorentzVector p1t; 
	    TLorentzVector p2t;
	    p1t.SetVectM(p13,HPidPhysicsConstants::mass(p1->getMostProbablePID()));
	    p2t.SetVectM(p23,HPidPhysicsConstants::mass(p2->getMostProbablePID()));

	    pair = p1t + p2t;

	    *(TLorentzVector *)this = pair;
	}	
	else *(TLorentzVector *)this = *p1 + *p2;

	setOpeningAngleDeg(TMath::RadToDeg() * p1->Angle(p2->Vect()));
	setIndexParticle1((Short_t) p1->getIndex());
	setIndexParticle2((Short_t) p2->getIndex());
	setPid1(p1->getMostProbablePID());
	setPid2(p2->getMostProbablePID());
	setPidProbability1(p1->getWeightForPID(p1->getMostProbablePID()));
	setPidProbability2(p2->getWeightForPID(p2->getMostProbablePID()));
	setInvariantMass(this->M());
    }
    else 
    {
	Error("calcPairProperties","pointer to particle1 and/or particle2 is null");
	return kFALSE;
    }
    return kTRUE;
}
// ---------------------------------------------------------------------------
void HPair::reset(void) 
{
    setIndexParticle1(-1);
    setIndexParticle2(-1);
    setPid1(-100);
    setPid2(-100);
    setPidProbability1(-1.);
    setPidProbability2(-1.);
    setOpeningAngleDeg(-1.);
    setIsCutFlag(0);
    SetPxPyPzE(0., 0., 0., 0.);
    setBitFieldPassedCuts(0);
    setInvariantMass(-1.);
}
// ---------------------------------------------------------------------------
Char_t HPair::getCharge(void) const
{
    // function assumes that HPidParticle category is present in current event

    HCategory* pCatPidPart = HPairFL::getCategory(catPidPart);
    if (!pCatPidPart) 
    {
	Warning("getCharge","no HPidParticle category found !");
	return -10;
    }

    HPidParticle* p1 = (HPidParticle*) pCatPidPart->getObject(getIndexParticle1());
    HPidParticle* p2 = (HPidParticle*) pCatPidPart->getObject(getIndexParticle2());
    
    if (!p1 || !p2) 
    {
	Warning("getCharge","HPidParticle not found for given index");
	return -10;
    }

    return (p1->getCharge() + p2->getCharge());
}
