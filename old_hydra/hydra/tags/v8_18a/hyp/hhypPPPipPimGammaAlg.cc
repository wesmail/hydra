//*-- AUTHOR : Tiago Perez
//*-- Modified : 05/01/2005 by Tiago Perez
//*-- Modified : 04/07/2005 by I. Froehlich
//*-- Modified : 20/01/2006 by T. Perez
//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////
//
// HHypPPPipPimGammaAlg
//
// This is a SELECTOR removing combinations which do not fullfill
// | miss4 | < 6000 MeV**2/c**4
// PtPz in the outer circle : 
//  ---> ( (pt*pt)/2 ) + ( (pl-26)*(pl-26) )> 4500
// 
////////////////////////////////////////////////////////////////////////

using namespace std;

#include <stdlib.h>
#include <iostream>
#include "hhypPPPipPimGammaAlg.h"
#include "hypinfodef.h"

ClassImp(HHypPPPipPimGammaAlg)

HHypPPPipPimGammaAlg::HHypPPPipPimGammaAlg(Char_t *name_i, Option_t par[])
  :HHypBaseAlgorithm(name_i,par)
{
}

HHypPPPipPimGammaAlg::~HHypPPPipPimGammaAlg()
{
}

Bool_t HHypPPPipPimGammaAlg::execute()
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
    // Temp var
    Double_t t1 = 0;
    Double_t pt = 0;
    Double_t pl = 0;
    
    if (mylist->getIterStatus() == kTRUE) {

      // calculating missing mass
      TLorentzVector pp_miss = (*beam) - (proton1 + proton2);
      TLorentzVector pippim_invmass = (pip + pim);
      TLorentzVector miss4 = (*beam) - (proton1 + proton2 + pip + pim);

      // Select event with miss4 around 0 :: missing gamma.
      // HARD-CODED 6000 !!!
      if (miss4.M2() > 6000 || miss4.M2() < -6000)
	mylist->removeComb();
      else {
	miss4.Boost(0.0, 0.0, -(*beam).Beta() );
	pt= miss4.Pt();
	pl= miss4.Pz();
	t1 = ( (pt*pt)/2 ) + ( (pl-26)*(pl-26) );
	// Select events outside of the outer circle
	// HARD-CODED 4500 !!!
	if (t1 < 4500)
	  mylist->removeComb();
      }

    }                           //END getIterStatus() == kTRUE
  }                             //END Iterator
  if (exitIdx > -1)
    return kTRUE;
  return kFALSE;
}

Bool_t HHypPPPipPimGammaAlg::init()
{
  return kTRUE;
}

Bool_t HHypPPPipPimGammaAlg::reinit()
{
  return kTRUE;
}

Bool_t HHypPPPipPimGammaAlg::finalize()
{
  return kTRUE;
}
