//*-- AUTHOR : J. Markert

//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////////
// HShowerHistFiller
//
// Container class for the shower hists
////////////////////////////////////////////////////////////////////////////
//#include <stdlib.h>

#include "hshowerhistfiller.h"
#include "hparamlist.h"
#include "hmonhist.h"
#include "htrendhist.h"
#include "hhistarray.h"
#include "hshowerconfig.h"

#include "showerdef.h"
#include "hshowerraw.h"
#include "hshowercal.h"
#include "hshowerhit.h"

#include "hades.h"
#include "hevent.h"
#include "hcategory.h"
#include <TList.h>
#include <TArrayI.h>
#include <TString.h>

#define RAW 0
#define CAL 1
#define HIT 2

// Color definitions
TArrayI HShowerHistFiller::colorsSector;
TArrayI HShowerHistFiller::colorsModule;
TArrayI HShowerHistFiller::colorsCuts;

// Hist Pointers
HMonHist2*	HShowerHistFiller::hhitsRaw=0;
HMonHist2*	HShowerHistFiller::hhitsCal=0;
HMonHist2*	HShowerHistFiller::hhitsHit=0;
HMonHist*	HShowerHistFiller::hCalCol=0;
HMonHist*	HShowerHistFiller::hCalRow=0;
HMonHist*	HShowerHistFiller::hRawMultTemp=0;
HTrendHist*	HShowerHistFiller::hRawMultTrend=0;
HMonHist*	HShowerHistFiller::hCalMultTemp=0;
HTrendHist*	HShowerHistFiller::hCalMultTrend=0;
HMonHist*	HShowerHistFiller::hHitMultTemp=0;
HHistArray*	HShowerHistFiller::hSumTemp=0;
HTrendHist*	HShowerHistFiller::hHitMultTrend=0;
HTrendArray*	HShowerHistFiller::hMultTrend=0;
HTrendArray* HShowerHistFiller::hSumTrend_0=0;
HTrendArray* HShowerHistFiller::hSumTrend_1=0;
HTrendArray* HShowerHistFiller::hSumTrend_2=0;
HTrendArray* HShowerHistFiller::hSumTrend_3=0;
HTrendArray* HShowerHistFiller::hSumTrend_4=0;
HTrendArray* HShowerHistFiller::hSumTrend_5=0;
HHistArray*  HShowerHistFiller::hSums_0=0; //!
HHistArray*  HShowerHistFiller::hSums_1=0; //!
HHistArray*  HShowerHistFiller::hSums_2=0; //!
HHistArray*  HShowerHistFiller::hSums_3=0; //!
HHistArray*  HShowerHistFiller::hSums_4=0; //!
HHistArray*  HShowerHistFiller::hSums_5=0; //!
HHistArray*  HShowerHistFiller::hCalRows[3]={0,0,0}; //!
HHistArray*  HShowerHistFiller::hCalCols[3]={0,0,0}; //!
HMonHist2*   HShowerHistFiller::hshowermult_raw=0;
HMonHist2*   HShowerHistFiller::hshowermulttemp_raw=0;
HTrendArray* HShowerHistFiller::hshowermulttrend_raw=0;

