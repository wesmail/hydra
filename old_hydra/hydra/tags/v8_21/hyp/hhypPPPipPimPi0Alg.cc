//*-- AUTHOR : Tiago Perez
//*-- Modified : 05/01/2005 by Tiago Perez
//*-- Modified : 04/07/2005 by I. Froehlich
//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////
//
// HHypPPPipPimPi0Alg
//
// This is a SELECTOR removing combinations which do not have
// a missing Pi0
//
////////////////////////////////////////////////////////////////////////

using namespace std;

#include <stdlib.h>
#include <iostream>
#include "hhypPPPipPimPi0Alg.h"
#include "hypinfodef.h"

ClassImp(HHypPPPipPimPi0Alg)

HHypPPPipPimPi0Alg::HHypPPPipPimPi0Alg(Char_t *name_i, Option_t par[])
  :HHypBaseAlgorithm(name_i,par)
{
}

HHypPPPipPimPi0Alg::~HHypPPPipPimPi0Alg()
{
}

Bool_t HHypPPPipPimPi0Alg::execute()
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
    TLorentzVector pip = mylist->getTLorentzVector("pi+", 1);
    TLorentzVector pim = mylist->getTLorentzVector("pi-", 1);

    if (mylist->getIterStatus() == kTRUE) {

      // calculating missing mass
      TLorentzVector miss4 = (*beam) - (proton1 + proton2 + pip + pim);
      TLorentzVector pp_miss = (*beam) - (proton1 + proton2);
      TLorentzVector pippim_invmass = (pip + pim);

      // NOW I HAVE TO DO THE SELECTION!!!!
      // Here I set all combinations unvalid which do not fulfill 
      // the requirement

      if (miss4.M2() < lower_pi_cut) {   // [MeV]
        mylist->removeComb();
	// cout << "lower removed " << miss4.M2() << endl;
      }
      else if (miss4.M2() > upper_pi_cut) {  // [MeV]
        mylist->removeComb();
	// cout << "upper removed " << miss4.M2() << endl;
      }
      // else cout <<  "keep " << miss4.M2() << endl;
      
      // Remove unphysical events here:
      if (pp_miss.M2() < pippim_invmass.M2())
	mylist->removeComb();

    }                           //END getIterStatus() == kTRUE
  }                             //END Iterator
  if (exitIdx > -1)
    return kTRUE;
  return kFALSE;
}

Bool_t HHypPPPipPimPi0Alg::init()
{
  pParams->registerCut("PPPipPimPi0_LOWER_CUT");
  pParams->registerCut("PPPipPimPi0_UPPER_CUT");
  return kTRUE;
}

Bool_t HHypPPPipPimPi0Alg::reinit()
{
  //Here, we set/reset the Pi0 cut for this run
  //Resolution could be run-dependent!

  if (!pParams->getCut("PPPipPimPi0_UPPER_CUT", upper_pi_cut)) {
    upper_pi_cut=62500;
    std::cout << "HHypPPPipPimPi0Alg::init: PI0_UPPER_CUT not found" << std::
      endl;
    std::cout << "using hardcoded PI0_UPPER_CUT" << std::endl;
    //    return kFALSE;
  }
  
  if (!pParams->getCut("PPPipPimPi0_LOWER_CUT", lower_pi_cut)) {
    lower_pi_cut=2500;
    std::cout << "HHypPPPipPimPi0Alg::init: PI0_LOWER_CUT not found" << std::
      endl;
    std::cout << "using hardcoded PI0_LOWER_CUT" << std::endl;
    //    return kFALSE;
  }
  
  std::cout << "PI0_LOWER_CUT is: " << lower_pi_cut << std::endl;
  std::cout << "PI0_UPPER_CUT is: " << upper_pi_cut << std::endl;
  
  return kTRUE;
}

Bool_t HHypPPPipPimPi0Alg::finalize()
{
  return kTRUE;
}
