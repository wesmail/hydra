//*-- Author : Dmitry Vasiliev (vassili@lns.infn.it)
/* 
 * Copyright P.Finocchiaro and D.Vasiliev (INFN-LNS, Catania) HADES collaboration
*/

//-------------------------------------------------------------------------
//
// File name:       hconfigrun.cc
// Subject:         Configuration of the analysis run.
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            April 2000
//
// Remarks:         under construction
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the source code for the member functions of 
// the class HConfigRun.
// This is a GUI class for definition of the configuration for the analysis
// run.
//-------------------------------------------------------------------------

#include "hconfigrun.h"
#include "TGLabel.h"
#include "honlmainwindow.h"

//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////////
// This is a GUI class for definition of the configuration for the analysis
// run.
// Remark : this class is under construction.
//////////////////////////////////////////////////////////////////////////
ClassImp(HConfigRun)


Bool_t kColor[24];

//---------------------------------------------------------------------
//******  HConfigRun::HConfigRun(const TGWindow,UInt_t w,UInt_t h,
//                               TGMainFrame *pp)
//
// Constructor of the class HConfigRun.
//
//---------------------------------------------------------------------

HConfigRun::HConfigRun(const TGWindow *p, UInt_t w, UInt_t h, 
		       TGMainFrame *pp) : TGMainFrame(p, w, h) {
//
// Constructor of the class HConfigRun.
//

//---------------------------------------------------------------
//********* frames
//---------------------------------------------------------------

fFrame = new TGCompositeFrame(this, 800, 700, kVerticalFrame);
AddFrame(fFrame, new TGLayoutHints(kLHintsNormal));

fF1 = new TGGroupFrame(fFrame, "Input source", kVerticalFrame);
fFrame->AddFrame(fF1, new TGLayoutHints(kLHintsNormal));

fF2 = new TGGroupFrame(fFrame, "Output file", kHorizontalFrame);
fFrame->AddFrame(fF2, new TGLayoutHints(kLHintsNormal));

fF3 = new TGGroupFrame(fFrame, "Geometry", kHorizontalFrame);
fFrame->AddFrame(fF3, new TGLayoutHints(kLHintsNormal));

fF4 = new TGGroupFrame(fFrame, "Calibration", kVerticalFrame);
fFrame->AddFrame(fF4, new TGLayoutHints(kLHintsNormal));

fF5 = new TGCompositeFrame(fFrame, 800, 200, kHorizontalFrame);
fFrame->AddFrame(fF5, new TGLayoutHints(kLHintsNormal));

fF11 = new TGCompositeFrame(fF1, 800, 100, kHorizontalFrame);
fF1->AddFrame(fF11, new TGLayoutHints(kLHintsNormal));

fF12 = new TGCompositeFrame(fF1, 800, 100, kHorizontalFrame);
fF1->AddFrame(fF12, new TGLayoutHints(kLHintsNormal,0,0,5,0));

fF31 = new TGCompositeFrame(fF3, 800, 100, kVerticalFrame);
fF3->AddFrame(fF31, new TGLayoutHints(kLHintsNormal));

fF32 = new TGCompositeFrame(fF3, 800, 100, kVerticalFrame);
fF3->AddFrame(fF32, new TGLayoutHints(kLHintsNormal));

fF321 = new TGCompositeFrame(fF32, 500, 100, kHorizontalFrame);
fF32->AddFrame(fF321, new TGLayoutHints(kLHintsNormal));

fF322 = new TGCompositeFrame(fF32, 500, 100, kVerticalFrame);
fF322->SetLayoutManager(new TGMatrixLayout(fF322,0,6,12));
fF32->AddFrame(fF322, new TGLayoutHints(kLHintsNormal));

//---------------------------------------------------------------
//********* radio buttons
//---------------------------------------------------------------

fHldRB = new TGRadioButton(fF11, "hld", M_CR_HLD);
fHldRB->Associate(this);
fF11->AddFrame(fHldRB, new TGLayoutHints(kLHintsNormal,2,2,2,2));

fDaqRB = new TGRadioButton(fF11, "daq", M_CR_DAQ);
fDaqRB->Associate(this);
fF11->AddFrame(fDaqRB, new TGLayoutHints(kLHintsNormal,6,2,2,2));

fRootRB = new TGRadioButton(fF11, "root", M_CR_ROOT);
fRootRB->Associate(this);
fF11->AddFrame(fRootRB, new TGLayoutHints(kLHintsNormal,6,2,2,2));

//---------------------------------------------------------------
//********* intput source name
//---------------------------------------------------------------

fF12->AddFrame(new TGLabel(fF12, new TGHotString("name")),
	       new TGLayoutHints(kLHintsNormal,2,2,2,2));

fInput = new TGTextEntry(fF12, fInputBuf = new TGTextBuffer(200), -1);
fF12->AddFrame(fInput, new TGLayoutHints(kLHintsNormal,2,2,2,2));
fInput->Resize(248, fInput->GetDefaultHeight());

//---------------------------------------------------------------
//********* output source name
//---------------------------------------------------------------

fF2->AddFrame(new TGLabel(fF2, new TGHotString("name")),
	      new TGLayoutHints(kLHintsNormal,2,2,2,2));

fOutput = new TGTextEntry(fF2, fOutputBuf = new TGTextBuffer(200), -1);
fF2->AddFrame(fOutput, new TGLayoutHints(kLHintsNormal,2,2,2,2));
fOutput->Resize(248, fOutput->GetDefaultHeight());

//---------------------------------------------------------------
//********* geometry
//---------------------------------------------------------------

fF31->AddFrame(new TGLabel(fF31,new TGHotString("TOF")),
	       new TGLayoutHints(kLHintsNormal,2,2,35,2));
fF31->AddFrame(new TGLabel(fF31,new TGHotString("MDC")),
	       new TGLayoutHints(kLHintsNormal,2,2,17,2));
fF31->AddFrame(new TGLabel(fF31,new TGHotString("RICH")),
	       new TGLayoutHints(kLHintsNormal,2,2,17,2));
fF31->AddFrame(new TGLabel(fF31,new TGHotString("SHOWER")),
	       new TGLayoutHints(kLHintsNormal,2,2,15,2));

fF321->AddFrame(new TGLabel(fF321,new TGHotString("sec1")),
		new TGLayoutHints(kLHintsNormal,14,2,2,2));
fF321->AddFrame(new TGLabel(fF321,new TGHotString("sec2")),
		new TGLayoutHints(kLHintsNormal,6,2,2,2));
fF321->AddFrame(new TGLabel(fF321,new TGHotString("sec3")),
		new TGLayoutHints(kLHintsNormal,8,2,2,2));
fF321->AddFrame(new TGLabel(fF321,new TGHotString("sec4")),
		new TGLayoutHints(kLHintsNormal,6,2,2,2));
fF321->AddFrame(new TGLabel(fF321,new TGHotString("sec5")),
		new TGLayoutHints(kLHintsNormal,6,2,2,2));
fF321->AddFrame(new TGLabel(fF321,new TGHotString("sec6")),
		new TGLayoutHints(kLHintsNormal,6,2,2,2));

for(Int_t i=0; i<24; i++) {

  fGeomB[i] = new TGTextButton(fF322, "    ", 200+i);
  fGeomB[i]->Associate(this);
  fF322->AddFrame(fGeomB[i]);
  fGeomB[i]->Resize(30,20);
  kColor[i] = kFALSE;

}


//---------------------------------------------------------------
//********* calibration
//---------------------------------------------------------------

fF4->AddFrame(new TGLabel(fF4, new TGHotString("under construction")),
	      new TGLayoutHints(kLHintsNormal,10,172,2,2));

//---------------------------------------------------------------
//********* fF5
//---------------------------------------------------------------

fSetB = new TGTextButton(fF5, "    Set     ", M_CR_SET);
fSetB->Associate(this);
fSetB->SetToolTipText("Set configuration");
fF5->AddFrame(fSetB, new TGLayoutHints(kLHintsNormal,10,2,2,2)); 

fReadB = new TGTextButton(fF5, "   Read   ", M_CR_READ);
fReadB->Associate(this);
fReadB->SetToolTipText("Read configuration from a file");
fF5->AddFrame(fReadB, new TGLayoutHints(kLHintsNormal,5,2,2,2)); 

fWriteB = new TGTextButton(fF5, "   Write   ", M_CR_WRITE);
fWriteB->Associate(this);
fWriteB->SetToolTipText("Save configuration to a file");
fF5->AddFrame(fWriteB, new TGLayoutHints(kLHintsNormal,5,2,2,2)); 

fCancelB = new TGTextButton(fF5, " Cancel ", M_CR_CANCEL);
fCancelB->Associate(this);
fCancelB->SetToolTipText("Close window");
fF5->AddFrame(fCancelB, new TGLayoutHints(kLHintsNormal,5,2,2,2)); 

//---------------------------------------------------------------

fMainWindow = pp;

MapSubwindows();

SetWindowName("Config Run");

Resize(GetDefaultSize());
MapWindow();

}


