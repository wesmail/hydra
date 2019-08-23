//-------------------------------------------------------------------------
//
// File name:       hslice.cc
// Subject:         Slicing of 2dim histogrammes.
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            March 2000
//
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the source code for the member functions of 
// the class HSlice.
// This is a GUI class which allows to slice 2dim histogrammes.
//
//-------------------------------------------------------------------------

#include "hslice.h"
#include <TGLabel.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <iostream.h>
#include <TIterator.h>
#include <TLine.h>
#include <TMarker.h>
#include <TObjArray.h>
#include <TArrayI.h>
#include <stdlib.h>

ClassImp(HSlice)


TH1D *slice;
TH1D *highlight;
TH1D *black;
TLine *line1;
TLine *line2;
TMarker *mark;
TH2F *hscan;
Int_t pxsliceold;
Int_t pysliceold;
Int_t x1, x2;
Int_t y1, y2;
Bool_t kFix;
Int_t lastview;

//---------------------------------------------------------------------
//******  HSlice::HSlice(const TGWindow,UInt_t w,UInt_t h, TObjArray *p1,
//                       TArrayI *p2, TArray *p3, Int_t *p4)
//
// Constructor of the class HSlice.
//
//---------------------------------------------------------------------

HSlice::HSlice(const TGWindow *p, UInt_t w, UInt_t h, TObjArray *p1,
	       TArrayI *p2, TArrayI *p3, Int_t *p4) : TGMainFrame(p, w, h) {

//---------------------------------------------------------------
//********* frames
//---------------------------------------------------------------

fFrame = new TGCompositeFrame(this, 800, 700, kVerticalFrame);
AddFrame(fFrame, new TGLayoutHints(kLHintsNormal));

fF1 = new TGCompositeFrame(fFrame, 800, 100, kHorizontalFrame);
fFrame->AddFrame(fF1, new TGLayoutHints(kLHintsNormal,0,0,10,5));

fF2 = new TGCompositeFrame(fFrame, 800, 300, kHorizontalFrame);
fFrame->AddFrame(fF2, new TGLayoutHints(kLHintsNormal));

fF3 = new TGCompositeFrame(fFrame, 800, 300, kHorizontalFrame);
fFrame->AddFrame(fF3, new TGLayoutHints(kLHintsNormal));

fF21 = new TGCompositeFrame(fF2, 500, 300, kVerticalFrame);
fF2->AddFrame(fF21, new TGLayoutHints(kLHintsNormal));

fF22 = new TGGroupFrame(fF2, " ", kHorizontalFrame);
fF2->AddFrame(fF22, new TGLayoutHints(kLHintsNormal));

fF221 = new TGCompositeFrame(fF22, 100, 300, kVerticalFrame);
fF22->AddFrame(fF221, new TGLayoutHints(kLHintsNormal));

fF222 = new TGCompositeFrame(fF22, 100, 300, kVerticalFrame);
fF22->AddFrame(fF222, new TGLayoutHints(kLHintsNormal,30,0,0,0));

fF31 = new TGGroupFrame(fF3, " ", kVerticalFrame);
fF3->AddFrame(fF31, new TGLayoutHints(kLHintsNormal));

fF32 = new TGCompositeFrame(fF3, 800, 100, kVerticalFrame);
fF3->AddFrame(fF32, new TGLayoutHints(kLHintsNormal,0,0,5,0));

fF311 = new TGCompositeFrame(fF31, 800, 100, kHorizontalFrame);
fF31->AddFrame(fF311, new TGLayoutHints(kLHintsNormal));

fF312 = new TGCompositeFrame(fF31, 800, 100, kHorizontalFrame);
fF31->AddFrame(fF312, new TGLayoutHints(kLHintsNormal));

fF313 = new TGCompositeFrame(fF31, 800, 100, kHorizontalFrame);
fF31->AddFrame(fF313, new TGLayoutHints(kLHintsNormal));

fF314 = new TGCompositeFrame(fF31, 800, 100, kHorizontalFrame);
fF31->AddFrame(fF314, new TGLayoutHints(kLHintsNormal));

fF321 = new TGCompositeFrame(fF32, 300, 100, kHorizontalFrame);
fF32->AddFrame(fF321, new TGLayoutHints(kLHintsNormal,0,0,20,3));

fF322 = new TGCompositeFrame(fF32, 300, 100, kHorizontalFrame);
fF32->AddFrame(fF322, new TGLayoutHints(kLHintsNormal,0,0,20,0));

//---------------------------------------------------------------
//********* radio buttons
//---------------------------------------------------------------

fXSlice = new TGRadioButton(fF1, "X slice", M_XSLICE);
fXSlice->Associate(this);
fF1->AddFrame(fXSlice, new TGLayoutHints(kLHintsNormal,20,2,2,2));

fYSlice = new TGRadioButton(fF1, "Y slice", M_YSLICE);
fYSlice->Associate(this);
fF1->AddFrame(fYSlice, new TGLayoutHints(kLHintsNormal,10,2,2,2));

f2dHist = new TGRadioButton(fF1, "2-dim hist", M_2DHIST);
f2dHist->Associate(this);
f2dHist->SetState(kButtonDown);
fF1->AddFrame(f2dHist, new TGLayoutHints(kLHintsNormal,10,2,2,2));

//---------------------------------------------------------------
//********* canvas
//---------------------------------------------------------------

fCurPad = (TPad*) gPad;
fSliceCanv = new TRootEmbeddedCanvas("slicecanv", fF21, 250, 250);
fF21->AddFrame(fSliceCanv, new TGLayoutHints(kLHintsNormal, 2,2,2,2));
fSliceCanv->GetCanvas()->SetFillColor(10);
if(fCurPad) fCurPad->cd();

//---------------------------------------------------------------
//********* vertical slider
//---------------------------------------------------------------

fVSlider = new TGVSlider(fF221, 200, kSlider1 | kScaleBoth, M_VSLIDER);
fVSlider->Associate(this);
fVSlider->SetRange(1,100);
fVSlider->SetPosition(100);
fVMin = 1;
fVMax = 100;
fVPos = 100;
fF221->AddFrame(fVSlider, new TGLayoutHints(kLHintsNormal,5,5,5,5));

fF222->AddFrame(new TGLabel(fF222, new TGHotString("bin number")),
		new TGLayoutHints(kLHintsNormal,2,2,40,2));
fVLB1 = new TGListBox(fF222, -1);
fF222->AddFrame(fVLB1, new TGLayoutHints(kLHintsNormal,2,2,1,1));
fVLB1->Resize(100,20);

fF222->AddFrame(new TGLabel(fF222, new TGHotString("bin content")),
		new TGLayoutHints(kLHintsNormal,2,2,10,2));
fVLB3 = new TGListBox(fF222, -1);
fF222->AddFrame(fVLB3, new TGLayoutHints(kLHintsNormal,2,2,1,1));
fVLB3->Resize(100,20);

fF222->AddFrame(new TGLabel(fF222, new TGHotString("range")),
		new TGLayoutHints(kLHintsNormal,2,2,10,2));
fVLB2 = new TGListBox(fF222, -1);
fF222->AddFrame(fVLB2, new TGLayoutHints(kLHintsNormal,2,2,1,40));
fVLB2->Resize(100,20);


//---------------------------------------------------------------
//********* horizontal slider
//---------------------------------------------------------------

fHSlider = new TGHSlider(fF311, 200, kSlider1 | kScaleBoth, M_HSLIDER);
fHSlider->Associate(this);
fHSlider->SetRange(1,100);
fHSlider->SetPosition(1);
fHMin = 1;
fHMax = 100;
fHPos = 1;
fF311->AddFrame(fHSlider, new TGLayoutHints(kLHintsNormal,15,5,5,5));

fF312->AddFrame(new TGLabel(fF312, new TGHotString("bin number")),
		new TGLayoutHints(kLHintsNormal,25,2,2,2));
fHLB1 = new TGListBox(fF312, -1);
fF312->AddFrame(fHLB1, new TGLayoutHints(kLHintsNormal,2,2,1,1));
fHLB1->Resize(100,20);

fF313->AddFrame(new TGLabel(fF313, new TGHotString("bin content")),
		new TGLayoutHints(kLHintsNormal,25,2,2,2));
fHLB3 = new TGListBox(fF313, -1);
fF313->AddFrame(fHLB3, new TGLayoutHints(kLHintsNormal,2,2,1,1));
fHLB3->Resize(100,20);

fF314->AddFrame(new TGLabel(fF314, new TGHotString("range")),
		new TGLayoutHints(kLHintsNormal,25,2,2,2));
fHLB2 = new TGListBox(fF314, -1);
fF314->AddFrame(fHLB2, new TGLayoutHints(kLHintsNormal,32,2,1,1));
fHLB2->Resize(100,20);

//---------------------------------------------------------------
//********* max text field, rescale and fix
//---------------------------------------------------------------

fF321->AddFrame(new TGLabel(fF321, new TGHotString("max")),
		new TGLayoutHints(kLHintsNormal,2,2,2,2));

fMax = new TGTextEntry(fF321, fMaxBuf = new TGTextBuffer(100), -1);
fF321->AddFrame(fMax, new TGLayoutHints(kLHintsNormal,3,2,2,2));
fMax->Resize(60, fMax->GetDefaultHeight());

fRescale = new TGTextButton(fF321, "Rescale", M_RESCALE);
fRescale->Associate(this);
fRescale->SetToolTipText("Redraw the histogram with scaling factor");
fF321->AddFrame(fRescale, new TGLayoutHints(kLHintsNormal, 3,2,2,2));

fFix = new TGTextButton(fF321, "   Fix   ", M_FIX);
fFix->Associate(this);
fFix->SetToolTipText("Fix scaling factor");
fFix->AllowStayDown(kTRUE);
kFix = kFALSE; 
fF321->AddFrame(fFix, new TGLayoutHints(kLHintsNormal, 3,2,2,2));

//---------------------------------------------------------------
//********* option combobox and redraw
//---------------------------------------------------------------

fF322->AddFrame(new TGLabel(fF322, new TGHotString("option")),
		new TGLayoutHints(kLHintsNormal,2,2,2,2));

fOptionCB = new TGComboBox(fF322, 1000);
fOptionCB->Associate(this);
fF322->AddFrame(fOptionCB, new TGLayoutHints(kLHintsNormal, 3,2,2,2));
fOptionCB->Resize(70,20);
lastview = 1;

fRedraw = new TGTextButton(fF322, "Redraw", M_REDRAW);
fRedraw->Associate(this);
fRedraw->SetToolTipText("Redraw the histogram");
fF322->AddFrame(fRedraw, new TGLayoutHints(kLHintsNormal, 3,2,1,2));

//---------------------------------------------------------------
//********* end of definition
//---------------------------------------------------------------

hscan = 0;
slice = 0;
highlight = 0;
black = 0;
line1 = 0;
line2 = 0;
mark = new TMarker();
mark->SetMarkerStyle(3);
mark->SetMarkerSize(1.3);
mark->SetMarkerColor(2);

fArray = p1;
fXArray = p2; 
fYArray = p3;
fMode = p4;

countSlice++;

MapSubwindows();

SetWindowName("Slice 2d histogram");

Resize(GetDefaultSize());

MapWindow();

restoreStatus();

}


