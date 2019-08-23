// $Id: hrichtrackletfiltersim.cc,v 1.8 2009-07-15 11:39:22 halo Exp $
// Last update by Thomas Eberl: 02/09/25 17:46:36
//
using namespace std;
#include "hrichtrackletfiltersim.h"
#include "hlinearcategory.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hrichdetector.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hades.h"
#include "richdef.h"
#include "TObjArray.h"
#include "hhitmatchsim.h"
#include "hhitmatchheadersim.h"
#include "hgeantkine.h"
#include "hrichutilfunc.h"
#include <iostream> 
#include <iomanip>
ClassImp(HRichTrackletFilterSim)

HRichTrackletFilterSim::HRichTrackletFilterSim(const Text_t *name,const Text_t *title) :
  HReconstructor(name,title)
{

}

HRichTrackletFilterSim::HRichTrackletFilterSim()
{

}


HRichTrackletFilterSim::~HRichTrackletFilterSim(void) {
 
}

Bool_t HRichTrackletFilterSim::init() {
    if (gHades) {
	HEvent *event=gHades->getCurrentEvent();
	HRuntimeDb *rtdb=gHades->getRuntimeDb();
	HSpectrometer *spec=gHades->getSetup();
	if (event && rtdb) {
	    HDetector *rich = spec->getDetector("Rich");
	     // HGEANT KINE INFO
	    fGeantKineCat =(HLinearCategory*) event->getCategory(catGeantKine);
	    if (!fGeantKineCat) 
	    { 
		Error("HRichCorrelatorSim::init",
		      "no GEANT Kine category available");
	    }                
	    iter_kine = (HIterator *)fGeantKineCat ->MakeIterator("native");
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

Bool_t HRichTrackletFilterSim::finalize() {
  
    return kTRUE;
}

Bool_t HRichTrackletFilterSim::delHitMatch(TObjArray *t)
{
    // remove hit obj from HLinearCategory
    // afterwards delete empty slot
   
    if (t)
    {
	for (Int_t i=0;i<(t->GetLast()+1);i++)
	{
	    ((TClonesArray*)((HLinearCategory*)getHitMatchCat())
	     ->getCollection())->Remove( (HHitMatchSim*)((*t)[i]) );
	    ((TClonesArray*)((HLinearCategory*)getHitMatchCat())
	     ->getCollection())->Compress();
	    nCounterNbRejectedHits++;
	}
	return kTRUE;
    }
    return kFALSE;
}

void HRichTrackletFilterSim::filterTrackletIndividual(Int_t swt)
{
    // swt: is a switch parameter 
    //      selecting the filtering method to apply
    //
    // filter tracklets only according to their own internal state 
    HHitMatchSim *pHM=0;
    pIterMatchHit->Reset();
    TObjArray *tmp = new TObjArray(5);
    while(( pHM= (HHitMatchSim *)pIterMatchHit->Next()))
    {
	//cout<<"in hit match filter cut"<<endl;
	if (!isGood(pHM,swt)) tmp->Add(pHM);
    }
    if (!delHitMatch(tmp)) Error("execute","Bad hits not deleted");
    delete tmp;
}

void HRichTrackletFilterSim::filterTrackletInGroup(Int_t swt)
{
    // swt: is a switch parameter 
    //      selecting the filtering method to apply
    //
    // filter tracklets according to their relation to others 

    switch(swt)
    {
    case 0:
	{
	HHitMatchSim *pHM=0;
	pIterMatchHit->Reset();
	Int_t nHM = ((HLinearCategory*)getHitMatchCat())->getEntries();
	cout<<"Nb of tracklets : "<<nHM<<endl;
	if (nHM==0) break;
	TObjArray *pArr = new TObjArray[nHM+1];
	while(( pHM= (HHitMatchSim *)pIterMatchHit->Next())) pArr->Add(pHM);
	TObjArray *deltmp = new TObjArray(5);

	Int_t nMaxArr = pArr->GetLast()+1;
	for (Int_t i=0;i<nMaxArr-1;i++)
	{
	    //nCnt++;
	    Bool_t isSingle=kTRUE;
	    
	    for (Int_t j=i+1;j<nMaxArr;j++)
	    {
		cout<<"index i: "<<i<<" / index j: "<<j<<"   ";
		Int_t r1 = ((HHitMatchSim*)(*pArr)[i])->getRichInd();
		Int_t r2 = ((HHitMatchSim*)(*pArr)[j])->getRichInd();
		Int_t m1 = ((HHitMatchSim*)(*pArr)[i])->getMdcInd();
		Int_t m2 = ((HHitMatchSim*)(*pArr)[j])->getMdcInd();
		cout<<"r1: "<<r1<<" r2: "<<r2<<" m1: "<<m1<<" m2: "<<m2<<endl;
		
		if (r1==r2 && m1==m2) 
		{
		    
		    HHitMatchSim* ht = ((HHitMatchSim*)(*pArr)[i]);
		    if (deltmp->IndexOf(ht)==-1) deltmp->Add(ht);
		    // avoid doubles
		    cout<<"removed index i: "<<i<<" / r1: "<<r1<<" m1: "<<m1<<endl;
		}
		if (r1==r2)  isSingle=kFALSE;
	    }
	    
	    if (isSingle)
	    {
		HHitMatchSim* ht = ((HHitMatchSim*)(*pArr)[i]);
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


Bool_t HRichTrackletFilterSim::isGood(HHitMatchSim* h,Int_t swt)
{
    // check certain properties of the object
    // return true or false whether it passes or not 
    
    Bool_t kSwitch = kFALSE;
    HTrackInfo* t = h->getTrackInfoObj();
    Int_t nPartNr = t->getPartNr();
    switch (swt)
    {

    case 0: 
	{
	    if (nPartNr>=2) kSwitch=kTRUE;
	    break;
	}
	
    case 1:
	{
	    if (h->getRichInd() > -1) kSwitch=kTRUE;
	    break;
	}

    case 2:
	{
	    for (Int_t i=0;i<nPartNr;i++)
	    {
		if (t->getParId(i)==2 || t->getParId(i)==3)
		{
		    HGeantKine* lepton = HRichUtilFunc::getKineObj(
					      t->getTrkNb(i),
					      (HLinearCategory*)
					      getGeantKineCat()  );
		    Int_t aPar, aMed, aMech;
		    lepton->getCreator(aPar,aMed,aMech);
		    if (aMed>=8 && aMed<=19) //RICH,target,solid radiators
		    {
			HGeantKine *lepton_parent = 
			    HRichUtilFunc::findParent(lepton,
				  (HLinearCategory*) getGeantKineCat());
			if(lepton_parent){
			    Int_t aTrackp, nLepParID;
			    lepton_parent->getParticle(aTrackp,nLepParID);
			    //h->dumpToStdoutSim();
			    
			    if ((nLepParID==1 && aMech==6) || //gamma conv
				(nLepParID==7 && aMech==5))   //pi0 Dalitz
			    {
				//HRichUtilFunc::dumpKine(lepton_parent);
				kSwitch=kTRUE;
			    }
			}
		    }
		}
	    }
	}
	
    }
    return kSwitch;

}


Int_t HRichTrackletFilterSim::execute()
{
   
    filterTrackletIndividual(2);
    Int_t kReturnValue=0;
    return kReturnValue;
}



void HRichTrackletFilterSim::resetCounters()
{
    nCntRTM=0;
    nCntRTMfull=0;
    nCntRTMnoTM=0;
    nCntRTMnoTR=0;
    nCntRTMnoRM=0;
    nCntProcessedObjs=0;
}
Int_t HRichTrackletFilterSim::countCorrelationObjects()
{
    HHitMatchSim *pHM=0;

    pIterMatchHit->Reset();
    Int_t nC=0;
    while(( pHM= (HHitMatchSim *)pIterMatchHit->Next()))
    {
	//cout<<"in HHitMatchSim loop !"<<endl;
	//pHM->dumpToStdoutSim();
	nCntProcessedObjs++;
	nC++;
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
    return nC;

}
void HRichTrackletFilterSim::dumpCorrelationObjectStat()
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






