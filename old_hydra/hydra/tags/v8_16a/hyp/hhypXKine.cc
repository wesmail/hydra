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

using namespace std;

ClassImp(HHypXKine)

HHypXKine::HHypXKine(char *name_i, Option_t par[])
  :HHypBaseAlgorithm(name_i,par)
{
	fitter=new HLagrangeSolmitzFit();

	kinetest=0;
}

HHypXKine::~HHypXKine()
{
	delete fitter;
}


Double_t HHypXKine::getMomErr(Int_t sec,Double_t P,Int_t pid)
{// Everything is calculated in MeV here!
	Double_t dP, M;

	M=HPidPhysicsConstants::mass(pid);// get mass in MeV

	{
		Double_t c1, c2;

		if(sec>=0 && sec<6){
			Double_t momResC1[6]={0.0289,0.0160,0.0993,0.0122,0.0106,0.0284};
			Double_t momResC2[6]={0.0221,0.0221,0.0221,0.0221,0.0221,0.0221};
			c1=momResC1[sec]*1e-3;
			c2=momResC2[sec];
		}else{
			Error("getMomErr","Sector<0 || Sector>5");
			c1=0;
			c2=0;
		}
/*		switch(sec){
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
		}*/
		

		// Formula is (dP/P)²=(c1*p)²+(c2/beta)²
		Double_t P2=P*P;
		dP=TMath::Sqrt(c1*c1*P2*P2 + c2*c2*(P2+M*M));
	}

	if(kinetest) kinetest->Fill(P,dP,M,sec);
	return(dP);
}

