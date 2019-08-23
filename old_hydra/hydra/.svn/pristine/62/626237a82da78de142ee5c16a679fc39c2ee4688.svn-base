// $Id: hrichcorrcounter.cc,v 1.10 2009-07-15 11:39:21 halo Exp $
// Last update by Thomas Eberl: 02/09/25 17:30:18
//
using namespace std;
#include "hrichcorrcounter.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hcategory.h"
#include "hiterator.h"
#include <iostream> 
#include <iomanip>
#include "hdebug.h"
#include "hades.h"
#include "richdef.h"
#include "hhitmatchheader.h"
#include "hhitmatch.h"
#include "hlinearcategory.h"
#include "hrichutilfunc.h"
#include "hkicktrack.h"
#include "kickdef.h"
ClassImp(HRichCorrCounter)

HRichCorrCounter::HRichCorrCounter(const Text_t *name,const Text_t *title) :
  HReconstructor(name,title)
{

}

HRichCorrCounter::HRichCorrCounter()
{

}

HRichCorrCounter::HRichCorrCounter(const Text_t *name,const Text_t *title,const Char_t* filename) :
  HReconstructor(name,title)
{
    pFileName  = filename;// output filename for diagnostic histos
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
	    fKickTrackCat = gHades->getCurrentEvent()->getCategory(catKickTrack);
	    if (!fKickTrackCat) {
		Error("init","KickTrack not found in input file");
		return kFALSE;
	    }
	    iterTracks = (HIterator *)fKickTrackCat->MakeIterator("native");
	    cout<<"itertracks :"<<iterTracks<<endl;

	}
	}
	iniCounters();
	pFileOut = new TFile(pFileName.Data(),"RECREATE");
	iniHistos();
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
    ngDRichminRTind=0;
    ngDRichminRSind=0;
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

    for (Int_t k=0;k<10;k++) mdcmult[k]=shomult[k]=tofmult[k]=0;

}


Int_t HRichCorrCounter::execute()
{
    
    nCounterProcessedNbEvents++; //total nb of processed evts

    HHitMatch *pHM=0;
    Int_t nNbCorrObjs=0;//nb of HHitMatch objs in evt
    pIterMatchHit->Reset();
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


    // ***************************************************************
    // ***** loop over HHitMatch *************************************
    // ***************************************************************
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

	pHMH->setNbDiscreteRichHits(nDRind);
	pHMH->setNbDiscreteRichRMHits(nDRichminRMind);//at least a RM corr
	pHMH->setNbDiscreteRichonlyRMHits(nDRMind);//only RM, nothing more
	
	pHMH->setNbDiscreteMdcSegHits(nDMind);

	pHMH->setNbDiscreteShowerHits(nDSind);

	pHMH->setNbDiscreteTofHits(nDTind);
	
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
	

	ngRichCnt+=pHMH->getNbRichHits();
	ngMdcSegCnt+=pHMH->getNbMdcSegHits();
	ngShowerCnt+=pHMH->getNbShowerHits();
	ngTofCnt+=pHMH->getNbTofHits();
    } //end while HHitMatchheader

    //***********************************************************************
    // this section is supposed to determine how many 
    // rings are correlated with 1, 2, 3, ... n MDC segments and/or META hits
     Int_t nHM = ((HLinearCategory*)getHitMatchCat())->getEntries();

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
     // example how to retrieve obj from cat knowing idx
     //((HHitMatch*)getHitMatchCat()->getObject(nHMInd[idx]))
     Int_t ni, ne; // first and last index of the same ring in the sorted arr
     Int_t swt = 0; // a switch with the decision to count the ring or not
     ni=ne=0;
     Int_t nRichRingCnt=0;
     Int_t nMdcSegCnt=0;

     while(ni<nHM)
     {
	ne=ni;

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
	    mdcmult[countDiscreteIndexes(m,submax)]++;
	    tofmult[countDiscreteIndexes(t,submax)]++;
	    shomult[countDiscreteIndexes(s,submax)]++;
	    delete m;
	    delete t;
	    delete s; 
	}
	ni=ne+1;// go to next tracklet
     } // end while
     delete six;
     fillHistos();
     return kTRUE;
     // end of execute function
}

Bool_t HRichCorrCounter::finalize() {
    
    dumpCorrelationStatus();
    pFileOut->cd();
    HRichUtilFunc::saveHistos(pFileOut,pHistArray);
    //    pFileOut->Close();
    return kTRUE;
}

