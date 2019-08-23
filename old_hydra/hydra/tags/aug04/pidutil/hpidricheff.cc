// hpidPDFRichEff.cc 3.05.2004  by Tassilo Christ
// mailto tassilo.christ@ph.tum.de


//helper class to check rich efficiency

#include "hpidricheff.h"
#include <iostream>
#include <stdlib.h>
#include <TMath.h>
#include <TString.h>
#include <TFile.h>
#include "hcategory.h"
#include "hpidtrackcand.h"
#include "hruntimedb.h"
#include "hiterator.h"
#include "hrichhit.h"
#include "hrichhitsim.h"
#include "hkicktrack.h"
#include "hkicktracksim.h"
#include "richdef.h"
#include "hpidfl.h"
#include "hrichanalysispar.h"
#include "hrichgeometrypar.h"
#include "hades.h"
#include "hevent.h"
#include "hpidgeanttrackset.h"
#include "hpidcommontrack.h"
#include "TH1F.h"
#include "piddef.h"
ClassImp(HPidRichEff)


HPidRichEff::HPidRichEff(Char_t* OutputFile, Int_t which_particle_ID, Bool_t getGeantMom)
{
  evcount=0;
  takeRealMom=getGeantMom;
  desiredParticleId=which_particle_ID;
  OutFileName = new TString(OutputFile);
  OutputFile=NULL;
}


HPidRichEff::~HPidRichEff(void) 
{
  delete OutFileName;
}

Bool_t HPidRichEff::init(void) 
{
  OutputFile = new TFile(OutFileName->Data(),"RECREATE");
  const Int_t nMomBins=31;
  Float_t bins[nMomBins+1] ={0.0, 20.0, 40.0, 60.0, 80.0, 100.0,
				  120.0, 140.0, 160.0, 180.0, 200.0,
				  250.0, 300.0, 350.0, 400.0, 450.0, 500.0,
				  600.0, 700.0, 800.0, 900.0, 1000.0,
				  1100.0, 1200.0, 1300.0, 1400.0, 1500.0,
				  1600.0, 1700.0, 1800.0, 1900.0 ,2000.0};

  //Momentum-histograms for hadrons/leptons with and without ring
  MomDistHadRing = new TH1F("MomDistHadRing","MomDistHadRing",nMomBins,bins);
  MomDistLepRing = new TH1F("MomDistLepRing","MomDistLepRing",nMomBins,bins);
  MomDistHadNoRing = new TH1F("MomDistHadNoRing","MomDistHadNoRing",nMomBins,bins);
  MomDistLepNoRing = new TH1F("MomDistLepNoRing","MomDistLepNoRing",nMomBins,bins);

  //Sum of all hadrons
  MomDistHadTotal = new TH1F("MomDistHadTotal","MomDistHadTotal",nMomBins,bins);
  MomDistLepTotal = new TH1F("MomDistLepTotal","MomDistLepTotal",nMomBins,bins);

  //Ratio of hadrons/leptons with ring to all hadrons/leptons
  MomDistHadRatio = new TH1F("MomDistHadRatio","MomDistHadRatio",nMomBins,bins);
  MomDistLepRatio = new TH1F("MomDistLepRatio","MomDistLepRatio",nMomBins,bins);    
  

  //Initialize the categories we need

  pInputCategory = gHades->getCurrentEvent()->getCategory(catPidTrackCand);
    if(!pInputCategory)
    {
      Error("init", "Cannot build HPidTrackCand Category");
      return kFALSE;
    }
  
  if((pIteratorInput = (HIterator *)pInputCategory->MakeIterator()) == NULL)
    {
      Error("init", "Cannot make an iterator for HPidTrackCand Category");
      return kFALSE;
    }
  return kTRUE;
  
}

Bool_t HPidRichEff::checkForRing(void)
{

  //check whether a ring is present;
  if(theCandidate->getRingId() >= 0)
    {
      if((theRing = theCandidate->getRichHit()) == NULL)
	{
	  Error("checkForRing PDF builder", "Cannot get HRichHit for ring: %d",
		theCandidate->getRingId());
	  
	  exit(-1);
	}
      
      return kTRUE;
    }
  return kFALSE;
}

Bool_t HPidRichEff::checkForKickTrack(void)
{
  //check whether a KickTrack is present;
  if(theCandidate->getKickTrackId() >= 0)
    {
      if((theKickTrack = theCandidate->getKickTrack()) == NULL)
	{
	  Error("checkForKickTrack PDF Builder", "Cannot get HKickTrack for the track: %d",
		theCandidate->getKickTrackId());
	  
	  exit (-1);
	}
      return kTRUE;
    }
  return kFALSE;
}

