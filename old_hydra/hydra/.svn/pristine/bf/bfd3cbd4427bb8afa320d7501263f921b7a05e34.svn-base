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
// If the start time reconstruction was done starting from a TOFINO track with multiplicity>1
// then the Flag will be 10 for lepton ID, 20 for pi- ID-
// Anyhow the original tof can be retrieved direct HPidHitData::getTof().
//
// To setup the constructor  call HPidTofRec(Text_t *name,Text_t *title, const Char_t *select, const Char_t *track),
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
//    nopidparam : do not get rich parameters from HPidAlgStandCutsPar
//    startrk    : catStartHit will be filled from RK
//    startspline: catStartHit will be filled from SPLINE
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
//   pidparam                    ON
//   startrk                     ON
//   startspline                 OFF
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
#include "hcategory.h"
#include "hmatrixcategory.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "TMath.h"
#include "hpidtrackcandsim.h"
#include "hpidgeanttrackset.h"
#include "hpidtrackdata.h"
#include "hpidhitdata.h"
#include "hpidphysicsconstants.h"
#include "hpidalgstandcutspar.h"
#include "hstarthit.h"
#include "hlocation.h"
#include "hlinearcategory.h"
#include "hstartdef.h"
#include "hgeantdef.h"
#include "showertofinodef.h"
#include "tofinodef.h"
#include "hshowerhittof.h"
#include "htofinocalsim.h"

#include <iostream>

ClassImp(HPidTofRec)

    HPidTofRec::HPidTofRec()
{
    // Default constructor

    richParam       =  NULL;
    pidalg          = -1111;
    fRichQparams[0] =  0.;
    fRichQparams[1] =  0.;
    fRichQparams[2] =  1000.;
    fRichQparams[3] =  0.;
    nCommonFlag     = -1;
    nStartAlg       =  ALG_SPLINE;
    for (Short_t nSs = 0; nSs < 6; nSs ++)
    {
	nMdcSeg[nSs] = 0;
	for (Short_t nMm = 0; nMm < 4; nMm ++) {
	    if (gHades->getSetup()->getDetector("Mdc")->getModule(nSs,nMm)) { nMdcSeg[nSs] ++; }
	}
    }

    hit      = 0;
    richcorr = 0;

    bRichMode        = kTRUE;
    bMinusMode       = kTRUE;
    bFillNTuple      = kFALSE;
    bChiCutInner     = kFALSE;
    bChiCutOuter     = kFALSE;
    bSkip            = kFALSE;
    bIsCommon        = kFALSE;
    bIsSimulation    = kFALSE;
    bPidParam        = kTRUE;
    bSetStartHit     = kFALSE;

    for (Int_t nAlg = 0; nAlg < MAX_MOM_ALGS; nAlg ++) {
	// switch on all the tracking algorythms
	bTrackAlg[nAlg] = kTRUE;
    }
    iterTrackCand = 0;
}

HPidTofRec::HPidTofRec(Text_t *name,Text_t *title, const Char_t *select, const Char_t *track)
:HReconstructor(name,title)
{
    // Constructor

    TString sel      = select;
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
    bPidParam        = kTRUE;
    bSetStartHit     = kFALSE;

    for (Int_t nAlg = 0; nAlg < MAX_MOM_ALGS; nAlg ++) {
	// switch on all the tracking algorythms
	bTrackAlg[nAlg] = kTRUE;
    }

    richParam       =  NULL;
    pidalg          = -1111;
    fRichQparams[0] =  0.;
    fRichQparams[1] =  0.;
    fRichQparams[2] =  1000.;
    fRichQparams[3] =  0.;

    nCommonFlag     = -1;
    nStartAlg       =  ALG_SPLINE;


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
    if (sel.Contains("nopidparam")) bPidParam        = kFALSE;

    if (sel.Contains("startrk"))    {
	nStartAlg        = ALG_RUNGEKUTTA;
	bSetStartHit     = kTRUE;
    }
    if (sel.Contains("startspline")) {
	nStartAlg        = ALG_SPLINE;
 	bSetStartHit     = kTRUE;
    }

    if (sel.Contains("common"))
    {
	if (tracking.Contains("spline"))  nCommonFlag = ALG_SPLINE;
	if (tracking.Contains("lowres"))  nCommonFlag = ALG_KICK;
	if (tracking.Contains("hires"))   nCommonFlag = ALG_KICK123;
	if (tracking.Contains("rk"))      nCommonFlag = ALG_RUNGEKUTTA;
	if (tracking.Contains("reft"))    nCommonFlag = ALG_REFT;
	if (nCommonFlag!=-1) { bIsCommon = kTRUE; }
	else Warning("init","HPidTofRec: No common tracking setted -> Switching Common Mode OFF");
    }

    for (Short_t nSs = 0; nSs < 6; nSs ++)
    {
	nMdcSeg[nSs] = 0;
	for (Short_t nMm = 0; nMm < 4; nMm ++) {
	    if (gHades->getSetup()->getDetector("Mdc")->getModule(nSs,nMm)) nMdcSeg[nSs] ++;
	}
    }

    hit      = 0;
    richcorr = 0;
    sDir     = "./";
    sFile    = "";
    iterTrackCand = 0;
}

