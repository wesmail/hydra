//*-- AUTHOR : J. Markert

////////////////////////////////////////////////////////////////////////////
// HMdcHistFiller
//
// Container class for the mdc hists
////////////////////////////////////////////////////////////////////////////
//#include <stdlib.h>

#include "hmdchistfiller.h"
#include "hparamlist.h"
#include "hmonhist.h"
#include "htrendhist.h"
#include "hhistarray.h"
#include "hmdcconfig.h"
#include "hmdcraw.h"
#include "hmdcraweventheader.h"
#include "hmdccal1.h"
#include "hmdccutstat.h"
#include "hades.h"
#include "hevent.h"
#include "hcategory.h"
#include "hmdcdef.h"
#include <TList.h>
#include <TArrayI.h>
#include <TString.h>

// Container/List pointers
HMdcCutStat*     HMdcHistFiller::cutstat=0;
HMdcConfig*      HMdcHistFiller::mdcconfig=0;

// Color definitions
TArrayI HMdcHistFiller::colorsSector;
TArrayI HMdcHistFiller::colorsModule;
TArrayI HMdcHistFiller::colorsCuts;

// Hist Pointers
HMonHist*      HMdcHistFiller::htime1Raw=0;
HMonHist*      HMdcHistFiller::htime1Cal1=0;
HTrendHist*    HMdcHistFiller::htime1Cal1MeanTrend=0;
HMonHist*      HMdcHistFiller::htime1Cal1MeanTrendtemp=0;
HHistArray*    HMdcHistFiller::hcutstat=0;
HHistArray*    HMdcHistFiller::hcutstatM=0;
HMonHist2*     HMdcHistFiller::hCal1Hits=0;
HMonHist2*     HMdcHistFiller::hrawRoc_Subev=0;
HMonHist2*     HMdcHistFiller::hrawError_Stat=0;

// Hist Addon Pointers
HMonHistAddon* HMdcHistFiller::htime1RawA=0;
HMonHistAddon* HMdcHistFiller::htime1Cal1A=0;
HMonHistAddon* HMdcHistFiller::htime1Cal1MeanTrendA=0;
HMonHistAddon* HMdcHistFiller::htime1Cal1MeanTrendtempA=0;
HMonHistAddon* HMdcHistFiller::hcutstatA=0;
HMonHistAddon* HMdcHistFiller::hcutstatMA=0;
HMonHistAddon* HMdcHistFiller::hCal1HitsA=0;
HMonHistAddon* HMdcHistFiller::hrawRoc_SubevA=0;
HMonHistAddon* HMdcHistFiller::hrawError_StatA=0;


ClassImp(HMdcHistFiller)

