//*-- Author : Dmitry Vasiliev (vassili@lns.infn.it)
/* 
 * Copyright P.Finocchiaro and D.Vasiliev (INFN-LNS, Catania) HADES collaboration
*/
using namespace std;
#include "hhistovisual.h"
#include <TGLabel.h>
#include "honlinecontrol.h"
#include "hhistocont.h"
#include "hhistogram.h"
#include "honlmainwindow.h"
#include "hslice.h"
#include <stdlib.h>
#include <TList.h>
#include <TGFileDialog.h>
#include <TFile.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TROOT.h>
#include "herror.h"
#include "hdisplaydb.h"

#include <iostream> 
#include <iomanip>

//-------------------------------------------------------------------------
//
// File name:       hhistovisual.cc
// Subject:         GUI panel for histogram display.
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            April 2000
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the source code for the member functions of 
// the class HHistoVisual.
// This is a GUI class for histogram display.
//
//-------------------------------------------------------------------------

//_HADES_CLASS_DESCRIPTION 
//////////////////////////////////////////////////////////////////////
// This is a GUI class for histogram display.
//////////////////////////////////////////////////////////////////////
ClassImp(HHistoVisual)

Float_t theta = 30.;
Float_t phi = 30.;
Bool_t kFix1;
Bool_t kFix2;
Bool_t kFix3;

//---------------------------------------------------------------------
//******  HHistoVisual::HHistoVisual(const TGWindow,UInt_t w,UInt_t h,
//                                   TGMainFrame *p2)
//
// Constructor of the class HHistoVisual.
//
//---------------------------------------------------------------------

