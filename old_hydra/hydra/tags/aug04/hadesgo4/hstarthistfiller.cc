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
#include "htboxchan.h"
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
HHistArray*      HStartHistFiller::hstrips_hodo_tof=0;
HHistArray*      HStartHistFiller::hstrips_hodo_pos=0;
HHistArray*      HStartHistFiller::hstrips_hodo_mult=0;
HHistArray*      HStartHistFiller::hstrips_hodo_raw=0;
HHistArray*      HStartHistFiller::hstrips_hodo_both_raw=0;
HHistArray*      HStartHistFiller::hstrips_hodo_cal=0;
HHistArray*      HStartHistFiller::hstrips_start_veto_raw=0;
HHistArray*      HStartHistFiller::hstrips_start_vetotemp_raw=0;
HTrendArray*     HStartHistFiller::hstartmeanstriptrend_raw=0;
HHistArray*      HStartHistFiller::hmult_start_veto_raw=0;
HHistArray*      HStartHistFiller::htime_start_raw=0;
HTrendArray*     HStartHistFiller::htime_trend_start=0;
HTrendArray*     HStartHistFiller::htime_trend_hodo=0;
HHistArray*      HStartHistFiller::htime_hodo_temp=0;
HHistArray*      HStartHistFiller::htime_start_temp=0;
HHistArray*      HStartHistFiller::htime_veto_raw=0;
HMonHist2*       HStartHistFiller::hstart_veto_corr_raw=0;
//Scalers
HTrendArray*     HStartHistFiller::hscaler[8]={0,0,0,0,0,0,0,0};
HHistArray*      HStartHistFiller::hscaler_tmp=0;
// Hist Addon Pointers
HMonHistAddon* HStartHistFiller::hstrips_hodo_tofA=0;
HMonHistAddon* HStartHistFiller::hstrips_hodo_posA=0;
HMonHistAddon* HStartHistFiller::hstrips_hodo_both_rawA=0;
HMonHistAddon* HStartHistFiller::hstrips_hodo_multA=0;
HMonHistAddon* HStartHistFiller::hstrips_hodo_rawA=0;
HMonHistAddon* HStartHistFiller::hstrips_hodo_calA=0;
HMonHistAddon* HStartHistFiller::hstrips_start_veto_rawA=0;
HMonHistAddon* HStartHistFiller::hstrips_start_vetotemp_rawA=0;
HMonHistAddon* HStartHistFiller::hstartmeanstriptrend_rawA=0;
HMonHistAddon* HStartHistFiller::hmult_start_veto_rawA=0;
HMonHistAddon* HStartHistFiller::htime_start_rawA=0;
HMonHistAddon* HStartHistFiller::htime_veto_rawA=0;
HMonHistAddon* HStartHistFiller::hstart_veto_corr_rawA=0;
HMonHistAddon* HStartHistFiller::htime_trend_hodoA=0;
HMonHistAddon* HStartHistFiller::htime_trend_startA=0;
HMonHistAddon* HStartHistFiller::htime_start_tempA=0;
HMonHistAddon* HStartHistFiller::htime_hodo_tempA=0;
//Scaler
HMonHistAddon* HStartHistFiller::hscalerA[8]={0,0,0,0,0,0,0,0};
HMonHistAddon* HStartHistFiller::hscaler_tmpA=0;

ClassImp(HStartHistFiller)

