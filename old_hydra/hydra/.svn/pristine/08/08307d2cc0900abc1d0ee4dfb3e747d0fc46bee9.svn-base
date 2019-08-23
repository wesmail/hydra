//*-- Author : Dan Magestro (magestro@gsi.de)
//*-- Modified : 24/01/2002
//*-- Modified : 20/01/2004 Jacek Otwinowski
//*-- Modified : 23/01/2004 A.Sadovski
//*-- Modified : 04/01/2005 Jacek Otwinowski
//*-- Modified : 08/02/2005 A.Sadovski
#ifndef HQAHISTOGRAMS_H
#define HQAHISTOGRAMS_H

#include "TObject.h"
#include "TH1.h"
#include "TH2.h"

class HQAHistograms : public TObject {

public:
  HQAHistograms();
  virtual ~HQAHistograms() {;}

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
  virtual void bookHistWall();
  virtual void bookHistKick();
  virtual void bookHistTrig();

  virtual void bookHistRichMDC();
  virtual void bookHistRichKick();

  virtual void bookHistMatching();
  virtual void bookHistPid();
  virtual void bookHistDaqScalers();

public:
  TList *histList;    //! List of all booked histograms

  // Start histograms
  
  TH1F *stCal_strip;               //! startCal: strip pattern
  TH1F *stLatchHist;               //! startCal: strip pattern
  TH1F *stCal_tof;                 //! startCal: time of flight
  TH2F *stHit_tof_vs_startstripe;  //! startHit: tofTOF vs startstripe
  TH2F *stHit_vs_stStrip;          //! startHit: startHitTime vs start stripe
  TH2F *stHit_vs_stStrip_Trigg[9]; //! startHit: startHitTime vs strip for 8 trigger inputs 
                                   //! trigger inputs are counted from 1 (hardware)        
  TH2F *stVertexXY;                //! event vertex information, X vs Y
  TH1F *stVertexZ;                 //! event vertex information, Z position
  // Daq Scalers histograms

  TH1F *histInput[8];              //! daqScaler: Trigger Box input 
  TH1F *histReduce[8];             //! daqScaler: Trigger Box accepted 
  TH1F *histStartScaler[8];        //! daqScaler: Start 8 channels
  TH1F *histVetoScaler[8];         //! daqScaler: Veto 8 channels
 

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
  TH1F *mdcCal1_t1[6][4];          //! mdcCal1: plane, sector t1 distribution
  TH1F *mdcCal1_t2mt1[6][4];       //! mdcCal1: plane, sector t2-t1 distribution
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
  TH1F *histProfTofino;    //! tofinoCal: Profile //recently (HistMultTofinoM by Tofino classification)
  TH1F *histMultTofinoM;   //! showerHitTof: Multiplicity in Tofino cell
  TH1F *histTofinoTof;     //! showerHitTof: Tofino time of flight

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
  TH1F *shoHit_nRow;           //! Shower Rows (All sectors)
  TH1F *shoHit_nCol;           //!  Shower Cols (All sectors)
  TH1F *shoHitSums[6][3];			//! ShowerSums 
  // Kick histograms
  TH2F *kickTrack_scat;             //! kickTrack: track distribution in lab frame
  TH1F *kickTrack_nSec;             //! kickTrack: num tracks/event/sector
  TH2F *kickTrack_pullVsSector;     //! kickTrack: sector vs. pull
  TH1F *kickTrack_massCharge;       //! kickTrack: mass * charge
  TH2F *kickTrack123_scat;          //! kickTrack123: track distribution in lab frame
  TH1F *kickTrack123_massCharge;    //! kickTrack123: mass * charge
  TH1F *splineTrack_massCharge;     //! spline track: mass * charge
  TH2F *splineTrack_scat;           //! splineTrack: track distribution in lab frame
  TH1F *rungeKuttaTrack_massCharge; //! spline track: mass * charge
  TH2F *rungeKuttaTrack_scat;       //! splineTrack: track distribution in lab frame

  // ForwardWall histograms
  TH1F *hWallCellSmall;   //!Wall cell Small
  TH1F *hWallCellMedium;  //!Wall cell Medium
  TH1F *hWallCellLarge;   //!Wall cell Large
  TH1F *hMultWall;        //!Wall mult
  TH2F *hWallCellTime;    //!Wall Time vs Cell
  TH2F *hWallCellAdc;     //!Wall Charge vs Cell
  TH2F *hWallXY;          //!Wall X vs Wall Y
  TH1F *hWallHitNumI;     //!Number of hits I   quarter
  TH1F *hWallHitNumII;    //!Number of hits II  quarter
  TH1F *hWallHitNumIII;   //!Number of hits III quarter
  TH1F *hWallHitNumIV;    //!Number of hits IV  quarter
                        
  
  //Tracking Kick12-Kick123-Spline sector-vise TOF/SHOWER-TOFINO
  TH1F *trackingKick12M_sys0[6];     //! KickPlane  tracking MDC12-META
  TH1F *trackingKick123_sys0[6];     //! KickPlane  tracking MDC12-3(4)
  TH1F *trackingSpline_sys0[6];      //! Spline     tracking MDC12-3(4)
  TH1F *trackingRK_sys0[6];          //! RungeKutta tracking MDC12-3(4)

