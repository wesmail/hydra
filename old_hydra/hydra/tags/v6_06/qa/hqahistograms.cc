#include <iostream.h>
#include <math.h>
// Root includes
#include <TObjString.h>
#include <TH1.h>
#include <TH2.h>
// Hydra includes
#include "hqahistograms.h"

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-//
//*-- Author : Dan Magestro (magestro@gsi.de)
//*-- Modified : 24/01/2002
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-//

//////////////////////////////////////////////////////////////////////////////
//
// HQAHistograms
//
// Contains general histograms for use by HQAMaker.
//
// BEGIN_HTML<b><font color="maroon">For more information, see the <a href="http://www-hades.gsi.de/computing/qa/">QA home page</a>.</font></b> END_HTML
//
//////////////////////////////////////////////////////////////////////////////

ClassImp(HQAHistograms)

HQAHistograms::HQAHistograms() {

  histList = new TList();
  resetPointers();
}

void HQAHistograms::resetPointers() {
// --- Zero all histogram pointers

  //---- Start histograms
  stCal_strip = 0;
  stCal_tof = 0;

  //---- Rich histograms
  richCal_row = 0;
  richCal_column = 0;
  richCal_nSec = 0;

  richHit_scat = 0;
  richHit_nSec = 0;

  richHit_phi = 0;
  richHit_theta = 0;
  richHit_radius = 0;
  richHit_centroid = 0;
  richHit_chargeAmpl = 0;
  richHit_ringCol = 0;
  richHit_ringRow = 0;
  richHit_ringLocMax4 = 0;

  richHit_houTraVsPatMat = 0;
  richHit_patMatVsTheta = 0;
  richHit_houTraVsTheta = 0;
  richHit_chargeAmplVsTheta = 0;
  richHit_radiusVsTheta = 0;

  //---- Mdc histograms
  mdcRaw_mboVsSector_m0 = 0;
  mdcRaw_mboVsSector_m1 = 0;
  mdcRaw_mboVsSector_m2 = 0;
  mdcRaw_mboVsSector_m3 = 0;

  mdcCal1_nVsSector_m0 = 0;
  mdcCal1_nVsSector_m1 = 0;
  mdcCal1_nVsSector_m2 = 0;
  mdcCal1_nVsSector_m3 = 0;

  mdcCal1_time1VsSector_m0 = 0;
  mdcCal1_time1VsSector_m1 = 0;
  mdcCal1_time1VsSector_m2 = 0;
  mdcCal1_time1VsSector_m3 = 0;

  mdcCal1_tatVsSector_m0 = 0;
  mdcCal1_tatVsSector_m1 = 0;
  mdcCal1_tatVsSector_m2 = 0;
  mdcCal1_tatVsSector_m3 = 0;

  mdcHit_scat_m0 = 0;
  mdcHit_scat_m1 = 0;
  mdcHit_scat_m2 = 0;
  mdcHit_scat_m3 = 0;

  //---- Tof histograms
  tofHit_scat = 0;
  tofHit_tof = 0;
  tofHit_n = 0;
  tofHit_nSec = 0;
  tofHit_phi = 0;
  tofHit_theta = 0;

  //---- Tofino histograms
  tfnCal_n = 0;
  tfnCal_tdc = 0;

  //---- Shower histograms
  shoHit_chargeVsSector_m0 = 0;
  shoHit_chargeVsSector_m1 = 0;
  shoHit_chargeVsSector_m2 = 0;

  shoHit_rowVsSector_m0 = 0;
  shoHit_rowVsSector_m1 = 0;
  shoHit_rowVsSector_m2 = 0;

  shoHit_colVsSector_m0 = 0;
  shoHit_colVsSector_m1 = 0;
  shoHit_colVsSector_m2 = 0;

  shoHit_sectorVsModule = 0;
  shoHit_scat = 0;
  shoHit_nSec = 0;
  shoHit_phi = 0;
  shoHit_theta = 0;

  shoHitTof_scat = 0;
  shoHitTof_nSec = 0;

  //---- Kick histograms
  kickTrack_scat = 0;
  kickTrack_nSec = 0;
  kickTrack_pullVsSector = 0;

}

