#include "hiterator.h"
#include "hcategory.h"
#include "hpidefficiencycalculation.h"
#include "hpidtrackcand.h"
#include "hpidparticlesim.h"
#include "hpidgeanttrackset.h"
#include "hpairsim.h"
#include "hshowerhittof.h"
#include "htofhit.h"
#include "hpidcandidate.h"
#include  "piddef.h"
#include "hiterator.h"
#include "hcategory.h"
#include "TNtuple.h"
#include "TVector3.h"
#include "hgeantkine.h"
#include "hkicktrack.h"
#include <iostream>
#include "piddef.h"
#include "hpidfl.h"
#include "hpairgeantdata.h"
#include "hrichhitsim.h"
#include "hmdcsegsim.h"
#include "htofhitsim.h"
#include "hshowerhittoftrack.h"
#include "richdef.h"
#include "hmdcdef.h"
#include "piddef.h"
#include "tofdef.h"
#include "hpidtrackcandsim.h"
#include "hmdctrkcand.h"
#include "showertofinodef.h"
#include "hmdctrackgdef.h"
#include "hmetamatch.h"
using namespace std;
ClassImp(HPidEfficiencyCalculation)
    
// ---------------------------------------------------------------------------
HPidEfficiencyCalculation::HPidEfficiencyCalculation(Int_t SELECTED_MOMENTUM_ALG,
						     Int_t o_sourceID, 
						     Int_t o_targetmedium,
						     const char* cOutFileName)
  :HSUDummyRec(kFALSE){
  clearMembers();
  sOutFileName = cOutFileName;
  selectedMomAlg = SELECTED_MOMENTUM_ALG;
  sourceID=o_sourceID;
  targetmedium=o_targetmedium;
  //cout << "MADE AN INSTANCE" << endl;
}

// ---------------------------------------------------------------------------
HPidEfficiencyCalculation::~HPidEfficiencyCalculation(){
  //delete pointer to ntuple
  if(NULL!=effNtuple){
    printf("Ntuple not null!!!\n");
    
  }
    
}
// ---------------------------------------------------------------------------

void HPidEfficiencyCalculation::clearMembers(){
    pItGeant       = NULL;
    pItGeant1      = NULL;
    pItPart        = NULL;
    pItPairs       = NULL;
    pItMdcSeg      = NULL;
    pItRichHit     = NULL;
    pItTOFHit      = NULL;
    pItShowerHitTof= NULL;
    effNtuple        = NULL;

    resetNtuple();
}