HPidTofRec::~HPidTofRec(void){
    // Destructor.

    if( iterTrackCand ) {
	delete  iterTrackCand;
	iterTrackCand = 0;
    }
}

Bool_t HPidTofRec::init(void){
    Info("init()", "PID Tof Reconstructor default initialization *** ");

   

    if(gHades->getCurrentEvent()->getCategory(catGeantKine) &&
       gHades->getEmbeddingMode() > 0)
    {
	isEmbedding = kTRUE;
    } else {
	isEmbedding = kFALSE;
    }
    pShowerHitTofCat = 0;
    pTofinoCalCat    = 0;

    if(isEmbedding){
	pShowerHitTofCat = (HCategory*)(gHades->getCurrentEvent()->getCategory(catShowerHitTofTrack));
	if(!pShowerHitTofCat)  Warning("init","No HShowerHitTof Input -> Switching usage of this category OFF");

	pTofinoCalCat = (HMatrixCategory*)(gHades->getCurrentEvent()->getCategory(catTofinoCal));
	if(!pTofinoCalCat)  Warning("init","No HTofinoCal Input -> Switching usage of this category OFF");
    }

    pTrackCand = gHades->getCurrentEvent()->getCategory(catPidTrackCand);
    if (!pTrackCand) {
	Error("init","HPidTofRec: No HPIDTrackCand Input -> Switching HPidTofRec OFF");
    }
    else { iterTrackCand= (HIterator *)pTrackCand->MakeIterator(); }

    if(bSetStartHit)
    {
	pStartHitCat = (HLinearCategory *)(gHades->getCurrentEvent()->getCategory(catStartHit));
	if (!pStartHitCat)
	{
	    pStartHitCat = new HLinearCategory("HStartHit");
	    if (!pStartHitCat) {
		Error("init()","Could not create catStartHit!");
	    return kFALSE;
	    }
	    else {
		gHades->getCurrentEvent()->addCategory(catStartHit,pStartHitCat,"Start");
	    }
	}
    }
    cout<<"Spline: "      << bTrackAlg[ALG_SPLINE]
	<<" Kick lowres: "<< bTrackAlg[ALG_KICK]
	<<" Kick hires: " << bTrackAlg[ALG_KICK123]
	<<" RungeKutta: " << bTrackAlg[ALG_RUNGEKUTTA]
	<<" RefTraj: "    << bTrackAlg[ALG_REFT]
	<<endl;
    cout<<" Rich Mode: "        << bRichMode
	<<" Minus Mode: "       << bMinusMode
	<<" ChiCutMode (INNER) "<< bChiCutInner
	<<" ChiCutMode (OUTER) "<< bChiCutOuter
	<<" Ntuple Mode: "      << bFillNTuple
	<<endl;

    cout<<"Skip Event: "        << bSkip
	<<" Common Mode: "      << bIsCommon
	<<" Simulation Mode: "  << bIsSimulation
	<<" StartHit alg : "    << nStartAlg
        <<" set StartHit  : "   << bSetStartHit
	<<endl;

    cout << " Mdc Sectors:\t\t";
    for (Int_t nSs = 0; nSs < 6; nSs ++) { cout << nMdcSeg[nSs]  << " "; }
    cout << endl;

    if (bRichMode)
    {
	if (bPidParam)
	{
	    if((richParam = (HPidAlgStandCutsPar *)gHades->getRuntimeDb()
		->getContainer(PIDALGSTANDCUTSPAR_NAME)) == NULL)
	    {
		Error("HPidTofRec::init()", "Cannot get parameters: %s", PIDALGSTANDCUTSPAR_NAME);
		return kFALSE;
	    }
	    if      (bTrackAlg[ALG_RUNGEKUTTA]) { pidalg = ALG_RUNGEKUTTA; }
	    else if (bTrackAlg[ALG_SPLINE])     { pidalg = ALG_SPLINE;     }
	    else if (bTrackAlg[ALG_REFT])       { pidalg = ALG_REFT;       }
	    else if (bTrackAlg[ALG_KICK123])    { pidalg = ALG_KICK123;    }
	    else if (bTrackAlg[ALG_KICK])       { pidalg = ALG_KICK;       }

	    if (-1111 == pidalg) {
		Error("HPidTofRec::init()","no valid momentum algorithm activated");
		return kFALSE;
	    }
	    richParam->setContext(pidalg);
	} else { // ! bPidParam
	    cout<<"Rich Quality parameters: RingPatMat "<< fRichQparams[0]
		<<" RingPadNr "                         << fRichQparams[1]
		<<" Centroid "                          << fRichQparams[2]
		<<" RingAverageCharge "                 << fRichQparams[3]
		<<endl;
	} // ! bPidParam
    }

    if (bFillNTuple)
    {
	if(bIsSimulation) {
	    hit     = new TNtuple("hit"   ,"hit"      ,
				  "P:Pol:Sec:Sys:Theta:Phi:Flag:Alg:"
				  "TofOld:TofNew:BetaOld:BetaNew:Mass2Old:Mass2New:"
				  "RecSys:RecPol:RecSec:Chi2In:Chi2Out:Z:R:TofinoMult:"
				  "Id:Rich:QSpline:GeantFlag:GeantP:GeantPID:GeantParent");
	}else {
	    hit    = new TNtuple("hit"   ,"hit"      ,
				 "P:Pol:Sec:Sys:Theta:Phi:Flag:Alg:"
				 "TofOld:TofNew:BetaOld:BetaNew:Mass2Old:Mass2New:"
				 "RecSys:RecPol:RecSec:Chi2In:Chi2Out:Z:R:TofinoMult:"
				 "Id:Rich:QSpline");
	}
	richcorr = new TNtuple("richcorr","rich corr",
			       "MdcTheta:RichTheta:MdcPhi:RichPhi:Sec:Sys:Pol:P:Alg:"
			       "Chi2In:Chi2Out:Z:R:PatMat:PadNr:Centr:Ampl");
    }

    bIsFirst = kTRUE;
    
    return kTRUE;
}