void HQAHistograms::activateHist() {
// This function removes histograms from the histList TList which
// are not listed in QAhists.h.

  TList *activeList = new TList();

  Char_t *hists[1000] = {
    #include "QAhists.h"
  };

  // Convert Char_t to TList of TObjStrings
  for(Int_t i=0; hists[i]!=0;i++)
    activeList->Add( new TObjString(hists[i]) );

  // Iterate over histList, remove entries which aren't in activeList
  TIter next(histList);
  while (TH1 *hist = (TH1*) next()) {
    if( ! activeList->Contains( hist->GetName() ) )
      histList->Remove(hist);
  }

}

void HQAHistograms::bookHist() {
// Book histograms and define axis titles and options for drawing

  bookHistStart();
  bookHistRich();
  bookHistMdc();
  bookHistTof();
  bookHistTofino();
  bookHistShower();
  bookHistKick();

  activateHist();
}

void HQAHistograms::bookHistStart() {

  stCal_strip = new TH1F("stCal_strip","startCal: strip pattern",8,0.5,8.5);
  stCal_strip->SetXTitle("strip number");
  histList->Add(stCal_strip,"start--");

  stCal_tof = new TH1F("stCal_tof","startCal: time of flight",100,-5.0,5.0);
  stCal_tof->SetXTitle("time of flight");
  histList->Add(stCal_tof,"start--");
}

void HQAHistograms::bookHistRich() {

  richCal_row = new TH1F("richCal_row","richCal: row",90,0,90);
  richCal_row->SetXTitle("row");
  histList->Add(richCal_row,"rich--");

  richCal_column = new TH1F("richCal_column","richCal: column",92,0,92);
  richCal_column->SetXTitle("column");
  histList->Add(richCal_column,"rich--");

  richCal_nSec = new TH1F("richCal_nSec","richCal: fired pads by sector",6,-0.5,5.5);
  histList->Add(richCal_nSec,"rich--");

  richHit_nSec = new TH1F("richHit_nSec","richHit: hits by sector",6,-0.5,5.5);
  histList->Add(richHit_nSec,"rich--");

  richHit_scat = new TH2F("richHit_scat","richHit: hit distribution in lab frame",105,-1.05,1.05,105,-1.05,1.05);
  histList->Add(richHit_scat,"rich--nobox");

  richHit_phi = new TH1F("richHit_phi","richHit: phi angle",120,0.,360.);
  richHit_phi->SetXTitle("#phi (degrees)");
  histList->Add(richHit_phi,"rich--");

  richHit_theta = new TH1F("richHit_theta","richHit: theta angle",80,10.,90.);
  richHit_theta->SetXTitle("#theta (degrees)");
  histList->Add(richHit_theta,"rich--");

  richHit_radius = new TH1F("richHit_radius","richHit: Ring radius",80,2,6);
  richHit_radius->SetXTitle("radius");
  histList->Add(richHit_radius,"rich--");

  richHit_centroid = new TH1F("richHit_centroid","richHit: Ring centroid",100,0,4);
  histList->Add(richHit_centroid,"rich--");

  richHit_chargeAmpl = new TH1F("richHit_chargeAmpl","richHit: Ring charge amplitude",100,0,1000);
  histList->Add(richHit_chargeAmpl,"rich--");

  richHit_ringCol = new TH1F("richHit_ringCol","richHit: Ring column",92,0,92);
  histList->Add(richHit_ringCol,"rich--");

  richHit_ringRow = new TH1F("richHit_ringRow","richHit: Ring row",90,0,90);
  histList->Add(richHit_ringRow,"rich--");

  richHit_ringLocMax4 = new TH1F("richHit_ringLocMax4","richHit: Ring Loc Max4",15,0,15);
  histList->Add(richHit_ringLocMax4,"rich--");

  richHit_houTraVsPatMat = new TH2F("richHit_houTraVsPatMat","richHit: ring algorithm comparison",100,0,200,100,0,800);
  richHit_houTraVsPatMat->SetXTitle("Hough Trans.");
  richHit_houTraVsPatMat->SetYTitle("Pattern Matrix");
  histList->Add(richHit_houTraVsPatMat,"rich--");

  richHit_patMatVsTheta = new TH2F("richHit_patMatVsTheta","richHit: #theta vs. PM",100,200,800,90,0,90);
  richHit_patMatVsTheta->SetXTitle("Pattern Matrix");
  richHit_patMatVsTheta->SetYTitle("#theta (degrees)");
  histList->Add(richHit_patMatVsTheta,"rich--");

  richHit_houTraVsTheta = new TH2F("richHit_houTraVsTheta","richHit: #theta vs. HT",100,0,200,90,0,90);
  richHit_houTraVsTheta->SetXTitle("Hough Trans.");
  richHit_houTraVsTheta->SetYTitle("#theta (degrees)");
  histList->Add(richHit_houTraVsTheta,"rich--");

  richHit_chargeAmplVsTheta = new TH2F("richHit_chargeAmplVsTheta","richHit: #theta vs. ring amplitude",100,0,1000,90,0,90);
  richHit_chargeAmplVsTheta->SetYTitle("#theta (degrees)");
  histList->Add(richHit_chargeAmplVsTheta,"rich--");

  richHit_radiusVsTheta = new TH2F("richHit_radiusVsTheta","richHit: #theta vs. radius",40,2,6,90,0,90);
  richHit_radiusVsTheta->SetXTitle("radius");
  richHit_radiusVsTheta->SetYTitle("#theta (degrees)");
  histList->Add(richHit_radiusVsTheta,"rich--");

}

