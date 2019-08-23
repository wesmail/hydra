//*-- Author : Dan Magestro (magestro@gsi.de)
//*-- Modified : 24/01/2002
#ifndef HQAHISTOGRAMS_H
#define HQAHISTOGRAMS_H

#include <TObject.h>

class TH1F;
class TH2F;

class HQAHistograms : public TObject {

public:
  HQAHistograms();
  virtual ~HQAHistograms() {}

  virtual void bookHist();
  virtual void activateHist();

  TList *getHistList() {return histList;}

private:

  void resetPointers();

  virtual void bookHistStart();
  virtual void bookHistRich();
  virtual void bookHistMdc();
  virtual void bookHistShower();
  virtual void bookHistTof();
  virtual void bookHistTofino();
  virtual void bookHistKick();

public:
  TList *histList;    //! List of all booked histograms

  // Start histograms
  TH1F *stCal_strip;               //! startCal: strip pattern
  TH1F *stCal_tof;                 //! startCal: time of flight

  // Rich histograms
  TH1F *richCal_row;               //! richCal: row
  TH1F *richCal_column;            //! richCal: column
  TH1F *richCal_nSec;              //! richCal: fired pads by sector

  TH2F *richHit_scat;              //! richHit: hit distribution in lab frame
  TH1F *richHit_nSec;              //! richHit: hits by sector

  TH1F *richHit_phi;               //! richHit: phi angle
  TH1F *richHit_theta;             //! richHit: theta angle
  TH1F *richHit_radius;            //! richHit: Ring radius
  TH1F *richHit_centroid;          //! richHit: Ring centroid
  TH1F *richHit_chargeAmpl;        //! richHit: Ring charge amplitude
  TH1F *richHit_ringCol;           //! richHit: Ring column
  TH1F *richHit_ringRow;           //! richHit: Ring row
  TH1F *richHit_ringLocMax4;       //! richHit: Ring Loc Max4

  TH2F *richHit_houTraVsPatMat;    //! richHit: ring algorithm comparison
  TH2F *richHit_patMatVsTheta;     //! richHit: #theta vs. PM
  TH2F *richHit_houTraVsTheta;     //! richHit: #theta vs. HT
  TH2F *richHit_chargeAmplVsTheta; //! richHit: #theta vs. ring amplitude
  TH2F *richHit_radiusVsTheta;     //! richHit: #theta vs. radius

  // Mdc histograms
  TH2F *mdcRaw_mboVsSector_m0; //! mdcRaw: Plane I, sector vs. mbo mult.
  TH2F *mdcRaw_mboVsSector_m1; //! mdcRaw: Plane II, sector vs. mbo mult.
  TH2F *mdcRaw_mboVsSector_m2; //! mdcRaw: Plane III, sector vs. mbo mult.
  TH2F *mdcRaw_mboVsSector_m3; //! mdcRaw: Plane IV, sector vs. mbo mult.

  TH2F *mdcCal1_nVsSector_m0;  //! mdcCal1: Plane I, sector vs. multiplicity
  TH2F *mdcCal1_nVsSector_m1;  //! mdcCal1: Plane II, sector vs. multiplicity
  TH2F *mdcCal1_nVsSector_m2;  //! mdcCal1: Plane III, sector vs. multiplicity
  TH2F *mdcCal1_nVsSector_m3;  //! mdcCal1: Plane IV, sector vs. multiplicity

  TH2F *mdcCal1_time1VsSector_m0;  //! mdcCal1: Plane I, sector vs. time1
  TH2F *mdcCal1_time1VsSector_m1;  //! mdcCal1: Plane II, sector vs. time1
  TH2F *mdcCal1_time1VsSector_m2;  //! mdcCal1: Plane III, sector vs. time1
  TH2F *mdcCal1_time1VsSector_m3;  //! mdcCal1: Plane IV, sector vs. time1

