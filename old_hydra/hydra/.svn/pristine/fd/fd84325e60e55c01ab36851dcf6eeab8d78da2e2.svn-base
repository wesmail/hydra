// $Id: hrichpairana.cc,v 1.4 2002-09-25 16:19:53 eberl Exp $
// Last update by Thomas Eberl: 02/09/25 17:42:10
//
#include "hrichpairana.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hmatrixcatiter.h"
#include "hrichhit.h"
#include <iostream.h>
#include "hdebug.h"
#include "hades.h"
#include "richdef.h"
#include "hhitmatch.h"
#include "hlinearcategory.h"
#include "hrichutilfunc.h"
ClassImp(HRichPairAna)

HRichPairAna::HRichPairAna(Text_t *name,Text_t *title) :
  HReconstructor(name,title)
{

}

HRichPairAna::HRichPairAna()
{

}


HRichPairAna::~HRichPairAna(void) {

}


Bool_t HRichPairAna::init() {
        if (gHades) {
	HEvent *event=gHades->getCurrentEvent();
	HRuntimeDb *rtdb=gHades->getRuntimeDb();
	HSpectrometer *spec=gHades->getSetup();
	if (event && rtdb) {
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
		
	    }
	}
	}
	iniCounters();
	iniHistos();
	pFileOut = new TFile("opangleRICH.root","RECREATE");
    return kTRUE;
}

void HRichPairAna::iniCounters()
{
    // count the different correlation obj species to check plausibility
    nCntRTMfull=0;
    nCntRTMnoRM=0;
    nCntRTMnoTM=0;
    nCntRTMnoTR=0;
    nCntRTM=0;
    nCntProcessedEvents=0;
    nCounterNbRejectedHits=0;
}

void HRichPairAna::iniHistos()
{
    pHistRTMopenAngle = new TH1F("RTM_opening_angle","RTM_opening_angle",100,0,20);
    pHistRMopenAngle = new TH1F("RM_opening_angle","RM_opening_angle",100,0,20);
}

Bool_t HRichPairAna::finalize() {
    cout<<"Number of processed events: "<<nCntProcessedEvents<<endl;
    cout<<"********************************"<<endl;
    cout<<"Full RTM (i.e. RT,RM,TM) correlation: "<<nCntRTMfull<<endl;
    cout<<"Objs containing RTM indexes: "<<nCntRTM<<endl;
    cout<<"RT,RM (no TM): "<<nCntRTMnoTM<<endl;
    cout<<"RT,TM (no RM): "<<nCntRTMnoRM<<endl;
    cout<<"RM,TM (no TR): "<<nCntRTMnoTR<<endl;
    cout<<"********************************"<<endl;
    
    pFileOut->cd();
    pHistRTMopenAngle->Write();
    pHistRMopenAngle->Write();
    pFileOut->Close();
    return kTRUE;
}

Int_t HRichPairAna::countRTMobjs()
{
Int_t nRTM=0;
HHitMatch *pHM=0;
pIterMatchHit->Reset();
while(( pHM= (HHitMatch *)pIterMatchHit->Next()))
{
    Int_t nRInd = pHM->getRichInd();
    Int_t nMInd= pHM->getMdcInd();
    Int_t nTInd= pHM->getTofInd();
    if (nRInd!=-1 && nTInd!=-1 && nMInd!=-1) nRTM++;
}
return nRTM;
}
Int_t HRichPairAna::countRMobjs()
{
Int_t nRM=0;
HHitMatch *pHM=0;
pIterMatchHit->Reset();
while(( pHM= (HHitMatch *)pIterMatchHit->Next()))
{
    Int_t nRInd = pHM->getRichInd();
    Int_t nMInd= pHM->getMdcInd();
    if (nRInd!=-1 && nMInd!=-1) nRM++;
}
return nRM;
}


