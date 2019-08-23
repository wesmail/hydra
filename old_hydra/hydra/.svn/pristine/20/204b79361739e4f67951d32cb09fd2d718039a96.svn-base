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
#include "TList.h"
#include "TArrayI.h"
#include "TString.h"

#define HODO_CELL_MAX 4
#define HODO_HIT_MAX 30

TArrayI HHodoHistFiller::colorsModule;
// Hist Pointers
// HHistArray*    HHodoHistFiller::hhits_raw=0;
// HMonHist*      HHodoHistFiller::hmulttdcall=0;
HHistArray*    HHodoHistFiller::hProf[MAX_HODO_MODULES]={0,0,0,0,0,0,0,0,0,0};
HMonHist2*     HHodoHistFiller::hRawTime[MAX_HODO_MODULES]={0,0,0,0,0,0,0,0,0,0};
HMonHist2*     HHodoHistFiller::hRawWidth[MAX_HODO_MODULES]={0,0,0,0,0,0,0,0,0,0};
HMonHist2*     HHodoHistFiller::hCrT[MAX_HODO_MODULES]={0,0,0,0,0,0,0,0,0,0};
HMonHist2*     HHodoHistFiller::hFibMult[MAX_HODO_MODULES]={0,0,0,0,0,0,0,0,0,0};

HMonHist2*     HHodoHistFiller::hSpot=0;
HMonHist2*     HHodoHistFiller::hNe5=0;
HMonHist2*     HHodoHistFiller::hTDCMult=0;
HTrendArray*   HHodoHistFiller::hEfficiency=0;
// HMonHist2*     HHodoHistFiller::hhodomult_raw=0;
// HMonHist2*     HHodoHistFiller::hhodomulttemp_raw=0;
// HTrendArray*   HHodoHistFiller::hhodomulttrend_raw=0;

// Hist Addon Pointers
// HMonHistAddon* HHodoHistFiller::hhits_rawA=0;
// HMonHistAddon* HHodoHistFiller::hmulttdcallA=0;
HMonHistAddon* HHodoHistFiller::hProfA[MAX_HODO_MODULES]={0,0,0,0,0,0,0,0,0,0};
HMonHistAddon* HHodoHistFiller::hRawTimeA[MAX_HODO_MODULES]={0,0,0,0,0,0,0,0,0,0};
HMonHistAddon* HHodoHistFiller::hRawWidthA[MAX_HODO_MODULES]={0,0,0,0,0,0,0,0,0,0};
HMonHistAddon* HHodoHistFiller::hCrTA[MAX_HODO_MODULES]={0,0,0,0,0,0,0,0,0,0};
HMonHistAddon* HHodoHistFiller::hFibMultA[MAX_HODO_MODULES]={0,0,0,0,0,0,0,0,0,0};

HMonHistAddon* HHodoHistFiller::hSpotA=0;
HMonHistAddon* HHodoHistFiller::hNe5A=0;
HMonHistAddon* HHodoHistFiller::hTDCMultA=0;
HMonHistAddon* HHodoHistFiller::hEfficiencyA=0;
// HMonHistAddon* HHodoHistFiller::hhodomult_rawA=0;
// HMonHistAddon* HHodoHistFiller::hhodomulttemp_rawA=0;
// HMonHistAddon* HHodoHistFiller::hhodomulttrend_rawA=0;

ClassImp(HHodoHistFiller)

