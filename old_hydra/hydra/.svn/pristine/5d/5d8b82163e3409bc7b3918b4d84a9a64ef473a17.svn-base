//*-- Author : B. Spruck
//*-- Modified : 19 Sep 2006
//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////
//
// HHypCopyKineChi2ToProb
//
// Copies TMath:Prob(kine_chi24,1) to Probability of Combination
// If no kine_chi24 is existing (or invalid), the combination is removed!
//
//  B. Spruck
////////////////////////////////////////////////////////////////////////

using namespace std;

#include <stdlib.h>
#include <iostream>
#include "hhypCopyKineChi2ToProb.h"
#include "hypinfodef.h"

ClassImp(HHypCopyKineChi2ToProb)

HHypCopyKineChi2ToProb::HHypCopyKineChi2ToProb(Char_t *name_i, Option_t par[])
:HHypBaseAlgorithm(name_i,par)
{
}

HHypCopyKineChi2ToProb::~HHypCopyKineChi2ToProb()
{
}

Bool_t HHypCopyKineChi2ToProb::execute()
{
	if (!beam) {
		cerr << algoName << " needs beam particle! " << endl;
		return kFALSE;
	}
	
	// Resetting the list and start looping over the combinations
	// Loop is only done over the VALID combinations

	mylist->CombIteratorReset();
	while (mylist->CombIterator()) {
		Float_t kine_chi2, kine_chi24;
			
		if (mylist->getUserValue(KINEFIT_CHI2, kine_chi2) &&
					mylist->getUserValue(KINEFIT_CHI24, kine_chi24)){

			if( kine_chi2>0.0 && kine_chi24>0.0){
				mylist->resetProbAlg(TMath::Prob(kine_chi24,1));// Or TMath::Prob(kine_chi24,1)... lets take the faster one
			}else{
				mylist->removeComb();
			}
		}else{
			mylist->removeComb();
		}
	}                             //END Iterator

	if (exitIdx > -1)
		return kTRUE;
	return kFALSE;
}

Bool_t HHypCopyKineChi2ToProb::init()
{
	return kTRUE;
}

Bool_t HHypCopyKineChi2ToProb::reinit()
{
	return kTRUE;
}

Bool_t HHypCopyKineChi2ToProb::finalize()
{
	return kTRUE;
}
