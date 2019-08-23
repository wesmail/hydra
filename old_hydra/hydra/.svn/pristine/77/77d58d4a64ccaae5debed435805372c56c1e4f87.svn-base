//*-- AUTHOR : B. Spruck

//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////////
// HHodoHistFiller
//
// Container class for the tof hists
////////////////////////////////////////////////////////////////////////////
//#include <stdlib.h>

#include "hhodohistfiller.h"
#include "hparamlist.h"
#include "hmonhist.h"
#include "htrendhist.h"
#include "hhistarray.h"

#include "hododef.h"
#include "hhodoraw.h"

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
HMonHist*      HHodoHistFiller::hProf1=0;
HMonHist*      HHodoHistFiller::hProf2=0;
// HMonHist*      HHodoHistFiller::hmulttdcall=0;
HMonHist2*     HHodoHistFiller::hRawTime=0;
HMonHist2*     HHodoHistFiller::hRawWidth=0;
HMonHist2*     HHodoHistFiller::hSpot=0;
HMonHist2*     HHodoHistFiller::hCrT1=0;
HMonHist2*     HHodoHistFiller::hCrT2=0;
HMonHist2*     HHodoHistFiller::hTDCMult=0;
HMonHist2*     HHodoHistFiller::hFib1Mult=0;
HMonHist2*     HHodoHistFiller::hFib2Mult=0;
// HMonHist2*     HHodoHistFiller::hhodomult_raw=0;
// HMonHist2*     HHodoHistFiller::hhodomulttemp_raw=0;
// HTrendArray*   HHodoHistFiller::hhodomulttrend_raw=0;

// Hist Addon Pointers
// HMonHistAddon* HHodoHistFiller::hhits_rawA=0;
HMonHistAddon* HHodoHistFiller::hProf1A=0;
HMonHistAddon* HHodoHistFiller::hProf2A=0;
// HMonHistAddon* HHodoHistFiller::hmulttdcallA=0;
HMonHistAddon* HHodoHistFiller::hRawTimeA=0;
HMonHistAddon* HHodoHistFiller::hRawWidthA=0;
HMonHistAddon* HHodoHistFiller::hSpotA=0;
HMonHistAddon* HHodoHistFiller::hCrT1A=0;
HMonHistAddon* HHodoHistFiller::hCrT2A=0;
HMonHistAddon* HHodoHistFiller::hTDCMultA=0;
HMonHistAddon* HHodoHistFiller::hFib1MultA=0;
HMonHistAddon* HHodoHistFiller::hFib2MultA=0;
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
	Int_t m1, m2;
	Int_t mod[MAX_HODO_HITS], fib[MAX_HODO_HITS], cnt;

	HHodoRaw* raw=0;

	rawiter->Reset();
	cnt=0;
	m1=0;
	m2=0;

	// Clear these to be sure...