void HRichPairAna::countCorrelationObjects()
{
    HHitMatch *pHM=0;

    pIterMatchHit->Reset();

    while(( pHM= (HHitMatch *)pIterMatchHit->Next()))
    {
	Int_t nRInd = pHM->getRichInd();
	Int_t nMInd= pHM->getMdcInd();
	Int_t nTInd= pHM->getTofInd();
	Int_t nSInd= pHM->getShowInd();
	Int_t nRM = pHM->getMatchedRichMdc();
	Int_t nRT = pHM->getMatchedRichTof();
	Int_t nRS = pHM->getMatchedRichShower();
	Int_t nMT = pHM->getMatchedMdcTof();
	Int_t nMS = pHM->getMatchedMdcShower();
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

}

Int_t HRichPairAna::fillRTMobjs(TObjArray* pRTM)
{
    Int_t c=0;
    HHitMatch *pHM=0;
    pIterMatchHit->Reset();
    while(( pHM= (HHitMatch *)pIterMatchHit->Next()))
    {
	Int_t nRInd = pHM->getRichInd();
	Int_t nMInd = pHM->getMdcInd();
	Int_t nTInd = pHM->getTofInd();
	if (nRInd!=-1 && nTInd!=-1 && nMInd!=-1)
	{
	    c++;
	    pRTM->Add(pHM);
	}
    }
    return c;
}
Int_t HRichPairAna::fillRMobjs(TObjArray* pRM)
{
    Int_t c=0;
    HHitMatch *pHM=0;
    pIterMatchHit->Reset();
    while(( pHM= (HHitMatch *)pIterMatchHit->Next()))
    {
	Int_t nRInd = pHM->getRichInd();
	Int_t nMInd = pHM->getMdcInd();
	Int_t nRM = pHM->getMatchedRichMdc();
	if (nRInd!=-1 && nMInd!=-1 && nRM==1)
	{
	    c++;
	    pRM->Add(pHM);
	}
    }
    return c;
}
Int_t HRichPairAna::fillRTMfullobjs(TObjArray* pRTM)
{
    Int_t c=0;
    HHitMatch *pHM=0;
    pIterMatchHit->Reset();
    while(( pHM= (HHitMatch *)pIterMatchHit->Next()))
    {
	Int_t nRInd = pHM->getRichInd();
	Int_t nMInd = pHM->getMdcInd();
	Int_t nTInd = pHM->getTofInd();
	Int_t nRM = pHM->getMatchedRichMdc();
	Int_t nRT = pHM->getMatchedRichTof();
	Int_t nMT = pHM->getMatchedMdcTof();
	if (nRInd!=-1 && nTInd!=-1 && nMInd!=-1
	    && nRM==1 &&  nRT==1 &&  nMT==1 )
	{
	    
	    c++;
	    pRTM->Add(pHM);
	}
    }
    return c;
}

void HRichPairAna::calcOpeningAngle(TObjArray *pArr,TH1* hist)
{
    // make all combinations of correlated objs and call
    // calcopeningangles from hrichutilfunc
    Int_t nCnt=0;
    Int_t nMaxArr = pArr->GetLast()+1;
    //cout<<" nMaxArr: "<<nMaxArr<<endl;
    for (Int_t i=0;i<nMaxArr;i++)
    {
	nCnt++;
	for (Int_t j=nCnt;j<nMaxArr;j++)
	{
// 	    Float_t t1 = ((HHitMatch*)(*pArr)[i])->getMdcTheta();
// 	    Float_t p1 = ((HHitMatch*)(*pArr)[i])->getMdcPhi();
// 	    Float_t t2 = ((HHitMatch*)(*pArr)[j])->getMdcTheta();
// 	    Float_t p2 = ((HHitMatch*)(*pArr)[j])->getMdcPhi();
	    Float_t t1 = ((HHitMatch*)(*pArr)[i])->getRichTheta();
	    Float_t p1 = ((HHitMatch*)(*pArr)[i])->getRichPhi();
	    Float_t t2 = ((HHitMatch*)(*pArr)[j])->getRichTheta();
	    Float_t p2 = ((HHitMatch*)(*pArr)[j])->getRichPhi();
	    //cout<<"t1: "<<t1<<" p1: "<<p1<<" t2: "<<t2<<" p2: "<<p2<<endl;
	    // FIXME: how to avoid doubles ???
	    Float_t opangle = HRichUtilFunc::calcOpeningAngleT(t1,p1,t2,p2);
	    if (opangle>0.) hist->Fill(opangle);
					
	}
    }
}
Bool_t HRichPairAna::delHitMatch(TObjArray *t)
{
    // remove hit obj from HLinearCategory
    // afterwards delete empty slot
   
    if (t)
    {
	for (Int_t i=0;i<(t->GetLast()+1);i++)
	{
	    ((TClonesArray*)((HLinearCategory*)getHitMatchCat())->getCollection())
	    ->Remove((HHitMatch*)((*t)[i]));
	    ((TClonesArray*)((HLinearCategory*)getHitMatchCat())
	     ->getCollection())->Compress();
	    nCounterNbRejectedHits++;
	}
	return kTRUE;
    }
    return kFALSE;
}

Bool_t HRichPairAna::isDoubleRingEvt()
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

Bool_t HRichPairAna::removeSingleRing()
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
}

