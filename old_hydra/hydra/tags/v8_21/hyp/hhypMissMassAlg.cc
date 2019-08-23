//*-- Author : B. Spruck
//*-- Modified : June 2008
//_HADES_CLASS_DESCRIPTION
////////////////////////////////////////////////////////////////////////
//
// HHypMissMassAlg
//
// does a cut on missing mass (beam - Sum[all particles])
// upper and lower cuts are set in option string by their names with
// LOWER_CUT=SOME_STUPID_NAME1
// UPPER_CUT=SOME_STUPID_NAME2
// suggestion: use only upper case letters
//
////////////////////////////////////////////////////////////////////////

using namespace std;

#include <stdlib.h>
#include <iostream>
#include "hhypMissMassAlg.h"
#include "hypinfodef.h"

ClassImp(HHypMissMassAlg)

HHypMissMassAlg::HHypMissMassAlg(Char_t *name_i, Option_t par[])
  :HHypBaseAlgorithm(name_i,par) {
}

HHypMissMassAlg::~HHypMissMassAlg() {
}

Bool_t HHypMissMassAlg::execute()
{
// 	Int_t nMaxPart;
// 	nMaxPart=mylist->getNpart();

  if (!beam) {
    cerr << algoName << " needs beam particle! " << endl;
    return kFALSE;
  }
  // Resetting the list and start looping over the combinations
  // Loop is only done over the VALID combinations
  mylist->CombIteratorReset();
  while (mylist->CombIterator()) {

		// Getting the particles
		TLorentzVector allpart;
		TLorentzVector part;
		allpart.Clear();

		Int_t nPart;
		nPart=mylist->getNvalid();// only valid particles

		for(Int_t i=0; i<nPart; i++){
			if(!mylist->isValidPart(i)) continue;// should not be necessary... unused particle

			part = mylist->getTLorentzVector(i);
			allpart+=part;
		}

    if (mylist->getIterStatus() == kTRUE) {

      // calculating missing mass
      TLorentzVector miss4 = (*beam) - allpart;
			Double_t miss4_2=miss4.M2();
			Bool_t reject=false;

      if (miss4_2 < lower_cut) {   // [MeV]
				mylist->removeComb();
				reject=true;
			}else if (miss4_2 > upper_cut) {  // [MeV]
				mylist->removeComb();
				reject=true;
      }
			if (histofile){
				if(histBefore) histBefore->Fill(miss4_2);
				if(!reject && histAfter) histAfter->Fill(miss4_2);
			}
    }                           //END getIterStatus() == kTRUE
  }                             //END Iterator
  if (exitIdx > -1)
    return kTRUE;
  return kFALSE;
}

Bool_t HHypMissMassAlg::init()
{
	TString *ll;
	ll=GetOpt("LOWER_CUT");
	if( ll) lower_cut_name=*ll;
	ll=GetOpt("UPPER_CUT");
	if(ll) upper_cut_name=*ll;
	if(lower_cut_name=="" || upper_cut_name==""){
		std::cerr << "HypMissMassAlg::init: Cut names not set! LOWER_CUT/UPPER_CUT"<<endl;
		return kFALSE;
	}
  pParams->registerCut((Char_t *)lower_cut_name.Data());
  pParams->registerCut((Char_t *)upper_cut_name.Data());

	histBefore=0;
	histAfter=0;
  if (histofile){
	  // need to get name from channel
  	TString input(channel->Get(initList));

		histBefore=new TH1I(input + TString("_mmcut_HistBefore"),"mmcut_HistBefore",
			5000,-1e6,1e6);
		histAfter=new TH1I(input + TString("_mmcut_HistAfter"),"mmcut_HistAfter",
			5000,-1e6,1e6);
	}

  return kTRUE;
}

Bool_t HHypMissMassAlg::reinit()
{
  // Here, we set/reset the cut for this run
  // Resolution could be run-dependent!

  if (!pParams->getCut((Char_t *)upper_cut_name.Data(), upper_cut)) {
		std::cout << "HHypMissMassAlg::reinit: UPPER_CUT: "<<upper_cut_name<<" not found" << std::endl;
    return kFALSE;
	}

  if (!pParams->getCut((Char_t *)lower_cut_name.Data(), lower_cut)) {
    std::cout << "HHypMissMassAlg::reinit: LOWER_CUT: "<<lower_cut_name<< " not found" << std::endl;
    return kFALSE;
	}

  std::cout << lower_cut_name<<" is: " << lower_cut << std::endl;
  std::cout << upper_cut_name<<" is: " << upper_cut << std::endl;

  return kTRUE;
}

Bool_t HHypMissMassAlg::finalize()
{
	if (histofile){
		if(histBefore) histBefore->Write();
		if(histAfter) histAfter->Write();
	}
  return kTRUE;
}
