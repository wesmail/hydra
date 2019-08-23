//*-- Author : B. Spruck
//*-- Modified : 20 Sep 2006
////////////////////////////////////////////////////////////////////////
//
// HHypCorrectEloss
//
// Corrects Momentum for Energy loss according to Olga's
// code in MdcG (uptodate Hydra version needed)
//
////////////////////////////////////////////////////////////////////////

using namespace std;

#include <stdlib.h>
#include <iostream>
#include "hhypCorrectEloss.h"
#include "hruntimedb.h"
#include "hypinfodef.h"

ClassImp(HHypCorrectEloss)

HHypCorrectEloss::HHypCorrectEloss(Char_t *name_i, Option_t par[])
:HHypBaseAlgorithm(name_i,par)
{
  enLossCorr = 0;
  // parameters loaded in init
}

HHypCorrectEloss::~HHypCorrectEloss()
{
  if(enLossCorr) delete enLossCorr;
}

Bool_t HHypCorrectEloss::execute()
{
	// Resetting the list and start looping over the combinations
	// Loop is only done over the VALID combinations
	
	Float_t temp[8];
	TLorentzVector l;
	TVector3 mom;
	Double_t m;

	// The momenta will be modified... if i am the first this
	// is /in principle/ not needed but who knows, am I?
	mylist->initcopyMomentum();
		
	mylist->CombIteratorReset();
	while (mylist->CombIterator()) {
		// Getting the particles

		Int_t prot_nr;
		prot_nr=0;

		Bool_t removeComb;
		removeComb=false;

		if(mylist->getNpart()!=4){
			cout << "HHypCorrectEloss::execute() only for 4part yet!!!"<<endl;
			exit(0);
		}

		for(Int_t i=0; i<8; i++) temp[i]=0;

		for(Int_t i=0; i<mylist->getNpart(); i++){
			l=mylist->getTLorentzVector(i);
			mom=l.Vect();
			m=mom.Mag();

		 Int_t p;
			p=mylist->getPid(i);

			if(histofile){
				switch(p){// fill array for diagnostic ntuple
					case 14:// p
						if(prot_nr){
							temp[2]=m;
						}else{
							temp[0]=m;
						}
						break;
					case 3:// e-
					case 9:// pi-
						temp[4]=m;
						break;
					case 2:// e+
					case 8:// pi+
						temp[6]=m;
						break;
					default:
						cout << "Pid: " << p << " not valid" << endl;
						exit(0);
				}
			}

			Float_t theta = atan2(sqrt(mom.X()*mom.X()+mom.Y()*mom.Y()),mom.Z())*TMath::RadToDeg();
			m = enLossCorr->getCorrMom(p, m, theta);

			if(histofile){
				switch(p){// fill array for diagnostic ntuple
					case 14:// p
						if(prot_nr){
							temp[3]=m;
						}else{
							temp[1]=m;
						}
						prot_nr++;
						break;
					case 3:// e-
					case 9:// pi-
						temp[5]=m;
						break;
					case 2:// e+
					case 8:// pi+
						temp[7]=m;
						break;
				}
			}
			
			mom.SetMag(m);
			mylist->setMomentum(i,mom);
		}
		
		if(!removeComb && histofile){
			qa->Fill(temp);
		}

		if (removeComb) {
			mylist->removeComb();
		}
 
	}                             //END Iterator

	if (exitIdx > -1)
		return kTRUE;
	return kFALSE;
}

Bool_t HHypCorrectEloss::init()
{
	// need to get name from channel
	TString input(channel->Get(initList));
	
	if (histofile){
		qa = new TNtuple(input + TString("_eloss_debug"),
			"Eloss Debug ntuple","p1_old:p1_new:p2_old:p2_new:pem_old:pem_new:pep_old:pep_new");
	}

	if((enLossCorr = (HEnergyLossCorrPar *)gHades->getRuntimeDb()
			->getContainer("EnergyLossCorrPar")) == NULL)
	{
		enLossCorr=new HEnergyLossCorrPar;
		enLossCorr->setDefaultPar("jan04");// or "nov02", or "aug04"
		Info("HHypCorrectEloss::init", "Cannot get parameters %s thus using default (jan04)", "EnergyLossCorrPar");
	}

	// Check for container content ... 
	if( enLossCorr->getInputVersion(1)<0 && enLossCorr->getInputVersion(2)<0){
		// Container was empty
		enLossCorr->setDefaultPar("jan04");// or "nov02", or "aug04"
		Info("HHypCorrectEloss::init", "Container %s empty, using default (jan04)", "EnergyLossCorrPar");
	}

	return kTRUE;
}

Bool_t HHypCorrectEloss::reinit()
{
	// In principle parameters should be checked for changes here ...
	return kTRUE;
}

Bool_t HHypCorrectEloss::finalize()
{
	if (histofile) qa->Write();
	return kTRUE;
}
