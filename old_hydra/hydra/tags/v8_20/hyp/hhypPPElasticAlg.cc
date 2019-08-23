//*-- Author : B. Spruck
//*-- Modified : 2005-04-25
//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////
//
// HHypPPElasticAlg
//
// HHypPPElasticAlg is an algorithm which selects PP elastic events
// out of PP events. It must be connected to the output of HHypPPXAlg.
// The cuts have to be discussed! At the moment they are just
// for testing.
//
////////////////////////////////////////////////////////////////////////

using namespace std;

#include <stdlib.h>
#include <iostream>
#include "hhypPPElasticAlg.h"
#include "hypinfodef.h"

ClassImp(HHypPPElasticAlg)

HHypPPElasticAlg::HHypPPElasticAlg(Char_t *name_i, Option_t par[])
:HHypBaseAlgorithm(name_i,par)
{
}

HHypPPElasticAlg::~HHypPPElasticAlg()
{
}

Bool_t HHypPPElasticAlg::execute()
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

      // here has to come the phi cut and (maybe) even a missing mass cut
      if (TMath::Abs(TMath::Abs(proton1.Phi() - proton2.Phi()) - TMath::Pi()) > 5.0 * TMath::DegToRad()) {      
	// Phi in Rad
        mylist->resetProbAlg(-1.);
      } else {
        TLorentzVector pp_miss = (*beam) - (proton1 + proton2);

        // Missing Mass should be ZERO
        if (pp_miss.M2() < -1000000 || pp_miss.M2() > 1000000)
          mylist->resetProbAlg(-1.);    // [MeV]
      }

    }                           //END getIterStatus() == kTRUE
  }                             //END Iterator

  if (exitIdx > -1)
    return kTRUE;
  return kFALSE;
}

Bool_t HHypPPElasticAlg::init()
{
  return kTRUE;
}

Bool_t HHypPPElasticAlg::reinit()
{
  return kTRUE;
}

Bool_t HHypPPElasticAlg::finalize()
{
  return kTRUE;
}
