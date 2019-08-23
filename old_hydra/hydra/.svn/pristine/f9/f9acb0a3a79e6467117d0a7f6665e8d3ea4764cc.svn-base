//*-- AUTHOR : Tiago Perez
//*-- Modified : 05/01/2005 by Tiago Perez
//*-- Modified : 04/07/2005 by I. Froehlich
//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////
//
// HHypPPPipPimMppMinvAlg
//
// This is a SELECTOR removing combinations which do not have
// Mmiss pp > Minv PipPim 
//
////////////////////////////////////////////////////////////////////////

using namespace std;

#include <stdlib.h>
#include <iostream>
#include "hhypPPPipPimMppMinvAlg.h"
#include "hypinfodef.h"

ClassImp(HHypPPPipPimMppMinvAlg)

HHypPPPipPimMppMinvAlg::HHypPPPipPimMppMinvAlg(Char_t *name_i, Option_t par[])
  :HHypBaseAlgorithm(name_i,par)
{
}

HHypPPPipPimMppMinvAlg::~HHypPPPipPimMppMinvAlg()
{
}

Bool_t HHypPPPipPimMppMinvAlg::execute()
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
      TLorentzVector pp_miss = (*beam) - (proton1 + proton2);
      TLorentzVector pippim_invmass = (pip + pim);

      // Remove unphysical events here:
      if (pp_miss.M2() < pippim_invmass.M2())
	mylist->removeComb();

    }                           //END getIterStatus() == kTRUE
  }                             //END Iterator
  if (exitIdx > -1)
    return kTRUE;
  return kFALSE;
}

Bool_t HHypPPPipPimMppMinvAlg::init()
{
//   pParams->registerCut("PPPipPimMppMinv_LOWER_CUT");
//   pParams->registerCut("PPPipPimMppMinv_UPPER_CUT");
  return kTRUE;
}

Bool_t HHypPPPipPimMppMinvAlg::reinit()
{
  //Here, we set/reset the Pi0 cut for this run
  //Resolution could be run-dependent!

//   if (!pParams->getCut("PPPipPimMppMinv_UPPER_CUT", upper_pi_cut)) {
//     upper_pi_cut=62500;
//     std::cout << "HHypPPPipPimMppMinvAlg::init: PI0_UPPER_CUT not found" << std::
//       endl;
//     std::cout << "using hardcoded PI0_UPPER_CUT" << std::endl;
//     //    return kFALSE;
//   }
  
//   if (!pParams->getCut("PPPipPimMppMinv_LOWER_CUT", lower_pi_cut)) {
//     lower_pi_cut=2500;
//     std::cout << "HHypPPPipPimMppMinvAlg::init: PI0_LOWER_CUT not found" << std::
//       endl;
//     std::cout << "using hardcoded PI0_LOWER_CUT" << std::endl;
//     //    return kFALSE;
//   }
  
//   std::cout << "PI0_LOWER_CUT is: " << lower_pi_cut << std::endl;
//   std::cout << "PI0_UPPER_CUT is: " << upper_pi_cut << std::endl;
  
  return kTRUE;
}

Bool_t HHypPPPipPimMppMinvAlg::finalize()
{
  return kTRUE;
}
