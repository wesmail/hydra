//*-- AUTHOR : I. Froehlich
//*-- Modified : 21/04/2005 by I. Froehlich
////////////////////////////////////////////////////////////////////////
//
// HHypPPPipPimProjector
//
// Projects the events (input HHypList) to ntuple
// The file option in HHypReco->AddAlgorithm should be used
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

#include "hhypPPPipPimProjector.h"
#include "hhyplist.h"
#include "hhypchannel.h"
#include "hypinfodef.h"

ClassImp(HHypPPPipPimProjector)

HHypPPPipPimProjector::HHypPPPipPimProjector(char *name_i, Option_t par[])
  :HHypBaseAlgorithm(name_i,par)
{
  simuflag = 0;
}

HHypPPPipPimProjector::~HHypPPPipPimProjector()
{
}

Bool_t HHypPPPipPimProjector::execute()
{
  // Reads the input particle(s) from the HHypList
  // Important: the "beam" has to be defined in the macro
  // The content of the ntuple is different in sim/exp

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

    if (mylist->getIterStatus() == kTRUE) {

      // calculating missing mass
      TLorentzVector pp_miss = (*beam) - (proton1 + proton2);
      TLorentzVector miss4 = (*beam) - (proton1 + proton2 + pip + pim);
      TLorentzVector pippim_invmass = (pip + pim);

      Double_t prob = mylist->getProbAlg();

      //std::cout << "got prob " << prob << std::endl;

      // this is for simulation :

      Double_t pip_source = 0;
      Double_t pim_source = 0;

      if (simuflag == 1) {

        HPidParticleSim *my_pip =
          (HPidParticleSim *) CatPartSim->getObject(mylist->
                                                    getIdxPidPart("pi+", 1));
        HPidParticleSim *my_pim =
          (HPidParticleSim *) CatPartSim->getObject(mylist->
                                                    getIdxPidPart("pi-", 1));

        if ((my_pip != NULL) && (my_pim != NULL)
            && (mylist->getIterStatus() == kTRUE)) {

          HPidGeantTrackSet *pipGeantSet = my_pip->getGeantTrackSet();
          HPidGeantTrackSet *pimGeantSet = my_pim->getGeantTrackSet();

          // look only for the 1st geant track, makes life more easy  [Ingo] 

          Int_t pip_geant_track = pipGeantSet->getGeantTrackID(0);
          Int_t pim_geant_track = pimGeantSet->getGeantTrackID(0);

          if (pip_geant_track >= 0) {
            HGeantKine *pip_geantkine =
              (HGeantKine *) pipGeantSet->getGeantKine(pip_geant_track);
            HGeantKine *pim_geantkine =
              (HGeantKine *) pimGeantSet->getGeantKine(pim_geant_track);

            Float_t geninfo, genweight;

            pip_geantkine->getGenerator(geninfo, genweight);

            // cout << "pip has geant: " << (geninfo - 14014000) << ":" << genweight << endl;
            pip_source = (geninfo - 14014000);

            pim_geantkine->getGenerator(geninfo, genweight);

            // cout << "pim has geant: " << (geninfo - 14014000) << ":" << genweight << endl;
            pim_source = (geninfo - 14014000);

          }
        }
      }
      /* end for simu */
      Float_t dtof;

      if (!mylist->getUserValue(DELTATOF_DTOF, dtof))
        dtof = -1;
      // cout << "dtof " << dtof << endl;

      Float_t pid_tracks;

      if (!mylist->getUserValue(FILLER_VALID_PIDTRACKS, pid_tracks))
        pid_tracks = -1;
      // cout << "pid_tracks " << pid_tracks << endl;

      if (simuflag == 0)
        miss->Fill(pp_miss.M2(), miss4.M2(), pippim_invmass.M2(), prob,
                   dtof, pid_tracks);
      else
        miss->Fill(pp_miss.M2(), miss4.M2(), pippim_invmass.M2(), prob,
                   dtof, pid_tracks, pip_source, pim_source);

    } else
      cerr << algoName << " got no TLorentzVector " << endl;

  }

  return kTRUE;
}

Bool_t HHypPPPipPimProjector::init()
{
  // Checks if we have sim/exp and book the ntuple

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

  // need to get name from channel
  TString input(channel->Get(initList));

  if (simuflag == 0)
    miss =
      new TNtuple(input + TString("_proj"), "Demo ntuple",
                  "pp_miss:miss4:pippim_invmass:fProbAlg:dtof:pidtr");
  else
    miss =
      new TNtuple(input + TString("_proj"), "Demo ntuple",
                  "pp_miss:miss4:pippim_invmass:fProbAlg:dtof:pidtr:pip_source:pim_source");

  return kTRUE;
}

Bool_t HHypPPPipPimProjector::reinit()
{
  return kTRUE;
}

Bool_t HHypPPPipPimProjector::finalize()
{
  miss->Write();
  return kTRUE;
}