HStartHistFiller::HStartHistFiller(const char* name,const char* title)
    : TNamed(name,title)
{
  colorsModule.Set(3);
  colorsStrips.Set(16);
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
    Int_t myMod=-1;
    Bool_t found_veto=kFALSE;

    iterraw->Reset();
    while ((raw=(HStartRaw *)iterraw->Next())!=0)
    {
         raw->getAddress(mod,strip);
         if(mod==1)found_veto=kTRUE;
    }

    iterraw->Reset();
    while ((raw=(HStartRaw *)iterraw->Next())!=0)
    {
	raw->getAddress(mod,strip);
	if(mod>=0)
	{
	    if(mod<3)
	    {
		(mod>=myMod)? myMod=mod:myMod=myMod;
		time=raw->getTime();
		if(mod==0)rawmultstart++;
		if(mod==1)rawmultveto++;
		if(hstrips_start_veto_raw)
		{
		    hstrips_start_veto_raw->fill(0,mod,strip);
		    if(found_veto&&mod==0) hstrips_start_veto_raw->fill(0,2,strip);
		}
		if(hstrips_start_vetotemp_raw)
		{
		    hstrips_start_vetotemp_raw->fill(0,mod,strip);
		    if(found_veto&&mod==0) hstrips_start_vetotemp_raw->fill(0,2,strip);
		}
		if(htime_start_raw && mod==0 && htime_start_temp)
		{
		    htime_start_raw ->fill(0,strip,time);
		    htime_start_temp->fill(0,strip,time);
		}
		if(htime_veto_raw  && mod==1) htime_veto_raw->fill(0,strip,time);
	    }
	    else
	    { //Hodoscopes
		if(hstrips_hodo_raw)hstrips_hodo_raw->fill(0,mod-3,strip);
		if(hstrips_hodo_both_raw){
		    if((raw->getTimeR()>-1 && raw->getTimeL()>-1)){
			hstrips_hodo_both_raw->fill(0,mod-3,strip);
		    }
		    if(raw->getTimeM()>-1){
			hstrips_hodo_both_raw->fill(0,mod-3,strip);
		    }
		    if(raw->getTime()>-1){
			hstrips_hodo_both_raw->fill(0,mod-3,strip);
		    }
		}
	    }
	}
    }
    if(hmult_start_veto_raw&&myMod>=0&&myMod<3)
    {
	hmult_start_veto_raw->fill(0,0,rawmultstart);
	hmult_start_veto_raw->fill(0,1,rawmultveto);
    }

    if(hstart_veto_corr_raw&&found_veto)
    {
	iterraw->Reset();
	Int_t vetostrip;
	HIterator* iterveto=iterraw;
	while ((raw=(HStartRaw *)iterraw->Next())!=0)
	{
	    raw->getAddress(mod,strip);
	    if(mod==0)
	    {
		iterveto->Reset();
		while ((raw=(HStartRaw *)iterveto->Next())!=0)
		{
		    raw->getAddress(mod,vetostrip);
		    if(mod==1) hstart_veto_corr_raw->getP()->Fill((Axis_t)strip,(Axis_t)vetostrip);
		}
	    }
	}
    }
}
void HStartHistFiller::fillCal(HIterator* itercal)
{
    Int_t nMultHod[3] = {0,0,0};
    Int_t nMod=0;
    Int_t nStrip=0;
    HStartCal* cal=0;
    Float_t fTimeL,fTimeR,fTimeM,fTime,fTof,fPos;
    Bool_t filled=kFALSE;
    Int_t myMod=-1;
    while ((cal=(HStartCal *)itercal->Next())!=0)
    {
	fTimeL = cal->getTimeL();
	fTimeR = cal->getTimeR();
	fTimeM = cal->getTimeM();
	fTime = cal->getTime();
	fTof = cal->getTof();
	fPos = cal->getPos();
	cal->getAddress(nMod,nStrip);
	if(nMod>=0&&nStrip>=0)
	{
            filled=kTRUE;
	    (nMod>=myMod)? myMod=nMod:myMod=myMod;
	    if(nMod>=3) nMultHod[nMod-3] = cal->getMult(nMod);

	    if((fTimeL>-1 && fTimeR>-1) || fTimeM >-1 || fTime>-1)
	    {
		if(nMod>=3 && hstrips_hodo_cal)hstrips_hodo_cal->fill(0,nMod-3,nStrip);
		if(nMod>=3 && htime_hodo_temp) htime_hodo_temp ->fill(0,nStrip,fTof);
	    }
	    if(hstrips_hodo_tof&&nMod>=3){
		hstrips_hodo_tof->fill(0,nMod-3,fTof);
	    }
	    if(hstrips_hodo_pos&&nMod>=3){
		hstrips_hodo_pos->fill(0,nMod-3,fPos);
	    }
	}
    }
    if(hstrips_hodo_mult&&filled&&myMod>=3)
    {
	hstrips_hodo_mult->fill(0,0,nMultHod[0]);
	hstrips_hodo_mult->fill(0,1,nMultHod[1]);
	hstrips_hodo_mult->fill(0,2,nMultHod[2]);
    }
}
void HStartHistFiller::fillHit(HIterator* iterhit)
{
    HStartHit* hit=0;
    while ((hit=(HStartHit *)iterhit->Next())!=0)
    {

    }
}
void HStartHistFiller::fillScaler(HIterator* iterbox)
{
    Int_t chan,scaler1=0,scaler2=0,scaler3=0,scaler4=0;
    HTBoxChan* box=0;

    while ((box=(HTBoxChan *)iterbox->Next())!=0)
    {
	box->get(chan,scaler1,scaler2,scaler3,scaler4);
	if(hscaler[chan])
	{
	    hscaler[chan]->fill(0,0,scaler1);
	    hscaler[chan]->fill(0,1,scaler2);
	}
    }
}

