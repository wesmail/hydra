//*-- AUTHOR : I. Froehlich
//*-- Modified : 21/04/2005 by I. Froehlich
//*-- Modified : 11/08/2005 by T. Perez      Added Geant ID to all the tracks.
//*-- Modified : 20/10/2005 by T. Perez      Sim All tracks have GeantID + Generator Info.
//               There is a DEBUG FLAG to minimize output. Maybe we can do that better.
//*-- Modified : 21/nov/2005 ... we can... later... --- Bjoern
//*-- Modified : 22/nov/2005 by T. Perez     DEBUG mesgs cleaned and reordered.
//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////
//
// HHypPPPipPimProjector
//
// Projects the events (input HHypList) to ntuple
// The file option in HHypReco->AddAlgorithm should be used
//
////////////////////////////////////////////////////////////////////////

using namespace std;

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "hevent.h"
#include "heventheader.h"
#include "hdetector.h"
#include "hratreeext.h"
#include "hcategory.h"
#include "hmatrixcategory.h"
#include "hlinearcategory.h"
#include "hlinearcatiter.h"
#include "hlocation.h"
#include "hiterator.h"
#include "hdebug.h"
#include "hades.h"

#include "hhypPPPipPimProjector.h"
#include "hhyplist.h"
#include "hhypchannel.h"
#include "hypinfodef.h"

#define DEBUG 0 // 1 a lot of cout with sim particle information.

ClassImp(HHypPPPipPimProjector)

  HHypPPPipPimProjector::HHypPPPipPimProjector(char *name_i, Option_t par[])
    :HHypBaseAlgorithm(name_i,par)
{
  simuflag = 0;
}

HHypPPPipPimProjector::~HHypPPPipPimProjector()
{
}