// ---------------------------------------------------------------------------
Bool_t HPidEfficiencyCalculation::init(void){
    //Init categories and iterators
  
  pItMdcTrkCand = getIterator(catMdcTrkCand,kTRUE);   
  
  if( NULL==pItMdcTrkCand){
    return kFALSE;
  } 

  pItMetaMatch = getIterator(catMetaMatch,kTRUE);   
  
  if( NULL==pItMetaMatch){
    return kFALSE;
  } 

  pItPidTrackCand = getIterator(catPidTrackCand,kTRUE);   
  
  if( NULL==pItPidTrackCand){
    return kFALSE;
  } 
  
    pItGeant = getIterator(catGeantKine,kTRUE);      //get main iterator 
                                                      //and reports errors
    if( NULL==pItGeant){
        return kFALSE;
    } 


    pItGeant1 = getIterator(catGeantKine,kTRUE);      //get auxiliary iterator 
                                                      
    if( NULL==pItGeant1){
        return kFALSE;
    } 


    pItPart = getIterator(catPidPart,kTRUE);
                                            
    if( NULL==pItPart){
        return kFALSE;
    } 

    pItPairs = getIterator(catPairs,kTRUE);
                                            
    if( NULL==pItPairs){
        return kFALSE;
    }

    pItMdcSeg = getIterator(catMdcSeg,kTRUE);
                                            
    if( NULL==pItMdcSeg){
        return kFALSE;
    }

    pItRichHit = getIterator(catRichHit,kTRUE);
                                            
    if( NULL==pItRichHit){
        return kFALSE;
    }

    pItTOFHit = getIterator(catTofHit,kTRUE);
                                            
    if( NULL==pItTOFHit){
        return kFALSE;
    }

    pItShowerHitTof = getIterator(catShowerHitTofTrack,kTRUE);
                                            
    if( NULL==pItShowerHitTof){
        return kFALSE;
    }

    //set output file
    if(openOutFile(sOutFileName.Data()) == NULL){ 
        return kFALSE;
    }
    //Create NTuple
    char cBuffer[200000];
    snprintf(cBuffer,sizeof(cBuffer),"%s:%s:%s:%s:%s:%s:%s:%s:%s:%s:%s",
	     EFF_NTUPLE_VARS_1,EFF_NTUPLE_VARS_2,EFF_NTUPLE_VARS_3,
	     EFF_NTUPLE_VARS_4,EFF_NTUPLE_VARS_5,EFF_NTUPLE_VARS_6,
	     EFF_NTUPLE_VARS_7,EFF_NTUPLE_VARS_8,EFF_NTUPLE_VARS_9,
	     EFF_NTUPLE_VARS_10,EFF_NTUPLE_VARS_11);

    printf("Ntuple =%s\n",cBuffer);
    if((effNtuple=new TNtuple("E","Ntuple with efficiency data",cBuffer))==NULL){
        Error("init", "Cannot create NTuple");
        return kFALSE;        
    }
    //ALL was initialized OK
    setInitOk();
    //cout <<"#####################DONE INIT######################" << endl;
    return kTRUE;
}
// ---------------------------------------------------------------------------

Int_t HPidEfficiencyCalculation::checkGeantTrackSet(const HPidGeantTrackSet* pGTS)
{
  Int_t nCorrTracks=pGTS->getNCorrelatedTrackIds();
  for(Int_t i=0;i<nCorrTracks-1;i++)
    {
      if(pGTS->getCorrelationFlag(i)<pGTS->getCorrelationFlag(i+1))
	{
	  cout << "Warning: lower: " <<pGTS->getCorrelationFlag(i) << endl;
	  cout << "Warning: upper: " <<pGTS->getCorrelationFlag(i+1) << endl;
	  cout << "####################### " << endl;
	}
    }
  return 0;
}

