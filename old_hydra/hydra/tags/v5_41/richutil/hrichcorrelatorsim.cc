// File: hrichcorrelatorsim.cc
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 01/08/21 19:51:49
//

#include "hrichcorrelatorsim.h"
#include "hgeantkine.h"
#include "hgeantrich.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hrichdetector.h"
#include "hmdcdetector.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hmatrixcatiter.h"
#include "hrichhit.h"
#include "hrichhitsim.h"
#include "hmdcseg.h"
#include "hmdcsegsim.h"
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
#include "hlinearcategory.h"
#include "hgeomvector2.h"
#include <fstream>
///////////////////////////////////////////////////////////
// HRichCorrelatorSim
//
// 
///////////////////////////////////////////////////////////

ClassImp(HRichCorrelatorSim)

HRichCorrelatorSim::HRichCorrelatorSim(Text_t *name,Text_t *title) :
  HRichCorrelator(name,title)
{
     
}
HRichCorrelatorSim::HRichCorrelatorSim(Text_t *name,Text_t *title, char* filename,Int_t sec) :
  HRichCorrelator(name,title)
{

    if (sec != -1)
    {
	nSelectedSector = sec-1; //sector should be given according to [1;6]
	                         //convention
	kNoSectorSelected = kFALSE;

    }
    else 
    {
	nSelectedSector = -1;
	kNoSectorSelected = kTRUE;// kTRUE switches the sector selection off
    }
    
    pFileName  = filename;
    //    cout<<pFileName->Data()<<endl;
}
HRichCorrelatorSim::HRichCorrelatorSim()
{

}


HRichCorrelatorSim::~HRichCorrelatorSim(void) 
{

}

