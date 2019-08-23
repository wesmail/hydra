//*-- AUTHOR : Marco Destefanis
//*-- Modified : 28/07/2005 by Marco Destefanis
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

#include "hhypPPKpPimProjector.h"
#include "hhyplist.h"
#include "hhypchannel.h"
#include "hypinfodef.h"


ClassImp(HHypPPKpPimProjector)
  
  HHypPPKpPimProjector::HHypPPKpPimProjector(char *name_i, Option_t par[]) : HHypBaseAlgorithm(name_i, par)
{
  simuflag = 0;
}


HHypPPKpPimProjector::~HHypPPKpPimProjector()
{
}


Bool_t HHypPPKpPimProjector::execute()
{
  // Reads the input particle(s) from the HHypList
  // Important: the "beam" has to be defined in the macro
  // The content of the ntuple is different is sim/exp

  

  if (!beam) 
    {
      cerr << algoName << " needs beam particle! " << endl;
      return kFALSE;
    }
  
  // Resetting the list and start looping over the combinations
  // Loop is only done over the VALID combinations
  mylist->CombIteratorReset();
  while (mylist->CombIterator()) {
    
    // Getting the particles 
    TLorentzVector proton1= mylist->getTLorentzVector("p",1);
    TLorentzVector proton2= mylist->getTLorentzVector("p",2);
    TLorentzVector k_p= mylist->getTLorentzVector("K+",1);
    TLorentzVector pim= mylist->getTLorentzVector("pi-",1);

    if ( mylist->getIterStatus() == kTRUE) { 
      
      // calculating missing mass
      TLorentzVector p1kp_miss = (*beam) - (proton1 + k_p);
      TLorentzVector p2kp_miss = (*beam) - (proton2 + k_p);
      TLorentzVector miss4 = (*beam) - (proton1 + proton2 + k_p + pim); 
//       TLorentzVector pp_miss = (*beam) - (proton1 + proton2); 
//       TLorentzVector kppim_invmass = (k_p+pim);

      TLorentzVector p1pim_invmass = (proton1+pim);  // non e piu utile?
      TLorentzVector p2pim_invmass = (proton2+pim);
 
      Double_t prob = mylist->getProbAlg();
      //std::cout << "got prob " << prob << std::endl;

      // this is for simulation :

      Double_t kp_source = 0;
      Double_t pim_source = 0;
  
      if (simuflag == 1) {
	
	HPidParticleSim * my_kp = (HPidParticleSim*)CatPartSim->getObject(mylist->getIdxPidPart("K+",1));
	HPidParticleSim * my_pim = (HPidParticleSim*)CatPartSim->getObject(mylist->getIdxPidPart("pi-",1));
	
	
	if ( (my_kp!= NULL) && (my_pim!= NULL) && (mylist->getIterStatus() == kTRUE)) {
	  
	  HPidGeantTrackSet * kpGeantSet  = my_kp->getGeantTrackSet();
	  HPidGeantTrackSet * pimGeantSet = my_pim->getGeantTrackSet();
	  
	  // look only for the 1st geant track, makes life more easy  [Ingo] 
	  
	  Int_t kp_geant_track = kpGeantSet->getGeantTrackID(0);
	  Int_t pim_geant_track = pimGeantSet->getGeantTrackID(0);
   
	  if (kp_geant_track>=0) {
	    HGeantKine * kp_geantkine  =( HGeantKine *) kpGeantSet->getGeantKine(kp_geant_track);
	    HGeantKine * pim_geantkine =( HGeantKine *) pimGeantSet->getGeantKine(pim_geant_track);
		    
	    Float_t geninfo,genweight;
	    kp_geantkine->getGenerator(geninfo,genweight);

	    // cout << "pip has geant: " << (geninfo - 14014000) << ":" << genweight << endl;
	    kp_source=(geninfo - 14014000);

	    pim_geantkine->getGenerator(geninfo,genweight);

	    // cout << "pim has geant: " << (geninfo - 14014000) << ":" << genweight << endl;
	    pim_source=(geninfo - 14014000);
    
	  }
	  
	}
		
	
      } /* end for simu */
      
     if (simuflag == 0)  
       miss->Fill(p1kp_miss.M2() , p2kp_miss.M2(), miss4.M2(), p1pim_invmass.M(), p2pim_invmass.M(), prob);
     else
       miss->Fill(p1kp_miss.M2() , p2kp_miss.M2(), miss4.M2(), p1pim_invmass.M(), p2pim_invmass.M(), prob, kp_source, pim_source);
     

    }
    else cerr << algoName << " got no TLorentzVector " << endl;

  }

  return kTRUE;
}

Bool_t HHypPPKpPimProjector::init()
{
  // Checks if we have sim/exp and book the ntuple

  simCat=gHades->getCurrentEvent()->getCategory(catGeantKine);

  if (!simCat) simuflag=0;
  else {
    simuflag=1;
    //cout << "Projector uses SIMULATION" << endl;

    CatPartSim = NULL;   // Category
    
    if((CatPartSim = gHades->getCurrentEvent()->getCategory( catPidPart )) == NULL) 
      {
	Error("init", "Cannot get catPidPartSim cat");
	return kFALSE;
      }
    
  }


  std::cout << "booking ntuples for " << algoName << std::endl;
  
  
  // need to get name from channel
  TString input(channel->Get(initList));

  if (simuflag == 0)    
    miss = new TNtuple(input+TString("_proj"),"Demo ntuple","p1kp_miss:p2kp_miss:miss4:p1pim_invmass:p2pim_invmass:fProbAlg");
  else
    miss = new TNtuple(input+TString("_proj"),"Demo ntuple","p1kp_miss:p2kp_miss:miss4:p1pim_invmass:p2pim_invmass:fProbAlg:kp_source:pim_source");

 
  std::cout << "...done" << std::endl;

  return kTRUE;
}

Bool_t HHypPPKpPimProjector::reinit()
{
  return kTRUE;
}

Bool_t HHypPPKpPimProjector::finalize()
{
  miss->Write();
  return kTRUE;
}