void HRichCorrCounter::iniHistos()
{

    
    //Int_t dTl=-20; // unused
    //Int_t dTh= 20; // unused
    //Int_t dTn= 40; // unused

    Int_t opl= 0;
    Int_t oph= 50;
    Int_t opn= 200;

pHistArray = new TObjArray(5);
 pH_MdcTofThetadTheta=new TH2F("pH_MdcTofThetadTheta","pH_MdcTofThetadTheta",
			       80,10,90,40,-20,20);
 pHistArray->Add(pH_MdcTofThetadTheta);

 pH_MdcShowerThetadTheta=new TH2F("pH_MdcShowerThetadTheta","pH_MdcShowerThetadTheta",
				  80,10,90,40,-20,20);
 pHistArray->Add(pH_MdcShowerThetadTheta);

 pH_RichMdcThetadTheta=new TH2F("pH_RichMdcThetadTheta","pH_RichMdcThetadTheta",
				80,10,90,80,-10,10);
 pHistArray->Add(pH_RichMdcThetadTheta);


 pH_RichMdcdTheta=new TH1F("pH_RichMdcdTheta","pH_RichMdcdTheta",200,-10,10);
 pHistArray->Add(pH_RichMdcdTheta);


 pH_MdcTofToftofdTheta=new TH2F("pH_MdcTofToftofdTheta","pH_MdcTofToftofdTheta",
				80,1,40,40,-20,20);
 pHistArray->Add(pH_MdcTofToftofdTheta);

 pH_MdcShowerTofinotofdTheta=new TH2F("pH_MdcShowerTofinotofdTheta","pH_MdcShowerTofinotofdTheta",
				80,1,40,40,-20,20);
 pHistArray->Add(pH_MdcShowerTofinotofdTheta);
 // opangle ----------------------
 pH_opangleMDC = new TH1F("pH_opangleMDC","pH_opangleMDC",opn,opl,oph);
 pHistArray->Add(pH_opangleMDC);

 pH_opangleTOF = new TH1F("pH_opangleTOF","pH_opangleTOF",opn,opl,oph);
 pHistArray->Add(pH_opangleTOF);

 pH_opangleMETA = new TH1F("pH_opangleMETA","pH_opangleMETA",opn,opl,oph);
 pHistArray->Add(pH_opangleMETA);

 pH_opangleMETAdThetaMDCMETA = new TH2F("pH_opangleMETAdThetaMDCMETA","pH_opangleMETAdThetaMDCMETA",opn,opl,oph,200,-2,2);
 pHistArray->Add(pH_opangleMETAdThetaMDCMETA);

//   pH_opangleMETAdPhiMDCMETA = new TH2F("pH_opangleMETAdPhiMDCMETA","pH_opangleMETAdPhiMDCMETA",opn,opl,oph,10,-5,5);
//   pHistArray->Add(pH_opangleMETAdPhiMDCMETA);
 // pH_opangleMETAdPhiMDCMETA = new TH2F("pH_opangleMETAdPhiMDCMETA","pH_opangleMETAdPhiMDCMETA",50,0,1,50,0,1);
 //pHistArray->Add(pH_opangleMETAdPhiMDCMETA);
   pH_opangleMETAdPhiMDCMETA = new TH2F("pH_opangleMETAdPhiMDCMETA","pH_opangleMETAdPhiMDCMETA",50,0,500,50,0,500);
   pHistArray->Add(pH_opangleMETAdPhiMDCMETA);

 //---------------------
 pH_MdcTofdTheta = new TH1F("pH_MdcTofdTheta","pH_MdcTofdTheta",20,-10,10);
 pHistArray->Add(pH_MdcTofdTheta);

 pH_MdcShowerdTheta = new TH1F("pH_MdcShowerdTheta","pH_MdcShowerdTheta",200,-10,10);
 pHistArray->Add(pH_MdcShowerdTheta);

pH_MdcMETAdTheta = new TH1F("pH_MdcMETAdTheta","pH_MdcMETAdTheta",200,-10,10);
 pHistArray->Add(pH_MdcMETAdTheta);

 // ------------- RINGS -------------

 pH_opangleTrksinRing = new TH2F("pH_opangleTrksinRing","pH_opangleTrksinRing",5,0,4,80,0,20);
 pHistArray->Add(pH_opangleTrksinRing);
 pH_RichDblPadNr      = new TH1D("pH_RichDblPadNr","pH_RichDblPadNr",50,0,50);
pHistArray->Add(pH_RichDblPadNr);
 pH_RichDblAmpl       = new TH1D("pH_RichDblAmpl","pH_RichDblAmpl",150,0,1500);
pHistArray->Add(pH_RichDblAmpl);
 pH_RichDblPatMat     = new TH1D("pH_RichDblPatMat","pH_RichDblPatMat",90,100,1000);
pHistArray->Add(pH_RichDblPatMat);
 pH_RichDblHouTra     = new TH1D("pH_RichDblHouTra","pH_RichDblHouTra",60,0,500);
pHistArray->Add(pH_RichDblHouTra);
 pH_RichSnglPadNr     = new TH1D("pH_RichSnglPadNr","pH_RichSnglPadNr",50,0,50);
pHistArray->Add(pH_RichSnglPadNr);
 pH_RichSnglAmpl      = new TH1D("pH_RichSnglAmpl","pH_RichSnglAmpl",150,0,1500);
pHistArray->Add(pH_RichSnglAmpl);
 pH_RichSnglPatMat    = new TH1D("pH_RichSnglPatMat","pH_RichSnglPatMat",90,100,1000);
pHistArray->Add(pH_RichSnglPatMat);
 pH_RichSnglHouTra    = new TH1D("pH_RichSnglHouTra","pH_RichSnglHouTra",60,0,500);
pHistArray->Add(pH_RichSnglHouTra);
}

