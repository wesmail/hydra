// File: hrichcorrelator.cc
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 01/10/25 15:32:50
//
///////////////////////////////////////////////////////////
// HRichCorrelator
//
// - this class matches hits in RICH, MDC and META
//   
// - it fills the category HHitMatch
//   cuts are applied in the isCorrelated() funcs
// 
///////////////////////////////////////////////////////////

#include "hrichcorrelator.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hmatrixcatiter.h"
#include "hrichhit.h"
#include "hmdcseg.h"
#include "htofhit.h"
#include "hshowerhittof.h"
#include "tofdef.h"
#include "hmdcdef.h"
#include "showertofinodef.h"
#include "showerdef.h"
#include "hdebug.h"
#include "hades.h"
#include "richdef.h"
#include "hhitmatch.h"
#include "hhitmatchheader.h"
#include "hlinearcategory.h"
#include "hgeomvector2.h"
#include "hrichutilfunc.h"
#include "hrichcorrelatorpar.h"
#include <fstream>

ClassImp(HRichCorrelator)

HRichCorrelator::HRichCorrelator(Text_t *name,Text_t *title) :
  HReconstructor(name,title)
{
     
}
HRichCorrelator::HRichCorrelator(Text_t *name,Text_t *title, char* filename) :
  HReconstructor(name,title)
{
    pFileName  = filename;// output filename for diagnostic histos
}
HRichCorrelator::HRichCorrelator()
{

}


HRichCorrelator::~HRichCorrelator(void) 
{

}

Bool_t HRichCorrelator::init() {
    //cout<<"we are in init() now !"<<endl;
    if (gHades) {
	HEvent *event=gHades->getCurrentEvent();
	HRuntimeDb *rtdb=gHades->getRuntimeDb();
	HSpectrometer *spec=gHades->getSetup();
	if (event && rtdb) {
	    HDetector *tof=spec->getDetector("Tof");
	    HDetector *shower = spec->getDetector("Shower");
	    HDetector *mdc = spec->getDetector("Mdc");
	    HDetector *rich = spec->getDetector("Rich");
	    // param container for the RTDB
	    HRichCorrelatorPar *pCorrPar =
	       (HRichCorrelatorPar*)rtdb->getContainer("RichCorrelatorParameters");
	    if (!pCorrPar)
	    {
		pCorrPar = new HRichCorrelatorPar;
		rtdb->addContainer(pCorrPar);
	    }
	    setCorrelationPar(pCorrPar);
	    if (!pCorrPar) return kFALSE;
	    // data containers
	    if (rich) { // RICH
		// RICH HIT container
		fRichPID=gHades->getCurrentEvent()->getCategory(catRichHit);
		if (!fRichPID) {
		    fRichPID=rich->buildCategory(catRichHit);
		    
		    if (!fRichPID) return kFALSE;
		    else gHades->getCurrentEvent()
			     ->addCategory(catRichHit, fRichPID, "Rich");
		}
    
		fRichIter = (HIterator*) fRichPID->MakeIterator();


		// Setup output category
		pHitMatchCat=event->getCategory(catMatchHit);
		if (!pHitMatchCat) {
		    pHitMatchCat=rich->buildCategory(catMatchHit);
		    
		    if (!pHitMatchCat) {
			Error("init","No HIT MATCH category defined");
			return kFALSE;
		    }
		    else event->addCategory(catMatchHit, pHitMatchCat, "Rich");
		}
		
		pIterMatchHit = (HIterator*)getHitMatchCat()->MakeIterator("native");

		pHitMatchHeaderCat=event->getCategory(catMatchHitHeader);
		if (!pHitMatchHeaderCat) {
		    pHitMatchHeaderCat=rich->buildCategory(catMatchHitHeader);
		    cout<<"i have built hhitmatchheader"<<endl;
		    if (!pHitMatchHeaderCat) {
			Error("init","No HIT MATCH HEADER category defined");
			return kFALSE;
		    }
		    else event->addCategory(catMatchHitHeader, pHitMatchHeaderCat
					    , "Rich");
		}
		
		pIterMatchHitHeader = (HIterator*)getHitMatchHeaderCat()
		    ->MakeIterator("native");
	    }
	    
	    if (mdc) { // MDC
		
		fMdcSeg=event->getCategory(catMdcSeg);
		if (!fMdcSeg) {
		    Error("init","No MDC segment category defined");
		    return kFALSE;
		}
		else fMdcSegIter=(HIterator *)fMdcSeg->MakeIterator();
	    }
	    if (tof) { // TOF
		fTofHits=event->getCategory(catTofHit);
		if (!fTofHits) {
		    fTofHits=tof->buildCategory(catTofHit);
		    if (!fTofHits) {
			Error("init","No TOF input");
			return kFALSE;
		    }
		}
		fTofIter=(HIterator *)fTofHits->MakeIterator();
	    }
	    
	    if (shower) { // SHOWER

		fShowerTofHits=event->getCategory(catShowerHitTof);
		if (!fShowerTofHits) {
		    fShowerTofHits = shower->buildCategory(catShowerHitTof);
		    if (!fShowerTofHits) {
			Error("init","No Shower input available");
			return kFALSE;
		    }
		}
		fShowerTofIter = (HIterator *)fShowerTofHits->MakeIterator();
	    }
	    

	    
	  
	} else {
	    Error ("init","! (event && rtdb)");
	    return kFALSE; //! (event && rtdb)
	}
	
	
	iniCounters();
        pFileOut = new TFile(pFileName,"RECREATE");
	return kTRUE;
    } else {
	Error ("init","! (gHades)");
	return kFALSE; //! (gHades)		
    }
}

Bool_t HRichCorrelator::reinit()
{
    //cout<<"in reinit function"<<endl;
    // called, when program enters execute() for the first time
    HRichCorrelatorPar*   pCorrPar = (HRichCorrelatorPar*)getCorrelationPar();
    if (pCorrPar->hasChanged()) // init only when the container has changed
    {
	iniCuts();
	iniSwitches();
	iniControlHistos();
	
	pCorrPar -> printParam();
    }
    return kTRUE;
}

void HRichCorrelator::iniCounters()
{
    // various counters
    // COUNTER INITIALIZATION
    nCounterProcessedNbEvents=0; //total nb of processed evts
    nCounterMin2Rings=0; //nb of events with min 2 rings 
    nCounterTotalNbRings=0; //total nb of processed rings
    nCounterRingsAbove45=0; //total nb of rings above 45 degrees pol angle
    nCounterRingsBelow45=0; //total nb of rings below 45 degrees pol angle
    nCounterEvtWritten=0; //total nb of events written to output after filter
    nCounterRingsWritten=0; //total nb of rings written to output after filter
    nCounterNbRejectedRings=0; //nb of (Mdc uncorr) rings deleted from 
                               //written events 
    nCounterRingsUnresMultHit=0; //number of rings for which more than one Mdc
                                 //hit was found (double rings)
    nCounterEventsUnresMultHit=0; //number of events in which rings 
                                  //with multiple Mdc hits were found
    nCounterRingsCorrMdc=0; //nb of rings corr with Mdc Segment
    nCounterRingsCorrShower=0; //nb of rings corr with Mdc and Shower (lep id)
    nCounterRingsCorrTof=0; //nb of rings corr with Mdc and Tof (lep id)

    ngNb_RM=0;//only
    ngNb_RT=0;
    ngNb_RS=0;
    ngNb_MT=0;
    ngNb_MS=0;
    ngNb_RcMcT=0;//  RM MT
    ngNb_RcMcTcR=0;//RM MT RT
    ngNb_RMcTcR=0;//    MT RT
    ngNb_RcMTcR=0;// RM    RT
    ngNb_RcMcS=0;//  RM MS
    ngNb_RcMcScR=0;//RM MS RS
    ngNb_RMcScR=0;//    MS RS
    ngNb_RcMScR=0;// RM    RS
    
    ngRTM=0;//3 indexes in obj
    ngRSM=0;
    ngRichCnt=0;
    ngMdcSegCnt=0;
    ngShowerCnt=0;
    ngTofCnt=0;
    ngDRichCnt=0;
    ngDMdcSegCnt=0;
    ngDShowerCnt=0;
    ngDTofCnt=0;
}

void HRichCorrelator::iniCuts() 
{
    // initialize parameters from RTDB containers
    // different cuts to be applied during hit matching
    HRichCorrelatorPar*   pCorrPar = (HRichCorrelatorPar*)getCorrelationPar();

    // FIXME: what to do with graphical cuts ?
    fMdcThetaCorrection    =  pCorrPar->getMdcThetaCorrection();
    nSelectedSector        =  pCorrPar->getSelectedSector();

    fRichMdcThetaCut       =  pCorrPar->getRichMdcThetaCut();
    fRichMdcThetaMinCut    =  pCorrPar-> getRichMdcThetaMinCut();
    fRichMdcThetaMaxCut    =  pCorrPar->getRichMdcThetaMaxCut();
    
    fRichTofThetaCut       =  pCorrPar->getRichTofThetaCut();
    fRichTofThetaMinCut    =  pCorrPar->getRichTofThetaMinCut();
    fRichTofThetaMaxCut    =  pCorrPar->getRichTofThetaMaxCut();
    
    fRichShowerThetaCut    =  pCorrPar->getRichShowerThetaCut();
    fRichShowerThetaMinCut =  pCorrPar->getRichShowerThetaMinCut();
    fRichShowerThetaMaxCut =  pCorrPar->getRichShowerThetaMaxCut();
    
    fMdcTofThetaCut        =  pCorrPar->getMdcTofThetaCut();
    fMdcTofThetaMinCut     =  pCorrPar->getMdcTofThetaMinCut();
    fMdcTofThetaMaxCut     =  pCorrPar->getMdcTofThetaMaxCut();
    
    fMdcShowerThetaCut     =  pCorrPar->getMdcShowerThetaCut();
    fMdcShowerThetaMinCut  =  pCorrPar->getMdcShowerThetaMinCut();
    fMdcShowerThetaMaxCut  =  pCorrPar->getMdcShowerThetaMaxCut();

    fRichMdcPhiCut      = pCorrPar->getRichMdcPhiCut();
    fRichTofPhiCut      = pCorrPar->getRichTofPhiCut();
    fRichShowerPhiCut   = pCorrPar->getRichShowerPhiCut();
    fMdcTofPhiCut       = pCorrPar->getMdcTofPhiCut();
    fMdcShowerPhiCut    = pCorrPar->getMdcShowerPhiCut();

    // T I M E  O F  F L I G H T 
    fTofTofMinCut          =  pCorrPar->getTofTofMinCut();
    // min time-of-flight cut from TOF
    fTofTofMaxCut          =  pCorrPar->getTofTofMaxCut();
    // max time-of-flight cut from TOF
    fTofinoTofMinCut       =  pCorrPar->getTofinoTofMinCut();
    // min time-of-flight cut from TOFINO
    fTofinoTofMaxCut       =  pCorrPar->getTofinoTofMaxCut();
    // max time-of-flight cut from TOFINO
    fShowerAmplification   =  pCorrPar->getShowerAmplification();
    // ratio between charge in Post1,2 to Pre

    nTofinoMultCut      = pCorrPar->getTofinoMultCut();
    //Tofino Multiplicity for valid corr w SHOWER

    // HISTOGRAM DIMENSIONS
    nNbBinsForPatMat = 160;
    nNbBinsForHouTra = 60;
    nHouTraMax = 500;

    // RICH RING QUALITY CUTS
    nPatMatCut   = pCorrPar->getRingPatMatCut();
    nHouTraCut   = pCorrPar->getRingHouTraCut();
    nPadNrCut    = pCorrPar->getRingPadNrCut();
    nRingAmplCut = pCorrPar->getRingAmplCut();
    nLocMax4Cut  = pCorrPar->getRingLocMax4Cut();
}

void HRichCorrelator::iniSwitches(void)
{
    HRichCorrelatorPar*   pCorrPar = (HRichCorrelatorPar*)getCorrelationPar();
    // require a ch. particle shower for valid correlation
    kShowerCond          = pCorrPar->getShowerCond();
    // do not require a cut on time of flight
    kIsNoCutOnTOFtof     = pCorrPar->getTofSwitch();
    kIsNoCutOnTOFINOtof  = pCorrPar->getTofinoSwitch();
    // do not check for RICH HIT
    kSwitchOffRich       = pCorrPar->getRichSwitch();
    // apply ring quality cut before matching
    kRingQualityCut      = pCorrPar->getRingQualitySwitch();
    // check for certain tofino multiplicity 
    kTofinoMultCut       = pCorrPar->getTofinoMultSwitch();
    // consider the magnetic field
    kMagnet              = pCorrPar->getMagnetSwitch();
    // restrict correlation search to one sector
    kNoSectorSelected    = pCorrPar->getSectorSelectionSwitch();
}

