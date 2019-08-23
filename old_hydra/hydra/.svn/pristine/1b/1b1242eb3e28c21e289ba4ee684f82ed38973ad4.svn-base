//*-- AUTHOR : B. Spruck
//*-- Modified : 26/10/2005 by B. Spruck
//*-- Modified : 15/nov/2005 by B. Spruck
//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////
//
// HHypPPEpEmGammaKine
//
// This is
// i)  an ALGORITHM which updates the momenta using the HypKine Object
// ii) a SELECTOR which removes background using the kine refit
//
////////////////////////////////////////////////////////////////////////

using namespace std;

#include <stdlib.h>
#include <iostream>
#include "hhypPPEpEmGammaKine.h"
#include "hypinfodef.h"
#include "hbasetrack.h"

ClassImp(HHypPPEpEmGammaKine)

HHypPPEpEmGammaKine::HHypPPEpEmGammaKine(char *name_i, Option_t par[])
:HHypBaseAlgorithm(name_i,par)
{
	fit=new HHypKineFitPPEpEmGamma();
}

HHypPPEpEmGammaKine::~HHypPPEpEmGammaKine()
{
	delete fit;
}

Float_t HHypPPEpEmGammaKine::getMomErr(Int_t sec)
{
	switch(sec){
		case 0: return(mom_err_sec_0);
		case 1: return(mom_err_sec_1);
		case 2: return(mom_err_sec_2);
		case 3: return(mom_err_sec_3);
		case 4: return(mom_err_sec_4);
		case 5: return(mom_err_sec_5);
		default:
			Error("getMomErr","Sector<0 || Sector>5");
			return(0.015);
	}
}

