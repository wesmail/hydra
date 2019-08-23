//*-- AUTHOR : Marco Destefanis
//*-- Modified : 01/08/2005 by Marco Destefanis
//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////
//
// HHypPKpLambdaMiss0Projector
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

#include "hhypPKpLambdaMiss0Projector.h"
#include "hhyplist.h"
#include "hhypchannel.h"
#include "hypinfodef.h"

#include "hgeomtransform.h"
#include "hgeomvertexfit.h"


ClassImp(HHypPKpLambdaMiss0Projector)
  
  HHypPKpLambdaMiss0Projector::HHypPKpLambdaMiss0Projector(Char_t *name_i, Option_t par[]) : HHypBaseAlgorithm(name_i, par)
{
  simuflag = 0;
}


HHypPKpLambdaMiss0Projector::~HHypPKpLambdaMiss0Projector()
{
}


Bool_t HHypPKpLambdaMiss0Projector::execute()
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


    Int_t idx_p1 = mylist->getIdxPidPart ("p",1);
    Int_t idx_p2 = mylist->getIdxPidPart ("p",2);
    Int_t idx_kp = mylist->getIdxPidPart ("K+",1);
    Int_t idx_pim = mylist->getIdxPidPart ("pi-",1);

    HPidParticle *track_p1  = NULL;
    HPidParticle *track_p2  = NULL;
    HPidParticle *track_kp  = NULL;
    HPidParticle *track_pim = NULL;
    HCategory *pidpartCat = gHades->getCurrentEvent()->getCategory(catPidPart);

    track_p1  = (HPidParticle *) pidpartCat->getObject(idx_p1);
    track_p2  = (HPidParticle *) pidpartCat->getObject(idx_p2);
    track_kp  = (HPidParticle *) pidpartCat->getObject(idx_kp);
    track_pim = (HPidParticle *) pidpartCat->getObject(idx_pim);


    Double_t rot[6][9]={ 
      { 1.0000000,  0.0000000,  0.0000000,  0.0000000,  1.0000000,  0.0000000,  0.0000000,  0.0000000,  1.0000000},
      
      { 0.5000000, -0.8660254,  0.0000000,  0.8660254,  0.5000000,  0.0000000,  0.0000000,  0.0000000,  1.0000000},
      
      {-0.5000000, -0.8660254,  0.0000000,  0.8660254, -0.5000000,  0.0000000,  0.0000000,  0.0000000,  1.0000000},
      
      {-1.0000000,  0.0000000,  0.0000000,  0.0000000, -1.0000000,  0.0000000,  0.0000000,  0.0000000,  1.0000000},
      
      {-0.5000000,  0.8660254,  0.0000000, -0.8660254, -0.5000000,  0.0000000,  0.0000000,  0.0000000,  1.0000000},
  
      {0.5000000,  0.8660254,  0.0000000, -0.8660254,  0.5000000,  0.0000000,  0.0000000,  0.0000000,  1.0000000}};
    HGeomTransform secTrans[6];
    for(Int_t s=0;s<6;s++) secTrans[s].setRotMatrix(rot[s]);



    if ( mylist->getIterStatus() == kTRUE) { 
      
      // calculating missing mass
      TLorentzVector p1kp_miss = (*beam) - (proton1 + k_p);
      TLorentzVector p2kp_miss = (*beam) - (proton2 + k_p);
      TLorentzVector miss4 = (*beam) - (proton1 + proton2 + k_p + pim); 
//       TLorentzVector pp_miss = (*beam) - (proton1 + proton2); 
//       TLorentzVector kppim_invmass = (k_p+pim);

      TLorentzVector p1pim_invmass = (proton1+pim);  // non e piu utile?
      TLorentzVector p2pim_invmass = (proton2+pim);

      Double_t prob;
      prob = mylist->getProbAlg();
      //std::cout << "got prob " << prob << std::endl;

      // this is for simulation :

      Double_t kp_source = 0;
      Double_t pim_source = 0;
  
      if (simuflag == 1) {
	
	HPidParticleSim * my_kp = (HPidParticleSim*)CatPartSim->getObject(mylist->getIdxPidPart("K+",1));
	HPidParticleSim * my_pim = (HPidParticleSim*)CatPartSim->getObject(mylist->getIdxPidPart("pi-",1));
	
	
	if ( (my_kp!= NULL) && (my_pim!= NULL) && (mylist->getIterStatus() == kTRUE)) {
	  
	  HPidGeantTrackSet * kpGeantSet  = (HPidGeantTrackSet*) my_kp->getGeantTrackSet();
	  HPidGeantTrackSet * pimGeantSet = (HPidGeantTrackSet*) my_pim->getGeantTrackSet();
	  
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


      // LAMBDA

      HGeomVertexFit fFitter1, fFitter2; 
      HGeomVector vertex1, vertex2;
      Float_t vtx_modulo1, vtx_modulo2;

      // Lambda vertex fitting

      HGeomVector rLocal_pion, alphaLocal_pion, rLab_pion, alphaLab_pion;
      HGeomVector rLocal_proton1, alphaLocal_proton1, rLab_proton1, alphaLab_proton1;
      HGeomVector rLocal_proton2, alphaLocal_proton2, rLab_proton2, alphaLab_proton2;

      rLocal_pion.setX( track_pim->getR() * TMath::Cos( ( track_pim->phiDeg() + 90 ) * TMath::Pi()/180 ) );
      rLocal_pion.setY( track_pim->getR() * TMath::Sin( ( track_pim->phiDeg() + 90 ) * TMath::Pi()/180 ) );  
      rLocal_pion.setZ( track_pim->getZ() ); // base vector in MDC system (punto)
      
      alphaLocal_pion.setX( TMath::Sin( track_pim->thetaDeg() * TMath::Pi()/180 )*TMath::Cos( track_pim->phiDeg() * TMath::Pi()/180 ));
      alphaLocal_pion.setY( TMath::Sin( track_pim->thetaDeg() * TMath::Pi()/180 )*TMath::Sin( track_pim->phiDeg() * TMath::Pi()/180 ));  
      alphaLocal_pion.setZ( TMath::Cos( track_pim->thetaDeg() * TMath::Pi()/180 )); // direction vector in MDC system

      rLab_pion     = HGeomVector(secTrans[track_pim->sector()].transFrom(rLocal_pion));      // MDC --> LAB system
      alphaLab_pion = HGeomVector(secTrans[track_pim->sector()].getRotMatrix()*alphaLocal_pion); 


      // proton 1
      rLocal_proton1.setX( track_p1->getR() * TMath::Cos( ( track_p1->phiDeg() + 90 ) * TMath::Pi()/180 ) );
      rLocal_proton1.setY( track_p1->getR() * TMath::Sin( ( track_p1->phiDeg() + 90 ) * TMath::Pi()/180 ) );  
      rLocal_proton1.setZ( track_p1->getZ() ); // base vector in MDC system (punto)
      
      alphaLocal_proton1.setX( TMath::Sin( track_p1->thetaDeg() * TMath::Pi()/180 )*TMath::Cos( track_p1->phiDeg() * TMath::Pi()/180 ));
      alphaLocal_proton1.setY( TMath::Sin( track_p1->thetaDeg() * TMath::Pi()/180 )*TMath::Sin( track_p1->phiDeg() * TMath::Pi()/180 ));  
      alphaLocal_proton1.setZ( TMath::Cos( track_p1->thetaDeg() * TMath::Pi()/180 ));// direction vector in MDC system

      rLab_proton1     = HGeomVector(secTrans[track_p1->sector()].transFrom(rLocal_proton1));	// MDC --> LAB system
      alphaLab_proton1 = HGeomVector(secTrans[track_p1->sector()].getRotMatrix()*alphaLocal_proton1); 

      // proton 2
      rLocal_proton2.setX( track_p2->getR() * TMath::Cos( ( track_p2->phiDeg() + 90 ) * TMath::Pi()/180 ) );
      rLocal_proton2.setY( track_p2->getR() * TMath::Sin( ( track_p2->phiDeg() + 90 ) * TMath::Pi()/180 ) );  
      rLocal_proton2.setZ( track_p2->getZ() ); // base vector in MDC system
      
      alphaLocal_proton2.setX( TMath::Sin( track_p2->thetaDeg() * TMath::Pi()/180 )*TMath::Cos( track_p2->phiDeg() * TMath::Pi()/180 ));
      alphaLocal_proton2.setY( TMath::Sin( track_p2->thetaDeg() * TMath::Pi()/180 )*TMath::Sin( track_p2->phiDeg() * TMath::Pi()/180 ));  
      alphaLocal_proton2.setZ( TMath::Cos( track_p2->thetaDeg() * TMath::Pi()/180 ));// direction vector in MDC system

      rLab_proton2     = HGeomVector(secTrans[track_p2->sector()].transFrom(rLocal_proton2));	// MDC --> LAB system
      alphaLab_proton2 = HGeomVector(secTrans[track_p2->sector()].getRotMatrix()*alphaLocal_proton2); 


      // Fitter

      // proton 1
      fFitter1.addLine(rLab_pion,alphaLab_pion);
      fFitter1.addLine(rLab_proton1,alphaLab_proton1);

      fFitter1.getVertex(vertex1);

      // proton 2
      fFitter2.addLine(rLab_pion,alphaLab_pion);
      fFitter2.addLine(rLab_proton2,alphaLab_proton2);

      fFitter2.getVertex(vertex2);


      vtx_modulo1 = (sqrt(vertex1.getX()*vertex1.getX() + vertex1.getY()*vertex1.getY() + vertex1.getZ()*vertex1.getZ()));
      vtx_modulo2 = (sqrt(vertex2.getX()*vertex2.getX() + vertex2.getY()*vertex2.getY() + vertex2.getZ()*vertex2.getZ()));


      // Minimum Distance

      Float_t min_dist1 = 0, min_dist2 = 0;
      Float_t mom_pim=0, mom_x_pim=0, mom_y_pim=0, mom_z_pim=0;// pi-
      Float_t mom_prot1=0, mom_x_prot1=0, mom_y_prot1=0, mom_z_prot1=0;// proton 1
      Float_t mom_prot2=0, mom_x_prot2=0, mom_y_prot2=0, mom_z_prot2=0;// proton 2
      Float_t pos_x_pim=0, pos_y_pim=0, pos_z_pim=0;// pi-
      Float_t pos_x_prot1=0, pos_y_prot1=0, pos_z_prot1=0;// proton 1
      Float_t pos_x_prot2=0, pos_y_prot2=0, pos_z_prot2=0;// proton 2


      mom_pim = pim.M();
      mom_prot1 = proton1.M();
      mom_prot2 = proton2.M();
      mom_x_pim = pim.Px()/mom_pim;
      mom_y_pim = pim.Py()/mom_pim;
      mom_z_pim = pim.Pz()/mom_pim;
      mom_x_prot1 = proton1.Px()/mom_prot1;
      mom_y_prot1 = proton1.Py()/mom_prot1;
      mom_z_prot1 = proton1.Pz()/mom_prot1;
      mom_x_prot2 = proton2.Px()/mom_prot2;
      mom_y_prot2 = proton2.Py()/mom_prot2;
      mom_z_prot2 = proton2.Pz()/mom_prot2;
      pos_x_pim = rLocal_pion.getX();
      pos_y_pim = rLocal_pion.getY();
      pos_z_pim = rLocal_pion.getZ();
      pos_x_prot1 = rLocal_proton1.getX();
      pos_y_prot1 = rLocal_proton1.getY();
      pos_z_prot1 = rLocal_proton1.getZ();
      pos_x_prot2 = rLocal_proton2.getX();
      pos_y_prot2 = rLocal_proton2.getY();
      pos_z_prot2 = rLocal_proton2.getZ();


      Float_t saul1 = 	sqrt(mom_y_pim*mom_y_pim*mom_z_prot1*mom_z_prot1 - 2*mom_y_pim*mom_z_prot1*mom_z_pim*mom_y_prot1 + mom_z_pim*mom_z_pim*mom_y_prot1*mom_y_prot1 + 
			     mom_z_pim*mom_z_pim*mom_x_prot1*mom_x_prot1 - 2*mom_z_pim*mom_x_prot1*mom_x_pim*mom_z_prot1 + mom_x_pim*mom_x_pim*mom_z_prot1*mom_z_prot1 +
			     mom_x_pim*mom_x_pim*mom_y_prot1*mom_y_prot1 - 2*mom_x_pim*mom_y_prot1*mom_y_pim*mom_x_prot1 + mom_y_pim*mom_y_pim*mom_x_prot1*mom_x_prot1);
//       std::cout<<saul<<std::endl;

      if ( saul1==0 ) continue; 
      min_dist1 = fabs(mom_y_pim*mom_z_prot1*pos_x_pim - mom_y_pim*mom_z_prot1*pos_x_prot1 - mom_z_pim*mom_y_prot1*pos_x_pim + mom_z_pim*mom_y_prot1*pos_x_prot1 -
		       mom_x_pim*mom_z_prot1*pos_y_pim + mom_x_pim*mom_z_prot1*pos_y_prot1 + mom_x_pim*mom_y_prot1*pos_z_pim - mom_x_pim*mom_y_prot1*pos_z_prot1 +
		       mom_z_pim*mom_x_prot1*pos_y_pim - mom_z_pim*mom_x_prot1*pos_y_prot1 - mom_y_pim*mom_x_prot1*pos_z_pim + mom_y_pim*mom_x_prot1*pos_z_prot1)/ saul1;


      Float_t saul2 = 	sqrt(mom_y_pim*mom_y_pim*mom_z_prot2*mom_z_prot2 - 2*mom_y_pim*mom_z_prot2*mom_z_pim*mom_y_prot2 + mom_z_pim*mom_z_pim*mom_y_prot2*mom_y_prot2 + 
			     mom_z_pim*mom_z_pim*mom_x_prot2*mom_x_prot2 - 2*mom_z_pim*mom_x_prot2*mom_x_pim*mom_z_prot2 + mom_x_pim*mom_x_pim*mom_z_prot1*mom_z_prot2 +
			     mom_x_pim*mom_x_pim*mom_y_prot2*mom_y_prot2 - 2*mom_x_pim*mom_y_prot2*mom_y_pim*mom_x_prot2 + mom_y_pim*mom_y_pim*mom_x_prot2*mom_x_prot2);
//       std::cout<<saul<<std::endl;

      if ( saul2==0 ) continue; 
      min_dist2 = fabs(mom_y_pim*mom_z_prot2*pos_x_pim - mom_y_pim*mom_z_prot2*pos_x_prot2 - mom_z_pim*mom_y_prot2*pos_x_pim + mom_z_pim*mom_y_prot2*pos_x_prot2 -
		       mom_x_pim*mom_z_prot2*pos_y_pim + mom_x_pim*mom_z_prot2*pos_y_prot2 + mom_x_pim*mom_y_prot2*pos_z_pim - mom_x_pim*mom_y_prot2*pos_z_prot2 +
		       mom_z_pim*mom_x_prot2*pos_y_pim - mom_z_pim*mom_x_prot2*pos_y_prot2 - mom_y_pim*mom_x_prot2*pos_z_pim + mom_y_pim*mom_x_prot2*pos_z_prot2)/ saul2;




      // END LAMBDA


     if (simuflag == 0)  
       miss->Fill(vertex1.getX(), vertex1.getY(), vertex1.getZ(), vertex2.getX(), vertex2.getY(), vertex2.getZ(),
		  vtx_modulo1, vtx_modulo2, min_dist1, min_dist2);
// 		  p1kp_miss.M2() , p2kp_miss.M2(), miss4.M2(), p1pim_invmass.M(), p2pim_invmass.M(), prob);
     else
       miss->Fill(vertex1.getX(), vertex1.getY(), vertex1.getZ(), vertex2.getX(), vertex2.getY(), vertex2.getZ(),
		  vtx_modulo1, vtx_modulo2, min_dist1, min_dist2);
// 		  p1kp_miss.M2() , p2kp_miss.M2(), miss4.M2(), p1pim_invmass.M(), p2pim_invmass.M(), prob, kp_source, pim_source);
     

    }
    else cerr << algoName << " got no TLorentzVector " << endl;

  }

  return kTRUE;
}

Bool_t HHypPKpLambdaMiss0Projector::init()
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
    miss = new TNtuple(input+TString("_proj"),"Demo ntuple","vertex1_X:vertex1_Y:vertex1_Z:vertex2_X:vertex2_Y:vertex2_Z:vtx_modulo1:vtx_modulo2:min_dist1:min_dist2");
// p1kp_miss:p2kp_miss:miss4:p1pim_invmass:p2pim_invmass:fProbAlg");
  else
    miss = new TNtuple(input+TString("_proj"),"Demo ntuple","vertex1_X:vertex1_Y:vertex1_Z:vertex2_X:vertex2_Y:vertex2_Z:vtx_modulo1:vtx_modulo2:min_dist1:min_dist2");
// p1kp_miss:p2kp_miss:miss4:p1pim_invmass:p2pim_invmass:fProbAlg:kp_source:pim_source");

 
  std::cout << "...done" << std::endl;

  return kTRUE;
}

Bool_t HHypPKpLambdaMiss0Projector::reinit()
{
  return kTRUE;
}

Bool_t HHypPKpLambdaMiss0Projector::finalize()
{
  miss->Write();
  return kTRUE;
}