void HRichCorrelator::iniControlHistos()
{

    pHistArray = new TObjArray(130);
    pHistRMTheta = new TH2D("Rich_Mdc_theta","Rich_Mdc_theta"
			    ,90,0,90,90,0,90);
    pHistArray->Add(pHistRMTheta);
    pHistRMPhi = new TH2D("Rich_Mdc_phi","Rich_Mdc_phi"
			  ,360,0,360,360,0,360);

    pHistArray->Add(pHistRMPhi);
    pHistDiffRMTheta1 = new TH1F("Rich_Mdc_theta_diff1","Rich_Mdc_theta_diff1"
				,100,fRichMdcThetaMinCut-1,fRichMdcThetaMaxCut+1);
    pHistArray->Add(pHistDiffRMTheta1);
    pHistDiffRMPhi1 = new TH1F("Rich_Mdc_phi_diff1","Rich_Mdc_phi_diff1"
			      ,100,-fRichMdcPhiCut-1,fRichMdcPhiCut+1);
    pHistArray->Add(pHistDiffRMPhi1);
    pHistDiffRMTheta2 = new TH1F("Rich_Mdc_theta_diff2","Rich_Mdc_theta_diff2"
				,100,fRichMdcThetaMinCut-1,fRichMdcThetaMaxCut+1);
    pHistArray->Add(pHistDiffRMTheta2);
    pHistDiffRMPhi2 = new TH1F("Rich_Mdc_phi_diff2","Rich_Mdc_phi_diff2"
			      ,100,-fRichMdcPhiCut-1,fRichMdcPhiCut+1);
    pHistArray->Add(pHistDiffRMPhi2);
    pHistDiffRMTheta3 = new TH1F("Rich_Mdc_theta_diff3","Rich_Mdc_theta_diff3"
				,100,fRichMdcThetaMinCut-1,fRichMdcThetaMaxCut+1);
    pHistArray->Add(pHistDiffRMTheta3);
    pHistDiffRMPhi3 = new TH1F("Rich_Mdc_phi_diff3","Rich_Mdc_phi_diff3"
			      ,100,-fRichMdcPhiCut-1,fRichMdcPhiCut+1);
    pHistArray->Add(pHistDiffRMPhi3);
    pHistDiffRMTheta4 = new TH1F("Rich_Mdc_theta_diff4","Rich_Mdc_theta_diff4"
				,100,fRichMdcThetaMinCut-1,fRichMdcThetaMaxCut+1);
    pHistArray->Add(pHistDiffRMTheta4);
    pHistDiffRMPhi4 = new TH1F("Rich_Mdc_phi_diff4","Rich_Mdc_phi_diff4"
			      ,100,-fRichMdcPhiCut-1,fRichMdcPhiCut+1);
    pHistArray->Add(pHistDiffRMPhi4);
    pHistDiffRMTheta5 = new TH1F("Rich_Mdc_theta_diff5","Rich_Mdc_theta_diff5"
				,100,fRichMdcThetaMinCut-1,fRichMdcThetaMaxCut+1);
    pHistArray->Add(pHistDiffRMTheta5);
    pHistDiffRMPhi5 = new TH1F("Rich_Mdc_phi_diff5","Rich_Mdc_phi_diff5"
			      ,100,-fRichMdcPhiCut-1,fRichMdcPhiCut+1);
    pHistArray->Add(pHistDiffRMPhi5);
    pHistDiffRMTheta6 = new TH1F("Rich_Mdc_theta_diff6","Rich_Mdc_theta_diff6"
				,100,fRichMdcThetaMinCut-1,fRichMdcThetaMaxCut+1);
    pHistArray->Add(pHistDiffRMTheta6);
    pHistDiffRMPhi6 = new TH1F("Rich_Mdc_phi_diff6","Rich_Mdc_phi_diff6"
			      ,100,-fRichMdcPhiCut-1,fRichMdcPhiCut+1);
    pHistArray->Add(pHistDiffRMPhi6);


    pHistRSTheta = new TH2D("Rich_Shower_theta","Rich_Shower_theta"
			    ,90,0,90,90,0,90);
    pHistArray->Add(pHistRSTheta);
    pHistRSPhi = new TH2D("Rich_Shower_phi","Rich_Shower_phi"
			  ,360,0,360,360,0,360);
    pHistArray->Add(pHistRSPhi);
    pHistDiffRSTheta1 = new TH1F("Rich_Shower_theta_diff1","Rich_Shower_theta_diff1"
				,100,fRichShowerThetaMinCut-1,fRichShowerThetaMaxCut+1);
    pHistArray->Add(pHistDiffRSTheta1);
    pHistDiffRSPhi1 = new TH1F("Rich_Shower_phi_diff1","Rich_Shower_phi_diff1"
			      ,100,-fRichShowerPhiCut-1,fRichShowerPhiCut+1);
    pHistArray->Add(pHistDiffRSPhi1);
    pHistDiffRSTheta2 = new TH1F("Rich_Shower_theta_diff2","Rich_Shower_theta_diff2"
				,100,fRichShowerThetaMinCut-1,fRichShowerThetaMaxCut+1);
    pHistArray->Add(pHistDiffRSTheta2);
    pHistDiffRSPhi2 = new TH1F("Rich_Shower_phi_diff2","Rich_Shower_phi_diff2"
			      ,100,-fRichShowerPhiCut-1,fRichShowerPhiCut+1);
    pHistArray->Add(pHistDiffRSPhi2);
    pHistDiffRSTheta3 = new TH1F("Rich_Shower_theta_diff3","Rich_Shower_theta_diff3"
				,100,fRichShowerThetaMinCut-1,fRichShowerThetaMaxCut+1);
    pHistArray->Add(pHistDiffRSTheta3);
    pHistDiffRSPhi3 = new TH1F("Rich_Shower_phi_diff3","Rich_Shower_phi_diff3"
			      ,100,-fRichShowerPhiCut-1,fRichShowerPhiCut+1);
    pHistArray->Add(pHistDiffRSPhi3);
    pHistDiffRSTheta4 = new TH1F("Rich_Shower_theta_diff4","Rich_Shower_theta_diff4"
				,100,fRichShowerThetaMinCut-1,fRichShowerThetaMaxCut+1);
    pHistArray->Add(pHistDiffRSTheta4);
    pHistDiffRSPhi4 = new TH1F("Rich_Shower_phi_diff4","Rich_Shower_phi_diff4"
			      ,100,-fRichShowerPhiCut-1,fRichShowerPhiCut+1);
    pHistArray->Add(pHistDiffRSPhi4);
    pHistDiffRSTheta5 = new TH1F("Rich_Shower_theta_diff5","Rich_Shower_theta_diff5"
				,100,fRichShowerThetaMinCut-1,fRichShowerThetaMaxCut+1);
    pHistArray->Add(pHistDiffRSTheta5);
    pHistDiffRSPhi5 = new TH1F("Rich_Shower_phi_diff5","Rich_Shower_phi_diff5"
			      ,100,-fRichShowerPhiCut-1,fRichShowerPhiCut+1);
    pHistArray->Add(pHistDiffRSPhi5);
    pHistDiffRSTheta6 = new TH1F("Rich_Shower_theta_diff6","Rich_Shower_theta_diff6"
				,100,fRichShowerThetaMinCut-1,fRichShowerThetaMaxCut+1);
    pHistArray->Add(pHistDiffRSTheta6);
    pHistDiffRSPhi6 = new TH1F("Rich_Shower_phi_diff6","Rich_Shower_phi_diff6"
			      ,100,-fRichShowerPhiCut-1,fRichShowerPhiCut+1);
    pHistArray->Add(pHistDiffRSPhi6);


    pHistRTTheta = new TH2D("Rich_Tof_theta","Rich_Tof_theta"
			    ,90,0,90,90,0,90);
    pHistArray->Add(pHistRTTheta);
    pHistRTPhi = new TH2D("Rich_Tof_phi","Rich_Tof_phi"
			  ,360,0,360,360,0,360);
    pHistArray->Add(pHistRTTheta);
    pHistDiffRTTheta1 = new TH1F("Rich_Tof_theta_diff1","Rich_Tof_theta_diff1"
				,100,fRichTofThetaMinCut-1,fRichTofThetaMaxCut+1);
    pHistArray->Add(pHistDiffRTTheta1);
    pHistDiffRTPhi1 = new TH1F("Rich_Tof_phi_diff1","Rich_Tof_phi_diff1"
			      ,100,-fRichTofPhiCut-1,fRichTofPhiCut+1);
    pHistArray->Add(pHistDiffRTPhi1);
    pHistDiffRTTheta2 = new TH1F("Rich_Tof_theta_diff2","Rich_Tof_theta_diff2"
				,100,fRichTofThetaMinCut-1,fRichTofThetaMaxCut+1);
    pHistArray->Add(pHistDiffRTTheta2);
    pHistDiffRTPhi2 = new TH1F("Rich_Tof_phi_diff2","Rich_Tof_phi_diff2"
			      ,100,-fRichTofPhiCut-1,fRichTofPhiCut+1);
    pHistArray->Add(pHistDiffRTPhi2);
    pHistDiffRTTheta3 = new TH1F("Rich_Tof_theta_diff3","Rich_Tof_theta_diff3"
				,100,fRichTofThetaMinCut-1,fRichTofThetaMaxCut+1);
    pHistArray->Add(pHistDiffRTTheta3);
    pHistDiffRTPhi3 = new TH1F("Rich_Tof_phi_diff3","Rich_Tof_phi_diff3"
			      ,100,-fRichTofPhiCut-1,fRichTofPhiCut+1);
    pHistArray->Add(pHistDiffRTPhi3);
    pHistDiffRTTheta4 = new TH1F("Rich_Tof_theta_diff4","Rich_Tof_theta_diff4"
				,100,fRichTofThetaMinCut-1,fRichTofThetaMaxCut+1);
    pHistArray->Add(pHistDiffRTTheta4);
    pHistDiffRTPhi4 = new TH1F("Rich_Tof_phi_diff4","Rich_Tof_phi_diff4"
			      ,100,-fRichTofPhiCut-1,fRichTofPhiCut+1);
    pHistArray->Add(pHistDiffRTPhi4);
    pHistDiffRTTheta5 = new TH1F("Rich_Tof_theta_diff5","Rich_Tof_theta_diff5"
				,100,fRichTofThetaMinCut-1,fRichTofThetaMaxCut+1);
    pHistArray->Add(pHistDiffRTTheta5);
    pHistDiffRTPhi5 = new TH1F("Rich_Tof_phi_diff5","Rich_Tof_phi_diff5"
			      ,100,-fRichTofPhiCut-1,fRichTofPhiCut+1);
    pHistArray->Add(pHistDiffRTPhi5);
    pHistDiffRTTheta6 = new TH1F("Rich_Tof_theta_diff6","Rich_Tof_theta_diff6"
				,100,fRichTofThetaMinCut-1,fRichTofThetaMaxCut+1);
    pHistArray->Add(pHistDiffRTTheta6);
    pHistDiffRTPhi6 = new TH1F("Rich_Tof_phi_diff6","Rich_Tof_phi_diff6"
			      ,100,-fRichTofPhiCut-1,fRichTofPhiCut+1);
    pHistArray->Add(pHistDiffRTPhi6);
    
    pHistMTTheta = new TH2D("Mdc_Tof_theta","Mdc_Tof_theta"
			    ,90,0,90,90,0,90);
    pHistArray->Add(pHistMTTheta);
    pHistMTPhi = new TH2D("Mdc_Tof_phi","Mdc_Tof_phi"
			  ,360,0,360,360,0,360);
    pHistArray->Add(pHistMTPhi);
    pHistDiffMTTheta1 = new TH1F("Mdc_Tof_theta_diff1","Mdc_Tof_theta_diff1"
				,100,fMdcTofThetaMinCut-1,fMdcTofThetaMaxCut+1);
    pHistArray->Add(pHistDiffMTTheta1);
    pHistDiffMTPhi1 = new TH1F("Mdc_Tof_phi_diff1","Mdc_Tof_phi_diff1"
			      ,100,-fMdcTofPhiCut-1,fMdcTofPhiCut+1);
    pHistArray->Add(pHistDiffMTPhi1);
    pHistDiffMTTheta2 = new TH1F("Mdc_Tof_theta_diff2","Mdc_Tof_theta_diff2"
				,100,fMdcTofThetaMinCut-1,fMdcTofThetaMaxCut+1);
    pHistArray->Add(pHistDiffMTTheta2);
    pHistDiffMTPhi2 = new TH1F("Mdc_Tof_phi_diff2","Mdc_Tof_phi_diff2"
			      ,100,-fMdcTofPhiCut-1,fMdcTofPhiCut+1);
    pHistArray->Add(pHistDiffMTPhi2);
    pHistDiffMTTheta3 = new TH1F("Mdc_Tof_theta_diff3","Mdc_Tof_theta_diff3"
				,100,fMdcTofThetaMinCut-1,fMdcTofThetaMaxCut+1);
    pHistArray->Add(pHistDiffMTTheta3);
    pHistDiffMTPhi3 = new TH1F("Mdc_Tof_phi_diff3","Mdc_Tof_phi_diff3"
			      ,100,-fMdcTofPhiCut-1,fMdcTofPhiCut+1);
    pHistArray->Add(pHistDiffMTPhi3);
    pHistDiffMTTheta4 = new TH1F("Mdc_Tof_theta_diff4","Mdc_Tof_theta_diff4"
				,100,fMdcTofThetaMinCut-1,fMdcTofThetaMaxCut+1);
    pHistArray->Add(pHistDiffMTTheta4);
    pHistDiffMTPhi4 = new TH1F("Mdc_Tof_phi_diff4","Mdc_Tof_phi_diff4"
			      ,100,-fMdcTofPhiCut-1,fMdcTofPhiCut+1);
    pHistArray->Add(pHistDiffMTPhi4);
    pHistDiffMTTheta5 = new TH1F("Mdc_Tof_theta_diff5","Mdc_Tof_theta_diff5"
				,100,fMdcTofThetaMinCut-1,fMdcTofThetaMaxCut+1);
    pHistArray->Add(pHistDiffMTTheta5);
    pHistDiffMTPhi5 = new TH1F("Mdc_Tof_phi_diff5","Mdc_Tof_phi_diff5"
			      ,100,-fMdcTofPhiCut-1,fMdcTofPhiCut+1);
    pHistArray->Add(pHistDiffMTPhi5);
    pHistDiffMTTheta6 = new TH1F("Mdc_Tof_theta_diff6","Mdc_Tof_theta_diff6"
				,100,fMdcTofThetaMinCut-1,fMdcTofThetaMaxCut+1);
    pHistArray->Add(pHistDiffMTTheta6);
    pHistDiffMTPhi6 = new TH1F("Mdc_Tof_phi_diff6","Mdc_Tof_phi_diff6"
			      ,100,-fMdcTofPhiCut-1,fMdcTofPhiCut+1);
    pHistArray->Add(pHistDiffMTPhi6);
    pHistMSTheta = new TH2D("Mdc_Shower_theta","Mdc_Shower_theta"
			    ,90,0,90,90,0,90);
    pHistArray->Add(pHistMSTheta);
    pHistMSPhi = new TH2D("Mdc_Shower_phi","Mdc_Shower_phi"
			  ,360,0,360,360,0,360);
    pHistArray->Add(pHistMSPhi);
    pHistDiffMSTheta1 = new TH1F("Mdc_Shower_theta_diff1","Mdc_Shower_theta_diff1"
				,100,fMdcShowerThetaMinCut-1,fMdcShowerThetaCut);
    pHistArray->Add(pHistDiffMSTheta1);
    pHistDiffMSPhi1 = new TH1F("Mdc_Shower_phi_diff1","Mdc_Shower_phi_diff1"
			      ,100,-fMdcShowerPhiCut-1,fMdcShowerPhiCut+1);
    pHistArray->Add(pHistDiffMSPhi1);
    pHistDiffMSTheta2 = new TH1F("Mdc_Shower_theta_diff2","Mdc_Shower_theta_diff2"
				,100,fMdcShowerThetaMinCut-1,fMdcShowerThetaMaxCut+1);
    pHistArray->Add(pHistDiffMSTheta2);
    pHistDiffMSPhi2 = new TH1F("Mdc_Shower_phi_diff2","Mdc_Shower_phi_diff2"
			      ,100,-fMdcShowerPhiCut-1,fMdcShowerPhiCut+1);
    pHistArray->Add(pHistDiffMSPhi2);
    pHistDiffMSTheta3 = new TH1F("Mdc_Shower_theta_diff3","Mdc_Shower_theta_diff3"
				,100,fMdcShowerThetaMinCut-1,fMdcShowerThetaMaxCut+1);
    pHistArray->Add(pHistDiffMSTheta3);
    pHistDiffMSPhi3 = new TH1F("Mdc_Shower_phi_diff3","Mdc_Shower_phi_diff3"
			      ,100,-fMdcShowerPhiCut-1,fMdcShowerPhiCut+1);
    pHistArray->Add(pHistDiffMSPhi3);
    pHistDiffMSTheta4 = new TH1F("Mdc_Shower_theta_diff4","Mdc_Shower_theta_diff4"
				,100,fMdcShowerThetaMinCut-1,fMdcShowerThetaMaxCut+1);
    pHistArray->Add(pHistDiffMSTheta4);
    pHistDiffMSPhi4 = new TH1F("Mdc_Shower_phi_diff4","Mdc_Shower_phi_diff4"
			      ,100,-fMdcShowerPhiCut-1,fMdcShowerPhiCut+1);
    pHistArray->Add(pHistDiffMSPhi4);
    pHistDiffMSTheta5 = new TH1F("Mdc_Shower_theta_diff5","Mdc_Shower_theta_diff5"
				,100,fMdcShowerThetaMinCut-1,fMdcShowerThetaMaxCut+1);
    pHistArray->Add(pHistDiffMSTheta5);
    pHistDiffMSPhi5 = new TH1F("Mdc_Shower_phi_diff5","Mdc_Shower_phi_diff5"
			      ,100,-fMdcShowerPhiCut-1,fMdcShowerPhiCut+1);
    pHistArray->Add(pHistDiffMSPhi5);
    pHistDiffMSTheta6 = new TH1F("Mdc_Shower_theta_diff6","Mdc_Shower_theta_diff6"
				,100,fMdcShowerThetaMinCut-1,fMdcShowerThetaMaxCut+1);
    pHistArray->Add(pHistDiffMSTheta6);
    pHistDiffMSPhi6 = new TH1F("Mdc_Shower_phi_diff6","Mdc_Shower_phi_diff6"
			      ,100,-fMdcShowerPhiCut-1,fMdcShowerPhiCut+1);
    pHistArray->Add(pHistDiffMSPhi6);
    pHistRMTRingHouTra = new TH1F("RMT_Hough","RMT_Hough"
				  ,nNbBinsForHouTra,0,nHouTraMax);
    pHistArray->Add(pHistRMTRingHouTra);
    pHistRMTRingPatMat = new TH1F("RMT_PatMat","RMT_PatMat"
				  ,nNbBinsForPatMat,0,800);
    pHistArray->Add(pHistRMTRingPatMat);
    pHistRMTRingAmplitude = new TH1F("RMT_Ampl","RMT_Ampl",100,0,1000);
    pHistArray->Add(pHistRMTRingAmplitude);
    pHistRMTRingPadNr = new TH1F("RMT_PadNr","RMT_PadNr",40,0,40);
    pHistArray->Add(pHistRMTRingPadNr);
    pHistRMTRingLocMax4 = new TH1F("RMT_LocMax4","RMT_LocMax4",15,0,15);
    pHistArray->Add(pHistRMTRingLocMax4);
    pHistRMTRingRadius = new TH1F("RMT_Radius","RMT_Radius",40,2,6);
    pHistArray->Add(pHistRMTRingRadius);
    pHistRMTRingCenterX = new TH1F("RMT_X","RMT_X",90,0,90);
    pHistArray->Add(pHistRMTRingCenterX);
    pHistRMTRingCenterY = new TH1F("RMT_Y","RMT_Y",90,0,90);
    pHistArray->Add(pHistRMTRingCenterY);
    pHistRMTRingTheta = new TH1F("RMT_Theta","RMT_Theta",90,0,90);
    pHistArray->Add(pHistRMTRingTheta);
    pHistRMTRingPhi = new TH1F("RMT_Phi","RMT_Phi",360,0,360);
    pHistArray->Add(pHistRMTRingPhi);
 
    pHistRMSRingHouTra = new TH1F("RMS_Hough","RMS_Hough"
				  ,nNbBinsForHouTra,0,nHouTraMax);
    pHistArray->Add(pHistRMSRingHouTra);
    pHistRMSRingPatMat = new TH1F("RMS_PatMat","RMS_PatMat"
				  ,nNbBinsForPatMat,0,800);
    pHistArray->Add(pHistRMSRingPatMat);
    pHistRMSRingAmplitude = new TH1F("RMS_Ampl","RMS_Ampl",100,0,1000);
    pHistArray->Add(pHistRMSRingAmplitude);
    pHistRMSRingPadNr = new TH1F("RMS_PadNr","RMS_PadNr",40,0,40);
    pHistArray->Add(pHistRMSRingPadNr);
    pHistRMSRingLocMax4 = new TH1F("RMS_LocMax4","RMS_LocMax4",15,0,15);
    pHistArray->Add(pHistRMSRingLocMax4);
    pHistRMSRingRadius = new TH1F("RMS_Radius","RMS_Radius",40,2,6);
    pHistArray->Add(pHistRMSRingRadius);
    pHistRMSRingCenterX = new TH1F("RMS_X","RMS_X",90,0,90);
    pHistArray->Add(pHistRMSRingCenterX);
    pHistRMSRingCenterY = new TH1F("RMS_Y","RMS_Y",90,0,90);
    pHistArray->Add(pHistRMSRingCenterY);
    pHistRMSRingTheta = new TH1F("RMS_Theta","RMS_Theta",90,0,90);
    pHistArray->Add(pHistRMSRingTheta);
    pHistRMSRingPhi = new TH1F("RMS_Phi","RMS_Phi",360,0,360);
    pHistArray->Add(pHistRMSRingPhi);

    pHistRMRingHouTra = new TH1F("RM_Hough","RM_Hough"
				 ,nNbBinsForHouTra,0,nHouTraMax);
    pHistArray->Add(pHistRMRingHouTra);
    pHistRMRingPatMat = new TH1F("RM_PatMat","RM_PatMat"
				 ,nNbBinsForPatMat,0,800);
    pHistArray->Add(pHistRMRingPatMat);
    pHistRMRingAmplitude = new TH1F("RM_Ampl","RM_Ampl",100,0,1000);
    pHistArray->Add(pHistRMRingAmplitude);
    pHistRMRingPadNr = new TH1F("RM_PadNr","RM_PadNr",40,0,40);
    pHistArray->Add(pHistRMRingPadNr);
    pHistRMRingLocMax4 = new TH1F("RM_LocMax4","RM_LocMax4",15,0,15);
    pHistArray->Add(pHistRMRingLocMax4);
    pHistRMRingRadius = new TH1F("RM_Radius","RM_Radius",40,2,6);
    pHistArray->Add(pHistRMRingRadius);
    pHistRMRingCenterX = new TH1F("RM_X","RM_X",90,0,90);
    pHistArray->Add(pHistRMRingCenterX);
    pHistRMRingCenterY = new TH1F("RM_Y","RM_Y",90,0,90);
    pHistArray->Add(pHistRMRingCenterY);
    pHistRMRingTheta = new TH1F("RM_Theta","RM_Theta",90,0,90);
    pHistArray->Add(pHistRMRingTheta);
    pHistRMRingPhi = new TH1F("RM_Phi","RM_Phi",360,0,360);
    pHistArray->Add(pHistRMRingPhi);
    pHistALLRingHouTra = new TH1F("ALL_Hough","ALL_Hough"
				  ,nNbBinsForHouTra,0,nHouTraMax);
    pHistArray->Add(pHistALLRingHouTra);
    pHistALLRingPatMat = new TH1F("ALL_PatMat","ALL_PatMat"
				  ,nNbBinsForPatMat,0,800);
    pHistArray->Add(pHistALLRingPatMat);
    pHistALLRingAmplitude = new TH1F("ALL_Ampl","ALL_Ampl",100,0,1000);
    pHistArray->Add(pHistALLRingAmplitude);
    pHistALLRingPadNr = new TH1F("ALL_PadNr","ALL_PadNr",40,0,40);
    pHistArray->Add(pHistALLRingPadNr);
    pHistALLRingLocMax4 = new TH1F("ALL_LocMax4","ALL_LocMax4",15,0,15);
    pHistArray->Add(pHistALLRingLocMax4);
    pHistALLRingRadius = new TH1F("ALL_Radius","ALL_Radius",40,2,6);
    pHistArray->Add(pHistALLRingRadius);
    pHistALLRingCenterX = new TH1F("ALL_X","ALL_X",90,0,90);
    pHistArray->Add(pHistALLRingCenterX);
    pHistALLRingCenterY = new TH1F("ALL_Y","ALL_Y",90,0,90);
    pHistArray->Add(pHistALLRingCenterY);
    pHistALLRingTheta = new TH1F("ALL_Theta","ALL_Theta",90,0,90);
    pHistArray->Add(pHistALLRingTheta);
    pHistALLRingPhi = new TH1F("ALL_Phi","ALL_Phi",360,0,360);
    pHistArray->Add(pHistALLRingPhi);

    pHistRMSRingHouTraTheta = new TH2D("RMS_HouTra_Theta","RMS_HouTra_Theta"
				       ,90,0,90,nNbBinsForHouTra,0,nHouTraMax);
    pHistArray->Add(pHistRMSRingHouTraTheta);
    pHistRMTRingHouTraTheta = new TH2D("RMT_HouTra_Theta","RMT_HouTra_Theta"
				       ,90,0,90,nNbBinsForHouTra,0,nHouTraMax);
    pHistArray->Add(pHistRMTRingHouTraTheta);
    pHistRMSRingPatMatTheta = new TH2D("RMS_PatMat_Theta","RMS_PatMat_Theta"
				       ,90,0,90,nNbBinsForPatMat,0,800);
    pHistArray->Add(pHistRMSRingPatMatTheta);
    pHistRMTRingPatMatTheta = new TH2D("RMT_PatMat_Theta","RMT_PatMat_Theta"
				       ,90,0,90,nNbBinsForPatMat,0,800);
    pHistArray->Add(pHistRMTRingPatMatTheta);
    pHistALLRingHouTraTheta = new TH2D("ALL_HouTra_Theta","ALL_HouTra_Theta"
				       ,90,0,90,nNbBinsForHouTra,0,nHouTraMax);
    pHistArray->Add(pHistALLRingHouTraTheta);
    pHistALLRingPatMatTheta = new TH2D("ALL_PatMat_Theta","ALL_PatMat_Theta"
				       ,90,0,90,nNbBinsForPatMat,0,800);
    pHistArray->Add(pHistALLRingPatMatTheta);
    pHistRMRingHouTraTheta = new TH2D("RM_HouTra_Theta","RM_HouTra_Theta"
				      ,90,0,90,nNbBinsForHouTra,0,nHouTraMax);
    pHistArray->Add(pHistRMRingHouTraTheta);
    pHistRMRingPatMatTheta = new TH2D("RM_PatMat_Theta","RM_PatMat_Theta"
				      ,90,0,90,nNbBinsForPatMat,0,800);
    pHistArray->Add(pHistRMRingPatMatTheta);
    pHistRMTRingAmplitudeTheta = new TH2D("RMT_Ampli_Theta","RMT_Ampli_Theta"
					  ,90,0,90,100,0,1000);
    pHistArray->Add(pHistRMTRingAmplitudeTheta);
    pHistRMRingAmplitudeTheta = new TH2D("RM_Ampli_Theta","RM_Ampli_Theta"
					 ,90,0,90,100,0,1000);
    pHistArray->Add(pHistRMRingAmplitudeTheta);
    pHistRMSRingAmplitudeTheta = new TH2D("RMS_Ampli_Theta","RMS_Ampli_Theta"
					  ,90,0,90,100,0,1000);
    pHistArray->Add(pHistRMSRingAmplitudeTheta);
    pHistALLRingAmplitudeTheta = new TH2D("ALL_Ampli_Theta","ALL_Ampli_Theta"
					  ,90,0,90,100,0,1000);
    pHistArray->Add(pHistALLRingAmplitudeTheta);
    pHistRMTRingPadNrTheta = new TH2D("RMT_PadNr_Theta","RMT_PadNr_Theta"
					  ,90,0,90,40,0,40);
    pHistArray->Add(pHistRMTRingPadNrTheta);
    pHistRMRingPadNrTheta = new TH2D("RM_PadNr_Theta","RM_PadNr_Theta"
					 ,90,0,90,40,0,40);
    pHistArray->Add(pHistRMRingPadNrTheta);
    pHistRMSRingPadNrTheta = new TH2D("RMS_PadNr_Theta","RMS_PadNr_Theta"
					  ,90,0,90,40,0,40);
    pHistArray->Add(pHistRMSRingPadNrTheta);
    pHistALLRingPadNrTheta = new TH2D("ALL_PadNr_Theta","ALL_PadNr_Theta"
					  ,90,0,90,40,0,40);
    pHistArray->Add(pHistALLRingPadNrTheta);
    pHistRMTRingLocMax4Theta = new TH2D("RMT_LocMax4_Theta","RMT_LocMax4_Theta"
					,90,0,90,15,0,15);
    pHistArray->Add(pHistRMTRingLocMax4Theta);
    pHistRMRingLocMax4Theta = new TH2D("RM_LocMax4_Theta","RM_LocMax4_Theta"
					 ,90,0,90,15,0,15);
    pHistArray->Add(pHistRMRingLocMax4Theta);
    pHistRMSRingLocMax4Theta = new TH2D("RMS_LocMax4_Theta","RMS_LocMax4_Theta"
					  ,90,0,90,15,0,15);
    pHistArray->Add(pHistRMSRingLocMax4Theta);
    pHistALLRingLocMax4Theta = new TH2D("ALL_LocMax4_Theta","ALL_LocMax4_Theta"
					  ,90,0,90,15,0,15);
    pHistArray->Add(pHistALLRingLocMax4Theta);
    
    
}
void HRichCorrelator::incrementRingCount(HRichHit *pRichHit){
  
  
    nCounterTotalNbRings++;
    Float_t fRT = pRichHit->getTheta();
    if (fRT>45.) nCounterRingsAbove45++;
    else nCounterRingsBelow45++;
}
Int_t HRichCorrelator::execute()
{
       if (nCounterProcessedNbEvents != 0 &&
 	nCounterProcessedNbEvents%50000==0) HRichUtilFunc::saveHistos(pFileOut,pHistArray);
       if (nCounterProcessedNbEvents != 0 &&
	   nCounterProcessedNbEvents%10000==0) 
       {
	   cout<<"********** "<<nCounterProcessedNbEvents
	       <<" EVTS PROCESSED **********"<<endl;
       }
    HRichHit         *pRichHit     =0;
    HTofHit          *pTofHit      =0;
    HShowerHitTof    *pShowerHitTof=0;
    HMdcSeg          *pMdcSeg      =0;

  
    //    cout<<"Nb of rings before quality check: "<<getRichHitCat()->getEntries()
    //<<endl;
    fRichIter->Reset();
    if (kRingQualityCut)
    {// iterate over RICH hits, check quality and remove bad ones from category
	TObjArray *tmp = new TObjArray(5);
	while((pRichHit = (HRichHit *)fRichIter->Next()))
	{
	    cout<<"in ring qual cut"<<endl;
	    if (!isGoodRing(pRichHit)) tmp->Add(pRichHit);
	}
	if (!delRichHit(tmp)) Error("execute","Bad Rich hits not deleted");
	delete tmp;
    }
    //cout<<"Nb of rings after quality check: "<<getRichHitCat()->getEntries()
    //<<endl;
    Int_t nTofObjCounter = 0;
    fTofIter->Reset();
    // iterate over TOF hits as there is always a TOF hit (1st level trigger)
    while((pTofHit = (HTofHit *)fTofIter->Next()))
    {
      nTofObjCounter++;
      // temporary arrays for matched hit objs
      TObjArray *pRT = new TObjArray(5);
      TObjArray *pMT = new TObjArray(5);

	// switchable if-Statement to fill only for selected sector
	if ( kNoSectorSelected || isSelectedSector(pTofHit->getSector()))
	{

	    if(!kSwitchOffRich)
	    {
		fRichIter->Reset();
		// iterate over RICH hits
		while((pRichHit = (HRichHit *)fRichIter->Next()))
		{
		    if (nTofObjCounter==1)//fill Rich nocut histos only once in evt
		    {			
			fillALLRingHistos(pRichHit);
			incrementRingCount(pRichHit);
		    }
		    if (isCorrelated(pRichHit,pTofHit))
		    {
			HHitMatch *pHM = new HHitMatch();
			fillHitMatch(pHM,pRichHit,pTofHit);
			pRT->Add(pHM);
			nCounterRingsCorrTof++;
		    }
		}//end of rich while
	    }// end of rich switch


	    fMdcSegIter->Reset();
	    while((pMdcSeg = (HMdcSeg *)fMdcSegIter->Next()))
	    {
		if (isCorrelated(pMdcSeg,pTofHit))
		{
		  if (pRT->GetLast()==-1)// no RT objs (RICH-TOF corr objs)
		    {
		      HHitMatch *pHM = new HHitMatch();
		      fillHitMatch(pHM,pMdcSeg,pTofHit);
		      pMT->Add(pHM);
		    }
		  else // RT objs in list
		    {
		      for (Int_t i=0;i<(pRT->GetLast()+1);i++)
			{
			  if( ((HHitMatch*)(*pRT)[i])->getMdcInd()==-1)// RT w/o MDC 
			    {
			      fillHitMatch((HHitMatch*)(*pRT)[i],pMdcSeg,pTofHit);
			    }
			  else // it is already RTM -> make copy and overwrite MDC info
			    {
			      HHitMatch *pHMcpy = new HHitMatch();
			      *pHMcpy = *((HHitMatch*)(*pRT)[i]);
			      fillHitMatch(pHMcpy,pMdcSeg,pTofHit);
			      pMT->Add(pHMcpy);
			    }
			}
		    }
		}//endif isCorr(MDC-TOF)
	    }//endwhile MDCSEG
	}// sector switch

	// now copy temp objs into HHitMatch category to store them in tree
	for (Int_t i=0;i<(pRT->GetLast()+1);i++)
	  {
	    HHitMatch* pHM = createHitMatch(((HHitMatch*)(*pRT)[i])->getSector());
	    *pHM = * ((HHitMatch*)(*pRT)[i]);
	    delete (HHitMatch*)(*pRT)[i];
	  }
	delete pRT;// delete temp obj array 

	for (Int_t j=0;j<(pMT->GetLast()+1);j++)
	  {
	    HHitMatch* pHM = createHitMatch(((HHitMatch*)(*pMT)[j])->getSector());
	    *pHM = * ((HHitMatch*)(*pMT)[j]);
	    delete (HHitMatch*)(*pMT)[j];
	  }
	delete pMT;
    }// endwhile TOFHIT


    // now begin from SHOWER, the second META detector
    fShowerTofIter->Reset();
    while((pShowerHitTof = (HShowerHitTof *)fShowerTofIter->Next()))
    {
      TObjArray* pRS = new TObjArray(5);
      TObjArray* pMS = new TObjArray(5);

	if ( kNoSectorSelected || isSelectedSector(pShowerHitTof->getSector()))
	{
	    if(!kSwitchOffRich)
	    {
		fRichIter->Reset();
		while((pRichHit = (HRichHit *)fRichIter->Next()))
		{
		    if (isCorrelated(pRichHit,pShowerHitTof))
		    {
		      HHitMatch *pHM = new HHitMatch();
		      fillHitMatch(pHM,pRichHit,pShowerHitTof);
		      pRS->Add(pHM);
		      nCounterRingsCorrShower++;
		    }
		}
	    }


	    fMdcSegIter->Reset();
	    while((pMdcSeg = (HMdcSeg *)fMdcSegIter->Next()))
	    {
		if (isCorrelated(pMdcSeg,pShowerHitTof))
		{
		  if (pRS->GetLast()==-1)// no RS objs
		    {
		      HHitMatch* pHM = new HHitMatch();
		      fillHitMatch(pHM,pMdcSeg,pShowerHitTof);
		      pMS->Add(pHM);
		    }
		  else
		    {
		      for (Int_t i=0;i<(pRS->GetLast()+1);i++)
			{
			  if (((HHitMatch*)(*pRS)[i])->getMdcInd()==-1)
			    {
			      fillHitMatch((HHitMatch*)(*pRS)[i],pMdcSeg,
					   pShowerHitTof);
			    }
			  else
			    {
			      HHitMatch *pHMcpy = new HHitMatch();
			      *pHMcpy = *((HHitMatch*)(*pRS)[i]);
			      fillHitMatch(pHMcpy,pMdcSeg,pShowerHitTof);
			      pMS->Add(pHMcpy);
			    }
			}

		    }
		}//isCorr
	    }// endwhile MDCSEG
	}// end sector switch

	for (Int_t i=0;i<(pRS->GetLast()+1);i++)
	  {
	    HHitMatch* pHM = createHitMatch(((HHitMatch*)(*pRS)[i])->getSector());
	    *pHM = * ((HHitMatch*)(*pRS)[i]);
	    delete (HHitMatch*)(*pRS)[i];
	  }
	delete pRS;

	for (Int_t j=0;j<(pMS->GetLast()+1);j++)
	  {
	    HHitMatch* pHM = createHitMatch(((HHitMatch*)(*pMS)[j])->getSector());
	    *pHM = * ((HHitMatch*)(*pMS)[j]);
	    delete (HHitMatch*)(*pMS)[j];
	  }
	delete pMS;
    }//endwhile showerhittof

    // now check RM correlations
    // get the indexes of all correlated Rich MdcSeg combinations.
    if (!kSwitchOffRich){
	Int_t nMaxNbRMCorrs = getRichHitCat()->getEntries()*
	    getMdcSegCat()->getEntries();
	Int_t *nRichInd = new Int_t[nMaxNbRMCorrs];
	Int_t *nMdcSegInd = new Int_t[nMaxNbRMCorrs];
	Int_t *nUsedInd = new Int_t[nMaxNbRMCorrs];
	for (Int_t i=0;i<nMaxNbRMCorrs;i++) 
	{
	    nRichInd[i]=-1;
	    nMdcSegInd[i]=-1;
	    nUsedInd[i]=-1;
	}
	Int_t nRMCorrCounter=0;
	fRichIter->Reset();
	// correlate all RICH and MDC hits seperately
	// and store the hit cat indexes in temp arrays
	// set also a flag that this array slot holds a corr
	while((pRichHit = (HRichHit *)fRichIter->Next()))
	{
	    fMdcSegIter->Reset();
	    while((pMdcSeg = (HMdcSeg *)fMdcSegIter->Next()))
	    {
		if(isCorrelated(pRichHit,pMdcSeg))
		{
		    nRichInd[nRMCorrCounter]=getRichHitCat()
			->getIndex(pRichHit);
		    nMdcSegInd[nRMCorrCounter]=getMdcSegCat()
			->getIndex(pMdcSeg);
		    nUsedInd[nRMCorrCounter]=0;//set a flag for a corr
		    nRMCorrCounter++;
		}
	    }
	}
	
	
	HHitMatch *pHM=0;
	Int_t nMdcHMInd,nRichHMInd;
	nMdcHMInd=nRichHMInd=0;
	// iterate over the new category HHitMatch
	// and complete the corr info with the help of the above
	// found exclusive RM correlations
	// this is the only bi-corr that was not checked seperately
	// in the above loops starting with a META detector
	pIterMatchHit->Reset();
	while(( pHM= (HHitMatch *)pIterMatchHit->Next()))
	{
	    nRichHMInd=pHM->getRichInd();
	    nMdcHMInd=pHM->getMdcInd();
	    if (nMdcHMInd>-1 && nRichHMInd>-1)//it's a (T|S)RM obj
	    {
		for (Int_t i=0;i<nMaxNbRMCorrs;i++)
		{
		    if (nRichInd[i]==-1) break;//jump out of the loop
		    if (nRichInd[i]==nRichHMInd  &&
			nMdcSegInd[i]==nMdcHMInd    )
		    {
			pHM->setMatchedRichMdc(1);
			nUsedInd[i]++;
			// it's correlated, set the flag !
		    }
		    // else it has a TR and TM but no RM correlation
		}
	    }
	    else if (nMdcHMInd>-1 && nRichHMInd==-1)//it's a (T|S)M obj
		{
		    // loop over the above found RM corrs
		    for (Int_t i=0;i<nMaxNbRMCorrs;i++)
		    {
			if (nMdcSegInd[i]==-1) break;//jump out of the loop
			if (nMdcSegInd[i]==nMdcHMInd)
			{
			    // it's correlated, set the flag,
			    // and set the Rich values 
			    HRichHit* r =((HRichHit*)getRichHitCat()
					  ->getObject(nRichInd[i]));
			    if(r) setRich(pHM,r);
			    pHM->setMatchedRichMdc(1);
			    nUsedInd[i]++;
			}
			// else it has a (T|S)M but no RM and no (T|S)R correlation
		    }
		}
	    else if (nMdcHMInd==-1 && nRichHMInd>-1)//it's a (T|S)R obj
		{
		    for (Int_t i=0;i<nMaxNbRMCorrs;i++)
		    {
			if (nRichInd[i]==-1) break;//jump out of the loop
			if (nRichInd[i]==nRichHMInd)
			{
			    // it's correlated, set the flag,
			    // and set the Rich values 
			    HMdcSeg* m =((HMdcSeg*)getMdcSegCat()
					 ->getObject(nMdcSegInd[i]));
			    if(m) setMdc(pHM,m);
			    pHM->setMatchedRichMdc(1);
			    nUsedInd[i]++;
			}
			// else it has a (T|S)R but no RM and no (T|S)M correlation
		    }
		}
	}
	for (Int_t i=0;i<nMaxNbRMCorrs;i++)
	{// take care of RM w/o T or S in any combination
	    if (nUsedInd[i]==-1) break;
	    if (nUsedInd[i]==0)//marks a RM corr that was not used above
	    {
		HMdcSeg* m =((HMdcSeg*)getMdcSegCat()
					 ->getObject(nMdcSegInd[i]));
		HRichHit* r =((HRichHit*)getRichHitCat()
					  ->getObject(nRichInd[i]));
		HHitMatch* pHM = createHitMatch(r->getSector());
		fillHitMatch(pHM,r,m);
	    }
	}
	delete nRichInd;
	delete nMdcSegInd;
	delete nUsedInd;
    }//endif switch off Rich
    
    fillControlHistos(); // use MatchHit objs to fill control histos
    HHitMatchHeader *pHMH = createHitMatchHeader();
    fillHitMatchHeader(pHMH);
    //******** debug info ***********
    //    cout<<"evts processed: "<<nCounterProcessedNbEvents<<endl;
    //    pHMH->dumpToStdout();
    //*******************************
    nCounterProcessedNbEvents++;



  
    return kTRUE;


}

