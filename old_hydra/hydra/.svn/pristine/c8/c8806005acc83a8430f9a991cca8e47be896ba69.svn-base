//*-- Author : Stefano Spataro (spataro@lns.infn.it)

// ***************************************************************************
//_HADES_CLASS_DESCRIPTION 
//////////////////////////////////////////////////////////////////////////////
// HPidTofRec
// In case of experiments where there exists no start hit and the TOF of the
// particles has to be recalculated. This class will reconstruct it,
// and the new value (plus the new beta and new mass2), will be written inside HPidTrackData
// To mark those objects as changed one can ask to HPidTrackData::getTofRecFlag(algorythm).
// The result should be 1 or 2 depending on the method which has been used, -1 if the
// reconstructor failed (such as in cases when there are only positive charged tracks and no
// leptons), 0 (default value) if the reconstructor did not run.
// Anyhow the original tof can be retrieved direct HPidHitData::getTof().
//
// To setup the constructor  call HPidTofRec(Text_t *name,Text_t *title, const char *select, const char *track),
// where "select" contains the key words for configuration (just "," separated list)
//    nospline   : switch off spline
//    nohires    : switch off high resolution kickplane
//    nolowres   : switch off low  resolution kickplane
//    nork       : switch off Runge Kutta tracking
//    noreft     : switch off Reference Trajectories tracking
//    norich     : switch off rich mode
//    ntuple     : switch on create,fill and write ntuple for debugging
//    chiin      : switch on the chi2>-1 cut for inner MDC segments
//    chiout     : switch on the chi2>-1 cut for outer MDC segments
//    common     : use spline as reconstructor for all the trackings
//    skip       : skip event in DST if start is not reconstructed
//    sim        : switch on simulation mode
//
// if common mode is switched on, "track" is the chosen tracking algorythm for global tof reconstruction:
//    spline     : spline
//    lowres     : low resolution kickplane
//    hires      : high resolution kickplane
//    rk         : Runge Kutta tracking
//    reft       : Reference Trajectories tracking
//
// defaults (empty string) are:
//   spline                      ON
//   low  resolution kickplane   ON
//   high resolution kickplane   ON
//   RungeKutta                  ON
//   Reference Trajectories      ON
//   fRichMode                   ON
//   ntuple                      OFF
//   chi cut (inner MDC) mode    OFF
//   chi cut (outer MDC) mode    OFF
//   skip event                  OFF
//   common                      OFF
//   simulation                  OFF
//
// Usage:
//
//   HPidTofRec *pidtofrec = new HPidTofRec("tofreconstructor","tofreconstructor",option,track);
//   pidtofrec->setQualityRichParams(200.,4.,2.8,5);
//   HTaskSet * pidtofrectask = new HTaskSet("","");
//   pidtofrectask->connect(pidtofrec);
//   masterTaskSet->add(pidtofrec);
//
//////////////////////////////////////////////////////////////////////////////
// ***************************************************************************
  
#include "hpidtofrec.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include <TMath.h>
#include "hpidtrackcandsim.h"
#include "hpidgeanttrackset.h"
#include "hpidtrackdata.h"
#include "hpidhitdata.h"
#include "hpidphysicsconstants.h"
#include <iostream>

  ClassImp(HPidTofRec)
  
  HPidTofRec::HPidTofRec()
{
  // Default constructor
  
  fRichQparams[0] = 0.; fRichQparams[1] = 0.; fRichQparams[2] = 1000.; fRichQparams[3] = 0.;
  nCommonFlag = -1;
  
  for (Short_t nSs=0; nSs<6;nSs++) 
    {
      nMdcSeg[nSs] = 0;
      for (Short_t nMm=0; nMm<4;nMm++)
	if (gHades->getSetup()->getDetector("Mdc")->getModule(nSs,nMm)) nMdcSeg[nSs]++;
    }
  
  hit = 0;
  richcorr = 0;

  bRichMode        = kTRUE;
  bMinusMode       = kTRUE;
  bFillNTuple      = kFALSE;
  bChiCutInner     = kFALSE;
  bChiCutOuter     = kFALSE;
  bSkip            = kFALSE;
  bIsCommon        = kFALSE;
  bIsSimulation    = kFALSE;
  
  for (Int_t nAlg=0; nAlg<nMomAlgs; nAlg++) // switch on all the tracking algorythms
    bTrackAlg[nAlg] = kTRUE;
}

