// $Id: hpairfilter.cc,v 1.5 2005-05-11 18:52:32 eberl Exp $
// Author: Thomas.Eberl@ph.tum.de, last modified : 2005-05-11 20:31:46
//
// ---------------------------------------------------------------------------
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
HPairFilter::HPairFilter(Text_t *name,Text_t *title) :
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
	    kIsReal=gHades->isReal();
	    HEvent *event=gHades->getCurrentEvent();
	    HRuntimeDb *rtdb=gHades->getRuntimeDb();
	    if (event && rtdb) 
	    {
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
	    cout<<"make HPairQA in filter"<<endl;
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
    pCuts->printParam();
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

