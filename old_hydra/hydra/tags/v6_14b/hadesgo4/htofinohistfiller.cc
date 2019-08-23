//*-- AUTHOR : J. Markert

////////////////////////////////////////////////////////////////////////////
// HTofinoHistFiller
//
// Container class for the tof hists
////////////////////////////////////////////////////////////////////////////
//#include <stdlib.h>

#include "htofinohistfiller.h"
#include "hparamlist.h"
#include "hmonhist.h"
#include "htrendhist.h"
#include "hhistarray.h"

#include "tofinodef.h"
#include "htofinoraw.h"
#include "htofinocal.h"
#include "hshowerhittof.h"

#include "hades.h"
#include "hevent.h"
#include "hcategory.h"
#include "hiterator.h"
#include <TList.h>
#include <TArrayI.h>
#include <TString.h>

#define TOFINO_CELL_MAX 4
#define TOFINO_HIT_MAX 30
// Color definitions
TArrayI HTofinoHistFiller::colorsSector;
TArrayI HTofinoHistFiller::colorsModule;
TArrayI HTofinoHistFiller::colorsCuts;

// Hist Pointers
HHistArray*    HTofinoHistFiller::hhits_raw=0;
HMonHist*      HTofinoHistFiller::hprof=0;
HMonHist*      HTofinoHistFiller::hmulttdcall=0;
HMonHist2*     HTofinoHistFiller::hAdc=0;
HMonHist2*     HTofinoHistFiller::hTdc=0;
HHistArray*    HTofinoHistFiller::hTofinoShower=0;

// Hist Addon Pointers
HMonHistAddon* HTofinoHistFiller::hhits_rawA=0;
HMonHistAddon* HTofinoHistFiller::hprofA=0;
HMonHistAddon* HTofinoHistFiller::hmulttdcallA=0;
HMonHistAddon* HTofinoHistFiller::hAdcA=0;
HMonHistAddon* HTofinoHistFiller::hTdcA=0;
HMonHistAddon* HTofinoHistFiller::hTofinoShowerA=0;

ClassImp(HTofinoHistFiller)

