//*-- AUTHOR : Marco Destefanis
//*-- Modified : 22/07/2005 by Marco Destefanis
//*-- Modified : 26/07/2005 by Marco Destefanis
//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////
//
// HHypPPLambdaXAlg
//
// This is a FILLER for the channel P P K_plus Pi_minus + something
//
////////////////////////////////////////////////////////////////////////

using namespace std;
#include <stdlib.h>
#include <iostream>
#include "hhypPPKpPimXAlg.h"
#include "hypinfodef.h"

ClassImp(HHypPPKpPimXAlg)
  
  HHypPPKpPimXAlg::HHypPPKpPimXAlg(char *name_i, Option_t par[]) : HHypBaseAlgorithm(name_i, par)
{
  filler = NULL;
}


HHypPPKpPimXAlg::~HHypPPKpPimXAlg()
{
  if (filler) delete filler;
}


Bool_t HHypPPKpPimXAlg::execute()
{
  exitIdx = filler->execute();
  
  if (exitIdx > -1) return kTRUE;

  return kFALSE;
}

Bool_t HHypPPKpPimXAlg::init()
{

  filler = new HHypListFiller("aaa","bbb");
  filler->SetExitList(exitList);
 
  filler->AddTrack(HPidPhysicsConstants::pid("p"));
  filler->AddTrack(HPidPhysicsConstants::pid("K+"));
  filler->AddTrack(HPidPhysicsConstants::pid("p"));
  filler->AddTrack(HPidPhysicsConstants::pid("pi-"));

  return filler->init();
}

Bool_t HHypPPKpPimXAlg::reinit()
{
  return kTRUE;
}

Bool_t HHypPPKpPimXAlg::finalize()
{
  return kTRUE;
}
