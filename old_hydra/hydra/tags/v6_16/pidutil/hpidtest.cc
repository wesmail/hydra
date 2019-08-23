// hpidTest.cc 22.06.2003  by Tassilo Christ
// mailto tassilo.christ@ph.tum.de


/////////////////////////////////////////////////////////////////////
//                                                                 //
// This reconstructer is intendet to test the prerformace of       //
// different combinations of PID algorithms, especially the RICH   //
// ring proptery algorithm                                         //
// 								   //
/////////////////////////////////////////////////////////////////////

#include "hpidtest.h"
#include <iostream>
#include <TMath.h>
#include <TString.h>
#include <TFile.h>
#include <TTree.h>
#include <TNtuple.h>
#include "hcategory.h"
#include "hpidtrackcand.h"
#include "hruntimedb.h"
#include "hiterator.h"
#include "hpidfl.h"
#include "hades.h"
#include "hevent.h"
#include "hgeantkine.h"
#include "hpidcommontrack.h"
#include "hpidgeanttrackset.h"
#include "hpidparticlesimexthit.h"
#include "hpidcandidate.h"
#include "hpidtrackcand.h"
#include "hpidhitdata.h"
#include "hkicktrack.h"
#include "hmdcclusinf.h"
#include "hmdchit.h"
#include "hmdcseg.h"
#include "hmdcdef.h"
#include "hmdctrackddef.h"
#include "hrichhit.h"
#include "hrichhitsim.h"

ClassImp(HPidTest)


HPidTest::HPidTest(Char_t* OutputFile)
{
    OutFileName = new TString(OutputFile);

}


HPidTest::~HPidTest(void) 
{

}

Bool_t HPidTest::init(void) 
{
  
  //recognizedParticles = new TNtuple("recognizedParticles","Recognized Particles","rec_ID:true_ID:exp_mom:true_mom:theta:seenInRICH:process:chi2:kick_charge");
  recognizedParticles = new TNtuple("recognizedParticles","Recognized Particles","rec_ID:true_ID:exp_mom:true_mom:theta:seenInRICH:process:chi2:kick_charge:system:sum0:sum1:sum2:tof");

  //Make a tree to store Close Pair Observables in
  CloseTrackObservables = new TTree("CloseTrackObservables","CloseTrackObservables");

  //Make the list of Observables
  TString* LeafList = new TString("nwiresmdc0/F:nwiresmdc1/F:clussizemdc0/F:clussizemdc1/F:chi2/F:cls_finder_lvl0/F:cls_finder_lvl1/F:kicktype/F:momentum/F:pmq/F:avcharge/F:padnr/F:centroid/F:locmax/F:ringtype/F:theta/F:phi/F:sector/F:corrtype/F:rec_ID/F:true_ID/F:seenInRICH/F:kick_charge/F");
  
  //Make a branch in the tree to hold the data
  CloseTrackObservables->Branch("CPPatterns",&PatternInstance, LeafList->Data());


  //Initialize the categories we need

  pParticleCategory = gHades->getCurrentEvent()->getCategory(catPidPart);
    if(!pParticleCategory)
    {
      Error("init","Cannot get Particles");
      return kFALSE;
    }
  if((pIteratorParticle = (HIterator *)pParticleCategory->MakeIterator()) == NULL)
    {
      Error("init", "Cannot make an iterator for particle category");
      return kFALSE;
    }

  pGeantKineCategory = gHades->getCurrentEvent()->getCategory(catGeantKine);
  if(!pGeantKineCategory)
    {
      Error("init","Cannot get GeantKine");
      return kFALSE;
    }
  if((pIteratorGeantKine = (HIterator *)pGeantKineCategory->MakeIterator()) == NULL)
    {
      Error("init", "Cannot make an iterator for GeantKine Category");
      return kFALSE;
    }
  
  pMdcSegCategory = gHades->getCurrentEvent()->getCategory(catMdcSeg);
  if(!pMdcSegCategory)
    {
      Error("init","Cannot get MdcSeg");
      return kFALSE;
    }
  if((pIteratorMdcSeg = (HIterator *)pMdcSegCategory->MakeIterator()) == NULL)
    {
      Error("init", "Cannot make an iterator for HMdcSeg Category");
      return kFALSE;
    }

  pMdcHitCategory = gHades->getCurrentEvent()->getCategory(catMdcHit);
  if(!pMdcHitCategory)
    {
      Error("init","Cannot get MdcHit");
      return kFALSE;
    }
  if((pIteratorMdcHit = (HIterator *)pMdcHitCategory->MakeIterator()) == NULL)
    {
      Error("init", "Cannot make an iterator for HMdcHit Category");
      return kFALSE;
    }

  pMdcClusInfCategory = gHades->getCurrentEvent()->getCategory(catMdcClusInf);
  if(!pMdcClusInfCategory)
    {
      Error("init","Cannot get MdcClusInf");
      return kFALSE;
    }
  if((pIteratorMdcClusInf = (HIterator *)pMdcClusInfCategory->MakeIterator()) == NULL)
    {
      Error("init", "Cannot make an iterator for HMdcClusInf Category");
      return kFALSE;
    }

  return kTRUE;
  
}