HPidTofRec::HPidTofRec(Text_t *name,Text_t *title, const char *select, const char *track) : 
  HReconstructor(name,title)
{
  // Constructor
  
  TString sel = select;
  sel.ToLower();
  TString tracking = track;
  tracking.ToLower();
 
  bRichMode        = kTRUE;
  bMinusMode       = kTRUE;
  bFillNTuple      = kFALSE;
  bChiCutInner     = kFALSE;
  bChiCutOuter     = kFALSE;
  bSkip            = kFALSE;
  bIsCommon        = kFALSE;
  bIsSimulation    = kFALSE;
 
  for (Int_t nAlg=0; nAlg<nMomAlgs; nAlg++) // switch on all the tracking algorythms
    bTrackAlg[nAlg] = kTRUE;
  
  fRichQparams[0] = 0.; fRichQparams[1] = 0.; fRichQparams[2] = 1000.; fRichQparams[3] = 0.;
  
  nCommonFlag = -1;
  
  if (sel.Contains("nospline"))   bTrackAlg[ALG_SPLINE]     = kFALSE;
  if (sel.Contains("nohires"))    bTrackAlg[ALG_KICK123]    = kFALSE;
  if (sel.Contains("nolowres"))   bTrackAlg[ALG_KICK]       = kFALSE;
  if (sel.Contains("nork"))       bTrackAlg[ALG_RUNGEKUTTA] = kFALSE;
  if (sel.Contains("noreft"))     bTrackAlg[ALG_REFT]       = kFALSE;
 
  if (sel.Contains("norich"))     bRichMode        = kFALSE;
  if (sel.Contains("ntuple"))     bFillNTuple      = kTRUE;
  if (sel.Contains("chiin"))      bChiCutInner     = kTRUE;
  if (sel.Contains("chiout"))     bChiCutOuter     = kTRUE;
  if (sel.Contains("skip"))       bSkip            = kTRUE;
  if (sel.Contains("sim"))        bIsSimulation    = kTRUE;
  if (sel.Contains("common")) {
    if (tracking.Contains("spline"))  nCommonFlag = ALG_SPLINE;
    if (tracking.Contains("lowres"))  nCommonFlag = ALG_KICK;
    if (tracking.Contains("hires"))   nCommonFlag = ALG_KICK123;
    if (tracking.Contains("rk"))      nCommonFlag = ALG_RUNGEKUTTA;
    if (tracking.Contains("reft"))    nCommonFlag = ALG_REFT;
    if (nCommonFlag!=-1) bIsCommon = kTRUE;
    else Warning("init","HPidTofRec: No common tracking setted -> Switching Common Mode OFF");
  }
  
  for (Short_t nSs=0; nSs<6;nSs++) 
    {
      nMdcSeg[nSs] = 0;
      for (Short_t nMm=0; nMm<4;nMm++)
	if (gHades->getSetup()->getDetector("Mdc")->getModule(nSs,nMm)) nMdcSeg[nSs]++;
    }
  
  hit    = 0;
  richcorr = 0;
  sDir  = "./";
  sFile = "";
}

HPidTofRec::~HPidTofRec(void){
  // Destructor.
}

