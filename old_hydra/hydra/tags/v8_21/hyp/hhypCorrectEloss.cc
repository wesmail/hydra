//*-- Author : B. Spruck
//*-- Modified : 20 Sep 2006
//*-- Modified : 05 Mar 2009
////////////////////////////////////////////////////////////////////////
//
// HHypCorrectEloss
//
// Corrects Momentum for Energy loss according to Olga's
// code in MdcG (uptodate Hydra version needed)
//
// Remark: combinations, where at least one theta or momentum is
// "invalid" or out of range, are rejected. This is tested by checking
// result of getDeltaMom() == 0.0
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
	
	Float_t temp[9];// for histofile
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

//		if(histofile &&mylist->getNpart()!=4){
//			cout << "HHypCorrectEloss::execute() writing histofile only for 4part yet!!!"<<endl;
//			exit(0);
//		}

		// Note: The histofile code was written for pp e+/pi+ e-/pi- and
		// will therefore only print two proton and one e+/pi+ e-/pi- per event
		// this might be changed in future if needed
		for(Int_t i=0; i<9; i++) temp[i]=0;

		for(Int_t i=0; i<mylist->getNpart(); i++){
         if( !mylist->isValidPart(i)) continue;// entry not valid
			l=mylist->getTLorentzVector(i);
			mom=l.Vect();
			m=mom.Mag();

			Int_t pID;
			pID=mylist->getPid(i);

			if(histofile){
				switch(pID){// fill array for diagnostic ntuple
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
//					case 0:
//						cerr << "Fatal Error in HHypCorrectEloss: Pid " << pID << " not valid" << endl;
//						exit(0);
					default:
						break;
				}
			}

         {
            Float_t theta = atan2(sqrt(mom.X()*mom.X()+mom.Y()*mom.Y()),mom.Z())*TMath::RadToDeg();
            // if theta or momentum are out of range (for momentum this means, particles of
            // that momentum can in principle not be registered) the result of getDeltaMom
            // is EXACTLY 0.0 ==>> This is the only possibility (up to now) to check
            // if any correction was done at all.
            Double_t dp;
            dp = enLossCorr->getDeltaMom(pID, m, theta);// 0.0 means out of range!!!
            if( dp==0.0) removeComb=true;
            //Float_t getCorrMom(Int_t pId, Float_t mom, Float_t theta) {return mom+getDeltaMom(pId,mom,theta);}
            // m = enLossCorr->getCorrMom(pID, m, theta);
            m += dp;// new momentum according to definition of getCorrMom()
         }
         
			if(histofile){
				switch(pID){// fill array for diagnostic ntuple
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
		
		if(histofile){
         temp[8]=removeComb;
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
			"Eloss Debug ntuple","p1_old:p1_new:p2_old:p2_new:pem_old:pem_new:pep_old:pep_new:removed");
	}

//	if((enLossCorr = (HEnergyLossCorrPar *)gHades->getRuntimeDb()
//			->getContainer("EnergyLossCorrPar")) == NULL)
//	{
		enLossCorr=new HEnergyLossCorrPar;
		enLossCorr->setDefaultPar("jan04");// or "nov02", or "aug04"
		Info("HHypCorrectEloss::init", "Cannot get parameters %s thus using default (jan04)", "EnergyLossCorrPar");
//	}

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
