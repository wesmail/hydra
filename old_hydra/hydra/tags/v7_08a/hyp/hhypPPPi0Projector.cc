//*-- Author : M. Wisniowski
//*-- Modified : 2005-9-13
////////////////////////////////////////////////////////////////////////
//
// HHypPPPi0Projector
//
// HHypPPPi0Projector projects any PP data. At the moment output contains
//
////////////////////////////////////////////////////////////////////////
 

using namespace std;

#include "hhypPPPi0Projector.h"

#define c 0.299792

ClassImp(HHypPPPi0Projector)

HHypPPPi0Projector::HHypPPPi0Projector(char *name_i, Option_t par[])
  :HHypBaseAlgorithm(name_i,par)
{
    simuflag = 0;
}

HHypPPPi0Projector::~HHypPPPi0Projector()
{
}

Bool_t HHypPPPi0Projector::execute()
{
    Short_t triggerBit = gHades->getCurrentEvent()->getHeader()->getTBit();

    if (!beam) 
	{
        cerr << algoName << " needs beam particle! " << endl;
        return kFALSE;
    }
    //cout<<"E beam + target = "<<beam->E()<<endl;
    // Resetting the list and start looping over the combinations
    // Loop is only done over the VALID combinations
  
    Float_t beta1=0, beta2=0, tof1=0, tof2=0, dist1=0, dist2=0;
    Short_t sector1=0, sector2=0; // number of sector for first and secound particle
  
    mylist->CombIteratorReset();
    while (mylist->CombIterator()) 
	{
        // Getting the particles
        TLorentzVector proton1 = mylist->getTLorentzVector("p", 1);
        TLorentzVector proton2 = mylist->getTLorentzVector("p", 2);

        if (mylist->getIterStatus() == kTRUE) 
		{
            HPidParticle  *PidPart = NULL;
	        HPidHitData   *PidData = NULL;
	        HPidTrackData *PidTrack = NULL;
	        HPidTrackCand *PidTrackCand= NULL;
            HRKTrackB     *RkTrack=NULL;
	  
            HCategory *pidpartCat =  gHades->getCurrentEvent()->getCategory(catPidPart);
            HCategory *trackCat   =  gHades->getCurrentEvent()->getCategory(catRKTrackB);

            if (pidpartCat != NULL) 
			{
                PidPart = (HPidParticle *) pidpartCat->getObject(mylist->getIdxPidPart(0, 0));
                if (PidPart != NULL) 
				{
	                beta1        = PidPart->getBetaExp();
		            sector1      = PidPart->sector();
	                PidData      = PidPart->getHitData();
		            PidTrack     = PidPart->getTrackData();
		            PidTrackCand = PidPart->getTrackCand();

                    if( PidPart->getMomAlg()==4 ) // Runge Kutta Alg
		            {
                        Int_t trackIndex = PidTrackCand->itsTrackData.nRKTrackInd;
                        RkTrack = (HRKTrackB *) trackCat->getObject(trackIndex);
          
                        if(RkTrack->getChiq() != 1000000)
                        {
                            proton1.SetTheta( RkTrack->getThetaSeg1RK() );           
                            proton1.SetPhi( fmod(sector1*60. + RkTrack->getPhiSeg1RK()*TMath::RadToDeg(),360.)*TMath::DegToRad() );
                        }
		           }	
		           if(      (PidData->iSystem)==0 )   tof1 = PidData->fShowerTimeOfFlight;                                     
		           else if( (PidData->iSystem)==1 )   tof1 = PidData->fTOFTimeOfFlight;
	            }
                PidPart = (HPidParticle *) pidpartCat->getObject(mylist->getIdxPidPart(0, 1));
                if (PidPart != NULL) {
	                beta2        = PidPart->getBetaExp();
		            sector2      = PidPart->sector();
	                PidData      = PidPart->getHitData();
		            PidTrack     = PidPart->getTrackData();
		            PidTrackCand = PidPart->getTrackCand();

                    Int_t trackIndex = PidTrackCand->itsTrackData.nRKTrackInd;
                    RkTrack = (HRKTrackB *) trackCat->getObject(trackIndex);
         
                    if( PidPart->getMomAlg()==4 ) // Runge Kutta Alg
		            {
                        if(RkTrack->getChiq() != 1000000)
                        {
                            proton2.SetTheta( RkTrack->getThetaSeg1RK() );           
                            proton2.SetPhi( fmod(sector2*60. + RkTrack->getPhiSeg1RK()*TMath::RadToDeg(),360.)*TMath::DegToRad() );
                        }
		            }	  
		            if(      (PidData->iSystem)==0 ) tof2 = PidData->fShowerTimeOfFlight;                                     
		            else if( (PidData->iSystem)==1 ) tof2 = PidData->fTOFTimeOfFlight;
	            }
            }
      
			if(simuflag == kFALSE)   // for real data beta recalculation (no start detector)
			{
	            dist1 = beta1*c*tof1;  // c ~= 0.3 m/ns
	            dist2 = beta2*c*tof2;  // [m]
                Float_t t = (tof1-tof2)/2;
	            tof1 = 8.5 + t;       // No START detector; need to recalculate beta
	            tof2 = 8.5 - t;       // 8.5 everage tof
	        
	            beta1 = dist1/(c*tof1);
	            beta2 = dist2/(c*tof2);
			}	
	  
            // calculating missing particle 4vector
            TLorentzVector pp_miss = (*beam) - (proton1 + proton2);  // beam = beam + target
			
            // 4vectors in CM system
			TLorentzVector pp_miss_cm = pp_miss; 
            TLorentzVector proton1_cm = proton1; 
            TLorentzVector proton2_cm = proton2; 
            pp_miss_cm.Boost(0.0, 0.0, -(*beam).Beta() ); 
			proton1_cm.Boost(0.0, 0.0, -(*beam).Beta() );
			proton2_cm.Boost(0.0, 0.0, -(*beam).Beta() );
	 
	        Short_t pid1=0, pid2=0;
            if( gcutFlag==1 )
	        {
	            if(  pCutG->IsInside(beta1,proton1.P())) { pid1+=14; }
	            if(pipCutG->IsInside(beta1,proton1.P())) { pid1+=8;  }
	            if(  pCutG->IsInside(beta2,proton2.P())) { pid2+=14; }
	            if(pipCutG->IsInside(beta2,proton2.P())) { pid2+=8;  }
	        }

            Float_t tanTh1 = TMath::Tan(proton1.Theta());
            Float_t tanTh2 = TMath::Tan(proton2.Theta());

            miss->Fill(pp_miss.M2(),pp_miss.P(), fabs(proton1.Phi() - proton2.Phi()), tanTh1*tanTh2, pp_miss_cm.CosTheta(), triggerBit,
	                   pid1, pid2);
            mom_beta->Fill(proton1.P(), proton2.P(), beta1, beta2, pid1, pid2, triggerBit);
            base->Fill(proton1.Px(),proton1.Py(),proton1.Pz(),
	                   proton2.Px(),proton2.Py(),proton2.Pz(),
					   proton1_cm.Px(),proton1_cm.Py(),proton1_cm.Pz(),
					   proton2_cm.Px(),proton2_cm.Py(),proton2_cm.Pz(),
					   pid1,pid2,triggerBit);
        } else
        cerr << algoName << " got no TLorentzVector " << endl;
    }
    return kTRUE;
}