//--------------------------------------------------------------------
//******  HSlice::~HSlice()
//
// Destructor of the class HSlice.
//
//--------------------------------------------------------------------

HSlice::~HSlice() {

if(fCurPad && fCurPad->IsOnHeap() && hscan) {

  Float_t horx1 = hscan->GetXaxis()->
    GetBinLowEdge(hscan->GetXaxis()->GetFirst());
  Float_t horx2 = hscan->GetXaxis()->
    GetBinUpEdge(hscan->GetXaxis()->GetLast());
  Float_t very1 = hscan->GetYaxis()->
    GetBinLowEdge(hscan->GetYaxis()->GetFirst());
  Float_t very2 = hscan->GetYaxis()->
    GetBinUpEdge(hscan->GetYaxis()->GetLast());

  ((TCanvas*)fCurPad)->FeedbackMode(kTRUE);

  Int_t pxmin = fCurPad->XtoAbsPixel(horx1);
  Int_t pxmax = fCurPad->XtoAbsPixel(horx2);
  Int_t pymin = fCurPad->YtoAbsPixel(very1);
  Int_t pymax = fCurPad->YtoAbsPixel(very2);

  fCurPad->GetCanvas()->cd();
  if(pysliceold) gVirtualX->DrawLine(pxmin,pysliceold,pxmax,pysliceold);
  if(pxsliceold) gVirtualX->DrawLine(pxsliceold,pymin,pxsliceold,pymax);
  pxsliceold = 0;
  pysliceold = 0;
  hscan = 0;
}

delete fRedraw; delete fOptionCB;
delete fFix; delete fRescale; delete fMax;
delete fHLB2; delete fHLB3; delete fHLB1;
delete fHSlider;
delete fVLB2; delete fVLB3; delete fVLB1;
delete fVSlider;
delete fSliceCanv;
delete f2dHist; delete fYSlice; delete fXSlice;
delete fF322; delete fF321;
delete fF314; delete fF313; delete fF312; delete fF311;
delete fF32; delete fF31;
delete fF222; delete fF221;
delete fF22; delete fF21;
delete fF3; delete fF2; delete fF1;
delete fFrame;

countSlice--;

}


