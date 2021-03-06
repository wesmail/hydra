//*-- AUTHOR : I. Froehlich
//*-- Modified : 04/07/2005 by I. Froehlich
//*-- Modified : 15/nov/2005 by B. Spruck
//_HADES_CLASS_DESCRIPTION
////////////////////////////////////////////////////////////////////////
//
// HHypPPPipPimPi0Kine
//
// This is
// i)  an ALGORITHM which updates the momenta using the HypKine Object
// ii) a SELECTOR which removes background using the kine refit
//
////////////////////////////////////////////////////////////////////////

using namespace std;

#include "hhypPPPipPimPi0Kine.h"
#include "hypinfodef.h"
#include "hbasetrack.h"

ClassImp(HHypPPPipPimPi0Kine)

HHypPPPipPimPi0Kine::HHypPPPipPimPi0Kine(Char_t *name_i, Option_t par[])
:HHypBaseAlgorithm(name_i,par)
{
	fit=new HHypKineFitPPPipPimPi0();
}

HHypPPPipPimPi0Kine::~HHypPPPipPimPi0Kine()
{
	delete fit;
}

Double_t HHypPPPipPimPi0Kine::getMomError(Double_t theta, Double_t phi, Double_t mass, Double_t p,Int_t sec,Bool_t simu)
{
	Double_t res;
	res=HypMomErr.getMomError( theta,  phi,  mass,  p, sec, simu);

	if(kinetest) kinetest->Fill(p,res,mass,sec);
	return(res);
}


Bool_t HHypPPPipPimPi0Kine::DoTheRefit(TVector3 *momentum,Int_t idx_p1,Int_t idx_p2,Int_t idx_pim,Int_t idx_pip,Float_t &prechi2,Float_t &chi2,Float_t *pulls)
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

	HCategory *ptrackcandCat = gHades->getCurrentEvent()->getCategory(catPidTrackCand);

	HPidTrackCand *p1=(HPidTrackCand *) ptrackcandCat->getObject(idx_p1);;
	HPidTrackCand *p2=(HPidTrackCand *) ptrackcandCat->getObject(idx_p2);
	HPidTrackCand *pim=(HPidTrackCand *) ptrackcandCat->getObject(idx_pim);
	HPidTrackCand *pip=(HPidTrackCand *) ptrackcandCat->getObject(idx_pip);

	HBaseTrack *b;

 Bool_t simuflag;
	simuflag=gHades->getCurrentEvent()->getCategory(catGeantKine);

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// Input Error calculation and Scaling up/down is
	// still under investigation and code will change in future
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