Bool_t HRichCorrelator::delRichHit(TObjArray *t)
{
    // remove rich hit obj from HLinearCategory
    // afterwards delete empty slot
    if (t)
    {
	for (Int_t i=0;i<(t->GetLast()+1);i++)
	{
	    ((TClonesArray*)((HLinearCategory*)getRichHitCat())->getCollection())
	    ->Remove((HRichHit*)((*t)[i]));
	    ((TClonesArray*)((HLinearCategory*)getRichHitCat())
	     ->getCollection())->Compress();
	    nCounterNbRejectedRings++;
	}
	return kTRUE;
    }
    return kFALSE;
}

Bool_t HRichCorrelator::finalize() {
    //cout<<"in finalize"<<endl;
     pProfRMTHTtheta=pHistRMTRingHouTraTheta->
 	ProfileX("prof_RMT_HTtheta",0,nNbBinsForHouTra);
     pHistArray->Add(pProfRMTHTtheta);
     pProfRMSHTtheta=pHistRMSRingHouTraTheta->
 	ProfileX("prof_RMS_HTtheta",0,nNbBinsForHouTra);
     pHistArray->Add(pProfRMSHTtheta);
     pProfRMTPMtheta=pHistRMTRingPatMatTheta->
 	ProfileX("prof_RMT_PMtheta",0,nNbBinsForPatMat);
     pHistArray->Add(pProfRMTPMtheta);
     pProfRMHTtheta=pHistRMRingHouTraTheta->
 	ProfileX("prof_RM_HTtheta",0,nNbBinsForHouTra);
     pHistArray->Add(pProfRMHTtheta);
     pProfRMPMtheta=pHistRMRingPatMatTheta->
 	ProfileX("prof_RM_PMtheta",0,nNbBinsForPatMat);
     pHistArray->Add(pProfRMPMtheta);
     pProfRMSPMtheta=pHistRMSRingPatMatTheta->
 	ProfileX("prof_RMS_PMtheta",0,nNbBinsForPatMat);
     pHistArray->Add(pProfRMSPMtheta);
     pProfALLHTtheta=pHistALLRingHouTraTheta->
 	ProfileX("prof_ALL_HTtheta",0,nNbBinsForHouTra);
     pHistArray->Add(pProfALLHTtheta);
     pProfALLPMtheta=pHistALLRingPatMatTheta->
 	ProfileX("prof_ALL_PMtheta",0,nNbBinsForPatMat);
     pHistArray->Add(pProfALLPMtheta);
    
     pProfRMPadNrTheta=pHistRMRingPadNrTheta->
 	ProfileX("prof_RM_PadNrTheta",0,40);
     pHistArray->Add(pProfRMPadNrTheta);
     pProfRMTPadNrTheta=pHistRMTRingPadNrTheta->
 	ProfileX("prof_RMT_PadNrTheta",0,40);
     pHistArray->Add(pProfRMTPadNrTheta);
     pProfRMSPadNrTheta=pHistRMSRingPadNrTheta->
 	ProfileX("prof_RMS_PadNrTheta",0,40);
     pHistArray->Add(pProfRMSPadNrTheta);
     pProfALLPadNrTheta=pHistALLRingPadNrTheta->
 	ProfileX("prof_ALL_PadNrTheta",0,40);
     pHistArray->Add(pProfALLPadNrTheta);

     pProfRMLocMax4Theta=pHistRMRingLocMax4Theta->
 	ProfileX("prof_RM_LocMax4Theta",0,15);
    pHistArray->Add(pProfRMLocMax4Theta);
     pProfRMTLocMax4Theta=pHistRMTRingLocMax4Theta->
 	ProfileX("prof_RMT_LocMax4Theta",0,15);
     pHistArray->Add(pProfRMTLocMax4Theta);
     pProfRMSLocMax4Theta=pHistRMSRingLocMax4Theta->
 	ProfileX("prof_RMS_LocMax4Theta",0,15);
     pHistArray->Add(pProfRMSLocMax4Theta);
     pProfALLLocMax4Theta=pHistALLRingLocMax4Theta->
	ProfileX("prof_ALL_LocMax4Theta",0,15);
     pHistArray->Add(pProfALLLocMax4Theta);

     pProfRMRingAmplitudeTheta=pHistRMRingAmplitudeTheta->
 	ProfileX("prof_RM_RingAmplitudeTheta",0,1000);
     pHistArray->Add(pProfRMRingAmplitudeTheta);
     pProfRMTRingAmplitudeTheta=pHistRMTRingAmplitudeTheta->
 	ProfileX("prof_RMT_RingAmplitudeTheta",0,1000);
     pHistArray->Add(pProfRMTRingAmplitudeTheta);
     pProfRMSRingAmplitudeTheta=pHistRMSRingAmplitudeTheta->
 	ProfileX("prof_RMS_RingAmplitudeTheta",0,1000);
    pHistArray->Add(pProfRMSRingAmplitudeTheta);
     pProfALLRingAmplitudeTheta=pHistALLRingAmplitudeTheta->
 	ProfileX("prof_ALL_RingAmplitudeTheta",0,1000);
     pHistArray->Add(pProfALLRingAmplitudeTheta);
     // go to output file
     pFileOut->cd();

     // comment this back in if you want to store the canvas 
     // in the histogram control file
//     // create canvas
//      TCanvas *canvas1 = makeCanvas1();
//      canvas1->Write();
//      TCanvas *canvas2 = makeCanvas2();
//      canvas2->Write();
//      TCanvas *canvas3 = HRichUtilFunc::makeMicroCanvas(pHistArray);
//      canvas3->Write();
//     // write histograms
     HRichUtilFunc::saveHistos(pFileOut,pHistArray);
     pFileOut->Close();

     dumpCorrelationStatus();
     
    return kTRUE;
}