Int_t HRichCorrCounter::fillHistos()
{
    Int_t nHM = ((HLinearCategory*)getHitMatchCat())->getEntries();
    //cout<<"fill Histos in EXP "<<nHM<<endl;
    if (nHM==0) return 0; // nothing to be done

    pIterMatchHit->Reset();
    HHitMatch* pHM=0;

    Int_t *MdcTof_MdcIndex=new Int_t[nHM];//store indexes used in MDC-TOF combinations
    Int_t *MdcTof_TofIndex=new Int_t[nHM];

    Int_t *RichMdc_MdcIndex=new Int_t[nHM];//store indexes used in MDC-TOF combinations
    Int_t *RichMdc_RichIndex=new Int_t[nHM];

    Int_t *MdcShower_MdcIndex=new Int_t[nHM];//store indexes used in MDC-TOF combinations
    Int_t *MdcShower_ShowerIndex=new Int_t[nHM];


    TObjArray *so = new TObjArray(5);

    for (Int_t i=0;i<nHM;i++) MdcTof_MdcIndex[i]=MdcTof_TofIndex[i]=RichMdc_MdcIndex[i]=RichMdc_RichIndex[i]=MdcShower_MdcIndex[i]=MdcShower_ShowerIndex[i]=-2;


    while(( pHM= (HHitMatch *)pIterMatchHit->Next()))
    {
	
	fillMdcTofThetadTheta(pHM,nHM,
			      MdcTof_MdcIndex,MdcTof_TofIndex);

	fillMdcShowerThetadTheta(pHM,nHM,
			      MdcShower_MdcIndex,MdcShower_ShowerIndex);

	fillRichMdcThetadTheta(pHM,nHM,
			      RichMdc_MdcIndex,RichMdc_RichIndex);

	// select objs for opening angle distribution
	selectObj(so,pHM);
    }

    calcOpeningAngleMDC(so,pH_opangleMDC);
    calcOpeningAngleTOF(so,pH_opangleTOF); 
    calcOpeningAngleMETA(so,pH_opangleMETA,pH_opangleMETAdThetaMDCMETA,pH_opangleMETAdPhiMDCMETA);
    delete so;
    delete [] MdcTof_MdcIndex;
    delete [] MdcTof_TofIndex;
    delete [] RichMdc_MdcIndex;
    delete [] RichMdc_RichIndex;
    delete [] MdcShower_MdcIndex;
    delete [] MdcShower_ShowerIndex;
    
    return 1;
}

Int_t HRichCorrCounter::selectObj(TObjArray* pRTM,HHitMatch* pHM)
{
    Int_t c=0;

    Int_t nRInd = pHM->getRichInd();
    Int_t nMInd = pHM->getMdcInd();
    Int_t nTInd = pHM->getTofInd();
    Int_t nSInd = pHM->getShowInd();

    Int_t nRM = pHM->getMatchedRichMdc();
    Int_t nRT = pHM->getMatchedRichTof();
    Int_t nMT = pHM->getMatchedMdcTof();
    
    Int_t nRS = pHM->getMatchedRichShower();
    Int_t nMS = pHM->getMatchedMdcShower();
    Float_t dThetaRM = TMath::Abs(pHM->getRichTheta()-pHM->getMdcTheta());
    // check if it is in TOF
    
    if (nRInd!=-1 && nTInd!=-1 && nMInd!=-1
	&& nRM==1 &&  nRT==1 &&  nMT==1 )
    {
	
	c++;
	if (dThetaRM<2.) pRTM->Add(pHM);
    }

    // check if it is in SHOWER
    else if (nRInd!=-1 && nSInd!=-1 && nMInd!=-1
	     && nRM==1 &&  nRS==1 &&  nMS==1 )
    {
	
	c++;
	if (dThetaRM<2.) pRTM->Add(pHM);
    }
    return c;
}