Bool_t HHypPPEpEmGammaKine::DoTheRefit(TVector3 *momentum,Int_t idx_p1,Int_t idx_p2,Int_t idx_em,Int_t idx_ep,Float_t &chi2,Float_t &chi24, Float_t *pulls)
{
	input[0]=momentum[0].Mag();
	input[1]=momentum[0].Theta();
	input[2]=momentum[0].Phi();

	input[3]=momentum[1].Mag();
	input[4]=momentum[1].Theta();
	input[5]=momentum[1].Phi();

	input[6]=momentum[2].Mag();
	input[7]=momentum[2].Theta();
	input[8]=momentum[2].Phi();

	input[9]=momentum[3].Mag();
	input[10]=momentum[3].Theta();
	input[11]=momentum[3].Phi();

	HCategory *pidpartCat = gHades->getCurrentEvent()->getCategory(catPidPart);

	HPidParticle *p1=(HPidParticle *) pidpartCat->getObject(idx_p1);;
	HPidParticle *p2=(HPidParticle *) pidpartCat->getObject(idx_p2);
	HPidParticle *em=(HPidParticle *) pidpartCat->getObject(idx_em);
	HPidParticle *ep=(HPidParticle *) pidpartCat->getObject(idx_ep);

	HBaseTrack *b;
	
	Float_t FakErrMom=0.76;
	Float_t FakErrTheta=2.0*0.76;
	Float_t FakErrPhi=2.0*0.76;

	//p1
	errInput[0]=input[0]*FakErrMom*getMomErr(p1->sector());

	b=(HBaseTrack *)(p1->getTrackData()->getBaseTrack(p1->getMomAlg()));
	errInput[1]=FakErrTheta*b->getErrTheta();
	errInput[2]=FakErrPhi*b->getErrPhi();

	//p2
	errInput[3]=input[3]*FakErrMom*getMomErr(p2->sector());

	b=(HBaseTrack *)(p2->getTrackData()->getBaseTrack(p2->getMomAlg()));
	errInput[4]=FakErrTheta*b->getErrTheta();
	errInput[5]=FakErrPhi*b->getErrPhi();

	// e-
	errInput[6]=input[6]*FakErrMom*getMomErr(em->sector());

	b=(HBaseTrack *)(em->getTrackData()->getBaseTrack(em->getMomAlg()));
	errInput[7]=FakErrTheta*b->getErrTheta();
	errInput[8]=FakErrPhi*b->getErrPhi();

	// e+
	errInput[9]=input[9]*FakErrMom*getMomErr(ep->sector());

	b=(HBaseTrack *)(ep->getTrackData()->getBaseTrack(ep->getMomAlg()));
	errInput[10]=FakErrTheta*b->getErrTheta();
	errInput[11]=FakErrPhi*b->getErrPhi();

	fit->ResetOutput();
	fit->SetInput(input,errInput);

	Int_t result;
	result=fit->minuitFit();
	if(result==4){// or maybe !=0 ?
		// result==0 means O.K.
		// result==4 : fit not converging
		// thus IMO this combination should be rejected. (Bjoern)
		// Difference: Anars code is not checking for this!
		return kFALSE;
	}

	fit->GetOutput(output,errOutput);

	chi2=0.;
	for(Int_t ii=0; ii<12; ii++)
	{
		Float_t cs;
		cs=(input[ii]-output[ii])/errInput[ii];
		chi2+=cs*cs;
	}

	fit->secondIter(input,output,errInput,output2,derOut);

	for(Int_t i=0; i<12; i++)
	{
		output[i]=output2[i];
	}

	momentum[0].SetXYZ(output[0]*sin(output[1])*cos(output[2]),output[0]*sin(output[1])*sin(output[2]),output[0]*cos(output[1]));
	momentum[1].SetXYZ(output[3]*sin(output[4])*cos(output[5]),output[3]*sin(output[4])*sin(output[5]),output[3]*cos(output[4]));
	momentum[2].SetXYZ(output[6]*sin(output[7])*cos(output[8]),output[6]*sin(output[7])*sin(output[8]),output[6]*cos(output[7]));
	momentum[3].SetXYZ(output[9]*sin(output[10])*cos(output[11]),output[9]*sin(output[10])*sin(output[11]),output[9]*cos(output[10]));

	chi24=0.;
	for(Int_t ii=0; ii<12; ii++)
	{
		Float_t cs;
/*		if(ii==3 || ii==6 || ii==9 || ii==0){
			cs=(1/input[ii]-1/output[ii])*(1/input[ii]-1/output[ii])/errInput[ii]/errInput[ii]*input[ii]*input[ii]*input[ii]*input[ii];
		}else{*/
			cs=(input[ii]-output[ii])/errInput[ii];
//		}
		chi24+=cs*cs;
	}

	for(Int_t i=0; i<12; i++){
		// Real Pull is:
		Double_t nom;
		nom=errInput[i]*errInput[i]-derOut[i];// derOut already squared
		if(nom>0){
			pulls[i]=(input[i]-output[i])/TMath::Sqrt(nom);
		}else{
			pulls[i]=-99.0;// NAN would not be visible in histogramm
		}
	}

	return kTRUE;
}

