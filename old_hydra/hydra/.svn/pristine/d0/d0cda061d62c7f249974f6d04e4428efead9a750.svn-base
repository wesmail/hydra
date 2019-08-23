// $Id: hrichpaircreator.cc,v 1.4 2002-09-25 16:19:53 eberl Exp $
// Last update by Thomas Eberl: 02/09/25 17:42:42
//
#include "hrichpaircreator.h"
#include "hdihitmatch.h"
#include "hrichcut.h"
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
#include "hrichhistfac.h"
#include <TH1.h>
#include <TFile.h>
#include <TObjArray.h>
ClassImp(HRichPairCreator)

HRichPairCreator::HRichPairCreator(Text_t *name,Text_t *title, char* filename) :
  HReconstructor(name,title)
{
    pFileName  = filename;// output filename for diagnostic histos
}

HRichPairCreator::HRichPairCreator()
{

}


HRichPairCreator::~HRichPairCreator(void) {

}


Bool_t HRichPairCreator::init() {
        if (gHades) {
	    HEvent *event=gHades->getCurrentEvent();
	    HRuntimeDb *rtdb=gHades->getRuntimeDb();
	    HSpectrometer *spec=gHades->getSetup();
	    if (event && rtdb) {
		HDetector *rich = spec->getDetector("Rich");
		if (rich) { 
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

		// Setup output category
		pHitDiMatchCat=event->getCategory(catDiMatchHit);
		if (!pHitDiMatchCat) {
		    pHitDiMatchCat=rich->buildCategory(catDiMatchHit);
		    
		    if (!pHitDiMatchCat) {
			Error("init","No HIT DIMATCH category defined");
			return kFALSE;
		    }
		    else event->addCategory(catDiMatchHit, pHitDiMatchCat, "Rich");
		}
		
		pIterDiMatchHit = (HIterator*)getHitDiMatchCat()->MakeIterator("native");

		}
	    }
	}
	

	pHistArray = new TObjArray(10);
	pFileOut = new TFile(pFileName,"RECREATE");
	initHistos();
	initCuts();
	nCounterProcessedNbEvents=0; //total nb of evts seen by this task
	return kTRUE;
}
void HRichPairCreator::initCuts()
{
    nRichPolarLow=20.;
    nRichPolarHigh=30.;
}
void HRichPairCreator::initHistos()
{
    // get "standard" histograms from Rich Histogram Factory
    TH1F* hist=HRichHistFac::getHOpeningAngle("MDC");
    pHistArray->Add(hist);
    // -----------------------------------------
    hist=HRichHistFac::getHOpeningAngle("KICK");
    pHistArray->Add(hist);
    // -----------------------------------------
    hist=HRichHistFac::getHOpeningAngle("META");
    pHistArray->Add(hist);
    // -----------------------------------------
    hist=HRichHistFac::getHInvMass("All");
    pHistArray->Add(hist);
    // -----------------------------------------
    hist=HRichHistFac::getHInvMass("Like_sign");
    pHistArray->Add(hist);
    // -----------------------------------------
    hist=HRichHistFac::getHInvMass("Unlike_sign");
    pHistArray->Add(hist);
    // -----------------------------------------
    hist=HRichHistFac::getHTof("AllPairs");
    pHistArray->Add(hist);
    // -----------------------------------------

    TH2F* hist2=HRichHistFac::getHMomvsMom("All","PtotvsPtot");
    pHistArray->Add(hist2);
    // -----------------------------------------
    hist2=HRichHistFac::getHMomvsMom("Unlike_sign","PtotvsPtot");
    pHistArray->Add(hist2);
    // -----------------------------------------
    hist2=HRichHistFac::getHMomvsMom("Like_sign","PtotvsPtot");
    pHistArray->Add(hist2);


    // -----------------------------------------
    hist2=HRichHistFac::getHMomvsMom("All","PtvsPtot");
    pHistArray->Add(hist2);
    // -----------------------------------------
    hist2=HRichHistFac::getHMomvsMom("Unlike_sign","PtvsPtot");
    pHistArray->Add(hist2);
    // -----------------------------------------
    hist2=HRichHistFac::getHMomvsMom("Like_sign","PtvsPtot");
    pHistArray->Add(hist2);

    // -----------------------------------------
    hist2=HRichHistFac::getHMomvsMom("All","PtvsPt");
    pHistArray->Add(hist2);
    // -----------------------------------------
    hist2=HRichHistFac::getHMomvsMom("Unlike_sign","PtvsPt");
    pHistArray->Add(hist2);
    // -----------------------------------------
    hist2=HRichHistFac::getHMomvsMom("Like_sign","PtvsPt");
    pHistArray->Add(hist2);
    // -----------------------------------------

    // Ring histos
    hist=HRichHistFac::getHRingAmplitude("1RingPerPair");
    pHistArray->Add(hist);
    hist=HRichHistFac::getHRingAmplitude("2RingsPerPair");
    pHistArray->Add(hist);
    hist2=HRichHistFac::getHRingAmplitudeTheta("1RingPerPair");
    pHistArray->Add(hist2);
    hist2=HRichHistFac::getHRingAmplitudeTheta("2RingsPerPair");
    pHistArray->Add(hist2);
    hist2=HRichHistFac::getHRingAmplitudeTheta("1RingPerPair.sec1.3");
    pHistArray->Add(hist2);
    hist2=HRichHistFac::getHRingAmplitudeTheta("2RingsPerPair.sec1.3");
    pHistArray->Add(hist2);
    hist2=HRichHistFac::getHRingAmplitudeTheta("1RingPerPair.sec2.5");
    pHistArray->Add(hist2);
    hist2=HRichHistFac::getHRingAmplitudeTheta("2RingsPerPair.sec2.5");
    pHistArray->Add(hist2);
    hist2=HRichHistFac::getHRingAmplitudeTheta("1RingPerPair.sec4.6");
    pHistArray->Add(hist2);
    hist2=HRichHistFac::getHRingAmplitudeTheta("2RingsPerPair.sec4.6");
    pHistArray->Add(hist2);
    //
    hist=HRichHistFac::getHRingRadius("1RingPerPair");
    pHistArray->Add(hist);
    hist=HRichHistFac::getHRingRadius("2RingsPerPair");
    pHistArray->Add(hist);
    hist2=HRichHistFac::getHRingRadiusTheta("1RingPerPair");
    pHistArray->Add(hist2);
    hist2=HRichHistFac::getHRingRadiusTheta("2RingsPerPair");
    pHistArray->Add(hist2);
    hist2=HRichHistFac::getHRingRadiusTheta("1RingPerPair.sec1.3");
    pHistArray->Add(hist2);
    hist2=HRichHistFac::getHRingRadiusTheta("2RingsPerPair.sec1.3");
    pHistArray->Add(hist2);
    hist2=HRichHistFac::getHRingRadiusTheta("1RingPerPair.sec2.5");
    pHistArray->Add(hist2);
    hist2=HRichHistFac::getHRingRadiusTheta("2RingsPerPair.sec2.5");
    pHistArray->Add(hist2);
    hist2=HRichHistFac::getHRingRadiusTheta("1RingPerPair.sec4.6");
    pHistArray->Add(hist2);
    hist2=HRichHistFac::getHRingRadiusTheta("2RingsPerPair.sec4.6");
    pHistArray->Add(hist2);
    //
    hist=HRichHistFac::getHRingPadNr("1RingPerPair");
    pHistArray->Add(hist);
    hist=HRichHistFac::getHRingPadNr("2RingsPerPair");
    pHistArray->Add(hist);
    hist2=HRichHistFac::getHRingPadNrTheta("1RingPerPair");
    pHistArray->Add(hist2);
    hist2=HRichHistFac::getHRingPadNrTheta("2RingsPerPair");
    pHistArray->Add(hist2);
    hist2=HRichHistFac::getHRingPadNrTheta("1RingPerPair.sec1.3");
    pHistArray->Add(hist2);
    hist2=HRichHistFac::getHRingPadNrTheta("2RingsPerPair.sec1.3");
    pHistArray->Add(hist2);
    hist2=HRichHistFac::getHRingPadNrTheta("1RingPerPair.sec2.5");
    pHistArray->Add(hist2);
    hist2=HRichHistFac::getHRingPadNrTheta("2RingsPerPair.sec2.5");
    pHistArray->Add(hist2);
    hist2=HRichHistFac::getHRingPadNrTheta("1RingPerPair.sec4.6");
    pHistArray->Add(hist2);
    hist2=HRichHistFac::getHRingPadNrTheta("2RingsPerPair.sec4.6");
    pHistArray->Add(hist2);
    //
    hist=HRichHistFac::getHRingLocMax4("1RingPerPair");
    pHistArray->Add(hist);
    hist=HRichHistFac::getHRingLocMax4("2RingsPerPair");
    pHistArray->Add(hist);
    hist2=HRichHistFac::getHRingLocMax4Theta("1RingPerPair");
    pHistArray->Add(hist2);
    hist2=HRichHistFac::getHRingLocMax4Theta("2RingsPerPair");
    pHistArray->Add(hist2);
    hist2=HRichHistFac::getHRingLocMax4Theta("1RingPerPair.sec1.3");
    pHistArray->Add(hist2);
    hist2=HRichHistFac::getHRingLocMax4Theta("2RingsPerPair.sec1.3");
    pHistArray->Add(hist2);
    hist2=HRichHistFac::getHRingLocMax4Theta("1RingPerPair.sec2.5");
    pHistArray->Add(hist2);
    hist2=HRichHistFac::getHRingLocMax4Theta("2RingsPerPair.sec2.5");
    pHistArray->Add(hist2);
    hist2=HRichHistFac::getHRingLocMax4Theta("1RingPerPair.sec4.6");
    pHistArray->Add(hist2);
    hist2=HRichHistFac::getHRingLocMax4Theta("2RingsPerPair.sec4.6");
    pHistArray->Add(hist2);
    //
    hist=HRichHistFac::getHRingPM("1RingPerPair");
    pHistArray->Add(hist);
    hist=HRichHistFac::getHRingPM("2RingsPerPair");
    pHistArray->Add(hist);
    hist2=HRichHistFac::getHRingPMTheta("1RingPerPair");
    pHistArray->Add(hist2);
    hist2=HRichHistFac::getHRingPMTheta("2RingsPerPair");
    pHistArray->Add(hist2);
    hist2=HRichHistFac::getHRingPMTheta("1RingPerPair.sec1.3");
    pHistArray->Add(hist2);
    hist2=HRichHistFac::getHRingPMTheta("2RingsPerPair.sec1.3");
    pHistArray->Add(hist2);
    hist2=HRichHistFac::getHRingPMTheta("1RingPerPair.sec2.5");
    pHistArray->Add(hist2);
    hist2=HRichHistFac::getHRingPMTheta("2RingsPerPair.sec2.5");
    pHistArray->Add(hist2);
    hist2=HRichHistFac::getHRingPMTheta("1RingPerPair.sec4.6");
    pHistArray->Add(hist2);
    hist2=HRichHistFac::getHRingPMTheta("2RingsPerPair.sec4.6");
    pHistArray->Add(hist2);
    //
    hist=HRichHistFac::getHRingHT("1RingPerPair");
    pHistArray->Add(hist);
    hist=HRichHistFac::getHRingHT("2RingsPerPair");
    pHistArray->Add(hist);
    hist2=HRichHistFac::getHRingHTTheta("1RingPerPair");
    pHistArray->Add(hist2);
    hist2=HRichHistFac::getHRingHTTheta("2RingsPerPair");
    pHistArray->Add(hist2);
    hist2=HRichHistFac::getHRingHTTheta("1RingPerPair.sec1.3");
    pHistArray->Add(hist2);
    hist2=HRichHistFac::getHRingHTTheta("2RingsPerPair.sec1.3");
    pHistArray->Add(hist2);
    hist2=HRichHistFac::getHRingHTTheta("1RingPerPair.sec2.5");
    pHistArray->Add(hist2);
    hist2=HRichHistFac::getHRingHTTheta("2RingsPerPair.sec2.5");
    pHistArray->Add(hist2);
    hist2=HRichHistFac::getHRingHTTheta("1RingPerPair.sec4.6");
    pHistArray->Add(hist2);
    hist2=HRichHistFac::getHRingHTTheta("2RingsPerPair.sec4.6");
    pHistArray->Add(hist2);
    //


    hist=HRichHistFac::getHOpeningAngle("MDC_1RingPerPair");
    pHistArray->Add(hist);
    // -----------------------------------------
    hist=HRichHistFac::getHOpeningAngle("MDC_2RingsPerPair");
    pHistArray->Add(hist);
    // -----------------------------------------
    hist=HRichHistFac::getHOpeningAngle("KICK_1RingPerPair");
    pHistArray->Add(hist);
    // -----------------------------------------
    hist=HRichHistFac::getHOpeningAngle("KICK_2RingsPerPair");
    pHistArray->Add(hist);
    // -----------------------------------------
    hist=new TH1F("MDC_META_diff","MDC_META_diff",200,-10,10);
    pHistArray->Add(hist);
}