void HRichCorrCounter::calcOpeningAngleMDC(TObjArray *pArr,TH1F* hist)
{
    // make all combinations of correlated objs and call
    // calcopeningangles from hrichutilfunc
    Int_t nCnt=0;
    Int_t nMaxArr = pArr->GetLast()+1;
    
    if (nMaxArr==0) return;
    //cout<<" nMaxArr: "<<nMaxArr<<endl;
    Int_t arrsize =  nMaxArr*nMaxArr;
    Int_t *Mdc1=new Int_t[arrsize];
    Int_t *Mdc2=new Int_t[arrsize];
    //cout<<"***************************************************"<<endl;
    for (Int_t i=0;i<arrsize;i++) Mdc1[i]=Mdc2[i]=-2;

    for (Int_t i=0;i<nMaxArr;i++)
    {
	nCnt++;
	Int_t ind1=((HHitMatch*)(*pArr)[i])->getMdcInd();
	if (ind1>-1)
	{
	    for (Int_t j=nCnt;j<nMaxArr;j++)
	    {
		Int_t ind2=((HHitMatch*)(*pArr)[j])->getMdcInd();
		
		//		if (ind2>-1 && checkCombinationSameCat(ind1,ind2,Mdc1,Mdc2,arrsize)==1)
		if (ind2>-1)
		{
		    //    cout<<"calc opangle !!! ind 1: "<<ind1<<" ind2: "<<ind2<<" nMaxArr^2:"<<nMaxArr*nMaxArr<<endl;
		    Float_t t1 = ((HHitMatch*)(*pArr)[i])->getMdcTheta();
		    Float_t p1 = ((HHitMatch*)(*pArr)[i])->getMdcPhi();
		    Float_t t2 = ((HHitMatch*)(*pArr)[j])->getMdcTheta();
		    Float_t p2 = ((HHitMatch*)(*pArr)[j])->getMdcPhi();
		    //cout<<"t1: "<<t1<<" p1: "<<p1<<" t2: "<<t2<<" p2: "<<p2<<endl;

		    Float_t opangle = HRichUtilFunc::calcOpeningAngleT(t1,p1,t2,p2);
		    //Float_t opangle = HRichUtilFunc::openAngleDeg(t1,p1,t2,p2);
		    //if (opangle>0.) hist->Fill(opangle);
		    if (opangle>0.)
		    {
		    // check for unlike sign pair
		    Float_t metat1,metap1,metat2,metap2;
		    metat1=metap1=metat2=metap2=-2.;
		    // first particle
		    if ( ((HHitMatch*)(*pArr)[i])->getShowInd() > -1 )
		    {
			metat1=((HHitMatch*)(*pArr)[i])->getShowerTheta();
			metap1=((HHitMatch*)(*pArr)[i])->getShowerPhi();
		    }
		    else if ( ((HHitMatch*)(*pArr)[i])->getTofInd() > -1 )
		    {
			metat1=((HHitMatch*)(*pArr)[i])->getTofTheta();
			metap1=((HHitMatch*)(*pArr)[i])->getTofPhi();
		    }
		    // second particle
		    if ( ((HHitMatch*)(*pArr)[j])->getShowInd() > -1 )
		    {
			metat2=((HHitMatch*)(*pArr)[j])->getShowerTheta();
			metap2=((HHitMatch*)(*pArr)[j])->getShowerPhi();
		    }
		    else if ( ((HHitMatch*)(*pArr)[j])->getTofInd() > -1 )
		    {
			metat2=((HHitMatch*)(*pArr)[j])->getTofTheta();
			metap2=((HHitMatch*)(*pArr)[j])->getTofPhi();
		    }

		    if ( ((t1-metat1)*(t2-metat2)) < 0. && opangle>0.) // it is an unlike sign pair
		    {
			//cout<<hist->GetName()<<" --- "<<opangle<<" --- "<<ind1<<" "<<ind2<<endl;
			//cout<<"t1: "<<t1<<" p1: "<<p1<<" t2: "<<t2<<" p2: "<<p2<<endl;
			if (checkCombinationSameCat(ind1,ind2,Mdc1,Mdc2,arrsize)==1) hist->Fill(opangle); // opangle with MDC for unlike sign pair.
			// check if rich hit is the same or different
			// if different : open pair
			// if the same  : unresolved pair in rich, resolved by mdc
			Int_t rind1 = ((HHitMatch*)(*pArr)[i])->getRichInd();
			Int_t rind2 = ((HHitMatch*)(*pArr)[j])->getRichInd();
			if (rind1>-1 && rind2>-1 && rind1==rind2) // it is the same ring, a Double_t ring
			{
			    pH_opangleTrksinRing->Fill(2,opangle); //2 tracks per 1 ring with opening angle opangle 
			    if (opangle<3.5)
			    {
				pH_RichDblPadNr->Fill( ((HHitMatch*)(*pArr)[i])->getRingPadNr() );
				pH_RichDblAmpl->Fill( ((HHitMatch*)(*pArr)[i])->getRingAmplitude() );
				pH_RichDblPatMat->Fill( ((HHitMatch*)(*pArr)[i])->getRingPatMat() );
				pH_RichDblHouTra->Fill( ((HHitMatch*)(*pArr)[i])->getRingHouTra() );
			    }
			}
			else if (rind1>-1 && rind2>-1 && rind1!=rind2) // it is not the same ring, most likely two singles
			{
			    pH_opangleTrksinRing->Fill(1,opangle);
			    if (opangle>5.)
			    {
				pH_RichSnglPadNr->Fill( ((HHitMatch*)(*pArr)[i])->getRingPadNr() );
				pH_RichSnglAmpl->Fill( ((HHitMatch*)(*pArr)[i])->getRingAmplitude() );
				pH_RichSnglPatMat->Fill( ((HHitMatch*)(*pArr)[i])->getRingPatMat() );
				pH_RichSnglHouTra->Fill( ((HHitMatch*)(*pArr)[i])->getRingHouTra() );
				pH_RichSnglPadNr->Fill( ((HHitMatch*)(*pArr)[j])->getRingPadNr() );
				pH_RichSnglAmpl->Fill( ((HHitMatch*)(*pArr)[j])->getRingAmplitude() );
				pH_RichSnglPatMat->Fill( ((HHitMatch*)(*pArr)[j])->getRingPatMat() );
				pH_RichSnglHouTra->Fill( ((HHitMatch*)(*pArr)[j])->getRingHouTra() );
			    }
			}
		    }
		    }//opangle>0	
		}
	    }
	}
    }
    delete [] Mdc1;
    delete [] Mdc2;

    
    
}
void HRichCorrCounter::calcOpeningAngleTOF(TObjArray *pArr,TH1F* hist)
{
    // make all combinations of correlated objs and call
    // calcopeningangles from hrichutilfunc
    Int_t nCnt=0;
    Int_t nMaxArr = pArr->GetLast()+1;
    //cout<<" nMaxArr: "<<nMaxArr<<endl;
    Int_t arrsize =  nMaxArr*nMaxArr;
    Int_t *Tof1=new Int_t[arrsize];
    Int_t *Tof2=new Int_t[arrsize];
    //    cout<<"***************************************************"<<endl;
    for (Int_t i=0;i<arrsize;i++) Tof1[i]=Tof2[i]=-2;

    for (Int_t i=0;i<nMaxArr;i++)
    {
	nCnt++;
	Int_t ind1=((HHitMatch*)(*pArr)[i])->getTofInd();
	if (ind1>-1)
	{
	    for (Int_t j=nCnt;j<nMaxArr;j++)
	    {
		Int_t ind2=((HHitMatch*)(*pArr)[j])->getTofInd();
		
		if (ind2>-1 && checkCombinationSameCat(ind1,ind2,Tof1,Tof2,arrsize)==1)
		{
		    //    cout<<"calc opangle !!! ind 1: "<<ind1<<" ind2: "<<ind2<<" nMaxArr^2:"<<nMaxArr*nMaxArr<<endl;
		    Float_t t1 = ((HHitMatch*)(*pArr)[i])->getTofTheta();
		    Float_t p1 = ((HHitMatch*)(*pArr)[i])->getTofPhi();
		    Float_t t2 = ((HHitMatch*)(*pArr)[j])->getTofTheta();
		    Float_t p2 = ((HHitMatch*)(*pArr)[j])->getTofPhi();
		    //cout<<"t1: "<<t1<<" p1: "<<p1<<" t2: "<<t2<<" p2: "<<p2<<endl;

		    Float_t opangle = HRichUtilFunc::calcOpeningAngleT(t1,p1,t2,p2);
		    //Float_t opangle = HRichUtilFunc::openAngleDeg(t1,p1,t2,p2);
		    if (opangle>0.) hist->Fill(opangle);
		}			
	    }
	}
    }

    delete [] Tof1;
    delete [] Tof2;

}