//---------------------------------------------------------------------
//******  HSlice::CloseWindow()
//
// This function closes the window.
//
//---------------------------------------------------------------------

void HSlice::CloseWindow() {

delete this;

}


//---------------------------------------------------------------------
//******  Bool_t HSlice::ProcessMessage(Long_t msg,Long_t parm1,Long_t parm2)
//
// This function processes all GUI items.
//
//---------------------------------------------------------------------

Bool_t HSlice::ProcessMessage(Long_t msg,Long_t parm1,Long_t parm2) {

char buf[10];
Int_t diff, step;
//TGTextLBEntry *pEntry;

switch(GET_MSG(msg)) {

 case kC_COMMAND:

  switch(GET_SUBMSG(msg)) {

  case kCM_COMBOBOX:
    if(parm1 == 1000) {
      if(f2dHist->GetState()) lastview = parm2;
    }
    break;

  case kCM_BUTTON:

    switch(parm1) {
      
    case M_FIX:
      if(!f2dHist->GetState()) {
	kFix = !kFix;
	if(kFix) {
	  fFix->AllowStayDown(kTRUE);
	  fFix->SetState(kButtonEngaged);
	}
	else {
	  fFix->AllowStayDown(kFALSE);
	  fFix->SetState(kButtonUp);
	}
      }
      break;

    case M_RESCALE:
      if(fXSlice->GetState()) makeXslice(kTRUE);
      if(fYSlice->GetState()) makeYslice(kTRUE);
      break;

    case M_REDRAW:
      drawHist();
      break;

    default:
      break;
    }
    break;

  case kCM_RADIOBUTTON:

    if(parm1 == M_XSLICE) {
      fXSlice->SetState(kButtonDown);
      fYSlice->SetState(kButtonUp);
      f2dHist->SetState(kButtonUp);
      if(fMode) *fMode = 1;
      setState();
      setCB();
      makeXslice();
    }
    if(parm1 == M_YSLICE) {
      fYSlice->SetState(kButtonDown);
      fXSlice->SetState(kButtonUp);
      f2dHist->SetState(kButtonUp);
      if(fMode) *fMode = 2;
      setState();
      setCB();
      makeYslice();
    }
    if(parm1 == M_2DHIST) {
      f2dHist->SetState(kButtonDown);
      fYSlice->SetState(kButtonUp);
      fXSlice->SetState(kButtonUp);
      if(fMode) *fMode = 3;
      setState(kFALSE);
      setCB(kFALSE);
      drawHist();
    }
    break;

  default:
    break;
  }
  break;

  case kC_VSLIDER:
  case kC_HSLIDER:
    switch (GET_SUBMSG(msg)) {

    case kSL_POS:

      sprintf(buf, "%ld", parm2);

      switch(parm1) {

      case M_VSLIDER:
	step = (fVMax - fVMin) / (fVSlider->GetHeight() - 16);
	diff = parm2 - fVPos;

	if(diff > 0) {
	  if(diff <= step) {
	    fVPos++;
	    fVSlider->SetPosition(fVPos);
	  }
	  else fVPos = parm2;
	}
	if(diff < 0) {
	  if((-diff) <= step) {
	    fVPos--;
	    fVSlider->SetPosition(fVPos);
	  }
	  else fVPos = parm2;
	}
	if(fXSlice->GetState()) makeXslice();
	if(fYSlice->GetState()) makeYslice();
	if(f2dHist->GetState()) drawHist();
	break;

      case M_HSLIDER:
	step = (fHMax - fHMin) / (fHSlider->GetHeight() - 16);
	diff = parm2 - fHPos;

	if(diff > 0) {
	  if(diff <= step) {
	    fHPos++;
	    fHSlider->SetPosition(fHPos);
	  }
	  else fHPos = parm2;
	}
	if(diff < 0) {
	  if((-diff) <= step) {
	    fHPos--;
	    fHSlider->SetPosition(fHPos);
	  }
	  else fHPos = parm2;
	}
	if(fXSlice->GetState()) makeXslice();
	if(fYSlice->GetState()) makeYslice();
	if(f2dHist->GetState()) drawHist();
	break;

      default:
	break;
      }
      break;

    default:
      break;
    }
    break;

 default:
   break;
}

return kTRUE;

}


