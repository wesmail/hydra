//*-- AUTHOR : B. Spruck
//*-- Modified : 10/nov/2006 by B. Spruck
//_HADES_CLASS_DESCRIPTION
////////////////////////////////////////////////////////////////////////
//
// HHypXKine
//
// This is
// i)  an ALGORITHM which updates the momenta using the HypKine Object
// ii) a SELECTOR which removes background using the kine refit
//
////////////////////////////////////////////////////////////////////////

#include "hhypXKine.h"

#define HYP_FITTER_PARTICLES	6

using namespace std;

ClassImp(HHypXKine)

HHypXKine::HHypXKine(Char_t *name_i, Option_t par[])
  :HHypBaseAlgorithm(name_i,par)
{
	fitter=new HLagrangeSolmitzFit();
	kinetest=NULL;
}

HHypXKine::~HHypXKine()
{
	delete fitter;
}

Double_t HHypXKine::getMomError(Double_t theta, Double_t phi, Double_t mass, Double_t p,Int_t sec,Bool_t simu)
	{
	Double_t res;
	res=HypMomErr.getMomError( theta,  phi,  mass,  p, sec, simu);

	if(kinetest) kinetest->Fill(p,res,mass,sec);
	return(res);
}

#define KINEDEBUG	0
Bool_t HHypXKine::execute()
{
 	Int_t nMaxPart;
 	nMaxPart=mylist->getNpart();

	if (nMaxPart > HYP_FITTER_PARTICLES){
		cerr << "HHypXKine::execute(): more than HYP_FITTER_PARTICLES not possible yet"<<endl;
		return kFALSE;
	}
	// max HYP_FITTER_PARTICLES particles ... at least in this code version

 Bool_t simuflag;
	simuflag=gHades->getCurrentEvent()->getCategory(catGeantKine);

	TVector3 momentum_ini[HYP_FITTER_PARTICLES];// nMaxPart
	TVector3 momentum_fitted[HYP_FITTER_PARTICLES];// nMaxPart
	Int_t pidofPart[HYP_FITTER_PARTICLES];// nMaxPart
	HPidTrackCand *pPidTrackCand[HYP_FITTER_PARTICLES];// nMaxPart
	Double_t cov_ini[HYP_FITTER_PARTICLES][9];// nMaxPart
	Double_t cov_fitted[HYP_FITTER_PARTICLES][9];// nMaxPart

	Double_t FakErrMom, FakErrTheta, FakErrPhi;
	if(simuflag){
		FakErrMom=FakErrMomSim;
		FakErrTheta=FakErrThetaSim;
		FakErrPhi=FakErrPhiSim;
	}else{
		FakErrMom=FakErrMomExp;
		FakErrTheta=FakErrThetaExp;
		FakErrPhi=FakErrPhiExp;
	}

	// The momenta will be modified... if i am the first this
	// is /in principle/ not needed but who knows, am I?
	mylist->initcopyMomentum();

#if KINEDEBUG
	cout << "=============== new Event ===========" << endl;
#endif
	mylist->CombIteratorReset();
	while (mylist->CombIterator()) {

#if KINEDEBUG
		cout << "------- new Comb ------" << endl;
#endif

		// clear particles...
		fitter->clearParticleSet();

		Double_t old_p[HYP_FITTER_PARTICLES];// nMaxPart
		Double_t old_the[HYP_FITTER_PARTICLES];// nMaxPart
		Double_t old_phi[HYP_FITTER_PARTICLES];// nMaxPart

	 Bool_t invalid_input=false;

		// Getting the particles
		Int_t icnt;
		icnt=0;// invalid particles should be at the end of list, but i do not rely on that and count myself
		Int_t nPart;
		nPart=mylist->getNvalid();// using that 
		for(Int_t i=0; i<nPart; i++){
			if(!mylist->isValidPart(i)) continue;// unused particle
			icnt++;

			Double_t momErr, theErr, phiErr;

			pidofPart[i]=mylist->getPid(i);
			momentum_ini[i]=mylist->getMomentum(i);
			pPidTrackCand[i]=mylist->getPidTrackCand(i);

#if KINEDEBUG
			cout << "Track: " << i << "  Sector: " <<pPidTrackCand[i]->getHitData()->getSector() << endl;
#endif

			// pPidTrackCand[i]->getCovarianceMatrix();

			Double_t theta, phi;
			theta=momentum_ini[i].Theta();
			if( theta<0.0) theta+=2.0*TMath::Pi();
			if( theta>=2.0*TMath::Pi()) theta-=2.0*TMath::Pi();
			phi=momentum_ini[i].Phi();
			if( phi<0.0) phi+=2.0*TMath::Pi();
			if( phi>=2.0*TMath::Pi()) phi-=2.0*TMath::Pi();

			// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			// Input Error calculation and Scaling up/down is
			// still under investigation and code will change in future
			// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			momErr=getMomError(theta,phi,
						HPidPhysicsConstants::mass(pidofPart[i]),
						momentum_ini[i].Mag(),
						pPidTrackCand[i]->getHitData()->getSector(),simuflag
					);

			if(momErr<0.0){
				invalid_input=true;
				break;
			}
			momErr*=FakErrMom;

         // pErr would be getErr2
         // 0 and 1 belongs to R and Z
         theErr=FakErrTheta*pPidTrackCand[i]->getTrackData()->getCovariance(ALG_RUNGEKUTTA).getErr(3);
         phiErr=FakErrPhi*pPidTrackCand[i]->getTrackData()->getCovariance(ALG_RUNGEKUTTA).getErr(4);


//          HBaseTrack *b;
//  			b=(HBaseTrack *) (pPidTrackCand[i]->getTrackData()->getBaseTrack( pPidTrackCand[i]->getTrackData()->getBestMomAlg()));
// 			theErr=FakErrTheta*b->getErrTheta();
// 			phiErr=FakErrPhi*b->getErrPhi();
			if( theErr>0.1 || phiErr>0.1 ){
//				cerr << algoName << " Theta or Phi Error>Pi().. does not make sense to fit! " << endl;
				invalid_input=true;
				break;
			}
			if(theErr<=0 || phiErr<=0 ){
//				cerr << algoName << " Theta or Phi Error<=0.. does not make sense to fit! " << endl;
				invalid_input=true;
				break;
			}
			/*{
				HPidHitData const *hit;
				hit=pPidTrackCand[i]->getHitData();
				if( hit){
					if (hit->fInnerMdcChiSquare < 0){
						invalid_input=true;
						break;
					}
				}else{
					cerr << algoName << "HHypXKine::execute(): Not Hit data! " << endl;
					exit(0);
				}
			}*/

			for(Int_t j=0; j<9; j++) cov_ini[i][j]=0;
			cov_ini[i][0] = momErr*momErr *1e-6;// MeV in GeV
			cov_ini[i][4] = theErr*theErr;
			cov_ini[i][8] = phiErr*phiErr;

			// MeV in GeV for Momentum!

#if KINEDEBUG
			cout << "PID: " <<pidofPart[i] << "  Mom: "<<momentum_ini[i].Mag()*1e-3 << "  Theta: "<< theta <<"  Phi: "<<phi <<endl;
			cout << "MomErr: " <<sqrt(cov_ini[i][0]) <<"  ThetaErr: " <<sqrt(cov_ini[i][4]) <<"  PhiErr: " <<sqrt(cov_ini[i][8]) <<endl;
#endif
			old_p[i]=momentum_ini[i].Mag();
			old_the[i]=theta;
			old_phi[i]=phi;
			fitter->addChargedParticle(pidofPart[i], momentum_ini[i].Mag()*1e-3, theta, phi, cov_ini[i]);
//			cout << ":: " << i <<": "<<pidofPart[i] <<" , "<< momentum_ini[i].Mag()*1e-3 << " , "<< theta <<" , " << phi<< endl;
		}

		if (icnt!=nPart){
			Error("HypXKine","BIG BIG error: valid particles!=getNValid()");
			continue;
		}		
		if (icnt<1){// useless to fit zero particles
			mylist->removeComb();
			continue;
		}
		if (invalid_input){// Unrealistic input errors
			mylist->removeComb();
			continue;
		}
		if (mylist->getIterStatus() != kTRUE) {
			cerr << algoName << " IterStatus is FALSE. Big Problem! " << endl;
			mylist->removeComb();
			continue;
		}

		Float_t mpid;
		Int_t miss_pid;

		if (!mylist->getUserValue(FILLER_MISSING_PID, mpid)) mpid = -999;
		miss_pid=(Int_t)mpid;
		if( miss_pid==-999){
			cout << "// Missing particle not supported: "<< miss_pid <<endl;
			mylist->removeComb();
			continue;
		}else if(miss_pid==0){
#if KINEDEBUG
			cout << "// Missing particle: "<< miss_pid <<endl;
#endif
		}// else ?

		if(miss_pid==0){// no neutral (missing) particle
			fitter->clearNeutralParticle();
#if KINEDEBUG
			cout <<"Missing nothing" << endl;
#endif
		}else{
			fitter->setNeutralParticle(miss_pid);
#if KINEDEBUG
			cout <<"Missing " << miss_pid << endl;
#endif
		}

		if (mylist->getIterStatus() != kTRUE) {
			cerr << algoName << " IterStatus is FALSE. Might be missing FILLER_MISSING_PID UserValue! " << endl;
			mylist->removeComb();
			continue;
		}

		// Now try to fit
		if(!fitter->fit()){
			cout << algoName << " Fitter Parameters not correct. " << endl;
			mylist->removeComb();
			continue;
		}

		// check result
		if(!fitter->isConvergence()){
#if KINEDEBUG
			cout << algoName << " Fitter not converged! " << endl;
#endif
			mylist->removeComb();
			continue;
		}

		// more exact result...
   	// Double_t getRetCode ();
		// Int_t    getIT ();
		// Double_t  get_dE ();
		// Bool_t   get_dMom (Double_t* dM);
		Double_t chi2=fitter->getChi2();
		// Double_t  getProb ();
		Double_t channel_code= fitter->getChannelCode();

#if KINEDEBUG
		cout << "Chi2: "<<chi2<<"   ChannelCode: "<<channel_code<<endl;
#endif

		Double_t pulls_p[HYP_FITTER_PARTICLES];// nMaxPart
		Double_t pulls_the[HYP_FITTER_PARTICLES];// nMaxPart
		Double_t pulls_phi[HYP_FITTER_PARTICLES];// nMaxPart

		// Now get the fitted tracks
		icnt=0;// invalid particles should be at the end of list, but i do not rely on that and count myself
		for(Int_t i=0; i<nPart; i++){
			if(!mylist->isValidPart(i)) continue;// unused particle

			//Double_t momErr, theErr, phiErr;
			Double_t p, theta, phi;

			fitter->getFittedChargedParticle ( icnt++, &p, &theta, &phi, cov_fitted[i]);
			p*=1e+3;// GeV in MeV
//			momentum_fitted[i].SetXYZ(p*sin(theta)*cos(phi),p*sin(theta)*sin(phi),p*cos(theta));
			momentum_fitted[i].SetMagThetaPhi(p,theta,phi);
			mylist->setMomentum(i, momentum_fitted[i]);

			//momErr=TMath::Sqrt(cov_fitted[i][0]*1e+6);// GeV in MeV
			//theErr=TMath::Sqrt(cov_fitted[i][4]);
			//phiErr=TMath::Sqrt(cov_fitted[i][8]);

			// Real Pull is:
			Double_t nom;
			Double_t mi, mf;
			//Double_t ki, kf;
			Double_t dki, dkf;
			mi=momentum_ini[i].Mag();
			//ki=1.0/mi;
			mf=p;//momentum_fitted[i].Mag();
			//kf=1.0/mf;
			dki=cov_ini[i][0]*1e+6/(mi*mi*mi*mi);
			dkf=cov_fitted[i][0]*1e+6/(mf*mf*mf*mf);
			nom=dki-dkf;
			if(nom>0.0){
				pulls_p[i]=(mf-mi)/(mi*mf*TMath::Sqrt(nom));
				if(fabs(mf-mi)<=1e-9){
					cout << "PULL P FAIL: "<<mi-mf<<" " <<nom<<endl;
				}
			}else{
				pulls_p[i]=-99.0;// NAN would not be visible in histogramm
			}

			nom=cov_ini[i][4]-cov_fitted[i][4];
			if(nom>0.0){
				pulls_the[i]=(momentum_ini[i].Theta()-theta)/TMath::Sqrt(nom);//momentum_fitted[i].Theta();
				if(fabs(momentum_ini[i].Theta()-theta)<=1e-9){
					cout << "PULL THETA FAIL: "<<momentum_ini[i].Theta()-theta<<" " <<nom<<endl;
				}
			}else{
				pulls_the[i]=-99.0;// NAN would not be visible in histogramm
			}

			nom=cov_ini[i][8]-cov_fitted[i][8];
			if(nom>0.0){
				Double_t dp;
				dp=momentum_ini[i].Phi()-phi;//momentum_fitted[i].Phi();
				if(dp>TMath::Pi()) dp-=2.0*TMath::Pi();
				if(dp<=-TMath::Pi()) dp+=2.0*TMath::Pi();
				pulls_phi[i]=dp/TMath::Sqrt(nom);
				if(fabs(dp)<=1e-9){
					cout << "PULL PHI FAIL: " <<momentum_ini[i].Phi()-phi <<" " <<nom<<endl;
				}
			}else{
				pulls_phi[i]=-99.0;// NAN would not be visible in histogramm
			}

			switch (pidofPart[i])
			{
					case 2:// e+
						hPullsMomEp->Fill(pulls_p[i]);
						hPullsThetaEp->Fill(pulls_the[i]);
						hPullsPhiEp->Fill(pulls_phi[i]);
						break;
					case 3:// e-
						hPullsMomEm->Fill(pulls_p[i]);
						hPullsThetaEm->Fill(pulls_the[i]);
						hPullsPhiEm->Fill(pulls_phi[i]);
						break;
					case 8:// Pi+
						hPullsMomPip->Fill(pulls_p[i]);
						hPullsThetaPip->Fill(pulls_the[i]);
						hPullsPhiPip->Fill(pulls_phi[i]);
						break;
					case 9:// Pi-
						hPullsMomPim->Fill(pulls_p[i]);
						hPullsThetaPim->Fill(pulls_the[i]);
						hPullsPhiPim->Fill(pulls_phi[i]);
						break;
					case 14:// Proton
						hPullsMomP->Fill(pulls_p[i]);
						hPullsThetaP->Fill(pulls_the[i]);
						hPullsPhiP->Fill(pulls_phi[i]);
						break;
					default:
						// have to ignore
						break;
			}

		}

		// Bool_t   getFittedNeutralParticle (           Double_t* p, Double_t* theta, Double_t* phi, Double_t* cov);

		if (mylist->getIterStatus() == kFALSE) {
			cout << "!!!!!error HHypXKine execute mylist->getIterStatus() == kFALSE!!!!!! " << endl;
			exit(2);
		}

		Double_t prob;
		prob=fitter->getProb();
		hProb->Fill(prob);
		mylist->resetProbAlg(prob);
		// mylist->resetProbAlg( TMath::Prob(chi2,1)); <<< 1 or 4 depending on missing particle
		//store more data here....
		mylist->setUserValue(KINEFIT_PROB, prob);
		mylist->setUserValue(KINEFIT_CHI2, chi2);

		Int_t cc_hi, cc_lo;
		cc_hi=(Int_t)(channel_code*1e-6);
		cc_lo=(Int_t)(channel_code-cc_hi*1e6);
		mylist->setUserValue(KINEFIT_CHANNEL_LO, cc_lo);// lo
		mylist->setUserValue(KINEFIT_CHANNEL_HI, cc_hi);// hi

		if (histofile) {
			Float_t tmp[80];
			Int_t ii;
			ii=0;
			tmp[ii++]=chi2;
			tmp[ii++]=cc_lo;// channel code lo
			tmp[ii++]=cc_hi;// channel code hi
			tmp[ii++]=miss_pid;
			for(Int_t i=0; i<4; i++){// only 4 not nPart, nMaxPart or HYP_FITTER_PARTICLES
				tmp[ii++]=pidofPart[i];
				tmp[ii++]=pulls_p[i];
				tmp[ii++]=pulls_the[i];
				tmp[ii++]=pulls_phi[i];
				tmp[ii++]=old_p[i];
				tmp[ii++]=old_the[i];
				tmp[ii++]=old_phi[i];
				tmp[ii++]=momentum_fitted[i].Mag();
				tmp[ii++]=momentum_fitted[i].Theta();
				tmp[ii++]=momentum_fitted[i].Phi();
				tmp[ii++]=TMath::Sqrt(cov_ini[i][0]*1e+6);
				tmp[ii++]=TMath::Sqrt(cov_ini[i][4]);
				tmp[ii++]=TMath::Sqrt(cov_ini[i][8]);
				tmp[ii++]=TMath::Sqrt(cov_fitted[i][0]*1e+6);
				tmp[ii++]=TMath::Sqrt(cov_fitted[i][4]);
				tmp[ii++]=TMath::Sqrt(cov_fitted[i][8]);
			}

			qa->Fill(tmp);
		}
	}                             //END Iterator

	if (exitIdx > -1)
		return kTRUE;
	return kFALSE;
}