void HQAHistograms::bookHistMdc() {

  mdcRaw_mboVsSector_m0 = new TH2F("mdcRaw_mboVsSector_m0","mdcRaw: Plane I, sector vs. mbo mult.",14,-0.5,15.5,6,-0.5,5.5);
  mdcRaw_mboVsSector_m0->SetXTitle("mbo #");
  mdcRaw_mboVsSector_m0->SetYTitle("sector");
  histList->Add(mdcRaw_mboVsSector_m0,"mdc--logz");

  mdcRaw_mboVsSector_m1 = new TH2F("mdcRaw_mboVsSector_m1","mdcRaw: Plane II, sector vs. mbo mult.",16,-0.5,15.5,6,-0.5,5.5);
  mdcRaw_mboVsSector_m1->SetXTitle("mbo #");
  mdcRaw_mboVsSector_m1->SetYTitle("sector");
  histList->Add(mdcRaw_mboVsSector_m1,"mdc--logz");

  mdcRaw_mboVsSector_m2 = new TH2F("mdcRaw_mboVsSector_m2","mdcRaw: Plane III, sector vs. mbo mult.",16,-0.5,15.5,6,-0.5,5.5);
  mdcRaw_mboVsSector_m2->SetXTitle("mbo #");
  mdcRaw_mboVsSector_m2->SetYTitle("sector");
  histList->Add(mdcRaw_mboVsSector_m2,"mdc--logz");

  mdcRaw_mboVsSector_m3 = new TH2F("mdcRaw_mboVsSector_m3","mdcRaw: Plane IV, sector vs. mbo mult.",16,-0.5,15.5,6,-0.5,5.5);
  mdcRaw_mboVsSector_m3->SetXTitle("mbo #");
  mdcRaw_mboVsSector_m3->SetYTitle("sector");
  histList->Add(mdcRaw_mboVsSector_m3,"mdc--logz");

  mdcCal1_nVsSector_m0 = new TH2F("mdcCal1_nVsSector_m0","mdcCal1: Plane I, sector vs. multiplicity",51,-0.5,50.5,6,-0.5,5.5);
  mdcCal1_nVsSector_m0->SetXTitle("# fired wires");
  mdcCal1_nVsSector_m0->SetYTitle("sector");
  histList->Add(mdcCal1_nVsSector_m0,"mdc--logz");

  mdcCal1_nVsSector_m1 = new TH2F("mdcCal1_nVsSector_m1","mdcCal1: Plane II, sector vs. multiplicity",51,-0.5,50.5,6,-0.5,5.5);
  mdcCal1_nVsSector_m1->SetXTitle("# fired wires");
  mdcCal1_nVsSector_m1->SetYTitle("sector");
  histList->Add(mdcCal1_nVsSector_m1,"mdc--logz");

  mdcCal1_nVsSector_m2 = new TH2F("mdcCal1_nVsSector_m2","mdcCal1: Plane III, sector vs. multiplicity",51,-0.5,50.5,6,-0.5,5.5);
  mdcCal1_nVsSector_m2->SetXTitle("# fired wires");
  mdcCal1_nVsSector_m2->SetYTitle("sector");
  histList->Add(mdcCal1_nVsSector_m2,"mdc--logz");

  mdcCal1_nVsSector_m3 = new TH2F("mdcCal1_nVsSector_m3","mdcCal1: Plane IV, sector vs. multiplicity",51,-0.5,50.5,6,-0.5,5.5);
  mdcCal1_nVsSector_m3->SetXTitle("# fired wires");
  mdcCal1_nVsSector_m3->SetYTitle("sector");
  histList->Add(mdcCal1_nVsSector_m3,"mdc--logz");

  mdcCal1_time1VsSector_m0 = new TH2F("mdcCal1_time1VsSector_m0","mdcCal1: Plane I, sector vs. time1",200,0.,1000.,6,-0.5,5.5);
  mdcCal1_time1VsSector_m0->SetXTitle("time1 (ns)");
  mdcCal1_time1VsSector_m0->SetYTitle("sector");
  histList->Add(mdcCal1_time1VsSector_m0,"mdc--");

  mdcCal1_time1VsSector_m1 = new TH2F("mdcCal1_time1VsSector_m1","mdcCal1: Plane II, sector vs. time1",200,0.,1000.,6,-0.5,5.5);
  mdcCal1_time1VsSector_m1->SetXTitle("time1 (ns)");
  mdcCal1_time1VsSector_m1->SetYTitle("sector");
  histList->Add(mdcCal1_time1VsSector_m1,"mdc--");

  mdcCal1_time1VsSector_m2 = new TH2F("mdcCal1_time1VsSector_m2","mdcCal1: Plane III, sector vs. time1",200,0.,1000.,6,-0.5,5.5);
  mdcCal1_time1VsSector_m2->SetXTitle("time1 (ns)");
  mdcCal1_time1VsSector_m2->SetYTitle("sector");
  histList->Add(mdcCal1_time1VsSector_m2,"mdc--");

  mdcCal1_time1VsSector_m3 = new TH2F("mdcCal1_time1VsSector_m3","mdcCal1: Plane IV, sector vs. time1",200,0.,1000.,6,-0.5,5.5);
  mdcCal1_time1VsSector_m3->SetXTitle("time1 (ns)");
  mdcCal1_time1VsSector_m3->SetYTitle("sector");
  histList->Add(mdcCal1_time1VsSector_m3,"mdc--");

  mdcCal1_tatVsSector_m0 = new TH2F("mdcCal1_tatVsSector_m0","mdcCal1: Plane I, sector vs. t.a.t.",150,0.,300.,6,-0.5,5.5);
  mdcCal1_tatVsSector_m0->SetXTitle("time above threshold (ns)");
  mdcCal1_tatVsSector_m0->SetYTitle("sector");
  histList->Add(mdcCal1_tatVsSector_m0,"mdc--");

  mdcCal1_tatVsSector_m1 = new TH2F("mdcCal1_tatVsSector_m1","mdcCal1: Plane II, sector vs. t.a.t.",150,0.,300.,6,-0.5,5.5);
  mdcCal1_tatVsSector_m1->SetXTitle("time above threshold (ns)");
  mdcCal1_tatVsSector_m1->SetYTitle("sector");
  histList->Add(mdcCal1_tatVsSector_m1,"mdc--");

  mdcCal1_tatVsSector_m2 = new TH2F("mdcCal1_tatVsSector_m2","mdcCal1: Plane III, sector vs. t.a.t.",150,0.,300.,6,-0.5,5.5);
  mdcCal1_tatVsSector_m2->SetXTitle("time above threshold (ns)");
  mdcCal1_tatVsSector_m2->SetYTitle("sector");
  histList->Add(mdcCal1_tatVsSector_m2,"mdc--");

  mdcCal1_tatVsSector_m3 = new TH2F("mdcCal1_tatVsSector_m3","mdcCal1: Plane IV, sector vs. t.a.t.",150,0.,300.,6,-0.5,5.5);
  mdcCal1_tatVsSector_m3->SetXTitle("time above threshold (ns)");
  mdcCal1_tatVsSector_m3->SetYTitle("sector");
  histList->Add(mdcCal1_tatVsSector_m3,"mdc--");

  mdcHit_scat_m0 = new TH2F("mdcHit_scat_m0","mdcHit: Plane I, hit distribution",100,-900,900,100,-900,900);
  mdcHit_scat_m0->SetXTitle("view from target");
  histList->Add(mdcHit_scat_m0,"mdc--");

  mdcHit_scat_m1 = new TH2F("mdcHit_scat_m1","mdcHit: Plane II, hit distribution",100,-1000,1000,100,-1000,1000);
  mdcHit_scat_m1->SetXTitle("view from target");
  histList->Add(mdcHit_scat_m1,"mdc--");

  mdcHit_scat_m2 = new TH2F("mdcHit_scat_m2","mdcHit: Plane III, hit distribution",100,-2000,2000,100,-2000,2000);
  mdcHit_scat_m2->SetXTitle("view from target");
  histList->Add(mdcHit_scat_m2,"mdc--");

  mdcHit_scat_m3 = new TH2F("mdcHit_scat_m3","mdcHit: Plane IV, hit distribution",100,-2500,2500,100,-2500,2500);
  mdcHit_scat_m3->SetXTitle("view from target");
  histList->Add(mdcHit_scat_m3,"mdc--");

}