HHistoVisual::HHistoVisual(const TGWindow *p, UInt_t w, 
			   UInt_t h, TGMainFrame *p2) : TGMainFrame(p, w, h) {
//
// Constructor of the class HHistoVisual.
//

fParent = p2;

//---------------------------------------------------------------
//********* frames
//---------------------------------------------------------------

fFrame = new TGCompositeFrame(this, 800, 700, kVerticalFrame);
AddFrame(fFrame, new TGLayoutHints(kLHintsNormal));

fMenuFile = new TGPopupMenu(fClient->GetRoot());
fMenuFile->AddEntry("Read histogrammes", M__HIV_READHIST);
fMenuFile->AddEntry("Save histogrammes", M__HIV_SAVEHIST);
fMenuFile->AddSeparator();
fMenuFile->AddEntry("Close", M__HIV_CLOSE);

fMenuPicture = new TGPopupMenu(fClient->GetRoot());
fMenuPicture->AddEntry("Edit picture", M__HIV_EDITPIC);
fMenuPicture->AddEntry("Save picture",M__HIV_SAVEPIC);

fMenuFile->Associate(this);
fMenuPicture->Associate(this);

fMenuBar = new TGMenuBar(fFrame,1,1,kHorizontalFrame);
fMenuBar->AddPopup("File",fMenuFile,new TGLayoutHints(kLHintsNormal,2,2,2,2));
fMenuBar->AddPopup("Picture",fMenuPicture, 
		   new TGLayoutHints(kLHintsNormal,2,2,2,2));
fFrame->AddFrame(fMenuBar, new TGLayoutHints(kLHintsNormal));

fF2 = new TGCompositeFrame(fFrame, 800, 400, kHorizontalFrame);
fFrame->AddFrame(fF2, new TGLayoutHints(kLHintsNormal));

fF3 = new TGGroupFrame(fFrame, "Drawing", kHorizontalFrame);
fFrame->AddFrame(fF3, new TGLayoutHints(kLHintsNormal));

fF21 = new TGGroupFrame(fF2, "List of histogrammes", kVerticalFrame);
fF2->AddFrame(fF21, new TGLayoutHints(kLHintsNormal));

fF22 = new TGCompositeFrame(fF2, 500, 400, kVerticalFrame);
fF2->AddFrame(fF22, new TGLayoutHints(kLHintsNormal,40,0,0,0));

fF221 = new TGCompositeFrame(fF22, 500, 100, kHorizontalFrame);
fF22->AddFrame(fF221, new TGLayoutHints(kLHintsNormal,0,0,6,0));

fF222 = new TGCompositeFrame(fF22, 500, 100, kVerticalFrame);
fF22->AddFrame(fF222, new TGLayoutHints(kLHintsNormal,0,0,2,0));

fF223 = new TGGroupFrame(fF22, "Display layout", kVerticalFrame);
fF22->AddFrame(fF223, new TGLayoutHints(kLHintsNormal,2,0,6,2));
fF223->SetLayoutManager(new TGMatrixLayout(fF223,0,4,2));

fF31 = new TGCompositeFrame(fF3, 500, 300, kVerticalFrame);
fF3->AddFrame(fF31, new TGLayoutHints(kLHintsNormal));

fF32 = new TGGroupFrame(fF3, "Control buttons", kVerticalFrame);
fF3->AddFrame(fF32, new TGLayoutHints(kLHintsNormal,3,0,0,0));

fF311 = new TGGroupFrame(fF31, "axis ranges", kHorizontalFrame);
fF31->AddFrame(fF311, new TGLayoutHints(kLHintsNormal));

fF3111 = new TGCompositeFrame(fF311, 500, 100, kVerticalFrame);
fF311->AddFrame(fF3111, new TGLayoutHints(kLHintsNormal));

fF3112 = new TGCompositeFrame(fF311, 500, 100, kHorizontalFrame);
fF311->AddFrame(fF3112, new TGLayoutHints(kLHintsNormal));

fF31111 = new TGCompositeFrame(fF3111, 500, 100, kHorizontalFrame);
fF3111->AddFrame(fF31111, new TGLayoutHints(kLHintsNormal));

fF31112 = new TGCompositeFrame(fF3111, 500, 100, kHorizontalFrame);
fF3111->AddFrame(fF31112, new TGLayoutHints(kLHintsNormal));

fF312 = new TGGroupFrame(fF31, "scaling", kHorizontalFrame);
fF31->AddFrame(fF312, new TGLayoutHints(kLHintsNormal));

fF313 = new TGGroupFrame(fF31, "option", kHorizontalFrame);
fF31->AddFrame(fF313, new TGLayoutHints(kLHintsNormal));

fF314 = new TGGroupFrame(fF31, "settings", kHorizontalFrame);
fF31->AddFrame(fF314, new TGLayoutHints(kLHintsNormal));

//---------------------------------------------------------------
//********* list box of histogrammes
//---------------------------------------------------------------

fHistoLB = new TGListBox(fF21, M__HIV_HISTOLIST);
fHistoLB->Associate(this);
if(gOnline) {
  Int_t j=1;
  for(Int_t i=0;i<gOnline->getHisto()->getEntries();i++) {

    if(gOnline->getHisto()->at(i)) 
      fHistoLB->AddEntry(gOnline->getHisto()->at(i)->GetName(),j++);
  }
}
fF21->AddFrame(fHistoLB, new TGLayoutHints(kLHintsNormal));
fHistoLB->Resize(160,150);

fUpdateB = new TGTextButton(fF21, "  Update  ", M__HIV_UPDATE);
fUpdateB->Associate(this);
fUpdateB->SetToolTipText("Update content of the list");
fF21->AddFrame(fUpdateB, new TGLayoutHints(kLHintsNormal,50,2,7,2));

//---------------------------------------------------------------
//********* zero, zero all, mult selection, select all, layout buttons
//---------------------------------------------------------------

fResetB = new TGTextButton(fF221, "   Zero  ", M__HIV_RESET);
fResetB->Associate(this);
fResetB->SetToolTipText("Clear content of highlighted histogrammes");
fF221->AddFrame(fResetB, new TGLayoutHints(kLHintsNormal,6,2,2,2));

fResetAllB = new TGTextButton(fF221, "Zero All", M__HIV_RESETALL);
fResetAllB->Associate(this);
fResetAllB->SetToolTipText("Clear content of all histogrammes");
fF221->AddFrame(fResetAllB, new TGLayoutHints(kLHintsNormal,2,2,2,2));

fMultipleCB = new TGCheckButton(fF222, "Multiple selection", 
				M__HIV_MULTISELECT);
fMultipleCB->Associate(this);
fMultipleCB->SetState(kButtonUp);
fF222->AddFrame(fMultipleCB, new TGLayoutHints(kLHintsNormal,5,0,8,5));

fSelAllB = new TGTextButton(fF222, "Select All", M__HIV_SELECTALL);
fSelAllB->Associate(this);
fSelAllB->SetToolTipText("Select all items in the list of histogrammes");
fF222->AddFrame(fSelAllB, new TGLayoutHints(kLHintsNormal,20,5,3,3));

for(Int_t i=0; i<16; i++) {
  fLayoutB[i] = new TGTextButton(fF223,"     ",500+i);
  fLayoutB[i]->Associate(this);
  fF223->AddFrame(fLayoutB[i]);
  histIdx[i] = -1;
  kColor[i] = kFALSE;
  count = 0;
  indi[0] = indi[1] = indj[0] = indj[1] = -1;
}

//---------------------------------------------------------------
//********* x start and x stop
//---------------------------------------------------------------

fF31111->AddFrame(new TGLabel(fF31111, new TGHotString("X start")),
		  new TGLayoutHints(kLHintsNormal,0,0,1,1));
fXStart = new TGTextEntry(fF31111, fXStartBuf = new TGTextBuffer(100), -1);
fF31111->AddFrame(fXStart, new TGLayoutHints(kLHintsNormal,2,0,0,0));
fXStart->Resize(60, fXStart->GetDefaultHeight());

fF31111->AddFrame(new TGLabel(fF31111, new TGHotString("X end")),
		  new TGLayoutHints(kLHintsNormal,2,0,1,1));
fXStop = new TGTextEntry(fF31111, fXStopBuf = new TGTextBuffer(100), -1);
fF31111->AddFrame(fXStop, new TGLayoutHints(kLHintsNormal,2,0,0,0));
fXStop->Resize(60, fXStop->GetDefaultHeight());

//---------------------------------------------------------------
//********* y start and y stop
//---------------------------------------------------------------

fF31112->AddFrame(new TGLabel(fF31112, new TGHotString("Y start")),
		  new TGLayoutHints(kLHintsNormal,0,0,1,1));
fYStart = new TGTextEntry(fF31112, fYStartBuf = new TGTextBuffer(100), -1);
fF31112->AddFrame(fYStart, new TGLayoutHints(kLHintsNormal,2,0,0,0));
fYStart->Resize(60, fYStart->GetDefaultHeight());

fF31112->AddFrame(new TGLabel(fF31112, new TGHotString("Y end")),
		  new TGLayoutHints(kLHintsNormal,2,0,1,1));
fYStop = new TGTextEntry(fF31112, fYStopBuf = new TGTextBuffer(100), -1);
fF31112->AddFrame(fYStop, new TGLayoutHints(kLHintsNormal,2,0,0,0));
fYStop->Resize(60, fYStop->GetDefaultHeight());

//---------------------------------------------------------------

fFix1B = new TGTextButton(fF3112, "  Fix  ", M__HIV_FIX1);
fFix1B->Associate(this);
fFix1B->SetToolTipText("Fix drawing option");
fFix1B->AllowStayDown(kTRUE);
kFix1 = kFALSE;
fF3112->AddFrame(fFix1B, new TGLayoutHints(kLHintsNormal,4,0,10,0));

//---------------------------------------------------------------
//********* scale
//---------------------------------------------------------------

fF312->AddFrame(new TGLabel(fF312, new TGHotString("max")),
		new TGLayoutHints(kLHintsNormal,10,0,1,1));
fScale = new TGTextEntry(fF312, fScaleBuf = new TGTextBuffer(100), -1);
fF312->AddFrame(fScale, new TGLayoutHints(kLHintsNormal,8,0,0,0));
fScale->Resize(100, fScale->GetDefaultHeight());

fFix2B = new TGTextButton(fF312, "  Fix  ", M__HIV_FIX2);
fFix2B->Associate(this);
fFix2B->SetToolTipText("Fix scaling factor");
fFix2B->AllowStayDown(kTRUE);
kFix2 = kFALSE;
fF312->AddFrame(fFix2B, new TGLayoutHints(kLHintsNormal,60,0,0,0));

//---------------------------------------------------------------
//********* option
//---------------------------------------------------------------

fF313->AddFrame(new TGLabel(fF313, new TGHotString("option")),
		new TGLayoutHints(kLHintsNormal,2,0,1,1));
fOptionCB = new TGComboBox(fF313, -1);
fOptionCB->Associate(this);
fOptionCB->AddEntry("no",1);
fOptionCB->AddEntry("lego",2);
fOptionCB->AddEntry("lego1",3);
fOptionCB->AddEntry("lego2",4);
fOptionCB->AddEntry("surf",5);
fOptionCB->AddEntry("surf1",6);
fOptionCB->AddEntry("surf2",7);
fOptionCB->AddEntry("surf3",8);
fOptionCB->AddEntry("surf4",9);
fOptionCB->AddEntry("col",10);
fOptionCB->AddEntry("colz",11);
fOptionCB->AddEntry("cont0",12);
fOptionCB->AddEntry("cont1",13);
fOptionCB->AddEntry("cont2",14);
fOptionCB->AddEntry("cont3",15);
fF313->AddFrame(fOptionCB, new TGLayoutHints(kLHintsNormal,5,0,0,0));
fOptionCB->Resize(100, 20);
fOptionCB->Select(1);

fFix3B = new TGTextButton(fF313, "  Fix  ", M__HIV_FIX3);
fFix3B->Associate(this);
fFix3B->SetToolTipText("Fix drawing option");
fFix3B->AllowStayDown(kTRUE);
kFix3 = kFALSE;
fF313->AddFrame(fFix3B, new TGLayoutHints(kLHintsNormal,60,0,0,0));

//---------------------------------------------------------------
//********* settings
//---------------------------------------------------------------

fSysDefaultB = new TGTextButton(fF314, "Sys Default", M__HIV_SYSDEFAULT);
fSysDefaultB->Associate(this);
fSysDefaultB->SetToolTipText("Show system default drawing options");
fF314->AddFrame(fSysDefaultB, new TGLayoutHints(kLHintsNormal,12,1,2,3));

fHisDefaultB = new TGTextButton(fF314, "Hist Default", M__HIV_HISDEFAULT);
fHisDefaultB->Associate(this);
fHisDefaultB->SetToolTipText("Show histogram default drawing options");
fF314->AddFrame(fHisDefaultB, new TGLayoutHints(kLHintsNormal,1,1,2,3));

fSaveB = new TGTextButton(fF314, "    Save    ", M__HIV_SAVEOPT);
fSaveB->Associate(this);
fSaveB->SetToolTipText("Save drawing options for the highlighted histogram");
fF314->AddFrame(fSaveB, new TGLayoutHints(kLHintsNormal,1,12,2,3));

//---------------------------------------------------------------
//********* control buttons
//---------------------------------------------------------------

fPrevB = new TGTextButton(fF32,"Previous", M__HIV_PREV);
fPrevB->Associate(this);
fF32->AddFrame(fPrevB, new TGLayoutHints(kLHintsNormal,5,5,40,2));

fNextB = new TGTextButton(fF32,"   Next   ", M__HIV_NEXT);
fNextB->Associate(this);
fF32->AddFrame(fNextB, new TGLayoutHints(kLHintsNormal,5,5,6,2));

fDrawB = new TGTextButton(fF32, "   Draw  ", M__HIV_DRAW);
fDrawB->Associate(this);
fDrawB->SetToolTipText("Draw selected histogram/mes");
fF32->AddFrame(fDrawB, new TGLayoutHints(kLHintsNormal,5,5,30,2));

fSliceB = new TGTextButton(fF32, "   Slice  ", M__HIV_SLICE);
fSliceB->Associate(this);
fSliceB->SetToolTipText("Open slice panel");
fF32->AddFrame(fSliceB, new TGLayoutHints(kLHintsNormal,5,5,6,35));

//---------------------------------------------------------------
//********* end of definition
//---------------------------------------------------------------

fDB = new HDisplayDB();
fBuffer = new HDrawingOpt();

countVisual++;

MapSubwindows();

SetWindowName("Histogram Display");

Resize(GetDefaultSize());
MapWindow();

SendMessage(this,MK_MSG(kC_COMMAND,kCM_BUTTON),500,0);
SendMessage(this,MK_MSG(kC_COMMAND,kCM_BUTTON),515,0);

}


