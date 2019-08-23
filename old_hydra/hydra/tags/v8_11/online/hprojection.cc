//*-- Author : Dmitry Vasiliev (vassili@lns.infn.it)
/* 
 * Copyright P.Finocchiaro and D.Vasiliev (INFN-LNS, Catania) HADES collaboration
*/

//-------------------------------------------------------------------------
//
// File name:       hprojection.cc
// Subject:         Slicing of 2dim histogrammes.
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            March 2000
//
// Remarks:         Obsolete class, which was used at the prototyping stage.
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the source code for the member functions of 
// the class HProjection.
// This class makes projections of 2dim histogrammes.
//
//-------------------------------------------------------------------------

#include "hprojection.h"
#include <TH1.h>
#include <TH2.h>
#include <TROOT.h>

//_HADES_CLASS_DESCRIPTION 
/////////////////////////////////////////////////////////////////////
// This class makes projections of 2dim histogrammes.
// Remark: obsolete class, which was used at the prototyping stage.
/////////////////////////////////////////////////////////////////////
ClassImp(HProjection)

void HProjection::project(TCanvas *p, Int_t canvid) {

  if(!p) return;

  TObject *select = p->GetSelected();
  if(!select) return;

  if(!select->InheritsFrom("TH2")) { p->SetUniqueID(0); return; }
  if(strcmp(select->GetOption()," ")) { p->SetUniqueID(0); return; }

  p->FeedbackMode(kTRUE);
  
  Int_t pyold = p->GetUniqueID();
  Int_t py = p->GetEventY();
  Float_t uxmin = p->GetUxmin();
  Float_t uxmax = p->GetUxmax();
  Int_t pxmin = p->XtoAbsPixel(uxmin);
  Int_t pxmax = p->XtoAbsPixel(uxmax);
  if(pyold) gVirtualX->DrawLine(pxmin,pyold,pxmax,pyold);
  gVirtualX->DrawLine(pxmin,py,pxmax,py);
  p->SetUniqueID(py);
  Float_t upy = p->AbsPixeltoY(py);
  Float_t y = p->PadtoY(upy);
  
  TCanvas *c2 = 0;
  
  if(canvid == 1) 
    c2 = (TCanvas*)gROOT->GetListOfCanvases()->FindObject("proj");
  if(canvid == 2) 
    c2 = (TCanvas*)gROOT->GetListOfCanvases()->FindObject("c2");
  if(c2 && c2->GetPrimitive("Projection") && 
     c2->GetPrimitive("Projection")->IsOnHeap()) 
     delete c2->GetPrimitive("Projection");
  if(!c2) c2 = new TCanvas("c2","c2",0,0,500,500);
  c2->cd();
  
  TH2* h = (TH2*)select;
  Int_t biny = h->GetYaxis()->FindBin(y);
  TH1D *hp = h->ProjectionX("",biny,biny);
  char title[80];
  sprintf(title,"Projection of biny=%d",biny);
  hp->SetName("Projection");
  hp->SetTitle(title);
  hp->Draw();
  c2->Update();

}



  