//---------------------------------------------------------------------
//******  HSlice::makeXslice(Bool_t kRescale = kFALSE)
//
// This function makes X projection of a selected 2d histogram.
//
//---------------------------------------------------------------------

void HSlice::makeXslice(Bool_t kRescale) {

  TObject *select = 0;
  TIterator *it = 0;
  TList *list;
  Bool_t kFound = kFALSE;

  Int_t mode = gVirtualX->GetDrawMode();

  //   cout << "mode : " << mode  << endl;

  fCurPad = (TPad*) gPad;

  if(!fCurPad && !hscan) return;

  if(fCurPad) {

    list = fCurPad->GetListOfPrimitives();

    if(list) {
      it = list->MakeIterator();

      while((select = it->Next()) != NULL) {
	if(!strcmp(select->ClassName(),"TH2F") && 
	   (!strcmp(it->GetOption(),"") ||
	    !strcmp(it->GetOption(),"col") ||
	    !strcmp(it->GetOption(),"colz") ||
	    !strcmp(it->GetOption(),"cont1") ||
	    !strcmp(it->GetOption(),"cont2") ||
	    !strcmp(it->GetOption(),"cont3"))) {
	  kFound = kTRUE;
	  if(!hscan || (hscan != ((TH2F*) select))) 
	    hscan = (TH2F*) select;
	}
      }
    }
  }

  if(!hscan) {
    if(fCurPad) fCurPad->cd();
    return;
  }

  setSliderRange(hscan);

  if(slice) { delete slice; slice = 0; }
  if(highlight) { delete highlight; highlight = 0; }
  if(black) { delete black; black = 0; }

  slice = hscan->ProjectionX("pr",fVMax+fVMin-fVSlider->GetPosition(),
			     fVMax+fVMin-fVSlider->GetPosition());

  if(!slice) {
    fCurPad->cd();
    return;
  }

  slice->GetXaxis()->SetRange(fHMin,fHMax);

  highlight = new TH1D("high","high",slice->GetXaxis()->GetNbins(),
		       slice->GetXaxis()->GetXmin(),
		       slice->GetXaxis()->GetXmax());
  highlight->GetXaxis()->SetRange(fHMin,fHMax);
  highlight->SetLineColor(kRed);
  highlight->SetFillColor(kRed);
  highlight->SetBinContent(fHSlider->GetPosition(),
			   slice->GetBinContent(fHSlider->GetPosition()));

  black = new TH1D("black","black",slice->GetXaxis()->GetNbins(),
		   slice->GetXaxis()->GetXmin(),
		   slice->GetXaxis()->GetXmax());
  black->GetXaxis()->SetRange(fHMin,fHMax);

  if(kRescale || fFix->GetState()) {
    Float_t max = atof(fMax->GetText());
    if(max > 0) {
      slice->SetMaximum(max);
      highlight->SetMaximum(max);
      black->SetMaximum();
    }
  }

  fSliceCanv->GetCanvas()->cd();
  slice->Draw();
  highlight->Draw("SAME");
  black->Draw("SAME");
  mark->DrawMarker(highlight->GetBinCenter(fHSlider->GetPosition()),
		   highlight->GetBinContent(fHSlider->GetPosition()));
  fSliceCanv->GetCanvas()->Modified();
  fSliceCanv->GetCanvas()->Update();

  char buf[50];

  sprintf(buf,"%i",fVMax+fVMin-fVSlider->GetPosition());
  fVLB1->RemoveEntry(1);
  fVLB1->AddEntry(buf,1);
  fVLB1->MapSubwindows();
  fVLB1->Layout();

  fVLB2->RemoveEntry(1);
  fVLB2->MapSubwindows();
  fVLB2->Layout();

  fVLB3->RemoveEntry(1);
  fVLB3->MapSubwindows();
  fVLB3->Layout();


  Float_t from, to;
  from = hscan->GetXaxis()->GetBinLowEdge(fHSlider->GetPosition());
  to = from + hscan->GetXaxis()->GetBinWidth(fHSlider->GetPosition());

  //  cout << from << "  " << to << endl;

  sprintf(buf,"%i",fHSlider->GetPosition());
  fHLB1->RemoveEntry(1);
  fHLB1->AddEntry(buf,1);
  fHLB1->MapSubwindows();
  fHLB1->Layout();

  sprintf(buf,"%.1f%s%.1f",from,"...",to);
  fHLB2->RemoveEntry(1);
  fHLB2->AddEntry(buf,1);
  fHLB2->MapSubwindows();
  fHLB2->Layout();

  sprintf(buf,"%f",highlight->GetBinContent(fHSlider->GetPosition()));
  fHLB3->RemoveEntry(1);
  fHLB3->AddEntry(buf,1);
  fHLB3->MapSubwindows();
  fHLB3->Layout();

  if(kFound) {
    Float_t horx1 = hscan->GetXaxis()->
      GetBinLowEdge(hscan->GetXaxis()->GetFirst());
    Float_t horx2 = hscan->GetXaxis()->
      GetBinUpEdge(hscan->GetXaxis()->GetLast());
    Float_t hory = hscan->GetYaxis()->GetBinCenter(fVMax+fVMin-
						   fVSlider->GetPosition());
    Float_t very1 = hscan->GetYaxis()->
      GetBinLowEdge(hscan->GetYaxis()->GetFirst());
    Float_t very2 = hscan->GetYaxis()->
      GetBinUpEdge(hscan->GetYaxis()->GetLast());
    Float_t verx = hscan->GetXaxis()->GetBinCenter(fHSlider->GetPosition());
    
    ((TCanvas*)fCurPad)->FeedbackMode(kTRUE);

    Int_t py = fCurPad->YtoAbsPixel(hory);
    Int_t px = fCurPad->XtoAbsPixel(verx);
    Int_t pxmin = fCurPad->XtoAbsPixel(horx1);
    Int_t pxmax = fCurPad->XtoAbsPixel(horx2);
    Int_t pymin = fCurPad->YtoAbsPixel(very1);
    Int_t pymax = fCurPad->YtoAbsPixel(very2);
    
    fCurPad->GetCanvas()->cd();
    if(pysliceold && mode != 1) gVirtualX->DrawLine(pxmin,pysliceold,
						    pxmax,pysliceold);
    gVirtualX->DrawLine(pxmin,py,pxmax,py);
    if(pxsliceold && mode != 1) gVirtualX->DrawLine(pxsliceold,pymin,
						    pxsliceold,pymax);
    gVirtualX->DrawLine(px,pymin,px,pymax);
    pxsliceold = px;
    pysliceold = py;
  }
  if(fCurPad && (fCurPad != fSliceCanv->GetCanvas())) fCurPad->cd();

}


