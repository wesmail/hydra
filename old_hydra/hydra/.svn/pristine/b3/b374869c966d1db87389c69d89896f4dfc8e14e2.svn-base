// hpidPDFBuilderSim.cc 13.05.2003  by Tassilo Christ
// mailto tassilo.christ@ph.tum.de


/////////////////////////////////////////////////////////////////////
//                                                                 //
// This reconstructer builds ntuples for single, double and fake   //
// RICH rings which are correlated to a KickTrack in MDC           //
// In the ntuples all information about the rings is stored        //
// that may be usful for PID					   //
//                                                                 // 
// It can also be run in a mode allowing to build joined	   //
// PDFs for ring-KickTrack-properties (larger pattern vectors)	   //
// This is required if pattern vectors for close pair ID are	   //
// to be reconstructed.                                            //
// There is also a mode where also rings are taken into account,   //
// which are correlated only to segments but not to KTs.	   //
//								   //
// To run in the "joined mode" set maceCPID to kTRUE               //
// To run in the "segment also" mode set useMDCSegments to kTRUE   //
// 								   //
/////////////////////////////////////////////////////////////////////

#include "hpidpdfbuildersim.h"
#include <iostream>
#include <stdlib.h>
#include <TMath.h>
#include <TString.h>
#include <TFile.h>
#include <TTree.h>
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
#include "hgeantkine.h"
#include "hmdcsegsim.h"
#include "hmdcseg.h"
#include "hmdchit.h"
#include "hmdcclusinf.h"
#include "hmdcdef.h"
#include "hmdctrackddef.h"
#include "hpidcommontrack.h"
#include "TH1F.h"
#include "piddef.h"
ClassImp(HPidPDFBuilderSim)


HPidPDFBuilderSim::HPidPDFBuilderSim(Option_t options[], Char_t* OutputFile)
{
  //Initialize the mode in which we run;
  TString optionString = options;

  if(optionString.Contains("USE_SIMINFO",TString::kIgnoreCase))
    {
      USE_SIMINFO=kTRUE;
    }
  else {USE_SIMINFO=kFALSE;}

  if(optionString.Contains("USE_SEGMENTS",TString::kIgnoreCase))
    {
      USE_SEGMENTS=kTRUE;
    }
  else{USE_SEGMENTS=kFALSE;}

  
  //N_Cand_FakeRing_KT = 0;
  //N_Cand_GoodRing_KT = 0;
  //N_Cand_Ring_Seg = 0;
  for(Int_t i=0;i<nMomentumBins;i++)
    {
      N_Cand_Hadron_KT[i] = 0;
      N_Cand_RichIneffSingle[i] = 0;
      N_Cand_RichIneffDouble[i] = 0;
    }

  //N_Cand_SuppLeptons=0;
  N_Cand_MdcIneff=0;
  //N_TotCand = 0;

  evcount=0;
  
  OutFileName = new TString(OutputFile);
 
  //We assemble the list of leaves we want to store in the tree
  LeafList = 
    new TString("dPhi/F:dTheta/F:pmq/F:padnr/F:centroid/F:locmax/F:avcharge/F:theta/F:phi/F:sector/F:neighbour/F:openingAngle/F:dalitz/F:gamma/F:ringtype/F:corrtype/F:mdcchisquare/F:iscorrgeantlep/F:nsegments/F:kicktype/F:nwiresmdc0/F:nwiresmdc1/F:clussizemdc0/F:clussizemdc1/F:clusfinderlvl0/F:clusfinderlvl1/F:momentum/F");
  
  tPatterns = new TTree("PidPatterns","PidPatterns");
  tPatterns->Branch("Patterns",&PatternInstance,LeafList->Data());

  evcount =0;

}


Bool_t HPidPDFBuilderSim::clearPattern()
{
//The RICH observables
  PatternInstance[p_dPhi]           = -99.0;
  PatternInstance[p_dTheta]         = -99.0;
  PatternInstance[p_pmq]            = -99.0;
  PatternInstance[p_padnr]  	    = -99.0;
  PatternInstance[p_centroid]       = -99.0;
  PatternInstance[p_locmax] 	    = -99.0;
  PatternInstance[p_avcharge]       = -99.0; 
  PatternInstance[p_theta] 	    = -99.0;
  PatternInstance[p_phi] 	    = -99.0;
  PatternInstance[p_sector] 	    = -99.0;
  PatternInstance[p_neighbour]      = -99.0;
  PatternInstance[p_openingAngle]   = -99.0;
  PatternInstance[p_dalitz]         = -99.0;  
  PatternInstance[p_gamma]          = -99.0;
  PatternInstance[p_ringtype]       = -99.0;
  PatternInstance[p_corrtype]       = -99.0;
  PatternInstance[p_mdcchisquare]   = -99.0;
  PatternInstance[p_iscorrgeantlep] = -99.0;
  PatternInstance[p_nsegments]      = -99.0;
  PatternInstance[p_kicktype]       = -99.0;
  PatternInstance[p_nwiresmdc0]     = -99.0;
  PatternInstance[p_nwiresmdc1]     = -99.0;
  PatternInstance[p_clussizemdc0]   = -99.0;
  PatternInstance[p_clussizemdc1]   = -99.0;
  PatternInstance[p_clusfinderlvl0] = -99.0;
  PatternInstance[p_clusfinderlvl1] = -99.0;
  PatternInstance[p_momentum]       = -99.0;

  return kTRUE;
}