HHitMatchHeader* HRichCorrelator::createHitMatchHeader()
{
    // HHitMatchHeader is meant to be a bookkeeping obj
    // storing info on an event-by-event basis
    HLocation loc;
    loc.set(1,0);//dummy sector set to zero
    HHitMatchHeader *pHMH = (HHitMatchHeader*)((HLinearCategory*)
			    getHitMatchHeaderCat())->getNewSlot(loc);
    if (pHMH) pHMH = new(pHMH) HHitMatchHeader;
    return pHMH;
}

Int_t HRichCorrelator::fillUniqueIndex(Int_t i,Int_t max,Int_t* iarr)
{
    // check if index i is already stored in iarr
    // if not then store it
    // iarr is initialized to -2
    
    Int_t n=0;
    do{
	if(i==iarr[n]) return 0;//i already stored
	else if(iarr[n]==-2) //new slot
	{
	    iarr[n]=i;
	    return n+1;
	}
	n++;
    } while(n<max);
}
Int_t HRichCorrelator::countDiscreteIndexes(Int_t *iarr,Int_t max)
{
    //count arr slots that are different from -2 (ini value)
    Int_t n=0;
    for(Int_t i=0;i<max;i++)
    {
	if(iarr[i]!=-2 && iarr[i]!=-1) n++;
	else break;
    }
    return n;
}
void HRichCorrelator::fillHitMatchHeader(HHitMatchHeader *pHMH)
{
    // iterate over HHitMatch (correlation or tracklet) objs
    // in this evt, categorize and count them. Finally fill 
    // HHitMatchHeader obj for this evt.
    HRichHit         *pRichHit     =0;
    HTofHit          *pTofHit      =0;
    HShowerHitTof    *pShowerHitTof=0;
    HMdcSeg          *pMdcSeg      =0;
    HHitMatch        *pHM          =0;
    Int_t nRichCnt=0;  // nb of different Rich hits in this evt
    fRichIter->Reset();// (!!! for min bias you get one from HT and
                       // one from PM with a different center !!!)
    while((pRichHit = (HRichHit *)fRichIter->Next())) nRichCnt++;
    Int_t nTofCnt=0;
    fTofIter->Reset();
    while((pTofHit = (HTofHit *)fTofIter->Next())) nTofCnt++;
    Int_t nMdcSegCnt=0;
    fMdcSegIter->Reset();
    while((pMdcSeg = (HMdcSeg *)fMdcSegIter->Next())) nMdcSegCnt++;
    Int_t nShowerCnt=0;
    fShowerTofIter->Reset();
    while((pShowerHitTof=(HShowerHitTof *)fShowerTofIter->Next())) nShowerCnt++;
    Int_t nNbCorrObjs=0;//nb of HHitMatch objs in evt
    pIterMatchHit->Reset();
    while(( pHM= (HHitMatch *)pIterMatchHit->Next())) nNbCorrObjs++;

    // helper arrays to determine number of diff indexes of hits
    Int_t *tRind = new Int_t[nNbCorrObjs];//cannot be bigger than nb of corrs
    Int_t *tMind = new Int_t[nNbCorrObjs];
    Int_t *tTind = new Int_t[nNbCorrObjs];
    Int_t *tSind = new Int_t[nNbCorrObjs];
    for (Int_t i=0;i<nNbCorrObjs;i++) tRind[i]=tMind[i]=tTind[i]=tSind[i]=-2;

    // local (for this evt) counter variables
    Int_t nlNb_RM=0;//only
    Int_t nlNb_RT=0;
    Int_t nlNb_RS=0;
    Int_t nlNb_MT=0;
    Int_t nlNb_MS=0;
    Int_t nlNb_RcMcT=0;//  RM MT
    Int_t nlNb_RcMcTcR=0;//RM MT RT
    Int_t nlNb_RMcTcR=0;//    MT RT
    Int_t nlNb_RcMTcR=0;// RM    RT
    Int_t nlNb_RcMcS=0;//  RM MS
    Int_t nlNb_RcMcScR=0;//RM MS RS
    Int_t nlNb_RMcScR=0;//    MS RS
    Int_t nlNb_RcMScR=0;// RM    RS

    Int_t nRTM=0;//3 indexes in obj
    Int_t nRSM=0;

    pIterMatchHit->Reset();
    while(( pHM= (HHitMatch *)pIterMatchHit->Next()))
    {
	//pHM->dumpToStdout();
	
	//indexes in respective hit cntr
	//useful to see whether det is in corr indep from specific one
	Int_t nRInd = pHM->getRichInd();
	Int_t nMInd = pHM->getMdcInd();
	Int_t nTInd = pHM->getTofInd();
	Int_t nSInd = pHM->getShowInd();
	if(nRInd!=-1) fillUniqueIndex(nRInd,nNbCorrObjs,tRind);
	if(nMInd!=-1) fillUniqueIndex(nMInd,nNbCorrObjs,tMind);
	if(nTInd!=-1) fillUniqueIndex(nTInd,nNbCorrObjs,tTind);
	if(nSInd!=-1) fillUniqueIndex(nSInd,nNbCorrObjs,tSind);

	//flags indicating a det bi correlation
	Int_t nRM = pHM->getMatchedRichMdc();
	Int_t nRT = pHM->getMatchedRichTof();
	Int_t nRS = pHM->getMatchedRichShower();
	Int_t nMT = pHM->getMatchedMdcTof();
	Int_t nMS = pHM->getMatchedMdcShower();
	// -------------------------------------------------
	//min a tri corr obj
	if (nRInd!=-1 && nTInd!=-1 && nMInd!=-1) nRTM++;

	else if (nRInd!=-1 && nSInd!=-1 && nMInd!=-1) nRSM++;

	//sub cases of the two above
	if (nRInd!=-1 && nTInd!=-1 && nMInd!=-1 && 
	    nRM==1 &&  nRT==1 &&  nMT==1 ) nlNb_RcMcTcR++;

	else if (nRInd!=-1 && nSInd!=-1 && nMInd!=-1 && 
	    nRM==1 &&  nRS==1 &&  nMS==1 ) nlNb_RcMcScR++;
	
	else if (nRInd!=-1 && nTInd!=-1 && nMInd!=-1 &&
		 nRM==1 && nRT==1) nlNb_RcMTcR++;

	else if (nRInd!=-1 && nSInd!=-1 && nMInd!=-1 &&
		 nRM==1 && nRS==1) nlNb_RcMScR++;

	else if (nRInd!=-1 && nTInd!=-1 && nMInd!=-1 &&
		 nRM==1 && nMT==1) nlNb_RcMcT++;

	else if (nRInd!=-1 && nSInd!=-1 && nMInd!=-1 &&
		 nRM==1 && nMS==1) nlNb_RcMcS++;

	else if (nRInd!=-1 && nTInd!=-1 && nMInd!=-1 &&
		 nRT==1 && nMT==1) nlNb_RMcTcR++;
	
	else if (nRInd!=-1 && nSInd!=-1 && nMInd!=-1 &&
		 nRS==1 && nMS==1) nlNb_RMcScR++;
	// only a bi corr object
	if (nRInd!=-1 && nMInd!=-1 && nTInd==-1 && nSInd==-1) nlNb_RM++;
	else if (nRInd!=-1 && nTInd!=-1 && nMInd==-1 && nSInd==-1) nlNb_RT++;
	else if (nRInd!=-1 && nSInd!=-1 && nMInd==-1 && nTInd==-1) nlNb_RS++;
	else if (nMInd!=-1 && nTInd!=-1 && nRInd==-1 && nSInd==-1) nlNb_MT++;
	else if (nMInd!=-1 && nSInd!=-1 && nRInd==-1 && nTInd==-1) nlNb_MS++;
    }// end while over HHitMatch

    // increment resp var for the whole run
    // and fill evt wise value in HHitMatchHeader obj
    // (there are n (Rich) hits in nNbCorrObjs tracklets
    //  in this evt, but only m<n are unique)
    // t?ind contains unique hit cat indexes used in evt
    Int_t nDRichCnt   = countDiscreteIndexes(tRind,nNbCorrObjs);
    Int_t nDMdcSegCnt = countDiscreteIndexes(tMind,nNbCorrObjs);
    Int_t nDShowerCnt = countDiscreteIndexes(tSind,nNbCorrObjs);
    Int_t nDTofCnt    = countDiscreteIndexes(tTind,nNbCorrObjs);
    // delete helper arrays
    delete tRind;
    delete tMind;
    delete tSind;
    delete tTind;
    // 'global' var for the whole run
    ngDRichCnt  +=nDRichCnt;
    ngDMdcSegCnt+=nDMdcSegCnt;
    ngDShowerCnt+=nDShowerCnt;
    ngDTofCnt   +=nDTofCnt;

    ngNbCorrObjs+=nNbCorrObjs;
    pHMH->setNbCorrObjs(nNbCorrObjs);

    pHMH->setNbRichHits(nRichCnt);
    pHMH->setNbDiscreteRichHits(nDRichCnt);

    pHMH->setNbMdcSegHits(nMdcSegCnt);
    pHMH->setNbDiscreteMdcSegHits(nDMdcSegCnt);

    pHMH->setNbShowerHits(nShowerCnt);
    pHMH->setNbDiscreteShowerHits(nDShowerCnt);

    pHMH->setNbTofHits(nTofCnt);
    pHMH->setNbDiscreteTofHits(nDTofCnt);
    
    ngRichCnt  +=nRichCnt;
    ngMdcSegCnt+=nMdcSegCnt;
    ngShowerCnt+=nShowerCnt;
    ngTofCnt   +=nTofCnt;
    ngNb_RM+=nlNb_RM;//only these two are matched
    pHMH->setNb_RM(nlNb_RM);
    ngNb_RT+=nlNb_RT;
    pHMH->setNb_RT(nlNb_RT);
    ngNb_RS+=nlNb_RS;
    pHMH->setNb_RS(nlNb_RS);
    ngNb_MT+=nlNb_MT;
    pHMH->setNb_MT(nlNb_MT);
    ngNb_MS+=nlNb_MS;
    pHMH->setNb_MS(nlNb_MS);
    // three diff dets contrib
    ngNb_RcMcT+=nlNb_RcMcT;//        RM MT
    pHMH->setNb_RcMcT(nlNb_RcMcT);
    ngNb_RcMcTcR+=nlNb_RcMcTcR;//    RM MT RT
    pHMH->setNb_RcMcTcR(nlNb_RcMcTcR);
    ngNb_RMcTcR+=nlNb_RMcTcR;//         MT RT
    pHMH->setNb_RMcTcR(nlNb_RMcTcR);
    ngNb_RcMTcR+=nlNb_RcMTcR;//      RM    RT
    pHMH->setNb_RcMTcR(nlNb_RcMTcR);
    ngNb_RcMcS+=nlNb_RcMcS;//        RM MS
    pHMH->setNb_RcMcS(nlNb_RcMcS);
    ngNb_RcMcScR+=nlNb_RcMcScR;//    RM MS RS
    pHMH->setNb_RcMcScR(nlNb_RcMcScR);
    ngNb_RMcScR+=nlNb_RMcScR;//         MS RS
    pHMH->setNb_RMcScR(nlNb_RMcScR);
    ngNb_RcMScR+=nlNb_RcMScR;//      RM    RS
    pHMH->setNb_RcMScR(nlNb_RcMScR);
    ngRTM+=nRTM;//min 3 indexes in obj indep of corr
    pHMH->setNb_minRMT(nRTM);
    ngRSM+=nRSM;
    pHMH->setNb_minRMS(nRSM);
     
}