Int_t HPidEfficiencyCalculation::execute(void) {
  //return 0;
  //Loop over all tracks in geant-kine. Pick electrons from meson decay
  pItGeant->Reset();

  //local pointers to objects relevant for the geant particle
  const HPidHitData *pHitData;        
  const HPidTrackData *pTrackData;    

  HPidParticleSim    *pPart;        
  HPidTrackCandSim   *pPidTrackCandSim;        
  HMetaMatch         *pMetaMatch;
  HPairSim      *pPairSim;
  HGeantKine *pKine;
  
  while((pKine=(HGeantKine*)pItGeant->Next()))
    {


      //We only care for leptons
      if(pKine->getID()!=2 && pKine->getID()!=3)
	{
	  continue;
	}
      resetNtuple();      

      //Is this a lepton from our source of simulated input data ?
      if(!isEmbeddedTrack(pKine))
	{
	  continue;
	}

      if(isEmbeddedTrack(pKine))
	{ 
	  eNtupleVars[E_isEmbeddedTrack]=kTRUE;
	}

      //Was this track in the HADES acceptance?
      if(!isInAcceptance(pKine))
	{
	  continue;
	}
      if(isInAcceptance(pKine))
	{
	  eNtupleVars[E_isAccepted]=kTRUE;
	}
      

      //Now we find out whether the track is part of the base sample to which we normalize our
      //efficiency later on! This has to be done in a mode-dependent fashion as we have different
      //types of input data


      //Is there ANY geant particle close to this GeantKine object ?
      if(isCloseToOtherParticle(pKine))
	{
	  eNtupleVars[E_isCloseToOtherParticle]=kTRUE;

	  //Is there a geant particle created by our source of simulated particles close to this GeantKine object ?
	  if(isCloseToEmbeddedParticle(pKine))
	    {
	      eNtupleVars[E_isCloseToEmbeddedParticle]=kTRUE;
	    }
	  
	}


      

      
      //Once we are here the particle we have determined whether the GeantKine objects has to be regarded part of the base-sample!
      //This does NOT yet fix the denominator of the base sample because some geant kine objects might be rejected on the pair
      //level due to pair cut operations

      
      //We now store all required geant information about this kine-object
      eNtupleVars[E_track_number]=pKine->getTrack();
      eNtupleVars[E_parent_track_number]=pKine->getParentTrack();
      eNtupleVars[E_geant_id] = pKine->getID();		  

      
      Int_t parent, medium, mechanism;
      pKine->getCreator(parent, medium, mechanism);
      eNtupleVars[E_medium] =  medium;
      eNtupleVars[E_mechanism] =  mechanism;

      eNtupleVars[E_geant_mom] = pKine->getTotalMomentum();
      pKine->getMomentum(eNtupleVars[E_geant_mom_x],eNtupleVars[E_geant_mom_y],eNtupleVars[E_geant_mom_z]);

      TVector3 geantMomentum(eNtupleVars[E_geant_mom_x],eNtupleVars[E_geant_mom_y],eNtupleVars[E_geant_mom_z]);

      eNtupleVars[E_geant_theta]   = geantMomentum.Theta();
      eNtupleVars[E_geant_phi] =     geantMomentum.Phi();
      //cout << "Now I need: " << pKine->getTrack() << endl;
      
      
      
	
      eNtupleVars[E_metaIndex]=findCorrespondingMETAHit(pKine->getTrack());
      eNtupleVars[E_ringIndex]=findCorrespondingRichRing(pKine->getTrack());
      
      
      //Let's check whether there was a pid-track candidate made with full reconstruction of this particle
      pItPidTrackCand->Reset();
      while((pPidTrackCandSim = (HPidTrackCandSim*)pItPidTrackCand->Next()))
	{
	  const HPidGeantTrackSet* pTrackSetCand = pPidTrackCandSim->getGeantTrackSet();
	  for(UInt_t c = 0; c<pTrackSetCand->getNCorrelatedTrackIds();c++)
	    {	      
	      if(pTrackSetCand->getGeantTrackID(c)==eNtupleVars[E_track_number])
 		{
		  if(pTrackSetCand->getCorrelationFlag(c)>=eNtupleVars[E_PidTrackCandIndex])
		    {
		      eNtupleVars[E_PidTrackCandIndex]=pTrackSetCand->getCorrelationFlag(c);
		      eNtupleVars[E_RKIndex]=pPidTrackCandSim->getTrackData()->getRKTrackInd();
		      eNtupleVars[E_KickIndex]=pPidTrackCandSim->getTrackData()->getKickTrackInd();
		      break;
		      
		    }
		}
	    }
	}
      

      eNtupleVars[E_innerSegmentIndex]=findCorrespondingMDCSegment(pKine->getTrack());
	
      pItMetaMatch->Reset();
      Int_t idx=-1;
      while((pMetaMatch = (HMetaMatch*)pItMetaMatch->Next()))
	{
	  idx++;
	  if(pMetaMatch->getSystem()!=eNtupleVars[E_system]) continue;

	  if(pMetaMatch->getMetaHitInd()!=eNtupleVars[E_metaIndex]) continue;
	  
	  
	  if(pMetaMatch->getNCandForRich()<=0) continue;
	  
	  if(pMetaMatch->getARichInd(0)!=eNtupleVars[E_ringIndex] && 
	     pMetaMatch->getARichInd(1)!=eNtupleVars[E_ringIndex] && 
	     pMetaMatch->getARichInd(2)!=eNtupleVars[E_ringIndex]) continue;

	  eNtupleVars[E_matchIndex]=idx;
	  break;
	}


      //Now we determine what is left of this geant-particle on the single-particle level (after PID)
      //Which is the biggest correlation this particle has made in the detectors?
      UInt_t maximumCorrelation=0;

      pItPart->Reset();
      //cout << "Here!"<< endl;
      while((pPart = (HPidParticleSim*)pItPart->Next()))
	{
	  //cout << "loop" << endl;
	  const HPidGeantTrackSet* pTrackSet = pPart->getGeantTrackSet();
	  
	  //Loop over all geant-particles that contributed to this pid-particle and check whether the
	  //current one is contained!
	  for(UInt_t c = 0; c<pTrackSet->getNCorrelatedTrackIds();c++)
	    {	      
	      //cout << "Track ID: " << pTrackSet->getGeantTrackID(c)<< endl;
	      if(pTrackSet->getGeantTrackID(c)==eNtupleVars[E_track_number])
 		{
		  eNtupleVars[E_isPartOfATrack]=kTRUE;
		  if(isSeenInSingleDetector(pTrackSet->getCorrelationFlag(c)))
		    {
		      //In this geant trackset the particle has been seen in only one detector! We do not want such stray correlations
		      //continues the for loop, goes to next pidparticle to search for current kine object there!
		      continue; 
		    }
		  if(pTrackSet->getCorrelationFlag(c)>=maximumCorrelation)
		    {

		      //This pidParticles's geant information contains the current geant particle
		      //and it is more prominent here than in any other pidparticle
		      eNtupleVars[E_geant_flag]=pTrackSet->getCorrelationFlag(c);
		      maximumCorrelation=pTrackSet->getCorrelationFlag(c);
		      if(eNtupleVars[E_geant_flag]>=76)
			{
			  eNtupleVars[E_isFullyReconstructed] = kTRUE;
			}

		      //We collect the information from the PidParticle which represents the geant object currently under investigation
		      pHitData = pPart->getHitData();
		      pTrackData = pPart->getTrackData();

		      eNtupleVars[E_charge]     = pPart->getCharge();
		      eNtupleVars[E_rec_mom]    = pPart->P();
		      eNtupleVars[E_beta]       = pPart->getBetaExp();
		      eNtupleVars[E_sector]     = pHitData->getSector();
		      eNtupleVars[E_theta]      = pPart->thetaDeg();
		      eNtupleVars[E_phi]        = pPart->phiDeg();
		      
		      eNtupleVars[E_rec_mom_kick]= pTrackData->getMomenta(ALG_KICK);
		      eNtupleVars[E_rec_mom_spline]= pTrackData->getMomenta(ALG_SPLINE);
		      eNtupleVars[E_rec_mom_rk]= pTrackData->getMomenta(ALG_RUNGEKUTTA);
		      
		      //eNtupleVars[E_geant_mom]  = pTrackSet->getTotalMomentum();
		      //eNtupleVars[E_geant_flag] = pTrackSet->getMostCommonCorrelation();
		      
		      eNtupleVars[E_inner_chi2] =  pHitData->fInnerMdcChiSquare;
		      eNtupleVars[E_outer_chi2] =  pHitData->fOuterMdcChiSquare;
		      
		      eNtupleVars[E_rich_corr_kick] = pHitData->hasRingCorrelation[ALG_KICK];
		      eNtupleVars[E_rich_corr_spline] = pHitData->hasRingCorrelation[ALG_SPLINE];
		      eNtupleVars[E_rich_corr_rk] = pHitData->hasRingCorrelation[ALG_RUNGEKUTTA];
		      eNtupleVars[E_rich_flag] = pHitData->getFlagRICH();
		      
		      eNtupleVars[E_rec_id]= pPart->getPid();
		      
		      if(pKine->getID()==pPart->getPid() &&
			 pHitData->hasRingCorrelation[selectedMomAlg])
			{
			  //Geant-particle was correctly identified by PID and has a ring 
			  //This is the requirement for PID to treat this as a lepton
			  eNtupleVars[E_isIdentified] = kTRUE;
			  //cout << "IsIdentifed!"  << endl;		      
			  //We have identified the particle - so there is no need to search in
			  //further pid particles!
			  break; 
		        
			}
		      
		      
		    }
		}
	    }

	}
      
      //What remains to be done is to check whether the geant object/pid-particle was rejected by a pair cut.
      //These pair cuts do two things: They reduce the base-sample (i.e. by excluding things that have close partner)
      //and they induce inefficiencies. We need to determine now whether a particle is part of the base sample (denominator)
      //and whether it is removed unduly ("inefficent") by a pair cut (numerator)
      
      if(isRemovedByActivePairCut(pKine))
	{
	  eNtupleVars[E_isRemovedByActivePairCut]=kTRUE;
	}
      

      //Now loop over all pairs and check whether this GeantKine object is contained in any of the surviving ones
      pItPairs->Reset();
      while((pPairSim = (HPairSim*)pItPairs->Next()) != NULL)
	{
	  Int_t correlationFlag = isPairedWithBackgroundLepton(pPairSim,pKine);

	  if(correlationFlag>0)
	    {
	      eNtupleVars[E_isLegOfPair]=kTRUE;
	      if(pPairSim->getIsCutFlag()==0)
		{
		  //This pair survived all pair cuts and it contains the currently interesting pKine object
		  if(correlationFlag>=76 && eNtupleVars[E_isLegOfSurvivingPair]==kFALSE)
		    {
		      eNtupleVars[E_isLegOfSurvivingPair]=kTRUE;
		    }
		  if(correlationFlag<76)
		    {
		      //This is not implemented as we can make more than one fake from the same geant particle
		      //This would require multiple ntuples to store the yields separately
		    }
		}
	    }
	 
	}

      effNtuple->Fill(eNtupleVars);
    
    
    //Trigger decision
    //quality of spline 
      
    //Loop over all PidParticles and check whether this guy was fully tracked and identified

    //Check whether a close-pair-cut will remove this guy from the sample

  //Store all leptons which are in geantkine and would survive a close pair cut 

  //Store the fraction which is tracked and identified.




  //Assumption: We use RK-tracking!

  // ideas:


  // The embedding should take into account the following caveats:
  // pick only those coming from the TARGET!!!!! (We have no chance - and no intention - to reconstruct conversion
  // leptons created in the mirror or MDC!!!
  // pick only those tracks that have no close partner (opangle >9 degree) as we want to get a true >single< track
  // efficiency.

  // go to geant kine container and find all >simulated< leptons (We are dealing with Jochen's embedded tracks)
  // check whether each found sim-lepton made it through the spectrometer.

  // If the track was seen in all detectors from inner MDC to META count it for normalization

  // record which detector failed to see this guy if any detector did so. Then this detector is "det_inefficient"
  // The spectrometer is "spec_inefficient" if the particle was NOT seen in RICH, inner MDC, outer MDC and META!
  // (All detectors are required to reconstruct a particle properly!

  // check whether this particular lepton has been IDENTIFIED correctly by PID (if not pid is "pid_inefficient")

  // How to account for tracks cut away by the application of the opening angle cut? "pair_inefficient"? 
  // Apply a 9 degree opening angle cut upon the lepton track and all it's neighbours - this should give us a true
  // single lepton sample


    }
  return 0;
}