Bool_t HPidTofRec::init(void){
  cout << "PID Tof Reconstructor default initialization *** " << endl;
  
  pTrackCand=gHades->getCurrentEvent()->getCategory(catPidTrackCand);
  if (!pTrackCand) {
    Error("init","HPidTofRec: No HPIDTrackCand Input -> Switching HPidTofRec OFF");
  }
  else iterTrackCand= (HIterator *)pTrackCand->MakeIterator();
  
  cout << "Spline: " <<bTrackAlg[ALG_SPLINE] <<" Kick lowres: "<< bTrackAlg[ALG_KICK] << " Kick hires: "<< bTrackAlg[ALG_KICK123] << " RungeKutta: "<< bTrackAlg[ALG_RUNGEKUTTA] << " RefTraj: "<< bTrackAlg[ALG_REFT] << endl;
  cout << " Rich Mode: "<<bRichMode<<" Minus Mode: " << bMinusMode << " ChiCutMode (INNER) " << bChiCutInner << " ChiCutMode (OUTER) " << bChiCutOuter<< " Ntuple Mode: " << bFillNTuple << endl;
  cout << "Skip Event: " << bSkip << " Common Mode: " << bIsCommon  << " Simulation Mode: " << bIsSimulation << endl;
  cout << " Mdc Sectors:\t\t";
  for (Int_t nSs=0; nSs<6; nSs++) cout << nMdcSeg[nSs]  << " "; 
  cout << endl;
  
  if (bRichMode)
    {
      cout << "Rich Quality parameters: RingPatMat "<< fRichQparams[0] << " RingPadNr " << fRichQparams[1] << " Centroid ";
      cout << fRichQparams[2] << " RingAmplitude " << fRichQparams[3] << endl;
    }
   
  if (bFillNTuple)
    {
      if(bIsSimulation)
	hit    = new TNtuple("hit"   ,"hit"      ,"P:Pol:Sec:Sys:Theta:Phi:Flag:Alg:TofOld:TofNew:BetaOld:BetaNew:Mass2Old:Mass2New:RecSys:RecPol:RecSec:Chi2In:Chi2Out:Z:R:TofinoMult:Id:Rich:QSpline:GeantFlag:GeantP:GeantPID:GeantParent");
      else hit    = new TNtuple("hit"   ,"hit"      ,"P:Pol:Sec:Sys:Theta:Phi:Flag:Alg:TofOld:TofNew:BetaOld:BetaNew:Mass2Old:Mass2New:RecSys:RecPol:RecSec:Chi2In:Chi2Out:Z:R:TofinoMult:Id:Rich:QSpline");
      richcorr = new TNtuple("richcorr","rich corr","MdcTheta:RichTheta:MdcPhi:RichPhi:Sec:Sys:Pol:P:Alg:Chi2In:Chi2Out:Z:R:PatMat:PadNr:Centr:Ampl");
    }
  
  bIsFirst = kTRUE;
  
  return kTRUE;
}