HHitMatch* HRichCorrelator::createHitMatch(Int_t s)
{
    //create a new HHitMatch object, input sector
    HLocation loc;
    loc.set(1,s);
    HHitMatch *pHitMatch = (HHitMatch*)((HLinearCategory*)
					getHitMatchCat())->getNewSlot(loc);
    nCounterRingsWritten++;
    if (pHitMatch!=NULL) pHitMatch = new(pHitMatch) HHitMatch;
    return pHitMatch;
}
void HRichCorrelator::setRich(HHitMatch* m,HRichHit* r){
    // copy info from RICH hit into HHitMatch
    m->setRichPhi(r->getPhi());
    m->setRichTheta(r->getTheta());
    m->setRingCenterX(r->iRingX);
    m->setRingCenterY(r->iRingY);
    m->setRingPatMat(r->iRingPatMat);
    m->setRingHouTra(r->iRingHouTra);
    m->setRingAmplitude((Int_t) r->iRingAmplitude);
    m->setRingPadNr((Int_t) r->iRingPadNr);
    m->setRingLocalMax4((Int_t) r->iRingLocalMax4);
    m->setCentroid(r->fRingCentroid);
    m->setRingMaxClusterSize(r->getRingMaxClusterSize());
    m->setRingMaxThrClusterSize(r->getRingMaxThrClusterSize());
    m->setRingTestFlags(r->fTests);
    m->setRadius(r->fRingRadius);
    m->setRichInd(getRichHitCat()->getIndex(r));
}
void HRichCorrelator::setMdc(HHitMatch* m,HMdcSeg* mdc){
    // copy info from MDC Seg into HHitMatch
    Float_t rad2deg = 180./TMath::Pi();
    // (multiplicative misalignment) correction stored !!!!
    // see correlator parameter container
    m->setMdcTheta(correctMdcSegTheta(mdc->getTheta()*rad2deg));
    m->setMdcPhi(mdc->getPhi()*rad2deg+correctMdcSegPhi(mdc->getSec()));
    m->setMdcClusterSize(mdc->getChi2());
    m->setMdcClusterHit(mdc->getFlag());
    m->setMdcFlagIO(mdc->getIOSeg());
    m->setMdcInd(getMdcSegCat()->getIndex(mdc));
}
void HRichCorrelator::setTof(HHitMatch* m, HTofHit* t){
    // copy info from Tof hit to HHitMatch
    Float_t tt = 0.; t->getTheta(tt);
    Float_t tp = 0.; t->getPhi(tp);   
    m->setTofTheta(tt);
    m->setTofPhi(tp);
    m->setTofTof(t->getTof());
    m->setTofInd(getTofHitCat()->getIndex(t));
}
void HRichCorrelator::setShower(HHitMatch* m,HShowerHitTof* st){
    // copy info from ShowerHitTof to HHitMatch
    Float_t dmmy= 0.;
    Float_t stt = 0.;
    Float_t stp = 0.;
    st->getSphereCoord(&dmmy,&stp,&stt);
    Float_t tof = st->getTof();
    m->setShowerTheta(stt);
    m->setShowerPhi(stp);
    m->setTofinoTof(tof);
    m->setShowerfSum0(st->getSum(0));
    m->setShowerfSum1(st->getSum(1));
    m->setShowerfSum2(st->getSum(2));
    m->setTofinoMul(st->getTofinoMult());
    m->setShowInd(getShowerTofHitCat()->getIndex(st));
}
void HRichCorrelator::fillHitMatch(HHitMatch* m, HRichHit* r, HMdcSeg* mdc)
{
    // check whether this correlation object already holds info
    // if not fill in detector specific info
    if((m->getRichInd())<0)
    {
	setRich(m,r);
    }
    if((m->getMdcInd())<0)
    {
	setMdc(m,mdc);
    }
    m->setMatchedRichMdc(1);
    m->setSector(r->getSector());
    
}
void HRichCorrelator::fillHitMatch(HHitMatch* m, HMdcSeg* mdc, HShowerHitTof* st)
{    
   
    setMdc(m,mdc);
    if(m->getShowInd()<0){
	setShower(m,st);
    }
    m->setMatchedMdcShower(1);
    if (st->getSector() == mdc->getSec()) m->setSector(st->getSector());
   
}

void HRichCorrelator::fillHitMatch(HHitMatch* m, HMdcSeg* mdc, HTofHit* t)
{
    
    setMdc(m,mdc);
    if(m->getTofInd()<0){
	setTof(m,t);
    }
    m->setMatchedMdcTof(1);
    //m->setMdcTofThetaCut(fMdcTofThetaCut);
    //m->setMdcTofPhiCut(fMdcTofPhiCut);
    if (t->getSector() == mdc->getSec()) m->setSector(t->getSector());
}

void HRichCorrelator::fillHitMatch(HHitMatch* m, HRichHit *r, HTofHit* t)
{
   
    setRich(m,r);
    if(m->getTofInd()<0){
	setTof(m,t);
    }
    m->setMatchedRichTof(1);
    if (t->getSector() == r->getSector()) m->setSector(t->getSector());
}

void HRichCorrelator::fillHitMatch(HHitMatch* m, HRichHit *r, HShowerHitTof* st)
{
    if(m->getShowInd()<0){
	setShower(m,st);
    }
    setRich(m,r);
    m->setMatchedRichShower(1);
    if (st->getSector() == r->getSector()) m->setSector(st->getSector());
    
}

void HRichCorrelator::fillALLRingHistos(HRichHit* h)
{
    // fill histos for all rich hits w/o selection or correlation
 pHistALLRingHouTra->Fill(h->iRingHouTra);
 pHistALLRingPatMat->Fill(h->iRingPatMat);
 pHistALLRingAmplitude->Fill((Int_t) h->iRingAmplitude);
 pHistALLRingPadNr->Fill((Int_t) h->iRingPadNr);
 pHistALLRingLocMax4->Fill((Int_t) h->iRingLocalMax4);
 pHistALLRingRadius->Fill(h->fRingRadius);
 pHistALLRingCenterX->Fill(h->iRingX);
 pHistALLRingCenterY->Fill(h->iRingY);
 pHistALLRingTheta->Fill(h->getTheta());
 pHistALLRingPhi->Fill(h->getPhi());
 pHistALLRingHouTraTheta->Fill(h->getTheta(),h->iRingHouTra);
 pHistALLRingPatMatTheta->Fill(h->getTheta(),h->iRingPatMat);
 pHistALLRingAmplitudeTheta->Fill(h->getTheta(),h->iRingAmplitude);
 pHistALLRingPadNrTheta->Fill(h->getTheta(),h->iRingPadNr);
 pHistALLRingLocMax4Theta->Fill(h->getTheta(),h->iRingLocalMax4);

}


