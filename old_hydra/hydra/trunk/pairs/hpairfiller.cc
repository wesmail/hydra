// $Id: hpairfiller.cc,v 1.16 2009-07-15 11:41:59 halo Exp $
// Author: Thomas.Eberl@ph.tum.de, last modified : 2006-11-13 18:18:19
//
// ---------------------------------------------------------------------------
//_HADES_CLASS_DESCRIPTION
///////////////////////////////////////////////////////////////////////////////
//                                                                           
// HPairFiller                                                                
//                                                                           
///////////////////////////////////////////////////////////////////////////////
//
using namespace std;
// ---------------------------------------------------------------------------
#include <iostream>
// ---------------------------------------------------------------------------
#include "TNtuple.h"
#include "TObjArray.h"
#include "TTree.h"
#include "TString.h"
// ---------------------------------------------------------------------------
#include <hades.h>
#include <hcategory.h>
#include <hevent.h>
#include <hiterator.h>
#include <hlinearcategory.h>
#include <hpidparticlesim.h>
#include <piddef.h>
#include <hpidcandidate.h>
#include <hpidtrackcand.h>
#include "hgeantdef.h"
// ---------------------------------------------------------------------------
#include "hpair.h"
#include "hpairfl.h"
#include "hpairqa.h"
#include "hpairsim.h"
#include "pairsdef.h"
// ---------------------------------------------------------------------------
#include "hpairfiller.h"
// ---------------------------------------------------------------------------
ClassImp(HPairFiller)
    // ---------------------------------------------------------------------------
    HPairFiller::HPairFiller(const Text_t *name,const Text_t *title) :
	HReconstructor(name,title)
{
    
    pQA=0;
    cQAFileName="";
    pCatPidParticle=0;
    pIterPidParticle=0;
    pCatPair=0;
    pIterPair=0;
    kIsReal=1;
}

// ---------------------------------------------------------------------------
HPairFiller::HPairFiller()
{
    pQA=0;
    cQAFileName="";
    pCatPidParticle=0;
    pIterPidParticle=0;
    pCatPair=0;
    pIterPair=0;
    kIsReal=1;

}

// ---------------------------------------------------------------------------
HPairFiller::~HPairFiller(void) 
{

}

// ---------------------------------------------------------------------------
Bool_t HPairFiller::init() 
{
    if (gHades) 
    {

	HEvent *event=gHades->getCurrentEvent();
	if (event) 
	{


	    HCategory* pCatGeantKine=event->getCategory(catGeantKine);
            if(pCatGeantKine) kIsReal = kFALSE;
            else              kIsReal = kTRUE;

	    // INPUT
	    pCatPidParticle=event->getCategory(catPidPart);
	    if (!pCatPidParticle) 
	    {
		Error("init","No PIDPARTICLE category defined");
		return kFALSE;
	    }
	    else pIterPidParticle = (HIterator*)getPidParticleCat()
		     ->MakeIterator("native");

	    // OUTPUT
	    pCatPair=event->getCategory(catPairs);
	    if (!pCatPair) 
	    {//build category

		TString pCatName="";
		if (kIsReal) pCatName="HPair";
		else pCatName="HPairSim";

		if ((pCatPair=HPairFL::buildLinearCategory(catPairs,pCatName.Data())))
		{
		    ((HLinearCategory*)pCatPair)->setDynamicObjects(kTRUE);
		    event->addCategory(catPairs,pCatPair,"Pairs");
		}
		else
		{
		    Error("init","No PAIRS category defined");
		    return kFALSE;
		}
	    }
	    else
	    {
		const Text_t* catName = pCatPair->getClassName();
		TString warning="Category ";
		warning+=catName;
		warning+=" already exists in input file ! SUBSTITUTION !";
		Warning("init",warning.Data());
	    }
	    pIterPair = (HIterator*)getPairCat()->MakeIterator("native");
	}
    }

    // QA
    if ( cQAFileName.Length()!=0 )
    {
	pQA = new HPairQA("Filler","Filler");
	pQA->setOutputFileName(getQAFileName());
	pQA->setNTupleFillingActive(1);
    }
    return kTRUE;
}

// ---------------------------------------------------------------------------
Bool_t HPairFiller::finalize() 
{
    if (pQA) 
    {
	pQA->finalize();
	delete pQA;
    }
    return kTRUE;
}


// ---------------------------------------------------------------------------
Int_t HPairFiller::execute()
{
    TObjArray* pAllPairsArray = createAllPossiblePairs();
    selectPairs(pAllPairsArray);
    fillPairCategory(pAllPairsArray);
    if (pQA) fillControlData(pAllPairsArray);
    deletePairArray(pAllPairsArray);

    return 0;
}