HPidPDFBuilderSim::~HPidPDFBuilderSim(void) 
{
  delete LeafList;
  delete LeafListDoubles;
  delete LeafListFakes;
  
  delete tPatterns;
  delete tDoublePatterns;
  delete tFakePatterns;
  
  delete OutFileName;
}

Bool_t HPidPDFBuilderSim::init(void) 
{
  
  //Initialize the categories we need
  

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

  //The rich hit category is needed only to include also those rings into
  //our investigation of neighbours which are not part of a track candidate
  pRichHitSimCategory = gHades->getCurrentEvent()->getCategory(catRichHit);
  if(!pRichHitSimCategory)
    {
      Error("init","Cannot get RichHitSimCategory");
      return kFALSE;
    }
  if((pIteratorRichHitSim = (HIterator *)pRichHitSimCategory->MakeIterator()) == NULL)
    {
      Error("init", "Cannot make an iterator for HRichHitSim Category");
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
  pAnalysisPar = (HRichAnalysisPar*)rtdb->getContainer("RichAnalysisParameters");
  pGeometryPar = (HRichGeometryPar*)rtdb->getContainer("RichGeometryParameters");
  return kTRUE;
  
}

Bool_t HPidPDFBuilderSim::checkForRing(void)
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

Bool_t HPidPDFBuilderSim::checkForKickTrack(void)
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

Int_t HPidPDFBuilderSim::checkForSegments(void)
{
  //check whether segments are associated to the Candidate
  if(theCandidate->getInnerMdcNumber() > 0)
    {
      return theCandidate->getInnerMdcNumber();
    }
  return 0;
}


Float_t HPidPDFBuilderSim::getCorrelationType(Bool_t hasARing, Bool_t hasAKickTrack, Bool_t hasNSegments)
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

Float_t HPidPDFBuilderSim::getMdcPhi()
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


Int_t HPidPDFBuilderSim::execute(void)
{
  evcount++;
  //cout << evcount<< endl;
  //Variables to decide what type of Candidate we are examining
  Bool_t hasARing = kFALSE;
  Bool_t hasAKickTrack = kFALSE;
  Int_t  hasNSegments = 0;
  //Float_t KickType = -99;
  Int_t leptonsInKickTrack=0;
  //Int_t cc=0;
  //Loop over all PidTrackCandidates;
  pIteratorInput->Reset();

  while((theCandidate = (HPidTrackCand *)pIteratorInput->Next()) != NULL)
    {
      clearPattern();      
      hasARing = checkForRing();
      hasAKickTrack = checkForKickTrack();
      hasNSegments = checkForSegments();

      PatternInstance[p_corrtype] = getCorrelationType(hasARing, hasAKickTrack, hasNSegments);
      if(PatternInstance[p_corrtype] ==-1) {
	Error("PDF Builder execute", "Unknown correlation");
	exit (-1);
      }
      
      if(hasARing && hasAKickTrack){
	bookRingObservables();
	bookKickObservables();
	if(hasNSegments && USE_SEGMENTS){PatternInstance[p_nsegments]=hasNSegments;}  
	tPatterns->Fill();
      }
      //This candidate was just a kicktrack - so we just count how many of those kicktracks are leptonic
      //or hadronic for the normalization of our histograms later
      if((!hasARing) && hasAKickTrack)
      {
	//There was a KickTrack but no ring - did we oversee a lepton in RICH that contributed to this KT??
	leptonsInKickTrack=HPidFL::countLeptonsInKickTrack((HKickTrackSim*) (theCandidate->getKickTrack()));
	Float_t fKickMomentum=theCandidate->getKickTrack()->getP();
	//if(fKickMomentum>1800) cout << fKickMomentum << endl;
	//Int_t wait;

	Int_t fMomentumBin = (Int_t)(fKickMomentum/200.0-0.5);	
	if(fMomentumBin>9) fMomentumBin=9;

        if(leptonsInKickTrack==0) 
	  {
	    N_Cand_Hadron_KT[fMomentumBin]++;
	    /*
	      if(fKickMomentum>1800) {
	      cout << "Pure Hadron!" << endl;
	      cout <<"******************" << endl;
	      }
	    */
	  }
        if(leptonsInKickTrack==1) 
	  {
	    //cout << "OOPS 1! " << endl;
	    //if the kicktrack was a fake or hadronic combination with leptonic admixture we do not count 
	    //the rich as inefficient
	    Float_t kicktype = getKickTrackType("noring");	    
	    if(kicktype==-2 || kicktype==8)
	      {
		//We have a fake combination with leptonic contribution
		//Thus we count it as a hadron/fake
		//cout << "hadron!" << endl;		
		N_Cand_Hadron_KT[fMomentumBin]++;
		//if(fKickMomentum>1800) {
		//cout << "Mixed Hadron!" << endl;
		//cout <<"******************" << endl;
		//}
	      }
	    else
	      { 
		N_Cand_RichIneffSingle[fMomentumBin]++;
		//if(fKickMomentum>1800) {
		//cout << "Ineff RICH single!" << endl;
		//cout <<"******************" << endl;
		//}
	      }
	  }
        if(leptonsInKickTrack==2)
	  {
	    //cout << "OOPS 2! " << endl;
	    //if the kicktrack was a fake or hadronic combination with leptonic admixture we do not count 
	    //the rich as inefficient
	    Float_t kicktype = getKickTrackType("noring");
	    if(kicktype==-2 || kicktype==8)
	      {
		//We have a fake combination with leptonic contribution
		//Thus we count it as a hadron/fake
		//cout << "hadron!" << endl;
		N_Cand_Hadron_KT[fMomentumBin]++;
		//if(fKickMomentum>1800) {
		//cout << "Mixed Hadron!" << endl;
		//cout <<"******************" << endl;
		//}
	      }
	    else
	      { 
		N_Cand_RichIneffDouble[fMomentumBin]++;
		//if(fKickMomentum>1800) {
		//cout << "Ineff RICH double!" << endl;
		//cout <<"******************" << endl;
		//}
		//cout << "Momentum Bin" << fMomentumBin << endl;
		//cin >> wait;
		//cout << "Candidate Momentum" << fKickMomentum << endl;
			
	      }
	  }
      }
    }

  //At the end we loop over all GOOD RINGS in ANY candidate and check whether there was at least ONE 
  //KT wich contains lepton information. In this way we get an idea of how many good rings are not
  //at all correlated to a leptonic MDC kicktrack
  Int_t ringcounter=-1;
  HRichHitSim* hitsimobject;
  pIteratorRichHitSim->Reset();
  while((hitsimobject = (HRichHitSim*) pIteratorRichHitSim->Next())!=NULL)
    {
      ringcounter++;
      if(getRingType(hitsimobject) && !findLeptonicTrackCandidate(ringcounter))
	{
	  //This ring was NOT correlated to ANY KT with leptonic content!
	  //cout << N_Cand_MdcIneff<<endl;
	  N_Cand_MdcIneff++;
	}
      
    }
  
  if(!(evcount%5000))
    {
      cout << "saving"  << endl;
      TFile* OutputFile = new TFile(OutFileName->Data(),"RECREATE");
      OutputFile->cd();
      
      tPatterns->Write();
      
      MomDistHadRing = new TH1F("MomDistHadRing","MomDistHadRing",nMomentumBins,0,2000);
      MomDistLepRing = new TH1F("MomDistLepRing","MomDistLepRing",nMomentumBins,0,2000);
      MomDistHadNoRing = new TH1F("MomDistHadNoRing","MomDistHadNoRing",nMomentumBins,0,2000);
      MomDistLepNoRing = new TH1F("MomDistLepNoRing","MomDistLepNoRing",nMomentumBins,0,2000);
      MomDistHadTotal = new TH1F("MomDistHadTotal","MomDistHadTotal",nMomentumBins,0,2000);
      MomDistLepTotal = new TH1F("MomDistLepTotal","MomDistLepTotal",nMomentumBins,0,2000);
      MomDistHadRatio = new TH1F("MomDistHadRatio","MomDistHadRatio",nMomentumBins,0,2000);
      MomDistLepRatio = new TH1F("MomDistLepRatio","MomDistLepRatio",nMomentumBins,0,2000);    
      

      cout << "Writing now!"<< endl;
      tPatterns->Draw("momentum>>MomDistHadRing","kicktype==-2 || kicktype == 0 || kicktype == 8");
      tPatterns->Draw("momentum>>MomDistLepRing", "(kicktype>=1 && kicktype<8)||kicktype==-1");
      
      for(Int_t i=0;i<nMomentumBins;i++)
	{
	  MomDistHadNoRing->SetBinContent(i+1,N_Cand_Hadron_KT[i]);
	  MomDistLepNoRing->SetBinContent(i+1,N_Cand_RichIneffSingle[i]+N_Cand_RichIneffDouble[i]);
	}
      
      MomDistHadTotal->Add(MomDistHadNoRing,MomDistHadRing,1.0,1.0);
      MomDistLepTotal->Add(MomDistLepNoRing,MomDistLepRing,1.0,1.0);
      
      MomDistLepRatio->Divide(MomDistLepRing,MomDistLepTotal);
      MomDistHadRatio->Divide(MomDistHadRing,MomDistHadTotal);
      
      MomDistHadRing->Write();
      MomDistLepRing->Write();
      MomDistHadNoRing->Write();
      MomDistLepNoRing->Write();
      MomDistHadTotal->Write();
      MomDistLepTotal->Write();
      MomDistHadRatio->Write();
      MomDistLepRatio->Write();

      delete MomDistHadRing;   
      delete MomDistLepRing; 
      delete MomDistHadNoRing;
      delete MomDistLepNoRing;
      delete MomDistHadTotal;
      delete MomDistLepTotal;
      delete MomDistHadRatio;  
      delete MomDistLepRatio;
      
      OutputFile->Close();
      
      TString* statfile = new TString("statisticsfile_");
      statfile->Append(OutFileName->Data());
      

      FILE* fp = fopen(statfile->Data(),"w+");
      fprintf(fp,"Statistics after %d events:\n",evcount);
      Int_t totalHadrons=0;
      Int_t totalMissedSingles=0;
      Int_t totalMissedDoubles=0;
      for(Int_t i=0;i<nMomentumBins;i++)
	{
	  fprintf(fp,"Single leptons missed (%d):%d\n",i,N_Cand_RichIneffSingle[i]);
	  fprintf(fp,"Double leptons missed (%d):%d\n",i,N_Cand_RichIneffDouble[i]);
	  fprintf(fp,"Hadronic candidates without ring (%d):%d\n",i,N_Cand_Hadron_KT[i]);
	  totalHadrons+=N_Cand_Hadron_KT[i];
	  totalMissedSingles+=N_Cand_RichIneffSingle[i];
	  totalMissedDoubles+=N_Cand_RichIneffDouble[i];
	}
      fprintf(fp,"Single leptons missed total:%d\n",totalMissedSingles);
      fprintf(fp,"Double leptons missed total:%d\n",totalMissedDoubles);
      fprintf(fp,"Hadronic candidates without ring total:%d\n",totalHadrons);
      fprintf(fp,"Number of leptons missed in MDC: %d\n",N_Cand_MdcIneff);
      
      fclose(fp);

      cout << "***************************" << endl;
      cout << "Statistics after " <<evcount << " events:" << endl;
      for(Int_t i=0;i<nMomentumBins;i++)
	{
	  cout << "Single leptons missed " << i << ":" << N_Cand_RichIneffSingle[i] << endl;
	  cout << "Double leptons missed " << i << ":" << N_Cand_RichIneffDouble[i] << endl;
	  cout << "Hadronic Candidiates without ring "  << i << ":" << N_Cand_Hadron_KT[i] << endl;
	}
      cout << "Single leptons missed total:" << totalMissedSingles << endl;
      cout << "Double leptons missed total:" << totalMissedDoubles << endl;
      cout << "Hadronic Candidiates without ring total:"  << totalHadrons << endl;
      cout << "Number of leptons missed in the MDC" << N_Cand_MdcIneff << endl;
      cout << "***************************" << endl;
      
      delete OutputFile;
    } 
  
  return 0;
}


Bool_t HPidPDFBuilderSim::finalize(void) 
{
  // Finalize the reconstructor
  // if any normalization or similar business needs to be done - do it here!
  // (computation of ratios, extraction of relative efficiencies etc.)

  TFile* OutputFile = new TFile(OutFileName->Data(),"RECREATE");
  OutputFile->cd();

  tPatterns->Write();

  //Append Normalization histograms to the oputput file:
  
  MomDistHadRing = new TH1F("MomDistHadRing","MomDistHadRing",nMomentumBins,0,2000);
  MomDistLepRing = new TH1F("MomDistLepRing","MomDistLepRing",nMomentumBins,0,2000);
  MomDistHadNoRing = new TH1F("MomDistHadNoRing","MomDistHadNoRing",nMomentumBins,0,2000);
  MomDistLepNoRing = new TH1F("MomDistLepNoRing","MomDistLepNoRing",nMomentumBins,0,2000);
  MomDistHadTotal = new TH1F("MomDistHadTotal","MomDistHadTotal",nMomentumBins,0,2000);
  MomDistLepTotal = new TH1F("MomDistLepTotal","MomDistLepTotal",nMomentumBins,0,2000);
  MomDistHadRatio = new TH1F("MomDistHadRatio","MomDistHadRatio",nMomentumBins,0,2000);
  MomDistLepRatio = new TH1F("MomDistLepRatio","MomDistLepRatio",nMomentumBins,0,2000);    

  tPatterns->Draw("momentum>>MomDistHadRing","kicktype==-2 || kicktype == 0 || kicktype == 8");
  tPatterns->Draw("momentum>>MomDistLepRing", "(kicktype>=1 && kicktype<8)||kicktype==-1");
    

  for(Int_t i=0;i<nMomentumBins;i++)
    {
      MomDistHadNoRing->SetBinContent(i+1,N_Cand_Hadron_KT[i]);
      MomDistLepNoRing->SetBinContent(i+1,N_Cand_RichIneffSingle[i]+N_Cand_RichIneffDouble[i]);
    }

  MomDistHadTotal->Add(MomDistHadNoRing,MomDistHadRing,1.0,1.0);
  MomDistLepTotal->Add(MomDistLepNoRing,MomDistLepRing,1.0,1.0);

  MomDistLepRatio->Divide(MomDistLepRing,MomDistLepTotal);
  MomDistHadRatio->Divide(MomDistHadRing,MomDistHadTotal);

  MomDistHadRing->Write();
  MomDistLepRing->Write();
  MomDistHadNoRing->Write();
  MomDistLepNoRing->Write();
  MomDistHadTotal->Write();
  MomDistLepTotal->Write();
  MomDistHadRatio->Write();
  MomDistLepRatio->Write();
  
  delete MomDistHadRing;   
  delete MomDistLepRing; 
  delete MomDistHadNoRing;
  delete MomDistLepNoRing;
  delete MomDistHadTotal;
  delete MomDistLepTotal;
  delete MomDistHadRatio;  
  delete MomDistLepRatio;
  
  OutputFile->Close();

  cout << "Analysis finshed!"<< endl;
  cout << "***************************" << endl;
  cout << "Statistics after " <<evcount << " events:" << endl;
  Int_t totalHadrons=0;
  Int_t totalMissedSingles=0;
  Int_t totalMissedDoubles=0;

  for(Int_t i=0;i<nMomentumBins;i++)
    {
      cout << "Single leptons missed " << i << ":" << N_Cand_RichIneffSingle[i] << endl;
      cout << "Double leptons missed " << i << ":" << N_Cand_RichIneffDouble[i] << endl;
      cout << "Hadronic Candidiates without ring "  << i << ":" << N_Cand_Hadron_KT[i] << endl;
      totalHadrons+=N_Cand_Hadron_KT[i];
      totalMissedSingles+=N_Cand_RichIneffSingle[i];
      totalMissedDoubles+=N_Cand_RichIneffDouble[i];
    }
  cout << "Single leptons missed total:" << totalMissedSingles << endl;
  cout << "Double leptons missed total:" << totalMissedDoubles << endl;
  cout << "Hadronic Candidiates without ring total:"  << totalHadrons << endl;
  cout << "Number of leptons missed in the MDC" << N_Cand_MdcIneff << endl;
  cout << "***************************" << endl;
  

  TString* statfile = new TString("statisticsfile_");
  statfile->Append(OutFileName->Data());
  
      
  FILE* fp = fopen(statfile->Data(),"w+");
  fprintf(fp,"Statistics after %d events:\n",evcount);

  for(Int_t i=0;i<nMomentumBins;i++)
    {
      fprintf(fp,"Single leptons missed (%d):%d\n",i,N_Cand_RichIneffSingle[i]);
      fprintf(fp,"Double leptons missed (%d):%d\n",i,N_Cand_RichIneffDouble[i]);
      fprintf(fp,"Hadronic candidates without ring (%d):%d\n",i,N_Cand_Hadron_KT[i]);
    }
  fprintf(fp,"Single leptons missed total:%d\n",totalMissedSingles);
  fprintf(fp,"Double leptons missed total:%d\n",totalMissedDoubles);
  fprintf(fp,"Hadronic candidates without ring total:%d\n",totalHadrons);
  fprintf(fp,"Number of leptons missed in MDC: %d\n",N_Cand_MdcIneff);
  
  fclose(fp);
  
  return kTRUE;
}


Float_t HPidPDFBuilderSim::getKickTrackType(Char_t* mode)
{
 if(!strcmp(mode,"withring"))
  {
    //Int_t wait;
    //Count how many leptons match in all detectors;
    Int_t nCommonLeptonsFull;
    
    //Count how many leptons match in Rich and MDC
    Int_t nCommonLeptonsRichMdc;
    
    //Count how many leptons match in Rich and MDC
    Int_t nCommonLeptonsMdcMeta;
    
    //Count how many Leptons were in the KickTrack
    Int_t nLeptonsInKickTrack=HPidFL::countLeptonsInKickTrack((HKickTrackSim*) theCandidate->getKickTrack());
    
    Int_t nMatchesFull, nMatchesMdcMeta;
  

    HPidFL::getGeantMatches(theCandidate, nCommonLeptonsRichMdc,nMatchesMdcMeta,nMatchesFull,nCommonLeptonsFull, nCommonLeptonsMdcMeta);
    
    if(!nMatchesMdcMeta)
      {
	//The kickplane algorithm has produced a fake correlation between hits in MDC and Meta!
	return -2.0;
      }
    //cout << "theCandidate pointer inside: " << theCandidate << endl;
    
    if(nCommonLeptonsRichMdc>nLeptonsInKickTrack) 
      { 
	cout << "strange case in event: " << evcount << endl;
	cout << "Common Leptons in Rich and Mdc: " << nCommonLeptonsRichMdc << endl;
	cout << "Common Leptons in Mdc and Meta: " << nCommonLeptonsMdcMeta << endl;
	cout << "Leptons in ring inside: " << PatternInstance[p_ringtype] <<endl;
	//cin >> wait;
      }
    if(nLeptonsInKickTrack==0)
      {
	//A purely hadronic kicktrack was correlated to a ring
	return 0.0;
      }
    
    else{
      if(!nCommonLeptonsMdcMeta)
	{
	  //We have common ids in mdc and meta - but not leptonic ones
	  //Here we intercept hadrons with with additional leptonic ids in their kicktrack
	  return 8.0;
	  
	}
      
      
      if(nCommonLeptonsRichMdc==nLeptonsInKickTrack) 
	{
	  //OK this was a lepton, return the number of leptons common in Rich and Mdc
	  return nCommonLeptonsRichMdc;
	}
      
      if(nCommonLeptonsRichMdc<nLeptonsInKickTrack){ 
	//One lepton in ring, different one in leptonic kicktrack => common leptons==0;
	if(PatternInstance[p_ringtype]==1 && nLeptonsInKickTrack==1) return 6;
	
	//One lepton in ring, two different ones in leptonic kicktrack => common leptons==0;
	if(nCommonLeptonsRichMdc==0 && PatternInstance[p_ringtype]==1 && nLeptonsInKickTrack>=2) return 3;
	
	//No lepton in ring, one lepton in kicktrack => common lepton==0;
	if(PatternInstance[p_ringtype]==0 && nLeptonsInKickTrack==1) return 4;
	
	//No lepton in ring, two or more leptons in kicktrack => common lepton==0;
	if(PatternInstance[p_ringtype]==0 && nLeptonsInKickTrack>=2) return 5;
	
	//One lepton in ring, same in kicktrack and one or more other leptons in kicktrack => common leptons == 1;
	if(nCommonLeptonsRichMdc==1 && PatternInstance[p_ringtype]==1 && nLeptonsInKickTrack>=2) return 7;
	
	//In all other cases return -1 (e.g. two leptons in ring, two in kicktrack, but no matches!)
	return -1;
	
      }
    }
    return -99.0;
  }

 if(!strcmp(mode,"noring"))
   {
     //cout << "checking without ring!" << endl;
     Int_t nCommonLeptonsMdcMeta=0;
    
     //Count how many Leptons were in the KickTrack
     Int_t nLeptonsInKickTrack=HPidFL::countLeptonsInKickTrack((HKickTrackSim*) theCandidate->getKickTrack());
     
     Int_t nMatchesFull=0;
     Int_t nMatchesMdcMeta=0;
     
     HPidFL::getGeantMatches(theCandidate,nMatchesMdcMeta,nMatchesFull, nCommonLeptonsMdcMeta);

     if(!nMatchesMdcMeta)
      {
	//The kickplane algorithm has produced a fake correlation between hits in MDC and Meta!
	//cout << "-2!!" << endl;
	return -2.0;
      }
     if(!nCommonLeptonsMdcMeta)
       {
	 //We have common ids in mdc and meta - but not leptonic ones
	 //Here we intercept hadrons with with additional leptonic ids in their kicktrack
	 //cout << "8!!" << endl;
	 return 8.0;
       }
     return nLeptonsInKickTrack;
   }
 return -1000;
}


Bool_t HPidPDFBuilderSim::bookKickObservables()
{
  //We are interested in the properties of the segment from which the kicktrack was made!
  //and whether this KT had geant matches in MDC & META!
  //is it a hadron, single or a double KT, or is it a fake produced by kickplane correlations;
  PatternInstance[p_kicktype] = getKickTrackType("withring");
  PatternInstance[p_momentum] = theCandidate->getKickTrack()->getP();
  Int_t sector=theCandidate->getKickTrack()->getSector();
  Int_t MdcSegmentId = theCandidate->getKickTrack()->getSegment();
  HLocation segLoc;
  segLoc.set(3,sector,0,MdcSegmentId);           // 0 - inner segment

  Int_t Mdc0HitId = ((HMdcSeg*)(pMdcSegCategory->getObject(segLoc)))->getHitInd(0);

  if(Mdc0HitId >= 0) {  //!!!!!!!!!!!!!!!!!!
    Int_t Mdc0ClusId = ((HMdcHit*)(pMdcHitCategory->getObject(Mdc0HitId)))->getClusInfIndex();
    PatternInstance[p_nwiresmdc0]   = ((HMdcClusInf*)(pMdcClusInfCategory->getObject(Mdc0ClusId)))->getNDrTimes();
    PatternInstance[p_clussizemdc0] = ((HMdcClusInf*)(pMdcClusInfCategory->getObject(Mdc0ClusId)))->getClusSize();
    PatternInstance[p_clusfinderlvl0] = ((HMdcClusInf*)(pMdcClusInfCategory->getObject(Mdc0ClusId)))->getLevelClFinding();
  }
  
  
  Int_t Mdc1HitId = ((HMdcSeg*)(pMdcSegCategory->getObject(segLoc)))->getHitInd(1);

  if(Mdc1HitId >= 0) {  //!!!!!!!!!!!!!!!!!!
    Int_t Mdc1ClusId = ((HMdcHit*)(pMdcHitCategory->getObject(Mdc1HitId)))->getClusInfIndex();
    PatternInstance[p_nwiresmdc1]   = ((HMdcClusInf*)(pMdcClusInfCategory->getObject(Mdc1ClusId)))->getNDrTimes();
    PatternInstance[p_clussizemdc1] = ((HMdcClusInf*)(pMdcClusInfCategory->getObject(Mdc1ClusId)))->getClusSize();
    PatternInstance[p_clusfinderlvl1] = ((HMdcClusInf*)(pMdcClusInfCategory->getObject(Mdc1ClusId)))->getLevelClFinding();
  }

  PatternInstance[p_mdcchisquare] = ((HMdcSeg*)(pMdcSegCategory->getObject(MdcSegmentId)))->getChi2();
  return kTRUE;
}




Float_t HPidPDFBuilderSim::getRingType(HRichHitSim* theSimRing)
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


Bool_t HPidPDFBuilderSim::bookRingObservables(void)
{
  //Array of contributing Geant Particles
  Int_t GeantTrackNumber;


  //Calculate the Angular Difference between the RICH and the MDC hit in phi and theta
  PatternInstance[p_dTheta]=theCandidate->getRichHit()->getTheta()-
    TMath::RadToDeg() * theCandidate->getKickTrack()->getTheta();

  PatternInstance[p_dPhi]=(theCandidate->getRichHit()->getPhi() - getMdcPhi())
    * TMath::Sin(theCandidate->getKickTrack()->getTheta());
  
  PatternInstance[p_pmq]=(float)theRing->getRingPatMat();
  PatternInstance[p_padnr]=(float)theRing->getRingPadNr();
  PatternInstance[p_avcharge]=(float)theRing->getRingAmplitude()/theRing->getRingPadNr();
  PatternInstance[p_centroid]=(float)theRing->getCentroid();
  PatternInstance[p_locmax]=(float)theRing->getRingLocalMax4();
  PatternInstance[p_phi]=(float)theRing->getPhi();
  PatternInstance[p_theta]=(float)theRing->getTheta();
  PatternInstance[p_sector]=(float)theRing->getSector();
  
  //Here we compute the less obvious properties of the ring 
  //which are available for experimental data
  
  //Check what type of ring is our neighbour
  PatternInstance[p_neighbour]= (float)findNeighbourType();
  if(PatternInstance[p_neighbour]>0){
    //cout << "neighbourtype:" << PatternInstance[p_neighbour] << endl;
  }

  //Here we retrieve (if desired) the simulation info)
  if(USE_SIMINFO){
    theSimRing = (HRichHitSim*)theRing;
    //We check whether this was a ring whose creator particles are seen
    //also in other detectorscontributing to this candidte!
    PatternInstance[p_iscorrgeantlep]=theCandidate->getGeantTrackSet()->getCommonTrack();

    //Loop over all contrib tracks in the ring
    PatternInstance[p_dalitz]=0.0;
    PatternInstance[p_gamma]=0.0;
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
		PatternInstance[p_dalitz]+=HPidFL::calcRichWeight(theSimRing,i); 
	      }
	    
	    //Check whether this track is a conversion one
	    if(HPidFL::isLepFromPi0GammaConv(GeantTrackNumber))
	      {
		//And add its contribution to the ring to the dalitz fraction
		PatternInstance[p_gamma]+=HPidFL::calcRichWeight(theSimRing,i); 
	      }
	    
	  }
	
      }
    PatternInstance[p_ringtype] = getRingType(theSimRing);

    if(PatternInstance[p_ringtype]==1.0)
      {
	//For a single ring we compute the (angular) next neighbour distance of a candidate
	//The candidate has to be MORE than just a ring!
	PatternInstance[p_openingAngle]=getClosestCandidateAngularDistance();
      }
  }    

  
  return kTRUE;
}

