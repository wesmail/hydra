// File: hrichtrackletfilter.cc
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 02/02/14 16:36:48
//

#include "hrichtrackletfilter.h"
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
#include <TObjArray.h>
#include "hhitmatch.h"
#include "hhitmatchheader.h"
#include <iostream.h>
ClassImp(HRichTrackletFilter)

HRichTrackletFilter::HRichTrackletFilter(Text_t *name,Text_t *title) :
  HReconstructor(name,title)
{

}

HRichTrackletFilter::HRichTrackletFilter()
{

}


HRichTrackletFilter::~HRichTrackletFilter(void) {
 
}

Bool_t HRichTrackletFilter::init() {
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
// 		fRichPID=gHades->getCurrentEvent()->getCategory(catRichHit);
// 		if (!fRichPID) {
// 		    fRichPID=rich->buildCategory(catRichHit);
		    
// 		    if (!fRichPID) return kFALSE;
// 		    else gHades->getCurrentEvent()
// 			     ->addCategory(catRichHit, fRichPID, "Rich");
// 		}
    
// 		fRichIter = (HIterator*) fRichPID->MakeIterator();


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

Bool_t HRichTrackletFilter::finalize() {
  
    return kTRUE;
}

Bool_t HRichTrackletFilter::delHitMatch(TObjArray *t)
{
    // remove hit obj from HLinearCategory
    // afterwards delete empty slot
   
    if (t)
    {
	for (Int_t i=0;i<(t->GetLast()+1);i++)
	{
	    ((TClonesArray*)((HLinearCategory*)getHitMatchCat())
	     ->getCollection())->Remove( (HHitMatch*)((*t)[i]) );
	    ((TClonesArray*)((HLinearCategory*)getHitMatchCat())
	     ->getCollection())->Compress();
	    nCounterNbRejectedHits++;
	}
	return kTRUE;
    }
    return kFALSE;
}

void HRichTrackletFilter::filterTrackletIndividual(Int_t swt)
{
    // swt: is a switch parameter 
    //      selecting the filtering method to apply
    //
    // filter tracklets only according to their own internal state 
    HHitMatch *pHM=0;
    pIterMatchHit->Reset();
    TObjArray *tmp = new TObjArray(5);
    while(( pHM= (HHitMatch *)pIterMatchHit->Next()))
    {
	//cout<<"in hit match filter cut"<<endl;
	if (!isGood(pHM,swt)) tmp->Add(pHM);
    }
    if (!delHitMatch(tmp)) Error("execute","Bad hits not deleted");
    delete tmp;
}

void HRichTrackletFilter::filterTrackletInGroup(Int_t swt)
{
    // swt: is a switch parameter 
    //      selecting the filtering method to apply
    //
    // filter tracklets according to their relation to others 

    switch(swt)
    {
    case 0:
	{
	HHitMatch *pHM=0;
	pIterMatchHit->Reset();
	Int_t nHM = ((HLinearCategory*)getHitMatchCat())->getEntries();
	cout<<"Nb of tracklets : "<<nHM<<endl;
	if (nHM==0) break;
	TObjArray *pArr = new TObjArray[nHM+1];
	while(( pHM= (HHitMatch *)pIterMatchHit->Next())) pArr->Add(pHM);
	TObjArray *deltmp = new TObjArray(5);
	//Int_t nCnt=0;
	Int_t nMaxArr = pArr->GetLast()+1;
	for (Int_t i=0;i<nMaxArr-1;i++)
	{
	    //nCnt++;
	    Bool_t isSingle=kTRUE;
	    
	    for (Int_t j=i+1;j<nMaxArr;j++)
	    {
		cout<<"index i: "<<i<<" / index j: "<<j<<"   ";
		Int_t r1 = ((HHitMatch*)(*pArr)[i])->getRichInd();
		Int_t r2 = ((HHitMatch*)(*pArr)[j])->getRichInd();
		Int_t m1 = ((HHitMatch*)(*pArr)[i])->getMdcInd();
		Int_t m2 = ((HHitMatch*)(*pArr)[j])->getMdcInd();
		cout<<"r1: "<<r1<<" r2: "<<r2<<" m1: "<<m1<<" m2: "<<m2<<endl;
		
		if (r1==r2 && m1==m2) 
		{
		    
		    HHitMatch* ht = ((HHitMatch*)(*pArr)[i]);
		    if (deltmp->IndexOf(ht)==-1) deltmp->Add(ht);
		    // avoid doubles
		    cout<<"removed index i: "<<i<<" / r1: "<<r1<<" m1: "<<m1<<endl;
		}
		if (r1==r2)  isSingle=kFALSE;
	    }
	    
	    if (isSingle)
	    {
		HHitMatch* ht = ((HHitMatch*)(*pArr)[i]);
		if (deltmp->IndexOf(ht)==-1) deltmp->Add(ht);
		// avoid doubles
	    }
	    
	}
	
	if (!delHitMatch(deltmp)) Error("execute","Bad hits not deleted");
	deltmp->Clear();
	pArr->Clear();
	}
	break;
    } //end switch
}


Bool_t HRichTrackletFilter::isGood(HHitMatch* h,Int_t swt)
{
    // check certain properties of the object
    // return true or false whether it passes or not 
    
    Bool_t kSwitch = kFALSE;
    
    switch (swt)
    {

    case 0: 
	{
	    //Int_t patmat = h->getRingPatMat();
	    Int_t houtra = h->getRingHouTra();
	    //if (houtra>=100) cout<<houtra<<endl;
	    if (houtra>=100) kSwitch=kTRUE;
	    break;
	}
	
    case 1:
	if (h->getRichInd() > -1) kSwitch=kTRUE;
	break;
    case 2:
	if (h->getMatchedRichMdc()==1) kSwitch=kTRUE;
	break;
    }
    return kSwitch;

}

Bool_t HRichTrackletFilter::isDoubleRingEvt()
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

Bool_t HRichTrackletFilter::removeSingleRing()
{
    HHitMatch *pHM=0;
    pIterMatchHit->Reset();
    Int_t nHM = ((HLinearCategory*)getHitMatchCat())->getEntries();
    cout<<"Nb of tracklets : "<<nHM<<endl;
    if (nHM==0) return kTRUE;
    TObjArray *pArr = new TObjArray[10];
    while(( pHM= (HHitMatch *)pIterMatchHit->Next())) {pArr->Add(pHM);}
    TObjArray *deltmp = new TObjArray(5);
    Int_t nCnt=0;
    Int_t nMaxArr = pArr->GetLast()+1;
    for (Int_t i=0;i<nMaxArr;i++)
    {
	    nCnt++;
	    Bool_t isSingle=kTRUE;

	    for (Int_t j=nCnt;j<nMaxArr;j++)
	    {
		cout<<"index i: "<<i<<" / index j: "<<j<<endl;
		Int_t r1 = ((HHitMatch*)(*pArr)[i])->getRichInd();
		Int_t r2 = ((HHitMatch*)(*pArr)[j])->getRichInd();
		if (r1==r2) isSingle=kFALSE;
	    }

	    if (isSingle && i!=nMaxArr-1)
	    {
		HHitMatch* ht = ((HHitMatch*)(*pArr)[i]);
		if (deltmp->IndexOf(ht)==-1) deltmp->Add(ht);
		// avoid doubles
	    }
	    
    }

    if (!delHitMatch(deltmp)) Error("execute","Bad hits not deleted");
    delete deltmp;
    delete pArr;
    return kTRUE;
}
Bool_t HRichTrackletFilter::isFilteredEvt(Int_t swt)
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
    default:
	kSwitch=kFALSE;
    }
    return kSwitch;
}