HHodoHistFiller::HHodoHistFiller(const Char_t* name,const Char_t* title)
: TNamed(name,title)
{
	colorsModule.Set(8);
//	colorsCuts.Set(4);
   
   for(Int_t i=0; i<ANZ_EFFIS; i++){
      effi_count_all[i]=0;
      effi_count[i]=0;
   }
}
HHodoHistFiller::~HHodoHistFiller()
{
}
void HHodoHistFiller::fillRawCat(HIterator* rawiter)
{
#define MAX_HODO_HITS	128
	Int_t mult[MAX_HODO_MODULES];
	Int_t mod[MAX_HODO_HITS], fib[MAX_HODO_HITS], cnt;

	HHodoRaw* raw=0;

	rawiter->Reset();
	cnt=0;
   for(Int_t i=0; i<MAX_HODO_MODULES; i++) mult[i]=0;

/*	hTDCMult->getP()->Clear();
	hFib1Mult->getP()->Clear();
	hFib2Mult->getP()->Clear();
*/
	while ((raw=(HHodoRaw*)rawiter->Next())!= 0 && cnt<MAX_HODO_HITS)
   {
   	Int_t m, f;
		m=raw->getModule();
		mod[cnt]=m;
		f=raw->getFiber();
		fib[cnt]=f;
      if( m>=0 && m<MAX_HODO_MODULES) mult[m]++;

/*		if(hTDCMult){
         Int_t nr;
			nr=raw->getNHits();
			hTDCMult->getP()->Fill((Axis_t)(m*128+f),(Axis_t)(nr>10?10:nr));
		}*/
		cnt++;// overflow check in while above
	}

	for(Int_t i=0; i<cnt; i++){
		if(mod[i]>=0 && mod[i]<MAX_HODO_MODULES){
         Int_t m;
         m=mod[i];
			if(hFibMult[m]) hFibMult[m]->getP()->Fill(fib[i],(mult[m]>20?20:mult[m]));
			for(Int_t j=i+1; j<cnt; j++){
				if( mod[j]==m){
					if(hCrT[m]){
						 hCrT[m]->getP()->Fill(fib[i],fib[j]);// Symetrisch
						 hCrT[m]->getP()->Fill(fib[j],fib[i]);
					}
				}
			}
      }
		if(mod[i]==2){
			for(Int_t j=0; j<cnt; j++){
				if( mod[j]==3){
					if(hSpot) hSpot->getP()->Fill(fib[i],fib[j]);// new: veto x/y
				}
			}
      }
		if(mod[i]==4){
			for(Int_t j=0; j<cnt; j++){
				if( mod[j]==5){
					if(hNe5) hNe5->getP()->Fill(fib[i],fib[j]);// new: ne5 corr
				}
			}
		}
	}

/*	if(hTDCMult){// Multiplicity of TDC Channel... in Percent
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
	}*/
   for(Int_t m=0; m<MAX_HODO_MODULES; m++){
      if(hFibMult[m]){// Multiplicity of Fibers... in Percent
         Float_t v=0;
         for(Int_t ii=1; ii<=96; ii++){// Ändern auf dynamische Kanalanzahl
            v=hFibMult[m]->getP()->GetBinContent(ii,2);
            if(v){
                v=100/v;
                for(Int_t i=1; i<=21; i++){
                  hFibMult[m]->getP()->SetBinContent(ii,i,hFibMult[m]->getP()->GetBinContent(ii,i)*v);
                }
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
	
	if(m>=0 && m<MAX_HODO_MODULES){
      if( hProf[m]){
          hProf[m]->getP(0,0)->Fill(f);
      }
      if(hRawTime[m]){
          for(Int_t i=0; i<nr; i++){
            if(i==4) break;// above has not time
            time=raw->getTime(i+1);
            hRawTime[m]->getP()->Fill((Axis_t)(f),(Axis_t)time);
          }
      }
      if(hRawWidth[m]){
          for(Int_t i=0; i<nr; i++){
            if(i==4) break;// above has not time
            width=raw->getWidth(i+1);
            hRawWidth[m]->getP()->Fill((Axis_t)(f),(Axis_t)width);
          }
      }
	}
}
void HHodoHistFiller::fillCal(HHodoCal* cal)
{
	Int_t nr=cal->getNHits();
	Int_t m=cal->getModule();
	Int_t f=cal->getStrip();

	if(m>=0 && m<MAX_HODO_MODULES){
      if( nr>0 && hProf[m]){
         hProf[m]->getP(0,1)->Fill(f);
      }
	}
}
void HHodoHistFiller::fillHitCat(HIterator* hititer)
{
#if MAX_HODO_MODULES<10
#error Efficiency code is valid for 10 Hodo Mods only
#endif
	Int_t mult[MAX_HODO_MODULES];
//	Int_t mod[MAX_HODO_HITS], fib[MAX_HODO_HITS], cnt;

	HHodoHit* hit=0;

	hititer->Reset();
//	cnt=0;
   for(Int_t i=0; i<MAX_HODO_MODULES; i++) mult[i]=0;

	while ((hit=(HHodoHit*)hititer->Next())!= 0 /*&& cnt<MAX_HODO_HITS*/)
   {
   	Int_t m, f;
		m=hit->getModule();
//		mod[cnt]=m;
		f=hit->getStrip();
//		fib[cnt]=f;
      if( m>=0 && m<MAX_HODO_MODULES) mult[m]++;
		
//      cnt++;// overflow check in while above
	}
   
   Int_t ec=0;
   if(mult[3] && mult[2]){
     effi_count_all[ec]++;
     if( mult[6]) effi_count[ec]++;
   }
   ec++;
   if(mult[2]){
     effi_count_all[ec]++;
     if( mult[3]) effi_count[ec]++;
   }
   ec++;
   if(mult[3]){
     effi_count_all[ec]++;
     if( mult[2]) effi_count[ec]++;
   }
   ec++;
   if(mult[4]){
     effi_count_all[ec]++;
     if( mult[5]) effi_count[ec]++;
   }
   ec++;
   if(mult[5]){
     effi_count_all[ec]++;
     if( mult[4]) effi_count[ec]++;
   }
   ec++;
   if(mult[6]){
     effi_count_all[ec]++;
     if( mult[4]) effi_count[ec]++;
   }
   ec++;
   if(mult[6]){
     effi_count_all[ec]++;
     if( mult[5]) effi_count[ec]++;
   }
   ec++;
   if(mult[3] && mult[2]){
     effi_count_all[ec]++;
     if( mult[9]) effi_count[ec]++;
   }
}
void HHodoHistFiller::fillHit(HHodoHit* hit)
{
	Int_t m=hit->getModule();
	Int_t f=hit->getStrip();

	if(m>=0 && m<MAX_HODO_MODULES){
      if( hProf[m]){
          hProf[m]->getP(0,3)->Fill(f);
      }
   }
}
void HHodoHistFiller::fillTrend()
{

  if(hEfficiency){
    for(Int_t i=0; i<ANZ_EFFIS; i++){
      if( effi_count_all[i]>=100){
        hEfficiency->fill(0,i,100.0*effi_count[i]/effi_count_all[i]);
        effi_count[i]=0;
        effi_count_all[i]=0;
      }
    }
  }
}
void   HHodoHistFiller::putHodoParams(HParamList* l,TList* histpool)
{
	// Puts all params of HHodoHistFiller to the parameter list of
	// HParamList (which ist used by the io);
	
	Text_t mytemp[500];
	l->add("colorsModule"           ,colorsModule);
//	l->add("colorsCuts"             ,colorsCuts  );
	
   for(Int_t m=0; m<MAX_HODO_MODULES; m++){
      TString name;
      
      name="hHodoProf";
      name+=m;
      hProfA[m]->getText(mytemp);
      l->add(name.Data(),mytemp,'s',500);
      
      name="hHodoRawTime";
      name+=m;
      hRawTimeA[m]->getText(mytemp);
      l->add(name.Data(),mytemp,'s',500);
      
      name="hHodoRawWidth";
      name+=m;
      hRawWidthA[m]->getText(mytemp);
      l->add(name.Data(),mytemp,'s',500);
      
      name="hHodoCrT";
      name+=m;
      hCrTA[m]->getText(mytemp);
      l->add(name.Data(),mytemp,'s',500);
    
      name="hHodoFibMult";
      name+=m;
      hFibMultA[m]->getText(mytemp);
      l->add(name.Data(),mytemp,'s',500);
   }

	hSpotA->getText(mytemp);
	l->add("hHodoSpot"                   ,mytemp,'s',500);
	hNe5A->getText(mytemp);
	l->add("hHodoNe5"                    ,mytemp,'s',500);
	hTDCMultA->getText(mytemp);
	l->add("hHodoTDCMult"                ,mytemp,'s',500);
	hEfficiencyA->getText(mytemp);
	l->add("hEfficiency"                 ,mytemp,'s',500);
}

Bool_t HHodoHistFiller::getHodoParams(HParamList* l,TList* histpool,TList* activeHists)
{
	if(!(l->fill("colorsModule",&colorsModule))) return kFALSE;
//	if(!(l->fill("colorsCuts"  ,&colorsCuts  ))) return kFALSE;
	
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
   for(Int_t m=0; m<MAX_HODO_MODULES; m++){
      TString name;
      
      name="hHodoProf";
      name+=m;
      if(!(l->fill(name.Data(),mytemp,500))) {return kFALSE;}
      else
      {
         histpool->Add(hProfA[m]=new HMonHistAddon(mytemp));
         if(hProfA[m]->getActive()==1)
         {
            activeHists->Add(hProf[m]=new HHistArray(*hProfA[m]));
            hProf[m]->getP(0,0)->SetFillColor(38);
            hProf[m]->getP(0,1)->SetFillColor(46);
            hProf[m]->getP(0,2)->SetFillColor(42);
            hProf[m]->getP(0,3)->SetFillColor(29);
         }
      }
      
      name="hHodoRawTime";
      name+=m;
      if(!(l->fill(name.Data(),mytemp,500))) {return kFALSE;}
      else
      {
          histpool->Add(hRawTimeA[m]=new HMonHistAddon(mytemp));
          if(hRawTimeA[m]->getActive()==1)
          {
            activeHists->Add(hRawTime[m]=new HMonHist2(*hRawTimeA[m]));
          }
      }
      
      name="hHodoRawWidth";
      name+=m;
      if(!(l->fill(name.Data(),mytemp,500))) {return kFALSE;}
      else
      {
          histpool->Add(hRawWidthA[m]=new HMonHistAddon(mytemp));
          if(hRawWidthA[m]->getActive()==1)
          {
            activeHists->Add(hRawWidth[m]=new HMonHist2(*hRawWidthA[m]));
          }
      }
      
      name="hHodoCrT";
      name+=m;
      if(!(l->fill(name.Data(),mytemp,500))) {return kFALSE;}
      else
      {
          histpool->Add(hCrTA[m]=new HMonHistAddon(mytemp));
          if(hCrTA[m]->getActive()==1)
          {
            activeHists->Add(hCrT[m]=new HMonHist2(*hCrTA[m]));
          }
      }
      
      name="hHodoFibMult";
      name+=m;
      if(!(l->fill(name.Data(),mytemp,500))) {return kFALSE;}
      else
      {
          histpool->Add(hFibMultA[m]=new HMonHistAddon(mytemp));
          if(hFibMultA[m]->getActive()==1)
          {
            activeHists->Add(hFibMult[m]=new HMonHist2(*hFibMultA[m]));
          }
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
   if(!(l->fill("hHodoNe5",mytemp,500))) {return kFALSE;}
	else
	{
		histpool->Add(hNe5A=new HMonHistAddon(mytemp));
		if(hNe5A->getActive()==1)
		{
			activeHists->Add(hNe5=new HMonHist2(*hNe5A));
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
	if(!(l->fill("hEfficiency",mytemp,500))) {return kFALSE;}
	else
	{
		histpool->Add(hEfficiencyA=new HMonHistAddon(mytemp));
		if(hEfficiencyA->getActive()==1)
		{
			activeHists->Add(hEfficiency=new HTrendArray(*hEfficiencyA));
         for(Int_t s=0; s<ANZ_EFFIS; s++){
			 hEfficiency->getP(0,s)->SetLineColor(colorsModule[s]);
			 hEfficiency->getP(0,s)->SetMarkerColor(colorsModule[s]);
         }
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