Bool_t HPidTofRec::reinit(void)
{
    //Check if parameter context corresponds to the
    //appropriate momentum algorithm index
    //If not -> return kFALSE (analysis can not be done in such case)
    //and print error message
    if (bPidParam) {
	return richParam->checkContext(pidalg);
    } else {
	return kTRUE;
    }
}

Int_t HPidTofRec::execute(void){
    // Execute function

    //-------------------------------------------------------------------------
    // create the ntuples in the first event loop
    if (bIsFirst && bFillNTuple)
    {
	TString dstFile = gHades->getOutputFile()->GetName();
	if (sFile == "") {
	    sFile = sDir + "/pidtofrec_" + dstFile(dstFile.Last('/') + 1,dstFile.Last('.') - dstFile.Last('/') - 1) + ".root";
	}else {
	    sFile = sDir + "/" + sFile;
	}
	cout << " *** HPidTofRec: Ntuple output file:\t" << sFile << "\t ***" << endl;
	bIsFirst = kFALSE;
    }
    //-------------------------------------------------------------------------

    //-------------------------------------------------------------------------
    // create helper arrays and init them
    Bool_t  bIsRichRec [2][MAX_MOM_ALGS];
    Bool_t  bIsMinusRec[2][MAX_MOM_ALGS];
    Float_t fStartTime [2][MAX_MOM_ALGS];
    Float_t fRecTof    [2][MAX_MOM_ALGS];
    Short_t nRecTofMult[2][MAX_MOM_ALGS];
    Float_t fRecStart  [2][MAX_MOM_ALGS];
    Short_t nIndex     [2][MAX_MOM_ALGS];
    Short_t nRecSys    [2][MAX_MOM_ALGS];
    Short_t nRecPol    [2][MAX_MOM_ALGS];
    Short_t nRecSec    [2][MAX_MOM_ALGS];
    Short_t nRecIndex  [2][MAX_MOM_ALGS];
    Bool_t  bIsRec     [2];

    //-------------------------------------------------------------------------

    //-------------------------------------------------------------------------
    // find a good particle to recalulate the start time.
    // In embedding mode this loop runs twice, once for real
    // and once for tracks with sim TOF measurements. The
    // start time correction will be done for sim /real tracks
    // separately. This is not fully consistent, but the only way
    // to mimic somehow the resolution effect for the simulated
    // tracks.

    Bool_t bIsRealTrack    = kTRUE;

    for(Int_t n = 0; n < 2; n ++)
    {
	bIsRec[n] = kFALSE;
	for(Int_t nAlg = 0; nAlg < MAX_MOM_ALGS; nAlg ++)
	{
	    bIsRichRec [n][nAlg] =  kFALSE;
	    bIsMinusRec[n][nAlg] =  kFALSE;
	    nIndex     [n][nAlg] =  0;
	    fStartTime [n][nAlg] =  0.;
	    nRecSys    [n][nAlg] = -10;
	    nRecPol    [n][nAlg] =  0;
	    nRecSec    [n][nAlg] = -1;
	    fRecTof    [n][nAlg] =  0.;
	    nRecTofMult[n][nAlg] = -1;
	    fRecStart  [n][nAlg] =  0.;
	    nRecIndex  [n][nAlg] = -1;
	}
    }
    HPidTrackCandSim *pCand = 0;

    Int_t nMax = 1;
    if(isEmbedding) nMax = 2;

    // loop  n==0 -> real tracks , n == 1 -> sim tracks
    for(Int_t n = 0; n < nMax; n ++ )
    {
	iterTrackCand->Reset();
	while ((pCand = (HPidTrackCandSim *)iterTrackCand->Next()) != 0) // begin of PidTrackCand iterator
	{
	    //-------------------------------------------------------------------------
	    // in embedding mode we have to take care if we
	    // use real or embedded tracks for the recalibration
	    // of the start time.
	    if(isEmbedding) {
		bIsRealTrack = isRealTrack(pCand);
		if(n == 0) { // real tracks
		    if(!bIsRealTrack){ continue; }
		} else {     // sim tracks
		    if( bIsRealTrack){ continue; }
		}
	    }
	    //-------------------------------------------------------------------------

	    for (Int_t nAlg = 0; nAlg < MAX_MOM_ALGS; nAlg ++)           // loop over tracking algorythms
	    {
		if (bTrackAlg[nAlg])                                    // selection of algorythm activated for reconstruction
		{
		    nIndex[n][nAlg] ++;
		    pCand->getTrackData()->nTofRecFlag[nAlg] = -1;      // reset HPidTofRec flag

		    if ((pCand->getTrackData()->bIsAccepted[nAlg])                 // check on isXXXAccepted()
			&& !((bChiCutInner)                                    &&  // check on chi for inner mdc segment
			     (pCand->getHitData()->fInnerMdcChiSquare == -1)
			    )
			&& !((bChiCutOuter)                                    &&  // check on chi for outer mdc segment
			     (nAlg != ALG_KICK)                                &&
			     (pCand->getHitData()->fOuterMdcChiSquare == -1)
			    )
			&& !((nAlg == ALG_SPLINE)                              &&  // skip spline tracks with Qspline==-1
			     (pCand->getTrackData()->fSplineChiSquare == -1)
			    )
			&& (  pCand->getHitData()->iSystem            != -1)       // skip tracks without META correlation
			/* // commented line, to use TofinoMult>1
			  && !((pCand->getHitData()->iSystem            ==  0)   &&  // skip system==0 && TofinoMult!=1
			  (pCand->getHitData()->iTofinoMult        !=  1)
			)
			*/
		      )
		    {
		        Short_t tofMult =-1;
			if (pCand->getHitData()->iSystem==0) tofMult = pCand->getHitData()->getTofinoMult();
			else tofMult = 1;
			if ((bRichMode) && (pCand->getHitData()->hasRingCorrelation[nAlg]))
			{  // check of RICH correlation

			    if (bFillNTuple) // fill RICH debug histos
			    {
				Float_t richcorr_ntuple[17] = {
				    pCand->getHitData()  ->fMdcTheta,
				    pCand->getHitData()  ->fRichTheta,
				    pCand->getHitData()  ->fMdcPhi,
				    pCand->getHitData()  ->fRichPhi,
				    pCand->getHitData()  ->nSector,
				    pCand->getHitData()  ->iSystem,
				    pCand->getTrackData()->nPolarity[nAlg],
				    pCand->getTrackData()->fMomenta[nAlg],
				    nAlg,
				    pCand->getHitData()  ->fInnerMdcChiSquare,
				    pCand->getHitData()  ->fOuterMdcChiSquare,
				    pCand->getHitData()  ->fMdcZCoord,
				    pCand->getHitData()  ->fMdcRCoord,
				    pCand->getHitData()  ->nRingPatMat,
				    pCand->getHitData()  ->nRingPadNr,
				    pCand->getHitData()  ->fRingCentroid,
				    pCand->getHitData()  ->nRingAmplitude
				};
				richcorr->Fill(richcorr_ntuple);
			    }  // enf of RICH debug histos

			    Bool_t bGoodRing = kFALSE;
			    if (bPidParam)
			    {
				Int_t mdc_sec = pCand->getHitData()->nSector;
				if (// positrons
				    ( (pCand->getHitData()->nRingPatMat   >  richParam->getValue(2, 0, mdc_sec, 0)) &&
				      (pCand->getHitData()->fRingCentroid >= richParam->getValue(2, 0, mdc_sec, 1)) &&
				      (pCand->getHitData()->nRingPadNr    >= richParam->getValue(2, 0, mdc_sec, 2)) &&
				      (pCand->getHitData()->nRingPatMat / pCand->getHitData()->nRingPadNr >= richParam->getValue(2, 0, mdc_sec, 3))
				    )
#ifdef THIS_IS_BEAUTIFUL_BUT_OBSOLETE // positrons and electrons have same thr
				    || // electrons
				    ( (pCand->getHitData()->nRingPatMat    >  richParam->getValue(3, 0, mdc_sec, 0)) &&
				      (pCand->getHitData()->fRingCentroid  >= richParam->getValue(3, 0, mdc_sec, 1)) &&
				      (pCand->getHitData()->nRingPadNr     >= richParam->getValue(3, 0, mdc_sec, 2)) &&
				      (pCand->getHitData()->nRingPatMat / pCand->getHitData()->nRingPadNr >= richParam->getValue(3, 0, mdc_sec, 3))
				    )
#endif // THIS_IS_BEAUTIFUL_BUT_OBSOLETE
				   ) {
				    bGoodRing = kTRUE;
				}
			    } else { // ! PidParam
				if ( (pCand->getHitData()->nRingPatMat   >  fRichQparams[0])    &&  // quality parameters
				     (pCand->getHitData()->nRingPadNr    >= fRichQparams[1])    &&
				     (pCand->getHitData()->fRingCentroid <  fRichQparams[2])    &&
				    ((pCand->getHitData()->nRingAmplitude / pCand->getHitData()->nRingPadNr) >= fRichQparams[3])
				   ) {
				    bGoodRing = kTRUE;
				}
			    } // ! PidParam
			    if (bGoodRing) {
				if ( (!bIsRichRec[n][nAlg])                                                           // if there are no other rich correlated tracks
				    || ( bIsRichRec[n][nAlg]                                                     &&   // or if new one has lower pad multiplicity
					 (tofMult                                   <  nRecTofMult[n][nAlg]     )      // priority on tracks with lower pad multiplicity
				       )	     
				    || ( bIsRichRec[n][nAlg]                                                     &&   // priority on sectors with more MDC chambers
					(tofMult                                   ==  nRecTofMult[n][nAlg]    ) &&
					(nMdcSeg[pCand->getHitData()->nSector]      > nMdcSeg[nRecSec[n][nAlg]])
				       )
				    || ( bIsRichRec[n][nAlg]                                                     &&   //  or if new one TOF, old one TOFINO
					(tofMult                                   ==  nRecTofMult[n][nAlg]    ) &&
					(nMdcSeg[pCand->getHitData()->nSector]     == nMdcSeg[nRecSec[n][nAlg]]) &&
					(pCand->getHitData()->iSystem              == 1                          &&
					 nRecSys[n][nAlg]                          == 0
					)
				       )
				    || ( bIsRichRec[n][nAlg]                                                     &&   // or if new one pol==1 and old  pol ==1
                                        (tofMult                                   ==  nRecTofMult[n][nAlg]    ) &&
					(nMdcSeg[pCand->getHitData()->nSector]     == nMdcSeg[nRecSec[n][nAlg]]) &&
					(pCand->getHitData()->iSystem              == nRecSys[n][nAlg]         ) &&
					(pCand->getTrackData()->nPolarity[nAlg]    == -1                         &&
					 nRecPol[n][nAlg]                          ==  1
					)
				       )
				    || ( bIsRichRec[n][nAlg]                                                     &&   // faster particle
					(tofMult                                   ==  nRecTofMult[n][nAlg]    ) &&
					(nMdcSeg[pCand->getHitData()->nSector]     == nMdcSeg[nRecSec[n][nAlg]]) &&
					(pCand->getHitData()->iSystem              == nRecSys[n][nAlg])          &&
					(pCand->getTrackData()->nPolarity[nAlg]    == nRecPol[n][nAlg])          &&
					(pCand->getHitData()->getTof()              < fRecTof[n][nAlg])
				       )
				   )
				{
				    bIsRec     [n]       = kTRUE;
				    bIsRichRec [n][nAlg] = kTRUE;
				    nRecIndex  [n][nAlg] = nIndex[n][nAlg];
				    fRecTof    [n][nAlg] = pCand->getHitData()->getTof();
				    nRecSys    [n][nAlg] = pCand->getHitData()->iSystem;
				    nRecTofMult[n][nAlg] = tofMult;
				    nRecPol    [n][nAlg] = pCand->getTrackData()->nPolarity[nAlg];
				    nRecSec    [n][nAlg] = pCand->getHitData()->nSector;

				    fRecStart [n][nAlg] = pCand->getHitData()->getTof()
					* pCand->getTrackData()->fCorrectedBeta[nAlg]
					* TMath::C() * 1e-6
					* sqrt(pCand->getTrackData()->fMomenta[nAlg]
					       * pCand->getTrackData()->fMomenta[nAlg]
					       + HPidPhysicsConstants::mass(3)
					       * HPidPhysicsConstants::mass(3))
					/ (TMath::C()
					   * 1e-6
					   * pCand->getTrackData()->fMomenta[nAlg])
					- pCand->getHitData()->getTof();
				}
			    } // end of lepton recognition
			} // end of rich loop

			if (bMinusMode && (!bIsRichRec[n][nAlg]) &&  (pCand->getTrackData()->nPolarity[nAlg] == -1))
			{
			    if ( (  !bIsMinusRec[n][nAlg])                                                      // if there are no other negative charged tracks
				|| ( bIsRichRec[n][nAlg]                                                     &&   // or if new one has lower pad multiplicity
				    (tofMult                                   <  nRecTofMult[n][nAlg]     )      // priority on tracks with lower pad multiplicity
				  )
				|| ( bIsMinusRec[n][nAlg]                                                    && // priority on sectors with more MDC chambers
				    (tofMult                                   ==  nRecTofMult[n][nAlg]    ) &&
				    (nMdcSeg[pCand->getHitData()->nSector]     > nMdcSeg[nRecSec[n][nAlg]]))
				|| ( bIsMinusRec[n][nAlg]                                                    && //  or if new one TOF, old one TOFINO
				    (tofMult                                   ==  nRecTofMult[n][nAlg]    ) &&
				    (nMdcSeg[pCand->getHitData()->nSector]    == nMdcSeg[nRecSec[n][nAlg]])  &&
				    (pCand->getHitData()->iSystem             == 1                           &&
				     nRecSys[n][nAlg]                         == 0)
				   )
				|| (bIsMinusRec[n][nAlg]                                                     && // faster particle
				    (tofMult                                   ==  nRecTofMult[n][nAlg]    ) &&
				    (nMdcSeg[pCand->getHitData()->nSector]    == nMdcSeg[nRecSec[n][nAlg]])  &&
				    (pCand->getHitData()->iSystem             == nRecSys[n][nAlg])           &&
				    (pCand->getHitData()->getTof()            <  fRecTof[n][nAlg]))
			       )
			    {
				bIsRec     [n]       = kTRUE;
				bIsMinusRec[n][nAlg] = kTRUE;
				nRecIndex  [n][nAlg] = nIndex[n][nAlg];
				fRecTof    [n][nAlg] = pCand->getHitData()->getTof();
				nRecSys    [n][nAlg] = pCand->getHitData()->iSystem;
				nRecTofMult[n][nAlg] = tofMult;
				nRecPol    [n][nAlg] = pCand->getTrackData()->nPolarity[nAlg];
				nRecSec    [n][nAlg] = pCand->getHitData()->nSector;
				fRecStart  [n][nAlg] = pCand->getHitData()->getTof()
				    * pCand->getTrackData()->fCorrectedBeta[nAlg]
				    * TMath::C()
				    * 1e-6
				    * sqrt(pCand->getTrackData()->fMomenta[nAlg]
					   * pCand->getTrackData()->fMomenta[nAlg]
					   + HPidPhysicsConstants::mass(8)
					   * HPidPhysicsConstants::mass(8))
				    / (TMath::C()
				       * 1e-6
				       * pCand->getTrackData()->fMomenta[nAlg])
				    - pCand->getHitData()->getTof();
			    }
			} // end of pi- recognition
		    } //  end of "quality" check of the track
		} // end of loop over different tracking
	    }
	} // end of HPidTrackCand iterator
    }
    //-------------------------------------------------------------------------


    //-------------------------------------------------------------------------
    Short_t nRecAlg = 0;

    // loop  n==0 -> real tracks , n == 1 -> sim tracks
    for(Int_t n = 0; n < nMax; n ++)
    {
	// time/beta/mass2 recalculation
	for (Int_t nAlg = 0; nAlg < MAX_MOM_ALGS; nAlg ++)
	{
	    nIndex[n][nAlg] = 0;
	}
	iterTrackCand->Reset();
	while ((pCand = (HPidTrackCandSim *)iterTrackCand->Next()) != 0) // begin of PidTrackCand iterator
	{
	    //-------------------------------------------------------------------------
	    // in embedding mode we have to take care if we
	    // use real or embedded tracks for the recalibration
	    // of the start time.
	    if(isEmbedding) {
		bIsRealTrack = isRealTrack(pCand);
		if(n == 0) { // real tracks
		    if(!bIsRealTrack){ continue; }
		} else {     // sim tracks
		    if( bIsRealTrack){ continue; }
		}
	    }
	    //-------------------------------------------------------------------------

	    for (Int_t nAlg = 0; nAlg < MAX_MOM_ALGS; nAlg ++)           // loop over tracking algorythms
	    {
		if (bTrackAlg[nAlg])                                     // selection of algorythm activated for reconstruction, plus isXXXAccepted
		{
		    nIndex[n][nAlg] ++;
		    if ((pCand->getTrackData()->bIsAccepted[nAlg])  &&   // check on isXXXAccepted()
			(pCand->getHitData()->iSystem != -1)        &&
			(bIsRec[n])
		       )
		    {
			if (bIsCommon) nRecAlg = nCommonFlag;
			else           nRecAlg = nAlg;
			if (nRecTofMult[n][nRecAlg] ==1)
			  {
			    if (  bIsRichRec[n][nRecAlg])                               pCand->getTrackData()->nTofRecFlag[nAlg] = 1;
			    if ((!bIsRichRec[n][nRecAlg]) && (bIsMinusRec[n][nRecAlg])) pCand->getTrackData()->nTofRecFlag[nAlg] = 2;
			  }
			else if (nRecTofMult[n][nRecAlg] >1)
			  {
			    if (  bIsRichRec[n][nRecAlg])                               pCand->getTrackData()->nTofRecFlag[nAlg] = 10;
			    if ((!bIsRichRec[n][nRecAlg]) && (bIsMinusRec[n][nRecAlg])) pCand->getTrackData()->nTofRecFlag[nAlg] = 20;
			  }
			
			if (pCand->getTrackData()->nTofRecFlag[nAlg] > 0) // if the start time was reconstructed
			{
			    pCand->getTrackData()->fTofRecTof[nAlg]         = pCand->getHitData()->getTof() + fRecStart[n][nRecAlg];
			    pCand->getTrackData()->fTofRecBeta[nAlg]        = pCand->getHitData()->getTof() * pCand->getTrackData()->fCorrectedBeta[nAlg] / pCand->getTrackData()->fTofRecTof[nAlg];
			    pCand->getTrackData()->fTofRecMassSquared[nAlg] = pCand->getTrackData()->fMomenta[nAlg] * pCand->getTrackData()->fMomenta[nAlg] * (1. / pCand->getTrackData()->fTofRecBeta[nAlg] / pCand->getTrackData()->fTofRecBeta[nAlg] - 1);
			}

			if (bFillNTuple)  // fill debug ntuples
			{
			    if (bIsSimulation)
			    {
				Float_t hit_ntuple[29] = {
				    pCand->getTrackData()->fMomenta[nAlg],
				    pCand->getTrackData()->nPolarity[nAlg],
				    pCand->getHitData()->nSector,
				    pCand->getHitData()->iSystem,
				    pCand->getHitData()->fMdcTheta,
				    pCand->getHitData()->fMdcPhi,
				    pCand->getTrackData()->nTofRecFlag[nAlg],
				    nAlg,
				    pCand->getHitData()->getTof(),
				    pCand->getTrackData()->fTofRecTof[nAlg],
				    pCand->getTrackData()->fCorrectedBeta[nAlg],
				    pCand->getTrackData()->fTofRecBeta[nAlg],
				    pCand->getTrackData()->fMassSquared[nAlg],
				    pCand->getTrackData()->fTofRecMassSquared[nAlg],
				    nRecSys[n][nAlg],
				    nRecPol[n][nAlg],
				    nRecSec[n][nAlg],
				    pCand->getHitData()->fInnerMdcChiSquare,
				    pCand->getHitData()->fOuterMdcChiSquare,
				    pCand->getHitData()->fMdcZCoord,
				    pCand->getHitData()->fMdcRCoord,
				    pCand->getHitData()->iTofinoMult,
				    (Float_t)(nRecIndex[n][nAlg] == nIndex[n][nAlg]),
				    pCand->getHitData()->hasRingCorrelation[nAlg],
				    pCand->getTrackData()->fSplineChiSquare,
				    pCand->getGeantTrackSet()->getCorrelationFlag(),
				    pCand->getGeantTrackSet()->getTotalMomentum(),
				    pCand->getGeantTrackSet()->getGeantPID(),
				    pCand->getGeantTrackSet()->getGeantParentID()
				};
				hit->Fill(hit_ntuple);
			    }  // end of ntuple filling sim
			    else
			    {
				Float_t hit_ntuple[25] = {
				    pCand->getTrackData()->fMomenta[nAlg],
				    pCand->getTrackData()->nPolarity[nAlg],
				    pCand->getHitData()->nSector,
				    pCand->getHitData()->iSystem,
				    pCand->getHitData()->fMdcTheta,
				    pCand->getHitData()->fMdcPhi,
				    pCand->getTrackData()->nTofRecFlag[nAlg],
				    nAlg,
				    pCand->getHitData()->getTof(),
				    pCand->getTrackData()->fTofRecTof[nAlg],
				    pCand->getTrackData()->fCorrectedBeta[nAlg],
				    pCand->getTrackData()->fTofRecBeta[nAlg],
				    pCand->getTrackData()->fMassSquared[nAlg],
				    pCand->getTrackData()->fTofRecMassSquared[nAlg],
				    nRecSys[n][nAlg],
				    nRecPol[n][nAlg],
				    nRecSec[n][nAlg],
				    pCand->getHitData()->fInnerMdcChiSquare,
				    pCand->getHitData()->fOuterMdcChiSquare,
				    pCand->getHitData()->fMdcZCoord,
				    pCand->getHitData()->fMdcRCoord,
				    pCand->getHitData()->iTofinoMult,
				    (Float_t)(nRecIndex[n][nAlg] == nIndex[n][nAlg]),
				    pCand->getHitData()->hasRingCorrelation[nAlg],
				    pCand->getTrackData()->fSplineChiSquare
				};
				hit->Fill(hit_ntuple);
			    }  // end of ntuple filling  exp
			}  // end of ntuple filling
		    } // cut on isXXXAccepted() sys!=-1 and tof recalcutation
		}  // end of algorythm loop
	    }
	}// end of loop on HPidTrackCand
    }
    //-------------------------------------------------------------------------

    //-------------------------------------------------------------------------
    // skip non recalibrated events if wanted
    if ( (!bIsRec[0] && !bIsRec[1]) && bSkip) { return kSkipEvent; }
    //-------------------------------------------------------------------------

    //-------------------------------------------------------------------------
    // fill the recalibrated start time if wanted
    if(bSetStartHit)
    {
	
	HLocation loc;
	loc.set(2,0,0);

	HStartHit* pHit = (HStartHit*)pStartHitCat->getObject(loc);
	if(!pHit){
	    pHit = (HStartHit*)pStartHitCat->getNewSlot(loc);
	}
	if(pHit){
	    pHit = new (pHit) HStartHit;
	    pHit->setModule(0);
	    pHit->setStrip(0);
	    if(bIsRec[0]){
		pHit->setTime(-fRecStart[0][nStartAlg]);
	    } else {
		pHit->setTime(0);
	    }
	    pHit->setStartMult(1);
	    pHit->setFlag(kTRUE);
	} else {
	    Error("execute()","NULL pointer for HStartHit slot retrieved!");
	}
    }
    //-------------------------------------------------------------------------

    return 0;
}