void HRichCorrCounter::calcOpeningAngleMETA(TObjArray *pArr,TH1F* hist,TH2F* hist2, TH2F* hist3)
{
    // find all hits in TOF and SHOWER from the selected tracklets
    // store the angles in META of all different hits
    // calc the opening angle for all possible combinations

    Int_t nCnt=0;
    Int_t nMaxArr = pArr->GetLast()+1;
    Int_t nMaxArr2 = nMaxArr*nMaxArr;

    Int_t *TofU = new Int_t[nMaxArr];
    Int_t *ShowerU = new Int_t[nMaxArr];
    Float_t *Theta = new Float_t[nMaxArr2];
    Float_t *Phi   = new Float_t[nMaxArr2];


    Float_t *DMdcMetaT = new Float_t[nMaxArr2];
    Float_t *DMdcMetaP = new Float_t[nMaxArr2];
    Float_t *KickMom   = new Float_t[nMaxArr2];
    for (Int_t i=0;i<nMaxArr;i++) TofU[i]=ShowerU[i]=-2;
    for (Int_t i=0;i<nMaxArr2;i++) Theta[i]=Phi[i]=KickMom[i]=-2.;


    for (Int_t i=0;i<nMaxArr;i++)
    {
	Int_t indt=((HHitMatch*)(*pArr)[i])->getTofInd();
	Int_t inds=((HHitMatch*)(*pArr)[i])->getShowInd();
	if (indt>-1)//TOF hit
	{
	    Int_t tof_ret=fillUniqueIndex(indt,nMaxArr,TofU);
	    tof_ret=-2;
	    if (tof_ret==-1) Error("HRichCorrCounter::calcOpeningAngleMETA",
				   "no unique tof index");
	    else if (tof_ret==-2) //not yet found tof 
	    {
		//HKickTrack* ktrack = getKickTrack(((HHitMatch*)(*pArr)[i]));
		//HKickTrack* ktrack = getKickTrack(((HHitMatch*)(*pArr)[i])->getMdcTheta(),((HHitMatch*)(*pArr)[i])->getMdcPhi(),((HHitMatch*)(*pArr)[i])->getSector());
		//if (ktrack)
		//{ 
		    //remember the angles
		    Int_t nb_ind=countDiscreteIndexesF(Theta,nMaxArr2);
		    Theta[nb_ind]=((HHitMatch*)(*pArr)[i])->getTofTheta();
		    Phi[nb_ind]  =((HHitMatch*)(*pArr)[i])->getTofPhi();
		    DMdcMetaT[nb_ind] = ((HHitMatch*)(*pArr)[i])->getMdcTheta() -
			((HHitMatch*)(*pArr)[i])->getTofTheta();
		    DMdcMetaP[nb_ind] = ((HHitMatch*)(*pArr)[i])->getMdcPhi() -
			((HHitMatch*)(*pArr)[i])->getTofPhi();
		    //KickMom[nb_ind] = ktrack->getP();
		    //}
	    }
	    else if (tof_ret>-1) {//the hit was stored before, do nothing
	    }
	}

	else if (inds>-1)//SHOWER hit
	{
	    Int_t shower_ret=fillUniqueIndex(inds,nMaxArr,ShowerU);
	    shower_ret=-2;
	    if (shower_ret==-1) Error("HRichCorrCounter::calcOpeningAngleMETA",
				   "no unique shower index");
	    else if (shower_ret==-2) //not yet found shower
	    {
		//HKickTrack* ktrack = getKickTrack(((HHitMatch*)(*pArr)[i]));
		//HKickTrack* ktrack = getKickTrack(((HHitMatch*)(*pArr)[i])->getMdcTheta(),((HHitMatch*)(*pArr)[i])->getMdcPhi(),((HHitMatch*)(*pArr)[i])->getSector());
		//if (ktrack)
		//{
		    //remember the angles
		    Int_t nb_ind=countDiscreteIndexesF(Theta,nMaxArr2);
		    Theta[nb_ind]=((HHitMatch*)(*pArr)[i])->getShowerTheta();
		    Phi[nb_ind]  =((HHitMatch*)(*pArr)[i])->getShowerPhi();
		    DMdcMetaT[nb_ind] = ((HHitMatch*)(*pArr)[i])->getMdcTheta() -
			((HHitMatch*)(*pArr)[i])->getShowerTheta();
		    DMdcMetaP[nb_ind] = ((HHitMatch*)(*pArr)[i])->getMdcPhi() -
			((HHitMatch*)(*pArr)[i])->getShowerPhi();
		    //KickMom[nb_ind] = ktrack->getP();
		    //}
	    }
	    else if (shower_ret>-1) {//the hit was stored before, do nothing
	    }
	}
	else Error("calcOpeningAngleMETA","it's neither a TOF nor Shower hit");
	
    }
    //cout<<"*********************"<<endl;
    // now calc opening angles from all combinations
    for (Int_t i=0;i<nMaxArr2;i++)
    {
	
	if (Theta[i]<0.) break;
	//cout<<"("<<Theta[i]<<","<<Phi[i]<<") ";
	nCnt++;

	    for (Int_t j=nCnt;j<nMaxArr2;j++)
	    {
		if (Theta[j]<0.) break;
		//cout<<"t1: "<
		//	cout<<"("<<Theta[i]<<","<<DMdcMetaT[i]<<","<<KickMom[i]<<") c "<<"("<<Theta[j]<<","<<DMdcMetaT[j]<<","<<KickMom[j]<<") -- ";
		Float_t opangle = HRichUtilFunc::calcOpeningAngleT(Theta[i],Phi[i],Theta[j],Phi[j]);
		//Float_t opangle = HRichUtilFunc::openAngleDeg(t1,p1,t2,p2);

		// all pairs
		//if (opangle>0.) hist->Fill(opangle);

		// unlike sign pairs
		if (opangle>0. && DMdcMetaT[i]*DMdcMetaT[j]<0.) hist->Fill(opangle);

		// all pairs
		//if (opangle>0.) hist2->Fill(opangle,DMdcMetaT[i]);
		//if (opangle>0.) hist2->Fill(opangle,DMdcMetaT[j]);

		// unlike sign pairs
		if (opangle>0. && DMdcMetaT[i]*DMdcMetaT[j]<0.) hist2->Fill(opangle,1./DMdcMetaT[i] );
		if (opangle>0. && DMdcMetaT[i]*DMdcMetaT[j]<0.) hist2->Fill(opangle,1./DMdcMetaT[j] );
		// dT:dT for unlike sign
		//if (opangle>0. && DMdcMetaT[i]*DMdcMetaT[j]<0.) hist3->Fill(DMdcMetaT[j],DMdcMetaT[i]);
		//if (opangle>0. && DMdcMetaT[i]*DMdcMetaT[j]<0.) hist3->Fill(1./(TMath::Abs(DMdcMetaT[j])),1./(TMath::Abs(DMdcMetaT[i])));
		// if (opangle>0. && DMdcMetaT[i]*DMdcMetaT[j]<0.) hist3->Fill(opangle,DMdcMetaP[j]);
		if (opangle>0. && DMdcMetaT[i]*DMdcMetaT[j]<0.) 
		{
		    //if (DMdcMetaT[i]<0. && DMdcMetaT[j]>0.) hist3->Fill(KickMom[i],KickMom[j]);// e- corresponds neg deflection
		    //else if (DMdcMetaT[i]>0. && DMdcMetaT[j]<0.) hist3->Fill(KickMom[j],KickMom[i]);
		}
	    }
    }
    //cout<<endl;
    delete [] TofU;
    delete [] ShowerU;
    delete [] Theta;
    delete [] Phi;
    delete [] DMdcMetaT;
    delete [] DMdcMetaP;
    delete [] KickMom;
}

