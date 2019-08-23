using namespace std;
#include <iostream> 
#include <iomanip>
#include <math.h>
// Root includes
#include <TObjString.h>
#include <TH1.h>
#include <TH2.h>
// Hydra includes
#include "hqahistograms.h"
#include "QAsectioncolors.h"

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-//
//*-- Author : Dan Magestro (magestro@gsi.de)
//*-- Modified : 24/01/2002
//*-- Modified : 20/01/2004 Jacek Otwinowski
//*-- Modified : 23/01/2004 A.Sadovski
//*-- Modified : 04/01/2005 Jacek Otwinowski
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

void HQAHistograms::resetPointers() 
{
// --- Zero all histogram pointers

  //---- Start histograms
  stCal_strip = 0;
  stCal_tof = 0;
  stHit_tof_vs_startstripe = 0;

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
  tfnCal_n        = 0;
  tfnCal_tdc      = 0;
  histProfTofino  = 0;
  histMultTofinoM = 0;
  histTofinoTof   = 0;


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
  shoHit_nCol=0;
  shoHit_nRow=0;

  for(Int_t s=0;s<6;s++){
     for(Int_t m=0;m<3;m++){
	shoHitSums[s][m]=0;
     }
  }
  //---- Tracking histograms
  kickTrack_scat             = 0;
  kickTrack_nSec             = 0;
  kickTrack_pullVsSector     = 0;
  kickTrack_massCharge       = 0;
  kickTrack123_scat          = 0;
  kickTrack123_massCharge    = 0;
  splineTrack_scat           = 0;
  splineTrack_massCharge     = 0;
  rungeKuttaTrack_scat       = 0;
  rungeKuttaTrack_massCharge = 0;
  for(Int_t s = 0; s < 6; s++)
  {
    trackingKick12M_sys0[s]=0;
    trackingKick123_sys0[s]=0;
    trackingSpline_sys0[ s]=0;
    trackingRK_sys0[     s]=0;
    trackingKick12M_sys1[s]=0;
    trackingKick123_sys1[s]=0;
    trackingSpline_sys1[ s]=0;
    trackingRK_sys1[     s]=0;
  }

  //---- Trigger histograms 
  trigRich_nhits   = 0;      
  trigRich_nSec    = 0;
  trigRich_theta   = 0;      
  trigRich_phi     = 0;        
  trigRich_rowVsCol= 0;   
  
  trigTof_nhits = 0;
  trigTof_nSec  = 0;
  trigTof_theta = 0;       
  trigTof_phi   = 0;         
  trigTof_time  = 0;        
  trigTof_xVsy  = 0;        
  
  trigShower_nhits   = 0;       
  trigShower_nSec    = 0;
  trigShower_theta   = 0;       
  trigShower_phi     = 0;         
  trigShower_rowVsCol= 0;    
  
  trigMULep_nhits   = 0;     
  trigMULep_TnSec   = 0;
  trigMULep_SnSec   = 0;
  trigMULep_momVsDTh= 0;  
  trigMULep_phiVsDPh= 0;    

  // Rich-MDC histograms

  richmdc_lep=0;
  for(Int_t s = 0; s < 6; s++)
  {
    richmdc_dtheta[s]=0;
    richmdc_dphi[s]=0;
  }

  // Rich-MDC histograms

  richkick_lep=0;
  richkick_ele=0;
  richkick_pos=0;
  for(Int_t s = 0; s < 6; s++)
  {
    richkick_dtheta[s]=0;
    richkick_dphi[s]=0;
    richkickS0_tof[s]=0;
    richkickS1_tof[s]=0;
  }

  for(Int_t s = 0; s < 2; s++)
  {
    richkickS_lep[s]=0;
    richkickS_tof_vs_rod[s]=0;
  }
  richkick_tof_vs_startstripe = 0;
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
  bookHistTrig();
  bookHistRichMDC();
  bookHistRichKick();

  activateHist();
}