Bool_t HRichTrackletFilter::isSkipNextTask(Int_t swt)
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

Int_t HRichTrackletFilter::execute()
{
    nCntProcessedEvents++;
    nCounterNbRejectedHits=0;
    filterTrackletIndividual(2);
    Int_t kReturnValue=0;
    if (isFilteredEvt(1) || isFilteredEvt(2)) kReturnValue = kSkipEvent;
    else if (isSkipNextTask(0)) kReturnValue = 1;
    return kReturnValue;


    //    Int_t nHM = ((HLinearCategory*)getHitMatchCat())->getEntries();
    //    cout<<"Nb of HitMatch objs :"<<nHM<<endl;
//      HHitMatchHeaderSim *pHMH = 0;
//      pIterMatchHitHeaderSim->Reset();
//      while(( pHMH= (HHitMatchHeaderSim *)pIterMatchHitHeaderSim->Next()))
//      {
//  	//pHMH->dumpToStdoutSim();
//      }


    //    cout<<"********************* start event ****************"<<endl;
//      resetCounters();
//      Int_t nObjs1 = countCorrelationObjects();
//      //    dumpCorrelationObjectStat();
//      filter();
//      resetCounters();
//      Int_t nObjs2 = countCorrelationObjects();
    //cout<<nObjs1<<"  "<<nObjs2<<endl;
//      if ( nObjs1-nObjs2 < nObjs1 ) 
//      {
//  	cout<<nObjs1<<"  "<<nObjs2<<endl;
//  	dumpCorrelationObjectStat();
//  	cout<<" *** nb of rejected tracklets : "<<nCounterNbRejectedHits<<endl;
//      }
    //cout<<"********************* stop event ****************"<<endl;

}
void HRichTrackletFilter::resetCounters()
{
    nCntRTM=0;
    nCntRTMfull=0;
    nCntRTMnoTM=0;
    nCntRTMnoTR=0;
    nCntRTMnoRM=0;
    nCntProcessedObjs=0;
}
Int_t HRichTrackletFilter::countCorrelationObjects()
{
    HHitMatch *pHM=0;

    pIterMatchHit->Reset();
    Int_t nC=0;
    while(( pHM= (HHitMatch *)pIterMatchHit->Next()))
    {
	//cout<<"in HHitMatchSim loop !"<<endl;
	//pHM->dumpToStdoutSim();
	nCntProcessedObjs++;
	nC++;
	Int_t nRInd = pHM->getRichInd();
	Int_t nMInd= pHM->getMdcInd();
	Int_t nTInd= pHM->getTofInd();
	//	Int_t nSInd= pHM->getShowInd();
	Int_t nRM = pHM->getMatchedRichMdc();
	Int_t nRT = pHM->getMatchedRichTof();
	//Int_t nRS = pHM->getMatchedRichShower();
	Int_t nMT = pHM->getMatchedMdcTof();
	//Int_t nMS = pHM->getMatchedMdcShower();
	if (nRInd!=-1 && nTInd!=-1 && nMInd!=-1) nCntRTM++;//obj contains
	//info from all 3 detectors
	if (nRInd!=-1 && nTInd!=-1 && nMInd!=-1 && 
	    nRM==1 &&  nRT==1 &&  nMT==1 ) nCntRTMfull++;

	else if (nRInd!=-1 && nTInd!=-1 && nMInd!=-1 &&
		 nRM==1 && nRT==1) nCntRTMnoTM++;

	else if (nRInd!=-1 && nTInd!=-1 && nMInd!=-1 &&
		 nRM==1 && nMT==1) nCntRTMnoTR++;

	else if (nRInd!=-1 && nTInd!=-1 && nMInd!=-1 &&
		 nRT==1 && nMT==1) nCntRTMnoRM++;
 
	
    }
    return nC;

}
void HRichTrackletFilter::dumpCorrelationObjectStat()
{
    cout<<"Number of processed objects: "<<nCntProcessedObjs<<endl;
    cout<<"********************************"<<endl;
    cout<<"Full RTM (i.e. RT,RM,TM) correlation: "<<nCntRTMfull<<endl;
    cout<<"Objs containing RTM indexes: "<<nCntRTM<<endl;
    cout<<"RT,RM (no TM): "<<nCntRTMnoTM<<endl;
    cout<<"RT,TM (no RM): "<<nCntRTMnoRM<<endl;
    cout<<"RM,TM (no TR): "<<nCntRTMnoTR<<endl;
    cout<<"********************************"<<endl;
}

Bool_t HRichTrackletFilter::containsRM()
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
    if (nC) r=kTRUE;
    return r;
}

Bool_t HRichTrackletFilter::isTargetEvt()
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
    return r;
}