Bool_t HRichPairCreator::finalize() {
     HRichUtilFunc::saveHistos(pFileOut,pHistArray);
     pFileOut->Close();

    return kTRUE;
}


Int_t HRichPairCreator::execute()
{
//      if (nCounterProcessedNbEvents != 0 &&
//   	nCounterProcessedNbEvents%5000==0) HRichUtilFunc::saveHistos(pFileOut,pHistArray);
    //cout<<"begin evt"<<endl;
    // select tracklets and store them in a obj array
    TObjArray* t = selectTracklets();
    // ----------------------------------------------
    if (!fillHistosFromTracklets(t)) Error("execute","fillHistosFromTracklets() failed");
    // ----------------------------------------------
    // make all combinations of objs in array
    if (!makePairs(t)) Error("execute","makePairs() failed");
    // --------------------------------------

    // delete temp obj array
    delete t;
    // ---------------------
    //    cout<<"###########################"<<endl;
    // use new pair objects to fill histos
    if (!fillHistosFromPairs()) Error("execute","fillHistosFromPairs() failed");
    // -----------------------------------
    Int_t kReturnValue=0;//proceed with next task
    //cout<<"end evt"<<endl;
    nCounterProcessedNbEvents++;
    return kReturnValue;
    
}

HDiHitMatch* HRichPairCreator::createHitDiMatch(HHitMatch *h1, HHitMatch *h2)
{
    //create a new HHitMatch object, input sector
    HLocation loc;
    loc.set(1,0);//dummy ?!!
    HDiHitMatch *hdi = (HDiHitMatch*)((HLinearCategory*)
					getHitDiMatchCat())->getNewSlot(loc);
    //cout<<"h1 "<<h1->getRichTheta()-h1->getMdcTheta()<<endl;
    //cout<<"h2 "<<h2->getRichTheta()-h2->getMdcTheta()<<endl;
    if (hdi!=NULL) hdi = new(hdi) HDiHitMatch(h1,h2);
    return hdi;

}

TObjArray* HRichPairCreator::selectTracklets()
{
    TObjArray *arr = new TObjArray(5);

    HHitMatch *h=0;
    
    pIterMatchHit->Reset();
    while(( h= (HHitMatch *)pIterMatchHit->Next())) 
    {
	//h->dumpToStdout();
	if (HRichCut::isFullRichMdcMetaTracklet(h) &&
	    HRichCut::isRMThetaDiff(h,2.)          &&
	    HRichCut::isTOFBetween(h,5.,10.)            ) 
	    // change cut here to select different kinds of tracks
	{
	    //cout<<h->getRichTheta()-h->getMdcTheta()<<endl;
	    arr->Add(h);
	}
    }

    return arr;
}

Int_t HRichPairCreator::fillHistosFromTracklets(TObjArray* t)
{
 Int_t ret_val=1;
 Int_t max = t->GetLast()+1;
 Int_t *tupt = new Int_t[max];
 Int_t *tups = new Int_t[max];
 for (Int_t j=0;j<max;j++) tupt[j]=tups[j]=-2;
    for (Int_t i=0;i<max;i++)
    {
	HHitMatch *hi = ((HHitMatch*)(*t)[i]);
	Bool_t kNew2Tuple=kFALSE;
	Float_t mt=hi->getMdcTheta();
	Float_t mm=0.;
	Int_t mdcidx=hi->getMdcInd();
	Int_t metaidx=-1;
	if (hi->getTofTheta()>0.) {mm=hi->getTofTheta();
	metaidx=hi->getTofInd(); 
	 kNew2Tuple=HRichCut::isNew2Tuple(mdcidx,metaidx,tupt,max);
	}
	else if (hi->getShowerTheta()>0.) {mm=hi->getShowerTheta();
	metaidx=hi->getShowInd();
	 kNew2Tuple=HRichCut::isNew2Tuple(mdcidx,metaidx,tups,max);
	}
	TH1F *hist = (TH1F*)pHistArray->FindObject("MDC_META_diff");
	if (kNew2Tuple) hist->Fill(mt-mm);
    }
    delete [] tupt;
    delete [] tups;
    return ret_val;
}

Int_t HRichPairCreator::makePairs(TObjArray* t)
{
    Int_t ret_val=1;

    // make all combinations of objs in array 
    // and create HDiHitMatch "pair" objs
    Int_t n=0;
    Int_t max = t->GetLast()+1;
    for (Int_t i=0;i<max;i++)
    {
	HHitMatch *hi = ((HHitMatch*)(*t)[i]);
	n++;
	for (Int_t j=n;j<max;j++)
	{
	    HHitMatch *hj = ((HHitMatch*)(*t)[j]);

	    //cout<<"&&&&&&&&&beginpair"<<endl;
	    //hi->dumpToStdout();
	    //hj->dumpToStdout();
	    
	    HDiHitMatch* hdi=createHitDiMatch(hi,hj);

	  Int_t ind1 = getHitMatchCat()->getIndex(hi);
	  Int_t ind2 = getHitMatchCat()->getIndex(hj);
	  hdi->setIndTrk1(ind1);
	  hdi->setIndTrk2(ind2);
//  	  cout<<"track1: "<<ind1<<" : "<<hi->getRichTheta()-hi->getMdcTheta()<<endl;
//  	  cout<<"track2: "<<ind2<<" : "<<hj->getRichTheta()-hj->getMdcTheta()<<endl;
//  	  cout<<"pair created"<<endl;
	  //cout<<"&&&&&&&&&endpair"<<endl;
	}
    }
    // --------------------------------------

    return ret_val;

}