void HRichCorrelator::fillControlHistos(void)
{
    // use HHitMatch objs to fill histos under certain conditions
    // e.g. only for corr objs that contain a RMT
    HHitMatch *pHitMatch=0;
    pIterMatchHit->Reset();
    while((pHitMatch = (HHitMatch *)pIterMatchHit->Next()))
    {
	if (pHitMatch->getMatchedRichMdc()==1 &&
	    pHitMatch->getMatchedMdcTof()==1    ) // it's at least a RM && MT
	{
	    pHistRMTRingHouTra->Fill(pHitMatch->getRingHouTra());
	    pHistRMTRingPatMat->Fill(pHitMatch->getRingPatMat());
	    pHistRMTRingAmplitude->Fill(pHitMatch->getRingAmplitude());
	    pHistRMTRingPadNr->Fill(pHitMatch->getRingPadNr());
	    pHistRMTRingLocMax4->Fill(pHitMatch->getRingLocalMax4());
	    pHistRMTRingRadius->Fill(pHitMatch->getRadius());
	    pHistRMTRingCenterX->Fill(pHitMatch->getRingCenterX());
	    pHistRMTRingCenterY->Fill(pHitMatch->getRingCenterY());
	    pHistRMTRingTheta->Fill(pHitMatch->getRichTheta());
	    pHistRMTRingPhi->Fill(pHitMatch->getRichPhi());
	    pHistRMTRingHouTraTheta->Fill(pHitMatch->getRichTheta(),
					  pHitMatch->getRingHouTra());
	    pHistRMTRingPatMatTheta->Fill(pHitMatch->getRichTheta(),
					  pHitMatch->getRingPatMat());
	    pHistRMTRingAmplitudeTheta->Fill(pHitMatch->getRichTheta(),
					     pHitMatch->getRingAmplitude());
	    pHistRMTRingPadNrTheta->Fill(pHitMatch->getRichTheta(),
					 pHitMatch->getRingPadNr()); 
	    pHistRMTRingLocMax4Theta->Fill(pHitMatch->getRichTheta(),
					     pHitMatch->getRingLocalMax4());
	}

	if (pHitMatch->getMatchedRichMdc()==1 &&
	    pHitMatch->getMatchedMdcShower()==1    ) // it's at least RM && MS
	{
	    pHistRMSRingHouTra->Fill(pHitMatch->getRingHouTra());
	    pHistRMSRingPatMat->Fill(pHitMatch->getRingPatMat());
	    pHistRMSRingAmplitude->Fill(pHitMatch->getRingAmplitude());
	    pHistRMSRingPadNr->Fill(pHitMatch->getRingPadNr());
	    pHistRMSRingLocMax4->Fill(pHitMatch->getRingLocalMax4());
	    pHistRMSRingRadius->Fill(pHitMatch->getRadius());
	    pHistRMSRingCenterX->Fill(pHitMatch->getRingCenterX());
	    pHistRMSRingCenterY->Fill(pHitMatch->getRingCenterY());
	    pHistRMSRingTheta->Fill(pHitMatch->getRichTheta());
	    pHistRMSRingPhi->Fill(pHitMatch->getRichPhi());
	    pHistRMSRingHouTraTheta->Fill(pHitMatch->getRichTheta(),
					  pHitMatch->getRingHouTra());
	    pHistRMSRingPatMatTheta->Fill(pHitMatch->getRichTheta(),
					  pHitMatch->getRingPatMat());
	    pHistRMSRingAmplitudeTheta->Fill(pHitMatch->getRichTheta(),
					     pHitMatch->getRingAmplitude());
	    pHistRMSRingPadNrTheta->Fill(pHitMatch->getRichTheta(),
					     pHitMatch->getRingPadNr());
	    pHistRMSRingLocMax4Theta->Fill(pHitMatch->getRichTheta(),
					     pHitMatch->getRingLocalMax4());

	}

	if (pHitMatch->getMatchedRichMdc()==1) // it's at least RM
	{
	    pHistRMTheta->Fill(pHitMatch->getRichTheta(),
			       pHitMatch->getMdcTheta());
	    pHistRMPhi->Fill(pHitMatch->getRichPhi(),
			     pHitMatch->getMdcPhi());
	    Int_t sec = pHitMatch->getSector();
	    char *histname1 = new char[50];
  	    sprintf(histname1,"Rich_Mdc_theta_diff%d",sec+1);
	    TH1F* h1 = ((TH1F*)(pHistArray->FindObject(histname1)));
	    if (h1) h1->Fill(pHitMatch->getRichTheta()-pHitMatch->getMdcTheta());
	    else cout<<h1<<" is a null pointer"<<endl;
	    delete histname1;
	    char *histname2 = new char[50];
  	    sprintf(histname2,"Rich_Mdc_phi_diff%d",sec+1);
	    ((TH1F*)(pHistArray->FindObject(histname2)))->
		Fill(pHitMatch->getRichPhi()-pHitMatch->getMdcPhi());
	    delete histname2;
	    pHistRMRingHouTra->Fill(pHitMatch->getRingHouTra());
	    pHistRMRingPatMat->Fill(pHitMatch->getRingPatMat());
	    pHistRMRingAmplitude->Fill(pHitMatch->getRingAmplitude());
	    pHistRMRingPadNr->Fill(pHitMatch->getRingPadNr());
	    pHistRMRingLocMax4->Fill(pHitMatch->getRingLocalMax4());
	    pHistRMRingRadius->Fill(pHitMatch->getRadius());
	    pHistRMRingCenterX->Fill(pHitMatch->getRingCenterX());
	    pHistRMRingCenterY->Fill(pHitMatch->getRingCenterY());
	    pHistRMRingTheta->Fill(pHitMatch->getRichTheta());
	    pHistRMRingPhi->Fill(pHitMatch->getRichPhi());
  	    pHistRMRingHouTraTheta->Fill(pHitMatch->getRichTheta(),
  					  pHitMatch->getRingHouTra());
	    pHistRMRingPatMatTheta->Fill(pHitMatch->getRichTheta(),
					  pHitMatch->getRingPatMat());
	    pHistRMRingAmplitudeTheta->Fill(pHitMatch->getRichTheta(),
					     pHitMatch->getRingAmplitude());
	    pHistRMRingPadNrTheta->Fill(pHitMatch->getRichTheta(),
					     pHitMatch->getRingPadNr());
	    pHistRMRingLocMax4Theta->Fill(pHitMatch->getRichTheta(),
					     pHitMatch->getRingLocalMax4());

	}

	if (pHitMatch->getMatchedRichTof()==1) // it's at least a RT
	{
	    pHistRTTheta->Fill(pHitMatch->getRichTheta(),
			       pHitMatch->getTofTheta());
	    pHistRTPhi->Fill(pHitMatch->getRichPhi(),
			     pHitMatch->getTofPhi());
	    Int_t sec = pHitMatch->getSector();
	    char *histname1 = new char[50];
  	    sprintf(histname1,"Rich_Tof_theta_diff%d",sec+1);
	    ((TH1F*)(pHistArray->FindObject(histname1)))->
		Fill(pHitMatch->getRichTheta()-pHitMatch->getTofTheta());
	    delete histname1;
	    char *histname2 = new char[50];
  	    sprintf(histname2,"Rich_Tof_phi_diff%d",sec+1);
	    ((TH1F*)(pHistArray->FindObject(histname2)))->
		Fill(pHitMatch->getRichPhi()-pHitMatch->getTofPhi());
	    delete histname2;
	}

	if (pHitMatch->getMatchedRichShower()==1) // it's at least a RS
	{
	    pHistRSTheta->Fill(pHitMatch->getRichTheta(),
			       pHitMatch->getShowerTheta());
	    pHistRSPhi->Fill(pHitMatch->getRichPhi(),
			     pHitMatch->getShowerPhi());
	    Int_t sec = pHitMatch->getSector();
	    char *histname1 = new char[50];
  	    sprintf(histname1,"Rich_Shower_theta_diff%d",sec+1);
	    ((TH1F*)(pHistArray->FindObject(histname1)))->
		Fill(pHitMatch->getRichTheta()-pHitMatch->getShowerTheta());
	    delete histname1;
	    char *histname2 = new char[50];
  	    sprintf(histname2,"Rich_Shower_phi_diff%d",sec+1);
	    ((TH1F*)(pHistArray->FindObject(histname2)))->
		Fill(pHitMatch->getRichPhi()-pHitMatch->getShowerPhi());
	    delete histname2;
	}

	if (pHitMatch->getMatchedMdcTof()==1) // it's at least a MT
	{
	    pHistMTTheta->Fill(pHitMatch->getMdcTheta(),
			       pHitMatch->getTofTheta());
	    pHistMTPhi->Fill(pHitMatch->getMdcPhi(),
			     pHitMatch->getTofPhi());
	    Int_t sec = pHitMatch->getSector();
	    char *histname1 = new char[50];
  	    sprintf(histname1,"Mdc_Tof_theta_diff%d",sec+1);
	    ((TH1F*)(pHistArray->FindObject(histname1)))->
		Fill(pHitMatch->getMdcTheta()-pHitMatch->getTofTheta());
	    delete histname1;
	    char *histname2 = new char[50];
  	    sprintf(histname2,"Mdc_Tof_phi_diff%d",sec+1);
	    ((TH1F*)(pHistArray->FindObject(histname2)))->
		Fill(pHitMatch->getMdcPhi()-pHitMatch->getTofPhi());
	    delete histname2;
	}

	if (pHitMatch->getMatchedMdcShower()==1) // it's at least a MS
	{
	    pHistMSTheta->Fill(pHitMatch->getMdcTheta(),
			       pHitMatch->getShowerTheta());
	    pHistMSPhi->Fill(pHitMatch->getMdcPhi(),
			     pHitMatch->getShowerPhi());

	    Int_t sec = pHitMatch->getSector();
	    char *histname1 = new char[50];
  	    sprintf(histname1,"Mdc_Shower_theta_diff%d",sec+1);
	    ((TH1F*)(pHistArray->FindObject(histname1)))->
		Fill(pHitMatch->getMdcTheta()-pHitMatch->getShowerTheta());
	    delete histname1;
	    char *histname2 = new char[50];
  	    sprintf(histname2,"Mdc_Shower_phi_diff%d",sec+1);
	    ((TH1F*)(pHistArray->FindObject(histname2)))->
		Fill(pHitMatch->getMdcPhi()-pHitMatch->getShowerPhi());
	    delete histname2;
	}
    }

}

Float_t HRichCorrelator::correctMdcSegPhi(Int_t s)
{
    // shift MDC Seg phi according to the sector
    Float_t nMdcPhiCorr=0;
    switch (s)
    {
    case 1:
	nMdcPhiCorr=+60.;
	break;
    case 2:
	nMdcPhiCorr=+120.;
	break;
    case 3:
	nMdcPhiCorr=+180.;
	break;
    case 4:
	nMdcPhiCorr=+240.;
	break;
    case 5:
	nMdcPhiCorr=-60.;
	break;
    }
    return nMdcPhiCorr;
}

Float_t HRichCorrelator::correctMdcSegTheta(Float_t th)
{
    // multiply MdcSeg polar angles by a const factor
    // to account for shifted target
    // E.g. target was 18mm more downstream than seg finder assumes
    // (info W. Koenig, apr01 gen1)
    // correction factor can be set in param container, only rough corr !!!
    // not necessary if alignment was done for DST production
    Float_t correctedTheta = th * fMdcThetaCorrection;
    return correctedTheta;
}
Bool_t HRichCorrelator::isGoodRing(HRichHit* r){
    // apply rich ring quality check
    //cout<<"in isGoodRing()"<<endl;
    
    // for min bias settings in the DST prod
    // we get usually the same ring twice:
    // one hit with patmat and one with houtra info.
    // two avoid multiplying the number of correlation objs
    // do e.g. if(r->getRingPatMat() >= nPatMatCut &&
    //            r->getRingHouTra() == 0)
    if((r->getRingPatMat() >= nPatMatCut ||
	r->getRingHouTra()>= nHouTraCut) &&
        r->getRingAmplitude() >= nRingAmplCut &&
        r->getRingPadNr() >= nPadNrCut &&
        r->getRingLocalMax4()>= nLocMax4Cut) 
    {
// 	cout<<"pat mat "<<r->getRingPatMat()<<">="<<nPatMatCut<<endl;
// 	cout<<"hou tra "<<r->getRingHouTra()<<">="<<nHouTraCut<<endl;
// 	cout<<"ampl "<<r->getRingAmplitude()<<">="<<nRingAmplCut<<endl;
// 	cout<<"pad nr "<<r->getRingPadNr()<<">="<<nPadNrCut<<endl;
// 	cout<<"loc max4 "<<r->getRingLocalMax4()<<">="<<nLocMax4Cut<<endl;
// 	cout<<"KEEP IT !!!!!!!!!!!!"<<endl;
	return kTRUE;
    }
    else 
    {
	//	cout<<"THROW IT !!!!!!"<<endl;
	return kFALSE;
    }
}
Bool_t HRichCorrelator::isSelectedSector(Int_t n)
{
    if (n == nSelectedSector) return kTRUE;
    else return kFALSE;
}

Bool_t HRichCorrelator::isCorrelated(HRichHit* pRichHit,HMdcSeg* pMdcSeg)
{
    // corr RICH - MDC
    Float_t rad2deg = 180./TMath::Pi();
    Int_t rs = pRichHit->getSector();
    Int_t ms = pMdcSeg->getSec();
    Float_t rt = pRichHit->getTheta();
    Float_t mt = correctMdcSegTheta(pMdcSeg->getTheta()*rad2deg);
    Float_t rp = pRichHit->getPhi();
    Float_t mp = (pMdcSeg->getPhi())*rad2deg + correctMdcSegPhi(ms);

    if ( rs == ms)
    {
	// asymmetric window
	Float_t fDeltaTheta = rt - mt;
	if ( fRichMdcThetaMinCut<fDeltaTheta &&
	     fRichMdcThetaMaxCut>fDeltaTheta &&
	     TMath::Abs(rp - mp) < fRichMdcPhiCut      )

	{
	    return kTRUE;
	}
    }
    
    return kFALSE;
}

Bool_t HRichCorrelator::isCorrelated(HRichHit* pRichHit, HHitMatch* pMatchHit)
{
  // is RICH corr with this already MATCHed HIT (MDC part) 
    // not used !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    Int_t rs = pRichHit->getSector();
    Int_t ms = pMatchHit->getSector();
    Float_t rt = pRichHit->getTheta();
    Float_t mt = pMatchHit->getMdcTheta();
    Float_t rp = pRichHit->getPhi();
    Float_t mp = pMatchHit ->getMdcPhi();

    if ( rs == ms)
    {
	
	// asymmetric window
	Float_t fDeltaTheta = rt - mt;
	if ( fRichMdcThetaMinCut < fDeltaTheta &&
	     fRichMdcThetaMaxCut > fDeltaTheta &&
	     TMath::Abs(rp - mp) < fRichMdcPhiCut      )

	{
	    return kTRUE;
	}
    }
    
    return kFALSE;
}

Bool_t HRichCorrelator::isCorrelated(HRichHit* pRichHit,HTofHit* pTofHit)
{
    // corr RICH TOF 
    Int_t rs = pRichHit->getSector();
    Float_t rt = pRichHit->getTheta();
    Float_t rp = pRichHit->getPhi();
    Int_t ts = (Int_t) pTofHit->getSector();
    Float_t tt = 0.; pTofHit->getTheta(tt);
    Float_t tp = 0.; pTofHit->getPhi(tp);
    Float_t tof = pTofHit->getTof();

    if ( rs == ts)
    {   // check if user wants to cut on T.O.F, apply cut
	if (kIsNoCutOnTOFtof || (tof>fTofTofMinCut && tof<fTofTofMaxCut))
	{
	    if(kMagnet){// if magnet is ON, check only phi corr
		if (TMath::Abs(rp - tp) < fRichTofPhiCut)
		{
		    
		    return kTRUE; 
		}
	    }
	    else{
		Float_t fDeltaTheta = rt - tt;
		if ( fRichTofThetaMinCut < fDeltaTheta &&
		     fRichTofThetaMaxCut > fDeltaTheta &&
		     TMath::Abs(rp - tp) < fRichTofPhiCut      )
		{
		    
		    return kTRUE; 
		}
	    }
	}
    }
    return kFALSE;
}

