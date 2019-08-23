#ifndef HMDCTRIGGER_H
#define HMDCTRIGGER_H

#include "hreconstructor.h"
#include <iostream.h>
#include <hstartdef.h>

class HCategory;
class HIterator;
class TH1F;
class TH2F;
class TFile;

class HMdcTrigger : public HReconstructor {
protected:
  HCategory* calStartCat;       //! pointer to the cal data
  HIterator* iter_start;         //! iterator on start cal data.
  TH1F * hstartcutstat;
  TH1F * hstart_time_s0to7;
  TH1F * hstart_time_s0to7_clean;
  TH1F * hstart_time_s8to15;
  TH1F * hstart_time_s8to15_clean;

  TH2F * hstartvetodistribution;
  TH2F * hstartvetodistribution_clean;

  TH1F * hstart_time[8];
  TH1F * hstart_time_clean[8];
  TH1F * hstart_time_history_clean[8];
  TH1F * hstart_time_history[8];

  TH1F * hveto_time[8];
  TH1F * hveto_time_clean[8];
  TH1F * hveto_time_history_clean[8];
  TH1F * hveto_time_history[8];


  Int_t binnum0,binnum1,binnum2,binnum3,binnum4,binnum5,binnum6,binnum7;
  Int_t binnum8,binnum9,binnum10,binnum11,binnum12,binnum13,binnum14,binnum15;

  Int_t bin0,bin1,bin2,bin3,bin4,bin5,bin6,bin7;
  Int_t bin8,bin9,bin10,bin11,bin12,bin13,bin14,bin15;

  Int_t maxbinnum;
  Int_t maxX;

  Float_t minstrip0,minstrip1,minstrip2,minstrip3,minstrip4,minstrip5,minstrip6,minstrip7;
  Float_t maxstrip0,maxstrip1,maxstrip2,maxstrip3,maxstrip4,maxstrip5,maxstrip6,maxstrip7;

  Float_t minstrip8,minstrip9,minstrip10,minstrip11,minstrip12,minstrip13,minstrip14,minstrip15;
  Float_t maxstrip8,maxstrip9,maxstrip10,maxstrip11,maxstrip12,maxstrip13,maxstrip14,maxstrip15;


  Int_t startmultmin,tofmultmin,tofinomultmin,vetomultmin;
  Int_t startmultmax,tofmultmax,tofinomultmax,vetomultmax;

  Bool_t dotimecut;                   // flag for a external calibration file (pulser)
  Bool_t startbit;
  Bool_t tofbit;
  Bool_t tofinobit;
  Bool_t fillcontrolhists;
  Int_t countnocut;
  Int_t countmultiplicityStartcut;
  Int_t counttimewindowStartcut;
  Int_t countskipedevents;

  Char_t *fNameRootTrigger;                     // file name of root output file
  TFile *file;
  //ofstream *fout;
public:
  HMdcTrigger();
  HMdcTrigger(Text_t* name,Text_t* title);
  ~HMdcTrigger();
  Bool_t init(void);
  Bool_t finalize(void) {return kTRUE;}
  Int_t execute(void);

  void setTimeCut()         {dotimecut=kTRUE;}
  void setStartBit()        {startbit=kTRUE;tofbit=kFALSE;tofinobit=kFALSE;}
  void setTofBit()          {tofbit=kTRUE;startbit=kFALSE;tofinobit=kFALSE;}
  void setTofinoBit()       {tofinobit=kTRUE;startbit=kFALSE;tofbit=kFALSE;}

  void setFillControlHists(){fillcontrolhists=kTRUE;}

  void setTimeWindowStart(Float_t strip0min,Float_t strip0max,Float_t strip1min,Float_t strip1max,
			  Float_t strip2min,Float_t strip2max,Float_t strip3min,Float_t strip3max,
			  Float_t strip4min,Float_t strip4max,Float_t strip5min,Float_t strip5max,
			  Float_t strip6min,Float_t strip6max,Float_t strip7min,Float_t strip7max)
  {
      minstrip0=strip0min;maxstrip0=strip0max;minstrip1=strip1min;maxstrip1=strip1max;
      minstrip2=strip2min;maxstrip2=strip2max;minstrip3=strip3min;maxstrip3=strip3max;
      minstrip4=strip4min;maxstrip4=strip4max;minstrip5=strip5min;maxstrip5=strip5max;
      minstrip6=strip6min;maxstrip6=strip6max;minstrip7=strip7min;maxstrip7=strip7max;
  }
   void setTimeWindowVeto(Float_t strip8min,Float_t strip8max,Float_t strip9min,Float_t strip9max,
			  Float_t strip10min,Float_t strip10max,Float_t strip11min,Float_t strip11max,
			  Float_t strip12min,Float_t strip12max,Float_t strip13min,Float_t strip13max,
			  Float_t strip14min,Float_t strip14max,Float_t strip15min,Float_t strip15max)
  {
      minstrip8=strip8min;maxstrip8=strip8max;minstrip9=strip9min;maxstrip9=strip9max;
      minstrip10=strip10min;maxstrip10=strip10max;minstrip11=strip11min;maxstrip11=strip11max;
      minstrip12=strip12min;maxstrip12=strip12max;minstrip13=strip13min;maxstrip13=strip13max;
      minstrip14=strip14min;maxstrip14=strip14max;minstrip15=strip15min;maxstrip15=strip15max;
  }




  void setStartMultiplicityCut(Int_t min,Int_t max)  {startmultmin=min;startmultmax=max;}
  void setVetoMultiplicityCut(Int_t min,Int_t max)   {vetomultmin=min;vetomultmax=max;}
  void setTofMultiplicityCut(Int_t min,Int_t max)    {tofmultmin=min;tofmultmax=max;}
  void setTofinoMultiplicityCut(Int_t min,Int_t max) {tofinomultmin=min;tofinomultmax=max;}

  void setOutputRoot (Char_t*);

protected:
  void createHist(TFile*);
  void writeHist (TFile*);
  void setDefaultTimeWindow();
  ClassDef(HMdcTrigger,0) // checks trigger conditions on start detector
};

#endif /* !HMDCTRIGGER_H */