Bool_t HRichCorrelatorSim::init() {
    if (gHades) {
	HEvent *event=gHades->getCurrentEvent();
	HRuntimeDb *rtdb=gHades->getRuntimeDb();
	HSpectrometer *spec=gHades->getSetup();
	if (event && rtdb) {
	    //	    HDetector *tof=spec->getDetector("Tof");
	    //HDetector *shower = spec->getDetector("Shower");
	    HMdcDetector *mdc =(HMdcDetector*)(spec->getDetector("Mdc"));
	    HRichDetector *rich = (HRichDetector*)spec->getDetector("Rich");
	    
	    // HGEANT KINE INFO
	    fGeantKineCat =(HLinearCategory*) event->getCategory(catGeantKine);
	    if (!fGeantKineCat) 
	    { 
		Error("HRichCorrelatorSim::init",
		      "no GEANT Kine category available");
	    }                
	    iter_kine = (HIterator *)fGeantKineCat ->MakeIterator("native");
	    // HGEANT RICH RAW INFO
	    fGeantRichCat =(HLinearCategory*) event
		->getCategory(catRichGeantRaw);
	    if (!fGeantRichCat) 
	    { 
		Error("HRichCorrelatorSim::init",
		      "no GEANT RICH RAW category available");
	    }                
	    iter_geantrichraw = (HIterator *) fGeantRichCat
		->MakeIterator("native");
//  	    fHGeantKine=event->getCategory(catGeantKine);
//  	    if (!fHGeantKine)
//  	    { 
//  		fHGeantKine = new HLinearCategory("HGeantKine");
//  		event->addCategory(catGeantKine,fHGeantKine,"HGeantKine"); 
//  	    }
//  	    iter = (HIterator *)fHGeantKine->MakeIterator("native"); 
	    
    
	    if (rich) { //Has the user set up a RICH?
		// RICH HIT container
		fRichPID=gHades->getCurrentEvent()->getCategory(catRichHit);
		if (!fRichPID) {
		    fRichPID=rich->buildMatrixCat("HRichHitSim",1);
		    
		    if (!fRichPID) return kFALSE;
		    else gHades->getCurrentEvent()
			     ->addCategory(catRichHit, fRichPID, "Rich");
		}
    
		fRichIter = (HIterator*) fRichPID->MakeIterator();
		if(!fRichIter) Error("init","no RichHitSim iter defined");

		//        //Setup output
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
		
	    }
	    
	    if (mdc) { //Has the user set up a MDC?
		fMdcSeg = gHades->getCurrentEvent()->getCategory(catMdcSeg);
		if (!fMdcSeg){
		    fMdcSeg = mdc->buildMatrixCategory("HMdcSegSim",0.5F);
		
		    if (!fMdcSeg) {
			Error("init","No MDC segment sim category defined");
			return kFALSE;
		    }
		}
		else {
		    gHades->getCurrentEvent()
			->addCategory(catMdcSeg,fMdcSeg,"Mdc");
		    fMdcSegIter=(HIterator *)fMdcSeg->MakeIterator();
		}
	    }
//  	    if (tof) { //Has the user set up a TOF?
//  		fTofHits=event->getCategory(catTofHit);
//  		if (!fTofHits) {
//  		    fTofHits=tof->buildCategory(catTofHit);
//  		    if (!fTofHits) {
//  			Error("init","No TOF input");
//  			return kFALSE;
//  		    }
//  		}
//  		fTofIter=(HIterator *)fTofHits->MakeIterator();
//  	    }
	    
//  	    if (shower) { //Has the user set up a Shower?

//  		//  fShowerHits=event->getCategory(catShowerHit);
//  //  		if (!fShowerHits) {
//  //  		    //fShowerHits = shower->buildCategory(catShowerHitTof);
//  //  		    if (!fShowerHits) {
//  //  			Error("init","No Shower input available");
//  //  			return kFALSE;
//  //  		    }
//  //  		}
//  //  		fShowerIter = (HIterator *)fShowerHits->MakeIterator();

//  		fShowerTofHits=event->getCategory(catShowerHitTof);
//  		if (!fShowerTofHits) {
//  		    //fShowerHits = shower->buildCategory(catShowerHitTof);
//  		    if (!fShowerTofHits) {
//  			Error("init","No Shower input available");
//  			return kFALSE;
//  		    }
//  		}
//  		fShowerTofIter = (HIterator *)fShowerTofHits->MakeIterator();
//  	    }
	    

	    
	  
	} else {
	    Error ("init","! (event && rtdb)");
	    return kFALSE; //! (event && rtdb)
	}
	iniCuts();
	iniSwitches();
        iniControlHistos();
	iniCounters();
	fMdcThetaCorrection=1.; // correction for Mdc Theta values
	pFileOut = new TFile(pFileName,"RECREATE");
	return kTRUE;
    } else {
	Error ("init","! (gHades)");
	return kFALSE; //! (gHades)		
    }
}

