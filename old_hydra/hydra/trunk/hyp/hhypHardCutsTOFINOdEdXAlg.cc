//*-- AUTHOR : B. Spruck
//*-- Modified : Juli 2008 by B. Spruck
//_HADES_CLASS_DESCRIPTION
////////////////////////////////////////////////////////////////////////
//
// HHypHardCutsTOFINOdEdXAlg
//
// Algorithm which removes combinations acconding to hard cuts
//
////////////////////////////////////////////////////////////////////////

using namespace std;

#include <stdlib.h>
#include <iostream>
#include "hhypHardCutsTOFINOdEdXAlg.h"
#include "hpidalghardcuts.h"
#include "hypinfodef.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "hpartialevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hpidgeanttrackset.h"
#include "hgeantheader.h"

ClassImp(HHypHardCutsTOFINOdEdXAlg)

HHypHardCutsTOFINOdEdXAlg::HHypHardCutsTOFINOdEdXAlg(Char_t *name_i, Option_t par[])
  :HHypBaseAlgorithm(name_i,par)
{
}

HHypHardCutsTOFINOdEdXAlg::~HHypHardCutsTOFINOdEdXAlg()
{
}

Int_t HHypHardCutsTOFINOdEdXAlg::CheckdEdX(Int_t iPid,Double_t mom,HPidTrackCand *pData)
{
	Int_t result=-1;
	HPidHitData *pHit;
	HPidTrackData *pTrack;
	pHit=pData->getHitData();
	pTrack=pData->getTrackData();
	if(pHit && pTrack && pHit->getSystem()==0){// TOFino only
		Float_t dEdX, pol;
		// dEdX=pHit->getTofEloss();// Tof only! Why ever this is not implemented for TOFino
		dEdX=pTrack->getCorrectedEloss(4); // Tof / Tofino Eloss depend on getSystem() ???
		pol=pTrack->getPolarity(ALG_RUNGEKUTTA);;
    if(MomdEdx) MomdEdx->Fill(mom*pol,dEdX);
		if(iPid==8 && bCut8) result=GCut_8->IsInside(mom*pol,dEdX);
		if(iPid==9 && bCut9) result=GCut_9->IsInside(mom*pol,dEdX);
		if(iPid==14 && bCut14) result=GCut_14->IsInside(mom*pol,dEdX);

		if( result==1){
			if(MomdEdxInside) MomdEdxInside->Fill(mom*pol,dEdX);
		}
	}
	return(result);// cannot make decision
}

Bool_t HHypHardCutsTOFINOdEdXAlg::execute()
{

	// Resetting the list and start looping over the combinations
	// Loop is only done over the VALID combinations
	mylist->CombIteratorReset();
	while (mylist->CombIterator()) {
		//cout << "CombIterator" << endl;
		Bool_t removeComb = kFALSE;
		Int_t nPart;
		nPart=mylist->getNvalid();// only valid particles
		for (Int_t i = 0; i < nPart; i++) {
			//loop over all particles
			if(!mylist->isValidPart(i)) continue;// not necess. unused particle

			Int_t result;
			result=CheckdEdX(mylist->getPid(i),mylist->getMomentum(i).Mag(),mylist->getPidTrackCand(i));
			if(result==0) removeComb=true;// outside of cut
			// else if ==-1 ... no decision because no parameter for pid
			// else if ==1 ... inside cut

		} // end loop over all particles

		if (removeComb) {
			mylist->removeComb();
		}

	} //END Iterator

	if (exitIdx > -1)
		return kTRUE;
	return kFALSE;
}

