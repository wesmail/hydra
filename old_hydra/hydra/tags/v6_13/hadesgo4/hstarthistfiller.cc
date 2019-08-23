//*-- AUTHOR : J. Markert

////////////////////////////////////////////////////////////////////////////
// HStartHistFiller
//
// Container class for the start hists
////////////////////////////////////////////////////////////////////////////
//#include <stdlib.h>

#include "hstarthistfiller.h"
#include "hparamlist.h"
#include "hmonhist.h"
#include "htrendhist.h"
#include "hhistarray.h"

#include "hstartdef.h"
#include "hstartraw.h"
#include "hstartcal.h"
#include "hstarthit.h"

#include "hades.h"
#include "hevent.h"
#include "hcategory.h"
#include "hiterator.h"
#include <TList.h>
#include <TArrayI.h>
#include <TString.h>

// Color definitions
TArrayI HStartHistFiller::colorsModule;
TArrayI HStartHistFiller::colorsStrips;
TArrayI HStartHistFiller::colorsCuts;

// Hist Pointers
HHistArray*      HStartHistFiller::hstrips_start_veto_raw=0;
HHistArray*      HStartHistFiller::hmult_start_veto_raw=0;
HHistArray*      HStartHistFiller::htime_start_raw=0;
HTrendArray*     HStartHistFiller::htime_trend_start=0;
HHistArray*      HStartHistFiller::htime_start_temp=0;
HHistArray*      HStartHistFiller::htime_veto_raw=0;
HMonHist2*       HStartHistFiller::hstart_veto_corr_raw=0;

// Hist Addon Pointers
HMonHistAddon* HStartHistFiller::hstrips_start_veto_rawA=0;
HMonHistAddon* HStartHistFiller::hmult_start_veto_rawA=0;
HMonHistAddon* HStartHistFiller::htime_start_rawA=0;
HMonHistAddon* HStartHistFiller::htime_veto_rawA=0;
HMonHistAddon* HStartHistFiller::hstart_veto_corr_rawA=0;
HMonHistAddon* HStartHistFiller::htime_trend_startA=0;
HMonHistAddon* HStartHistFiller::htime_start_tempA=0;

ClassImp(HStartHistFiller)