//--------------------------------------------------------------------
//******  HHistoVisual::~HHistoVisual()
//
// Destructor of the class HHistoVisual.
//
//--------------------------------------------------------------------

HHistoVisual::~HHistoVisual() {
//
// Destructor of the class HHistoVisual.
//

if(fDB) { delete fDB; fDB = 0; } 

fCanvas = (TCanvas*) gROOT->GetListOfCanvases()->FindObject("maindisp"); 
if(fCanvas ) {
  fCanvas->Clear();
  fCanvas->Modified();
  fCanvas->Update();
  fCanvas = 0;
}
else fCanvas = 0;
fParent = 0;

delete fSliceB; delete fDrawB; delete fNextB; delete fPrevB;
delete fSaveB; delete fHisDefaultB; delete fSysDefaultB;
delete fFix3B; delete fOptionCB; delete fFix2B; delete fScale; delete fFix1B;
delete fYStop; delete fYStart;
delete fXStop; delete fXStart;
for(Int_t i=16;i--;delete fLayoutB[i]);
delete fSelAllB;
delete fMultipleCB;
delete fResetAllB; delete fResetB;
delete fUpdateB;
delete fHistoLB;
delete fF314; delete fF313; delete fF312; 
delete fF31112; delete fF31111;
delete fF3112; delete fF3111;
delete fF311;
delete fF32; delete fF31;
delete fF223; delete fF222; delete fF221;
delete fF22; delete fF21;
delete fF3; delete fF2;
delete fMenuBar;
delete fMenuPicture; delete fMenuFile;
delete fFrame;

countVisual--;

}


//---------------------------------------------------------------------
//******  HHistoVisual::CloseWindow()
//
// This function closes the window.
//
//---------------------------------------------------------------------

void HHistoVisual::CloseWindow() {
//
// This function closes the window.
//

delete this;

}



//---------------------------------------------------------------------
//******  Bool_t HHistoVisual::ProcessMessage(Long_t msg, Long_t parm1,
//                                            Long_t parm2)
//
// This function processes all GUI widgets.
//
//---------------------------------------------------------------------

