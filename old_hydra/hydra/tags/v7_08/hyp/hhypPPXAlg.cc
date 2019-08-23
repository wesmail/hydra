//*-- Author : B. Spruck
//*-- Modified : 2005-04-25
////////////////////////////////////////////////////////////////////////
//
// HHypPPXAlg
//
// HHypPPXAlg is an algorithm which selects PP (+X) events.
// This is the starting Algo for any PP elastic or PP Pi0 Algo.
// F.e. HHypPPElastic needs this as input.
//
////////////////////////////////////////////////////////////////////////

using namespace std;

#include <stdlib.h>
#include <iostream>
#include "hhypPPXAlg.h"
#include "hypinfodef.h"

ClassImp(HHypPPXAlg)

HHypPPXAlg::HHypPPXAlg(char *name_i, Option_t par[])
  :HHypBaseAlgorithm(name_i,par)
{
  filler = NULL;
}

HHypPPXAlg::~HHypPPXAlg()
{
  if (filler)
    delete filler;
}

Bool_t HHypPPXAlg::execute()
{
  exitIdx = filler->execute();

  if (exitIdx > -1)
    return kTRUE;
  return kFALSE;
}

Bool_t HHypPPXAlg::init()
{

  /* say filler which particles I want to have */
  filler = new HHypListFiller("aaa", "bbb");
  filler->SetExitList(exitList);

  filler->AddTrack(HPidPhysicsConstants::pid("p"));
  filler->AddTrack(HPidPhysicsConstants::pid("p"));

  return filler->init();
}

Bool_t HHypPPXAlg::reinit()
{
  return kTRUE;
}

Bool_t HHypPPXAlg::finalize()
{
  return kTRUE;
}