void HQAHistograms::bookHistTof() {

  tofHit_scat = new TH2F("tofHit_scat","tofHit: hit distribution in lab frame",100,-2750,2750,100,-2750,2750);
  tofHit_scat->SetXTitle("X_{lab} (mm)");
  tofHit_scat->SetYTitle("Y_{lab} (mm)");
  histList->Add(tofHit_scat,"tof--nobox");

  tofHit_n = new TH1F("tofHit_n","tofHit: hit multiplicity",51,-0.5,50.5);
  tofHit_n->SetXTitle("num hits");
  histList->Add(tofHit_n,"tof--");

  tofHit_tof = new TH1F("tofHit_tof","tofHit: time of flight",200,0.,50.);
  tofHit_tof->SetXTitle("time of flight (ns)");
  histList->Add(tofHit_tof,"tof--");

  tofHit_nSec = new TH1F("tofHit_nSec","tofHit: num hits/event/sector",6,-0.5,5.5);
  tofHit_nSec->SetXTitle("sector");
  histList->Add(tofHit_nSec,"tof--logy");

  tofHit_phi = new TH1F("tofHit_phi","tofHit: phi angle",120,0.,360.);
  tofHit_phi->SetXTitle("#phi (degrees)");
  histList->Add(tofHit_phi,"tof--");

  tofHit_theta = new TH1F("tofHit_theta","tofHit: theta angle",50,40.,90.);
  tofHit_theta->SetXTitle("#theta (degrees)");
  histList->Add(tofHit_theta,"tof--");

}

