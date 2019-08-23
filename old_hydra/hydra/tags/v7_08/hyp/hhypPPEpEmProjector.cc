//*-- Author : B. Spruck
//*-- Modified : 2005-09-07
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

ClassImp(HHypPPEpEmProjector)

HHypPPEpEmProjector::HHypPPEpEmProjector(char *name_i, Option_t par[])
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
	
		// Getting the particles 
		TLorentzVector proton1 = mylist->getTLorentzVector("p", 1);
		TLorentzVector proton2 = mylist->getTLorentzVector("p", 2);
		TLorentzVector ep = mylist->getTLorentzVector("e+", 1);
		TLorentzVector em = mylist->getTLorentzVector("e-", 1);
	
		if (mylist->getIterStatus() == kTRUE) {
	
			// calculating missing mass
			TLorentzVector pp_miss = (*beam) - (proton1 + proton2);
			TLorentzVector miss4 = (*beam) - (proton1 + proton2 + ep + em);
			TLorentzVector epem_invmass = (ep + em);
		
			Double_t prob = mylist->getProbAlg();
		
			//std::cout << "got prob " << prob << std::endl;
		
			// this is for simulation :
		
			Double_t ep_source = 0;
			Double_t em_source = 0;
		
			if (simuflag == 1) {
		
				HPidParticleSim *my_ep =
				(HPidParticleSim *) CatPartSim->getObject(mylist->
															getIdxPidPart("e+", 1));
				HPidParticleSim *my_em =
				(HPidParticleSim *) CatPartSim->getObject(mylist->
															getIdxPidPart("e-", 1));
		
				if ((my_ep != NULL) && (my_em != NULL)
					&& (mylist->getIterStatus() == kTRUE)) {
		
					HPidGeantTrackSet *epGeantSet = my_ep->getGeantTrackSet();
					HPidGeantTrackSet *emGeantSet = my_em->getGeantTrackSet();
			
					// look only for the 1st geant track, makes life more easy  [Ingo] 
			
					Int_t ep_geant_track = epGeantSet->getGeantTrackID(0);
					Int_t em_geant_track = emGeantSet->getGeantTrackID(0);
			
					if (ep_geant_track >= 0) {
						HGeantKine *ep_geantkine =
						(HGeantKine *) epGeantSet->getGeantKine(ep_geant_track);
						HGeantKine *em_geantkine =
						(HGeantKine *) emGeantSet->getGeantKine(em_geant_track);
			
						Float_t geninfo, genweight;
			
						ep_geantkine->getGenerator(geninfo, genweight);
			
						// cout << "ep has geant: " << (geninfo - 141400) << ":" << genweight << endl;
			
			#warning "generator info might be wrong (old 14014000; new 141400)!!!"
						ep_source = (geninfo - 141400);
			
						em_geantkine->getGenerator(geninfo, genweight);
			
						// cout << "em has geant: " << (geninfo - 141400) << ":" << genweight << endl;
			#warning "same here!!!"
						em_source = (geninfo - 141400);
			
					}
				}
			}
			/* end for simu */
			Float_t dtof;
		
			if (!mylist->getUserValue(DELTATOF_DTOF, dtof))
				dtof = -1;
			// cout << "dtof " << dtof << endl;
		
			Float_t pid_tracks;
		
			if (!mylist->getUserValue(FILLER_VALID_PIDTRACKS, pid_tracks))
				pid_tracks = -1;
			// cout << "pid_tracks " << pid_tracks << endl;
		
			if (simuflag == 0){
				miss->Fill(pp_miss.M2(), miss4.M2(), epem_invmass.M2(), prob,
						dtof, pid_tracks);
			}else{
				miss->Fill(pp_miss.M2(), miss4.M2(), epem_invmass.M2(), prob,
						dtof, pid_tracks, ep_source, em_source);
			}
				
			if (evHeader) {
				UInt_t downscalingFlag = evHeader->getDownscalingFlag();
				UInt_t triggerDecision = evHeader->getTriggerDecision();
				header->Fill(downscalingFlag,triggerDecision);
			}
		}else{
			cerr << algoName << " got no TLorentzVector " << endl;
		}
	}
	
	return kTRUE;
}

Bool_t HHypPPEpEmProjector::init()
{
// Checks if we have sim/exp and book the ntuple

	simCat = gHades->getCurrentEvent()->getCategory(catGeantKine);
	
	if (!simCat){
		simuflag = 0;
	}else{
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
	
	if (simuflag == 0){
		miss =
		new TNtuple(input + TString("_proj"), "Masses",
					"pp_miss:miss4:epem_invmass:fProbAlg:dtof:pidtr");
	}else{
		miss =
		new TNtuple(input + TString("_proj"), "Masses",
					"pp_miss:miss4:epem_invmass:fProbAlg:dtof:pidtr:ep_source:em_source");
	}
	
	header = new TNtuple(input + TString("_hdr"), "Header info",
				"downscalingFlag:triggerDecision");
	
	return kTRUE;
}

Bool_t HHypPPEpEmProjector::reinit()
{
	return kTRUE;
}

Bool_t HHypPPEpEmProjector::finalize()
{
	miss->Write();
	header->Write();
	return kTRUE;
}
