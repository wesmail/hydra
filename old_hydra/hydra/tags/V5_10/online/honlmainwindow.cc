//-------------------------------------------------------------------------
//
// File name:       honlmainwindow.cc
// Subject:         Main GUI panel.
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            March 2000
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the source code for the member functions of 
// the class HOnlMainWindow.
// HOnlMainWindow is a GUI class which maintains the main panel of 
// the program HTOP.
//
//-------------------------------------------------------------------------

#include "honlmainwindow.h"
#include <TGLabel.h>
#include "honlobjbrowser.h"
#include "hhistovisual.h"
#include "hades.h"
#include <stdlib.h>
#include <TSystem.h>
#include "htofhistaction.h"
#include "honlinecontrol.h"
#include "hmemorycheck.h"
#include <TGMsgBox.h>
#include <TGFileDialog.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <TList.h>
#include "herror.h"
#include "hpreferences.h"
#include "hslice.h"
#include "hcontourmanager.h"
#include "hhistocont.h"
#include "hhistogram.h"

#include <iostream.h>

Int_t HSlice::countSlice;
Int_t HPreferences::countPref;
Int_t HHistoVisual::countVisual;
Int_t HContourManager::countContour;
Int_t HOnlObjBrowser::countBrowser;

ClassImp(HOnlMainWindow)

//---------------------------------------------------------------------
//******  HOnlMainWindow::HOnlMainWindow(const TGWindow,UInt_t w,UInt_t h)
//
// Constructor of the class HOnlMainWindow.
//
//---------------------------------------------------------------------

HOnlMainWindow::HOnlMainWindow(const TGWindow *p, UInt_t w, UInt_t h) :
  TGMainFrame(p, w, h) {

//---------------------------------------------------------------
//********* frames and popup menus
//---------------------------------------------------------------

fFrame = new TGCompositeFrame(this, 800, 700, kVerticalFrame);
AddFrame(fFrame, new TGLayoutHints(kLHintsNormal));

fMenuFile = new TGPopupMenu(fClient->GetRoot());
fMenuFile->AddEntry("Config run", M__CONFIGRUN);
fMenuFile->AddSeparator();
fMenuFile->AddEntry("Save...",M__OBJSAVE);
fMenuFile->AddEntry("Save as",M__OBJSAVEAS);
fMenuFile->AddSeparator();
fMenuFile->AddEntry("Exit", M__EXIT);

fMenuHisto = new TGPopupMenu(fClient->GetRoot());
fMenuHisto->AddEntry("Object manager", M__OBJMANAGER);
fMenuHisto->AddSeparator();
fMenuHisto->AddEntry("Contour manager", M__CONTMANAGER);
fMenuHisto->AddSeparator();
fMenuHisto->AddEntry("Histogram display",M__HISTOVISUAL);

fMenuTools = new TGPopupMenu(fClient->GetRoot());
fMenuTools->AddEntry("Open display window", M__CANVAS);
fMenuTools->AddSeparator();
fMenuTools->AddEntry("Slice 2-dim histogram", M__SLICE);
fMenuTools->AddSeparator();
fMenuTools->AddEntry("Histogram browser", M__HISTACTION);

fMenuOptions = new TGPopupMenu(fClient->GetRoot());
fMenuOptions->AddEntry("Preferences", M__PREF);

fMenuHelp = new TGPopupMenu(fClient->GetRoot());
fMenuHelp->AddEntry("About this program", M__HELP);

fMenuFile->Associate(this);
fMenuHisto->Associate(this);
fMenuTools->Associate(this);
fMenuOptions->Associate(this);
fMenuHelp->Associate(this);

fMenuBar = new TGMenuBar(fFrame,1,1,kHorizontalFrame);
fMenuBar->AddPopup("File",fMenuFile,new TGLayoutHints(kLHintsNormal,2,2,2,2));
fMenuBar->AddPopup("Histogram",fMenuHisto,
		   new TGLayoutHints(kLHintsNormal,2,2,2,2));
fMenuBar->AddPopup("Tools",fMenuTools,
		   new TGLayoutHints(kLHintsNormal,2,2,2,2));
fMenuBar->AddPopup("Options",fMenuOptions,
		   new TGLayoutHints(kLHintsNormal,2,2,2,2));
fMenuBar->AddPopup("Help",fMenuHelp,new TGLayoutHints(kLHintsNormal,2,2,2,2));
fFrame->AddFrame(fMenuBar, new TGLayoutHints(kLHintsNormal));

fF1 = new TGCompositeFrame(fFrame, 700, 100, kHorizontalFrame);
fFrame->AddFrame(fF1, new TGLayoutHints(kLHintsNormal,0,0,10,0));

//---------------------------------------------------------------
//********* fF1 subframe
//---------------------------------------------------------------

fF1->AddFrame(new TGLabel(fF1, new TGHotString("Process events")),
	      new TGLayoutHints(kLHintsNormal,10,5,5,5));

fEvents = new TGTextEntry(fF1, fEventsBuf = new TGTextBuffer(100), -1);
fEventsBuf->AddText(0,"100"); 
fEvents->Associate(this);
fF1->AddFrame(fEvents, new TGLayoutHints(kLHintsNormal,5,5,3,3));
fEvents->Resize(90,fEvents->GetDefaultHeight());

fF1->AddFrame(new TGLabel(fF1, new TGHotString("Event loop")),
	      new TGLayoutHints(kLHintsNormal,10,5,5,5));

fEvLoop = new TGTextButton(fF1, "   Start   ", M__RUN);
fEvLoop->Associate(this);
fEvLoop->SetToolTipText("Click me to start event loop");
fF1->AddFrame(fEvLoop, new TGLayoutHints(kLHintsNormal, 5,5,3,3));

fVisual = 0;
fBrowser = 0;
fHistAction = 0;
fCanvW = -640;
fCanvH = 640;
fCanvCol = 10;

//------- static members ---------------------------------------

HSlice::countSlice = 0;
HPreferences::countPref = 0;
HHistoVisual::countVisual = 0;
HContourManager::countContour = 0;
HOnlObjBrowser::countBrowser = 0;

//--------------------------------------------------------------

fArray = new TObjArray(500);
fXArray.Set(500);
fYArray.Set(500);
fSliceMode = 3; // 2dim plot

//--------------------------------------------------------------

MapSubwindows();

SetWindowName("Online");

Resize(GetDefaultSize());
MapWindow();

}


