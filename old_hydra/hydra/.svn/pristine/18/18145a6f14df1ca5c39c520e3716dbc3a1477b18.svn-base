//*-- AUTHOR : J. Markert

////////////////////////////////////////////////////////////////////////////
// HRichHistFiller
//
// Container class for the rich hists
////////////////////////////////////////////////////////////////////////////
//#include <stdlib.h>

#include "hrichhistfiller.h"
#include "hparamlist.h"
#include "hmonhist.h"
#include "htrendhist.h"
#include "hhistarray.h"

#include "richdef.h"
#include "hrichraw.h"
#include "hrichcal.h"
#include "hrichhit.h"

#include "hades.h"
#include "hevent.h"
#include "hcategory.h"
#include <TList.h>
#include <TArrayI.h>
#include <TString.h>

// Color definitions
TArrayI HRichHistFiller::colorsSector;
TArrayI HRichHistFiller::colorsCuts;

// Hist Pointers
HHistArray*      HRichHistFiller::hpulseheight=0;
HHistArray*      HRichHistFiller::hpulseheightM=0;

// Hist Addon Pointers
HMonHistAddon* HRichHistFiller::hpulseheightA=0;
HMonHistAddon* HRichHistFiller::hpulseheightMA=0;

ClassImp(HRichHistFiller)

HRichHistFiller::HRichHistFiller(const char* name,const char* title)
    : TNamed(name,title)
{
  colorsSector.Set(6);
  colorsCuts.Set(4);
}
HRichHistFiller::~HRichHistFiller()
{
}
void HRichHistFiller::fillRaw(HRichRaw* raw)
{
        Int_t s=raw->getSector();
        //Int_t r=raw->getRow();
        //Int_t c=raw->getCol();
        Float_t ch=raw->getCharge();
	if(hpulseheight)hpulseheight->fill(0,s,ch);
	if(hpulseheightM)hpulseheightM->fill(0,s,ch);
}
void HRichHistFiller::fillHit(HRichHit* hit)
{
}
void HRichHistFiller::fillCal(HRichCal* cal)
{
}
void HRichHistFiller::fillTrend()
{
}
void   HRichHistFiller::putRichParams(HParamList* l,TList* histpool)
{
    // Puts all params of HRichHistFiller to the parameter list of
    // HParamList (which ist used by the io);

       Text_t mytemp[500];
       l->add("colorsSector"           ,colorsSector);
       l->add("colorsCuts"             ,colorsCuts  );
       hpulseheightA->getText(mytemp);
       l->add("hpulsheight"            ,mytemp,500);
       hpulseheightMA->getText(mytemp);
       l->add("hpulsheightM"           ,mytemp,500);
}
Bool_t HRichHistFiller::getRichParams(HParamList* l,TList* histpool,TList* activeHists)
{
    if(!(l->fill("colorsSector",&colorsSector))) return kFALSE;
    if(!(l->fill("colorsCuts"  ,&colorsCuts  ))) return kFALSE;

    Text_t mytemp[500];
    if(!(l->fill("hpulseheight",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(hpulseheightA=new HMonHistAddon(mytemp));
	if(hpulseheightA->getActive()==1)
	{
	    activeHists->Add(hpulseheight=new HHistArray(*hpulseheightA));
	    hpulseheight->getP(0,0)->SetLineColor(colorsSector[0]);
	    hpulseheight->getP(0,1)->SetLineColor(colorsSector[1]);
	    hpulseheight->getP(0,2)->SetLineColor(colorsSector[2]);
	    hpulseheight->getP(0,3)->SetLineColor(colorsSector[3]);
	    hpulseheight->getP(0,4)->SetLineColor(colorsSector[4]);
	    hpulseheight->getP(0,5)->SetLineColor(colorsSector[5]);
	}
    }
    if(!(l->fill("hpulseheightM",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(hpulseheightMA=new HMonHistAddon(mytemp));
	if(hpulseheightMA->getActive()==1)
	{
	    activeHists->Add(hpulseheightM=new HHistArray(*hpulseheightMA));
	    hpulseheightM->getP(0,0)->SetLineColor(colorsSector[0]);
	    hpulseheightM->getP(0,1)->SetLineColor(colorsSector[1]);
	    hpulseheightM->getP(0,2)->SetLineColor(colorsSector[2]);
	    hpulseheightM->getP(0,3)->SetLineColor(colorsSector[3]);
	    hpulseheightM->getP(0,4)->SetLineColor(colorsSector[4]);
	    hpulseheightM->getP(0,5)->SetLineColor(colorsSector[5]);
	}
    }
    return kTRUE;
}