Bool_t HPidPDFBuilderSim::getGeantMatches(HPidTrackCand* giveCandidate,Int_t& nMatchesMdcMeta, Int_t& nMatchesFull,
					 Int_t& nLeptonicMatchesMdcMeta)
{
  Char_t ListOfTracks[10];
  Short_t IdsOfTracks[10];
  for (Int_t i =0;i< 10;i++){
    ListOfTracks[i]=-99;
    IdsOfTracks[i]=-99;
  }
  nMatchesMdcMeta=0;
  nMatchesFull=0;
  nLeptonicMatchesMdcMeta=0;

  HGeantKine* theKine;

  //This function checks, how many matches have been found in the common track set 
  giveCandidate->getGeantTrackSet()->fillCommonTracks(ListOfTracks,IdsOfTracks,10);
  Int_t i=0;
  while(ListOfTracks[i]>=0)
    {
      if(ListOfTracks[i]==HPidCommonTrack::kTrackFull){
	nMatchesFull++;
	nMatchesMdcMeta++;
      }
      if(ListOfTracks[i]==HPidCommonTrack::kTrackKickPlane){
	nMatchesMdcMeta++;
      }

      pIteratorGeantKine->Reset();
      while((theKine=(HGeantKine *)pIteratorGeantKine->Next())!=0)
	{
	  Int_t aTrack, aID;
	  theKine->getParticle(aTrack,aID);
	  if (aTrack==IdsOfTracks[i] && (aID==2 ||aID==3))
	    {
	      if(ListOfTracks[i]==HPidCommonTrack::kTrackKickPlane)
		{
		  nLeptonicMatchesMdcMeta++;
		}
	    }
	} // end while kine loop
      
      i++;
    }
  return kTRUE;
}


