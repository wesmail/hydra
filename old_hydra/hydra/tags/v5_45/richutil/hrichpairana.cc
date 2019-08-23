// File: hrichpairana.cc
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 01/10/11 17:32:24
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
	pFileOut = new TFile("pairanahist.root","RECREATE");
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
}

void HRichPairAna::iniHistos()
{
pHistRTMopenAngle = new TH1F("RTM_opening_angle","RTM_opening_angle",100,0,10);
pHistRMopenAngle = new TH1F("RM_opening_angle","RM_opening_angle",100,0,10);
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
	    Float_t t1 = ((HHitMatch*)(*pArr)[i])->getMdcTheta();
	    Float_t p1 = ((HHitMatch*)(*pArr)[i])->getMdcPhi();
	    Float_t t2 = ((HHitMatch*)(*pArr)[j])->getMdcTheta();
	    Float_t p2 = ((HHitMatch*)(*pArr)[j])->getMdcPhi();
	    //cout<<"t1: "<<t1<<" p1: "<<p1<<" t2: "<<t2<<" p2: "<<p2<<endl;
	    // FIXME: how to avoid doubles ???
	    Float_t opangle = HRichUtilFunc::calcOpeningAngleT(t1,p1,t2,p2);
	    if (opangle>0.) hist->Fill(opangle);
					
	}
    }
}
Int_t HRichPairAna::execute()
{
    nCntProcessedEvents++;
    //    Int_t nHM = ((HLinearCategory*)getHitMatchCat())->getEntries();
    //    cout<<"Nb of HitMatch objs :"<<nHM<<endl;
    countCorrelationObjects();
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
    return 0;
}












