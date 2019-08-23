//*-- AUTHOR : Marco Destefanis
//*-- Modified : 26/07/2005 by Marco Destefanis
////////////////////////////////////////////////////////////////////////
//
// HHypPPKpPimMiss0Alg
//
// This is a SELECTOR removing combinations which do not have
// a missing Kplus
//
////////////////////////////////////////////////////////////////////////

using namespace std;
#include <stdlib.h>
#include <iostream>
#include "hhypPPKpPimMiss0Alg.h"
#include "hypinfodef.h"

ClassImp(HHypPPKpPimMiss0Alg)
  
  HHypPPKpPimMiss0Alg::HHypPPKpPimMiss0Alg(char *name_i , Option_t par[]) : HHypBaseAlgorithm(name_i, par)
{
}


HHypPPKpPimMiss0Alg::~HHypPPKpPimMiss0Alg()
{
}


Bool_t HHypPPKpPimMiss0Alg::execute()
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

 
      // miss4 has to go to 0

      if (miss4.M2() < -15000000.) // [MeV]
	mylist->removeComb();
      if (miss4.M2() > 15000000.) // [MeV]
	mylist->removeComb();


      // Remove unphysical events here:
//       if (p1kp_miss.M2() < p1pim_invmass.M2()) 
// 	mylist->removeComb();
//       if (p2kp_miss.M2() < p2pim_invmass.M2()) 
// 	mylist->removeComb();

//       if (pp_miss.M2() < pippim_invmass.M2()) 
// 	mylist->removeComb();

      // Remove invariant mass higher than 3 GeV/c2
      if ( p1pim_invmass.M() > 3000 ) mylist->removeComb();
      if ( p2pim_invmass.M() > 3000 ) mylist->removeComb();

      // Remove squared missing mass higher than 5000000 -> 2.2 GeV/c2
      if ( p1kp_miss.M2() >  5000000 ) mylist->removeComb();
      if ( p1kp_miss.M2() < -5000000 ) mylist->removeComb();
      if ( p2kp_miss.M2() >  5000000 ) mylist->removeComb();
      if ( p2kp_miss.M2() < -5000000 ) mylist->removeComb();

      // Remove tracks with wrong particle-charge connection
      if ( (track_p1->getCharge()) < 0 ) mylist->removeComb();
      if ( (track_p2->getCharge()) < 0 ) mylist->removeComb();
      if ( (track_kp->getCharge()) < 0 ) mylist->removeComb();
      if ( (track_pim->getCharge()) > 0 ) mylist->removeComb();

      // Remove combinations with an high beta value
      if ( track_p1->getBetaExp() > 1.5 ) mylist->removeComb();
      if ( track_p2->getBetaExp() > 1.5 ) mylist->removeComb();
      if ( track_kp->getBetaExp() > 1.5 ) mylist->removeComb();
      if ( track_pim->getBetaExp() < -1.5 ) mylist->removeComb();


    } //END getIterStatus() == kTRUE
  } //END Iterator
  if (exitIdx > -1) return kTRUE;
  return kFALSE;
}

Bool_t HHypPPKpPimMiss0Alg::init()
{
  return kTRUE;
}

Bool_t HHypPPKpPimMiss0Alg::reinit()
{
  return kTRUE;
}

Bool_t HHypPPKpPimMiss0Alg::finalize()
{
  return kTRUE;
}