Int_t HRichCorrelatorSim::execute()
{
    //cout<<"in execute"<<endl;
    if (nCounterProcessedNbEvents != 0 &&
	nCounterProcessedNbEvents%10000==0) cout<<"********** "
						<<nCounterProcessedNbEvents
			      <<" EVTS PROCESSED **********"<<endl;
    HRichHitSim         *pRichHit     =0;
    //    HTofHit          *pTofHit      =0;
    //HShowerHitTof    *pShowerHitTof=0;
    HMdcSegSim          *pMdcSeg      =0;

    fRichIter->Reset();

    Int_t nCounterRingsInEvent=0;//local ring in event counter

    Bool_t kRichMdcCorr    = kFALSE;
//      Bool_t kMdcTofCorr     = kFALSE;
//      Bool_t kMdcShowerCorr  = kFALSE;
//      Bool_t kRichShowerCorr = kFALSE;
//      Bool_t kRichTofCorr    = kFALSE;

    TObjArray *pRichHitToBeDeleted = 
	new TObjArray(3);// expands automagically if necessary 
   



    while((pRichHit = (HRichHitSim *)fRichIter->Next()))
    {
	//iterateGeantRichRaw();
	Int_t nRichHitTrackNb = iterateKine(pRichHit);
	//	cout<<"in rich loop"<<endl;
	Bool_t kRichHitToBeDeleted = kTRUE;//delete this ring
	// switchable if-Statement to fill only for selected sector
	if ( kNoSectorSelected || isSelectedSector(pRichHit->getSector()))
	{
	    fillALLRingHistos(pRichHit);
	    nCounterRingsInEvent++;
	    if (nCounterRingsInEvent==2) nCounterMin2Rings++;
	    nCounterTotalNbRings++;
	    Float_t fRT = pRichHit->getTheta();
	    if (fRT>45.) nCounterRingsAbove45++;
	    else nCounterRingsBelow45++;
	    
	    Bool_t kRingAlreadyMatched = kFALSE;
	
	    HHitMatch *pHitMatch=0;

	    if(fMdcSegIter) fMdcSegIter->Reset();
	    else Error("execute","fMdcSegIter is null");

	    //cout<<"fMdcSegIter "<<fMdcSegIter<<endl;

	    while((pMdcSeg = (HMdcSegSim *)fMdcSegIter->Next()))
	    {
		Int_t nNbMdcSegTr=pMdcSeg->getNTracks();
		for (Int_t i=0;i<nNbMdcSegTr;i++)
		{
		    if (pMdcSeg->getTrack(i) == nRichHitTrackNb)
		    {
			cout<<nNbMdcSegTr<<" "<<pMdcSeg->getTrack(i)<<" "
			    <<(Int_t)pMdcSeg->getNTimes(i)<<endl;
		    }
		}
		//cout<<"in Seg Sim Loop"<<endl;
		if (isCorrelated(pRichHit,pMdcSeg))
		{
		    nCounterRingsCorrMdc++;
		    kRichHitToBeDeleted=kFALSE;//do not delete this ring
		    kRichMdcCorr=kTRUE;
		    
		    if (!kRingAlreadyMatched) 
		    {
			pHitMatch = createHitMatch(pRichHit->getSector());
			fillHitMatch(pHitMatch,pRichHit,pMdcSeg);
			kRingAlreadyMatched=kTRUE;
		    }
		    else
		    { // what if there are even more than 2 close tracks ?
			// now the last is taken as second !
			addMatchedMdcSeg(pHitMatch,pMdcSeg);
		    }
		    
		    //  if (kEvalShower)
//  		    {
//  			fShowerTofIter->Reset();
//  			while((pShowerHitTof = (HShowerHitTof *)
//  			       fShowerTofIter->Next()))
//  			{
//  			    if (isCorrelated(pRichHit,pShowerHitTof))
//  			    {
//  				fillHitMatch(pHitMatch,pRichHit,pShowerHitTof);
//  				kRichShowerCorr=kTRUE;
//  			    }
			    
//  			    if (isCorrelated(pMdcSeg,pShowerHitTof))
//  			    {
//  				fillHitMatch(pHitMatch,pMdcSeg,pShowerHitTof);
//  				nCounterRingsCorrShower++;
//  				kMdcShowerCorr=kTRUE;
//  			    }
//  			} // end while shower 
			
//  		    }
		    
		    



//  		    if (kEvalTof)
//  		    {
//  			fTofIter->Reset();
//  			while((pTofHit = (HTofHit *)fTofIter->Next()))
//  			{
//  			    if (isCorrelated(pRichHit,pTofHit))
//  			    {
//  				fillHitMatch(pHitMatch,pRichHit,pTofHit);
//  				kRichTofCorr=kTRUE;
//  			    }
			    
//  			    if (isCorrelated(pMdcSeg,pTofHit))
//  			    {
//  				fillHitMatch(pHitMatch,pMdcSeg,pTofHit);
//  				nCounterRingsCorrTof++;
//  				kMdcTofCorr=kTRUE;
//  			    }
//  			} // end while tof
			
//  		    }// end of eval tof
		    
		} // end if rich mdc corr 
		
		
		
	    } // end while mdc segment
	    
	} // end if sector selection switch
	
	if (kRichHitToBeDeleted) pRichHitToBeDeleted->Add(pRichHit); 
	// save address of ring to be deleted 
	
    }// end while rich
    
    
    fillControlHistos(); // use MatchHit objs to fill histos

    nCounterProcessedNbEvents++;



    // set condition for persistency here

    //    if ((kRichMdcCorr && kMdcShowerCorr) ||
    //       (kRichMdcCorr && kMdcTofCorr))
    if (kRichMdcCorr)
    {
	nCounterEvtWritten++;
	// if a correlation was found, the event (HRichHit)  
	// is made persistent,
	// before, however, the uncorrelated RICH hits are removed
	//
	for (Int_t i=0;i<(pRichHitToBeDeleted->GetLast()+1);i++)
	{
	    if(delRichHit((HRichHitSim*)(*pRichHitToBeDeleted)[i]) ){} 
		//cout<<"Ring deleted !!"<<endl;
	}

	//cout<<"correlation found, writing event "<<evtcntr<<endl;

	delete pRichHitToBeDeleted;
	return 0; // go on processing event data --> write evt to file
	
    }


    delete pRichHitToBeDeleted;
    return kSkipEvent; //skips writing out event to file


}

