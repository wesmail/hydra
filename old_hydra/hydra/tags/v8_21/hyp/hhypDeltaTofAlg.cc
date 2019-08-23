//*-- AUTHOR : I. Froehlich
//*-- Modified : 05.07.2005 by T. Perez
//*-- Modified : 07.07.2005 by T. Perez
//*-- Modified : 01.02.2006 by T. Perez  Now pointer to Track Categories are checked before use.
//_HADES_CLASS_DESCRIPTION
////////////////////////////////////////////////////////////////////////
//
// HHypDeltaTofAlg
//
// Algorithm which removes combinations acconding to time-of-flight
// differences
//
// This code is most likely infunctional, as the algorithm is not
// implemented correctly (see warnings). This code should be
// examined, corrected or even better rewritten.
//
// Features:
// 1. TimeOfFlight from HSpline has already been recalculated  due to the
//    lack of START detector. But this should affect time differences.
//    Original Time can anyhow be recover with a little bit more work.
// 2. It will only work with less than 10 particles in an event. Anyhow this
//    is harcoded somewhere else, it should be changed top something more
//    flexible.
// 3. We should pass DEBUG flag as a execution parameter.
//    -> On "init" or "constructor" --  debugFlag=0 default.
// 4. Many variables can be declared earlier and the reused. to save time (?).
// 5. Can we have to differnt TrackingAlgos in the same file for 2 different
//    HPidTrackCands? --> Now i assume NO. so its checked once per pair.
//  MOMENTUM ALGORITHMS INDEXES defined in  "$HADDIR/pid/piddef.h"
//  ALG_KICK       0
//  ALG_KICK123    1
//  ALG_SPLINE     2
//  ALG_RUNGEKUTTA 4
//  tracking cats defined here : /misc/halo/anal/dev/mdctrackG/hmdctrackgdef.h
//
//    Tiago Perez.
//
//  Added new method getTrackInfo(). This method handles all that complicated
//  pointers to retrieve track info. Then sets TOF_EXP and pathLength
//    T. Perez
////////////////////////////////////////////////////////////////////////

#warning "This code is not written in a way to be compatible with other hyp features"
#warning "and (most likely)"
#warning "will not work as the algorithm is not implemented correctly"
#warning "(and THIS is really serious)"
#warning "Advise: Use HypDeltaTofMeanAlg instead."

using namespace std;

#include "hhypDeltaTofAlg.h"
#include "TVector3.h"
#define deltaTOFdebug 0

ClassImp(HHypDeltaTofAlg)

HHypDeltaTofAlg::HHypDeltaTofAlg(Char_t *name_i, Option_t par[])
  :HHypBaseAlgorithm(name_i,par)
{
  dtof = -1;
  sigmaDeltaTof = -1;

  tof1_exp = -1;
  tof2_exp = -1;
  tof1_theo = -1;
  tof2_theo = -1;
  beta1_th = -1;
  beta2_th = -1;
  pathLength1 = -1;
  pathLength2 = -1;

  momIndex1 = -1;
  momIndex2 = -1;
  trackIndex1 = -1;
  trackIndex2 = -1;
}

HHypDeltaTofAlg::~HHypDeltaTofAlg()
{
}

