//*-- Author : B. Spruck
//*-- Modified : 30.11.2006
//_HADES_CLASS_DESCRIPTION
////////////////////////////////////////////////////////////////////////
//
// HHypPPEpEmProjector
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

#include "hhypPPEpEmProjector.h"
#include "hhyplist.h"
#include "hhypchannel.h"
#include "hypinfodef.h"

#define DEBUG 0 // 1 a lot of cout with sim particle information.

ClassImp(HHypPPEpEmProjector)

HHypPPEpEmProjector::HHypPPEpEmProjector(Char_t *name_i, Option_t par[])
:HHypBaseAlgorithm(name_i,par)
{
	simuflag = 0;
}

HHypPPEpEmProjector::~HHypPPEpEmProjector()
{
}

Bool_t HHypPPEpEmProjector::execute()
{
	// Reads the input particle(s) from the HHypList
	// Important: the "beam" has to be defined in the macro
	// The content of the ntuple is different in sim/exp
	
	if (!beam) {
		cerr << algoName << " needs beam particle! " << endl;
		return kFALSE;
	}
		
	HEventHeader *evHeader = gHades->getCurrentEvent()->getHeader();
	
	// Resetting the list and start looping over the combinations
	// Loop is only done over the VALID combinations
	mylist->CombIteratorReset();
	while (mylist->CombIterator()) {
		Float_t fakes;// Once it was one time per Event, now it might be different for every combination

      fakes=mylist->getNrOfFakes();
	
		// Getting the particles
		TLorentzVector proton1 = mylist->getTLorentzVector("p", 1);
		TLorentzVector proton2 = mylist->getTLorentzVector("p", 2);
		TLorentzVector ep = mylist->getTLorentzVector("e+", 1);
		TLorentzVector em = mylist->getTLorentzVector("e-", 1);

		if (mylist->getIterStatus() != kTRUE){
			// seems the particles which I want are not inside
			// maybe another reaction?
			continue;
		}

		Double_t ep_beta, em_beta;
		ep_beta=mylist->getBeta("e+",1);
		em_beta=mylist->getBeta("e-",1);
	
		Double_t prob = mylist->getProbAlg();
		Float_t p1_in_chi2, p2_in_chi2;
		Float_t ep_in_chi2, em_in_chi2;
      Float_t p1_MdcR, p1_MdcZ, p1_DistVertex;
      Float_t p2_MdcR, p2_MdcZ, p2_DistVertex;
      Float_t ep_MdcR, ep_MdcZ, ep_DistVertex;
      Float_t em_MdcR, em_MdcZ, em_DistVertex;
	
		Double_t opang= ep.Vect().Angle(em.Vect());
		//cout << "opang: " << opang << endl;
	
		ep_in_chi2=-1;
		em_in_chi2=-1;
		p1_in_chi2=-1;
		p2_in_chi2=-1;
      p1_DistVertex=-1;
      p2_DistVertex=-1;
      ep_DistVertex=-1;
      em_DistVertex=-1;
	
		{
			HCategory *pidPartCandCat = gHades->getCurrentEvent()->getCategory(catPidTrackCand);
			if (pidPartCandCat != NULL) {
				HPidTrackCand *pPidTrackCand;
				pPidTrackCand =(HPidTrackCand *) pidPartCandCat->getObject(mylist->getIdxPidTrackCand("e+", 1));
				if (pPidTrackCand != NULL) {
					HPidHitData const *hit;
					hit=pPidTrackCand->getHitData();
					if(hit){
						ep_in_chi2=hit->fInnerMdcChiSquare;
//							ep_DistVertex=hit->getDistanceToVertex(ALG_RUNGEKUTTA);
//							ep_MdcR=hit->getMdcRCoord();
//                     ep_MdcZ=hit->getMdcZCoord();
						ep_DistVertex=hit->fDistanceToVertex[ALG_RUNGEKUTTA];
						ep_MdcR=hit->fMdcRCoord;
						ep_MdcZ=hit->fMdcZCoord;
					}else{
						Error("execute","no e+ pidHitData");
					}
/*					if(ep_MdcR!=pPidTrackCand->getR() || ep_MdcZ!=pPidTrackCand->getZ() ){
						cerr <<"jammer e+"<<endl;
					}*/
				}else{
					Error("execute","no e+ pidParticle");
				}
				pPidTrackCand =(HPidTrackCand *) pidPartCandCat->getObject(mylist->getIdxPidTrackCand("e-", 1));
				if (pPidTrackCand != NULL) {
					HPidHitData const *hit;
					hit=pPidTrackCand->getHitData();
					if(hit){
						em_in_chi2=hit->fInnerMdcChiSquare;
						em_DistVertex=hit->fDistanceToVertex[ALG_RUNGEKUTTA];
						em_MdcR=hit->fMdcRCoord;
						em_MdcZ=hit->fMdcZCoord;
					}else{
						Error("execute","no e- pidHitData");
					}
/*					if(em_MdcR!=pPidTrackCand->getR() || em_MdcZ!=pPidTrackCand->getZ() ){
						cerr <<"jammer e-"<<endl;
					}*/
				}else{
					Error("execute","no e- pidParticle");
				}
				
				pPidTrackCand =(HPidTrackCand *) pidPartCandCat->getObject(mylist->getIdxPidTrackCand("p", 1));
				if (pPidTrackCand != NULL) {
					HPidHitData const *hit;
					hit=pPidTrackCand->getHitData();
					if(hit){
						p1_in_chi2=hit->fInnerMdcChiSquare;
						p1_DistVertex=hit->fDistanceToVertex[ALG_RUNGEKUTTA];
						p1_MdcR=hit->fMdcRCoord;
						p1_MdcZ=hit->fMdcZCoord;
					}else{
						Error("execute","no p pidHitData");
					}
/*					if(p1_MdcR!=pPidTrackCand->getR() || p1_MdcZ!=pPidTrackCand->getZ() ){
						cerr <<"jammer p1"<<endl;
					}*/
				}else{
					Error("execute","no p pidParticle");
				}
				
				pPidTrackCand =(HPidTrackCand *) pidPartCandCat->getObject(mylist->getIdxPidTrackCand("p", 2));
				if (pPidTrackCand != NULL) {
					HPidHitData const *hit;
					hit=pPidTrackCand->getHitData();
					if(hit){
						p2_in_chi2=hit->fInnerMdcChiSquare;
						p2_DistVertex=hit->fDistanceToVertex[ALG_RUNGEKUTTA];
						p2_MdcR=hit->fMdcRCoord;
						p2_MdcZ=hit->fMdcZCoord;
					}else{
						Error("execute","no p pidHitData");
					}
/*					if(p2_MdcR!=pPidTrackCand->getR() || p2_MdcZ!=pPidTrackCand->getZ() ){
						cerr <<"jammer p2"<<endl;
					}*/
				}else{
					Error("execute","no p pidParticle");
				}
			}else{
				Error("execute","no pidPartCandCat");
			}
		}
	
		if (mylist->getIterStatus() == kTRUE) {
		
			// calculating missing mass
			TLorentzVector pp_miss = (*beam) - (proton1 + proton2);
			TLorentzVector miss4 = (*beam) - (proton1 + proton2 + ep + em);
			TLorentzVector epem_invmass = (ep + em);
			
			//boost "Eta" into the CM to make angular distributions
			//make a copy to not disturb the other particles
			TLorentzVector ang_eta = (*beam) - (proton1 + proton2);
			ang_eta.Boost(-beam->BoostVector());
	
			//next step is to extract the helicity angle
			TLorentzVector ang_ep  = (TLorentzVector)ep;
			TLorentzVector ang_em  = (TLorentzVector)em;
	
			//first boost into CM
			ang_ep.Boost(-beam->BoostVector());
			ang_em.Boost(-beam->BoostVector());
			
			//Rotate such that eta points to beam axis
			Double_t ang_phi = ang_eta.Phi();
			Double_t ang_theta = ang_eta.Theta();
			ang_eta.RotateZ(-ang_phi);
			ang_eta.RotateY(-ang_theta);
			ang_ep.RotateZ(-ang_phi);
			ang_ep.RotateY(-ang_theta);
			ang_em.RotateZ(-ang_phi);
			ang_em.RotateY(-ang_theta);
	
			//boost into eta rest system
			ang_ep.Boost(-ang_eta.BoostVector());
			ang_em.Boost(-ang_eta.BoostVector());
			ang_eta.Boost(-ang_eta.BoostVector());
			
			//Rotate such that gamma points to beam axis
			TLorentzVector virt_ph = ang_ep + ang_em;
			ang_phi = virt_ph.Phi();
			ang_theta = virt_ph.Theta();
			ang_ep.RotateZ(-ang_phi);
			ang_ep.RotateY(-ang_theta);
			ang_em.RotateZ(-ang_phi);
			ang_em.RotateY(-ang_theta);
			virt_ph.RotateZ(-ang_phi);
			virt_ph.RotateY(-ang_theta);
			
			//boost into gamma rest system
			ang_ep.Boost(-virt_ph.BoostVector());
			ang_em.Boost(-virt_ph.BoostVector());
			
			//check for decay angle
			Double_t ang_alpha;
			if (ang_ep.Theta() < ang_em.Theta())
				ang_alpha = fabs(cos(ang_ep.Theta()));
			else
				ang_alpha = fabs(cos(ang_em.Theta()));
	
			// this is for simulation :
			
			Int_t ep_id = 0;
			Int_t em_id = 0;
			Int_t p1_id = 0;
			Int_t p2_id = 0;
			Float_t ep_geninfo = 0;
			Float_t em_geninfo = 0;
			Float_t p1_geninfo = 0;
			Float_t p2_geninfo = 0;
			Int_t ep_parentTrack = 0;
			Int_t em_parentTrack = 0;
			Int_t p1_parentTrack = 0;
			Int_t p2_parentTrack = 0;
			Int_t ep_creation=0;
			Int_t em_creation=0;
			Float_t ep_vx=0, ep_vy=0, ep_vz=0;
			Float_t em_vx=0, em_vy=0, em_vz=0;

#if 1
			if (simuflag == 1) {
		
				//	cerr<<"\n\nsimuflag = 1"<<endl;
					
				HPidTrackCandSim *my_ep = (HPidTrackCandSim *)
					CatTrackCandSim->getObject(mylist->getIdxPidTrackCand("e+", 1));
				HPidTrackCandSim *my_em = (HPidTrackCandSim *)
					CatTrackCandSim->getObject(mylist->getIdxPidTrackCand("e-", 1));
				HPidTrackCandSim *my_p1 = (HPidTrackCandSim *)
					CatTrackCandSim->getObject(mylist->getIdxPidTrackCand("p", 1));
				HPidTrackCandSim *my_p2 = (HPidTrackCandSim *)
					CatTrackCandSim->getObject(mylist->getIdxPidTrackCand("p", 2));
	
				if ((my_ep != NULL) && (my_em != NULL)
						&& (mylist->getIterStatus() == kTRUE)) {
					HPidGeantTrackSet *epGeantSet = my_ep->getGeantTrackSet();
					HPidGeantTrackSet *emGeantSet = my_em->getGeantTrackSet();
					HPidGeantTrackSet *p1GeantSet = my_p1->getGeantTrackSet();
					HPidGeantTrackSet *p2GeantSet = my_p2->getGeantTrackSet();
									
					// look only for the 1st geant track, makes life more easy  [Ingo]
					Int_t ep_geant_track = epGeantSet->getGeantTrackID(0);
					Int_t em_geant_track = emGeantSet->getGeantTrackID(0);
					Int_t p1_geant_track = p1GeantSet->getGeantTrackID(0);
					Int_t p2_geant_track = p2GeantSet->getGeantTrackID(0);
								
					if (ep_geant_track >= 0) {
						HGeantKine *ep_geantkine =
							(HGeantKine *) epGeantSet->getGeantKine(ep_geant_track);
						HGeantKine *em_geantkine =
							(HGeantKine *) emGeantSet->getGeantKine(em_geant_track);
						HGeantKine *p1_geantkine =
							(HGeantKine *) epGeantSet->getGeantKine(p1_geant_track);
						HGeantKine *p2_geantkine =
							(HGeantKine *) emGeantSet->getGeantKine(p2_geant_track);
							
						//	    Float_t geninfo,
						Float_t genweight;
						Int_t dummy;
										
						ep_geantkine->getGenerator(ep_geninfo, genweight);
						em_geantkine->getGenerator(em_geninfo, genweight);
						p1_geantkine->getGenerator(p1_geninfo, genweight);
						p2_geantkine->getGenerator(p2_geninfo, genweight);
				
						// Parent Track
						ep_parentTrack = ep_geantkine->getParentTrack();
						em_parentTrack = em_geantkine->getParentTrack();
						p1_parentTrack = p1_geantkine->getParentTrack();
						p2_parentTrack = p2_geantkine->getParentTrack();
														
						// Particle ID
						ep_id = ep_geantkine->getID();
						em_id = em_geantkine->getID();
						p1_id = p1_geantkine->getID();
						p2_id = p2_geantkine->getID();
				
						// How are e+e- produced
						ep_geantkine->getCreator(dummy,dummy,ep_creation);
						em_geantkine->getCreator(dummy,dummy,em_creation);
						ep_geantkine->getVertex(ep_vx,ep_vy,ep_vz);
						em_geantkine->getVertex(em_vx,em_vy,em_vz);
						///////////////////////////////////////////////////////////////////
#if DEBUG==1
						// DEBUG
						HLinearCategory *myCatGeantKine = (HLinearCategory*)gHades->getCurrentEvent()->getCategory(catGeantKine);
						Int_t parentParent = 0;
						Float_t parent1genInfo = 0, parent1genWeight=0;
						Int_t tracks = 0;
						Int_t parentID = -1;
				
						// I define the max number of track as the higher trackNumber;
						// I don't know hoe to get the number of track in the event from geant.
						// Tracks have numbers from 1->N (is not starting at 0 !!!).
						if( em_geant_track > tracks) tracks =  em_geant_track;
						if( ep_geant_track > tracks) tracks =  ep_geant_track;
						if( p1_geant_track > tracks)  tracks =  p1_geant_track;
						if( p2_geant_track > tracks)  tracks =  p2_geant_track;

						if (ep_id== 2 && em_id== 3 && p1_id == 14 && p2_id == 14) {
							cerr.setf(ios::fixed);
							cerr.setf(ios::showpoint);
							cerr.precision(0);
							cerr<<"\n \tID of the 4 particles : "<<endl;
							cerr<<"           \t E+ \t\t E- \t\t p1 \t\t p2"<<endl;
							cerr<<"ID         \t"<<ep_id<<"\t\t"<<em_id<<"\t\t"<<p1_id<<"\t\t"<<p2_id<<endl;
							cerr<<"GeantTrack \t"<<ep_geant_track<<"\t\t"<< em_geant_track<<"\t\t"<<p1_geant_track<<"\t\t"<<p2_geant_track<<endl;
							cerr<<"ParentTrack\t"<<ep_parentTrack<<"\t\t"<<em_parentTrack <<"\t\t"<<p1_parentTrack<<"\t\t"<<p2_parentTrack<<endl;
							cerr<<"gen info e+ "<<ep_geninfo<<endl;
							cerr<<"gen info e- "<<em_geninfo<<endl;
							cerr<<"gen info p1 "<<p1_geninfo<<endl;
							cerr<<"gen info p2 "<<p1_geninfo<<endl;
							// parentID
							if (ep_parentTrack > 0) {
								HGeantKine * parent1 = (HGeantKine *)myCatGeantKine->getObject(ep_parentTrack-1);
								parentID = parent1->getID();
								parentParent = parent1->getParentTrack();
								cerr<<  "Parent of e+ ID = "<<parentID
									<<"\nParentParent     = "<<parentParent<<endl;
								if (parentParent == 0) {
									parent1->getGenerator(parent1genInfo, parent1genWeight);
									cerr<<"GeneratorInfo    = "<<parent1genInfo<<endl;
								}
							}
			
							if (em_parentTrack > 0) {
								HGeantKine * parent1 = (HGeantKine *)myCatGeantKine->getObject(em_parentTrack-1);
								parentID = parent1->getID();
								parentParent = parent1->getParentTrack();
								cerr<<  "Parent of e- ID = "<<parentID
									<<"\nParentParent     = "<<parentParent<<endl;
								if (parentParent == 0) {
									parent1->getGenerator(parent1genInfo, parent1genWeight);
									cerr<<"GeneratorInfo    = "<<parent1genInfo<<endl;
								}
							}
	
							if (p1_parentTrack > 0) {
								HGeantKine * parent1 = (HGeantKine *)myCatGeantKine->getObject(p1_parentTrack-1);
								parentID = parent1->getID();
								parentParent = parent1->getParentTrack();
								cerr<<  "Parent of proton1 ID = "<<parentID
									<<"\nParentParent     = "<<parentParent<<endl;
								if (parentParent == 0) {
									parent1->getGenerator(parent1genInfo, parent1genWeight);
									cerr<<"GeneratorInfo    = "<<parent1genInfo<<endl;
								}
							}
	
							if (p2_parentTrack > 0) {
								HGeantKine * parent1 = (HGeantKine *)myCatGeantKine->getObject(p2_parentTrack-1);
								parentID = parent1->getID();
								parentParent = parent1->getParentTrack();
								cerr<<  "Parent of proton2 ID = "<<parentID
									<<"\nParentParent     = "<<parentParent<<endl;
								if (parentParent == 0) {
									parent1->getGenerator(parent1genInfo, parent1genWeight);
									cerr<<"GeneratorInfo    = "<<parent1genInfo<<endl;
								}
							}
							// End Parent ID + Source
							for (Int_t i = 0 ; i<tracks; i++)
								cerr<<"Particle("<<i<<") = "<<((HGeantKine *)myCatGeantKine->getObject(i))->getID()<<endl;
						} // end if(e+ &&e- &&p1&& p2)
#endif
					}
				}
			} /* end for simu */
#endif
			// The order MUST be the same than in the ntuple booking!!!
			Float_t temp[75];
			Int_t tmp_cnt;
			tmp_cnt=0;
	
			Float_t miss_pid;
			if (!mylist->getUserValue(FILLER_MISSING_PID, miss_pid)) miss_pid = -999;
			
			// Base
			temp[tmp_cnt++]=pp_miss.M2();
			temp[tmp_cnt++]=miss4.M2();
			temp[tmp_cnt++]=epem_invmass.M2();
			temp[tmp_cnt++]=prob;
			temp[tmp_cnt++]=opang;
			temp[tmp_cnt++]=ang_theta;
			temp[tmp_cnt++]=ang_alpha;
			temp[tmp_cnt++]=mylist->getOrder();
			temp[tmp_cnt++]=miss_pid;
	
			// Trigger
			if(nt_trigger) {
				UInt_t downscalingFlag = evHeader->getDownscalingFlag();
				UInt_t triggerDecision = evHeader->getTriggerDecision();
				UInt_t triggerBits = evHeader->getTBit();
				temp[tmp_cnt++]=downscalingFlag;
				temp[tmp_cnt++]=triggerDecision;
				temp[tmp_cnt++]=triggerBits;
	 			temp[tmp_cnt++]=evHeader->getEventSeqNumber();
	 			temp[tmp_cnt++]=evHeader->getEventRunNumber();
			}
			
			// Dtof and Kine extensions
			if(nt_dtof_refit){
				Float_t dtof, kine_chi2, kine_prechi2;
				Float_t pid_tracks;
					
				if (!mylist->getUserValue(DELTATOF_DTOF, dtof))
					dtof = -1;
				if (!mylist->getUserValue(KINEFIT_CHI2, kine_chi2))
					kine_chi2 = -1;
				if (!mylist->getUserValue(KINEFIT_PRECHI2, kine_prechi2))
					kine_prechi2 = -1;
				if (!mylist->getUserValue(FILLER_VALID_PIDTRACKS, pid_tracks))
					pid_tracks = -1;
							
				temp[tmp_cnt++]=dtof;
				temp[tmp_cnt++]=pid_tracks;
				temp[tmp_cnt++]=kine_chi2;
				temp[tmp_cnt++]=kine_prechi2;
				temp[tmp_cnt++]=fakes;
				temp[tmp_cnt++]=ep_in_chi2;
				temp[tmp_cnt++]=em_in_chi2;
			}
	
			// Full Lorentz vectors for particles
			if(nt_full_lorentz){
				temp[tmp_cnt++]=proton1.X();
				temp[tmp_cnt++]=proton1.Y();
				temp[tmp_cnt++]=proton1.Z();
				temp[tmp_cnt++]=proton2.X();
				temp[tmp_cnt++]=proton2.Y();
				temp[tmp_cnt++]=proton2.Z();
				temp[tmp_cnt++]=ep.X();
				temp[tmp_cnt++]=ep.Y();
				temp[tmp_cnt++]=ep.Z();
				temp[tmp_cnt++]=em.X();
				temp[tmp_cnt++]=em.Y();
				temp[tmp_cnt++]=em.Z();

				temp[tmp_cnt++]=ep_beta;
				temp[tmp_cnt++]=em_beta;
			}
         // MDC und Vertex informations
         if(nt_mdc_and_vertex){
				temp[tmp_cnt++]=p1_MdcR;
				temp[tmp_cnt++]=p1_MdcZ;
				temp[tmp_cnt++]=p1_DistVertex;
				temp[tmp_cnt++]=p2_MdcR;
				temp[tmp_cnt++]=p2_MdcZ;
				temp[tmp_cnt++]=p2_DistVertex;
				temp[tmp_cnt++]=ep_MdcR;
				temp[tmp_cnt++]=ep_MdcZ;
				temp[tmp_cnt++]=ep_DistVertex;
				temp[tmp_cnt++]=em_MdcR;
				temp[tmp_cnt++]=em_MdcZ;
				temp[tmp_cnt++]=em_DistVertex;
         }
	
			// Simulation
			if (simuflag != 0){
				// Base geant
				temp[tmp_cnt++]=ep_id;
				temp[tmp_cnt++]=em_id;
				temp[tmp_cnt++]=p1_id;
				temp[tmp_cnt++]=p2_id;
				temp[tmp_cnt++]=ep_geninfo;
				temp[tmp_cnt++]=em_geninfo;
				temp[tmp_cnt++]=p1_geninfo;
				temp[tmp_cnt++]=p2_geninfo;
				if( nt_full_geant){
					temp[tmp_cnt++]=ep_parentTrack;
					temp[tmp_cnt++]=em_parentTrack;
					temp[tmp_cnt++]=p1_parentTrack;
					temp[tmp_cnt++]=p2_parentTrack;
					temp[tmp_cnt++]=ep_creation;
					temp[tmp_cnt++]=em_creation;
					temp[tmp_cnt++]=ep_vx;
					temp[tmp_cnt++]=ep_vy;
					temp[tmp_cnt++]=ep_vz;
					temp[tmp_cnt++]=em_vx;
					temp[tmp_cnt++]=em_vy;
					temp[tmp_cnt++]=em_vz;
				}
			}
			miss->Fill(temp);
		}else{
			cerr << algoName << " got no TLorentzVector " << endl;
		}
	}
	
	return kTRUE;
}

