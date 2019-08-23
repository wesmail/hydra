//*-- AUTHOR : B. Spruck

//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////////
// HHodoHistFiller
//
// Container class for the hodo hists
////////////////////////////////////////////////////////////////////////////
//#include <stdlib.h>

#include "hhodohistfiller.h"
#include "hparamlist.h"
#include "hmonhist.h"
#include "htrendhist.h"
#include "hhistarray.h"

#include "hododef.h"
#include "hhodoraw.h"
#include "hhodocal.h"
#include "hhodohit.h"

#include "hades.h"
#include "hevent.h"
#include "hcategory.h"
#include "hiterator.h"
#include <TList.h>
#include <TArrayI.h>
#include <TString.h>

#define HODO_CELL_MAX 4
#define HODO_HIT_MAX 30
// Color definitions
TArrayI HHodoHistFiller::colorsSector;
TArrayI HHodoHistFiller::colorsModule;
TArrayI HHodoHistFiller::colorsCuts;

// Hist Pointers
// HHistArray*    HHodoHistFiller::hhits_raw=0;
// HMonHist*      HHodoHistFiller::hmulttdcall=0;
HHistArray*    HHodoHistFiller::hProf1=0;
HHistArray*    HHodoHistFiller::hProf2=0;
HHistArray*    HHodoHistFiller::hProf3=0;
HHistArray*    HHodoHistFiller::hProf4=0;
HMonHist2*     HHodoHistFiller::hRawTime1=0;
HMonHist2*     HHodoHistFiller::hRawTime2=0;
HMonHist2*     HHodoHistFiller::hRawTime3=0;
HMonHist2*     HHodoHistFiller::hRawTime4=0;
HMonHist2*     HHodoHistFiller::hRawWidth1=0;
HMonHist2*     HHodoHistFiller::hRawWidth2=0;
HMonHist2*     HHodoHistFiller::hRawWidth3=0;
HMonHist2*     HHodoHistFiller::hRawWidth4=0;
HMonHist2*     HHodoHistFiller::hCrT1=0;
HMonHist2*     HHodoHistFiller::hCrT2=0;
HMonHist2*     HHodoHistFiller::hCrT3=0;
HMonHist2*     HHodoHistFiller::hCrT4=0;
HMonHist2*     HHodoHistFiller::hFib1Mult=0;
HMonHist2*     HHodoHistFiller::hFib2Mult=0;
HMonHist2*     HHodoHistFiller::hFib3Mult=0;
HMonHist2*     HHodoHistFiller::hFib4Mult=0;

HMonHist2*     HHodoHistFiller::hSpot=0;
HMonHist2*     HHodoHistFiller::hTDCMult=0;
// HMonHist2*     HHodoHistFiller::hhodomult_raw=0;
// HMonHist2*     HHodoHistFiller::hhodomulttemp_raw=0;
// HTrendArray*   HHodoHistFiller::hhodomulttrend_raw=0;