#define KINEDEBUG	0
Bool_t HHypXKine::execute()
{
	Int_t npart;
	npart=mylist->getNpart();
	
	if (npart != 4) return kFALSE;
	//needs 4 particles ... at least in this code version

	TVector3 momentum_ini[4];// npart
	TVector3 momentum_fitted[4];// npart
	Int_t pidofPart[4];// npart
	HPidParticle *pPidPart[4];// npart
	Double_t cov_ini[4][9];// npart
	Double_t cov_fitted[4][9];// npart

	Float_t FakErrMom=1.255;//*0.35;
	Float_t FakErrTheta=1.11;//*0.4;
	Float_t FakErrPhi=1.11;//*0.4;
	
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

		// Getting the particles 
		for(Int_t i=0; i<npart; i++){
			Double_t momErr, theErr, phiErr;
			HBaseTrack *b;
			
#if KINEDEBUG
			cout << "Track: " << i << endl;
#endif
			
			pidofPart[i]=mylist->getPid(i);
			momentum_ini[i]=mylist->getMomentum(i);
			pPidPart[i]=mylist->getPidParticle(i);
			
			// pPidPart[i]->getCovarianceMatrix();
			momErr=FakErrMom*getMomErr(pPidPart[i]->sector(),momentum_ini[i].Mag(),pidofPart[i]);
			b=(HBaseTrack *)(pPidPart[i]->getTrackData()->getBaseTrack(pPidPart[i]->getMomAlg()));
			theErr=FakErrTheta*b->getErrTheta();
			phiErr=FakErrPhi*b->getErrPhi();
                            
			for(Int_t j=0; j<9; j++) cov_ini[i][j]=0;
			cov_ini[i][0] = momErr*momErr *1e-6;// MeV in GeV
			cov_ini[i][4] = theErr*theErr;
			cov_ini[i][8] = phiErr*phiErr;

			// MeV in GeV for Momentum!
			Double_t theta, phi;
			theta=momentum_ini[i].Theta();
			if( theta<0.0) theta+=2.0*TMath::Pi();
			if( theta>=2.0*TMath::Pi()) theta-=2.0*TMath::Pi();
			phi=momentum_ini[i].Phi();
			if( phi<0.0) phi+=2.0*TMath::Pi();
			if( phi>=2.0*TMath::Pi()) phi-=2.0*TMath::Pi();
			
#if KINEDEBUG
			cout << "PID: " <<pidofPart[i] << "  Mom: "<<momentum_ini[i].Mag()*1e-3 << "  Theta: "<< theta <<"  Phi: "<<phi <<endl;
			cout << "MomErr: " <<sqrt(cov_ini[i][0]) <<"  ThetaErr: " <<sqrt(cov_ini[i][4]) <<"  PhiErr: " <<sqrt(cov_ini[i][8]) <<endl;
#endif
			
			fitter->addChargedParticle(pidofPart[i], momentum_ini[i].Mag()*1e-3, theta, phi, cov_ini[i]);
//			cout << ":: " << i <<": "<<pidofPart[i] <<" , "<< momentum_ini[i].Mag()*1e-3 << " , "<< theta <<" , " << phi<< endl;
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
		// Float_t  get_dE ();
		// Bool_t   get_dMom (Float_t* dM);
		Double_t chi2=fitter->getChi2();
		// Float_t  getProb ();
		Double_t channel_code= fitter->getChannelCode();

#if KINEDEBUG
		cout << "Chi2: "<<chi2<<"   ChannelCode: "<<channel_code<<endl;
#endif

		// Now get the fitted tracks
		for(Int_t i=0; i<npart; i++){
			//Double_t momErr, theErr, phiErr;
			Double_t p, theta, phi;

			fitter->getFittedChargedParticle ( i, &p, &theta, &phi, cov_fitted[i]);
			p*=1e+3;// GeV in MeV
			momentum_fitted[i].SetXYZ(p*sin(theta)*cos(phi),p*sin(theta)*sin(phi),p*cos(theta));
			mylist->setMomentum(i, momentum_fitted[i]);
		
			//momErr=TMath::Sqrt(cov_fitted[i][0]*1e+6);// GeV in MeV
			//theErr=TMath::Sqrt(cov_fitted[i][4]);
			//phiErr=TMath::Sqrt(cov_fitted[i][8]);
		}
			
		// Bool_t   getFittedNeutralParticle (           Float_t* p, Float_t* theta, Float_t* phi, Double_t* cov);
		
		if (mylist->getIterStatus() == kFALSE) {
			cout << "!!!!!error HHypXKine execute mylist->getIterStatus() == kFALSE!!!!!! " << endl;
			exit(2);
		}
		mylist->resetProbAlg( TMath::Prob(chi2,1));
		//store more data here....
		mylist->setUserValue(KINEFIT_CHI24, chi2);

		Float_t pulls_p[4];// npart
		Float_t pulls_the[4];// npart
		Float_t pulls_phi[4];// npart
		
		for(Int_t i=0; i<npart; i++){
			// Real Pull is:
			Double_t nom;
			Double_t mi, mf;
			Double_t ki, kf;
			Double_t dki, dkf;
			mi=momentum_ini[i].Mag();
			ki=1.0/mi;
			mf=momentum_fitted[i].Mag();
			kf=1.0/mf;
			dki=cov_ini[i][0]*1e+6/(mi*mi*mi*mi);
			dkf=cov_fitted[i][0]*1e+6/(mf*mf*mf*mf);
			nom=dki-dkf;
			if(nom>0){
				pulls_p[i]=(ki-kf)/TMath::Sqrt(nom);
			}else{
				pulls_p[i]=-99.0;// NAN would not be visible in histogramm
			}
		
			nom=cov_ini[i][4]-cov_fitted[i][4];
			if(nom>0){
				Double_t dt;
				dt=momentum_ini[i].Theta()-momentum_fitted[i].Theta();
				pulls_the[i]=dt/TMath::Sqrt(nom);
			}else{
				pulls_the[i]=-99.0;// NAN would not be visible in histogramm
			}
		
			nom=cov_ini[i][8]-cov_fitted[i][8];
			if(nom>0){
				Double_t dp;
				dp=momentum_ini[i].Phi()-momentum_fitted[i].Phi();
				if(dp>TMath::Pi()) dp-=2.0*TMath::Pi();
				if(dp<=-TMath::Pi()) dp+=2.0*TMath::Pi();
				pulls_phi[i]=dp/TMath::Sqrt(nom);
			}else{
				pulls_phi[i]=-99.0;// NAN would not be visible in histogramm
			}
		}
		
		mylist->setUserValue(KINEFIT_CHANNEL_LO, (((Int_t)channel_code)%1000000));// lo
		mylist->setUserValue(KINEFIT_CHANNEL_HI, ((Int_t)(channel_code*1e-6)));// hi
		
		if (histofile) {
			Float_t tmp[30];
			Int_t ii;
			ii=0;
			tmp[ii++]=chi2;
			tmp[ii++]=((Int_t)channel_code)%1000000;// lo
			tmp[ii++]=(Int_t)(channel_code*1e-6);// hi
			tmp[ii++]=miss_pid;
			for(Int_t i=0; i<4; i++){
				tmp[ii++]=pidofPart[i];
				tmp[ii++]=pulls_p[i];
				tmp[ii++]=pulls_the[i];
				tmp[ii++]=pulls_phi[i];
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
	fitter->setBeamTargetParametra(14, -0.005011, 0.006559, 2.994717, 14);

	use_err_fixed=false;
//	use_err_fixed = (GetOpt("ERR_FIXED") != NULL);
//	if(use_err_fixed) Info("init","using fixed kine ERRORS");
	
	// need to get name from channel
	TString output(channel->Get(exitList));
	
	if (histofile){
		qa = new TNtuple(output + TString("_kine_debug"), "Kine Debug ntuple",
			"chi24:channel_code_lo:channel_code_hi:miss_pid"
			":pid1:pull_p1:pull_the1:pull_phi1"
			":pid2:pull_p2:pull_the2:pull_phi2"
			":pid3:pull_p3:pull_the3:pull_phi3"
			":pid4:pull_p4:pull_the4:pull_phi4"
		);
	}

	kinetest = new TNtuple(output + TString("_kine_errors"),
		"Kine DebugErrors ntuple","p:dp:m:sec");

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
	if (histofile) qa->Write();
	if (kinetest) kinetest->Write();
	
	return kTRUE;
}