Bool_t HHypXKine::init()
{

	// in GeV!
	fitter->setBeamTargetParametra(beam_pid, beam->Px()*1e-3, beam->Py()*1e-3, beam->Pz()*1e-3, target_pid);

	// need to get name from channel
	TString output(channel->Get(exitList));

	if (histofile){
		qa = new TNtuple(output + TString("_kine_debug"), "Kine Debug ntuple",
			"chi2:channel_code_lo:channel_code_hi:miss_pid"
			":pid1:pull_p1:pull_the1:pull_phi1"
			":old_p1:old_the1:old_phi1"
			":new_p1:new_the1:new_phi1"
			":old_err_p1:old_err_the1:old_err_phi1"
			":new_err_p1:new_err_the1:new_err_phi1"
			":pid2:pull_p2:pull_the2:pull_phi2"
			":old_p2:old_the2:old_phi2"
			":new_p2:new_the2:new_phi2"
			":old_err_p2:old_err_the2:old_err_phi2"
			":new_err_p2:new_err_the2:new_err_phi2"
			":pid3:pull_p3:pull_the3:pull_phi3"
			":old_p3:old_the3:old_phi3"
			":new_p3:new_the3:new_phi3"
			":old_err_p3:old_err_the3:old_err_phi3"
			":new_err_p3:new_err_the3:new_err_phi3"
			":pid4:pull_p4:pull_the4:pull_phi4"
			":old_p4:old_the4:old_phi4"
			":new_p4:new_the4:new_phi4"
			":old_err_p4:old_err_the4:old_err_phi4"
			":new_err_p4:new_err_the4:new_err_phi4"
		);
		kinetest = new TNtuple(output + TString("_kine_errors"),
			"Kine DebugErrors ntuple","p:dp:m:sec");

		const Int_t BINS=100;
		const Double_t PULLS_MIN=-10.;
		const Double_t PULLS_MAX=10.;
		TString tmp;
		tmp=output + TString("_kine_PullsMomEp");
		hPullsMomEp=new TH1I(tmp,tmp,BINS,PULLS_MIN,PULLS_MAX);
		tmp=output + TString("_kine_PullsThetaEp");
		hPullsThetaEp=new TH1I(tmp,tmp,BINS,PULLS_MIN,PULLS_MAX);
		tmp=output + TString("_kine_PullsPhiEp");
		hPullsPhiEp=new TH1I(tmp,tmp,BINS,PULLS_MIN,PULLS_MAX);
		tmp=output + TString("_kine_PullsMomEm");
		hPullsMomEm=new TH1I(tmp,tmp,BINS,PULLS_MIN,PULLS_MAX);
		tmp=output + TString("_kine_PullsThetaEm");
		hPullsThetaEm=new TH1I(tmp,tmp,BINS,PULLS_MIN,PULLS_MAX);
		tmp=output + TString("_kine_PullsPhiEm");
		hPullsPhiEm=new TH1I(tmp,tmp,BINS,PULLS_MIN,PULLS_MAX);
		tmp=output + TString("_kine_PullsMomPip");
		hPullsMomPip=new TH1I(tmp,tmp,BINS,PULLS_MIN,PULLS_MAX);
		tmp=output + TString("_kine_PullsThetaPip");
		hPullsThetaPip=new TH1I(tmp,tmp,BINS,PULLS_MIN,PULLS_MAX);
		tmp=output + TString("_kine_PullsPhiPip");
		hPullsPhiPip=new TH1I(tmp,tmp,BINS,PULLS_MIN,PULLS_MAX);
		tmp=output + TString("_kine_PullsMomPim");
		hPullsMomPim=new TH1I(tmp,tmp,BINS,PULLS_MIN,PULLS_MAX);
		tmp=output + TString("_kine_PullsThetaPim");
		hPullsThetaPim=new TH1I(tmp,tmp,BINS,PULLS_MIN,PULLS_MAX);
		tmp=output + TString("_kine_PullsPhiPim");
		hPullsPhiPim=new TH1I(tmp,tmp,BINS,PULLS_MIN,PULLS_MAX);
		tmp=output + TString("_kine_PullsMomP");
		hPullsMomP=new TH1I(tmp,tmp,BINS,PULLS_MIN,PULLS_MAX);
		tmp=output + TString("_kine_PullsThetaP");
		hPullsThetaP=new TH1I(tmp,tmp,BINS,PULLS_MIN,PULLS_MAX);
		tmp=output + TString("_kine_PullsPhiP");
		hPullsPhiP=new TH1I(tmp,tmp,BINS,PULLS_MIN,PULLS_MAX);

		tmp=output + TString("_kine_Probability");
		hProb=new TH1I(tmp,tmp,100,0.0,1.0);
	}

	HypMomErr.getFaktors(FakErrMomExp,FakErrThetaExp,FakErrPhiExp,FakErrMomSim,FakErrThetaSim,FakErrPhiSim);

	return kTRUE;
}

Bool_t HHypXKine::reinit()
{
	//Here, we set/reset the Momentum Error
	//Resolution could be run-dependent!

	return kTRUE;
}

Bool_t HHypXKine::finalize()
{
	if (histofile){
		hPullsMomEp->Write();
		hPullsThetaEp->Write();
		hPullsPhiEp->Write();
		hPullsMomEm->Write();
		hPullsThetaEm->Write();
		hPullsPhiEm->Write();
		hPullsMomPip->Write();
		hPullsThetaPip->Write();
		hPullsPhiPip->Write();
		hPullsMomPim->Write();
		hPullsThetaPim->Write();
		hPullsPhiPim->Write();
		hPullsMomP->Write();
		hPullsThetaP->Write();
		hPullsPhiP->Write();
		hProb->Write();
		qa->Write();
	}
	if (kinetest) kinetest->Write();

	return kTRUE;
}