// Hist Addon Pointers
HMonHistAddon*	HShowerHistFiller::hhitsRawA=0;
HMonHistAddon*	HShowerHistFiller::hhitsCalA=0;
HMonHistAddon*	HShowerHistFiller::hhitsHitA=0;
HMonHistAddon*	HShowerHistFiller::hCalRowA=0;
HMonHistAddon*	HShowerHistFiller::hCalColA=0;
HMonHistAddon*	HShowerHistFiller::hRawMultTempA=0;
HMonHistAddon*	HShowerHistFiller::hRawMultTrendA=0;
HMonHistAddon*	HShowerHistFiller::hCalMultTempA=0;
HMonHistAddon*	HShowerHistFiller::hCalMultTrendA=0;
HMonHistAddon*	HShowerHistFiller::hHitMultTempA=0;
HMonHistAddon*	HShowerHistFiller::hHitMultTrendA=0;
HMonHistAddon*	HShowerHistFiller::hMultTrendA=0;
HMonHistAddon*	HShowerHistFiller::hSumTempA=0;
HMonHistAddon*	HShowerHistFiller::hSumTrendA_0=0;
HMonHistAddon*	HShowerHistFiller::hSumTrendA_1=0;
HMonHistAddon*	HShowerHistFiller::hSumTrendA_2=0;
HMonHistAddon*	HShowerHistFiller::hSumTrendA_3=0;
HMonHistAddon*	HShowerHistFiller::hSumTrendA_4=0;
HMonHistAddon*	HShowerHistFiller::hSumTrendA_5=0;
HMonHistAddon*	HShowerHistFiller::hSumsA_0=0;
HMonHistAddon*	HShowerHistFiller::hSumsA_1=0;
HMonHistAddon*	HShowerHistFiller::hSumsA_2=0;
HMonHistAddon*	HShowerHistFiller::hSumsA_3=0;
HMonHistAddon*	HShowerHistFiller::hSumsA_4=0;
HMonHistAddon*	HShowerHistFiller::hSumsA_5=0;
HMonHistAddon*	HShowerHistFiller::hCalRowsA[3]={0,0,0};
HMonHistAddon*	HShowerHistFiller::hCalColsA[3]={0,0,0};
HMonHistAddon*  HShowerHistFiller::hshowermult_rawA=0;
HMonHistAddon*  HShowerHistFiller::hshowermulttemp_rawA=0;
HMonHistAddon*  HShowerHistFiller::hshowermulttrend_rawA=0;

ClassImp(HShowerHistFiller)

HShowerHistFiller::HShowerHistFiller(const char* name,const char* title)
    : TNamed(name,title)
{
  colorsSector.Set(6);
  colorsModule.Set(3);
  colorsCuts.Set(4);
  showerconfig = new HShowerConfig("ShowerConfig","ShowerConfig");
	
}
HShowerHistFiller::~HShowerHistFiller()
{
    if(showerconfig)delete showerconfig;
}
void HShowerHistFiller::fillRaw(HShowerRaw* raw)
{
	Int_t loc[4] = {raw->getSector(),raw->getModule(),raw->getRow(),raw->getCol()};
 	//Int_t col     =raw->getCol();
 	//Float_t charge=raw->getCharge();

	showerconfig->setHits(RAW,loc[0]);
	if(hhitsRaw)hhitsRaw->getP()->Fill(loc[0],loc[1]);
	if(hRawMultTemp)
	    hRawMultTemp->getP()->Fill(gHades->getCurrentEvent()->getCategory(catShowerRaw)->getEntries());
	
	if(hshowermult_raw)    hshowermult_raw    ->getP()->Fill(loc[1],loc[0]);
	if(hshowermulttemp_raw)hshowermulttemp_raw->getP()->Fill(loc[1],loc[0]);

}
void HShowerHistFiller::fillCal(HShowerCal* cal){
	
	Int_t loc[4] = {cal->getSector(),cal->getModule(),cal->getRow(),cal->getCol()};
	showerconfig->setHits(CAL,loc[0]);
	switch (loc[1]){
	case 0:
		if(hCalCols[0])hCalCols[0]->getP(0,loc[0])->Fill(loc[3]);
		if(hCalRows[0])hCalRows[0]->getP(0,loc[0])->Fill(loc[2]);
		break;
	case 1:
		if(hCalCols[1])hCalCols[1]->getP(0,loc[0])->Fill(loc[3]);
		if(hCalRows[1])hCalRows[1]->getP(0,loc[0])->Fill(loc[2]);
		break;
	case 2:
		if(hCalCols[2])hCalCols[2]->getP(0,loc[0])->Fill(loc[3]);
		if(hCalRows[2])hCalRows[2]->getP(0,loc[0])->Fill(loc[2]);
		break;
	default: 
		break;

	}	

	if(hhitsCal)
		    hhitsCal->getP()->Fill(loc[0],loc[1]);

	if(hCalCol){
		hCalCol->getP()->Fill(loc[2]+(loc[0]*32)+50);
	}
	if(hCalRow){
		hCalRow->getP()->Fill(loc[3]+(loc[0]*32)+50);
	}
	if(hCalMultTemp)
		hCalMultTemp->getP()->Fill(
				gHades->getCurrentEvent()->getCategory(catShowerCal)->getEntries());

}
void HShowerHistFiller::fillSums(HHistArray* hist,Int_t mod, Float_t *sums){
	 if(sums[mod])
       hist->fill(0,mod,sums[mod]);  
}
void HShowerHistFiller::fillHit(HShowerHit* hit) {
	//Int_t nIndex = hit->getSector()*3 + hit->getModule();
	Float_t sums[3] = {hit->getSum(0),hit->getSum(1),hit->getSum(2)};
	Int_t loc[4] = 	{hit->getSector(),hit->getModule(),hit->getRow(),hit->getCol()};
	if(sums[0])
		hSumTemp->getP(0,loc[0]*3)->Fill(sums[0]);
	if(sums[1])
		hSumTemp->getP(0,loc[0]*3+1)->Fill(sums[1]);
	if(sums[2])
		hSumTemp->getP(0,loc[0]*3+2)->Fill(sums[2]);
	
	
	showerconfig->setHits(HIT,loc[0]);
	switch(loc[0]){
	case 0:
				fillSums(hSums_0,loc[1],sums);
				break;
	case 1:
				fillSums(hSums_1,loc[1],sums);
				break;
	case 2:
				fillSums(hSums_2,loc[1],sums);
				break;
	case 3:
				fillSums(hSums_3,loc[1],sums);
				break;
	case 4:
				fillSums(hSums_4,loc[1],sums);
				break;
	case 5:
				fillSums(hSums_5,loc[1],sums);
				break;
	default:
			break;
	}
	if(hhitsHit)
	        hhitsHit->getP()->Fill(loc[0],loc[1]);

	if(hHitMultTemp)
	    hHitMultTemp->getP()->Fill(
				       gHades->getCurrentEvent()->getCategory(catShowerHit)->getEntries());
}

