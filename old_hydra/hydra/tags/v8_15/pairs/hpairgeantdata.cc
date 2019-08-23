// $Id: hpairgeantdata.cc,v 1.16 2007-11-20 10:37:47 kempter Exp $
// Author: Thomas.Eberl@ph.tum.de, last modified : 2006-06-28 13:46:01
// Last change by J. Otwinowski, 2005-10-26
// ---------------------------------------------------------------------------
//_HADES_CLASS_DESCRIPTION
///////////////////////////////////////////////////////////////////////////////
//                                                                           
// HPairGeantData
//                                                                   
// data container to store the geant info of the two particles building a pair
// plus geant information on the pair 
// inferred from the geant info of the singles
///////////////////////////////////////////////////////////////////////////////
//
using namespace std;
// ---------------------------------------------------------------------------
#include <iostream>
// ---------------------------------------------------------------------------
#include <hcategory.h>
#include <hiterator.h>
#include <hpidgeanttrackset.h>
#include <hpidparticlesim.h>
// ---------------------------------------------------------------------------
#include "hpairsim.h"
#include "hpairsimext.h"
#include "hpairfl.h"
#include "piddef.h"
// ---------------------------------------------------------------------------
#include "hpairgeantdata.h"
// ---------------------------------------------------------------------------
ClassImp(HPairGeantData)
// ---------------------------------------------------------------------------
const TString HPairGeantData::sSimVars=
"Gpid1:GparentId1:GprocessId1:Gmom1:Gpid2:GparentId2:GprocessId2:Gmom2"
":Ginvmass:Gopang:Grap:Gpt:Gcharge:GparentTrackNb1:GparentTrackNb2:GdecayId"
":GCommonDet1:GCommonDet2:Gvx1:Gvy1:Gvz1:Gvx2:Gvy2:Gvz2:Gmed1:Gmed2"
":Ggeninfo1:Ggenweight1:Ggeninfo2:Ggenweight2:GgrandparentTrackNb1"
":GgrandparentTrackNb2:GgrandparentId1:GgrandparentId2"
":Ggeninfo1_1:Ggeninfo1_2:Ggeninfo2_1:Ggeninfo2_2";
const Int_t HPairGeantData::SIMDATAITEMS = 38;
// ---------------------------------------------------------------------------
HPairGeantData::HPairGeantData() 
{
    
    reset();
}