Bool_t HRichCorrelator::isShower(HShowerHitTof* pSHT)
{
    // information by J. Otwinowski
    // check whether there was a shower indicating a lepton
    Int_t nModule=(Int_t) pSHT->getModule();
    
    Float_t fSum0 = pSHT->getSum(0);
    Float_t fSum1 = pSHT->getSum(1);
    Float_t fSum2 = pSHT->getSum(2);

    if (nModule==0) 
    {
	if (fSum1/fSum0 > fShowerAmplification ||
	    fSum2/fSum0 > fShowerAmplification)
	{
	    return kTRUE; // is a valid (lepton ?) shower
	}
    }
    return kFALSE;
}

Bool_t HRichCorrelator::isCorrelated(HRichHit* pRichHit,HShowerHitTof* pShowerHitTof)
{
    // corr RICH SHOWERHITTOF
    Int_t rs    = pRichHit->getSector();
    Float_t rt  = pRichHit->getTheta();
    Float_t rp  = pRichHit->getPhi();
    Int_t   sts = (Int_t) pShowerHitTof->getSector();
    Float_t dmmy= 0.;
    Float_t stt = 0.;
    Float_t stp = 0.;
    pShowerHitTof->getSphereCoord(&dmmy,&stp,&stt);
    Float_t tof = pShowerHitTof->getTof();
    Int_t nTofinoMult = pShowerHitTof->getTofinoMult();
    Bool_t kTofino = kFALSE;
    if (kTofinoMultCut) // do we want a tofino mult check ?
    {
	if (nTofinoMult==nTofinoMultCut)  kTofino=kTRUE;
	else kTofino=kFALSE;
    }
    else kTofino=kTRUE;
    if ( rs == sts && kTofino)
    {
	if (kIsNoCutOnTOFINOtof || // check if we want to cut on T.O.F of TOFINO
	    (tof>fTofinoTofMinCut && tof<fTofinoTofMaxCut))// apply cut
	{
	    if(kMagnet){// if magnet is on cut only on phi 
		        // (azimuthal angle unchanged by mag field in 1st approx)
		if ( TMath::Abs(rp - stp) < fRichShowerPhiCut  )
		{
		    if (kShowerCond)// do we require a valid lepton shower ?
		    {
			if(isShower(pShowerHitTof)) return kTRUE;
			else return kFALSE;
		    }
		    else return kTRUE;
		}
	    }
	    else{
		Float_t fDeltaTheta = rt - stt;
		if ( fRichShowerThetaMinCut < fDeltaTheta &&
		     fRichShowerThetaMaxCut > fDeltaTheta &&
		     TMath::Abs(rp - stp) < fRichShowerPhiCut      )
		{
		    if (kShowerCond)
		    {
			if(isShower(pShowerHitTof)) return kTRUE;
			else return kFALSE;
		    }
		    else return kTRUE;
		}
	    }
	}
    }
    return kFALSE;

}

Bool_t HRichCorrelator::isCorrelated(HMdcSeg* pMdcSeg,HShowerHitTof* pShowerHitTof)
{
    // corr MDCSEG - SHOWERHITTOF
    Float_t rad2deg = 180./TMath::Pi();
    Int_t ms = pMdcSeg->getSec();
    Float_t mt = correctMdcSegTheta(pMdcSeg->getTheta()*rad2deg);
    Float_t mp = (pMdcSeg->getPhi())*rad2deg + correctMdcSegPhi(ms);
    Int_t   sts = (Int_t) pShowerHitTof->getSector();
    Float_t dmmy= 0.;
    Float_t stt = 0.;
    Float_t stp = 0.;
    pShowerHitTof->getSphereCoord(&dmmy,&stp,&stt);
    Float_t tof = pShowerHitTof->getTof();
    Int_t nTofinoMult = pShowerHitTof->getTofinoMult();
    Bool_t kTofino = kFALSE;
    if (kTofinoMultCut)
    {
	if (nTofinoMult==nTofinoMultCut)  kTofino=kTRUE;
	else kTofino=kFALSE;
    }
    else kTofino=kTRUE;
    if ( ms == sts && kTofino)   
    {
	
	if (kIsNoCutOnTOFINOtof || 
	  (tof>fTofinoTofMinCut && tof<fTofinoTofMaxCut))
	{
	    if(kMagnet){
		if (  TMath::Abs(mp - stp) < fMdcShowerPhiCut      )
		{
		    if (kShowerCond)
		    {
			if(isShower(pShowerHitTof)) return kTRUE;
			else return kFALSE;
		    }
		    else return kTRUE;
		}
	    }
	    else{
		Float_t fDeltaTheta = mt - stt;
		// set for polar angle variable asymmetric windows 
		// according to the misalignment !
		if ( fMdcShowerThetaMinCut < fDeltaTheta &&
		     fMdcShowerThetaMaxCut > fDeltaTheta &&
		     TMath::Abs(mp - stp) < fMdcShowerPhiCut      )
		{
		    if (kShowerCond)
		    {
			if(isShower(pShowerHitTof)) return kTRUE;
			else return kFALSE;
		    }
		    else return kTRUE;
		}
	    }
	}
    }
    return kFALSE;
}

Bool_t HRichCorrelator::isCorrelated(HMdcSeg* pMdcSeg,HTofHit* pTofHit)
{
    // corr MDCSEG - TOF
    Float_t rad2deg = 180./TMath::Pi();
    Int_t ms = pMdcSeg->getSec();
    Float_t mt = correctMdcSegTheta(pMdcSeg->getTheta()*rad2deg);
    Float_t mp = (pMdcSeg->getPhi())*rad2deg + correctMdcSegPhi(ms);
    Int_t ts = (Int_t) pTofHit->getSector();
    Float_t tt = 0.; pTofHit->getTheta(tt);
    Float_t tp = 0.; pTofHit->getPhi(tp);
    Float_t tof = pTofHit->getTof();
    if ( ms == ts)
    {
	if (kIsNoCutOnTOFtof || (tof>fTofTofMinCut && tof<fTofTofMaxCut))
	{
	    if(kMagnet){
		if (TMath::Abs(mp - tp) < fMdcTofPhiCut      )
		{
		    return kTRUE; 
		}
	    }
	    else{
		Float_t fDeltaTheta = mt - tt;
		if ( fMdcTofThetaMinCut < fDeltaTheta &&
		     fMdcTofThetaMaxCut > fDeltaTheta &&
		     TMath::Abs(mp - tp) < fMdcTofPhiCut      )
		{
		    return kTRUE; 
		}
	    }
	}
    }
    return kFALSE;

}

void HRichCorrelator::dumpCorrelationStatus()
{
    // FIXME: ring counters are unchecked and maybe not up-to-date
    cout<<"*******************************************"<<endl;
    cout<<"*** INFORMATION ***************************"<<endl;
    cout<<"*******************************************"<<endl;
    cout<<"this status information belongs to the tree in ..."<<endl;
    cout<<((TFile*)gHades->getOutputFile())->GetName()<<endl;
    TString filename(((TFile*)gHades->getOutputFile())->GetName());
    filename.Remove(filename.Length()-5, filename.Length());
    filename.Append(".stat");
    std::ofstream statusOut(filename.Data(),ios::out);
    cout<<"Nb of evts processed   : "<<nCounterProcessedNbEvents<<endl;
    cout<<"Nb of tracklets created: "<<ngNbCorrObjs<<endl;
    if(ngRichCnt>0)     cout<<"Tot Nb of RICH hits   : "<<ngRichCnt<<endl;
    if(ngMdcSegCnt>0)   cout<<"Tot Nb of MDCSEG hits : "<<ngMdcSegCnt<<endl;
    if(ngTofCnt>0)      cout<<"Tot Nb of TOF hits    : "<<ngTofCnt<<endl;
    if(ngShowerCnt>0)   cout<<"Tot Nb of SHOWER hits : "<<ngShowerCnt<<endl;
    if(ngDRichCnt>0)    cout<<"Nb of different RICH hits in tracklets   : "
			    <<ngDRichCnt<<endl;
    if(ngDMdcSegCnt>0)  cout<<"Nb of different MDCSEG hits in tracklets : "
			    <<ngDMdcSegCnt<<endl;
    if(ngDTofCnt>0)     cout<<"Nb of different TOF hits in tracklets    : "
			    <<ngDTofCnt<<endl;
    if(ngDShowerCnt>0)  cout<<"Nb of different SHOWER hits in tracklets : "
			    <<ngDShowerCnt<<endl;
    if(ngNb_RM>0)       cout<<"Nb of only RM corr : "<<ngNb_RM<<endl;
    if(ngNb_RT>0)       cout<<"Nb of only RT corr : "<<ngNb_RT<<endl;
    if(ngNb_RS>0)       cout<<"Nb of only RS corr : "<<ngNb_RS<<endl;
    if(ngNb_MT>0)       cout<<"Nb of only MT corr : "<<ngNb_MT<<endl;
    if(ngNb_MS>0)       cout<<"Nb of only MS corr : "<<ngNb_MS<<endl;
    if(ngRTM>0)         cout<<"Nb of min RMT indexes in tracklet : "
			    <<ngRTM<<endl;
    if(ngRSM>0)         cout<<"Nb of min RMS indexes in tracklet : "
			    <<ngRSM<<endl;
    if(ngNb_RcMcT>0)    cout<<"Nb of exact RM MT    corr : "<<ngNb_RcMcT<<endl;
    if(ngNb_RcMcTcR>0)  cout<<"Nb of exact RM MT RT corr : "<<ngNb_RcMcTcR<<endl;
    if(ngNb_RMcTcR>0)   cout<<"Nb of exact    MT RT corr : "<<ngNb_RMcTcR<<endl;
    if(ngNb_RcMTcR>0)   cout<<"Nb of exact RM    RT corr : "<<ngNb_RcMTcR<<endl;
    if(ngNb_RcMcS>0)    cout<<"Nb of exact RM MS    corr : "<<ngNb_RcMcS<<endl;
    if(ngNb_RcMcScR>0)  cout<<"Nb of exact RM MS RS corr : "<<ngNb_RcMcScR<<endl;
    if(ngNb_RMcScR>0)   cout<<"Nb of exact    MS RS corr : "<<ngNb_RMcScR<<endl;
    if(ngNb_RcMScR>0)   cout<<"Nb of exact RM    RS corr : "<<ngNb_RcMScR<<endl;

    statusOut<<"Nb of evts processed   : "<<nCounterProcessedNbEvents<<endl;
    statusOut<<"Nb of tracklets created: "<<ngNbCorrObjs<<endl;
    if(ngRichCnt>0)     statusOut<<"Tot Nb of RICH hits   : "<<ngRichCnt<<endl;
    if(ngMdcSegCnt>0)   statusOut<<"Tot Nb of MDCSEG hits : "<<ngMdcSegCnt<<endl;
    if(ngTofCnt>0)      statusOut<<"Tot Nb of TOF hits    : "<<ngTofCnt<<endl;
    if(ngShowerCnt>0)   statusOut<<"Tot Nb of SHOWER hits : "<<ngShowerCnt<<endl;
    if(ngDRichCnt>0)    statusOut<<"Nb of different RICH hits in tracklets   : "
			    <<ngDRichCnt<<endl;
    if(ngDMdcSegCnt>0)  statusOut<<"Nb of different MDCSEG hits in tracklets : "
			    <<ngDMdcSegCnt<<endl;
    if(ngDTofCnt>0)     statusOut<<"Nb of different TOF hits in tracklets    : "
			    <<ngDTofCnt<<endl;
    if(ngDShowerCnt>0)  statusOut<<"Nb of different SHOWER hits in tracklets : "
			    <<ngDShowerCnt<<endl;
    if(ngNb_RM>0)       statusOut<<"Nb of only RM corr : "<<ngNb_RM<<endl;
    if(ngNb_RT>0)       statusOut<<"Nb of only RT corr : "<<ngNb_RT<<endl;
    if(ngNb_RS>0)       statusOut<<"Nb of only RS corr : "<<ngNb_RS<<endl;
    if(ngNb_MT>0)       statusOut<<"Nb of only MT corr : "<<ngNb_MT<<endl;
    if(ngNb_MS>0)       statusOut<<"Nb of only MS corr : "<<ngNb_MS<<endl;
    if(ngRTM>0)         statusOut<<"Nb of min RMT indexes in tracklet : "
			    <<ngRTM<<endl;
    if(ngRSM>0)         statusOut<<"Nb of min RMS indexes in tracklet : "
			    <<ngRSM<<endl;
    if(ngNb_RcMcT>0)    statusOut<<"Nb of exact RM MT    corr : "<<ngNb_RcMcT<<endl;
    if(ngNb_RcMcTcR>0)  statusOut<<"Nb of exact RM MT RT corr : "<<ngNb_RcMcTcR<<endl;
    if(ngNb_RMcTcR>0)   statusOut<<"Nb of exact    MT RT corr : "<<ngNb_RMcTcR<<endl;
    if(ngNb_RcMTcR>0)   statusOut<<"Nb of exact RM    RT corr : "<<ngNb_RcMTcR<<endl;
    if(ngNb_RcMcS>0)    statusOut<<"Nb of exact RM MS    corr : "<<ngNb_RcMcS<<endl;
    if(ngNb_RcMcScR>0)  statusOut<<"Nb of exact RM MS RS corr : "<<ngNb_RcMcScR<<endl;
    if(ngNb_RMcScR>0)   statusOut<<"Nb of exact    MS RS corr : "<<ngNb_RMcScR<<endl;
    if(ngNb_RcMScR>0)   statusOut<<"Nb of exact RM    RS corr : "<<ngNb_RcMScR<<endl;


    //-------------------------
//     if(ngRichCnt>0)     statusOut<<"Nb of RICH  : "<<ngRichCnt<<endl;
//     if(ngMdcSegCnt>0)   statusOut<<"Nb of MDCSEG: "<<ngMdcSegCnt<<endl;
//     if(ngTofCnt>0)      statusOut<<"Nb of TOF   : "<<ngTofCnt<<endl;
//     if(ngShowerCnt>0)   statusOut<<"Nb of SHOWER: "<<ngShowerCnt<<endl;
//     if(ngDRichCnt>0)    statusOut<<"Nb of different RICH  : "<<ngDRichCnt<<endl;
//     if(ngDMdcSegCnt>0)  statusOut<<"Nb of different MDCSEG: "<<ngDMdcSegCnt<<endl;
//     if(ngDTofCnt>0)     statusOut<<"Nb of different TOF   : "<<ngDTofCnt<<endl;
//     if(ngDShowerCnt>0)  statusOut<<"Nb of different SHOWER: "<<ngDShowerCnt<<endl;
//     if(ngNb_RM>0)       statusOut<<"Nb of only RM : "<<ngNb_RM<<endl;
//     if(ngNb_RT>0)       statusOut<<"Nb of only RT : "<<ngNb_RT<<endl;
//     if(ngNb_RS>0)       statusOut<<"Nb of only RS : "<<ngNb_RS<<endl;
//     if(ngNb_MT>0)       statusOut<<"Nb of only MT : "<<ngNb_MT<<endl;
//     if(ngNb_MS>0)       statusOut<<"Nb of only MS : "<<ngNb_MS<<endl;
//     if(ngRTM>0)         statusOut<<"Nb of min RMT : "<<ngRTM<<endl;
//     if(ngRSM>0)         statusOut<<"Nb of min RMS : "<<ngRSM<<endl;
//     if(ngNb_RcMcT>0)    statusOut<<"Nb of RM MT   : "<<ngNb_RcMcT<<endl;
//     if(ngNb_RcMcTcR>0)  statusOut<<"Nb of RM MT RT: "<<ngNb_RcMcTcR<<endl;
//     if(ngNb_RMcTcR>0)   statusOut<<"Nb of    MT RT: "<<ngNb_RMcTcR<<endl;
//     if(ngNb_RcMTcR>0)   statusOut<<"Nb of RM    RT: "<<ngNb_RcMTcR<<endl;
//     if(ngNb_RcMcS>0)    statusOut<<"Nb of RM MS   : "<<ngNb_RcMcS<<endl;
//     if(ngNb_RcMcScR>0)  statusOut<<"Nb of RM MS RS: "<<ngNb_RcMcScR<<endl;
//     if(ngNb_RMcScR>0)   statusOut<<"Nb of    MS RS: "<<ngNb_RMcScR<<endl;
//     if(ngNb_RcMScR>0)   statusOut<<"Nb of RM    RS: "<<ngNb_RcMScR<<endl;
//     statusOut.close();
//     statusOut<<"*** Total nb of processed rings "<<nCounterTotalNbRings<<endl;
//     statusOut<<"*** Rings above 45 deg (TOF) "<<nCounterRingsAbove45<<endl;
//     statusOut<<"*** Rings with full RMT correlation "<<endl;
//     statusOut<<"*** Tracks with full RMT correlation "<<nCounterRingsCorrTof
// 	<<" ("<<nCounterRingsCorrTof/nCounterRingsAbove45<<"%)"<<endl;
//     statusOut<<"*** Rings below 45 deg (SHOWER) "<<nCounterRingsBelow45<<endl;
//     statusOut<<"*** Rings with full RMS correlation "<<endl;
//     statusOut<<"*** Tracks with full RMS correlation "<<nCounterRingsCorrShower
// 	<<" ("<<nCounterRingsCorrShower/nCounterRingsBelow45<<"%)"<<endl;
//     statusOut<<"*** Number of Rich Mdc correlations: "
// 	<<nCounterRingsCorrMdc
// 	<<" ("<<nCounterRingsCorrMdc/nCounterTotalNbRings<<"%)"<<endl;
//     statusOut<<"*******************************************"<<endl;
//     statusOut<<"*** Number of evts with minimum 2 rings "<<nCounterMin2Rings<<endl;
//     statusOut<<"*** Number of deleted (uncorr rings in corr evts) rings: "
// 	<<nCounterNbRejectedRings<<endl;
//     statusOut<<"*** Number of RMS || RMT evts written to file: "
// 	<<nCounterEvtWritten<<endl;
//     statusOut<<"*** Number of matched rings written to file: "
// 	<<nCounterRingsWritten<<endl;
//     statusOut<<"*** Number of matched rings with unresolved multiple Mdc hits: "
// 	<<nCounterRingsUnresMultHit<<endl;
//     statusOut<<"*******************************************"<<endl;
//     statusOut<<"*** Flags *********************************"<<endl;
//     statusOut<<"*** Charged particle shower required: ";
//     if (kShowerCond) statusOut<<"yes"<<endl;
//     else statusOut<<"no"<<endl;
//     statusOut<<"*** loop over TOF hits: ";
//     statusOut<<"*** sector selection: ";
//     if (kNoSectorSelected) statusOut<<"no"<<endl;
//     else statusOut<<" yes, sec: "<<nSelectedSector<<endl;
    
//     statusOut.close();
//     cout<<"*******************************************"<<endl;
//     cout<<"*** Total nb of processed rings "<<nCounterTotalNbRings<<endl;
//     cout<<"*** Rings above 45 deg (TOF) "<<nCounterRingsAbove45<<endl;
//     cout<<"*** Rings with full RMT correlation "<<endl;
//     cout<<"*** Tracks with full RMT correlation "<<nCounterRingsCorrTof
// 	<<" ("<<nCounterRingsCorrTof/nCounterRingsAbove45<<"%)"<<endl;
//     cout<<"*** Rings below 45 deg (SHOWER) "<<nCounterRingsBelow45<<endl;
//     cout<<"*** Rings with full RMS correlation "<<endl;
//     cout<<"*** Tracks with full RMS correlation "<<nCounterRingsCorrShower
// 	<<" ("<<nCounterRingsCorrShower/nCounterRingsBelow45<<"%)"<<endl;
//     cout<<"*** Number of Rich Mdc correlations: "
// 	<<nCounterRingsCorrMdc
// 	<<" ("<<nCounterRingsCorrMdc/nCounterTotalNbRings<<"%)"<<endl;
//     cout<<"*******************************************"<<endl;
//     cout<<"*** Number of evts with minimum 2 rings "<<nCounterMin2Rings<<endl;
//     cout<<"*** Number of deleted (uncorr rings in corr evts) rings: "
// 	<<nCounterNbRejectedRings<<endl;
//     cout<<"*** Number of RMS || RMT evts written to file: "
// 	<<nCounterEvtWritten<<endl;
//     cout<<"*** Number of matched rings written to file: "
// 	<<nCounterRingsWritten<<endl;
//     cout<<"*** Number of matched rings with unresolved multiple Mdc hits: "
// 	<<nCounterRingsUnresMultHit<<endl;
//     cout<<"*******************************************"<<endl;
//     cout<<"*** Flags *********************************"<<endl;
//     cout<<"*** Charged particle shower required: ";
//     cout<<"*** loop over SHOWER hits: ";
//     cout<<"*** sector selection: ";
//     if (kNoSectorSelected) cout<<"no"<<endl;
//     else cout<<" yes, sec: "<<nSelectedSector+1<<endl;
    // FIXME: store cuts in resulting ROOT file !!!!
}

