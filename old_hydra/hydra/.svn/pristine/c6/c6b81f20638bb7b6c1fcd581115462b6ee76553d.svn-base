//*-- AUTHOR : I. Froehlich
//*-- Modified : 04/07/2005 by I. Froehlich
//*-- Modified : 15/nov/2005 by B. Spruck
//*-- Modified : 20/Jan/2006 by T. Perez
//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////
//
// HHypPPPipPimGammaKine
//
// This is 
// i)  an ALGORITHM which updates the momenta using the HypKine Object
// ii) a SELECTOR which removes background using the kine refit
//
////////////////////////////////////////////////////////////////////////

#include "hhypPPPipPimGammaKine.h"

using namespace std;

ClassImp(HHypPPPipPimGammaKine)

HHypPPPipPimGammaKine::HHypPPPipPimGammaKine(Char_t *name_i, Option_t par[])
  :HHypBaseAlgorithm(name_i,par)
{
	fit=new HHypKineFitPPPipPimGamma();
}

HHypPPPipPimGammaKine::~HHypPPPipPimGammaKine()
{
	delete fit;
}

Double_t HHypPPPipPimGammaKine::getMomErr(Int_t sec,Double_t P,Int_t pid)
{
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// Input Error calculation and Scaling up/down is
	// still under investigation and code will change in future
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	
	Double_t dP, M;

	M=HPidPhysicsConstants::mass(pid);// get mass in MeV

		Double_t c1, c2;
		switch(sec){
			case 0:
				c1=3.197e-5;
				c2=0.0561;
				break;
			case 1:
				c1=3.317e-5;
				c2=0.02067;
				break;
			case 2:
				c1=7.572e-5;
				c2=0.1254;
				break;
			case 3:
				c1=2.383e-5;
				c2=0.02371;
				break;
			case 4:
				c1=1.602e-5;
				c2=0.02777;
				break;
			case 5:
				c1=3.556e-5;
				c2=0.06099;
				break;
			default:
				Error("getMomErr","Sector<0 || Sector>5");
				c1=0;
				c2=0;
				break;
		}

		// Formula is (dP/P)²=(c1*p)²+(c2/beta)²
		Double_t P2=P*P;
		dP=TMath::Sqrt(c1*c1*P2*P2 + c2*c2*(P2+M*M));
		// cout << "dp/P= " << dP/P << " Sec: "<<sec<<" dp= "<< dP<<" P= " <<P <<" M= " << M << endl;
	kinetest->Fill(P,dP,M,sec);
	return(dP);
}