Int_t HRichCorrCounter::checkCombination(Int_t i,Int_t j,Int_t *iarr,Int_t *jarr,Int_t evtsize)
{
    // this function returns 0 if the index pair i,j was used before in this evt

    Int_t i_ret=fillUniqueIndex(i,evtsize,iarr);

    if (i_ret==-1) 
    { 
	cout<<"i:"<<i<<" j:"<<j<<" maxArr:"<<evtsize<<endl;
	Error("HRichCorrCounter::checkCombination",
	      "no unique i index");
	return -1;
    }
    else if (i_ret==-2)//new slot
    {

	Int_t i_ind=countDiscreteIndexes(iarr,evtsize);

	if (i_ind>evtsize) Error("HRichCorrCounter::checkCombination",
			     "too many stored indexes");
	
	jarr[i_ind-1]=j;//remember the sec index 

    }
    else //index i used before, check whether j was also used before
    {
	
	if (jarr[i_ret]==j) return 0;//combination was used before 

	Int_t i_ind=countDiscreteIndexes(iarr,evtsize);
	iarr[i_ind]=i;// new index comb with same i and new j
	jarr[i_ind]=j;// store these indexes in new pair
    }
    return 1;//it is a new combination 
}

Int_t HRichCorrCounter::checkCombinationSameCat(Int_t i,Int_t j,Int_t *iarr,Int_t *jarr,Int_t evtsize)
{
    
    Int_t maxUsed = countDiscreteIndexes(iarr,evtsize);//symm for both arrays
//      cout<<"in checkCombiSameCat: "<<maxUsed<<endl;
//      for (Int_t n=0;n<maxUsed+1;n++) cout<<iarr[n]<<" ";
//      cout<<endl;
//      for (Int_t n=0;n<maxUsed+1;n++) cout<<jarr[n]<<" ";
//      cout<<endl;
    for(Int_t dim=0;dim<2;dim++)
    {
	//	cout<<"checking tuple: ("<<i<<","<<j<<")"<<endl;
	for (Int_t k=0;k<maxUsed;k++)
	{
	    if (iarr[k]==i && jarr[k]==j) return 0;
	    
	}
	Int_t tmp=i;
	i=j;
	j=tmp;
    }
    // it is a new combi, store it !
    
    if (maxUsed<evtsize) iarr[maxUsed]=i;
    if (maxUsed<evtsize) jarr[maxUsed]=j;
    else Error("checkCombinationSameCat","index out of array bounds");
    return 1;
}
   
Int_t HRichCorrCounter::isStored(Int_t i, Int_t *iarr, Int_t max)
{
    Int_t c=-1;
    for (Int_t k=0;k<max;k++)
    {
	if (iarr[k] == i) return k;
    }
    return c;
}

Int_t HRichCorrCounter::isStoredWithOffset(Int_t i, Int_t *iarr, Int_t max, Int_t offset)
{
    Int_t c=-1;
    for (Int_t k=offset+1;k<max;k++)
    {
	if (iarr[k] == i) return k;
    }
    return c;
}

