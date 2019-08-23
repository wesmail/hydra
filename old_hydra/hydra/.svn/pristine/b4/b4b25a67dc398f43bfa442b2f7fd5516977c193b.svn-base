//*-- AUTHOR : B. Spruck
//*-- Modified : 01.12.2008 by B. Spruck
//_HADES_CLASS_DESCRIPTION
////////////////////////////////////////////////////////////////////////
//
// HHypDeltaTofMeanWeightAlg
//
// Algorithm which "removes" combinations acconding to time-of-flight
// differences
//
// This is an easy "mean start time" calculation, dtof is
// SUM( weight*(tof-mean)) / SUM(weight)
// Weight is taking from some arbitrary momentum errors
// and there dependence on Time of flight
// There more improvements possible!!!
// F.i. one SHOULD include measured TOF error from TOF(ino) detector
//
////////////////////////////////////////////////////////////////////////

using namespace std;

#include "hhypDeltaTofMeanWeightAlg.h"
#include "TVector3.h"
#define deltaTOFdebug 0

ClassImp(HHypDeltaTofMeanWeightAlg)

HHypDeltaTofMeanWeightAlg::HHypDeltaTofMeanWeightAlg(Char_t *name_i, Option_t par[])
  :HHypBaseAlgorithm(name_i,par)
{
}

HHypDeltaTofMeanWeightAlg::~HHypDeltaTofMeanWeightAlg()
{
}