// Hist Addon Pointers
// HMonHistAddon* HHodoHistFiller::hhits_rawA=0;
// HMonHistAddon* HHodoHistFiller::hmulttdcallA=0;
HMonHistAddon* HHodoHistFiller::hProf1A=0;
HMonHistAddon* HHodoHistFiller::hProf2A=0;
HMonHistAddon* HHodoHistFiller::hProf3A=0;
HMonHistAddon* HHodoHistFiller::hProf4A=0;
HMonHistAddon* HHodoHistFiller::hRawTime1A=0;
HMonHistAddon* HHodoHistFiller::hRawTime2A=0;
HMonHistAddon* HHodoHistFiller::hRawTime3A=0;
HMonHistAddon* HHodoHistFiller::hRawTime4A=0;
HMonHistAddon* HHodoHistFiller::hRawWidth1A=0;
HMonHistAddon* HHodoHistFiller::hRawWidth2A=0;
HMonHistAddon* HHodoHistFiller::hRawWidth3A=0;
HMonHistAddon* HHodoHistFiller::hRawWidth4A=0;
HMonHistAddon* HHodoHistFiller::hCrT1A=0;
HMonHistAddon* HHodoHistFiller::hCrT2A=0;
HMonHistAddon* HHodoHistFiller::hCrT3A=0;
HMonHistAddon* HHodoHistFiller::hCrT4A=0;
HMonHistAddon* HHodoHistFiller::hFib1MultA=0;
HMonHistAddon* HHodoHistFiller::hFib2MultA=0;
HMonHistAddon* HHodoHistFiller::hFib3MultA=0;
HMonHistAddon* HHodoHistFiller::hFib4MultA=0;
HMonHistAddon* HHodoHistFiller::hSpotA=0;
HMonHistAddon* HHodoHistFiller::hTDCMultA=0;
// HMonHistAddon* HHodoHistFiller::hhodomult_rawA=0;
// HMonHistAddon* HHodoHistFiller::hhodomulttemp_rawA=0;
// HMonHistAddon* HHodoHistFiller::hhodomulttrend_rawA=0;

ClassImp(HHodoHistFiller)

