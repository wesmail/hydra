// File: hrichevtfilter.cc
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 02/02/25 18:00:18
//

#include "hrichevtfilter.h"
#include "hlinearcategory.h"
#include "hmatrixcategory.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hrichdetector.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hrichhit.h"
#include "hades.h"
#include "richdef.h"
#include "hmdcdef.h"
#include "hmdcseg.h"
#include "tofinodef.h"
#include "htofhitsim.h"
#include "tofdef.h"
#include "hhitmatch.h"
#include "hhitmatchheader.h"
#include <iostream.h>
ClassImp(HRichEvtFilter)

HRichEvtFilter::HRichEvtFilter(Text_t *name,Text_t *title,Int_t filt) :
  HReconstructor(name,title)
{
    nFilter=filt;
}

HRichEvtFilter::HRichEvtFilter()
{

}


HRichEvtFilter::~HRichEvtFilter(void) {
 
}

Bool_t HRichEvtFilter::init() {
    if (gHades) {
	HEvent *event=gHades->getCurrentEvent();
	HRuntimeDb *rtdb=gHades->getRuntimeDb();
	HSpectrometer *spec=gHades->getSetup();
	if (event && rtdb) {
	    HDetector *tofino = spec->getDetector("Tofino");
	    if (tofino){
		fTofinoCal = event->getCategory(catTofinoCal);
	    }

	    HDetector *tof=spec->getDetector("Tof");
	    if (tof) { // TOF
		fTofHits=event->getCategory(catTofHit);
		if (!fTofHits) {
		    Error("init","No TOF input");
		    return kFALSE;
		}
	    }
	    fTofIter=(HIterator *)fTofHits->MakeIterator();

	    HDetector *mdc = spec->getDetector("Mdc");
	    if (mdc) { // MDC
		
		fMdcSeg=event->getCategory(catMdcSeg);
		if (!fMdcSeg) {
		    Error("init","No MDC segment category defined");
		    return kFALSE;
		}
		else fMdcSegIter=(HIterator *)fMdcSeg->MakeIterator();
	    }

	    HDetector *rich = spec->getDetector("Rich");
	    
	    if (rich) { //Has the user set up a RICH?
		// RICH HIT container
 		pHitRichCat=gHades->getCurrentEvent()->getCategory(catRichHit);
// 		if (!pHitRichCat) {
// 		    pHitRichCat=rich->buildCategory(catRichHit);
		    
// 		    if (!pHitRichCat) return kFALSE;
// 		    else gHades->getCurrentEvent()
// 			     ->addCategory(catRichHit, pHitRichCat, "Rich");
// 		}
                if (!pHitRichCat) Warning("HRichEvtFilter","init():no Rich Hit cat");
 		if (pHitRichCat) fRichIter = (HIterator*) pHitRichCat->MakeIterator();


		//        //Setup output
		pHitMatchCat=event->getCategory(catMatchHit);
//  		if (!pHitMatchCat) {
//  		    pHitMatchCat=rich->buildCategory(catMatchHit);
		    
//  		    if (!pHitMatchCat) {
//  			Error("init","No HIT MATCH category defined");
//  			return kFALSE;
//  		    }
//  		    else event->addCategory(catMatchHit, pHitMatchCat, "Rich");
//  		}
		
		if (pHitMatchCat) pIterMatchHit = (HIterator*)getHitMatchCat()->MakeIterator("native");
		

		pHitMatchHeaderCat=event->getCategory(catMatchHitHeader);
//  		if (!pHitMatchHeaderCat) {
//  		    pHitMatchHeaderCat=rich->buildCategory(catMatchHitHeader);
		    
//  		    if (!pHitMatchHeaderCat) {
//  			Error("init","No HIT MATCH HEADER category defined");
//  			return kFALSE;
//  		    }
//  		    else event->addCategory(catMatchHitHeader, pHitMatchHeaderCat, "Rich");
//  		}
		
		if (pHitMatchHeaderCat) pIterMatchHitHeader = (HIterator*)getHitMatchHeaderCat() -> MakeIterator("native");
	    }
	
	}
    }
    resetCounters();
    return kTRUE;
}