Bool_t HHypDeltaTofMeanWeightAlg::execute()
{
  if (mylist->getNpart() < 2)
    return kFALSE;
  //needs at least 2 particles

  // Resetting the list and start looping over the combinations
  // Loop is only done over the VALID combinations
  mylist->CombIteratorReset();

// cout<<"HHypDeltaTofAlg3::execute() "<<GetName()<<endl;

	const Double_t  c = 299.792458; // units [mm/ns]

	Int_t nMaxPart;
	nMaxPart=mylist->getNpart();

  while (mylist->CombIterator()) {
		Int_t used_part;
		Double_t mean_exp, mean_theo;
// cout << "  Comb"<<endl;

		Double_t dtof_old;
		dtof_old=0.0;
		Double_t dtof2_old;
		dtof2_old=0.0;

		Double_t weight_all;
		Double_t weight_all2;
		Double_t weight_mean;
		weight_all=0.0;
		weight_all2=0.0;
		weight_mean=0.0;

		used_part=0;
		mean_exp=0.0;
		mean_theo=0.0;
    for (Int_t i = 0; i < nMaxPart; i++) {
			// Objects to get tracking
			Int_t           hyp_my_pid;
			HPidTrackCand   *myparticle;

			TLorentzVector  partHypLV;
			TVector3        partHypV3;
			Double_t        myhypmass;
			Double_t tof_exp_tof, tof_theo_from_mom, beta_theo_from_mom, pathLength_tracking;

      //loop over all particles
			if(!mylist->isValidPart(i)) continue;// unused particle
			HPidTrackCand *track = mylist->getPidTrackCand(i);

			hyp_my_pid = mylist->getPid(i);
			myparticle = mylist->getPidTrackCand(i);

			//Double_t beta_org = track->getTrackData()->getBeta(ALG_RUNGEKUTTA);
      //tof_exp_tof = myparticle->getTrackData()->getTofRecTof(ALG_RUNGEKUTTA);        // Exp TOF
      pathLength_tracking = myparticle->getTrackData()->getPathLength(ALG_RUNGEKUTTA); // Path Length
			Double_t beta_ex;
			beta_ex=mylist->getBeta(i);
      tof_exp_tof = pathLength_tracking / (beta_ex * c);

      // Claculate Theoretical Values Here.
      myhypmass = HPidPhysicsConstants::mass(hyp_my_pid);
      partHypV3 = mylist->getMomentum(i);
      partHypLV.SetVectM(partHypV3, myhypmass);
      beta_theo_from_mom = partHypLV.Beta();       // calculated beta from Ptracking and Mhypothesis
      tof_theo_from_mom = pathLength_tracking / (beta_theo_from_mom * c);
			Double_t mom = partHypV3.Mag(); //was: track->getTrackData()->getMomenta(ALG_RUNGEKUTTA);

			Double_t weight;
			weight=(1.0+mom*mom/(myhypmass*myhypmass))/0.05;// 5% Error ... but this doesnt matter at all... as long as no chi2 requiered
// 			weight=1./(1.0+myhypmass*myhypmass/(mom*mom));
			weight_all+=weight;
			weight_all2+=weight*weight;
			weight_mean+=weight*(tof_theo_from_mom-tof_exp_tof);

			if(histPartDtofB) histPartDtofB->Fill(tof_exp_tof-tof_theo_from_mom);
			if(histPartDtofBW) histPartDtofBW->Fill(weight*(tof_exp_tof-tof_theo_from_mom));

			dtof_old+=weight*fabs(tof_exp_tof-tof_theo_from_mom);// lineare abweichung
			dtof2_old+=weight*weight*(tof_exp_tof-tof_theo_from_mom)*(tof_exp_tof-tof_theo_from_mom);// quadr

			if (histofile) {
				Int_t ch=track->getTrackData()->getPolarity(ALG_RUNGEKUTTA);
				if(histBefore) histBefore->Fill(mom,beta_ex*ch);
			}

// cout << "    "<<i<<
// 	": tof_exp(mes) "<<tof_exp_tof<<" tof_th(beta) "<<tof_theo_from_mom<<
// 	": beta_exp(list) "<<beta_ex<<" beta_th "<<beta_theo_from_mom<<" beta(track) "<<beta_org<<
// 	endl;
			used_part++;
    }                           // end loop over all particles

		weight_mean/=weight_all;

// cout <<"  Mean: exp "<<mean_exp/used_part<<" th "<<mean_theo/used_part<<" mean "<<mean<<endl;

		dtof_old/=weight_all;
		dtof2_old/=weight_all2;

		Double_t dtof;
		Double_t dtof2;
		dtof=0.0;
		dtof2=0.0;

    Float_t mean_old;
		if (mylist->getUserValue(DELTATOF_OFFSET, mean_old)){
			weight_mean+=mean_old;
		}
    mylist->setUserValue(DELTATOF_OFFSET,weight_mean);

		for (Int_t i = 0; i < nMaxPart; i++) {
			if(!mylist->isValidPart(i)) continue;// unused particle
			//look if we have the particles
			Int_t           hyp_my_pid;
			HPidTrackCand   *myparticle;

			TLorentzVector  partHypLV;
			TVector3        partHypV3;
			Double_t        myhypmass;
			Double_t tof_exp_tof, tof_theo_from_mom, beta_theo_from_mom, pathLength_tracking;

			HPidTrackCand *track = mylist->getPidTrackCand(i);

      hyp_my_pid = mylist->getPid(i);
      myparticle = mylist->getPidTrackCand(i);

      pathLength_tracking = myparticle->getTrackData()->getPathLength(ALG_RUNGEKUTTA); // Path Length

      // Claculate Theoretical Values Here.
      myhypmass = HPidPhysicsConstants::mass(hyp_my_pid);
      partHypV3 = mylist->getMomentum(i);
      partHypLV.SetVectM(partHypV3, myhypmass);
      beta_theo_from_mom = partHypLV.Beta();       // calculated beta from Ptracking and Mhypothesis
      tof_theo_from_mom = pathLength_tracking / (beta_theo_from_mom * c);

			//Double_t beta_org = track->getTrackData()->getBeta(ALG_RUNGEKUTTA);
			Double_t beta_ex;
			beta_ex=mylist->getBeta(i);
      tof_exp_tof = pathLength_tracking / (beta_ex * c);

			Double_t mom = partHypV3.Mag(); // was: track->getTrackData()->getMomenta(ALG_RUNGEKUTTA);

			Double_t weight;
			weight=(1.0+mom*mom/(myhypmass*myhypmass))/0.05;// 5% Error ... but this doesnt matter at all... as long as no chi2 requiered
// 			weight=1./(1.0+myhypmass*myhypmass/(mom*mom));
//			weight_mean+=weight*(tof_theo_from_mom-tof_exp_tof);

			if(histPartDtof) histPartDtof->Fill(tof_exp_tof-tof_theo_from_mom);
			if(histPartDtofW) histPartDtofW->Fill(weight*(tof_exp_tof-tof_theo_from_mom));
			if(histW) histW->Fill(weight);

			dtof+=weight*fabs(tof_exp_tof-tof_theo_from_mom);// lineare abweichung
			dtof2+=weight*weight*(tof_exp_tof-tof_theo_from_mom)*(tof_exp_tof-tof_theo_from_mom);

    }
		dtof/=weight_all;
		dtof2/=weight_all2;

		Bool_t isok=is_sim_ok();
		if (histofile) {
			for (Int_t i = 0; i < nMaxPart; i++) {
				if(!mylist->isValidPart(i)) continue;// unused particle
				HPidTrackCand *track = mylist->getPidTrackCand(i);
				Int_t pid = mylist->getPid(i);
				Int_t ch=track->getTrackData()->getPolarity(ALG_RUNGEKUTTA);
// 				Double_t mom = track->getTrackData()->getMomenta(ALG_RUNGEKUTTA);
      	Double_t mom = mylist->getMomentum(i).Mag();
				Double_t beta_ex=mylist->getBeta(i);
				if(qa) qa->Fill(pid, mom, beta_ex, ch, dtof, dtof2, dtof_old, dtof2_old, isok);
				if(histAfter && isok) histAfter->Fill(mom,beta_ex*ch);
			}

			if(histDeltaDtof) histDeltaDtof->Fill(dtof_old-dtof);
			if(histDtof) histDtof->Fill(dtof);
			if(histDtof2) histDtof2->Fill(dtof2);
			if(histDtofOld) histDtofOld->Fill(dtof_old);
			if(histDtof2Old) histDtof2Old->Fill(dtof2_old);
		}

    mylist->setUserValue(DELTATOF_DTOF, dtof);
    mylist->setUserValue(DELTATOF_DTOF2, dtof2);

  } //END Iterator
// cout <<"------"<<endl;
  if (exitIdx > -1)
    return kTRUE;
  return kFALSE;
}                               // End Execute()

