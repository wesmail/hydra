// @(#)$Id: hpidtrackfiller.cc,v 1.23 2005-08-07 09:25:28 christ Exp $
//*-- Author  : Marcin Jaskula 29/06/2002
//*-- Modified: Marcin Jaskula 09/10/2002
//              - HPidTrackFillerPar used
//*-- Modified: Marcin Jaskula 31/10/2002
//              - getMdc* moved from global to static function
//*-- Modified: Marcin Jaskula 10/11/2002
//              - input categories must be created before init() !!!
//*-- Modified: Marcin Jaskula and Jacek Otwinowski 05/02/2003
//              - checkTofinoMult() removed bacause of HPidTofinoTrackFilter
//*-- Modified: Marcin Jaskula 14/03/2003
//              - meta overlap added
//Restructured: Tassilo Christ 11.11.03 to comply with new pid/tracking interface
//              based on metamatch and basetrack classes by anar rustamov

////////////////////////////////////////////////////////////////////////////////
//
// HPidTrackFiller
//
// A simple track filler of HPidTrackCand category.
// The data from HKickTrack are used and matching with RICH is done.
// For the matched ring correlated HMdcSegs are found for the close pair
// rejection. For all not HMdcSegs not matched in any HKickTrack the searching
// for corresponding rings is done as well.
//
// The options set in the constructor may modify this behaviour: see
// setParameters() description.
//
////////////////////////////////////////////////////////////////////////////////

#include "hpidtrackfiller.h"

#include "hades.h"
#include "hevent.h"
#include "heventheader.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hdetector.h"
//#include "hspectrometer.h"

#include "hbasetrack.h"
#include "hrichhit.h"
#include "hmdcseg.h"
#include "hpidtrackcand.h"
#include "hpidtrackcandsim.h"
#include "hpidfl.h"

#include "kickdef.h"
#include "richdef.h"
#include "hmdcdef.h"
#include "tofdef.h"
#include "showertofinodef.h"
#include "hmdctrackgdef.h"
#include "hmetamatch.h"
#include "hruntimedb.h"
#include "hshowerhittoftrack.h"
#include <iostream>
#include "hpidtrackfillerpar.h"

#include "showertofinodef.h"
#include "hshowerhittof.h"
#include "htofhit.h"
#include "hmdctrkcand.h"
#include "hpidmdcfunc.h"

// -----------------------------------------------------------------------------

#define DEF_ARRAY_SIZE 10000
#define SAFE_DELETE(A) { if(A) { delete (A); A = NULL; }}

// -----------------------------------------------------------------------------

ClassImp(HPidTrackFiller)

// -----------------------------------------------------------------------------

HPidTrackFiller::HPidTrackFiller(Option_t par[])
    : HReconstructor("PidTrackFiller", "PID track candidate filler")
{
// Default constructor.
// For options see setMatchingConditions() description.
  setDefault();
  setMatchingConditions(par);
}

// -----------------------------------------------------------------------------

HPidTrackFiller::HPidTrackFiller(Text_t name[], Text_t title[],Option_t par[])
    : HReconstructor(name, title)
{
// Default constructor.
// For options see setMatchingConditions() description.
  setDefault();
  setMatchingConditions(par);
}

// -----------------------------------------------------------------------------

HPidTrackFiller::~HPidTrackFiller(void)
{
    SAFE_DELETE(pitCand);
    SAFE_DELETE(pitMatch);
}

// -----------------------------------------------------------------------------

void HPidTrackFiller::setMatchingConditions(Option_t par[])
{
// Set parameters by names. Options (may be separated by comma or blank chars):
// MAKESIMCATEGPRY - build HPidTrackCandSim instead of the REAL data category
// SKIPNOLEPTON - skip the event when there is no matchig RICH with KP
// SKIPTOFINOMULT - skip the track when tofino mult. > 1
// ANALYZELVL2ONLY - skip events without positive 2nd level decision
// NOCHI2SEG0 -  do not remove track with chi^2<0 from seg0
// NOCHI2SEG1 -  do not remove track with chi^2<0 from seg1

  TString s = par;
  
  s.ToUpper();
  
  bSkipNoLepton = (strstr(s.Data(), "SKIPNOLEPTON") != NULL);
  bSkipTofinoMult = (strstr(s.Data(), "SKIPTOFINOMULT") != NULL);
  bMakeSimCategory = (strstr(s.Data(), "MAKESIMCATEGORY") != NULL);
  bNoChi2Selection[0] = (strstr(s.Data(), "NOCHI2SEG0") != NULL);
  bNoChi2Selection[1] = (strstr(s.Data(), "NOCHI2SEG1") != NULL);
  bAnalyzeLVL2Only = (strstr(s.Data(), "ANALYZELVL2ONLY") != NULL);
}