void HStartHistFiller::fillTrend()
{

    for(Int_t m=0;m<3;m++){
	if(hstrips_start_vetotemp_raw&&hstartmeanstriptrend_raw){
	    hstartmeanstriptrend_raw->fill(0,m,hstrips_start_vetotemp_raw->getP()->GetMean());
	}
    }
    if(hstrips_start_vetotemp_raw)hstrips_start_vetotemp_raw->getP()->Reset();

    for(Int_t s=0;s<8;s++)
    {
	if(htime_start_temp)
	    if(htime_start_temp->getP(0,s))
	    {
		if(htime_trend_start)
		{
		    htime_trend_start->fill(0,s,htime_start_temp->getP(0,s)->GetMean());
		    htime_start_temp->getP(0,s)->Reset();
		}
	    }
	}
    for(Int_t s=0;s<16;s++)
    {
	if(htime_hodo_temp)
	    if(htime_hodo_temp->getP(0,s))
	    {
		if(htime_trend_hodo)htime_trend_hodo->fill(0,s,htime_hodo_temp->getP(0,s)->GetMean());
	    }
    }
    if(hscaler_tmp)
    {
	for(int i=0;i<8;i++)
	{
	    if(hscaler_tmp->getP(0,i))
	    {
		//			hscaler_input->fill(0,i,hscaler_tmp->getP(0,i)->GetMean());
		//		cout<<"In  nr "<<i<<" Value: "<<hscaler_tmp->getP(0,i)->GetMean()<<endl;
	    }
	    if(hscaler_tmp->getP(0,i+4))
	    {
		//			hscaler_output->fill(0,i,hscaler_tmp->getP(0,i+4)->GetMean());
		//		cout<<"Out  nr "<<i<<" Value: "<<hscaler_tmp->getP(0,i+4)->GetMean()<<endl;
	    }
	}
    }
}
void   HStartHistFiller::putStartParams(HParamList* l,TList* histpool)
{
	// Puts all params of HStartHistFiller to the parameter list of
	// HParamList (which ist used by the io);

	Text_t mytemp[500];
	char cBuff[100];
	l->add("colorsModule"           ,colorsModule);
	l->add("colorsStrips"           ,colorsStrips);
	l->add("colorsCuts"             ,colorsCuts  );
	hstrips_hodo_tofA->getText(mytemp);
	l->add("hstrips_hodo_tof"            ,mytemp,500);
	hstrips_hodo_posA->getText(mytemp);
	l->add("hstrips_hodo_pos"            ,mytemp,500);
	hstrips_hodo_multA->getText(mytemp);
	l->add("hstrips_hodo_mult"           ,mytemp,500);
	hstrips_hodo_rawA->getText(mytemp);
	l->add("hstrips_hodo_raw"            ,mytemp,500);
	hstrips_hodo_both_rawA->getText(mytemp);
	l->add("hstrips_hodo_both_raw"       ,mytemp,500);
	hstrips_hodo_calA->getText(mytemp);
	l->add("hstrips_hodo_cal"            ,mytemp,500);
	hstrips_start_veto_rawA->getText(mytemp);
	l->add("hstrips_start_veto_raw"      ,mytemp,500);
	hstrips_start_vetotemp_rawA->getText(mytemp);
	l->add("hstrips_start_vetotemp_raw"  ,mytemp,500);
	hstartmeanstriptrend_rawA->getText(mytemp);
	l->add("hstartmeanstriptrend_raw"    ,mytemp,500);
	hmult_start_veto_rawA->getText(mytemp);
	l->add("hmult_start_veto_raw"        ,mytemp,500);
	htime_start_rawA->getText(mytemp);
	l->add("htime_start_raw"             ,mytemp,500);
	htime_veto_rawA->getText(mytemp);
	l->add("htime_veto_raw"              ,mytemp,500);
	hstart_veto_corr_rawA->getText(mytemp);
	l->add("hstart_veto_corr_raw"        ,mytemp,500);
	htime_trend_hodo->getText(mytemp);
	l->add("htime_trend_hodo"            ,mytemp,500);
	htime_trend_start->getText(mytemp);
	l->add("htime_trend_start"           ,mytemp,500);
	htime_start_temp->getText(mytemp);
	l->add("htime_start_temp"            ,mytemp,500);
	htime_hodo_temp->getText(mytemp);
	l->add("htime_hodo_temp"             ,mytemp,500);
	for(int i=0;i<8;i++){
		snprintf(cBuff,sizeof(cBuff),"hscaler_%d",i);
		hscaler[i]->getText(mytemp);
		l->add(cBuff,mytemp,500);
	}
}
Bool_t HStartHistFiller::getStartParams(HParamList* l,TList* histpool,TList* activeHists)
{
    if(!(l->fill("colorsModule",&colorsModule))) return kFALSE;
    if(!(l->fill("colorsStrips",&colorsStrips))) return kFALSE;
    if(!(l->fill("colorsCuts"  ,&colorsCuts  ))) return kFALSE;

    Text_t mytemp[500];
    if(!(l->fill("hstrips_hodo_raw",mytemp,500))) {return kFALSE;}
    else {
		histpool->Add(hstrips_hodo_rawA=new HMonHistAddon(mytemp));
	if(hstrips_hodo_rawA->getActive()==1)
	{
	    activeHists->Add(hstrips_hodo_raw=new HHistArray(*hstrips_hodo_rawA));
	    hstrips_hodo_raw->getP(0,0)->SetLineColor(colorsModule[0]);
	    hstrips_hodo_raw->getP(0,1)->SetLineColor(colorsModule[1]);

	}
	}
	if(!(l->fill("hstrips_hodo_mult",mytemp,500))) {return kFALSE;}
    else {
		histpool->Add(hstrips_hodo_multA=new HMonHistAddon(mytemp));
		if(hstrips_hodo_multA->getActive()==1) {
			activeHists->Add(hstrips_hodo_mult=
				new HHistArray(*hstrips_hodo_multA));
			hstrips_hodo_mult->getP(0,0)->SetLineColor(colorsModule[0]);
			hstrips_hodo_mult->getP(0,1)->SetLineColor(colorsModule[1]);
			hstrips_hodo_mult->getP(0,2)->SetLineColor(colorsModule[2]);

		}
    }if(!(l->fill("hstrips_hodo_both_raw",mytemp,500))) {return kFALSE;}
    else {
		histpool->Add(hstrips_hodo_both_rawA=new HMonHistAddon(mytemp));
		if(hstrips_hodo_both_rawA->getActive()==1) {
			activeHists->Add(hstrips_hodo_both_raw=
				new HHistArray(*hstrips_hodo_both_rawA));
			hstrips_hodo_both_raw->getP(0,0)->SetLineColor(colorsModule[0]);
			hstrips_hodo_both_raw->getP(0,1)->SetLineColor(colorsModule[1]);

		}
    }
	if(!(l->fill("hstrips_hodo_tof",mytemp,500))) {return kFALSE;}
    else {
		histpool->Add(hstrips_hodo_tofA=new HMonHistAddon(mytemp));
		if(hstrips_hodo_tofA->getActive()==1) {
			activeHists->Add(hstrips_hodo_tof=
				new HHistArray(*hstrips_hodo_tofA));
			hstrips_hodo_tof->getP(0,0)->SetLineColor(colorsModule[0]);
			hstrips_hodo_tof->getP(0,1)->SetLineColor(colorsModule[1]);
		}
	}
	if(!(l->fill("hstrips_hodo_pos",mytemp,500))) {return kFALSE;}
    else {
		histpool->Add(hstrips_hodo_posA=new HMonHistAddon(mytemp));
		if(hstrips_hodo_posA->getActive()==1) {
			activeHists->Add(hstrips_hodo_pos=
				new HHistArray(*hstrips_hodo_posA));
			hstrips_hodo_pos->getP(0,0)->SetLineColor(colorsModule[0]);
			hstrips_hodo_pos->getP(0,1)->SetLineColor(colorsModule[1]);
		}
	}
	if(!(l->fill("hstrips_hodo_cal",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(hstrips_hodo_calA=new HMonHistAddon(mytemp));
	if(hstrips_hodo_calA->getActive()==1)
	{
	    activeHists->Add(hstrips_hodo_cal=new HHistArray(*hstrips_hodo_calA));
	    hstrips_hodo_cal->getP(0,0)->SetLineColor(colorsModule[0]);
	    hstrips_hodo_cal->getP(0,1)->SetLineColor(colorsModule[1]);

	}
    }
    if(!(l->fill("hstrips_start_veto_raw",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(hstrips_start_veto_rawA=new HMonHistAddon(mytemp));
	if(hstrips_start_veto_rawA->getActive()==1)
	{
	    activeHists->Add(hstrips_start_veto_raw=new HHistArray(*hstrips_start_veto_rawA));
	    hstrips_start_veto_raw->getP(0,0)->SetLineColor(colorsModule[0]);
	    hstrips_start_veto_raw->getP(0,1)->SetLineColor(colorsModule[1]);
            hstrips_start_veto_raw->getP(0,2)->SetLineColor(colorsModule[2]);
	}
    }
    if(!(l->fill("hstrips_start_vetotemp_raw",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(hstrips_start_vetotemp_rawA=new HMonHistAddon(mytemp));
	if(hstrips_start_vetotemp_rawA->getActive()==1)
	{
	    activeHists->Add(hstrips_start_vetotemp_raw=new HHistArray(*hstrips_start_vetotemp_rawA));
	    hstrips_start_vetotemp_raw->getP(0,0)->SetLineColor(colorsModule[0]);
	    hstrips_start_vetotemp_raw->getP(0,1)->SetLineColor(colorsModule[1]);
            hstrips_start_vetotemp_raw->getP(0,2)->SetLineColor(colorsModule[2]);
	}
    }
    if(!(l->fill("hstartmeanstriptrend_raw",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(hstartmeanstriptrend_rawA=new HMonHistAddon(mytemp));
	if(hstartmeanstriptrend_rawA->getActive()==1)
	{
	    activeHists->Add(hstartmeanstriptrend_raw=new HTrendArray(*hstartmeanstriptrend_rawA));
	    hstartmeanstriptrend_raw->getP(0,0)->SetLineColor(colorsModule[0]);
	    hstartmeanstriptrend_raw->getP(0,1)->SetLineColor(colorsModule[1]);
            hstartmeanstriptrend_raw->getP(0,2)->SetLineColor(colorsModule[2]);
	    hstartmeanstriptrend_raw->getP(0,0)->SetMarkerColor(colorsModule[0]);
	    hstartmeanstriptrend_raw->getP(0,1)->SetMarkerColor(colorsModule[1]);
            hstartmeanstriptrend_raw->getP(0,2)->SetMarkerColor(colorsModule[2]);
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
	if(!(l->fill("htime_trend_hodo",mytemp,500))) {return kFALSE;}
	else{
		histpool->Add(htime_trend_hodoA=new HMonHistAddon(mytemp));
		if(htime_trend_hodoA->getActive()==1){
			activeHists->Add(htime_trend_hodo= 
					new HTrendArray(*htime_trend_hodoA));	
			for(Int_t s=0;s<16;s++){
				htime_trend_hodo->getP(0,s)->SetLineColor(colorsStrips[s]);
			}
		}	
	}
	if(!(l->fill("htime_hodo_temp",mytemp,500))) {return kFALSE;}
	else{
		histpool->Add(htime_hodo_tempA=new HMonHistAddon(mytemp));
		if(htime_hodo_tempA->getActive()==1){
			activeHists->Add(htime_hodo_temp= 
					new HHistArray(*htime_hodo_tempA));	
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
	char cBuff[100];
	for(int i=0;i<8;i++){
        snprintf(cBuff,sizeof(cBuff),"hscaler_%d",i);
		if(!(l->fill(cBuff,mytemp,500))) {return kFALSE;}
		else{
			histpool->Add(hscalerA[i]=new HMonHistAddon(mytemp));
			if(hscalerA[i]->getActive()==1){	
				activeHists->Add(hscaler[i]=new HTrendArray(*hscalerA[i]));
				hscaler[i]->getP(0,0)->SetLineColor(colorsStrips[0]);
				hscaler[i]->getP(0,1)->SetLineColor(colorsStrips[1]);
			}
		}
	}
	
	if(!(l->fill("hscaler_tmp",mytemp,500))) {return kFALSE;}
	else{
		histpool->Add(hscaler_tmpA=new HMonHistAddon(mytemp));
		if(hscaler_tmpA->getActive()==1){
			activeHists->Add(hscaler_tmp= 
					new HHistArray(*hscaler_tmpA));	
		}	
	}
		
    return kTRUE;
}