HStartHistFiller::HStartHistFiller(const char* name,const char* title)
    : TNamed(name,title)
{
  colorsModule.Set(2);
  colorsStrips.Set(8);
  colorsCuts.Set(4);
}
HStartHistFiller::~HStartHistFiller()
{
}
void HStartHistFiller::fillRaw(HIterator* iterraw)
{
    HStartRaw* raw=0;
    Int_t mod;
    Int_t strip;
    Short_t time;
    Int_t rawmultstart=0;
    Int_t rawmultveto=0;

    while ((raw=(HStartRaw *)iterraw->Next())!=0) {
        raw->getAddress(mod,strip);
	time=raw->getTime();
        if(mod==0)rawmultstart++;
        if(mod==1)rawmultveto++;
	if(hstrips_start_veto_raw)hstrips_start_veto_raw->fill(0,mod,strip);
	if(htime_start_raw && mod==0){
		htime_start_raw  ->fill(0,strip,time);
		htime_start_temp->fill(0,strip,time);
	}
	if(htime_veto_raw  && mod==1)  htime_veto_raw   ->fill(0,strip,time);
    }
    if(hmult_start_veto_raw)
    {
	hmult_start_veto_raw->fill(0,0,rawmultstart);
        hmult_start_veto_raw->fill(0,1,rawmultveto);
    }
    if(hstart_veto_corr_raw)
    {
        iterraw->Reset();
        Int_t vetostrip;
	HIterator* iterveto=iterraw;
	while ((raw=(HStartRaw *)iterraw->Next())!=0) {
	    raw->getAddress(mod,strip);
	    if(mod==0)
	    {
		iterveto->Reset();
		while ((raw=(HStartRaw *)iterveto->Next())!=0) {
		    raw->getAddress(mod,vetostrip);
		    if(mod==1)hstart_veto_corr_raw->getP()->Fill((Axis_t)strip,(Axis_t)vetostrip);
		}
	    }
	}
    }
}
void HStartHistFiller::fillCal(HIterator* itercal)
{
    HStartCal* cal=0;
    while ((cal=(HStartCal *)itercal->Next())!=0) {

    }
}
void HStartHistFiller::fillHit(HIterator* iterhit)
{
    HStartHit* hit=0;
    while ((hit=(HStartHit *)iterhit->Next())!=0) {

    }
}
void HStartHistFiller::fillTrend() {
	for(Int_t s=0;s<8;s++){
		if(htime_start_temp->getP(0,s)){
			htime_trend_start->
					fill(0,s,htime_start_temp->getP(0,s)->GetMean());	
			htime_start_temp->getP(0,s)->Reset();
		}
	}
}
void   HStartHistFiller::putStartParams(HParamList* l,TList* histpool)
{
    // Puts all params of HStartHistFiller to the parameter list of
    // HParamList (which ist used by the io);

       Text_t mytemp[500];
       l->add("colorsModule"           ,colorsModule);
       l->add("colorsStrips"           ,colorsStrips);
       l->add("colorsCuts"             ,colorsCuts  );
       hstrips_start_veto_rawA->getText(mytemp);
       l->add("hstrips_start_veto_raw"            ,mytemp,500);
       hmult_start_veto_rawA->getText(mytemp);
       l->add("hmult_start_veto_raw"              ,mytemp,500);
       htime_start_rawA->getText(mytemp);
       l->add("htime_start_raw"                   ,mytemp,500);
       htime_veto_rawA->getText(mytemp);
       l->add("htime_veto_raw"                    ,mytemp,500);
       hstart_veto_corr_rawA->getText(mytemp);
       l->add("hstart_veto_corr_raw"              ,mytemp,500);
	htime_trend_start->getText(mytemp);
	l->add("htime_trend_start"              ,mytemp,500);
	htime_start_temp->getText(mytemp);
	l->add("htime_start_temp"              ,mytemp,500);
}
Bool_t HStartHistFiller::getStartParams(HParamList* l,TList* histpool,TList* activeHists)
{
    if(!(l->fill("colorsModule",&colorsModule))) return kFALSE;
    if(!(l->fill("colorsStrips",&colorsStrips))) return kFALSE;
    if(!(l->fill("colorsCuts"  ,&colorsCuts  ))) return kFALSE;

    Text_t mytemp[500];
    if(!(l->fill("hstrips_start_veto_raw",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(hstrips_start_veto_rawA=new HMonHistAddon(mytemp));
	if(hstrips_start_veto_rawA->getActive()==1)
	{
	    activeHists->Add(hstrips_start_veto_raw=new HHistArray(*hstrips_start_veto_rawA));
	    hstrips_start_veto_raw->getP(0,0)->SetLineColor(colorsModule[0]);
	    hstrips_start_veto_raw->getP(0,1)->SetLineColor(colorsModule[1]);

	}
    }
    if(!(l->fill("hmult_start_veto_raw",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(hmult_start_veto_rawA=new HMonHistAddon(mytemp));
	if(hmult_start_veto_rawA->getActive()==1)
	{
	    activeHists->Add(hmult_start_veto_raw=new HHistArray(*hmult_start_veto_rawA));
	    hmult_start_veto_raw->getP(0,0)->SetLineColor(colorsModule[0]);
	    hmult_start_veto_raw->getP(0,1)->SetLineColor(colorsModule[1]);

	}
    }
    if(!(l->fill("htime_start_raw",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(htime_start_rawA=new HMonHistAddon(mytemp));
	if(htime_start_rawA->getActive()==1)
	{
	    activeHists->Add(htime_start_raw=new HHistArray(*htime_start_rawA));
	    htime_start_raw->getP(0,0)->SetLineColor(colorsStrips[0]);
            htime_start_raw->getP(0,1)->SetLineColor(colorsStrips[1]);
            htime_start_raw->getP(0,2)->SetLineColor(colorsStrips[2]);
            htime_start_raw->getP(0,3)->SetLineColor(colorsStrips[3]);
            htime_start_raw->getP(0,4)->SetLineColor(colorsStrips[4]);
            htime_start_raw->getP(0,5)->SetLineColor(colorsStrips[5]);
            htime_start_raw->getP(0,6)->SetLineColor(colorsStrips[6]);
            htime_start_raw->getP(0,7)->SetLineColor(colorsStrips[7]);
	}
    }
    if(!(l->fill("htime_veto_raw",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(htime_veto_rawA=new HMonHistAddon(mytemp));
	if(htime_veto_rawA->getActive()==1)
	{
	    activeHists->Add(htime_veto_raw=new HHistArray(*htime_veto_rawA));
	    htime_veto_raw->getP(0,0)->SetLineColor(colorsStrips[0]);
            htime_veto_raw->getP(0,1)->SetLineColor(colorsStrips[1]);
            htime_veto_raw->getP(0,2)->SetLineColor(colorsStrips[2]);
            htime_veto_raw->getP(0,3)->SetLineColor(colorsStrips[3]);
            htime_veto_raw->getP(0,4)->SetLineColor(colorsStrips[4]);
            htime_veto_raw->getP(0,5)->SetLineColor(colorsStrips[5]);
            htime_veto_raw->getP(0,6)->SetLineColor(colorsStrips[6]);
            htime_veto_raw->getP(0,7)->SetLineColor(colorsStrips[7]);
	}
    }
    if(!(l->fill("hstart_veto_corr_raw",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(hstart_veto_corr_rawA=new HMonHistAddon(mytemp));
	if(hstart_veto_corr_rawA->getActive()==1)
	{
	    activeHists->Add(hstart_veto_corr_raw=new HMonHist2(*hstart_veto_corr_rawA));
	}
    }

	if(!(l->fill("htime_trend_start",mytemp,500))) {return kFALSE;}
	else{
		histpool->Add(htime_trend_startA=new HMonHistAddon(mytemp));
		if(htime_trend_startA->getActive()==1){
			activeHists->Add(htime_trend_start= 
					new HTrendArray(*htime_trend_startA));	
			for(Int_t s=0;s<8;s++){
				htime_trend_start->getP(0,s)->SetLineColor(colorsStrips[s]);
			}
		}	
	}
	if(!(l->fill("htime_start_temp",mytemp,500))) {return kFALSE;}
	else{
		histpool->Add(htime_start_tempA=new HMonHistAddon(mytemp));
		if(htime_start_tempA->getActive()==1){
			activeHists->Add(htime_start_temp= 
					new HHistArray(*htime_start_tempA));	
		}	
	}
		
    return kTRUE;
}