HMdcHistFiller::HMdcHistFiller(const char* name,const char* title)
    : TNamed(name,title)
{
  colorsSector.Set(6);
  colorsModule.Set(4);
  colorsCuts.Set(4);

  mdcconfig   = new HMdcConfig("MdcConfig","MdcConfig");
}
HMdcHistFiller::~HMdcHistFiller()
{
    delete mdcconfig;
}
HMdcConfig* HMdcHistFiller::getMdcConfig()
{
    return mdcconfig;
}
void HMdcHistFiller::fillRaw(HMdcRaw* raw)
{
    Int_t s,m,mb,ch;
    raw->getAddress(s,m,mb,ch);
    Float_t time1=raw->getTime(1);
    //Float_t time2=raw->getTime(2);
    if(htime1Raw)htime1Raw->getP()->Fill(time1);
}
void HMdcHistFiller::fillRawHeader(HMdcRawEventHeader* rawhead)
{
    Int_t roc          =rawhead->getRoc();
    Int_t subeventsize =rawhead->getSubEvtSize();
    Int_t errorword    =rawhead->getErrorWord();
    Int_t s            =rawhead->getSector();
    Int_t m            =rawhead->getModule();

    if(hrawError_Stat&&s>=0&&m>=0)
    {
	for(Int_t i=0;i<4;i++){if(((errorword>>i)&1)!=0)hrawError_Stat->getP()->Fill(m*6+s,i);}
    }
    if(hrawRoc_Subev)hrawRoc_Subev->getP()->Fill(subeventsize,roc);
}
void HMdcHistFiller::fillCal1(HMdcCal1* cal1)
{
    Int_t s,m,l,c;
    cal1->getAddress(s,m,l,c);
    Float_t time1=cal1->getTime1();
    //Float_t time2=cal1->getTime2();
    if(htime1Cal1)htime1Cal1->getP()->Fill(time1);
    if(htime1Cal1MeanTrendtemp)htime1Cal1MeanTrendtemp->getP()->Fill(time1);

    if(hCal1Hits)hCal1Hits->getP()->Fill(s,m,1);
}
void HMdcHistFiller::fillCutStat()
{

    if(hcutstat){
	cutstat=HMdcCutStat::getExObject();
	if(cutstat)
	{
	    Int_t t1,t2,t1t2,wire,t1_t1t2,cutall,allhits;
	    for(Int_t m=0; m<4; m++) {
		for(Int_t s=0; s<6; s++) {
		    cutstat->getCal1StatCut(s,m,&t1,&t2,&t1t2,&wire,&t1_t1t2,&cutall,&allhits);
		    if(allhits!=0){
			hcutstat->setBinContent(0,1,m*6+s+1,(Float_t)(allhits-t1)     /allhits); //3
			hcutstat->setBinContent(0,2,m*6+s+1,(Float_t)(allhits-t1t2)   /allhits); //5
			hcutstat->setBinContent(0,3,m*6+s+1,(Float_t)(allhits-cutall) /allhits); //4
			hcutstat->setBinContent(0,0,m*6+s+1,(Float_t)allhits/allhits); //2

			hcutstatM->setBinContent(0,1,m*6+s+1,(Float_t)(allhits-t1)     /allhits); //3
			hcutstatM->setBinContent(0,2,m*6+s+1,(Float_t)(allhits-t1t2)   /allhits); //5
			hcutstatM->setBinContent(0,3,m*6+s+1,(Float_t)(allhits-cutall) /allhits); //4
			hcutstatM->setBinContent(0,0,m*6+s+1,(Float_t)(allhits)/allhits); //2
		    }
		}
	    }
	    if(hcutstatA->getResetable()==1)cutstat->clear();
	}
    }
}
void HMdcHistFiller::fillTrend()
{
    if(htime1Cal1MeanTrend)htime1Cal1MeanTrend->fill(htime1Cal1MeanTrendtemp->getP()->GetMean(), htime1Cal1MeanTrendtemp->getP()->GetRMS());
    if(htime1Cal1MeanTrendtemp)htime1Cal1MeanTrendtemp->getP()->Reset();
}
void HMdcHistFiller::fillMdcConfig(Int_t fCount)
{
    HCategory* catRaw=gHades->getCurrentEvent()->getCategory(catMdcRaw);
    if(catRaw)
    {
	mdcconfig->setEvtSeqNb(catRaw->getEntries());
	mdcconfig->setEvtNb(fCount);
	mdcconfig->setRawMult(catRaw->getEntries());
    }
    HCategory* catCal1=gHades->getCurrentEvent()->getCategory(catMdcCal1);
    if(catCal1)
    {
	mdcconfig->setCalMult(catCal1->getEntries());
    }
}
void   HMdcHistFiller::putMdcParams(HParamList* l,TList* histpool)
{
    // Puts all params of HMdcHistFiller to the parameter list of
    // HParamList (which ist used by the io);

       Text_t mytemp[500];
       l->add("colorsSector"           ,colorsSector);
       l->add("colorsModule"           ,colorsModule);
       l->add("colorsCuts"             ,colorsCuts  );
       htime1RawA->getText(mytemp);
       l->add("htime1Raw"              ,mytemp,500);
       htime1Cal1A->getText(mytemp);
       l->add("htime1Cal1"             ,mytemp,500);
       htime1Cal1MeanTrendA->getText(mytemp);
       l->add("htime1Cal1MeanTrend"    ,mytemp,500);
       htime1Cal1MeanTrendtempA->getText(mytemp);
       l->add("htime1Cal1MeanTrendTemp",mytemp,500);
       hcutstatA->getText(mytemp);
       l->add("hcutstat"               ,mytemp,500);
       hcutstatMA->getText(mytemp);
       l->add("hcutstatM"              ,mytemp,500);
       hCal1HitsA->getText(mytemp);
       l->add("hcal1hits"              ,mytemp,500);
       hrawRoc_SubevA->getText(mytemp);
       l->add("hrawRoc_Subev"          ,mytemp,500);
       hrawError_StatA->getText(mytemp);
       l->add("hrawError_Stat"         ,mytemp,500);
}
Bool_t HMdcHistFiller::getMdcParams(HParamList* l,TList* histpool,TList* activeHists)
{
    if(!(l->fill("colorsSector",&colorsSector))) return kFALSE;
    if(!(l->fill("colorsModule",&colorsModule))) return kFALSE;
    if(!(l->fill("colorsCuts"  ,&colorsCuts  ))) return kFALSE;

    Text_t mytemp[500];
    if(!(l->fill("htime1Raw",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(htime1RawA=new HMonHistAddon(mytemp));
	if(htime1RawA->getActive()==1)
	{
	    activeHists->Add(htime1Raw=new HMonHist(*htime1RawA));
	    htime1RawA->getText(mytemp);
            cout<<mytemp<<endl;
	}
    }
    if(!(l->fill("htime1Cal1",mytemp,500))) return kFALSE;
    else
    {
	histpool->Add(htime1Cal1A=new HMonHistAddon(mytemp));
	if(htime1Cal1A->getActive()==1)
	{
	    activeHists->Add(htime1Cal1=new HMonHist(*htime1Cal1A));
	}
    }
    if(!(l->fill("htime1Cal1MeanTrend",mytemp,500))) return kFALSE;
    else
    {
	histpool->Add(htime1Cal1MeanTrendA=new HMonHistAddon(mytemp));
	if(htime1Cal1MeanTrendA->getActive()==1)
	{
	    activeHists->Add(htime1Cal1MeanTrend=new HTrendHist(*htime1Cal1MeanTrendA));
	}
    }
    if(!(l->fill("htime1Cal1MeanTrendTemp",mytemp,500))) return kFALSE;
    else
    {
	histpool->Add(htime1Cal1MeanTrendtempA=new HMonHistAddon(mytemp));
	if(htime1Cal1MeanTrendtempA->getActive()==1)
	{
	    activeHists->Add(htime1Cal1MeanTrendtemp=new HMonHist(*htime1Cal1MeanTrendtempA));
	}
    }
    if(!(l->fill("hcutstat",mytemp,500))) return kFALSE;
    else
    {
	histpool->Add(hcutstatA=new HMonHistAddon(mytemp));
	if(hcutstatA->getActive()==1)
	{
	    activeHists->Add(hcutstat=new HHistArray(*hcutstatA));
	    hcutstat->getP(0,0)->SetFillColor(colorsCuts[0]);
	    hcutstat->getP(0,1)->SetFillColor(colorsCuts[1]);
	    hcutstat->getP(0,2)->SetFillColor(colorsCuts[2]);
	    hcutstat->getP(0,3)->SetFillColor(colorsCuts[3]);

	}
    }
    if(!(l->fill("hcutstatM",mytemp,500))) return kFALSE;
    else
    {
	histpool->Add(hcutstatMA=new HMonHistAddon(mytemp));
	if(hcutstatMA->getActive()==1)
	{
	    activeHists->Add(hcutstatM=new HHistArray(*hcutstatMA));
	    hcutstatM->getP(0,0)->SetFillColor(colorsCuts[0]);
	    hcutstatM->getP(0,1)->SetFillColor(colorsCuts[1]);
	    hcutstatM->getP(0,2)->SetFillColor(colorsCuts[2]);
	    hcutstatM->getP(0,3)->SetFillColor(colorsCuts[3]);

	}
    }
    if(!(l->fill("hcal1hits",mytemp,500))) return kFALSE;
    else
    {
	histpool->Add(hCal1HitsA=new HMonHistAddon(mytemp));
	if(hCal1HitsA->getActive()==1)
	{
	    activeHists->Add(hCal1Hits=new HMonHist2(*hCal1HitsA));
	}
    }
    if(!(l->fill("hrawRoc_Subev",mytemp,500))) return kFALSE;
    else
    {
	histpool->Add(hrawRoc_SubevA=new HMonHistAddon(mytemp));
	if(hrawRoc_SubevA->getActive()==1)
	{
	    activeHists->Add(hrawRoc_Subev=new HMonHist2(*hrawRoc_SubevA));
	}
    }
    if(!(l->fill("hrawError_Stat",mytemp,500))) return kFALSE;
    else
    {
	histpool->Add(hrawError_StatA=new HMonHistAddon(mytemp));
	if(hrawError_StatA->getActive()==1)
	{
	    activeHists->Add(hrawError_Stat=new HMonHist2(*hrawError_StatA));
	}
    }

    return kTRUE;
}
