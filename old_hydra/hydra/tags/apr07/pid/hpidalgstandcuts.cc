//*-- Author : Jacek Otwinowski 22/02/2005
//*-- Author : Jacek Otwinowski 22/02/2005 
//*-- Modified : Jacek Otwinowski 15/03/2005
//*-- Modified : Jacek Otwinowski 19/04/2005
//*-- Modified : Jacek Otwinowski 10/05/2005
//*-- Modified : Jacek Otwinowski 23/05/2005
//*-- Modified : Jacek Otwinowski 23/08/2006
//*-- Modified : Jacek Otwinowski 11/09/2006
//*-- Modified : Y.C.Pachmayer 18/09/2006
//*-- Modified : Y.C.Pachmayer 08/10/2006
//*-- Modified : Y.C.Pachmayer 11/12/2006: put default values for rungekutta in case kickplane is used, added theta and phi from rich and rungekutta, added eventNr

//_HADES_CLASS_DESCRIPTION
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidAlgStandCuts                                                           //
//                                                                            //    
// Test algorithm for the PID with the standard cuts (windows):               //
// 1. RICH-rings (PM, AC, NP, RC)                                             //       
// 2. Beta vs Momentum windows in the system 0 and system 1                   //
// 3. Pre-Shower algorithm (F vs momentum, Sum0 vs momentum)                  //       
// 4. Pre-Shower algorithm (sum2+sum1-sum0 vs momentum)                  	  //       
//                                                                            //
// In case of lepton in the window the pdf vector element is set to 1.        //
// Otherwise, it is set to 0   												  //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#pragma implementation

#include "hpidalgstandcuts.h"
#include "hpidparticlesim.h"
#include "hpidtrackcand.h"
#include "hpidreconstructor.h"
#include "hpidphysicsconstants.h"

#include "hades.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "hpartialevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hshowerdetector.h"

#include "hkicktrack.h"
#include "hshowerhittof.h"
#include "hshowerhittoftrack.h"
#include "hgeantkine.h"
#include "hpidgeanttrackset.h"
#include "hgeantheader.h"

// -----------------------------------------------------------------------------

ClassImp(HPidAlgStandCuts)

// -----------------------------------------------------------------------------

HPidAlgStandCuts::HPidAlgStandCuts(TString outFileName, Option_t *opt_algorithms)
                        : HPidAlgorithm("StandCuts", algStandCuts)
{
// constructor
setDefaults();

setOutFileName(outFileName);
setAlgorithms(opt_algorithms);
}

// -----------------------------------------------------------------------------

HPidAlgStandCuts::HPidAlgStandCuts(Float_t fWeight,TString outFileName,Option_t *opt_algorithms)
                        : HPidAlgorithm("StandCuts", algStandCuts, fWeight)
{
// constructor with weight
setDefaults();

setOutFileName(outFileName);
setAlgorithms(opt_algorithms);
}

// -----------------------------------------------------------------------------

void HPidAlgStandCuts::setDefaults(void)
{
// default values
pParams = NULL;
bSim = kFALSE;
iSimEventFlag=0;

pOutFile =NULL;
pNtuple =NULL;
pMdcDiscriminator = NULL;

bUseRichRingCuts=kFALSE;
bUseMdcDeDx=kFALSE; 
bUseTofPvsBeta=kFALSE; 
bUseTofinoPvsBeta=kFALSE;
bUseShowerMaxFvsP=kFALSE;
bUseShowerSum0vsP=kFALSE;
bUseShowerSumDiffvsP=kFALSE;
	
bRich=kFALSE;     
bMdc=kFALSE; 
bShower=kFALSE; 
bTofino=kFALSE;
bTof=kFALSE;  
}

// -----------------------------------------------------------------------------

void HPidAlgStandCuts::clear(void)
{
// clear
    bRich=kFALSE;     
    bMdc=kFALSE; 
    bShower=kFALSE; 
    bTofino=kFALSE;
    bTof=kFALSE;  
}

// -----------------------------------------------------------------------------

