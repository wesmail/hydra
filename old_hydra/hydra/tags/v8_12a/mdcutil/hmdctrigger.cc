//*-- AUTHOR   : j.Markert

//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////////////
//
// HMdcTrigger
//
//
///////////////////////////////////////////////////////////////////////////////
using namespace std;
#include "hmdctrigger.h"
#include "hmdcdef.h"
#include "hdebug.h"
#include "hades.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hevent.h"
#include "heventheader.h"
#include "hcategory.h"
#include "hlinearcategory.h"
#include "hstartcal.h"
#include <fstream> 
#include <iostream> 
#include <iomanip>
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"

ClassImp(HMdcTrigger)

HMdcTrigger::HMdcTrigger(void) {
  calStartCat=NULL;
  iter_start=NULL;
  dotimecut = kFALSE;
  startbit=kFALSE;
  tofbit=kFALSE;
  tofinobit=kFALSE;
  fillcontrolhists=kFALSE;

  binnum0 =1;
  binnum1 =1;
  binnum2 =1;
  binnum3 =1;
  binnum4 =1;
  binnum5 =1;
  binnum6 =1;
  binnum7 =1;
  binnum8 =1;
  binnum9 =1;
  binnum10 =1;
  binnum11 =1;
  binnum12 =1;
  binnum13 =1;
  binnum14 =1;
  binnum15 =1;

  bin0 =1;
  bin1 =1;
  bin2 =1;
  bin3 =1;
  bin4 =1;
  bin5 =1;
  bin6 =1;
  bin7 =1;
  bin8 =1;
  bin9 =1;
  bin10 =1;
  bin11 =1;
  bin12 =1;
  bin13 =1;
  bin14 =1;
  bin15 =1;

  countnocut=0;
  countmultiplicityStartcut=0;
  counttimewindowStartcut=0;
  countskipedevents=0;

  maxbinnum=10000;
  maxX=9999;
  setDefaultTimeWindow();
  setStartMultiplicityCut(1,2);
  setVetoMultiplicityCut(0,1);
  setTofMultiplicityCut(1,2);
  setTofinoMultiplicityCut(1,2);

}

HMdcTrigger::HMdcTrigger(Text_t* name,Text_t* title)
                 :  HReconstructor(name,title) {
  calStartCat=NULL;
  iter_start=NULL;
  dotimecut = kFALSE;
  startbit=kFALSE;
  tofbit=kFALSE;
  tofinobit=kFALSE;
  fillcontrolhists=kFALSE;

  binnum0 =1;
  binnum1 =1;
  binnum2 =1;
  binnum3 =1;
  binnum4 =1;
  binnum5 =1;
  binnum6 =1;
  binnum7 =1;
  binnum8 =1;
  binnum9 =1;
  binnum10 =1;
  binnum11 =1;
  binnum12 =1;
  binnum13 =1;
  binnum14 =1;
  binnum15 =1;

  bin0 =1;
  bin1 =1;
  bin2 =1;
  bin3 =1;
  bin4 =1;
  bin5 =1;
  bin6 =1;
  bin7 =1;
  bin8 =1;
  bin9 =1;
  bin10 =1;
  bin11 =1;
  bin12 =1;
  bin13 =1;
  bin14 =1;
  bin15 =1;

  countnocut=0;
  countmultiplicityStartcut=0;
  counttimewindowStartcut=0;
  countskipedevents=0;

  maxbinnum=10000;
  maxX=9999;
  setDefaultTimeWindow();
  setStartMultiplicityCut(0,10);
  setVetoMultiplicityCut(0,10);
  setTofMultiplicityCut(0,10);
  setTofinoMultiplicityCut(0,10);

}


HMdcTrigger::~HMdcTrigger(void) {
  // destructor deletes the iterator
  if (iter_start) delete iter_start;
  iter_start=0;

    if(file && fillcontrolhists) writeHist(file);

    if (fNameRootTrigger)  delete fNameRootTrigger;
}