Bool_t HRichEvtFilter::finalize() {
    cout<<"Number of rejected evts: "<<nCounterNbRejectedEvts<<endl;
    cout<<"Number of remaining evts: "<<nCntProcessedEvents-nCounterNbRejectedEvts<<endl;
    cout<<"Percentage of rejected evts: "<<(((Float_t)nCounterNbRejectedEvts)/((Float_t)nCntProcessedEvents))*100<<"%"<<endl;
    return kTRUE;
}

Int_t HRichEvtFilter::execute()
{
    // refer to isFilteredEvt(swt) for filter options
    nCntProcessedEvents++;

    Int_t kReturnValue=0;

    if (isFilteredEvt(nFilter)) 
    {
	nCounterNbRejectedEvts++;
	//cout<<nCntProcessedEvents<<"  event is skipped"<<endl;
	kReturnValue = kSkipEvent;
    }
    else if (isSkipNextTask(0)) kReturnValue = 1;


    return kReturnValue;
}

void HRichEvtFilter::resetCounters()
{
    nCntProcessedEvents=0;
    nCounterNbRejectedEvts=0;
}

Bool_t HRichEvtFilter::isFilteredEvt(Int_t swt)
{

    // if function returns kTRUE evt is skipped
    Bool_t kSwitch=kFALSE;
    
    switch (swt)
    {
    case 0:
	if (!isDoubleRingEvt()) kSwitch=kTRUE;
	break;
	
    case 1:
	if (!containsRM()) kSwitch=kTRUE;
	break;
      
    case 2:
	if (!isTargetEvt()) kSwitch=kTRUE;
	break;
	
    case 3:
	if (!containsRichHit()) kSwitch=kTRUE;
	break;

    case 4:
	if (!isTofMultTrigger()) kSwitch=kTRUE;
	break;

    case 5:
	if (!containsCorrRichHit()) kSwitch=kTRUE;
	break;

    default:
	{
	    Error("isFilteredEvt","no valid filter function");//this is dirty
	    kSwitch=kFALSE;
	}
    }
    return kSwitch;
}

Bool_t HRichEvtFilter::isSkipNextTask(Int_t swt)
{
    Bool_t kSwitch=kFALSE;
    
    switch (swt)
    {
    case 0:
	
	break;
	
    default:
	kSwitch=kFALSE;
	
    }
    return kSwitch;
}

Bool_t HRichEvtFilter::containsRM()
{
    // should be easier by a call to getNrPart() after
    // updateHeader();
    Bool_t r=kFALSE;
    HHitMatch *pHM=0;

    pIterMatchHit->Reset();
    Int_t nC=0;
    while(( pHM= (HHitMatch *)pIterMatchHit->Next()))
    {
	if (pHM->getMatchedRichMdc()==1) nC++;
    }
    if (nC==0)
    {
	cout<<"/////////////////// RINGS  ///////////////////"<<endl;
	      HRichHit *pRH = 0;
//      Int_t nC=0;
      fRichIter->Reset();
      while((pRH = (HRichHit *)fRichIter->Next()))
      {
	  pRH->dumpToStdout();
      }
//      if (nC) r=kTRUE;
//      cout<<nC<<" ---> nb of RichHits in evt"<<endl;

	cout<<"/////////////////// no RM found ///////////////////"<<endl;
	cout<<"/////////////////// Evt: "<<nCntProcessedEvents<<"///////////////////"<<endl;
	pIterMatchHit->Reset();
	while(( pHM= (HHitMatch *)pIterMatchHit->Next()))
	{
	    pHM->dumpToStdout();
	}
    }
    if (nC) r=kTRUE;
    //cout<<nC<<" ---> nb of RICH-MDC-Corrs in evt"<<endl;
    return r;
}