HTofinoHistFiller::HTofinoHistFiller(const char* name,const char* title)
    : TNamed(name,title)
{
  colorsSector.Set(6);
  colorsModule.Set(4);
  colorsCuts.Set(4);
}
HTofinoHistFiller::~HTofinoHistFiller()
{
}
void HTofinoHistFiller::fillRawCat(HIterator* rawiter)
{
  Int_t s,c,mult=0,count=0;
  Float_t t;
  HTofinoRaw* raw=0;

  rawiter->Reset();
  TH1* h=hprof->getP();

  while ((raw=(HTofinoRaw*)rawiter->Next())!= 0 &&count<30){
      s=raw->getSector();
      c=raw->getCell();
      t=raw->getTime();

      if(t>0. && t<4096.) {mult++;h->SetBinContent(s*4+c+1,h->GetBinContent(s*4+c+1)+1);}
      count++;
   }
  hmulttdcall->getP()->Fill((Float_t)(mult));

}
void HTofinoHistFiller::fillRaw(HTofinoRaw* raw)
{
  Float_t time=raw->getTime();
  Float_t charge=raw->getCharge();
  Char_t s=raw->getSector();
  Char_t c=raw->getCell();
  if(hhits_raw)hhits_raw->fill(0,s,c);
  if(hAdc)hAdc->getP()->Fill((Axis_t)(s*4+c),(Axis_t)charge);
  if(hTdc)hTdc->getP()->Fill((Axis_t)(s*4+c),(Axis_t)time);
}
void HTofinoHistFiller::fillCal(HTofinoCal* cal)
{
}
void HTofinoHistFiller::fillShowerTofino(HIterator* caliter,HIterator* showTofIter){
	HTofinoCal *pTofCal=NULL;
	HShowerHitTof *pShowHitTof=NULL;
	Int_t nTofSec,nTofCell,nIndex;
	Int_t tofinoSec[TOFINO_HIT_MAX],tofino_cell[TOFINO_HIT_MAX];
	Int_t tofCal[6][4],showTofHit[6][4];
	Int_t nTofino=0;
	caliter->Reset();
	showTofIter->Reset();
	for(Int_t s=0;s<6;s++){
		for(Int_t c=0;c<TOFINO_CELL_MAX;c++){
			tofCal[s][c]=showTofHit[s][c]=-1;
		}
	}
	while ((pTofCal=(HTofinoCal*)caliter->Next())!= 0){	
		nTofSec = pTofCal->getSector();
		nTofCell = pTofCal->getCell();
		nIndex = nTofSec*TOFINO_CELL_MAX+nTofCell;
		tofCal[nTofSec][nTofCell]=nTofCell;
	}
	while (((pShowHitTof=(HShowerHitTof*)showTofIter->Next())!=0)&&
				(nTofino<TOFINO_HIT_MAX)){
		tofinoSec[nTofino]=pShowHitTof->getSector();
		tofino_cell[nTofino]=pShowHitTof->getTofinoCell();
		showTofHit[tofinoSec[nTofino]][tofino_cell[nTofino]]=tofino_cell[nTofino];
		nTofino++;	
	}
	for(Int_t s=0;s<6;s++){
		for(Int_t c=0;c<TOFINO_CELL_MAX;c++){
			nIndex = s*TOFINO_CELL_MAX+c;
			if((tofCal[s][c]>-1)&&(showTofHit[s][c]==-1)){
				showTofHit[s][c]=4;
			}		
		}
		for(Int_t c=0;c<TOFINO_CELL_MAX;c++){
			if((tofCal[s][c]<0)&&(showTofHit[s][c]<0)) continue;
			nIndex = s*TOFINO_CELL_MAX+c;
			if((showTofHit[s][c]!=TOFINO_CELL_MAX)){//Correlated
				hTofinoShower->fill(0,0,nIndex);			
			}		
			if(showTofHit[s][c]==TOFINO_CELL_MAX){ //No correlation
				hTofinoShower->fill(0,1,nIndex);
			}
		}
	}
}
void HTofinoHistFiller::fillTrend()
{
}
void   HTofinoHistFiller::putTofinoParams(HParamList* l,TList* histpool)
{
    // Puts all params of HTofinoHistFiller to the parameter list of
    // HParamList (which ist used by the io);

       Text_t mytemp[500];
       l->add("colorsSector"           ,colorsSector);
       l->add("colorsModule"           ,colorsModule);
       l->add("colorsCuts"             ,colorsCuts  );
       hhits_rawA->getText(mytemp);
       l->add("hhits_raw"              ,mytemp,500);
       hprofA->getText(mytemp);
       l->add("hprof"                  ,mytemp,500);
       hmulttdcallA->getText(mytemp);
       l->add("hmulttdcall"            ,mytemp,500);
       hAdcA->getText(mytemp);
       l->add("hAdc"                   ,mytemp,500);
       hTdcA->getText(mytemp);
       l->add("hTdc"                   ,mytemp,500);
       hTofinoShowerA->getText(mytemp);
       l->add("hTofinoShower"                   ,mytemp,500);
}
Bool_t HTofinoHistFiller::getTofinoParams(HParamList* l,TList* histpool,TList* activeHists)
{
    if(!(l->fill("colorsSector",&colorsSector))) return kFALSE;
    if(!(l->fill("colorsModule",&colorsModule))) return kFALSE;
    if(!(l->fill("colorsCuts"  ,&colorsCuts  ))) return kFALSE;

    Text_t mytemp[500];
    if(!(l->fill("hhits_raw",mytemp,500))) {return kFALSE;}
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
    }
    if(!(l->fill("hprof",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(hprofA=new HMonHistAddon(mytemp));
	if(hprofA->getActive()==1)
	{
	    activeHists->Add(hprof=new HMonHist(*hprofA));
	    hprof->getP()->SetNdivisions(6);
	}
    }
    if(!(l->fill("hmulttdcall",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(hmulttdcallA=new HMonHistAddon(mytemp));
	if(hmulttdcallA->getActive()==1)
	{
	    activeHists->Add(hmulttdcall=new HMonHist(*hmulttdcallA));
	}
    }
    if(!(l->fill("hAdc",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(hAdcA=new HMonHistAddon(mytemp));
	if(hAdcA->getActive()==1)
	{
	    activeHists->Add(hAdc=new HMonHist2(*hAdcA));
	}
    }
    if(!(l->fill("hTdc",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(hTdcA=new HMonHistAddon(mytemp));
	if(hTdcA->getActive()==1)
	{
	    activeHists->Add(hTdc=new HMonHist2(*hTdcA));
	}
    }
	if(!(l->fill("hTofinoShower",mytemp,500))) {return kFALSE;}
	else{
		histpool->Add(hTofinoShowerA=new HMonHistAddon(mytemp));
		if(hTofinoShowerA->getActive()==1){
			activeHists->Add(hTofinoShower=new HHistArray(*hTofinoShowerA));	
			hTofinoShower->getP(0,0)->SetFillColor(38);
			hTofinoShower->getP(0,1)->SetFillColor(46);
		}
	}
    return kTRUE;
}