void HRichPairAna::filterTrackletInGroup(Int_t swt)
{

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
	Int_t nCnt=0;
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
    }
}

void HRichPairAna::filterTrackletIndividual(Int_t swt)
{

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

Bool_t HRichPairAna::isGood(HHitMatch* h,Int_t swt)
{
    // check certain properties of the object
    // return true or false whether it passes or not 
    
    Bool_t kSwitch = kFALSE;
    
    switch (swt)
    {

    case 0: 
	{
	    Int_t patmat = h->getRingPatMat();
	    Int_t houtra = h->getRingHouTra();
	    
	    if (houtra>=28 && patmat==0) kSwitch=kTRUE;
	    break;
	}

    case 1:
    if (h->getRichInd() > -1) kSwitch=kTRUE;
    break;

    }
    return kSwitch;
}


Int_t HRichPairAna::execute()
{
    nCntProcessedEvents++;
    //filterTrackletIndividual(1);
    //Int_t nHM = ((HLinearCategory*)getHitMatchCat())->getEntries();
    //cout<<"Nb of HitMatch objs :"<<nHM<<endl;
    filterTrackletInGroup(0);

    Int_t kReturnValue=0;
    if (isFilteredEvt(0)) kReturnValue = kSkipEvent;
    else if (isSkipNextTask(0)) kReturnValue = 1;
    return kReturnValue;
    
}

Bool_t HRichPairAna::isFilteredEvt(Int_t swt)
{

    // if function returns kTRUE evt is skipped
    Bool_t kSwitch=kFALSE;
    
    switch (swt)
    {
    case 0:
	if (!isDoubleRingEvt()) kSwitch=kTRUE;
	break;
	
    default:
	kSwitch=kFALSE;
    }
    return kSwitch;
}

Bool_t HRichPairAna::isSkipNextTask(Int_t swt)
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

void HRichPairAna::calcOpenAngles()
{
    //    filter(); // do it before calling this function
    countCorrelationObjects();
    //    cout<<"Number of rejected objects: "<<nCounterNbRejectedHits<<endl;
    Int_t nRTM = countRTMobjs();
    Int_t nRM  = countRMobjs();
    TObjArray *pRTM = new TObjArray(5);
    TObjArray *pRM = new TObjArray(5);
    if (nRTM>1)
    {
 	Int_t nbObjs=fillRTMfullobjs(pRTM);
 	if (nbObjs>1) calcOpeningAngle(pRTM,pHistRTMopenAngle);
    }
    if (nRM>1)
    {
	Int_t nbObjs=fillRMobjs(pRM);
 	if (nbObjs>1) calcOpeningAngle(pRM,pHistRMopenAngle);
    }
    delete pRTM;
    delete pRM;
    
}