void HMdcTrigger::setDefaultTimeWindow()
{
    // Sets the the default values for the cut in starttime for each single strip

    //setTimeWindowStart(37.2,38.2,36.3,37.5,37.3,38.5,36.5,37.7,0,1000,0,1000,0,1000,0,1000);
    setTimeWindowStart(-1000.,1000.,-1000.,1000.,-1000.,1000.,-1000.,1000.,-1000.,1000.,-1000.,1000.,-1000.,1000.,-1000.,1000.);
    setTimeWindowVeto (-1000.,1000.,-1000.,1000.,-1000.,1000.,-1000.,1000.,-1000.,1000.,-1000.,1000.,-1000.,1000.,-1000.,1000.);

}


void HMdcTrigger::setOutputRoot(Char_t *c)
{
    // Sets rootfile output of HMdcTrigger where all created histograms were written.
    //
    if (fNameRootTrigger) delete fNameRootTrigger;
    fNameRootTrigger = new Char_t[strlen(c)+1];
    strcpy(fNameRootTrigger, c);
}

Bool_t HMdcTrigger::init(void) {
  cout<<"init hmdctrigger"<<endl;
    calStartCat=gHades->getCurrentEvent()->getCategory(catStartCal);       //????
  if (!calStartCat) {
    calStartCat=gHades->getSetup()->getDetector("Start")->buildCategory(catStartCal);
    if (!calStartCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catStartCal,calStartCat,"Start");
  }
  iter_start=(HIterator *)calStartCat->MakeIterator("native");


  //TFile *file = NULL;
  if (fNameRootTrigger && fillcontrolhists)
  {file = new TFile(fNameRootTrigger,"RECREATE");}
  if (file && fillcontrolhists) createHist(file);

 cout<<"****************************************************************************"<<endl;
 cout<<"************************HMdcTrigger*****************************************"<<endl;
 if(file)cout<<"*"<<" Outputfile: "<<fNameRootTrigger<<endl;
 if(!file)cout<<"*"<<" no Outputfile set"<<endl;

 if(tofbit)cout<<"*"<<" Tof trigger selected"<<endl;
 if(startbit)cout<<"*"<<" Start trigger selected"<<endl;
 if(tofinobit)cout<<"*"<<" Tofino trigger selected"<<endl;
 if((!startbit)&&(!tofbit)&&(!tofinobit))cout<<"*"<<" no trigger selected"<<endl;

 cout<<"* Start  multiplicity cut between "<<startmultmin<<" and "<<startmultmax<<endl;
 cout<<"* Veto   multiplicity cut between "<<vetomultmin<<" and "<<vetomultmax<<endl;
 cout<<"* Tof    multiplicity cut between "<<tofmultmin<<" and "<<tofmultmax<<endl;
 cout<<"* Tofino multiplicity cut between "<<tofinomultmin<<" and "<<tofinomultmax<<endl;

 if(dotimecut)
 {
     cout<<"*"<<" strip0 window from "<<minstrip0<<" to "<<maxstrip0<<endl;
     cout<<"*"<<" strip1 window from "<<minstrip1<<" to "<<maxstrip1<<endl;
     cout<<"*"<<" strip2 window from "<<minstrip2<<" to "<<maxstrip2<<endl;
     cout<<"*"<<" strip3 window from "<<minstrip3<<" to "<<maxstrip3<<endl;
     cout<<"*"<<" strip4 window from "<<minstrip4<<" to "<<maxstrip4<<endl;
     cout<<"*"<<" strip5 window from "<<minstrip5<<" to "<<maxstrip5<<endl;
     cout<<"*"<<" strip6 window from "<<minstrip6<<" to "<<maxstrip6<<endl;
     cout<<"*"<<" strip7 window from "<<minstrip7<<" to "<<maxstrip7<<endl;
 }

 if(!dotimecut)cout<<"* No time window set for strips"<<endl;

 cout<<"****************************************************************************"<<endl;
 cout<<"****************************************************************************"<<endl;


   

  fActive=kTRUE;
  return kTRUE;

}

void HMdcTrigger::writeHist(TFile* file)
{
    // All created histograms are written to a rootfile.
    file->cd();
    hstartcutstat->Write();
    hstart_time_s0to7->Write();
    hstart_time_s0to7_clean->Write();

    hstart_time_s8to15->Write();
    hstart_time_s8to15_clean->Write();

    hstartvetodistribution->Write();
    hstartvetodistribution_clean->Write();

    for(Int_t i=0;i<8;i++){
	hstart_time[i]->Write();
	hstart_time_clean[i]->Write();;
       // hstart_time_history[i]->Write();
       // hstart_time_history_clean[i]->Write();
        hveto_time[i]->Write();
	hveto_time_clean[i]->Write();;
       // hveto_time_history[i]->Write();
       // hveto_time_history_clean[i]->Write();

    }
    // close root file
    file->Save();
    file->Close();
    delete file;
}


