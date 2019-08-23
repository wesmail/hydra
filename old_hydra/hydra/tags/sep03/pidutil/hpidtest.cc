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
#include "hpidhitdata.h"
ClassImp(HPidTest)


HPidTest::HPidTest(Char_t* OutputFile)
{
  

}


HPidTest::~HPidTest(void) 
{

}

Bool_t HPidTest::init(void) 
{
  
  recognizedParticles = new TNtuple("recognizedParticles","Recognized Particles","rec_ID:true_ID:exp_mom:true_mom:theta:seenInRICH:process,chi2");

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

  return kTRUE;
  
}


Int_t HPidTest::execute(void)
{
  evcount++;
  Int_t particleCounter =0;
  Int_t itsReconstructedId = -99;
  Int_t itsTrueId = -99;
  Int_t itsReconstructedCharge = 0;
  const HPidGeantTrackSet* theTrackSet;
  Char_t CommonDetectors[100];
  Short_t CommonTracks[100];

  //Loop over all PidParticles;
  pIteratorParticle->Reset();
  while((theParticle = (HPidParticleSimExtHit*) pIteratorParticle->Next())!=NULL)
    { 

      //get reconstructed geant Id of the particle;
      itsReconstructedId = theParticle->getPid();
      
      itsReconstructedCharge = HPidPhysicsConstants::charge(itsReconstructedId);
      
      //check what was the true geant id;
      theTrackSet = theParticle->getGeantTrackSet();
      theTrackSet->fillCommonTracks(CommonDetectors, CommonTracks,100);
      if(!(CommonDetectors[0]==HPidCommonTrack::kTrackFull || 
	   CommonDetectors[0]==HPidCommonTrack::kTrackKickPlane ||
	   CommonDetectors[0]==HPidCommonTrack::kTrackNotSet))
	{
	  itsTrueId=0; //We have a fake association of mdc and meta
	}
      else
	{
	  itsTrueId = theParticle->getTrackPid();	  
	}

      //itsTrueCharge = HPidPhysicsConstants::charge(itsTrueId);
      if(itsTrueId>0) //no fakes!
	{
	  recognizedParticles->Fill(itsReconstructedId, itsTrueId,
				    theParticle->P(), theParticle->getTrackMom()->Mag(),
				    theParticle->thetaDeg(),
				    (Float_t)(theParticle->getHPidHitData()->getFlagRICH()),
				    (Float_t)(theParticle->getTrack()->getProcess(),theParticle->getHPidHitData()->iMdcChiSquare)
				    );
	}
      else //fakes here
	{
	  recognizedParticles->Fill(itsReconstructedId, itsTrueId,
				    theParticle->P(), -99,
				    theParticle->thetaDeg(),
				    (Float_t)(theParticle->getHPidHitData()->getFlagRICH()),
				    -99,theParticle->getHPidHitData()->iMdcChiSquare
				    );
	}
      particleCounter++;
    }

  if(!(evcount%1000))
    {
      TFile* OutputFile = new TFile("pidPerformance.root","RECREATE");
      OutputFile->cd();
      recognizedParticles->Write();
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
  OutputFile->Close();
  
  return kTRUE;
}































