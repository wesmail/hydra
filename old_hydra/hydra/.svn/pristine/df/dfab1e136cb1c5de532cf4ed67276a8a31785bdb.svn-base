//*-- Author : Jochen Markert

// ***************************************************************************
//_HADES_CLASS_DESCRIPTION
//////////////////////////////////////////////////////////////////////////////
// HPidMdcStart
//
// This tasks reconstructs the starttime for experiments without start detector.
// All TOF Clusters (or Hits if Clusters are absent and HShowerHitTof measurements
// are collected. The earliest TOF is selected and a correction value calulated
// which shifts this value to a given TimeValue (~7-8ns). The correction factor
// is written to HStartHit object, thus it can be retrieved by other tasks to be
// used to remove the jitter from the trigger. If a StartHit already exists a error
// message will emmited and the existing value will not be overwritten.
// the task can be configured in the following way:
//
// void   setPrint(Bool_t doprint) : print the selected and sorted TOF values and the
//                                   correction value
// void   setTimeVal(Float_t val)  : set the reforence TimeValue (default value is 7.5)
//
//
//////////////////////////////////////////////////////////////////////////////
// ***************************************************************************
  
#include "hpidmdcstart.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hcategory.h"
#include "hmatrixcategory.h"
#include "hlinearcategory.h"
#include "hlocation.h"
#include "hiterator.h"
#include "htool.h"
#include "hstarthit.h"
#include "hshowerhittof.h"
#include "htofcluster.h"
#include "htofhit.h"
#include "hstartdef.h"
#include "showertofinodef.h"
#include "tofdef.h"
#include "hgeantdef.h"
#include "TMath.h"
#include "TArrayF.h"

#include <iostream>
using namespace std;

ClassImp(HPidMdcStart)

    HPidMdcStart::HPidMdcStart()
{
    // Default constructor
    initParameters();
}

HPidMdcStart::HPidMdcStart(Text_t *name,Text_t *title)
:HReconstructor(name,title)
{
    // Constructor
    initParameters();

}

HPidMdcStart::~HPidMdcStart(void){
    // Destructor.
    if(iterShowerHitTofCat) delete iterShowerHitTofCat;
    if(iterTofClusterCat)   delete iterTofClusterCat;
    if(iterTofHitCat)       delete iterTofHitCat;
    iterShowerHitTofCat = NULL;
    iterTofClusterCat   = NULL;
    iterTofHitCat       = NULL;
}

void HPidMdcStart::initParameters(){
    isEmbedding         = kFALSE;
    iterShowerHitTofCat = NULL;
    iterTofClusterCat   = NULL;
    iterTofHitCat       = NULL;
    timeVal             = 7.5;
    print               = kFALSE;
}
Bool_t HPidMdcStart::init(void){

    HEvent* event = gHades->getCurrentEvent();

    if(!event){
	Error("init()","Could not retrieve current Event!");
        exit(1);
    }


    if(event->getCategory(catGeantKine) &&
       gHades->getEmbeddingMode() > 0)
    {
	isEmbedding = kTRUE;
    } else {
	isEmbedding = kFALSE;
    }

    pShowerHitTofCat = NULL;
    pTofClusterCat   = NULL;
    pTofHitCat       = NULL;

    if(isEmbedding){
	pShowerHitTofCat = (HCategory*)(event->getCategory(catShowerHitTofTrack));
	if(!pShowerHitTofCat) {
	    Warning("init","No HShowerHitTof Input -> Switching usage of this category OFF");
	}
    } else {
     	pShowerHitTofCat = (HCategory*)(event->getCategory(catShowerHitTof));
	if(!pShowerHitTofCat) {
	    Warning("init","No HShowerHitTof Input -> Switching usage of this category OFF");
	}
    }

    if(pShowerHitTofCat) {
	iterShowerHitTofCat = (HIterator*)pShowerHitTofCat->MakeIterator();
    }

    pTofClusterCat = (HMatrixCategory*)(event->getCategory(catTofCluster));
    if(!pTofClusterCat) {
	Warning("init","No HTofCluster Input -> Switching usage of this category OFF");
    } else {
	iterTofClusterCat = (HIterator*)pTofClusterCat->MakeIterator();
    }

    pTofHitCat = (HMatrixCategory*)(event->getCategory(catTofHit));
    if(!pTofHitCat) {
	Warning("init","No HTofHit Input -> Switching usage of this category OFF");
    } else {
	iterTofHitCat = (HIterator*)pTofHitCat->MakeIterator();
    }


    pStartHitCat = (HLinearCategory *)(event->getCategory(catStartHit));
    if (!pStartHitCat)
    {
	pStartHitCat = new HLinearCategory("HStartHit");
	if (!pStartHitCat) {
	    Error("init()","Could not create catStartHit!");
	    return kFALSE;
	}
	else {
	    event->addCategory(catStartHit,pStartHitCat,"Start");
	}
    }
    
    return kTRUE;
}

