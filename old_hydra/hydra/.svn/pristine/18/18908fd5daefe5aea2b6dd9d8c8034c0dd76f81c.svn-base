// $Id: htimelvl1evtfilter.cc,v 1.1 2007-02-09 15:35:36 halo Exp $
// Last update by Filip Krizek: 24/11/06 
//
using namespace std;
#include "htimelvl1evtfilter.h"
#include "hlinearcategory.h"
#include "hmatrixcategory.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hades.h"
#include "tofinodef.h"
#include "htofhitsim.h"
#include "htofinocalsim.h"
#include "tofdef.h"
#include <iostream> 
#include <iomanip>
ClassImp(HTimeLvl1EvtFilter)

HTimeLvl1EvtFilter::HTimeLvl1EvtFilter(Text_t *name,Text_t *title,TString opt,Int_t m,Float_t timecut) :
  HReconstructor(name,title)
{
    filterOption=opt;
    nMult=m;
    fTimeCut=timecut;
}

HTimeLvl1EvtFilter::HTimeLvl1EvtFilter()
{

}


HTimeLvl1EvtFilter::~HTimeLvl1EvtFilter(void) {
 
}

Bool_t HTimeLvl1EvtFilter::init() {
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
		fTofinoIter=(HIterator *)fTofinoCal->MakeIterator();

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
	   	    	   
	}
	
    }
    
    resetCounters();
    return kTRUE;
}

Bool_t HTimeLvl1EvtFilter::finalize() {
    cout<<"Number of rejected evts: "<<nCounterNbRejectedEvts<<endl;
    cout<<"Number of remaining evts: "<<nCntProcessedEvents-nCounterNbRejectedEvts<<endl;
    cout<<"Percentage of rejected evts: "<<(((Float_t)nCounterNbRejectedEvts)/((Float_t)nCntProcessedEvents))*100<<"%"<<endl;
    return kTRUE;
}

Int_t HTimeLvl1EvtFilter::execute()
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

void HTimeLvl1EvtFilter::resetCounters()
{
    nCntProcessedEvents=0;
    nCounterNbRejectedEvts=0;
}

Bool_t HTimeLvl1EvtFilter::isFilteredEvt(TString opt)
{

    // if function returns kTRUE evt is skipped
    Bool_t kSwitch=kFALSE;
    
    if (opt.Contains("tofmult"))
    {
	if (!isTofMultTrigger()) kSwitch=kTRUE;
    }
       
    return kSwitch;
}

Bool_t HTimeLvl1EvtFilter::isSkipNextTask(Int_t swt)
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




Bool_t HTimeLvl1EvtFilter::isTofMultTrigger()
{

    Bool_t r=kFALSE;
    Int_t nTH=0;
    //nTH = ((HLinearCategory*)getHitTofCat())->getEntries();
    Int_t nTiH=0;
    //nTiH = ((HMatrixCategory*)getCalTofinoCat())->getEntries();
    Float_t tof;

    fTofIter=(HIterator *)fTofHits->MakeIterator();
    fTofinoIter=(HIterator *)fTofinoCal->MakeIterator();
    HTofHit *pTof = 0;
    HTofinoCal *pTofino = 0;

    fTofIter->Reset();
    while((pTof = (HTofHit *)fTofIter->Next()))
      {
	tof=pTof->getTof();
	 if(tof < fTimeCut) nTH++;
      }
    

    fTofinoIter->Reset();
    while((pTofino = (HTofinoCal *)fTofinoIter->Next()))
      {
	tof=pTofino->getTime();
	 if(tof < fTimeCut) nTiH++;
      }
    

    //    cout<<"TOF Mult: "<<nTH<<"   and Tofino Mult: "<<nTiH<<endl;
    if ((nTH+nTiH) >= nMult){
      r=kTRUE;     
    }
    return r;
}