void HShowerHistFiller::fillTrend(Int_t nEvents){

	if(hRawMultTrend)
		hRawMultTrend->fill(
				    hRawMultTemp->getP()->GetMean(), hRawMultTemp->getP()->GetRMS());
	if(hRawMultTemp)
		hRawMultTemp->getP()->Reset();
	
	if(hCalMultTrend)
		hCalMultTrend->fill(
				    hCalMultTemp->getP()->GetMean(), hCalMultTemp->getP()->GetRMS());
	if(hCalMultTemp)
		hCalMultTemp->getP()->Reset();
	if(hHitMultTrend)
		hHitMultTrend->fill(
				    hHitMultTemp->getP()->GetMean(), hHitMultTemp->getP()->GetRMS());
	if(hHitMultTemp)
		hHitMultTemp->getP()->Reset();
				
	if(hSumTemp &&nEvents==0){
		for(Int_t m=0;m<3;m++){
			hSumTrend_0->fill(0,m,hSumTemp->getP(0,m)->GetMean());
			hSumTrend_1->fill(0,m,hSumTemp->getP(0,m+3)->GetMean());
			hSumTrend_2->fill(0,m,hSumTemp->getP(0,m+6)->GetMean());
			hSumTrend_3->fill(0,m,hSumTemp->getP(0,m+9)->GetMean());
			hSumTrend_4->fill(0,m,hSumTemp->getP(0,m+12)->GetMean());
			hSumTrend_5->fill(0,m,hSumTemp->getP(0,m+15)->GetMean());
			hSumTemp->getP(0,m)->Reset();
			hSumTemp->getP(0,3+m)->Reset();
			hSumTemp->getP(0,6+m)->Reset();
			hSumTemp->getP(0,9+m)->Reset();
			hSumTemp->getP(0,12+m)->Reset();
			hSumTemp->getP(0,15+m)->Reset();
			
		}
	}
	for(Int_t s=0;s<6;s++){
	    if(hshowermulttemp_raw&&hshowermulttrend_raw){
		hshowermulttrend_raw->fill(0,s,hshowermulttemp_raw->getP()->Integral(1,3,s+1,s+1));
	    }
	}
	if(hshowermulttemp_raw)hshowermulttemp_raw->getP()->Reset();

}
void   HShowerHistFiller::putShowerParams(HParamList* l,TList* histpool)
{
    // Puts all params of HShowerHistFiller to the parameter list of
    // HParamList (which ist used by the io);
    char  buf[80];
    Text_t mytemp[500];
    l->add("colorsSector"           ,colorsSector);
    l->add("colorsModule"           ,colorsModule);
    l->add("colorsCuts"             ,colorsCuts  );
    showerconfig->setColorsSector(colorsSector);
    hCalColA->getText(mytemp);
    l->add("hCalCol"               ,mytemp,'s',500);
    hCalRowA->getText(mytemp);
    l->add("hCalRow"               ,mytemp,'s',500);
    hhitsRawA->getText(mytemp);
    l->add("hhitsRaw"              ,mytemp,'s',500);
    hhitsCalA->getText(mytemp);
    l->add("hhitsCal"              ,mytemp,'s',500);
    hhitsHitA->getText(mytemp);
    l->add("hhitsHit"              ,mytemp,'s',500);
    hRawMultTempA->getText(mytemp);
    l->add("hRawMultTemp"          ,mytemp,'s',500);
    hRawMultTrendA->getText(mytemp);
    l->add("hRawMultTrend"         ,mytemp,'s',500);
    hCalMultTempA->getText(mytemp);
    l->add("hCalMultTemp"          ,mytemp,'s',500);
    hCalMultTrendA->getText(mytemp);
    l->add("hCalMultTrend"         ,mytemp,'s',500);
    hHitMultTempA->getText(mytemp);
    l->add("hHitMultTemp"          ,mytemp,'s',500);
    hHitMultTrendA->getText(mytemp);
    l->add("hHitMultTrend"         ,mytemp,'s',500);
    hMultTrendA->getText(mytemp);
    l->add("hMultTrend"            ,mytemp,'s',500);
    hSumsA_0->getText(mytemp);
    l->add("hSums_0"               ,mytemp,'s',500);
    hSumsA_1->getText(mytemp);
    l->add("hSums_1"               ,mytemp,'s',500);
    hSumsA_2->getText(mytemp);
    l->add("hSums_2"               ,mytemp,'s',500);
    hSumsA_3->getText(mytemp);
    l->add("hSums_3"               ,mytemp,'s',500);
    hSumsA_4->getText(mytemp);
    l->add("hSums_4"               ,mytemp,'s',500);
    hSumsA_5->getText(mytemp);
    l->add("hSums_5"               ,mytemp,'s',500);
	hSumTrendA_0->getText(mytemp);
    l->add("hSumTrend_0"           ,mytemp,'s',500);
	hSumTrendA_1->getText(mytemp);
    l->add("hSumTrend_1"           ,mytemp,'s',500);
	hSumTrendA_2->getText(mytemp);
    l->add("hSumTrend_2"           ,mytemp,'s',500);
	hSumTrendA_3->getText(mytemp);
    l->add("hSumTrend_3"           ,mytemp,'s',500);
	hSumTrendA_4->getText(mytemp);
    l->add("hSumTrend_4"           ,mytemp,'s',500);
	hSumTrendA_5->getText(mytemp);
    l->add("hSumTrend_5"           ,mytemp,'s',500);

    hSumTemp->getText(mytemp);
    l->add("hSumTemp",mytemp,500);

    for(Int_t m=0; m<3;m++)
    {
	sprintf(buf,"hCalRows_%d",m);
	hCalRows[m]->getText(mytemp);
	l->add(buf              ,mytemp,'s',500);

	sprintf(buf,"hCalCols_%d",m);
	hCalCols[m]->getText(mytemp);
	l->add(buf              ,mytemp,'s',500);
    }
    hshowermult_rawA->getText(mytemp);
    l->add("hshowermult_raw"        ,mytemp,'s',500);
    hshowermulttemp_rawA->getText(mytemp);
    l->add("hshowermulttemp_raw"    ,mytemp,'s',500);
    hshowermulttrend_rawA->getText(mytemp);
    l->add("hshowermulttrend_raw"   ,mytemp,'s',500);

}