void HQAHistograms::bookHistStart() {
char name[256];
char title[256];

  stCal_strip = new TH1F("stCal_strip","startCal: strip pattern",16,0.5,16.5);
  stCal_strip->SetXTitle("strip number");
  histList->Add(stCal_strip,"start--");

  stCal_tof = new TH1F("stCal_tof","startCal: time of flight",300,-10.0,10.0);
  stCal_tof->SetXTitle("time of flight");
  histList->Add(stCal_tof,"start--");
   
  sprintf(name,"stHit_tof_vs_startstripe");
  sprintf(title,"Start-Tof: stHit_tofTOF_vs_startstripe");
  stHit_tof_vs_startstripe = new TH2F(name,title,8,0,8,300,0,30);
  stHit_tof_vs_startstripe->SetXTitle("start stripe");
  stHit_tof_vs_startstripe->SetYTitle("tof");

  histList->Add(stHit_tof_vs_startstripe,"start--");
  
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
  histList->Add(mdcCal1_nVsSector_m0,"mdc--logz,sectorhist");

  mdcCal1_nVsSector_m1 = new TH2F("mdcCal1_nVsSector_m1","mdcCal1: Plane II, sector vs. multiplicity",51,-0.5,50.5,6,-0.5,5.5);
  mdcCal1_nVsSector_m1->SetXTitle("# fired wires");
  histList->Add(mdcCal1_nVsSector_m1,"mdc--logz,sectorhist");

  mdcCal1_nVsSector_m2 = new TH2F("mdcCal1_nVsSector_m2","mdcCal1: Plane III, sector vs. multiplicity",51,-0.5,50.5,6,-0.5,5.5);
  mdcCal1_nVsSector_m2->SetXTitle("# fired wires");
  histList->Add(mdcCal1_nVsSector_m2,"mdc--logz,sectorhist");

  mdcCal1_nVsSector_m3 = new TH2F("mdcCal1_nVsSector_m3","mdcCal1: Plane IV, sector vs. multiplicity",51,-0.5,50.5,6,-0.5,5.5);
  mdcCal1_nVsSector_m3->SetXTitle("# fired wires");
  histList->Add(mdcCal1_nVsSector_m3,"mdc--logz,sectorhist");

  mdcCal1_time1VsSector_m0 = new TH2F("mdcCal1_time1VsSector_m0","mdcCal1: Plane I, sector vs. time1",200,0.,1000.,6,-0.5,5.5);
  mdcCal1_time1VsSector_m0->SetXTitle("time1 (ns)");
  histList->Add(mdcCal1_time1VsSector_m0,"mdc--sectorhist");

  mdcCal1_time1VsSector_m1 = new TH2F("mdcCal1_time1VsSector_m1","mdcCal1: Plane II, sector vs. time1",200,0.,1000.,6,-0.5,5.5);
  mdcCal1_time1VsSector_m1->SetXTitle("time1 (ns)");
  histList->Add(mdcCal1_time1VsSector_m1,"mdc--sectorhist");

  mdcCal1_time1VsSector_m2 = new TH2F("mdcCal1_time1VsSector_m2","mdcCal1: Plane III, sector vs. time1",200,0.,1000.,6,-0.5,5.5);
  mdcCal1_time1VsSector_m2->SetXTitle("time1 (ns)");
  histList->Add(mdcCal1_time1VsSector_m2,"mdc--sectorhist");

  mdcCal1_time1VsSector_m3 = new TH2F("mdcCal1_time1VsSector_m3","mdcCal1: Plane IV, sector vs. time1",200,0.,1000.,6,-0.5,5.5);
  mdcCal1_time1VsSector_m3->SetXTitle("time1 (ns)");
  histList->Add(mdcCal1_time1VsSector_m3,"mdc--sectorhist");

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


  //***************************************************
  // time above threshold vs. drift time = t2-t1 vs. t1
  // here: 24 2-dim histograms, one could also think of one histogram per plane (=chamber type) = 4 2-dim histos
  //
  Char_t title[100], name[100];
  Float_t t1tatx[2] = {   0., 250.};
  Float_t t1taty[2] = { -20., 500.};
  Int_t   t1tatn[2] = { 250 , 370 };
  //
  //Sector-color description
  //                     blue,  red ,black, green,magn,yell
  //Int_t   SectColor[6]={   4 ,  2   ,  1  ,  8   , 6  ,  5 };
  #include "QAsectioncolors.h"
  //Char_t *SectDescr[6]={ "s1", "s3" ,"s6" , "s2" ,"s4","s5"};
  //please be careful every time with "same" option for
  //overlayed histograms: by definition D.Magestro made
  //normal 1-dim plot to be blue-colored. So first color
  //of any overlayed histogramm should be blue, otherwise
  //it will be blue anyway whyle printing histograms (finalisation
  //stage), so better if we agree to keep "blue" color to be
  //always first in a color pallete of any overlayed plots
  for(Int_t im=0;im<4; im++){
    sprintf(title,"(t2-t1) vs. t1 for plane%i",im);
    sprintf(name,"mdcCal1_t2mt1_vs_t1_plane[%i]",im);
    mdcCal1_t2mt1_vs_t1_plane[im] = new TH2F(name,title,t1tatn[0],t1tatx[0],t1tatx[1],t1tatn[1],t1taty[0],t1taty[1]);
    mdcCal1_t2mt1_vs_t1_plane[im]->SetXTitle("t2-t1 (ns)");
    mdcCal1_t2mt1_vs_t1_plane[im]->SetYTitle("t1 (ns)");
    histList->Add(mdcCal1_t2mt1_vs_t1_plane[im],"mdc--planehist");
  }

  for(Int_t im=0;im<4; im++){
    for(Int_t is=0;is<6; is++){
       //bi-plots--
       sprintf(title,"(t2-t1) vs. t1 for sector%i module%i",is,im);
       sprintf(name,"mdcCal1_t2mt1_vs_t1[%i][%i]",is,im);
       mdcCal1_t2mt1_vs_t1[is][im] = new TH2F(name,title,t1tatn[0],t1tatx[0],t1tatx[1],t1tatn[1],t1taty[0],t1taty[1]);
       mdcCal1_t2mt1_vs_t1[is][im]->SetXTitle("t2-t1 (ns)");
       mdcCal1_t2mt1_vs_t1[is][im]->SetYTitle("t1 (ns)");
       if(is==0){histList->Add(mdcCal1_t2mt1_vs_t1[is][im],"mdc--newpage");}
       else{     histList->Add(mdcCal1_t2mt1_vs_t1[is][im],"mdc--");}
    }
  }

  for(Int_t im=0;im<4; im++){
    for(Int_t is=0;is<6; is++){
       //dN/d[t1]
       //sprintf(title,"t1 for sector%i module%i",is,im);
       sprintf(title,"Drift time for mod%i s%i",im,is);
       sprintf(name,"mdcCal1_t1[%i][%i]",is,im);
       mdcCal1_t1[is][im] = new TH1F(name,title,t1tatn[1],t1taty[0],t1taty[1]);
       mdcCal1_t1[is][im]->SetXTitle("t1 (ns)");
       mdcCal1_t1[is][im]->SetYTitle("dN/d[t1]");
       mdcCal1_t1[is][im]->SetLineColor(SectColor[is]);
       mdcCal1_t1[is][im]->SetLineWidth(8);
       //if(is==0 && im==0){histList->Add(mdcCal1_t1[is][im],"mdc--newpage");}
       //else{
         if(is==0){ histList->Add(mdcCal1_t1[is][im],"mdc--6sec");}
         else{      histList->Add(mdcCal1_t1[is][im],"mdc--same--6sec");}
       //}
    }
  }

  for(Int_t im=0;im<4; im++){
    for(Int_t is=0;is<6; is++){
       //single-plots--
       //dN/d[t2-t1]
       //sprintf(title,"(t2-t1) for sector%i module%i",is,im);
       sprintf(title,"Time above threshold for mod%i s%i",im,is);
       sprintf(name,"mdcCal1_t2mt1[%i][%i]",is,im);
       mdcCal1_t2mt1[is][im] = new TH1F(name,title,t1tatn[0],t1tatx[0],t1tatx[1]);
       mdcCal1_t2mt1[is][im]->SetXTitle("t2-t1 (ns)");
       mdcCal1_t2mt1[is][im]->SetYTitle("dN/d[t2-t1] (ns)");
       mdcCal1_t2mt1[is][im]->SetLineColor(SectColor[is]);
       mdcCal1_t2mt1[is][im]->SetLineWidth(8);
       if(is==0 && im==0){histList->Add(mdcCal1_t2mt1[is][im],"mdc--newpage");}
       else{     
         if(is==0){ histList->Add(mdcCal1_t2mt1[is][im],"mdc--6sec");}
         else{      histList->Add(mdcCal1_t2mt1[is][im],"mdc--same--6sec");}
       }
    }
  }
  //***************************************************


  mdcHit_scat_m0 = new TH2F("mdcHit_scat_m0","mdcHit: Plane I, hit distribution",100,-900,900,100,-900,900);
  mdcHit_scat_m0->SetXTitle("view from target");
  histList->Add(mdcHit_scat_m0,"mdc--newpage");

  mdcHit_scat_m1 = new TH2F("mdcHit_scat_m1","mdcHit: Plane II, hit distribution",100,-1000,1000,100,-1000,1000);
  mdcHit_scat_m1->SetXTitle("view from target");
  histList->Add(mdcHit_scat_m1,"mdc--");

  mdcHit_scat_m2 = new TH2F("mdcHit_scat_m2","mdcHit: Plane III, hit distribution",100,-2000,2000,100,-2000,2000);
  mdcHit_scat_m2->SetXTitle("view from target");
  histList->Add(mdcHit_scat_m2,"mdc--");

  mdcHit_scat_m3 = new TH2F("mdcHit_scat_m3","mdcHit: Plane IV, hit distribution",100,-2500,2500,100,-2500,2500);
  mdcHit_scat_m3->SetXTitle("view from target");
  histList->Add(mdcHit_scat_m3,"mdc--");

  //***************************************************
  //                 blue ,magn,yell, red , gray , green  , lightgray
  Int_t   color[7]={  4   , 6 ,  5 ,   2  ,  12  ,   8    ,  32    };
  //Char_t *descr[7]={"All","t1","t2","t1t2","wire","t1+t12","cutall"};
  for(Int_t i=0; i<7; i++){
      sprintf(title,"Events and cut counts %i",i);
      sprintf(name,"mdcCutCounts[%i]",i);
      mdcCutCounts[i] = new TH1F(name,title,24,0.,24.);
      mdcCutCounts[i]->SetXTitle("p1{s1,s2,...,s6}, p2{s1,...,s6},...,p4{s1..}");
      mdcCutCounts[i]->SetYTitle("Counts");
      mdcCutCounts[i]->SetLineColor(color[i]);
      mdcCutCounts[i]->SetLineWidth(8);
      if(i==0){mdcCutCounts[i]->SetFillColor(color[i]);}
      if(i==0){
        histList->Add(mdcCutCounts[i],"mdc--");
      }
      else{
        histList->Add(mdcCutCounts[i],"mdc--same");
      }
      
  }
  //***************************************************


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


  //"Tofino profile"
  histProfTofino = new TH1F("histProfTofino","Tofino Profile",26,0.,26.);
  histProfTofino->TH1::SetStats(kFALSE);
  histProfTofino->SetNdivisions(220);
  histProfTofino->GetXaxis()->SetTitle("Tofino Cell #");
  histProfTofino->GetYaxis()->SetTitle("Number of Hits / Max number of Hits");
  histList->Add(histProfTofino,"tofino--");

  histMultTofinoM = new TH1F("histMultTofinoM","Multiplicity in Cell",15,0.,15.);
  histMultTofinoM->SetXTitle("Number of hits in cell");
  histMultTofinoM->SetYTitle("Counts");
  histList->Add(histMultTofinoM,"tofino--");

  histTofinoTof = new TH1F("histTofinoTof","Time of flight in TOFINO", 50,0.,50.);
  histTofinoTof->SetXTitle("tof [ns]");
  histTofinoTof->SetYTitle("Counts");
  histList->Add(histTofinoTof,"tofino--");

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
  histList->Add(shoHit_chargeVsSector_m0,"shower--logz,sectorhist");

  shoHit_chargeVsSector_m1 = new TH2F("shoHit_chargeVsSector_m1" ,"showerHit: Mod 1, charge on loc. max.",100,0.,100.,6,-0.5,5.5);
  shoHit_chargeVsSector_m1->SetXTitle("charge on loc. max.");
  histList->Add(shoHit_chargeVsSector_m1,"shower--logz,sectorhist");

  shoHit_chargeVsSector_m2 = new TH2F("shoHit_chargeVsSector_m2" ,"showerHit: Mod 2, charge on loc. max.",100,0.,100.,6,-0.5,5.5);
  shoHit_chargeVsSector_m2->SetXTitle("charge on loc. max.");
  histList->Add(shoHit_chargeVsSector_m2,"shower--logz,sectorhist");

  shoHit_rowVsSector_m0 = new TH2F("shoHit_rowVsSector_m0" ,"showerHit: Mod 0, sector vs. row",32,0.,32.,6,-0.5,5.5);
  shoHit_rowVsSector_m0->SetXTitle("charge on loc. max.");
  histList->Add(shoHit_rowVsSector_m0,"shower--logz,sectorhist");

  shoHit_rowVsSector_m1 = new TH2F("shoHit_rowVsSector_m1" ,"showerHit: Mod 1, sector vs. row",32,0.,32.,6,-0.5,5.5);
  shoHit_rowVsSector_m1->SetXTitle("charge on loc. max.");
  histList->Add(shoHit_rowVsSector_m1,"shower--logz,sectorhist");

  shoHit_rowVsSector_m2 = new TH2F("shoHit_rowVsSector_m2" ,"showerHit: Mod 2, sector vs. row",32,0.,32.,6,-0.5,5.5);
  shoHit_rowVsSector_m2->SetXTitle("charge on loc. max.");
  histList->Add(shoHit_rowVsSector_m2,"shower--logz,sectorhist");

  shoHit_colVsSector_m0 = new TH2F("shoHit_colVsSector_m0" ,"showerHit: Mod 0, sector vs. col",32,0.,32.,6,-0.5,5.5);
  shoHit_colVsSector_m0->SetXTitle("charge on loc. max.");
  histList->Add(shoHit_colVsSector_m0,"shower--logz,sectorhist");

  shoHit_colVsSector_m1 = new TH2F("shoHit_colVsSector_m1" ,"showerHit: Mod 1, sector vs. col",32,0.,32.,6,-0.5,5.5);
  shoHit_colVsSector_m1->SetXTitle("charge on loc. max.");
  histList->Add(shoHit_colVsSector_m1,"shower--logz,sectorhist");

  shoHit_colVsSector_m2 = new TH2F("shoHit_colVsSector_m2" ,"showerHit: Mod 2, sector vs. col",32,0.,32.,6,-0.5,5.5);
  shoHit_colVsSector_m2->SetXTitle("charge on loc. max.");
  histList->Add(shoHit_colVsSector_m2,"shower--logz,sectorhist");

  shoHit_sectorVsModule = new TH2F("shoHit_sectorVsModule" ,"showerHit: Sectors vs. Modules",11,-0.25,5.25,5,-0.25,2.25);
  shoHit_sectorVsModule->SetXTitle("sector");
  shoHit_sectorVsModule->SetYTitle("module");
  shoHit_sectorVsModule->GetXaxis()->SetTitleOffset(4);
  shoHit_sectorVsModule->GetYaxis()->SetTitleOffset(4);
  histList->Add(shoHit_sectorVsModule,"shower--lego2");
	shoHit_nRow = new TH1F("shoHit_nRow","ShowerHit: Rows (All sectors)",32,0,31);
	shoHit_nRow->SetXTitle("Row");
	shoHit_nCol = new TH1F("shoHit_nCol","ShowerHit: Cols (All sectors)",32,0,31);
	shoHit_nCol->SetXTitle("Col");
  histList->Add(shoHit_nRow,"shower--");
  histList->Add(shoHit_nCol,"shower--");
	Int_t   color[3]={  4   , 30 ,  6 };
	char buf[80];
	char buf1[80];
	for(Int_t s=0;s<6;s++){
		for(Int_t m=0;m<3;m++){
			sprintf(buf,"showersums_%i_%i",s,m);
			sprintf(buf1,"Shower Sums (Normalized): Sector %i",s);
			shoHitSums[s][m] = new TH1F(buf,buf1,80,0,80);
			shoHitSums[s][m]->SetXTitle("Charge");
			shoHitSums[s][m]->SetLineColor(color[m]);
			shoHitSums[s][m]->SetLineWidth(8);
			if(s==0 && m==0)
			histList->Add(shoHitSums[s][m],"shower--newpage--3mod");
			else {
				if(m==0) histList->Add(shoHitSums[s][m],"shower--3mod");
				else histList->Add(shoHitSums[s][m],"shower--same--3mod");
			}
		}
	}
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
  histList->Add(kickTrack_scat,"tracking--nobox");

  kickTrack_pullVsSector = new TH2F("kickTrack_pullVsSector","kickTrack: sector vs. pull",120,-6.,6.,11,-0.25,5.25);
  kickTrack_pullVsSector->SetXTitle("pull");
  kickTrack_pullVsSector->SetYTitle("sector");
  histList->Add(kickTrack_pullVsSector,"tracking--");

  kickTrack_nSec = new TH1F("kickTrack_nSec","kickTrack: num tracks/event/sector",6,-0.5,5.5);
  kickTrack_nSec->SetXTitle("sector");
  histList->Add(kickTrack_nSec,"shower--logy");

  kickTrack_massCharge = new TH1F("kickTrack_massCharge","kickTrack: mass * charge",125,-500.,2000.);
  kickTrack_massCharge->SetXTitle("mass*charge");
  histList->Add(kickTrack_massCharge,"tracking--");

  kickTrack123_scat = new TH2F("kickTrack123_scat","kickTrack123B: track distribution in lab frame",105,-1.05,1.05,105,-1.05,1.05);
  kickTrack123_scat->SetXTitle("sin(theta)*sin(phi-90)");
  kickTrack123_scat->SetYTitle("sin(theta)*cos(phi-90)");
  histList->Add(kickTrack123_scat,"tracking--nobox");

  kickTrack123_massCharge = new TH1F("kickTrack123_massCharge","kickTrack123: mass * charge",125,-500.,2000.);
  kickTrack123_massCharge->SetXTitle("mass*charge");
  histList->Add(kickTrack123_massCharge,"tracking--");

  splineTrack_scat = new TH2F("splineTrack_scat","splineTrack: track distribution in lab frame",105,-1.05,1.05,105,-1.05,1.05);
  splineTrack_scat->SetXTitle("sin(theta)*sin(phi-90)");
  splineTrack_scat->SetYTitle("sin(theta)*cos(phi-90)");
  histList->Add(splineTrack_scat,"tracking--nobox");

  splineTrack_massCharge = new TH1F("splineTrack_massCharge","splineTrack: mass * charge",125,-500.,2000.);
  splineTrack_massCharge->SetXTitle("mass*charge");
  histList->Add(splineTrack_massCharge,"tracking--");

  rungeKuttaTrack_scat = new TH2F("rungeKuttaTrack_scat","Runge Kutta: track distribution in lab frame",105,-1.05,1.05,105,-1.05,1.05);
  rungeKuttaTrack_scat->SetXTitle("sin(theta)*sin(phi-90)");
  rungeKuttaTrack_scat->SetYTitle("sin(theta)*cos(phi-90)");
  histList->Add(rungeKuttaTrack_scat,"tracking--nobox");

  rungeKuttaTrack_massCharge = new TH1F("rungeKuttaTrack_massCharge","Runge Kutta: mass * charge",125,-500.,2000.);
  rungeKuttaTrack_massCharge->SetXTitle("mass*charge");
  histList->Add(rungeKuttaTrack_massCharge,"tracking--");
  
  char name[256],title[256];
  //Int_t   SectColor[6]={   4 ,  2   ,  1  ,  8   , 6  ,  5 };

  //For TOF-region, Then TOFINO/SHOWER than again for TOF and again for TOFINO... To have nice look on PDF
  for(Int_t s=0; s<6; s++){//for TOFINO/SHOWER
    sprintf(name ,"trackingKick12M_sys0[%d]",s);
    sprintf(title,"KickTrack12-META sec%d (mass for TOFINO)",s);
    trackingKick12M_sys0[s] = new TH1F(name,title,125,-500,2000);

    trackingKick12M_sys0[s]->SetXTitle("M [MeV]");
    trackingKick12M_sys0[s]->SetYTitle("counts/event");
    trackingKick12M_sys0[s]->SetLineColor(SectColor[s]);
    trackingKick12M_sys0[s]->SetLineWidth(4);

    if(s == 0){ histList->Add(trackingKick12M_sys0[s],"tracking--6sec--newpage");       }
    else      { histList->Add(trackingKick12M_sys0[s],"tracking--same--6sec"); }
  }
  for(Int_t s=0; s<6; s++){//for TOF
    sprintf(name ,"trackingKick12M_sys1[%d]",s);
    sprintf(title,"KickTrack12-META sec%d (mass for TOF)",s);
    trackingKick12M_sys1[s] = new TH1F(name,title,125,-500,2000);

    trackingKick12M_sys1[s]->SetXTitle("M [MeV]");
    trackingKick12M_sys1[s]->SetYTitle("counts/event");
    trackingKick12M_sys1[s]->SetLineColor(SectColor[s]);
    trackingKick12M_sys1[s]->SetLineWidth(4);

    if(s == 0){ histList->Add(trackingKick12M_sys1[s],"tracking--6sec");       }
    else      { histList->Add(trackingKick12M_sys1[s],"tracking--same--6sec"); }
  }
  
  for(Int_t s=0; s<6; s++){ //for TOFINO/SHOWER
    sprintf(name ,"trackingKick123_sys0[%d]",s);
    sprintf(title,"Mass KickTrack123(4) sec%d (mass for TOFINO)",s);
    trackingKick123_sys0[s] = new TH1F(name,title,125,-500,2000);

    trackingKick123_sys0[s]->SetXTitle("M [MeV]");
    trackingKick123_sys0[s]->SetYTitle("counts/event");
    trackingKick123_sys0[s]->SetLineColor(SectColor[s]);
    trackingKick123_sys0[s]->SetLineWidth(4);

    if(s == 0){ histList->Add(trackingKick123_sys0[s],"tracking--6sec");     }
    else      { histList->Add(trackingKick123_sys0[s],"tracking--same--6sec"); }
  }
  for(Int_t s=0; s<6; s++){//for TOF
    sprintf(name ,"trackingKick123_sys1[%d]",s);
    sprintf(title,"Mass KickTrack123(4) sec%d (mass for TOF)",s);
    trackingKick123_sys1[s] = new TH1F(name,title,125,-500,2000);

    trackingKick123_sys1[s]->SetXTitle("M [MeV]");
    trackingKick123_sys1[s]->SetYTitle("counts/event");
    trackingKick123_sys1[s]->SetLineColor(SectColor[s]);
    trackingKick123_sys1[s]->SetLineWidth(4);

    if(s == 0){ histList->Add(trackingKick123_sys1[s],"tracking--6sec");     }
    else      { histList->Add(trackingKick123_sys1[s],"tracking--same--6sec"); }
  }
 
  for(Int_t s=0; s<6; s++){//for TOFINO/SHOWER
    sprintf(name ,"trackingSpline_sys0[%d]",s);
    sprintf(title,"Mass SplineTrack123(4) sec%d (mass for TOFINO)",s);
    trackingSpline_sys0[s] = new TH1F(name,title,125,-500,2000);

    trackingSpline_sys0[s]->SetXTitle("M [MeV]");
    trackingSpline_sys0[s]->SetYTitle("counts/event");
    trackingSpline_sys0[s]->SetLineColor(SectColor[s]);
    trackingSpline_sys0[s]->SetLineWidth(4);

    if(s == 0){ histList->Add(trackingSpline_sys0[s],"tracking--6sec");	}
    else      { histList->Add(trackingSpline_sys0[s],"tracking--same--6sec"); }
  }
  for(Int_t s=0; s<6; s++){//for TOF
    sprintf(name ,"trackingSpline_sys1[%d]",s);
    sprintf(title,"Mass SplineTrack123(4) sec%d (mass for TOF)",s);
    trackingSpline_sys1[s] = new TH1F(name,title,125,-500,2000);

    trackingSpline_sys1[s]->SetXTitle("M [MeV]");
    trackingSpline_sys1[s]->SetYTitle("counts/event");
    trackingSpline_sys1[s]->SetLineColor(SectColor[s]);
    trackingSpline_sys1[s]->SetLineWidth(4);

    if(s == 0){ histList->Add(trackingSpline_sys1[s],"tracking--6sec");	}
    else      { histList->Add(trackingSpline_sys1[s],"tracking--same--6sec"); }
  }

  for(Int_t s=0; s<6; s++){//for TOFINO/SHOWER
    sprintf(name ,"trackingRK_sys0[%d]",s);
    sprintf(title,"Mass Runge.K.Track123(4) sec%d (mass for TOFINO)",s);
    trackingRK_sys0[s] = new TH1F(name,title,125,-500,2000);

    trackingRK_sys0[s]->SetXTitle("M [MeV]");
    trackingRK_sys0[s]->SetYTitle("counts/event");
    trackingRK_sys0[s]->SetLineColor(SectColor[s]);
    trackingRK_sys0[s]->SetLineWidth(4);

    if(s == 0){ histList->Add(trackingRK_sys0[s],"tracking--6sec");	}
    else      { histList->Add(trackingRK_sys0[s],"tracking--same--6sec"); }
  }
  for(Int_t s=0; s<6; s++){//for TOF
    sprintf(name ,"trackingRK_sys1[%d]",s);
    sprintf(title,"Mass Runge.K.Track123(4) sec%d (mass for TOF)",s);
    trackingRK_sys1[s] = new TH1F(name,title,125,-500,2000);

    trackingRK_sys1[s]->SetXTitle("M [MeV]");
    trackingRK_sys1[s]->SetYTitle("counts/event");
    trackingRK_sys1[s]->SetLineColor(SectColor[s]);
    trackingRK_sys1[s]->SetLineWidth(4);

    if(s == 0){ histList->Add(trackingRK_sys1[s],"tracking--6sec");	}
    else      { histList->Add(trackingRK_sys1[s],"tracking--same--6sec"); }
  }

}