/*	Float_t FakErrMom=0.76;
	Float_t FakErrTheta=2.0*0.76;
	Float_t FakErrPhi=2.0*0.76;
*/

	Float_t FakErrMom=1.255*0.6;
	Float_t FakErrTheta=1.11*0.6;
	Float_t FakErrPhi=1.11*0.6;
	Double_t momErr;

	//p1
	momErr=getMomError(input[1],input[2],14,input[0],p1->getHitData()->getSector(),simuflag);
	errInput[0]=FakErrMom*momErr;

	b=(HBaseTrack *)(p1->getTrackData()->getBaseTrack(p1->getTrackData()->getBestMomAlg()));
	errInput[1]=FakErrTheta*b->getErrTheta();
	errInput[2]=FakErrPhi*b->getErrPhi();

	//p2
	momErr=getMomError(input[4],input[5],14,input[3],p2->getHitData()->getSector(),simuflag);
	errInput[3]=FakErrMom*momErr;

	b=(HBaseTrack *)(p2->getTrackData()->getBaseTrack(p2->getTrackData()->getBestMomAlg()));
	errInput[4]=FakErrTheta*b->getErrTheta();
	errInput[5]=FakErrPhi*b->getErrPhi();

	// pi-
	momErr=getMomError(input[7],input[8],9,input[6],pim->getHitData()->getSector(),simuflag);
	errInput[6]=FakErrMom*momErr;

	b=(HBaseTrack *)(pim->getTrackData()->getBaseTrack(pim->getTrackData()->getBestMomAlg()));
	errInput[7]=FakErrTheta*b->getErrTheta();
	errInput[8]=FakErrPhi*b->getErrPhi();

	// pi+
	momErr=getMomError(input[10],input[11],8,input[9],pip->getHitData()->getSector(),simuflag);
	errInput[9]=FakErrMom*momErr;

	b=(HBaseTrack *)(pip->getTrackData()->getBaseTrack(pip->getTrackData()->getBestMomAlg()));
	errInput[10]=FakErrTheta*b->getErrTheta();
	errInput[11]=FakErrPhi*b->getErrPhi();


	fit->ResetOutput();
	fit->SetInput(input,errInput);

	Int_t result;
	result=fit->minuitFit();
	if(result==4){// or maybe !=0 ?
		// result==0 means O.K.
		// result==4 : fit not converging
		// thus IMO this comination should be rejected. (Bjoern)
		// Difference: Anars code is not checking for this!
		return kFALSE;
	}

	fit->GetOutput(output,errOutput);

	prechi2=0.;
	for(Int_t ii=0; ii<12; ii++)
	{
		Float_t cs;
		cs=(input[ii]-output[ii])/errInput[ii];
		prechi2+=cs*cs;
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

	chi2=0.;
	for(Int_t ii=0; ii<12; ii++)
	{
		Float_t cs;
/*		if(ii==3 || ii==6 || ii==9 || ii==0){
			cs=(1/input[ii]-1/output[ii])/errInput[ii]*input[ii]*input[ii];
		}else{*/
			cs=(input[ii]-output[ii])/errInput[ii];
//		}
		chi2+=cs*cs;
	}

	for(Int_t i=0; i<12; i++){
		// Real Pull is:
		Double_t nom;
		nom=errInput[i]*errInput[i]-derOut[i];// derOut already squared
		if(nom>0){
			pulls[i]=(input[i]-output[i])/TMath::Sqrt(nom);
		}else{
			pulls[i]=-99.0;// NAN not visible in histogramm
		}
	}

	return kTRUE;
}

