//*-- Author : B. Spruck
//*-- Modified : 19 Sep 2006
//_HADES_CLASS_DESCRIPTION
////////////////////////////////////////////////////////////////////////
//
// HHypCopyKineChi2ToProb
//
// Copies TMath:Prob(kine_chi2,1) to Probability of Combination
// If no kine_chi2 is existing (or invalid), the combination is removed!
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

	// Resetting the list and start looping over the combinations
	// Loop is only done over the VALID combinations

	mylist->CombIteratorReset();
	while (mylist->CombIterator()) {
		Float_t /*kine_prechi2,*/ kine_chi2;

		if (/*mylist->getUserValue(KINEFIT_PRECHI2, kine_prechi2) &&*/
					mylist->getUserValue(KINEFIT_CHI2, kine_chi2)){

			if( /*kine_prechi2>0.0 &&*/ kine_chi2>0.0){
				mylist->resetProbAlg(TMath::Prob(kine_chi2,1));
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
