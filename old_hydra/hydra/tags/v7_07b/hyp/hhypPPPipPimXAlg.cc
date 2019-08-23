//*-- AUTHOR : Tiago Perez
//*-- Modified : 05/01/2005 by Tiago Perez
//*-- Modified : 04/07/2005 by I. Froehlich
////////////////////////////////////////////////////////////////////////
//
// HHypPPPipPimXAlg
//
// This is a FILLER for the channel P P Pip Pim + something
//
////////////////////////////////////////////////////////////////////////

using namespace std;

#include <stdlib.h>
#include <iostream>
#include "hhypPPPipPimXAlg.h"
#include "hypinfodef.h"

ClassImp(HHypPPPipPimXAlg)

HHypPPPipPimXAlg::HHypPPPipPimXAlg(char *name_i, Option_t par[])
  :HHypBaseAlgorithm(name_i,par)
{
  filler = NULL;
}

HHypPPPipPimXAlg::~HHypPPPipPimXAlg()
{
  if (filler)
    delete filler;
}

Bool_t HHypPPPipPimXAlg::execute()
{

  // cout << "HHypPPPipPimXAlg" << endl;
  exitIdx = filler->execute();

  if (exitIdx > -1) {
    //cout << "exitIdx " << exitIdx << endl;
    return kTRUE;
  }

  return kFALSE;
}

Bool_t HHypPPPipPimXAlg::init()
{

  filler = new HHypListFiller("aaa", "bbb");
  filler->SetExitList(exitList);

  filler->AddTrack(HPidPhysicsConstants::pid("p"));
  filler->AddTrack(HPidPhysicsConstants::pid("p"));
  filler->AddTrack(HPidPhysicsConstants::pid("pi+"));
  filler->AddTrack(HPidPhysicsConstants::pid("pi-"));

  return filler->init();
}

Bool_t HHypPPPipPimXAlg::reinit()
{
  return kTRUE;
}

Bool_t HHypPPPipPimXAlg::finalize()
{
  return kTRUE;
}