Int_t HPidRichEff::execute(void)
{
  Int_t nHadRing=0;
  Int_t nHadNoRing=0;
  Int_t nLepRing=0;
  Int_t nLepNoRing=0;

  //Get all candidates
  Int_t wait;
  evcount++;
  Bool_t hasARing = kFALSE;
  Bool_t hasAKickTrack = kFALSE;
  Int_t leptonsInKickTrack=0;
  Float_t fKickMomentum = -1.0;
  Float_t fTrueMomentum = -1.0;

  //Count how many leptons match in all detectors
  Int_t nCommonLeptonsFull;
  
  //Count how many leptons match in Rich and MDC
  Int_t nCommonLeptonsRichMdc;
  
  //Count how many leptons match in META and MDC
  Int_t nCommonLeptonsMdcMeta;
  
  //Count how many particles matched in all detectors/in MDC and META
  Int_t nMatchesFull, nMatchesMdcMeta;

  //Is this candidate an electron?
  Int_t GeantPid;

  //Loop over all PidTrackCandidates;
  pIteratorInput->Reset();

  while((theCandidate = (HPidTrackCand *)pIteratorInput->Next()) != NULL)
    {
      leptonsInKickTrack=0;
      fKickMomentum = -1.0;
      hasARing = checkForRing();
      hasAKickTrack = checkForKickTrack();

      if(!hasAKickTrack)
	{
	  //cout << "HPidTrackCandidate without kicktrack -ohoh!" << endl;
	  continue;
	}
      
      leptonsInKickTrack=HPidFL::countLeptonsInKickTrack((HKickTrackSim*) (theCandidate->getKickTrack()));

      fKickMomentum=theCandidate->getKickTrack()->getP();
      fTrueMomentum = HPidFL::getKineMomentum(theCandidate);      
      //Case 1: Ring+Kicktrack
      if(hasARing)
	{

	  //This function counts how many particles/leptons were common in which detectors
	  HPidFL::getGeantMatches(theCandidate, nCommonLeptonsRichMdc,nMatchesMdcMeta,nMatchesFull,
				  nCommonLeptonsFull, nCommonLeptonsMdcMeta, GeantPid);


	  if(GeantPid!=desiredParticleId && desiredParticleId !=-99) continue; //Pick only positrons
	  if(nCommonLeptonsMdcMeta>0) //A lepton in the outer detectors and the MDC!
	    {
	      if(takeRealMom)
		MomDistLepRing->Fill(fTrueMomentum);
	      else
		MomDistLepRing->Fill(fKickMomentum);
	      nLepRing++;
	    }
	  else
	    {
	      if(takeRealMom)
		MomDistHadRing->Fill(fTrueMomentum);
	      else
		MomDistHadRing->Fill(fKickMomentum);
	      nHadRing++;
	    }
	  
	}
      
      else //Track without ring
	{
	  //If no ring was found this function checks for common tracks in the MDC and in META
	  HPidFL::getGeantMatches(theCandidate,nMatchesMdcMeta,nMatchesFull, nCommonLeptonsMdcMeta, GeantPid);

	  if(GeantPid!=desiredParticleId && desiredParticleId!=-99) continue;
	  if(nCommonLeptonsMdcMeta>0)
	    {
	      if(takeRealMom)
		MomDistLepNoRing->Fill(fTrueMomentum);	      
	      else
		MomDistLepNoRing->Fill(fKickMomentum);
	      nLepNoRing++;
	    }
	  else
	    {
	      if(takeRealMom)
		MomDistHadNoRing->Fill(fTrueMomentum);
	      else
		MomDistHadNoRing->Fill(fKickMomentum);
	      nHadNoRing++;
	    }

      
	}



      /*
      if(leptonsInKickTrack)
	{
	  cout << "HRICHEFF: leptons in kicktrack = " << leptonsInKickTrack << endl;
	  cout << "HRICHEFF: Leptonic matches KT/META = " << nCommonLeptonsMdcMeta << endl;
	  cout << "HRICHEFF: matches KT/META = " << nMatchesMdcMeta << endl;
	  cin >> wait;
	}
      */
    }
  //cout << "HRICHEFF: " << nHadRing << " "<< nHadNoRing<< " "<< nLepRing<< " "<< nLepNoRing<< endl;
  /*if(!(evcount%2000))
    {
    //save();
    } 
  */
  //cin >> wait;
  return 0;
}

Bool_t HPidRichEff::save(void)
{
  OutputFile->cd();

  MomDistHadTotal->Add(MomDistHadNoRing,MomDistHadRing,1.0,1.0);
  MomDistLepTotal->Add(MomDistLepNoRing,MomDistLepRing,1.0,1.0);

  MomDistLepRatio->Divide(MomDistLepRing,MomDistLepTotal);
  MomDistHadRatio->Divide(MomDistHadRing,MomDistHadTotal);
  
  MomDistHadRing->SetDirectory(OutputFile);
  MomDistLepRing->SetDirectory(OutputFile);
  MomDistHadNoRing->SetDirectory(OutputFile);
  MomDistLepNoRing->SetDirectory(OutputFile);
  MomDistHadTotal->SetDirectory(OutputFile);
  MomDistLepTotal->SetDirectory(OutputFile);
  MomDistHadRatio->SetDirectory(OutputFile);
  MomDistLepRatio->SetDirectory(OutputFile);
  cout << "saving" << endl;

  MomDistHadRing->Write();
  MomDistLepRing->Write();
  MomDistHadNoRing->Write();
  MomDistLepNoRing->Write();
  MomDistHadTotal->Write();
  MomDistLepTotal->Write();
  MomDistHadRatio->Write();
  MomDistLepRatio->Write();

  //OutputFile->Close();
  return kTRUE;
}

Bool_t HPidRichEff::finalize(void) 
{
  return save();
}




