//--------------------------------------------------------------------
//******  HConfigRun::~HConfigRun()
//
// Destructor of the class HConfigRun.
//
//--------------------------------------------------------------------

HConfigRun::~HConfigRun() {
//
// Destructor of the class HConfigRun.
//

delete fCancelB; delete fWriteB; delete fReadB; delete fSetB;
for(Int_t i=24;i--;delete fGeomB[i]);
delete fOutput; delete fInput;
delete fRootRB; delete fDaqRB; delete fHldRB;
delete fF322; delete fF321;
delete fF32; delete fF31;
delete fF12; delete fF11;
delete fF5; delete fF4; delete fF3; delete fF2; delete fF1;
delete fFrame;

fMainWindow = 0;

}


//---------------------------------------------------------------------
//******  HConfigRun::CloseWindow()
//
// This function closes the window.
//
//---------------------------------------------------------------------

void HConfigRun::CloseWindow() {
//
// This function closes the window.
//

delete this;

}


//---------------------------------------------------------------------
//******  Bool_t HConfigRun::ProcessMessage(Long_t msg, Long_t parm1,
//                                          Long_t parm2)
//
// This function processes all GUI items.
//
//---------------------------------------------------------------------

Bool_t HConfigRun::ProcessMessage(Long_t msg,Long_t parm1,Long_t parm2) {
//
// This function processes all GUI widgets.
//

return kTRUE;

}


//---------------------------------------------------------------------
//******  Bool_t HConfigRun::toGreen(Window_t id)
//
// This function changes the button color to green.
//
//---------------------------------------------------------------------

Bool_t HConfigRun::toGreen(Window_t id) {
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
//******  Bool_t HConfigRun::toDefault(Window_t id)
//
// This function changes the button color to default (gray).
//
//---------------------------------------------------------------------

Bool_t HConfigRun::toDefault(Window_t id) {
//
// This function changes the button color to default (gray).
//

  SetWindowAttributes_t wattr;
  wattr.fMask = kWABackPixel;
  wattr.fBackgroundPixel = fgDefaultFrameBackground;
  gVirtualX->ChangeWindowAttributes(id, &wattr);
  return kTRUE;

}







