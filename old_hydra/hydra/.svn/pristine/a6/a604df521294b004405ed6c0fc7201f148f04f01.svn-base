// File: hrichevtfiltersim.cc
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 02/02/18 19:10:13
//

#include "hrichevtfiltersim.h"
#include "hlinearcategory.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hrichdetector.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hades.h"
#include "richdef.h"
#include "hmdcdef.h"
#include "hmdcseg.h"
#include "hhitmatchsim.h"
#include "hhitmatchheadersim.h"
#include <iostream.h>
ClassImp(HRichEvtFilterSim)

HRichEvtFilterSim::HRichEvtFilterSim(Text_t *name,Text_t *title) :
  HReconstructor(name,title)
{

}

HRichEvtFilterSim::HRichEvtFilterSim()
{

}


HRichEvtFilterSim::~HRichEvtFilterSim(void) {
 
}

Bool_t HRichEvtFilterSim::init() {
    if (gHades) {
	HEvent *event=gHades->getCurrentEvent();
	HRuntimeDb *rtdb=gHades->getRuntimeDb();
	HSpectrometer *spec=gHades->getSetup();
	if (event && rtdb) {
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
                if (!pHitRichCat) Error("HRichEvtFilterSim","init():no Rich Hit cat");
 		fRichIter = (HIterator*) pHitRichCat->MakeIterator();


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
		

		pHitMatchHeaderCat=event->getCategory(catMatchHitHeader);
		pIterMatchHitHeader = (HIterator*)getHitMatchHeaderCat()
		    ->MakeIterator("native");
	    }
	
	}
    }
    resetCounters();
    return kTRUE;
}

Bool_t HRichEvtFilterSim::finalize() {
    cout<<"Number of rejected evts: "<<nCounterNbRejectedEvts<<endl;
    cout<<"Number of remaining evts: "<<nCntProcessedEvents-nCounterNbRejectedEvts<<endl;
    cout<<"Percentage of rejected evts: "<<(((Float_t)nCounterNbRejectedEvts)/((Float_t)nCntProcessedEvents))*100<<"%"<<endl;
    return kTRUE;
}

Int_t HRichEvtFilterSim::execute()
{
    // refer to isFilteredEvt(swt) for filter options
    nCntProcessedEvents++;

    Int_t kReturnValue=0;

    if (isFilteredEvt(1) || isFilteredEvt(2)) 
    {
	nCounterNbRejectedEvts++;
	//cout<<nCntProcessedEvents<<"  event is skipped"<<endl;
	kReturnValue = kSkipEvent;
    }
    else if (isSkipNextTask(0)) kReturnValue = 1;


    return kReturnValue;
}

void HRichEvtFilterSim::resetCounters()
{
    nCntProcessedEvents=0;
    nCounterNbRejectedEvts=0;
}

Bool_t HRichEvtFilterSim::isFilteredEvt(Int_t swt)
{

    // if function returns kTRUE evt is skipped
    Bool_t kSwitch=kFALSE;
    
    switch (swt)
    {
    case 0:

	break;
	
    case 1:

	break;
      
    case 2:

	break;
	
    case 3:

	break;

    default:
	{
	    Error("isFilteredEvt","no valid filter function");//this is dirty
	    kSwitch=kFALSE;
	}
    }
    return kSwitch;
}

Bool_t HRichEvtFilterSim::isSkipNextTask(Int_t swt)
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