void HQAHistograms::bookHistTrig() {
  trigRich_nhits = new TH1F("trigRich_nhits","trigRich: multiplicity distribution",20,0,20);
  trigRich_nhits->SetXTitle("hits per event");
  histList->Add(trigRich_nhits,"trigger--logy");

  trigRich_nSec = new TH1F("trigRich_nSec","trigRich: num hits/event/sector",6,-0.5,5.5);
  trigRich_nSec->SetXTitle("sector");
  histList->Add(trigRich_nSec,"trigger--logy");

  trigRich_theta = new TH1F("trigRich_theta","trigRich: theta angle",80,10.,90.);
  trigRich_theta->SetXTitle("#theta (degrees)");
  histList->Add(trigRich_theta,"trigger--");

  trigRich_phi = new TH1F("trigRich_phi","trigRich: phi angle",120,0.,360.);
  trigRich_phi->SetXTitle("#phi (degrees)");
  histList->Add(trigRich_phi,"trigger--");

  trigRich_rowVsCol = new TH2F("trigRich_rowVsCol","trigRich: hit distribution",220,-110.,110.,220,-110,110);
  trigRich_rowVsCol->SetXTitle("Col (pads)");
  trigRich_rowVsCol->SetYTitle("Row (pads)");
  histList->Add(trigRich_rowVsCol,"trigger--");


  trigTof_nhits = new TH1F("trigTof_nhits","trigTof: multiplicity distribution",50,0,50);
  trigTof_nhits->SetXTitle("hits per event");
  histList->Add(trigTof_nhits,"trigger--");

  trigTof_nSec = new TH1F("trigTof_nSec","trigTof: num hits/event/sector",6,-0.5,5.5);
  trigTof_nSec->SetXTitle("sector");
  histList->Add(trigTof_nSec,"trigger--logy");

  trigTof_theta = new TH1F("trigTof_theta","trigTof: theta angle",50,40.,90.);
  trigTof_theta->SetXTitle("#theta (degrees)");
  histList->Add(trigTof_theta,"trigger--");

  trigTof_phi = new TH1F("trigTof_phi","trigTof: phi angle",120,0.,360.);
  trigTof_phi->SetXTitle("#phi (degrees)");
  histList->Add(trigTof_phi,"trigger--");

  trigTof_time = new TH1F("trigTof_time","trigTof: time of flight",120,0.,120.);
  trigTof_time->SetXTitle("tof (ns)");
  histList->Add(trigTof_time,"trigger--");

  // not used!!
  trigTof_xVsy = new TH2F("trigTof_xVsy","trigTof: hit distribution",96,0,96,96,0,96);
  trigTof_xVsy->SetXTitle("X (pads)");
  trigTof_xVsy->SetYTitle("Y (pads)");
  histList->Add(trigTof_xVsy,"trigger--");


  trigShower_nhits = new TH1F("trigShower_nhits","trigShower: multiplicity distribution",50,0,50);
  trigShower_nhits->SetXTitle("hits per event");
  histList->Add(trigShower_nhits,"trigger--");

  trigShower_nSec = new TH1F("trigShower_nSec","trigShower: num hits/event/sector",6,-0.5,5.5);
  trigShower_nSec->SetXTitle("sector");
  histList->Add(trigShower_nSec,"trigger--logy");

  trigShower_theta = new TH1F("trigShower_theta","trigShower: theta angle",50,10.,50.);
  trigShower_theta->SetXTitle("#theta (degrees)");
  histList->Add(trigShower_theta,"trigger--");

  trigShower_phi = new TH1F("trigShower_phi","trigShower: phi angle",120,0.,360.);
  trigShower_phi->SetXTitle("#phi (degrees)");
  histList->Add(trigShower_phi,"trigger--");

  trigShower_rowVsCol = new TH2F("trigShower_rowVsCol","trigShower: hit distribution",120,-60,60,120,-60,60);
  trigShower_rowVsCol->SetXTitle("Col (pads)");
  trigShower_rowVsCol->SetYTitle("Row (pads)");
  histList->Add(trigShower_rowVsCol,"trigger--");


  trigMULep_nhits = new TH1F("trigMULep_nhits","trigMULep: multiplicity distribution",10,0,10);
  trigMULep_nhits->SetXTitle("hits per event");
  histList->Add(trigMULep_nhits,"trigger--logy");

  trigMULep_TnSec = new TH1F("trigMULep_TnSec","trigMULep: num hits/event/sector (tof)",6,-0.5,5.5);
  trigMULep_TnSec->SetXTitle("sector");
  histList->Add(trigMULep_TnSec,"trigger--logy");

  trigMULep_SnSec = new TH1F("trigMULep_SnSec","trigMULep: num hits/event/sector (shower)",6,-0.5,5.5);
  trigMULep_SnSec->SetXTitle("sector");
  histList->Add(trigMULep_SnSec,"trigger--logy");

  trigMULep_momVsDTh = new TH2F("trigMULep_momVsDTh","trigMULep: momentum(*charge) versus Delta_theta",90,0,90,200,-1000,1000);
  trigMULep_momVsDTh->SetXTitle("Delta_Theta (degrees)");
  trigMULep_momVsDTh->SetYTitle("Momentum * Charge (MeV)");
  histList->Add(trigMULep_momVsDTh,"trigger--");

  trigMULep_phiVsDPh = new TH2F("trigMULep_phiVsDPh","trigMULep: Delta_phi versus Phi",60,0,60,120,-60,60);
  trigMULep_phiVsDPh->SetXTitle("Phi (degrees)");
  trigMULep_phiVsDPh->SetYTitle("Delta_phi (degrees)");
  histList->Add(trigMULep_phiVsDPh,"trigger--");

}