Int_t HRichPairCreator::fillHistosFromPairs()
{
    Int_t ret_val=1;
    Int_t max=getHitDiMatchCat()->getEntries();
    // helper arrays to determine unique nature of index tuples
    Int_t *MDCoptuple=new Int_t[max];
    Int_t *KICKoptuple=new Int_t[max];
    Int_t *METAoptuple=new Int_t[max];
    //---
    Int_t *KICKinvmasstuple=new Int_t[max];
    //---
    Int_t *PtVsPttuple=new Int_t[max];
    Int_t *PtVsPtottuple=new Int_t[max];
    Int_t *PtotVsPtottuple=new Int_t[max];
    //---
    Int_t *TofTuple=new Int_t[max];
    Int_t *ShowerTuple=new Int_t[max];

    for (Int_t i=0;i<max;i++) MDCoptuple[i]=KICKoptuple[i]=METAoptuple[i]=
				  KICKinvmasstuple[i]=
				  PtVsPttuple[i]=PtVsPtottuple[i]=
				  PtotVsPtottuple[i]=
				  TofTuple[i]=ShowerTuple[i]=-2;
    // --------------------------------------------------------
    TObjArray* pairs1ring = new TObjArray(5);
    TObjArray* pairs2rings = new TObjArray(5);
    // ----

    
    HDiHitMatch *h=0;
    pIterDiMatchHit->Reset();
    //cout<<"&&&&&&&&&&& begin evt &&&&&&&&&&&"<<endl;
    while(( h= (HDiHitMatch *)pIterDiMatchHit->Next())) 
    {

//  	HHitMatch *hi = (HHitMatch*)getHitMatchCat()
//  	    ->getObject(h->getIndTrk1());
//  	HHitMatch *hj = (HHitMatch*)getHitMatchCat()
//  	    ->getObject(h->getIndTrk2());

//  	cout<<"track1: "<<h->getIndTrk1()<<" : "<<hi->getRichTheta()-hi->getMdcTheta()<<endl;
//  	cout<<"track2: "<<h->getIndTrk2()<<" : "<<hj->getRichTheta()-hj->getMdcTheta()<<endl;

	if(!fillOpangMDC(h,MDCoptuple,max,"MDC")) Error("fillHistosFromPairs",
						  "error in fillOpangMDC");
  	if(!fillOpangKICK(h,KICKoptuple,max,"KICK")) Error("fillHistosFromPairs",
						    "error in fillOpangKICK");
  	if(!fillOpangMETA(h,METAoptuple,max)) Error("fillHistosFromPairs",
						    "error in fillOpangMETA");
	//---
	if(!fillInvMass(h,KICKinvmasstuple,max)) Error("fillHistosFromPairs",
						  "error in fillInvMass");
	//---
	if(!fillPt_vs_Pt(h,PtVsPttuple,max)) Error("fillHistosFromPairs",
						  "error in fillPt_vs_Pt");
	if(!fillPt_vs_Ptot(h,PtVsPtottuple,max)) Error("fillHistosFromPairs",
						  "error in fillPt_vs_Ptot");
	if(!fillPtot_vs_Ptot(h,PtotVsPtottuple,max)) Error("fillHistosFromPairs",
						  "error in fillPtot_vs_Ptot");

	if(!fillTof(h,TofTuple,ShowerTuple,max)) Error("fillHistosFromPairs",
						  "error in fillTof");

	// PAIR OPENING ANGLE CUT
	if(HRichCut::NbRingsPerMDCUNLIKEPair(h)==1 &&
	   HRichCut::isOpangBetween(h,5.,12.)     &&
	   HRichCut::isInKickTrack(h)                 
	   ) pairs1ring->Add(h);
	if(HRichCut::NbRingsPerMDCUNLIKEPair(h)==2 && 
	   HRichCut::isOpangBetween(h,12.,50.)      &&
	   HRichCut::isInKickTrack(h)                 
	   ) pairs2rings->Add(h);
    }
    //cout<<"&&&&&&&&&&& end evt &&&&&&&&&&&&&"<<endl;

    
    if(!fillHistosRingProperties(pairs1ring,"1RingPerPair")) Error("","");
    if(!fillHistosRingProperties(pairs2rings,"2RingsPerPair")) Error("","");

    delete MDCoptuple;
    delete KICKoptuple;
    delete METAoptuple;
    delete KICKinvmasstuple;
    delete TofTuple;
    delete ShowerTuple;
    delete pairs1ring;
    delete pairs2rings;
    return ret_val;
}

Int_t HRichPairCreator::fillHistosRingProperties(TObjArray* arr,Char_t* c1)
{
    // use pairs selected into array to fill histograms
    // c1 is a string characterising the content of the pair array
    // it is passed to the name of the filled histograms
    TString title(c1);
    TString mdct("MDC_"); mdct.Append(title);
    TString kickt("KICK_"); kickt.Append(title);
    Int_t ret_val=1;
    Int_t max = arr->GetLast()+1;


    // do not use a feature twice 
    // remember the index or tuple of the feature to be histogrammed in its
    // respective category and make sure that the same is not used twice
    // for the same histogram
    Int_t *MDCoptuple      = new Int_t[max];
    Int_t *KICKoptuple     = new Int_t[max];
    Int_t *RICHindexAmpl   = new Int_t[max];
    Int_t *RICHindexAmplT  = new Int_t[max];
    Int_t *RICHindexAmplTS13  = new Int_t[max];
    Int_t *RICHindexAmplTS25  = new Int_t[max];
    Int_t *RICHindexAmplTS46  = new Int_t[max];
    Int_t *RICHindexRad   = new Int_t[max];
    Int_t *RICHindexRadT  = new Int_t[max];
    Int_t *RICHindexRadTS13  = new Int_t[max];
    Int_t *RICHindexRadTS25  = new Int_t[max];
    Int_t *RICHindexRadTS46  = new Int_t[max];
    Int_t *RICHindexPad    = new Int_t[max];
    Int_t *RICHindexPadT   = new Int_t[max];
    Int_t *RICHindexPadTS13   = new Int_t[max];
    Int_t *RICHindexPadTS25   = new Int_t[max];
    Int_t *RICHindexPadTS46   = new Int_t[max];
    Int_t *RICHindexLocMax4   = new Int_t[max];
    Int_t *RICHindexLocMax4T   = new Int_t[max];
    Int_t *RICHindexLocMax4TS13   = new Int_t[max];
    Int_t *RICHindexLocMax4TS25   = new Int_t[max];
    Int_t *RICHindexLocMax4TS46   = new Int_t[max];
    Int_t *RICHindexHT   = new Int_t[max];
    Int_t *RICHindexHTT   = new Int_t[max];
    Int_t *RICHindexHTTS13   = new Int_t[max];
    Int_t *RICHindexHTTS25   = new Int_t[max];
    Int_t *RICHindexHTTS46   = new Int_t[max];
    Int_t *RICHindexPM   = new Int_t[max];
    Int_t *RICHindexPMT   = new Int_t[max];
    Int_t *RICHindexPMTS13   = new Int_t[max];
    Int_t *RICHindexPMTS25   = new Int_t[max];
    Int_t *RICHindexPMTS46   = new Int_t[max];

    // default init to -2 necessary for cuts
    for (Int_t i=0;i<max;i++) 
    {
	 MDCoptuple[i]=-2; 
	 KICKoptuple[i]=-2;
	 RICHindexAmpl[i]=-2;
	 RICHindexAmplT[i]=-2;
	 RICHindexAmplTS13[i]=-2;
	 RICHindexAmplTS25[i]=-2;
	 RICHindexAmplTS46[i]=-2;
	 RICHindexRad[i]=-2;
	 RICHindexRadT[i]=-2;
	 RICHindexRadTS13[i]=-2;
	 RICHindexRadTS25[i]=-2;
	 RICHindexRadTS46[i]=-2;
	 RICHindexPad[i]=-2;   
	 RICHindexPadT[i]=-2;
	 RICHindexPadTS13[i]=-2;
	 RICHindexPadTS25[i]=-2;
	 RICHindexPadTS46[i]=-2;
	 RICHindexLocMax4[i]=-2;
	 RICHindexLocMax4T[i]=-2;
	 RICHindexLocMax4TS13[i]=-2;
	 RICHindexLocMax4TS25[i]=-2;
	 RICHindexLocMax4TS46[i]=-2;
	 RICHindexHT[i]=-2;
	 RICHindexHTT[i]=-2;
	 RICHindexHTTS13[i]=-2;
	 RICHindexHTTS25[i]=-2;
	 RICHindexHTTS46[i]=-2;
	 RICHindexPM[i]=-2;
	 RICHindexPMT[i]=-2;
	 RICHindexPMTS13[i]=-2;
	 RICHindexPMTS25[i]=-2;
	 RICHindexPMTS46[i]=-2;

    }


    // loop over pairs in arr
    for (Int_t i=0;i<max;i++)
    {
	HDiHitMatch *pair = ((HDiHitMatch*)(*arr)[i]);
	if(HRichCut::isUnlikeSignPair(pair))//redundant here
	{
	    // opening angle spectra of selected pairs
	    if(!fillOpangMDC(pair,MDCoptuple,max,mdct.Data())) Error("fillHistosRingProperties",
						  "error in fillOpangMDC");
	    if(!fillOpangKICK(pair,KICKoptuple,max,kickt.Data())) Error("fillHistosRingProperties",
						  "error in fillOpangMDC");


	    // ring property spectra for rings contained in pair
	    // 1D


	    if(!fillRingPadNr(pair,RICHindexPad,max,c1)) Error("","");
	    if(!fillRingAmplitude(pair,RICHindexAmpl,max,c1)) Error("","");
	    if(!fillRingRadius(pair,RICHindexRad,max,c1)) Error("","");
	    if(!fillRingLocMax4(pair,RICHindexLocMax4,max,c1)) Error("","");
	    if(!fillRingPM(pair,RICHindexPM,max,c1)) Error("","");
	    if(!fillRingHT(pair,RICHindexHT,max,c1)) Error("","");

	    // 2D: property vs polar angle
	    if(!fillRingPadNrTheta(pair,RICHindexPadT,max,c1)) Error("","");
	    if(!fillRingAmplitudeTheta(pair,RICHindexAmplT,max,c1)) Error("","");
	    if(!fillRingRadiusTheta(pair,RICHindexRadT,max,c1)) Error("","");
	    if(!fillRingLocMax4Theta(pair,RICHindexLocMax4T,max,c1)) Error("","");
	    if(!fillRingPMTheta(pair,RICHindexPMT,max,c1)) Error("","");
	    if(!fillRingHTTheta(pair,RICHindexHTT,max,c1)) Error("","");

	    // 2D: property vs polar angle with cut on sector
	    if(!fillRingPadNrThetaSec(pair,RICHindexPadTS13,max,c1,".sec1.3",1,3)) Error("","");
	    if(!fillRingPadNrThetaSec(pair,RICHindexPadTS25,max,c1,".sec2.5",2,5)) Error("","");
	    if(!fillRingPadNrThetaSec(pair,RICHindexPadTS46,max,c1,".sec4.6",4,6)) Error("","");

	    if(!fillRingAmplitudeThetaSec(pair,RICHindexAmplTS13,max,c1,".sec1.3",1,3)) Error("","");
	    if(!fillRingAmplitudeThetaSec(pair,RICHindexAmplTS25,max,c1,".sec2.5",2,5)) Error("","");
	    if(!fillRingAmplitudeThetaSec(pair,RICHindexAmplTS46,max,c1,".sec4.6",4,6)) Error("","");
	    if(!fillRingRadiusThetaSec(pair,RICHindexRadTS13,max,c1,".sec1.3",1,3)) Error("","");
	    if(!fillRingRadiusThetaSec(pair,RICHindexRadTS25,max,c1,".sec2.5",2,5)) Error("","");
	    if(!fillRingRadiusThetaSec(pair,RICHindexRadTS46,max,c1,".sec4.6",4,6)) Error("","");

	    if(!fillRingLocMax4ThetaSec(pair,RICHindexLocMax4TS13,max,c1,".sec1.3",1,3)) Error("","");
	    if(!fillRingLocMax4ThetaSec(pair,RICHindexLocMax4TS25,max,c1,".sec2.5",2,5)) Error("","");
	    if(!fillRingLocMax4ThetaSec(pair,RICHindexLocMax4TS46,max,c1,".sec4.6",4,6)) Error("","");


	    if(!fillRingPMThetaSec(pair,RICHindexPMTS13,max,c1,".sec1.3",1,3)) Error("","");
	    if(!fillRingPMThetaSec(pair,RICHindexPMTS25,max,c1,".sec2.5",2,5)) Error("","");
	    if(!fillRingPMThetaSec(pair,RICHindexPMTS46,max,c1,".sec4.6",4,6)) Error("","");

	    if(!fillRingHTThetaSec(pair,RICHindexHTTS13,max,c1,".sec1.3",1,3)) Error("","");
	    if(!fillRingHTThetaSec(pair,RICHindexHTTS25,max,c1,".sec2.5",2,5)) Error("","");
	    if(!fillRingHTThetaSec(pair,RICHindexHTTS46,max,c1,".sec4.6",4,6)) Error("","");
	}
    }

    // clean up memory
    delete MDCoptuple; 
    delete KICKoptuple;
    delete RICHindexAmpl;
    delete RICHindexAmplT;
    delete RICHindexRad;
    delete RICHindexRadT;
    delete RICHindexAmplTS13;
    delete RICHindexAmplTS25;
    delete RICHindexAmplTS46;
    delete RICHindexRadTS13;
    delete RICHindexRadTS25;
    delete RICHindexRadTS46;
    delete RICHindexPad;   
    delete RICHindexPadT;
    delete RICHindexPadTS13;
    delete RICHindexPadTS25;
    delete RICHindexPadTS46;
    delete RICHindexLocMax4;
    delete RICHindexLocMax4T;
    delete RICHindexLocMax4TS13;
    delete RICHindexLocMax4TS25;
    delete RICHindexLocMax4TS46;
    delete RICHindexHT;
    delete RICHindexHTT;
    delete RICHindexHTTS13;
    delete RICHindexHTTS25;
    delete RICHindexHTTS46;
    delete RICHindexPM;
    delete RICHindexPMT;
    delete RICHindexPMTS13;
    delete RICHindexPMTS25;
    delete RICHindexPMTS46;


    return ret_val;

}