// ---------------------------------------------------------------------------
Bool_t HPidEfficiencyCalculation::finalize(void) {
        //write ntuple to file
        return writeAndCloseOutFile();
}

void HPidEfficiencyCalculation::resetNtuple()   {
  for(Int_t n=0;n<MAX_VARS;n++)
    {
      eNtupleVars[n]=-99;
    }
  
  eNtupleVars[E_isPartOfATrack] = kFALSE;
  eNtupleVars[E_isFullyReconstructed] = kFALSE;
  eNtupleVars[E_isIdentified]=kFALSE;
  eNtupleVars[E_isAccepted]=kFALSE;
  eNtupleVars[E_isCloseToOtherParticle]=kFALSE;
  eNtupleVars[E_isEmbeddedTrack]=kFALSE;
  eNtupleVars[E_isCloseToEmbeddedParticle]=kFALSE;
  eNtupleVars[E_isRemovedByActivePairCut]=kFALSE;
  eNtupleVars[E_isLegOfPair]=kFALSE;
  eNtupleVars[E_isLegOfSurvivingPair]=kFALSE;
}

Bool_t HPidEfficiencyCalculation::isInAcceptance(HGeantKine* pKine)
{
  //cout << "checking acceptance: First MDC hit:" << pKine->getFirstMdcHit()<< endl;
  //cout << "checking acceptance: First TOF hit:" << pKine->getFirstTofHit()<< endl;
  if((pKine->getFirstMdcHit())>-1 && (pKine->getFirstTofHit())>-1)
	 return kTRUE;
     else return kFALSE;
}