Bool_t HHistoVisual::ProcessMessage(Long_t msg,Long_t parm1,Long_t parm2) {
//
// This function processes all GUI widgets.
//

HDrawingOpt *p = 0;
TList *pList = 0;
TIterator *it = 0;
TGTextLBEntry *pEntry = 0;
Int_t idx = -1;
Int_t i,j;
HHistogram *pHisto = 0;
TGFileInfo fi; // to save a picture
char savefilename[100];
const char *filetypes[] = { "Postscript files",     "*.ps",
			    "Gif files",    "*.gif",
			    "Root files",   "*.root",
			    "All files",    "*.*",
			    0,               0 };
fi.fFileTypes = (const char **)filetypes;  
TFile *pFile;

switch(GET_MSG(msg)) {

 case kC_COMMAND:

  switch(GET_SUBMSG(msg)) {

  case kCM_MENU:

    switch(parm1) {

    case M__HIV_CLOSE:
      CloseWindow();
      break;

    case M__HIV_SAVEPIC:
      if(!createCanvas()) break;
      new TGFileDialog(fClient->GetRoot(), this, kFDSave, &fi);
      sprintf(savefilename,"%s",fi.fFilename);
      if(!strcmp(savefilename,"(null)")) break;
      sprintf(savefilename,"%s",fi.fFilename);
      fCanvas->cd();
      fCanvas->Print(savefilename);
      break;

    case M__HIV_READHIST:
      new TGFileDialog(fClient->GetRoot(), this, kFDOpen, &fi);
      sprintf(savefilename,"%s",fi.fFilename);
      if(!strcmp(savefilename,"(null)")) break;
      fDB->import(savefilename);
      updateList();
      break;

    case M__HIV_SAVEHIST:
      if(!gOnline) break;
      new TGFileDialog(fClient->GetRoot(), this, kFDSave, &fi);
      sprintf(savefilename,"%s",fi.fFilename);
      if(!strcmp(savefilename,"(null)")) break;
      pFile = new TFile(savefilename,"RECREATE");
      if(!pFile) break;
      i = 1;
      while((pEntry = (TGTextLBEntry*) fHistoLB->Select(i,kFALSE))) {
	idx = gOnline->getHisto()->find(pEntry->GetText()->GetString());
	if(idx == -1) break;
	pHisto = gOnline->getHisto()->at(idx);
	if(!pHisto) break;
	if(pHisto->getDim() == 1) pHisto->getHist1()->Write();
	if(pHisto->getDim() == 2) pHisto->getHist2()->Write();
	i++;
      }
      pFile->Close();
      if(pFile) { delete pFile; pFile = 0; }
      break;

    case M__HIV_EDITPIC:
      if(!createCanvas()) break;
      fCanvas->cd();
      fCanvas->EditorBar();
      break;

    default:
      break;
    }
    break;

  case kCM_BUTTON:

    switch(parm1) {

    case M__HIV_UPDATE:
      updateList();
      break;

    case M__HIV_DRAW:
      gStyle->SetOptStat(1);
      if(fMultipleCB->GetState()) automaticDraw();
      else userDraw();
      break;

    case M__HIV_SELECTALL:
      fMultipleCB->SetState(kButtonDown);
      SendMessage(this,MK_MSG(kC_COMMAND,kCM_CHECKBUTTON),
 		  M__HIV_MULTISELECT,0);
      fHistoLB->SetMultipleSelections(kTRUE);
      i = 1;
      while((pEntry = (TGTextLBEntry*) fHistoLB->Select(i,kFALSE))) {
	fHistoLB->Select(i,kTRUE);
	i++;
      }
      break;

    case M__HIV_RESETALL:
      if(!gOnline) break;
      i = 1;
      while((pEntry = (TGTextLBEntry*) fHistoLB->Select(i,kFALSE))) {
	idx = gOnline->getHisto()->find(pEntry->GetText()->GetString());
	if(idx == -1) break;
	pHisto = gOnline->getHisto()->at(idx);
	if(!pHisto) break;
	if(pHisto->getDim() == 1) pHisto->getHist1()->Reset();
	if(pHisto->getDim() == 2) pHisto->getHist2()->Reset();
	i++;
      }
      SendMessage(this,MK_MSG(kC_COMMAND,kCM_BUTTON),M__HIV_DRAW,0);
      break;

    case M__HIV_RESET:
      if(!gOnline) break;
      if(fMultipleCB->GetState()) {
	if(pList) pList->Clear();
	if(!pList) pList = new TList();
	if(!pList) break;
	fHistoLB->GetSelectedEntries(pList);
	if(it) { delete it; it = 0; }
	it = pList->MakeIterator();
	while((pEntry = (TGTextLBEntry*) it->Next()) != NULL) {
	  idx = gOnline->getHisto()->find(pEntry->GetText()->GetString());
	  if(idx == -1) break;
	  pHisto = gOnline->getHisto()->at(idx);
	  if(!pHisto) break;
	  if(pHisto->getDim() == 1) pHisto->getHist1()->Reset();
	  if(pHisto->getDim() == 2) pHisto->getHist2()->Reset();
	}
	if(pList) pList->Clear();
	if(it) { delete it; it = 0; }
	SendMessage(this,MK_MSG(kC_COMMAND,kCM_BUTTON),M__HIV_SELECTALL,0);
      }
      else {
	pEntry = (TGTextLBEntry*) fHistoLB->GetSelectedEntry();
	if(!pEntry) break;
	  idx = gOnline->getHisto()->find(pEntry->GetText()->GetString());
	  if(idx == -1) break;
	  pHisto = gOnline->getHisto()->at(idx);
	  if(!pHisto) break;
	  if(pHisto->getDim() == 1) pHisto->getHist1()->Reset();
	  if(pHisto->getDim() == 2) pHisto->getHist2()->Reset();
      }	
      SendMessage(this,MK_MSG(kC_COMMAND,kCM_BUTTON),M__HIV_DRAW,0);
      break;

    case M__HIV_NEXT:
      if(fMultipleCB->GetState()) break;
      idx = fHistoLB->GetSelected();
      if(idx < 0) idx = 0;
      pEntry = (TGTextLBEntry*) fHistoLB->Select(++idx,kFALSE);
      if(pEntry) {
	fHistoLB->Select(idx,kTRUE);
	if(idx%7 == 0) fHistoLB->SetTopEntry(idx);
	SendMessage(this,MK_MSG(kC_COMMAND,kCM_LISTBOX),M__HIV_HISTOLIST,idx);
	SendMessage(this,MK_MSG(kC_COMMAND,kCM_BUTTON),M__HIV_DRAW,0);
      }
      else {
	if(gROOT->GetListOfCanvases()->FindObject("maindisp")) {
	  fCanvas = (TCanvas*) gROOT->GetListOfCanvases()->
	    FindObject("maindisp");
	  fCanvas->Clear();
	  fCanvas->Modified();
	  fCanvas->Update();
	}
      }
      break;

    case M__HIV_PREV:
      if(fMultipleCB->GetState()) break;
      idx = fHistoLB->GetSelected();
      if(idx < 0) idx = 0;
      pEntry = (TGTextLBEntry*) fHistoLB->Select(--idx,kFALSE);
      if(pEntry) {
	fHistoLB->Select(idx,kTRUE);
	if(idx%7 == 0) fHistoLB->SetTopEntry(idx-6);
	SendMessage(this,MK_MSG(kC_COMMAND,kCM_LISTBOX),M__HIV_HISTOLIST,idx);
	SendMessage(this,MK_MSG(kC_COMMAND,kCM_BUTTON),M__HIV_DRAW,0);
      }
      else {
	if(gROOT->GetListOfCanvases()->FindObject("maindisp")) {
	  fCanvas = (TCanvas*) gROOT->GetListOfCanvases()->
	    FindObject("maindisp");
	  fCanvas->Clear();
	  fCanvas->Modified();
	  fCanvas->Update();
	}
      }
      break;

    case M__HIV_SLICE:
      if(HSlice::countSlice) {
	HError::message("The requested panel is already open.");
	break;
      }
      SendMessage(((HOnlMainWindow*)fParent),MK_MSG(kC_COMMAND,kCM_MENU),
		  M__SLICE,0);
      //      new HSlice(fClient->GetRoot(),1,1);
      break;

    case M__HIV_SYSDEFAULT:
      if(fMultipleCB->GetState()) break;
      pEntry = (TGTextLBEntry*) fHistoLB->GetSelectedEntry();
      if(!pEntry) break;
      if(!gOnline) break;
      idx = gOnline->getHisto()->find(pEntry->GetText()->GetString());
      pHisto = gOnline->getHisto()->at(idx);
      if(!pHisto) {
	idx = fDB->find(pEntry->GetText()->GetString());
	pHisto = fDB->at(idx);
	if(!pHisto) break;
      }
      drawing(pHisto,1);
      break;

    case M__HIV_HISDEFAULT:
      if(fMultipleCB->GetState()) break;
      pEntry = (TGTextLBEntry*) fHistoLB->GetSelectedEntry();
      if(!pEntry) break;
      if(!gOnline) break;
      idx = gOnline->getHisto()->find(pEntry->GetText()->GetString());
      pHisto = gOnline->getHisto()->at(idx);
      if(!pHisto) break;
      drawing(pHisto,2);
      break;

    case M__HIV_SAVEOPT:
      if(!gOnline) break;
      if(fMultipleCB->GetState()) break;
      p = new HDrawingOpt();
      if(!p) break;
      if(!pList) pList = new TList();
      if(!pList) break;
      pList->Clear();

      p->xstart = atof(fXStartBuf->GetString());
      p->xstop = atof(fXStopBuf->GetString());
      p->ystart = atof(fYStartBuf->GetString());
      p->ystop = atof(fYStopBuf->GetString());
      p->scale = atof(fScaleBuf->GetString());
      if(!strcmp(isSelected(fOptionCB),"no")) 
	sprintf((*p).option,"%s","");
      else sprintf((*p).option,"%s",isSelected(fOptionCB));

      fHistoLB->GetSelectedEntries(pList);
      if(it) { delete it; it = 0; }
      it = pList->MakeIterator();

      while((pEntry = (TGTextLBEntry*) it->Next()) != NULL) {
	idx = gOnline->getHisto()->find(pEntry->GetText()->GetString());
	if(idx != -1) {
	  pHisto = gOnline->getHisto()->at(idx);
	  if(!pHisto) break;
	  pHisto->setDrawingOpt(p,kTRUE);
	  if(gPad) pHisto->setTheta(gPad->GetTheta());
	  if(gPad) pHisto->setPhi(gPad->GetPhi());
	}
	if(idx == -1) { // runtime db
	  idx = fDB->find(pEntry->GetText()->GetString());
	  pHisto = fDB->at(idx);
	  if(!pHisto) break;
	  pHisto->setDrawingOpt(p,kTRUE);
	  if(gPad) pHisto->setTheta(gPad->GetTheta());
	  if(gPad) pHisto->setPhi(gPad->GetPhi());
	}
      }
      if(p) { delete p; p = 0; }
      break;

    case M__HIV_FIX1:
      kFix1 = !kFix1;
      if(kFix1) fFix1B->SetState(kButtonEngaged);
      else fFix1B->SetState(kButtonUp);
      break;

    case M__HIV_FIX2:
      kFix2 = !kFix2;
      if(kFix2) fFix2B->SetState(kButtonEngaged);
      else fFix2B->SetState(kButtonUp);
      break;

    case M__HIV_FIX3:
      kFix3 = !kFix3;
      if(kFix3) fFix3B->SetState(kButtonEngaged);
      else fFix3B->SetState(kButtonUp);
      break;

    default:
      // Process the panel "Display Layout"

      if(parm1>=500 && parm1<=515 && !fMultipleCB->GetState()) {
	if(count==2) { count = 0; indi[0]=indi[1]=indj[0]=indj[1]=-1; }
	count++;
	indi[count-1] = (parm1-500)/4;
	indj[count-1] = (parm1-500)%4;
	displayLayout();
      }
      break;
    }
    break;
  
  case kCM_CHECKBUTTON:

    switch(parm1) {
    case M__HIV_MULTISELECT:
      fHistoLB->SetMultipleSelections(fMultipleCB->GetState());
      clearTF();
      if(gROOT->GetListOfCanvases()->FindObject("maindisp")) {
	fCanvas = (TCanvas*) gROOT->GetListOfCanvases()->
	  FindObject("maindisp");
	fCanvas->Clear();
	fCanvas->cd();
	fCanvas->Modified();
	fCanvas->Update();
      }
      for(i=0;i<16;i++) {
	kColor[i] = kFALSE;
	toDefault(fLayoutB[i]->GetId());
	fClient->NeedRedraw(fLayoutB[i]);
      }
      count = 2;
      for(j=0; j<16; j++) histIdx[j] = -1;
      break;

    default:
      break;
    }
    break;

  case kCM_LISTBOX:

    switch(parm1) {

    case M__HIV_HISTOLIST:
      if(fMultipleCB->GetState()) break;
      pEntry = (TGTextLBEntry*) fHistoLB->GetSelectedEntry();
      if(!pEntry) break;
      if(!gOnline) break;
      idx = gOnline->getHisto()->find(pEntry->GetText()->GetString());
      pHisto = gOnline->getHisto()->at(idx);
      if(!pHisto) {
	idx = fDB->find(pEntry->GetText()->GetString());
	pHisto = fDB->at(idx);
	if(!pHisto) break;
      }
      drawing(pHisto,2);
      break;
    default:
      break;
    }
    break;

  default:
    break;
  }

 default:
   break;
}

return kTRUE;

}