Bool_t HHypPPPipPimPi0Kine::execute()
{
// 	if (mylist->getNpart() != 4) return kFALSE;
	//needs 4 particles

	TVector3 momentum[4];

	// The momenta will be modified... if i am the first this
	// is /in principle/ not needed but who knows, am I?
	mylist->initcopyMomentum();

	mylist->CombIteratorReset();
	while (mylist->CombIterator()) {
 		if (mylist->getNvalid() != 4) continue;// p p pi+ pi-

		// Getting the particles
		TLorentzVector proton1 = mylist->getTLorentzVector("p", 1);
		TLorentzVector proton2 = mylist->getTLorentzVector("p", 2);
		TLorentzVector pim = mylist->getTLorentzVector("pi-", 1);
		TLorentzVector pip = mylist->getTLorentzVector("pi+", 1);

		Int_t idxProton1=mylist->getIdxPidTrackCand("p", 1);
		Int_t idxProton2=mylist->getIdxPidTrackCand("p", 2);
		Int_t idxPiM=mylist->getIdxPidTrackCand("pi-", 1);
		Int_t idxPiP=mylist->getIdxPidTrackCand("pi+", 1);

		if (mylist->getIterStatus() != kTRUE) continue;

//		if (mylist->getIterStatus() == kTRUE) {

//	 	This was part of Anars code... but imo this should not be in here!
// 	calculating missing mass and cutting is not part of kinematic refit!!!
//			TLorentzVector miss4 = (*beam) - (proton1 + proton2 + pip + pim);
//			Double_t miss_4=miss4.Mag();
//			if(miss_4<1e6 /*40000.0*/)
			{
				Float_t prechi2, chi2;
				Float_t pulls[12];

				momentum[0]=proton1.Vect();
				momentum[1]=proton2.Vect();
				momentum[2]=pim.Vect();
				momentum[3]=pip.Vect();
				if(DoTheRefit(momentum,// order in momentum: proton, proton, pi-, pi+
						idxProton1,idxProton2,idxPiM,idxPiP,prechi2,chi2,pulls)){

					if (histofile) {
						Float_t tmp[55];
						Int_t ii;
						ii=0;
						tmp[ii++]=chi2;
						tmp[ii++]=prechi2;
						tmp[ii++]=7;// missing Pi0
						tmp[ii++]=14;// Proton
						tmp[ii++]=pulls[0];
						tmp[ii++]=pulls[1];
						tmp[ii++]=pulls[2];
						tmp[ii++]=14;// Proton
						tmp[ii++]=pulls[3];
						tmp[ii++]=pulls[4];
						tmp[ii++]=pulls[5];
						tmp[ii++]=8;// Pi+
						tmp[ii++]=pulls[6];
						tmp[ii++]=pulls[7];
						tmp[ii++]=pulls[8];
						tmp[ii++]=9;// Pi-
						tmp[ii++]=pulls[9];
						tmp[ii++]=pulls[10];
						tmp[ii++]=pulls[11];
						tmp[ii++]=proton1.Vect().Mag();
						tmp[ii++]=proton2.Vect().Mag();
						tmp[ii++]=pim.Vect().Mag();
						tmp[ii++]=pip.Vect().Mag();
						tmp[ii++]=proton1.Vect().Phi();
						tmp[ii++]=proton2.Vect().Phi();
						tmp[ii++]=pim.Vect().Phi();
						tmp[ii++]=pip.Vect().Phi();
						tmp[ii++]=proton1.Vect().Theta();
						tmp[ii++]=proton2.Vect().Theta();
						tmp[ii++]=pim.Vect().Theta();
						tmp[ii++]=pip.Vect().Theta();

						for(Int_t i=0; i<12; i++){
							tmp[ii++]=errInput[i];
						}
						qa->Fill(tmp);
					}

					proton1.SetVect(momentum[0]);
					proton2.SetVect(momentum[1]);
					pim.SetVect(momentum[2]);
					pip.SetVect(momentum[3]);

					mylist->setMomentum("p", 1, momentum[0]);
					mylist->setMomentum("p", 2, momentum[1]);
					mylist->setMomentum("pi-", 1, momentum[2]);
					mylist->setMomentum("pi+", 1, momentum[3]);
					if (mylist->getIterStatus() == kFALSE) {
						cout << "!!!!!error HHypPPPipPimPi0Kine execute mylist->getIterStatus() == kFALSE!!!!!! " << endl;
						exit(2);
					}

					mylist->resetProbAlg(TMath::Prob(chi2,1));
					//store more data here....
					mylist->setUserValue(KINEFIT_CHI2, chi2);
					mylist->setUserValue(KINEFIT_PRECHI2, prechi2);
				}else{
					// Fit failed -> Remove Combination
//					cout << "HHypPPPipPimPi0Kine DoTheRefit() == kFALSE" <<endl;
					mylist->removeComb();
				}
			}
// 		}else{
// 			cerr << algoName << " got no TLorentzVector " << endl;
// 		}
	}                             //END Iterator

	if (exitIdx > -1)
		return kTRUE;
	return kFALSE;
}

Bool_t HHypPPPipPimPi0Kine::init()
{
	// need to get name from channel
	TString output(channel->Get(exitList));

	if (histofile){
		qa = new TNtuple(output + TString("_kine_debug"), "Kine Debug ntuple",
			"chi2:prechi2:miss_pid"
			":pid1:pull_p1:pull_the1:pull_phi1"
			":pid2:pull_p2:pull_the2:pull_phi2"
			":pid3:pull_p3:pull_the3:pull_phi3"
			":pid4:pull_p4:pull_the4:pull_phi4"
			":old_p_p1:old_p_p2:old_p_pim:old_p_pip"
			":old_phi_p1:old_phi_p2:old_phi_pim:old_phi_pip"
			":old_the_p1:old_the_p2:old_the_pim:old_the_pip"
			":errin_p_p1:errin_the_p1:errin_phi_p1"
			":errin_p_p2:errin_the_p2:errin_phi_p2"
			":errin_p_pim:errin_the_pim:errin_phi_pim"
			":errin_p_pip:errin_the_pip:errin_phi_pip"
		);
	}

	kinetest = new TNtuple(output + TString("_kine_errors"),
		"Kine DebugErrors ntuple","p:dp:m:sec");

/*
	pParams->registerCut("MomentumError_SEC_0");
	pParams->registerCut("MomentumError_SEC_1");
	pParams->registerCut("MomentumError_SEC_2");
	pParams->registerCut("MomentumError_SEC_3");
	pParams->registerCut("MomentumError_SEC_4");
	pParams->registerCut("MomentumError_SEC_5");
*/
	return kTRUE;
}