void HQAHistograms::bookHistTofino() {

  tfnCal_n = new TH1F("tfnCal_n","tofinoCal: multiplicity",25,-0.5,24.5);
  tfnCal_n->SetXTitle("multiplicity");
  histList->Add(tfnCal_n,"tofino--");

  tfnCal_tdc = new TH1F("tfnCal_tdc","tofinoCal: tdc time",100,0.,100.);
  tfnCal_tdc->SetXTitle("tdc (ns)");
  histList->Add(tfnCal_tdc,"tofino--");

}

void HQAHistograms::bookHistShower() {

  shoHit_scat = new TH2F("shoHit_scat","showerHit: hit distribution in lab frame",100,-2100,2100,100,-2100,2100);
  shoHit_scat->SetXTitle("X_{lab} (mm)");
  shoHit_scat->SetYTitle("Y_{lab} (mm)");
  histList->Add(shoHit_scat,"shower--nobox");

  shoHit_theta = new TH1F("shoHit_theta","showerHit: theta angle",50,5.,55.);
  shoHit_theta->SetXTitle("#theta (degrees)");
  histList->Add(shoHit_theta,"shower--");

  shoHit_phi = new TH1F("shoHit_phi","showerHit: phi angle",120,0.,360.);
  shoHit_phi->SetXTitle("#phi (degrees)");
  histList->Add(shoHit_phi,"shower--");

  shoHit_chargeVsSector_m0 = new TH2F("shoHit_chargeVsSector_m0" ,"showerHit: Mod 0, charge on loc. max.",100,0.,100.,6,-0.5,5.5);
  shoHit_chargeVsSector_m0->SetXTitle("charge on loc. max.");
  shoHit_chargeVsSector_m0->SetYTitle("sector");
  histList->Add(shoHit_chargeVsSector_m0,"shower--logz");

  shoHit_chargeVsSector_m1 = new TH2F("shoHit_chargeVsSector_m1" ,"showerHit: Mod 1, charge on loc. max.",100,0.,100.,6,-0.5,5.5);
  shoHit_chargeVsSector_m1->SetXTitle("charge on loc. max.");
  shoHit_chargeVsSector_m1->SetYTitle("sector");
  histList->Add(shoHit_chargeVsSector_m1,"shower--logz");

  shoHit_chargeVsSector_m2 = new TH2F("shoHit_chargeVsSector_m2" ,"showerHit: Mod 2, charge on loc. max.",100,0.,100.,6,-0.5,5.5);
  shoHit_chargeVsSector_m2->SetXTitle("charge on loc. max.");
  shoHit_chargeVsSector_m2->SetYTitle("sector");
  histList->Add(shoHit_chargeVsSector_m2,"shower--logz");

  shoHit_rowVsSector_m0 = new TH2F("shoHit_rowVsSector_m0" ,"showerHit: Mod 0, sector vs. row",32,0.,32.,6,-0.5,5.5);
  shoHit_rowVsSector_m0->SetXTitle("charge on loc. max.");
  shoHit_rowVsSector_m0->SetYTitle("sector");
  histList->Add(shoHit_rowVsSector_m0,"shower--logz");

  shoHit_rowVsSector_m1 = new TH2F("shoHit_rowVsSector_m1" ,"showerHit: Mod 1, sector vs. row",32,0.,32.,6,-0.5,5.5);
  shoHit_rowVsSector_m1->SetXTitle("charge on loc. max.");
  shoHit_rowVsSector_m1->SetYTitle("sector");
  histList->Add(shoHit_rowVsSector_m1,"shower--logz");

  shoHit_rowVsSector_m2 = new TH2F("shoHit_rowVsSector_m2" ,"showerHit: Mod 2, sector vs. row",32,0.,32.,6,-0.5,5.5);
  shoHit_rowVsSector_m2->SetXTitle("charge on loc. max.");
  shoHit_rowVsSector_m2->SetYTitle("sector");
  histList->Add(shoHit_rowVsSector_m2,"shower--logz");

  shoHit_colVsSector_m0 = new TH2F("shoHit_colVsSector_m0" ,"showerHit: Mod 0, sector vs. col",32,0.,32.,6,-0.5,5.5);
  shoHit_colVsSector_m0->SetXTitle("charge on loc. max.");
  shoHit_colVsSector_m0->SetYTitle("sector");
  histList->Add(shoHit_colVsSector_m0,"shower--logz");

  shoHit_colVsSector_m1 = new TH2F("shoHit_colVsSector_m1" ,"showerHit: Mod 1, sector vs. col",32,0.,32.,6,-0.5,5.5);
  shoHit_colVsSector_m1->SetXTitle("charge on loc. max.");
  shoHit_colVsSector_m1->SetYTitle("sector");
  histList->Add(shoHit_colVsSector_m1,"shower--logz");

  shoHit_colVsSector_m2 = new TH2F("shoHit_colVsSector_m2" ,"showerHit: Mod 2, sector vs. col",32,0.,32.,6,-0.5,5.5);
  shoHit_colVsSector_m2->SetXTitle("charge on loc. max.");
  shoHit_colVsSector_m2->SetYTitle("sector");
  histList->Add(shoHit_colVsSector_m2,"shower--logz");

  shoHit_sectorVsModule = new TH2F("shoHit_sectorVsModule" ,"showerHit: num hits by module",11,-0.25,5.25,5,-0.25,2.25);
  shoHit_sectorVsModule->SetXTitle("sector");
  shoHit_sectorVsModule->SetYTitle("module");
  histList->Add(shoHit_sectorVsModule,"shower--logz");

  shoHit_nSec = new TH1F("shoHit_nSec","showerHit: num hits/event/sector",6,-0.5,5.5);
  shoHit_nSec->SetXTitle("sector");
  histList->Add(shoHit_nSec,"shower--logy");

  shoHitTof_scat = new TH2F("shoHitTof_scat","showerHitTof: hit distribution in lab frame",100,-2100,2100,100,-2100,2100);
  shoHitTof_scat->SetXTitle("X_{lab} (mm)");
  shoHitTof_scat->SetYTitle("Y_{lab} (mm)");
  histList->Add(shoHitTof_scat,"shower--nobox");

  shoHitTof_nSec = new TH1F("shoHitTof_nSec","showerHitTof: num hits/event/sector",6,-0.5,5.5);
  shoHitTof_nSec->SetXTitle("sector");
  histList->Add(shoHitTof_nSec,"shower--logy");

}