void HPidAlgStandCuts::setAlgorithms(Option_t *opt_algorithms)
{
	TString sOptAlgorithms = opt_algorithms;
	sOptAlgorithms.ToUpper();

	bUseRichRingCuts = (strstr(sOptAlgorithms.Data(),"RICHRINGCUTS") != NULL);
	bUseMdcDeDx=kFALSE;
	//bUseMdcDeDx = (strstr(sOptAlgorithms.Data(),"MDCDEDX") != NULL);
	bUseTofPvsBeta = (strstr(sOptAlgorithms.Data(),"TOFPVSBETA") != NULL);
	bUseTofinoPvsBeta = (strstr(sOptAlgorithms.Data(),"TOFINOPVSBETA") != NULL);
	bUseShowerMaxFvsP = (strstr(sOptAlgorithms.Data(),"SHOWERMAXFVSP") != NULL);
	bUseShowerSum0vsP = (strstr(sOptAlgorithms.Data(),"SHOWERSUM0VSP") != NULL);
	bUseShowerSumDiffvsP = (strstr(sOptAlgorithms.Data(),"SHOWERSUMDIFFVSP") != NULL);
}

// -----------------------------------------------------------------------------

Bool_t HPidAlgStandCuts::init(void)
{

	// init parameters
	if((pParams = (HPidAlgStandCutsPar *)gHades->getRuntimeDb()
						->getContainer(PIDALGSTANDCUTSPAR_NAME)) == NULL)
	{
		Error("init", "Cannot get parameters: %s", PIDALGSTANDCUTSPAR_NAME);
		return kFALSE;
	}
	pParams->setContext(pRec->iSelectedMomAlg);
	
	// check simulation or experiment
	if( gHades->isReal() == kFALSE ) bSim = kTRUE;

	// check the event stucture in case of simulation
	if (bSim == kTRUE)
	{
	  //HRecEvent* pGeantEvent       = (HRecEvent*)(gHades->getCurrentEvent());
	  //HPartialEvent* pPartialEvent = pGeantEvent->getPartialEvent(catSimul);
	  //HGeantHeader* pGeantHeader   = (HGeantHeader*)(pPartialEvent->getSubHeader());

        // not exist at the moment
		//iSimEventFlag = pGeantHeader->getEventFlag();

		//iSimEventFlag = 2; // urqmd like events
		iSimEventFlag = -4; // pluto like events
	}

	//open output file and build output ntuple (parameter)
    if(sOutFileName.IsNull() == 0)	
	{
		if(openOutFile() == 0) 
		{ 
			::Error("HPidAlgStandCuts::init","Cannot open output file");
			return kFALSE ;
		}
		if(buildOutNtuple() == 0) 
		{
			::Error("HPidAlgStandCuts::init","Cannot build output ntuple");
			return kFALSE;
		}
	}    
    pMdcDiscriminator = new TF1("MdcDiscriminator","[0]*1/((x-[3])/1000)^[1]+[2]"); 
    if(pMdcDiscriminator==0)
      {
	::Error("HPidAlgStandCuts::init","Cannot build discriminator function for MDC");
      }
    else
      {
	pMdcDiscriminator->SetParameter(0,200);
	pMdcDiscriminator->SetParameter(1,0.18);
	pMdcDiscriminator->SetParameter(2,-90);
	pMdcDiscriminator->SetParameter(3,100);
      }
return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HPidAlgStandCuts::openOutFile()
{
// open output ntuple file
	pOutFile = new TFile(sOutFileName.Data(),"recreate");
	if(pOutFile != NULL)  
	{
		pOutFile->cd();
		return kTRUE;
	}
return kFALSE;	
}

// -----------------------------------------------------------------------------

Bool_t HPidAlgStandCuts::buildOutNtuple()
{
// bild output ntuple for sim/exp
	if( bSim == kFALSE )
	{
            pNtuple = new TNtuple("pLep","Lept track properties",
				  "pid:rich_pmat:rich_acharge:rich_npad:rich_rcent:tof:beta:mom:charge:mass2:show_maxF:show_sum0:mdc_theta:mdc_phi:rich_theta:rich_phi:rk_theta:rk_phi:sec:sys:isrich:ismdc:istof:istofino:isshower:chi2inmdc:chi2outmdc:trackr:trackz:dist2vertex:pull:splinechi2:qmetamatch:closestleptoncandidateisfitted:closesthadroncandidateisfitted:anglewithclosestleptoncandidate:anglewithclosesthadroncandidate:trigger_ds_flag:trigger_ds_fact:trigger_decision:trigger_tbit:mult:IOm_chi2:rkchi2:metaeloss:tofinomult:inmdcdedx:inmdcdedxsigma:outmdcdedx:outmdcdedxsigma:combinedmdcdedx:combinedmdcdedxsigma:show_sum1:show_sum2:evtNr:dxMdcMeta:dyMdcMeta:dxRkMeta:dyRkMeta:dzRkMeta");
	}
	else
	{
	    pNtuple = new TNtuple("pLep","Lept track properties",
				  "pid:rich_pmat:rich_acharge:rich_npad:rich_rcent:tof:beta:mom:charge:mass2:show_maxF:show_sum0:mdc_theta:mdc_phi:rich_theta:rich_phi:rk_theta:rk_phi:sec:sys:isrich:istof:istofino:isshower:chi2inmdc:chi2outmdc:trackr:trackz:dist2vertex:pull:splinechi2:gcomtrk:gpid:gparentid:gparttrk:gparenttrk:gmom:gvx:gvy:gvz:ggeninfo:ggenweight:gmech:gcomtrk1:gmech1:gmed:qmetamatch:closestleptoncandidateisfitted:closesthadroncandidateisfitted:anglewithclosestleptoncandidate:anglewithclosesthadroncandidate:ggeninfo1:ggeninfo2:mult:IOm_chi2:rkchi2:metaeloss:inmdcdedx:inmdcdedxsigma:outmdcdedx:outmdcdedxsigma:combinedmdcdedx:combinedmdcdedxsigma:show_sum1:show_sum2:evtNr:dxMdcMeta:dyMdcMeta:dxRkMeta:dyRkMeta:dzRkMeta");
	}

	if (pNtuple != NULL) return kTRUE;

return kFALSE;
}

// -----------------------------------------------------------------------------

Bool_t HPidAlgStandCuts::writeOutFile(void)
{
// write output ntuple to file
   if(pOutFile == NULL)
   {
      Error("writeOutFile", "No output file");
      return kFALSE;
   }
	
   if(pOutFile != NULL) 
   {
   	   Warning("writeOutFile","writing...");

	   pOutFile->cd();
	   pOutFile->Write();

	   delete pOutFile;
	   pOutFile = NULL;

	   Warning("writeOutFile","ok");
   }
return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HPidAlgStandCuts::reinit(void)
{
//Check if parameter context corresponds to the
//appropriate momentum algorithm index
//If not -> return kFALSE (analysis can not be done in such case)
//and print error message
    return pParams->checkContext(pRec->iSelectedMomAlg);
}

// -----------------------------------------------------------------------------

Bool_t HPidAlgStandCuts::finalize(void)
{
	// write output file
	
	if(sOutFileName.IsNull()==0)
	{
    	return writeOutFile();
	}

return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HPidAlgStandCuts::isLepInRich(Int_t iPid,HPidHitData *pHitData) 
{
// Check RICH ring quality by using standard cuts   
	
Int_t sec = pHitData->getSector(); 

Stat_t iRingPatMatrThresh 	= pParams->getValue(iPid,0,sec,0); 
							// particle id, 0 - hist offset, sector, 0 bin value 
Float_t fRingCentroidThresh = pParams->getValue(iPid,0,sec,1);
Stat_t 	iRingPadNrThres    	= pParams->getValue(iPid,0,sec,2);
Float_t fRingAvChargeThres  = pParams->getValue(iPid,0,sec,3);

Float_t fRingAvChrg = ((Float_t)pHitData->nRingAmplitude)/((Float_t)pHitData->nRingPadNr);

  if(pHitData->nRingPatMat <= iRingPatMatrThresh )  return kFALSE;
  if(pHitData->fRingCentroid >= fRingCentroidThresh) return  kFALSE;	
  if(pHitData->nRingPadNr <= iRingPadNrThres ) return  kFALSE;
  if(fRingAvChrg <= fRingAvChargeThres ) return  kFALSE; 	

return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HPidAlgStandCuts::isLepInShower(Int_t iPid,HPidHitData *pHitData,HPidTrackData *pTrackData) 
{
// Check whether lepton fulfills PreShower conditions 
// F(p)- threshold, Sum0(p)- upper limit

Int_t   sec = pHitData->getSector();
Int_t   iSelectedMomAlg = pRec->iSelectedMomAlg;
Float_t fMom    = pTrackData->fMomenta[iSelectedMomAlg];

Float_t fF10=0, fF20=0;
Float_t fSumDiff = -1000.;
Float_t fSum0   = pHitData->fShowerSum[0];
Float_t fSum1   = pHitData->fShowerSum[1];
Float_t fSum2   = pHitData->fShowerSum[2];

if(fSum0 != 0) 
{
	 fF10 = fSum1 / fSum0;  
	 fF20 = fSum2 / fSum0;
}

fSumDiff = fSum2+fSum1-fSum0;

Float_t fMaxF   = TMath::Max(fF10,fF20);
Float_t a,b,c,d,e,f,g,h,i,j,k;

a = pParams->getValue(iPid,1,sec,0);
b = pParams->getValue(iPid,1,sec,1);
c = pParams->getValue(iPid,1,sec,2);
d = pParams->getValue(iPid,1,sec,3);

e = pParams->getValue(iPid,2,sec,0);
f = pParams->getValue(iPid,2,sec,1);
g = pParams->getValue(iPid,2,sec,2);

h = pParams->getValue(iPid,7,sec,0);
i = pParams->getValue(iPid,7,sec,1);
j = pParams->getValue(iPid,7,sec,2);
k = pParams->getValue(iPid,7,sec,3);

Float_t fThrMaxF = (a+b*fMom+c*fMom*fMom+d*fMom*fMom*fMom);
Float_t fUpperLimitSum0 = (e+f*fMom+g*fMom*fMom);
Float_t fThrSumDiff = (h+i*fMom+j*fMom*fMom+k*fMom*fMom*fMom);

if(	bUseShowerMaxFvsP && fMaxF == 0.0 ) return kFALSE;
if( bUseShowerMaxFvsP && fMaxF <= fThrMaxF ) return kFALSE;
if( bUseShowerSum0vsP && fSum0 >= fUpperLimitSum0 ) return kFALSE;
if( bUseShowerSumDiffvsP && fSumDiff <= fThrSumDiff ) return kFALSE;

return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HPidAlgStandCuts::isLepInTofino(Int_t iPid,HPidHitData *pHitData, HPidTrackData *pTrackData) 
{
// Check whether lepton is in beta vs momentum window (system 0)

Int_t sec = pHitData->getSector();
Int_t   iSelectedMomAlg = pRec->iSelectedMomAlg;

Float_t fBeta   = pTrackData->getBeta(iSelectedMomAlg);
Float_t fMom    = pTrackData->fMomenta[iSelectedMomAlg];

  if(fBeta <= pParams->getValue(iPid,3,sec,fMom)) return kFALSE;
  // upper limit not used in case of Tofino
  //if(fBeta >= pBetaParams->getValue(iPid,4,sec,fMom) ) return kFALSE;

return kTRUE;
}

Bool_t HPidAlgStandCuts::isLepInMdc(Int_t iPid,HPidHitData *pHitData, HPidTrackData *pTrackData) 
{
  // at the moment this function is not used for hard cut analysis/ single lepton identification
  // thus this function is always kTRUE at the moment. This needs to be changed as soon as one
  // has determined how to use this cut for particle identification or removal of conversion or ...

  // Check whether this track has a lepton-like energy loss

  Int_t   iSelectedMomAlg = pRec->iSelectedMomAlg;
  Int_t   iPolarity = pTrackData->getPolarity(iSelectedMomAlg);
  Float_t fMdcEloss ;
  if(iSelectedMomAlg> ALG_KICK)
  {
      fMdcEloss = pHitData->getCombinedMdcdEdx();
  }
  else
  {
      fMdcEloss = pHitData->getInnerMdcdEdx();
  }

  Float_t fMom    = pTrackData->fMomenta[iSelectedMomAlg];

  if(iPolarity<0) return kTRUE; //Do not use this for negative tracks
  if(fMom<300) return kTRUE;    //Do not use this for slow particles

  //Compute a momentum-dependent threshold below which all particles are treated as electrons
  //and above which they are discarded.

  //Temporarily hardwired cut to discriminate between eleptrons and hadrons -> polynomial in next step!
  //if(fMdcEloss > pMdcDiscriminator->Eval(fMom)) return kFALSE;
  if(fMdcEloss > 130) return kFALSE;

return kTRUE;
}


// -----------------------------------------------------------------------------

Bool_t HPidAlgStandCuts::isLepInTof(Int_t iPid,HPidHitData *pHitData, HPidTrackData *pTrackData) 
{
// Check whether lepton is in beta vs momentum window (system 1)

Int_t   sec = pHitData->getSector();
Int_t   iSelectedMomAlg = pRec->iSelectedMomAlg;

Float_t fBeta   = pTrackData->getBeta(iSelectedMomAlg);
Float_t fMom    = pTrackData->fMomenta[iSelectedMomAlg];

        if(fBeta <= pParams->getValue(iPid,5,sec,fMom)) return kFALSE;
		if(fBeta >= pParams->getValue(iPid,6,sec,fMom)) return kFALSE;

return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HPidAlgStandCuts::fillOutNtuple(Int_t pid,HPidTrackCandSim *pTrack)
{ 
//fill ntuple in experiment and in simulation
HPidTrackData *pTrackData = NULL;
HPidHitData *pHitData = NULL;
HPidGeantTrackSet *pGeantSet = NULL;

	if((pTrackData = pTrack->getTrackData()) == NULL)
	{
	  ::Error("HPidAlgStandCuts::fillOutNtuple", "Cannot get pTrackData");
	  return kFALSE;
	}

	if((pHitData = pTrack->getHitData()) == NULL)
	{
	  ::Error("HPidAlgStandCuts::fillOutNtuple", "Cannot get pHitData");
	  return kFALSE;
	}

	if(bSim == kTRUE)
	{
	   if((pGeantSet = pTrack->getGeantTrackSet()) == NULL)
	   {
	      ::Error("HPidAlgStandCuts::fillOutNtuple", "Cannot get pGeantSet");
	      return kFALSE;
	   }
	}

Float_t splinechi2=-1, rkchi2=-1, thetaRK=-1,phiRK=-1; //Segment variables modified by RK tracking;
Int_t nEvtNr=-1;
Float_t combinedmdcdedx=-1, combinedmdcdedxsigma=-1;

Int_t   iSelectedMomAlg = pRec->iSelectedMomAlg;
Float_t dxRkMeta=-1, dyRkMeta=-1, dzRkMeta=-1;

//pidtrack data
Float_t charge     = pTrackData->nPolarity[iSelectedMomAlg];
Float_t beta       = pTrackData->getBeta(iSelectedMomAlg);
Float_t mom        = pTrackData->fMomenta[iSelectedMomAlg];
Float_t mass2      = pTrackData->getMass2(iSelectedMomAlg);
Float_t trackr     = pTrackData->fTrackR[iSelectedMomAlg];
Float_t trackz     = pTrackData->fTrackZ[iSelectedMomAlg];
Float_t pull       = pTrackData->fPull;
Float_t qmetamatch = pTrackData->fMetaMatchingQuality;
Float_t IOm_chi2   = pTrackData->qIOMatching[iSelectedMomAlg];
Float_t metaeloss  = pTrackData->getCorrectedEloss(iSelectedMomAlg);

// only x and y coordinates of dMdcMeta is stored, because in the corresponding coordinate system dz is zero
Float_t dxMdcMeta  = pTrackData->getMdcMetadx();
Float_t dyMdcMeta  = pTrackData->getMdcMetady();

if(iSelectedMomAlg> ALG_KICK123) splinechi2 = pTrackData->fSplineChiSquare;
if(iSelectedMomAlg> ALG_SPLINE)
{
    rkchi2     = pTrackData->fRKChiSquare;
    // theta and phi from inner segment in degree, because: pRKTrack->getThetaSeg1RK()*TMath::RadToDeg();
    thetaRK    = pTrackData->fRKTheta;
    phiRK      = pTrackData->fRKPhi;
    dxRkMeta   = pTrackData->getRkMetadx();
    dyRkMeta   = pTrackData->getRkMetady();
    dzRkMeta   = pTrackData->getRkMetadz();
}

//Bool_t isclosepaircandidate  = pTrackData->bIsClosePairCandidate;
//Float_t anglewithclosepaircandidate = pTrackData->fAngleWithClosePairCandidate;
 

 Bool_t  closestleptoncandidateisfitted  = pTrackData->closestLeptonCandidateIsFitted();
 Bool_t  closesthadroncandidateisfitted  = pTrackData->closestHadronCandidateIsFitted();
 Float_t anglewithclosestleptoncandidate = pTrackData->getAngleWithClosestLeptonCandidate();
 Float_t anglewithclosesthadroncandidate = pTrackData->getAngleWithClosestHadronCandidate();


//pid hit data
Float_t theta = pHitData->fMdcTheta;
Float_t phi = pHitData->fMdcPhi;
Float_t thetaRich=pHitData->fRichTheta;
Float_t phiRich=pHitData->fRichPhi;
Int_t sec = pHitData->getSector(); 
Int_t sys = pHitData->iSystem;
Float_t chi2inmdc = pHitData->fInnerMdcChiSquare;
Float_t chi2outmdc = pHitData->fOuterMdcChiSquare;
Float_t dist2vertex = pHitData->fDistanceToVertex[iSelectedMomAlg];
Float_t tof = pHitData->getTof();
Int_t mult = pHitData->iIndMatch;

//rich
Float_t rich_pmat  = pHitData->nRingPatMat;
Float_t rich_acharge = ((Float_t)pHitData->nRingAmplitude)/((Float_t)pHitData->nRingPadNr);
Float_t rich_rcent = pHitData->fRingCentroid;
Float_t rich_npad  = pHitData->nRingPadNr;

//shower
Float_t show_sum0   = pHitData->fShowerSum[0];
Float_t show_sum1   = pHitData->fShowerSum[1];
Float_t show_sum2   = pHitData->fShowerSum[2];
Float_t f10 = -1.0;
Float_t f20 = -1.0;
if(show_sum0 != 0) 
{
  f10 = show_sum1 / show_sum0;  
  f20 = show_sum2 / show_sum0;
}
Float_t show_maxF = TMath::Max(f10,f20);

// tof and tofino
Int_t tofinomult = pHitData->getTofinoMult();
    
// mdc
Float_t inmdcdedx = pHitData->getInnerMdcdEdx();
Float_t inmdcdedxsigma = pHitData->getInnerMdcdEdxSigma();
Float_t outmdcdedx = pHitData->getOuterMdcdEdx();
Float_t outmdcdedxsigma = pHitData->getOuterMdcdEdxSigma();

if(iSelectedMomAlg> ALG_KICK)
{
combinedmdcdedx=pHitData->getCombinedMdcdEdx();
combinedmdcdedxsigma=pHitData->getCombinedMdcdEdxSigma();
}

// event number
nEvtNr    =  gHades->getCurrentEvent()->getHeader()->getEventSeqNumber();

//geant part
Int_t  gcomtrk=-1,gpid=-1,gparentid=-1,gparttrk=-1,gparenttrk=-1, gmech = -1, gcomtrk1=-1, gmech1=-1, gmed=-1;
Float_t gmom=-1,gvx=-10000,gvy=-10000,gvz=-10000,ggeninfo=-1,ggenweight=-1,ggeninfo1=-1,ggeninfo2=-1;

if( bSim == kTRUE) 
{
   if ( pGeantSet->getMostCommonCorrelation() != 0)	
   {
       gcomtrk = pGeantSet->getCorrelationFlag(0);
       gmech = pGeantSet->getGeantProcessID(0);
       gmed = pGeantSet->getGeantMediumID(0);
       if ( pGeantSet->getNCorrelatedTrackIds() > 1)
	   {
	       // interesting the second one
	       gcomtrk1 = pGeantSet->getCorrelationFlag(1);
	       gmech1 = pGeantSet->getGeantProcessID(1);
	   }

       gparttrk=pGeantSet->getGeantTrackID(0);
       gpid=pGeantSet->getGeantPID(0);
       gvx=pGeantSet->getGeantVertexX(0);
       gvy=pGeantSet->getGeantVertexY(0);
       gvz=pGeantSet->getGeantVertexZ(0);
       ggeninfo=pGeantSet->getGenInfo(0);
       ggenweight=pGeantSet->getGenWeight(0);

       if(iSimEventFlag == -4) // pluto like events
       {
	   ggeninfo=pGeantSet->getGenInfo(0);
	   ggeninfo1=pGeantSet->getGenInfo1(0);
	   ggeninfo2=pGeantSet->getGenInfo2(0);
       }
       gmom = pGeantSet->getTotalMomentum(0);

       //get parent pid
       gparenttrk= pGeantSet->getGeantParentTrackID(0);
       gparentid=  pGeantSet->getGeantParentID(0);

   }
}
	// fill ntupe
	if(bSim == kFALSE) //experiment
	{
        // trigger information 
		Int_t trigger_ds_flag  = gHades->getCurrentEvent()->getHeader()->getDownscalingFlag();
		Int_t trigger_ds_fact  = gHades->getCurrentEvent()->getHeader()->getDownscaling();
		Int_t trigger_decision = gHades->getCurrentEvent()->getHeader()->getTriggerDecision();
		Int_t trigger_tbit     = gHades->getCurrentEvent()->getHeader()->getTBit();

		Float_t val[] = { pid,rich_pmat,rich_acharge,rich_npad,rich_rcent,tof,beta,mom,charge,mass2,show_maxF,show_sum0,theta,phi,thetaRich,phiRich,thetaRK,phiRK,sec,sys,bRich,bMdc,bTof,bTofino,bShower,chi2inmdc,chi2outmdc,trackr,trackz,dist2vertex,pull,splinechi2,qmetamatch,closestleptoncandidateisfitted,closesthadroncandidateisfitted,anglewithclosestleptoncandidate, anglewithclosesthadroncandidate,trigger_ds_flag,trigger_ds_fact,trigger_decision,trigger_tbit,mult,IOm_chi2,rkchi2,metaeloss,tofinomult,inmdcdedx,inmdcdedxsigma,outmdcdedx,outmdcdedxsigma,combinedmdcdedx,combinedmdcdedxsigma,show_sum1,show_sum2,nEvtNr,dxMdcMeta,dyMdcMeta,dxRkMeta,dyRkMeta,dzRkMeta};
                pNtuple->Fill(val);
	}
	else
	{
          Float_t val[] = { pid,rich_pmat,rich_acharge,rich_npad,rich_rcent,tof,beta,mom,charge,mass2,show_maxF,show_sum0,theta,phi,thetaRich,phiRich,thetaRK,phiRK,sec,sys,bRich,bTof,bTofino,bShower,chi2inmdc,chi2outmdc,trackr,trackz,dist2vertex,pull,splinechi2,gcomtrk,gpid,gparentid,gparttrk,gparenttrk,gmom,gvx,gvy,gvz,ggeninfo,ggenweight, gmech, gcomtrk1,gmech1,gmed,qmetamatch,closestleptoncandidateisfitted,closesthadroncandidateisfitted,anglewithclosestleptoncandidate, anglewithclosesthadroncandidate,ggeninfo1,ggeninfo2,mult,IOm_chi2,rkchi2,metaeloss,inmdcdedx,inmdcdedxsigma,outmdcdedx,outmdcdedxsigma,combinedmdcdedx,combinedmdcdedxsigma,show_sum1,show_sum2,nEvtNr,dxMdcMeta,dyMdcMeta,dxRkMeta,dyRkMeta,dzRkMeta};
	  pNtuple->Fill(val);
	}
return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HPidAlgStandCuts::calculatePDF(HPidTrackCand *pTrack,
		Float_t afReturn[], Short_t &nCatIndex)
{
// Calculate PDF from data (return 1 in case of lepton in the window)  

HPidTrackData *pTrackData = NULL;
HPidHitData *pHitData = NULL;

    if((pTrackData = pTrack->getTrackData()) == NULL)
    {
        ::Error("HPidAlgStandCuts::calculatePDF", "Cannot get pTrackData");
        return kFALSE;
    }

    if((pHitData = pTrack->getHitData()) == NULL)
    {
        ::Error("HPidAlgStandCuts::calculatePDF", "Cannot get pHitData");
        return kFALSE;
    }

Int_t   iSelectedMomAlg = pRec->iSelectedMomAlg;
Float_t fCharge = pTrackData->nPolarity[iSelectedMomAlg];
Int_t   iSys    = pHitData->iSystem;

Bool_t  bRingCorrelated = kFALSE; 

Int_t   iId;
Int_t   i;

    /*
	// only leptons in momentum dependent windows
	if( (pTrack->getHitData()->hasRingCorrelation[iSelectedMomAlg]) == kFALSE )
	{
		 return kTRUE;
	}
	*/

	// check Rich-MDC momentum dependent correlation 
    bRingCorrelated = pTrack->getHitData()->hasRingCorrelation[iSelectedMomAlg]; 

    // loop over predefined particles
    for(i = 0; i < pRec->particlesNumber(); i++)
    {
		// clear flags
        clear();

        iId = pRec->getParticleId(i);        
		if(iId != 2 && iId != 3) continue;

        // compare the particle charge
        if(fCharge != HPidPhysicsConstants::charge(iId))
        {
            afReturn[i] = 0.0f;
            continue;
        }
		// calculate PDFs
        if(! HPidPhysicsConstants::isFake(iId))
        {
	  // rich 
	  if(bUseRichRingCuts && isLepInRich(iId,pHitData) != 0) bRich=kTRUE; 
	  
	  // mdc dedx works only for experiment
	  if(bUseMdcDeDx)
	    {
	      if(bSim==0 && isLepInMdc(iId,pHitData,pTrackData) != 0)
		{ 
		  bMdc=kTRUE; 
		}
	    }
	  else
	    {
	      bMdc=kTRUE;
	    }

	  if(iSys == 0)
	    {
	      if(bUseTofinoPvsBeta && isLepInTofino(iId,pHitData,pTrackData) != 0) bTofino=kTRUE;
	      
	      if(bUseShowerMaxFvsP || bUseShowerSum0vsP || bUseShowerSumDiffvsP)
		{
		  if(isLepInShower(iId,pHitData,pTrackData) != 0) bShower=kTRUE;
		}
	    }
	  else
	    {
	      if(bUseTofPvsBeta && isLepInTof(iId,pHitData,pTrackData) != 0) bTof=kTRUE;
	    }

		//cout << "bRingCorrelated " << bRingCorrelated<< " bRich "<< bRich << " bMdc " << 
		//bMdc <<" bTof " << bTof <<" bTofino " << bTofino<< " bShower " <<bShower << endl;
	  
	  	if(bSim==0)
		{
			// Mdc de/dx good only for eperiment
			if(bRingCorrelated && bRich && bMdc && (bTof || (bTofino && bShower))) 
	    	{
	      		afReturn[i] = 1.0f;
	    	}
	  		else 
	    	{
	     	 	afReturn[i] = 0.0f;
	    	}
        }
		else 
		{
			if(bRingCorrelated && bRich && (bTof || (bTofino && bShower))) 
	    	{
	      		afReturn[i] = 1.0f;
	    	}
	  		else 
	    	{
	      		afReturn[i] = 0.0f;
	    	}
		}
		}
        else
	  {
            // background functions
	    afReturn[i] = 0.0f;
	  }
	
	if(pNtuple != NULL) // fill control ntuple
	  {
	    // only correlated lepton tracks in the ntuple
	    if(bRingCorrelated) fillOutNtuple(iId,(HPidTrackCandSim*)pTrack);
	  }
    }
    return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HPidAlgStandCuts::calculateCL(HPidTrackCand *pTrack,
            Float_t afReturn[], Short_t &nCatIndex)
{
    return kTRUE;
}

// -----------------------------------------------------------------------------

void HPidAlgStandCuts::print(void) const
{
// Empty function for printing info of the algorithm

    printf("\tStandCuts algorithm\n");
}