Bool_t HHypPPPipPimPi0Kine::reinit()
{
	//Here, we set/reset the Momentum Error
	//Resolution could be run-dependent!
/*
 Bool_t simuflag;
	simuflag=gHades->getCurrentEvent()->getCategory(catGeantKine);

//	if (!pParams->getCut("MomentumError_SEC_0", mom_err_sec_0))
	{
		if(simuflag) mom_err_sec_0=0.02*1.26; else mom_err_sec_0=0.06;
		std::cout << "HHypPPPipPimPi0Kine::init: MomentumError_SEC_0 not found" << std::endl;
		std::cout << "using hardcoded MomentumError_SEC_0" << std::endl;
	}

//	if (!pParams->getCut("MomentumError_SEC_1", mom_err_sec_1))
	{
		if(simuflag) mom_err_sec_1=0.02*1.26; else mom_err_sec_1=0.046;
		std::cout << "HHypPPPipPimPi0Kine::init: MomentumError_SEC_1 not found" << std::endl;
		std::cout << "using hardcoded MomentumError_SEC_1" << std::endl;
	}

//	if (!pParams->getCut("MomentumError_SEC_2", mom_err_sec_2))
	{
		if(simuflag) mom_err_sec_2=0.025*1.26; else mom_err_sec_2=0.17;
		std::cout << "HHypPPPipPimPi0Kine::init: MomentumError_SEC_2 not found" << std::endl;
		std::cout << "using hardcoded MomentumError_SEC_2" << std::endl;
	}

//	if (!pParams->getCut("MomentumError_SEC_3", mom_err_sec_3))
	{
		if(simuflag) mom_err_sec_3=0.02*1.26; else mom_err_sec_3=0.044;
		std::cout << "HHypPPPipPimPi0Kine::init: MomentumError_SEC_3 not found" << std::endl;
		std::cout << "using hardcoded MomentumError_SEC_3" << std::endl;
	}

//	if (!pParams->getCut("MomentumError_SEC_4", mom_err_sec_4))
	{
		if(simuflag) mom_err_sec_4=0.02*1.26; else mom_err_sec_4=0.04;
		std::cout << "HHypPPPipPimPi0Kine::init: MomentumError_SEC_4 not found" << std::endl;
		std::cout << "using hardcoded MomentumError_SEC_4" << std::endl;
	}

//	if (!pParams->getCut("MomentumError_SEC_5", mom_err_sec_5))
	{
		if(simuflag) mom_err_sec_5=0.025*1.26; else mom_err_sec_5=0.071;
		std::cout << "HHypPPPipPimPi0Kine::init: MomentumError_SEC_5 not found" << std::endl;
		std::cout << "using hardcoded MomentumError_SEC_5" << std::endl;
	}

	std::cout << "MomentumError_SEC_0 is: " << mom_err_sec_0 << std::endl;
	std::cout << "MomentumError_SEC_1 is: " << mom_err_sec_1 << std::endl;
	std::cout << "MomentumError_SEC_2 is: " << mom_err_sec_2 << std::endl;
	std::cout << "MomentumError_SEC_3 is: " << mom_err_sec_3 << std::endl;
	std::cout << "MomentumError_SEC_4 is: " << mom_err_sec_4 << std::endl;
	std::cout << "MomentumError_SEC_5 is: " << mom_err_sec_5 << std::endl;
*/
	return kTRUE;
}

Bool_t HHypPPPipPimPi0Kine::finalize()
{
	if (histofile) qa->Write();

	kinetest->Write();
	return kTRUE;
}