//---------------------------------------------------------------------
//******  Bool_t HHistoVisual::toDefault(Window_t id)
//
// This function changes button color to default one (gray).
//
//---------------------------------------------------------------------

Bool_t HHistoVisual::toDefault(Window_t id) {
//
// This function changes the button color to default one (gray).
//

  SetWindowAttributes_t wattr;
  wattr.fMask = kWABackPixel;
  wattr.fBackgroundPixel = fgDefaultFrameBackground;
  gVirtualX->ChangeWindowAttributes(id, &wattr);
  return kTRUE;

}


//---------------------------------------------------------------------
//******  Bool_t HHistoVisual::toGreen(Window_t id)
//
// This function changes the button color to green.
//
//---------------------------------------------------------------------

Bool_t HHistoVisual::toGreen(Window_t id) {
//
// This function changes the button color to green.
//

  SetWindowAttributes_t wattr;
  wattr.fMask = kWABackPixel;
  gClient->GetColorByName("green", wattr.fBackgroundPixel);
  gVirtualX->ChangeWindowAttributes(id, &wattr);
  return kTRUE;

}


//---------------------------------------------------------------------
//******  void HHistoVisual::drawHist(HHistogram *hist)
//
// This function draws an object of the class HHistogram.
//
//---------------------------------------------------------------------

void HHistoVisual::drawHist(HHistogram *hist) {
//
// This function draws an object of the class HHistogram.
//

  if(!hist) return;
  if(!gPad || !createCanvas()) return;

  Int_t dim = hist->getDim();

  if(fMultipleCB->GetState()) {
    hist->setDrawingOpt(kFALSE);
    if(dim == 1) hist->getHist1()->Draw(hist->getHist1()->GetOption());
    if(dim == 2) hist->getHist2()->Draw(hist->getHist2()->GetOption());
    return;
  }

  HDrawingOpt *p = new HDrawingOpt();
  if(!p) return;

  p->xstart = atof(fXStartBuf->GetString());
  p->xstop = atof(fXStopBuf->GetString());
  p->ystart = atof(fYStartBuf->GetString());
  p->ystop = atof(fYStopBuf->GetString());
  p->scale = atof(fScaleBuf->GetString());
  if(!strcmp(isSelected(fOptionCB),"no"))
    sprintf((*p).option,"%s","");
  else sprintf((*p).option,"%s",isSelected(fOptionCB));

  hist->setDrawingOpt(p,kFALSE);
  if(p) { delete p; p = 0; }

  gPad->SetTheta(theta);
  gPad->SetPhi(phi);

  if(dim == 1) {
    if(!strcmp(isSelected(fOptionCB),"no"))
      hist->getHist1()->Draw();
    else hist->getHist1()->Draw(isSelected(fOptionCB));
  }
  if(dim == 2) {
    if(!strcmp(isSelected(fOptionCB),"no"))
      hist->getHist2()->Draw();
    else hist->getHist2()->Draw(isSelected(fOptionCB));
  }

}


//---------------------------------------------------------------------
//******  void HHistoVisual::drawing(HHistogram *hist, Int_t mode)
//
// This function sets the fields with the values of drawing settings.
// mode = 1 -- sys default
// mode = 2 -- hist default
//
//---------------------------------------------------------------------