HHodoHistFiller::HHodoHistFiller(const char* name,const char* title)
: TNamed(name,title)
{
	colorsSector.Set(6);
	colorsModule.Set(4);
	colorsCuts.Set(4);
}
HHodoHistFiller::~HHodoHistFiller()
{
}
void HHodoHistFiller::fillRawCat(HIterator* rawiter)
{
#define MAX_HODO_HITS	128
	Int_t m1, m2, m3, m4;
	Int_t mod[MAX_HODO_HITS], fib[MAX_HODO_HITS], cnt;

	HHodoRaw* raw=0;

	rawiter->Reset();
	cnt=0;
	m1=0; m2=0; m3=0; m4=0;

/*	hTDCMult->getP()->Clear();
	hFib1Mult->getP()->Clear();
	hFib2Mult->getP()->Clear();
*/
	while ((raw=(HHodoRaw*)rawiter->Next())!= 0 && cnt<MAX_HODO_HITS)
   {
   	Int_t m, f, nr;
		m=raw->getModule();
		mod[cnt]=m;
		f=raw->getFiber();
		fib[cnt]=f;
		if(m==0 && f<96) m1++;
		if(m==1 && f<96) m2++;
		if(m==2 && f<96) m3++;
		if(m==3 && f<96) m4++;

		if(hTDCMult){
			nr=raw->getNHits();
			hTDCMult->getP()->Fill((Axis_t)(m*128+f),(Axis_t)(nr>10?10:nr));
		}
		cnt++;// overflow check in while above
	}

	for(Int_t i=0; i<cnt; i++){
		if(mod[i]==0){
			if(hFib1Mult) hFib1Mult->getP()->Fill(fib[i],(m1>20?20:m1));
			for(Int_t j=i+1; j<cnt; j++){
				if( mod[j]==0){
					if(hCrT1){
						 hCrT1->getP()->Fill(fib[i],fib[j]);// Symetrisch
						 hCrT1->getP()->Fill(fib[j],fib[i]);
					}
				}else if(mod[j]==1){
//					if(hSpot) hSpot->getP()->Fill(fib[i],fib[j]);// old pion setup
				}
			}
		}else if( mod[i]==1){
			if(hFib2Mult) hFib2Mult->getP()->Fill(fib[i],(m2>20?20:m2));
			for(Int_t j=i+1; j<cnt; j++){
				if(mod[j]==1){
					if(hCrT2){
						 hCrT2->getP()->Fill(fib[i],fib[j]);// Symetrisch
						 hCrT2->getP()->Fill(fib[j],fib[i]);
					}
				}else if(mod[j]==0){
//					if(hSpot) hSpot->getP()->Fill(fib[j],fib[i]);// old pion setup
				}
			}
		}else if( mod[i]==2){
			if(hFib3Mult) hFib3Mult->getP()->Fill(fib[i],(m3>20?20:m3));
			for(Int_t j=i+1; j<cnt; j++){
				if(mod[j]==2){
					if(hCrT3){
						 hCrT3->getP()->Fill(fib[i],fib[j]);// Symetrisch
						 hCrT3->getP()->Fill(fib[j],fib[i]);
					}
				}else if(mod[j]==3){
					if(hSpot) hSpot->getP()->Fill(fib[i],fib[j]);// new: veto x/y
				}
			}
		}else if( mod[i]==3){
			if(hFib4Mult) hFib4Mult->getP()->Fill(fib[i],(m4>20?20:m4));
			for(Int_t j=i+1; j<cnt; j++){
				if(mod[j]==3){
					if(hCrT4){
						 hCrT4->getP()->Fill(fib[i],fib[j]);// Symetrisch
						 hCrT4->getP()->Fill(fib[j],fib[i]);
					}
				}else if(mod[j]==2){
					if(hSpot) hSpot->getP()->Fill(fib[j],fib[i]);// new: veto x/y
				}
			}
		}
	}

	if(hTDCMult){// Multiplicity of TDC Channel... in Percent
		Float_t v=0;
		for(Int_t ii=1; ii<=256; ii++){
			v=hTDCMult->getP()->GetBinContent(ii,2);
			if(v){
				v=100/v;
				for(Int_t i=1; i<=11; i++){
					hTDCMult->getP()->SetBinContent(ii,i,hTDCMult->getP()->GetBinContent(ii,i)*v);
				}
			}
		}
	}
	if(hFib1Mult){// Multiplicity of Fibers... in Percent
		Float_t v=0;
		for(Int_t ii=1; ii<=96; ii++){
			v=hFib1Mult->getP()->GetBinContent(ii,2);
			if(v){
				v=100/v;
				for(Int_t i=1; i<=21; i++){
					hFib1Mult->getP()->SetBinContent(ii,i,hFib1Mult->getP()->GetBinContent(ii,i)*v);
				}
			}
		}
	}
	if(hFib2Mult){// Multiplicity of Fibers... in Percent
		Float_t v=0;
		for(Int_t ii=1; ii<=96; ii++){
			v=hFib2Mult->getP()->GetBinContent(ii,2);
			if(v){
				v=100/v;
				for(Int_t i=1; i<=21; i++){
					hFib2Mult->getP()->SetBinContent(ii,i,hFib2Mult->getP()->GetBinContent(ii,i)*v);
				}
			}
		}
   }
	if(hFib3Mult){// Multiplicity of Fibers... in Percent
		Float_t v=0;
		for(Int_t ii=1; ii<=96; ii++){
			v=hFib3Mult->getP()->GetBinContent(ii,2);
			if(v){
				v=100/v;
				for(Int_t i=1; i<=21; i++){
					hFib3Mult->getP()->SetBinContent(ii,i,hFib3Mult->getP()->GetBinContent(ii,i)*v);
				}
			}
		}
   }
	if(hFib4Mult){// Multiplicity of Fibers... in Percent
		Float_t v=0;
		for(Int_t ii=1; ii<=96; ii++){
			v=hFib4Mult->getP()->GetBinContent(ii,2);
			if(v){
				v=100/v;
				for(Int_t i=1; i<=21; i++){
					hFib4Mult->getP()->SetBinContent(ii,i,hFib4Mult->getP()->GetBinContent(ii,i)*v);
				}
			}
		}
   }
}