  TH2F *mdcCal1_tatVsSector_m0;    //! mdcCal1: Plane I, sector vs. t.a.t.
  TH2F *mdcCal1_tatVsSector_m1;    //! mdcCal1: Plane II, sector vs. t.a.t.
  TH2F *mdcCal1_tatVsSector_m2;    //! mdcCal1: Plane III, sector vs. t.a.t.
  TH2F *mdcCal1_tatVsSector_m3;    //! mdcCal1: Plane IV, sector vs. t.a.t.

  TH2F *mdcCal1_t2mt1_vs_t1[6][4]; //! mdcCal1: plane, sector t2-t1 vs. t1
  TH2F *mdcCal1_t2mt1_vs_t1_plane[4]; //! mdcCal1: plane(all sectors) t2-t1 vs. t1
  TH1F *mdcCutCounts[7];           //! mdc event counts total and vs. cuts applied

  TH2F *mdcHit_scat_m0;    //! mdcHit: Plane I, hit distribution
  TH2F *mdcHit_scat_m1;    //! mdcHit: Plane II, hit distribution
  TH2F *mdcHit_scat_m2;    //! mdcHit: Plane III, hit distribution
  TH2F *mdcHit_scat_m3;    //! mdcHit: Plane IV, hit distribution


  // Tof histograms
  TH2F *tofHit_scat;       //! tofHit: hit distribution in lab frame
  TH1F *tofHit_tof;        //! tofHit: time of flight
  TH1F *tofHit_n;          //! tofHit: hit multiplicity
  TH1F *tofHit_nSec;       //! tofHit: hit multiplicity/sector
  TH1F *tofHit_phi;        //! tofHit: phi angle
  TH1F *tofHit_theta;      //! tofHit: theta angle

  // Tofino histograms
  TH1F *tfnCal_n;          //! tofinoCal: multiplicity
  TH1F *tfnCal_tdc;        //! tofinoCal: tdc time

  // Shower histograms
  TH2F *shoHit_scat;           //! showerHit: hit distribution in lab frame
  TH1F *shoHit_theta;          //! showerHit: theta angle
  TH1F *shoHit_phi;            //! showerHit: phi angle

  TH2F *shoHit_chargeVsSector_m0;   //! showerHit: Mod 0, charge on loc. max.
  TH2F *shoHit_chargeVsSector_m1;   //! showerHit: Mod 1, charge on loc. max.
  TH2F *shoHit_chargeVsSector_m2;   //! showerHit: Mod 2, charge on loc. max.

  TH2F *shoHit_rowVsSector_m0;   //! showerHit: Mod 0, sector vs. row
  TH2F *shoHit_rowVsSector_m1;   //! showerHit: Mod 1, sector vs. row
  TH2F *shoHit_rowVsSector_m2;   //! showerHit: Mod 2, sector vs. row

  TH2F *shoHit_colVsSector_m0;   //! showerHit: Mod 0, sector vs. col
  TH2F *shoHit_colVsSector_m1;   //! showerHit: Mod 1, sector vs. col
  TH2F *shoHit_colVsSector_m2;   //! showerHit: Mod 2, sector vs. col

  TH2F *shoHit_sectorVsModule; //! showerHit: num hits by module
  TH1F *shoHit_nSec;           //! showerHit: num hits/event/sector

  TH2F *shoHitTof_scat;        //! showerHitTof: hit distribution in lab frame
  TH1F *shoHitTof_nSec;        //! showerHitTof: num hits/event/sector

  // Kick histograms
  TH2F *kickTrack_scat;         //! kickTrack: track distribution in lab frame
  TH1F *kickTrack_nSec;         //! kickTrack: num tracks/event/sector
  TH2F *kickTrack_pullVsSector; //! kickTrack: sector vs. pull
  TH1F *kickTrack_massCharge;   //! kickTrack: mass * charge


  ClassDef(HQAHistograms,1) // QA general histograms
};

#endif
