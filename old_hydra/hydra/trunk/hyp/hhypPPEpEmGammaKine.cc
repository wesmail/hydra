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

#include "hhypPPEpEmGammaKine.h"
#include "hypinfodef.h"
#include "hbasetrack.h"

ClassImp(HHypPPEpEmGammaKine)

HHypPPEpEmGammaKine::HHypPPEpEmGammaKine(Char_t *name_i, Option_t par[])
:HHypBaseAlgorithm(name_i,par)
{
	fit=new HHypKineFitPPEpEmGamma();
	kinetest=NULL;
}

HHypPPEpEmGammaKine::~HHypPPEpEmGammaKine()
{
	delete fit;
}

Double_t HHypPPEpEmGammaKine::getMomError(Double_t theta, Double_t phi, Double_t mass, Double_t p,Int_t sec,Bool_t simu)
{
	Double_t res;
	res=HypMomErr.getMomError( theta,  phi,  mass,  p, sec, simu);

	if(kinetest) kinetest->Fill(p,res,mass,sec);
	return(res);
}

Bool_t HHypPPEpEmGammaKine::DoTheRefit(TVector3 *momentum,Int_t idx_p1,Int_t idx_p2,Int_t idx_em,Int_t idx_ep,Float_t &prechi2,Float_t &chi2, Float_t *pulls)
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
	HPidTrackCand *em=(HPidTrackCand *) ptrackcandCat->getObject(idx_em);
	HPidTrackCand *ep=(HPidTrackCand *) ptrackcandCat->getObject(idx_ep);

	HBaseTrack *b;

 Bool_t simuflag;
	simuflag=gHades->getCurrentEvent()->getCategory(catGeantKine);

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// Input Error calculation and Scaling up/down is
	// still under investigation and code will change in future
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	Double_t FakErrMom, FakErrTheta, FakErrPhi;
	if( simuflag){// sim
//		FakErrMom= 1.37;
//		FakErrTheta=1.11;
//		FakErrPhi=1.11;
		FakErrMom=0.65;
		FakErrTheta=0.95;
		FakErrPhi=0.95 ;
	}else{// exp
		FakErrMom=0.7;// war 1.37;
		FakErrTheta=1.0; // war 1.11;
		FakErrPhi=1.0 ; //war 1.11;
	}

	Double_t momErr;

	//p1
	momErr=getMomError(input[1],input[2],14,input[0],p1->getHitData()->getSector(),simuflag);
	errInput[0]=FakErrMom*momErr;//getMomErr(p1->getHitData()->getSector(),input[0],14);// Proton

	b=(HBaseTrack *)(p1->getTrackData()->getBaseTrack(p1->getTrackData()->getBestMomAlg()));
	errInput[1]=FakErrTheta*b->getErrTheta();
	errInput[2]=FakErrPhi*b->getErrPhi();
	if( momErr<0.0 || b->getErrTheta()>0.1/*TMath::Pi()*/ || b->getErrPhi()>0.1/*TMath::Pi()*/ ){
		cerr << algoName << "MomErr<0, Theta or Phi Error>Pi().. does not make sense to fit! " << endl;
		return kFALSE;
	}

	//p2
	momErr=getMomError(input[4],input[5],14,input[3],p2->getHitData()->getSector(),simuflag);
	errInput[3]=FakErrMom*momErr;//FakErrMom*getMomErr(p2->getHitData()->getSector(),input[3],14);// Proton

	b=(HBaseTrack *)(p2->getTrackData()->getBaseTrack(p2->getTrackData()->getBestMomAlg()));
	errInput[4]=FakErrTheta*b->getErrTheta();
	errInput[5]=FakErrPhi*b->getErrPhi();
	if( momErr<0.0 || b->getErrTheta()>0.1/*TMath::Pi()*/ || b->getErrPhi()>0.1/*TMath::Pi()*/ ){
		cerr << algoName << "MomErr<0, Theta or Phi Error>Pi().. does not make sense to fit! " << endl;
		return kFALSE;
	}

	// e-
	momErr=getMomError(input[7],input[8],3,input[6],em->getHitData()->getSector(),simuflag);
	errInput[6]=FakErrMom*momErr;//FakErrMom*getMomErr(em->getHitData()->getSector(),input[6],3);// e-

	b=(HBaseTrack *)(em->getTrackData()->getBaseTrack(em->getTrackData()->getBestMomAlg()));
	errInput[7]=FakErrTheta*b->getErrTheta();
	errInput[8]=FakErrPhi*b->getErrPhi();
	if( momErr<0.0 || b->getErrTheta()>0.1/*TMath::Pi()*/ || b->getErrPhi()>0.1/*TMath::Pi()*/ ){
		cerr << algoName << "MomErr<0, Theta or Phi Error>Pi().. does not make sense to fit! " << endl;
		return kFALSE;
	}

	// e+
	momErr=getMomError(input[10],input[11],2,input[9],ep->getHitData()->getSector(),simuflag);
	errInput[9]=FakErrMom*momErr;//FakErrMom*getMomErr(ep->getHitData()->getSector(),input[9],2);// e+

	b=(HBaseTrack *)(ep->getTrackData()->getBaseTrack(ep->getTrackData()->getBestMomAlg()));
	errInput[10]=FakErrTheta*b->getErrTheta();
	errInput[11]=FakErrPhi*b->getErrPhi();
	if( momErr<0.0 || b->getErrTheta()>0.1/*TMath::Pi()*/ || b->getErrPhi()>0.1/*TMath::Pi()*/ ){
		cerr << algoName << "MomErr<0, Theta or Phi Error>Pi().. does not make sense to fit! " << endl;
		return kFALSE;
	}

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
			cs=(1/input[ii]-1/output[ii])*(1/input[ii]-1/output[ii])/errInput[ii]/errInput[ii]*input[ii]*input[ii]*input[ii]*input[ii];
		}else{*/
			cs=(input[ii]-output[ii])/errInput[ii];
//		}
		chi2+=cs*cs;
	}

	for(Int_t i=0; i<12; i++){
		// Real Pull is:
		Double_t nom;
		// Preisfrage: ist das der Pull des Impulses oder 1/Impuls???
		// folgender Auskommentierter Code ist ein Test!!!
/*		if(i==0 ||i==3 ||i==6||i==9){
			nom=(input[i]*input[i])/errInput[i]*errInput[i]-(output[i]*output[i])/derOut[i];// derOut already squared
			if(nom>0){
				pulls[i]=(1.0/input[i]-1.0/output[i])/TMath::Sqrt(nom);
			}else{
				pulls[i]=-99.0;// NAN would not be visible in histogramm
			}
		}else*/{
		nom=errInput[i]*errInput[i]-derOut[i];// derOut already squared
		if(nom>0){
			pulls[i]=(input[i]-output[i])/TMath::Sqrt(nom);
		}else{
			pulls[i]=-99.0;// NAN would not be visible in histogramm
		}
	}
	}

	return kTRUE;
}