Int_t HRichPairCreator::fillRingPadNr(HDiHitMatch *h,Int_t *tup,Int_t max,Char_t* title)
{
    Int_t ret_val=1;

    // retrieve tracks that formed pair
    HHitMatch *track1 = (HHitMatch*)getHitMatchCat()
	->getObject(h->getIndTrk1());
    HHitMatch *track2 = (HHitMatch*)getHitMatchCat()
	->getObject(h->getIndTrk2());

	
    Int_t *indHitTrk1 = h->getIndHitsTrk1();
    Int_t *indHitTrk2 = h->getIndHitsTrk2();
    Int_t ind1 = indHitTrk1[0];//index for RICH
    Int_t ind2 = indHitTrk2[0];//c.f. hdihitmatch.h
    
    Bool_t kNewIndex1=HRichCut::isNewIndex(ind1,tup,max);    
    Bool_t kNewIndex2=HRichCut::isNewIndex(ind2,tup,max);


    if (kNewIndex1)
    {
	TString s(HRichHistFac::getHRingPadNrBasename());
	s.Append(title);
	TH1F* h1 = ((TH1F*)(pHistArray->FindObject(s.Data())));

	if (h1&&HRichCut::isRICHBetween(track1,nRichPolarLow,nRichPolarHigh)) h1->Fill(track1->getRingPadNr());
    }

    if (kNewIndex2)
    {
	TString s(HRichHistFac::getHRingPadNrBasename());
	s.Append(title);
	TH1F* h1 = ((TH1F*)(pHistArray->FindObject(s.Data())));
	if (h1&&HRichCut::isRICHBetween(track2,nRichPolarLow,nRichPolarHigh)) h1->Fill(track2->getRingPadNr());
    }
    return ret_val;
}
Int_t HRichPairCreator::fillRingPadNrTheta(HDiHitMatch *h,Int_t *tup,Int_t max,Char_t* title)
{
    Int_t ret_val=1;

    // retrieve tracks that formed pair
    HHitMatch *track1 = (HHitMatch*)getHitMatchCat()
	->getObject(h->getIndTrk1());
    HHitMatch *track2 = (HHitMatch*)getHitMatchCat()
	->getObject(h->getIndTrk2());
	
    Int_t *indHitTrk1 = h->getIndHitsTrk1();
    Int_t *indHitTrk2 = h->getIndHitsTrk2();
    Int_t ind1 = indHitTrk1[0];//index for RICH
    Int_t ind2 = indHitTrk2[0];//c.f. hdihitmatch.h
    
    Bool_t kNewIndex1=HRichCut::isNewIndex(ind1,tup,max);    
    Bool_t kNewIndex2=HRichCut::isNewIndex(ind2,tup,max);


    if (kNewIndex1)
    {
	TString s(HRichHistFac::getHRingPadNrThetaBasename());
	s.Append(title);
	TH2F* h1 = ((TH2F*)(pHistArray->FindObject(s.Data())));
	if (h1) h1->Fill(track1->getRichTheta(),
			 track1->getRingPadNr());
    }

    if (kNewIndex2)
    {
	TString s(HRichHistFac::getHRingPadNrThetaBasename());
	s.Append(title);
	TH2F* h1 = ((TH2F*)(pHistArray->FindObject(s.Data())));
	if (h1) h1->Fill(track2->getRichTheta(),
			 track2->getRingPadNr());
    }
    return ret_val;
}

Int_t HRichPairCreator::fillRingPadNrThetaSec(HDiHitMatch *h,Int_t *tup,Int_t max,Char_t* title,Char_t *title2,Int_t sec1, Int_t sec2)
{
    Int_t ret_val=1;

    // retrieve tracks that formed pair
    HHitMatch *track1 = (HHitMatch*)getHitMatchCat()
	->getObject(h->getIndTrk1());
    HHitMatch *track2 = (HHitMatch*)getHitMatchCat()
	->getObject(h->getIndTrk2());
	
    Int_t *indHitTrk1 = h->getIndHitsTrk1();
    Int_t *indHitTrk2 = h->getIndHitsTrk2();
    Int_t ind1 = indHitTrk1[0];//index for RICH
    Int_t ind2 = indHitTrk2[0];//c.f. hdihitmatch.h
    
    if ( (track1->getSector()==sec1 || track1->getSector()==sec2) &&
	 (track2->getSector()==sec1 || track2->getSector()==sec2)    )
    {

	Bool_t kNewIndex1=HRichCut::isNewIndex(ind1,tup,max);    
	Bool_t kNewIndex2=HRichCut::isNewIndex(ind2,tup,max);


	if (kNewIndex1)
	{
	    TString s(HRichHistFac::getHRingPadNrThetaBasename());
	    s.Append(title);
	    s.Append(title2);
	    TH2F* h1 = ((TH2F*)(pHistArray->FindObject(s.Data())));
	    if (h1) h1->Fill(track1->getRichTheta(),
			     track1->getRingPadNr());
	}
	
	if (kNewIndex2)
	{
	    TString s(HRichHistFac::getHRingPadNrThetaBasename());
	    s.Append(title);
	    s.Append(title2);
	    TH2F* h1 = ((TH2F*)(pHistArray->FindObject(s.Data())));
	    if (h1) h1->Fill(track2->getRichTheta(),
			     track2->getRingPadNr());
	}
    }
    return ret_val;
}


Int_t HRichPairCreator::fillRingAmplitude(HDiHitMatch *h,Int_t *tup,Int_t max,Char_t* title)
{
    Int_t ret_val=1;

    // retrieve tracks that formed pair
    HHitMatch *track1 = (HHitMatch*)getHitMatchCat()
	->getObject(h->getIndTrk1());
    HHitMatch *track2 = (HHitMatch*)getHitMatchCat()
	->getObject(h->getIndTrk2());
	
    Int_t *indHitTrk1 = h->getIndHitsTrk1();
    Int_t *indHitTrk2 = h->getIndHitsTrk2();
    Int_t ind1 = indHitTrk1[0];//index for RICH
    Int_t ind2 = indHitTrk2[0];//c.f. hdihitmatch.h
    
    Bool_t kNewIndex1=HRichCut::isNewIndex(ind1,tup,max);    
    Bool_t kNewIndex2=HRichCut::isNewIndex(ind2,tup,max);


    if (kNewIndex1)
    {
	TString s(HRichHistFac::getHRingAmplitudeBasename());
	s.Append(title);
	TH1F* h1 = ((TH1F*)(pHistArray->FindObject(s.Data())));
	if (h1&&HRichCut::isRICHBetween(track1,nRichPolarLow,nRichPolarHigh)) h1->Fill(track1->getRingAmplitude());
    }

    if (kNewIndex2)
    {
	TString s(HRichHistFac::getHRingAmplitudeBasename());
	s.Append(title);
	TH1F* h1 = ((TH1F*)(pHistArray->FindObject(s.Data())));
	if (h1&&HRichCut::isRICHBetween(track2,nRichPolarLow,nRichPolarHigh)) h1->Fill(track2->getRingAmplitude());
    }
    return ret_val;
}