TCanvas* HRichCorrelator::makeCanvas2()
{
    TCanvas *canvas = new TCanvas("Correlation_Plots2","Correlation_Plots2",10,10,600,500);
    canvas->Divide(4,3);
    canvas->SetLogy();
    canvas->cd(1);//HT profile vs polar angle
    pProfALLHTtheta->SetLineColor(2);
    pProfALLHTtheta->SetMaximum(nHouTraMax);
    pProfALLHTtheta->Draw();
    pProfRMTHTtheta->SetLineColor(6);
    pProfRMTHTtheta->Draw("same");
    pProfRMHTtheta->SetLineColor(8);
    pProfRMHTtheta->Draw("same");
    pProfRMSHTtheta->SetLineColor(4);
    pProfRMSHTtheta->Draw("same");
    canvas->cd(2);//PM profile vs polar angle
    pProfALLPMtheta->SetLineColor(2);
    pProfALLPMtheta->SetMaximum(800);
    pProfALLPMtheta->Draw();
    pProfRMTPMtheta->SetLineColor(6);
    pProfRMTPMtheta->Draw("same");
    pProfRMPMtheta->SetLineColor(8);
    pProfRMPMtheta->Draw("same");
    pProfRMSPMtheta->SetLineColor(4);
    pProfRMSPMtheta->Draw("same");
    canvas->cd(3);//Amplitude profile vs polar angle
    pProfALLRingAmplitudeTheta->SetLineColor(2);
    pProfALLRingAmplitudeTheta->SetMaximum(600);
    pProfALLRingAmplitudeTheta->Draw();
    pProfRMRingAmplitudeTheta->SetLineColor(8);
    pProfRMRingAmplitudeTheta->Draw("same");
    pProfRMSRingAmplitudeTheta->SetLineColor(4);
    pProfRMSRingAmplitudeTheta->Draw("same");
    pProfRMTRingAmplitudeTheta->SetLineColor(6);
    pProfRMTRingAmplitudeTheta->Draw("same");
    canvas->cd(4);//Loc Max4 profile vs polar angle
    pProfALLLocMax4Theta->SetLineColor(2);
    pProfALLLocMax4Theta->SetMaximum(15);
    pProfALLLocMax4Theta->Draw();
    pProfRMLocMax4Theta->SetLineColor(8);
    pProfRMLocMax4Theta->Draw("same");
    pProfRMSLocMax4Theta->SetLineColor(4);
    pProfRMSLocMax4Theta->Draw("same");
    pProfRMTLocMax4Theta->SetLineColor(6);
    pProfRMTLocMax4Theta->Draw("same");
    canvas->cd(5);//PadNr profile vs polar angle
    pProfALLPadNrTheta->SetLineColor(2);
    pProfALLPadNrTheta->SetMaximum(40);
    pProfALLPadNrTheta->Draw();
    pProfRMPadNrTheta->SetLineColor(8);
    pProfRMPadNrTheta->Draw("same");
    pProfRMSPadNrTheta->SetLineColor(4);
    pProfRMSPadNrTheta->Draw("same");
    pProfRMTPadNrTheta->SetLineColor(6);
    pProfRMTPadNrTheta->Draw("same");
    canvas->cd(6);
    pHistALLRingAmplitudeTheta->SetMarkerColor(2);
    pHistALLRingAmplitudeTheta->Draw();
    pHistRMRingAmplitudeTheta->SetMarkerColor(8);
    pHistRMRingAmplitudeTheta->Draw("same");
    canvas->cd(7);
    pHistALLRingPadNrTheta->SetMarkerColor(2);
    pHistALLRingPadNrTheta->Draw();
    pHistRMRingPadNrTheta->SetMarkerColor(8);
    pHistRMRingPadNrTheta->Draw("same");
    canvas->cd(8);
    pHistALLRingLocMax4Theta->SetMarkerColor(2);
    pHistALLRingLocMax4Theta->Draw();
    pHistRMRingLocMax4Theta->SetMarkerColor(8);
    pHistRMRingLocMax4Theta->Draw("same");
    canvas->cd(9);
    pHistALLRingHouTraTheta->SetMarkerColor(2);
    pHistALLRingHouTraTheta->Draw();
    pHistRMRingHouTraTheta->SetMarkerColor(8);
    pHistRMRingHouTraTheta->Draw("same");
    canvas->cd(10);
    pHistALLRingPatMatTheta->SetMarkerColor(2);
    pHistALLRingPatMatTheta->Draw();
    pHistRMRingPatMatTheta->SetMarkerColor(8);
    pHistRMRingPatMatTheta->Draw("same");
    canvas->Modified();
    canvas->Update();
    return canvas;
}
TCanvas* HRichCorrelator::makeCanvas1()
{
    //gStyle->SetLineWidth(3);
    TCanvas *canvas = new TCanvas("Correlation_Plots1","Correlation_Plots1",10,10,600,500);
    Int_t nLineWidth = 3;//in pixel
    canvas->Divide(3,3);
    canvas->SetLogy();
    canvas->cd(1);
    gPad->SetLogy();
    pHistALLRingHouTra->SetLineColor(2);
    pHistALLRingHouTra->SetLineWidth(nLineWidth);
    pHistALLRingHouTra->Draw();
    pHistRMTRingHouTra->SetLineColor(6);
    pHistRMTRingHouTra->SetLineWidth(nLineWidth);
    pHistRMTRingHouTra->Draw("same");
    pHistRMRingHouTra->SetLineColor(8);
    pHistRMRingHouTra->SetLineWidth(nLineWidth);
    pHistRMRingHouTra->Draw("same");
    pHistRMSRingHouTra->SetLineColor(4);
    pHistRMSRingHouTra->SetLineWidth(nLineWidth);
    pHistRMSRingHouTra->Draw("same");
    canvas->cd(2);
    gPad->SetLogy();
    pHistALLRingPatMat->SetLineColor(2);
    pHistALLRingPatMat->SetLineWidth(nLineWidth);
    pHistALLRingPatMat->Draw();
    pHistRMTRingPatMat->SetLineColor(6);
    pHistRMTRingPatMat->SetLineWidth(nLineWidth);
    pHistRMTRingPatMat->Draw("same");
    pHistRMRingPatMat->SetLineColor(8);
    pHistRMRingPatMat->SetLineWidth(nLineWidth);
    pHistRMRingPatMat->Draw("same");
    pHistRMSRingPatMat->SetLineColor(4);
    pHistRMSRingPatMat->SetLineWidth(nLineWidth);
    pHistRMSRingPatMat->Draw("same");
    canvas->cd(3);
    gPad->SetLogy();
    pHistALLRingAmplitude->SetLineColor(2);
    pHistALLRingAmplitude->SetLineWidth(nLineWidth);
    pHistALLRingAmplitude->Draw();
    pHistRMTRingAmplitude->SetLineColor(6);
    pHistRMTRingAmplitude->SetLineWidth(nLineWidth);
    pHistRMTRingAmplitude->Draw("same");
    pHistRMRingAmplitude->SetLineColor(8);
    pHistRMRingAmplitude->SetLineWidth(nLineWidth);
    pHistRMRingAmplitude->Draw("same");
    pHistRMSRingAmplitude->SetLineColor(4);
    pHistRMSRingAmplitude->SetLineWidth(nLineWidth);
    pHistRMSRingAmplitude->Draw("same");
    canvas->cd(4);
    gPad->SetLogy();
    pHistALLRingPadNr->SetLineColor(2);
    pHistALLRingPadNr->SetLineWidth(nLineWidth);
    pHistALLRingPadNr->Draw();
    pHistRMTRingPadNr->SetLineColor(6);
    pHistRMTRingPadNr->SetLineWidth(nLineWidth);
    pHistRMTRingPadNr->Draw("same");
    pHistRMRingPadNr->SetLineColor(8);
    pHistRMRingPadNr->SetLineWidth(nLineWidth);
    pHistRMRingPadNr->Draw("same");
    pHistRMSRingPadNr->SetLineColor(4);
    pHistRMSRingPadNr->SetLineWidth(nLineWidth);
    pHistRMSRingPadNr->Draw("same");
    canvas->cd(5);
    gPad->SetLogy();
    pHistALLRingLocMax4->SetLineColor(2);
    pHistALLRingLocMax4->SetLineWidth(nLineWidth);
    pHistALLRingLocMax4->Draw();
    pHistRMTRingLocMax4->SetLineColor(6);
    pHistRMTRingLocMax4->SetLineWidth(nLineWidth);
    pHistRMTRingLocMax4->Draw("same");
    pHistRMRingLocMax4->SetLineColor(8);
    pHistRMRingLocMax4->SetLineWidth(nLineWidth);
    pHistRMRingLocMax4->Draw("same");
    pHistRMSRingLocMax4->SetLineColor(4);
    pHistRMSRingLocMax4->SetLineWidth(nLineWidth);
    pHistRMSRingLocMax4->Draw("same");
//      canvas->cd(6);
//      pProfALLHTtheta->SetLineColor(2);
//      pProfALLHTtheta->SetMaximum(nHouTraMax);
//      pProfALLHTtheta->Draw();
//      pProfRMTHTtheta->SetLineColor(6);
//      pProfRMTHTtheta->Draw("same");
//      pProfRMHTtheta->SetLineColor(8);
//      pProfRMHTtheta->Draw("same");
//      pProfRMSHTtheta->SetLineColor(4);
//      pProfRMSHTtheta->Draw("same");
//      canvas->cd(7);
//      pProfALLPMtheta->SetLineColor(2);
//      pProfALLPMtheta->SetMaximum(800);
//      pProfALLPMtheta->Draw();
//      pProfRMTPMtheta->SetLineColor(6);
//      pProfRMTPMtheta->Draw("same");
//      pProfRMPMtheta->SetLineColor(8);
//      pProfRMPMtheta->Draw("same");
//      pProfRMSPMtheta->SetLineColor(4);
//      pProfRMSPMtheta->Draw("same");
//      canvas->cd(8);
    

    //  canvas->cd(8);
//      pHistALLRingAmplitudeTheta->SetLineColor(2);
//      pHistALLRingAmplitudeTheta->Draw();
//      canvas->cd(9);
//      pHistRMRingAmplitudeTheta->SetLineColor(2);
//      pHistRMRingAmplitudeTheta->Draw();
    canvas->Modified();
    canvas->Update();
    return canvas;
}