// ---------------------------------------------------------------------------
HPairGeantData::HPairGeantData(HPairSim* p) 
{
    reset();

    HPairData::init((HPair*)p);
    if (init(p)) setGInit();
    else ::Error("ctor HPairSim","geant data not init");
}
// ---------------------------------------------------------------------------
HPairGeantData::HPairGeantData(HPairSimExt* p) 
{
    reset();
    HPairData::init(p);
    if (init(p)) setGInit();
    else ::Error("ctor HPairSimExt","geant data not init");
}
// ---------------------------------------------------------------------------
HPairGeantData::~HPairGeantData() 
{

}
// ---------------------------------------------------------------------------
void HPairGeantData::print()
{
    cout << "HPairGeantData _________________________________________" << endl;
    cout << "G pid1: " <<nGPID1<<" --- G pid2: " <<nGPID2<<endl;
    cout << "G mass: " <<fGMass<<" --- G op: " 
	 <<fGOpang<<" --- G rap: "<<fGRapidity<<endl;
}
// ---------------------------------------------------------------------------
Bool_t HPairGeantData::init(HPairSim* p)
{
    if (!p)
    {
	::Error("HPairGeantData::init(HPairSim*)",
		"pointer to pair is NULL");
	return kFALSE;
    }

    HCategory* pCatPart = HPairFL::getCategory(catPidPart);
    if (!pCatPart)
    {
	::Error("HPairGeantData::init(HPairSim*)",
		"no pid particle (sim) category found");
	return kFALSE;
    }
    
    // this works, as HPairData mother object
    // has already been initialized in ctor
    // think about "isInit" flagged to explicitely check that this was done!!!
    HPidParticleSim* p1 =(HPidParticleSim*)pCatPart
	->getObject(p->getIndexParticle1());
    HPidParticleSim* p2 =(HPidParticleSim*)pCatPart
	->getObject(p->getIndexParticle2());
    
    if (!p1 || !p2 ) 
    {
	::Error("init(HPairSim*)",
		"HPidParticleSim 1 or 2 not found in category");
	return kFALSE;
    }    

    return initData(p,p1,p2);
}
// ---------------------------------------------------------------------------
Bool_t HPairGeantData::init(HPairSimExt* p)
{
    
    if (!p)
    {
	::Error("HPairGeantData::init(HPairSim*)",
		"pointer to pair is NULL");
	return kFALSE;
    }

    HPidParticleSim* p1 = p->getPidParticle1();
    HPidParticleSim* p2 = p->getPidParticle2();

    if (!p1 || !p2 ) 
    {
	::Error("init(HPairSimExt*)",
		"HPidParticleSim 1 or 2 not found in category");
	return kFALSE;
    }    

    return initData(p,p1,p2);

}
// ---------------------------------------------------------------------------
Bool_t HPairGeantData::initData(HPairSim* p,
				HPidParticleSim* p1, HPidParticleSim* p2)
{
    
    if (!p)
    {
	::Error("initData()",
		"pointer to pair is NULL");
	return kFALSE;
    }

    if (!p1 || !p2 ) 
    {
	::Error("initData()",
		"HPidParticleSim 1 or 2 not found in category");
	return kFALSE;
    }    

    // geant particle id of reconstructed particle
    nGPID1       = p1->getGeantPID();
    nGPID2       = p2->getGeantPID();

    // get geant info corresponding to reconstructed particle
    const HPidGeantTrackSet* pGTrackSet1 =
	(HPidGeantTrackSet*) p1->getGeantTrackSet();
    const HPidGeantTrackSet* pGTrackSet2 
	= (HPidGeantTrackSet*) p2->getGeantTrackSet();

    if (!pGTrackSet1 || !pGTrackSet2) 
    {
	::Error("initData()","geant trackset 1 or 2 not found");
	return kFALSE; 
    }

    TLorentzVector *plvec1, *plvec2;
    plvec1 = plvec2 = NULL;

    // copy geant info from particle 1
    nGTrackNumber1       = pGTrackSet1->getGeantTrackID();
    nGParentPID1         = pGTrackSet1->getGeantParentID(); 
    nGProcessID1         = pGTrackSet1->getGeantProcessID(); 
    nGmed1               = pGTrackSet1->getGeantMediumID();
    nGParentTrackNumber1 = pGTrackSet1->getGeantParentTrackID();

    fGmom1 = pGTrackSet1->getTotalMomentum();
    sGCommonDetectors1 = pGTrackSet1->getMostCommonCorrelation();

    // production vertex of geant particle in [mm]
    fGvx1=pGTrackSet1->getGeantVertexX();
    fGvy1=pGTrackSet1->getGeantVertexY();
    fGvz1=pGTrackSet1->getGeantVertexZ();

    // weights for enhanced production rates in certain channels
    fGgenweight1 = pGTrackSet1->getGenWeight();

    // in case of urqmd
    fGgeninfo1 = pGTrackSet1->getGenInfo();

    // in case of pluto events propagate the geant parent track and parent ID
    fGgeninfo1_1 = pGTrackSet1->getGenInfo1();
    fGgeninfo2_1 = pGTrackSet1->getGenInfo2();

    // parent and grandparent info
    nGGrandParentTrackNumber1 = pGTrackSet1->getGeantGrandParentTrackID(); 
    nGGrandParentPID1         = pGTrackSet1->getGeantGrandParentID();
   

    // -----------------------------------

    // copy geant info from particle 2 
    nGTrackNumber2       = pGTrackSet2->getGeantTrackID();
    nGParentPID2         = pGTrackSet2->getGeantParentID(); 
    nGProcessID2         = pGTrackSet2->getGeantProcessID(); 
    nGmed2               = pGTrackSet2->getGeantMediumID();
    nGParentTrackNumber2 = pGTrackSet2->getGeantParentTrackID();

    fGmom2 = pGTrackSet2->getTotalMomentum();
    sGCommonDetectors2 = pGTrackSet2->getMostCommonCorrelation();

    // production vertex of geant particle in [mm]
    fGvx2=pGTrackSet2->getGeantVertexX();
    fGvy2=pGTrackSet2->getGeantVertexY();
    fGvz2=pGTrackSet2->getGeantVertexZ();

    // weights for enhanced production rates in certain channels
    fGgenweight2 = pGTrackSet2->getGenWeight();

    // in case of urqmd
    fGgeninfo1 = pGTrackSet1->getGenInfo();

    // in case of pluto events propagate the geant parent track and parent ID
    fGgeninfo1_2 = pGTrackSet2->getGenInfo1();
    fGgeninfo2_2 = pGTrackSet2->getGenInfo2();

    // parent and grandparent info
    nGGrandParentTrackNumber2 = pGTrackSet2->getGeantGrandParentTrackID(); 
    nGGrandParentPID2         = pGTrackSet2->getGeantGrandParentID();

    // -----------------------------------


    if(pGTrackSet1->getGeantTrackID(0) > 0 && pGTrackSet2->getGeantTrackID(0) > 0)
    {
	// For objects stemming from real data (embedding mode)
        // we have to skip this part

	plvec1 = (pGTrackSet1->buildGeantLorentzVector());
	if (plvec1) GLorentzVector1 = *plvec1;
	else
	{
	    ::Error("initData()","lorentz vector of geant particle 1 not built");
	    return kFALSE;
	}

	plvec2 = (pGTrackSet2->buildGeantLorentzVector());
	if (plvec2) GLorentzVector2 = *plvec2;
	else
	{
	    ::Error("initData()","lorentz vector of geant particle 2 not built");
	    return kFALSE;
	}

	// store geant info for pair
	TLorentzVector GeantPair = GLorentzVector1 + GLorentzVector2;

	fGMass               = GeantPair.M();
	fGOpang              = TMath::RadToDeg() * plvec1->Angle(plvec2->Vect());
	fGRapidity           = GeantPair.Rapidity();
	fGTransverseMomentum = GeantPair.Pt();

	cGCharge = HPidPhysicsConstants::charge(nGPID1) +
	    HPidPhysicsConstants::charge(nGPID2);

	// determine pair id from geant info
	nDecayId = calculateDecayID();
    }

    // local clean up

    // delete lorentz vectors
    if(plvec1)
    {
	delete plvec1;
	plvec1 = NULL;
    }
    if(plvec2)
    {
	delete plvec2;
	plvec2 = NULL;
    }
    
    return kTRUE;
}
// ---------------------------------------------------------------------------
void HPairGeantData::reset()
{
     kIsGInit = kFALSE;
     cGCharge=-1;
     fGMass=-1.;
     fGOpang=-1.;
     fGRapidity=-1.;
     fGTransverseMomentum=-1.;
     fGgeninfo1 = -1.;
     fGgeninfo1_1 = -1.;
     fGgeninfo1_2 = -1.;
     fGgeninfo2 = -1.;
     fGgeninfo2_1 = -1.;
     fGgeninfo2_2 = -1.;
     fGgenweight1 = -1.;
     fGgenweight2 = -1.;
     fGmom1=-1.;
     fGmom2=-1.;
     fGvx1 = -10000.;
     fGvx2 = -10000.;
     fGvy1 = -10000.;
     fGvy2 = -10000.;
     fGvz1 = -10000.;
     fGvz2 = -10000.;
     nDecayId=-1;
     nGGrandParentPID1=-1;
     nGGrandParentPID2=-1;
     nGGrandParentTrackNumber1=-1;
     nGGrandParentTrackNumber2=-1;
     nGPID1=-1;
     nGPID2=-1;
     nGParentPID1=-1;
     nGParentPID2=-1;
     nGParentTrackNumber1=-1;
     nGParentTrackNumber2=-1;
     nGProcessID1=-1;
     nGProcessID2=-1;
     nGTrackNumber1=-1;
     nGTrackNumber2=-1;
     nGmed1 = -1;
     nGmed2 = -1;
     sGCommonDetectors1 = -1;
     sGCommonDetectors2 = -1;

     
}
// ---------------------------------------------------------------------------
Float_t* HPairGeantData::getGArray() const
{
    Float_t* arr = new Float_t[SIMDATAITEMS];
    for (Int_t i=0;i<SIMDATAITEMS;i++) arr[i] = -100.;

    arr[0]=(Float_t)nGPID1;
    arr[1]=(Float_t)nGParentPID1;
    arr[2]=(Float_t)nGProcessID1;
    arr[3]=fGmom1;
    arr[4]=(Float_t)nGPID2;
    arr[5]=(Float_t)nGParentPID2;
    arr[6]=(Float_t)nGProcessID2;
    arr[7]=fGmom2;
    arr[8]=fGMass;
    arr[9]=fGOpang;
    arr[10]=fGRapidity;
    arr[11]=fGTransverseMomentum;
    arr[12]=(Float_t)cGCharge;
    arr[13]=(Float_t)nGParentTrackNumber1;
    arr[14]=(Float_t)nGParentTrackNumber2;
    arr[15]=(Float_t)nDecayId;
    arr[16]=(Float_t)sGCommonDetectors1;
    arr[17]=(Float_t)sGCommonDetectors2;
    arr[18]=fGvx1;
    arr[19]=fGvy1;
    arr[20]=fGvz1;
    arr[21]=fGvx2;
    arr[22]=fGvy2;
    arr[23]=fGvz2;
    arr[24]=nGmed1;
    arr[25]=nGmed2;
    arr[26]=fGgeninfo1;
    arr[27]=fGgenweight1;
    arr[28]=fGgeninfo2;
    arr[29]=fGgenweight2;
    arr[30]=(Float_t)nGGrandParentTrackNumber1;
    arr[31]=(Float_t)nGGrandParentTrackNumber2;
    arr[32]=(Float_t)nGGrandParentPID1;
    arr[33]=(Float_t)nGGrandParentPID2;
    arr[34]=fGgeninfo1_1;
    arr[35]=fGgeninfo1_2;
    arr[36]=fGgeninfo2_1;
    arr[37]=fGgeninfo2_2;
    return arr;
}
// ---------------------------------------------------------------------------
const TString& HPairGeantData::getSimVarNames()
{
    return sSimVars;
}
// ---------------------------------------------------------------------------
Int_t HPairGeantData::calculateDecayID()
{
  //cout << "calculating decayid for : " << sGCommonDetectors1 << " and: " << sGCommonDetectors2 << endl;


    // both tracks must have been completely reconstructed in GEANT
    if (sGCommonDetectors1<76 || sGCommonDetectors2<76) return -1;
    //cout << "corrflags OK" << endl;

    // the 2 particles must have had the same parent
    if (nGParentTrackNumber1!=nGParentTrackNumber2) return -1;
    //cout << "same parent" << endl;


    Int_t nDecayId=-1;

    // a e+e- pair in any case
    if ((nGPID1==2 || nGPID1==3) && (nGPID2==3 || nGPID2==2))
    {
      //cout << "Geant pid for leptons found!" << endl;

	// first check is redundant, second asks for GAMMA
	if (nGParentPID1 == nGParentPID2 && 
	    nGParentPID1==HPidPhysicsConstants::pid("g"))
	{
	  //cout << "Gamma" << endl;

	    // GEANT photo pair production: 6
	    if (nGProcessID1 == nGProcessID2 && nGProcessID1==6)
	      {
		//cout << "conversion!" << endl;
		//should we check in addition the medium of creation ???
		// conversion inside the RICH used to be (aMed>=8 && aMed<=19)
		nDecayId = CONVERSION;
	      }
	}

	// first check is redundant, second asks for PI0
	if (nGParentPID1 == nGParentPID2 && 
	    nGParentPID1==HPidPhysicsConstants::pid("pi0"))
	{
	  //cout << "pi0" << endl;

	    // GEANT direct decay mode: 5
	    if (nGProcessID1 == nGProcessID2 && nGProcessID1==5)
	      {
		//should we check in addition the medium of creation ???
		// conversion inside the RICH used to be (aMed>=8 && aMed<=19)
		nDecayId = PI0DALITZ;
		//cout << "dalitz" << endl;
	      }
	}

	// first check is redundant, second asks for ETA
	if (nGParentPID1 == nGParentPID2 && 
	    nGParentPID1==HPidPhysicsConstants::pid("eta"))
	{
	  //cout << "eta" << endl;
	    // GEANT direct decay mode: 5
	    if (nGProcessID1 == nGProcessID2 && nGProcessID1==5)
	      {
		//should we check in addition the medium of creation ???
		// conversion inside the RICH used to be (aMed>=8 && aMed<=19)
		nDecayId = ETADALITZ;
		//cout << "dalitz" << endl;
	      }
	}

    } // end e+e- pair

    return nDecayId;
}