Int_t HRichPairCreator::fillRingAmplitudeTheta(HDiHitMatch *h,Int_t *tup,Int_t max,Char_t* title)
{
    Int_t ret_val=1;

    // retrieve tracks that formed pair
    HHitMatch *track1 = (HHitMatch*)getHitMatchCat()
	->getObject(h->getIndTrk1());
    HHitMatch *track2 = (HHitMatch*)getHitMatchCat()
	->getObject(h->getIndTrk2());

    // retrieve indexes of hits that formed respective track
    Int_t *indHitTrk1 = h->getIndHitsTrk1();
    Int_t *indHitTrk2 = h->getIndHitsTrk2();
    Int_t ind1 = indHitTrk1[0];//index for RICH
    Int_t ind2 = indHitTrk2[0];//c.f. hdihitmatch.h
    
    Bool_t kNewIndex1=HRichCut::isNewIndex(ind1,tup,max);    
    Bool_t kNewIndex2=HRichCut::isNewIndex(ind2,tup,max);


    if (kNewIndex1)
    {
	TString s(HRichHistFac::getHRingAmplitudeThetaBasename());
	s.Append(title);
	TH2F* h1 = ((TH2F*)(pHistArray->FindObject(s.Data())));
	//cout<<track1->getRichTheta()<<" "<<track1->getRingAmplitude()<<endl;
	if (h1) h1->Fill(track1->getRichTheta(),
			 track1->getRingAmplitude());
    }

    if (kNewIndex2)
    {
	TString s(HRichHistFac::getHRingAmplitudeThetaBasename());
	s.Append(title);
	TH2F* h1 = ((TH2F*)(pHistArray->FindObject(s.Data())));
	if (h1) h1->Fill(track2->getRichTheta(),
			 track2->getRingAmplitude());
    }
    return ret_val;
}

Int_t HRichPairCreator::fillRingAmplitudeThetaSec(HDiHitMatch *h,Int_t *tup,Int_t max,Char_t* title,Char_t *title2,Int_t sec1, Int_t sec2)
{
    Int_t ret_val=1;

    // retrieve tracks that formed pair
    HHitMatch *track1 = (HHitMatch*)getHitMatchCat()
	->getObject(h->getIndTrk1());
    HHitMatch *track2 = (HHitMatch*)getHitMatchCat()
	->getObject(h->getIndTrk2());
	
    Int_t *indHitTrk1 = h->getIndHitsTrk1();
    Int_t *indHitTrk2 = h->getIndHitsTrk2();
    Int_t ind1 = indHitTrk1[0];//index for RICH
    Int_t ind2 = indHitTrk2[0];//c.f. hdihitmatch.h
    
    if ( (track1->getSector()==sec1 || track1->getSector()==sec2) &&
	 (track2->getSector()==sec1 || track2->getSector()==sec2)    )
    {

	Bool_t kNewIndex1=HRichCut::isNewIndex(ind1,tup,max);    
	Bool_t kNewIndex2=HRichCut::isNewIndex(ind2,tup,max);


	if (kNewIndex1)
	{
	    TString s(HRichHistFac::getHRingAmplitudeThetaBasename());
	    s.Append(title);
	    s.Append(title2);
	    TH2F* h1 = ((TH2F*)(pHistArray->FindObject(s.Data())));
	    if (h1) h1->Fill(track1->getRichTheta(),
			     track1->getRingAmplitude());
	}
	
	if (kNewIndex2)
	{
	    TString s(HRichHistFac::getHRingAmplitudeThetaBasename());
	    s.Append(title);
	    s.Append(title2);
	    TH2F* h1 = ((TH2F*)(pHistArray->FindObject(s.Data())));
	    if (h1) h1->Fill(track2->getRichTheta(),
			     track2->getRingAmplitude());
	}
    }
    return ret_val;
}
Int_t HRichPairCreator::fillRingRadius(HDiHitMatch *h,Int_t *tup,Int_t max,Char_t* title)
{
    Int_t ret_val=1;

    // retrieve tracks that formed pair
    HHitMatch *track1 = (HHitMatch*)getHitMatchCat()
	->getObject(h->getIndTrk1());
    HHitMatch *track2 = (HHitMatch*)getHitMatchCat()
	->getObject(h->getIndTrk2());
	
    Int_t *indHitTrk1 = h->getIndHitsTrk1();
    Int_t *indHitTrk2 = h->getIndHitsTrk2();
    Int_t ind1 = indHitTrk1[0];//index for RICH
    Int_t ind2 = indHitTrk2[0];//c.f. hdihitmatch.h
    
    Bool_t kNewIndex1=HRichCut::isNewIndex(ind1,tup,max);    
    Bool_t kNewIndex2=HRichCut::isNewIndex(ind2,tup,max);


    if (kNewIndex1)
    {
	TString s(HRichHistFac::getHRingRadiusBasename());
	s.Append(title);
	TH1F* h1 = ((TH1F*)(pHistArray->FindObject(s.Data())));
	if (h1&&HRichCut::isRICHBetween(track1,nRichPolarLow,nRichPolarHigh)) h1->Fill(track1->getRadius());
    }

    if (kNewIndex2)
    {
	TString s(HRichHistFac::getHRingRadiusBasename());
	s.Append(title);
	TH1F* h1 = ((TH1F*)(pHistArray->FindObject(s.Data())));
	if (h1&&HRichCut::isRICHBetween(track2,nRichPolarLow,nRichPolarHigh)) h1->Fill(track2->getRadius());
    }
    return ret_val;
}

Int_t HRichPairCreator::fillRingRadiusTheta(HDiHitMatch *h,Int_t *tup,Int_t max,Char_t* title)
{
    Int_t ret_val=1;

    // retrieve tracks that formed pair
    HHitMatch *track1 = (HHitMatch*)getHitMatchCat()
	->getObject(h->getIndTrk1());
    HHitMatch *track2 = (HHitMatch*)getHitMatchCat()
	->getObject(h->getIndTrk2());

    // retrieve indexes of hits that formed respective track
    Int_t *indHitTrk1 = h->getIndHitsTrk1();
    Int_t *indHitTrk2 = h->getIndHitsTrk2();
    Int_t ind1 = indHitTrk1[0];//index for RICH
    Int_t ind2 = indHitTrk2[0];//c.f. hdihitmatch.h
    
    Bool_t kNewIndex1=HRichCut::isNewIndex(ind1,tup,max);    
    Bool_t kNewIndex2=HRichCut::isNewIndex(ind2,tup,max);


    if (kNewIndex1)
    {
	TString s(HRichHistFac::getHRingRadiusThetaBasename());
	s.Append(title);
	TH2F* h1 = ((TH2F*)(pHistArray->FindObject(s.Data())));
	//cout<<track1->getRichTheta()<<" "<<track1->getRingAmplitude()<<endl;
	if (h1) h1->Fill(track1->getRichTheta(),
			 track1->getRadius());
    }

    if (kNewIndex2)
    {
	TString s(HRichHistFac::getHRingRadiusThetaBasename());
	s.Append(title);
	TH2F* h1 = ((TH2F*)(pHistArray->FindObject(s.Data())));
	if (h1) h1->Fill(track2->getRichTheta(),
			 track2->getRadius());
    }
    return ret_val;
}

Int_t HRichPairCreator::fillRingRadiusThetaSec(HDiHitMatch *h,Int_t *tup,Int_t max,Char_t* title,Char_t *title2,Int_t sec1, Int_t sec2)
{
    Int_t ret_val=1;

    // retrieve tracks that formed pair
    HHitMatch *track1 = (HHitMatch*)getHitMatchCat()
	->getObject(h->getIndTrk1());
    HHitMatch *track2 = (HHitMatch*)getHitMatchCat()
	->getObject(h->getIndTrk2());
	
    Int_t *indHitTrk1 = h->getIndHitsTrk1();
    Int_t *indHitTrk2 = h->getIndHitsTrk2();
    Int_t ind1 = indHitTrk1[0];//index for RICH
    Int_t ind2 = indHitTrk2[0];//c.f. hdihitmatch.h
    
    if ( (track1->getSector()==sec1 || track1->getSector()==sec2) &&
	 (track2->getSector()==sec1 || track2->getSector()==sec2)    )
    {

	Bool_t kNewIndex1=HRichCut::isNewIndex(ind1,tup,max);    
	Bool_t kNewIndex2=HRichCut::isNewIndex(ind2,tup,max);


	if (kNewIndex1)
	{
	    TString s(HRichHistFac::getHRingRadiusThetaBasename());
	    s.Append(title);
	    s.Append(title2);
	    TH2F* h1 = ((TH2F*)(pHistArray->FindObject(s.Data())));
	    if (h1) h1->Fill(track1->getRichTheta(),
			     track1->getRadius());
	}
	
	if (kNewIndex2)
	{
	    TString s(HRichHistFac::getHRingRadiusThetaBasename());
	    s.Append(title);
	    s.Append(title2);
	    TH2F* h1 = ((TH2F*)(pHistArray->FindObject(s.Data())));
	    if (h1) h1->Fill(track2->getRichTheta(),
			     track2->getRadius());
	}
    }
    return ret_val;
}

Int_t HRichPairCreator::fillRingLocMax4(HDiHitMatch *h,Int_t *tup,Int_t max,Char_t* title)
{
    Int_t ret_val=1;

    // retrieve tracks that formed pair
    HHitMatch *track1 = (HHitMatch*)getHitMatchCat()
	->getObject(h->getIndTrk1());
    HHitMatch *track2 = (HHitMatch*)getHitMatchCat()
	->getObject(h->getIndTrk2());
	
    Int_t *indHitTrk1 = h->getIndHitsTrk1();
    Int_t *indHitTrk2 = h->getIndHitsTrk2();
    Int_t ind1 = indHitTrk1[0];//index for RICH
    Int_t ind2 = indHitTrk2[0];//c.f. hdihitmatch.h
    
    Bool_t kNewIndex1=HRichCut::isNewIndex(ind1,tup,max);    
    Bool_t kNewIndex2=HRichCut::isNewIndex(ind2,tup,max);


    if (kNewIndex1)
    {
	TString s(HRichHistFac::getHRingLocMax4Basename());
	s.Append(title);
	TH1F* h1 = ((TH1F*)(pHistArray->FindObject(s.Data())));
	if (h1&&HRichCut::isRICHBetween(track1,nRichPolarLow,nRichPolarHigh)) h1->Fill(track1->getRingLocalMax4());
    }

    if (kNewIndex2)
    {
	TString s(HRichHistFac::getHRingLocMax4Basename());
	s.Append(title);
	TH1F* h1 = ((TH1F*)(pHistArray->FindObject(s.Data())));
	if (h1&&HRichCut::isRICHBetween(track2,nRichPolarLow,nRichPolarHigh)) h1->Fill(track2->getRingLocalMax4());
    }
    return ret_val;
}