Bool_t HPidTofRec::finalize(void){
    if(bFillNTuple)
    {
	TFile *r = TFile::Open(sFile,"RECREATE");
	hit     ->Write();
	richcorr->Write();

	r->Save();
	r->Close();
	r->Delete();

	hit     ->Delete();
	richcorr->Delete();
    }

    return kTRUE;
}

Bool_t HPidTofRec::isRealTrack(HPidTrackCand* pCand)
{
    // checks by the TOF measurement of the
    // HPidTrackCand object is related to a real track
    // or embedded track. Return kTRUE if the track
    // is real.

    //-------------------------------------------------------------------------
    if(isEmbedding)
    {
	HPidTrackCandSim* pCandSim        = (HPidTrackCandSim*) pCand;
	HPidGeantTrackSet* pGeantTrackSet = pCandSim->getGeantTrackSet();

	if(pCand->getHitData()->iSystem == 1)
	{ // TOF hit
	    for(UInt_t i = 0; i < pGeantTrackSet->getNTOFTracks() ; i ++){
		if(pGeantTrackSet->getTOFTrack(i) > 0) { return kFALSE; }
	    }
	}

	if(pCand->getHitData()->iSystem == 0)
	{
	    //-------------------------------------------------------------------------
	    // for the TOFINO we have to do some effort
	    // because the TOFINO track number is not used by
	    // HPidTrackCand

	    if(pShowerHitTofCat && pTofinoCalCat)
	    {
		// TOFINO hit
		HShowerHitTof* pShowerhittof = (HShowerHitTof*)pShowerHitTofCat->getObject(pCand->getHitData()->getIndShower());
		if(pShowerhittof)
		{
		    HLocation loc;
		    loc.set(3,pShowerhittof->getSector(),0,pShowerhittof->getTofinoCell());

		    HTofinoCalSim* ptofinocalsim = 0;
		    if(loc[1] >= 0) {
			ptofinocalsim = (HTofinoCalSim*) pTofinoCalCat->getObject(loc);

			if(ptofinocalsim) {
			    if(ptofinocalsim->getTrack(0) > 0) { return kFALSE; }
			} else {
			    Error("execute()","NULL pointer received for TofinoCal object!");
			}
		    }
		} else {
		    Error("execute()","NULL pointer received for ShowerHitTof object with index = %i!",pCand->getHitData()->getIndShower());
		}
	    }
	}
	//-------------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------
    return kTRUE;
}