// ---------------------------------------------------------------------------
Bool_t HPairFiller::deletePairArray(TObjArray* pPairs)
{

#if 0
    for (Int_t i=0;i<pPairs->GetLast()+1;i++) 
    {
	if (kIsReal) delete (HPair*)(*pPairs)[i];
	else         delete (HPairSim*)(*pPairs)[i];
    }
    delete pPairs;
#endif

    pPairs->Delete();
    delete pPairs;
    pPairs=NULL;
    
    return kTRUE;
}

// ---------------------------------------------------------------------------
Bool_t HPairFiller::fillPairCategory(TObjArray* pPairs)
{
    if (pPairs)
    {
	for (Int_t iPairIdx=0; iPairIdx<pPairs->GetLast()+1; iPairIdx++)
	{

	    /////////////////////////////////////////////////////////////////////
	    // pair was marked as bad, do no fill it in category
	    if ( ((HPair*)(*pPairs)[iPairIdx]) -> getIsCutFlag() != 0 ) continue;
	    /////////////////////////////////////////////////////////////////////

	    if (kIsReal) 
	    {
		if (!fillPair((HPair*)(*pPairs)[iPairIdx])) return kFALSE;
	    }
	    else
	    {
		if (!fillPair((HPairSim*)(*pPairs)[iPairIdx])) return kFALSE;
	    }
	}
    }
    else return kFALSE;
    return kTRUE;
}

// ---------------------------------------------------------------------------
Bool_t HPairFiller::fillPair(HPair* pPair)
{
    HLocation dummyLoc;
    dummyLoc.set(1,0);
    if (pPair)
    {
	HPair* pPairPlacement = (HPair*)((HLinearCategory*)
					 getPairCat())->getNewSlot(dummyLoc);
	if (pPairPlacement) 
	{
	    HPair* pPairCp = new (pPairPlacement) HPair();
	    *pPairCp = *pPair;
	}
	else 
	{
	    Error("fillPair","no slot in category available"); 
	    return kFALSE;
	}
    }
    else
    {
	Error("fillPair","pair address (argument) is zero");
	return kFALSE;
    }
    return kTRUE;
}

// ---------------------------------------------------------------------------
Bool_t HPairFiller::fillPair(HPairSim* pPairSim)
{
    HLocation dummyLoc;
    dummyLoc.set(1,0);
    if (pPairSim)
    {
	HPairSim* pPairPlacement = (HPairSim*)((HLinearCategory*)
					       getPairCat())->getNewSlot(dummyLoc);

	if (pPairPlacement) 
	{
	    HPairSim* pPairCp = new (pPairPlacement) HPairSim();
	    *pPairCp = *pPairSim;

	}
	else 
	{
	    Error("fillPair","no slot in category available"); 
	    return kFALSE;
	}
    }
    else
    {
	Error("fillPair","pair address (argument) is zero");
	return kFALSE;
    }
    return kTRUE;
}

// ---------------------------------------------------------------------------
TObjArray* HPairFiller::createAllPossiblePairs(void)
{
    HPair *pPair = NULL;
    HPairSim *pPairSim = NULL;
    
    // create HPairs systematically from all HPidParticles
    // store them in a TObjArray and return it
    //
    // allows to select them or process them further 
    // before writing them into the category
    
    TObjArray* t = new TObjArray(5);
    
    HCategory* pCatPart = getPidParticleCat();
    if (!pCatPart) 
    {
	Error("createAllPossiblePairs","Particle category not found");
	return 0;
    }

    Int_t nEntries = pCatPart->getEntries();

    for (Int_t iParticle1=0; iParticle1<nEntries-1; iParticle1++)
    {
	for (Int_t iParticle2=iParticle1+1; iParticle2<nEntries; iParticle2++)
	{
	    if (t)
	    {
		if (kIsReal) 
		{
		    pPair = new HPair(
				      (HPidParticle*)pCatPart
				      ->getObject(iParticle1),
				      (HPidParticle*)pCatPart
				      ->getObject(iParticle2)
				      );
		    t->Add(pPair);
		}
		else 
		{
		    pPairSim = new HPairSim(
					    (HPidParticleSim*)pCatPart
					    ->getObject(iParticle1),
					    (HPidParticleSim*)pCatPart
					    ->getObject(iParticle2)
					    );
		    t->Add(pPairSim);
		}
	    }
	    else 
	    {
		Error("createAllPossiblePairs","TObjArray not created");
		return 0;
	    }
	    
	}
    }
    return t;
}