Bool_t HRichEvtFilter::isTargetEvt()
{
    Bool_t r=kFALSE;
    Int_t nTrkNb = 3;//high MDC efficiency and central coll trigger

    // simple version
    //Int_t n=((HLinearCategory*)getMdcSegCat())->getEntries();
    //if (n>nTrkNb) r=kTRUE;

    // check if counted MDC segment is before magnet
    HMdcSeg *pMdcSeg = 0;
    Int_t nC=0;
    fMdcSegIter->Reset();
    while((pMdcSeg = (HMdcSeg *)fMdcSegIter->Next()))
    {
	if ( pMdcSeg->getIOSeg() == 0 ) nC++;
    }
    if (nC>nTrkNb) r=kTRUE;
    //    cout<<nC<<" ---> nb of MDC Segments in evt"<<endl;
    return r;
}

Bool_t HRichEvtFilter::isDoubleRingEvt()
{
    // currently a double ring is defined by a ring 
    // which occurs twice in two different tracklets

    HHitMatch *pHM=0;
    pIterMatchHit->Reset();
    Int_t nHM = ((HLinearCategory*)getHitMatchCat())->getEntries();
    Int_t *nRInd = new Int_t[nHM];
    for (Int_t i=0;i<nHM;i++) nRInd[i]=-2;
    Int_t ntc = 0;
    
    while(( pHM= (HHitMatch *)pIterMatchHit->Next()))
    {
	Int_t nRind = pHM->getRichInd();
	nRInd[ntc]=nRind;
	ntc++;
    }
    
    Int_t *six = new Int_t[nHM]; // sorted indexes
    TMath::Sort(nHM,nRInd,six); // descending
    
    Bool_t hasPair = kFALSE;
    for (Int_t j=1;j<nHM;j++) // start from the 2nd
    {
	Int_t idx = six[j];
	if ( nRInd[idx] > -1 ) // not a default value
	{
	    if ( nRInd[idx] == nRInd[idx-1] ) // it is the same RICH ring
	    {
		hasPair=kTRUE;
		break;
		//((HHitMatch*)getHitMatchCat()->getObject(nHMInd[idx]))
		//->dumpToStdout();
	    }
	}
    }

    return hasPair;

}

Bool_t HRichEvtFilter::containsRichHit()
{
    Bool_t r=kFALSE;
    Int_t nRH=0;
    nRH = ((HLinearCategory*)getHitRichCat())->getEntries();
    //    cout<<"number of Rich hits in evt: "<<nRH<<endl;
    if (nRH) r=kTRUE;

    
//      HRichHit *pRH = 0;
//      Int_t nC=0;
//      fRichHitIter->Reset();
//      while(pRH = (HRichHit *)fRichHitIter->Next())
//      {
//  	nC++;
//      }
//      if (nC) r=kTRUE;
//      cout<<nC<<" ---> nb of RichHits in evt"<<endl;

    return r;

}

Bool_t HRichEvtFilter::isTofMultTrigger()
{

    Bool_t r=kFALSE;
    Int_t nTH=0;
    nTH = ((HLinearCategory*)getHitTofCat())->getEntries();
    Int_t nTiH=0;
    nTiH = ((HMatrixCategory*)getCalTofinoCat())->getEntries();
    //    cout<<"TOF Mult: "<<nTH<<"   and Tofino Mult: "<<nTiH<<endl;
    if ((nTH+nTiH) >= 4) r=kTRUE;
    return r;
}
Bool_t HRichEvtFilter::containsCorrRichHit()
{
    Bool_t r=kFALSE;
    HHitMatch *pHM=0;
    pIterMatchHit->Reset();
    Int_t nC=0;
    
    while(( pHM= (HHitMatch *)pIterMatchHit->Next()))
    {
	if (pHM->getRichInd()!=-1) nC++;
    }

    if (nC) r=kTRUE;
    return r;

}