Bool_t HHypPPPipPimGammaKine::DoTheRefit(TVector3 *momentum,Int_t idx_p1,Int_t idx_p2,Int_t idx_pim,Int_t idx_pip,Float_t &chi2,Float_t &chi24,Float_t *pulls)
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

	HPidParticle *p1=(HPidParticle *) pidpartCat->getObject(idx_p1);
	HPidParticle *p2=(HPidParticle *) pidpartCat->getObject(idx_p2);
	HPidParticle *pim=(HPidParticle *) pidpartCat->getObject(idx_pim);
	HPidParticle *pip=(HPidParticle *) pidpartCat->getObject(idx_pip);
	
	HBaseTrack *b;

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// Input Error calculation and Scaling up/down is
	// still under investigation and code will change in future
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	
/*	Float_t FakErrMom=2.2;
	Float_t FakErrTheta=2.2;
	Float_t FakErrPhi=2.2;*/
	Float_t FakErrMom=0.50;
	Float_t FakErrTheta=1.0;//0.50;
	Float_t FakErrPhi=1.0;//0.50;

	//p1
	errInput[0]=FakErrMom*getMomErr(p1->sector(),input[0],14);// Proton
	
	b=(HBaseTrack *)(p1->getTrackData()->getBaseTrack(p1->getMomAlg()));
	errInput[1]=FakErrTheta*b->getErrTheta();
	errInput[2]=FakErrPhi*b->getErrPhi();
	
	//p2
	errInput[3]=FakErrMom*getMomErr(p2->sector(),input[3],14);// Proton
	
	b=(HBaseTrack *)(p2->getTrackData()->getBaseTrack(p2->getMomAlg()));
	errInput[4]=FakErrTheta*b->getErrTheta();
	errInput[5]=FakErrPhi*b->getErrPhi();
	
	// pi-
	errInput[6]=FakErrMom*getMomErr(pim->sector(),input[6],9);// Pi-
	
	b=(HBaseTrack *)(pim->getTrackData()->getBaseTrack(pim->getMomAlg()));
	errInput[7]=FakErrTheta*b->getErrTheta();
	errInput[8]=FakErrPhi*b->getErrPhi();
	
	// pi+
	errInput[9]=FakErrMom*getMomErr(pip->sector(),input[9],8);// Pi+
	
	b=(HBaseTrack *)(pip->getTrackData()->getBaseTrack(pip->getMomAlg()));
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
		chi2+=(input[ii]-output[ii])*(input[ii]-output[ii])/errInput[ii]/errInput[ii];
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
		if(ii==3 || ii==6 || ii==9 || ii==0){
			chi24+=(1/input[ii]-1/output[ii])*(1/input[ii]-1/output[ii])/errInput[ii]/errInput[ii]*input[ii]*input[ii]*input[ii]*input[ii]; 
		}else{
			chi24+=(input[ii]-output[ii])*(input[ii]-output[ii])/errInput[ii]/errInput[ii];
		}
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

