//*-- Author : M. Wisniowski
//*-- Modified : 2005-9-13
//_HADES_CLASS_DESCRIPTION 
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
#define P_mass 938.272309999999998

ClassImp(HHypPPPi0Projector)

HHypPPPi0Projector::HHypPPPi0Projector(Char_t *name_i, Option_t par[])
  :HHypBaseAlgorithm(name_i,par)
{
    simuflag = 0;
}

HHypPPPi0Projector::~HHypPPPi0Projector()
{
}

Bool_t HHypPPPi0Projector::execute()
{
#if(1)
    
	Short_t triggerBit = gHades->getCurrentEvent()->getHeader()->getTBit();
    HEventHeader *evHeader = gHades->getCurrentEvent()->getHeader();

    UInt_t dsf = evHeader->getDownscaling();
    //UInt_t triggerDecision = evHeader->getTriggerDecision();

    if (!beam) 
	{
        cerr << algoName << " needs beam particle! " << endl;
        return kFALSE;
    }
  
    Float_t beta1=0, beta2=0, tof1=0, tof2=0, dist1=0, dist2=0;
	Float_t RKchiq1=-10, RKchiq2=-10;     // RK chi2 for track1 & track2
    Short_t sector1=-1, sector2=-1, system1=-1, system2=-1; // number of sector for first and secound particle

    Int_t p1_gk_parentID = 0, p1_gk_parentTrack = 0, p1_gk_ID = -100;
    Int_t p2_gk_parentID = 0, p2_gk_parentTrack = 0, p2_gk_ID = -100;
	Float_t genweight, p1_geninfo=-10, p2_geninfo=-10;
	Float_t dtof_chi2=100000000;
    Float_t best_dtof_chi2=100000000;
    Bool_t  isbest_dtof=0;
 
    // Resetting the list and start looping over the combinations
    // Loop is only done over the VALID combinations

    mylist->CombIteratorReset();
    while (mylist->CombIterator()) 
	{
	    if (!mylist->getUserValue(DELTATOF_CHI2, dtof_chi2))
		{
	        dtof_chi2 = -1;
		}
		if(dtof_chi2 < best_dtof_chi2 && dtof_chi2 != (-1) ) best_dtof_chi2 = dtof_chi2;
    }
	
    mylist->CombIteratorReset();
    Int_t ncomb=mylist->getNcomb();
	Int_t icomb=0;
    while (mylist->CombIterator()) 
	{
	    //cout<<"ncomb : "<<ncomb<<"   icomb : "<<icomb<<endl;
        //Getting the particles
        //TLorentzVector proton1 = mylist->getTLorentzVector("p", 1);
        //TLorentzVector proton2 = mylist->getTLorentzVector("p", 2);
        TLorentzVector proton1(0,0,0,0);
        TLorentzVector proton2(0,0,0,0);
						
	    if (!mylist->getUserValue(DELTATOF_CHI2, dtof_chi2))
		{
	        dtof_chi2 = -1;
		}

        if(dtof_chi2==best_dtof_chi2) isbest_dtof=1;

		//cout<<"dtof: "<<dtof<<"   chi2: "<<chi2<<endl;

        if (mylist->getIterStatus() == kTRUE) 
		{
		    HPidParticle  *PidPart = NULL;
		    HPidHitData   *PidData = NULL;
		    HPidTrackData *pTrack = NULL;
		    HPidTrackCand *PidTrackCand= NULL;
		    HRKTrackB     *RkTrack=NULL;
	  
            //-------------------- simulation ----------------------------------------------
            if (simuflag == 1 ) 
            {
                HPidParticleSim *my_p1 = (HPidParticleSim *) CatPartSim->
                                         getObject(mylist->getIdxPidPart(icomb, 0));
                HPidParticleSim *my_p2 = (HPidParticleSim *) CatPartSim->
                                         getObject(mylist->getIdxPidPart(icomb, 1));
                
		HPidGeantTrackSet *p1GeantSet = (HPidGeantTrackSet*) my_p1->getGeantTrackSet();
                HPidGeantTrackSet *p2GeantSet = (HPidGeantTrackSet*) my_p2->getGeantTrackSet();

                Int_t p1_geant_track = p1GeantSet->getGeantTrackID();
                Int_t p2_geant_track = p2GeantSet->getGeantTrackID();
	  
                if (p1_geant_track >= 0) 
                {
	                HGeantKine *p1_geantkine =
                                  (HGeantKine *) p1GeantSet->getGeantKine(p1_geant_track);
                    HGeantKine *p2_geantkine =
                                  (HGeantKine *) p2GeantSet->getGeantKine(p2_geant_track);

	                p1_gk_parentTrack = p1_geantkine->getParentTrack();
	                p2_gk_parentTrack = p2_geantkine->getParentTrack();
					p1_geantkine->getGenerator(p1_geninfo, genweight);
					p2_geantkine->getGenerator(p2_geninfo, genweight);
	    
	                HLinearCategory *myCatGeantKine = (HLinearCategory*)gHades->
                                               getCurrentEvent()->getCategory(catGeantKine);

 	                if (p1_gk_parentTrack > 0) 
                    {
		                HGeantKine * parent1 = (HGeantKine *)myCatGeantKine->getObject(p1_gk_parentTrack-1);
		                p1_gk_parentID = parent1->getID();
		            }
 	                if (p2_gk_parentTrack > 0) 
                    {
		                HGeantKine * parent2 = (HGeantKine *)myCatGeantKine->getObject(p2_gk_parentTrack-1);
		                p2_gk_parentID = parent2->getID();
		            }
	                p1_gk_ID  = p1_geantkine->getID();
	                p2_gk_ID  = p2_geantkine->getID();
                }
            }  // simuflag
            //-------------------- simulation end -------------------------------------------

            HCategory *pidpartCat =  gHades->getCurrentEvent()->getCategory(catPidPart);
            HCategory *trackCat   =  gHades->getCurrentEvent()->getCategory(catRKTrackB);

            if (pidpartCat != NULL && trackCat != NULL ) 
			{
                PidPart = (HPidParticle *) pidpartCat->getObject(mylist->getIdxPidPart(icomb, 0));
                if (PidPart != NULL) 
				{
	                PidData      = (HPidHitData*)   PidPart->getHitData();
	                pTrack       = (HPidTrackData*) PidPart->getTrackData();
			PidTrackCand = PidPart->getTrackCand();
			system1      = PidData->iSystem;
			sector1      = PidPart->sector();
	                beta1        = PidPart->getBetaExp();

                    if( PidPart->getMomAlg()==4 ) // Runge Kutta Alg
		            {
                        Int_t trackIndex = PidTrackCand->itsTrackData.nRKTrackInd;
                        RkTrack = (HRKTrackB *) trackCat->getObject(trackIndex);
                        RKchiq1 = RkTrack->getChiq();

						Float_t P  = pTrack->fMomenta[4];
						Float_t Th = RkTrack->getThetaSeg1RK();
						Float_t Ph = fmod(sector1*60. + RkTrack->getPhiSeg1RK()*TMath::RadToDeg(),360.)*TMath::DegToRad();

						TVector3       p1(P*sin(Th)*cos(Ph),P*sin(Th)*sin(Ph),P*cos(Th));
						proton1.SetVectM(p1,P_mass);
		            }	
		            if(      system1==0 )   tof1 = PidData->fShowerTimeOfFlight;                                     
		            else if( system1==1 )   tof1 = PidData->fTOFTimeOfFlight;
	            }
                PidPart = (HPidParticle *) pidpartCat->getObject(mylist->getIdxPidPart(icomb, 1));
                if (PidPart != NULL) {
	                PidData      = (HPidHitData*)   PidPart->getHitData();
	                pTrack       = (HPidTrackData*) PidPart->getTrackData();
			PidTrackCand = PidPart->getTrackCand();
			system2      = PidData->iSystem;
			sector2      = PidPart->sector();
			beta2        = PidPart->getBetaExp();

                    if( PidPart->getMomAlg()==4 ) // Runge Kutta Alg
		            {
                        Int_t trackIndex = PidTrackCand->itsTrackData.nRKTrackInd;
                        RkTrack = (HRKTrackB *) trackCat->getObject(trackIndex);
                        RKchiq2 = RkTrack->getChiq();
						    
						Float_t P  = pTrack->fMomenta[4];
						Float_t Th = RkTrack->getThetaSeg1RK();
						Float_t Ph = fmod(sector2*60. + RkTrack->getPhiSeg1RK()*TMath::RadToDeg(),360.)*TMath::DegToRad();

						TVector3       p2(P*sin(Th)*cos(Ph),P*sin(Th)*sin(Ph),P*cos(Th));
						proton2.SetVectM(p2,P_mass);
		            }	 
		            if(      system2==0 ) tof2 = PidData->fShowerTimeOfFlight;                                     
		            else if( system2==1 ) tof2 = PidData->fTOFTimeOfFlight;
	            }
            }
      
			if(simuflag == kFALSE)   // for real data beta recalculation (no start detector)
			{
	            dist1 = beta1*c*tof1;       // c ~= 0.3 m/ns
	            dist2 = beta2*c*tof2;       // [m]
                Float_t t = (tof1-tof2)/2;
	            tof1 = 7.5 + t;             // No START detector; need to recalculate beta
	            tof2 = 7.5 - t;             // 8.5 everage tof
	        
	            beta1 = dist1/(c*tof1);
	            beta2 = dist2/(c*tof2);
			}	
	  
            // calculating missing particle 4vector
            TLorentzVector pp_miss = (*beam) - (proton1 + proton2);  // beam = beam + target
			if(proton1.E()==0 || proton2.E()==0) {cout<<"empty proton"<<endl; continue;}
			
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
			
            if(simuflag==1)
			{
              Float_t fpp[]={ sector1, sector2, system1, system2,
			                  pp_miss.M2(), pp_miss.P(), fabs(proton1.Phi() - proton2.Phi()),
			                  tanTh1*tanTh2, pp_miss_cm.CosTheta(),
                              proton1.P(), proton1.Theta(), proton1.Phi(),
							  proton2.P(), proton2.Theta(), proton2.Phi(),
				  	          proton1_cm.P(), proton1_cm.Theta(), proton1_cm.Phi(),
							  proton2_cm.P(), proton2_cm.Theta(), proton2_cm.Phi(),
			                  pid1, pid2, beta1, beta2, 
							  dtof_chi2, isbest_dtof, RKchiq1, RKchiq2, ncomb, dsf, triggerBit,
							  p1_gk_parentTrack, p1_gk_parentID, p1_gk_ID, p1_geninfo, 
					          p2_gk_parentTrack, p2_gk_parentID, p2_gk_ID, p2_geninfo}; 
			
              pp->Fill(fpp);
			}
			else
			{
              Float_t fpp[]={ sector1, sector2, system1, system2,
			                  pp_miss.M2(), pp_miss.P(), fabs(proton1.Phi() - proton2.Phi()),
			                  tanTh1*tanTh2, pp_miss_cm.CosTheta(),
                              proton1.P(), proton1.Theta(), proton1.Phi(),
							  proton2.P(), proton2.Theta(), proton2.Phi(),
				  	          proton1_cm.P(), proton1_cm.Theta(), proton1_cm.Phi(),
							  proton2_cm.P(), proton2_cm.Theta(), proton2_cm.Phi(),
			                  pid1, pid2, beta1, beta2, 
							  dtof_chi2, isbest_dtof, RKchiq1, RKchiq2, ncomb, dsf, triggerBit};

              pp->Fill(fpp);
			}
		} else
        cerr << algoName << " got no TLorentzVector " << endl;
	    icomb++;
    }
	#endif
    return kTRUE;
}