//--------------------------------------------------------------------
//******  HOnlMainWindow::~HOnlMainWindow()
//
// Destructor of the class HOnlMainwindow.
//
//--------------------------------------------------------------------

HOnlMainWindow::~HOnlMainWindow() {

if(fCanvas && fCanvas->IsOnHeap()) delete fCanvas;
delete fEvLoop;
delete fEvents;
delete fF1;
delete fMenuBar;
delete fMenuHelp; delete fMenuOptions; delete fMenuTools; 
delete fMenuHisto; delete fMenuFile;
delete fFrame;
if(fArray) delete fArray;

}


//---------------------------------------------------------------------
//******  HOnlMainWindow::CloseWindow()
//
// This function closes the window.
//
//---------------------------------------------------------------------

void HOnlMainWindow::CloseWindow() {

delete this;

}


//---------------------------------------------------------------------
//******  Bool_t HOnlMainWindow::ProcessMessage(Long_t msg, Long_t parm1,
//                                              Long_t parm2)
//
// This function processes all GUI items.
//
//---------------------------------------------------------------------

Bool_t HOnlMainWindow::ProcessMessage(Long_t msg,Long_t parm1,Long_t parm2) {

Int_t retval = 0;
const char *filetypes[] = { "All files",     "*",
			    "ROOT files",    "*.root",
			    "ROOT macros",   "*.C",
			    0,               0 };
TGFileInfo fi;
fi.fFileTypes = (char **)filetypes;  
char filename[200];

switch(GET_MSG(msg)) {
 case kC_COMMAND:

  switch(GET_SUBMSG(msg)) {

  case kCM_BUTTON:
    if(parm1==M__RUN) {
      if(!gHades) break;
      ((TGTextButton*)fEvLoop)->SetText(new TGHotString("Running"));
      toGreen(fEvLoop->GetId());
      fClient->NeedRedraw(fEvLoop);
      gSystem->AddTimer(new HOnlTimer(this,1000));
    }
    break;

  case kCM_MENU:

    switch(parm1) {

    case M__CONFIGRUN:
      coffeebreak();
      break;

    case M__OBJMANAGER:
      if(HOnlObjBrowser::countBrowser) {
	HError::message("The requested panel is already open.");
	break;
      }
      fBrowser = new HOnlObjBrowser(fClient->GetRoot(),1,1);
      break;

    case M__CONTMANAGER:
      if(HContourManager::countContour) {
	HError::message("The requested panel is already open.");
	break;
      }
      fContManager = new HContourManager(fClient->GetRoot(),1,1,this);
      break;

    case M__OBJSAVE:
      retval = HError::warning("All online objects will be saved to 
the file 'honline.root' and removed from the memory. 
So, HTOP will exit after that."); 
      if(retval == 1) {
	if(gOnline) {
	  gOnline->setSaveFile("honline.root");
	  delete gOnline;
	}
	/*	fMenuFile->DisableEntry(M__CONFIGRUN);
	fMenuFile->DisableEntry(M__OBJSAVE);
	fMenuFile->DisableEntry(M__OBJSAVEAS);
	fMenuHisto->DisableEntry(M__OBJMANAGER);
	fMenuHisto->DisableEntry(M__HISTOVISUAL);
	fMenuHisto->DisableEntry(M__CONTMANAGER);*/
	SendMessage(this,MK_MSG(kC_COMMAND,kCM_MENU),M__EXIT,0);
      }
      break;

    case M__OBJSAVEAS:
      new TGFileDialog(fClient->GetRoot(), this, kFDSave, &fi);
      sprintf(filename,"%s",fi.fFilename);
      if(!strcmp(filename,"(null)")) break;
      if(gOnline) gOnline->setSaveFile(fi.fFilename);
      retval = HError::warning("All online objects will be saved to 
the file on the disk and removed from the memory. 
So, HTOP will exit after that."); 
      if(retval == 1) {
	if(gOnline) delete gOnline;
	/*	fMenuFile->DisableEntry(M__CONFIGRUN);
	fMenuFile->DisableEntry(M__OBJSAVE);
	fMenuFile->DisableEntry(M__OBJSAVEAS);
	fMenuHisto->DisableEntry(M__OBJMANAGER);
	fMenuHisto->DisableEntry(M__HISTOVISUAL);
	fMenuHisto->DisableEntry(M__CONTMANAGER);*/
	SendMessage(this,MK_MSG(kC_COMMAND,kCM_MENU),M__EXIT,0);
      }
      break;

    case M__EXIT:
      //it is necessary to close first the browser and then display
      //window; the order is important because there are cross correlations
      //between the windows

      if(fBrowser && HOnlObjBrowser::countBrowser) delete fBrowser;
      if(fVisual && HHistoVisual::countVisual) delete fVisual;
      if(fContManager && HContourManager::countContour) delete fContManager;
      if(gHades) delete gHades;
      CloseWindow();
      break;

    case M__HISTOVISUAL:
      if(HHistoVisual::countVisual) {
	HError::message("The requested panel is already open.");
	break;
      }
      fVisual = new HHistoVisual(fClient->GetRoot(),1,1,this);
      break;
      
    case M__HISTACTION:
      fHistAction = new HTofHistAction(fClient->GetRoot(),1,1);
      break;

    case M__HELP:
      new TGMsgBox(fClient->GetRoot(),this,"This Project",
		   "HADES Time-of-flight Online Project (HTOP)
for the HADES collaboration

Paolo Finocchiaro (INFN-LNS, Catania)  finocchiaro@lns.infn.it
Dmitry Vasiliev (INFN-LNS, Catania)  vassili@lns.infn.it\n",0,kMBClose,
		   &retval);
      break;

    case M__CANVAS:
      createCanvas();
      break;

    case M__PREF:
      if(HPreferences::countPref) {
	HError::message("The requested panel is already open.");
	break;
      }
      new HPreferences(fClient->GetRoot(),1,1,this);
      break;

    case M__SLICE:
      if(HSlice::countSlice) {
	HError::message("The requested panel is already open.");
	break;
      }
      new HSlice(fClient->GetRoot(),1,1,fArray,&fXArray,&fYArray,&fSliceMode);
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
//******  Bool_t HOnlMainWindow::toGreen(Window_t id)
//
// This function changes button color to green.
//
//---------------------------------------------------------------------

Bool_t HOnlMainWindow::toGreen(Window_t id) {

  SetWindowAttributes_t wattr;
  wattr.fMask = kWABackPixel;
  gClient->GetColorByName("green", wattr.fBackgroundPixel);
  gVirtualX->ChangeWindowAttributes(id, &wattr);
  return kTRUE;

}


//---------------------------------------------------------------------
//******  Bool_t HOnlMainWindow::toRed(Window_t id)
//
// This function changes button color to red.
//
//---------------------------------------------------------------------

Bool_t HOnlMainWindow::toRed(Window_t id) {

  SetWindowAttributes_t wattr;
  wattr.fMask = kWABackPixel;
  gClient->GetColorByName("red", wattr.fBackgroundPixel);
  gVirtualX->ChangeWindowAttributes(id, &wattr);
  return kTRUE;

}


//---------------------------------------------------------------------
//******  Bool_t HOnlMainWindow::toDefault(Window_t id)
//
// This function changes button color to default one (gray).
//
//---------------------------------------------------------------------

Bool_t HOnlMainWindow::toDefault(Window_t id) {

  SetWindowAttributes_t wattr;
  wattr.fMask = kWABackPixel;
  wattr.fBackgroundPixel = fgDefaultFrameBackground;
  gVirtualX->ChangeWindowAttributes(id, &wattr);
  return kTRUE;

}


//---------------------------------------------------------------------
//******  Bool_t HOnlMainWindow::HandleTimer(TTimer *)
//
// This function starts event loop.
//
//---------------------------------------------------------------------

Bool_t HOnlMainWindow::HandleTimer(TTimer *) {

  gHades->eventLoop(atoi(fEventsBuf->GetString()));
  toDefault(fEvLoop->GetId());
  ((TGTextButton*)fEvLoop)->SetText(new TGHotString("Start"));
  fClient->NeedRedraw(fEvLoop);
  if(gPad) {
    gPad->Modified();
    gPad->Update();
  }
  return kTRUE;

}
  
  
//---------------------------------------------------------------------
//******  void HOnlMainWindow::coffeebreak()
//
// Produces a message when the called panel is under construction.
//
//---------------------------------------------------------------------

void HOnlMainWindow::coffeebreak() {

  Int_t retval;
  new TGMsgBox(fClient->GetRoot(),this,"Coffee break",
	       "\nThe panel you requested is taking a coffee break.\n",0,
	       kMBClose,&retval);

}


//---------------------------------------------------------------------
//******  void HOnlMainWindow::getCanvPref(Int_t *p1, Int_t *p2, Int_t *p3)
//
// Returns canvas preferencies.
//
//---------------------------------------------------------------------

void HOnlMainWindow::getCanvPref(Int_t *p1, Int_t *p2, Int_t *p3) {

  if(p1) *p1 = fCanvW;
  if(p2) *p2 = fCanvH;
  if(p3) *p3 = fCanvCol;

}


//---------------------------------------------------------------------
//******  void HOnlMainWindow::setCanvPref(Int_t w, Int_t h, Int_t c)
//
// Sets canvas preferencies.
//
//---------------------------------------------------------------------

void HOnlMainWindow::setCanvPref(Int_t w, Int_t h, Int_t c) {

  fCanvW = w;
  fCanvH = h;
  fCanvCol = c;

}


//---------------------------------------------------------------------
//******  void HOnlMainWindow::createCanvas()
//
// Opens a canvas.
//
//---------------------------------------------------------------------

void HOnlMainWindow::createCanvas() {

  Int_t retval = 0;
  fCanvas = (TCanvas*) gROOT->GetListOfCanvases()->FindObject("maindisp");
  if(fCanvas) {
    retval = HError::warning("There is already a canvas on the heap, 
which will be destroyed.");
    if(retval == 1) { delete fCanvas; fCanvas = 0; }
    else return;
  }

  gStyle->SetScreenFactor();
  Int_t w = fCanvW;
  Int_t h = fCanvH;

  fCanvas = new TCanvas("maindisp","maindisp",w,h);
  fCanvas->SetFillColor(fCanvCol);
  fCanvas->Modified();
  fCanvas->Update();

}


//---------------------------------------------------------------------
//******  void HOnlMainWindow::updateCursorList()
//
// This function scans histogram container and creates list of cursors
// for all found 2dim histogrammes. This serves for the panel slice 2d.
//
//---------------------------------------------------------------------

void HOnlMainWindow::updateCursorList() {

  if(!gOnline) return;
  Int_t i;
  HHistogram *pHisto;
  Int_t x,y;
  Int_t idx;

  for(i=0; i<500; i++) {

    if(fArray->At(i) && fXArray[i] > 0 && fYArray[i] > 0) {
      
      idx = gOnline->getHisto()->find(((TH2F*)fArray->At(i))->GetName());
      pHisto = gOnline->getHisto()->at(idx);
      if(pHisto) {
	pHisto->setCursors(fXArray[i],fYArray[i]);
      }
    }
  }

  fArray->Clear();
  for(i=0;i<500;i++) {
    fXArray[i] = 0;
    fYArray[i] = 0;
  }

  for(i=0; i<gOnline->getHisto()->getEntries(); i++) {

    pHisto = gOnline->getHisto()->at(i);
    if(pHisto) {
      if(pHisto->getDim() == 2) {
	pHisto->getCursors(x,y);
	fArray->AddAt(pHisto->getHist2(),i);
	fXArray[i] = x;
	fYArray[i] = y;
      }
    }
  }

}