//returns kTRUE if a segment exists in which this track was found
Int_t HPidEfficiencyCalculation::findCorrespondingMDCSegment(Int_t geantTrackNumber)
{
  pItMdcSeg->Reset();
  //cout << "iterating over segments:" << endl;
  //cout << "searching for: " << geantTrackNumber << endl;
  HMdcSegSim* pSeg=NULL;
  Int_t idx=0;
  while((pSeg=(HMdcSegSim*)pItMdcSeg->Next()))
    {
      for(Int_t i = 0; i < pSeg->getNTracks(); i++) //loop over all contributing tracks ...
	{  
	  //cout << pSeg->getTrack(i) << endl;
	  if(pSeg->getTrack(i) == geantTrackNumber)
	    {
	      //cout << "yep!" << endl;
	      return idx;
	    }
	}
      idx++;
    }
  return -99;
}

//returns kTRUE if a META hit exists in which this track was found
Int_t HPidEfficiencyCalculation::findCorrespondingMETAHit(Int_t geantTrackNumber)
{
  pItTOFHit->Reset();
  HTofHitSim* pTof=NULL;
  Int_t idx=0;
  while((pTof=(HTofHitSim*)pItTOFHit->Next()))
    {
      if(pTof->getNTrack1()== geantTrackNumber)
	{
	  eNtupleVars[E_system]=1;
	  return idx;
	}
      if(pTof->getNTrack2()== geantTrackNumber)
	{
	  eNtupleVars[E_system]=1;
	  return idx;
	}
      idx++;
    }
  
  idx=0;
  pItShowerHitTof->Reset();
  HShowerHitTofTrack* pShw=NULL;
  while((pShw=(HShowerHitTofTrack*)pItShowerHitTof->Next()))
    {
      if(pShw->getTrack()== geantTrackNumber)
	{
	  eNtupleVars[E_system]=0;
	  return idx;
	}
      idx++;
    }


  return -99;

}