Bool_t HHypPPPi0Projector::init()
{
    if (paramFile.EndsWith("root")) 
    {
        if(TFile::Open(paramFile))
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
	TFile *f=GetHFile();
	f->cd();
    if(simuflag==1)
    {
        pp =  new TNtuple(input + TString("_pp"),
                "sector1:sector2:system1:system2:miss_M2:miss_P:pp_dphi:tanT1tanT2:miss_cosThCM:P1:Th1:Phi1:P2:Th2:Ph2:P1CM:Th1CM:Phi1CM:P2CM:Th2CM:Phi2CM:pid1:pid2:beta1:beta2:dtof_chi2:isbest_dtof:RKchiq1:RKchiq2:ncomb:dsf:triggerBit:p1_gk_parentTrack:p1_gk_parentID:p1_gk_ID:p1_geninfo:p2_gk_parentTrack:p2_gk_parentID:p2_gk_ID:p2_geninfo", 
                "sector1:sector2:system1:system2:miss_M2:miss_P:pp_dphi:tanT1tanT2:miss_cosThCM:P1:Th1:Phi1:P2:Th2:Ph2:P1CM:Th1CM:Phi1CM:P2CM:Th2CM:Phi2CM:pid1:pid2:beta1:beta2:dtof_chi2::isbest_dtof:RKchiq1:RKchiq2:ncomb:dsf:triggerBit:p1_gk_parentTrack:p1_gk_parentID:p1_gk_ID:p1_geninfo:p2_gk_parentTrack:p2_gk_parentID:p2_gk_ID:p2_geninfo"); 
	}
	else
	{
        pp =  new TNtuple(input + TString("_pp"),
                "sector1:sector2:system1:system2:miss_M2:miss_P:pp_dphi:tanT1tanT2:miss_cosThCM:P1:Th1:Phi1:P2:Th2:Ph2:P1CM:Th1CM:Phi1CM:P2CM:Th2CM:Phi2CM:pid1:pid2:beta1:beta2:dtof_chi2:isbest_dtof:RKchiq1:RKchiq2:ncomb:dsf:triggerBit", 
                "sector1:sector2:system1:system2:miss_M2:miss_P:pp_dphi:tanT1tanT2:miss_cosThCM:P1:Th1:Phi1:P2:Th2:Phi2:P1CM:Th1CM:Phi1CM:P2CM:Th2CM:Phi2CM:pid1:pid2:beta1:beta2:dtof_chi2:isbest_dtof:RKchiq1:RKchiq2:ncomb:dsf:triggerBit" ); 
	}
	//f->Close();
    return kTRUE;
}

Bool_t HHypPPPi0Projector::reinit()
{
  return kTRUE;
}

Bool_t HHypPPPi0Projector::finalize()
{
  pp->Write();
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
    return kTRUE;
}
