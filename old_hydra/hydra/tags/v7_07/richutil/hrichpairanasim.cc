// $Id: hrichpairanasim.cc,v 1.6 2003-01-24 18:24:39 halo Exp $
// Last update by Thomas Eberl: 02/09/25 17:42:26
//
using namespace std;
#include "hrichpairanasim.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hmatrixcatiter.h"
#include "hrichhit.h"
#include <iostream> 
#include <iomanip>
#include "hdebug.h"
#include "hades.h"
#include "richdef.h"
#include "hhitmatchsim.h"
#include "hhitmatchheadersim.h"
#include "hlinearcategory.h"
#include "hrichutilfunc.h"
ClassImp(HRichPairAnaSim)

HRichPairAnaSim::HRichPairAnaSim(Text_t *name,Text_t *title) :
  HReconstructor(name,title)
{

}

HRichPairAnaSim::HRichPairAnaSim()
{

}


HRichPairAnaSim::~HRichPairAnaSim(void) {

}


Bool_t HRichPairAnaSim::init() {
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
		

		pHitMatchHeaderCat=event->getCategory(catMatchHitHeader);
		pIterMatchHitHeaderSim = (HIterator*)getHitMatchHeaderCat()
		    ->MakeIterator("native");
	    }
	}
	}
	iniCounters();
	iniHistos();
	pFileOut = new TFile("pairanasimhist.root","RECREATE");
    return kTRUE;
}

void HRichPairAnaSim::iniCounters()
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

void HRichPairAnaSim::iniHistos()
{
    pHistRTMopenAngle = new TH1F("RTM_opening_angle","RTM_opening_angle",100,0,20);
    pHistRMopenAngle = new TH1F("RM_opening_angle","RM_opening_angle",100,0,20);
}

