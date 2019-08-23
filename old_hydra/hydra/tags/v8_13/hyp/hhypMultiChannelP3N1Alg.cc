//*-- Author : 
//*-- Modified : 
//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////
//
// HHypMultiChannelP3N1Alg
//
// This Algorythm is a wrapper for the hhypmultichannelfiller
// It lookes for combinations with 3pos and 1neg tracks in the final state
//
////////////////////////////////////////////////////////////////////////

using namespace std;

#include <stdlib.h>
#include <iostream>
#include "hhypMultiChannelP3N1Alg.h"
#include "hypinfodef.h"

ClassImp(HHypMultiChannelP3N1Alg)

  HHypMultiChannelP3N1Alg::HHypMultiChannelP3N1Alg(char *name_i, Option_t par[])
:HHypBaseAlgorithm(name_i, par)
{
  // 4 Particles
  filler = NULL;

}

HHypMultiChannelP3N1Alg::~HHypMultiChannelP3N1Alg()
{
  if (filler)
    delete          filler;
}

Bool_t HHypMultiChannelP3N1Alg::execute()
{

  exitIdx = filler->execute();

  if (exitIdx > -1)
    return kTRUE;

  return kFALSE;
}

Bool_t HHypMultiChannelP3N1Alg::init()
{
  Bool_t flag;

  filler = new HHypMultiChannelFiller("filler", "filler");
  filler->setNPlus(3);
  filler->setNMinus(1);
  filler->SetExitList(exitList);
  flag = filler->init();
  return flag;
}

Bool_t HHypMultiChannelP3N1Alg::reinit()
{
  return kTRUE;
}

Bool_t HHypMultiChannelP3N1Alg::finalize()
{
  return kTRUE;
}
