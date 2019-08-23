// $Id: hrichpaircreatorsim.cc,v 1.7 2008-09-18 13:05:36 halo Exp $
// Last update by Thomas Eberl: 02/09/25 17:43:00
//
using namespace std;
#include "hrichpaircreatorsim.h"
#include "hdihitmatchsim.h"
#include "hrichcut.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hrichdetector.h"
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
#include "hlinearcategory.h"
#include "hrichutilfunc.h"
#include "hrichhistfac.h"
#include "htrackinfo.h"
#include "TH1.h"
#include "TFile.h"

ClassImp(HRichPairCreatorSim)

HRichPairCreatorSim::HRichPairCreatorSim(Text_t *name,Text_t *title, Char_t* filename) :
  HRichPairCreator(name,title,filename)
{
    pFileName  = filename;// output filename for diagnostic histos
}

HRichPairCreatorSim::HRichPairCreatorSim()
{

}


HRichPairCreatorSim::~HRichPairCreatorSim(void) {

}


Bool_t HRichPairCreatorSim::init() {
        if (gHades) {
	    HEvent *event=gHades->getCurrentEvent();
	    HRuntimeDb *rtdb=gHades->getRuntimeDb();
	    HSpectrometer *spec=gHades->getSetup();
	    if (event && rtdb) {
		HRichDetector *rich = (HRichDetector*)spec->getDetector("Rich");
		if (rich) { 
		    pHitMatchCat=event->getCategory(catMatchHit);
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
		    pHitDiMatchCat=rich->buildLinearCat("HDiHitMatchSim");
		    cout<<"i have built HDiHitMatchSim category"<<endl;
		    if (!pHitDiMatchCat) {
			Error("init","No HIT DIMATCH SIM category defined");
			return kFALSE;
		    }
		    else event->addCategory(catDiMatchHit, pHitDiMatchCat, "Rich");
		}
		
		pIterDiMatchHit = (HIterator*)getHitDiMatchCat()->MakeIterator("native");

	    }
	}


	//	pHistArraySim = new TObjArray(10);
	pHistArray    = new TObjArray(10);
	pFileOut = new TFile(pFileName,"RECREATE");
	initCuts();//for HRichPairCreator
	initHistos();//for HRichPairCreator 
	initHistosSim();
	nCounterProcessedNbEvents=0; //total nb of evts seen by this task
	return kTRUE;
}

void HRichPairCreatorSim::initHistosSim()
{
      
    TH1F* hist=HRichHistFac::getHOpeningAngle("MDC_ConvPairs");
    pHistArray->Add(hist);
    
    hist=HRichHistFac::getHOpeningAngle("MDC_ConvPairs_UNLIKE");
    pHistArray->Add(hist);
    
    hist=HRichHistFac::getHOpeningAngle("KICK_ConvPairs_UNLIKE");
    pHistArray->Add(hist);
   
    hist=HRichHistFac::getHOpeningAngle("MDC_DalitzPairs");
    pHistArray->Add(hist);
      
    hist=HRichHistFac::getHOpeningAngle("MDC_DalitzPairs_UNLIKE");
    pHistArray->Add(hist);

    hist=HRichHistFac::getHOpeningAngle("KICK_DalitzPairs_UNLIKE");
    pHistArray->Add(hist);

}

Bool_t HRichPairCreatorSim::finalize() {
    HRichPairCreator::finalize();
    return kTRUE;
}


Int_t HRichPairCreatorSim::execute()
{
    if (!HRichPairCreator::execute())
    {
//  	if (nCounterProcessedNbEvents != 0 &&
//  	    nCounterProcessedNbEvents%5000==0) HRichUtilFunc::saveHistos(pFileOut,pHistArray);
	if(!selectAndFillPairsSim()) Error("","");;
    }
    Int_t kReturnValue=0;
    return kReturnValue;
    
}