Bool_t HPidTest::clearCPPattern()
{
//The RICH observables
  PatternInstance[cp_nwiresmdc0]     = -99.0;
  PatternInstance[cp_nwiresmdc1]     = -99.0;
  PatternInstance[cp_clussizemdc0]   = -99.0;
  PatternInstance[cp_clussizemdc1]   = -99.0;
  PatternInstance[cp_mdcchisquare]   = -99.0;
  PatternInstance[cp_clusfinderlvl0] = -99.0;
  PatternInstance[cp_clusfinderlvl1] = -99.0;
  PatternInstance[cp_kicktype]       = -99.0;
  PatternInstance[cp_momentum]       = -99.0;

  PatternInstance[cp_pmq]            = -99.0;
  PatternInstance[cp_avcharge]       = -99.0; 
  PatternInstance[cp_padnr]  	    = -99.0;
  PatternInstance[cp_centroid]       = -99.0;
  PatternInstance[cp_locmax] 	    = -99.0;
  PatternInstance[cp_ringtype]       = -99.0;

  PatternInstance[cp_theta] 	    = -99.0;
  PatternInstance[cp_phi] 	    = -99.0;
  PatternInstance[cp_sector] 	    = -99.0;
  PatternInstance[cp_corrtype]       = -99.0;

  PatternInstance[cp_recID]       = -99.0;
  PatternInstance[cp_trueID]       = -99.0;
  PatternInstance[cp_seenInRICH]       = -99.0;
  PatternInstance[cp_kickcharge]       = -99.0;
  
  return kTRUE;
}

Bool_t HPidTest::fillCPPattern(HPidParticleSimExtHit* pParticle)
{
  bookKickObservables(pParticle->getPidCandidate()->getTrackCand());
  //bookRingObservables(pParticle->getPidCandidate()->getTrackCand());
  cout << HPidFL::computeCloseTrackProbability(pParticle->getHPidHitData(),1) << endl;
  return kTRUE;

}