  TH1F *trackingKick12M_sys1[6];     //! KickPlane  tracking MDC12-META
  TH1F *trackingKick123_sys1[6];     //! KickPlane  tracking MDC12-META
  TH1F *trackingSpline_sys1[6];      //! Spline     tracking MDC12-3(4)
  TH1F *trackingRK_sys1[6];          //! RungeKutta tracking MDC12-3(4)

  // Trigger histograms //from Alberica
  TH1F *trigRich_nhits;         //! matchURich: hit multiplicity  
  TH1F *trigRich_nSec;          //! matchURich: hits by sector
  TH1F *trigRich_theta;         //! matchURich: theta
  TH1F *trigRich_phi;           //! matchURich: phi
  TH2F *trigRich_rowVsCol;      //! matchURich: row versus column

  TH1F *trigTof_nhits;          //! matchUTof: hit multiplicity  
  TH1F *trigTof_nSec;           //! matchUTof: hits by sector
  TH1F *trigTof_theta;          //! matchUTof: theta
  TH1F *trigTof_phi;            //! matchUTof: phi
  TH1F *trigTof_time;           //! matchUTof: time
  TH2F *trigTof_xVsy;           //! matchUTof: x versus y

  TH1F *trigShower_nhits;       //! matchUShower: hit multiplicity  
  TH1F *trigShower_nSec;        //! matchUShower: hits by sector
  TH1F *trigShower_theta;       //! matchUShower: theta
  TH1F *trigShower_phi;         //! matchUShower: phi
  TH2F *trigShower_rowVsCol;    //! matchUShower: row versus column

  TH1F *trigMULep_nhits;        //! matchULep: hit multiplicity
  TH1F *trigMULep_TnSec;        //! matchULepT: hits by sector (tof)
  TH1F *trigMULep_SnSec;        //! matchULepS: hits by sector (shower)
  TH2F *trigMULep_momVsDTh;     //! matchULep: mom * charge versus delta theta
  TH2F *trigMULep_phiVsDPh;     //! matchULep: phi versus delta phi

  // Rich-Mdc histograms
  TH1F *richmdc_dtheta[6];   //! RichHit-MdcInerSeg: delta theta (sectorwise)
  TH1F *richmdc_dphi[6];     //! RichHit-MdcInerSeg: delta phi (sectorwise) 
  TH1F *richmdc_lep;         //! RichHit-MdcInerSeg: leptons per event (sectorwise)  
  // Rich-Kick histograms
  TH1F *richkick_dtheta[6];  //! RichHit-KickTrack: delta theta (sectorwise)
  TH1F *richkick_dphi[6];    //! RichHit-KickTrack: delta theta (sectorwise)
  TH1F *richkickS0_tof[6];   //! RichHit-KickTrack: tof sys0 (sectorwise)
  TH1F *richkickS1_tof[6];   //! RichHit-KickTrack: tof sys1 (sectorwise)

  TH1F *richkick_lep;        //! RichHit-KickTrack: leptons per event (sectorwise) 
  TH1F *richkick_ele;        //! RichHit-KickTrack: electrons per event (sectorwise) 
  TH1F *richkick_pos;        //! RichHit-KickTrack: positrons per event (sectorwise) 
  TH1F *richkickS_lep[2];    //! RichHit-KickTrack: leptons per event vs system (sectorwise)  
  TH2F *richkickS_tof_vs_rod[2]; //! RichHit-KickTrack: tof_vs_rod  vs system 
  TH2F *richkick_tof_vs_startstripe; //! RichHit-KickTrack: tofTOF vs startstripe 
  
  TH1F *hseckick;             //! Number of kick   objects matched to meta per sector
  TH1F *hseckick0;            //! the same for system 0
  TH1F *hseckick1;            //! the same for system 1
  TH1F *hsecspline;           //! Number of spline objects matched to meta per sector
  TH1F *hsecspline0;          //! the same for system 0
  TH1F *hsecspline1;          //! the same for system 1