Bool_t HShowerHistFiller::getShowerParams(HParamList* l,TList* histpool,TList* activeHists)
{
    char buf[80];
    if(!(l->fill("colorsSector",&colorsSector))) return kFALSE;
    if(!(l->fill("colorsModule",&colorsModule))) return kFALSE;
    if(!(l->fill("colorsCuts"  ,&colorsCuts  ))) return kFALSE;

    Text_t mytemp[500];
    if(!(l->fill("hhitsRaw",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(hhitsRawA=new HMonHistAddon(mytemp));
	if(hhitsRawA->getActive()==1){
	    activeHists->Add(hhitsRaw=new HMonHist2(*hhitsRawA));
	}
    }
    if(!(l->fill("hhitsCal",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(hhitsCalA=new HMonHistAddon(mytemp));
	if(hhitsCalA->getActive()==1){
	    activeHists->Add(hhitsCal=new HMonHist2(*hhitsCalA));
	}
    }
    if(!(l->fill("hhitsHit",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(hhitsHitA=new HMonHistAddon(mytemp));
	if(hhitsHitA->getActive()==1){
	    activeHists->Add(hhitsHit=new HMonHist2(*hhitsHitA));
	}
    }
    if(!(l->fill("hCalCol",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(hCalColA=new HMonHistAddon(mytemp));
	if(hCalColA->getActive()==1){
	    activeHists->Add(hCalCol=new HMonHist(*hCalColA));
	}
    }
    if(!(l->fill("hCalRow",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(hCalRowA=new HMonHistAddon(mytemp));
	if(hCalRowA->getActive()==1){
	    activeHists->Add(hCalRow=new HMonHist(*hCalRowA));
	}
    }
    if(!(l->fill("hRawMultTrend",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(hRawMultTrendA = new HMonHistAddon(mytemp));
	if(hRawMultTrendA->getActive()==1) {
	    activeHists->Add(hRawMultTrend=new HTrendHist(*hRawMultTrendA));
	}
    }
    if(!(l->fill("hRawMultTemp",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(hRawMultTempA = new HMonHistAddon(mytemp));
	if(hRawMultTempA->getActive()==1) {
	    activeHists->Add(hRawMultTemp = new HMonHist(*hRawMultTempA));
	}
    }
    if(!(l->fill("hCalMultTrend",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(hCalMultTrendA = new HMonHistAddon(mytemp));
	if(hCalMultTrendA->getActive()==1) {
	    activeHists->Add(hCalMultTrend=new HTrendHist(*hCalMultTrendA));
	}
    }
    if(!(l->fill("hCalMultTemp",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(hCalMultTempA = new HMonHistAddon(mytemp));
	if(hCalMultTempA->getActive()==1) {
	    activeHists->Add(hCalMultTemp = new HMonHist(*hCalMultTempA));
	}
    }
    if(!(l->fill("hHitMultTrend",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(hHitMultTrendA = new HMonHistAddon(mytemp));
	if(hHitMultTrendA->getActive()==1) {
	    activeHists->Add(hHitMultTrend=new HTrendHist(*hHitMultTrendA));
	}
    }
    if(!(l->fill("hMultTrend",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(hMultTrendA = new HMonHistAddon(mytemp));
	if(hMultTrendA->getActive()==1) {
	    activeHists->Add(hMultTrend=new HTrendArray(*hMultTrendA));
	}
    }
    if(!(l->fill("hHitMultTemp",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(hHitMultTempA = new HMonHistAddon(mytemp));
	if(hHitMultTempA->getActive()==1) {
	    activeHists->Add(hHitMultTemp = new HMonHist(*hHitMultTempA));
	}
    }
    if(!(l->fill("hSums_0",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(hSumsA_0 = new HMonHistAddon(mytemp));
	if(hSumsA_0->getActive()==1) {
	    activeHists->Add(hSums_0 = new HHistArray(*hSumsA_0));
	    hSums_0->getP(0,0)->SetLineColor(colorsModule[0]);
	    hSums_0->getP(0,1)->SetLineColor(colorsModule[1]);
	    hSums_0->getP(0,2)->SetLineColor(colorsModule[2]);
	}
    }
    if(!(l->fill("hSums_1",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(hSumsA_1 = new HMonHistAddon(mytemp));
	if(hSumsA_1->getActive()==1) {
	    activeHists->Add(hSums_1 = new HHistArray(*hSumsA_1));
	    hSums_1->getP(0,0)->SetLineColor(colorsModule[0]);
	    hSums_1->getP(0,1)->SetLineColor(colorsModule[1]);
	    hSums_1->getP(0,2)->SetLineColor(colorsModule[2]);
	}
    }
    if(!(l->fill("hSums_2",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(hSumsA_2 = new HMonHistAddon(mytemp));
	if(hSumsA_2->getActive()==1) {
	    activeHists->Add(hSums_2 = new HHistArray(*hSumsA_2));
	    hSums_2->getP(0,0)->SetLineColor(colorsModule[0]);
	    hSums_2->getP(0,1)->SetLineColor(colorsModule[1]);
	    hSums_2->getP(0,2)->SetLineColor(colorsModule[2]);
	}
    }
    if(!(l->fill("hSums_3",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(hSumsA_3 = new HMonHistAddon(mytemp));
	if(hSumsA_3->getActive()==1) {
	    activeHists->Add(hSums_3 = new HHistArray(*hSumsA_3));
	    hSums_3->getP(0,0)->SetLineColor(colorsModule[0]);
	    hSums_3->getP(0,1)->SetLineColor(colorsModule[1]);
	    hSums_3->getP(0,2)->SetLineColor(colorsModule[2]);
	}
    }
    if(!(l->fill("hSums_4",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(hSumsA_4 = new HMonHistAddon(mytemp));
	if(hSumsA_4->getActive()==1) {
	    activeHists->Add(hSums_4 = new HHistArray(*hSumsA_4));
	    hSums_4->getP(0,0)->SetLineColor(colorsModule[0]);
	    hSums_4->getP(0,1)->SetLineColor(colorsModule[1]);
	    hSums_4->getP(0,2)->SetLineColor(colorsModule[2]);
	}
    }
    if(!(l->fill("hSums_5",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(hSumsA_5 = new HMonHistAddon(mytemp));
	if(hSumsA_5->getActive()==1) {
	    activeHists->Add(hSums_5 = new HHistArray(*hSumsA_5));
	    hSums_5->getP(0,0)->SetLineColor(colorsModule[0]);
	    hSums_5->getP(0,1)->SetLineColor(colorsModule[1]);
	    hSums_5->getP(0,2)->SetLineColor(colorsModule[2]);
	}
    }
	if(!(l->fill("hSumTrend_0",mytemp,500))) {return kFALSE;}
    else
    {
    histpool->Add(hSumTrendA_0 = new HMonHistAddon(mytemp));
    if(hSumTrendA_0->getActive()==1) {
        activeHists->Add(hSumTrend_0=new HTrendArray(*hSumTrendA_0));
		hSumTrend_0->getP(0,0)->SetLineColor(colorsModule[0]);
		hSumTrend_0->getP(0,2)->SetLineColor(colorsModule[2]);
		hSumTrend_0->getP(0,1)->SetLineColor(colorsModule[1]);
    }
    }

	if(!(l->fill("hSumTrend_1",mytemp,500))) {return kFALSE;}
    else {
    	histpool->Add(hSumTrendA_1 = new HMonHistAddon(mytemp));
    	if(hSumTrendA_0->getActive()==1) {
        	activeHists->Add(hSumTrend_1=new HTrendArray(*hSumTrendA_1));
		hSumTrend_1->getP(0,0)->SetLineColor(colorsModule[0]);
		hSumTrend_1->getP(0,2)->SetLineColor(colorsModule[2]);
		hSumTrend_1->getP(0,1)->SetLineColor(colorsModule[1]);
    	}
    }
	if(!(l->fill("hSumTrend_2",mytemp,500))) {return kFALSE;}
    else {
    	histpool->Add(hSumTrendA_2 = new HMonHistAddon(mytemp));
    	if(hSumTrendA_2->getActive()==1) {
        	activeHists->Add(hSumTrend_2=new HTrendArray(*hSumTrendA_2));
		hSumTrend_2->getP(0,0)->SetLineColor(colorsModule[0]);
		hSumTrend_2->getP(0,2)->SetLineColor(colorsModule[2]);
		hSumTrend_2->getP(0,1)->SetLineColor(colorsModule[1]);
    	}
    }
	if(!(l->fill("hSumTrend_3",mytemp,500))) {return kFALSE;}
    else {
    	histpool->Add(hSumTrendA_3 = new HMonHistAddon(mytemp));
    	if(hSumTrendA_3->getActive()==1) {
        	activeHists->Add(hSumTrend_3=new HTrendArray(*hSumTrendA_3));
		hSumTrend_3->getP(0,0)->SetLineColor(colorsModule[0]);
		hSumTrend_3->getP(0,2)->SetLineColor(colorsModule[2]);
		hSumTrend_3->getP(0,1)->SetLineColor(colorsModule[1]);
    	}
    }
	if(!(l->fill("hSumTrend_4",mytemp,500))) {return kFALSE;}
    else {
    	histpool->Add(hSumTrendA_4 = new HMonHistAddon(mytemp));
    	if(hSumTrendA_0->getActive()==1) {
        	activeHists->Add(hSumTrend_4=new HTrendArray(*hSumTrendA_4));
		hSumTrend_4->getP(0,0)->SetLineColor(colorsModule[0]);
		hSumTrend_4->getP(0,2)->SetLineColor(colorsModule[2]);
		hSumTrend_4->getP(0,1)->SetLineColor(colorsModule[1]);
    	}
    }
	if(!(l->fill("hSumTrend_5",mytemp,500))) {return kFALSE;}
    else {
    	histpool->Add(hSumTrendA_5 = new HMonHistAddon(mytemp));
    	if(hSumTrendA_5->getActive()==1) {
        	activeHists->Add(hSumTrend_5=new HTrendArray(*hSumTrendA_5));
		hSumTrend_5->getP(0,0)->SetLineColor(colorsModule[0]);
		hSumTrend_5->getP(0,2)->SetLineColor(colorsModule[2]);
		hSumTrend_5->getP(0,1)->SetLineColor(colorsModule[1]);
    	}
    }

		if(!(l->fill("hSumTemp",mytemp,500))) {return kFALSE;}
		else {
        	histpool->Add(hSumTempA = new HMonHistAddon(mytemp));
        	if(hSumTempA->getActive()==1) {
        	activeHists->Add(hSumTemp = new HHistArray(*hSumTempA));	
			}
		}

    for(Int_t m=0; m<3; m++ )
    {
	sprintf(buf,"hCalRows_%d",m);
	if(!(l->fill(buf,mytemp,500))) {return kFALSE;}
	else
	{
	    histpool->Add(hCalRowsA[m] = new HMonHistAddon(mytemp));
	    if(hCalRowsA[m]->getActive()==1) {
		activeHists->Add(hCalRows[m] = new HHistArray(*hCalRowsA[m]));
		for(Int_t s =0;s<6;s++){
		    hCalRows[m]->getP(0,s)->SetLineColor(colorsSector[s]);
		}
	    }
	}
	sprintf(buf,"hCalCols_%d",m);
	if(!(l->fill(buf,mytemp,500))) {return kFALSE;}
	else
	{
	    histpool->Add(hCalColsA[m] = new HMonHistAddon(mytemp));
	    if(hCalColsA[m]->getActive()==1) {
		activeHists->Add(hCalCols[m] = new HHistArray(*hCalColsA[m]));
		for(Int_t s =0;s<6;s++){
		    hCalCols[m]->getP(0,s)->SetLineColor(colorsSector[s]);
		}
	    }
	}
    }
    if(!(l->fill("hshowermult_raw",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(hshowermult_rawA=new HMonHistAddon(mytemp));
	if(hshowermult_rawA->getActive()==1)
	{
	    activeHists->Add(hshowermult_raw=new HMonHist2(*hshowermult_rawA));
	}
    }
    if(!(l->fill("hshowermulttemp_raw",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(hshowermulttemp_rawA=new HMonHistAddon(mytemp));
	if(hshowermulttemp_rawA->getActive()==1)
	{
	    activeHists->Add(hshowermulttemp_raw=new HMonHist2(*hshowermulttemp_rawA));
	}
    }
    if(!(l->fill("hshowermulttrend_raw",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(hshowermulttrend_rawA=new HMonHistAddon(mytemp));
	if(hshowermulttrend_rawA->getActive()==1)
	{
	    activeHists->Add(hshowermulttrend_raw=new HTrendArray(*hshowermulttrend_rawA));
	    for(Int_t s=0;s<6;s++){
		hshowermulttrend_raw->getP(0,s)->SetLineColor(colorsSector[s]);
		hshowermulttrend_raw->getP(0,s)->SetMarkerColor(colorsSector[s]);
	    }
	}
    }

    return kTRUE;
}

void HShowerHistFiller::fillShowerConfig(Int_t fCount)
{
    HCategory* catRaw=gHades->getCurrentEvent()->getCategory(catShowerRaw);
    if(catRaw)
    {
	showerconfig->setEvtSeqNb(catRaw->getEntries());
	showerconfig->setEvtNb(fCount);
	showerconfig->setRawMult(catRaw->getEntries());
    }
    HCategory* catCal=gHades->getCurrentEvent()->getCategory(catShowerCal);
    if(catCal)
    {
	showerconfig->setCalMult(catCal->getEntries());
    }
}
void HShowerHistFiller::resetHits(){
    if (showerconfig)
    {
	showerconfig->resetHits(RAW);
	showerconfig->resetHits(CAL);
	showerconfig->resetHits(HIT);
    }
}
