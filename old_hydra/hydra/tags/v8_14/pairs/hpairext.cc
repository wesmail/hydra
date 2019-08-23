// $Id: hpairext.cc,v 1.4 2006-08-16 15:42:18 halo Exp $
// Author: Thomas.Eberl@ph.tum.de, last modified : 2006-06-30 16:44:44
//
//
// ---------------------------------------------------------------------------
//_HADES_CLASS_DESCRIPTION
///////////////////////////////////////////////////////////////////////////////
//                                                                           
// HPairExt                                                                    
// The only possible interface is HPidParticle, make sure no calls to deeper 
// levels are needed ! 
//                                                                           
///////////////////////////////////////////////////////////////////////////////
//
using namespace std;
// ---------------------------------------------------------------------------
#include <iostream>
// ---------------------------------------------------------------------------
#include <hades.h>
// ---------------------------------------------------------------------------
#include "hpairext.h"
// ---------------------------------------------------------------------------
ClassImp(HPairExt)
// ---------------------------------------------------------------------------
HPairExt::HPairExt() 
{
    reset();
}

// ---------------------------------------------------------------------------
HPairExt::HPairExt(HPidParticle *p1, HPidParticle *p2  ):
    itsParticle1(*(p1)),itsParticle2(*(p2))
{
    //reset();
    // copy the particles and store them in the pair
    //itsParticle1  = * (p1);
    //itsParticle2  = * (p2);

    calcPairProperties(&itsParticle1,&itsParticle2);
}

// ---------------------------------------------------------------------------
HPairExt::~HPairExt() 
{

}

// ---------------------------------------------------------------------------
HPairExt::HPairExt(const HPairExt& source) 
{

}

// ---------------------------------------------------------------------------
void HPairExt::print()
{
    HPair::print();
}


// ---------------------------------------------------------------------------
void HPairExt::reset(void) 
{

}

// ---------------------------------------------------------------------------
#if 0
Int_t HPairExt::getMostProbablePID(HPidParticle& p,HPidTrackData& t)
{
  Float_t currentMax=0.0;  
  Int_t pid=-99;
  for(Int_t j =0;j<p.getNPossibleSpecies();j++)
    {
      if(p.getWeight(j)>currentMax)
	{
	  currentMax=p.getWeight(j);
	  pid=p.getPid(j);
	}
    }
    if( !(currentMax>0.0)){ //no decision. Make artificial IDs
        if( t.nPolarity[p.getMomAlg()]<0){
            pid =HPidPhysicsConstants::artificialNeg();
        }
        else if (t.nPolarity[p.getMomAlg()]>0){
            pid =HPidPhysicsConstants::artificialPos();
        }
    }

  return pid;
}
#endif
// ---------------------------------------------------------------------------
#if 0
Bool_t HPairExt::calcPairPropertiesExt() 
{

    // TEMPORARY FIX to repair the mass in the lorentz vector 
    TLorentzVector pair;
    if (gHades->isReal())
    {
	TVector3 p13 = itsParticle1.Vect();
	TVector3 p23 = itsParticle2.Vect();
	
	TLorentzVector p1t; 
	TLorentzVector p2t;
	p1t.SetVectM(p13,HPidPhysicsConstants::mass(getMostProbablePID(itsParticle1,itsTrackData1)));
	p2t.SetVectM(p23,HPidPhysicsConstants::mass(getMostProbablePID(itsParticle2,itsTrackData2)));
	
	pair = p1t + p2t;
	
	*(TLorentzVector *)this = pair;
    }	
    else *(TLorentzVector *)this = itsParticle1 + itsParticle2;
    
    setOpeningAngleDeg(TMath::RadToDeg() * itsParticle1.Angle(itsParticle2.Vect()));
//     setIndexParticle1((Short_t) p1->getIndex());
//     setIndexParticle2((Short_t) p2->getIndex());
    setPid1(getMostProbablePID(itsParticle1,itsTrackData1));
    setPid2(getMostProbablePID(itsParticle2,itsTrackData2));
    //setPidProbability1(p1->getWeightForPID(p1->getMostProbablePID()));
    //setPidProbability2(p2->getWeightForPID(p2->getMostProbablePID()));
    setInvariantMass(this->M());


    /*
    if (p1 && p2)
    {
	reset();
    }
    else 
    {
	Error("calcPairPropertiesExt","pointer to particle1 and/or particle2 is null");
	return kFALSE;
    }
    */

    return kTRUE;
}
#endif
// ---------------------------------------------------------------------------
Char_t HPairExt::getCharge(void) const
{

    return (itsParticle1.getCharge() + itsParticle2.getCharge());
}
// ---------------------------------------------------------------------------
void HPairExt::Clear(Option_t *opt)
{
    itsParticle1.Clear();
    itsParticle2.Clear();
    
}


