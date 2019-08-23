//*-- AUTHOR : J. Markert

//_HADES_CLASS_DESCRIPTION 
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
HMonHist*      HTofHistFiller::hmulttof_raw=0;
HMonHist*      HTofHistFiller::hmulttoftofino_raw=0;
HMonHist*      HTofHistFiller::htofpatternL_raw=0;
HMonHist*      HTofHistFiller::htofpatternR_raw=0;
HMonHist2*     HTofHistFiller::htoftdcL_raw=0;
HMonHist2*     HTofHistFiller::htoftdcR_raw=0;
HMonHist2*     HTofHistFiller::htofadcL_raw=0;
HMonHist2*     HTofHistFiller::htofadcR_raw=0;
HMonHist2*     HTofHistFiller::htoftdcSum_hit=0;
HMonHist2*     HTofHistFiller::htoftdcDiff_hit=0;
HMonHist2*     HTofHistFiller::htofadcSum_hit=0;
HMonHist2*     HTofHistFiller::htofadcPos_hit=0;
HMonHist*      HTofHistFiller::htofsum_hit=0;
HMonHist*      HTofHistFiller::htoftheta_hit=0;
HMonHist*      HTofHistFiller::htofphi_hit=0;
HMonHist2*     HTofHistFiller::htofyvsx_hit=0;
HMonHist2*     HTofHistFiller::htofmult_raw=0;
HMonHist2*     HTofHistFiller::htofmulttemp_raw=0;
HTrendArray*   HTofHistFiller::htofmulttrend_raw=0;