void HQAHistograms::bookHistRichMDC()
{
char name[256],title[256];
Int_t   SectColor[6]={   4 ,  2   ,  1  ,  8   , 6  ,  5 };

  for(Int_t s = 0; s < 6; s++)
  {
    sprintf(name,"richmdc_dtheta_sec%d",s);
    sprintf(title,"RichHit-MdcInerSeg: delta theta sec%d",s);
    richmdc_dtheta[s] = new TH1F(name,title,80,-40,40);

    richmdc_dtheta[s]->SetXTitle("#Delta #Theta [deg]");
    richmdc_dtheta[s]->SetYTitle("counts/event");
    richmdc_dtheta[s]->SetLineColor(SectColor[s]);
    richmdc_dtheta[s]->SetLineWidth(4);

    if(s == 0) histList->Add(richmdc_dtheta[s],"richmdc--6sec");
    else histList->Add(richmdc_dtheta[s],"richmdc--same--6sec");

  }

  for(Int_t s = 0; s < 6; s++)
  {
    sprintf(name,"richmdc_dphi_sec%d",s);
    sprintf(title,"RichHit-MdcInerSeg: delta phi sec%d",s);
    richmdc_dphi[s] = new TH1F(name,title,80,-40,40);
    richmdc_dphi[s]->SetXTitle("#Delta #Phi*sin#Theta [deg]");
    richmdc_dphi[s]->SetYTitle("counts/event");
    richmdc_dphi[s]->SetLineColor(SectColor[s]);
    richmdc_dphi[s]->SetLineWidth(4);

    if(s ==0) histList->Add(richmdc_dphi[s],"richmdc--6sec");
    else  histList->Add(richmdc_dphi[s],"richmdc--same--6sec");
  }

  richmdc_lep = new TH1F("richmdc_lep","RichHit-MdcInerSeg: leptons (|#Delta #Theta|<5 deg, |#Delta #Phi|<5 deg) ",6,0,6);
  richmdc_lep->SetXTitle("sector");
  richmdc_lep->SetYTitle("leptons/event");
  richmdc_lep->SetLineWidth(4);
  histList->Add(richmdc_lep,"richmdc--");
}