//returns kTRUE if a RICH ring exists in which this track was found
Int_t HPidEfficiencyCalculation::findCorrespondingRichRing(Int_t geantTrackNumber)
{
  
  pItRichHit->Reset();
  HRichHitSim* pRich=NULL;
  Int_t idx=0;
  while((pRich=(HRichHitSim*)pItRichHit->Next()))
    {
      if(pRich->track1 == geantTrackNumber ||
	 pRich->track2 == geantTrackNumber ||
	 pRich->track3 == geantTrackNumber )
	{
	  return idx;
	}
      idx++;
    }
  return -99;
}

Bool_t HPidEfficiencyCalculation::isCloseToOtherParticle(HGeantKine* pKine,Bool_t b_UseSourceParticlesOnly)
{
  HGeantKine *pKine1=NULL;
  
  Int_t trackNumber,particleID,trackNumber1,particleID1;
  Int_t parentTrackNumber,medium,mechanism,parentTrackNumber1,medium1,mechanism1;
  
  pKine->getParticle(trackNumber,particleID);
  pKine->getCreator(parentTrackNumber,medium,mechanism);
  
  pItGeant1->Reset();
  while((pKine1 = (HGeantKine*) pItGeant1->Next()) != NULL){

    if(pKine1->getFirstMdcHit()==-1)
      {
	continue; //This is required because we would get crazy with all conversion stuff in shower!
      }
    pKine1->getParticle(trackNumber1,particleID1);
    pKine1->getCreator(parentTrackNumber1,medium1,mechanism1);

    if(trackNumber!=trackNumber1) { 
      //Close particles in the chambers are considered a reason NOT to use this lepton for efficiency calculations
      Double_t diff_Opang=calcOpeningAngleKine(pKine,pKine1);

      if (diff_Opang<9.5)
	{ //There was some other geant kine object close by

	  if(b_UseSourceParticlesOnly) //If we restrict ourselves to source particles then we need ...
	    {
	      if(isEmbeddedTrack(pKine1)) //...this condition also
		{
		  return kTRUE;
		}
	    }
	  else //If we are NOT restricted to source particles we return kTRUE for ANY close neighbour!
	    {
	      return kTRUE;
	    }
	}
    }
  }
  return kFALSE;
}

