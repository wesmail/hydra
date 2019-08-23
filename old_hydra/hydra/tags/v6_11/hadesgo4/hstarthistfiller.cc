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
#include <TList.h>
#include <TArrayI.h>
#include <TString.h>

// Color definitions
TArrayI HStartHistFiller::colorsModule;
TArrayI HStartHistFiller::colorsCuts;

// Hist Pointers
HHistArray*      HStartHistFiller::hstrips_start_veto_raw=0;
HHistArray*      HStartHistFiller::hstrips_start_veto_rawM=0;

// Hist Addon Pointers
HMonHistAddon* HStartHistFiller::hstrips_start_veto_rawA=0;
HMonHistAddon* HStartHistFiller::hstrips_start_veto_rawMA=0;

ClassImp(HStartHistFiller)

HStartHistFiller::HStartHistFiller(const char* name,const char* title)
    : TNamed(name,title)
{
  colorsModule.Set(2);
  colorsCuts.Set(4);
}
HStartHistFiller::~HStartHistFiller()
{
}
void HStartHistFiller::fillRaw(HStartRaw* raw)
{
    Int_t mod;
    Int_t strip;
    //Short_t time=raw->getTime();
    raw->getAddress(mod,strip);
    if(hstrips_start_veto_raw)hstrips_start_veto_raw->fill(0,mod,strip);
    if(hstrips_start_veto_rawM)hstrips_start_veto_rawM->fill(0,mod,strip);
}
void HStartHistFiller::fillHit(HStartHit* hit)
{
}
void HStartHistFiller::fillCal(HStartCal* cal)
{
}
void HStartHistFiller::fillTrend()
{
}
void   HStartHistFiller::putStartParams(HParamList* l,TList* histpool)
{
    // Puts all params of HStartHistFiller to the parameter list of
    // HParamList (which ist used by the io);

       Text_t mytemp[500];
       l->add("colorsModule"           ,colorsModule);
       l->add("colorsCuts"             ,colorsCuts  );
       hstrips_start_veto_rawA->getText(mytemp);
       l->add("hstrips_start_veto_raw"              ,mytemp,500);
       hstrips_start_veto_rawMA->getText(mytemp);
       l->add("hstrips_start_veto_rawM"              ,mytemp,500);
}
Bool_t HStartHistFiller::getStartParams(HParamList* l,TList* histpool,TList* activeHists)
{
    if(!(l->fill("colorsModule",&colorsModule))) return kFALSE;
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
    if(!(l->fill("hstrips_start_veto_rawM",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(hstrips_start_veto_rawMA=new HMonHistAddon(mytemp));
	if(hstrips_start_veto_rawMA->getActive()==1)
	{
	    activeHists->Add(hstrips_start_veto_rawM=new HHistArray(*hstrips_start_veto_rawMA));
	    hstrips_start_veto_rawM->getP(0,0)->SetLineColor(colorsModule[0]);
	    hstrips_start_veto_rawM->getP(0,1)->SetLineColor(colorsModule[1]);

	}
    }
    return kTRUE;
}