void HHodoHistFiller::fillRaw(HHodoRaw* raw)
{
	Int_t nr=raw->getNHits();
	Int_t m=raw->getModule();
	Int_t f=raw->getFiber();

	Float_t time, width;
	
	HHistArray*    hProf=0;
	HMonHist2*     hRawTime=0;
	HMonHist2*     hRawWidth=0;
	
	switch(m){
		case 0:
			hProf=hProf1;
			hRawTime=hRawTime1;
			hRawWidth=hRawWidth1;
			break;
		case 1:
			hProf=hProf2;
			hRawTime=hRawTime2;
			hRawWidth=hRawWidth2;
			break;
		case 2:
			hProf=hProf3;
			hRawTime=hRawTime3;
			hRawWidth=hRawWidth3;
			break;
		case 3:
			hProf=hProf4;
			hRawTime=hRawTime4;
			hRawWidth=hRawWidth4;
			break;
	}
	
	if( hProf){
		hProf->getP(0,0)->Fill(f);
	}
	if(hRawTime){
		for(Int_t i=0; i<nr; i++){
			if(i==4) break;// above has not time
			time=raw->getTime(i+1);
			hRawTime->getP()->Fill((Axis_t)(f),(Axis_t)time);
		}
	}
	if(hRawWidth){
		for(Int_t i=0; i<nr; i++){
			if(i==4) break;// above has not time
			width=raw->getWidth(i+1);
			hRawWidth->getP()->Fill((Axis_t)(f),(Axis_t)width);
		}
	}
}
void HHodoHistFiller::fillCal(HHodoCal* cal)
{
	Int_t nr=cal->getNHits();
	Int_t m=cal->getModule();
	Int_t f=cal->getStrip();

	HHistArray*      hProf=0;
	
	switch(m){
		case 0:
			hProf=hProf1;
			break;
		case 1:
			hProf=hProf2;
			break;
		case 2:
			hProf=hProf3;
			break;
		case 3:
			hProf=hProf4;
			break;
	}
	
	if( nr>0 && hProf){
		hProf->getP(0,1)->Fill(f);
	}
}
void HHodoHistFiller::fillHit(HHodoHit* hit)
{
	Int_t m=hit->getModule();
	Int_t f=hit->getStrip();

	HHistArray*      hProf=0;
	
	switch(m){
		case 0:
			hProf=hProf1;
			break;
		case 1:
			hProf=hProf2;
			break;
		case 2:
			hProf=hProf3;
			break;
		case 3:
			hProf=hProf4;
			break;
	}
	
	if( hProf){
		hProf->getP(0,3)->Fill(f);
	}
}
void HHodoHistFiller::fillTrend()
{
	
/*	for(Int_t s=0;s<6;s++){
		if(hhodomulttemp_raw&&hhodomulttrend_raw){
			hhodomulttrend_raw->fill(0,s,hhodomulttemp_raw->getP()->Integral(1,4,s+1,s+1));
		}
	}
	if(hhodomulttemp_raw)hhodomulttemp_raw->getP()->Reset();
	
*/
}
void   HHodoHistFiller::putHodoParams(HParamList* l,TList* histpool)
{
	// Puts all params of HHodoHistFiller to the parameter list of
	// HParamList (which ist used by the io);
	
	Text_t mytemp[500];
	l->add("colorsSector"           ,colorsSector);
	l->add("colorsModule"           ,colorsModule);
	l->add("colorsCuts"             ,colorsCuts  );
	
	hProf1A->getText(mytemp);
	l->add("hHodoProf1"                  ,mytemp,500);
	hProf2A->getText(mytemp);
	l->add("hHodoProf2"                  ,mytemp,500);
	hProf3A->getText(mytemp);
	l->add("hHodoProf3"                  ,mytemp,500);
	hProf4A->getText(mytemp);
	l->add("hHodoProf4"                  ,mytemp,500);
	
	hRawTime1A->getText(mytemp);
	l->add("hHodoRawTime1"                   ,mytemp,500);
	hRawTime2A->getText(mytemp);
	l->add("hHodoRawTime2"                   ,mytemp,500);
	hRawTime3A->getText(mytemp);
	l->add("hHodoRawTime3"                   ,mytemp,500);
	hRawTime4A->getText(mytemp);
	l->add("hHodoRawTime4"                   ,mytemp,500);
	
	hRawWidth1A->getText(mytemp);
	l->add("hHodoRawWidth1"                   ,mytemp,500);
	hRawWidth2A->getText(mytemp);
	l->add("hHodoRawWidth2"                   ,mytemp,500);
	hRawWidth3A->getText(mytemp);
	l->add("hHodoRawWidth3"                   ,mytemp,500);
	hRawWidth4A->getText(mytemp);
	l->add("hHodoRawWidth4"                   ,mytemp,500);
	
	hCrT1A->getText(mytemp);
	l->add("hHodoCrT1"                   ,mytemp,500);
	hCrT2A->getText(mytemp);
	l->add("hHodoCrT2"                   ,mytemp,500);
	hCrT3A->getText(mytemp);
	l->add("hHodoCrT3"                   ,mytemp,500);
	hCrT4A->getText(mytemp);
	l->add("hHodoCrT4"                   ,mytemp,500);	

	hFib1MultA->getText(mytemp);
	l->add("hHodoFib1Mult"                   ,mytemp,500);
	hFib2MultA->getText(mytemp);
	l->add("hHodoFib2Mult"                   ,mytemp,500);
	hFib3MultA->getText(mytemp);
	l->add("hHodoFib3Mult"                   ,mytemp,500);
	hFib4MultA->getText(mytemp);
	l->add("hHodoFib4Mult"                   ,mytemp,500);

	hSpotA->getText(mytemp);
	l->add("hHodoSpot"                   ,mytemp,500);
	hTDCMultA->getText(mytemp);
	l->add("hHodoTDCMult"                   ,mytemp,500);
}

