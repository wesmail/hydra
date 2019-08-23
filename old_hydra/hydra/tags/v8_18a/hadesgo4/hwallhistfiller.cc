//*-- AUTHOR : B. Spruck

//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////////
// HWallHistFiller
//
// Container class for the wall hists
////////////////////////////////////////////////////////////////////////////
//#include <stdlib.h>

#include "hwallhistfiller.h"
#include "hparamlist.h"
#include "hmonhist.h"
#include "htrendhist.h"
#include "hhistarray.h"

#include "walldef.h"
#include "hwallraw.h"
#include "hwallhit.h"

#include "hades.h"
#include "hevent.h"
#include "hcategory.h"
#include "hiterator.h"
#include <TList.h>
#include <TArrayI.h>
#include <TString.h>

// Color definitions
TArrayI HWallHistFiller::colorsSector;
TArrayI HWallHistFiller::colorsModule;
TArrayI HWallHistFiller::colorsCuts;

// Hist Pointers
HHistArray*      HWallHistFiller::hProf1=0;
HHistArray*      HWallHistFiller::hProf2=0;
HHistArray*      HWallHistFiller::hProf3=0;
HMonHist2*     HWallHistFiller::hRawTime1=0;
HMonHist2*     HWallHistFiller::hRawWidth1=0;
HMonHist2*     HWallHistFiller::hHitTime=0;
HMonHist2*     HWallHistFiller::hHitCharge=0;
HMonHist2*     HWallHistFiller::hHitXY=0;
HMonHist2*     HWallHistFiller::hFib1Mult=0;
HHistArray*    HWallHistFiller::hRawMult=0;

// Hist Addon Pointers
HMonHistAddon* HWallHistFiller::hProf1A=0;
HMonHistAddon* HWallHistFiller::hProf2A=0;
HMonHistAddon* HWallHistFiller::hProf3A=0;
HMonHistAddon* HWallHistFiller::hRawTime1A=0;
HMonHistAddon* HWallHistFiller::hRawWidth1A=0;
HMonHistAddon* HWallHistFiller::hHitTimeA=0;
HMonHistAddon* HWallHistFiller::hHitChargeA=0;
HMonHistAddon* HWallHistFiller::hHitXYA=0;
HMonHistAddon* HWallHistFiller::hFib1MultA=0;
HMonHistAddon* HWallHistFiller::hRawMultA=0;

ClassImp(HWallHistFiller)

HWallHistFiller::HWallHistFiller(const Char_t* name,const Char_t* title)
: TNamed(name,title)
{
	colorsSector.Set(6);
	colorsModule.Set(4);
	colorsCuts.Set(4);
}
HWallHistFiller::~HWallHistFiller()
{
}
void HWallHistFiller::fillRawCat(HIterator* rawiter)
{
	Int_t wallCell;
	Int_t wallCellArr[5]={0,0,0,0,0};
	HWallRaw* raw=0;

	rawiter->Reset();
	while ((raw=(HWallRaw*)rawiter->Next())!= 0)
   {
		if(raw->getNHits()<=0) continue;// will never happen 
		wallCellArr[4]++;// all
		wallCell=raw->getCell();

		//I quarter -> 1-6;61-66
		if (((wallCell >= 0 ) && (wallCell <= 5))||
				((wallCell >= 12 ) && (wallCell <= 17)) ||
				((wallCell >= 24 ) && (wallCell <= 29)) ||
				((wallCell >= 36 ) && (wallCell <= 41)) ||
				((wallCell >= 48 ) && (wallCell <= 53)) ||
				((wallCell >= 60 ) && (wallCell <= 64))) wallCellArr[0]++;

		//II quarter -> 7-12;67-72
		if (((wallCell >= 6 ) && (wallCell <= 11))||
			((wallCell >= 18 ) && (wallCell <= 23)) ||
			((wallCell >= 30 ) && (wallCell <= 35)) ||
			((wallCell >= 42 ) && (wallCell <= 47)) ||
			((wallCell >= 54 ) && (wallCell <= 59)) ||
			((wallCell >= 67 ) && (wallCell <= 71))) wallCellArr[1]++;

		//III quarter -> 73-78;133-138
		if (((wallCell >= 72 ) && (wallCell <= 76))||
				((wallCell >= 84 ) && (wallCell <= 89)) ||
				((wallCell >= 96 ) && (wallCell <= 101)) ||
				((wallCell >= 108 ) && (wallCell <= 113)) ||
				((wallCell >= 120 ) && (wallCell <= 125)) ||
				((wallCell >= 132 ) && (wallCell <= 137))) wallCellArr[2]++;

		//IV quarter -> 79-84;139-144
		if (((wallCell >= 79 ) && (wallCell <= 83))||
				((wallCell >= 90 ) && (wallCell <= 95)) ||
				((wallCell >= 102 ) && (wallCell <= 107)) ||
				((wallCell >= 114 ) && (wallCell <= 119)) ||
				((wallCell >= 126 ) && (wallCell <= 131)) ||
				((wallCell >= 138 ) && (wallCell <= 143))) wallCellArr[3]++;
	}
	if(hRawMult){
		// hRawMult->getP()->Fill((Axis_t)mult);
      for(Int_t i=0;i<5;i++)
		{
			hRawMult->fill(0,i,wallCellArr[i]);
		}
	}
}