void HMdcTrigger::createHist(TFile* file)
{
  file->cd();
  cout<<"creating histograms"<<endl;
  hstartcutstat=new TH1F("hstartcutstat","statistics of cuts on start" ,20,0,20);
 
  hstart_time_s0to7=new TH1F("hstart_time_s01234567","start time spectrum" ,400,-20,20);
  hstart_time_s0to7_clean=new TH1F("hstart_time_s01234567_clean","start time spectrum clean" ,400,-20,20);
  hstart_time_s8to15=new TH1F("hstart_time_s8to15","veto time spectrum" ,400,-20,20);
  hstart_time_s8to15_clean=new TH1F("hstart_time_s8to15_clean","veto time spectrum clean" ,400,-20,20);

  hstartvetodistribution      = new TH2F("start_veto_distribution","start veto distribution",100,0,10,100,0,10);
  hstartvetodistribution->SetXTitle("start multiplicity");
  hstartvetodistribution->SetYTitle("veto multiplicity");

  hstartvetodistribution_clean= new TH2F("start_veto_distribution_clean","start veto distribution clean",100,0,10,100,0,10);
  hstartvetodistribution_clean->SetXTitle("start multiplicity");
  hstartvetodistribution_clean->SetYTitle("veto multiplicity");

  for(Int_t i=0;i<8;i++){
     Char_t title[60], tmp[60];

    sprintf(title,"%s%i","start time strip",i);
    sprintf(tmp, "%s%i", "hstart_time_s",i);
    hstart_time[i] = new TH1F(tmp, title,400 ,-20,20);
    sprintf(title,"%s%i%s","start time strip",i," clean");
    sprintf(tmp, "%s%i%s", "hstart_time_s",i,"_clean");
    hstart_time_clean[i] = new TH1F(tmp, title,400 ,-20,20);

    sprintf(title,"%s%i","start time history strip",i);
    sprintf(tmp, "%s%i", "hstart_time_history_s",i);
   // hstart_time_history[i] = new TH1F(tmp, title,maxbinnum ,0,maxbinnum);
    sprintf(title,"%s%i%s","start time history strip",i," clean");
    sprintf(tmp, "%s%i%s", "hstart_time_history_s",i,"_clean");
   // hstart_time_history_clean[i] = new TH1F(tmp, title,maxbinnum ,0,maxbinnum);

  }


 for(Int_t i=0;i<8;i++){
    Char_t title[60], tmp[60];

    sprintf(title,"%s%i","veto time strip",i);
    sprintf(tmp, "%s%i", "hveto_time_s",i);
    hveto_time[i] = new TH1F(tmp, title,400 ,-20,20);
    sprintf(title,"%s%i%s","veto time strip",i," clean");
    sprintf(tmp, "%s%i%s", "hveto_time_s",i,"_clean");
    hveto_time_clean[i] = new TH1F(tmp, title,400 ,-20,20);

    sprintf(title,"%s%i","veto time history strip",i);
    sprintf(tmp, "%s%i", "hveto_time_history_s",i);
   // hveto_time_history[i] = new TH1F(tmp, title,maxbinnum ,0,maxbinnum);
    sprintf(title,"%s%i%s","veto time history strip",i," clean");
    sprintf(tmp, "%s%i%s", "hveto_time_history_s",i,"_clean");
   // hveto_time_history_clean[i] = new TH1F(tmp, title,maxbinnum ,0,maxbinnum);

  }

}





