// $Id: hrichtrackletfilter.cc,v 1.5 2003-01-24 18:24:43 halo Exp $
// Last update by Thomas Eberl: 02/09/25 17:46:16
//
using namespace std;
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
#include <iostream> 
#include <iomanip>
ClassImp(HRichTrackletFilter)

HRichTrackletFilter::HRichTrackletFilter(Text_t *name,Text_t *title,Int_t filt) :
  HReconstructor(name,title)
{
    nFilter=filt;
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

Int_t HRichTrackletFilter::execute()
{
    nCntProcessedEvents++;
    nCounterNbRejectedHits=0;
    //filterTrackletIndividual(5);
    filterTrackletIndividual(nFilter);
	

    Int_t kReturnValue=0;//proceed normally
    if (isEvtWithoutCorr()) kReturnValue = kSkipEvent;
    return kReturnValue;
}

Bool_t HRichTrackletFilter::isEvtWithoutCorr()
{
    Bool_t r=kFALSE;
    Int_t nHM = ((HLinearCategory*)getHitMatchCat())->getEntries();
    if (!nHM) r=kTRUE;
    return r;
}

Bool_t HRichTrackletFilter::delHitMatch(TObjArray *t)
{
    // remove hit obj from HLinearCategory
    // afterwards delete empty slot
    // could be turned into "delLinObj"
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
	
    case 1:// at least a correlated ring
	if (h->getRichInd() > -1) kSwitch=kTRUE;
	break;
    case 2:// at least a RICH MDC corr
	if (h->getMatchedRichMdc()==1) kSwitch=kTRUE;
	break;
    case 3:// at least 
	if (h->getMdcFlagIO()!=1) kSwitch=kTRUE;
	break;
    case 4:// apply theta cut
	if (isBentTrack(h)) kSwitch=kTRUE;
	break;
    case 5:// is min RM corr
	if (isCloseRMinTheta(h)) kSwitch=kTRUE;
	break;
    default:
	Error("no default","no default");
    }
    return kSwitch;

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


Bool_t HRichTrackletFilter::isBentTrack(HHitMatch* h)
{
    // in the case of a lowfield (i.e. 11.5% of full field ~ 400A)
    // measurement, this function checks whether the tracklet,
    // that was produced with no polar angle cut,
    // shows a large bending (theta variation between MDC12
    // and TOF) or not. If not, it is a stiff particle, i.e.: high
    // momentum

    Bool_t r = kTRUE;
    Float_t dThetaCut = 15.;
    Float_t rt,mt,tt,st;
    rt=mt=tt=st=0.;
    if (h->getRichInd() !=-1) rt=h->getRichTheta();
    if (h->getMdcInd()  !=-1) mt=h->getMdcTheta();
    if (h->getTofInd()  !=-1) tt=h->getTofTheta();
    if (h->getShowInd() !=-1) st=h->getShowerTheta();
    
    Int_t rmc,rtc,rsc,mtc,msc;
    rmc=rtc=rsc=mtc=msc=-1;
    rmc=h->getMatchedRichMdc();
    rtc=h->getMatchedRichTof();
    rsc=h->getMatchedRichShower();
    mtc=h->getMatchedMdcTof();
    msc=h->getMatchedMdcShower();

    Float_t dTheta=0.;
    if     (mtc==1) dTheta=mt-tt;
    else if(rtc==1) dTheta=rt-tt;
    else if(msc==1) dTheta=mt-st;
    else if(rsc==1) dTheta=rt-st;

    
    if (TMath::Abs(dTheta) < dThetaCut) r=kFALSE;

    return r;
}


Bool_t HRichTrackletFilter::isCloseRMinTheta(HHitMatch* h)
{
    Bool_t r = kTRUE;
    Float_t dTheta=2.;
    if (h->getMatchedRichMdc()  == 1)
    {
	Float_t rt,mt;
	rt=mt=0.;
	if (h->getRichInd() != -1) rt=h->getRichTheta();
	if (h->getMdcInd()  != -1) mt=h->getMdcTheta();
	if (rt>0. && mt>0.)
	{
	    if (TMath::Abs(rt-mt)>dTheta) r=kFALSE;
	}
	else Error("HRichTrackletFilter::isCloseRMinTheta","wrong theta");
    }
    return r;
}