void HWallHistFiller::fillRaw(HWallRaw* raw)
{
	Int_t nr=raw->getNHits();
	Int_t f=raw->getCell();

	Float_t time, width;
	
	if( hProf1){
		hProf1->fill(0,0,f);
		hProf1->getP(0,1)->Fill(65,0.0025);
		hProf1->getP(0,1)->Fill(66,0.0025);
		hProf1->getP(0,1)->Fill(77,0.0025);
		hProf1->getP(0,1)->Fill(78,0.0025);
	}
	if( hProf2){
		hProf2->fill(0,0,f);
		//hProf2->fill(0,1,144);
	}
	if( hProf3){
		hProf3->fill(0,0,f);
		hProf3->getP(0,1)->Fill(217,0.01);
		hProf3->getP(0,1)->Fill(218,0.01);
		hProf3->getP(0,1)->Fill(219,0.01);
		hProf3->getP(0,1)->Fill(229,0.01);
		hProf3->getP(0,1)->Fill(282,0.01);
		hProf3->getP(0,1)->Fill(292,0.01);
		hProf3->getP(0,1)->Fill(293,0.01);
		hProf3->getP(0,1)->Fill(294,0.01);
	}
	if(hRawTime1){
		for(Int_t i=0; i<nr; i++){
			if(i==4) break;// above has not time
			time=raw->getTime(i+1);
			hRawTime1->getP()->Fill((Axis_t)(f),(Axis_t)time);
		}
	}
	if(hRawWidth1){
		for(Int_t i=0; i<nr; i++){
			if(i==4) break;// above has not time
			width=raw->getWidth(i+1);
			hRawWidth1->getP()->Fill((Axis_t)(f),(Axis_t)width);
		}
	}
}

void HWallHistFiller::fillHit(HWallHit* hit)
{
    Int_t wallCell;
    Float_t wallX,wallY,wallZ,wallTime,wallCharge;

    wallCell=hit->getCell();
    wallTime=hit->getTime();
    wallCharge=hit->getCharge();
    hit->getXYZLab(wallX,wallY,wallZ);

    wallX=wallX/10;
	 wallY=wallY/10; 
    if(hHitXY) hHitXY->getP()->Fill(wallX,wallY);      
    if(hHitTime) hHitTime->getP()->Fill((Float_t)wallCell,wallTime);
    if(hHitCharge) hHitCharge->getP()->Fill((Float_t)wallCell,wallCharge);
}

void HWallHistFiller::fillTrend()
{
}
void   HWallHistFiller::putWallParams(HParamList* l,TList* histpool)
{
	// Puts all params of HWallHistFiller to the parameter list of
	// HParamList (which ist used by the io);
	
	Text_t mytemp[500];
	l->add("colorsSector"           ,colorsSector);
	l->add("colorsModule"           ,colorsModule);
	l->add("colorsCuts"             ,colorsCuts  );
	
	hProf1A->getText(mytemp);
	l->add("hWallProf1"                  ,mytemp,'s',500);
	
	hProf2A->getText(mytemp);
	l->add("hWallProf2"                  ,mytemp,'s',500);
	
	hProf3A->getText(mytemp);
	l->add("hWallProf3"                  ,mytemp,'s',500);
	
	hRawTime1A->getText(mytemp);
	l->add("hWallRawTime1"               ,mytemp,'s',500);
	
	hRawWidth1A->getText(mytemp);
	l->add("hWallRawWidth1"              ,mytemp,'s',500);
	
	hHitTimeA->getText(mytemp);
	l->add("hWallHitTime"                ,mytemp,'s',500);
	
	hHitChargeA->getText(mytemp);
	l->add("hWallHitCharge"              ,mytemp,'s',500);
	
	hHitXYA->getText(mytemp);
	l->add("hWallHitXY"                  ,mytemp,'s',500);

	hFib1MultA->getText(mytemp);
	l->add("hWallFib1Mult"               ,mytemp,'s',500);

	hRawMultA->getText(mytemp);
	l->add("hWallRawMult"                ,mytemp,'s',500);
}