Bool_t HPidPDFBuilderSim::getGeantMatches(HPidTrackCand* giveCandidate, Int_t& nMatchesRichMdc,
					 Int_t& nMatchesMdcMeta, Int_t& nMatchesFull,
					 Int_t& nLeptonicMatchesFull, Int_t& nLeptonicMatchesMdcMeta)
{
  Char_t ListOfTracks[10];
  Short_t IdsOfTracks[10];
  for (Int_t i =0;i< 10;i++){
    ListOfTracks[i]=-99;
    IdsOfTracks[i]=-99;
  }
  nMatchesMdcMeta=0;
  nMatchesRichMdc=0;
  nMatchesFull=0;
  nLeptonicMatchesFull=0;
  nLeptonicMatchesMdcMeta=0;

  HGeantKine* theKine;

  //This function checks, how many matches have been found in the common track set 
  giveCandidate->getGeantTrackSet()->fillCommonTracks(ListOfTracks,IdsOfTracks,10);
  Int_t i=0;
  while(ListOfTracks[i]>=0)
    {
      if(ListOfTracks[i]==HPidCommonTrack::kTrackFull){
	nMatchesFull++;
	nMatchesRichMdc++;
	nMatchesMdcMeta++;
      }
      if(ListOfTracks[i]==HPidCommonTrack::kTracklet){
	nMatchesRichMdc++;
      }
      if(ListOfTracks[i]==HPidCommonTrack::kTrackKickPlane){
	nMatchesMdcMeta++;
      }

      pIteratorGeantKine->Reset();
      while((theKine=(HGeantKine *)pIteratorGeantKine->Next())!=0)
	{
	  Int_t aTrack, aID;
	  theKine->getParticle(aTrack,aID);
	  if (aTrack==IdsOfTracks[i] && (aID==2 ||aID==3))
	    {
	      if(ListOfTracks[i]==HPidCommonTrack::kTrackFull)
		{
		  nLeptonicMatchesFull++;
		  nLeptonicMatchesMdcMeta++;
		}
	      if(ListOfTracks[i]==HPidCommonTrack::kTrackKickPlane)
		{
		  nLeptonicMatchesMdcMeta++;
		}
	      
	    }
	} // end while kine loop
      
      i++;
    }
  return kTRUE;
}