void HHistoVisual::drawing(HHistogram *hist, Int_t mode) {
//
// This function sets the fields with the values of drawing settings.
// mode = 1 -- sys default
// mode = 2 -- hist default
//

  if(!hist) return;
  
  char str[20] = "";
  Int_t dim = hist->getDim();

  // clear all text fields

  if(!fFix1B->GetState()) {
    fXStartBuf->Clear(); fXStopBuf->Clear();
    fYStartBuf->Clear(); fYStopBuf->Clear();
  }
  if(!fFix2B->GetState()) fScaleBuf->Clear(); 
  if(!fFix3B->GetState()) highlight(fOptionCB,"no");

  // if mode == 1

  if(mode == 1) {

    hist->setDrawingOpt();

    if(dim == 1) {
      if(!fFix1B->GetState()) {
	sprintf(str,"%.2f",hist->getHist1()->GetXaxis()->GetXmin());
	fXStartBuf->AddText(0,str);
	sprintf(str,"%.2f",hist->getHist1()->GetXaxis()->GetXmax());
	fXStopBuf->AddText(0,str);
      }
      if(!fFix2B->GetState()) {
	sprintf(str,"%.2f",hist->getHist1()->GetMaximum());
	fScaleBuf->AddText(0,str);
      }
    }

    if(dim == 2) {
      if(!fFix1B->GetState()) {
	sprintf(str,"%.2f",hist->getHist2()->GetXaxis()->GetXmin());
	fXStartBuf->AddText(0,str);
	sprintf(str,"%.2f",hist->getHist2()->GetXaxis()->GetXmax());
	fXStopBuf->AddText(0,str);
	sprintf(str,"%.2f",hist->getHist2()->GetYaxis()->GetXmin());
	fYStartBuf->AddText(0,str);
	sprintf(str,"%.2f",hist->getHist2()->GetYaxis()->GetXmax());
	fYStopBuf->AddText(0,str);
      }
      if(!fFix2B->GetState()) {
	sprintf(str,"%.2f",hist->getHist2()->GetMaximum());
	fScaleBuf->AddText(0,str);
      }
    }

  }

  if(mode == 2) {

    hist->setDrawingOpt(kFALSE);

    if(dim == 1) {

      if(!fFix1B->GetState()) {
	sprintf(str,"%.2f",hist->getHist1()->GetXaxis()->
		GetBinLowEdge(hist->getHist1()->GetXaxis()->GetFirst()));
	fXStartBuf->AddText(0,str);
	sprintf(str,"%.2f",hist->getHist1()->GetXaxis()->
		GetBinUpEdge(hist->getHist1()->GetXaxis()->GetLast()));
	fXStopBuf->AddText(0,str);
      }	
      if(!fFix2B->GetState()) {
	sprintf(str,"%.2f",hist->getHist1()->GetMaximum());
	fScaleBuf->AddText(0,str);
      }
      if(!fFix3B->GetState()) {
	if(!strcmp(hist->getHist1()->GetOption(),"")) 
	  highlight(fOptionCB,"no");
	else highlight(fOptionCB,hist->getHist1()->GetOption());
	theta = hist->getTheta();
	phi = hist->getPhi();
	if(theta == 0.0 && phi == 0.0) { theta = phi = 30.0; }
      }
    }

    if(dim == 2) {

      if(!fFix1B->GetState()) {
	sprintf(str,"%.2f",hist->getHist2()->GetXaxis()->
		GetBinLowEdge(hist->getHist2()->GetXaxis()->GetFirst()));
	fXStartBuf->AddText(0,str);
	sprintf(str,"%.2f",hist->getHist2()->GetXaxis()->
		GetBinUpEdge(hist->getHist2()->GetXaxis()->GetLast()));
	fXStopBuf->AddText(0,str);
	sprintf(str,"%.2f",hist->getHist2()->GetYaxis()->
		GetBinLowEdge(hist->getHist2()->GetYaxis()->GetFirst()));
	fYStartBuf->AddText(0,str);
	sprintf(str,"%.2f",hist->getHist2()->GetYaxis()->
		GetBinUpEdge(hist->getHist2()->GetYaxis()->GetLast()));
	fYStopBuf->AddText(0,str);
      }
      if(!fFix2B->GetState()) {

	sprintf(str,"%.2f",hist->getHist2()->GetMaximum());
	fScaleBuf->AddText(0,str);
      }
      if(!fFix3B->GetState()) {

	if(!strcmp(hist->getHist2()->GetOption(),"")) 
	  highlight(fOptionCB,"no");
	else highlight(fOptionCB,hist->getHist2()->GetOption());
	theta = hist->getTheta();
	phi = hist->getPhi();
	if(theta == 0.0 && phi == 0.0) { theta = phi = 30.0; }
      }
    }

  }

  fClient->NeedRedraw(fXStart); fClient->NeedRedraw(fXStop);
  fClient->NeedRedraw(fYStart); fClient->NeedRedraw(fYStop);
  fClient->NeedRedraw(fScale);

}


//---------------------------------------------------------------------
//******  void HHistoVisual::setCanvasDivision(Int_t number)
//
// This function calculates the xDiv and yDiv parameters which are used 
// to divide the canvas into subpads.
// This function is used in case of the automatic display layout
// (checkbutton "Multiple selection" is engaged).
// The function takes as an input parameter the total number of histogrammes
// to be displayed.
// Called from automaticDraw()
//
//---------------------------------------------------------------------

void HHistoVisual::setCanvasDivision(Int_t number) {
//
// This function calculates the xDiv and yDiv parameters which are used 
// to divide the canvas into subpads.
// This function is used in case of the automatic display layout
// (checkbutton "Multiple selection" is engaged).
// The function takes as an input parameter the total number of histogrammes
// to be displayed.
// Called from automaticDraw()
//

  Int_t i, j, k;
  for(i = 1; i<50; i++) {
    k = 0;
    for(j = i-1; j <= i; j++) {
      if(number <= i*j) {
	k = j;
	break;
      }
    }
    if(number <= i*k) break;
  }
  xDiv = i;
  yDiv = k;

}


//---------------------------------------------------------------------
//******  void HHistoVisual::automaticDraw()
//
// This function draws a set of histogrammes in the canvas in case of 
// the automatic display layout (checkbutton "Multiple selection" is engaged). 
// Called when the button "Draw" is clicked.
//
//---------------------------------------------------------------------

void HHistoVisual::automaticDraw() {
//
// This function draws a set of histogrammes in the canvas in case of 
// the automatic display layout (checkbutton "Multiple selection" is engaged). 
// Called when the button "Draw" is clicked.
//

  if(!createCanvas()) return;
  if(!gOnline) return;
  TList *pList = new TList;
  if(!pList) return;

  fHistoLB->GetSelectedEntries(pList);

  TGTextLBEntry *pEntry = 0;
  TIterator *it = 0;
  HHistogram *pHisto = 0;

  it = pList->MakeIterator();
  Int_t number = 0;
  Int_t idx = -1;

  while((pEntry = (TGTextLBEntry*) it->Next()) != NULL) number++;

  setCanvasDivision(number);
  fCanvas->Clear();
  fCanvas->Divide(xDiv,yDiv);
  
  if(it) { delete it; it = 0; }

  it = pList->MakeIterator();
  number = 0;

  while((pEntry = (TGTextLBEntry*) it->Next()) != NULL) {

    idx = gOnline->getHisto()->find(pEntry->GetText()->GetString());
    if(idx != -1) {

      pHisto = gOnline->getHisto()->at(idx);
      number++;
      fCanvas->cd(number);
      if(fMultipleCB->GetState()) drawHist(pHisto);
    }
    else {
      idx = fDB->find(pEntry->GetText()->GetString());
      if(idx != -1) {
	pHisto = fDB->at(idx);
	number++;
	fCanvas->cd(number);
	if(fMultipleCB->GetState()) drawHist(pHisto);
      }
    }
  }
  if(pList) pList->Clear();
  pList = 0;
  if(it) { delete it; it = 0; }

  //  fCanvas->cd();
  fCanvas->Modified();
  fCanvas->Update();

}