Bool_t HWallHistFiller::getWallParams(HParamList* l,TList* histpool,TList* activeHists)
{
	if(!(l->fill("colorsSector",&colorsSector))) return kFALSE;
	if(!(l->fill("colorsModule",&colorsModule))) return kFALSE;
	if(!(l->fill("colorsCuts"  ,&colorsCuts  ))) return kFALSE;
	
	Text_t mytemp[500];
	
	if(!(l->fill("hWallProf1",mytemp,500))) {return kFALSE;}
	else
	{
		histpool->Add(hProf1A=new HMonHistAddon(mytemp));
		if(hProf1A->getActive()==1)
		{
			activeHists->Add(hProf1=new HHistArray(*hProf1A));
			hProf1->getP(0,0)->SetFillColor(46);
			hProf1->getP(0,1)->SetFillColor(38);
		}
	}
	if(!(l->fill("hWallProf2",mytemp,500))) {return kFALSE;}
	else
	{
		histpool->Add(hProf2A=new HMonHistAddon(mytemp));
		if(hProf2A->getActive()==1)
		{
			activeHists->Add(hProf2=new HHistArray(*hProf2A));
			hProf2->getP(0,0)->SetFillColor(46);
			hProf2->getP(0,1)->SetFillColor(38);
		}
	}
	if(!(l->fill("hWallProf3",mytemp,500))) {return kFALSE;}
	else
	{
		histpool->Add(hProf3A=new HMonHistAddon(mytemp));
		if(hProf3A->getActive()==1)
		{
			activeHists->Add(hProf3=new HHistArray(*hProf3A));
			hProf3->getP(0,0)->SetFillColor(46);
			hProf3->getP(0,1)->SetFillColor(38);
		}
	}
	if(!(l->fill("hWallRawTime1",mytemp,500))) {return kFALSE;}
	else
	{
		histpool->Add(hRawTime1A=new HMonHistAddon(mytemp));
		if(hRawTime1A->getActive()==1)
		{
			activeHists->Add(hRawTime1=new HMonHist2(*hRawTime1A));
		}
	}
	if(!(l->fill("hWallRawWidth1",mytemp,500))) {return kFALSE;}
	else
	{
		histpool->Add(hRawWidth1A=new HMonHistAddon(mytemp));
		if(hRawWidth1A->getActive()==1)
		{
			activeHists->Add(hRawWidth1=new HMonHist2(*hRawWidth1A));
		}
	}
	if(!(l->fill("hWallHitTime",mytemp,500))) {return kFALSE;}
	else
	{
		histpool->Add(hHitTimeA=new HMonHistAddon(mytemp));
		if(hHitTimeA->getActive()==1)
		{
			activeHists->Add(hHitTime=new HMonHist2(*hHitTimeA));
		}
	}
	if(!(l->fill("hWallHitCharge",mytemp,500))) {return kFALSE;}
	else
	{
		histpool->Add(hHitChargeA=new HMonHistAddon(mytemp));
		if(hHitChargeA->getActive()==1)
		{
			activeHists->Add(hHitCharge=new HMonHist2(*hHitChargeA));
		}
	}
	if(!(l->fill("hWallHitXY",mytemp,500))) {return kFALSE;}
	else
	{
		histpool->Add(hHitXYA=new HMonHistAddon(mytemp));
		if(hHitXYA->getActive()==1)
		{
			activeHists->Add(hHitXY=new HMonHist2(*hHitXYA));
		}
	}
	if(!(l->fill("hWallFib1Mult",mytemp,500))) {return kFALSE;}
	else
	{
		histpool->Add(hFib1MultA=new HMonHistAddon(mytemp));
		if(hFib1MultA->getActive()==1)
		{
			activeHists->Add(hFib1Mult=new HMonHist2(*hFib1MultA));
		}
	}
	if(!(l->fill("hWallRawMult",mytemp,500))) {return kFALSE;}
	else
	{
		histpool->Add(hRawMultA=new HMonHistAddon(mytemp));
		if(hRawMultA->getActive()==1)
		{
			activeHists->Add(hRawMult=new HHistArray(*hRawMultA));
			for(Int_t sec=0;sec<5;sec++){
		      hRawMult->getP(0,sec)->SetLineColor(colorsSector[sec]);
			}
		}
	}
	return kTRUE;
}
