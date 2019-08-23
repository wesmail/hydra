// hpidPDFBuilderExp.cc 23.07.2003  by Tassilo Christ
// mailto tassilo.christ@ph.tum.de


/////////////////////////////////////////////////////////////////////
//                                                                 //
// This reconstructer builds a tree holding information about the  //
// RICH rings and the KickTracks which are correlated to one       //
// in an event. The purpose is to compare the correlation function //
// of those ring/kt-patterns later and compare the simulation to   //
// experiment                                                      //
/////////////////////////////////////////////////////////////////////

using namespace std;
#include "hpidpdfbuilderexp.h"
#include <cstdlib>
#include <iostream>
#include <TMath.h>
#include <TString.h>
#include <TFile.h>
#include <TTree.h>
#include "hcategory.h"
#include "hpidtrackcand.h"
#include "hruntimedb.h"
#include "hiterator.h"
#include "hrichhit.h"
#include "hkicktrack.h"
#include "richdef.h"
#include "hpidfl.h"
#include "hrichanalysispar.h"
#include "hrichgeometrypar.h"
#include "hades.h"
#include "hevent.h"
#include "hmdcseg.h"
#include "hmdchit.h"
#include "hmdcclusinf.h"
#include "hmdcdef.h"
#include "hmdctrackddef.h"

ClassImp(HPidPDFBuilderExp)


HPidPDFBuilderExp::HPidPDFBuilderExp(Char_t* Extension,Char_t* OutputFile)
{
  N_Cand_Total = 0;

  evcount=0;
  
  OutFileName = new TString(OutputFile);
  OutFileName->Append(Extension);
  OutFileName->Append(".root");
  
  //We assemble the list of leaves we want to store in the tree
  LeafList = 
    new TString("dPhi/F:dTheta/F:pmq/F:padnr/F:centroid/F:locmax/F:avcharge/F:theta/F:phi/F:sector/F:neighbour/F:openingAngle/F:mdcchisquare/F:nsegments/F:nwiresmdc0/F:nwiresmdc1/F:clussizemdc0/F:clussizemdc1/F");
  
  CandidateProperties = new TTree("CandidateProperties","CandidateProperties");
  CandidateProperties->Branch("CandidateProperties",&PatternInstance,LeafList->Data());
  
  evcount =0;

}


Bool_t HPidPDFBuilderExp::clearPattern()
{
//The RICH observables
  PatternInstance[n_dPhi]           = -99.0;
  PatternInstance[n_dTheta]         = -99.0;
  PatternInstance[n_pmq]            = -99.0;
  PatternInstance[n_padnr]  	    = -99.0;
  PatternInstance[n_centroid]       = -99.0;
  PatternInstance[n_locmax] 	    = -99.0;
  PatternInstance[n_avcharge]       = -99.0; 
  PatternInstance[n_theta] 	    = -99.0;
  PatternInstance[n_phi] 	    = -99.0;
  PatternInstance[n_sector] 	    = -99.0;
  PatternInstance[n_openingAngle]   = -99.0;
  PatternInstance[n_mdcchisquare]   = -99.0;
  PatternInstance[n_nsegments]      = -99.0;
  PatternInstance[n_nwiresmdc0]     = -99.0;
  PatternInstance[n_nwiresmdc1]     = -99.0;
  PatternInstance[n_clussizemdc0]   = -99.0;
  PatternInstance[n_clussizemdc1]   = -99.0;
  
  return kTRUE;
}


HPidPDFBuilderExp::~HPidPDFBuilderExp(void) 
{
  delete LeafList;
  delete CandidateProperties;
  delete OutFileName;
}

Bool_t HPidPDFBuilderExp::init(void) 
{
  
  //Initialize the categories we need
  
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

  if((pAuxIterator = (HIterator *)pInputCategory->MakeIterator()) == NULL)
    {
      Error("init", "Cannot make an inner iterator for HPidTrackCand Category");
      return kFALSE;
    }

  //We need the analysis parameters of the rich analysis in order to determine the
  //critical window in which to search for a neighbour!
  HRuntimeDb* rtdb=gHades->getRuntimeDb();
  //pAnalysisPar = (HRichAnalysisPar*)rtdb->getContainer("RichAnalysisParameters");
  //pGeometryPar = (HRichGeometryPar*)rtdb->getContainer("RichGeometryParameters");
  return kTRUE;
  
}

Bool_t HPidPDFBuilderExp::checkForRing(void)
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

Bool_t HPidPDFBuilderExp::checkForKickTrack(void)
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

