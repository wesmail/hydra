//*-- Author : B. Spruck
//*-- Modified : 2 Now 2005
////////////////////////////////////////////////////////////////////////
//
// HHypPPEpEmGammaAlg
//
// work in progress
//
////////////////////////////////////////////////////////////////////////

using namespace std;

#include <stdlib.h>
#include <iostream>
#include "hhypPPEpEmGammaAlg.h"
#include "hypinfodef.h"

ClassImp(HHypPPEpEmGammaAlg)

HHypPPEpEmGammaAlg::HHypPPEpEmGammaAlg(char *name_i, Option_t par[])
:HHypBaseAlgorithm(name_i,par)
{
}

HHypPPEpEmGammaAlg::~HHypPPEpEmGammaAlg()
{
}

Bool_t HHypPPEpEmGammaAlg::execute()
{
	if (!beam) {
		cerr << algoName << " needs beam particle! " << endl;
		return kFALSE;
	}
	// Resetting the list and start looping over the combinations
	// Loop is only done over the VALID combinations
	mylist->CombIteratorReset();
	while (mylist->CombIterator()) {
	
		// Getting the particles 
		TLorentzVector proton1 = mylist->getTLorentzVector("p", 1);
		TLorentzVector proton2 = mylist->getTLorentzVector("p", 2);
		TLorentzVector ep = mylist->getTLorentzVector("e+", 1);
		TLorentzVector em = mylist->getTLorentzVector("e-", 1);
	
		if (mylist->getIterStatus() == kTRUE) {
	
			// calculating missing mass
			TLorentzVector miss4 = (*beam) - (proton1 + proton2 + ep + em);
			TLorentzVector pp_miss = (*beam) - (proton1 + proton2);
			TLorentzVector epem_invmass = (ep + em);
		
			// NOW I HAVE TO DO THE SELECTION!!!!
			// Here I set all combinations unvalid which do not fulfill 
			// the requirement
		
			if (miss4.M2() < lower_gamma_cut) {   // [MeV]
				mylist->removeComb();
				// cout << "lower removed " << miss4.M2() << endl;
			}
			else if (miss4.M2() > upper_gamma_cut) {  // [MeV]
				mylist->removeComb();
				// cout << "upper removed " << miss4.M2() << endl;
			}
			// else cout <<  "keep " << miss4.M2() << endl;
			
			// Remove unphysical events here:
			// This wont work with missing mass==0
//			if (pp_miss.M2() < epem_invmass.M2()){
//				mylist->removeComb();
//			}
		}                           //END getIterStatus() == kTRUE
	}                             //END Iterator
	if (exitIdx > -1)
		return kTRUE;
	return kFALSE;
}

Bool_t HHypPPEpEmGammaAlg::init()
{
	pParams->registerCut("PPEpEmGamma_LOWER_CUT");
	pParams->registerCut("PPEpEmGamma_UPPER_CUT");
	
	return kTRUE;
}

Bool_t HHypPPEpEmGammaAlg::reinit()
{
	//Here, we set/reset the Gamma ("Restmass") cut for this run
	//Resolution could be run-dependent!
	
	if (!pParams->getCut("PPEpEmGamma_UPPER_CUT", upper_gamma_cut)) {
		upper_gamma_cut=23600.0;
		std::cout << "HHypPPEpEmGammaAlg::init: GAMMA_UPPER_CUT not found" << std::endl;
		std::cout << "using hardcoded GAMMA_UPPER_CUT" << std::endl;
	//    return kFALSE;
	}
	
	if (!pParams->getCut("PPEpEmGamma_LOWER_CUT", lower_gamma_cut)) {
		lower_gamma_cut=-23600.0;
		std::cout << "HHypPPEpEmGammaAlg::init: GAMMA_LOWER_CUT not found" << std::endl;
		std::cout << "using hardcoded GAMMA_LOWER_CUT" << std::endl;
	//    return kFALSE;
	}
	
	std::cout << "GAMMA_LOWER_CUT is: " << lower_gamma_cut << std::endl;
	std::cout << "GAMMA_UPPER_CUT is: " << upper_gamma_cut << std::endl;
	
	return kTRUE;
}

Bool_t HHypPPEpEmGammaAlg::finalize()
{
	return kTRUE;
}
