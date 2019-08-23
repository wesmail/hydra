// $Id: hpairevtfilter.cc,v 1.5 2007-11-20 10:37:47 kempter Exp $
// Author: Thomas.Eberl@ph.tum.de, last modified : 2006-05-29 13:40:52
//
// --------------------------------------------------------------------------  
//_HADES_CLASS_DESCRIPTION
//////////////////////////////////////////////////////////////////////////////
//                                                                             
// HPairEvtFilter                                                              
//                                                                             
///////////////////////////////////////////////////////////////////////////////
//                                                                             
using namespace std;
// --------------------------------------------------------------------------  
#include <iostream> 
#include <iomanip>
// --------------------------------------------------------------------------  
#include <hades.h>
#include <hcategory.h>
#include <hevent.h>
#include <heventheader.h>
#include <hiterator.h>
#include <hpidparticle.h>
#include "hgeantdef.h"

// --------------------------------------------------------------------------  
#include "hpairevtfilter.h"
// --------------------------------------------------------------------------  
ClassImp(HPairEvtFilter)
// --------------------------------------------------------------------------  
HPairEvtFilter::HPairEvtFilter(Text_t *name,Text_t *title,
			       TString opt,
			       Int_t nLower,
			       Int_t nUpper) :
    HReconstructor(name,title)
{
    filterOption=opt;
    nLowerMult=nLower;
    nUpperMult=nUpper;
    zVertMin=-10000.;
    zVertMax=10000.;

    // default init
    pCatPidParticle=0;
    pIterPidParticle=0;

}
// --------------------------------------------------------------------------  
HPairEvtFilter::HPairEvtFilter(Text_t *name,Text_t *title,
			       TString opt,
			       Double_t zMin,
			       Double_t zMax) :
    HReconstructor(name,title)
{
    filterOption=opt;
    nLowerMult=0;
    nUpperMult=1000;
    zVertMin=zMin;
    zVertMax=zMax;

    // default init
    pCatPidParticle=0;
    pIterPidParticle=0;

}
// --------------------------------------------------------------------------  
HPairEvtFilter::HPairEvtFilter()
{

}
// --------------------------------------------------------------------------  
HPairEvtFilter::~HPairEvtFilter(void) 
{
 
}
// --------------------------------------------------------------------------  
Bool_t HPairEvtFilter::init() 
{
    if (gHades) 
    {

	HEvent *event=gHades->getCurrentEvent();

	if (event) 
	{
	    HCategory* pCatGeantKine=event->getCategory(catGeantKine);
	    if(pCatGeantKine) kIsReal = kFALSE;
	    else              kIsReal = kTRUE;

	    if (kIsReal) cout<<"REAL event mode selected!"<<endl;
	    else cout<<"SIMULATION event mode selected!"<<endl;

	    // needed categories depend on requested cuts
	    // -------------------------------------------------------
	    if (filterOption.Contains("multrange"))
	    {

		pCatPidParticle=event->getCategory(catPidPart);
                if (!pCatPidParticle)
                {
                    Error("init","No PIDPARTICLE category defined");
                    return kFALSE;
                }
                else pIterPidParticle = (HIterator*)getPidParticleCat()
                         ->MakeIterator("native");

		if (!pIterPidParticle)
		{
                    Error("init","No PIDPARTICLE iterator defined");
                    return kFALSE;
                }

	    }
	    // -------------------------------------------------------

	} // endif event   
	resetCounters();


    }
    return kTRUE;
}
// --------------------------------------------------------------------------  
Bool_t HPairEvtFilter::finalize() 
{
    cout<<"************** EVT FILTER TASK *****************************"<<endl;
    cout<<"Number of rejected evts: "<<nCounterNbRejectedEvts<<endl;
    cout<<"Number of remaining evts: "
	<<nCntProcessedEvents-nCounterNbRejectedEvts<<endl;
    cout<<"Percentage of rejected evts: "
	<<(((Float_t)nCounterNbRejectedEvts)/
	   ((Float_t)nCntProcessedEvents))*100<<"%"<<endl;
    cout<<"Percentage of accepted evts: "
	<<100-(((Float_t)nCounterNbRejectedEvts)/
	       ((Float_t)nCntProcessedEvents))*100<<"%"<<endl;
    cout<<"************************************************************"<<endl;
    return kTRUE;
}
// --------------------------------------------------------------------------  
Int_t HPairEvtFilter::execute()
{
    // refer to isDiscardedEvt(swt) for filter options
    nCntProcessedEvents++;

    Int_t kReturnValue=0;

    if (isDiscardedEvt(filterOption)) 
    {
	nCounterNbRejectedEvts++;
	kReturnValue = kSkipEvent;
	//cout<<nCntProcessedEvents<<"  event is skipped"<<endl;
    }
    else if (isSkipNextTask(0)) kReturnValue = 1;


    return kReturnValue;
}
// --------------------------------------------------------------------------  
void HPairEvtFilter::resetCounters()
{
    nCntProcessedEvents=0;
    nCounterNbRejectedEvts=0;
}
// --------------------------------------------------------------------------  
Bool_t HPairEvtFilter::isDiscardedEvt(TString opt)
{

    // if function returns kTRUE evt is skipped

    Bool_t kBadParticleMultiplicityRange = kFALSE;
    Bool_t kNotTargetRange               = kFALSE;
    Bool_t kNotFirstTarget               = kFALSE;
    Bool_t kNotSecondTarget              = kFALSE;


    if (opt.Contains("multrange"))
    {
	if (!isParticleMultRange()) kBadParticleMultiplicityRange = kTRUE;
    }


    if (opt.Contains("targetz"))
    {
	if (!isTargetZ(zVertMin, zVertMax)) kNotTargetRange = kTRUE;
	}
    else if (opt.Contains("target1"))
    {
	if (!isTarget(1)) kNotFirstTarget = kTRUE;
    }
    else if (opt.Contains("target2"))
    {
	if (!isTarget(2)) kNotSecondTarget = kTRUE;
    }

    Bool_t kIsDiscarded = kFALSE;

    // if one of the event selections was not matched, discard event  
    if (
	kBadParticleMultiplicityRange ||
	kNotTargetRange               ||
	kNotFirstTarget               ||
	kNotSecondTarget                 
	) kIsDiscarded = kTRUE;

    return kIsDiscarded;
}
// --------------------------------------------------------------------------  
Bool_t HPairEvtFilter::isSkipNextTask(Int_t swt)
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
// --------------------------------------------------------------------------  
Bool_t HPairEvtFilter::isParticleMultRange()
{
    // return value
    Bool_t r=kFALSE;

    // local counter for charged particle multiplicity in event
    Int_t nLC = 0;
    
    // COUNT particles which have NOT been identified as leptons
    HPidParticle* pPart = 0;

    //    Int_t n = getPidParticleCat()->getEntries();
    if (pIterPidParticle) pIterPidParticle->Reset();
    else Error("isParticleMultRange","no valid iterator");

    while ( (pPart = (HPidParticle*) pIterPidParticle->Next()) )
    {
	if (pPart->getPid()!=2 && pPart->getPid()!=3) nLC++;
    }


    // condition that selects event, true means event will be processed
    if (nLC>=nLowerMult && nLC<=nUpperMult) r=kTRUE;

    // debug output
    //        cout << "Particle Mult: " << nLC <<endl;

    return r;
}
// --------------------------------------------------------------------------  
Bool_t HPairEvtFilter::isTarget(Int_t nTargetNumber)
{
    // return value
    Bool_t r=kFALSE;

    // limit of first target for nov02
    Double_t dLimitFirstTarget = -25.; // in mm on z-axis

    // target information from event header
    Double_t dVertexZ = ( (HEventHeader*)
			  ( (HEvent*)gHades->getCurrentEvent())->getHeader() ) 
	-> getVertexZ();

    //    cout << "Event Vertex _z :" << dVertexZ << endl;
    // event vertex_z < -25 ==> first target
    // event vertex_z > -25 ==> second target
    if      ( dVertexZ < dLimitFirstTarget && nTargetNumber == 1) r=kTRUE;
    else if ( dVertexZ > dLimitFirstTarget && nTargetNumber == 1) r=kFALSE;
    else if ( dVertexZ < dLimitFirstTarget && nTargetNumber == 2) r=kFALSE;
    else if ( dVertexZ > dLimitFirstTarget && nTargetNumber == 2) r=kTRUE;

    // debug output
    //    cout << "Event Vertex: " << dVertexZ << endl;

    return r;
}
// --------------------------------------------------------------------------  
Bool_t HPairEvtFilter::isTargetZ(Double_t zMin, Double_t zMax)
{
    // target information from event header
    Double_t dVertexZ = ( (HEventHeader*)
			  ( (HEvent*)gHades->getCurrentEvent())->getHeader() ) 
	-> getVertexZ();

    if      (zMin<=dVertexZ && dVertexZ<zMax) return kTRUE;
    return kFALSE;
}
// --------------------------------------------------------------------------  