Int_t HRichPairCreator::fillRingLocMax4Theta(HDiHitMatch *h,Int_t *tup,Int_t max,Char_t* title)
{
    Int_t ret_val=1;

    // retrieve tracks that formed pair
    HHitMatch *track1 = (HHitMatch*)getHitMatchCat()
	->getObject(h->getIndTrk1());
    HHitMatch *track2 = (HHitMatch*)getHitMatchCat()
	->getObject(h->getIndTrk2());

    // retrieve indexes of hits that formed respective track
    Int_t *indHitTrk1 = h->getIndHitsTrk1();
    Int_t *indHitTrk2 = h->getIndHitsTrk2();
    Int_t ind1 = indHitTrk1[0];//index for RICH
    Int_t ind2 = indHitTrk2[0];//c.f. hdihitmatch.h
    
    Bool_t kNewIndex1=HRichCut::isNewIndex(ind1,tup,max);    
    Bool_t kNewIndex2=HRichCut::isNewIndex(ind2,tup,max);


    if (kNewIndex1)
    {
	TString s(HRichHistFac::getHRingLocMax4ThetaBasename());
	s.Append(title);
	TH2F* h1 = ((TH2F*)(pHistArray->FindObject(s.Data())));
	//cout<<track1->getRichTheta()<<" "<<track1->getRingLocMax4()<<endl;
	if (h1) h1->Fill(track1->getRichTheta(),
			 track1->getRingLocalMax4());
    }

    if (kNewIndex2)
    {
	TString s(HRichHistFac::getHRingLocMax4ThetaBasename());
	s.Append(title);
	TH2F* h1 = ((TH2F*)(pHistArray->FindObject(s.Data())));
	if (h1) h1->Fill(track2->getRichTheta(),
			 track2->getRingLocalMax4());
    }
    return ret_val;
}

Int_t HRichPairCreator::fillRingLocMax4ThetaSec(HDiHitMatch *h,Int_t *tup,Int_t max,Char_t* title,Char_t *title2,Int_t sec1, Int_t sec2)
{
    Int_t ret_val=1;

    // retrieve tracks that formed pair
    HHitMatch *track1 = (HHitMatch*)getHitMatchCat()
	->getObject(h->getIndTrk1());
    HHitMatch *track2 = (HHitMatch*)getHitMatchCat()
	->getObject(h->getIndTrk2());
	
    Int_t *indHitTrk1 = h->getIndHitsTrk1();
    Int_t *indHitTrk2 = h->getIndHitsTrk2();
    Int_t ind1 = indHitTrk1[0];//index for RICH
    Int_t ind2 = indHitTrk2[0];//c.f. hdihitmatch.h
    
    if ( (track1->getSector()==sec1 || track1->getSector()==sec2) &&
	 (track2->getSector()==sec1 || track2->getSector()==sec2)    )
    {

	Bool_t kNewIndex1=HRichCut::isNewIndex(ind1,tup,max);    
	Bool_t kNewIndex2=HRichCut::isNewIndex(ind2,tup,max);


	if (kNewIndex1)
	{
	    TString s(HRichHistFac::getHRingLocMax4ThetaBasename());
	    s.Append(title);
	    s.Append(title2);
	    TH2F* h1 = ((TH2F*)(pHistArray->FindObject(s.Data())));
	    if (h1) h1->Fill(track1->getRichTheta(),
			     track1->getRingLocalMax4());
	}
	
	if (kNewIndex2)
	{
	    TString s(HRichHistFac::getHRingLocMax4ThetaBasename());
	    s.Append(title);
	    s.Append(title2);
	    TH2F* h1 = ((TH2F*)(pHistArray->FindObject(s.Data())));
	    if (h1) h1->Fill(track2->getRichTheta(),
			     track2->getRingLocalMax4());
	}
    }
    return ret_val;
}
Int_t HRichPairCreator::fillRingPM(HDiHitMatch *h,Int_t *tup,Int_t max,Char_t* title)
{
    Int_t ret_val=1;

    // retrieve tracks that formed pair
    HHitMatch *track1 = (HHitMatch*)getHitMatchCat()
	->getObject(h->getIndTrk1());
    HHitMatch *track2 = (HHitMatch*)getHitMatchCat()
	->getObject(h->getIndTrk2());
	
    Int_t *indHitTrk1 = h->getIndHitsTrk1();
    Int_t *indHitTrk2 = h->getIndHitsTrk2();
    Int_t ind1 = indHitTrk1[0];//index for RICH
    Int_t ind2 = indHitTrk2[0];//c.f. hdihitmatch.h
    
    Bool_t kNewIndex1=HRichCut::isNewIndex(ind1,tup,max);    
    Bool_t kNewIndex2=HRichCut::isNewIndex(ind2,tup,max);


    if (kNewIndex1)
    {
	TString s(HRichHistFac::getHRingPMBasename());
	s.Append(title);
	TH1F* h1 = ((TH1F*)(pHistArray->FindObject(s.Data())));
	if (h1&&HRichCut::isRICHBetween(track1,nRichPolarLow,nRichPolarHigh)) h1->Fill(track1->getRingPatMat());
    }

    if (kNewIndex2)
    {
	TString s(HRichHistFac::getHRingPMBasename());
	s.Append(title);
	TH1F* h1 = ((TH1F*)(pHistArray->FindObject(s.Data())));
	if (h1&&HRichCut::isRICHBetween(track2,nRichPolarLow,nRichPolarHigh)) h1->Fill(track2->getRingPatMat());
    }
    return ret_val;
}

Int_t HRichPairCreator::fillRingPMTheta(HDiHitMatch *h,Int_t *tup,Int_t max,Char_t* title)
{
    Int_t ret_val=1;

    // retrieve tracks that formed pair
    HHitMatch *track1 = (HHitMatch*)getHitMatchCat()
	->getObject(h->getIndTrk1());
    HHitMatch *track2 = (HHitMatch*)getHitMatchCat()
	->getObject(h->getIndTrk2());

    // retrieve indexes of hits that formed respective track
    Int_t *indHitTrk1 = h->getIndHitsTrk1();
    Int_t *indHitTrk2 = h->getIndHitsTrk2();
    Int_t ind1 = indHitTrk1[0];//index for RICH
    Int_t ind2 = indHitTrk2[0];//c.f. hdihitmatch.h
    
    Bool_t kNewIndex1=HRichCut::isNewIndex(ind1,tup,max);    
    Bool_t kNewIndex2=HRichCut::isNewIndex(ind2,tup,max);


    if (kNewIndex1)
    {
	TString s(HRichHistFac::getHRingPMThetaBasename());
	s.Append(title);
	TH2F* h1 = ((TH2F*)(pHistArray->FindObject(s.Data())));
	//cout<<track1->getRichTheta()<<" "<<track1->getRingAmplitude()<<endl;
	if (h1) h1->Fill(track1->getRichTheta(),
			 track1->getRingPatMat());
    }

    if (kNewIndex2)
    {
	TString s(HRichHistFac::getHRingPMThetaBasename());
	s.Append(title);
	TH2F* h1 = ((TH2F*)(pHistArray->FindObject(s.Data())));
	if (h1) h1->Fill(track2->getRichTheta(),
			 track2->getRingPatMat());
    }
    return ret_val;
}

Int_t HRichPairCreator::fillRingPMThetaSec(HDiHitMatch *h,Int_t *tup,Int_t max,Char_t* title,Char_t *title2,Int_t sec1, Int_t sec2)
{
    Int_t ret_val=1;

    // retrieve tracks that formed pair
    HHitMatch *track1 = (HHitMatch*)getHitMatchCat()
	->getObject(h->getIndTrk1());
    HHitMatch *track2 = (HHitMatch*)getHitMatchCat()
	->getObject(h->getIndTrk2());
	
    Int_t *indHitTrk1 = h->getIndHitsTrk1();
    Int_t *indHitTrk2 = h->getIndHitsTrk2();
    Int_t ind1 = indHitTrk1[0];//index for RICH
    Int_t ind2 = indHitTrk2[0];//c.f. hdihitmatch.h
    
    if ( (track1->getSector()==sec1 || track1->getSector()==sec2) &&
	 (track2->getSector()==sec1 || track2->getSector()==sec2)    )
    {

	Bool_t kNewIndex1=HRichCut::isNewIndex(ind1,tup,max);    
	Bool_t kNewIndex2=HRichCut::isNewIndex(ind2,tup,max);


	if (kNewIndex1)
	{
	    TString s(HRichHistFac::getHRingPMThetaBasename());
	    s.Append(title);
	    s.Append(title2);
	    TH2F* h1 = ((TH2F*)(pHistArray->FindObject(s.Data())));
	    if (h1) h1->Fill(track1->getRichTheta(),
			     track1->getRingPatMat());
	}
	
	if (kNewIndex2)
	{
	    TString s(HRichHistFac::getHRingPMThetaBasename());
	    s.Append(title);
	    s.Append(title2);
	    TH2F* h1 = ((TH2F*)(pHistArray->FindObject(s.Data())));
	    if (h1) h1->Fill(track2->getRichTheta(),
			     track2->getRingPatMat());
	}
    }
    return ret_val;
}
Int_t HRichPairCreator::fillRingHT(HDiHitMatch *h,Int_t *tup,Int_t max,Char_t* title)
{
    Int_t ret_val=1;

    // retrieve tracks that formed pair
    HHitMatch *track1 = (HHitMatch*)getHitMatchCat()
	->getObject(h->getIndTrk1());
    HHitMatch *track2 = (HHitMatch*)getHitMatchCat()
	->getObject(h->getIndTrk2());
	
    Int_t *indHitTrk1 = h->getIndHitsTrk1();
    Int_t *indHitTrk2 = h->getIndHitsTrk2();
    Int_t ind1 = indHitTrk1[0];//index for RICH
    Int_t ind2 = indHitTrk2[0];//c.f. hdihitmatch.h
    
    Bool_t kNewIndex1=HRichCut::isNewIndex(ind1,tup,max);    
    Bool_t kNewIndex2=HRichCut::isNewIndex(ind2,tup,max);


    if (kNewIndex1)
    {
	TString s(HRichHistFac::getHRingHTBasename());
	s.Append(title);
	TH1F* h1 = ((TH1F*)(pHistArray->FindObject(s.Data())));
	if (h1&&HRichCut::isRICHBetween(track1,nRichPolarLow,nRichPolarHigh)) h1->Fill(track1->getRingHouTra());
    }

    if (kNewIndex2)
    {
	TString s(HRichHistFac::getHRingHTBasename());
	s.Append(title);
	TH1F* h1 = ((TH1F*)(pHistArray->FindObject(s.Data())));
	if (h1&&HRichCut::isRICHBetween(track2,nRichPolarLow,nRichPolarHigh)) h1->Fill(track2->getRingHouTra());
    }
    return ret_val;
}