Int_t HPidPDFBuilderExp::checkForSegments(void)
{
  //check whether segments are associated to the Candidate
  if(theCandidate->getInnerMdcNumber() > 0)
    {
      return theCandidate->getInnerMdcNumber();
    }
  return 0;
}

/*
Float_t HPidPDFBuilderExp::getCorrelationType(Bool_t hasARing, Bool_t hasAKickTrack, Bool_t hasNSegments)
{
  //Determine what type of correlation the candidate shows
  if(hasARing && hasAKickTrack && hasNSegments) 
    return (Float_t)R_KT_S;
  
  if(hasARing && hasAKickTrack && !hasNSegments) 
    return (Float_t)R_KT;
  
  if(hasARing && !hasAKickTrack && hasNSegments) 
    return (Float_t)R_S;
  
  if(!hasARing && hasAKickTrack && hasNSegments) 
    return (Float_t)KT_S;
  
  if(!hasARing && hasAKickTrack && !hasNSegments) 
    return (Float_t)KT;

  return -1.0;
} 
*/

Float_t HPidPDFBuilderExp::getMdcPhi()
{
  Int_t iSector = theKickTrack->getSector();
  Float_t fPhi = TMath::RadToDeg() * theKickTrack->getPhi();

  switch(iSector)
    {
    case 0:
      break;
      
    case 1:
    case 2:
    case 3:
    case 4:
      fPhi += 60.0f * iSector;
      break;
      
    default:
      fPhi -= 60.0f;
      break;
    }
  
  return fPhi;
}


Int_t HPidPDFBuilderExp::execute(void)
{
  evcount++;

  //Variables to decide what type of Candidate we are examining
  Bool_t hasARing = kFALSE;
  Bool_t hasAKickTrack = kFALSE;
  Int_t  hasNSegments = 0;

  //Loop over all PidTrackCandidates;
  pIteratorInput->Reset();

  while((theCandidate = (HPidTrackCand *)pIteratorInput->Next()) != NULL)
    {
      /*
      cout << "Candidate Nr,: " <<cc <<endl;
      cout << "ringid: " << theCandidate->getRingId()<< endl;
      cout << "kickid: " << theCandidate->getKickTrackId()<< endl;
      cout << "mdcid0: " << theCandidate->getInnerMdcId(0)<< endl;
      cout << "mdcid1: " << theCandidate->getInnerMdcId(1)<< endl;
      cout << "metaid: " << theCandidate->getMetaId()<< endl;*/

      clearPattern();      
      hasARing = checkForRing();
      hasAKickTrack = checkForKickTrack();
      hasNSegments = checkForSegments();

      /*
      PatternInstance[n_corrtype] = getCorrelationType(hasARing, hasAKickTrack, hasNSegments);
      if(PatternInstance[n_corrtype] ==-1) {
	Error("PDF Builder execute", "Unknown correlation");
	exit (-1);
      }
      */
      
      if(hasARing && hasAKickTrack){
	bookRingObservables();
	bookKickObservables();
	if(hasNSegments){PatternInstance[n_nsegments]=hasNSegments;}  
	CandidateProperties->Fill();
	N_Cand_Total++;
      }
    }

  if(!(evcount%5000))
    {
      cout << "saving"  << endl;
      TFile* OutputFile = new TFile(OutFileName->Data(),"RECREATE");
      OutputFile->cd();
      
      CandidateProperties->Write();
      
      OutputFile->Close();
      
      TString* statfile = new TString("statisticsfile_");
      statfile->Append(OutFileName->Data());
      

      FILE* fp = fopen(statfile->Data(),"w+");
      fprintf(fp,"Statistics after %d events:\n",evcount);
      fprintf(fp,"Number of candidates: %d\n",N_Cand_Total);     
      fclose(fp);

      cout << "***************************" << endl;
      cout << "Statistics after " <<evcount << " events:" << endl;
      cout << "Number of candidates: " << N_Cand_Total << endl;
      cout << "***************************" << endl;
      
      delete OutputFile;
    } 
  
  return 0;
}


Bool_t HPidPDFBuilderExp::finalize(void) 
{
  // Finalize the reconstructor
  // if any normalization or similar business needs to be done - do it here!
  // (computation of ratios, extraction of relative efficiencies etc.)

  TFile* OutputFile = new TFile(OutFileName->Data(),"RECREATE");
  OutputFile->cd();

  CandidateProperties->Write();

  OutputFile->Close();
  
  cout << "***************************" << endl;
  cout << "Statistics:" << endl;
  cout << "Number of candidates" << N_Cand_Total << endl;
  cout << "***************************" << endl;


  return kTRUE;
}