Float_t HPidPDFBuilderSim::getClosestCandidateAngularDistance()
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


NEIGHBOUR HPidPDFBuilderSim::findNeighbourType()
{
  //This function investigates whether a ring is found in the vicinity of the currently investigated
  //candidate and then determines whether it was a single, double, fake or uncorrelated ring (one
  //which is NOT present in the candidate category)!

  Int_t nMirrTracks1, nMirrTracks2;

  HRichHitSim* theClosestNeighbour = getClosestNeighbourRing();
  if(!theClosestNeighbour) 
    {
      //No neighbouring ring was found
      return NO_RING_NEIGHBOUR;
    }
  else 
    {
      if (HPidFL::isDoubleLepOnMirror(theClosestNeighbour, nMirrTracks1, nMirrTracks2))
      {
	return DOUBLE_RING_NEIGHBOUR;
      }
    else //It was not a double - was it a single ?
      {
	if (HPidFL::isLepOnMirror(theClosestNeighbour, nMirrTracks1))
	  {
	    return SINGLE_RING_NEIGHBOUR;
	  }
	else return FAKE_RING_NEIGHBOUR; //It was obviously a fake
      }
    }
}

HRichHitSim* HPidPDFBuilderSim::getClosestNeighbourRing()
{
  //Loop over all rings in the event, regardless whether they were correlated or not
  pIteratorRichHitSim->Reset();

  HRichHitSim* pRichHitSim;
  HRichHitSim* theClosestNeighbour=0;

  Int_t RingNumber=-1;
  Float_t RingCenterDistance=100.0;
  Float_t AuxRingCenterDistance;

  while((pRichHitSim = (HRichHitSim *)pIteratorRichHitSim->Next()) != NULL)
    {
      RingNumber++;
      //Check whether it's NOT the same as the current!
      if(RingNumber != theCandidate->getRingId())
	{
	  AuxRingCenterDistance=TMath::Sqrt(TMath::Power(pRichHitSim->getPadX()-theRing->getPadX(),2.0)+
					    TMath::Power(pRichHitSim->getPadY()-theRing->getPadY(),2.0));
	  if(AuxRingCenterDistance<RingCenterDistance //We are closer than the last one
	     && AuxRingCenterDistance<=(float)(2+2*pAnalysisPar->iRingRadius)) // and we are within the critial area
	    {
	      RingCenterDistance = AuxRingCenterDistance;
	      theClosestNeighbour = pRichHitSim;
	    }
	}
    }
  return theClosestNeighbour;
}


