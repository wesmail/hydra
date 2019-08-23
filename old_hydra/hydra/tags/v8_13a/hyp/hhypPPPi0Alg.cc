//*-- Author : Marcin Wisniowski
//*-- Modified : 31.08.2005
//_HADES_CLASS_DESCRIPTION 
	////////////////////////////////////////////////////////////////////////
	//
	// HHypPPPi0Alg
	//
	// HHypPPPi0Alg is an algorithm which selects P P -> P P Pi0 events
	// out of P P X events. It must be connected to the output of HHypPPXAlg.
	//
	////////////////////////////////////////////////////////////////////////

using namespace std;

#include <stdlib.h>
#include <iostream>
#include "hhypPPPi0Alg.h"
#include "hypinfodef.h"

ClassImp(HHypPPPi0Alg)

HHypPPPi0Alg::HHypPPPi0Alg(char *name_i, Option_t par[])
:HHypBaseAlgorithm(name_i,par)
{
}

HHypPPPi0Alg::~HHypPPPi0Alg()
{
}

Bool_t HHypPPPi0Alg::execute()
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

    if (mylist->getIterStatus() == kTRUE) {

      // NOW I HAVE TO DO THE SELECTION!!!!
      // Here I set all combinations unvalid which do not fulfill
      // the requirement

	// Phi in Rad
        //mylist->resetProbAlg(-1.);
        TLorentzVector pp_miss = (*beam) - (proton1 + proton2);

        // Missing Mass should be = Pi0 mass
        if (pp_miss.M2() < -900000000 || pp_miss.M2() > 900000000)                    
          mylist->resetProbAlg(-1.);    // [MeV]

    }                           //END getIterStatus() == kTRUE
  }                             //END Iterator

  if (exitIdx > -1)
    return kTRUE;
  return kFALSE;
}

Bool_t HHypPPPi0Alg::init()
{
  return kTRUE;
}

Bool_t HHypPPPi0Alg::reinit()
{
  return kTRUE;
}

Bool_t HHypPPPi0Alg::finalize()
{
  return kTRUE;
}
