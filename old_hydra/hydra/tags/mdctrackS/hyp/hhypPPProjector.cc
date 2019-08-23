//*-- Author : B. Spruck
//*-- Modified : 2005-04-25
////////////////////////////////////////////////////////////////////////
//
// HHypPPProjector
//
// HHypPPProjector projects any PP data. At the moment output contains
// delta_phi and missing_mass.
// Connects for example to HHypPPXAlg or HHypPPElasticAlg.
//
////////////////////////////////////////////////////////////////////////

using namespace std;

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "hevent.h"
#include "heventheader.h"
#include "hdetector.h"
#include "hratreeext.h"
#include "hcategory.h"
#include "hmatrixcategory.h"
#include "hlinearcategory.h"
#include "hlinearcatiter.h"
#include "hlocation.h"
#include "hiterator.h"
#include "hdebug.h"
#include "hades.h"

#include "hhypPPProjector.h"
#include "hhyplist.h"
#include "hhypchannel.h"
#include "hypinfodef.h"

ClassImp(HHypPPProjector)

HHypPPProjector::HHypPPProjector(char *name_i, Option_t par[])
  :HHypBaseAlgorithm(name_i,par)
{
  simuflag = 0;
}

HHypPPProjector::~HHypPPProjector()
{
}

Bool_t HHypPPProjector::execute()
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

    if (mylist->getIterStatus() == kTRUE) {

      // calculating missing mass
      TLorentzVector pp_miss = (*beam) - (proton1 + proton2);

      Double_t prob = mylist->getProbAlg();

      miss->Fill(pp_miss.M2(), fabs(proton1.Phi() - proton2.Phi()), prob);

    } else
      cerr << algoName << " got no TLorentzVector " << endl;

  }

  return kTRUE;
}

Bool_t HHypPPProjector::init()
{

  simCat = gHades->getCurrentEvent()->getCategory(catGeantKine);

  if (!simCat) {
    simuflag = 0;
  } else {
    simuflag = 1;
    //cout << "Projector uses SIMULATION" << endl;

    CatPartSim = NULL;          // Category

    if ((CatPartSim =
         gHades->getCurrentEvent()->getCategory(catPidPart)) == NULL) {
      Error("init", "Cannot get catPidPartSim cat");
      return kFALSE;
    }
  }

  // need to get name from channel
  TString input(channel->Get(initList));

  miss =
    new TNtuple(input + TString("_proj"), "PP Demo ntuple",
                "pp_miss:pp_dphi:fProbAlg");

  return kTRUE;
}

Bool_t HHypPPProjector::reinit()
{
  return kTRUE;
}

Bool_t HHypPPProjector::finalize()
{
  miss->Write();
  // delete miss ???
  return kTRUE;
}