Bool_t HHypPPEpEmProjector::init()
{
  // Checks if we have sim/exp and book the ntuple
	
  nt_full_lorentz = (GetOpt("LORENTZ") != NULL);
  nt_dtof_refit = (GetOpt("DTOF_REFIT") != NULL);
  nt_trigger = (GetOpt("TRIGGER") != NULL);
  nt_full_geant = (GetOpt("FULL_GEANT") != NULL);
  nt_mdc_and_vertex = (GetOpt("MDC_VERTEX") != NULL);


  simCat = gHades->getCurrentEvent()->getCategory(catGeantKine);
  CatTrackCandSim = NULL;          // Category
	
  if (!simCat){
    simuflag = 0;
  }else{
    simuflag = 1;

 	
    if ((CatTrackCandSim =
	 gHades->getCurrentEvent()->getCategory(catPidTrackCand)) == NULL) {
      Error("init", "Cannot get catPidTrackCandSim cat");
      return kFALSE;
    }
	
  }
	
  // need to get name from channel
  TString input(channel->Get(initList));

  TString st_base("pp_miss:miss4:epem_invmass:fProbAlg:opang:ppmiss_theta:epem_alpha:order:miss_pid");
  TString st_trigger(":downscalingFlag:triggerDecision:triggerBits:evtSeqNr:evtRunnr");
  TString st_dtof_kine(":dtof:pidtr:kine_chi2:kine_prechi2:fakes:ep_in_chi2:em_in_chi2");
  TString st_full_lorentz(
			  ":p1_mx:p1_my:p1_mz"
			  ":p2_mx:p2_my:p2_mz"
			  ":ep_mx:ep_my:ep_mz"
			  ":em_mx:em_my:em_mz"
			  ":ep_beta:em_beta"
			  );
  TString mdc_and_vertex(
        ":p1_MdcR:p1_MdcZ:p1_DistV"
        ":p2_MdcR:p2_MdcZ:p2_DistV"
        ":ep_MdcR:ep_MdcZ:ep_DistV"
        ":em_MdcR:em_MdcZ:em_DistV"
        );
	
  // Create String
  if(nt_trigger) st_base+=st_trigger;
  if(nt_dtof_refit) st_base+=st_dtof_kine;
  if(nt_full_lorentz) st_base+=st_full_lorentz;
  if(nt_mdc_and_vertex) st_base+=mdc_and_vertex;

	
  if (simuflag != 0){
    // Add String for sims
    TString st_base_geant(":ep_id:em_id:p1_id:p2_id"
			  ":ep_geninfo:em_geninfo:p1_geninfo:p2_geninfo");
    TString st_full_geant(":ep_partrack:em_partrack:p1_partrack:p2_partrack"
			  ":ep_creation:em_creation"
			  ":ep_vx:ep_vy:ep_vz"
			  ":em_vx:em_vy:em_vz"
			  );

    st_base+=st_base_geant;
    if( nt_full_geant) st_base+=st_full_geant;
  }
  miss = new TNtuple(input + TString("_em_proj"), "EM Channel Ntuple", st_base);
  cout << "--- " << input <<" PROJECTOR is using ---\n" << st_base << endl;
	
  //-----------------------------------------------------------------------
	
  return kTRUE;
}

Bool_t HHypPPEpEmProjector::reinit()
{
  return kTRUE;
}

Bool_t HHypPPEpEmProjector::finalize()
{
  miss->Write();
  return kTRUE;
}