Bool_t HHypPPEpEmGammaKine::execute()
{
	if (mylist->getNpart() != 4)
		return kFALSE;
	//needs 4 particles

	TVector3 momentum[4];

	mylist->CombIteratorReset();
	while (mylist->CombIterator()) {

		// Getting the particles
		TLorentzVector proton1 = mylist->getTLorentzVector("p", 1);
		TLorentzVector proton2 = mylist->getTLorentzVector("p", 2);
		TLorentzVector em = mylist->getTLorentzVector("e-", 1);
		TLorentzVector ep = mylist->getTLorentzVector("e+", 1);

		Int_t idxProton1=mylist->getIdxPidPart("p", 1);
		Int_t idxProton2=mylist->getIdxPidPart("p", 2);
		Int_t idxElectron=mylist->getIdxPidPart("e-", 1);
		Int_t idxPositron=mylist->getIdxPidPart("e+", 1);

		if (mylist->getIterStatus() == kTRUE) {

//			if(miss_4<1e6 /*40000.0*/)// some cut copied from Refitter - stupid!!!!
			{
				Float_t chi2, chi24;
				Float_t pulls[32];// war 12
				momentum[0]=proton1.Vect();
				momentum[1]=proton2.Vect();
				momentum[2]=em.Vect();
				momentum[3]=ep.Vect();
				if(DoTheRefit(momentum,// order in momentum: proton, proton, e-, e+
						idxProton1,idxProton2,idxElectron,idxPositron,chi2,chi24,pulls)){

					if (histofile) {
						Float_t tmp[50];
						Int_t ii;
						ii=0;
						tmp[ii++]=chi2;
						tmp[ii++]=chi24;
						tmp[ii++]=pulls[0];
						tmp[ii++]=pulls[1];
						tmp[ii++]=pulls[2];
						tmp[ii++]=pulls[3];
						tmp[ii++]=pulls[4];
						tmp[ii++]=pulls[5];
						tmp[ii++]=pulls[6];
						tmp[ii++]=pulls[7];
						tmp[ii++]=pulls[8];
						tmp[ii++]=pulls[9];
						tmp[ii++]=pulls[10];
						tmp[ii++]=pulls[11];
						tmp[ii++]=proton1.Vect().Mag();
						tmp[ii++]=proton2.Vect().Mag();
						tmp[ii++]=em.Vect().Mag();
						tmp[ii++]=ep.Vect().Mag();
						tmp[ii++]=proton1.Vect().Phi();
						tmp[ii++]=proton2.Vect().Phi();
						tmp[ii++]=em.Vect().Phi();
						tmp[ii++]=ep.Vect().Phi();
						tmp[ii++]=proton1.Vect().Theta();
						tmp[ii++]=proton2.Vect().Theta();
						tmp[ii++]=em.Vect().Theta();
						tmp[ii++]=ep.Vect().Theta();

						for(Int_t i=0; i<12; i++){
							tmp[ii++]=errInput[i];
						}
						qa->Fill(tmp);
					}

					proton1.SetVect(momentum[0]);
					proton2.SetVect(momentum[1]);
					em.SetVect(momentum[2]);
					ep.SetVect(momentum[3]);

					mylist->setMomentum("p", 1, momentum[0]);
					mylist->setMomentum("p", 2, momentum[1]);
					mylist->setMomentum("e-", 1, momentum[2]);
					mylist->setMomentum("e+", 1, momentum[3]);
					if (mylist->getIterStatus() == kFALSE) {
						cout << "!!!!!error HHypPPEpEmGammaKine execute mylist->getIterStatus() == kFALSE!!!!!! " << endl;
						exit(2);
					}
					mylist->resetProbAlg( (Float_t) chi2);
					mylist->setUserValue(KINEFIT_CHI2, chi2);
					mylist->setUserValue(KINEFIT_CHI24, chi24);
				}else{
					// Fit failed -> Remove Combination
					cout << "HHypPPEpEmGammaKine DoTheRefit() == kFALSE"  << endl;
					mylist->removeComb();
				}
			}
		}else{
			cerr << algoName << " got no TLorentzVector " << endl;
		}
	}                             //END Iterator

	if (exitIdx > -1)
		return kTRUE;
	return kFALSE;
}

Bool_t HHypPPEpEmGammaKine::init()
{
	// need to get name from channel
	TString input(channel->Get(initList));

	if (histofile){
		qa = new TNtuple(input + TString("_kine_debug"), "Kine Debug ntuple",
			"chi2:chi24"
			":pull_p_p1:pull_the_p1:pull_phi_p1"
			":pull_p_p2:pull_the_p2:pull_phi_p2"
			":pull_p_em:pull_the_em:pull_phi_em"
			":pull_p_ep:pull_the_ep:pull_phi_ep"
			":old_p_p1:old_p_p2:old_p_em:old_p_ep"
			":old_phi_p1:old_phi_p2:old_phi_em:old_phi_ep"
			":old_the_p1:old_the_p2:old_the_em:old_the_ep"
			":errin_p_p1:errin_the_p1:errin_phi_p1"
			":errin_p_p2:errin_the_p2:errin_phi_p2"
			":errin_p_em:errin_the_em:errin_phi_em"
			":errin_p_ep:errin_the_ep:errin_phi_ep"
		);
	}

	pParams->registerCut("MomentumError_SEC_0");
	pParams->registerCut("MomentumError_SEC_1");
	pParams->registerCut("MomentumError_SEC_2");
	pParams->registerCut("MomentumError_SEC_3");
	pParams->registerCut("MomentumError_SEC_4");
	pParams->registerCut("MomentumError_SEC_5");

	return kTRUE;
}

