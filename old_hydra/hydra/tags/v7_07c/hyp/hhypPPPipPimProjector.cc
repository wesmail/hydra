//*-- AUTHOR : I. Froehlich
//*-- Modified : 21/04/2005 by I. Froehlich
//*-- Modified : 11/08/2005 by T. Perez      Added Geant ID to all the tracks.
//*-- Modified : 20/10/2005 by T. Perez      Sim All tracks have GeantID + Generator Info.
//               There is a DEBUG FLAG to minimize output. Maybe we can do that better.
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

#define DEBUG 0

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
    TLorentzVector pip = mylist->getTLorentzVector("pi+", 1);
    TLorentzVector pim = mylist->getTLorentzVector("pi-", 1);

    if (mylist->getIterStatus() == kTRUE) {

      // calculating missing mass
      TLorentzVector pp_miss = (*beam) - (proton1 + proton2);
      TLorentzVector miss4 = (*beam) - (proton1 + proton2 + pip + pim);
      TLorentzVector pippim_invmass = (pip + pim);

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

      if (simuflag == 1) {

	//	cerr<<"\n\nsimuflag = 1"<<endl;

        HPidParticleSim *my_pip = (HPidParticleSim *) CatPartSim
	  ->getObject(mylist->getIdxPidPart("pi+", 1));
        HPidParticleSim *my_pim = (HPidParticleSim *) CatPartSim
	  ->getObject(mylist->getIdxPidPart("pi-", 1));
        HPidParticleSim *my_p1 =
          (HPidParticleSim *) CatPartSim->getObject(mylist->
                                                    getIdxPidPart("p", 1));
        HPidParticleSim *my_p2 =
          (HPidParticleSim *) CatPartSim->getObject(mylist->
                                                    getIdxPidPart("p", 2));

        if ((my_pip != NULL) && (my_pim != NULL)
            && (mylist->getIterStatus() == kTRUE)) {
	  //	  cerr<<"my_pip ..."<<endl;
          
	  HPidGeantTrackSet *pipGeantSet = my_pip->getGeantTrackSet();
          HPidGeantTrackSet *pimGeantSet = my_pim->getGeantTrackSet();
          HPidGeantTrackSet *p1GeantSet = my_p1->getGeantTrackSet();
          HPidGeantTrackSet *p2GeantSet = my_p2->getGeantTrackSet();
	  //	  cerr<<"PID Pi+ = "<<my_pip->getPid()<<endl;
          // look only for the 1st geant track, makes life more easy  [Ingo] 

          Int_t pip_geant_track = pipGeantSet->getGeantTrackID(0);
          Int_t pim_geant_track = pimGeantSet->getGeantTrackID(0);
          Int_t p1_geant_track = p1GeantSet->getGeantTrackID(0);
          Int_t p2_geant_track = p2GeantSet->getGeantTrackID(0);
	  
	  Int_t numberOfCorrTracks = pipGeantSet->getNCorrelatedTrackIds();
	  for (Int_t i = 0; i < numberOfCorrTracks; i++ ) {
	    // cerr<<"pip_geant_track    = "<<pip_geant_track<<endl;
	    // cerr<<"pip_geant_track("<<i<<") = "<<pipGeantSet->getGeantTrackID(i)<<endl;
	  }
	  //	  pipGeantSet
	  // cerr<<"pim_geant_track    = "<<pim_geant_track<<endl;
	  
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

 	    Int_t pip_parentID = 0, pip_parentTrack = 0;
 	    Int_t pim_parentID = 0, pim_parentTrack = 0;
 	    Int_t p1_parentID = 0, p1_parentTrack = 0;
 	    Int_t p2_parentID = 0, p2_parentTrack = 0;
	    
 	    Int_t pip_parentParent = 0;
 	    Float_t parent1genInfo = 0, parent1genWeight=0;
	    //	    Float_t p1_source = 0, p2_source = 0;

            pip_geantkine->getGenerator(pip_geninfo, genweight);	    
            // cout << "pip has geant: " << (geninfo - 14014000) << ":" << genweight << endl;
	    //            pip_source = (geninfoPip - 14014000);
            pim_geantkine->getGenerator(pim_geninfo, genweight);
            // cout << "pim has geant: " << (geninfo - 14014000) << ":" << genweight << endl;
	    //            pim_source = (geninfopim - 14014000);
            p1_geantkine->getGenerator(p1_geninfo, genweight);
            p2_geantkine->getGenerator(p2_geninfo, genweight);	    
// 	    p1_source = (geninfop1 - 14014000);
// 	    p2_source = (geninfoP2 - 14014000);

	    pip_parentTrack = pip_geantkine->getParentTrack();
	    pim_parentTrack = pim_geantkine->getParentTrack();
	    p1_parentTrack = p1_geantkine->getParentTrack();
	    p2_parentTrack = p2_geantkine->getParentTrack();
	    
	    HLinearCategory *myCatGeantKine = (HLinearCategory*)gHades->getCurrentEvent()->getCategory(catGeantKine);
	    
	    // Particle ID
	    pip_id = pip_geantkine->getID();
	    pim_id = pim_geantkine->getID();
	    p1_id  = p1_geantkine->getID();
	    p2_id  = p2_geantkine->getID();
	    
	    ///////////////////////////////////////////////////////////////////
	    // DEBUG 
	    if (DEBUG == 1) {
	      Int_t tracks = 0;
	      if( pim_geant_track > tracks) tracks =  pim_geant_track;
	      if( pip_geant_track > tracks) tracks =  pip_geant_track;
	      if( p1_geant_track > tracks)  tracks =  p1_geant_track;
	      if( p2_geant_track > tracks)  tracks =  p2_geant_track;
	    
	    
	      if (pip_id== 8 && pim_id== 9 && p1_id == 14 && p2_id == 14) {
		cerr.setf(ios::fixed);
		cerr.setf(ios::showpoint);
		cerr.precision(0);

		cerr<<"\n \tID of the 4 particles : "<<endl;
		cerr<<"           \t Pi+ \t\t Pi- \t\t p1 \t\t P2"<<endl;
		cerr<<"ID         \t"<<pip_id<<"\t\t"<<pim_id<<"\t\t"<<p1_id<<"\t\t"<<p2_id<<endl;
		cerr<<"GeantTrack \t"<<pip_geant_track<<"\t\t"<< pim_geant_track<<"\t\t"<<p1_geant_track<<"\t\t"<<p2_geant_track<<endl;
		cerr<<"ParentTrack\t"<<pip_parentTrack<<"\t\t"<<pim_parentTrack <<"\t\t"<<p1_parentTrack<<"\t\t"<<p2_parentTrack<<endl;
		cerr<<"gen info p+ "<<pip_geninfo<<endl;
		cerr<<"gen info p- "<<pim_geninfo<<endl;
		cerr<<"gen info p1 "<<p1_geninfo<<endl;
		cerr<<"gen info p2 "<<p1_geninfo<<endl;
		// geatnTracks from 1 -> N .   getObject(i) 0->N-1 !!!1
	      
		// parentID
		if (pip_parentTrack > 0) {
		  HGeantKine * parent1 = (HGeantKine *)myCatGeantKine->getObject(pip_parentTrack-1);
		  pip_parentID = parent1->getID();
		  pip_parentParent = parent1->getParentTrack();
		  cerr<<  "Parent of pi+ ID = "<<pip_parentID
		      <<"\nParentParent     = "<<pip_parentParent<<endl;
		  if (pip_parentParent == 0) {
		    parent1->getGenerator(parent1genInfo, parent1genWeight);
		    cerr<<"GeneratorInfo    = "<<parent1genInfo<<endl;
		  }
		}
		else cerr<<"pi+ gen info = "<<pip_geninfo<<endl;
		if (pim_parentTrack > 0) {
		  HGeantKine * parent1 = (HGeantKine *)myCatGeantKine->getObject(pim_parentTrack-1);
		  pip_parentID = parent1->getID();
		  pip_parentParent = parent1->getParentTrack();
		  cerr<<  "Parent of pi- ID = "<<pip_parentID
		      <<"\nParentParent     = "<<pip_parentParent<<endl;
		  if (pip_parentParent == 0) {
		    parent1->getGenerator(parent1genInfo, parent1genWeight);
		    cerr<<"GeneratorInfo    = "<<parent1genInfo<<endl;
		  }
		}
		else cerr<<"pi- gen info = "<<pim_geninfo<<endl;
		if (p1_parentTrack > 0) {
		  HGeantKine * parent1 = (HGeantKine *)myCatGeantKine->getObject(p1_parentTrack-1);
		  pip_parentID = parent1->getID();
		  pip_parentParent = parent1->getParentTrack();
		  cerr<<  "Parent of proton1 ID = "<<pip_parentID
		      <<"\nParentParent     = "<<pip_parentParent<<endl;
		  if (pip_parentParent == 0) {
		    parent1->getGenerator(parent1genInfo, parent1genWeight);
		    cerr<<"GeneratorInfo    = "<<parent1genInfo<<endl;
		  }
		}
		else cerr<<"proton1 gen info = "<<p1_geninfo<<endl;
		if (p2_parentTrack > 0) {
		  HGeantKine * parent1 = (HGeantKine *)myCatGeantKine->getObject(p2_parentTrack-1);
		  pip_parentID = parent1->getID();
		  pip_parentParent = parent1->getParentTrack();
		  cerr<<  "Parent of proton2 ID = "<<pip_parentID
		      <<"\nParentParent     = "<<pip_parentParent<<endl;
		  if (pip_parentParent == 0) {
		    parent1->getGenerator(parent1genInfo, parent1genWeight);
		    cerr<<"GeneratorInfo    = "<<parent1genInfo<<endl;
		  }
		}
		else cerr<<"proton2 gen info = "<<p2_geninfo<<endl;
		// End Parent ID + Source

		for (Int_t i = 0 ; i<tracks; i++)
		  cerr<<"Particle("<<i<<") = "<<((HGeantKine *)myCatGeantKine->getObject(i))->getID()<<endl;
	      } // end if(pi+ &&pi- &&p1&& p2)
	    } //END IF DEBUG
	  }
	}
      } /* end for simu */

      Float_t dtof;
      if (!mylist->getUserValue(DELTATOF_DTOF, dtof))
        dtof = -1;
      // cout << "dtof " << dtof << endl;

      Float_t pid_tracks;

      if (!mylist->getUserValue(FILLER_VALID_PIDTRACKS, pid_tracks))
        pid_tracks = -1;
      // cout << "pid_tracks " << pid_tracks << endl;

      if (simuflag == 0)
        miss->Fill(pp_miss.M2(), miss4.M2(), pippim_invmass.M2(), prob,
                   dtof, pid_tracks);
      else
        miss->Fill(pp_miss.M2(), miss4.M2(), pippim_invmass.M2(), prob,
                   dtof, pid_tracks, pip_geninfo, pim_geninfo, p1_geninfo, 
		   p2_geninfo, pip_id, pim_id, p1_id, p2_id);

      if (evHeader) {
	UInt_t downscalingFlag = evHeader->getDownscalingFlag();
	UInt_t triggerDecision = evHeader->getTriggerDecision();
	header->Fill(downscalingFlag,triggerDecision);
      }
    } else
      cerr << algoName << " got no TLorentzVector " << endl;

  }

  return kTRUE;
}

Bool_t HHypPPPipPimProjector::init()
{
  // Checks if we have sim/exp and book the ntuple

  simCat = gHades->getCurrentEvent()->getCategory(catGeantKine);

  if (!simCat)
    simuflag = 0;
  else {
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

  if (simuflag == 0)
    miss =
      new TNtuple(input + TString("_proj"), "Masses",
                  "pp_miss:miss4:pippim_invmass:fProbAlg:dtof:pidtr");
  else
    miss =
      new TNtuple(input + TString("_proj"), "Masses",
                  "pp_miss:miss4:pippim_invmass:fProbAlg:dtof:pidtr:pip_geninfo:pim_geninfo:p1_geninfo:p2_geninfo:pip_id:pim_id:p1_id:p2_id");

  header = new TNtuple(input + TString("_hdr"), "Header info",
		     "downscalingFlag:triggerDecision");

  return kTRUE;
}

Bool_t HHypPPPipPimProjector::reinit()
{
  return kTRUE;
}

Bool_t HHypPPPipPimProjector::finalize()
{
  miss->Write();
  header->Write();
  return kTRUE;
}