void HQAHistograms::bookHistRichKick()
{
char name[256],title[256];
Int_t   SectColor[6]={   4 ,  2   ,  1  ,  8   , 6  ,  5 };

  for(Int_t s = 0; s < 6; s++)
  {
    sprintf(name,"richkick_dtheta_sec%d",s);
    sprintf(title,"RichHit-KickTrack: delta theta sec%d",s);
    richkick_dtheta[s] = new TH1F(name,title,80,-40,40);
    richkick_dtheta[s]->SetXTitle("#Delta #Theta [deg]");
    richkick_dtheta[s]->SetYTitle("counts/event");
    richkick_dtheta[s]->SetLineColor(SectColor[s]);
    richkick_dtheta[s]->SetLineWidth(4);

    if(s==0) histList->Add(richkick_dtheta[s],"richkick--6sec");
    else histList->Add(richkick_dtheta[s],"richkick--same--6sec");
  }

  for(Int_t s = 0; s < 6; s++)
  {
    sprintf(name,"richkick_dphi_sec%d",s);
    sprintf(title,"RichHit-KickTrack: delta phi sec%d",s);
    richkick_dphi[s] = new TH1F(name,title,80,-40,40);
    richkick_dphi[s]->SetXTitle("#Delta #Phi*sin#Theta [deg]");
    richkick_dphi[s]->SetYTitle("counts/event");
    richkick_dphi[s]->SetLineColor(SectColor[s]);
    richkick_dphi[s]->SetLineWidth(4);
    if(s==0) histList->Add(richkick_dphi[s],"richkick--6sec");
    else  histList->Add(richkick_dphi[s],"richkick--same--6sec");

  }

  for(Int_t s = 0; s < 6; s++)
  {
    sprintf(name,"richkick_tof_sys0_sec%d",s);
    sprintf(title,"RichHit-KickTrack: tof sys0 sec%d (|#Delta #Theta|<5 deg, |#Delta #Phi|<5 deg)",s);
    richkickS0_tof[s] = new TH1F(name,title,45,0,15);
    richkickS0_tof[s]->SetXTitle("tof [ns]");
    richkickS0_tof[s]->SetYTitle("counts/event");
    richkickS0_tof[s]->SetLineColor(SectColor[s]);
    richkickS0_tof[s]->SetLineWidth(4);
    if(s==0) histList->Add(richkickS0_tof[s],"richkick--6sec");
    else  histList->Add(richkickS0_tof[s],"richkick--same--6sec");
  }

  for(Int_t s = 0; s < 6; s++)
  {
    sprintf(name,"richkick_tof_sys1_sec%d",s);
    sprintf(title,"RichHit-KickTrack: tof sys1 sec%d (|#Delta #Theta|<5 deg, |#Delta #Phi|<5 deg)",s);
    richkickS1_tof[s] = new TH1F(name,title,45,0,15);
    richkickS1_tof[s]->SetXTitle("tof [ns]");
    richkickS1_tof[s]->SetYTitle("counts/event");
    richkickS1_tof[s]->SetLineColor(SectColor[s]);
    richkickS1_tof[s]->SetLineWidth(4);
    if(s==0) histList->Add(richkickS1_tof[s],"richkick--6sec");
    else  histList->Add(richkickS1_tof[s],"richkick--same--6sec");
  }

  richkick_lep = new TH1F("richkick_lep","RichHit-KickTrack: leptons (|#Delta #Theta|<5 deg, |#Delta #Phi|<5 deg) ",6,0,6);
  richkick_lep->SetXTitle("sector");
  richkick_lep->SetYTitle("leptons/event");
  richkick_lep->SetLineWidth(4);
  histList->Add(richkick_lep,"richkick--newpage");

  richkick_ele = new TH1F("richkick_ele","RichHit-KickTrack: electrons (|#Delta #Theta|<5 deg, |#Delta #Phi|<5 deg) ",6,0,6);
  richkick_ele->SetXTitle("sector");
  richkick_ele->SetYTitle("electrons/event");
  richkick_ele->SetLineWidth(4);
  histList->Add(richkick_ele,"richkick--");

  richkick_pos = new TH1F("richkick_pos","RichHit-KickTrack: positrons (|#Delta #Theta|<5 deg, |#Delta #Phi|<5 deg) ",6,0,6);
  richkick_pos->SetXTitle("sector");
  richkick_pos->SetYTitle("positrons/event");
  histList->Add(richkick_pos,"richkick--");

  for(Int_t iSys=0; iSys<2; iSys++)
  {
      sprintf(name,"richkick_lep_sys%d",iSys);
      sprintf(title,"RichHit-KickTrack: leptons sys%d (|#Delta #Theta|<5 deg,|#Delta #Phi|<5 deg)",iSys);

      richkickS_lep[iSys] = new TH1F(name,title,6,0,6);
      richkickS_lep[iSys]->SetXTitle("sector");
      richkickS_lep[iSys]->SetYTitle("leptons/event");
      richkickS_lep[iSys]->SetLineWidth(4);
      histList->Add(richkickS_lep[iSys],"richkick--");
  }

  for(Int_t iSys=0; iSys<2; iSys++)
  {
	sprintf(name,"richkick_tof_vs_rod_sys%d",iSys);
	sprintf(title,"RichHit-KickTrack: tof_vs_rod sys%d (|#Delta #Theta|<5 deg,|#Delta #Phi|<5 deg)",iSys);

	if(iSys==0) richkickS_tof_vs_rod[iSys] = new TH2F(name,title,24,0,24,200,0,20);
	if(iSys==1)  richkickS_tof_vs_rod[iSys] = new TH2F(name,title,384,0,384,200,0,20);
	
	richkickS_tof_vs_rod[iSys]->SetXTitle("rod");
	richkickS_tof_vs_rod[iSys]->SetYTitle("tof");
	
	histList->Add(richkickS_tof_vs_rod[iSys],"richkick--");
  }

    sprintf(name,"richkick_tof_vs_startstripe");
    sprintf(title,"RichHit-KickTrack: tofTOF_vs_startstripe (|#Delta #Theta|<5 deg,|#Delta #Phi|<5 deg)");
	richkick_tof_vs_startstripe = new TH2F(name,title,8,0,8,200,0,20);

	richkick_tof_vs_startstripe->SetXTitle("start stripe");
	richkick_tof_vs_startstripe->SetYTitle("tof");
	histList->Add(richkick_tof_vs_startstripe,"richkick--");
}