/*	hTDCMult->getP()->Clear();
	hFib1Mult->getP()->Clear();
	hFib2Mult->getP()->Clear();
*/
	while ((raw=(HHodoRaw*)rawiter->Next())!= 0 && cnt<MAX_HODO_HITS)
   {
   	Int_t m,f,nr;
		m=raw->getModule();
		mod[cnt]=m;
		f=raw->getFiber();
		fib[cnt]=f;
		if(m==0 && f<96) m1++;
		if(m==1 && f<96) m2++;

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
				if(mod[j]==1){
					if(hSpot) hSpot->getP()->Fill(fib[i],fib[j]);
				}else if( mod[j]==0){
					if(hCrT1){
						 hCrT1->getP()->Fill(fib[i],fib[j]);// Symetrisch
						 hCrT1->getP()->Fill(fib[j],fib[i]);
					}
				}
			}
		}else if( mod[i]==1){
			if(hFib2Mult) hFib2Mult->getP()->Fill(fib[i],(m2>20?20:m2));
			for(Int_t j=i+1; j<cnt; j++){
				if(mod[j]==0){
					if(hSpot) hSpot->getP()->Fill(fib[j],fib[i]);
				}else if(mod[j]==1){
					if(hCrT2){
						 hCrT2->getP()->Fill(fib[i],fib[j]);// Symetrisch
						 hCrT2->getP()->Fill(fib[j],fib[i]);
					}
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
}

void HHodoHistFiller::fillRaw(HHodoRaw* raw)
{
	Int_t nr=raw->getNHits();
	Int_t m=raw->getModule();
	Int_t f=raw->getFiber();

	Float_t time, width;

	if(m==0 && hProf1){
		hProf1->getP()->Fill(f);
	}
	if(m==1 && hProf2){
		hProf2->getP()->Fill(f);
	}
	if(hRawTime){
		for(Int_t i=0; i<nr; i++){
			if(i==4) break;// above has not time
			time=raw->getTime(i+1);
			hRawTime->getP()->Fill((Axis_t)(m*128+f),(Axis_t)time);
		}
	}
	if(hRawWidth){
		for(Int_t i=0; i<nr; i++){
			if(i==4) break;// above has not time
			width=raw->getWidth(i+1);
			hRawWidth->getP()->Fill((Axis_t)(m*128+f),(Axis_t)width);
		}
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
	l->add("hProf1"                  ,mytemp,500);
	hProf2A->getText(mytemp);
	l->add("hProf2"                  ,mytemp,500);
	hRawTimeA->getText(mytemp);
	l->add("hRawTime"                   ,mytemp,500);
	hRawWidthA->getText(mytemp);
	l->add("hRawWidth"                   ,mytemp,500);
	hSpotA->getText(mytemp);
	l->add("hSpot"                   ,mytemp,500);
	hCrT1A->getText(mytemp);
	l->add("hCrT1"                   ,mytemp,500);
	hCrT2A->getText(mytemp);
	l->add("hCrT2"                   ,mytemp,500);
	hTDCMultA->getText(mytemp);
	l->add("hTDCMult"                   ,mytemp,500);
	hFib1MultA->getText(mytemp);
	l->add("hFib1Mult"                   ,mytemp,500);
	hFib2MultA->getText(mytemp);
	l->add("hFib2Mult"                   ,mytemp,500);
}

Bool_t HHodoHistFiller::getHodoParams(HParamList* l,TList* histpool,TList* activeHists)
{
	if(!(l->fill("colorsSector",&colorsSector))) return kFALSE;
	if(!(l->fill("colorsModule",&colorsModule))) return kFALSE;
	if(!(l->fill("colorsCuts"  ,&colorsCuts  ))) return kFALSE;
	
	Text_t mytemp[500];
/*	if(!(l->fill("hhits_raw",mytemp,500))) {return kFALSE;}
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
	if(!(l->fill("hProf1",mytemp,500))) {return kFALSE;}
	else
	{
		histpool->Add(hProf1A=new HMonHistAddon(mytemp));
		if(hProf1A->getActive()==1)
		{
			activeHists->Add(hProf1=new HMonHist(*hProf1A));
		}
	}
	if(!(l->fill("hProf2",mytemp,500))) {return kFALSE;}
	else
	{
		histpool->Add(hProf2A=new HMonHistAddon(mytemp));
		if(hProf2A->getActive()==1)
		{
			activeHists->Add(hProf2=new HMonHist(*hProf2A));
		}
	}
	if(!(l->fill("hRawTime",mytemp,500))) {return kFALSE;}
	else
	{
		histpool->Add(hRawTimeA=new HMonHistAddon(mytemp));
		if(hRawTimeA->getActive()==1)
		{
			activeHists->Add(hRawTime=new HMonHist2(*hRawTimeA));
		}
	}
	if(!(l->fill("hRawWidth",mytemp,500))) {return kFALSE;}
	else
	{
		histpool->Add(hRawWidthA=new HMonHistAddon(mytemp));
		if(hRawWidthA->getActive()==1)
		{
			activeHists->Add(hRawWidth=new HMonHist2(*hRawWidthA));
		}
	}
	if(!(l->fill("hSpot",mytemp,500))) {return kFALSE;}
	else
	{
		histpool->Add(hSpotA=new HMonHistAddon(mytemp));
		if(hSpotA->getActive()==1)
		{
			activeHists->Add(hSpot=new HMonHist2(*hSpotA));
		}
	}
	if(!(l->fill("hCrT1",mytemp,500))) {return kFALSE;}
	else
	{
		histpool->Add(hCrT1A=new HMonHistAddon(mytemp));
		if(hCrT1A->getActive()==1)
		{
			activeHists->Add(hCrT1=new HMonHist2(*hCrT1A));
		}
	}
	if(!(l->fill("hCrT2",mytemp,500))) {return kFALSE;}
	else
	{
		histpool->Add(hCrT2A=new HMonHistAddon(mytemp));
		if(hCrT2A->getActive()==1)
		{
			activeHists->Add(hCrT2=new HMonHist2(*hCrT2A));
		}
	}
	if(!(l->fill("hTDCMult",mytemp,500))) {return kFALSE;}
	else
	{
		histpool->Add(hTDCMultA=new HMonHistAddon(mytemp));
		if(hTDCMultA->getActive()==1)
		{
			activeHists->Add(hTDCMult=new HMonHist2(*hTDCMultA));
		}
	}
	if(!(l->fill("hFib1Mult",mytemp,500))) {return kFALSE;}
	else
	{
		histpool->Add(hFib1MultA=new HMonHistAddon(mytemp));
		if(hFib1MultA->getActive()==1)
		{
			activeHists->Add(hFib1Mult=new HMonHist2(*hFib1MultA));
		}
	}
	if(!(l->fill("hFib2Mult",mytemp,500))) {return kFALSE;}
	else
	{
		histpool->Add(hFib2MultA=new HMonHistAddon(mytemp));
		if(hFib2MultA->getActive()==1)
		{
			activeHists->Add(hFib2Mult=new HMonHist2(*hFib2MultA));
		}
	}
/*	if(!(l->fill("hhodomulttrend_raw",mytemp,500))) {return kFALSE;}
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