void HRichCorrelatorSim::dumpKine(HGeantKine *kine)
{
    Int_t aTrack, aID;
    Int_t aPar, aMed, aMech;
    Float_t ax, ay, az;
    Float_t apx, apy, apz;
    Float_t aInfo, aWeight;
    Float_t ptot;
    kine->getParticle(aTrack,aID);
    kine->getCreator(aPar,aMed,aMech);
    kine->getVertex(ax,ay,az);
    kine->getMomentum(apx,apy,apz);
    kine->getGenerator(aInfo,aWeight);
    ptot=kine->getTotalMomentum();
    cout<<"-----------------------------------------------------------"<<endl;
    cout<<"--- GEANT track number: "<<aTrack<<endl;
    cout<<"--- track number of parent particle: "<<aPar<<endl;
    cout<<"--- GEANT particle ID: "<<aID<<endl;
    cout<<"--- GEANT medium of creation: "<<aMed<<endl;
    cout<<"--- GEANT creation mechanism: "<<aMech<<endl;
    cout<<"--- x of track vertex (in mm): "<<ax<<endl;
    cout<<"--- y                        : "<<ay<<endl;
    cout<<"--- z                        : "<<az<<endl;
    cout<<"--- x component of particle momentum (in MeV/c): "<<apx<<endl;
    cout<<"--- y                                          : "<<apy<<endl;
    cout<<"--- z                                          : "<<apz<<endl;
    cout<<"--- total momentum                             : "<<ptot<<endl;
    cout<<"--- event generator info: "<<aInfo<<endl;
    cout<<"--- associated weight: "<<aWeight<<endl;
    cout<<"--- "<<endl;
    cout<<"--- "<<endl;
    cout<<"--- "<<endl;
    cout<<"--- "<<endl;
    cout<<"-----------------------------------------------------------"<<endl;
}

