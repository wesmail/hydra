//*-- Author : B. Spruck
//*-- Modified : 2005-04-25
////////////////////////////////////////////////////////////////////////
//
// HHypPPEpEmGammaAlg
//
// Not yet used ... work in progress
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

  // can modify something here...
  //mylist->...

  //  cout << algoName << ":" << exitIdx << endl;

  if (exitIdx > -1)
    return kTRUE;
  return kFALSE;
}

Bool_t HHypPPEpEmGammaAlg::init()
{

  /* needed: say filler which particles I want to have */

  return kTRUE;
}

Bool_t HHypPPEpEmGammaAlg::reinit()
{
  return kTRUE;
}

Bool_t HHypPPEpEmGammaAlg::finalize()
{
  return kTRUE;
}
