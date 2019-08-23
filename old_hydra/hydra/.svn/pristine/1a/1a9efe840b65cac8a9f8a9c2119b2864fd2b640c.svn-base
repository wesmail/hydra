//*-- AUTHOR : B. Spruck
//*-- Modified : 26/10/2005 by B. Spruck
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
	kk=new HHypKineRefitUtils(2200.0);
	fitEta=new HHypKineFitEta();
}

HHypPPEpEmGammaKine::~HHypPPEpEmGammaKine()
{
	delete fitEta;
	delete kk;
}

Bool_t HHypPPEpEmGammaKine::DoTheRefit(TVector3 *momentum,Int_t idx_p1,Int_t idx_p2,Int_t idx_em,Int_t idx_ep,Float_t &chi2,Float_t &chi24)
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
	
	//p1
	errInput[0]=input[0]*7.0/100.0;
	if(p1->sector()==2 || p1->sector()==5)
		errInput[0]=input[0]*15./100.;
	
	b=(HBaseTrack *)(p1->getTrackData()->getBaseTrack(p1->getMomAlg()));
	errInput[1]=2*b->getErrTheta();
	errInput[2]=2*b->getErrPhi();
	
	//p2
	errInput[3]=input[3]*7./100.;
	if(p2->sector()==2 || p2->sector()==5)
		errInput[3]=input[3]*15./100.;
	
	b=(HBaseTrack *)(p2->getTrackData()->getBaseTrack(p2->getMomAlg()));
	errInput[4]=2*b->getErrTheta();
	errInput[5]=2*b->getErrPhi();
	
	// e-
	errInput[6]=input[6]*7./100.;
	if(em->sector()==2 || em->sector()==5)
		errInput[6]=input[6]*15./100.;
	
	b=(HBaseTrack *)(em->getTrackData()->getBaseTrack(em->getMomAlg()));
	errInput[7]=2*b->getErrTheta();
	errInput[8]=2*b->getErrPhi();
	
	// e+
	errInput[9]=input[9]*7./100.;
	if(ep->sector()==2 || ep->sector()==5)
		errInput[9]=input[9]*15./100.;
	
	b=(HBaseTrack *)(ep->getTrackData()->getBaseTrack(ep->getMomAlg()));
	errInput[10]=2*b->getErrTheta();
	errInput[11]=2*b->getErrPhi();

	
	fitEta->ResetOutput();
	fitEta->SetInput(input,errInput);
	
	fitEta->minuitFitElec(1);
	
	fitEta->GetOutput(output,errOutput);
	
	chi2=0.;
	for(Int_t ii=0; ii<12; ii++)
	{
		chi2+=(input[ii]-output[ii])*(input[ii]-output[ii])/errInput[ii]/errInput[ii];
	}

	kk->secondIterElec(input,output,errInput,output2,derOut);
	
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
	
    return kTRUE;
}

Bool_t HHypPPEpEmGammaKine::execute()
{
	if (mylist->getNpart() != 4)
		return kFALSE;
	//needs 4 particles
	
//	Int_t hyppid[4];
//	HPidParticle *particle[4];
	TVector3 momentum[4];
	
	mylist->CombIteratorReset();
	while (mylist->CombIterator()) {
/*		for (int i = 0; i < (mylist->getNpart()); i++) {
			hyppid[i] = mylist->getPid(i);
			particle[i] = (HPidParticle *) mylist->getPidParticle(i);
			momentum[i] = mylist->getMomentum(i);
		}*/
	
		// Cuts copied from Refitter
//		if(qSpline1<0 || qSpline2<0 || qSpline3<0 || qSpline4<0) return kFALSE;
//		if(momSpline1>2900. ||momSpline2>2900. || momSpline3>2900. || momSpline4>2900.) return kFALSE;
//		if(theta3<0 || theta4<0) return kFALSE;

		
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
	
			// calculating missing mass
			TLorentzVector miss4 = (*beam) - (proton1 + proton2 + ep + em);
		
			Float_t miss_4=miss4.Mag();
//			cout << "Miss4: " << miss_4 << " MeV " << endl;
			if(miss_4<1e6 /*40000.0*/){// some cut copied from Refitter - stupid!!!!
				Float_t chi2, chi24;
				momentum[0]=proton1.Vect();
				momentum[1]=proton2.Vect();
				momentum[2]=em.Vect();
				momentum[3]=ep.Vect();
				if(DoTheRefit(momentum,// order in momentum: proton, proton, e-, e+
						idxProton1,idxProton2,idxElectron,idxPositron,chi2,chi24)){
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
					cout << "!!!!!error HHypPPEpEmGammaKine DoTheRefit() == kFALSE!!!!!! "  << endl;
				}
			}
				
//			miss4 = (*beam) - (proton1 + proton2 + ep + em);
//			miss_4=miss4.Mag();
//			cout << "Miss4: " << miss_4 << " MeV " << endl;
			
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
  return kTRUE;
}

Bool_t HHypPPEpEmGammaKine::reinit()
{
  return kTRUE;
}

Bool_t HHypPPEpEmGammaKine::finalize()
{
  return kTRUE;
}
