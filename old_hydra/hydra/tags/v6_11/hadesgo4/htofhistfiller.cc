//*-- AUTHOR : J. Markert

////////////////////////////////////////////////////////////////////////////
// HTofHistFiller
//
// Container class for the tof hists
////////////////////////////////////////////////////////////////////////////
//#include <stdlib.h>

#include "htofhistfiller.h"
#include "hparamlist.h"
#include "hmonhist.h"
#include "htrendhist.h"
#include "hhistarray.h"

#include "tofdef.h"
#include "htofraw.h"
#include "htofcal.h"
#include "htofhit.h"

#include "hades.h"
#include "hevent.h"
#include "hcategory.h"
#include <TList.h>
#include <TArrayI.h>
#include <TString.h>

// Color definitions
TArrayI HTofHistFiller::colorsSector;
TArrayI HTofHistFiller::colorsModule;
TArrayI HTofHistFiller::colorsCuts;

// Hist Pointers
HHistArray*    HTofHistFiller::hhits_raw=0;
HHistArray*    HTofHistFiller::hhits_rawM=0;

// Hist Addon Pointers
HMonHistAddon* HTofHistFiller::hhits_rawA=0;
HMonHistAddon* HTofHistFiller::hhits_rawMA=0;

ClassImp(HTofHistFiller)

HTofHistFiller::HTofHistFiller(const char* name,const char* title)
    : TNamed(name,title)
{
  colorsSector.Set(6);
  colorsModule.Set(4);
  colorsCuts.Set(4);
}
HTofHistFiller::~HTofHistFiller()
{
}
void HTofHistFiller::fillRaw(HTofRaw* raw)
{
  //Float_t rtime=raw->getRightTime();
  //Float_t ltime=raw->getLeftTime();
  //Float_t rcharge=raw->getRightCharge();
  //Float_t lcharge=raw->getLeftCharge();
  //Short_t nHit=raw->getNHit();
  Char_t s=raw->getSector();
  Char_t m=raw->getModule();
  Char_t c=raw->getCell();
  if(hhits_raw)hhits_raw->fill(0,s,m*8+c);
  if(hhits_rawM)hhits_rawM->fill(0,s,m*8+c);
}
void HTofHistFiller::fillCal(HTofCal* cal)
{
}
void HTofHistFiller::fillHit(HTofHit* hit)
{
}
void HTofHistFiller::fillTrend()
{
}
void   HTofHistFiller::putTofParams(HParamList* l,TList* histpool)
{
    // Puts all params of HTofHistFiller to the parameter list of
    // HParamList (which ist used by the io);

       Text_t mytemp[500];
       l->add("colorsSector"           ,colorsSector);
       l->add("colorsModule"           ,colorsModule);
       l->add("colorsCuts"             ,colorsCuts  );
       hhits_rawA->getText(mytemp);
       l->add("hhits_raw"              ,mytemp,500);
       hhits_rawMA->getText(mytemp);
       l->add("hhits_rawM"              ,mytemp,500);
}
Bool_t HTofHistFiller::getTofParams(HParamList* l,TList* histpool,TList* activeHists)
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
    if(!(l->fill("hhits_rawM",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(hhits_rawMA=new HMonHistAddon(mytemp));
	if(hhits_rawMA->getActive()==1)
	{
	    activeHists->Add(hhits_rawM=new HHistArray(*hhits_rawMA));
	    hhits_rawM->getP(0,0)->SetLineColor(colorsSector[0]);
	    hhits_rawM->getP(0,1)->SetLineColor(colorsSector[1]);
	    hhits_rawM->getP(0,2)->SetLineColor(colorsSector[2]);
	    hhits_rawM->getP(0,3)->SetLineColor(colorsSector[3]);
	    hhits_rawM->getP(0,4)->SetLineColor(colorsSector[4]);
	    hhits_rawM->getP(0,5)->SetLineColor(colorsSector[5]);
	}
    }
    return kTRUE;
}