Int_t HPidMdcStart::execute(void){
    // Execute function

    //----------------------------------------------------
    // create and init working arrays and counters
    const Int_t size    = 100;
    const Int_t sizeSum = 200;

    TArrayF aTof(size);
    TArrayF aShowerTof(size);
    TArrayF aSumTof(sizeSum);

    aTof      .Reset(1000.);
    aShowerTof.Reset(1000.);
    aSumTof   .Reset(1000.);

    Int_t ctTof       = 0;
    Int_t ctShowerTof = 0;
    Int_t ctSumTof    = 0;
    //----------------------------------------------------

    //----------------------------------------------------
    // TOF part

    //----------------------------------------------------
    // if TofClusters exists take info from there
    if(iterTofClusterCat){

	iterTofClusterCat->Reset();
	HTofCluster* pTofCluster;

	while((pTofCluster = (HTofCluster*)iterTofClusterCat->Next()) != 0 ) {

	    if(ctTof < size){
		aTof   [ctTof]    = pTofCluster->getTof();
		ctTof    ++;
	    } else {
		Warning("execute()","TofCuster skipped!");
	    }
            if(ctSumTof < sizeSum){
		aSumTof[ctSumTof] = pTofCluster->getTof();
		ctSumTof ++;
	    } else {
		Warning("execute()","TofCuster skipped!");
	    }

	}
    //----------------------------------------------------
    } else {
	//----------------------------------------------------
        // No TofClusters available try to get info from TofHit
	if(iterTofHitCat){

	    iterTofHitCat->Reset();
	    HTofHit* pTofHit;

	    while((pTofHit = (HTofHit*)iterTofHitCat->Next()) != 0 ) {

		if(ctTof < size){
		    aTof   [ctTof]    = pTofHit->getTof();
		    ctTof    ++;
		} else {
		    Warning("execute()","TofCuster skipped!");
		}
		if(ctSumTof < sizeSum){
		    aSumTof[ctSumTof] = pTofHit->getTof();
		    ctSumTof ++;
		} else {
		    Warning("execute()","TofCuster skipped!");
		}
	    }
	}
	//----------------------------------------------------
    }
    //----------------------------------------------------

    //----------------------------------------------------
    // TOFino part
    if(iterShowerHitTofCat){

	iterShowerHitTofCat->Reset();
	HShowerHitTof* pShowerTof;

	while((pShowerTof = (HShowerHitTof*)iterShowerHitTofCat->Next()) != 0 ) {

	    if(ctShowerTof < size){
		aShowerTof   [ctShowerTof]    = pShowerTof->getTof();
		ctShowerTof    ++;
	    } else {
		Warning("execute()","HShowerHitTof skipped!");
	    }
	    if(ctSumTof < sizeSum){
		aSumTof[ctSumTof] = pShowerTof->getTof();
		ctSumTof ++;
	    } else {
		Warning("execute()","HShowerHitTof skipped!");
	    }
	}
    }
    //----------------------------------------------------

    //----------------------------------------------------
    // Sort the working arrays by TOF and find the shortest
    // TOF. Calculate a offset to shift these value to selected timeVal (~7.5ns).
    // This value will written to StartHit cat to alow MDC
    // to have a good start value.
    HTool::sort(size   ,aTof      .GetArray(),0, kFALSE,kTRUE); // up , overwrite
    HTool::sort(size   ,aShowerTof.GetArray(),0, kFALSE,kTRUE); // up , overwrite
    HTool::sort(sizeSum,aSumTof   .GetArray(),0, kFALSE,kTRUE); // up , overwrite
    Float_t corr = 0;
    if(ctSumTof > 0){
	corr = timeVal - aSumTof[0];
    }
    if(print){
	cout<<"-----------"<<endl;
	cout<<"Event : "<<gHades->getEventCounter()<<endl;
	cout<<"Mult TOF: "<<ctTof<<" Mult Shower: "<<ctShowerTof<<" Mult SUM "<<ctSumTof<<endl;
	cout<<"TOF    winner :" <<aTof      [0]<<endl;
	cout<<"Shower winner :" <<aShowerTof[0]<<endl;
	cout<<"Sum    winner :" <<aSumTof   [0]<<endl;
	cout<<"Corr          :" <<corr<<endl;
	for(Int_t i = 0; i < ctSumTof; i ++){
	    cout<<i<< " " << aSumTof[i] <<endl;
	}
    }
    //----------------------------------------------------


    //-------------------------------------------------------------------------
    // fill the recalibrated start time
    HLocation loc;
    loc.set(2,0,0);

    HStartHit* pHit = (HStartHit*)pStartHitCat->getObject(loc);
    if(pHit){
	Warning("execute()","HStartHit object exists already! will skip and not overwrite!");
        return 0;
    }

    if(!pHit){
	pHit = (HStartHit*)pStartHitCat->getNewSlot(loc);
    }

    if(pHit){
	pHit = new (pHit) HStartHit;
	pHit->setModule(0);
	pHit->setStrip(0);
	pHit->setTime(-corr);
	pHit->setStartMult(1);
	pHit->setFlag(kTRUE);
    } else {
	Error("execute()","NULL pointer for HStartHit slot retrieved!");
    }
    //-------------------------------------------------------------------------

    return 0;
}


Bool_t HPidMdcStart::finalize(void){

    return kTRUE;
}
