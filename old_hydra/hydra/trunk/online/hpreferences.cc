//*-- Author : Dmitry Vasiliev (vassili@lns.infn.it)
/* 
 * Copyright P.Finocchiaro and D.Vasiliev (INFN-LNS, Catania) HADES collaboration
*/

//-------------------------------------------------------------------------
//
// File name:       hpreferences.cc
// Subject:         Preferences (canvas options, memory limits).
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            April 2000
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the source code for the member functions of 
// the class HPreferences.
// This is a GUI class which allows the user to define and save 
// the preferences.
// That means canvas dimensions, background color etc.
//
//-------------------------------------------------------------------------

#include "hpreferences.h"
#include "honlmainwindow.h"
#include "honlinecontrol.h"
#include "hmemorycheck.h"
#include "TGLabel.h"
#include <stdlib.h>
#include "TCanvas.h"
#include "TROOT.h"
#include "herror.h"

//_HADES_CLASS_DESCRIPTION 
/////////////////////////////////////////////////////////////////////
// This is a GUI class which allows the user to define and save 
// the preferences.
// That means canvas dimensions, background color etc.
//////////////////////////////////////////////////////////////////////
ClassImp(HPreferences)

//---------------------------------------------------------------------
//******  HPreferences::HPreferences(const TGWindow *p,UInt_t w,UInt_t h,
//                                   TGMainFrame *p2 = 0)
//
// Constructor of the class HPreferences.
//
//---------------------------------------------------------------------

HPreferences::HPreferences(const TGWindow *p, UInt_t w, UInt_t h,
			      TGMainFrame *p2) : TGMainFrame(p,w,h) {
//
// Constructor of the class HPreferences.
//

fParent = p2;

//---------------------------------------------------------------
//********* frames
//---------------------------------------------------------------

fFrame = new TGCompositeFrame(this, 400, 400, kVerticalFrame);
AddFrame(fFrame, new TGLayoutHints(kLHintsNormal));

fF0 = new TGCompositeFrame(fFrame, 400, 300, kVerticalFrame);
fFrame->AddFrame(fF0, new TGLayoutHints(kLHintsNormal));

fF00 = new TGCompositeFrame(fFrame, 400, 100, kVerticalFrame);
fF00->SetLayoutManager(new TGMatrixLayout(fF00,0,3,10));
fFrame->AddFrame(fF00, new TGLayoutHints(kLHintsNormal));

fTab = new TGTab(fF0, 400, 300);
fTab->Associate(this);
fF0->AddFrame(fTab, new TGLayoutHints(kLHintsNormal));


//---------------------------------------------------------------
//********* canvas panel
//---------------------------------------------------------------

fCanvFrame = fTab->AddTab("Display window");
fCanvFrame->SetLayoutManager(new TGVerticalLayout(fCanvFrame));

fF1 = new TGCompositeFrame(fCanvFrame, 400, 300, kHorizontalFrame);
fCanvFrame->AddFrame(fF1, new TGLayoutHints(kLHintsNormal,0,0,5,5));

fF2 = new TGGroupFrame(fCanvFrame, "display window sizes", kHorizontalFrame);
fCanvFrame->AddFrame(fF2, new TGLayoutHints(kLHintsNormal,0,0,3,3));

fF3 = new TGCompositeFrame(fCanvFrame, 400, 300, kHorizontalFrame);
fCanvFrame->AddFrame(fF3, new TGLayoutHints(kLHintsNormal,0,0,3,3));

fMenuBarRB = new TGRadioButton(fF1, "Menu bar", M_PREF_MENUBAR);
fMenuBarRB->Associate(this);
fF1->AddFrame(fMenuBarRB, new TGLayoutHints(kLHintsNormal,3,5,3,2));

fNoMenuBarRB = new TGRadioButton(fF1, "No menu bar", M_PREF_NOMENUBAR);
fNoMenuBarRB->Associate(this);
fF1->AddFrame(fNoMenuBarRB, new TGLayoutHints(kLHintsNormal,3,5,3,2));

fF2->AddFrame(new TGLabel(fF2, new TGHotString("width")),
	      new TGLayoutHints(kLHintsNormal,2,2,5,2));
fCanvWidthCB = new TGComboBox(fF2, M_PREF_WIDTH);
fCanvWidthCB->Associate(this);
fCanvWidthCB->AddEntry("256",1);
fCanvWidthCB->AddEntry("384",2);
fCanvWidthCB->AddEntry("512",3);
fCanvWidthCB->AddEntry("640",4);
fCanvWidthCB->AddEntry("768",5);
fCanvWidthCB->AddEntry("896",6);
fCanvWidthCB->AddEntry("1024",7);
fCanvWidthCB->AddEntry("1152",8);
fCanvWidthCB->AddEntry("1280",9);
fF2->AddFrame(fCanvWidthCB, new TGLayoutHints(kLHintsNormal,2,2,4,1));
fCanvWidthCB->Resize(60,20);

fF2->AddFrame(new TGLabel(fF2, new TGHotString("height")),
	      new TGLayoutHints(kLHintsNormal,10,2,5,2));
fCanvHeightCB = new TGComboBox(fF2, M_PREF_HEIGHT);
fCanvHeightCB->Associate(this);
fCanvHeightCB->AddEntry("256",1);
fCanvHeightCB->AddEntry("384",2);
fCanvHeightCB->AddEntry("512",3);
fCanvHeightCB->AddEntry("640",4);
fCanvHeightCB->AddEntry("768",5);
fCanvHeightCB->AddEntry("896",6);
fCanvHeightCB->AddEntry("1024",7);
fF2->AddFrame(fCanvHeightCB, new TGLayoutHints(kLHintsNormal,2,2,4,1));
fCanvHeightCB->Resize(60,20);


fF3->AddFrame(new TGLabel(fF3, new TGHotString("display background color")),
	      new TGLayoutHints(kLHintsNormal,2,2,2,2));
fCanvColorCB = new TGComboBox(fF3, M_PREF_COLOR);
fCanvColorCB->Associate(this);
fCanvColorCB->AddEntry("white",1);
fCanvColorCB->AddEntry("black",2);
fCanvColorCB->AddEntry("red",3);
fCanvColorCB->AddEntry("green",4);
fCanvColorCB->AddEntry("blue",5);
fCanvColorCB->AddEntry("yellow",6);
fCanvColorCB->AddEntry("brown",7);
fF3->AddFrame(fCanvColorCB, new TGLayoutHints(kLHintsNormal,4,2,1,1));
fCanvColorCB->Resize(90,20);

//---------------------------------------------------------------
//********* memory panel
//---------------------------------------------------------------

fMemFrame = fTab->AddTab("Memory");
fMemFrame->SetLayoutManager(new TGHorizontalLayout(fMemFrame));

fMemFrame->AddFrame(new TGLabel(fMemFrame, new TGHotString("Memory limit")),
		    new TGLayoutHints(kLHintsNormal,10,10,20,10));
fMemCB = new TGComboBox(fMemFrame, M_PREF_MEM);
fMemCB->Associate(this);
fMemCB->AddEntry("1 MB",1);
fMemCB->AddEntry("3 MB",2);
fMemCB->AddEntry("5 MB",3);
fMemCB->AddEntry("10 MB",4);
fMemCB->AddEntry("15 MB",5);
fMemCB->AddEntry("20 MB",6);
fMemCB->AddEntry("25 MB",7);
fMemCB->AddEntry("30 MB",8);
fMemCB->AddEntry("50 MB",9);
fMemCB->AddEntry("100 MB",10);
fMemFrame->AddFrame(fMemCB, new TGLayoutHints(kLHintsNormal,2,5,19,3));
fMemCB->Resize(90,20);

//---------------------------------------------------------------
//********* apply, set and close buttons
//---------------------------------------------------------------

fApply = new TGTextButton(fF00, "Apply", M_PREF_APPLY);
fApply->Associate(this);
fApply->SetToolTipText("Apply these settings");
fApply->Resize(50,20);
fF00->AddFrame(fApply);

fSet = new TGTextButton(fF00, "Set", M_PREF_SET);
fSet->Associate(this);
fSet->SetToolTipText("Set these settings");
fSet->Resize(50,20);
fF00->AddFrame(fSet);

fClose = new TGTextButton(fF00, "Close", M_PREF_CLOSE);
fClose->Associate(this);
fClose->SetToolTipText("Close this window");
fClose->Resize(50,20);
fF00->AddFrame(fClose);

//---------------------------------------------------------------
//********* end of definition
//---------------------------------------------------------------

readPref();

countPref++;

MapSubwindows();

SetWindowName("Preferences");

Resize(GetDefaultSize());

MapWindow();

}