Bool_t HPidTest::bookKickObservables(HPidTrackCand* theCandidate)
{
  //We are interested in the properties of the segment from which the kicktrack was made!
  //and whether this KT had geant matches in MDC & META!
  //is it a hadron, single or a double KT, or is it a fake produced by kickplane correlations;
  //PatternInstance[cp_kicktype] = getKickTrackType("withring");
  PatternInstance[cp_kicktype] = HPidFL::countLeptonsInKickTrack((HKickTrackSim*)theCandidate->getKickTrack());
  PatternInstance[cp_momentum] = theCandidate->getKickTrack()->getP();
  Int_t sector=theCandidate->getKickTrack()->getSector();
  Int_t MdcSegmentId = theCandidate->getKickTrack()->getSegment();
  HLocation segLoc;
  segLoc.set(3,sector,0,MdcSegmentId);           // 0 - inner segment

  Int_t Mdc0HitId = ((HMdcSeg*)(pMdcSegCategory->getObject(segLoc)))->getHitInd(0);

  if(Mdc0HitId >= 0) {  //!!!!!!!!!!!!!!!!!!
    Int_t Mdc0ClusId = ((HMdcHit*)(pMdcHitCategory->getObject(Mdc0HitId)))->getClusInfIndex();
    PatternInstance[cp_nwiresmdc0]   = ((HMdcClusInf*)(pMdcClusInfCategory->getObject(Mdc0ClusId)))->getNDrTimes();
    PatternInstance[cp_clussizemdc0] = ((HMdcClusInf*)(pMdcClusInfCategory->getObject(Mdc0ClusId)))->getClusSize();
    PatternInstance[cp_clusfinderlvl0] = ((HMdcClusInf*)(pMdcClusInfCategory->getObject(Mdc0ClusId)))->getLevelClFinding();
  }
  
  
  Int_t Mdc1HitId = ((HMdcSeg*)(pMdcSegCategory->getObject(segLoc)))->getHitInd(1);

  if(Mdc1HitId >= 0) {  //!!!!!!!!!!!!!!!!!!
    Int_t Mdc1ClusId = ((HMdcHit*)(pMdcHitCategory->getObject(Mdc1HitId)))->getClusInfIndex();
    PatternInstance[cp_nwiresmdc1]   = ((HMdcClusInf*)(pMdcClusInfCategory->getObject(Mdc1ClusId)))->getNDrTimes();
    PatternInstance[cp_clussizemdc1] = ((HMdcClusInf*)(pMdcClusInfCategory->getObject(Mdc1ClusId)))->getClusSize();
    PatternInstance[cp_clusfinderlvl1] = ((HMdcClusInf*)(pMdcClusInfCategory->getObject(Mdc1ClusId)))->getLevelClFinding();
  }
  
  PatternInstance[cp_mdcchisquare] = ((HMdcSeg*)(pMdcSegCategory->getObject(MdcSegmentId)))->getChi2();
  return kTRUE;
}

Float_t HPidTest::getRingType(HRichHitSim* theSimRing)
{
  Int_t nMirrTracks1, nMirrTracks2;
  //Check whether this ring is of leptonic origin
  if (HPidFL::isDoubleLepOnMirror(theSimRing, nMirrTracks1, nMirrTracks2))
    {
      return 2.0; 
    }
  else //It was not a double - was it a single ?
    {
      if (HPidFL::isLepOnMirror(theSimRing, nMirrTracks1))
	{
	  return 1.0; 
	}
      else {
	return 0.0;
      } //It was obviously a fake
    }
}