Bool_t HHypPPPipPimGammaKine::execute()
{
	if (mylist->getNpart() != 4)
		return kFALSE;
	//needs 4 particles

	TVector3 momentum[4];
	
	// The momenta will be modified... if i am the first this
	// is /in principle/ not needed but who knows, am I?
	mylist->initcopyMomentum();
	
	mylist->CombIteratorReset();
	while (mylist->CombIterator()) {

		// Getting the particles 
		TLorentzVector proton1 = mylist->getTLorentzVector("p", 1);
		TLorentzVector proton2 = mylist->getTLorentzVector("p", 2);
		TLorentzVector pim = mylist->getTLorentzVector("pi-", 1);
		TLorentzVector pip = mylist->getTLorentzVector("pi+", 1);
		
		Int_t idxProton1=mylist->getIdxPidPart("p", 1);
		Int_t idxProton2=mylist->getIdxPidPart("p", 2);
		Int_t idxPiM=mylist->getIdxPidPart("pi-", 1);
		Int_t idxPiP=mylist->getIdxPidPart("pi+", 1);
	
		if (mylist->getIterStatus() == kTRUE) {
	
			// calculating missing mass
//			TLorentzVector miss4 = (*beam) - (proton1 + proton2 + pip + pim);
		
//			Double_t miss_4=miss4.Mag();
//			if(miss_4<1e6 /*40000.0*/)// some cut copied from Refitter - stupid!!!!
			{
				Float_t chi2, chi24;
				Float_t pulls[12];
				
				momentum[0]=proton1.Vect();
				momentum[1]=proton2.Vect();
				momentum[2]=pim.Vect();
				momentum[3]=pip.Vect();
				if(DoTheRefit(momentum,// order in momentum: proton, proton, pi-, pi+
						idxProton1,idxProton2,idxPiM,idxPiP,chi2,chi24,pulls)){
					
					if (histofile) {
						Float_t tmp[30];
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
						cout << "!!!!!error HHypPPPipPimGammaKine execute mylist->getIterStatus() == kFALSE!!!!!! " << endl;
						exit(2);
					}
					mylist->resetProbAlg( TMath::Prob(chi24,1));
					//store more data here....
					mylist->setUserValue(KINEFIT_CHI2, chi2);
					mylist->setUserValue(KINEFIT_CHI24, chi24);
				}else{
					// Fit failed -> Remove Combination
//					cout << "HHypPPPipPimPi0Kine DoTheRefit() == kFALSE" <<endl;
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

Bool_t HHypPPPipPimGammaKine::init()
{
	// need to get name from channel
	TString input(channel->Get(initList));
	
	if (histofile){
		qa = new TNtuple(input + TString("_kine_debug"), "Kine Debug ntuple",
			"chi2:chi24"
			":pull_p_p1:pull_the_p1:pull_phi_p1"
			":pull_p_p2:pull_the_p2:pull_phi_p2"
			":pull_p_pim:pull_the_pim:pull_phi_pim"
			":pull_p_pip:pull_the_pip:pull_phi_pip"
			":old_p_p1:old_p_p2:old_p_pim:old_p_pip"
			":old_phi_p1:old_phi_p2:old_phi_pim:old_phi_pip"
			":old_the_p1:old_the_p2:old_the_pim:old_the_pip"
			":errin_p_p1:errin_the_p1:errin_phi_p1"
			":errin_p_p2:errin_the_p2:errin_phi_p2"
			":errin_p_pim:errin_the_pim:errin_phi_pim"
			":errin_p_pip:errin_the_pip:errin_phi_pip"
		);
	}
	
	kinetest = new TNtuple(input + TString("_kine_errors"),
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

Bool_t HHypPPPipPimGammaKine::reinit()
{
	//Here, we set/reset the Momentum Error
	//Resolution could be run-dependent!
/*
 Bool_t simuflag;
	simuflag=gHades->getCurrentEvent()->getCategory(catGeantKine);
		
	if (!pParams->getCut("MomentumError_SEC_0", mom_err_sec_0)) {
		if(simuflag) mom_err_sec_0=0.02; else mom_err_sec_0=0.06;
		std::cout << "HHypPPPipPimGammaKine::init: MomentumError_SEC_0 not found" << std::endl;
		std::cout << "using hardcoded MomentumError_SEC_0" << std::endl;
	}
	
	if (!pParams->getCut("MomentumError_SEC_1", mom_err_sec_1)) {
		if(simuflag) mom_err_sec_1=0.02; else mom_err_sec_1=0.046;
		std::cout << "HHypPPPipPimGammaKine::init: MomentumError_SEC_1 not found" << std::endl;
		std::cout << "using hardcoded MomentumError_SEC_1" << std::endl;
	}
	
	if (!pParams->getCut("MomentumError_SEC_2", mom_err_sec_2)) {
		if(simuflag) mom_err_sec_2=0.025; else mom_err_sec_2=0.17;
		std::cout << "HHypPPPipPimGammaKine::init: MomentumError_SEC_2 not found" << std::endl;
		std::cout << "using hardcoded MomentumError_SEC_2" << std::endl;
	}
	
	if (!pParams->getCut("MomentumError_SEC_3", mom_err_sec_3)) {
		if(simuflag) mom_err_sec_3=0.02; else mom_err_sec_3=0.044;
		std::cout << "HHypPPPipPimGammaKine::init: MomentumError_SEC_3 not found" << std::endl;
		std::cout << "using hardcoded MomentumError_SEC_3" << std::endl;
	}

	if (!pParams->getCut("MomentumError_SEC_4", mom_err_sec_4)) {
		if(simuflag) mom_err_sec_4=0.02; else mom_err_sec_4=0.04;
		std::cout << "HHypPPPipPimGammaKine::init: MomentumError_SEC_4 not found" << std::endl;
		std::cout << "using hardcoded MomentumError_SEC_4" << std::endl;
	}
	
	if (!pParams->getCut("MomentumError_SEC_5", mom_err_sec_5)) {
		if(simuflag) mom_err_sec_5=0.025; else mom_err_sec_5=0.071;
		std::cout << "HHypPPPipPimGammaKine::init: MomentumError_SEC_5 not found" << std::endl;
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

Bool_t HHypPPPipPimGammaKine::finalize()
{
	if (histofile) qa->Write();
	kinetest->Write();
	
	return kTRUE;
}
