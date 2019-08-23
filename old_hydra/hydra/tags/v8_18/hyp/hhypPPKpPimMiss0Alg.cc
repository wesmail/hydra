//*-- AUTHOR : Marco Destefanis
//*-- Modified : 26/07/2005 by Marco Destefanis
//_HADES_CLASS_DESCRIPTION 
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
  
  HHypPPKpPimMiss0Alg::HHypPPKpPimMiss0Alg(Char_t *name_i , Option_t par[]) : HHypBaseAlgorithm(name_i, par)
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

    Int_t idx_p1 = mylist->getIdxPidTrackCand ("p",1);
    Int_t idx_p2 = mylist->getIdxPidTrackCand ("p",2);
    Int_t idx_kp = mylist->getIdxPidTrackCand ("K+",1);
    Int_t idx_pim = mylist->getIdxPidTrackCand ("pi-",1);

    HPidTrackCand *track_p1  = NULL;
    HPidTrackCand *track_p2  = NULL;
    HPidTrackCand *track_kp  = NULL;
    HPidTrackCand *track_pim = NULL;
    HCategory *pidpartCat = gHades->getCurrentEvent()->getCategory(catPidTrackCand);

    track_p1  = (HPidTrackCand *) pidpartCat->getObject(idx_p1);
    track_p2  = (HPidTrackCand *) pidpartCat->getObject(idx_p2);
    track_kp  = (HPidTrackCand *) pidpartCat->getObject(idx_kp);
    track_pim = (HPidTrackCand *) pidpartCat->getObject(idx_pim);

//  HPidTrackCand *PidPart = NULL;
//   HCategory *pidpartCat = gHades->getCurrentEvent()->getCategory(catPidTrackCand);

//   PidPart = (HPidTrackCand *) pidpartCat->getObject(idx);

//     HPidTrackCand* track = mylist->getPidTrackCand();

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
      if ( (track_p1->getTrackData()->getPolarity(ALG_RUNGEKUTTA)) < 0 ) mylist->removeComb();
      if ( (track_p2->getTrackData()->getPolarity(ALG_RUNGEKUTTA)) < 0 ) mylist->removeComb();
      if ( (track_kp->getTrackData()->getPolarity(ALG_RUNGEKUTTA)) < 0 ) mylist->removeComb();
      if ( (track_pim->getTrackData()->getPolarity(ALG_RUNGEKUTTA)) > 0 ) mylist->removeComb();

      // Remove combinations with an high beta value
      if ( track_p1->getTrackData()->getBeta(ALG_RUNGEKUTTA) > 1.5 ) mylist->removeComb();
      if ( track_p2->getTrackData()->getBeta(ALG_RUNGEKUTTA) > 1.5 ) mylist->removeComb();
      if ( track_kp->getTrackData()->getBeta(ALG_RUNGEKUTTA) > 1.5 ) mylist->removeComb();
      if ( track_pim->getTrackData()->getBeta(ALG_RUNGEKUTTA) < -1.5 ) mylist->removeComb();


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
