#include "hrichdiag.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hrichdetector.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hmatrixcatiter.h"
#include "hlocation.h"
#include "hrichraw.h"
#include "hrichcalpar.h"
#include "hrichcal.h"
#include "hdebug.h"
#include "hades.h"
#include "richdef.h"
#include "hrichutilfunc.h"
#include <iostream.h>

ClassImp(HRichDiag)

HRichDiag::HRichDiag(Text_t *name,Text_t *title,char* filename,Int_t n) :
  HReconstructor(name,title)
{
    pCalIter = NULL;
    pHitIter = NULL;
    nCounterProcessedNbEvents = 0;
    pUtil= new HRichUtilFunc();
    pFileOut = new TFile(filename,"RECREATE");
    nEvtNum=n;// ATTENTION ! not larger than 10k !!
}

HRichDiag::HRichDiag()
{
    pCalIter = NULL;
    pHitIter = NULL;
    nCounterProcessedNbEvents = 0;
}


HRichDiag::~HRichDiag(void) {
   if (pCalIter) delete pCalIter;
   if (pHitIter) delete pHitIter;
}


Bool_t HRichDiag::init() {

    HRichDetector *pRichDet = (HRichDetector*)gHades->getSetup()
                                                  ->getDetector("Rich");
    // CAL
    pCalCat=gHades->getCurrentEvent()->getCategory(catRichCal);
    if (!pCalCat) {
      pCalCat=pRichDet->buildCategory(catRichCal);

      if (!pCalCat) return kFALSE;
      else gHades->getCurrentEvent()
                         ->addCategory(catRichCal, pCalCat, "Rich");
    }

    pCalIter = (HMatrixCatIter*)getCalCat()->MakeIterator(); 
    // HIT
    pHitCat=gHades->getCurrentEvent()->getCategory(catRichHit);
    if (!pHitCat) {
      pHitCat=pRichDet->buildCategory(catRichHit);

      if (!pHitCat) return kFALSE;
      else gHades->getCurrentEvent()
                         ->addCategory(catRichHit, pHitCat, "Rich");
    }

    pHitIter = (HIterator*)getHitCat()->MakeIterator(); 

    xbins = new (Float_t*)[6];
    chrgavrg = new (Float_t*)[6];
    padmult  = new (Float_t*)[6];

    for (Int_t i=0;i<6;i++){
	xbins[i] = new Float_t[nEvtNum];
	chrgavrg[i] = new Float_t[nEvtNum];
	padmult[i] = new Float_t[nEvtNum];
    }
    for (Int_t j=0;j<6;j++){
	for (Int_t k=0;k<nEvtNum;k++) xbins[j][k]=k;
    }

    initControlHistos();
    return kTRUE;
}

Bool_t HRichDiag::finalize() {
    TObjArray pGraphArray(12);
    pFileOut->cd();
    for (Int_t i=0;i<6;i++)
    {
	pGraphArray.Add(new TGraph(nEvtNum,xbins[i],chrgavrg[i]));
	pGraphArray.Add(new TGraph(nEvtNum,xbins[i],padmult[i]));
	pGraphArray[i]->Write();
	pGraphArray[i+1]->Write();
    }
    pUtil->saveHistos(pFileOut,pHistArray);
    return kTRUE;
    
}


Int_t HRichDiag::execute()
{
    if (nCounterProcessedNbEvents != 0 &&
	nCounterProcessedNbEvents%50000==0) pUtil->saveHistos(pFileOut,pHistArray);
    if (nCounterProcessedNbEvents != 0 &&
	nCounterProcessedNbEvents%10000==0) 
	{
	    cout<<"********** "
						<<nCounterProcessedNbEvents
		<<" EVTS PROCESSED **********"<<endl;
	}
    //cout<<"PROCESSING RICH EVENT : "<<nCounter<<endl;
    Int_t nPadMultiplicity[6];
    for (Int_t i=0;i<6;i++) nPadMultiplicity[i]=0;
    Float_t fChargeSum[6];
    for (Int_t j=0;j<6;j++) fChargeSum[j]=0.;
    HRichCal *pCal;
    pCalIter->Reset(); 
    while((pCal = (HRichCal *)pCalIter->Next()))
    {
	nPadMultiplicity[pCal->getSector()]++;
	fChargeSum[pCal->getSector()]+=pCal->getCharge();
	
	// cout<<"evt no:"<<pCal->getEventNr()<<" charge:"<<pCal->getCharge()<<" row:"<<pCal->getRow()<<" col:"<<pCal->getCol()<<" sec:"<<pCal->getSector()<<endl;
	

    }//end while over cal

    for (Int_t i=0;i<6;i++) {
	padmult[i][nCounterProcessedNbEvents]=nPadMultiplicity[i];
	if (nPadMultiplicity[i]) 
	    chrgavrg[i][nCounterProcessedNbEvents] =
		fChargeSum[i]/nPadMultiplicity[i];
	else chrgavrg[i][nCounterProcessedNbEvents]=0.;
	((TH2D*)(*pHistArray)[i])->Fill(nPadMultiplicity[i],
				     chrgavrg[i][nCounterProcessedNbEvents]);
    }
    nCounterProcessedNbEvents++;
    return 0;
}

void HRichDiag::initControlHistos()
{
    
    
    pHistArray = new TObjArray(6);
    TH2D* pHistPadMultChrgPerPad1 = new TH2D("Pad_mult_chrg_per_pad1",
					     "Pad_mult_chrg_per_pad1",
					     3000,0,3000,300,0,300);
    pHistArray->Add(pHistPadMultChrgPerPad1);
    TH2D* pHistPadMultChrgPerPad2 = new TH2D("Pad_mult_chrg_per_pad2",
					     "Pad_mult_chrg_per_pad2",
					     3000,0,3000,300,0,300);
    pHistArray->Add(pHistPadMultChrgPerPad2);
    TH2D* pHistPadMultChrgPerPad3 = new TH2D("Pad_mult_chrg_per_pad3",
					     "Pad_mult_chrg_per_pad3",
					     3000,0,3000,300,0,300);
    pHistArray->Add(pHistPadMultChrgPerPad3);
    TH2D* pHistPadMultChrgPerPad4 = new TH2D("Pad_mult_chrg_per_pad4",
					     "Pad_mult_chrg_per_pad4",
					     3000,0,3000,300,0,300);
    pHistArray->Add(pHistPadMultChrgPerPad4);
    TH2D* pHistPadMultChrgPerPad5 = new TH2D("Pad_mult_chrg_per_pad5",
					     "Pad_mult_chrg_per_pad5",
					     3000,0,3000,300,0,300);
    pHistArray->Add(pHistPadMultChrgPerPad5);
    
    TH2D* pHistPadMultChrgPerPad6 = new TH2D("Pad_mult_chrg_per_pad6",
					     "Pad_mult_chrg_per_pad6",
					     3000,0,3000,300,0,300);
    pHistArray->Add(pHistPadMultChrgPerPad6);

}