//---------------------------------------------------------------------
//******  void HHistoVisual::userDraw()
//
// This function draws a histogram in the canvas in case of 
// the user defined display layout 
// (checkbutton "Multiple selection" is in the up position). 
// Called when the button "Draw" is clicked.
//
//---------------------------------------------------------------------

void HHistoVisual::userDraw() {
//
// This function draws a histogram in the canvas in case of 
// the user defined display layout 
// (checkbutton "Multiple selection" is in the up position). 
// Called when the button "Draw" is clicked.
//

  if(!isLayout()) return;
  if(!gOnline) return;
  if(!createCanvas()) return;

  Int_t idx = -1;
  TGTextLBEntry *pEntry = (TGTextLBEntry*) fHistoLB->GetSelectedEntry();
  if(!pEntry) return;
  
  HHistogram *pHisto;
  idx = gOnline->getHisto()->find(pEntry->GetText()->GetString());
  pHisto = gOnline->getHisto()->at(idx);
  if(!pHisto) {
    idx = fDB->find(pEntry->GetText()->GetString());
    pHisto = fDB->at(idx);
    if(!pHisto) return;
    idx += 1000;
  }

  Int_t tobedeleted[16];
  Int_t toberedrawn[16];
  Int_t i;
  Int_t j, k;
  Int_t l, ll;
  char padname[10];
  Float_t xmin = 0.0F;
  Float_t xmax = 0.0F;
  Float_t ymin = 0.0F;
  Float_t ymax = 0.0F;
  TPad *pad;
  HHistogram *pHistoRedraw = 0;
  Bool_t kLoopWasBroken = kFALSE;

  for(i=0;i<16;i++) tobedeleted[i] = toberedrawn[i] = -1; 

  if(gPad && gPad != fCanvas) {
    theta = gPad->GetTheta();
    phi = gPad->GetPhi();
  }
  fCanvas->Clear();

  // calculate indexes of histogrammes to be deleted and to be redrawn

  j = 0;
  k = 0;
  ll = 0;

  for(i=0;i<16;i++) {
    if(kColor[i] || histIdx[i] == idx) {
      if(histIdx[i] != -1) {
	if(j==0) tobedeleted[j++] = histIdx[i];
	kLoopWasBroken = kFALSE;
	for(l=0;l<j;l++) { 
	  if(histIdx[i] == tobedeleted[l]) {
	    kLoopWasBroken = kTRUE;
	    break;
	  }
	}
	if(!kLoopWasBroken) tobedeleted[j++] = histIdx[i];
      }
    }
    else {
      if(histIdx[i] != -1) {
	if(k==0) toberedrawn[k++] = histIdx[i];
	kLoopWasBroken = kFALSE;
	for(l=0;l<k;l++) { 
	  if(histIdx[i] == toberedrawn[l]) {
	    kLoopWasBroken = kTRUE;
	    break;
	  }
	}
	if(!kLoopWasBroken) toberedrawn[k++] = histIdx[i];
      }
    }
  }

  // say "Addio" to those who were not lucky
  // set in histIdx indexes of histogrammes to be deleted to -1

  for(l=0; l<j; l++) {
    for(i=0; i<16; i++) {
      if(histIdx[i] == tobedeleted[l]) histIdx[i] = -1;
    }
    for(ll=0; ll<k; ll++) {
      if(toberedrawn[ll] == tobedeleted[l]) toberedrawn[ll] = -1;
    }
  }

  for(i=0;i<16;i++) { if(kColor[i]) histIdx[i] = idx; }


  // redraw those who survived after repressions

  for(i=0; i<k; i++) {

    if(toberedrawn[i]<1000)
      pHistoRedraw = gOnline->getHisto()->at(toberedrawn[i]);
    else pHistoRedraw = fDB->at(toberedrawn[i]-1000);

    if(!pHistoRedraw) continue;
    calcPad(toberedrawn[i],&xmin,&ymin,&xmax,&ymax);
    sprintf(padname,"%s%i","pad",i);

    //  cout<<"redraw: "<<xmin<<"  "<<ymin<<"  "<<xmax<<"  "<<ymax<<endl;

    pad = new TPad("padname","padname",xmin,ymin,xmax,ymax);
    pad->SetFillColor(fCanvas->GetFillColor());
    fCanvas->cd();
    pad->Draw();
    pad->cd();
    drawing(pHistoRedraw,2);
    drawHist(pHistoRedraw);
  }

  // draw the histogram that has to be drawn

  calcPad(idx,&xmin,&ymin,&xmax,&ymax);
  sprintf(padname,"%s%i","pad",k);

  //  cout<<"draw: "<<xmin<<"  "<<ymin<<"  "<<xmax<<"  "<<ymax<<endl;

  pad = new TPad("padname","padname",xmin,ymin,xmax,ymax);
  pad->SetFillColor(fCanvas->GetFillColor());
  fCanvas->cd();
  pad->Draw();
  pad->cd();
  drawing(pHisto,2);
  drawHist(pHisto);
  //  fCanvas->cd();
  fCanvas->Modified();
  fCanvas->Update();

}

  
//---------------------------------------------------------------------
//******  void HHistoVisual::clearTF()
//
// This function clears all text fields.
//
//---------------------------------------------------------------------

void HHistoVisual::clearTF() {
//
// This function clears all text fields.
//

  fXStartBuf->Clear(); fXStopBuf->Clear();
  fYStartBuf->Clear(); fYStopBuf->Clear();
  fScaleBuf->Clear(); highlight(fOptionCB,"no");
  fClient->NeedRedraw(fXStart); fClient->NeedRedraw(fXStop);
  fClient->NeedRedraw(fYStart); fClient->NeedRedraw(fYStop);
  fClient->NeedRedraw(fScale);

}


//---------------------------------------------------------------------
//******  Bool_t HHistoVisual::isLayout()
//
// This function checks if the display layout is set. 
//
//---------------------------------------------------------------------

Bool_t HHistoVisual::isLayout() {
//
// This function checks if the display layout is set. 
//

  Int_t i;
  for(i=0;i<16;i++) { if(kColor[i]) return kTRUE; }
  return kFALSE;

}


//---------------------------------------------------------------------
//******  void HHistovisual::highlight(TGComboBox *p, const char* name)
//
// This function highlights the entry in the combo box.
//
//---------------------------------------------------------------------

void HHistoVisual::highlight(TGComboBox *p, const char* name) {
//
// This function highlights the entry in the combo box.
//

  if(!p || !name) return;
  TGTextLBEntry *pEntry;
  TGListBox *pListBox;
  Int_t i = 1;

  pListBox = const_cast <TGListBox*> (p->GetListBox());
  if(!pListBox) return;

  while((pEntry = (TGTextLBEntry*) pListBox->Select(i,kFALSE))) {

    if(!strcmp(pEntry->GetText()->GetString(),name)) {
      p->Select(i);
      break;
    }
    i++;
  }

  pListBox = 0;

}


//---------------------------------------------------------------------
//******  const char* HHistovisual::isSelected(TGComboBox *p)
//
// This function returns the text in the highlighted entry of the combo box.
//
//---------------------------------------------------------------------

