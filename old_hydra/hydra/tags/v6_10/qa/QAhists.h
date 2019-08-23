//*-- Author : Dan Magestro (magestro@gsi.de)
//*-- Modified : 24/01/2002
//
//  QAhists.h
//
//  List of general histograms to be printed by HQAOutputPS.
//
//  Note: If this file is changed, hqahistograms.cc needs
//        to be recompiled

  // Active Start histograms
   "stCal_strip",
   "stCal_tof",

  // Active Rich histograms
   "richCal_row",
   "richCal_column",
   "richHit_scat",
   "richHit_phi",
   "richHit_theta",
   "richHit_radius",
   "richHit_centroid",
   "richHit_chargeAmpl",
   "richHit_ringCol",
   "richHit_ringRow",
   "richHit_ringLocMax4",
   "richHit_houTraVsPatMat",
   "richHit_patMatVsTheta",
   "richHit_houTraVsTheta",
   "richHit_chargeAmplVsTheta",
   "richHit_radiusVsTheta",

  // Active Mdc histograms
   //"mdcRaw_mboVsSector_m0",
   //"mdcRaw_mboVsSector_m1",
   //"mdcRaw_mboVsSector_m2",
   //"mdcRaw_mboVsSector_m3",
   //"mdcCal1_time1VsSector_m0",
   //"mdcCal1_time1VsSector_m1",
   //"mdcCal1_time1VsSector_m2",
   //"mdcCal1_time1VsSector_m3",
   //"mdcCal1_nVsSector_m0",
   //"mdcCal1_nVsSector_m1",
   //"mdcCal1_nVsSector_m2",
   //"mdcCal1_nVsSector_m3",
   "mdcCal1_t2mt1_vs_t1[0][0]",
   "mdcCal1_t2mt1_vs_t1[1][0]",
   "mdcCal1_t2mt1_vs_t1[2][0]",
   "mdcCal1_t2mt1_vs_t1[3][0]",
   "mdcCal1_t2mt1_vs_t1[4][0]",
   "mdcCal1_t2mt1_vs_t1[5][0]",
   "mdcCal1_t2mt1_vs_t1[0][1]",
   "mdcCal1_t2mt1_vs_t1[1][1]",
   "mdcCal1_t2mt1_vs_t1[2][1]",
   "mdcCal1_t2mt1_vs_t1[3][1]",
   "mdcCal1_t2mt1_vs_t1[4][1]",
   "mdcCal1_t2mt1_vs_t1[5][1]",
   "mdcCal1_t2mt1_vs_t1[0][2]",
   "mdcCal1_t2mt1_vs_t1[1][2]",
   "mdcCal1_t2mt1_vs_t1[2][2]",
   "mdcCal1_t2mt1_vs_t1[3][2]",
   "mdcCal1_t2mt1_vs_t1[4][2]",
   "mdcCal1_t2mt1_vs_t1[5][2]",
   "mdcCal1_t2mt1_vs_t1[0][3]",
   "mdcCal1_t2mt1_vs_t1[1][3]",
   "mdcCal1_t2mt1_vs_t1[2][3]",
   "mdcCal1_t2mt1_vs_t1[3][3]",
   "mdcCal1_t2mt1_vs_t1[4][3]",
   "mdcCal1_t2mt1_vs_t1[5][3]",
   "mdcCal1_t2mt1_vs_t1_plane[0]",
   "mdcCal1_t2mt1_vs_t1_plane[1]",
   "mdcCal1_t2mt1_vs_t1_plane[2]",
   "mdcCal1_t2mt1_vs_t1_plane[3]",
   "mdcCutCounts[0]",
   "mdcCutCounts[1]",
   "mdcCutCounts[2]",
   "mdcCutCounts[3]",
   //"mdcCutCounts[4]",
   "mdcCutCounts[5]",
   "mdcCutCounts[6]",
   "mdcCal1_tat_m0",
   "mdcCal1_tat_m1",
   "mdcCal1_tat_m2",
   "mdcCal1_tat_m3",
   "mdcHit_scat_m0",
   "mdcHit_scat_m1",
   "mdcHit_scat_m2",
   "mdcHit_scat_m3",

  // Active Tof histograms
   "tofHit_scat",
   "tofHit_phi",
   "tofHit_theta",
   "tofHit_tof",
   "tofHit_n",

  // Active Tofino histograms
   "tfnCal_n",
   "tfnCal_tdc",

  // Active Shower histograms
   "shoHit_scat",
   "shoHit_phi",
   "shoHit_theta",
   "shoHit_chargeVsSector_m0",
   "shoHit_chargeVsSector_m1",
   "shoHit_chargeVsSector_m2",
   "shoHit_rowVsSector_m0",
   "shoHit_rowVsSector_m1",
   "shoHit_rowVsSector_m2",
   "shoHit_colVsSector_m0",
   "shoHit_colVsSector_m1",
   "shoHit_colVsSector_m2",
   // "shoHit_nSec",
   // "shoHit_sectorVsModule",
   "shoHitTof_scat",
   // "shoHitTof_nSec",

  // Active Kick histograms
  "kickTrack_scat",
  "kickTrack_pullVsSector",
  "kickTrack_massCharge"

  // Active Corr histograms