Bool_t HPidPDFBuilderSim::findLeptonicTrackCandidate(Int_t ringcounter)
{
  //Count common leptons in all hits
  Int_t nCommonLeptonsFull;
  
  //Count how many leptons match in Rich and MDC
  Int_t nCommonLeptonsRichMdc;
  
  //Count how many leptons match in Rich and MDC
  Int_t nCommonLeptonsMdcMeta;
  
  //Count commen particles in all hits/mdc meta hits
  Int_t nMatchesFull, nMatchesMdcMeta;

  HPidTrackCand* currentCandidate;
  pIteratorInput->Reset();  
  while((currentCandidate = (HPidTrackCand*) pIteratorInput->Next())!=NULL)
    {
      if(currentCandidate->getRingId() == ringcounter && currentCandidate->getKickTrackId()>=0)
	{
	  HPidFL::getGeantMatches(currentCandidate, nCommonLeptonsRichMdc,nMatchesMdcMeta,nMatchesFull,
				  nCommonLeptonsFull, nCommonLeptonsMdcMeta);

	  //The ring was used to build this candidate
	  if(nCommonLeptonsRichMdc>0)
	    {
	      //This candidate is made of a ring and a kicktrack and it contains matching Geant Info
	      return kTRUE;
	    }
	}
    }
  return kFALSE;
}






