Bool_t HRichCorrelatorSim::isVertexInsideRich(Float_t x,Float_t y,Float_t z)
{
    if ((z>-40.0F && 
	 z<400.0F && 
	 x*x+y*y+(z+500.0F)*(z+500.0F) < 810000.0F))
    {
	return kTRUE;
    }
    else
    {
	return kFALSE;
    }
}
Bool_t HRichCorrelatorSim::isLepton(Int_t nParticleId)
{
    if (nParticleId == 2 ||
	nParticleId == 3   ) // GEANT particle ids for positron (2)
	                     // and electron (3)
    {
	return kTRUE;
    }
    else
    {
	return kFALSE;
    }
}
void HRichCorrelatorSim::iterateGeantRichRaw()
{
HGeantRichPhoton *geantrichraw=NULL;
iter_geantrichraw->Reset();
 while((geantrichraw=(HGeantRichPhoton *)iter_geantrichraw->Next())!=0)
 {
     cout<<geantrichraw->getTrack()<<endl;
 }
}
Int_t HRichCorrelatorSim::iterateKine(HRichHitSim* pHit)
{
    Int_t nTrackKine=0;
    Int_t nId=0;

    HGeantKine * kine =0;

    Float_t rt = pHit->getTheta();
    Float_t rp = pHit->getPhi();
    Int_t nGeantRichTrack1 = pHit->track1;
    Int_t nGeantRichTrack2 = pHit->track2;
    Int_t nGeantRichTrack3 = pHit->track3;
    Int_t nGeantRichWeight1 = pHit->weigTrack1;
    Int_t nGeantRichWeight2 = pHit->weigTrack2;
    Int_t nGeantRichWeight3 = pHit->weigTrack3;
    Float_t x,y,z;
    x = y = z  =0;

    iter_kine->Reset();

    while((kine=(HGeantKine *)iter_kine->Next())!=0){
	
	kine->getParticle(nTrackKine,nId);
	kine->getVertex(x,y,z);
	
	// is a lepton and is created inside the rich
	if ( isLepton(nId) ) 
	{
	    //	    cout<<"is Lepton !"<<endl;
	    if (isVertexInsideRich(x,y,z))
	    {
		//cout<<" has good vertex !"<<endl;
		Float_t fPartTheta=-1.;
		Float_t fPartPhi=-1.;
		calcParticleAngles(kine,fPartTheta,fPartPhi);
		// check if selected particle goes through ring
		if (isInsideRing(fPartTheta,fPartPhi,rt,rp))
		{
		    // check if found particle has same track nb
		    // as photon source of the ring
		    if (nTrackKine==nGeantRichTrack1 ||
			nTrackKine==nGeantRichTrack2 ||
			nTrackKine==nGeantRichTrack3)
		    {
			cout<<" Rich  theta: "<<rt<<"  Rich phi: "<<rp<<endl;
			cout<<" Geant theta: "<<fPartTheta
			    <<" Geant phi: "<<fPartPhi<<endl;
			cout<<" Rich track1: "<<nGeantRichTrack1
			    <<" Rich track2: "<<nGeantRichTrack2
			    <<" Rich track3: "<<nGeantRichTrack3<<endl;
			cout<<" Rich weight1: "<<nGeantRichWeight1
			    <<" Rich weight2: "<<nGeantRichWeight2
			    <<" Rich weight3: "<<nGeantRichWeight3<<endl;
			//			<<" Geant track: "<<nTrackKine<<endl;
			dumpKine(kine);
			// set a flag ?? what to do with the info ?
		    }
		    else
		    {
			// ring was not created by this particle
		    }
		    return nTrackKine;
		} //endif insideRing
	    } //endif vertex
	} // endif particle id
    } // end while loop
    return 0;//no matching particle found in KINE --> a fake !
    
}