Bool_t HPidPDFBuilderExp::bookKickObservables()
{
  //We are interested in the properties of the segment from which the kicktrack was made!
  PatternInstance[n_openingAngle]=getClosestCandidateAngularDistance();
  Int_t sector=theCandidate->getKickTrack()->getSector();
  Int_t MdcSegmentId = theCandidate->getKickTrack()->getSegment();
  HLocation segLoc;
  segLoc.set(3,sector,0,MdcSegmentId);           // 0 - inner segment

  Int_t Mdc0HitId = ((HMdcSeg*)(pMdcSegCategory->getObject(segLoc)))->getHitInd(0);

  if(Mdc0HitId >= 0) {  //!!!!!!!!!!!!!!!!!!
    Int_t Mdc0ClusId = ((HMdcHit*)(pMdcHitCategory->getObject(Mdc0HitId)))->getClusInfIndex();
    PatternInstance[n_nwiresmdc0]   = ((HMdcClusInf*)(pMdcClusInfCategory->getObject(Mdc0ClusId)))->getNDrTimes();
    PatternInstance[n_clussizemdc0] = ((HMdcClusInf*)(pMdcClusInfCategory->getObject(Mdc0ClusId)))->getClusSize();
  }
  
  
  Int_t Mdc1HitId = ((HMdcSeg*)(pMdcSegCategory->getObject(segLoc)))->getHitInd(1);

  if(Mdc1HitId >= 0) {  //!!!!!!!!!!!!!!!!!!
    Int_t Mdc1ClusId = ((HMdcHit*)(pMdcHitCategory->getObject(Mdc1HitId)))->getClusInfIndex();
    PatternInstance[n_nwiresmdc1]   = ((HMdcClusInf*)(pMdcClusInfCategory->getObject(Mdc1ClusId)))->getNDrTimes();
    PatternInstance[n_clussizemdc1] = ((HMdcClusInf*)(pMdcClusInfCategory->getObject(Mdc1ClusId)))->getClusSize();
  }

  PatternInstance[n_mdcchisquare] = ((HMdcSeg*)(pMdcSegCategory->getObject(MdcSegmentId)))->getChi2();
  return kTRUE;
}


Bool_t HPidPDFBuilderExp::bookRingObservables(void)
{
  //Calculate the Angular Difference between the RICH and the MDC hit in phi and theta
  PatternInstance[n_dTheta]=theCandidate->getRichHit()->getTheta()-
    TMath::RadToDeg() * theCandidate->getKickTrack()->getTheta();

  PatternInstance[n_dPhi]=(theCandidate->getRichHit()->getPhi() - getMdcPhi())
    * TMath::Sin(theCandidate->getKickTrack()->getTheta());
  
  PatternInstance[n_pmq]=(float)theRing->getRingPatMat();
  PatternInstance[n_padnr]=(float)theRing->getRingPadNr();
  PatternInstance[n_avcharge]=(float)theRing->getRingAmplitude()/theRing->getRingPadNr();
  PatternInstance[n_centroid]=(float)theRing->getCentroid();
  PatternInstance[n_locmax]=(float)theRing->getRingLocalMax4();
  PatternInstance[n_phi]=(float)theRing->getPhi();
  PatternInstance[n_theta]=(float)theRing->getTheta();
  PatternInstance[n_sector]=(float)theRing->getSector();
  
  return kTRUE;
}


Float_t HPidPDFBuilderExp::getClosestCandidateAngularDistance()
{
  //We need an inner loop over all track candidates

  HPidTrackCand* theAuxCandidate;
  Float_t openingAngle = 360.0;
  Float_t auxOpeningAngle;
  pAuxIterator->Reset();

  //Find the closest candidate (with ring) to the current candidate 
  while((theAuxCandidate = (HPidTrackCand *)pAuxIterator->Next()) != NULL)
    {
      //Check whether this candidate has a ring and make sure it's NOT the same as the current!
      if(theAuxCandidate->getRingId()>0 && theAuxCandidate->getRingId() != theCandidate->getRingId())
	{
	  
	  auxOpeningAngle = HPidFL::calcOpeningAngleT(theAuxCandidate->getRichHit()->getTheta(),
						      theAuxCandidate->getRichHit()->getPhi(),
						      theCandidate->getRichHit()->getTheta(),
						      theCandidate->getRichHit()->getPhi());
							      
	  if(auxOpeningAngle<openingAngle)
	    {openingAngle=auxOpeningAngle;}
	}
    
    }
  if(openingAngle==360.0) openingAngle = -99.0;
  return openingAngle;
}