//---------------------------------------------------------------------
//******  HSlice::makeYslice(Bool_t kRescale = kFALSE)
//
// This function makes Y projection of a selected 2d histogram.
//
//---------------------------------------------------------------------

void HSlice::makeYslice(Bool_t kRescale) {

  TObject *select = 0;
  TIterator *it = 0;
  TList *list;
  Bool_t kFound = kFALSE;

  Int_t mode = gVirtualX->GetDrawMode();

  fCurPad = (TPad*) gPad;
  if(!fCurPad && !hscan) return;

  if(fCurPad) {

    list = fCurPad->GetListOfPrimitives();

    if(list) {
      it = list->MakeIterator();

      while((select = it->Next()) != NULL) {
	if(!strcmp(select->ClassName(),"TH2F") && 
	   (!strcmp(it->GetOption(),"") ||
	    !strcmp(it->GetOption(),"col") ||
	    !strcmp(it->GetOption(),"colz") ||
	    !strcmp(it->GetOption(),"cont1") ||
	    !strcmp(it->GetOption(),"cont2") ||
	    !strcmp(it->GetOption(),"cont3"))) {
	  kFound = kTRUE;
	  if(!hscan || (hscan != ((TH2F*) select))) 
	    hscan = (TH2F*) select;
	}
      }
    }
  }

  if(!hscan) {
    if(fCurPad) fCurPad->cd();
    return;
  }

  setSliderRange(hscan);

  if(slice) { delete slice; slice = 0; }
  if(highlight) { delete highlight; highlight = 0; }
  if(black) { delete black; black = 0; }

  slice = hscan->ProjectionY("pr",fHSlider->GetPosition(),
			     fHSlider->GetPosition());
  if(!slice) {
    fCurPad->cd();
    return;
  }

  slice->GetXaxis()->SetRange(fVMin,fVMax);

  highlight = new TH1D("high","high",slice->GetXaxis()->GetNbins(),
		       slice->GetXaxis()->GetXmin(),
		       slice->GetXaxis()->GetXmax());
  highlight->GetXaxis()->SetRange(fVMin,fVMax);
  highlight->SetLineColor(kRed);
  highlight->SetFillColor(kRed);
  highlight->SetBinContent(fVMax+fVMin-fVSlider->GetPosition(),
			   slice->GetBinContent(fVMax+fVMin-
						fVSlider->GetPosition()));
  
  black = new TH1D("black","black",slice->GetXaxis()->GetNbins(),
		   slice->GetXaxis()->GetXmin(),
		   slice->GetXaxis()->GetXmax());

  black->GetXaxis()->SetRange(fVMin,fVMax);

  if(kRescale || fFix->GetState()) {
    Float_t max = atof(fMax->GetText());
    if(max > 0) {
      slice->SetMaximum(max);
      highlight->SetMaximum(max);
      black->SetMaximum(max);
    }
  }
  fSliceCanv->GetCanvas()->cd();
  slice->Draw();
  highlight->Draw("SAME");
  black->Draw("SAME");
  mark->DrawMarker(highlight->GetBinCenter(fVMax+fVMin
					   -fVSlider->GetPosition()),
		   highlight->GetBinContent(fVMax+fVMin
					    -fVSlider->GetPosition()));
  fSliceCanv->GetCanvas()->Modified();
  fSliceCanv->GetCanvas()->Update();

  char buf[50];

  sprintf(buf,"%i",fHSlider->GetPosition());
  fHLB1->RemoveEntry(1);
  fHLB1->AddEntry(buf,1);
  fHLB1->MapSubwindows();
  fHLB1->Layout();

  fHLB2->RemoveEntry(1);
  fHLB2->MapSubwindows();
  fHLB2->Layout();

  fHLB3->RemoveEntry(1);
  fHLB3->MapSubwindows();
  fHLB3->Layout();

  Float_t from, to;
  from = hscan->GetYaxis()->GetBinLowEdge(fVMax+fVMin-fVSlider->GetPosition());
  to = from + hscan->GetYaxis()->GetBinWidth(fVMax+fVMin
					     -fVSlider->GetPosition());

  //  cout << from << "  " << to << endl;

  sprintf(buf,"%i",fVMax+fVMin-fVSlider->GetPosition());
  fVLB1->RemoveEntry(1);
  fVLB1->AddEntry(buf,1);
  fVLB1->MapSubwindows();
  fVLB1->Layout();

  sprintf(buf,"%.1f%s%.1f",from,"...",to);
  fVLB2->RemoveEntry(1);
  fVLB2->AddEntry(buf,1);
  fVLB2->MapSubwindows();
  fVLB2->Layout();

  sprintf(buf,"%f",highlight->GetBinContent(fVMax+fVMin
					    -fVSlider->GetPosition()));
  fVLB3->RemoveEntry(1);
  fVLB3->AddEntry(buf,1);
  fVLB3->MapSubwindows();
  fVLB3->Layout();

  if(kFound) {
    Float_t horx1 = hscan->GetXaxis()->
      GetBinLowEdge(hscan->GetXaxis()->GetFirst());
    Float_t horx2 = hscan->GetXaxis()->
      GetBinUpEdge(hscan->GetXaxis()->GetLast());
    Float_t hory = hscan->GetYaxis()->GetBinCenter(fVMax+fVMin-
						   fVSlider->GetPosition());
    Float_t very1 = hscan->GetYaxis()->
      GetBinLowEdge(hscan->GetYaxis()->GetFirst());
    Float_t very2 = hscan->GetYaxis()->
      GetBinUpEdge(hscan->GetYaxis()->GetLast());
    Float_t verx = hscan->GetXaxis()->GetBinCenter(fHSlider->GetPosition());
    
    ((TCanvas*)fCurPad)->FeedbackMode(kTRUE);
    
    Int_t py = fCurPad->YtoAbsPixel(hory);
    Int_t px = fCurPad->XtoAbsPixel(verx);
    Int_t pxmin = fCurPad->XtoAbsPixel(horx1);
    Int_t pxmax = fCurPad->XtoAbsPixel(horx2);
    Int_t pymin = fCurPad->YtoAbsPixel(very1);
    Int_t pymax = fCurPad->YtoAbsPixel(very2);
    
    fCurPad->GetCanvas()->cd();
    if(pysliceold && mode != 1) gVirtualX->DrawLine(pxmin,pysliceold,
						    pxmax,pysliceold);
    gVirtualX->DrawLine(pxmin,py,pxmax,py);
    if(pxsliceold && mode != 1) gVirtualX->DrawLine(pxsliceold,pymin,
						    pxsliceold,pymax);
    gVirtualX->DrawLine(px,pymin,px,pymax);
    pxsliceold = px;
    pysliceold = py;
  }
  if(fCurPad && (fCurPad != fSliceCanv->GetCanvas())) fCurPad->cd();

}