Bool_t HHypPPEpEmGammaKine::execute()
{
// 	if (mylist->getNpart() != 4)
// 		return kFALSE;
	//needs 4 particles

	TVector3 momentum[4];

	// The momenta will be modified... if i am the first this
	// is /in principle/ not needed but who knows, am I?
	mylist->initcopyMomentum();

	mylist->CombIteratorReset();
	while (mylist->CombIterator()) {
 		if (mylist->getNvalid() != 4) continue;// p p e+ e-

		Float_t miss_pid;
		if (!mylist->getUserValue(FILLER_MISSING_PID, miss_pid)) miss_pid = -999;

		if(miss_pid!=1){
			Error("HHypPPEpEmGammaKine::execute()","This Fitter fits only missing gamma, but FILLER_MISSING_PID is %i ",(Int_t)miss_pid);
			continue;
		}

		// Getting the particles
		TLorentzVector proton1 = mylist->getTLorentzVector("p", 1);
		TLorentzVector proton2 = mylist->getTLorentzVector("p", 2);
		TLorentzVector em = mylist->getTLorentzVector("e-", 1);
		TLorentzVector ep = mylist->getTLorentzVector("e+", 1);

		Int_t idxProton1=mylist->getIdxPidTrackCand("p", 1);
		Int_t idxProton2=mylist->getIdxPidTrackCand("p", 2);
		Int_t idxElectron=mylist->getIdxPidTrackCand("e-", 1);
		Int_t idxPositron=mylist->getIdxPidTrackCand("e+", 1);

		if (mylist->getIterStatus() != kTRUE) continue;
		{
		 Bool_t invalid_input=false;
			for(Int_t i=0; i<4; i++){
				HPidTrackCand *pPidTrackCand;
				HPidHitData const *hit;
				pPidTrackCand=mylist->getPidTrackCand(i);
				hit=0;
				if(pPidTrackCand) hit=pPidTrackCand->getHitData();
				if( hit){
					if (hit->fInnerMdcChiSquare < 0){
						invalid_input=true;
						break;
					}
				}else{
					cerr << algoName << "HHypXKine::execute(): Not PidPart or Hit data! " << endl;
					exit(0);
				}
			}
			if(invalid_input){
				mylist->removeComb();
				continue;
			}
		}

		if (mylist->getIterStatus() == kTRUE) {

				Float_t prechi2, chi2;
				Float_t pulls[32];// war 12
				momentum[0]=proton1.Vect();
				momentum[1]=proton2.Vect();
				momentum[2]=em.Vect();
				momentum[3]=ep.Vect();
				if(DoTheRefit(momentum,// order in momentum: proton, proton, e-, e+
						idxProton1,idxProton2,idxElectron,idxPositron,prechi2,chi2,pulls)){

					if (histofile) {
						Float_t tmp[50];
						Int_t ii;
						ii=0;
						tmp[ii++]=chi2;
						tmp[ii++]=prechi2;
						tmp[ii++]=1;// missing gamma

						tmp[ii++]=14;// Proton
						tmp[ii++]=pulls[0];
						tmp[ii++]=pulls[1];
						tmp[ii++]=pulls[2];
						tmp[ii++]=proton1.Vect().Mag();
						tmp[ii++]=proton1.Vect().Theta();
						tmp[ii++]=proton1.Vect().Phi();
						tmp[ii++]=momentum[0].Mag();
						tmp[ii++]=momentum[0].Theta();
						tmp[ii++]=momentum[0].Phi();

						tmp[ii++]=14;// Proton
						tmp[ii++]=pulls[3];
						tmp[ii++]=pulls[4];
						tmp[ii++]=pulls[5];
						tmp[ii++]=proton2.Vect().Mag();
						tmp[ii++]=proton2.Vect().Theta();
						tmp[ii++]=proton2.Vect().Phi();
						tmp[ii++]=momentum[1].Mag();
						tmp[ii++]=momentum[1].Theta();
						tmp[ii++]=momentum[1].Phi();

						tmp[ii++]=2;// e+
						tmp[ii++]=pulls[6];
						tmp[ii++]=pulls[7];
						tmp[ii++]=pulls[8];
						tmp[ii++]=ep.Vect().Mag();
						tmp[ii++]=ep.Vect().Theta();
						tmp[ii++]=ep.Vect().Phi();
						tmp[ii++]=momentum[3].Mag();
						tmp[ii++]=momentum[3].Theta();
						tmp[ii++]=momentum[3].Phi();

						tmp[ii++]=3;// e-
						tmp[ii++]=pulls[9];
						tmp[ii++]=pulls[10];
						tmp[ii++]=pulls[11];
						tmp[ii++]=em.Vect().Mag();
						tmp[ii++]=em.Vect().Theta();
						tmp[ii++]=em.Vect().Phi();
						tmp[ii++]=momentum[2].Mag();
						tmp[ii++]=momentum[2].Theta();
						tmp[ii++]=momentum[2].Phi();

/*					for(Int_t i=0; i<12; i++){
							tmp[ii++]=errInput[i];
						}*/
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
					mylist->resetProbAlg( TMath::Prob(chi2,1));
					mylist->setUserValue(KINEFIT_CHI2, chi2);
					mylist->setUserValue(KINEFIT_PRECHI2, prechi2);
				}else{
					// Fit failed -> Remove Combination
				//	cout << "HHypPPEpEmGammaKine DoTheRefit() == kFALSE"  << endl;
					mylist->removeComb();
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
	TString output(channel->Get(exitList));

	if (histofile){
		qa = new TNtuple(output + TString("_kine_debug"), "Kine Debug ntuple",
			"chi2:prechi2:miss_pid"

			":pid1:pull_p1:pull_the1:pull_phi1"
			":old_p1:old_the1:old_phi1"
			":new_p1:new_the1:new_phi1"

			":pid2:pull_p2:pull_the2:pull_phi2"
			":old_p2:old_the2:old_phi2"
			":new_p2:new_the2:new_phi2"

			":pid3:pull_p3:pull_the3:pull_phi3"
			":old_p3:old_the3:old_phi3"
			":new_p3:new_the3:new_phi3"

			":pid4:pull_p4:pull_the4:pull_phi4"
			":old_p4:old_the4:old_phi4"
			":new_p4:new_the4:new_phi4"

//			":errin_p_p1:errin_the_p1:errin_phi_p1"
//			":errin_p_p2:errin_the_p2:errin_phi_p2"
//			":errin_p_em:errin_the_em:errin_phi_em"
//			":errin_p_ep:errin_the_ep:errin_phi_ep"
		);
		kinetest = new TNtuple(output + TString("_kine_errors"),
			"Kine DebugErrors ntuple","p:dp:m:sec");
	}

	return kTRUE;
}

Bool_t HHypPPEpEmGammaKine::reinit()
{
	//Here, we set/reset the Momentum Error
	//Resolution could be run-dependent!

// Bool_t simuflag;
//	simuflag=gHades->getCurrentEvent()->getCategory(catGeantKine);

	return kTRUE;
}

Bool_t HHypPPEpEmGammaKine::finalize()
{
	if (histofile) qa->Write();
	if (kinetest) kinetest->Write();
	return kTRUE;
}