Int_t HRichPairCreatorSim::selectAndFillPairsSim(void)
{

    Int_t ret_val=1;
    // select pairs according to the GEANT information
    // of their tracks

    // declare different kinds of pairs
    TObjArray* GEANTconvpairs = new TObjArray(5);
    TObjArray* GEANTdalitzpairs = new TObjArray(5);
//      TObjArray* 1ringpairs = new TObjArray(5);
//      TObjArray* 2ringpairs = new TObjArray(5);
    // ----

    // loop over category containing pairs and select them in temp arrays
    HDiHitMatchSim *pair=0;
    pIterDiMatchHit->Reset();

    while((pair = (HDiHitMatchSim *)pIterDiMatchHit->Next()))
    {
	if(HRichCut::isGEANTPair(pair,(HLinearCategory*)getHitMatchCat(),
				 "Conversion"))
	{
	    GEANTconvpairs->Add(pair);
	}
	if(HRichCut::isGEANTPair(pair,(HLinearCategory*)getHitMatchCat(),
				 "pi0Dalitz"))
	{
	    GEANTdalitzpairs->Add(pair);
	}

	// ---
//  	if(HRichCut::NbRingsPerMDCPair(pair)==1 &&
//  	   ) 1ringpairs->Add(pair);
//  	if(HRichCut::NbRingsPerMDCPair(pair)==2) 2ringpairs->Add(pair);
    }
    // fill selected pairs in respective histograms
    fillHistosGEANTPairs(GEANTconvpairs,"MDC_ConvPairs",
			       "MDC_ConvPairs_UNLIKE","KICK_ConvPairs_UNLIKE");
    fillHistosGEANTPairs(GEANTdalitzpairs,"MDC_DalitzPairs",
			       "MDC_DalitzPairs_UNLIKE","KICK_DalitzPairs_UNLIKE");
    //fillHistosRingProperties(1ringpairs,"1ringPerPair");
    //fillHistosRingProperties(2ringpairs,"2ringsPerPair");
    delete GEANTconvpairs;
    delete GEANTdalitzpairs;
    //delete 1ringpairs;
    //delete 2ringpairs;
    return ret_val;
}



Int_t HRichPairCreatorSim::fillHistosGEANTPairs(TObjArray* arr,Char_t* c1,
						    Char_t* c2,Char_t* c3)
{
    // check for doubles with respect to a object property
    // before booking in histo
    Int_t ret_val=1;
    Int_t max = arr->GetLast()+1;
    Int_t *MDCoptuple=new Int_t[max];
    Int_t *MDCopUNLIKEtuple=new Int_t[max];
    Int_t *KICKopUNLIKEtuple=new Int_t[max];
    for (Int_t i=0;i<max;i++) MDCoptuple[i]=MDCopUNLIKEtuple[i]=
				  KICKopUNLIKEtuple[i]=-2;

    for (Int_t i=0;i<max;i++)
    {
	HDiHitMatchSim *pair = ((HDiHitMatchSim*)(*arr)[i]);

	fillOpangMDC(pair,MDCoptuple,max,c1);//check for doubles

	if(HRichCut::isUnlikeSignPair(pair)) fillOpangMDC(pair,MDCopUNLIKEtuple,
						   max,c2);
	if(HRichCut::isUnlikeSignPair(pair)) fillOpangKICK(pair,KICKopUNLIKEtuple,
						   max,c3);
    }

    delete MDCoptuple;
    delete MDCopUNLIKEtuple;
    delete KICKopUNLIKEtuple;
    return ret_val;
}


//  Int_t HRichPairCreatorSim::fillOpangMDCSim(HDiHitMatch *h,Int_t *tup,Int_t max,Char_t *title)
//  {
//      // cuts on pairs
    
//      // opening angle calculated with MDC angle information
//      // do not use the same combination twice ! Exclude "self-combinations"
//      Int_t ret_val=1;
//      Int_t *indHitTrk1 = h->getIndHitsTrk1();
//      Int_t *indHitTrk2 = h->getIndHitsTrk2();
//      Int_t ind1 = indHitTrk1[1];//index for MDC
//      Int_t ind2 = indHitTrk2[1];//c.f. hdihitmatch.h
//      Bool_t kNew2Tuple=HRichCut::isNew2Tuple(ind1,ind2,tup,max);
//      Float_t opa      =h->getOpangMDC();
//      if (opa>0. && kNew2Tuple)
//      {
//  	//h->dumpToStdout();
//  	TString s(HRichHistFac::getHOpeningAngleBasename());
//  	s.Append(title);
//  	TH1F* h1 = ((TH1F*)(pHistArray->FindObject(s.Data())));
//  	if (h1) h1->Fill(opa);
//      }
//      return ret_val;
//  }



 