//---------------------------------------------------------------------
//******  HSlice::drawHist()
//
// This function draws selected 2d histogram.
//
//---------------------------------------------------------------------

void HSlice::drawHist() {

  TObject *select = 0;
  TIterator *it = 0;
  TList *list;
  Bool_t kDrawn = kFALSE;

  TGTextLBEntry *pEntry = (TGTextLBEntry*) fOptionCB->GetSelectedEntry();
  if(!pEntry) return;

  list = fSliceCanv->GetCanvas()->GetListOfPrimitives();

  if(list) {
    it = list->MakeIterator();
    while((select = it->Next()) != NULL) {
      if(!strcmp(select->ClassName(),"TH2F")) kDrawn = kTRUE;
      if(pEntry && kDrawn) {
	if(!strcmp(it->GetOption(),pEntry->GetText()->GetString()) ||
	   (!strcmp(it->GetOption(),"") && 
	    !strcmp(pEntry->GetText()->GetString(),"no"))) {
	  kDrawn = kTRUE;
	}
	else kDrawn = kFALSE;
	break;
      }
    }
  }

  fCurPad = (TPad*) gPad;
  if(!fCurPad && !hscan) return;

  if(fCurPad) {

    if(it) { delete it; it = 0; }
    list = fCurPad->GetListOfPrimitives();

    if(list) {
      it = list->MakeIterator();

      while((select = it->Next()) != NULL) {
	if(!strcmp(select->ClassName(),"TH2F") && 
	   (!strcmp(it->GetOption(),"") ||
	    !strcmp(it->GetOption(),"col") ||
	    !strcmp(it->GetOption(),"colz") ||
	    !strcmp(it->GetOption(),"cont1") ||
	    !strcmp(it->GetOption(),"cont2") ||
	    !strcmp(it->GetOption(),"cont3"))) {
	  if(!hscan || (hscan != ((TH2F*) select))) { 
	    hscan = (TH2F*) select;
	    kDrawn = kFALSE;
	  }
	}
      }
    }
  }

  if(!hscan) {
    if(fCurPad) fCurPad->cd();
    return;
  }

  if(x1 != hscan->GetXaxis()->GetFirst() || x2 != hscan->GetXaxis()->GetLast()
     || y1 != hscan->GetYaxis()->GetFirst() || 
     y2 != hscan->GetYaxis()->GetLast()) kDrawn = kFALSE;

  x1 = hscan->GetXaxis()->GetFirst();
  x2 = hscan->GetXaxis()->GetLast();
  y1 = hscan->GetYaxis()->GetFirst();
  y2 = hscan->GetYaxis()->GetLast();


  if(!kDrawn || gVirtualX->GetDrawMode() == 1) {
    fSliceCanv->GetCanvas()->cd();
    if(!strcmp(pEntry->GetText()->GetString(),"no")) hscan->Draw();
    else hscan->Draw(pEntry->GetText()->GetString());
    fSliceCanv->GetCanvas()->Modified();
    fSliceCanv->GetCanvas()->Update();
  }

  char buf[50];
  Float_t from, to;

  setSliderRange(hscan);

  sprintf(buf,"%i",fHSlider->GetPosition());
  fHLB1->RemoveEntry(1);
  fHLB1->AddEntry(buf,1);
  fHLB1->MapSubwindows();
  fHLB1->Layout();

  from = hscan->GetXaxis()->GetBinLowEdge(fHSlider->GetPosition());
  to = from + hscan->GetXaxis()->GetBinWidth(fHSlider->GetPosition());
  sprintf(buf,"%.1f%s%.1f",from,"...",to);
  fHLB2->RemoveEntry(1);
  fHLB2->AddEntry(buf,1);
  fHLB2->MapSubwindows();
  fHLB2->Layout();

  sprintf(buf,"%f",hscan->GetCellContent(fHSlider->GetPosition(),
					 fVMax+fVMin-fVSlider->GetPosition()));
  fHLB3->RemoveEntry(1);
  fHLB3->AddEntry(buf,1);
  fHLB3->MapSubwindows();
  fHLB3->Layout();

  sprintf(buf,"%i",fVMax+fVMin-fVSlider->GetPosition());
  fVLB1->RemoveEntry(1);
  fVLB1->AddEntry(buf,1);
  fVLB1->MapSubwindows();
  fVLB1->Layout();

  from = hscan->GetYaxis()->GetBinLowEdge(fVMax+fVMin-fVSlider->GetPosition());
  to = from + hscan->GetYaxis()->GetBinWidth(fVMax+fVMin
					     -fVSlider->GetPosition());
  sprintf(buf,"%.1f%s%.1f",from,"...",to);
  fVLB2->RemoveEntry(1);
  fVLB2->AddEntry(buf,1);
  fVLB2->MapSubwindows();
  fVLB2->Layout();

  sprintf(buf,"%f",hscan->GetCellContent(fHSlider->GetPosition(),
					 fVMax+fVMin-fVSlider->GetPosition()));
  fVLB3->RemoveEntry(1);
  fVLB3->AddEntry(buf,1);
  fVLB3->MapSubwindows();
  fVLB3->Layout();

  // line drawing

  Float_t horx1 = hscan->GetXaxis()->
    GetBinLowEdge(hscan->GetXaxis()->GetFirst());
  Float_t horx2 = hscan->GetXaxis()->
    GetBinUpEdge(hscan->GetXaxis()->GetLast());
  Float_t hory = hscan->GetYaxis()->GetBinCenter(fVMax+fVMin-
						 fVSlider->GetPosition());
  Float_t very1 = hscan->GetYaxis()->
    GetBinLowEdge(hscan->GetYaxis()->GetFirst());
  Float_t very2 = hscan->GetYaxis()->
    GetBinUpEdge(hscan->GetYaxis()->GetLast());
  Float_t verx = hscan->GetXaxis()->GetBinCenter(fHSlider->GetPosition());

  Int_t mode = gVirtualX->GetDrawMode();

  fSliceCanv->GetCanvas()->FeedbackMode(kTRUE);

  Int_t pxold = fSliceCanv->GetCanvas()->GetUniqueID()/10000;
  Int_t pyold = fSliceCanv->GetCanvas()->GetUniqueID()%10000;
  Int_t py = fSliceCanv->GetCanvas()->YtoAbsPixel(hory);
  Int_t px = fSliceCanv->GetCanvas()->XtoAbsPixel(verx);
  Int_t pxmin = fSliceCanv->GetCanvas()->XtoAbsPixel(horx1);
  Int_t pxmax = fSliceCanv->GetCanvas()->XtoAbsPixel(horx2);
  Int_t pymin = fSliceCanv->GetCanvas()->YtoAbsPixel(very1);
  Int_t pymax = fSliceCanv->GetCanvas()->YtoAbsPixel(very2);

  fSliceCanv->GetCanvas()->cd();
  if(pyold && mode != 1) gVirtualX->DrawLine(pxmin,pyold,pxmax,pyold);
  gVirtualX->DrawLine(pxmin,py,pxmax,py);
  if(pxold && mode != 1) gVirtualX->DrawLine(pxold,pymin,pxold,pymax);
  gVirtualX->DrawLine(px,pymin,px,pymax);
  fSliceCanv->GetCanvas()->SetUniqueID(10000*px+py);

  if(fCurPad && (fCurPad != fSliceCanv->GetCanvas())) fCurPad->cd();

}