Int_t HPidTofRec::execute(void){
  // Execute function
  
  if (bIsFirst && bFillNTuple)
    {
      TString dstFile = gHades->getOutputFile()->GetName();
      if (sFile=="") sFile = sDir+"/pidtofrec_"+dstFile(dstFile.Last('/')+1,dstFile.Last('.')-dstFile.Last('/')-1)+".root";  
      else sFile = sDir + "/" + sFile;
      cout << " *** HPidTofRec: Ntuple output file:\t" << sFile << "\t ***" << endl;  
      bIsFirst = kFALSE;
    }
   
  Bool_t bIsRichRec[nMomAlgs], bIsMinusRec[nMomAlgs];
  Float_t fStartTime[nMomAlgs], fRecTof[nMomAlgs], fRecStart[nMomAlgs];
  Short_t nIndex[nMomAlgs], nRecSys[nMomAlgs],nRecPol[nMomAlgs], nRecSec[nMomAlgs], nRecIndex[nMomAlgs] ;
  Bool_t bIsRec = kFALSE;

  for (Short_t nAlg=0; nAlg<nMomAlgs; nAlg++)
    {
      bIsRichRec[nAlg]    = kFALSE;
      bIsMinusRec[nAlg]   = kFALSE;
      nIndex[nAlg] = 0;
      fStartTime[nAlg] = 0.;
      nRecSys[nAlg]   = -10;
      nRecPol[nAlg]   = 0;
      nRecSec[nAlg]   = -1;
      fRecTof[nAlg]   = 0.;
      fRecStart[nAlg] = 0.;
      nRecIndex[nAlg] = -1;
    }
  
  HPidTrackCandSim *pCand  = 0;
  
  iterTrackCand->Reset();
  while ((pCand = (HPidTrackCandSim *)iterTrackCand->Next()) != 0) // begin of PidTrackCand iterator
    {
      for (Int_t nAlg=0; nAlg<nMomAlgs; nAlg++)  // loop over tracking algorythms
	if (bTrackAlg[nAlg]) // selection of algorythm activated for reconstruction
	  {
	    nIndex[nAlg]++;
	    pCand->getTrackData()->nTofRecFlag[nAlg] = -1; // reset HPidTofRec flag
	    
	    if ((pCand->getTrackData()->bIsAccepted[nAlg])                                                // check on isXXXAccepted()
		&& !((bChiCutInner) && (pCand->getHitData()->fInnerMdcChiSquare==-1))                     //  check on chi for inner mdc segment
		&& !((bChiCutOuter) && (nAlg!=ALG_KICK) && (pCand->getHitData()->fOuterMdcChiSquare==-1)) //  check on chi for outer mdc segment
		&& !((nAlg==ALG_SPLINE) && (pCand->getTrackData()->fSplineChiSquare==-1))                 // skip spline tracks with Qspline==-1
		&& (pCand->getHitData()->iSystem!=-1)                                                     // skip tracks without META correlation
		&& !((pCand->getHitData()->iSystem==0) && (pCand->getHitData()->iTofinoMult!=1)))          // skip system==0 && TofinoMult!=1
	      {
		if ((bRichMode) && (pCand->getHitData()->hasRingCorrelation[nAlg])) {  // check of RICH correlation
		  
		  if (bFillNTuple) // fill RICH debug histos
		    {
		      Float_t richcorr_ntuple[17] = {
			pCand->getHitData()->fMdcTheta,pCand->getHitData()->fRichTheta,pCand->getHitData()->fMdcPhi,pCand->getHitData()->fRichPhi,pCand->getHitData()->nSector,
			pCand->getHitData()->iSystem,pCand->getTrackData()->nPolarity[nAlg],pCand->getTrackData()->fMomenta[nAlg],nAlg,pCand->getHitData()->fInnerMdcChiSquare,
			pCand->getHitData()->fOuterMdcChiSquare,pCand->getHitData()->fMdcZCoord,pCand->getHitData()->fMdcRCoord,pCand->getHitData()->nRingPatMat,pCand->getHitData()->nRingPadNr,
			pCand->getHitData()->fRingCentroid,pCand->getHitData()->nRingAmplitude};
		      richcorr->Fill(richcorr_ntuple);
		    }  // enf of RICH debug histos
		  
		  if ((pCand->getHitData()->nRingPatMat>fRichQparams[0])    &&  // quality parameters
		      (pCand->getHitData()->nRingPadNr>=fRichQparams[1])    &&
		      (pCand->getHitData()->fRingCentroid<fRichQparams[2])  &&
		      (pCand->getHitData()->nRingAmplitude>=fRichQparams[3])) 
		    {
		      if ((!bIsRichRec[nAlg])    // if there are no other rich correlated tracks
			  || (bIsRichRec[nAlg] && (nMdcSeg[pCand->getHitData()->nSector]>nMdcSeg[nRecSec[nAlg]])) // priority on sectors with more MDC chambers
			  || (bIsRichRec[nAlg] && (nMdcSeg[pCand->getHitData()->nSector]==nMdcSeg[nRecSec[nAlg]]) && (pCand->getHitData()->iSystem==1&&nRecSys[nAlg]==0)) //  or if new one TOF, old one TOFINO
			  || (bIsRichRec[nAlg] && (nMdcSeg[pCand->getHitData()->nSector]==nMdcSeg[nRecSec[nAlg]]) && (pCand->getHitData()->iSystem==nRecSys[nAlg]) && (pCand->getTrackData()->nPolarity[nAlg]==-1 && nRecPol[nAlg]==1))  // or if new one pol==1 and old  pol ==1
			  || (bIsRichRec[nAlg] && (nMdcSeg[pCand->getHitData()->nSector]==nMdcSeg[nRecSec[nAlg]]) && (pCand->getHitData()->iSystem==nRecSys[nAlg]) && (pCand->getTrackData()->nPolarity[nAlg]==nRecPol[nAlg]) && (pCand->getHitData()->getTof()<fRecTof[nAlg])))  // faster particle
			{ 
			  bIsRec = kTRUE;
			  bIsRichRec[nAlg] = kTRUE;
			  nRecIndex[nAlg] = nIndex[nAlg];
			  fRecTof[nAlg] = pCand->getHitData()->getTof();
			  nRecSys[nAlg] = pCand->getHitData()->iSystem;
			  nRecPol[nAlg] = pCand->getTrackData()->nPolarity[nAlg];
			  nRecSec[nAlg] = pCand->getHitData()->nSector;
			  fRecStart[nAlg] = pCand->getHitData()->getTof()*pCand->getTrackData()->fCorrectedBeta[nAlg]*TMath::C()*1e-6*sqrt(pCand->getTrackData()->fMomenta[nAlg]*pCand->getTrackData()->fMomenta[nAlg]+HPidPhysicsConstants::mass(3)*HPidPhysicsConstants::mass(3))/(TMath::C()*1e-6*pCand->getTrackData()->fMomenta[nAlg])-pCand->getHitData()->getTof();
			}
		    } // end of lepton recognition 
		} // end of rich loop
		
		if (bMinusMode && (!bIsRichRec[nAlg]) &&  (pCand->getTrackData()->nPolarity[nAlg]==-1))
		  {
		    if ((!bIsMinusRec[nAlg])    // if there are no other negative charged trackss
			|| (bIsMinusRec[nAlg] && (nMdcSeg[pCand->getHitData()->nSector]>nMdcSeg[nRecSec[nAlg]])) // priority on sectors with more MDC chambers
			|| (bIsMinusRec[nAlg] && (nMdcSeg[pCand->getHitData()->nSector]==nMdcSeg[nRecSec[nAlg]]) && (pCand->getHitData()->iSystem==1&&nRecSys[nAlg]==0)) //  or if new one TOF, old one TOFINO
			|| (bIsMinusRec[nAlg] && (nMdcSeg[pCand->getHitData()->nSector]==nMdcSeg[nRecSec[nAlg]]) && (pCand->getHitData()->iSystem==nRecSys[nAlg]) && (pCand->getHitData()->getTof()<fRecTof[nAlg])))  // faster particle
		      {
			bIsRec = kTRUE;
			bIsMinusRec[nAlg] = kTRUE;
			nRecIndex[nAlg] = nIndex[nAlg];
			fRecTof[nAlg] = pCand->getHitData()->getTof();
			nRecSys[nAlg] = pCand->getHitData()->iSystem;
			nRecPol[nAlg] = pCand->getTrackData()->nPolarity[nAlg];
			nRecSec[nAlg] = pCand->getHitData()->nSector;
			fRecStart[nAlg] = pCand->getHitData()->getTof()*pCand->getTrackData()->fCorrectedBeta[nAlg]*TMath::C()*1e-6*sqrt(pCand->getTrackData()->fMomenta[nAlg]*pCand->getTrackData()->fMomenta[nAlg]+HPidPhysicsConstants::mass(8)*HPidPhysicsConstants::mass(8))/(TMath::C()*1e-6*pCand->getTrackData()->fMomenta[nAlg])-pCand->getHitData()->getTof();
		      }  
		  } // end of pi- recognition
	      } //  end of "quality" check of the track
	  } // end of loop over different tracking
    } // end of HPidTrackCand iterator
  
  Short_t nRecAlg = 0;
  // time/beta/mass2 recalculation
  for (Short_t nAlg=0; nAlg<nMomAlgs; nAlg++)
    {
      nIndex[nAlg] = 0;
    }
  iterTrackCand->Reset();
  while ((pCand = (HPidTrackCandSim *)iterTrackCand->Next()) != 0) // begin of PidTrackCand iterator
    {
      for (Int_t nAlg=0; nAlg<nMomAlgs; nAlg++)  // loop over tracking algorythms
	if (bTrackAlg[nAlg]) // selection of algorythm activated for reconstruction, plus isXXXAccepted
	  {
	    nIndex[nAlg]++;
	    if ((pCand->getTrackData()->bIsAccepted[nAlg]) && (pCand->getHitData()->iSystem!=-1) && (bIsRec))  // check on isXXXAccepted()
	      {
		if (bIsCommon) nRecAlg = nCommonFlag;
		else nRecAlg = nAlg;
		if (bIsRichRec[nRecAlg]) pCand->getTrackData()->nTofRecFlag[nAlg] = 1;
		if ((!bIsRichRec[nRecAlg]) && (bIsMinusRec[nRecAlg])) pCand->getTrackData()->nTofRecFlag[nAlg] = 2;
		if (pCand->getTrackData()->nTofRecFlag[nAlg]>0) // if the start time was reconstructed
		  {
		    pCand->getTrackData()->fTofRecTof[nAlg] = pCand->getHitData()->getTof()+fRecStart[nRecAlg];
		    pCand->getTrackData()->fTofRecBeta[nAlg] = pCand->getHitData()->getTof()*pCand->getTrackData()->fCorrectedBeta[nAlg]/pCand->getTrackData()->fTofRecTof[nAlg];
		    pCand->getTrackData()->fTofRecMassSquared[nAlg] = pCand->getTrackData()->fMomenta[nAlg]*pCand->getTrackData()->fMomenta[nAlg]*(1./pCand->getTrackData()->fTofRecBeta[nAlg]/pCand->getTrackData()->fTofRecBeta[nAlg]-1);
		  }
		
		if (bFillNTuple)  // fill debug ntuples
		  {
		    if (bIsSimulation) 
		      {
			Float_t hit_ntuple[29] = {
			  pCand->getTrackData()->fMomenta[nAlg],pCand->getTrackData()->nPolarity[nAlg],pCand->getHitData()->nSector,pCand->getHitData()->iSystem,pCand->getHitData()->fMdcTheta,
			  pCand->getHitData()->fMdcPhi,pCand->getTrackData()->nTofRecFlag[nAlg],nAlg,pCand->getHitData()->getTof(),pCand->getTrackData()->fTofRecTof[nAlg],
			  pCand->getTrackData()->fCorrectedBeta[nAlg],pCand->getTrackData()->fTofRecBeta[nAlg],pCand->getTrackData()->fMassSquared[nAlg],pCand->getTrackData()->fTofRecMassSquared[nAlg],nRecSys[nAlg],
			  nRecPol[nAlg],nRecSec[nAlg],pCand->getHitData()->fInnerMdcChiSquare,pCand->getHitData()->fOuterMdcChiSquare,pCand->getHitData()->fMdcZCoord,
			  pCand->getHitData()->fMdcRCoord,pCand->getHitData()->iTofinoMult,(Float_t)(nRecIndex[nAlg]==nIndex[nAlg]),pCand->getHitData()->hasRingCorrelation[nAlg],pCand->getTrackData()->fSplineChiSquare,
			  pCand->getGeantTrackSet()->getCorrelationFlag(),pCand->getGeantTrackSet()->getTotalMomentum(),pCand->getGeantTrackSet()->getGeantPID(),pCand->getGeantTrackSet()->getGeantParentID()};
			hit->Fill(hit_ntuple);
		      }              // end of ntuple filling sim
		    else
		      {
			Float_t hit_ntuple[25] = {
			  pCand->getTrackData()->fMomenta[nAlg],pCand->getTrackData()->nPolarity[nAlg],pCand->getHitData()->nSector,pCand->getHitData()->iSystem,pCand->getHitData()->fMdcTheta,
			  pCand->getHitData()->fMdcPhi,pCand->getTrackData()->nTofRecFlag[nAlg],nAlg,pCand->getHitData()->getTof(),pCand->getTrackData()->fTofRecTof[nAlg],
			  pCand->getTrackData()->fCorrectedBeta[nAlg],pCand->getTrackData()->fTofRecBeta[nAlg],pCand->getTrackData()->fMassSquared[nAlg],pCand->getTrackData()->fTofRecMassSquared[nAlg],nRecSys[nAlg],
			  nRecPol[nAlg],nRecSec[nAlg],pCand->getHitData()->fInnerMdcChiSquare,pCand->getHitData()->fOuterMdcChiSquare,pCand->getHitData()->fMdcZCoord,
			  pCand->getHitData()->fMdcRCoord,pCand->getHitData()->iTofinoMult,(Float_t)(nRecIndex[nAlg]==nIndex[nAlg]),pCand->getHitData()->hasRingCorrelation[nAlg],pCand->getTrackData()->fSplineChiSquare};
			hit->Fill(hit_ntuple);
		      }              // end of ntuple filling  exp
		  }  // end of ntuple filling
	      } // cut on isXXXAccepted() sys!=-1 and tof recalcutation
	  }  // end of algorythm loop
    } // end of loop on HPidTrackCand
  
  if (!bIsRec && bSkip) return kSkipEvent;
  
  return 0;
}


Bool_t HPidTofRec::finalize(void){
  if(bFillNTuple)
    {
      TFile *r = TFile::Open(sFile,"RECREATE");
      hit  ->Write();
      richcorr->Write();
      
      r->Save();
      r->Close();
      r->Delete();
      
      hit->Delete();
      richcorr->Delete();
    }
  
  return kTRUE;
}