Bool_t HRichPairAnaSim::finalize() {
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

Int_t HRichPairAnaSim::countRTMobjs()
{
Int_t nRTM=0;
HHitMatchSim *pHM=0;
pIterMatchHit->Reset();
while(( pHM= (HHitMatchSim *)pIterMatchHit->Next()))
{
    Int_t nRInd = pHM->getRichInd();
    Int_t nMInd= pHM->getMdcInd();
    Int_t nTInd= pHM->getTofInd();
    if (nRInd!=-1 && nTInd!=-1 && nMInd!=-1) nRTM++;
}
return nRTM;
}
Int_t HRichPairAnaSim::countRMobjs()
{
Int_t nRM=0;
HHitMatchSim *pHM=0;
pIterMatchHit->Reset();
while(( pHM= (HHitMatchSim *)pIterMatchHit->Next()))
{
    Int_t nRInd = pHM->getRichInd();
    Int_t nMInd= pHM->getMdcInd();
    if (nRInd!=-1 && nMInd!=-1) nRM++;
}
return nRM;
}


void HRichPairAnaSim::countCorrelationObjects()
{
    HHitMatchSim *pHM=0;

    pIterMatchHit->Reset();

    while(( pHM= (HHitMatchSim *)pIterMatchHit->Next()))
    {
	//cout<<"in HHitMatchSim loop !"<<endl;
	//pHM->dumpToStdoutSim();
	Int_t nRInd = pHM->getRichInd();
	Int_t nMInd= pHM->getMdcInd();
	Int_t nTInd= pHM->getTofInd();
	//Int_t nSInd= pHM->getShowInd(); // unused
	Int_t nRM = pHM->getMatchedRichMdc();
	Int_t nRT = pHM->getMatchedRichTof();
	//Int_t nRS = pHM->getMatchedRichShower(); // unused
	Int_t nMT = pHM->getMatchedMdcTof();
	//Int_t nMS = pHM->getMatchedMdcShower(); // unused
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

Int_t HRichPairAnaSim::fillRTMobjs(TObjArray* pRTM)
{
    Int_t c=0;
    HHitMatchSim *pHM=0;
    pIterMatchHit->Reset();
    while(( pHM= (HHitMatchSim *)pIterMatchHit->Next()))
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
Int_t HRichPairAnaSim::fillRMobjs(TObjArray* pRM)
{
    Int_t c=0;
    HHitMatchSim *pHM=0;
    pIterMatchHit->Reset();
    while(( pHM= (HHitMatchSim *)pIterMatchHit->Next()))
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
Int_t HRichPairAnaSim::fillRTMfullobjs(TObjArray* pRTM)
{
    Int_t c=0;
    HHitMatchSim *pHM=0;
    pIterMatchHit->Reset();
    while(( pHM= (HHitMatchSim *)pIterMatchHit->Next()))
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

void HRichPairAnaSim::calcOpeningAngle(TObjArray *pArr,TH1* hist)
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
	    Float_t t1 = ((HHitMatchSim*)(*pArr)[i])->getMdcTheta();
	    Float_t p1 = ((HHitMatchSim*)(*pArr)[i])->getMdcPhi();
	    Float_t t2 = ((HHitMatchSim*)(*pArr)[j])->getMdcTheta();
	    Float_t p2 = ((HHitMatchSim*)(*pArr)[j])->getMdcPhi();
	    //cout<<"t1: "<<t1<<" p1: "<<p1<<" t2: "<<t2<<" p2: "<<p2<<endl;
	    // FIXME: how to avoid doubles ???
	    Float_t opangle = HRichUtilFunc::calcOpeningAngleT(t1,p1,t2,p2);
	    if (opangle>0.) hist->Fill(opangle);
					
	}
    }
}
Bool_t HRichPairAnaSim::delHitMatch(TObjArray *t)
{
    // remove hit obj from HLinearCategory
    // afterwards delete empty slot
   
    if (t)
    {
	for (Int_t i=0;i<(t->GetLast()+1);i++)
	{
	    ((TClonesArray*)((HLinearCategory*)getHitMatchCat())->getCollection())
	    ->Remove((HHitMatchSim*)((*t)[i]));
	    ((TClonesArray*)((HLinearCategory*)getHitMatchCat())
	     ->getCollection())->Compress();
	    nCounterNbRejectedHits++;
	}
	return kTRUE;
    }
    return kFALSE;
}
void HRichPairAnaSim::filter()
{

    HHitMatchSim *pHM=0;
    pIterMatchHit->Reset();
    TObjArray *tmp = new TObjArray(5);
    while(( pHM= (HHitMatchSim *)pIterMatchHit->Next()))
    {
	//cout<<"in hit match filter cut"<<endl;
	if (!isGood(pHM)) tmp->Add(pHM);
    }
    if (!delHitMatch(tmp)) Error("execute","Bad hits not deleted");
    delete tmp;
}

Bool_t HRichPairAnaSim::isGood(HHitMatchSim* h)
{
    // check certain properties of the object
    // return true or false whether it passes or not 
    
    Int_t patmat = h->getRingPatMat();
    Int_t houtra = h->getRingHouTra();

    if (houtra>=28 && patmat==0) return kTRUE;
    //    if (houtra==0 && patmat >= 200) return kTRUE;
    else return kFALSE;

}

Int_t HRichPairAnaSim::execute()
{
    nCntProcessedEvents++;
    //    Int_t nHM = ((HLinearCategory*)getHitMatchCat())->getEntries();
    //    cout<<"Nb of HitMatch objs :"<<nHM<<endl;
    HHitMatchHeaderSim *pHMH = 0;
    pIterMatchHitHeaderSim->Reset();
    while(( pHMH= (HHitMatchHeaderSim *)pIterMatchHitHeaderSim->Next()))
    {
	pHMH->dumpToStdoutSim();
    }
    //filter();
    countCorrelationObjects();
    
    //    cout<<"Number of rejected objects: "<<nCounterNbRejectedHits<<endl;
//      Int_t nRTM = countRTMobjs();
//      Int_t nRM  = countRMobjs();
//      TObjArray *pRTM = new TObjArray(5);
//      TObjArray *pRM = new TObjArray(5);
//      if (nRTM>1)
//      {
//   	Int_t nbObjs=fillRTMfullobjs(pRTM);
//   	if (nbObjs>1) calcOpeningAngle(pRTM,pHistRTMopenAngle);
//      }
//      if (nRM>1)
//      {
//  	Int_t nbObjs=fillRMobjs(pRM);
//   	if (nbObjs>1) calcOpeningAngle(pRM,pHistRMopenAngle);
//      }
//      delete pRTM;
//      delete pRM;
    return 0;
}












