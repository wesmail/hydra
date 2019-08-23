//*-- Author : B. Spruck
//*-- Modified : 2005-04-25
////////////////////////////////////////////////////////////////////////
//
// HHypPPEpEmXAlg
//
// Not yet used ... work in progress
//
////////////////////////////////////////////////////////////////////////

using namespace std;

#include <stdlib.h>
#include <iostream>
#include "hhypPPEpEmXAlg.h"
#include "hypinfodef.h"

ClassImp(HHypPPEpEmXAlg)

HHypPPEpEmXAlg::HHypPPEpEmXAlg(char *name_i, Option_t par[])
  :HHypBaseAlgorithm(name_i,par)
{
  filler = NULL;
}

HHypPPEpEmXAlg::~HHypPPEpEmXAlg()
{
  if (filler)
    delete filler;
}

Bool_t HHypPPEpEmXAlg::execute()
{
  //cout << "HHypPPEpEmXAlg" << endl;
  exitIdx = filler->execute();

  if (exitIdx > -1) {
    //cout << "exitIdx " << exitIdx << endl;
    return kTRUE;
  }
  return kFALSE;
}

Bool_t HHypPPEpEmXAlg::init()
{
  filler = new HHypListFiller("aaa", "bbb");
  filler->SetExitList(exitList);

  filler->AddTrack(HPidPhysicsConstants::pid("p"));
  filler->AddTrack(HPidPhysicsConstants::pid("p"));
  filler->AddTrack(HPidPhysicsConstants::pid("e+"));
  filler->AddTrack(HPidPhysicsConstants::pid("e-"));

  return filler->init();
}

Bool_t HHypPPEpEmXAlg::reinit()
{
  return kTRUE;
}

Bool_t HHypPPEpEmXAlg::finalize()
{
  return kTRUE;
}