Int_t HMdcTrigger::execute(void) {
    countnocut++; // counts all events
    if(fillcontrolhists)hstartcutstat->SetBinContent(1,countnocut);

    HStartCal *startcal=NULL;
    Int_t startmod;
    Int_t sstrip;
    Float_t stime;
    Int_t strip0=0;
    Int_t strip1=0;
    Int_t strip2=0;
    Int_t strip3=0;
    Int_t strip4=0;
    Int_t strip5=0;
    Int_t strip6=0;
    Int_t strip7=0;
    Int_t strip8=0;
    Int_t strip9=0;
    Int_t strip10=0;
    Int_t strip11=0;
    Int_t strip12=0;
    Int_t strip13=0;
    Int_t strip14=0;
    Int_t strip15=0;

    //    Float_t start_time=0;
    Int_t count_start=0;
    Int_t count_start_check=0;
    Int_t mul_start=0;
    Int_t mul_veto=0;
   // I have one start time for all hits in this event but if more start times that I skip
    iter_start ->Reset();
    while ((startcal=(HStartCal*)iter_start->Next())!= 0) {
      //first I count hits in Start if count>1 than I skip such event for the moment 
      if(startcal->getStrip()<8 && (startcal->getModule()==0)) mul_start++;
      if(startcal->getStrip()<8 && (startcal->getModule()==1)) mul_veto++;
      count_start++;
    }
    if(fillcontrolhists) hstartvetodistribution->Fill(mul_start,mul_veto);        // fill histogram for start distribution (no cuts)
    
    iter_start ->Reset();
    // now I know number of hits in Start and for the moment I choose only those where there is only one 
    //****************************************************************************************************
    //****************************************************************************************************
    // trigger conditions:
    //
    // Int_t  getTBit(void)     - return the integer value
    // Bool_t IsTBit(Int_t i)   - check bit i of the pattern
    // Bool_t IsTofBit(void)    - check TOF bit of the pattern (TOF&START)
    // Bool_t IsStartBit(void)  - check START bit of the pattern (START&!VETO)
    // Bool_t IsTofinoBit(void) - check TOFINO bit of the pattern (TOFINO)
    //
    //
    //****************************************************************************************************
    //****************************************************************************************************
    //if(count_start==1) {
   /*  This part does not work anylonger, because of missing Tofbit
    if(
       ((startbit) && gHades->getCurrentEvent()->getHeader()->IsStartBit())||     // startbit set
       ((tofbit)   && gHades->getCurrentEvent()->getHeader()->IsTofBit())||       // tofbit set
       ((tofinobit)&& gHades->getCurrentEvent()->getHeader()->IsTofinoBit())||    // tofinobit set
       ((!startbit)&&(!tofbit)&&(!tofinobit))
      )                                    // no bit set

    {
    */
    tofbit=kTRUE;
    if(
	   ((startbit)&&((mul_start>=startmultmin&&mul_start<startmultmax)        // startbit set
			 &&(mul_veto>=vetomultmin&&mul_veto<vetomultmax)))||
	   ((tofbit)&&((mul_start>=startmultmin&&mul_start<startmultmax)          // tofbit set
		       &&(mul_veto>=vetomultmin&&mul_veto<vetomultmax)))||
	   (((!tofbit)&&(!startbit)&&(!tofinobit))                                  // no bit set
	    &&((mul_start>=startmultmin&&mul_start<startmultmax)
	               &&(mul_veto>=vetomultmin&&mul_veto<vetomultmax)))

	  )
	{
            countmultiplicityStartcut++; // counts events after multiplicity cut
	    if(fillcontrolhists){
		hstartvetodistribution_clean->Fill(mul_start,mul_veto);
                hstartcutstat->SetBinContent(2,countmultiplicityStartcut);
 
	    }               // fill histogram for start distribution with the cut conditions
	    iter_start ->Reset();
	    while ((startcal=(HStartCal*) iter_start->Next())!= 0) {
		count_start_check++;
	       // if( count_start_check>1) printf("I have a problem check>1!\n");

		startmod=startcal->getModule();
		if(startmod==0){
		    sstrip  =startcal->getStrip();
		}else
		{
                 sstrip  =startcal->getStrip()+8;
		}
		stime   =startcal->getTime();
		//printf("strip,time%i %f\n",sstrip,stime);
                
               // cout<<"test "<<startmod<<" "<<sstrip<<endl;

		strip0=0;
		strip1=0;
		strip2=0;
		strip3=0;
		strip4=0;
		strip5=0;
		strip6=0;
		strip7=0;
		strip8=0;
		strip9=0;
		strip10=0;
		strip11=0;
		strip12=0;
		strip13=0;
		strip14=0;
		strip15=0;

		// if start time is in the start time  peak from this strip
		// cut the same as MDC cuts
		if(file) file->cd();
		//start
		if(dotimecut){
		    if(sstrip==0) {
			strip0=1;
			if(file){
			    if(fillcontrolhists){
				hstart_time[0]->Fill(stime);
				hstart_time_s0to7->Fill(stime);
			       // if (bin0<maxX)hstart_time_history[0]->SetBinContent(bin0,stime);
				bin0++;}}}

		    if(sstrip==1) {
			strip1=1;
			if(file){
                            if(fillcontrolhists){
			    hstart_time[1]->Fill(stime);
			    hstart_time_s0to7->Fill(stime);
			    //if(bin1<maxX)hstart_time_history[1]->SetBinContent(bin1,stime);
			    bin1++;}}}

		    if(sstrip==2) {
			strip2=1;
			if(file){
                            if(fillcontrolhists){
			    hstart_time[2]->Fill(stime);
			    hstart_time_s0to7->Fill(stime);
			   // hstart_time_history[2]->SetBinContent(bin2,stime);
			    bin2++;}}}

		    if(sstrip==3) {
			strip3=1;
			if(file){
                            if(fillcontrolhists){
			    hstart_time[3]->Fill(stime);
			    hstart_time_s0to7->Fill(stime);
			   // if(bin3<maxX)hstart_time_history[3]->SetBinContent(bin3,stime);
			    bin3++;}}}
                     //
		    if(sstrip==4) {
			strip4=1;
			if(file){
                            if(fillcontrolhists){
			    hstart_time[4]->Fill(stime);
			    hstart_time_s0to7->Fill(stime);
			   // if(bin4<maxX)hstart_time_history[4]->SetBinContent(bin4,stime);
			    bin4++;}}}



		    if(sstrip==5) {
			strip5=1;
			if(file){
                            if(fillcontrolhists){
			    hstart_time[5]->Fill(stime);
			    hstart_time_s0to7->Fill(stime);
			   // if(bin5<maxX)hstart_time_history[5]->SetBinContent(bin5,stime);
			    bin5++;}}}


		    if(sstrip==6) {
			strip6=1;
			if(file){
                            if(fillcontrolhists){
			    hstart_time[6]->Fill(stime);
			    hstart_time_s0to7->Fill(stime);
			   // if(bin6<maxX)hstart_time_history[6]->SetBinContent(bin6,stime);
			    bin6++;}}}


		    if(sstrip==7) {
			strip7=1;
			if(file){
                            if(fillcontrolhists){
			    hstart_time[7]->Fill(stime);
			    hstart_time_s0to7->Fill(stime);
			   // if(bin7<maxX)hstart_time_history[7]->SetBinContent(bin7,stime);
			    bin7++;}}}

		    //veto
		    if(sstrip==8) {
			strip8=1;
			if(file){
                            if(fillcontrolhists){
			    hveto_time[0]->Fill(stime);
			    hstart_time_s8to15->Fill(stime);
			   // if(bin8<maxX)hveto_time_history[0]->SetBinContent(bin8,stime);
			    bin8++;}}}

		    if(sstrip==9) {
			strip9=1;
			if(file){
                            if(fillcontrolhists){
			    hveto_time[1]->Fill(stime);
			    hstart_time_s8to15->Fill(stime);
			   // if(bin9<maxX)hveto_time_history[1]->SetBinContent(bin9,stime);
			    bin9++;}}}

		    if(sstrip==10) {
			strip10=1;
			if(file){
                            if(fillcontrolhists){
			    hveto_time[2]->Fill(stime);
			    hstart_time_s8to15->Fill(stime);
			   // if(bin10<maxX)hveto_time_history[2]->SetBinContent(bin10,stime);
			    bin10++;}}}

		    if(sstrip==11) {
			strip11=1;
			if(file){
                            if(fillcontrolhists){
			    hveto_time[3]->Fill(stime);
			    hstart_time_s8to15->Fill(stime);
			   // if(bin11<maxX)hveto_time_history[3]->SetBinContent(bin11,stime);
			    bin11++;}}}
                    //
                     if(sstrip==12) {
			strip12=1;
			if(file){
                            if(fillcontrolhists){
			    hveto_time[4]->Fill(stime);
			    hstart_time_s8to15->Fill(stime);
			   // if(bin12<maxX)hveto_time_history[4]->SetBinContent(bin12,stime);
			    bin12++;}}}


                     if(sstrip==13) {
			strip13=1;
			if(file){
                            if(fillcontrolhists){
			    hveto_time[5]->Fill(stime);
			    hstart_time_s8to15->Fill(stime);
			   // if(bin13<maxX)hveto_time_history[5]->SetBinContent(bin13,stime);
			    bin13++;}}}


                      if(sstrip==14) {
			strip14=1;
			if(file){
                            if(fillcontrolhists){
			    hveto_time[6]->Fill(stime);
			    hstart_time_s8to15->Fill(stime);
			    //if(bin14<maxX)hveto_time_history[6]->SetBinContent(bin14,stime);
			    bin14++;}}}


                      if(sstrip==15) {
			strip15=1;
			if(file){
                            if(fillcontrolhists){
			    hveto_time[7]->Fill(stime);
			    hstart_time_s8to15->Fill(stime);
			    //if(bin15<maxX)hveto_time_history[7]->SetBinContent(bin15,stime);
			    bin15++;}}}

		}

		if(dotimecut){
		    //start
		    if(sstrip==0&&((stime>minstrip0)&&(stime<maxstrip0))) {
			strip0=1;
			if(file){
                            if(fillcontrolhists){
			    hstart_time_clean[0]->Fill(stime);
			    hstart_time_s0to7_clean->Fill(stime);
			    //if(binnum0<maxX)hstart_time_history_clean[0]->SetBinContent(binnum0,stime);
			    binnum0++;}}}
		    if(sstrip==1&&((stime>minstrip1)&&(stime<maxstrip1))) {
			strip1=1;
			if(file){
                            if(fillcontrolhists){
			    hstart_time_clean[1]->Fill(stime);
			    hstart_time_s0to7_clean->Fill(stime);
			   // if(binnum1<maxX)hstart_time_history_clean[1]->SetBinContent(binnum1,stime);
			    binnum1++;}}}
		    if(sstrip==2&&((stime>minstrip2)&&(stime<maxstrip2))) {
			strip2=1;
			if(file){
                            if(fillcontrolhists){
			    hstart_time_clean[2]->Fill(stime);
			    hstart_time_s0to7_clean->Fill(stime);
			   // if(binnum2<maxX)hstart_time_history_clean[2]->SetBinContent(binnum2,stime);
			    binnum2++;}}}
		    if(sstrip==3&&((stime>minstrip3)&&(stime<maxstrip3))) {
			strip3=1;
			if(file){
                            if(fillcontrolhists){
			    hstart_time_clean[3]->Fill(stime);
			    hstart_time_s0to7_clean->Fill(stime);
			   // if(binnum3<maxX)hstart_time_history_clean[3]->SetBinContent(binnum3,stime);
			    binnum3++;}}}

		    if(sstrip==4&&((stime>minstrip4)&&(stime<maxstrip4))) {
			strip4=1;
			if(file){
                            if(fillcontrolhists){
				hstart_time_clean[4]->Fill(stime);
			    hstart_time_s0to7_clean->Fill(stime);
			    //if(binnum4<maxX)hstart_time_history_clean[4]->SetBinContent(binnum4,stime);
			    binnum4++;}}}
		    if(sstrip==5&&((stime>minstrip5)&&(stime<maxstrip5))) {
			strip5=1;
			if(file){
                            if(fillcontrolhists){
			    hstart_time_clean[5]->Fill(stime);
			    hstart_time_s0to7_clean->Fill(stime);
			   // if(binnum5<maxX)hstart_time_history_clean[5]->SetBinContent(binnum5,stime);
			    binnum5++;}}}
		    if(sstrip==6&&((stime>minstrip6)&&(stime<maxstrip6))) {
			strip6=1;
			if(file){
                            if(fillcontrolhists){
			    hstart_time_clean[6]->Fill(stime);
			    hstart_time_s0to7_clean->Fill(stime);
			   // if(binnum6<maxX)hstart_time_history_clean[6]->SetBinContent(binnum6,stime);
			    binnum6++;}}}
		    if(sstrip==7&&((stime>minstrip7)&&(stime<maxstrip7))) {
			strip7=1;
			if(file){
                            if(fillcontrolhists){
			    hstart_time_clean[7]->Fill(stime);
			    hstart_time_s0to7_clean->Fill(stime);
			   // if(binnum7<maxX)hstart_time_history_clean[7]->SetBinContent(binnum7,stime);
			    binnum7++;}}}
                //veto
                   if(sstrip==8&&((stime>minstrip8)&&(stime<maxstrip8))) {
			strip8=1;
			if(file){
                            if(fillcontrolhists){
			    hveto_time_clean[0]->Fill(stime);
			    hstart_time_s8to15_clean->Fill(stime);
			   // if(binnum8<maxX)hveto_time_history_clean[0]->SetBinContent(binnum8,stime);
			    binnum8++;}}}
		    if(sstrip==9&&((stime>minstrip9)&&(stime<maxstrip9))) {
			strip9=1;
			if(file){
                            if(fillcontrolhists){
			    hveto_time_clean[1]->Fill(stime);
			    hstart_time_s8to15_clean->Fill(stime);
			   // if(binnum9<maxX)hveto_time_history_clean[1]->SetBinContent(binnum9,stime);
			    binnum9++;}}}
		    if(sstrip==10&&((stime>minstrip10)&&(stime<maxstrip10))) {
			strip10=1;
			if(file){
                            if(fillcontrolhists){
			    hveto_time_clean[2]->Fill(stime);
			    hstart_time_s8to15_clean->Fill(stime);
			   // if(binnum10<maxX)hveto_time_history_clean[2]->SetBinContent(binnum10,stime);
			    binnum10++;}}}
		    if(sstrip==11&&((stime>minstrip11)&&(stime<maxstrip11))) {
			strip11=1;
			if(file){
                            if(fillcontrolhists){
			    hveto_time_clean[3]->Fill(stime);
			    hstart_time_s8to15_clean->Fill(stime);
			   // if(binnum11<maxX)hveto_time_history_clean[3]->SetBinContent(binnum11,stime);
			    binnum11++;}}}

		    if(sstrip==12&&((stime>minstrip12)&&(stime<maxstrip12))) {
			strip12=1;
			if(file){
                            if(fillcontrolhists){
			    hveto_time_clean[4]->Fill(stime);
			    hstart_time_s8to15_clean->Fill(stime);
			   // if(binnum12<maxX)hveto_time_history_clean[4]->SetBinContent(binnum12,stime);
			    binnum12++;}}}
		    if(sstrip==13&&((stime>minstrip13)&&(stime<maxstrip13))) {
			strip13=1;
			if(file){
                            if(fillcontrolhists){
			    hveto_time_clean[5]->Fill(stime);
			    hstart_time_s8to15_clean->Fill(stime);
			   // if(binnum13<maxX)hveto_time_history_clean[5]->SetBinContent(binnum13,stime);
			    binnum13++;}}}
		    if(sstrip==14&&((stime>minstrip14)&&(stime<maxstrip14))) {
			strip14=1;
			if(file){
                            if(fillcontrolhists){
			    hveto_time_clean[6]->Fill(stime);
			    hstart_time_s8to15_clean->Fill(stime);
			   // if(binnum14<maxX)hveto_time_history_clean[6]->SetBinContent(binnum14,stime);
			    binnum14++;}}}
		    if(sstrip==15&&((stime>minstrip15)&&(stime<maxstrip15))) {
			strip15=1;
			if(file){
                            if(fillcontrolhists){
			    hveto_time_clean[7]->Fill(stime);
			    hstart_time_s8to15_clean->Fill(stime);
			   // if(binnum15<maxX)hveto_time_history_clean[7]->SetBinContent(binnum15,stime);
			    binnum15++;}}}

		}
	    }
	}
   // }//

    if ((strip0||strip1)||(strip2||strip3)||(strip4||strip5)||(strip6||strip7)) {
	counttimewindowStartcut++; // counts events after time window cut if one is applied
        if(fillcontrolhists)hstartcutstat->SetBinContent(3,counttimewindowStartcut);
	return 0;
    }
    else {
        countskipedevents++; // counts skiped events
        if(fillcontrolhists)hstartcutstat->SetBinContent(4,countskipedevents);
	return kSkipEvent;}
}
