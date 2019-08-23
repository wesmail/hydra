// File: hrichcorrcounter.cc
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 01/12/13 17:15:53
//
#include "hrichcorrcounter.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hcategory.h"
#include "hiterator.h"
#include <iostream.h>
#include "hdebug.h"
#include "hades.h"
#include "richdef.h"
#include "hhitmatchheader.h"
#include "hhitmatch.h"
#include "hlinearcategory.h"
#include "hrichutilfunc.h"
ClassImp(HRichCorrCounter)

HRichCorrCounter::HRichCorrCounter(Text_t *name,Text_t *title) :
  HReconstructor(name,title)
{

}

HRichCorrCounter::HRichCorrCounter()
{

}


HRichCorrCounter::~HRichCorrCounter(void) {

}


Bool_t HRichCorrCounter::init() {
        if (gHades) {
	HEvent *event=gHades->getCurrentEvent();
	HRuntimeDb *rtdb=gHades->getRuntimeDb();
	HSpectrometer *spec=gHades->getSetup();
	if (event && rtdb) {
	    HDetector *rich = spec->getDetector("Rich");
	    
	    if (rich) { //Has the user set up a RICH?

		pHitMatchCat=event->getCategory(catMatchHit);
		if (!pHitMatchCat) 
		{
		    pHitMatchCat=rich->buildCategory(catMatchHit);
		    
		    if (!pHitMatchCat) {
			Error("init","No HIT MATCH category defined");
			return kFALSE;
		    }
		    else event->addCategory(catMatchHit, pHitMatchCat, "Rich");
		}
		
		pIterMatchHit = (HIterator*)getHitMatchCat()->MakeIterator("native");
		
		pHitMatchHeaderCat=event->getCategory(catMatchHitHeader);
		if (!pHitMatchHeaderCat) 
		{
		    pHitMatchHeaderCat=rich->buildCategory(catMatchHitHeader);
		    cout<<"i have built hhitmatchheader"<<endl;
		    if (!pHitMatchHeaderCat) {
			Error("init","No HIT MATCH HEADER category defined");
			return kFALSE;
		    }
		    else event->addCategory(catMatchHitHeader, pHitMatchHeaderCat, "Rich");
		}
		
		pIterMatchHitHeader = (HIterator*)getHitMatchHeaderCat()
		    ->MakeIterator("native");
	    }
	}
	}
	iniCounters();

    return kTRUE;
}

void HRichCorrCounter::iniCounters()
{
    nCounterProcessedNbEvents=0; //total nb of processed evts
    ngNbCorrObjs=0;
    ngNb_RM=0;//only
    ngNb_RT=0;
    ngNb_RS=0;
    ngNb_MT=0;
    ngNb_MS=0;
    ngNb_RcMcT=0;//  RM MT
    ngNb_RcMcTcR=0;//RM MT RT
    ngNb_RMcTcR=0;//    MT RT
    ngNb_RcMTcR=0;// RM    RT
    ngNb_RcMcS=0;//  RM MS
    ngNb_RcMcScR=0;//RM MS RS
    ngNb_RMcScR=0;//    MS RS
    ngNb_RcMScR=0;// RM    RS
    
    ngRTM=0;//3 indexes in obj
    ngRSM=0;
    ngRichCnt=0;
    ngMdcSegCnt=0;
    ngShowerCnt=0;
    ngTofCnt=0;

    ngDRind=0;
    ngDRichminRMind=0;
    ngDRichminRMTind=0;
    ngDRichminRMSind=0;
    ngDRcMcTcRind=0;
    ngDRcMcScRind=0;
    ngDRcMTcRind=0;
    ngDRcMScRind=0;
    ngDRcMcTind=0;
    ngDRcMcSind=0;
    ngDRMcTcRind=0;
    ngDRMcScRind=0;
    ngDRMind=0;
    ngDRTind=0;
    ngDRSind=0;
    ngDMTind=0;
    ngDMSind=0;
    ngDMind=0;
    ngDTind=0;
    ngDSind=0;

    for (Int_t k=0;k<10;k++) mdcmult[k]=0;

}

