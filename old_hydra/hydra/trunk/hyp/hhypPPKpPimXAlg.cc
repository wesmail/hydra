//*-- AUTHOR : Marco Destefanis
//*-- Modified : 22/07/2005 by Marco Destefanis
//*-- Modified : 26/07/2005 by Marco Destefanis
//_HADES_CLASS_DESCRIPTION
////////////////////////////////////////////////////////////////////////
//
// HHypPPLambdaXAlg
//
// *** this Alg is not needed anymore, please use HHypAllFillerAlg instead ***
//
//  hyp->AddAlgorithm("empty",new HHypAllFillerAlg("All Filler",
//                      standardHypOption + "Reactions=(p p K+ pi-;)"),"channame");
//
// if you want too include up to two fakes
//  hyp->AddAlgorithm("empty",new HHypAllFillerAlg("All Filler",
//     standardHypOption + "Reactions="
//     "(p p K- pi-;)"
//     "(p p K- pi- fake+;)"
//     "(p p K- pi- fake-;)"
//     "(p p K- pi- fake+ fake+;)"
//     "(p p K- pi- fake- fake-;)"
//     "(p p K- pi- fake+ fake-;)"
//     ),"channame");
// see documentation in sourcecode and WIKI!!!
// For inclusive filling (aka M.W.) special care has to be taken
// with hyp->AddAlgorithm("empty",new HHypAllFillerInclusiveAlg("All Inc Filler",
//     standardHypOption + "Reactions=(p p K- pi-;)"),"channame");
//
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

  HHypPPKpPimXAlg::HHypPPKpPimXAlg(Char_t *name_i, Option_t par[]) : HHypBaseAlgorithm(name_i, par)
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