Bool_t HPidEfficiencyCalculation::isCloseToEmbeddedParticle(HGeantKine* pKine)
{
  return isCloseToOtherParticle(pKine,kTRUE); // Returns kTRUE ONLY if the close particle is from the simulation source
}


/*
Bool_t HPidEfficiencyCalculation::isEmbeddedLepton(HGeantKine* pKine)
{
  Int_t pid = pKine->getID();
  if(isEmbeddedTrack(pKine) && (pid==2 || pid==3))
    {
      return kTRUE;
    }
  return kFALSE;
}
*/


Bool_t HPidEfficiencyCalculation::isEmbeddedTrack(HGeantKine* pKine)
{

  Int_t parentTrackNumber,medium,mechanism;
  pKine->getCreator(parentTrackNumber,medium,mechanism);
  Int_t pid = pKine->getID();

  if(sourceID==URQMD_PURE)
    {
      //If we use URQMD simulations without any embedding we accept all tracks from the target
      //no matter what process they where created in. (After all we should see everything originating
      //close to the vertex!)
      if(medium==targetmedium && (pid==2 || pid==3))
	{
	  return kTRUE;
	}
      else
	{
	  return kFALSE;
	}
    }
  
  if(sourceID==URQMD_EMBEDDED)
    {
      //If we use URQMD simulations with embedded white lepton tracks we accept all leptons from the white
      //generator
      if(parentTrackNumber==0 && (pid==2 || pid==3))
	{
	  //cout << "HERE" << endl;
	  return kTRUE;
	}
      else
	{
	  return kFALSE;
	}
    }
  return kFALSE;
}


Bool_t HPidEfficiencyCalculation::isEmbeddedTrack(Int_t geantTrackNumber)
{
  HGeantKine* pKine=HPidFL::getGeantKineObjFromTrkNbFast(geantTrackNumber);
  return isEmbeddedTrack(pKine);
}


Bool_t HPidEfficiencyCalculation::isBackgroundTrack(Int_t geantTrackNumber)
{
  return !isEmbeddedTrack(geantTrackNumber);
}


Float_t HPidEfficiencyCalculation::calcOpeningAngleKine(HGeantKine *kine1,HGeantKine *kine2)
{
    //input 2 kine objects
    //output opening angle of trajectories
  if(kine1==kine2)
    {
      cout << "Calculating angle of particle with itself!" << endl;
    }

    TVector3 vec1;
    if (kine1)
      {
	Float_t xMom1,yMom1,zMom1;
	kine1->getMomentum(xMom1,yMom1,zMom1);
	vec1.SetX(xMom1);
	vec1.SetY(yMom1);
	vec1.SetZ(zMom1); 
      }
    else
      {
	::Error("HPidEfficiencyCalcultion::calcOpeningAngleKine","first pointer is 0");
      }

    TVector3 vec2;
    if (kine2)
      {
	Float_t xMom2,yMom2,zMom2;
	kine2->getMomentum(xMom2,yMom2,zMom2);
	vec2.SetX(xMom2);
	vec2.SetY(yMom2);
	vec2.SetZ(zMom2);
	
      }
    else
      {
	::Error("HPidEfficiencyCalcultion::calcOpeningAngleKine","second pointer is 0");
      }

    return TMath::RadToDeg()*vec1.Angle(vec2);
    
}


