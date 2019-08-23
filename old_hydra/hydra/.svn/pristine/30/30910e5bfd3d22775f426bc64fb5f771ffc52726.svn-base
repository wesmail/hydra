// $Id: hpairfilter.cc,v 1.10 2009-07-15 11:41:59 halo Exp $
// Author: Thomas.Eberl@ph.tum.de, last modified : 2006-11-24 11:29:15
//
// ---------------------------------------------------------------------------
//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////////////
//                                                                           
// HPairFilter                                                                
//                                                                           
///////////////////////////////////////////////////////////////////////////////
//
using namespace std;
// ---------------------------------------------------------------------------
#include <iostream>
// ---------------------------------------------------------------------------
#include <stdlib.h>
// ---------------------------------------------------------------------------
#include <hades.h>
#include <hcategory.h>
#include <hevent.h>
#include <hiterator.h>
#include <hruntimedb.h>
#include "hgeantdef.h"

// ---------------------------------------------------------------------------
#include "hpair.h"
#include "hpaircutpar.h"
#include "hpairsim.h"
#include "hpairqa.h"
// ---------------------------------------------------------------------------
#include "hpairfilter.h"
// ---------------------------------------------------------------------------
ClassImp(HPairFilter)
// ---------------------------------------------------------------------------
HPairFilter::HPairFilter(const Text_t *name,const Text_t *title) :
  HReconstructor(name,title)
{
    pQA=0;
    cQAFileName="";
}

// ---------------------------------------------------------------------------
HPairFilter::HPairFilter()
{
    pQA=0;
    cQAFileName="";
}
// ---------------------------------------------------------------------------
HPairFilter::~HPairFilter(void) 
{

}
// ---------------------------------------------------------------------------
Bool_t HPairFilter::init() 
{
        if (gHades) 
	{
	    HEvent *event=gHades->getCurrentEvent();
	    HRuntimeDb *rtdb=gHades->getRuntimeDb();
	    if (event && rtdb) 
	    {
		HCategory* pCatGeantKine=event->getCategory(catGeantKine);
		if(pCatGeantKine) kIsReal = kFALSE;
		else              kIsReal = kTRUE;

		pCuts = (HPairCutPar*)rtdb->getContainer("PairCutParameters");
		if (!pCuts) Error("HPairFilter::init",
				  "PairCutParameters initialization via RTDB returned a NULL pointer");
		
		// INPUT
		pCatPair=event->getCategory(catPairs);
		if (!pCatPair) 
		{
		    Error("init","No PAIRS category defined");
		    return kFALSE;
		}
		else pIterPair = (HIterator*)getPairCat()
			 ->MakeIterator("native");

		// OUTPUT
	    }
	}

	// QA
	if ( cQAFileName.Length()!=0 )
	{
	    pQA = new HPairQA("Filter","Filter");
	    pQA->setOutputFileName(getQAFileName());
	    pQA->setNTupleFillingActive(1);
	    pQA->setCutQAFillingActive(1);
	    //	    pQA->setHistogramFillingActive(1);
	}

	return kTRUE;
}
// ---------------------------------------------------------------------------
Bool_t HPairFilter::reinit() 
{
    if (pCuts) pCuts->printParam();
    return kTRUE;
}
// ---------------------------------------------------------------------------
Bool_t HPairFilter::finalize() 
{
    if (pQA) 
    {
	pQA->finalize();
	delete pQA;
    }
    return kTRUE;
}


// ---------------------------------------------------------------------------
Int_t HPairFilter::execute()
{
    pIterPair->Reset();

    if (kIsReal)
    {
	HPair* pPair = 0;
	while( (pPair = (HPair*) pIterPair->Next()) ) 
	{
	    //if (pCuts->check(pPair)) pPair->print();
	    pCuts->check(pPair);
	}
    }
    else
    {
	HPairSim* pPairSim = 0;
	while( (pPairSim = (HPairSim*) pIterPair->Next()) ) 
	{
	    pCuts->check(pPairSim);
	    //	    if (pCuts->check(pPairSim)) pPairSim->print();
	}
    }

    // ---------------------
    if (pCuts->getBitFieldRecursiveFlags()) pCuts->checkRecursive(pIterPair);
    if (pQA) pQA->fillData(pIterPair);
    return 0;
}