Bool_t HHodoHistFiller::getHodoParams(HParamList* l,TList* histpool,TList* activeHists)
{
	if(!(l->fill("colorsSector",&colorsSector))) return kFALSE;
	if(!(l->fill("colorsModule",&colorsModule))) return kFALSE;
	if(!(l->fill("colorsCuts"  ,&colorsCuts  ))) return kFALSE;
	
	Text_t mytemp[500];
/*	if(!(l->fill("hHodohits_raw",mytemp,500))) {return kFALSE;}
	else
	{
		histpool->Add(hhits_rawA=new HMonHistAddon(mytemp));
		if(hhits_rawA->getActive()==1)
		{
			activeHists->Add(hhits_raw=new HHistArray(*hhits_rawA));
			hhits_raw->getP(0,0)->SetLineColor(colorsSector[0]);
			hhits_raw->getP(0,1)->SetLineColor(colorsSector[1]);
			hhits_raw->getP(0,2)->SetLineColor(colorsSector[2]);
			hhits_raw->getP(0,3)->SetLineColor(colorsSector[3]);
			hhits_raw->getP(0,4)->SetLineColor(colorsSector[4]);
			hhits_raw->getP(0,5)->SetLineColor(colorsSector[5]);
		}
	}*/
	if(!(l->fill("hHodoProf1",mytemp,500))) {return kFALSE;}
	else
	{
		histpool->Add(hProf1A=new HMonHistAddon(mytemp));
		if(hProf1A->getActive()==1)
		{
			activeHists->Add(hProf1=new HHistArray(*hProf1A));
			hProf1->getP(0,0)->SetFillColor(38);
			hProf1->getP(0,1)->SetFillColor(46);
			hProf1->getP(0,2)->SetFillColor(42);
			hProf1->getP(0,3)->SetFillColor(29);
		}
	}
	if(!(l->fill("hHodoProf2",mytemp,500))) {return kFALSE;}
	else
	{
		histpool->Add(hProf2A=new HMonHistAddon(mytemp));
		if(hProf2A->getActive()==1)
		{
			activeHists->Add(hProf2=new HHistArray(*hProf2A));
			hProf2->getP(0,0)->SetFillColor(38);
			hProf2->getP(0,1)->SetFillColor(46);
			hProf2->getP(0,2)->SetFillColor(42);
			hProf2->getP(0,3)->SetFillColor(29);
		}
	}
	if(!(l->fill("hHodoProf3",mytemp,500))) {return kFALSE;}
	else
	{
		histpool->Add(hProf3A=new HMonHistAddon(mytemp));
		if(hProf3A->getActive()==1)
		{
			activeHists->Add(hProf3=new HHistArray(*hProf3A));
			hProf3->getP(0,0)->SetFillColor(38);
			hProf3->getP(0,1)->SetFillColor(46);
			hProf3->getP(0,2)->SetFillColor(42);
			hProf3->getP(0,3)->SetFillColor(29);
		}
	}
	if(!(l->fill("hHodoProf4",mytemp,500))) {return kFALSE;}
	else
	{
		histpool->Add(hProf4A=new HMonHistAddon(mytemp));
		if(hProf4A->getActive()==1)
		{
			activeHists->Add(hProf4=new HHistArray(*hProf4A));
			hProf4->getP(0,0)->SetFillColor(38);
			hProf4->getP(0,1)->SetFillColor(46);
			hProf4->getP(0,2)->SetFillColor(42);
			hProf4->getP(0,3)->SetFillColor(29);
		}
	}
	if(!(l->fill("hHodoRawTime1",mytemp,500))) {return kFALSE;}
	else
	{
		histpool->Add(hRawTime1A=new HMonHistAddon(mytemp));
		if(hRawTime1A->getActive()==1)
		{
			activeHists->Add(hRawTime1=new HMonHist2(*hRawTime1A));
		}
	}
	if(!(l->fill("hHodoRawTime2",mytemp,500))) {return kFALSE;}
	else
	{
		histpool->Add(hRawTime2A=new HMonHistAddon(mytemp));
		if(hRawTime2A->getActive()==1)
		{
			activeHists->Add(hRawTime2=new HMonHist2(*hRawTime2A));
		}
	}
	if(!(l->fill("hHodoRawTime3",mytemp,500))) {return kFALSE;}
	else
	{
		histpool->Add(hRawTime3A=new HMonHistAddon(mytemp));
		if(hRawTime3A->getActive()==1)
		{
			activeHists->Add(hRawTime3=new HMonHist2(*hRawTime3A));
		}
	}
	if(!(l->fill("hHodoRawTime4",mytemp,500))) {return kFALSE;}
	else
	{
		histpool->Add(hRawTime4A=new HMonHistAddon(mytemp));
		if(hRawTime4A->getActive()==1)
		{
			activeHists->Add(hRawTime4=new HMonHist2(*hRawTime4A));
		}
	}
	if(!(l->fill("hHodoRawWidth1",mytemp,500))) {return kFALSE;}
	else
	{
		histpool->Add(hRawWidth1A=new HMonHistAddon(mytemp));
		if(hRawWidth1A->getActive()==1)
		{
			activeHists->Add(hRawWidth1=new HMonHist2(*hRawWidth1A));
		}
	}
	if(!(l->fill("hHodoRawWidth2",mytemp,500))) {return kFALSE;}
	else
	{
		histpool->Add(hRawWidth2A=new HMonHistAddon(mytemp));
		if(hRawWidth2A->getActive()==1)
		{
			activeHists->Add(hRawWidth2=new HMonHist2(*hRawWidth2A));
		}
	}
	if(!(l->fill("hHodoRawWidth3",mytemp,500))) {return kFALSE;}
	else
	{
		histpool->Add(hRawWidth3A=new HMonHistAddon(mytemp));
		if(hRawWidth3A->getActive()==1)
		{
			activeHists->Add(hRawWidth3=new HMonHist2(*hRawWidth3A));
		}
	}
	if(!(l->fill("hHodoRawWidth4",mytemp,500))) {return kFALSE;}
	else
	{
		histpool->Add(hRawWidth4A=new HMonHistAddon(mytemp));
		if(hRawWidth4A->getActive()==1)
		{
			activeHists->Add(hRawWidth4=new HMonHist2(*hRawWidth4A));
		}
	}
	if(!(l->fill("hHodoCrT1",mytemp,500))) {return kFALSE;}
	else
	{
		histpool->Add(hCrT1A=new HMonHistAddon(mytemp));
		if(hCrT1A->getActive()==1)
		{
			activeHists->Add(hCrT1=new HMonHist2(*hCrT1A));
		}
	}
	if(!(l->fill("hHodoCrT2",mytemp,500))) {return kFALSE;}
	else
	{
		histpool->Add(hCrT2A=new HMonHistAddon(mytemp));
		if(hCrT2A->getActive()==1)
		{
			activeHists->Add(hCrT2=new HMonHist2(*hCrT2A));
		}
	}
	if(!(l->fill("hHodoCrT3",mytemp,500))) {return kFALSE;}
	else
	{
		histpool->Add(hCrT3A=new HMonHistAddon(mytemp));
		if(hCrT3A->getActive()==1)
		{
			activeHists->Add(hCrT3=new HMonHist2(*hCrT3A));
		}
	}
	if(!(l->fill("hHodoCrT4",mytemp,500))) {return kFALSE;}
	else
	{
		histpool->Add(hCrT4A=new HMonHistAddon(mytemp));
		if(hCrT4A->getActive()==1)
		{
			activeHists->Add(hCrT4=new HMonHist2(*hCrT4A));
		}
	}
	if(!(l->fill("hHodoFib1Mult",mytemp,500))) {return kFALSE;}
	else
	{
		histpool->Add(hFib1MultA=new HMonHistAddon(mytemp));
		if(hFib1MultA->getActive()==1)
		{
			activeHists->Add(hFib1Mult=new HMonHist2(*hFib1MultA));
		}
	}
	if(!(l->fill("hHodoFib2Mult",mytemp,500))) {return kFALSE;}
	else
	{
		histpool->Add(hFib2MultA=new HMonHistAddon(mytemp));
		if(hFib2MultA->getActive()==1)
		{
			activeHists->Add(hFib2Mult=new HMonHist2(*hFib2MultA));
		}
	}
	if(!(l->fill("hHodoFib3Mult",mytemp,500))) {return kFALSE;}
	else
	{
		histpool->Add(hFib3MultA=new HMonHistAddon(mytemp));
		if(hFib3MultA->getActive()==1)
		{
			activeHists->Add(hFib3Mult=new HMonHist2(*hFib3MultA));
		}
	}
	if(!(l->fill("hHodoFib4Mult",mytemp,500))) {return kFALSE;}
	else
	{
		histpool->Add(hFib4MultA=new HMonHistAddon(mytemp));
		if(hFib4MultA->getActive()==1)
		{
			activeHists->Add(hFib4Mult=new HMonHist2(*hFib4MultA));
		}
	}
	if(!(l->fill("hHodoSpot",mytemp,500))) {return kFALSE;}
	else
	{
		histpool->Add(hSpotA=new HMonHistAddon(mytemp));
		if(hSpotA->getActive()==1)
		{
			activeHists->Add(hSpot=new HMonHist2(*hSpotA));
		}
	}
	if(!(l->fill("hHodoTDCMult",mytemp,500))) {return kFALSE;}
	else
	{
		histpool->Add(hTDCMultA=new HMonHistAddon(mytemp));
		if(hTDCMultA->getActive()==1)
		{
			activeHists->Add(hTDCMult=new HMonHist2(*hTDCMultA));
		}
	}
/*	if(!(l->fill("hHodohodomulttrend_raw",mytemp,500))) {return kFALSE;}
	else
	{
		histpool->Add(hhodomulttrend_rawA=new HMonHistAddon(mytemp));
		if(hhodomulttrend_rawA->getActive()==1)
		{
			activeHists->Add(hhodomulttrend_raw=new HTrendArray(*hhodomulttrend_rawA));
			for(Int_t s=0;s<6;s++){
				hhodomulttrend_raw->getP(0,s)->SetLineColor(colorsSector[s]);
				hhodomulttrend_raw->getP(0,s)->SetMarkerColor(colorsSector[s]);
			}
		}
	}*/
	return kTRUE;
}