Bool_t HHypHardCutsTOFINOdEdXAlg::init()
{
	// need to get name from channel
	TString input(channel->Get(initList));

/*
  // init lepton parameters
  if((pidParams = (HPidAlgStandCutsPar *)gHades->getRuntimeDb()
      ->getContainer(PIDALGSTANDCUTSPAR_NAME)) == NULL)
    {
      Error("HHypHardCutsTOFINOdEdXAlg::init", "Cannot get parameters: %s", PIDALGSTANDCUTSPAR_NAME);
      return kFALSE;
    }

  // init hadron parameters
  if((pidParams2 = (HPidAlgHardCutsTOFINOdEdXPar *)gHades->getRuntimeDb()
      ->getContainer("PidAlgHardCutsTOFINOdEdXPar")) == NULL)
    {
      Error("HHypHardCutsTOFINOdEdXAlg::init", "Cannot get parameters: %s", "PidAlgHardCutsPar");
      return kFALSE;
    }

  pidParams2->registerCut("hardcut_pid_9");
  pidParams2->registerCut("hardcut_pid_8");
  pidParams2->registerCut("hardcut_pid_14");
*/

  bCut8 = kFALSE;
  bCut9 = kFALSE;
  bCut14 = kFALSE;

	// temporary workaround
  TString *path = GetOpt("cutfile"), filename;
  if (path == NULL) {
    Error("HHypHardCutsTOFINOdEdXAlg" , "no cutfile given");
  }

  //hardcuts = new HPidAlgHardCutsTOFINOdEdX(*path);
  // constructor
  filename = *path;
  if (filename.IsNull()){
		Error("HHypHardCutsTOFINOdEdXAlg::init","No input file");
	}else{
		TFile *fFile;
		// Load param file for graphical cut.
		Info("HHypHardCutsTOFINOdEdXAlg::init","Load file: "+filename);
		fFile = TFile::Open(filename,"READ");

		GCut_8 = (TCutG*)fFile->Get("hardcut_tofino_dedx_pid_8");
		if (GCut_8) bCut8 = kTRUE; else Warning("HHypHardCutsTOFINOdEdXAlg::init","No pi+ cut");
		GCut_9 = (TCutG*)fFile->Get("hardcut_tofino_dedx_pid_9");
		if (GCut_9) bCut9 = kTRUE; else Warning("HHypHardCutsTOFINOdEdXAlg::init","No pi- cut");
		GCut_14 = (TCutG*)fFile->Get("hardcut_tofino_dedx_pid_14");
		if (GCut_14) bCut14 = kTRUE; else Warning("HHypHardCutsTOFINOdEdXAlg::init","No proton cut");
	}

  MomdEdx=0;
  MomdEdxInside=0;

	if(histofile){
	  MomdEdx = new TH2I(input + TString("TOFINO_MomdEdx"),"TOFINO MomdEdx;Polarity*Momentum;dEdx Mean", 150,-2000,2000,400,-1,20);
	  MomdEdxInside = new TH2I(input + TString("TOFINO_MomdEdx_Inside"),"TOFINO MomdEdx inside;Polarity*Momentum;dEdx Mean", 150,-2000,2000,400,-1,20);
// 		MomdEdx->SetBit(TH1::kCanRebin);
	}

  return kTRUE;
}

Bool_t HHypHardCutsTOFINOdEdXAlg::reinit()
{
/*
  GCut_8 = pidParams2->getCut("hardcut_pid_8");
  if (!GCut_8)
    Error("HPidAlgHardCutsTOFINOdEdX::init","No pi+ cut");
  GCut_9 = pidParams2->getCut("hardcut_pid_9");
  if (!GCut_8)
    Error("HPidAlgHardCutsTOFINOdEdX::init","No pi- cut");
  GCut_14 = pidParams2->getCut("hardcut_pid_14");
  if (!GCut_8)
    Error("HPidAlgHardCutsTOFINOdEdX::init","No p cut");
*/
  return kTRUE;
}

Bool_t HHypHardCutsTOFINOdEdXAlg::finalize()
{
	if(histofile){
	  if(MomdEdx) MomdEdx->Write();
	  if(MomdEdxInside) MomdEdxInside->Write();
	}

  return kTRUE;
}