Int_t HRichCorrCounter::execute()
{
    
    nCounterProcessedNbEvents++; //total nb of processed evts

    HHitMatch        *pHM          =0;
    Int_t nNbCorrObjs=0;//nb of HHitMatch objs in evt
    pIterMatchHit->Reset();
    //cout<<nCounterProcessedNbEvents-1<<endl;
    //cout<<"======================================================================"<<endl;
    while(( pHM= (HHitMatch *)pIterMatchHit->Next())) nNbCorrObjs++;

    // helper arrays to determine number of diff indexes of hits
    Int_t *tRind = new Int_t[nNbCorrObjs];//cannot be bigger than nb of corrs
    Int_t *tRichminRMind = new Int_t[nNbCorrObjs];
    Int_t *tRichminRTind = new Int_t[nNbCorrObjs];
    Int_t *tRichminRSind = new Int_t[nNbCorrObjs];
    Int_t *tRichminRMTind = new Int_t[nNbCorrObjs];
    Int_t *tRichminRMSind = new Int_t[nNbCorrObjs];
    Int_t *tRcMcTcRind = new Int_t[nNbCorrObjs];
    Int_t *tRcMcScRind = new Int_t[nNbCorrObjs];
    Int_t *tRcMTcRind = new Int_t[nNbCorrObjs];
    Int_t *tRcMScRind = new Int_t[nNbCorrObjs];
    Int_t *tRcMcTind = new Int_t[nNbCorrObjs];
    Int_t *tRcMcSind = new Int_t[nNbCorrObjs];
    Int_t *tRMcTcRind = new Int_t[nNbCorrObjs];
    Int_t *tRMcScRind = new Int_t[nNbCorrObjs];
    Int_t *tRMind = new Int_t[nNbCorrObjs];
    Int_t *tRTind = new Int_t[nNbCorrObjs];
    Int_t *tRSind = new Int_t[nNbCorrObjs];
    Int_t *tMTind = new Int_t[nNbCorrObjs];
    Int_t *tMSind = new Int_t[nNbCorrObjs];
    Int_t *tMind = new Int_t[nNbCorrObjs];
    Int_t *tTind = new Int_t[nNbCorrObjs];
    Int_t *tSind = new Int_t[nNbCorrObjs];

    for (Int_t i=0;i<nNbCorrObjs;i++) 
    {
	tRind[i]=tRichminRMind[i]=tRichminRTind[i]=tRichminRSind[i]=tRichminRMTind[i]=tRichminRMSind[i]=tRcMcTcRind[i]=tRcMcScRind[i]=tRcMTcRind[i]=tRcMScRind[i]=tRcMcTind[i]=tRcMcSind[i]=tRMcTcRind[i]=tRMcScRind[i]=tRMind[i]=tRTind[i]=tRSind[i]=tMTind[i]=tMSind[i]=tMind[i]=tTind[i]=tSind[i]=-2;
    }
    // local (for this evt) counter variables
    Int_t nlNb_RM=0;//only
    Int_t nlNb_RT=0;
    Int_t nlNb_RS=0;
    Int_t nlNb_MT=0;
    Int_t nlNb_MS=0;
    Int_t nlNb_RcMcT=0;//  RM MT
    Int_t nlNb_RcMcTcR=0;//RM MT RT
    Int_t nlNb_RMcTcR=0;//    MT RT
    Int_t nlNb_RcMTcR=0;// RM    RT
    Int_t nlNb_RcMcS=0;//  RM MS
    Int_t nlNb_RcMcScR=0;//RM MS RS
    Int_t nlNb_RMcScR=0;//    MS RS
    Int_t nlNb_RcMScR=0;// RM    RS

    Int_t nRTM=0;//3 indexes in obj
    Int_t nRSM=0;

    pIterMatchHit->Reset();
    while(( pHM= (HHitMatch *)pIterMatchHit->Next()))
    {
	//pHM->dumpToStdout();
	
	//indexes in respective hit cntr
	//useful to see whether det is in corr indep from specific one
	Int_t nRInd = pHM->getRichInd();
	Int_t nMInd = pHM->getMdcInd();
	Int_t nTInd = pHM->getTofInd();
	Int_t nSInd = pHM->getShowInd();

	Int_t tmp=0;

	if(nRInd!=-1)      tmp=fillUniqueIndex(nRInd,nNbCorrObjs,tRind);
	if(nMInd!=-1)      tmp=fillUniqueIndex(nMInd,nNbCorrObjs,tMind);
	if(nTInd!=-1)      tmp=fillUniqueIndex(nTInd,nNbCorrObjs,tTind);
	if(nSInd!=-1)      tmp=fillUniqueIndex(nSInd,nNbCorrObjs,tSind);

	//flags indicating a det bi correlation
	Int_t nRM = pHM->getMatchedRichMdc();
	Int_t nRT = pHM->getMatchedRichTof();
	Int_t nRS = pHM->getMatchedRichShower();
	Int_t nMT = pHM->getMatchedMdcTof();
	Int_t nMS = pHM->getMatchedMdcShower();
	// -------------------------------------------------
	// min a RM correlation in the tracklet
	if (nRM==1 && nRInd!=-1) tmp=fillUniqueIndex(nRInd,nNbCorrObjs,tRichminRMind);
	if (nRT==1 && nRInd!=-1) tmp=fillUniqueIndex(nRInd,nNbCorrObjs,tRichminRTind);
	if (nRS==1 && nRInd!=-1) tmp=fillUniqueIndex(nRInd,nNbCorrObjs,tRichminRSind);
	//min a tri corr obj
	if (nRInd!=-1 && nTInd!=-1 && nMInd!=-1) 
	{
	    nRTM++;
	    tmp=fillUniqueIndex(nRInd,nNbCorrObjs,tRichminRMTind);
	}
	else if (nRInd!=-1 && nSInd!=-1 && nMInd!=-1) 
	{
	    nRSM++;
	    tmp=fillUniqueIndex(nRInd,nNbCorrObjs,tRichminRMSind);
	}

	//sub cases of the two above
	if (nRInd!=-1 && nTInd!=-1 && nMInd!=-1 && 
	    nRM==1 &&  nRT==1 &&  nMT==1 ) 
	{
	    nlNb_RcMcTcR++;
	    tmp=fillUniqueIndex(nRInd,nNbCorrObjs,tRcMcTcRind);
	}

	else if (nRInd!=-1 && nSInd!=-1 && nMInd!=-1 && 
	    nRM==1 &&  nRS==1 &&  nMS==1 ) 
	{
	    nlNb_RcMcScR++;
	    tmp=fillUniqueIndex(nRInd,nNbCorrObjs,tRcMcScRind);
	}
	
	else if (nRInd!=-1 && nTInd!=-1 && nMInd!=-1 &&
		 nRM==1 && nRT==1) 
	{
	    nlNb_RcMTcR++;
	    tmp=fillUniqueIndex(nRInd,nNbCorrObjs,tRcMTcRind);
	}

	else if (nRInd!=-1 && nSInd!=-1 && nMInd!=-1 &&
		 nRM==1 && nRS==1) 
	    {
		nlNb_RcMScR++;
		tmp=fillUniqueIndex(nRInd,nNbCorrObjs,tRcMScRind);
	    }

	else if (nRInd!=-1 && nTInd!=-1 && nMInd!=-1 &&
		 nRM==1 && nMT==1) 
	{
	    nlNb_RcMcT++;
	    tmp=fillUniqueIndex(nRInd,nNbCorrObjs,tRcMcTind);
	}

	else if (nRInd!=-1 && nSInd!=-1 && nMInd!=-1 &&
		 nRM==1 && nMS==1) 
	{
	    nlNb_RcMcS++;
	    tmp=fillUniqueIndex(nRInd,nNbCorrObjs,tRcMcSind);
	}

	else if (nRInd!=-1 && nTInd!=-1 && nMInd!=-1 &&
		 nRT==1 && nMT==1) 
	{
	    nlNb_RMcTcR++;
	    tmp=fillUniqueIndex(nRInd,nNbCorrObjs,tRMcTcRind);
	}
	
	else if (nRInd!=-1 && nSInd!=-1 && nMInd!=-1 &&
		 nRS==1 && nMS==1) 
	{
	    nlNb_RMcScR++;
	    tmp=fillUniqueIndex(nRInd,nNbCorrObjs,tRMcScRind);
	}
	// only a bi corr object -----------------------------------
	if (nRInd!=-1 && nMInd!=-1 && nTInd==-1 && nSInd==-1 &&
	    nRM==1) 
	{
	    nlNb_RM++;
	    tmp=fillUniqueIndex(nRInd,nNbCorrObjs,tRMind);
	}
	else if (nRInd!=-1 && nTInd!=-1 && nMInd==-1 && nSInd==-1 &&
		 nRT==1) 
	{
	    nlNb_RT++;
	    tmp=fillUniqueIndex(nRInd,nNbCorrObjs,tRTind);
	}
	else if (nRInd!=-1 && nSInd!=-1 && nMInd==-1 && nTInd==-1 &&
		 nRS==1) 
	{
	    nlNb_RS++;
	    tmp=fillUniqueIndex(nRInd,nNbCorrObjs,tRSind);
	}
	else if (nMInd!=-1 && nTInd!=-1 && nRInd==-1 && nSInd==-1 &&
		 nMT==1) 
	{
	    nlNb_MT++;
	    tmp=fillUniqueIndex(nMInd,nNbCorrObjs,tMTind);  
	}
	else if (nMInd!=-1 && nSInd!=-1 && nRInd==-1 && nTInd==-1 &&
		 nMS==1) 
	{
	    nlNb_MS++;
	    tmp=fillUniqueIndex(nMInd,nNbCorrObjs,tMSind);
	}
    }// end while over HHitMatch
    //cout<<"======================================================================"<<endl;
    // increment resp var for the whole run
    // and fill evt wise value in HHitMatchHeader obj
    // (there are n (Rich) hits in nNbCorrObjs tracklets
    //  in this evt, but only m<n are unique)
    // t?ind contains unique hit cat indexes used in evt

    Int_t nDRind          = countDiscreteIndexes(tRind,nNbCorrObjs);
    Int_t nDRichminRMind  = countDiscreteIndexes(tRichminRMind,nNbCorrObjs);
    Int_t nDRichminRTind  = countDiscreteIndexes(tRichminRTind,nNbCorrObjs);
    Int_t nDRichminRSind  = countDiscreteIndexes(tRichminRSind,nNbCorrObjs);
    Int_t nDRichminRMTind = countDiscreteIndexes(tRichminRMTind,nNbCorrObjs);
    Int_t nDRichminRMSind = countDiscreteIndexes(tRichminRMSind,nNbCorrObjs);
    Int_t nDRcMcTcRind    = countDiscreteIndexes(tRcMcTcRind,nNbCorrObjs);
    Int_t nDRcMcScRind    = countDiscreteIndexes(tRcMcScRind,nNbCorrObjs);
    Int_t nDRcMTcRind     = countDiscreteIndexes(tRcMTcRind,nNbCorrObjs);
    Int_t nDRcMScRind     = countDiscreteIndexes(tRcMScRind,nNbCorrObjs);
    Int_t nDRcMcTind      = countDiscreteIndexes(tRcMcTind,nNbCorrObjs);
    Int_t nDRcMcSind      = countDiscreteIndexes(tRcMcSind,nNbCorrObjs);
    Int_t nDRMcTcRind     = countDiscreteIndexes(tRMcTcRind,nNbCorrObjs);
    Int_t nDRMcScRind     = countDiscreteIndexes(tRMcScRind,nNbCorrObjs);
    Int_t nDRMind         = countDiscreteIndexes(tRMind,nNbCorrObjs);
    Int_t nDRTind         = countDiscreteIndexes(tRTind,nNbCorrObjs);
    Int_t nDRSind         = countDiscreteIndexes(tRSind,nNbCorrObjs);
    Int_t nDMTind         = countDiscreteIndexes(tMTind,nNbCorrObjs);
    Int_t nDMSind         = countDiscreteIndexes(tMSind,nNbCorrObjs);
    Int_t nDMind          = countDiscreteIndexes(tMind,nNbCorrObjs);
    Int_t nDTind          = countDiscreteIndexes(tTind,nNbCorrObjs);
    Int_t nDSind          = countDiscreteIndexes(tSind,nNbCorrObjs);
    
    // delete helper arrays
    delete tRind ;
    delete tRichminRMind ;
    delete tRichminRTind ;
    delete tRichminRSind ;
    delete tRichminRMTind ;
    delete tRichminRMSind ;
    delete tRcMcTcRind ;
    delete tRcMcScRind ;
    delete tRcMTcRind ;
    delete tRcMScRind ;
    delete tRcMcTind ;
    delete tRcMcSind ;
    delete tRMcTcRind ;
    delete tRMcScRind ;
    delete tRMind ;
    delete tRTind ;
    delete tRSind ;
    delete tMTind ;
    delete tMSind ;
    delete tMind ;
    delete tTind ;
    delete tSind ;

    // 'global' var for the whole run
    ngDRind+=nDRind;        
    ngDRichminRMind+=nDRichminRMind;
    ngDRichminRTind+=nDRichminRTind;
    ngDRichminRSind+=nDRichminRSind;
    ngDRichminRMTind+=nDRichminRMTind;
    ngDRichminRMSind+=nDRichminRMSind;
    ngDRcMcTcRind+=nDRcMcTcRind;  
    ngDRcMcScRind+=nDRcMcScRind;  
    ngDRcMTcRind+=nDRcMTcRind;   
    ngDRcMScRind+=nDRcMScRind;   
    ngDRcMcTind+=nDRcMcTind;    
    ngDRcMcSind+=nDRcMcSind;    
    ngDRMcTcRind+=nDRMcTcRind;   
    ngDRMcScRind+=nDRMcScRind;   
    ngDRMind+=nDRMind;       
    ngDRTind+=nDRTind;       
    ngDRSind+=nDRSind;       
    ngDMTind+=nDMTind;       
    ngDMSind+=nDMSind;       
    ngDMind+=nDMind;        
    ngDTind+=nDTind;        
    ngDSind+=nDSind;   

    ngNbCorrObjs+=nNbCorrObjs;

    HHitMatchHeader *pHMH = 0;
    pIterMatchHitHeader->Reset();
    while(( pHMH= (HHitMatchHeader *)pIterMatchHitHeader->Next()))
    {
	
 	

	pHMH->setNbCorrObjs(nNbCorrObjs);
	//pHMH->setNbRichHits(nRichCnt);
	pHMH->setNbDiscreteRichHits(nDRind);
	pHMH->setNbDiscreteRichRMHits(nDRichminRMind);
	pHMH->setNbDiscreteRichonlyRMHits(nDRMind);
	
	//	pHMH->setNbMdcSegHits(nMdcSegCnt);
	pHMH->setNbDiscreteMdcSegHits(nDMind);
	//pHMH->setNbShowerHits(nShowerCnt);
	pHMH->setNbDiscreteShowerHits(nDSind);
	
	//pHMH->setNbTofHits(nTofCnt);
	pHMH->setNbDiscreteTofHits(nDTind);
	
// 	ngRichCnt  +=nRichCnt;
// 	ngMdcSegCnt+=nMdcSegCnt;
// 	ngShowerCnt+=nShowerCnt;
// 	ngTofCnt   +=nTofCnt;

    ngNb_RM+=nlNb_RM;//only these two are matched
    pHMH->setNb_RM(nlNb_RM);
    ngNb_RT+=nlNb_RT;
    pHMH->setNb_RT(nlNb_RT);
    ngNb_RS+=nlNb_RS;
    pHMH->setNb_RS(nlNb_RS);
    ngNb_MT+=nlNb_MT;
    pHMH->setNb_MT(nlNb_MT);
    ngNb_MS+=nlNb_MS;
    pHMH->setNb_MS(nlNb_MS);
    // three diff dets contrib
    ngNb_RcMcT+=nlNb_RcMcT;//        RM MT
    pHMH->setNb_RcMcT(nlNb_RcMcT);
    ngNb_RcMcTcR+=nlNb_RcMcTcR;//    RM MT RT
    pHMH->setNb_RcMcTcR(nlNb_RcMcTcR);
    ngNb_RMcTcR+=nlNb_RMcTcR;//         MT RT
    pHMH->setNb_RMcTcR(nlNb_RMcTcR);
    ngNb_RcMTcR+=nlNb_RcMTcR;//      RM    RT
    pHMH->setNb_RcMTcR(nlNb_RcMTcR);
    ngNb_RcMcS+=nlNb_RcMcS;//        RM MS
    pHMH->setNb_RcMcS(nlNb_RcMcS);
    ngNb_RcMcScR+=nlNb_RcMcScR;//    RM MS RS
    pHMH->setNb_RcMcScR(nlNb_RcMcScR);
    ngNb_RMcScR+=nlNb_RMcScR;//         MS RS
    pHMH->setNb_RMcScR(nlNb_RMcScR);
    ngNb_RcMScR+=nlNb_RcMScR;//      RM    RS
    pHMH->setNb_RcMScR(nlNb_RcMScR);
    ngRTM+=nRTM;//min 3 indexes in obj indep of corr
    pHMH->setNb_minRMT(nRTM);
    ngRSM+=nRSM;
    pHMH->setNb_minRMS(nRSM);
     

    //ngNbCorrObjs+=pHMH->getNbCorrObjs();
    ngRichCnt+=pHMH->getNbRichHits();
    ngMdcSegCnt+=pHMH->getNbMdcSegHits();
    ngShowerCnt+=pHMH->getNbShowerHits();
    ngTofCnt+=pHMH->getNbTofHits();
    // ngDRichCnt+=pHMH->getNbDiscreteRichHits();
//     ngDMdcSegCnt+=pHMH->getNbDiscreteMdcSegHits();
//     ngDShowerCnt+=pHMH->getNbDiscreteShowerHits();
//     ngDTofCnt+=pHMH->getNbDiscreteTofHits();
//     ngNb_RcMcT+=pHMH->getNb_RcMcT();
//     ngNb_RcMcTcR+=pHMH->getNb_RcMcTcR();
//     ngNb_RMcTcR+=pHMH->getNb_RMcTcR();
//     ngNb_RcMTcR+=pHMH->getNb_RcMTcR();
//     ngNb_RcMcS+=pHMH->getNb_RcMcS();
//     ngNb_RcMcScR+=pHMH->getNb_RcMcScR();
//     ngNb_RMcScR+=pHMH->getNb_RMcScR();
//     ngNb_RcMScR+=pHMH->getNb_RcMScR();
//     ngRTM+=pHMH->getNb_minRMT();
//     ngRSM+=pHMH->getNb_minRMS();
//     ngNb_RM+=pHMH->getNb_RM();
//     ngNb_RT+=pHMH->getNb_RT();
//     ngNb_RS+=pHMH->getNb_RS();
//     ngNb_MT+=pHMH->getNb_MT();
//     ngNb_MS+=pHMH->getNb_MS();
	
    }

    // this section is supposed to determine how many 
    // rings are correlated with 1, 2, 3, ... n MDC segments and/or META hits
     Int_t nHM = ((HLinearCategory*)getHitMatchCat())->getEntries();
     //cout<<"number of hitmatch "<<nHM<<endl;
     if (nHM==0) return 0; // nothing to be done
     Int_t *nRInd = new Int_t[nHM]; // arrays to store hit cat indexes
     Int_t *nMInd = new Int_t[nHM];
     Int_t *nTInd = new Int_t[nHM];
     Int_t *nSInd = new Int_t[nHM];
     Int_t *nHInd = new Int_t[nHM];
     // initialization needed for function like fillUniqueIndexes
     for (Int_t i=0;i<nHM;i++) nRInd[i]=nMInd[i]=nTInd[i]=nSInd[i]=nHInd[i]=-2;
     Int_t ntc = 0;
     pIterMatchHit->Reset();
     while(( pHM= (HHitMatch *)pIterMatchHit->Next()))
     {
 	nRInd[ntc] = pHM->getRichInd();
 	nMInd[ntc] = pHM->getMdcInd();
 	nTInd[ntc] = pHM->getTofInd();
 	nSInd[ntc] = pHM->getShowInd();
	nHInd[ntc] = ((HLinearCategory*)getHitMatchCat())->getIndex(pHM);
 	ntc++;
     }
    
     Int_t *six = new Int_t[nHM]; // sorted indexes
     TMath::Sort(nHM,nRInd,six); // descending
     //((HHitMatch*)getHitMatchCat()->getObject(nHMInd[idx]))
     Int_t ni, ne; // first and last index of the same ring in the sorted arr
     Int_t swt = 0; // a switch with the decision to count the ring or not
     ni=ne=0;
     Int_t nRichRingCnt=0;
     Int_t nMdcSegCnt=0;
     //     cout<<"before "<<ni<<" "<<ne<<endl;
     while(ni<nHM)
     {
	ne=ni;
 	//cout<<"while "<<ni<<" "<<ne<<endl;
 	for (Int_t j=ni;j<nHM;j++)
 	{
	    //  cout<<"for 1 ni "<<ni<<" ne "<<ne<<" j "<<j<<" "<<nHM<<endl;
	    // 	    ne=ni;
 	    Int_t idx   = six[j];
 	    Int_t idxp1 = idx;
	    if (nRInd[idx] <= -1) 
	    {
		swt = 1; // not a tracklet with a ring
		break;
	    }
 	    if (j+1 < nHM) idxp1 = six[j+1]; // get index of the next
 	    else 
	    {
		swt = 2;
		break; // the last tracklet
	    }
		//cout<<"RICH index:  "<<nRInd[idx]<<"  "<<nRInd[idxp1]<<endl;
		//cout<<"MDC index:  "<<nMInd[idx]<<"  "<<nMInd[idxp1]<<endl;
	    if ( nRInd[idx] == nRInd[idxp1] ) // next tracklet has same RICH ring
	    {
		ne=j+1; //lastindex of subset incremented
		//((HHitMatch*)getHitMatchCat()->getObject(nHMInd[idx]))
		//->dumpToStdout();
	    }
	    else
	    {
		swt = 2; // next tracklet has different or no RICH ring
		break;
	    }
	    //cout<<"for 2 ni "<<ni<<" ne "<<ne<<" j "<<j<<" "<<nHM<<endl;
 	} // end for
 	
 	// now we know ninitial and nend in the sorted index array of rings for one ring
	//cout<<"ne "<<ne<<" ni "<<ni<<endl;
 	/*
	  ne-ni = 0 && swt=0 --> impossible
	  ne-ni = 0 && swt=1 --> not a tracklet with a ring --> skip it
	  ne-ni = 0 && swt=2 --> it was the last one        --> check it for MDC corr
	  ne-ni = 0 && swt=2 --> the ring of this tracklet doesn't occur twice in evt 
	  */
	Int_t submax=ne-ni;
	if (submax==0)
	{
	    switch(swt)
	    {
	    case 0:
		{
		    cout<<"ERROR: ne-ni == 0 && swt == 0"<<endl;
		    break;
		}
	    case 1:
		{
		    break;
		}
	    case 2:
		{
		    //cout<<"submax in case 2: "<<submax<<endl;
		    submax+=1;
		    Int_t *m = new Int_t[submax];
		    Int_t *t = new Int_t[submax];
		    Int_t *s = new Int_t[submax];
		    for (Int_t kk=0;kk<submax;kk++) m[kk]=t[kk]=s[kk]=-2;
		    for (Int_t k=ni;k<ni+submax;k++)
		    {
			Int_t idx   = six[k];
			HHitMatch *hm = ((HHitMatch*)getHitMatchCat()->getObject(nHInd[idx]));
			Int_t rm=hm->getMatchedRichMdc();
			Int_t rt=hm->getMatchedRichTof();
			Int_t rs=hm->getMatchedRichShower();
			if (nMInd[idx]>-1 && rm==1) fillUniqueIndex(nMInd[idx],submax,m);
			if (nTInd[idx]>-1 && rt==1) fillUniqueIndex(nTInd[idx],submax,t);
			if (nSInd[idx]>-1 && rs==1) fillUniqueIndex(nSInd[idx],submax,s);
		    }
		    nRichRingCnt++;
		    if (countDiscreteIndexes(m,submax)>1) cout<<"Error Error"<<endl;
		    if (countDiscreteIndexes(m,submax)) nMdcSegCnt=countDiscreteIndexes(m,submax);
		    mdcmult[countDiscreteIndexes(m,submax)]++;
		    tofmult[countDiscreteIndexes(t,submax)]++;
		    shomult[countDiscreteIndexes(s,submax)]++;
		    delete m;
		    delete t;
		    delete s;
		    break;
		}
	    }

 	}
	else
	{
	    submax+=1;
	    //cout<<"submax in else: "<<submax<<endl;
	    Int_t *m = new Int_t[submax];
	    Int_t *t = new Int_t[submax];
	    Int_t *s = new Int_t[submax];
	    for (Int_t kk=0;kk<submax;kk++) m[kk]=t[kk]=s[kk]=-2;
	    Int_t hlp=-1;
	    for (Int_t k=ni;k<ni+submax;k++)
	    {
		Int_t idx   = six[k];
		HHitMatch *hm = ((HHitMatch*)getHitMatchCat()->getObject(nHInd[idx]));
		Int_t rm=hm->getMatchedRichMdc();
		Int_t rt=hm->getMatchedRichTof();
		Int_t rs=hm->getMatchedRichShower();
		if (nMInd[idx]>-1 && rm==1) fillUniqueIndex(nMInd[idx],submax,m);
		if (nTInd[idx]>-1 && rt==1) fillUniqueIndex(nTInd[idx],submax,t);
		if (nSInd[idx]>-1 && rs==1) fillUniqueIndex(nSInd[idx],submax,s);
		if (hlp !=-1 && hlp!=nRInd[idx]) cout<<"Error !!!!!!!!!"<<endl;
		hlp=nRInd[six[k]];
		//if (nMInd[idx]>-1 && rm==1) cout<<"k "<<k<<" six[k] "<<six[k]<<" nMInd[idx] "<<nMInd[idx]<<endl;
		//if (nRInd[idx]>-1 && rm==1) cout<<"k "<<k<<" six[k] "<<six[k]<<" nRInd[idx] "<<nRInd[idx]<<endl;

	    }
	    nRichRingCnt++;
	    if (countDiscreteIndexes(m,submax)) nMdcSegCnt=countDiscreteIndexes(m,submax);
	    //cout<<"discrete mdc cnt "<<countDiscreteIndexes(m,submax)<<endl;
	    mdcmult[countDiscreteIndexes(m,submax)]++;
	    tofmult[countDiscreteIndexes(t,submax)]++;
	    shomult[countDiscreteIndexes(s,submax)]++;
	    delete m;
	    delete t;
	    delete s; 
	}
	ni=ne+1;// go to next tracklet
     } // end while
     //      for (Int_t kkk=0;kkk<10;kkk++)
     //      {
     //  	if (kkk>0 && mdcmult[kkk]!=0) cout<<mdcmult[kkk]<<" ring(s) have "<<kkk
     //  			       <<" correlated MDC 1 segments"<<endl;
     //      }

     // compare discrete Nb of RichHits
     // if (nRichRingCnt) cout<<"  nRichRingCnt: // "<<nRichRingCnt<<" --";
//      if (nRichRingCnt && nDRichminRMind) 
//      {
// 	 cout<<"-- nDRind: "<<nDRind<<endl;
// 	 cout<<"nDRichminRMind: "<<nDRichminRMind<<"  nMdcSegCnt: "<<nMdcSegCnt<<endl;
// 	 cout<<"%%%%%%%%%%%%%%%%%%%%%% end of evt %%%%%%%%%%%%%%%%%%%%%%%%%"<<endl;
//      }
//      else if (nRichRingCnt && !nDRichminRMind)
//      {
// 	 cout<<endl;
// 	 cout<<"%%%%%%%%%%%%%%%%%%%%%% end of evt %%%%%%%%%%%%%%%%%%%%%%%%%"<<endl;
//      }
//      if (nDRind != nRichRingCnt ) 
//      {
// 	 cout<<"Error: nDRind: "<<nDRind<<"  nRichRingCnt: "<<nRichRingCnt<<endl;
//      }
//      if (nMdcSegCnt==4)
//      {
// 	 pIterMatchHit->Reset();
// 	 while(( pHM= (HHitMatch *)pIterMatchHit->Next()))
// 	 {
// 	     //	  pHM->dumpToStdout();
// 	 }
    


     // }
     delete six;
     //     
     return kTRUE;
}