Bool_t HRichCorrelatorSim::isInsideRing(Float_t pt,Float_t pp,
					Float_t rt,Float_t rp)
{
    // FIXME !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    const Float_t correctRichTheta = 2.; // GEANT geometry problem ??
    // FIXME !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    const Float_t fRingRadius = 5.; // window in degrees
    if(TMath::Abs(rt+correctRichTheta-pt)<fRingRadius && 
       TMath::Abs(rp-pp)<fRingRadius) return kTRUE;
    else return kFALSE;
}
void HRichCorrelatorSim::calcParticleAngles(HGeantKine *kine,Float_t &fpt, Float_t &fpp)
{
    //dumpKine(kine);
    Float_t rad2deg = 180./TMath::Pi();
    Float_t pX,pY,pZ;
    pX=pY=pZ=0;
    kine->getMomentum(pX,pY,pZ);
    Float_t pT = TMath::Sqrt(pX*pX+pY*pY);
    Float_t pTot = kine->getTotalMomentum();
    fpt = TMath::ASin(pT/pTot) * rad2deg; //theta (polar) of particle
    fpp = TMath::ASin(pY/pT) *rad2deg; //phi (azimuth) of particle
    if (pX<0.) fpp = 180.-fpp;
    else if (pY<0.) fpp+=360.;
    //cout<<" Geant theta: "<<fpt
    //<< "Geant phi: "<<fpp<<endl;

}
Bool_t HRichCorrelatorSim::finalize() {
    //cout<<"in finalize"<<endl;
    pProfRMTHTtheta=pHistRMTRingHouTraTheta->
	ProfileX("prof_RMT_HTtheta",0,nNbBinsForHouTra);
    pProfRMSHTtheta=pHistRMSRingHouTraTheta->
	ProfileX("prof_RMS_HTtheta",0,nNbBinsForHouTra);
    pProfRMTPMtheta=pHistRMTRingPatMatTheta->
	ProfileX("prof_RMT_PMtheta",0,nNbBinsForPatMat);
    pProfRMHTtheta=pHistRMRingHouTraTheta->
	ProfileX("prof_RM_HTtheta",0,nNbBinsForHouTra);
    pProfRMPMtheta=pHistRMRingPatMatTheta->
	ProfileX("prof_RM_PMtheta",0,nNbBinsForPatMat);
    pProfRMSPMtheta=pHistRMSRingPatMatTheta->
	ProfileX("prof_RMS_PMtheta",0,nNbBinsForPatMat);
    pProfALLHTtheta=pHistALLRingHouTraTheta->
	ProfileX("prof_ALL_HTtheta",0,nNbBinsForHouTra);
    pProfALLPMtheta=pHistALLRingPatMatTheta->
	ProfileX("prof_ALL_PMtheta",0,nNbBinsForPatMat);
    

    pProfRMPadNrTheta=pHistRMRingPadNrTheta->
	ProfileX("prof_RM_PadNrTheta",0,40);
    pProfRMTPadNrTheta=pHistRMTRingPadNrTheta->
	ProfileX("prof_RMT_PadNrTheta",0,40);
    pProfRMSPadNrTheta=pHistRMSRingPadNrTheta->
	ProfileX("prof_RMS_PadNrTheta",0,40);
    pProfALLPadNrTheta=pHistALLRingPadNrTheta->
	ProfileX("prof_ALL_PadNrTheta",0,40);

    pProfRMLocMax4Theta=pHistRMRingLocMax4Theta->
	ProfileX("prof_RM_LocMax4Theta",0,15);
    pProfRMTLocMax4Theta=pHistRMTRingLocMax4Theta->
	ProfileX("prof_RMT_LocMax4Theta",0,15);
    pProfRMSLocMax4Theta=pHistRMSRingLocMax4Theta->
	ProfileX("prof_RMS_LocMax4Theta",0,15);
    pProfALLLocMax4Theta=pHistALLRingLocMax4Theta->
	ProfileX("prof_ALL_LocMax4Theta",0,15);

    pProfRMRingAmplitudeTheta=pHistRMRingAmplitudeTheta->
	ProfileX("prof_RM_RingAmplitudeTheta",0,1000);
    pProfRMTRingAmplitudeTheta=pHistRMTRingAmplitudeTheta->
	ProfileX("prof_RMT_RingAmplitudeTheta",0,1000);
    pProfRMSRingAmplitudeTheta=pHistRMSRingAmplitudeTheta->
	ProfileX("prof_RMS_RingAmplitudeTheta",0,1000);
    pProfALLRingAmplitudeTheta=pHistALLRingAmplitudeTheta->
	ProfileX("prof_ALL_RingAmplitudeTheta",0,1000);
    // go to output file
    pFileOut->cd();
    
    // create canvas
    TCanvas *canvas1 = makeCanvas1();
    canvas1->Write();
    TCanvas *canvas2 = makeCanvas2();
    canvas2->Write();
    // write histograms
    pProfRMRingAmplitudeTheta->Write();
    pProfRMTRingAmplitudeTheta->Write();
    pProfRMSRingAmplitudeTheta->Write();
    pProfALLRingAmplitudeTheta->Write();
    
    pProfALLPadNrTheta->Write();
    pProfRMPadNrTheta->Write();
    pProfRMSPadNrTheta->Write();
    pProfRMTPadNrTheta->Write();

    pProfALLLocMax4Theta->Write();
    pProfRMLocMax4Theta->Write();
    pProfRMSLocMax4Theta->Write();
    pProfRMTLocMax4Theta->Write();

    pProfRMTHTtheta->Write();
    pProfRMSHTtheta->Write();
    pProfRMTPMtheta->Write();
    pProfRMSPMtheta->Write();
    pProfRMHTtheta->Write();
    pProfRMPMtheta->Write();
    pProfALLHTtheta->Write();
    pProfALLPMtheta->Write();
    pHistRMTheta->Write();
    pHistRMPhi->Write();
    pHistDiffRMTheta->Write();
    pHistDiffRMPhi->Write();
    pHistRSTheta->Write();
    pHistRSPhi->Write();
    pHistDiffRSTheta->Write();
    pHistDiffRSPhi->Write();
    pHistRTTheta->Write();
    pHistRTPhi->Write();
    pHistDiffRTTheta->Write();
    pHistDiffRTPhi->Write();
    
    pHistMTTheta->Write();
    pHistMTPhi->Write();
    pHistDiffMTTheta->Write();
    pHistDiffMTPhi->Write();
    
    pHistMSTheta->Write();
    pHistMSPhi->Write();
    pHistDiffMSTheta->Write();
    pHistDiffMSPhi->Write();
    pHistRMTRingHouTra ->Write();
    pHistRMTRingPatMat ->Write();
    pHistRMTRingAmplitude->Write(); 
    pHistRMTRingPadNr->Write();
    pHistRMTRingLocMax4->Write();
    pHistRMTRingRadius->Write();
    pHistRMTRingCenterX->Write();
    pHistRMTRingCenterY->Write();
    pHistRMTRingTheta->Write();
    pHistRMTRingPhi->Write();
    pHistRMSRingHouTra ->Write();
    pHistRMSRingPatMat ->Write();
    pHistRMSRingAmplitude->Write(); 
    pHistRMSRingPadNr->Write();
    pHistRMSRingLocMax4->Write();
    pHistRMSRingRadius->Write();
    pHistRMSRingCenterX->Write();
    pHistRMSRingCenterY->Write();
    pHistRMSRingTheta->Write();
    pHistRMSRingPhi->Write();
    pHistRMTRingHouTraTheta->Write();
    pHistRMTRingPatMatTheta->Write();
    pHistRMSRingHouTraTheta->Write();
    pHistRMSRingPatMatTheta->Write();
    pHistALLRingHouTra ->Write();
    pHistALLRingPatMat ->Write();
    pHistALLRingAmplitude->Write(); 
    pHistALLRingPadNr->Write();
    pHistALLRingLocMax4->Write();
    pHistALLRingRadius->Write();
    pHistALLRingCenterX->Write();
    pHistALLRingCenterY->Write();
    pHistALLRingTheta->Write();
    pHistALLRingPhi->Write();
    pHistALLRingHouTraTheta->Write();
    pHistALLRingPatMatTheta->Write();
    pHistRMRingHouTra ->Write();
    pHistRMRingPatMat ->Write();
    pHistRMRingAmplitude->Write(); 
    pHistRMRingPadNr->Write();
    pHistRMRingLocMax4->Write();
    pHistRMRingRadius->Write();
    pHistRMRingCenterX->Write();
    pHistRMRingCenterY->Write();
    pHistRMRingTheta->Write();
    pHistRMRingPhi->Write();
    pHistRMRingHouTraTheta->Write();
    pHistRMRingPatMatTheta->Write();
    pHistRMTRingAmplitudeTheta->Write();
    pHistRMRingAmplitudeTheta->Write();
    pHistRMSRingAmplitudeTheta->Write();
    pHistALLRingAmplitudeTheta->Write();
    pHistRMTRingPadNrTheta->Write();
    pHistRMRingPadNrTheta->Write();
    pHistRMSRingPadNrTheta->Write();
    pHistALLRingPadNrTheta->Write();
    pHistRMTRingLocMax4Theta->Write();
    pHistRMRingLocMax4Theta->Write();
    pHistRMSRingLocMax4Theta->Write();
    pHistALLRingLocMax4Theta->Write();
    //close file 
    pFileOut->Close();

    dumpCorrelationStatus();

    return kTRUE;
}