const char* HHistoVisual::isSelected(TGComboBox *p) {
//
// This function returns the text in the highlighted entry of the combo box.
//

  if(!p) return 0;

  TGTextLBEntry *pEntry = (TGTextLBEntry*) p->GetSelectedEntry();

  if(!pEntry) return 0;

  return (pEntry->GetText()->GetString());

}


//---------------------------------------------------------------------
//******  void HHistoVisual::updateList()
//
// This function updates the list of histogrammes.
//
//---------------------------------------------------------------------

void HHistoVisual::updateList() {
//
// This function updates the list of histogrammes.
//

  if(!gOnline) return;
  TGTextLBEntry *pEntry;
  Int_t i;

  i = 1;
  while((pEntry = (TGTextLBEntry*) fHistoLB->Select(i,kFALSE))) {
    fHistoLB->RemoveEntry(i);
    i++;
  }

  i = 1;

  for(Int_t j=0;j<gOnline->getHisto()->getEntries();j++) {

    if(gOnline->getHisto()->at(j)) 
      fHistoLB->AddEntry(gOnline->getHisto()->at(j)->GetName(),i++);
  }

  for(Int_t j=0;j<fDB->getEntries();j++) {

    if(fDB->at(j)) 
      fHistoLB->AddEntry(fDB->at(j)->GetName(),i++);
  }

  fHistoLB->MapSubwindows();
  fHistoLB->Layout();

}


//---------------------------------------------------------------------
//******  void HHistoVisual::calcPad(Int_t index, Float_t *x1, Float_t *y1,
//                                                Float_t *x2, Float_t *y2)
//
// This function calculates dimensions (x1,y1,x2,y2) of the pad for 
// the histogram with idx = index.
//
//---------------------------------------------------------------------

void HHistoVisual::calcPad(Int_t index, Float_t *x1, Float_t *y1,
			                Float_t *x2, Float_t *y2) {
//
// This function calculates dimensions (x1,y1,x2,y2) of the pad for 
// the histogram with idx = index.
//

  if(!x1 || !y1 || !x2 || !y2 || index<0) return;

  Int_t imin = -1;
  Int_t imax = -1;
  Int_t jmin = -1;
  Int_t jmax = -1;

  Float_t X[8] = { 0.01, 0.24, 0.26, 0.49, 0.51, 0.74, 0.76, 0.99 };
  Float_t Y[8] = { 0.99, 0.76, 0.74, 0.51, 0.49, 0.26, 0.24, 0.01 };

  for(Int_t i=0; i<16; i++) {

    if(index == histIdx[i] && imin==-1 && jmin==-1) { imin=i/4; jmin=i%4; }
    if(index == histIdx[i] && imin!=-1 && jmin!=-1) { imax=i/4; jmax=i%4; }

  }

  *x1 = X[jmin*2];
  *y1 = Y[imax*2 + 1];
  *x2 = X[jmax*2 + 1];
  *y2 = Y[imin*2];

}


//---------------------------------------------------------------------
//******  void HHistoVisual::displayLayout()
//
// This function redraws the layout buttons.
//
//---------------------------------------------------------------------

void HHistoVisual::displayLayout() {
//
// This function redraws the layout buttons.
//

  Int_t i = 0;
  Int_t imin, imax, jmin, jmax;

  imin = imax = jmin = jmax = -1;

  if(indi[1] == -1 && indj[1] == -1) { // count == 1

    for(i=0;i<16;i++) {
      kColor[i] = kFALSE;
      toDefault(fLayoutB[i]->GetId());
      if(i == (4*indi[0]+indj[0])) {
	kColor[i] = kTRUE;
	toGreen(fLayoutB[i]->GetId());
      }
      fClient->NeedRedraw(fLayoutB[i]);
    }
  }
  else { // count == 2

    imin = (indi[0]<indi[1]) ? indi[0] : indi[1];
    imax = (indi[0]>indi[1]) ? indi[0] : indi[1];
    jmin = (indj[0]<indj[1]) ? indj[0] : indj[1];
    jmax = (indj[0]>indj[1]) ? indj[0] : indj[1];

    for(i=0;i<16;i++) {

      if(i/4 >= imin && i/4 <= imax && i%4 >= jmin && i%4 <= jmax) {
	kColor[i] = kTRUE;
	toGreen(fLayoutB[i]->GetId());
      }
      else {
	kColor[i] = kFALSE;
	toDefault(fLayoutB[i]->GetId());
      }
      fClient->NeedRedraw(fLayoutB[i]);
    }
  }

}


//---------------------------------------------------------------------
//******  Bool_t HHistoVisual::createCanvas()
//
// This function creates the display canvas if necessary.
//
//---------------------------------------------------------------------

Bool_t HHistoVisual::createCanvas() {
//
// This function creates the display canvas if necessary.
//

  HOnlMainWindow *pWind = (HOnlMainWindow*) fParent;
  fCanvas = (TCanvas*) gROOT->GetListOfCanvases()->FindObject("maindisp");
  if(pWind && pWind->IsOnHeap()) {
    if(!fCanvas) {
      pWind->createCanvas();
      fCanvas = pWind->getCanvas();
    }
    return kTRUE;
  }

  return kFALSE;

}


//---------------------------------------------------------------------
//******  void HHistoVisual::saveToBuffer()
//
// This function saves to a buffer the current drawing options.
//
//---------------------------------------------------------------------

void HHistoVisual::saveToBuffer() {
//
// This function saves to a buffer the current drawing options.
//

  if(!fBuffer) fBuffer = new HDrawingOpt();
  fBuffer->xstart = atof(fXStartBuf->GetString());
  fBuffer->xstop = atof(fXStopBuf->GetString());
  fBuffer->ystart = atof(fYStartBuf->GetString());
  fBuffer->ystop = atof(fYStopBuf->GetString());
  fBuffer->scale = atof(fScaleBuf->GetString());
  if(!strcmp(isSelected(fOptionCB),"no"))
    sprintf((*fBuffer).option,"%s","");
  else sprintf((*fBuffer).option,"%s",isSelected(fOptionCB));

}


//---------------------------------------------------------------------
//******  void HHistoVisual::restoreBuffer()
//
// This function fills all the drawing options fields with the buffer content.
//
//---------------------------------------------------------------------

void HHistoVisual::restoreBuffer() {
//
// This function fills all the drawing options fields with the buffer content.
//

  if(!fBuffer) return;
  clearTF();
  char cont[50];
  sprintf(cont,"%f",fBuffer->xstart);
  fXStartBuf->AddText(0,cont);
  sprintf(cont,"%f",fBuffer->xstop);
  fXStopBuf->AddText(0,cont);
  sprintf(cont,"%f",fBuffer->ystart);
  fYStartBuf->AddText(0,cont);
  sprintf(cont,"%f",fBuffer->ystop);
  fYStopBuf->AddText(0,cont);
  sprintf(cont,"%f",fBuffer->scale);
  fScaleBuf->AddText(0,cont);
  if(!strcmp((*fBuffer).option,"")) 
    highlight(fOptionCB,"no");
  else highlight(fOptionCB,(*fBuffer).option);
  
  fClient->NeedRedraw(fXStart); fClient->NeedRedraw(fXStop);
  fClient->NeedRedraw(fYStart); fClient->NeedRedraw(fYStop);
  fClient->NeedRedraw(fScale);

}