Int_t HRichPairCreator::fillRingHTTheta(HDiHitMatch *h,Int_t *tup,Int_t max,Char_t* title)
{
    Int_t ret_val=1;

    // retrieve tracks that formed pair
    HHitMatch *track1 = (HHitMatch*)getHitMatchCat()
	->getObject(h->getIndTrk1());
    HHitMatch *track2 = (HHitMatch*)getHitMatchCat()
	->getObject(h->getIndTrk2());

    // retrieve indexes of hits that formed respective track
    Int_t *indHitTrk1 = h->getIndHitsTrk1();
    Int_t *indHitTrk2 = h->getIndHitsTrk2();
    Int_t ind1 = indHitTrk1[0];//index for RICH
    Int_t ind2 = indHitTrk2[0];//c.f. hdihitmatch.h
    
    Bool_t kNewIndex1=HRichCut::isNewIndex(ind1,tup,max);    
    Bool_t kNewIndex2=HRichCut::isNewIndex(ind2,tup,max);


    if (kNewIndex1)
    {
	TString s(HRichHistFac::getHRingHTThetaBasename());
	s.Append(title);
	TH2F* h1 = ((TH2F*)(pHistArray->FindObject(s.Data())));
	//cout<<track1->getRichTheta()<<" "<<track1->getRingAmplitude()<<endl;
	if (h1) h1->Fill(track1->getRichTheta(),
			 track1->getRingHouTra());
    }

    if (kNewIndex2)
    {
	TString s(HRichHistFac::getHRingHTThetaBasename());
	s.Append(title);
	TH2F* h1 = ((TH2F*)(pHistArray->FindObject(s.Data())));
	if (h1) h1->Fill(track2->getRichTheta(),
			 track2->getRingHouTra());
    }
    return ret_val;
}

Int_t HRichPairCreator::fillRingHTThetaSec(HDiHitMatch *h,Int_t *tup,Int_t max,Char_t* title,Char_t *title2,Int_t sec1, Int_t sec2)
{
    Int_t ret_val=1;

    // retrieve tracks that formed pair
    HHitMatch *track1 = (HHitMatch*)getHitMatchCat()
	->getObject(h->getIndTrk1());
    HHitMatch *track2 = (HHitMatch*)getHitMatchCat()
	->getObject(h->getIndTrk2());
	
    Int_t *indHitTrk1 = h->getIndHitsTrk1();
    Int_t *indHitTrk2 = h->getIndHitsTrk2();
    Int_t ind1 = indHitTrk1[0];//index for RICH
    Int_t ind2 = indHitTrk2[0];//c.f. hdihitmatch.h
    
    if ( (track1->getSector()==sec1 || track1->getSector()==sec2) &&
	 (track2->getSector()==sec1 || track2->getSector()==sec2)    )
    {

	Bool_t kNewIndex1=HRichCut::isNewIndex(ind1,tup,max);    
	Bool_t kNewIndex2=HRichCut::isNewIndex(ind2,tup,max);


	if (kNewIndex1)
	{
	    TString s(HRichHistFac::getHRingHTThetaBasename());
	    s.Append(title);
	    s.Append(title2);
	    TH2F* h1 = ((TH2F*)(pHistArray->FindObject(s.Data())));
	    if (h1) h1->Fill(track1->getRichTheta(),
			     track1->getRingHouTra());
	}
	
	if (kNewIndex2)
	{
	    TString s(HRichHistFac::getHRingHTThetaBasename());
	    s.Append(title);
	    s.Append(title2);
	    TH2F* h1 = ((TH2F*)(pHistArray->FindObject(s.Data())));
	    if (h1) h1->Fill(track2->getRichTheta(),
			     track2->getRingHouTra());
	}
    }
    return ret_val;
}

Int_t HRichPairCreator::fillOpangMDC(HDiHitMatch *h,Int_t *tup,Int_t max,const char *title)
{
    // cuts on pairs
    
    // opening angle calculated with MDC angle information
    // do not use the same combination twice ! Exclude "self-combinations"
    Int_t ret_val=1;
    Int_t *indHitTrk1 = h->getIndHitsTrk1();
    Int_t *indHitTrk2 = h->getIndHitsTrk2();
    Int_t ind1 = indHitTrk1[1];//index for MDC
    Int_t ind2 = indHitTrk2[1];//c.f. hdihitmatch.h
    Bool_t kNew2Tuple=HRichCut::isNew2Tuple(ind1,ind2,tup,max);
    Float_t opa      =h->getOpangMDC();
    if (opa>0. && kNew2Tuple && HRichCut::isUnlikeSignPair(h))
    {
	TString s(HRichHistFac::getHOpeningAngleBasename());
	s.Append(title);
	TH1F* h1 = ((TH1F*)(pHistArray->FindObject(s.Data())));
	if (h1) h1->Fill(opa);
    }
    return ret_val;
}

Int_t HRichPairCreator::fillOpangKICK(HDiHitMatch *h,Int_t *tup,Int_t max,const char *title)
{
    // cuts on pairs
    
    // opening angle calculated with KICKTRACK angle information
    // do not use the same combination twice ! Exclude "self-combinations"
    Int_t ret_val=1;
    Int_t *indHitTrk1 = h->getIndHitsTrk1();
    Int_t *indHitTrk2 = h->getIndHitsTrk2();
    Int_t ind1 = indHitTrk1[4];//index for KICK
    Int_t ind2 = indHitTrk2[4];//c.f. hdihitmatch.h
    Bool_t kNew2Tuple=HRichCut::isNew2Tuple(ind1,ind2,tup,max);
    Float_t opa      =h->getOpangKICK();
    if (opa>0. && kNew2Tuple && HRichCut::isUnlikeSignPair(h))
    {
	TString s(HRichHistFac::getHOpeningAngleBasename());
	s.Append(title);
	TH1F* h1 = ((TH1F*)(pHistArray->FindObject(s.Data())));
	if (h1) h1->Fill(opa);
    }
    return ret_val;
}

Int_t HRichPairCreator::fillOpangMETA(HDiHitMatch *h,Int_t *tup,Int_t max)
{
    // cuts on pairs
    
    // opening angle calculated with META angle information
    // do not use the same combination twice ! Exclude "self-combinations"
    Int_t ret_val=1;
    Int_t *indHitTrk1 = h->getIndHitsTrk1();
    Int_t *indHitTrk2 = h->getIndHitsTrk2();
    Int_t indtof1 = indHitTrk1[2];//index for TOF
    Int_t indtof2 = indHitTrk2[2];//c.f. hdihitmatch.h
    Int_t indshow1 = indHitTrk1[3];//index for SHOWER
    Int_t indshow2 = indHitTrk2[3];//c.f. hdihitmatch.h
    Int_t ind1,ind2;
    if (indtof1>-1) ind1=indtof1;
    else if (indshow1>-1) ind1=indshow1;
    else Error("fillOpangMETA","no first META index");
    if (indtof2>-1) ind2=indtof2;
    else if (indshow2>-1) ind2=indshow2;
    else Error("fillOpangMETA","no second META index");

    Bool_t kNew2Tuple=HRichCut::isNew2Tuple(ind1,ind2,tup,max);
    Float_t opa      =h->getOpangMETA();
    if (opa>0. && kNew2Tuple && HRichCut::isUnlikeSignPair(h))
    {
	TString s(HRichHistFac::getHOpeningAngleBasename());
	s.Append("META");
	TH1F* h1 = ((TH1F*)(pHistArray->FindObject(s.Data())));
	if (h1) h1->Fill(opa);
    }
    return ret_val;
}