//---------------------------------------------------------------------
//******  HSlice::setSliderRange(TH2F *hist)
//
// This function sets ranges of sliders.
//
//---------------------------------------------------------------------

void HSlice::setSliderRange(TH2F *hist) {

  if(!hist) return;
  Int_t xmin = hist->GetXaxis()->GetFirst();
  Int_t xmax = hist->GetXaxis()->GetLast();
  Int_t ymin = hist->GetYaxis()->GetFirst();
  Int_t ymax = hist->GetYaxis()->GetLast();

  //  cout << "xmin: "<<xmin<< "  xmax: "<<xmax << endl;
  //  cout << "fHMin: "<<fHMin<< "  fHMax: "<<fHMax << endl;
  //  cout << "ymin: "<<ymin<< "  ymax: "<<ymax << endl;
  //  cout << "fVMin: "<<fVMin<< "  fVMax: "<<fVMax << endl;

  if(xmin != fHMin || xmax != fHMax) {
    fHSlider->SetRange(xmin,xmax);
    fHMin = xmin;
    fHMax = xmax;
    fHSlider->SetPosition(fHMin);
    fHPos = fHMin;
  }

  if(ymin != fVMin || ymax != fVMax) {
    fVSlider->SetRange(ymin,ymax);
    fVMin = ymin;
    fVMax = ymax;
    fVSlider->SetPosition(fVMax);
    fVPos = fVMax;
  }

}