  // MetaMatch histograms
  TH2F *hXdiffvsshowersector; //! Difference between X SHOWER hit position and crossing of track through SHOWER, vs sector
  TH2F *hYdiffvsshowersector; //! Difference between Y SHOWER hit position and crossing of track through SHOWER, vs sector
  TH2F *hXdiffvsshw;          //! Diff.  ShwX and MdcX vs sector / SigmaShw
  TH2F *hYdiffvsshw;          //! Diff.  ShwY and MdcY vs sector / SigmaShw
  TH2F *hshower_quality;      //! SHOWER quality vs sector
  TH2F *htof_quality;         //! TOF    quality vs sector
  TH2F *hXdiffvstofstrip;     //! Difference between X TOF hit position and crossing of track through TOF, vs TOF strip
  TH2F *hYdiffvstofstrip;     //! Difference between Y TOF hit position and crossing of track through TOF, vs TOF strip
  TH2F *hXdiffvsshoCol;       //! Difference between X SHOWER hit position and crossing of track through SHOWER, vs Column 
  TH2F *hXdiffvsshoRow;       //! Difference between X SHOWER hit position and crossing of track through SHOWER, vs Row 
  TH2F *hYdiffvsshoCol;       //! Difference between Y SHOWER hit position and crossing of track through SHOWER, vs Column 
  TH2F *hYdiffvsshoRow;       //! Difference between Y SHOWER hit position and crossing of track through SHOWER, vs Row 



    TH2F *hXdiffvsshowersectorRK;
    TH2F *hXdiffvstofstripRK;
    TH2F *hYdiffvsshowersectorRK;
    TH2F *hYdiffvstofstripRK;
    TH2F *hshower_qualityRK;
    TH2F *htof_qualityRK;

    TH2F *hXdiffvsshoColRK;
    TH2F *hXdiffvsshoRowRK;
    TH2F *hYdiffvsshoColRK;
    TH2F *hYdiffvsshoRowRK;



  //Pid Histograms
  TH1F* hNTrackCandPerSecKick;    //! Number of HPidTrackCand-objects per sector (Kick);
  TH1F* hNTrackCandLepPerSecKick;    //! Number of HPidTrackCand-objects with ring per sector (Kick);
  TH1F* hNTrackCandPerSecRK;    //! Number of HPidTrackCand-objects per sector (RK);
  TH1F* hNTrackCandLepPerSecRK;    //! Number of HPidTrackCand-objects with ring per sector (RK);

  TH1F* hpidtrk_multrk;       //! multiplicity of kicktracks
  TH1F* hpidtrk_multkick;     //! multiplicity of runge-kutta tracks

  // Runge-Kutta
  TH2F *hpidtrk_richrk_tof_vs_rod_sys0; //! lepton time vs rod for sys0 
  TH2F *hpidtrk_richrk_tof_vs_rod_sys1; //! lepton time vs rod for sys1
  TH2F *hpidtrk_richrk_tof_vs_startstrip; //! lepton time vs strip for start 
  // Kick Track  
  TH2F *hpidtrk_richkick_tof_vs_rod_sys0; //! lepton time vs rod for sys0
  TH2F *hpidtrk_richkick_tof_vs_rod_sys1; //! lepton time vs rod for sys1
  TH2F *hpidtrk_richkick_tof_vs_startstrip; //! lepton time vs strip for start
  // phi and theta distribution of tracks
  TH1F *hpidtrk_KICK_theta_sec[6];     //! theta distr. for all tracks Kick 
  TH1F *hpidtrk_SPLINE_theta_sec[6];   //! theta distr. for all tracks Spline 
  TH1F *hpidtrk_RK_theta_sec[6];       //! theta distr. for all tracks RK   
  TH1F *hpidtrk_KICK_neg_theta_sec[6]; //! theta distr. for neg. tracks and mom > 300 Kick 
  TH1F *hpidtrk_SPLINE_neg_theta_sec[6]; //! theta distr. for neg. tracks and mom > 300 Spline 
  TH1F *hpidtrk_RK_neg_theta_sec[6];   //! theta distr. for neg. tracks and mom > 300 RK   
  TH1F *hpidtrk_KICK_phi;              //! phi distr. for all tracks Kick                 
  TH1F *hpidtrk_SPLINE_phi;              //! phi distr. for all tracks Kick                 
  TH1F *hpidtrk_RK_phi;                //! phi distr. for all tracks RK   
  TH1F *hpidtrk_KICK_neg_phi;          //! phi distr. for neg. tracks and mom > 300 Kick 
  TH1F *hpidtrk_SPLINE_neg_phi;          //! phi distr. for neg. tracks and mom > 300 Kick 
  TH1F *hpidtrk_RK_neg_phi;            //! phi distr. for neg. tracks and mom > 300 RK   

  // momentum reconstructed - calculated from velocity
  TH2F *hpidtrk_kick_momdif_sys0_sec[6]; //! kick, sys0
  TH2F *hpidtrk_kick_momdif_sys1_sec[6]; //! kick, sys1
  TH2F *hpidtrk_rk_momdif_sys0_sec[6];   //! runge-kutta, sya0
  TH2F *hpidtrk_rk_momdif_sys1_sec[6];   //! runge-kutta, sys1

  ClassDef(HQAHistograms,1) // QA general histograms
};


#endif