void HQAHistograms::bookHistKick() {

  kickTrack_scat = new TH2F("kickTrack_scat","kickTrack: track distribution in lab frame",105,-1.05,1.05,105,-1.05,1.05);
  kickTrack_scat->SetXTitle("sin(theta)*sin(phi-90)");
  kickTrack_scat->SetYTitle("sin(theta)*cos(phi-90)");
  histList->Add(kickTrack_scat,"kickplane--nobox");

  kickTrack_pullVsSector = new TH2F("kickTrack_pullVsSector","kickTrack: sector vs. pull",120,-6.,6.,11,-0.25,5.25);
  kickTrack_pullVsSector->SetXTitle("pull");
  kickTrack_pullVsSector->SetYTitle("sector");
  histList->Add(kickTrack_pullVsSector,"kickplane--");

  kickTrack_nSec = new TH1F("kickTrack_nSec","kickTrack: num tracks/event/sector",6,-0.5,5.5);
  kickTrack_nSec->SetXTitle("sector");
  histList->Add(kickTrack_nSec,"shower--logy");

  kickTrack_massCharge = new TH1F("kickTrack_massCharge","kickTrack: mass * charge",125,-500.,2000.);
  kickTrack_massCharge->SetXTitle("mass*charge");
  histList->Add(kickTrack_massCharge,"kickplane");

}