Bool_t HHypDeltaTofAlg::execute()
{
  if (mylist->getNpart() < 2)
    return kFALSE;
  //needs at least 2 particles

  // Resetting the list and start looping over the combinations
  // Loop is only done over the VALID combinations
  mylist->CombIteratorReset();
  if (deltaTOFdebug == 1)
    cerr << "-------------------------------------" << endl;

  Int_t j = 0;

  while (mylist->CombIterator()) {
    j++;
    // ************** START DEBUG MSG 1/3 ***************************//
    if (deltaTOFdebug == 1)
      cerr<<"Combination # "<<j<< " prob " << mylist->getProbAlg() << endl;
    // ************** END DEBUG MSG 1/3   ***************************//

    dtof = 0;
    sigmaDeltaTof = 0;
    Double_t chi2 = 0;
    Double_t chi = 0;

    Int_t i = 0;
    Double_t mean = 0;

    
    

    for (i = 0; i < (mylist->getNpart() - 1); i++) {
      //loop over all REAMINING particles
      /* particle 0 is the reference particle */
      hyppid1 = mylist->getPid(0);
      particle1 = (HPidTrackCand *) mylist->getPidTrackCand(0);
      
      hyppid2 = mylist->getPid(i + 1);
      particle2 = mylist->getPidTrackCand(i + 1);
      
      // making DeltaTof here
      //get mom algo
      momIndex1 = particle1->getTrackData()->getBestMomAlg();
      momIndex2 = particle2->getTrackData()->getBestMomAlg();

      // ************** START DEBUG MSG 2 ***************************//
      if (deltaTOFdebug == 1){
	cerr<<"<debug> HHypDeltaTofAlg::"<<__func__<<"(): L"<<__LINE__<<endl;
	cerr<<" i = "<<i<<endl;
				cerr<<" particle 1 is "<</*particle1->getPid()<<*/"  hyppid1 :"<<hyppid1<<endl;
				cerr<<" particle 2 is "<</*particle2->getPid()<<*/"  hyppid2 :"<<hyppid2<<endl;
	cerr<<" momIndex 1  = "<<momIndex1<<endl;
	cerr<<" momIndex 2  = "<<momIndex2<<endl;
      }

			TVector3 Vect3;
      getTrackInfo(momIndex1, momIndex2);
      // Claculate Theoretical Values Here.
      //part 1
      mass1 = HPidPhysicsConstants::mass(hyppid1);
			Vect3.SetMagThetaPhi(
				particle1->getTrackData()->getMomenta(ALG_RUNGEKUTTA),
				particle1->getTrackData()->getRKTheta()*TMath::DegToRad(),
				particle1->getTrackData()->getRKPhi()*TMath::DegToRad()
			);
      partHyp1mom = Vect3;
      partHyp1.SetVectM(partHyp1mom, mass1);
      beta1_th = partHyp1.Beta();       // calculated beta from Ptracking and Mhypothesis
      tof1_theo = pathLength1 / (beta1_th * c);

      //part 2
      mass2 = HPidPhysicsConstants::mass(hyppid2);
			Vect3.SetMagThetaPhi(
				particle2->getTrackData()->getMomenta(ALG_RUNGEKUTTA),
				particle2->getTrackData()->getRKTheta()*TMath::DegToRad(),
				particle2->getTrackData()->getRKPhi()*TMath::DegToRad()
			);
      partHyp2mom = Vect3;
      partHyp2.SetVectM(partHyp2mom, mass2);
      beta2_th = partHyp2.Beta();       // calculated beta from Ptracking and Mhypothesis
      tof2_theo = pathLength2 / (beta2_th * c);

      //******now I come to the errors*****//
      //1.) error for tof_theo is based on momentum error
      Double_t momentum1 = partHyp1mom.Mag();
      Double_t momentum2 = partHyp2mom.Mag();

      //2.) How to get the relative mom. error? Just some approx.
      Double_t relative_momentum_error1 = 0.05 + (momentum1 / 2000) * 0.15;
      Double_t relative_momentum_error2 = 0.05 + (momentum2 / 2000) * 0.15;

      Double_t sigma_tof1_theo = tof1_theo * (2 * relative_momentum_error1);
      Double_t sigma_tof2_theo = tof2_theo * (2 * relative_momentum_error2);

      //****** END error calc *******//

      //      cerr<<" i = "<<i<<endl;
      deltaTofExp[i] = (tof2_exp - tof1_exp);
      deltaTofTheo[i] = (tof2_theo - tof1_theo);
      //store relative errors SQUARED
      deltaTofExpError[i] =
        ((0.250 / tof1_exp) * (0.250 / tof1_exp) +
         (0.250 / tof2_exp) * (0.250 / tof2_exp));
      deltaTofTheoError[i] =
        ((sigma_tof1_theo / tof1_theo) * (sigma_tof1_theo / tof1_theo) +
         (sigma_tof2_theo / tof2_theo) * (sigma_tof2_theo / tof2_theo));

      ///////////////// START DEBUG MSG 2/3 //////////////////////////////////
      if (deltaTOFdebug == 1) {
        cerr << "\t tof1_exp " << tof1_exp << "\t tof2_exp " << tof2_exp <<
          endl;
        cerr << "\t tof1_theo " << tof1_theo << "\t tof2_theo " << tof2_theo <<
          endl;
        cerr << "\t deltaTofExp[" << i << "] " << deltaTofExp[i] <<
          "\t deltaTofTheo[" << i << "] " << deltaTofTheo[i] << endl;;
        cerr << "\t deltaTofTheoError[" << i << "] " << deltaTofTheoError[i] <<
          endl;
      }                         // End if DEBUG message
      ////////////////////////////////////////////////////////////////////////
    }                           // end loop over all particles

    //calculation mean, sigma etc...
    Int_t k;

    // mean
    dtof = 0;mean = 0;

    for (k = 0; k < i; k++) {
#warning "Why fabs()??? This kills the direction of the offset!!!"
      deltaTOF[k] = fabs(fabs(deltaTofExp[k]) - fabs(deltaTofTheo[k]));
      dtof = dtof + deltaTOF[k];

      //alterative: making chi2-test with constant value =0
      // I assume a constant error for each data point....

      chi2 +=
        ((deltaTofExp[k] - deltaTofTheo[k]) * (deltaTofExp[k] -
                                               deltaTofTheo[k])) /
        sqrt(deltaTofExpError[k] + deltaTofTheoError[k]);

      //I also calculate a mean. This is needed for the
      //start time recalculation
#warning "Why fabs()??? This kills the direction of the offset!!!"
      mean += (fabs(deltaTofExp[k]) - fabs(deltaTofTheo[k]));

      //      cerr<<"\t deltaTOF["<<k<<"] = "<<deltaTOF[k]<<endl;
    }
    dtof = (1. / i) * dtof;
    if (deltaTOFdebug == 1)
      cerr << "\t dtof " << dtof << endl;

    // sigma
    sigmaDeltaTof = 0;
    for (k = 0; k < i; k++) {
      sigmaDeltaTof = pow((dtof - deltaTOF[k]), 2);     // dtof**2
    }
    sigmaDeltaTof = sqrt((1. / (i - 1)) * sigmaDeltaTof);

    //normalize chi2
	if(i!=1)
	{
      chi2 /= (i - 1);
      mean /= (i - 1);
	}
    chi = sqrt(chi2);

    mylist->resetProbAlg(exp(-chi2));

    //store more data here....
    mylist->setUserValue(DELTATOF_DTOF, dtof);
    mylist->setUserValue(DELTATOF_SIGMA, sigmaDeltaTof);
    mylist->setUserValue(DELTATOF_OFFSET,tof1_theo - tof1_exp - mean);
    mylist->setUserValue(DELTATOF_CHI2,chi2);

    //cout << "tof1_theo "
    // << tof1_theo << " tof exp:" <<  tof1_exp  << " mean:" << mean << " chi:" << chi <<  " dtof:" << dtof <<endl;

    if (histofile) {

      UInt_t ntracks = mylist->getNpart();

      for (UInt_t j = 0; j < ntracks; j++) {
        //look if we have the particles
        HPidTrackCand *track = mylist->getPidTrackCand(j);

        if (track) {
          Int_t pid = mylist->getPid(j);
          Double_t beta = track->getTrackData()->getBeta(ALG_RUNGEKUTTA) * track->getTrackData()->getPolarity(ALG_RUNGEKUTTA);
          Double_t mom = track->getTrackData()->getMomenta(ALG_RUNGEKUTTA);

          qa->Fill(pid, mom, beta, dtof, sigmaDeltaTof, mylist->getProbAlg());
        }
      }
    }

    // ************** START DEBUG 3/3 ***********************//
    if (deltaTOFdebug == 1) {
      cerr<<"\t dtof "<<dtof<<endl;
      cerr<<"\t chi  "<<chi <<endl;
    } // ************ END DEBUG 3/3  ************************//

    
    mylist->resetProbAlg(exp(-0.5*chi));

  } //END Iterator

  if (exitIdx > -1)
    return kTRUE;
  return kFALSE;
}                               // End Execute()