Bool_t HHypPPEpEmGammaKine::reinit()
{
	//Here, we set/reset the Momentum Error
	//Resolution could be run-dependent!

	bool simuflag;
	simuflag=gHades->getCurrentEvent()->getCategory(catGeantKine);

//	if (!pParams->getCut("MomentumError_SEC_0", mom_err_sec_0))
	{
		if(simuflag) mom_err_sec_0=0.02; else mom_err_sec_0=0.06*0.82;
		std::cout << "HHypPPEpEmGammaKine::init: MomentumError_SEC_0 not found" << std::endl;
		std::cout << "using hardcoded MomentumError_SEC_0" << std::endl;
	}

//	if (!pParams->getCut("MomentumError_SEC_1", mom_err_sec_1))
	{
		if(simuflag) mom_err_sec_1=0.02; else mom_err_sec_1=0.046*0.82;
		std::cout << "HHypPPEpEmGammaKine::init: MomentumError_SEC_1 not found" << std::endl;
		std::cout << "using hardcoded MomentumError_SEC_1" << std::endl;
	}

//	if (!pParams->getCut("MomentumError_SEC_2", mom_err_sec_2))
	{
		if(simuflag) mom_err_sec_2=0.025; else mom_err_sec_2=0.17*0.82;
		std::cout << "HHypPPEpEmGammaKine::init: MomentumError_SEC_2 not found" << std::endl;
		std::cout << "using hardcoded MomentumError_SEC_2" << std::endl;
	}

//	if (!pParams->getCut("MomentumError_SEC_3", mom_err_sec_3))
	{
		if(simuflag) mom_err_sec_3=0.02; else mom_err_sec_3=0.044*0.82;
		std::cout << "HHypPPEpEmGammaKine::init: MomentumError_SEC_3 not found" << std::endl;
		std::cout << "using hardcoded MomentumError_SEC_3" << std::endl;
	}

//	if (!pParams->getCut("MomentumError_SEC_4", mom_err_sec_4))
	{
		if(simuflag) mom_err_sec_4=0.02; else mom_err_sec_4=0.04*0.82;
		std::cout << "HHypPPEpEmGammaKine::init: MomentumError_SEC_4 not found" << std::endl;
		std::cout << "using hardcoded MomentumError_SEC_4" << std::endl;
	}

//	if (!pParams->getCut("MomentumError_SEC_5", mom_err_sec_5))
	{
		if(simuflag) mom_err_sec_5=0.025; else mom_err_sec_5=0.071*0.82;
		std::cout << "HHypPPEpEmGammaKine::init: MomentumError_SEC_5 not found" << std::endl;
		std::cout << "using hardcoded MomentumError_SEC_5" << std::endl;
	}

	std::cout << "MomentumError_SEC_0 is: " << mom_err_sec_0 << std::endl;
	std::cout << "MomentumError_SEC_1 is: " << mom_err_sec_1 << std::endl;
	std::cout << "MomentumError_SEC_2 is: " << mom_err_sec_2 << std::endl;
	std::cout << "MomentumError_SEC_3 is: " << mom_err_sec_3 << std::endl;
	std::cout << "MomentumError_SEC_4 is: " << mom_err_sec_4 << std::endl;
	std::cout << "MomentumError_SEC_5 is: " << mom_err_sec_5 << std::endl;

	return kTRUE;
}

Bool_t HHypPPEpEmGammaKine::finalize()
{
	if (histofile) qa->Write();

	return kTRUE;
}
