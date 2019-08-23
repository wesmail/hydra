//*-- AUTHOR : J. Markert

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

// Color definitions
TArrayI HShowerHistFiller::colorsSector;
TArrayI HShowerHistFiller::colorsModule;
TArrayI HShowerHistFiller::colorsCuts;

// Hist Pointers
HMonHist2*     HShowerHistFiller::hhitsRaw=0;
HMonHist2*     HShowerHistFiller::hhitsRawM=0;

// Hist Addon Pointers
HMonHistAddon* HShowerHistFiller::hhitsRawA=0;
HMonHistAddon* HShowerHistFiller::hhitsRawMA=0;

ClassImp(HShowerHistFiller)

HShowerHistFiller::HShowerHistFiller(const char* name,const char* title)
    : TNamed(name,title)
{
  colorsSector.Set(6);
  colorsModule.Set(4);
  colorsCuts.Set(4);
}
HShowerHistFiller::~HShowerHistFiller()
{
}
void HShowerHistFiller::fillRaw(HShowerRaw* raw)
{
        Int_t        s=raw->getSector();
        Int_t        m=raw->getModule();
        //Int_t row     =raw->getRow();
        //Int_t col     =raw->getCol();
        //Float_t charge=raw->getCharge();

   if(hhitsRaw)hhitsRaw->getP()->Fill(s,m);
   if(hhitsRawM)hhitsRawM->getP()->Fill(s,m);
}
void HShowerHistFiller::fillHit(HShowerHit* hit)
{
}
void HShowerHistFiller::fillCal(HShowerCal* cal)
{
}
void HShowerHistFiller::fillTrend()
{
}
void   HShowerHistFiller::putShowerParams(HParamList* l,TList* histpool)
{
    // Puts all params of HShowerHistFiller to the parameter list of
    // HParamList (which ist used by the io);

       Text_t mytemp[500];
       l->add("colorsSector"           ,colorsSector);
       l->add("colorsModule"           ,colorsModule);
       l->add("colorsCuts"             ,colorsCuts  );
       hhitsRawA->getText(mytemp);
       l->add("hhitsRaw"               ,mytemp,500);
       hhitsRawMA->getText(mytemp);
       l->add("hhitsRawM"               ,mytemp,500);
}
Bool_t HShowerHistFiller::getShowerParams(HParamList* l,TList* histpool,TList* activeHists)
{
    if(!(l->fill("colorsSector",&colorsSector))) return kFALSE;
    if(!(l->fill("colorsModule",&colorsModule))) return kFALSE;
    if(!(l->fill("colorsCuts"  ,&colorsCuts  ))) return kFALSE;

    Text_t mytemp[500];
    if(!(l->fill("hhitsRaw",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(hhitsRawA=new HMonHistAddon(mytemp));
	if(hhitsRawA->getActive()==1)
	{
	    activeHists->Add(hhitsRaw=new HMonHist2(*hhitsRawA));
	}
    }
    if(!(l->fill("hhitsRawM",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(hhitsRawMA=new HMonHistAddon(mytemp));
	if(hhitsRawMA->getActive()==1)
	{
	    activeHists->Add(hhitsRawM=new HMonHist2(*hhitsRawMA));
	}
    }
    return kTRUE;
}
