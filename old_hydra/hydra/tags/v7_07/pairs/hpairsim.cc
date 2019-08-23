// $Id: hpairsim.cc,v 1.4 2005-06-23 17:20:04 jacek Exp $
// Author: Thomas.Eberl@ph.tum.de, last modified : 2005-05-05 12:22:35
//
// ---------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
//                                                                           
// HPairSim                                                                     
//                                                                           
///////////////////////////////////////////////////////////////////////////////
//
using namespace std;
// ---------------------------------------------------------------------------
#include <iostream>
// ---------------------------------------------------------------------------
#include <hades.h>
#include <hlinearcategory.h>
#include <hpidgeanttrackset.h>
#include <hpidparticle.h>
#include <hpidparticlesim.h>
// ---------------------------------------------------------------------------
#include "pairsdef.h"
// ---------------------------------------------------------------------------
#include "hpairsim.h"
// ---------------------------------------------------------------------------
ClassImp(HPairSim)
// ---------------------------------------------------------------------------
HPairSim::HPairSim() 
{
    reset();
}

// ---------------------------------------------------------------------------
HPairSim::HPairSim(HPidParticleSim *p1, HPidParticleSim *p2  ) 
{
    reset();
    calcPairProperties((HPidParticle*) p1,(HPidParticle*) p2);
    calcPairPropertiesSim((HPidParticleSim*) p1,(HPidParticleSim*) p2);
    
}

// ---------------------------------------------------------------------------
HPairSim::~HPairSim() 
{

}

// ---------------------------------------------------------------------------
HPairSim::HPairSim(const HPairSim& source) 
{

}

// ---------------------------------------------------------------------------
void HPairSim::print()
{
    HPair::print();
    printf("--- SIM\n");
    TLorentzVector GP = GLorentzVector1+GLorentzVector2;


    printf("GPair: 4Momentum : (px=%5.1f, py=%5.1f, pz=%5.1f, E=%5.1f)\nMom. mag.: p=%5.1f MeV/c\n",
	   GP.X(), GP.Y(), GP.Z(), GP.T(), GP.P());

    printf("GPair:  inv. mass (MeV/c2) = %5.1f, pt = %5.1f, rapidity = %5.1f\n", 
	   GP.M(), GP.Pt(), GP.Rapidity());

    printf("GPair:  inv. mass (MeV/c2) = %5.1f, pt = %5.1f, rapidity = %5.1f\n", 
	   fGInvMass, GP.Pt(), GP.Rapidity());

    printf("GPair:  opening angle (deg) = %5.1f, charge = %2i\npid1 = %2i, pid2 = %2i\n",
	   fGOpang, cGCharge, nGPid1, nGPid2);

    printf("--- \n");
    printf("GVec1: 4Momentum : (px=%5.1f, py=%5.1f, pz=%5.1f, E=%5.1f)\nMom. mag.: p=%5.1f MeV/c\n",
	   GLorentzVector1.X(), GLorentzVector1.Y(), GLorentzVector1.Z(), GLorentzVector1.T(), GLorentzVector1.P());
    printf("GVec1:  inv. mass (MeV/c2) = %5.1f, pt = %5.1f, rapidity = %5.1f\n", 
	   GLorentzVector1.M(), GLorentzVector1.Pt(), GLorentzVector1.Rapidity());

    printf("--- \n");
    printf("GVec2: 4Momentum : (px=%5.1f, py=%5.1f, pz=%5.1f, E=%5.1f)\nMom. mag.: p=%5.1f MeV/c\n",
	   GLorentzVector2.X(), GLorentzVector2.Y(), GLorentzVector2.Z(), GLorentzVector2.T(), GLorentzVector2.P());
    printf("GVec2:  inv. mass (MeV/c2) = %5.1f, pt = %5.1f, rapidity = %5.1f\n", 
	   GLorentzVector2.M(), GLorentzVector2.Pt(), GLorentzVector2.Rapidity());

    //    printf("--- \n");
}


// ---------------------------------------------------------------------------
void HPairSim::reset(void) 
{

    nGPid1  = -1;
    nGPid2  = -1;
    fGOpang = -1.;
    cIsTrue = -1;
    cGCharge = -10;
}

// ---------------------------------------------------------------------------
Bool_t HPairSim::calcPairPropertiesSim(HPidParticleSim *p1, HPidParticleSim *p2  ) 
{
    if (p1 && p2)
    {

	nGPid1 = p1->getGeantPID();
	nGPid2 = p2->getGeantPID();

	HPidGeantTrackSet* pGTrackSet1 = (HPidGeantTrackSet*) p1->getGeantTrackSet();
	HPidGeantTrackSet* pGTrackSet2 = (HPidGeantTrackSet*) p2->getGeantTrackSet();

	if (pGTrackSet1 && pGTrackSet2)
	{

	    // commoness

// 	    if (pGTrackSet1->getMostCommonCorrelation() >= 76 ||
// 		pGTrackSet2->getMostCommonCorrelation() >= 76   )
// 	    cout << pGTrackSet1->getMostCommonCorrelation() << "  "<<
// 		pGTrackSet2->getMostCommonCorrelation()<<endl;


	    // GEANT momentum

// 	    TVector3* pvec31 = (pGTrackSet1->buildGeantMomentumVector());
// 	    TLorentzVector plvec1;
// 	    plvec1.SetVectM(*pvec31,HPidPhysicsConstants::mass(nGPid1));
// 	    GLorentzVector1 = plvec1;

	    TLorentzVector* plvec1 = (pGTrackSet1->buildGeantLorentzVector());
	    if (plvec1) 
		{
		   GLorentzVector1 = *plvec1;
		}
 	    else return kFALSE;

	    //if (nGPid1==2 || nGPid2==3) printf("GPair: 4Momentum : (px=%5.1f, 
	    //py=%5.1f, pz=%5.1f, E=%5.1f)\nMom. mag.: p=%5.1f MeV/c\n",plvec1->X(),
            //plvec1->Y(),plvec1->Z(),plvec1->E(),plvec1->P());

// 	    TVector3* pvec32 = (pGTrackSet2->buildGeantMomentumVector());
// 	    TLorentzVector plvec2;
// 	    plvec2.SetVectM(*pvec32,HPidPhysicsConstants::mass(nGPid2));
// 	    GLorentzVector2 = plvec2;

	    TLorentzVector* plvec2 = (pGTrackSet2->buildGeantLorentzVector());
 	    if (plvec2) 
		{
			GLorentzVector2 = *plvec2;
		}
 	    else return kFALSE;

 	    fGOpang = TMath::RadToDeg() * plvec1->Angle(plvec2->Vect());
	    fGInvMass = (*plvec1 + *plvec2).M();

        // delete TLorentzVectors
		if (plvec1)
		{
		  delete plvec1;
	 	  plvec1 = NULL;
		}

		if (plvec2)
		{
		  delete plvec2;
		  plvec2 = NULL;
		}
	}
    }
    else 
    {
	Error("calcPairPropertiesSim","pointer to particle1 and/or particle2 is null");
	return kFALSE;
    }
    return kTRUE;
}