Bool_t HHypPPPipPimProjector::execute()
{
  // Reads the input particle(s) from the HHypList
  // Important: the "beam" has to be defined in the macro
  // The content of the ntuple is different in sim/exp
	
  //return kFALSE;
	
  if (!beam) {
    cerr << algoName << " needs beam particle! " << endl;
    return kFALSE;
  }
	
  HEventHeader *evHeader = gHades->getCurrentEvent()->getHeader();

	Int_t fakes;// Nur einmal pro Event
	m_pContItPart->Reset();
	fakes=0;
	while ( m_pContItPart->Next() != NULL) fakes++;
	fakes-=4;
	
  // Resetting the list and start looping over the combinations
  // Loop is only done over the VALID combinations
  mylist->CombIteratorReset();
  while (mylist->CombIterator()) {
	
    // Getting the particles
    TLorentzVector proton1 = mylist->getTLorentzVector("p", 1);
    TLorentzVector proton2 = mylist->getTLorentzVector("p", 2);
    TLorentzVector pip = mylist->getTLorentzVector("pi+", 1);
    TLorentzVector pim = mylist->getTLorentzVector("pi-", 1);
	
	if (mylist->getIterStatus() != kTRUE){
//		cerr << algoName << " got no TLorentzVector " << endl;
		continue;
	}
	
      // calculating missing mass
      TLorentzVector pp_miss = (*beam) - (proton1 + proton2);
      TLorentzVector miss4 = (*beam) - (proton1 + proton2 + pip + pim);
      TLorentzVector pippim_invmass = (pip + pim);
		
      // calculate pp pair
      TLorentzVector pp_invmass = (proton1 + proton2);
      
      //boost "Eta" into the CM to make angular distributions
      //make a copy to not disturb the other particles
      TLorentzVector ang_eta = (*beam) - (proton1 + proton2);
      ang_eta.Boost(-beam->BoostVector());
	
      //boost pp pair into pp rest frame
      //it does not matter which proton
      TLorentzVector ang_p = (proton1);
      ang_p.Boost(-pp_invmass.BoostVector());
      

      Double_t prob = mylist->getProbAlg();
		
      //std::cout << "got prob " << prob << std::endl;
		
      // this is for simulation :
		
      Int_t pip_id = 0;
      Int_t pim_id = 0;
      Int_t p1_id = 0;
      Int_t p2_id = 0;
      Float_t pip_geninfo = 0;
      Float_t pim_geninfo = 0;
      Float_t p1_geninfo = 0;
      Float_t p2_geninfo = 0;
			
			Float_t g_p1_mx=0, g_p1_my=0, g_p1_mz=0;
			Float_t g_p2_mx=0, g_p2_my=0, g_p2_mz=0;
			Float_t g_pip_mx=0, g_pip_my=0, g_pip_mz=0;
			Float_t g_pim_mx=0, g_pim_my=0, g_pim_mz=0;
	
      if (simuflag == 1) {

	HPidParticleSim *my_pip = (HPidParticleSim *) CatPartSim
	  ->getObject(mylist->getIdxPidPart("pi+", 1));
	HPidParticleSim *my_pim = (HPidParticleSim *) CatPartSim
	  ->getObject(mylist->getIdxPidPart("pi-", 1));
	HPidParticleSim *my_p1 = (HPidParticleSim *) CatPartSim
	  ->getObject(mylist->getIdxPidPart("p", 1));
	HPidParticleSim *my_p2 =(HPidParticleSim *) CatPartSim
	  ->getObject(mylist->getIdxPidPart("p", 2));

	if ((my_pip != NULL) && (my_pim != NULL)
	    && (mylist->getIterStatus() == kTRUE)) {        
	  HPidGeantTrackSet *pipGeantSet =(HPidGeantTrackSet*) my_pip->getGeantTrackSet();
	  HPidGeantTrackSet *pimGeantSet =(HPidGeantTrackSet*) my_pim->getGeantTrackSet();
	  HPidGeantTrackSet *p1GeantSet  =(HPidGeantTrackSet*) my_p1->getGeantTrackSet();
	  HPidGeantTrackSet *p2GeantSet  =(HPidGeantTrackSet*) my_p2->getGeantTrackSet();
	  // look only for the 1st geant track, makes life more easy  [Ingo]			
	  Int_t pip_geant_track = pipGeantSet->getGeantTrackID(0);
	  Int_t pim_geant_track = pimGeantSet->getGeantTrackID(0);
	  Int_t p1_geant_track = p1GeantSet->getGeantTrackID(0);
	  Int_t p2_geant_track = p2GeantSet->getGeantTrackID(0);
	  
	  if (pip_geant_track >= 0) {
	    HGeantKine *pip_geantkine =
	      (HGeantKine *) pipGeantSet->getGeantKine(pip_geant_track);
	    HGeantKine *pim_geantkine =
	      (HGeantKine *) pimGeantSet->getGeantKine(pim_geant_track);
	    HGeantKine *p1_geantkine =
	      (HGeantKine *) pipGeantSet->getGeantKine(p1_geant_track);
	    HGeantKine *p2_geantkine =
	      (HGeantKine *) pimGeantSet->getGeantKine(p2_geant_track);
		
	    //	    Float_t geninfo,
	    Float_t genweight;
				
	    Int_t pip_parentTrack = 0;
	    Int_t pim_parentTrack = 0;
	    Int_t p1_parentTrack = 0;
	    Int_t p2_parentTrack = 0;				       
		
	    pip_geantkine->getGenerator(pip_geninfo, genweight);
	    pim_geantkine->getGenerator(pim_geninfo, genweight);
	    p1_geantkine->getGenerator(p1_geninfo, genweight);
	    p2_geantkine->getGenerator(p2_geninfo, genweight);
				
	    pip_parentTrack = pip_geantkine->getParentTrack();
	    pim_parentTrack = pim_geantkine->getParentTrack();
	    p1_parentTrack = p1_geantkine->getParentTrack();
	    p2_parentTrack = p2_geantkine->getParentTrack();
			

					
	    // Particle ID
	    pip_id = pip_geantkine->getID();
	    pim_id = pim_geantkine->getID();
	    p1_id  = p1_geantkine->getID();
	    p2_id  = p2_geantkine->getID();
					
						// Kinematics
						p1_geantkine->getMomentum(g_p1_mx,g_p1_my,g_p1_mz);
						p2_geantkine->getMomentum(g_p2_mx,g_p2_my,g_p2_mz);
						pip_geantkine->getMomentum(g_pip_mx,g_pip_my,g_pip_mz);
						pim_geantkine->getMomentum(g_pim_mx,g_pim_my,g_pim_mz);
	    
		 ///////////////////////////////////////////////////////////////////
#if DEBUG==1	    
	    // DEBUG 
	    HLinearCategory *myCatGeantKine = (HLinearCategory*)gHades->getCurrentEvent()->getCategory(catGeantKine);
	    Int_t pip_parentParent = 0;
	    Float_t parent1genInfo = 0;
	    Float_t parent1genWeight=0;
	    Int_t tracks = 0;
	    Int_t pip_parentID = -1;
	      
	    // I define the max number of track as the higher trackNumber; 
	    // I don't know hoe to get the number of track in the event from geant.
	    // Tracks have numbers from 1->N (is not starting at 0 !!!).
	    if( pim_geant_track > tracks) tracks =  pim_geant_track;
	    if( pip_geant_track > tracks) tracks =  pip_geant_track;
	    if( p1_geant_track > tracks)  tracks =  p1_geant_track;
	    if( p2_geant_track > tracks)  tracks =  p2_geant_track;
											
	    if (pip_id== 8 && pim_id== 9 && p1_id == 14 && p2_id == 14) {
	      cout.setf(ios::fixed);
	      cout.setf(ios::showpoint);
	      cout.precision(0);
	      cout<<"\n \tID of the 4 particles : "<<endl;
	      cout<<"           \t Pi+ \t\t Pi- \t\t p1 \t\t P2"<<endl;
	      cout<<"ID         \t"<<pip_id<<"\t\t"<<pim_id<<"\t\t"<<p1_id<<"\t\t"<<p2_id<<endl;
	      cout<<"GeantTrack \t"<<pip_geant_track<<"\t\t"<< pim_geant_track<<"\t\t"<<p1_geant_track<<"\t\t"<<p2_geant_track<<endl;
	      cout<<"ParentTrack\t"<<pip_parentTrack<<"\t\t"<<pim_parentTrack <<"\t\t"<<p1_parentTrack<<"\t\t"<<p2_parentTrack<<endl;
	      cout<<"gen info p+ "<<pip_geninfo<<endl;
	      cout<<"gen info p- "<<pim_geninfo<<endl;
	      cout<<"gen info p1 "<<p1_geninfo<<endl;
	      cout<<"gen info p2 "<<p1_geninfo<<endl;
	      // parentID
	      if (pip_parentTrack > 0) {
		HGeantKine * parent1 = (HGeantKine *)myCatGeantKine->getObject(pip_parentTrack-1);
		pip_parentID = parent1->getID();
		pip_parentParent = parent1->getParentTrack();
		cout<<  "Parent of pi+ ID = "<<pip_parentID
		    <<"\nParentParent     = "<<pip_parentParent<<endl;
		if (pip_parentParent == 0) {
		  parent1->getGenerator(parent1genInfo, parent1genWeight);
		  cout<<"GeneratorInfo    = "<<parent1genInfo<<endl;
		}
	      }
	      if (pim_parentTrack > 0) {
		HGeantKine * parent1 = (HGeantKine *)myCatGeantKine->getObject(pim_parentTrack-1);
		pip_parentID = parent1->getID();
		pip_parentParent = parent1->getParentTrack();
		cout<<  "Parent of pi- ID = "<<pip_parentID
		    <<"\nParentParent     = "<<pip_parentParent<<endl;
		if (pip_parentParent == 0) {
		  parent1->getGenerator(parent1genInfo, parent1genWeight);
		  cout<<"GeneratorInfo    = "<<parent1genInfo<<endl;
		}
	      }
	      if (p1_parentTrack > 0) {
		HGeantKine * parent1 = (HGeantKine *)myCatGeantKine->getObject(p1_parentTrack-1);
		pip_parentID = parent1->getID();
		pip_parentParent = parent1->getParentTrack();
		cout<<  "Parent of proton1 ID = "<<pip_parentID
		    <<"\nParentParent     = "<<pip_parentParent<<endl;
		if (pip_parentParent == 0) {
		  parent1->getGenerator(parent1genInfo, parent1genWeight);
		  cout<<"GeneratorInfo    = "<<parent1genInfo<<endl;
		}
	      }
	      if (p2_parentTrack > 0) {
		HGeantKine * parent1 = (HGeantKine *)myCatGeantKine->getObject(p2_parentTrack-1);
		pip_parentID = parent1->getID();
		pip_parentParent = parent1->getParentTrack();
		cout<<  "Parent of proton2 ID = "<<pip_parentID
		    <<"\nParentParent     = "<<pip_parentParent<<endl;
		if (pip_parentParent == 0) {
		  parent1->getGenerator(parent1genInfo, parent1genWeight);
		  cout<<"GeneratorInfo    = "<<parent1genInfo<<endl;
		}
	      }
	      // End Parent ID + Source
	      for (Int_t i = 0 ; i<tracks; i++)
		cout<<"Particle("<<i+1<<") = "<<((HGeantKine *)myCatGeantKine->getObject(i))->getID()<<endl;
	    } // end if(pi+ &&pi- &&p1&& p2)
#endif
	  }
	}
      } // end of simulation part

			// The order MUST be the same than in the ntuple booking!!!
			Float_t temp[60];
			Int_t tmp_cnt;
			tmp_cnt=0;
	
			Float_t miss_pid;
			if (!mylist->getUserValue(FILLER_MISSING_PID, miss_pid)) miss_pid = -999;
			
			// Base
			temp[tmp_cnt++]=pp_miss.M2();
			temp[tmp_cnt++]=miss4.M2();
			temp[tmp_cnt++]=pippim_invmass.M2();
			temp[tmp_cnt++]=cos(ang_eta.Theta());
			temp[tmp_cnt++]=cos(ang_p.Theta());
			temp[tmp_cnt++]=prob;
			temp[tmp_cnt++]=miss_pid;
	
			// Trigger
			if(nt_trigger) {
				UInt_t downscalingFlag = evHeader->getDownscalingFlag();
				UInt_t triggerDecision = evHeader->getTriggerDecision();
				temp[tmp_cnt++]=downscalingFlag;
				temp[tmp_cnt++]=triggerDecision;
			}
				
			// Dtof and Kine extensions
			if(nt_dtof_refit){
				Float_t dtof, kine_chi2, kine_chi24;
				Float_t pid_tracks;
		
				if (!mylist->getUserValue(DELTATOF_DTOF, dtof))
					dtof = -1;
				if (!mylist->getUserValue(KINEFIT_CHI2, kine_chi2))
					kine_chi2 = -1;
				if (!mylist->getUserValue(KINEFIT_CHI24, kine_chi24))
					kine_chi24 = -1;
				if (!mylist->getUserValue(FILLER_VALID_PIDTRACKS, pid_tracks))
					pid_tracks = -1;
					
				temp[tmp_cnt++]=dtof;
				temp[tmp_cnt++]=pid_tracks;
				temp[tmp_cnt++]=kine_chi2;
				temp[tmp_cnt++]=kine_chi24;
				temp[tmp_cnt++]=fakes;
			}
	
			// Full Lorentz vectors for particles
			if(nt_full_lorentz){
				temp[tmp_cnt++]=proton1.X();
				temp[tmp_cnt++]=proton1.Y();
				temp[tmp_cnt++]=proton1.Z();
				temp[tmp_cnt++]=proton2.X();
				temp[tmp_cnt++]=proton2.Y();
				temp[tmp_cnt++]=proton2.Z();
				temp[tmp_cnt++]=pip.X();
				temp[tmp_cnt++]=pip.Y();
				temp[tmp_cnt++]=pip.Z();
				temp[tmp_cnt++]=pim.X();
				temp[tmp_cnt++]=pim.Y();
				temp[tmp_cnt++]=pim.Z();
			}
	
			// Simulation
			if (simuflag != 0){
				// Base geant
				temp[tmp_cnt++]=pip_id;
				temp[tmp_cnt++]=pim_id;
				temp[tmp_cnt++]=p1_id;
				temp[tmp_cnt++]=p2_id;
				temp[tmp_cnt++]=pip_geninfo;
				temp[tmp_cnt++]=pim_geninfo;
				temp[tmp_cnt++]=p1_geninfo;
				temp[tmp_cnt++]=p2_geninfo;

				if(nt_full_geant){
					temp[tmp_cnt++]=g_p1_mx;
					temp[tmp_cnt++]=g_p1_my;
					temp[tmp_cnt++]=g_p1_mz;
					temp[tmp_cnt++]=g_p2_mx;
					temp[tmp_cnt++]=g_p2_my;
					temp[tmp_cnt++]=g_p2_mz;
					temp[tmp_cnt++]=g_pip_mx;
					temp[tmp_cnt++]=g_pip_my;
					temp[tmp_cnt++]=g_pip_mz;
					temp[tmp_cnt++]=g_pim_mx;
					temp[tmp_cnt++]=g_pim_my;
					temp[tmp_cnt++]=g_pim_mz;
				}
			}
				
			miss->Fill(temp);
	}
		
	return kTRUE;
}