// -----------------------------------------------------------------------------

Bool_t HPidTrackFiller::reinit(void)
{
    if(bInitOk == kFALSE)
        Warning("reinit", "init() didn't succeed");
    else
        bInitOk = kTRUE;

    return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HPidTrackFiller::init(void)
{
// get/build all used categories and iterators
  
  meanDevChange=0.0;
  nShifts=0;
  trkcandindices.Set(1000);
  metahitindices.Set(1000);
  systems.Set(1000);
  sectors.Set(1000);
  multiplicities.Set(1000);

  nTriples=0;

  HEvent *pEvent;

    bInitOk = kFALSE;

    if(gHades == NULL)
    {
        Error("init", "gHades == NULL");
        return kFALSE;
    }

    if((pEvent = gHades->getCurrentEvent()) == NULL)
      {
	Error("init", "gHades->getCurrentEvent() == NULL");
        return kFALSE;
      }
    
    // delete iterators (if exist)
    SAFE_DELETE(pitMatch);
    SAFE_DELETE(pitCand);

    if((pCatMatch = pEvent->getCategory(catMetaMatch)) == NULL)
      {
	Error("init", "No catMetaMatch");
	return kFALSE;
      }

    if((pCatTrkCand = pEvent->getCategory(catMdcTrkCand)) == NULL){
		Error("init", "No catMdcTrkCand");
		return kFALSE;
	}
    if((pCatMdcSeg = pEvent->getCategory(catMdcSeg)) == NULL){
		Error("init", "No catMdcSeg");
		return kFALSE;
      }
	if(bMakeSimCategory){ 
		if((pCatTof = pEvent->getCategory(catTofHit)) == NULL){
			Error("init", "No catTofHit");
			return kFALSE;
		}
		if((pCatShower = pEvent->getCategory(catShowerHitTofTrack)) == NULL){
            Error("init", "No catShowerHitTofTrack");
            return kFALSE;
        }
	}
	else{
		if((pCatTof = pEvent->getCategory(catTofHit)) == NULL){
            Error("init", "No catTofHit");
            return kFALSE;
        }
		if((pCatShower = pEvent->getCategory(catShowerHitTof)) == NULL){
            Error("init", "No catShowerHitTof");
            return kFALSE;
        }
	}	
	
    if((pitMatch = (HIterator *)pCatMatch->MakeIterator()) == NULL)
    {
        Error("init", "Cannot make catMetaMatch category iterator");
        return kFALSE;
    }

    // get output category
    if((pCatCand = buildOutputCategory()) == NULL)
      {
	Error("init", "Cannot build PidTrackCand Category");
	return kFALSE;
      }

    if((pitCand = (HIterator *)pCatCand->MakeIterator()) == NULL)
    {
        Error("init", "Cannot make an iterator for PidTrack Category");
        return kFALSE;
    }

    if((pTrackFillerParam = (HPidTrackFillerPar *)gHades->getRuntimeDb()
                        ->getContainer(HPIDTRACKFILLERPAR_NAME)) == NULL)
    {
        Error("init", "Cannot get parameters: %s", HPIDTRACKFILLERPAR_NAME);
        return kFALSE;
    }

    bInitOk = kTRUE;

    return kTRUE;
}

// -----------------------------------------------------------------------------

HCategory* HPidTrackFiller::buildOutputCategory(void)
{
  HCategory* pOutCat;
  if(bMakeSimCategory)
  {
    pOutCat = HPidTrackCandSim::buildPidTrackCandCategory();
  }
  else
    pOutCat = HPidTrackCand::buildPidTrackCandCategory();

  return pOutCat;
}

// -----------------------------------------------------------------------------

Bool_t HPidTrackFiller::isSkipEvent()
{
  Bool_t skip = kFALSE;
  HCategory* pCat  = HPidFL::getCategory(catKickTrack);
  if (pCat==NULL)
    {
      ::Error("HPidTrackFiller::isSkipEvent()","No KickTrack category found!");
    }
  HEventHeader* pEvtHeader = gHades->getCurrentEvent()->getHeader();
  if(!pEvtHeader->IsTBit(0))//No trigger bit 0 set;
    {
      skip = kTRUE;
    }
  if(!pEvtHeader->getTriggerDecision())//Negative 2nd level trigger decision
    {
      skip = kTRUE;
    }
  if(pCat->getEntries()<=0)//Not at least one KickTrack
    {
      skip = kTRUE;
    }

  return skip;
}

Int_t HPidTrackFiller::execute(void)
{
  //cout << endl;
  //cout <<"#####################################" << endl;

  if(!bMakeSimCategory && bAnalyzeLVL2Only)
    {
      if(isSkipEvent())
	{
	  return kSkipEvent;
	}
    }

  Bool_t  bIsLepton;
  Int_t Number=0;

  Bool_t hasCorrelatedRing[nMomAlgs]; //The decision whether a ring was matched to a particular track candidate is done
  //based on a correlation window between MDC and RICH. The width of this window is
  //momentum dependent! As we have different momentum methods the answer may be different
  //depending on what method you execute!
  
  for(Int_t i=0;i<nMomAlgs;i++)
    {
      hasCorrelatedRing[i]=kFALSE;
    }
  
  HMetaMatch* pMatch=NULL;
  HPidTrackCand *pCand=NULL;
  if(bInitOk == kFALSE)
    {
      Error("execute", "Class not initialized");
      return -1;
    }
  
  // clear the data from the old event
  pCatCand->Clear();
  //No leptons yet in this event - if this is still 0 at the end we skip the complete event!
  bIsLepton = kFALSE;
  
  //Loop over all HMetaMatch Objects
  pitMatch->Reset();
  
  trkcandindices.Reset();
  metahitindices.Reset();
  systems.Reset();
  sectors.Reset();
  multiplicities.Reset();
  
  nTriples=0;
  Int_t nSector=-1;
  while((pMatch = (HMetaMatch *)pitMatch->Next()) != NULL)
    {
      //cout <<"pMatch start-----------------------------------------"<<endl;
      for(Int_t i=0;i<nMomAlgs;i++)
	{
	  hasCorrelatedRing[i]=kFALSE;
	}
      nSector=pMatch->getSector();
      //check kickplane and spline.
      //The rest will be added later
      if(!pMatch->isKickAccepted() && !pMatch->isSplineAccepted() && !pMatch->isRungeKuttaAccepted()){
	//if(!pMatch->isSplineAccepted()){
	continue;
      }
      

      nTotalTrack++;
      if(checkChi2(pMatch)?nAccTrack++:nRejTrack++);
      
      if((!bNoChi2Selection[0] || !bNoChi2Selection[1]) && (!checkChi2(pMatch))){//this is default
	//Track have negative chi^2, skip it.
	continue;
      }
      
      
      //check for MetaMatch-specific conditions!
      if((bSkipTofinoMult) && ( ! checkTofinoMult(pMatch)))
	continue;
      
      //We have survived all checks - now we make a candidate from the match
      Number++;
      pCand = getNextSlot(pMatch);
      //cout << "Making candidate: " << Number << endl;
      //correct the ring position according using the vertex-z-coordinate to compensate the change in
      //the mapping between pad position and angular emission direction!
      Float_t shift=0.0;
      if( pCand->getHitData()->getFlagRICH())
	{
	  shift=correctRingShift(pCand);
	}
      
      assignRingCorrelations(pCand); //For each momentum value we check whether the ring lies in the momentum dependent window!
      
      if(pCand->getHitData()->hasRingCorrelation[ALG_SPLINE])
	{
	  meanDevChange+=shift;
	  //cout << "Spline track found, adding: "  << shift << endl;
	  nShifts++;
	}
      
     if(bIsLepton==kFALSE && bSkipNoLepton==kTRUE)  //Did we see not lepton yet and do we care about leptons?
       bIsLepton=checkForLepton(pMatch);
    }


  pitCand->Reset();
  HPidTrackCand* currentCandidate=NULL;
  Float_t tmpOpangle=-1.0;
  Int_t nc=0;
  Int_t nm=0;
  while((currentCandidate = (HPidTrackCand*)pitCand->Next()) != NULL)//loop over all pidtrackcandidates
    {
      //cout << "Candidate: " << nc << endl;
      if( !(currentCandidate->getHitData()->getFlagRICH()) )
	{
	  //cout << "Skipping canidate without ring" << endl;
	  nc++;
	  continue;
	}
      pitMatch->Reset();
      //compare all meta matches with negative chi2 in in the inner mdc with the current candidate
      nm=0;
      while( (pMatch = (HMetaMatch *)pitMatch->Next()) != NULL)
	{
	  //cout << "MetaMatch: " << nm << " at adddress: " << pMatch << endl;

	  if(pCatMatch->getIndex(pMatch) == pCand->getHitData()->iIndMatch)
	    {
	      //cout << "Skipping 0 " << endl;
	      nm++;
	      continue;
	    }
	  if( pMatch->getARichInd(0)<0 )
	    {
	      //cout << "Skipping 1 " << endl;
	      nm++;
	      continue;
	    }
	  HMdcTrkCand* pMdcTrackCand = HPidFL::getMdcTrkCand(pMatch->getTrkCandInd());
	  if( HPidMdcFunc::getMdcChiSquare(HPidFL::getMdcSeg(pMdcTrackCand->getSeg1Ind())) > -1.0)
	    {
	      //cout << "Skipping 2 " << endl;
	      nm++;
	      continue;
	    }

	  if( !(pMatch->isSplineAccepted()) )
	    {
	      //cout << "Skipping 3 " << endl;
	      nm++;
	      continue;
	    }

	  
	  tmpOpangle=getAngularDistance(pMatch, currentCandidate);
	  
	  if(tmpOpangle>0.0)
	    {
	      currentCandidate->getTrackData()->setIsCPCandidate(kTRUE);
	    }
	  currentCandidate->getTrackData()->fAngleWithClosePairCandidate=tmpOpangle;
	  nm++;
	}
      nc++;
    }  

  // skip the events with no lepton track
  if((bSkipNoLepton == kTRUE) && (bIsLepton == kFALSE))
    return kSkipEvent;
  
  return 0;
}

Float_t HPidTrackFiller::getAngularDistance(HMetaMatch* pMatch, HPidTrackCand* pCand)
{
  HMdcTrkCand* pMdcTrackCand = HPidFL::getMdcTrkCand(pMatch->getTrkCandInd());	 
  HMdcSeg* pMMSeg = HPidFL::getMdcSeg(pMdcTrackCand->getSeg1Ind());

  if(pCand->getHitData()->iIndInnerSeg==pMdcTrackCand->getSeg1Ind())
    {
      cout << "Alert - this cannot be" << endl;
    }

  Float_t fThetaMM = HPidFL::getMdcTheta( pMMSeg->getTheta());
  Float_t fThetaCND = pCand->getHitData()->fMdcTheta;

  Float_t fPhiMM = HPidFL::getMdcPhi( pMMSeg->getSec(),pMMSeg->getPhi());
  Float_t fPhiCND = pCand->getHitData()->fMdcPhi;

  Float_t fDeltaPhi = (fPhiMM-fPhiCND) * TMath::Sin(TMath::DegToRad()*fThetaMM);
  Float_t fDeltaTheta = (fThetaMM-fThetaCND);
  //cout << "dTheta; " << fDeltaTheta <<endl;
  //cout << "dPhi; " << fDeltaPhi <<endl;

  Float_t fAngularDifference = fDeltaPhi*fDeltaPhi + fDeltaTheta*fDeltaTheta;
  //cout << "square diff: " << fAngularDifference << endl;
  fAngularDifference = TMath::Sqrt(fAngularDifference);

  //cout << "returning " << fAngularDifference << endl;
  if(fAngularDifference<=9.0)
    {
      return fAngularDifference;
    }
  return -2.0;
}

Float_t HPidTrackFiller::correctRingShift(HPidTrackCand* pCand)
{
  //cout << endl;
  //cout <<" new candidate!" << endl;
  //cout << "pCand-ptr: " << pCand << endl;
  Float_t PolynomialCoeff[8];
  Float_t RingShift=0.0;
  //cout << "Appying ring shift" << endl;
  HPidHitData* pHitData = pCand->getHitData();
  
  //cout << "Absolute difference value before shift:" << TMath::Abs(pHitData->getDeltaThetaMDCRICH())<< endl;
  Float_t thetaRich=pHitData->fRichTheta;
  //cout << "Theta in RICH: " << thetaRich << endl;
  //cout << "ring index: " << pHitData->iIndRICH;

  Float_t thetaMdc=pHitData->fMdcTheta;
  //cout << "Theta in Mdc: " << thetaMdc << endl;
  //cout << "seg index: " << pHitData->iIndInnerSeg;

  Float_t diffbefore=TMath::Abs(pHitData->getDeltaThetaMDCRICH());
  //cout << "diffbefore: " << diffbefore << endl;
  //cout << "VertexZ: " << gHades->getCurrentEvent()->getHeader()->getVertexZ() << endl;
  getPolynomialCoefficients(PolynomialCoeff, pCand->getHitData()->nSector);
  for(Int_t i=0;i<8;i++)
    {
      //cout << "Coefficient " << i << " is " << PolynomialCoeff[i] << endl;
      RingShift+=(Float_t)(PolynomialCoeff[i]*TMath::Power(thetaRich,i));
    }
  //cout << "Correction: " << RingShift << " z-coord:"<< gHades->getCurrentEvent()->getHeader()->getVertexZ() <<endl;
  
  pHitData->fRichTheta+=RingShift;
  //cout << "Absolute difference value after shift:" << TMath::Abs(pHitData->getDeltaThetaMDCRICH())<< endl;
  Float_t diffafter=TMath::Abs(pHitData->getDeltaThetaMDCRICH());
  //cout << "diffafter: " << diffafter << endl;
  //if(diffafter<diffbefore)
    //cout <<"better" << endl;
    //else
    //cout <<"worse" << endl;

  return diffbefore-diffafter;
  
}

void HPidTrackFiller::getPolynomialCoefficients(Float_t* coefficients,Int_t sector)
{
  //Determine Vertex information
  Float_t VertexZCoord=0.0;
  //cout << "now getting polynomial correction"<< endl;
  VertexZCoord=gHades->getCurrentEvent()->getHeader()->getVertexZ();
  pTrackFillerParam->getPolynomialCoeff(sector,VertexZCoord, coefficients);
}

void HPidTrackFiller::assignRingCorrelations(HPidTrackCand* pCand)
{
  HPidHitData* pHitData = pCand->getHitData();
  HPidTrackData* pTrackData = pCand->getTrackData();
  if(!pHitData->getFlagRICH()) return; //No ring - no questions

  //Get deviation of MDC track and RICH ring;
  Float_t dThetaMDCRICH=pHitData->getDeltaThetaMDCRICH();
  Float_t dPhiMDCRICH=pHitData->getDeltaPhiMDCRICH();
  Int_t sector= pHitData->nSector;
  
  for(Int_t AlgCounter=0;AlgCounter<nMomAlgs;AlgCounter++) {
    pCand->getHitData()->hasRingCorrelation[AlgCounter]=kFALSE;
    if(pTrackData->fMomenta[AlgCounter]>0){
      if( dPhiMDCRICH > 1.0 * pTrackFillerParam->getDeltaPhiMin(sector, pTrackData->fMomenta[AlgCounter]) && 
	  dPhiMDCRICH < 1.0 * pTrackFillerParam->getDeltaPhiMax(sector, pTrackData->fMomenta[AlgCounter]) &&
	  dThetaMDCRICH >  1.0 * pTrackFillerParam->getDeltaThetaMin(sector, pTrackData->fMomenta[AlgCounter]) && 
	  dThetaMDCRICH < 1.0 * pTrackFillerParam->getDeltaThetaMax(sector, pTrackData->fMomenta[AlgCounter]) ){
	//track has a ring
	pCand->getHitData()->hasRingCorrelation[AlgCounter]=kTRUE;
      }
    }
  }
}

Bool_t HPidTrackFiller::checkForLepton(HMetaMatch* pMatch)
{
  if(pMatch->getARichInd(0)>-1) //Matching between TrkCand and its' descendants and RICH - this could be a lepton!
    return kTRUE;
  else
    return kFALSE;
}

  
Bool_t HPidTrackFiller::finalize(void)
{
  meanDevChange/=nShifts;
  cout <<"Mean change in deviation per ring:" << meanDevChange << endl;
// 
	print();
	printf("------------------------------------------------------\n");
	printf("HPidTrackFiller Chi^2 cut\n");
	printf("Total tracks: %d. Accepted: %d. Rejected: %d\n",
	nTotalTrack,nAccTrack,nRejTrack);
	printf("Ratio: %f\n",nAccTrack/(float)nTotalTrack);
	printf("------------------------------------------------------\n");
    return kTRUE;
}

// -----------------------------------------------------------------------------

void HPidTrackFiller::setDefault(void)
{
// set default values inside the construtors

  pCatMatch   = NULL;
  pCatCand    = NULL;
  pCatTrkCand = NULL;
  pCatTof     = NULL;
  pCatShower  = NULL;
  pCatMdcSeg  = NULL;
  lTrack.set(1, 0);
  
  pitMatch  = NULL;
  pitCand   = NULL;
  
  bSkipNoLepton    = kFALSE;
  bSkipTofinoMult  = kFALSE;
  bNoChi2Selection[0] = kFALSE;//Default is to accept only tracks with chi^2>0
  bNoChi2Selection[1] = kFALSE;//Default is to accept only tracks with chi^2>0
  
  bInitOk   = kFALSE;
  nAccTrack=nRejTrack=nTotalTrack=0;
  
  bAnalyzeLVL2Only=kFALSE;
}

// -----------------------------------------------------------------------------

HPidTrackCand* HPidTrackFiller::getNextSlot(void)
{
// Get next slot from the output category

HPidTrackCand    *pOut = NULL;
static HLocation  locDummy;

    if(pCatCand == NULL)
    {
        Error("getNextSlot", "Output category not set: use init/reinit");
        return NULL;
    }

    if((pOut = (HPidTrackCand *) pCatCand->getNewSlot(locDummy)) == NULL)
    {
        Error("getNextSlot", "No new slot");
        return NULL;
    }
    return new(pOut) HPidTrackCand;
}

// -----------------------------------------------------------------------------

HPidTrackCand* HPidTrackFiller::getNextSlot(HMetaMatch *pMatch)
{
// get next slot from the output category and fills it with data from HMetaMatch

  Int_t iMatchIndex = pCatMatch->getIndex(pMatch);

  HPidTrackCand    *pOut = NULL;
  static HLocation  locDummy;
  
  if(pCatCand == NULL)
    {
      Error("getNextSlot", "Output category not set: use init/reinit");
      return NULL;
    }
  
  if((pOut = (HPidTrackCand *) pCatCand->getNewSlot(locDummy)) == NULL)
    {
      Error("getNextSlot", "No new slot");
      return NULL;
    }
  if(bMakeSimCategory)
    {
      //cout << "Filling now!" << endl;
      if(pMatch->getSector()>5 || pMatch->getSector()<0)
	cout << "Nonsense sector in trackfiller!: " << pMatch->getSector() << endl;
      return new (pOut) HPidTrackCandSim(pMatch, iMatchIndex);
    }
  else
    {
      return new(pOut) HPidTrackCand(pMatch,iMatchIndex);
    }
}

// -----------------------------------------------------------------------------

Bool_t HPidTrackFiller::checkTofinoMult(HMetaMatch *pMatch)
{
// Check the condition for the tofino multiplicity > 1
// This function returns true only for mult==1

  HShowerHitTof *pShwTof = NULL;
  HShowerHitTofTrack *pShwTofTrack = NULL;

  if(pMatch->getSystem() != 0)
    return kTRUE;
  
  if(pMatch->getMetaHitInd() < 0)
    {
      ::Error("HPidTrackFiller::checkTofinoMult","Invalid MetaMatch index %d",pMatch->getMetaHitInd());
      return kFALSE;
    }
  
  //check for real data tofino category
  if(HPidFL::getCategory(catShowerHitTof) && !bMakeSimCategory)
    {
      pShwTof = HPidFL::getShowerHitTof(pMatch->getMetaHitInd());
      if(pShwTof == NULL)
	{
	  Error("checkTofinoMult", "Cannot get HShowerHitTof %d" ,
		pMatch->getMetaHitInd());
	  
	  return kFALSE;
	}
      if(pShwTof->getTofinoMult() <= 1)
	return kTRUE;
    }
  
  //check for sim data
  else if (HPidFL::getCategory(catShowerHitTofTrack) && bMakeSimCategory)
  {
    pShwTofTrack = HPidFL::getShowerHitTofTrack(pMatch->getMetaHitInd());
    if(pShwTofTrack == NULL)
      {
	Error("checkTofinoMult", "Cannot get HShowerHitTofTrack %d" ,
	      pMatch->getMetaHitInd());
	
	return kFALSE;
      }
    if(pShwTofTrack->getTofinoMult() <= 1)
      return kTRUE;
  }
  else
    ::Error("HPidTrackFiller::checkTofinoMult","No ShowerHitTofCategory present!");
  
  
  return kFALSE;
}

// -----------------------------------------------------------------------------
Bool_t HPidTrackFiller::checkChi2(HMetaMatch *pMatch){
	//check Chi2 from segments (if they exist)
	//accept only track with positive Chi2
    //use Options NOCHI2SEG0 && NOCHI2SEG1 for setting in which mdc segments
    //chi^2 has to be checked
    //
	HMdcTrkCand *pTrkCand = NULL;
	HMdcSeg		*pMdcSeg1 = NULL;
	HMdcSeg		*pMdcSeg2 = NULL;
	Float_t		fChi2In   = 0.;
	Float_t		fChi2Out  = 0.;
	Int_t		nSeg1Ind  = -1;
	Int_t		nSeg2Ind  = -1;
	Int_t nTrCand = pMatch->getTrkCandInd();

	if(nTrCand>-1){
		pTrkCand = (HMdcTrkCand *)pCatTrkCand->getObject(nTrCand);
		if( NULL!= pTrkCand ){
			nSeg1Ind = pTrkCand->getSeg1Ind();
			nSeg2Ind = pTrkCand->getSeg2Ind();

			pMdcSeg1 = (HMdcSeg*)pCatMdcSeg->getObject(nSeg1Ind);
			pMdcSeg2 = (HMdcSeg*)pCatMdcSeg->getObject(nSeg2Ind);
            //check first segment
			if( NULL != pMdcSeg1 && !bNoChi2Selection[0]){
				fChi2In = pMdcSeg1->getChi2();		
			}
            //check second segment
			if( NULL != pMdcSeg2 && !bNoChi2Selection[1]){
				fChi2Out = pMdcSeg2->getChi2();		
			}
		}
	}
	else{
		Error("checkChi2","TrkCandInd not found");
	}
	if( fChi2In<0 || fChi2Out <0 ){
		return kFALSE;
	}


return kTRUE;
}
void HPidTrackFiller::print(void) const
{
// Print info about the recontructor params

    printf("HPidTrackFiller:\n");
    printf("SKIPNOLEPTON   : %s\n", (bSkipNoLepton) ? "YES" : "NO");
    printf("SKIPTOFNIOMULT : %s\n", (bSkipTofinoMult) ? "YES" : "NO");
    printf("NOCHI2SEG0: %s\n", (bNoChi2Selection[0]) ? "YES" : "NO");
    printf("NOCHI2SEG1: %s\n", (bNoChi2Selection[1]) ? "YES" : "NO");
}