Bool_t HHypDeltaTofMeanWeightAlg::init()
{
  // need to get name from channel
  TString input(channel->Get(initList));

  simCat = gHades->getCurrentEvent()->getCategory(catGeantKine);
  CatTrackCandSim = NULL;          // Category

  if (!simCat){
    simuflag = 0;
  }else{
    simuflag = 1;


    if ((CatTrackCandSim =
	 gHades->getCurrentEvent()->getCategory(catPidTrackCand)) == NULL) {
      Error("init", "Cannot get catPidTrackCandSim cat");
      return kFALSE;
    }

  }

  if (histofile){
    qa =
       new TNtuple(input + TString("_dtofmeanweight_debug"), "Spectra ntuple",
                   "pid:mom:beta:ch:dtof:dtof2:dtof_old:dtof2_old:isok");
		histdir=histofile->mkdir(input + TString("_dtofmeanweight_hist"),"dtof histograms");
		histdir->cd();
		histBefore=new TH2I(input + TString("_dtofmeanweight_HistBefore"),"dtofmeanweight_HistBefore",
			500,0,3000,500,-2,2);
		histAfter=new TH2I(input + TString("_dtofmeanweight_HistAfter"),"dtofmeanweight_HistAfter",
			500,0,3000,500,-2,2);
		histDeltaDtof=new TH1I(input + TString("_dtofmeanweight_HistDeltaDtof"),"dtofmeanweight_HistDeltaDtof",
			5000,-25,25);
		histDtof=new TH1I(input + TString("_dtofmeanweight_HistDtof"),"dtofmeanweight_HistDtof",
			500,-2,2);
		histDtofOld=new TH1I(input + TString("_dtofmeanweight_HistDtofOld"),"dtofmeanweight_HistDtofOld",
			500,-2,2);
		histDtof2=new TH1I(input + TString("_dtofmeanweight_HistDtof2"),"dtofmeanweight_HistDtof2",
			500,-2,2);
		histDtof2Old=new TH1I(input + TString("_dtofmeanweight_HistDtof2Old"),"dtofmeanweight_HistDtof2Old",
			500,-2,2);
		histPartDtofB=new TH1I(input + TString("_dtofmeanweight_HistPartDtofB"),"dtofmeanweight_HistPartDtofB",
			5000,-25,25);
		histPartDtof=new TH1I(input + TString("_dtofmeanweight_HistPartDtof"),"dtofmeanweight_HistPartDtof",
			5000,-25,25);
		histPartDtofBW=new TH1I(input + TString("_dtofmeanweight_HistPartDtofBW"),"dtofmeanweight_HistPartDtofBW",
			5000,-25,25);
		histPartDtofW=new TH1I(input + TString("_dtofmeanweight_HistPartDtofW"),"dtofmeanweight_HistPartDtofW",
			5000,-25,25);
		histW=new TH1I(input + TString("_dtofmeanweight_HistW"),"dtofmeanweight_HistW",
			5000,0,50);
		histdir->SaveSelf(kTRUE);
	}
  return kTRUE;
}