Bool_t HHypPPPi0Projector::init()
{
    if (paramFile.EndsWith("root")) 
    {
        if(TFile *f = TFile::Open(paramFile))
	    {
            if((pCutG   = (TCutG*)gROOT->FindObject("p_mom_beta_cut")  )!=NULL &&
	          ( pipCutG = (TCutG*)gROOT->FindObject("pip_mom_beta_cut"))!=NULL  )  gcutFlag=1;
		    else
		    {
		        cout<<endl<<"warinng : pip_mom_beta_cut does not exist"<<endl;
			    gcutFlag=0;
		    }
	    }
    }
	
    simCat = gHades->getCurrentEvent()->getCategory(catGeantKine);

    if (!simCat) 
	{
        simuflag = 0;
    }
	else 
	{
        simuflag = 1;
        //cout << "Projector uses SIMULATION" << endl;

        CatPartSim = NULL;          // Category

        if ((CatPartSim =
             gHades->getCurrentEvent()->getCategory(catPidPart)) == NULL) {
            Error("init", "Cannot get catPidPartSim cat");
            return kFALSE;
        }
    }

    // need to get name from channel
    TString input(channel->Get(initList));

    miss =
      new TNtuple(input + TString("_proj"),
                "pp_miss_mass:pp_miss_mom:pp_dphi:tanT1tanT2:miss_cosThCM:triggerBit:pid1:pid2",
                "pp_miss_mass:pp_miss_mom:pp_dphi:tanT1tanT2:miss_cosThCM:triggerBit:pid1:pid2");
				
    mom_beta = 
      new TNtuple(input + TString("_mom"),
	            "mom1:mom2:beta1:beta2:pid1:pid2:triggerBit",
				"mom1:mom2:beta1:beta2:pid1:pid2:triggerBit");

    base =
      new TNtuple(input + TString("_base"),
	            "px1:py1:pz1:px2:py2:pz2:px1CM:py1CM:pz1CM:px2CM:py2CM:pz2CM:pid1:pid2:triggerBit",
  	            "px1:py1:pz1:px2:py2:pz2:px1CM:py1CM:pz1CM:px2CM:py2CM:pz2CM:pid1:pid2:triggerBit");
  
    return kTRUE;
}

Bool_t HHypPPPi0Projector::reinit()
{
  return kTRUE;
}

Bool_t HHypPPPi0Projector::finalize()
{
  miss->Write();
  mom_beta->Write();
  base->Write();
  // delete miss ???
  return kTRUE;
}

Bool_t HHypPPPi0Projector::IsOpposit(Short_t sec1, Short_t sec2)
{
  if(sec1==0 && sec2==3) return 1; else
  if(sec1==1 && sec2==4) return 1; else
  if(sec1==2 && sec2==5) return 1; else
  if(sec1==3 && sec2==0) return 1; else
  if(sec1==4 && sec2==1) return 1; else
  if(sec1==5 && sec2==2) return 1; else
  return 0;
}
			  
Bool_t HHypPPPi0Projector::SetParamFile(TString pFile)
{
  paramFile=pFile;
}
