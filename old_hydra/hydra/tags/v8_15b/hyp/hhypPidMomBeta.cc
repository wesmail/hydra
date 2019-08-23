//*-- AUTHOR : M. Wisniowski 16.11.2007

// Check PIDs basing on momentum vs beta method. 
// if PIDs of tracks does not corespond to the hypothesis probAlg value for certain combination is set to 0 

using namespace std;

#include "hhypPidMomBeta.h"

#define c   299792458
#define R2D 57.2957795130823229
#define D2R 1.74532925199432955e-02

#define DEBUG 0

ClassImp(HHypPidMomBeta)

HHypPidMomBeta::HHypPidMomBeta(char *name_i, Option_t par[])
  :HHypBaseAlgorithm(name_i,par)
{
  cerr<<"HHypPidMomBeta::init()"<<endl;
}

HHypPidMomBeta::~HHypPidMomBeta()
{
}

Bool_t HHypPidMomBeta::execute()
{

    mylist->CombIteratorReset();
    Int_t ncomb = mylist->getNcomb();
    Int_t npart = mylist->getNpart();
    float tof_exp[npart];
    float tof_mom[npart];    // momentum depandent time of flight
	float tof_new[npart];
    float beta_new[npart];   // recalculated beta
	float track_lenght[npart];
	float RKmomentum[npart];
    float delta_tof[ncomb];
    float delta_tof_chi2[ncomb];
	bool  pid_check_OK[ncomb];
	TVector3 vect;
    TLorentzVector par(0,0,0,0);
	int icomb =-1;
    while (mylist->CombIterator())
    {
	    icomb++;
		float max = 0;
		int   best_comb=0;

        if (mylist->getIterStatus() == kTRUE)
        {
            HPidParticle  *PidPart;
            const HPidHitData   *pHit   = NULL;
            const HPidTrackData *pTrack = NULL;

			pid_check_OK[icomb]   = 1;
			delta_tof_chi2[icomb] = 0;
			delta_tof[icomb]      = 0;
				
            for(int ipart=0 ; ipart<npart; ipart++)
            {
			    tof_exp[ipart]=0;
				track_lenght[ipart]=0;
				tof_mom[ipart]=0;
				tof_new[ipart]=0;
				beta_new[ipart]=0;
				RKmomentum[ipart]=0;
					
				HCategory *pidpartCat =  gHades->getCurrentEvent()->getCategory(catPidPart);
                PidPart = (HPidParticle *) pidpartCat->getObject(mylist->getIdxPidPart(icomb, ipart));
                pHit   = PidPart->getHitData();
                pTrack = PidPart->getTrackData();

                bool system = (bool)pHit->iSystem;
                if( system==0 )      tof_exp[ipart] = pHit->fShowerTimeOfFlight;
                else if( system==1 ) tof_exp[ipart] = pHit->fTOFTimeOfFlight;
				
                RKmomentum[ipart] = pTrack->fMomenta[4];  //Runge Kutta momentum
				float Theta       = pTrack->getRKTheta();
				float Phi         = pTrack->getRKPhi(); 
				vect.SetXYZ(RKmomentum[ipart]*sin(Theta*D2R)*cos(Phi*D2R),
				            RKmomentum[ipart]*sin(Theta*D2R)*sin(Phi*D2R),
							RKmomentum[ipart]*cos(Theta*D2R)); 
                par.SetVectM(vect, HPidPhysicsConstants::mass(mylist->getPid(icomb, ipart)));
                track_lenght[ipart] = PidPart->getBetaExp() * c * tof_exp[ipart];
                tof_mom[ipart] = track_lenght[ipart]/(par.Beta()*c);
            }
				
            for(int ipart=1 ; ipart<npart; ipart++)
			{
			    float delta_tof = ( tof_exp[0] - tof_exp[ipart] ) / 2 ;
			    float tof_mean  = ( tof_mom[0] + tof_mom[ipart] ) / 2 ;
			    tof_new[ipart]  = tof_mean - delta_tof ; 
			    tof_new[0]     += (tof_mean + delta_tof)/((float)npart-1.) ; 
			}
			
            for(int ipart=0 ; ipart<npart; ipart++)
			{
				delta_tof_chi2[ icomb ] += pow( tof_new[ ipart ] - tof_mom[ ipart ], 2 );
            }
			
			if(DEBUG)								
			cerr<<" icomb : "<<icomb<<"("<<ncomb<<")   dtof : "<<delta_tof[icomb]<<"   dt_chi2 : "<<delta_tof_chi2[icomb]<<endl;
				
            for(int ipart=0 ; ipart<npart; ipart++)
            {
			    beta_new[ipart] = track_lenght[ipart] / ( c * tof_new[ipart] );
			    pid_check_OK[icomb] *= checkPID(RKmomentum[ipart], beta_new[ipart], mylist->getPid(icomb, ipart) );
													
				if(DEBUG)								
                cerr<<ipart<<"  tof_old : "<<tof_exp[ipart]<<"   tof_new : "<<tof_new[ipart]<<"   beta_new : "<<beta_new[ipart]<<endl;
            }
  
            if (histofile)
            for(int ipart=0 ; ipart<npart; ipart++)
            {
			    qa -> Fill(RKmomentum[ipart], beta_new[ipart], delta_tof_chi2[icomb],  pid_check_OK[icomb]);
 		    }
				
            if(DEBUG) 
			cerr<<" pid_check_OK : "<<pid_check_OK[icomb]<<endl<<endl;
				
		    mylist->setProbAlg(icomb, (mylist->getProbAlg(icomb))*pid_check_OK[icomb]);
        }
    }
	return kTRUE;
}// End Execute()

Bool_t HHypPidMomBeta::checkPID(float momentum, float beta, int ID)
{
    if(DEBUG)
	{
        cerr<<" checkPID :   momentum : "<<momentum<<"   beta : "<<beta<<"   ID : "<<ID
	        <<"  isInside : "<<p_CutG  ->IsInside(beta,momentum)<<endl;
	}

    switch(ID)
	{
	  case 14: if(p_CutG  ->IsInside(beta, momentum) ) return 1; else return 0;
	  case 8:  if(pip_CutG->IsInside(beta, momentum) ) return 1; else return 0;
	  case 9:  if(pip_CutG->IsInside(beta, momentum) ) return 1; else return 0;
	  default: return 1;
	}
}

Bool_t HHypPidMomBeta::init()
{

  if (paramFile.EndsWith("root") && TFile::Open(paramFile))
  {
     if( (p_CutG     = (TCutG*)gROOT->FindObject("p_mom_beta_cut")  )!=NULL &&
         (pip_CutG   = (TCutG*)gROOT->FindObject("pip_mom_beta_cut"))!=NULL    )
     {  gcutFlag=1;}
     else
     {
        cout<<endl<<"warinng : pip_mom_beta_cut does not exist"<<endl;
        gcutFlag=0;
     }
  }
  
  enLossCorr.setDefaultPar("jan04");
	  

  // need to get name from channel
  TString input(channel->Get(initList));
  if (histofile)
  {
    TFile *f=GetHFile();
    f->cd();
    qa = 
      new TNtuple(input + TString("_pid"), "mom:beta:delta_tof_chi2:pidOK",
                                           "mom:beta:delta_tof_chi2:pidOK");
  }
  return kTRUE;
}

Bool_t HHypPidMomBeta::reinit()
{
  return kTRUE;
}

Bool_t HHypPidMomBeta::finalize()
{

  if (histofile)
     qa->Write();

  return kTRUE;

}

Bool_t HHypPidMomBeta::SetParamFile(TString pFile)
{
    paramFile=pFile;
}
	