Bool_t HHypPPPipPimProjector::init()
{
	// Checks if we have sim/exp and book the ntuple
	
	nt_full_lorentz = (GetOpt("LORENTZ") != NULL);
	nt_dtof_refit = (GetOpt("DTOF_REFIT") != NULL);
	nt_trigger = (GetOpt("TRIGGER") != NULL);
	nt_full_geant = (GetOpt("FULL_GEANT") != NULL);

	simCat = gHades->getCurrentEvent()->getCategory(catGeantKine);

	if (!simCat){
		simuflag = 0;
	}else {
		simuflag = 1;
	
		CatPartSim = NULL;          // Category
		
		if ((CatPartSim =
				gHades->getCurrentEvent()->getCategory(catPidPart)) == NULL) {
			Error("init", "Cannot get catPidPartSim cat");
			return kFALSE;
		}	
	}

	// need to get name from channel
	TString input(channel->Get(initList));
	
	TString st_base("pp_miss:miss4:pippim_invmass:ppmiss_theta:pp_theta:fProbAlg:miss_pid");
	TString st_trigger(":downscalingFlag:triggerDecision");
	TString st_dtof_kine(":dtof:pidtr:kine_chi2:kine_chi24:fakes");
	TString st_full_lorentz(
		":p1_mx:p1_my:p1_mz"
		":p2_mx:p2_my:p2_mz"
		":pip_mx:pip_my:pip_mz"
		":pim_mx:pim_my:pim_mz"
	);
	
  // Create String
  if(nt_trigger) st_base+=st_trigger;
  if(nt_dtof_refit) st_base+=st_dtof_kine;
  if(nt_full_lorentz) st_base+=st_full_lorentz;
	
	if (simuflag != 0){
		// Add String for sims
		TString st_base_geant(":pip_id:pim_id:p1_id:p2_id"
				":pip_geninfo:pim_geninfo:p1_geninfo:p2_geninfo");
		TString st_full_geant(
			":g_p1_mx:g_p1_my:g_p1_mz"
			":g_p2_mx:g_p2_my:g_p2_mz"
			":g_ep_mx:g_ep_my:g_ep_mz"
			":g_em_mx:g_em_my:g_em_mz"
		);
	
		st_base+=st_base_geant;

		if(nt_full_geant) st_base+=st_full_geant;
	}
	miss = new TNtuple(input + TString("_had_proj"), "Masses", st_base);
	cout << "--- " << input <<" PROJECTOR is using ---\n" << st_base << endl;
	

	//---------- Initialization of HPidParticle Container -----------------------
	m_pContItPart = NULL;         // Iterator
	
	HCategory      *m_pContCatPart= NULL;        // Category
	
	if ((m_pContCatPart =
		gHades->getCurrentEvent()->getCategory(catPidPart)) == NULL) {
		Error("init", "Cannot get catPidPart cat");
		return kFALSE;
	}
	m_pContItPart = (HIterator *) m_pContCatPart->MakeIterator();
	//-----------------------------------------------------------------------
  
  return kTRUE;
}

Bool_t HHypPPPipPimProjector::reinit()
{
  return kTRUE;
}

Bool_t HHypPPPipPimProjector::finalize()
{
  miss->Write();
  return kTRUE;
}