Int_t HRichPairCreator::fillInvMass(HDiHitMatch *h,Int_t *tup,Int_t max)
{
    // this function retrieves the invariant mass from a pair
    // and books it in a standard histogram from the histogram factory
    Int_t ret_val=1;
    
    Int_t *indHitTrk1 = h->getIndHitsTrk1();
    Int_t *indHitTrk2 = h->getIndHitsTrk2();
    Int_t ind1 = indHitTrk1[4];//index for KICK
    Int_t ind2 = indHitTrk2[4];//c.f. hdihitmatch.h
    Bool_t kNew2Tuple    =HRichCut::isNew2Tuple(ind1,ind2,tup,max);
    Float_t invmass      =h->getInvMass();
    //-- use this to check consistency and to select like-sign / unlike-sign pairs
    Int_t corrchrg       =h->getCharge();
    Int_t kickchrg       =h->getKICKCharge();
	
    if (invmass>0. && kNew2Tuple)
    {
	TString s(HRichHistFac::getHInvMassBasename());
	s.Append("All");
	TH1F* h1 = ((TH1F*)(pHistArray->FindObject(s.Data())));
	if (h1) h1->Fill(invmass);
    }

    if ((corrchrg==2 || corrchrg==-2) && corrchrg==kickchrg && 
	invmass>0. && kNew2Tuple)
    {
	TString s(HRichHistFac::getHInvMassBasename());
	s.Append("Like_sign");
	TH1F* h1 = ((TH1F*)(pHistArray->FindObject(s.Data())));
	if (h1) h1->Fill(invmass);
    }
    
    
    if (corrchrg==0 && corrchrg==kickchrg &&
	invmass>0. && kNew2Tuple)
    {
	TString s(HRichHistFac::getHInvMassBasename());
	s.Append("Unlike_sign");
	TH1F* h1 = ((TH1F*)(pHistArray->FindObject(s.Data())));
	if (h1) h1->Fill(invmass);
    }

    return ret_val;
}

Int_t HRichPairCreator::fillTof(HDiHitMatch *h,Int_t *tup1,
				Int_t *tup2,Int_t max)
{
    // this function retrieves the time of flight from the two 
    // tracks  forming a pair
    // and books it in a standard histogram from the histogram factory
    Int_t ret_val=1;

    // retrieve tracks that formed pair
    HHitMatch *track1 = (HHitMatch*)getHitMatchCat()
	->getObject(h->getIndTrk1());
    HHitMatch *track2 = (HHitMatch*)getHitMatchCat()
	->getObject(h->getIndTrk2());

    Int_t *indHitTrk1 = h->getIndHitsTrk1();
    Int_t *indHitTrk2 = h->getIndHitsTrk2();
    Int_t indtof1 = indHitTrk1[2];//index for TOF
    Int_t indtof2 = indHitTrk2[2];//c.f. hdihitmatch.h
    Int_t indshow1 = indHitTrk1[3];//index for SHOWER
    Int_t indshow2 = indHitTrk2[3];//c.f. hdihitmatch.h

    TString s(HRichHistFac::getHTofBasename());
    s.Append("AllPairs");
    TH1F* h1 = ((TH1F*)(pHistArray->FindObject(s.Data())));
    
    if (indtof1>-1 && HRichCut::isNewIndex(indtof1,tup1,max) ) 
    {

	if (h1) h1->Fill(track1->getTofTof());

    }
    else if (indshow1>-1 && HRichCut::isNewIndex(indshow1,tup2,max) )
    {

	if (h1) h1->Fill(track1->getTofinoTof());

    } 
    //    else Error("fillTof","no first META index");

    if (indtof2>-1 && HRichCut::isNewIndex(indtof2,tup1,max) ) 
    {

	if (h1) h1->Fill(track2->getTofTof());
	
    }
    else if (indshow2>-1 && HRichCut::isNewIndex(indshow2,tup2,max) )
    {
	
	if (h1) h1->Fill(track2->getTofinoTof());

    } 
    //    else Error("fillTof","no second META index");

    return ret_val;
}

Int_t HRichPairCreator::fillPt_vs_Pt(HDiHitMatch *h,Int_t *tup,Int_t max)
{
    // this function retrieves the transverse momenta from a pair
    // and books it in a standard histogram from the histogram factory
    Int_t ret_val=1;
    
    Int_t *indHitTrk1 = h->getIndHitsTrk1();
    Int_t *indHitTrk2 = h->getIndHitsTrk2();
    Int_t ind1 = indHitTrk1[4];//index for KICK
    Int_t ind2 = indHitTrk2[4];//c.f. hdihitmatch.h
    Bool_t kNew2Tuple    =HRichCut::isNew2Tuple(ind1,ind2,tup,max);
    Float_t pt1          =h->getMomtTrk1();
    Float_t pt2          =h->getMomtTrk2();
    //-- use this to check consistency and to select like-sign / unlike-sign pairs
    Int_t corrchrg       =h->getCharge();
    Int_t kickchrg       =h->getKICKCharge();
    Float_t opa          =h->getOpangKICK();
    
    if (opa>0. && kNew2Tuple)
    {
	//	TString s(HRichHistFac::getHPtvsPtBasename());
	TString s("PtvsPt");
	s.Append("All");
	TH2F* h1 = ((TH2F*)(pHistArray->FindObject(s.Data())));
	if (h1) h1->Fill(pt1,pt2);
    }

    if (opa>0. && (corrchrg==2 || corrchrg==-2) && corrchrg==kickchrg && 
	kNew2Tuple)
    {
	TString s("PtvsPt");
	//TString s(HRichHistFac::getHPtvsPtBasename());
	s.Append("Like_sign");
	TH2F* h1 = ((TH2F*)(pHistArray->FindObject(s.Data())));
	if (h1) h1->Fill(pt1,pt2);
    }
    
    
    if (opa>0. && corrchrg==0 && corrchrg==kickchrg &&
	kNew2Tuple)
    {
	TString s("PtvsPt");
	//TString s(HRichHistFac::getHPtvsPtBasename());
	s.Append("Unlike_sign");
	TH2F* h1 = ((TH2F*)(pHistArray->FindObject(s.Data())));
	if (h1) h1->Fill(pt1,pt2);
    }

    return ret_val;
    
}

Int_t HRichPairCreator::fillPt_vs_Ptot(HDiHitMatch *h,Int_t *tup,Int_t max)
{
    // this function retrieves the transverse momenta from a pair
    // and books it in a standard histogram from the histogram factory
    Int_t ret_val=1;
    
    Int_t *indHitTrk1 = h->getIndHitsTrk1();
    Int_t *indHitTrk2 = h->getIndHitsTrk2();
    Int_t ind1 = indHitTrk1[4];//index for KICK
    Int_t ind2 = indHitTrk2[4];//c.f. hdihitmatch.h
    Bool_t kNew2Tuple    =HRichCut::isNew2Tuple(ind1,ind2,tup,max);
    Float_t pt1          =h->getMomtTrk1();
    Float_t pt2          =h->getMomtTrk2();
    Float_t p1           =h->getMomTrk1();
    Float_t p2           =h->getMomTrk2();
    //-- use this to check consistency and to select like-sign / unlike-sign pairs
    Int_t corrchrg       =h->getCharge();
    Int_t kickchrg       =h->getKICKCharge();
    Float_t opa          =h->getOpangKICK();
    if (opa>0. && kNew2Tuple)
    {
	TString s("PtvsPtot");
	//	TString s(HRichHistFac::getHPtvsPtotBasename());
	s.Append("All");
	TH2F* h1 = ((TH2F*)(pHistArray->FindObject(s.Data())));
	if (h1) h1->Fill(p1,pt1);
	if (h1) h1->Fill(p2,pt2);
    }

    if (opa>0. && (corrchrg==2 || corrchrg==-2) && corrchrg==kickchrg && 
	kNew2Tuple)
    {
	TString s("PtvsPtot");
	//TString s(HRichHistFac::getHPtvsPtotBasename());
	s.Append("Like_sign");
	TH2F* h1 = ((TH2F*)(pHistArray->FindObject(s.Data())));
	if (h1) h1->Fill(p1,pt1);
	if (h1) h1->Fill(p2,pt2);
    }
    
    
    if (opa>0. && corrchrg==0 && corrchrg==kickchrg &&
	kNew2Tuple)
    {
	TString s("PtvsPtot");
	//TString s(HRichHistFac::getHPtvsPtotBasename());
	s.Append("Unlike_sign");
	TH2F* h1 = ((TH2F*)(pHistArray->FindObject(s.Data())));
	if (h1) h1->Fill(p1,pt1);
	if (h1) h1->Fill(p2,pt2);
    }

    return ret_val;
    
}

Int_t HRichPairCreator::fillPtot_vs_Ptot(HDiHitMatch *h,Int_t *tup,Int_t max)
{
    // this function retrieves the transverse momenta from a pair
    // and books it in a standard histogram from the histogram factory
    Int_t ret_val=1;
    
    Int_t *indHitTrk1 = h->getIndHitsTrk1();
    Int_t *indHitTrk2 = h->getIndHitsTrk2();
    Int_t ind1 = indHitTrk1[4];//index for KICK
    Int_t ind2 = indHitTrk2[4];//c.f. hdihitmatch.h
    Bool_t kNew2Tuple    =HRichCut::isNew2Tuple(ind1,ind2,tup,max);
    Float_t p1          =h->getMomTrk1();
    Float_t p2          =h->getMomTrk2();
    //-- use this to check consistency and to select like-sign / unlike-sign pairs
    Int_t corrchrg       =h->getCharge();
    Int_t kickchrg       =h->getKICKCharge();
    Float_t opa          =h->getOpangKICK();
    if (opa>0. && kNew2Tuple)
    {
	TString s("PtotvsPtot");
	//TString s(HRichHistFac::getHPtotvsPtotBasename());
	s.Append("All");
	TH2F* h1 = ((TH2F*)(pHistArray->FindObject(s.Data())));
	if (h1) h1->Fill(p1,p2);
    }

    if (opa>0. && (corrchrg==2 || corrchrg==-2) && corrchrg==kickchrg && 
	kNew2Tuple)
    {
	TString s("PtotvsPtot");
	//TString s(HRichHistFac::getHPtotvsPtotBasename());
	s.Append("Like_sign");
	TH2F* h1 = ((TH2F*)(pHistArray->FindObject(s.Data())));
	if (h1) h1->Fill(p1,p2);
    }
    
    
    if (opa>0. && corrchrg==0 && corrchrg==kickchrg &&
	kNew2Tuple)
    {
	TString s("PtotvsPtot");
	//TString s(HRichHistFac::getHPtotvsPtotBasename());
	s.Append("Unlike_sign");
	TH2F* h1 = ((TH2F*)(pHistArray->FindObject(s.Data())));
	if (h1) h1->Fill(p1,p2);
    }

    return ret_val;
    
}