// ---------------------------------------------------------------------------
void HPairFiller::selectPairs(TObjArray* pPairs)
{
#warning THIS FCN MAKES ASSUMPTIONS ABOUT LEPTONS BEYOND THE PID ALGOS !!!
#warning IT REQUIRES LEPTONS TO HAVE AN ASSOCIATED RING !!!
    // this is a fix for pid std hard cut particle output 
    HCategory* pCatPart = getPidParticleCat();
    HPair*             p = NULL;
    HPidParticle* pPart1 = NULL;
    HPidParticle* pPart2 = NULL;
    Int_t nMomInd = -1;
    Int_t nRing1  = -1;
    Int_t nRing2  = -1; 
    Int_t nPid1   =  0;
    Int_t nPid2   =  0;
    for (Int_t i=0;i<pPairs->GetLast()+1;i++) 
    {
	p =  (HPair*)(*pPairs)[i];
        pPart1 = 
	    (HPidParticle*)pCatPart->getObject(p->getIndexParticle1());
        pPart2 = 
	    (HPidParticle*)pCatPart->getObject(p->getIndexParticle2());
        nPid1 = pPart1->getPid();
        nPid2 = pPart2->getPid();

        //Check ring, leptons have rings!
        nMomInd = pPart1->getMomAlg();
	const HPidHitData* pHitData1 = pPart1->getHitData();
	const HPidHitData* pHitData2 = pPart2->getHitData();
        nRing1 = pHitData1->getRingCorrelation(nMomInd);
        nRing2 = pHitData2->getRingCorrelation(nMomInd);
	
        //Do not make pairs with lepton without ring.
        if(((nPid1==2||nPid1==3)&&!nRing1) || ((nPid2==2||nPid2==3)&&!nRing2)){
            p -> setIsCutFlag(1);
        }

	// also allow lepton + non-lepton pairs for cutting later (sample "B")!!
        //if at least one particle is lepton, pair is OK
        if((nPid1!=2 && nPid1!=3) &&  (nPid2!=2 && nPid2!=3)){
            p -> setIsCutFlag(1);
        }
    }
}
// ---------------------------------------------------------------------------
HPairSim* HPairFiller::createPair(HPidParticleSim* pParticleSim1, HPidParticleSim* pParticleSim2)
{
    HLocation dummyLoc;
    dummyLoc.set(1,0);
    HPairSim* pPairSim = (HPairSim*)((HLinearCategory*)
				     getPairCat())->getNewSlot(dummyLoc);
    if (pPairSim) pPairSim = new(pPairSim) HPairSim(pParticleSim1,pParticleSim2);
    return pPairSim;
}

// ---------------------------------------------------------------------------
HPair* HPairFiller::createPair(HPidParticle* pParticle1, HPidParticle* pParticle2)
{
    HLocation dummyLoc;
    dummyLoc.set(1,0);
    HPair* pPair = (HPair*)((HLinearCategory*)
			    getPairCat())->getNewSlot(dummyLoc);
    if (pPair) pPair = new(pPair) HPair(pParticle1,pParticle2);
    return pPair;
}

// ---------------------------------------------------------------------------
Bool_t HPairFiller::fillControlData(TObjArray* t)
{
    pQA->fillData(t);
    return kTRUE;
}
// ---------------------------------------------------------------------------
void HPairFiller::diag()
{
    pIterPair->Reset();
    if (kIsReal)
    {
	HPair* pPair = 0;
	while( (pPair = (HPair*) pIterPair->Next()) ) 
	{
	    pPair->print();
	}
    }
    else
    {
	HPairSim* pPair = 0;
	while( (pPair = (HPairSim*) pIterPair->Next()) ) 
	{
	    pPair->print();
	}
    }

#if 0
    pIterPidParticle->Reset();
    if (kIsReal)
    {
	HPidParticle* pParticle = 0;
	while( (pParticle = (HPidParticle*) pIterPidParticle->Next()) ) 
	{
	    pParticle->print();
	}
    }
    else
    {
	HPidParticleSim* pParticleSim = 0;
	while( (pParticleSim = (HPidParticleSim*) pIterPidParticle->Next()) ) 
	{
	    pParticleSim->print();
	    //  	    cout<<(Int_t)pParticleSim->getPid(0)<<" "<<pParticleSim->getWeight(0)<<endl;
 	    cout<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
	}
    }
#endif

}
