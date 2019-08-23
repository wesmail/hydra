// $Id: hpairgeantdata.cc,v 1.10 2005-07-25 07:26:43 eberl Exp $
// Author: Thomas.Eberl@ph.tum.de, last modified : 2005-05-26 02:23:12
//
// ---------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
//                                                                           
// HPairGeantData                                                                   
//                                                                           
///////////////////////////////////////////////////////////////////////////////
//
using namespace std;
// ---------------------------------------------------------------------------
#include <iostream>
// ---------------------------------------------------------------------------
#include <hcategory.h>
#include <hiterator.h>
#include <hgeantkine.h>
#include <hpidgeanttrackset.h>
#include <hpidparticlesim.h>
// ---------------------------------------------------------------------------
#include "hpairsim.h"
#include "hpairfl.h"
#include "piddef.h"
// ---------------------------------------------------------------------------
#include "hpairgeantdata.h"
// ---------------------------------------------------------------------------
ClassImp(HPairGeantData)
// ---------------------------------------------------------------------------
const TString HPairGeantData::sSimVars="Gpid1:GparentId1:GprocessId1:Gmom1:Gpid2:GparentId2:GprocessId2:Gmom2:Ginvmass:Gopang:Grap:Gpt:Gcharge:GparentTrackNb1:GparentTrackNb2:GdecayId:GCommonDet1:GCommonDet2:Gvx1:Gvy1:Gvz1:Gvx2:Gvy2:Gvz2:Gmed1:Gmed2:Ggeninfo1:Ggenweight1:Ggeninfo2:Ggenweight2:GgrandparentTrackNb1:GgrandparentTrackNb2:GgrandparentId1:GgrandparentId2";
const Int_t HPairGeantData::SIMDATAITEMS = 34;
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
    init(p);
}

// ---------------------------------------------------------------------------
HPairGeantData::~HPairGeantData() 
{

}

