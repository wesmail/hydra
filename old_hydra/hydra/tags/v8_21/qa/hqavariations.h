//*-- Author : Dan Magestro (magestro@gsi.de)
//*-- Modified : 24/01/2002
#ifndef HQAVARIATIONS_H
#define HQAVARIATIONS_H

#include "TObject.h"

class TProfile;

class HQAVariations : public TObject {

public:
  HQAVariations(Int_t intervalSize = 5000);
  virtual ~HQAVariations() {}

  virtual void bookHist();
  virtual void activateHist();

  TList *getHistList() {return histList;}

private:
  virtual void bookHistEvent();
  virtual void bookHistStart();
  virtual void bookHistRich();
  virtual void bookHistPid();
  virtual void bookHistMdc();
  virtual void bookHistShower();
  virtual void bookHistTof();
  virtual void bookHistTofino();
  virtual void bookHistKick();
  virtual void bookHistTrig();

  void resetPointers();

public:
  TList *histList;    //! List of booked histograms

  Int_t maxEvents;    //! Max. #events to be QA'd (= 200*IntervalSize)

  // Event histograms
  TProfile *evtHeader_eventSize_Var;  //! evtHeader: event size

  // Start histograms
  TProfile *stCal_meanStrip_Var;  //! startCal: mean strip number
  
  // Pid histograms
  TProfile *pidTrackCand_n_Var; //! pidTrackCand objects/event
  TProfile *pidTrackCandLep_n_Var; //! pidTrackCand objects with ring/event

  // Rich histograms
  TProfile *richCal_n_Var;     //! richCal: fired pads/event
  TProfile *richHit_n_Var;     //! richHit: hits/event

  // Mdc histograms
  TProfile *mdcCal1_time1_m0_Var;  //! mdcCal1: Plane I, average time1
  TProfile *mdcCal1_time1_m1_Var;  //! mdcCal1: Plane II, average time1
  TProfile *mdcCal1_time1_m2_Var;  //! mdcCal1: Plane III, average time1
  TProfile *mdcCal1_time1_m3_Var;  //! mdcCal1: Plane IV, average time1

  TProfile *mdcCal1_time2m1_m0_Var;  //! mdcCal1: Plane I, average time above threshold
  TProfile *mdcCal1_time2m1_m1_Var;  //! mdcCal1: Plane II, average time above threshold
  TProfile *mdcCal1_time2m1_m2_Var;  //! mdcCal1: Plane III, average time above threshold
  TProfile *mdcCal1_time2m1_m3_Var;  //! mdcCal1: Plane IV, average time above threshold

  // Tof histograms
  TProfile *tofHit_n_Var;       //! tofHit: num hits/event

  // Tofino histograms

  // Shower histograms
  TProfile *shoHit_n_Var;          //! showerHit: num hits/event
  TProfile *shoHitTof_n_Var;       //! showerHitTof: num hits/event

  // Kick histograms
  TProfile *kickTrack_n_Var;       //! kickTrack: num tracks/event

  // Trigger histograms
  TProfile *trigRich_n_Var;      //! trigRich    : RichIPU   hits/event
  TProfile *trigTof_n_Var;       //! trigTof     : TofIPU    hits/event  
  TProfile *trigShower_n_Var;    //! trigShower  : ShowerIPU hits/event
  TProfile *trigTlepton_n_Var;   //! trigTlepton : MU Leptons from Tof hits    / event
  TProfile *trigSlepton_n_Var;   //! trigSlepton : MU Leptons from Shower hits / event

  ClassDef(HQAVariations,1) // QA variation histograms
};

#endif