Bool_t HHypDeltaTofAlg::init()
{
  // need to get name from channel
  TString input(channel->Get(initList));

  if (histofile)
    qa =
      new TNtuple(input + TString("_dtof_debug"), "Spectra ntuple",
                  "pid:mom:beta:dtof:sigma:probalg");

  return kTRUE;
}

Bool_t HHypDeltaTofAlg::reinit()
{
  return kTRUE;
}

Bool_t HHypDeltaTofAlg::finalize()
{

  if (histofile)
    qa->Write();

  return kTRUE;

}

Bool_t HHypDeltaTofAlg::getTrackInfo(Int_t momIndex1, Int_t momIndex2)
{
  if (deltaTOFdebug == 1){
    cerr<<"<debug> HHypDeltaTofAlg::"<<__func__<<"(): L"<<__LINE__<<endl;
  }

  HCategory *pidTrackCandCat =
    (HCategory *) gHades->getCurrentEvent()->getCategory(catPidTrackCand);
/*  HCategory *pidCandidateCat =
    (HCategory *) gHades->getCurrentEvent()->getCategory(catPidCandidate);

  HPidCandidate *pidCand1 =
    (HPidCandidate *) pidCandidateCat->getObject(particle1->
                                                 getPidCandidateIndex());
  HPidCandidate *pidCand2 =
    (HPidCandidate *) pidCandidateCat->getObject(particle2->
                                                 getPidCandidateIndex());
*/
  HPidTrackCand *pidTrackCand1 =particle1;
    //(HPidTrackCand *) pidTrackCandCat->getObject(pidCand1->getTrackCandIndex());
  HPidTrackCand *pidTrackCand2 =particle2;
    //(HPidTrackCand *) pidTrackCandCat->getObject(pidCand2->getTrackCandIndex());
  pidTrackCandCat = pidTrackCand1->buildPidTrackCandCategory();


  // DEFAULT VALUES to -1
  tof1_exp = -1;              // Exp TOF
  pathLength1 = -1;           // Path Length
  tof2_exp = -1;              // Exp TOF
  pathLength2 = -1;           // Path Length
  if (momIndex1 == ALG_SPLINE) {
    if (deltaTOFdebug == 1){
      cerr<<"<debug> HHypDeltaTofAlg::"<<__func__<<"(): L"<<__LINE__<<" ::  Using SPLINE momentum"<<endl;
    }
    trackIndex1 = pidTrackCand1->itsTrackData.nSplineTrackInd;
    trackIndex2 = pidTrackCand2->itsTrackData.nSplineTrackInd;

    HCategory *splineCat =
      (HCategory *) gHades->getCurrentEvent()->getCategory(catSplineTrack);
    if (splineCat==NULL){
      cerr<<"<error> HHypDeltaTofAlg::"<<__func__<<"(): L"<<__LINE__<<" ::  No RungeKutta Tracking Category Found !"<<endl;
      return kFALSE;
    }
    HSplineTrack *track1 = (HSplineTrack *) splineCat->getObject(trackIndex1);
    HSplineTrack *track2 = (HSplineTrack *) splineCat->getObject(trackIndex2);
    // get Tracking Values
    tof1_exp = track1->getTof();        // Exp TOF
    pathLength1 = track1->getTofDist(); // Path Length
    tof2_exp = track2->getTof();        // Exp TOF
    pathLength2 = track2->getTofDist(); // Path Length
  }
  else if (momIndex1 == ALG_RUNGEKUTTA) {
    if (deltaTOFdebug == 1){
      cerr<<"<debug> HHypDeltaTofAlg::"<<__func__<<"(): L"<<__LINE__<<" :: Using RK momentum"<<endl;
    }
    trackIndex1 = pidTrackCand1->itsTrackData.nRKTrackInd;
    trackIndex2 = pidTrackCand2->itsTrackData.nRKTrackInd;
    HCategory *trackCat =
      (HCategory *) gHades->getCurrentEvent()->getCategory(catRKTrackB);

    if (trackCat==NULL){
      cerr<<"<error> HHypDeltaTofAlg::"<<__func__<<"(): L"<<__LINE__<<" :: No RungeKutta Tracking Category Found !"<<endl;
      return kFALSE;
    }
    HRKTrackB *track1 = (HRKTrackB *) trackCat->getObject(trackIndex1);
    HRKTrackB *track2 = (HRKTrackB *) trackCat->getObject(trackIndex2);
    // get Tracking Values
    tof1_exp = track1->getTof();        // Exp TOF
    pathLength1 = track1->getTofDist(); // Path Length
    tof2_exp = track2->getTof();        // Exp TOF
    pathLength2 = track2->getTofDist(); // Path Length
    
  }
  else {
    cerr<<"<error> HHypDeltaTofAlg::"<<__func__<<"(): L"<<__LINE__<<" :  MOMENTUM ALG not not found !!! \n" << endl;
  }

  return kTRUE;
}