// ---------------------------------------------------------------------------
void HPairGeantData::print()
{
}
// ---------------------------------------------------------------------------
Bool_t HPairGeantData::init(HPairSim* p)
{
    HCategory* pCatPart = HPairFL::getCategory(catPidPart);
    HCategory* pCatKine = HPairFL::getCategory(catGeantKine);
    
    HIterator *iter_kine = (HIterator*)(pCatKine->MakeIterator("native"));
	  

    HPidParticleSim* p1    = (HPidParticleSim*)pCatPart->getObject((Int_t) sIdxP1);
    HPidParticleSim* p2    = (HPidParticleSim*)pCatPart->getObject((Int_t) sIdxP2);
    
    if (!p1 || !p2 ) return kFALSE;

    nGPID1       = p1->getGeantPID();
    nGPID2       = p2->getGeantPID();

    HPidGeantTrackSet* pGTrackSet1 = (HPidGeantTrackSet*) p1->getGeantTrackSet();
    HPidGeantTrackSet* pGTrackSet2 = (HPidGeantTrackSet*) p2->getGeantTrackSet();

    if (!pGTrackSet1 || !pGTrackSet2) return kFALSE; 

    nGParentPID1 = pGTrackSet1->getGeantParentID(); 
    nGProcessID1 = pGTrackSet1->getGeantProcessID(); 
    nGmed1 = pGTrackSet1->getGeantMediumID();
    nGParentTrackNumber1 = pGTrackSet1->getGeantParentTrackID();

    TLorentzVector* plvec1 = (pGTrackSet1->buildGeantLorentzVector());
    if (plvec1) GLorentzVector1 = *plvec1;
    else return kFALSE;

    nGParentPID2 = pGTrackSet2->getGeantParentID(); 
    nGProcessID2 = pGTrackSet2->getGeantProcessID(); 
    nGmed2 = pGTrackSet2->getGeantMediumID();
    nGParentTrackNumber2 = pGTrackSet2->getGeantParentTrackID();

    TLorentzVector* plvec2 = (pGTrackSet2->buildGeantLorentzVector());
    if (plvec2) GLorentzVector2 = *plvec2;
    else return kFALSE;

    TLorentzVector GeantPair = GLorentzVector1 + GLorentzVector2;

    fGmom1 = pGTrackSet1->getTotalMomentum();
    fGmom2 = pGTrackSet2->getTotalMomentum();

    fGMass      = GeantPair.M();
    fGOpang     = TMath::RadToDeg() * plvec1->Angle(plvec2->Vect());
    fGRapidity  = GeantPair.Rapidity();
    fGTransverseMomentum = GeantPair.Pt();

    cGCharge = HPidPhysicsConstants::charge(nGPID1) + HPidPhysicsConstants::charge(nGPID2);

    sGCommonDetectors1 = pGTrackSet1->getMostCommonCorrelation();
    sGCommonDetectors2 = pGTrackSet2->getMostCommonCorrelation();
    //cout << "got correlation1: " <<sGCommonDetectors1<< endl;
    //cout << "got correlation2: " <<sGCommonDetectors2<< endl;
    HGeantKine* pKine1 = pGTrackSet1->getGeantKine(pGTrackSet1->getGeantTrackID());
    HGeantKine* pKine2 = pGTrackSet2->getGeantKine(pGTrackSet2->getGeantTrackID());

    pKine1->getVertex(fGvx1,fGvy1,fGvz1);
    pKine2->getVertex(fGvx2,fGvy2,fGvz2);
    pKine1->getGenerator(fGgeninfo1,fGgenweight1);
    pKine2->getGenerator(fGgeninfo2,fGgenweight2);

    nDecayId = calculateDecayID();

    // find grand parent of particles and store track number and pid
    // needed for simulations with enhanced branching ratios 

	HGeantKine* pKineParent1=NULL;
    HGeantKine* pKineGrandParent1 = NULL;

    if (pKine1 && pCatKine)
    {
	//pKineParent1 = HPairFL::findParent(pKine1,pCatKine);
	pKineParent1 = HPairFL::findParent(pKine1,iter_kine);
	if (pKineParent1)
	{
	    //pKineGrandParent1 = HPairFL::findParent(pKineParent1,pCatKine);
	    pKineGrandParent1 = HPairFL::findParent(pKineParent1,iter_kine);
	
	    if (pKineGrandParent1)
	    {
		pKineGrandParent1->getParticle(nGGrandParentTrackNumber1,
					       nGGrandParentPID1);
// 		Int_t nPar, nMed;
// 		pKineGrandParent1->getCreator(nPar,nMed,nGGrandProcessID1);
	    }
	    else 
	    {
		//if the lepton comes from a dalitz decay, it does not
		//have a grand parent, do not show error message
		//::Error("init","no kine grand parent 1 found");
	    }
	}
	else 
	{
	    // if the reconstructed particle was a fake, the found
	    // kine object might have no parent as it stems directly from
	    // the generator, do not show error message in this case

// 	    ::Error("init","no kine parent 1 found");
// 	    HPairFL::dumpKine(pKine1);

	}
    }
    else ::Error("init","no geantkine category or object found");

	HGeantKine* pKineParent2 = NULL;
    HGeantKine* pKineGrandParent2 =  NULL;

    if (pKine2 && pCatKine)
    {
    //pKineParent2 = HPairFL::findParent(pKine2,pCatKine);
    pKineParent2 = HPairFL::findParent(pKine2,iter_kine);
	if (pKineParent2)
	{
	    //pKineGrandParent2 = HPairFL::findParent(pKineParent2,pCatKine);
	    pKineGrandParent2 = HPairFL::findParent(pKineParent2,iter_kine);
	
	    if (pKineGrandParent2)
	    {
		pKineGrandParent2->getParticle(nGGrandParentTrackNumber2,
					       nGGrandParentPID2);
// 		Int_t nPar, nMed;
// 		pKineGrandParent2->getCreator(nPar,nMed,nGGrandProcessID2);
	    }
	    else 
	    {
		//if the lepton comes from a dalitz decay, it does not
		//have a grand parent, do not show error message
		//::Error("init","no kine grand parent 2 found");
	    }
	}
	else 
	{
	    // if the reconstructed particle was a fake, the found
	    // kine object might have no parent as it stems directly from
	    // the generator, do not show error message in this case

// 	    ::Error("init","no kine parent 2 found");
// 	    HPairFL::dumpKine(pKine2);

	}
    }
    else ::Error("init","no geantkine category or object found");



    // delete TLorenzVectors
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
    
    // delete iterator
	if (iter_kine)
	{
	  delete iter_kine;
	  iter_kine = NULL;
	}

    return kFALSE;
}
// ---------------------------------------------------------------------------
void HPairGeantData::reset()
{
     nGPID1=-1;
     nGParentPID1=-1;
     nGProcessID1=-1;
     nGGrandParentPID1=-1;
//      nGGrandProcessID1=-1;

     nGPID2=-1;
     nGParentPID2=-1;
     nGProcessID2=-1;
     nGGrandParentPID2=-1;
//      nGGrandProcessID2=-1;

     nGTrackNumber1=-1;
     nGTrackNumber2=-1;
     nGParentTrackNumber1=-1;
     nGParentTrackNumber2=-1;
     nGGrandParentTrackNumber1=-1;
     nGGrandParentTrackNumber2=-1;

     fGmom1=-1.;
     fGmom2=-1.;
    
     fGMass=-1.;
     fGOpang=-1.;
     fGRapidity=-1.;
     fGTransverseMomentum=-1.;
     cGCharge=-1;

     nDecayId=-1;

     sGCommonDetectors1 = -1;
     sGCommonDetectors2 = -1;
     
     fGvx1 = -10000.;
     fGvy1 = -10000.;
     fGvz1 = -10000.;
     fGvx2 = -10000.;
     fGvy2 = -10000.;
     fGvz2 = -10000.;

     nGmed1 = -1;
     nGmed2 = -1;
     
     fGgeninfo1 = -1.;
     fGgeninfo2 = -1.;

     fGgenweight1 = -1.;
     fGgenweight2 = -1.;
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
//     arr[34]=(Float_t)nGGrandProcessID1;
//     arr[35]=(Float_t)nGGrandProcessID2;
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