//--------------------------------------------------------------------
//******  HPreferences::~HPreferences()
//
// Destructor of the class HPreferences.
//
//--------------------------------------------------------------------

HPreferences::~HPreferences() {
//
// Destructor of the class HPreferences.
//

delete fClose; delete fSet; delete fApply;
delete fMemCB;
delete fCanvColorCB;
delete fCanvHeightCB; delete fCanvWidthCB;
delete fNoMenuBarRB; delete fMenuBarRB;
delete fF3; delete fF2; delete fF1;
delete fTab;
delete fF00; delete fF0;
delete fFrame;

countPref--;

}


//---------------------------------------------------------------------
//******  HPreferences::CloseWindow()
//
// This function closes the window.
//
//---------------------------------------------------------------------

void HPreferences::CloseWindow() {
//
// This function closes the window.
//

delete this;

}


//---------------------------------------------------------------------
//******  Bool_t HPreferences::ProcessMessage(Long_t msg,Long_t parm1,
//                                                       Long_t parm2)
//
// This function processes all GUI items.
//
//---------------------------------------------------------------------

Bool_t HPreferences::ProcessMessage(Long_t msg,Long_t parm1,Long_t parm2) {
//
// This function processes all GUI widgets.
//

switch(GET_MSG(msg)) {

 case kC_COMMAND:

  switch(GET_SUBMSG(msg)) {

  case kCM_BUTTON:
    if(parm1==M_PREF_CLOSE) CloseWindow();
    if(parm1==M_PREF_APPLY) {
      if(!gROOT->GetListOfCanvases()->FindObject("maindisp")) {
	HError::message("Canvas 'maindisp' is not found.");
	break;
      }
      else applyPref();
    }
    if(parm1==M_PREF_SET) { setPref(); CloseWindow(); }
    break;

  case kCM_RADIOBUTTON:
    if(parm1==M_PREF_MENUBAR) {
      fMenuBarRB->SetState(kButtonDown);
      fNoMenuBarRB->SetState(kButtonUp);
    }
    if(parm1==M_PREF_NOMENUBAR) {
      fNoMenuBarRB->SetState(kButtonDown);
      fMenuBarRB->SetState(kButtonUp);
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
//******  HPreferences::readPref()
//
// This function reads current preferences from HOnlMainWindow.
//
//---------------------------------------------------------------------

void HPreferences::readPref() {
//
// This function reads current preferences from HOnlMainWindow.
//

  Int_t memlim[] = { 0, 1, 3, 5, 10, 15, 20, 25, 30, 50, 100 };
  Int_t dimen[] = { 0, 256, 384, 512, 640, 768, 896, 1024, 1152, 1280 };
  Int_t color[] = { 0, 10, 1, 2, 3, 4, 5, 28 };
  Int_t i = 0;
  Int_t temp = 0;
  Int_t w = 0;
  Int_t h = 0;
  Int_t col = 0;
  HOnlMainWindow *pWind;

  if(fParent && fParent->IsOnHeap()) {
    if(gOnline) {
      temp = gOnline->getMemCheck()->getMemLimit() / 1000000;
      for(i=1;i<11;i++) {
	if(memlim[i] == temp) break;
      }
      fMemCB->Select(i);
    }

    pWind = (HOnlMainWindow*) fParent;
    pWind->getCanvPref(&w,&h,&col);

    if(w<0) {
      fMenuBarRB->SetState(kButtonUp);
      fNoMenuBarRB->SetState(kButtonDown);
      w *= -1;
    }
    else {
      fMenuBarRB->SetState(kButtonDown);
      fNoMenuBarRB->SetState(kButtonUp);
    }
    for(i=1;i<10;i++) {
      if(dimen[i] == w) break;
    }
    fCanvWidthCB->Select(i);
    for(i=1;i<8;i++) {
      if(dimen[i] == h) break;
    }
    fCanvHeightCB->Select(i);
    for(i=1;i<8;i++) {
      if(color[i] == col) break;
    }
    fCanvColorCB->Select(i);
  }

}


//---------------------------------------------------------------------
//******  HPreferences::setPref()
//
// This function sets preferences.
//
//---------------------------------------------------------------------

void HPreferences::setPref() {
//
// This function sets preferences.
//

  Int_t memlim[] = { 0, 1, 3, 5, 10, 15, 20, 25, 30, 50, 100 };
  Int_t dimen[] = { 0, 256, 384, 512, 640, 768, 896, 1024, 1152, 1280 };
  Int_t color[] = { 0, 10, 1, 2, 3, 4, 5, 28 };

  HOnlMainWindow *pWind = (HOnlMainWindow*) fParent;

  if(fParent && fParent->IsOnHeap()) {
    if(gOnline) {
      gOnline->getMemCheck()->setMemLimit(memlim[fMemCB->GetSelected()]*
					  1000000);
    }

    if(fMenuBarRB->GetState()) 
      pWind->setCanvPref(dimen[fCanvWidthCB->GetSelected()],
			 dimen[fCanvHeightCB->GetSelected()],
			 color[fCanvColorCB->GetSelected()]);
    else pWind->setCanvPref(-dimen[fCanvWidthCB->GetSelected()],
			    dimen[fCanvHeightCB->GetSelected()],
			    color[fCanvColorCB->GetSelected()]);

  }

}


//---------------------------------------------------------------------
//******  HPreferences::applyPref()
//
// This function applies preferences.
//
//---------------------------------------------------------------------

void HPreferences::applyPref() {
//
// This function applies preferences.
//

  Int_t dimen[] = { 0, 256, 384, 512, 640, 768, 896, 1024, 1152, 1280 };
  Int_t color[] = { 0, 10, 1, 2, 3, 4, 5, 28 };
  TPad *padsave;

  HOnlMainWindow *pWind = (HOnlMainWindow*) fParent;

  if(fParent && fParent->IsOnHeap()) {

    if(pWind->getCanvas() && pWind->getCanvas()->IsOnHeap()) {
      padsave = (TPad*) gPad;
      pWind->getCanvas()->SetFillColor(color[fCanvColorCB->GetSelected()]);

      pWind->getCanvas()->SetWindowSize(dimen[fCanvWidthCB->GetSelected()],
					dimen[fCanvHeightCB->GetSelected()]);
      if(fMenuBarRB->GetState()) pWind->getCanvas()->GetCanvasImp()
				   ->ShowMenuBar();
      else pWind->getCanvas()->GetCanvasImp()->ShowMenuBar(kFALSE);
      pWind->getCanvas()->Modified();
      pWind->getCanvas()->Update();
      gPad = padsave;
    }
  }

}