//---------------------------------------------------------------------
//******  HSlice::setCB(Bool_t kClear = kTRUE)
//
// This function sets entries of fOptionCB. If kClear simply clears it.
//
//---------------------------------------------------------------------

void HSlice::setCB(Bool_t kClear) {

  if(!fOptionCB) return;
  TGTextLBEntry *pEntry;
  TGListBox *pListBox;
  Int_t i = 1;

  pListBox = const_cast <TGListBox*> (fOptionCB->GetListBox());
  if(!pListBox) return;

  while((pEntry = (TGTextLBEntry*) pListBox->Select(i,kFALSE))) {
    if(i==1) {
      pEntry->SetText(new TGString(""));
      fOptionCB->Select(i);
    }
    fOptionCB->RemoveEntry(i++);
  }
  pListBox = 0;
  
  if(kClear) {
    fRedraw->AllowStayDown(kTRUE);
    fRedraw->SetState(kButtonEngaged);
    return;
  }
  fOptionCB->AddEntry("no",1);
  fOptionCB->AddEntry("col",2);
  fOptionCB->AddEntry("colz",3);
  fOptionCB->AddEntry("cont1",4);
  fOptionCB->AddEntry("cont2",5);
  fOptionCB->AddEntry("cont3",6);
  fOptionCB->Select(lastview);
  fRedraw->AllowStayDown(kFALSE);
  fRedraw->SetState(kButtonUp);

}


//---------------------------------------------------------------------
//******  HSlice::setState(Bool_t kEnable = kTRUE)
//
// This function sets status of buttons Rescale and Fix.
//
//---------------------------------------------------------------------

void HSlice::setState(Bool_t kEnable) {

  if(!kEnable) {
    fMax->SetState(kFALSE);
    fRescale->AllowStayDown(kTRUE);
    fRescale->SetState(kButtonEngaged);
    fFix->AllowStayDown(kTRUE);
    fFix->SetState(kButtonEngaged);
    kFix = kTRUE;
    return;
  }

  fMax->SetState(kTRUE);
  fRescale->AllowStayDown(kFALSE);
  fRescale->SetState(kButtonUp);
  fFix->SetState(kButtonUp);
  kFix = kFALSE;
}


//---------------------------------------------------------------------
//******  HSlice::restoreStatus()
//
// This function restores status of the panel when it was closed last time.
//
//---------------------------------------------------------------------

void HSlice::restoreStatus() {

  if(!fMode) return;
  fXSlice->SetState(kButtonUp);
  fYSlice->SetState(kButtonUp);
  f2dHist->SetState(kButtonUp);
  if(*fMode == 1) {
    fXSlice->SetState(kButtonDown);
    SendMessage(this,MK_MSG(kC_COMMAND,kCM_RADIOBUTTON),M_XSLICE,0);
  }
  if(*fMode == 2) {
    fYSlice->SetState(kButtonDown);
    SendMessage(this,MK_MSG(kC_COMMAND,kCM_RADIOBUTTON),M_YSLICE,0);
    makeYslice();
  }
  if(*fMode == 3) {
    f2dHist->SetState(kButtonDown);
    SendMessage(this,MK_MSG(kC_COMMAND,kCM_RADIOBUTTON),M_2DHIST,0);
    drawHist();
  }

}


//---------------------------------------------------------------------
//******  HSlice::setCursors(const char* name)
//
// This function saves histogram cursors according to the sliders positions.
//
//---------------------------------------------------------------------

void HSlice::setCursors(const char* name) {

  if(!name) return;
  if(!fArray || !fXArray || !fYArray || !fMode) return;

  Int_t i;
  TH2F *hist;

  for(i=0; i<(fArray->GetLast()+1); i++) {

    if(fArray->At(i)) {
      hist = (TH2F*) fArray->At(i);
      if(hist) {

	if(!strcmp(hist->GetName(),name)) {
	  if(fVPos != 0 && fHPos != 0) {
	    fXArray[i] = fHPos;
	    fYArray[i] = fVMax+fVMin - fVPos;
	    break;
	  }
	}
      }
    }
  }

}



