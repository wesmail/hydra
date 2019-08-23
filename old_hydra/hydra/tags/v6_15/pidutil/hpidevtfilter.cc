// $Id: hpidevtfilter.cc,v 1.1 2003-02-26 08:53:54 eberl Exp $
// Last update by Thomas Eberl: 03/02/26 09:02:06
//
using namespace std;
#include "hpidevtfilter.h"
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
#include <iostream> 
#include <iomanip>
ClassImp(HPidEvtFilter)

HPidEvtFilter::HPidEvtFilter(Text_t *name,Text_t *title,TString opt,Int_t m) :
  HReconstructor(name,title)
{
    filterOption=opt;
    nMult=m;
}

HPidEvtFilter::HPidEvtFilter()
{

}


HPidEvtFilter::~HPidEvtFilter(void) {
 
}

Bool_t HPidEvtFilter::init() {
    if (gHades) {
	HEvent *event=gHades->getCurrentEvent();
	HRuntimeDb *rtdb=gHades->getRuntimeDb();
	HSpectrometer *spec=gHades->getSetup();
	if (event && rtdb) {

	    if (filterOption.Contains("tofmult"))
	    {
		HDetector *tofino = spec->getDetector("Tofino");
		if (tofino){
		    fTofinoCal = event->getCategory(catTofinoCal);
		}
		
		HDetector *tof=spec->getDetector("Tof");
		if (tof) 
		{ // TOF
		    fTofHits=event->getCategory(catTofHit);
		    if (!fTofHits) {
			Error("init","No TOF input");
			return kFALSE;
		    }
		}
		fTofIter=(HIterator *)fTofHits->MakeIterator();
	    }

	    if (filterOption.Contains("mdcmult"))
	    {
		HDetector *mdc = spec->getDetector("Mdc");
		if (mdc) { // MDC
		    
		    fMdcSeg=event->getCategory(catMdcSeg);
		    if (!fMdcSeg) {
			Error("init","No MDC segment category defined");
			return kFALSE;
		    }
		    else fMdcSegIter=(HIterator *)fMdcSeg->MakeIterator();
		}
	    }

	    if (filterOption.Contains("ringmult"))
	    {
		HDetector *rich = spec->getDetector("Rich");
		
		if (rich) 
		{ //Has the user set up a RICH?
		    // RICH HIT container
		    pHitRichCat=gHades->getCurrentEvent()->getCategory(catRichHit);
		    if (!pHitRichCat) Warning("HPidEvtFilter","init():no Rich Hit cat");
		    if (pHitRichCat) fRichIter = (HIterator*) pHitRichCat->MakeIterator();
		}
	    }

	}
	
    }
    
    resetCounters();
    return kTRUE;
}

Bool_t HPidEvtFilter::finalize() {
    cout<<"Number of rejected evts: "<<nCounterNbRejectedEvts<<endl;
    cout<<"Number of remaining evts: "<<nCntProcessedEvents-nCounterNbRejectedEvts<<endl;
    cout<<"Percentage of rejected evts: "<<(((Float_t)nCounterNbRejectedEvts)/((Float_t)nCntProcessedEvents))*100<<"%"<<endl;
    return kTRUE;
}

Int_t HPidEvtFilter::execute()
{
    // refer to isFilteredEvt(swt) for filter options
    nCntProcessedEvents++;

    Int_t kReturnValue=0;

    if (isFilteredEvt(filterOption)) 
    {
	nCounterNbRejectedEvts++;
	//cout<<nCntProcessedEvents<<"  event is skipped"<<endl;
	kReturnValue = kSkipEvent;
    }
    else if (isSkipNextTask(0)) kReturnValue = 1;


    return kReturnValue;
}

void HPidEvtFilter::resetCounters()
{
    nCntProcessedEvents=0;
    nCounterNbRejectedEvts=0;
}

Bool_t HPidEvtFilter::isFilteredEvt(TString opt)
{

    // if function returns kTRUE evt is skipped
    Bool_t kSwitch=kFALSE;
    
    if (opt.Contains("tofmult"))
    {
	if (!isTofMultTrigger()) kSwitch=kTRUE;
    }

    else if (opt.Contains("mdcmult"))
    {
	if (!isMdcMultTrigger()) kSwitch=kTRUE;
    }

    else if (opt.Contains("ringmult"))
    {
	if (!containsRichHit()) kSwitch=kTRUE;
    }

    return kSwitch;
}

Bool_t HPidEvtFilter::isSkipNextTask(Int_t swt)
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



Bool_t HPidEvtFilter::isMdcMultTrigger()
{
    Bool_t r=kFALSE;
    Int_t nTrkNb = nMult;//high MDC efficiency and central coll trigger

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


Bool_t HPidEvtFilter::containsRichHit()
{
    Bool_t r=kFALSE;
    Int_t nRH=0;
    nRH = ((HLinearCategory*)getHitRichCat())->getEntries();
    //    cout<<"number of Rich hits in evt: "<<nRH<<endl;
    if (nRH) r=kTRUE;

    return r;

}

Bool_t HPidEvtFilter::isTofMultTrigger()
{

    Bool_t r=kFALSE;
    Int_t nTH=0;
    nTH = ((HLinearCategory*)getHitTofCat())->getEntries();
    Int_t nTiH=0;
    nTiH = ((HMatrixCategory*)getCalTofinoCat())->getEntries();
    //    cout<<"TOF Mult: "<<nTH<<"   and Tofino Mult: "<<nTiH<<endl;
    if ((nTH+nTiH) >= nMult) r=kTRUE;
    return r;
}