Bool_t HHypDeltaTofMeanWeightAlg::reinit()
{
  return kTRUE;
}

Bool_t HHypDeltaTofMeanWeightAlg::finalize()
{

  if (histofile){
    if(qa) qa->Write();
		histdir->cd();
		histdir->Write();
		if(histBefore) histBefore->Write();
		if(histAfter) histAfter->Write();
		if(histDtof) histDtof->Write();
		if(histDtofOld) histDtofOld->Write();
		if(histDtof2) histDtof2->Write();
		if(histDtof2Old) histDtof2Old->Write();
		if(histDeltaDtof) histDeltaDtof->Write();
		if(histPartDtofB) histPartDtofB->Write();
		if(histPartDtof) histPartDtof->Write();
		if(histPartDtofBW) histPartDtofBW->Write();
		if(histPartDtofW) histPartDtofW->Write();
		if(histW) histW->Write();
		histofile->cd();
	}

  return kTRUE;

}

Bool_t HHypDeltaTofMeanWeightAlg::is_sim_ok()
{
	if(!simuflag) return true;

	Int_t nMaxPart;
	nMaxPart=mylist->getNpart();

  for (Int_t i = 0; i < nMaxPart; i++) {
		//loop over all particles
		if(!mylist->isValidPart(i)) continue;// unused particle

		HPidTrackCandSim *my_ep = (HPidTrackCandSim *)
				CatTrackCandSim->getObject(mylist->getIdxPidTrackCand(i));
		if(my_ep==NULL) return false;

		HPidGeantTrackSet *epGeantSet = my_ep->getGeantTrackSet();
		if(epGeantSet==NULL) return false;

		// look only for the 1st geant track, makes life more easy  [Ingo]
		Int_t ep_geant_track = epGeantSet->getGeantTrackID(0);

		HGeantKine *ep_geantkine =
			(HGeantKine *) epGeantSet->getGeantKine(ep_geant_track);
		if(ep_geantkine==NULL) return false;

		// Particle ID
		if(ep_geantkine->getID()!=mylist->getPid(i)) return false;
	}

	return true;
}
