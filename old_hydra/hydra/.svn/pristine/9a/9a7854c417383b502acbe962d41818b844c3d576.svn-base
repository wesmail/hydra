// @(#)$Id: hpidtrackfiller.cc,v 1.38 2007-08-06 14:06:15 kempter Exp $
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

//_HADES_CLASS_DESCRIPTION
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

#include "piddef.h"
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
#include "TVector3.h"
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

//Parse the option string for the condition to apply to a HMetaMatch before creating
//a HPidTrackCand from 
void HPidTrackFiller::setMatchingConditions(Option_t par[])
{
// Set parameters by names. Options (may be separated by comma or blank chars):
// MAKESIMCATEGPRY - build HPidTrackCandSim instead of the REAL data category
// SKIPNOLEPTON - skip the event when there is no matchig RICH with track
// SKIPTOFINOMULT - skip the track when tofino mult. > 1
// NOCHI2SEG0 -  do not remove track with chi^2<0 from seg0
// NOCHI2SEG1 -  do not remove track with chi^2<0 from seg1

  TString s = par;
  s.ToUpper();

  bSkipNoLepton = (strstr(s.Data(), "SKIPNOLEPTON") != NULL);
  bSkipTofinoMult = (strstr(s.Data(), "SKIPTOFINOMULT") != NULL);
  bMakeSimCategory = (strstr(s.Data(), "MAKESIMCATEGORY") != NULL);
  bNoChi2Selection[0] = (strstr(s.Data(), "NOCHI2SEG0") != NULL);
  bNoChi2Selection[1] = (strstr(s.Data(), "NOCHI2SEG1") != NULL);
  //bUsePlutoGenerator = (strstr(s.Data(), "USEPLUTOGENERATOR") != NULL);
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
  if(bMakeSimCategory) //get shower sim category
    { 
      if((pCatTof = pEvent->getCategory(catTofHit)) == NULL){
	Error("init", "No catTofHit");
	return kFALSE;
      }
      if((pCatShower = pEvent->getCategory(catShowerHitTofTrack)) == NULL){
	Error("init", "No catShowerHitTofTrack");
	return kFALSE;
      }
    }
  else //get shower real category
    {
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

Int_t HPidTrackFiller::execute(void)
{
  //cout << "execute" << endl;
  //No leptons yet in this event - if this is still 0 at the end we skip the complete event!
  Bool_t  bIsLeptonEvent = kFALSE;
  
  //local variable;
  Float_t shift=0.0;

  //The decision whether a ring was matched to a particular track candidate is done
  //based on a correlation window between MDC and RICH. The width of this window is
  //momentum dependent! As we have different momentum methods the answer may be different
  //depending on what method you execute!
  Bool_t hasCorrelatedRing[MAX_MOM_ALGS]; 

  for(Int_t i=0;i<MAX_MOM_ALGS;i++)
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

  //Loop over all HMetaMatch Objects
  pitMatch->Reset();

  //loop over all input objects
  while((pMatch = (HMetaMatch *)pitMatch->Next()) != NULL)
    {
      //cout << "match instance" << endl;
      for(Int_t i=0;i<MAX_MOM_ALGS;i++)
	{
	  hasCorrelatedRing[i]=kFALSE;
	}
      //cout << "hello 1" << endl;
      
      //At least one tracking method must have had success otherwise we skip this guy!
      
      if(!pMatch->isKickAccepted() && !pMatch->isSplineAccepted() && !pMatch->isRungeKuttaAccepted()){
	continue;
      }
      //cout << "hello 2" << endl;
      
      nTotalTrack++;
      
      Bool_t ok=checkChi2(pMatch);

      (ok)?nAccTrack++:nRejTrack++;
      
      if((!bNoChi2Selection[0] || !bNoChi2Selection[1]) && (!checkChi2(pMatch))){//this is default
	//Track has negative chi^2, skip it.
	continue;
      }
      //cout << "hello 3" << endl;
      
      //check for Shower Multiplicity
      if((bSkipTofinoMult) && ( ! checkTofinoMult(pMatch)))
	continue;
      
      //cout << "hello 4" << endl;
      //We have survived all checks - now we make a candidate from the match
      pCand = getNextSlot(pMatch);
      //cout << "done" << endl;

      //Correct the ring position according using the vertex-z-coordinate of the track to compensate the 
      //change in the mapping between pad position and angular emission direction!
      //This track has a ring and this ring should be shifted
      if( pCand->getHitData()->getFlagRICH()) 
	{
	  shift = correctRingShift(pCand);
	  nShifts++;
	  meanDevChange+=shift;
	}

      //For each momentum value we check whether the ring lies in the momentum dependent window!
      //The answer can be different for different methods of momentum computation
      assignRingCorrelations(pCand); 
      assignMetaCorrelations(pCand); 
      
      //Did we see a lepton already in this event and do we care about leptons?
      if(bIsLeptonEvent==kFALSE && bSkipNoLepton==kTRUE) 
	bIsLeptonEvent=checkForLepton(pMatch);
    }


  //Check for the closest MetaMatch that has a/has no ring associated to a segment and store it 
  //as closest lepton/hadron candidate
  pitCand->Reset();
  HPidTrackCand* currentCandidate=NULL;

  
  Float_t tmpOpangle=180.0;

  //loop over all pidtrackcandidates
  //cout << "Now assigning neighbours" << endl;
  while((currentCandidate = (HPidTrackCand*)pitCand->Next()) != NULL)
    {
      
      tmpOpangle = 180.0;

      if( !(currentCandidate->getHitData()->getFlagRICH()) ) 
	{
	  //This candidate is not a lepton candidate - we do not care what it was surrounded by
	  //One could perform this check based upon the correlation flags belonging to the individual
	  //tracking methods. However one would need three flags then - so we stick to one flag only
	  //and accept that we perform this check for some tracks which are in reality not considered
	  //a lepton candidate any more.
	  continue;
	}

      pitMatch->Reset();

      //compare all meta matches with segment in the inner mdc with the current candidate
      while( (pMatch = (HMetaMatch *)pitMatch->Next()) != NULL)
	{
	  if(pCatMatch->getIndex(pMatch) == currentCandidate->getHitData()->iIndMatch)
	    {
	      //The current candidate was made from the same HMetaMatch object that we 
	      //are currently investigating! This means we cannot compare them to one another!
	      //(The opening angle is by definition 0.0 degrees!)
	      continue;
	    }

	  //find track candidate for MetaMatch 
	  HMdcTrkCand* pMMTrkCand = HPidFL::getMdcTrkCand(pMatch->getTrkCandInd());
	  if(pMMTrkCand->getSeg1Ind()<0)
	    {
	      Error("HPidTrackFiller::execute","There was a trk-cand object withou valid inner segment index!");
	      exit(-1);
	    }

	  if(currentCandidate->getHitData()->iIndInnerSeg==pMMTrkCand->getSeg1Ind())
	    {
	      //The current PidTrackCandidate and the Meta Match where made from the same segment but
	      //different MetaMatch objects!! Those guys also have opening angle 0 by definition and
	      //will be rejected by the no-same-hit condition! We do not analyze them therefore.
	      continue;
	    }
	  
	  //Get segment from MetaMatch
	  HMdcSeg* pMMSeg = HPidFL::getMdcSeg(pMMTrkCand->getSeg1Ind());
	  
	  tmpOpangle=getAngularDistance(pMatch, currentCandidate);

	  //We don't care for egments further away than 25 degree
	  if(tmpOpangle>25)
	    {
	      continue;
	    }

	  //Append this segment to the list of close neighbours
	  if(pMatch->getARichInd(0)<0)
	    { 
	      tmpOpangle+=1000;
	    }

	  //If the particle segment failed to fit - we assign a negative angle to it
	  if(pMMSeg->getChi2()<0)
	    {
	      tmpOpangle*=-1;
	    }
	  currentCandidate->getTrackData()->appendCloseTracklet(tmpOpangle,
	    HPidMdcFunc::getMdcClusInfoIndexBySegment(pMMSeg,0),
	    HPidMdcFunc::getMdcClusInfoIndexBySegment(pMMSeg,1)); 
	}
      //currentCandidate->getTrackData()->sortTracklets();
    }
  
  // skip the events with no lepton track.
  if((bSkipNoLepton == kTRUE) && (bIsLeptonEvent == kFALSE))
    return kSkipEvent;
  
  return 0;
}

Float_t HPidTrackFiller::getAngularDistance(HMetaMatch* pMatch, HPidTrackCand* pCand)
{
  //Temporary vector objects for scalar product computation
  TVector3 vecCandidate;
  TVector3 vecMetaMatch;

  //Get segment from MetaMatch
  HMdcTrkCand* pMdcTrackCand = HPidFL::getMdcTrkCand(pMatch->getTrkCandInd());	 
  HMdcSeg* pMMSeg = HPidFL::getMdcSeg(pMdcTrackCand->getSeg1Ind());

  if(pCand->getHitData()->iIndInnerSeg==pMdcTrackCand->getSeg1Ind())
    {
      Error("HPidTrackFiller::getAngularDistance()",
	    "The current candidate and the MetaMatch we comare it to have the same segment index. This should be intercepted!");
      exit(-1);
    }

  //Conversion to degrees
  Float_t fThetaMM = HPidFL::getMdcTheta( pMMSeg->getTheta());
  Float_t fThetaCND = pCand->getHitData()->fMdcTheta;

  //Conversion to degrees and sector shift
  Float_t fPhiMM = HPidFL::getMdcPhi( pMMSeg->getSec(),pMMSeg->getPhi());
  Float_t fPhiCND = pCand->getHitData()->fMdcPhi;

  //Above all angles are in degree! We need them in radians!
  vecCandidate.SetMagThetaPhi(1.0,fThetaCND*TMath::DegToRad(),fPhiCND*TMath::DegToRad());
  vecMetaMatch.SetMagThetaPhi(1.0,fThetaMM*TMath::DegToRad() ,fPhiMM*TMath::DegToRad());
  
  //The return value is in degrees!
  Float_t fAngularDifference=TMath::RadToDeg()*vecCandidate.Angle(vecMetaMatch);

  return fAngularDifference;
}

//Shift the theta position of the ring associated to the trajectory according to the
//z-position of the vertex. This is required, because the parametrization used to
//map polar-angle to pad-position is only exact for particles emitted from the (0,0,0)-vertex
Float_t HPidTrackFiller::correctRingShift(HPidTrackCand* pCand)
{

  Float_t PolynomialCoeff[8];
  Float_t RingShift=0.0;

  HPidHitData* pHitData = pCand->getHitData();
  

  Float_t thetaRich=pHitData->fRichTheta;

  //The absolute difference in polar angle between RICH and MDC before the shift
  Float_t diffbefore=TMath::Abs(pHitData->getDeltaThetaMDCRICH());

  //retrieve shift-parametrization for this angular coordinate from the parameter container
  getPolynomialCoefficients(PolynomialCoeff, pCand->getHitData()->nSector);

  for(Int_t i=0;i<8;i++)
    {
      RingShift+=(Float_t)(PolynomialCoeff[i]*TMath::Power(thetaRich,i));
    }

  
  pHitData->fRichTheta+=RingShift;
  Float_t diffafter=TMath::Abs(pHitData->getDeltaThetaMDCRICH());
 
  return diffbefore-diffafter;
  
}

void HPidTrackFiller::getPolynomialCoefficients(Float_t* coefficients,Int_t sector)
{
  //Determine Vertex information from event data
  Float_t VertexZCoord=0.0;
  VertexZCoord=gHades->getCurrentEvent()->getHeader()->getVertexZ();
  
  //get coefficients from parameter container
  pTrackFillerParam->getPolynomialCoeff(sector,VertexZCoord, coefficients);
}

void HPidTrackFiller::assignRingCorrelations(HPidTrackCand* pCand)
{
  
  HPidHitData* pHitData = pCand->getHitData();
  HPidTrackData* pTrackData = pCand->getTrackData();
  if(!pHitData->getFlagRICH()) return; //No ring - no shift required

  //Get deviation of MDC track and RICH ring;
  //Convention: RICH-coordinate - MdcCoordinate!
  Float_t dThetaMDCRICH=pHitData->getDeltaThetaMDCRICH();
  Float_t dPhiMDCRICH=pHitData->getDeltaPhiMDCRICH();
  Float_t dThetaRKRICH=-99.0;
  Float_t dPhiRKRICH=-99.0;
  if(pTrackData->getRKTrackInd()>=0)
    {
      dThetaRKRICH=pHitData->getDeltaThetaRKRICH(pTrackData->getRKTheta());
      dPhiRKRICH=pHitData->getDeltaPhiRKRICH(pTrackData->getRKPhi(),pTrackData->getRKTheta());
      pTrackData->setRKRichMatchingQuality( sqrt( dThetaRKRICH * dThetaRKRICH + dPhiRKRICH * dPhiRKRICH));
    }
  Int_t sector = pHitData->nSector;
  
  //check for each momentum algorithm, whether the ring was in the window
  for(Int_t AlgCounter=0;AlgCounter<MAX_MOM_ALGS;AlgCounter++) 
    {

      //reset the flag indicating positive correlation
      pCand->getHitData()->hasRingCorrelation[AlgCounter]=kFALSE;

      //As RK is modifying the track direction it needs special treatment!
      if(pTrackData->bIsAccepted[AlgCounter])
	{
	  
	  if(AlgCounter==ALG_RUNGEKUTTA)
	    {
	      if( dPhiRKRICH > pTrackFillerParam->getDeltaPhiMin(sector, pTrackData->fMomenta[AlgCounter]) && 
		  dPhiRKRICH < pTrackFillerParam->getDeltaPhiMax(sector, pTrackData->fMomenta[AlgCounter]) &&
		  dThetaRKRICH >  pTrackFillerParam->getDeltaThetaMin(sector, pTrackData->fMomenta[AlgCounter]) && 
		  dThetaRKRICH < pTrackFillerParam->getDeltaThetaMax(sector, pTrackData->fMomenta[AlgCounter]) )
		{
		  
		  //this track has a ring
		  pCand->getHitData()->hasRingCorrelation[AlgCounter]=kTRUE;
		}
	    }
	  else
	    {	      
	      if( dPhiMDCRICH > pTrackFillerParam->getDeltaPhiMin(sector, pTrackData->fMomenta[AlgCounter]) && 
		  dPhiMDCRICH < pTrackFillerParam->getDeltaPhiMax(sector, pTrackData->fMomenta[AlgCounter]) &&
		  dThetaMDCRICH >  pTrackFillerParam->getDeltaThetaMin(sector, pTrackData->fMomenta[AlgCounter]) && 
		  dThetaMDCRICH < pTrackFillerParam->getDeltaThetaMax(sector, pTrackData->fMomenta[AlgCounter]) ){
		
		//this track has a ring
		pCand->getHitData()->hasRingCorrelation[AlgCounter]=kTRUE;
	      }
	      
	    }
	}
      
    }
}


void HPidTrackFiller::assignMetaCorrelations(HPidTrackCand* pCand)
{
  
  HPidTrackData* pTrackData = pCand->getTrackData();
  HPidHitData* pHitData = pCand->getHitData();
  Int_t sector = pHitData->getSector();
  Short_t system = pHitData->getSystem();

  if(system<0)
    {
      return;
    }

  //Get deviation of MDC/RK track and META hit;
  //In RK tracking the convention ois reversed with respect to the original MDC tracking
  //=> Dx -> -Dx, Dy -> -Dy
  Float_t dxRkMeta=-pTrackData->getRkMetadx();
  Float_t dyRkMeta=-pTrackData->getRkMetady();

  Float_t dxMdcMeta=pTrackData->getMdcMetadx();
  Float_t dyMdcMeta=pTrackData->getMdcMetady();

  
  //check for each momentum algorithm, whether the ring was in the window
  for(Int_t AlgCounter=0;AlgCounter<MAX_MOM_ALGS;AlgCounter++) 
    {
      if(pTrackData->bIsAccepted[AlgCounter])
	{
	  if(AlgCounter==ALG_RUNGEKUTTA)
	    {
	      if(
		 dxRkMeta>pTrackFillerParam->getDeltaRkMetaXMin(sector,system) &&
		 dxRkMeta<pTrackFillerParam->getDeltaRkMetaXMax(sector,system) &&
		 dyRkMeta>pTrackFillerParam->getDeltaRkMetaYMin(sector,system) &&
		 dyRkMeta<pTrackFillerParam->getDeltaRkMetaYMax(sector,system) 
		 )
		{
		  pCand->getHitData()->setMetaTrackCorrelation(AlgCounter,kTRUE);
		}
	    }
	  else
	    {
	      if(
		 dxMdcMeta>pTrackFillerParam->getDeltaMdcMetaXMin(sector,system) &&
		 dxMdcMeta<pTrackFillerParam->getDeltaMdcMetaXMax(sector,system) &&
		 dyMdcMeta>pTrackFillerParam->getDeltaMdcMetaYMin(sector,system) &&
		 dyMdcMeta<pTrackFillerParam->getDeltaMdcMetaYMax(sector,system)
		 )
		{
		  pCand->getHitData()->setMetaTrackCorrelation(AlgCounter,kTRUE);
		}
	      
	    }

	}
      
    }
}


Bool_t HPidTrackFiller::checkForLepton(HMetaMatch* pMatch)
{
  //Matching between TrkCand and RICH - this could be a lepton!
  if(pMatch->getARichInd(0)>-1) 
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
  if(bMakeSimCategory)
    {
      cout <<"NUMBER OF NO-PARENT-TRACKS: " << HPidGeantTrackSet::nullparent << endl;
    }
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
  
  pTrackFillerParam = NULL;
  
  bSkipNoLepton    = kFALSE;
  bSkipTofinoMult  = kFALSE;
  bNoChi2Selection[0] = kFALSE;//Default is to accept only tracks with chi^2>0
  bNoChi2Selection[1] = kFALSE;//Default is to accept only tracks with chi^2>0
  //bUsePlutoGenerator = kFALSE; //By default we assume URQMD

  bInitOk   = kFALSE;
  nAccTrack=nRejTrack=nTotalTrack=nShifts=0;
  meanDevChange = 0.0;
}

// -----------------------------------------------------------------------------

// Get next slot from the output category - no filling of data done here
HPidTrackCand* HPidTrackFiller::getNextSlot(void)
{

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

// get next slot from the output category and fill it with data from HMetaMatch
HPidTrackCand* HPidTrackFiller::getNextSlot(HMetaMatch *pMatch)
{
  //cout <<"trying to get next slot:" << endl;
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
      //cout <<"OK2" << endl;
      //cout << "Instantiating track cand" << endl;
      return new (pOut) HPidTrackCandSim(pMatch, iMatchIndex);
    }
  else
    {
      return new(pOut) HPidTrackCand(pMatch,iMatchIndex);
    }
}

// -----------------------------------------------------------------------------

// Check the condition for the tofino multiplicity > 1
// This function returns true only for mult<=1
Bool_t HPidTrackFiller::checkTofinoMult(HMetaMatch *pMatch)
{
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
//check Chi2 from MDC segments (if they exist) and accept only track with positive Chi2
//use Options NOCHI2SEG0 && NOCHI2SEG1 to decide in which mdc segments chi^2 has to be checked
//returns kTRUE if track is to be accepted
Bool_t HPidTrackFiller::checkChi2(HMetaMatch *pMatch){

  HMdcTrkCand *pTrkCand = NULL;
  HMdcSeg     *pMdcSeg1 = NULL;
  HMdcSeg     *pMdcSeg2 = NULL;
  Float_t     fChi2In   = 0.;
  Float_t     fChi2Out  = 0.;
  Int_t	      nSeg1Ind  = -1;
  Int_t	      nSeg2Ind  = -1;
  Int_t       nTrCand = pMatch->getTrkCandInd();
  
  if(nTrCand>-1)
    {
      //This should be replaced by the corresponding HPidFL-routine!
      pTrkCand = (HMdcTrkCand *)pCatTrkCand->getObject(nTrCand);
      if( NULL!= pTrkCand ){
	nSeg1Ind = pTrkCand->getSeg1Ind();
	nSeg2Ind = pTrkCand->getSeg2Ind();
	
	pMdcSeg1 = (HMdcSeg*)pCatMdcSeg->getObject(nSeg1Ind);//Should this be replaced by call to HPidFL-=routine?
	pMdcSeg2 = (HMdcSeg*)pCatMdcSeg->getObject(nSeg2Ind);//Should this be replaced by call to HPidFL-=routine?
	//check first segment
	if( NULL != pMdcSeg1 && !bNoChi2Selection[0]){ //We DO cut on inner chi^2
	  fChi2In = pMdcSeg1->getChi2();		
	}
	//check second segment
	if( NULL != pMdcSeg2 && !bNoChi2Selection[1]){ //We DO cut on outer chi^2
	  fChi2Out = pMdcSeg2->getChi2();		
	}
      }
    }
  else
    {
      Error("checkChi2","TrkCandInd not found");
    }
  if( fChi2In<0 || fChi2Out <0 )
    {
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
    //printf("USEPLUTOGENERATOR: %s\n",(bUsePlutoGenerator) ? "YES" : "NO");
}