Int_t HRichCorrCounter::fillRichMdcThetadTheta(HHitMatch* h,Int_t evtsize,Int_t* m,Int_t* r)
{
    Int_t mi,ri;
    mi=ri=-1;
    //check if it is a valid RICH-MDC matched tracklet
    if (h->getMdcInd() > -1 && h->getRichInd() > -1 &&
	h->getMatchedRichMdc() == 1 )
    {
	mi = h->getMdcInd();
	ri = h->getRichInd();
    }
    else return 0;

    // check whether this bi-corr was used before to fill histo 
    if (checkCombination(mi,ri,m,r,evtsize)==0) return 0;

    Float_t mt,rt;
    mt=rt=0.;
    mt=h->getMdcTheta();
    rt=h->getRichTheta();

    Float_t dTheta=mt-rt;//MDC-RICH theta

    pH_RichMdcThetadTheta->Fill(mt,dTheta);
    pH_RichMdcdTheta->Fill(dTheta);
    return 1;
}

Int_t HRichCorrCounter::fillMdcShowerThetadTheta(HHitMatch* h,Int_t evtsize,Int_t* m,Int_t* s)
{
    Int_t mi,si;
    mi=si=-1;
    //check if it is a valid MDC-TOF matched tracklet
    if (h->getMdcInd()  > -1 && h->getShowInd()  > -1 && 
	h->getMatchedMdcShower() == 1 && h->getMatchedRichMdc() == 1 && h->getMatchedRichShower() == 1)
    {
	mi = h->getMdcInd();
	si = h->getShowInd();
    }
    else return 0;

    // check whether this bi-corr was used before to fill histo 
    if (checkCombination(mi,si,m,s,evtsize)==0) return 0;
    
    
    // calculate values and fill them in histo
    Float_t mt,st,rt;
    mt=st=rt=0.;
    mt=h->getMdcTheta();
    st=h->getShowerTheta();
    rt=h->getRichTheta();
    Float_t tof=h->getTofinoTof();
    Float_t dTheta=mt-st;//MDC-TOF theta
    Float_t dThetaRM=TMath::Abs(mt-rt);//RICH-MDC theta
    //cout<<dTheta<<" ////// "<<mi<<"::"<<ti<<" ////// "<<nCounterProcessedNbEvents<<"*********"<<endl;
    if (dThetaRM>2.) return 0;
    pH_MdcMETAdTheta->Fill(dTheta);
    pH_MdcShowerdTheta->Fill(dTheta);
    pH_MdcShowerThetadTheta->Fill(mt,dTheta);
    pH_MdcShowerTofinotofdTheta->Fill(tof,dTheta);
    return 1;
}
Int_t HRichCorrCounter::fillMdcTofThetadTheta(HHitMatch* h,Int_t evtsize,Int_t* m,Int_t* t)
{
    Int_t mi,ti;
    mi=ti=-1;
    //check if it is a valid MDC-TOF matched tracklet
    if (h->getMdcInd()  > -1 && h->getTofInd()  > -1 && 
	h->getMatchedMdcTof() == 1 && h->getMatchedRichMdc() == 1 && h->getMatchedRichTof() == 1)
    {
	mi = h->getMdcInd();
	ti = h->getTofInd();
    }
    else return 0;

    // check whether this bi-corr was used before to fill histo 
    if (checkCombination(mi,ti,m,t,evtsize)==0) return 0;
    
    
    // calculate values and fill them in histo
    Float_t mt,tt,rt;
    mt=tt=rt=0.;
    mt=h->getMdcTheta();
    tt=h->getTofTheta();
    rt=h->getRichTheta();
    Float_t tof=h->getTofTof();
    Float_t dTheta=mt-tt;//MDC-TOF theta
    Float_t dThetaRM=TMath::Abs(mt-rt);//RICH-MDC theta
    //cout<<dTheta<<" ////// "<<mi<<"::"<<ti<<" ////// "<<nCounterProcessedNbEvents<<"*********"<<endl;
    if (dThetaRM>2.) return 0;
    pH_MdcMETAdTheta->Fill(dTheta);
    pH_MdcTofdTheta->Fill(dTheta);
    pH_MdcTofThetadTheta->Fill(mt,dTheta);
    pH_MdcTofToftofdTheta->Fill(tof,dTheta);
    return 1;
}

