//*-- AUTHOR : Marco Destefanis
//*-- Modified : 26/07/2005 by Marco Destefanis
//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////
//
// HHypPKpLambdaMiss0Alg
//
// This is a SELECTOR removing combinations which do not have
// a missing Kplus
//
////////////////////////////////////////////////////////////////////////

using namespace std;
#include <stdlib.h>
#include <iostream>
#include "hhypPKpLambdaMiss0Alg.h"
#include "hypinfodef.h"

#include "hgeomtransform.h"
#include "hgeomvertexfit.h"

ClassImp(HHypPKpLambdaMiss0Alg)
  
  HHypPKpLambdaMiss0Alg::HHypPKpLambdaMiss0Alg(char *name_i , Option_t par[]) : HHypBaseAlgorithm(name_i, par)
{
}


HHypPKpLambdaMiss0Alg::~HHypPKpLambdaMiss0Alg()
{
}


Bool_t HHypPKpLambdaMiss0Alg::execute()
{

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
    TLorentzVector kp= mylist->getTLorentzVector("K+",1);
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

//  HPidParticle *PidPart = NULL;
//   HCategory *pidpartCat = gHades->getCurrentEvent()->getCategory(catPidPart);

//   PidPart = (HPidParticle *) pidpartCat->getObject(idx);

//     HPidParticle* track = mylist->getPidParticle();

    if ( mylist->getIterStatus() == kTRUE) { 
      
      // calculating missing mass
      TLorentzVector miss4 = (*beam) - (proton1 + proton2 + kp + pim); 
      TLorentzVector p1kp_miss = (*beam) - (proton1 + kp);
      TLorentzVector p2kp_miss = (*beam) - (proton2 + kp);
      TLorentzVector p1pim_invmass = (proton1+pim);
      TLorentzVector p2pim_invmass = (proton2+pim);

//       TLorentzVector pp_miss = (*beam) - (proton1 + proton2); 
//       TLorentzVector pippim_invmass = (pip+pim);
 

      // NOW I HAVE TO DO THE SELECTION!!!!
      // Here I set all combinations unvalid which do not fulfill 
      // the requirement

      //cout << miss4.M2() << endl;

Double_t rot[6][9]={ 
  { 1.0000000,  0.0000000,  0.0000000,  0.0000000,  1.0000000,  0.0000000,  0.0000000,  0.0000000,  1.0000000},
  
  { 0.5000000, -0.8660254,  0.0000000,  0.8660254,  0.5000000,  0.0000000,  0.0000000,  0.0000000,  1.0000000},
  
  {-0.5000000, -0.8660254,  0.0000000,  0.8660254, -0.5000000,  0.0000000,  0.0000000,  0.0000000,  1.0000000},
  
  {-1.0000000,  0.0000000,  0.0000000,  0.0000000, -1.0000000,  0.0000000,  0.0000000,  0.0000000,  1.0000000},
  
  {-0.5000000,  0.8660254,  0.0000000, -0.8660254, -0.5000000,  0.0000000,  0.0000000,  0.0000000,  1.0000000},
  
  {0.5000000,  0.8660254,  0.0000000, -0.8660254,  0.5000000,  0.0000000,  0.0000000,  0.0000000,  1.0000000}};
      HGeomTransform secTrans[6];
      for(int s=0;s<6;s++) secTrans[s].setRotMatrix(rot[s]);
      
      HGeomVertexFit fFitter1, fFitter2; 
      HGeomVector vertex1, vertex2;
      Float_t vtx_modulo1, vtx_modulo2;

      // Lambda vertex fitting

//       Float_t r_pion, z_pion, theta_pion, phi_pion;
      HGeomVector rLocal_pion, alphaLocal_pion, rLab_pion, alphaLab_pion;
//       Float_t r_proton, z_proton, theta_proton, phi_proton;
      HGeomVector rLocal_proton1, alphaLocal_proton1, rLab_proton1, alphaLab_proton1;
      HGeomVector rLocal_proton2, alphaLocal_proton2, rLab_proton2, alphaLab_proton2;

//       rLocal_pion.setX(track_pim.getR()*TMath::Cos(track_pim.phiDeg()+TMath::PiOver2()));
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
//       Float_t px1=0, py1=0, pz1=0, p1=0;// pi
//       Float_t px2=0, py2=0, pz2=0, p2=0;// p
//       Float_t x1=0, y1=0, z1=0;// pi
//       Float_t x2=0, y2=0, z2=0;// p

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
//       x1 = pi_vertex.X();//       x2 = p_vertex.X();
//       y1 = pi_vertex.Y();//       y2 = p_vertex.Y();
//       z1 = pi_vertex.Z();//       z2 = p_vertex.Z();


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


      // Cuts
      if (min_dist1 > 100) mylist->removeComb();
      if (min_dist2 > 100) mylist->removeComb();
      if (vertex1.getX() < -600 || vertex1.getX() > 600) mylist->removeComb();
      if (vertex1.getY() < -600 || vertex1.getY() > 600) mylist->removeComb();
      if (vertex1.getZ() < -2000) mylist->removeComb();
      if (vertex2.getX() < -600 || vertex2.getX() > 600) mylist->removeComb();
      if (vertex2.getY() < -600 || vertex2.getY() > 600) mylist->removeComb();
      if (vertex2.getZ() < -2000) mylist->removeComb();
      if (vtx_modulo1 > 4000) mylist->removeComb();
      if (vtx_modulo2 > 4000) mylist->removeComb();


      // Remove unphysical events here:
//       if (p1kp_miss.M2() < p1pim_invmass.M2()) 
// 	mylist->removeComb();
//       if (p2kp_miss.M2() < p2pim_invmass.M2()) 
// 	mylist->removeComb();

//       if (pp_miss.M2() < pippim_invmass.M2()) 
// 	mylist->removeComb();



    } //END getIterStatus() == kTRUE
  } //END Iterator
  if (exitIdx > -1) return kTRUE;
  return kFALSE;
}

Bool_t HHypPKpLambdaMiss0Alg::init()
{
  return kTRUE;
}

Bool_t HHypPKpLambdaMiss0Alg::reinit()
{
  return kTRUE;
}

Bool_t HHypPKpLambdaMiss0Alg::finalize()
{
  return kTRUE;
}