// Hist Addon Pointers
HMonHistAddon* HTofHistFiller::hhits_rawA=0;
HMonHistAddon* HTofHistFiller::hmulttof_rawA=0;
HMonHistAddon* HTofHistFiller::hmulttoftofino_rawA=0;
HMonHistAddon* HTofHistFiller::htofpatternL_rawA=0;
HMonHistAddon* HTofHistFiller::htofpatternR_rawA=0;
HMonHistAddon* HTofHistFiller::htoftdcL_rawA=0;
HMonHistAddon* HTofHistFiller::htoftdcR_rawA=0;
HMonHistAddon* HTofHistFiller::htofadcL_rawA=0;
HMonHistAddon* HTofHistFiller::htofadcR_rawA=0;
HMonHistAddon* HTofHistFiller::htoftdcSum_hitA=0;
HMonHistAddon* HTofHistFiller::htoftdcDiff_hitA=0;
HMonHistAddon* HTofHistFiller::htofadcSum_hitA=0;
HMonHistAddon* HTofHistFiller::htofadcPos_hitA=0;
HMonHistAddon* HTofHistFiller::htofsum_hitA=0;
HMonHistAddon* HTofHistFiller::htoftheta_hitA=0;
HMonHistAddon* HTofHistFiller::htofphi_hitA=0;
HMonHistAddon* HTofHistFiller::htofyvsx_hitA=0;
HMonHistAddon* HTofHistFiller::htofmult_rawA=0;
HMonHistAddon* HTofHistFiller::htofmulttemp_rawA=0;
HMonHistAddon* HTofHistFiller::htofmulttrend_rawA=0;

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
  Float_t rtime=raw->getRightTime();
  Float_t ltime=raw->getLeftTime();
  Float_t rcharge=raw->getRightCharge();
  Float_t lcharge=raw->getLeftCharge();
  //Short_t nHit=raw->getNHit();
  Int_t s=raw->getSector();
  Int_t m=raw->getModule();
  Int_t c=raw->getCell();
  Int_t itof2= (s*64+m*8+c+1);

  if(hhits_raw)hhits_raw->fill(0,s,m*8+c);
  if(htofpatternL_raw && ltime>10 && ltime<3900)htofpatternL_raw->getP()->Fill(itof2);
  if(htofpatternR_raw && rtime>10 && rtime<3900)htofpatternR_raw->getP()->Fill(itof2);
  if(htoftdcL_raw )htoftdcL_raw->getP()->Fill(itof2,ltime);
  if(htoftdcR_raw )htoftdcR_raw->getP()->Fill(itof2,rtime);
  if(htofadcL_raw )htofadcL_raw->getP()->Fill(itof2,lcharge);
  if(htofadcR_raw )htofadcR_raw->getP()->Fill(itof2,rcharge);

  if(rtime>0)
  {
      if(htofmult_raw)    htofmult_raw    ->getP()->Fill(m*8+c,s);
      if(htofmulttemp_raw)htofmulttemp_raw->getP()->Fill(m*8+c,s);
  }
  if(ltime>0)
  {
      if(htofmult_raw)    htofmult_raw    ->getP()->Fill(m*8+c,s);
      if(htofmulttemp_raw)htofmulttemp_raw->getP()->Fill(m*8+c,s);
  }

}
void HTofHistFiller::fillRawMult(Int_t tofmult,Int_t tofinomult)
{
  if(hmulttof_raw)hmulttof_raw->getP()->Fill(tofmult);
  if(hmulttoftofino_raw)hmulttoftofino_raw->getP()->Fill(tofmult+tofinomult);
}
void HTofHistFiller::fillHit(HTofHit* hit)
{
      Int_t s          =hit->getSector();
      Int_t m          =hit->getModule();
      Int_t c          =hit->getCell();
      Float_t xpos     =hit->getXpos();
      Float_t tof      =hit->getTof();
      Float_t Edep     =hit->getEdep();
      Float_t xposAdc  =hit->getXposAdc();
      Int_t   Adcflag  =hit->getAdcFlag();

      Float_t dist =0;
      Float_t theta=0;
      Float_t phi  =0;
      Float_t lab_x=0;
      Float_t lab_y=0;
      Float_t lab_z=0;

      hit->getDistance(dist);
      hit->getTheta(theta);
      hit->getPhi(phi);
      hit->getXYZLab(lab_x,lab_y,lab_z);
      Int_t itof2= (s*64+m*8+c+1);

      Float_t corrT=((dist-2100)/300);
      Float_t time = (tof - corrT);

      //if(startdet>3) time=time+1;

      //Int_t rod  =m*8+c;
      //Int_t ypos;
      //if(rod <32) ypos= rod*30+15;
      //if(rod>=32) ypos=(rod-32)*20+970;
      //ypos=800.-ypos;

      if(htoftdcSum_hit)htoftdcSum_hit->getP()->Fill(itof2,time);
      if(htoftdcDiff_hit)htoftdcDiff_hit->getP()->Fill(itof2,xpos);
      if(Adcflag==2)
      {
	  if(htofadcSum_hit)htofadcSum_hit->getP()->Fill(itof2,Edep);
	  if(htofadcPos_hit)htofadcPos_hit->getP()->Fill(itof2,xposAdc);
      }
      if( htofsum_hit)  htofsum_hit  ->getP()->Fill(time);
      if( htoftheta_hit)htoftheta_hit->getP()->Fill(theta);
      if( htofphi_hit)  htofphi_hit  ->getP()->Fill(phi);
      if( htofyvsx_hit) htofyvsx_hit ->getP()->Fill(lab_x,lab_y);

}
void HTofHistFiller::fillTrend()
{

    for(Int_t s=0;s<6;s++){
	if(htofmulttemp_raw&&htofmulttrend_raw){
	    htofmulttrend_raw->fill(0,s,htofmulttemp_raw->getP()->Integral(1,64,s+1,s+1));
	}
    }
    if(htofmulttemp_raw)htofmulttemp_raw->getP()->Reset();


}
void   HTofHistFiller::putTofParams(HParamList* l,TList* histpool)
{
    // Puts all params of HTofHistFiller to the parameter list of
    // HParamList (which ist used by the io);

       Text_t mytemp[500];
       l->add("colorsSector"           ,colorsSector);
       l->add("colorsModule"           ,colorsModule);
       l->add("colorsCuts"             ,colorsCuts  );
       htofmult_rawA->getText(mytemp);
       l->add("htofmult_raw"           ,mytemp,500);
       htofmulttemp_rawA->getText(mytemp);
       l->add("htofmulttemp_raw"       ,mytemp,500);
       htofmulttrend_rawA->getText(mytemp);
       l->add("htofmulttrend_raw"      ,mytemp,500);
       hhits_rawA->getText(mytemp);
       l->add("hhits_raw"              ,mytemp,500);
       hmulttof_rawA->getText(mytemp);
       l->add("hmulttof_raw"           ,mytemp,500);
       htofpatternL_rawA->getText(mytemp);
       l->add("htofpatternL_raw"       ,mytemp,500);
       htofpatternR_rawA->getText(mytemp);
       l->add("htofpatternR_raw"       ,mytemp,500);
       htoftdcL_rawA->getText(mytemp);
       l->add("htoftdcL_raw"           ,mytemp,500);
       htoftdcR_rawA->getText(mytemp);
       l->add("htoftdcR_raw"           ,mytemp,500);
       htofadcL_rawA->getText(mytemp);
       l->add("htofadcL_raw"           ,mytemp,500);
       htofadcR_rawA->getText(mytemp);
       l->add("htofadcR_raw"           ,mytemp,500);
       htoftdcSum_hitA->getText(mytemp);
       l->add("htoftdcSum_hit"         ,mytemp,500);
       htoftdcDiff_hitA->getText(mytemp);
       l->add("htoftdcDiff_hit"        ,mytemp,500);
       htofadcSum_hitA->getText(mytemp);
       l->add("htofadcSum_hit"         ,mytemp,500);
       htofadcPos_hitA->getText(mytemp);
       l->add("htofadcPos_hit"         ,mytemp,500);
       htofsum_hitA->getText(mytemp);
       l->add("htofsum_hit"            ,mytemp,500);
       htoftheta_hitA->getText(mytemp);
       l->add("htoftheta_hit"          ,mytemp,500);
       htofphi_hitA->getText(mytemp);
       l->add("htofphi_hit"            ,mytemp,500);
       htofyvsx_hitA->getText(mytemp);
       l->add("htofyvsx_hit"           ,mytemp,500);
       hmulttoftofino_rawA->getText(mytemp);
       l->add("htofmulttoftofino_raw"     ,mytemp,500);
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
    if(!(l->fill("htofmult_raw",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(htofmult_rawA=new HMonHistAddon(mytemp));
	if(htofmult_rawA->getActive()==1)
	{
	    activeHists->Add(htofmult_raw=new HMonHist2(*htofmult_rawA));
	}
    }
    if(!(l->fill("htofmulttemp_raw",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(htofmulttemp_rawA=new HMonHistAddon(mytemp));
	if(htofmulttemp_rawA->getActive()==1)
	{
	    activeHists->Add(htofmulttemp_raw=new HMonHist2(*htofmulttemp_rawA));
	}
    }
    if(!(l->fill("htofmulttrend_raw",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(htofmulttrend_rawA=new HMonHistAddon(mytemp));
	if(htofmulttrend_rawA->getActive()==1)
	{
	    activeHists->Add(htofmulttrend_raw=new HTrendArray(*htofmulttrend_rawA));
	    for(Int_t s=0;s<6;s++){
		htofmulttrend_raw->getP(0,s)->SetLineColor(colorsSector[s]);
		htofmulttrend_raw->getP(0,s)->SetMarkerColor(colorsSector[s]);
	    }
	}
    }
    if(!(l->fill("hmulttof_raw",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(hmulttof_rawA=new HMonHistAddon(mytemp));
	if(hmulttof_rawA->getActive()==1)
	{
	    activeHists->Add(hmulttof_raw=new HMonHist(*hmulttof_rawA));
	}
    }
    if(!(l->fill("htofpatternL_raw",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(htofpatternL_rawA=new HMonHistAddon(mytemp));
	if(htofpatternL_rawA->getActive()==1)
	{
	    activeHists->Add(htofpatternL_raw=new HMonHist(*htofpatternL_rawA));
	}
    }
    if(!(l->fill("htofpatternR_raw",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(htofpatternR_rawA=new HMonHistAddon(mytemp));
	if(htofpatternR_rawA->getActive()==1)
	{
	    activeHists->Add(htofpatternR_raw=new HMonHist(*htofpatternR_rawA));
	}
    }
    if(!(l->fill("htoftdcL_raw",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(htoftdcL_rawA=new HMonHistAddon(mytemp));
	if(htoftdcL_rawA->getActive()==1)
	{
	    activeHists->Add(htoftdcL_raw=new HMonHist2(*htoftdcL_rawA));
	}
    }
    if(!(l->fill("htoftdcR_raw",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(htoftdcR_rawA=new HMonHistAddon(mytemp));
	if(htoftdcR_rawA->getActive()==1)
	{
	    activeHists->Add(htoftdcR_raw=new HMonHist2(*htoftdcR_rawA));
	}
    }
    if(!(l->fill("htofadcL_raw",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(htofadcL_rawA=new HMonHistAddon(mytemp));
	if(htofadcL_rawA->getActive()==1)
	{
	    activeHists->Add(htofadcL_raw=new HMonHist2(*htofadcL_rawA));
	}
    }
    if(!(l->fill("htofadcR_raw",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(htofadcR_rawA=new HMonHistAddon(mytemp));
	if(htofadcR_rawA->getActive()==1)
	{
	    activeHists->Add(htofadcR_raw=new HMonHist2(*htofadcR_rawA));
	}
    }
    if(!(l->fill("htoftdcSum_hit",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(htoftdcSum_hitA=new HMonHistAddon(mytemp));
	if(htoftdcSum_hitA->getActive()==1)
	{
	    activeHists->Add(htoftdcSum_hit=new HMonHist2(*htoftdcSum_hitA));
	}
    }
    if(!(l->fill("htoftdcDiff_hit",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(htoftdcDiff_hitA=new HMonHistAddon(mytemp));
	if(htoftdcDiff_hitA->getActive()==1)
	{
	    activeHists->Add(htoftdcDiff_hit=new HMonHist2(*htoftdcDiff_hitA));
	}
    }
    if(!(l->fill("htofadcSum_hit",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(htofadcSum_hitA=new HMonHistAddon(mytemp));
	if(htofadcSum_hitA->getActive()==1)
	{
	    activeHists->Add(htofadcSum_hit=new HMonHist2(*htofadcSum_hitA));
	}
    }
    if(!(l->fill("htofadcPos_hit",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(htofadcPos_hitA=new HMonHistAddon(mytemp));
	if(htofadcPos_hitA->getActive()==1)
	{
	    activeHists->Add(htofadcPos_hit=new HMonHist2(*htofadcPos_hitA));
	}
    }
    if(!(l->fill("htofsum_hit",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(htofsum_hitA=new HMonHistAddon(mytemp));
	if(htofsum_hitA->getActive()==1)
	{
	    activeHists->Add(htofsum_hit=new HMonHist(*htofsum_hitA));
	}
    }
    if(!(l->fill("htoftheta_hit",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(htoftheta_hitA=new HMonHistAddon(mytemp));
	if(htoftheta_hitA->getActive()==1)
	{
	    activeHists->Add(htoftheta_hit=new HMonHist(*htoftheta_hitA));
	}
    }
    if(!(l->fill("htofphi_hit",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(htofphi_hitA=new HMonHistAddon(mytemp));
	if(htofphi_hitA->getActive()==1)
	{
	    activeHists->Add(htofphi_hit=new HMonHist(*htofphi_hitA));
	}
    }
    if(!(l->fill("htofyvsx_hit",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(htofyvsx_hitA=new HMonHistAddon(mytemp));
	if(htofyvsx_hitA->getActive()==1)
	{
	    activeHists->Add(htofyvsx_hit=new HMonHist2(*htofyvsx_hitA));
	}
    }
    if(!(l->fill("htofmulttoftofino_raw",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(hmulttoftofino_rawA=new HMonHistAddon(mytemp));
	if(hmulttoftofino_rawA->getActive()==1)
	{
	    activeHists->Add(hmulttoftofino_raw=new HMonHist(*hmulttoftofino_rawA));
	}
    }
     return kTRUE;
}