Bool_t HPidTest::bookRingObservables(HPidTrackCand* theCandidate)
{
  //Array of contributing Geant Particles
  Int_t GeantTrackNumber;
  HRichHit* theRing;
  if((theRing = theCandidate->getRichHit())==NULL)
    return 0;
  //Calculate the Angular Difference between the RICH and the MDC hit in phi and theta
  /*
    PatternInstance[cp_dTheta]=theCandidate->getRichHit()->getTheta()-
    TMath::RadToDeg() * theCandidate->getKickTrack()->getTheta();

    PatternInstance[cp_dPhi]=(theCandidate->getRichHit()->getPhi() - getMdcPhi())
    * TMath::Sin(theCandidate->getKickTrack()->getTheta());
    */

  PatternInstance[cp_pmq]=(float)theRing->getRingPatMat();
  PatternInstance[cp_padnr]=(float)theRing->getRingPadNr();
  PatternInstance[cp_avcharge]=(float)theRing->getRingAmplitude()/theRing->getRingPadNr();
  PatternInstance[cp_centroid]=(float)theRing->getCentroid();
  PatternInstance[cp_locmax]=(float)theRing->getRingLocalMax4();
  PatternInstance[cp_phi]=(float)theRing->getPhi();
  PatternInstance[cp_theta]=(float)theRing->getTheta();
  PatternInstance[cp_sector]=(float)theRing->getSector();
  
  //Here we compute the less obvious properties of the ring 
  //which are available for experimental data
  
  //Check what type of ring is our neighbour
  //PatternInstance[cp_neighbour]= (float)findNeighbourType();
  //if(PatternInstance[cp_neighbour]>0){
    //cout << "neighbourtype:" << PatternInstance[cp_neighbour] << endl;
  //}
  
  //Here we retrieve (if desired) the simulation info)
  /*
    if(USE_SIMINFO){
    theSimRing = (HRichHitSim*)theRing;
    //We check whether this was a ring whose creator particles are seen
    //also in other detectorscontributing to this candidte!
    PatternInstance[cp_iscorrgeantlep]=theCandidate->getGeantTrackSet()->getCommonTrack();
    
    //Loop over all contrib tracks in the ring
    PatternInstance[cp_dalitz]=0.0;
    PatternInstance[cp_gamma]=0.0;
    for(Int_t i =0;i<HPidGeantTrackSet::kMaxRichTracks;i++)
    {
    //Get the Geant Track ID if present
    GeantTrackNumber=theCandidate->getGeantTrackSet()->getRichTrack(i);
    if(GeantTrackNumber!=0)
    {
    //Check whether this track is a dalitz one
    if(HPidFL::isLepFromPi0DalitzDecay(GeantTrackNumber,"urqmd"))
    {
    //cout << "checking for dalitz component" << endl;
    //And add its contribution to the ring to the dalitz fraction
    PatternInstance[cp_dalitz]+=HPidFL::calcRichWeight(theSimRing,i); 
    }
    
    //Check whether this track is a conversion one
    if(HPidFL::isLepFromPi0GammaConv(GeantTrackNumber))
    {
    //And add its contribution to the ring to the dalitz fraction
    PatternInstance[cp_gamma]+=HPidFL::calcRichWeight(theSimRing,i); 
    }
    
    }
    
    }
    PatternInstance[cp_ringtype] = getRingType(theSimRing);
    
    if(PatternInstance[cp_ringtype]==1.0)
    {
    //For a single ring we compute the (angular) next neighbour distance of a candidate
    //The candidate has to be MORE than just a ring!
    PatternInstance[cp_openingAngle]=getClosestCandidateAngularDistance();
    }
    }    
  */
  HRichHitSim* theSimRing = (HRichHitSim*)theRing;
  PatternInstance[cp_ringtype] = getRingType(theSimRing);
  return kTRUE;
}

