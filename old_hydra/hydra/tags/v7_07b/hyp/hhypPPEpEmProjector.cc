//*-- Author : B. Spruck
//*-- Modified : 2005-04-25
////////////////////////////////////////////////////////////////////////
//
// HHypPPEpEmProjector
//
// Not yet used ... work in progress
//
////////////////////////////////////////////////////////////////////////

using namespace std;

#include <stdlib.h>
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

#include "hhypPPEpEmProjector.h"
#include "hhyplist.h"
#include "hhypchannel.h"
#include "hypinfodef.h"

ClassImp(HHypPPEpEmProjector)

HHypPPEpEmProjector::HHypPPEpEmProjector(char *name_i, Option_t par[])
  :HHypBaseAlgorithm(name_i,par)
{
  simuflag = 0;
}

HHypPPEpEmProjector::~HHypPPEpEmProjector()
{
}

Bool_t HHypPPEpEmProjector::execute()
{

  if (!beam) {
    cerr << algoName << " needs beam particle! " << endl;
    return kFALSE;
  }

  HHypList *p_hyplist = NULL;

  if (sourceIdx >= 0) {
    HCategory *listCat = gHades->getCurrentEvent()->getCategory(catHypList);

    p_hyplist = (HHypList *) listCat->getObject(sourceIdx);
  }

  if (!p_hyplist)
    return kFALSE;

  p_hyplist->CombIteratorReset();


  while (p_hyplist->CombIterator()) {

    TLorentzVector proton1 = p_hyplist->getTLorentzVector("p", 1);
    TLorentzVector proton2 = p_hyplist->getTLorentzVector("p", 2);
    TLorentzVector ep = p_hyplist->getTLorentzVector("e+", 1);
    TLorentzVector em = p_hyplist->getTLorentzVector("e-", 1);

    if (p_hyplist->getIterStatus() == kTRUE) {

      // calculating missing mass
      TLorentzVector pp_miss = (*beam) - (proton1 + proton2);
      TLorentzVector miss4 = (*beam) - (proton1 + proton2 + ep + em);
      TLorentzVector epem_invmass = (ep + em);

      Double_t prob = p_hyplist->getProbAlg();

      if (simuflag == 0)
        miss->Fill(pp_miss.M2(), miss4.M2(), epem_invmass.M2(), prob);
      else
        miss->Fill(pp_miss.M2(), miss4.M2(), epem_invmass.M2(), prob);


    } else
      cerr << algoName << " got no TLorentzVector " << endl;

  }

  return kTRUE;
}

Bool_t HHypPPEpEmProjector::init()
{

  simCat = gHades->getCurrentEvent()->getCategory(catGeantKine);

  if (!simCat)
    simuflag = 0;
  else {
    simuflag = 1;
    //cout << "Projector uses SIMULATION" << endl;

    CatPartSim = NULL;          // Category

    if ((CatPartSim =
         gHades->getCurrentEvent()->getCategory(catPidPart)) == NULL) {
      Error("init", "Cannot get catPidPartSim cat");
      return kFALSE;
    }

  }

  std::cout << "booking ntuples for " << algoName << std::endl;

  // need to get name from channel
  TString input(channel->Get(initList));

  if (simuflag == 0)
    miss =
      new TNtuple(input + TString("_proj"), "Demo ntuple",
                  "pp_miss:miss4:epem_invmass:fProbAlg");
  else
    miss =
      new TNtuple(input + TString("_proj"), "Demo ntuple",
                  "pp_miss:miss4:epem_invmass:fProbAlg");

  std::cout << "...done" << std::endl;

  return kTRUE;
}

Bool_t HHypPPEpEmProjector::reinit()
{
  return kTRUE;
}

Bool_t HHypPPEpEmProjector::finalize()
{
  miss->Write();
  return kTRUE;
}