Bool_t HRichCorrCounter::finalize() {
    
    dumpCorrelationStatus();
    
    return kTRUE;
}
Int_t HRichCorrCounter::fillUniqueIndex(Int_t i,Int_t max,Int_t* iarr)
{
    // check if index i is already stored in iarr
    // if not then store it
    // iarr is initialized to -2
    
    Int_t n=0;
    do{
	if(i==iarr[n]) return 0;//i already stored
	else if(iarr[n]==-2) //new slot
	{
	    iarr[n]=i;
	    return n+1;
	}
	n++;
    } while(n<max);
    return 0;
}
Int_t HRichCorrCounter::countDiscreteIndexes(Int_t *iarr,Int_t max)
{
    //count arr slots that are different from -2 (ini value)
    Int_t n=0;
    for(Int_t i=0;i<max;i++)
    {
	if(iarr[i]!=-2 && iarr[i]!=-1) n++;
	else break;
    }
    return n;
}
void HRichCorrCounter::dumpCorrelationStatus()
{
    cout<<"*******************************************"<<endl;
    cout<<"*** INFORMATION ***************************"<<endl;
    cout<<"*******************************************"<<endl;
//    cout<<"this status information belongs to the tree in ..."<<endl;
//     cout<<((TFile*)gHades->getInputFile())->GetName()<<endl;
//     TString filename(((TFile*)gHades->getInputFile())->GetName());
//     filename.Remove(filename.Length()-5, filename.Length());
//    filename.Append(".stat");
    //    std::ofstream statusOut(filename.Data(),ios::out);
//     cout<<"Nb of evts processed   : "<<nCounterProcessedNbEvents<<endl;
//     cout<<"Nb of tracklets : "<<ngNbCorrObjs<<endl;
//     if(ngRichCnt>0)     cout<<"Tot Nb of RICH hits   : "<<ngRichCnt<<endl;
//     if(ngMdcSegCnt>0)   cout<<"Tot Nb of MDCSEG hits : "<<ngMdcSegCnt<<endl;
//     if(ngTofCnt>0)      cout<<"Tot Nb of TOF hits    : "<<ngTofCnt<<endl;
//     if(ngShowerCnt>0)   cout<<"Tot Nb of SHOWER hits : "<<ngShowerCnt<<endl;
//     if(ngDRichCnt>0)    cout<<"Nb of different RICH hits in tracklets   : "
// 			    <<ngDRichCnt<<endl;
//     if(ngDMdcSegCnt>0)  cout<<"Nb of different MDCSEG hits in tracklets : "
// 			    <<ngDMdcSegCnt<<endl;
//     if(ngDTofCnt>0)     cout<<"Nb of different TOF hits in tracklets    : "
// 			    <<ngDTofCnt<<endl;
//     if(ngDShowerCnt>0)  cout<<"Nb of different SHOWER hits in tracklets : "
// 			    <<ngDShowerCnt<<endl;
//     if(ngNb_RM>0)       cout<<"Nb of only RM corr : "<<ngNb_RM<<endl;
//     if(ngNb_RT>0)       cout<<"Nb of only RT corr : "<<ngNb_RT<<endl;
//     if(ngNb_RS>0)       cout<<"Nb of only RS corr : "<<ngNb_RS<<endl;
//     if(ngNb_MT>0)       cout<<"Nb of only MT corr : "<<ngNb_MT<<endl;
//     if(ngNb_MS>0)       cout<<"Nb of only MS corr : "<<ngNb_MS<<endl;
//     if(ngRTM>0)         cout<<"Nb of min RMT indexes in tracklet : "
// 			    <<ngRTM<<endl;
//     if(ngRSM>0)         cout<<"Nb of min RMS indexes in tracklet : "
// 			    <<ngRSM<<endl;
//     if(ngNb_RcMcT>0)    cout<<"Nb of exact RM MT    corr : "<<ngNb_RcMcT<<endl;
//     if(ngNb_RcMcTcR>0)  cout<<"Nb of exact RM MT RT corr : "<<ngNb_RcMcTcR<<endl;
//     if(ngNb_RMcTcR>0)   cout<<"Nb of exact    MT RT corr : "<<ngNb_RMcTcR<<endl;
//     if(ngNb_RcMTcR>0)   cout<<"Nb of exact RM    RT corr : "<<ngNb_RcMTcR<<endl;
//     if(ngNb_RcMcS>0)    cout<<"Nb of exact RM MS    corr : "<<ngNb_RcMcS<<endl;
//     if(ngNb_RcMcScR>0)  cout<<"Nb of exact RM MS RS corr : "<<ngNb_RcMcScR<<endl;
//     if(ngNb_RMcScR>0)   cout<<"Nb of exact    MS RS corr : "<<ngNb_RMcScR<<endl;
//     if(ngNb_RcMScR>0)   cout<<"Nb of exact RM    RS corr : "<<ngNb_RcMScR<<endl;
    
     cout<<"Nb of evts processed   : "<<nCounterProcessedNbEvents<<endl;
    cout<<"Nb of tracklets created: "<<ngNbCorrObjs<<endl;
    if(ngRichCnt>0)     cout<<"Tot Nb of RICH hits   : "<<ngRichCnt<<endl;
    if(ngMdcSegCnt>0)   cout<<"Tot Nb of MDCSEG hits : "<<ngMdcSegCnt<<endl;
    if(ngTofCnt>0)      cout<<"Tot Nb of TOF hits    : "<<ngTofCnt<<endl;
    if(ngShowerCnt>0)   cout<<"Tot Nb of SHOWER hits : "<<ngShowerCnt<<endl;

    if(ngDRind>0)           cout<<"Nb of diff Rich   hits : "<<ngDRind<<endl;
    if(ngDMind>0)           cout<<"Nb of diff Mdc    hits : "<<ngDMind<<endl;
    if(ngDTind>0)           cout<<"Nb of diff Tof    hits : "<<ngDTind<<endl;
    if(ngDSind>0)           cout<<"Nb of diff Shower hits : "<<ngDSind<<endl;
    

    if(ngDRichminRMind>0)   cout<<"Nb of diff Rich hits in min RM corr : "<<ngDRichminRMind<<endl;
    if(ngDRichminRTind>0)   cout<<"Nb of diff Rich hits in min RT corr : "<<ngDRichminRTind<<endl;
    if(ngDRichminRSind>0)   cout<<"Nb of diff Rich hits in min RS corr : "<<ngDRichminRSind<<endl;
    if(ngDRichminRMTind>0)  cout<<"Nb of diff Rich hits in min RMT index : "<<ngDRichminRMTind<<endl;
    if(ngDRichminRMSind>0)  cout<<"Nb of diff Rich hits in min RMS index : "<<ngDRichminRMSind<<endl;
    if(ngDRcMcTcRind>0)     cout<<"Nb of diff Rich hits in RM MT RT : "<<ngDRcMcTcRind<<endl;
    if(ngDRcMcScRind>0)     cout<<"Nb of diff Rich hits in RM MS RS : "<<ngDRcMcScRind<<endl;
    if(ngDRcMTcRind>0)      cout<<"Nb of diff Rich hits in RM    RT : "<<ngDRcMTcRind<<endl;
    if(ngDRcMScRind>0)      cout<<"Nb of diff Rich hits in RM    RS : "<<ngDRcMScRind<<endl;
    if(ngDRcMcTind>0)       cout<<"Nb of diff Rich hits in RM MT    : "<<ngDRcMcTind<<endl;
    if(ngDRcMcSind>0)       cout<<"Nb of diff Rich hits in RM MS    : "<<ngDRcMcSind<<endl;
    if(ngDRMcTcRind>0)      cout<<"Nb of diff Rich hits in    MT RT : "<<ngDRMcTcRind <<endl;
    if(ngDRMcScRind>0)      cout<<"Nb of diff Rich hits in    MS RS : "<<ngDRMcScRind<<endl;
    if(ngDRMind>0)          cout<<"Nb of diff Rich hits in only RM : "<<ngDRMind<<endl;
    if(ngDRTind>0)          cout<<"Nb of diff Rich hits in only RT : "<<ngDRTind<<endl;
    if(ngDRSind>0)          cout<<"Nb of diff Rich hits in only RS : "<<ngDRSind <<endl;
    if(ngDMTind>0)          cout<<"Nb of diff Mdc hits in only MT : "<<ngDMTind<<endl;
    if(ngDMSind>0)          cout<<"Nb of diff Mdc hits in only MS : "<<ngDMSind<<endl;

    if(ngNb_RM>0)       cout<<"Nb of only RM corr : "<<ngNb_RM<<endl;
    if(ngNb_RT>0)       cout<<"Nb of only RT corr : "<<ngNb_RT<<endl;
    if(ngNb_RS>0)       cout<<"Nb of only RS corr : "<<ngNb_RS<<endl;
    if(ngNb_MT>0)       cout<<"Nb of only MT corr : "<<ngNb_MT<<endl;
    if(ngNb_MS>0)       cout<<"Nb of only MS corr : "<<ngNb_MS<<endl;
    if(ngRTM>0)         cout<<"Nb of min RMT indexes in tracklet : "
			    <<ngRTM<<endl;
    if(ngRSM>0)         cout<<"Nb of min RMS indexes in tracklet : "
			    <<ngRSM<<endl;
    if(ngNb_RcMcT>0)    cout<<"Nb of exact RM MT    corr : "<<ngNb_RcMcT<<endl;
    if(ngNb_RcMcTcR>0)  cout<<"Nb of exact RM MT RT corr : "<<ngNb_RcMcTcR<<endl;
    if(ngNb_RMcTcR>0)   cout<<"Nb of exact    MT RT corr : "<<ngNb_RMcTcR<<endl;
    if(ngNb_RcMTcR>0)   cout<<"Nb of exact RM    RT corr : "<<ngNb_RcMTcR<<endl;
    if(ngNb_RcMcS>0)    cout<<"Nb of exact RM MS    corr : "<<ngNb_RcMcS<<endl;
    if(ngNb_RcMcScR>0)  cout<<"Nb of exact RM MS RS corr : "<<ngNb_RcMcScR<<endl;
    if(ngNb_RMcScR>0)   cout<<"Nb of exact    MS RS corr : "<<ngNb_RMcScR<<endl;
    if(ngNb_RcMScR>0)   cout<<"Nb of exact RM    RS corr : "<<ngNb_RcMScR<<endl;
    for (Int_t k=0;k<10;k++)
    {
	if (k!=0 && mdcmult[k]!=0) cout<<mdcmult[k]<<" ring(s) have "<<k
				       <<" correlated MDC 1 segments"<<endl;
	if (k!=0 && tofmult[k]!=0) cout<<tofmult[k]<<" ring(s) have "<<k
				       <<" correlated TOF hits"<<endl;
	if (k!=0 && shomult[k]!=0) cout<<shomult[k]<<" ring(s) have "<<k
				       <<" correlated SHOWER hits"<<endl;

    }
    
}