Bool_t HPidEfficiencyCalculation::isRemovedByActivePairCut(HGeantKine* pKine)
{
  if(!pKine)
    {
      ::Error("HPidEfficiencyCalculation::isRemovedByActivePairCut","kine-pointer==NULL");
    }
  Int_t trackNumber,particleID;
  pKine->getParticle(trackNumber,particleID);

  HPairSim * pPair=NULL;
  pItPairs->Reset();
  Int_t comDet=0;
  while(( pPair= (HPairSim  *)pItPairs->Next()) != NULL)
    {
      comDet = isPairedWithBackgroundLepton(pPair,pKine);
      if(comDet>=76 && isDileptonPair(pPair)){ //This is important to be consistent with Laura's/Witek's implementation
	//This particle was one of the tracks in this pair and it was fully tracked!
	if(pPair->getPid1()==3 || 
	   pPair->getPid1()==2 ||
	   pPair->getPid2()==3 ||
	   pPair->getPid2()==2)
	  {
	    //At least one of the tracks is identified as a lepton
	    if(pPair->getIsCutFlag()!=0){
	      //This pair did NOT survive the active cuts - for efficiency correction purposes these should be
	      //all direct ones
	      return kTRUE;
	    }
	  }
      }
    }
  return kFALSE;
}


Bool_t HPidEfficiencyCalculation::isDileptonPair(HPairSim *pair){
  
  if((pair->getPid1()==2 || pair->getPid1()==3) && (pair->getPid2()==2 || pair->getPid2()==3))
    {
      return kTRUE;
    }
  else 
    {
      return kFALSE;
    }

}

Int_t HPidEfficiencyCalculation::isPairedWithBackgroundLepton(HPairSim* pPair, HGeantKine* pKine)
{
  if(isDileptonPair(pPair))
    {
      return isPairedWithBackgroundTrack(pPair, pKine);
    }
  return kFALSE;
}

Int_t HPidEfficiencyCalculation::isPairedWithBackgroundTrack(HPairSim* pPair, HGeantKine* pKine)
{
  Int_t geantTrackNumber = pKine->getTrack();
  //returns correlation flag of the kine object in this pair provided that the pair object is a 
  //source-background combination of tracks

  HPairGeantData pg(pPair);

  
  if(pg.getTrackNumber1()==geantTrackNumber && isEmbeddedTrack(pKine)) 
    {
      // The first particle is from the (embedded) source
      if(isBackgroundTrack(pg.getTrackNumber2()))
	{
	  //The second particle is not from the (embedded) source
	  return pg.getCommonDetectors1();
	}
      else return 0;
    }
  
  else if(pg.getTrackNumber2()==geantTrackNumber && isEmbeddedTrack(pKine))
    { 
      // The second particle is from the (embedded) source
      if(isBackgroundTrack(pg.getTrackNumber1()))
	{
	  //The first particle is not from the (embedded) source
	  return pg.getCommonDetectors2();
	}
      else return 0;
	 }
  
  return 0;
}


Bool_t HPidEfficiencyCalculation::isSeenInSingleDetector(Int_t correlationFlag)
{
  if(correlationFlag==1 ||
     correlationFlag==2 ||
     correlationFlag==4 ||
     correlationFlag==8 ||
     correlationFlag==64 )
    {
      return kTRUE;
    }
  return kFALSE;
}