Int_t HRichCorrCounter::fillUniqueIndex(Int_t i,Int_t max,Int_t* iarr)
{
    // check if value i is already stored in iarr
    // if not then store it
    // iarr is initialized to -2
    // if already stored returns index
    // if newly stored returns index of new slot
    Int_t n=0;
    do{
	if(i==iarr[n]) return n;//i already stored
	else if(iarr[n]==-2) //new slot
	{
	    iarr[n]=i;//store value i in new slot
	    return -2;
	}
	n++;
    } while(n<max);
    return -1;
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
Int_t HRichCorrCounter::countDiscreteIndexesF(Float_t *iarr,Int_t max)
{
    //count arr slots that are different from -2 (ini value)
    Int_t n=0;
    for(Int_t i=0;i<max;i++)
    {
	if( iarr[i] > -2. ) n++;
	else break;
    }
    return n;
}
void HRichCorrCounter::dumpCorrelationStatus()
{
    cout<<"*******************************************"<<endl;
    cout<<"*** INFORMATION ***************************"<<endl;
    cout<<"*******************************************"<<endl;
    cout<<"Nb of evts processed   : "<<nCounterProcessedNbEvents<<endl;
    cout<<"Nb of tracklets found  : "<<ngNbCorrObjs<<endl;
    cout<<"Average tracklet mult  : "<<((Float_t)ngNbCorrObjs)/
	((Float_t)nCounterProcessedNbEvents)<<endl;
    cout<<"*** Nb of diff hits indep of tracklets in remaining evts ***"<<endl;
    if(ngRichCnt>0)     cout<<"Tot Nb of  RICH hits   : "<<ngRichCnt<<endl;
    if(ngMdcSegCnt>0)   cout<<"Tot Nb of  MDCSEG hits : "<<ngMdcSegCnt<<endl;
    if(ngTofCnt>0)      cout<<"Tot Nb of  TOF hits    : "<<ngTofCnt<<endl;
    if(ngShowerCnt>0)   cout<<"Tot Nb of  SHOWER hits : "<<ngShowerCnt<<endl;
    cout<<"*** Nb of different hits in tracklets in remaining evts ***"<<endl;
    if(ngDRind>0)           cout<<"Nb of diff Rich   hits : "<<ngDRind<<endl;
    if(ngDMind>0)           cout<<"Nb of diff Mdc    hits : "<<ngDMind<<endl;
    if(ngDTind>0)           cout<<"Nb of diff Tof    hits : "<<ngDTind<<endl;
    if(ngDSind>0)           cout<<"Nb of diff Shower hits : "<<ngDSind<<endl;
    cout<<"*******************************************"<<endl;

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
    cout<<"************** CORRELATIONS **************"<<endl;
    if(ngNb_RM>0)       cout<<"Nb of exact RM corr : "<<ngNb_RM<<endl;
    if(ngNb_RT>0)       cout<<"Nb of exact RT corr : "<<ngNb_RT<<endl;
    if(ngNb_RS>0)       cout<<"Nb of exact RS corr : "<<ngNb_RS<<endl;
    if(ngNb_MT>0)       cout<<"Nb of exact MT corr : "<<ngNb_MT<<endl;
    if(ngNb_MS>0)       cout<<"Nb of exact MS corr : "<<ngNb_MS<<endl;
    cout<<"*******************************************"<<endl;
    if(ngRTM>0)         cout<<"Nb of min RMT indexes in tracklet : "
			    <<ngRTM<<endl;
    if(ngRSM>0)         cout<<"Nb of min RMS indexes in tracklet : "
			    <<ngRSM<<endl;
    cout<<"*******************************************"<<endl;
    if(ngNb_RcMcT>0)    cout<<"Nb of exact RM MT    corr : "<<ngNb_RcMcT<<endl;
    if(ngNb_RcMcTcR>0)  cout<<"Nb of exact RM MT RT corr : "<<ngNb_RcMcTcR<<endl;
    if(ngNb_RMcTcR>0)   cout<<"Nb of exact    MT RT corr : "<<ngNb_RMcTcR<<endl;
    if(ngNb_RcMTcR>0)   cout<<"Nb of exact RM    RT corr : "<<ngNb_RcMTcR<<endl;
    if(ngNb_RcMcS>0)    cout<<"Nb of exact RM MS    corr : "<<ngNb_RcMcS<<endl;
    if(ngNb_RcMcScR>0)  cout<<"Nb of exact RM MS RS corr : "<<ngNb_RcMcScR<<endl;
    if(ngNb_RMcScR>0)   cout<<"Nb of exact    MS RS corr : "<<ngNb_RMcScR<<endl;
    if(ngNb_RcMScR>0)   cout<<"Nb of exact RM    RS corr : "<<ngNb_RcMScR<<endl;
    cout<<"*******************************************"<<endl;
    for (Int_t k=0;k<10;k++)
    {
	if (k!=0 && mdcmult[k]!=0) cout<<mdcmult[k]<<" ring(s) have "<<k
				       <<" correlated MDC 1 segments"<<endl;
	if (k!=0 && tofmult[k]!=0) cout<<tofmult[k]<<" ring(s) have "<<k
				       <<" correlated TOF hits"<<endl;
	if (k!=0 && shomult[k]!=0) cout<<shomult[k]<<" ring(s) have "<<k
				       <<" correlated SHOWER hits"<<endl;
	cout<<"*******************************************"<<endl;
    }
    
}


HKickTrack* HRichCorrCounter::getKickTrack(HHitMatch* h)
{
    //identify the track given by HKickTrack
    //which corresponds to my tracklet
    //check on theta, phi and tof(ino) ?

    Float_t mt=-2.;
    Float_t mp=-2.;
    Int_t sec = h->getSector();
    if (h->getMdcInd() > -1)
    {
	mt = h->getMdcTheta();
	mp = h->getMdcPhi();
    }
    else return 0;
    Float_t tof=-2.;

    if (h->getTofInd() > -1) tof=h->getTofTof();
    else if (h->getShowInd() > -1) tof=h->getTofinoTof();

    Float_t fMinThetaDiff=0.001;
    Float_t fMinPhiDiff=0.001;
    Float_t tofdiff=0.001;
    Float_t r2d = 57.29578;

    HKickTrack *track = 0;
    iterTracks->Reset();

    while ((track=(HKickTrack*) iterTracks->Next())!=0) 
    {
	
	Float_t trackTheta = track->getTheta()*r2d;
	Float_t trackPhi = track->getPhi()*r2d + (track->getSector()*60.);
	if(trackPhi > 360.) trackPhi-= 360.;
	
	if (sec==track->getSector())
	{
	    if( TMath::Abs(tof-track->getTof()) < tofdiff && TMath::Abs(trackTheta-mt) < fMinThetaDiff && TMath::Abs(trackPhi-mp) < fMinPhiDiff )
	    {
		//cout<<trackTheta<<"/"<<mt<<"  "<<trackPhi<<"/"<<mp<<"  "<<track->getP()<<"/"<<track->getTof()<<"/"<<tof<<endl;
		return track;
	    }
        }
    }   

    return 0;
}