Int_t HPidTest::execute(void)
{
  evcount++;
  Int_t particleCounter =0;
  Float_t itsReconstructedId = -99.0;
  Float_t itsTrueId = -99.0;
  Float_t itsReconstructedCharge = 0.0;
  Float_t kickCharge = -99.0;
  Float_t itsSystem = -1.0;

  Float_t itsSum0=-99.;
  Float_t itsSum1=-99.;
  Float_t itsSum2=-99.;
  Float_t itsTof=-99.;

  const HPidGeantTrackSet* theTrackSet;
  Char_t CommonDetectors[1];
  Short_t CommonTracks[1];



  //Loop over all PidParticles;
  pIteratorParticle->Reset();
  while((theParticle = (HPidParticleSimExtHit*) pIteratorParticle->Next())!=NULL)
    { 

      clearCPPattern();
      fillCPPattern(theParticle);
      itsReconstructedId = -99.0;
      itsTrueId = -99.0;
      itsReconstructedCharge = 0.0;
      kickCharge = -99.0;
      itsSystem=-1.0;
      CommonDetectors[0]=HPidCommonTrack::kTrackNotSet;
      CommonTracks[0]=-99;

      //get reconstructed geant Id of the particle;
      itsReconstructedId = (Float_t)(theParticle->getPid());
      
      itsReconstructedCharge = (Float_t)(HPidPhysicsConstants::charge((Int_t)itsReconstructedId));
      
      HPidHitData* phitdata=theParticle->getHPidHitData();

      itsTof=phitdata->tof;
      itsSum0=phitdata->sum0;
      itsSum1=phitdata->sum1;
      itsSum2=phitdata->sum2;
      
      //check what was the true geant id;
      theTrackSet = theParticle->getGeantTrackSet();
      theTrackSet->fillCommonTracks(CommonDetectors, CommonTracks,1);

      if(!(CommonDetectors[0]==HPidCommonTrack::kTrackFull || 
	   CommonDetectors[0]==HPidCommonTrack::kTrackKickPlane))
	{
	  itsTrueId=0.0; //We have a fake association of mdc and meta
	  kickCharge = (Float_t)(theParticle->getKickTrack()->getCharge());
	  itsSystem = (Float_t)(theParticle->getKickTrack()->getSystem());
	  if(itsTof<0 && itsSystem==0) cout << "UAH!" << endl;
	  if(!(kickCharge==1.0 || kickCharge==-1.0))cout << "OUCH fake!" << endl;
	}
      else
	{
	  itsTrueId = (Float_t)(theParticle->getTrackPid());	  
	  kickCharge = (Float_t)(theParticle->getKickTrack()->getCharge());
	  itsSystem = (Float_t)(theParticle->getKickTrack()->getSystem());
	  if(itsTof<0 && itsSystem==0) cout << "UAH!" << endl;
	  if(!(kickCharge==1.0 || kickCharge==-1.0))cout << "OUCH true!" << endl;
	}

      if(itsTrueId>0) //no fakes!
	{
	  if(!(kickCharge==1.0 || kickCharge==-1.0)) cout <<"MegaOUCH!"<< endl;

	  recognizedParticles->Fill(itsReconstructedId,
				    itsTrueId,
				    theParticle->P(),
				    theParticle->getTrackMom()->Mag(),
				    theParticle->thetaDeg(),
				    (Float_t)(theParticle->getHPidHitData()->getFlagRICH()),
				    (Float_t)(theParticle->getTrack()->getProcess()),
				    theParticle->getHPidHitData()->iMdcChiSquare,
				    kickCharge, itsSystem,
				    itsSum0,itsSum1,itsSum2, itsTof);
	}
      else //fakes here
	{
	  if(!(kickCharge==1.0 || kickCharge==-1.0)) cout <<"MegaOUCH!"<< endl;

	  recognizedParticles->Fill(itsReconstructedId,
				    itsTrueId,
				    theParticle->P(),
				    -99.0,
				    theParticle->thetaDeg(),
				    (Float_t)(theParticle->getHPidHitData()->getFlagRICH()),
				    -99.0,
				    theParticle->getHPidHitData()->iMdcChiSquare,
				    kickCharge,itsSystem,
				    itsSum0,itsSum1,itsSum2, itsTof);
	}
      PatternInstance[cp_momentum]=theParticle->P();
      PatternInstance[cp_mdcchisquare]=theParticle->getHPidHitData()->iMdcChiSquare;
      PatternInstance[cp_kickcharge]=kickCharge;      
      PatternInstance[cp_recID]=itsReconstructedId;      
      PatternInstance[cp_trueID]=itsTrueId;      
      PatternInstance[cp_theta]=theParticle->thetaDeg();
      PatternInstance[cp_phi]=theParticle->phiDeg();
      PatternInstance[cp_seenInRICH]=(Float_t)(theParticle->getHPidHitData()->getFlagRICH());
      CloseTrackObservables->Fill();
      particleCounter++;
    }

  if(!(evcount%1000))
    {
      TFile* OutputFile = new TFile(OutFileName->Data(),"RECREATE");
      OutputFile->cd();
      recognizedParticles->Write();
      CloseTrackObservables->Write();
      OutputFile->Close();
      delete OutputFile;
    } 
  
  return 0;
}


Bool_t HPidTest::finalize(void) 
{

  TFile* OutputFile = new TFile("pidPerformance.root","RECREATE");
  OutputFile->cd();
  recognizedParticles->Write();
  CloseTrackObservables->Write();
  OutputFile->Close();
  
  return kTRUE;
}































