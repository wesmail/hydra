#include "htofeventdisplay.h"
#include "htofconfigpanel.h"
#include "htofmonitor.h"
#include "hades.h"
#include "hspectrometer.h"
#include "hruntimedb.h"
#include "htofdetector.h"
#include "htofhitf.h"
#include "htofcalibrater.h"
#include "htask.h"
#include "htaskset.h"
#include "hrootsource.h"
#include "hldfilesource.h"
#include "hldremotesource.h"
#include "htofunpacker.h"
#include "htofrod.h"
#include "htofgeom.h"
#include "htofhistaction.h"
#include <stdlib.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TGLabel.h>
#include <TGFileDialog.h>
#include <TGMsgBox.h>
#include <TSystem.h>

//----------------------------------------------------------
//
//Author : Dmitry Vasiliev (LNS, Catania)
//
//----------------------------------------------------------
//----------------------------------------------------------
//
// HTofEventDisplay class
// it is an interface between a user and the TOF part of HYDRA
//
// contains GUI elements, histogram container, configuration
//
//------------------------------------------------------------

ClassImp(HTofEventDisplay)
ClassImp(HTofHisto)

TCanvas *canvA;
Int_t listbox[3]; // array of selected items in ListBoxes
Int_t events; // number of events to be processed

HTofHisto::HTofHisto(Int_t sec, Int_t mod, Int_t rod) : TObject() {

  char histoname[100];
  char histotitle[100];
  Int_t chan;
  Float_t xmin;
  Float_t xmax;

  //left time

  sprintf(histoname,"%s%i%s%i%s%i","left_",sec+1,"_",mod+1,"_",rod+1);
  sprintf(histotitle,"%s","TOF raw data");
  chan = 1024;
  xmin = 0.;
  xmax = 4096.;
  left = new TH1F(histoname,histotitle,chan,xmin,xmax);
  left->GetXaxis()->SetTitle("left time [ch]");

  //right time

  sprintf(histoname,"%s%i%s%i%s%i","right_",sec+1,"_",mod+1,"_",rod+1);
  sprintf(histotitle,"%s","TOF raw data");
  chan = 1024;
  xmin = 0.;
  xmax = 4096.;
  right = new TH1F(histoname,histotitle,chan,xmin,xmax);
  right->GetXaxis()->SetTitle("right time [ch]");

  //half sum

  sprintf(histoname,"%s%i%s%i%s%i","halfsum_",sec+1,"_",mod+1,"_",rod+1);
  sprintf(histotitle,"%s","TOF raw data");
  chan = 1024;
  xmin = 0.;
  xmax = 4096.;
  halfs = new TH1F(histoname,histotitle,chan,xmin,xmax);
  halfs->GetXaxis()->SetTitle("halfsum [ch]");

  //half diff

  sprintf(histoname,"%s%i%s%i%s%i","halfdif_",sec+1,"_",mod+1,"_",rod+1);
  sprintf(histotitle,"%s","TOF raw data");
  chan = 1024;
  xmin = -2048.;
  xmax = 2048.;
  halfd = new TH1F(histoname,histotitle,chan,xmin,xmax);
  halfd->GetXaxis()->SetTitle("halfdif time [ch]");

  //tof

  sprintf(histoname,"%s%i%s%i%s%i","tof_",sec+1,"_",mod+1,"_",rod+1);
  sprintf(histotitle,"%s","TOF hits");
  chan = 200;
  xmin = -20.;
  xmax = 40.;
  tof = new TH1F(histoname,histotitle,chan,xmin,xmax);
  tof->GetXaxis()->SetTitle("tof [ns]");

  //x position

  sprintf(histoname,"%s%i%s%i%s%i","x_",sec+1,"_",mod+1,"_",rod+1);
  sprintf(histotitle,"%s","TOF hits");
  chan = 200;
  xmin = -150.;
  xmax = 150.;
  xpos = new TH1F(histoname,histotitle,chan,xmin,xmax);
  xpos->GetXaxis()->SetTitle("x [cm]");

  /*  //left vs. right
  sprintf(histoname,"%s%i%s%i%s%i","left vs. right_",
	  sec+1,"_",mod+1,"_",rod+1);
  sprintf(histotitle,"%s","TOF data");
  lr = new TH2F(histoname,histotitle,200,0.,4090.,200,0.,4090.);

  //left vs. tof
  sprintf(histoname,"%s%i%s%i%s%i","left vs. tof_",
	  sec+1,"_",mod+1,"_",rod+1);
  sprintf(histotitle,"%s","TOF data");
  lt = new TH2F(histoname,histotitle,200,0.,20.,200,0.,4090.);

  //left vs. xpos
  sprintf(histoname,"%s%i%s%i%s%i","left vs. xpos_",
	  sec+1,"_",mod+1,"_",rod+1);
  sprintf(histotitle,"%s","TOF data");
  lx = new TH2F(histoname,histotitle,200,-150.,150.,200,0.,4090.);

  //right vs. tof
  sprintf(histoname,"%s%i%s%i%s%i","right vs. tof_",
	  sec+1,"_",mod+1,"_",rod+1);
  sprintf(histotitle,"%s","TOF data");
  rt = new TH2F(histoname,histotitle,200,0.,20.,200,0.,4090.);

  //right vs. xpos
  sprintf(histoname,"%s%i%s%i%s%i","right vs. xpos_",
	  sec+1,"_",mod+1,"_",rod+1);
  sprintf(histotitle,"%s","TOF data");
  rx = new TH2F(histoname,histotitle,200,-150.,150.,200,0.,4090.);

  //tof vs. xpos
  sprintf(histoname,"%s%i%s%i%s%i","tof vs. xpos_",
	  sec+1,"_",mod+1,"_",rod+1);
  sprintf(histotitle,"%s","TOF data");
  tx = new TH2F(histoname,histotitle,200,-150.,150.,200,0.,20.);*/

}


void HTofHisto::reset() {

  if(left) left->Reset();
  if(right) right->Reset();
  if(tof) tof->Reset();
  if(xpos) xpos->Reset();
  if(halfs) halfs->Reset();
  if(halfd) halfd->Reset();
  //  if(lr) lr->Reset();
  //  if(lt) lt->Reset();
  //  if(lx) lx->Reset();
  //  if(rt) rt->Reset();
  //  if(rx) rx->Reset();
  //  if(tx) tx->Reset();

}



Bool_t HTofDisplayTimer::Notify() {

  fTimer->HandleTimer(0);
  TurnOff();
  return kFALSE;

}



HTofEventDisplay::HTofEventDisplay(const TGWindow *p, UInt_t w, UInt_t h) : 
                                   TGMainFrame(p,w,h) {

char tmp[20];
Int_t i;

fFrame = new TGCompositeFrame(this, 700, 700, kHorizontalFrame);
AddFrame(fFrame, new TGLayoutHints(kLHintsNormal));


fF1 = new TGCompositeFrame(fFrame, 500, 700, kVerticalFrame);
fFrame->AddFrame(fF1, new TGLayoutHints(kLHintsNormal, 2, 2, 2, 2));


fF2 = new TGCompositeFrame(fFrame, 200, 700, kVerticalFrame);
fFrame->AddFrame(fF2, new TGLayoutHints(kLHintsNormal, 2, 2, 2, 2));

fC1 = new TGGroupFrame(fF2, "control buttons", kHorizontalFrame);
fF2->AddFrame(fC1, new TGLayoutHints(kLHintsNormal));

fC4 = new TGGroupFrame(fF2, "histogrammes", kHorizontalFrame);
fF2->AddFrame(fC4, new TGLayoutHints(kLHintsNormal));


fA1 = new TGGroupFrame(fF2, "select", kVerticalFrame);
fF2->AddFrame(fA1, new TGLayoutHints(kLHintsNormal));

fA2 = new TGCompositeFrame(fA1, 100, 100, kHorizontalFrame);
fA1->AddFrame(fA2, new TGLayoutHints(kLHintsNormal));

fA3 = new TGCompositeFrame(fA1, 100, 100, kHorizontalFrame);
fA1->AddFrame(fA3, new TGLayoutHints(kLHintsNormal));

fC2 = new TGGroupFrame(fF2, "event loop", kVerticalFrame);
fF2->AddFrame(fC2, new TGLayoutHints(kLHintsNormal));

fC3 = new TGGroupFrame(fF2, "color scaling", kVerticalFrame);
fF2->AddFrame(fC3, new TGLayoutHints(kLHintsNormal));

 
fCanvas = new TRootEmbeddedCanvas("main_canvas",fF1,545,545,kFixedSize);
fF1->AddFrame(fCanvas, new TGLayoutHints(kLHintsNormal));
canvA = fCanvas->GetCanvas();
canvA->SetFillColor(1);
canvA->cd();


fB1 = new TGCompositeFrame(fA2, 130, 100, kVerticalFrame);
fA2->AddFrame(fB1, new TGLayoutHints(kLHintsNormal));

fB2 = new TGCompositeFrame(fA2, 130, 100, kVerticalFrame);
fA2->AddFrame(fB2, new TGLayoutHints(kLHintsNormal));

fB3 = new TGCompositeFrame(fA2, 130, 100, kVerticalFrame);
fA2->AddFrame(fB3, new TGLayoutHints(kLHintsNormal));


fB1->AddFrame(new TGLabel(fB1, new TGHotString("Sector")),
	      new TGLayoutHints(kLHintsLeft|kLHintsTop,10,5,5,0));

fListSec = new TGListBox(fB1,M_LIST_SEC);
fListSec->Associate(this);
for(i=0;i<6;i++) {
  sprintf(tmp,"%i",i+1);
  fListSec->AddEntry(tmp,i+1);
}
fB1->AddFrame(fListSec, new TGLayoutHints(kLHintsLeft|kLHintsTop,10,5,5,0));
fListSec->Resize(42,140);


fB2->AddFrame(new TGLabel(fB2, new TGHotString("Module")),
	      new TGLayoutHints(kLHintsLeft|kLHintsTop,10,5,5,0));

fListMod = new TGListBox(fB2,M_LIST_MOD);
fListMod->Associate(this);
for(i=0;i<8;i++) {
  sprintf(tmp,"%i",i+1);
  fListMod->AddEntry(tmp,i+1);
}
fB2->AddFrame(fListMod, new TGLayoutHints(kLHintsLeft|kLHintsTop,10,5,5,0));
fListMod->Resize(42,140);


fB3->AddFrame(new TGLabel(fB3, new TGHotString("Rod")),
	      new TGLayoutHints(kLHintsLeft|kLHintsTop,10,5,5,0));

fListRod = new TGListBox(fB3,M_LIST_ROD);
fListRod->Associate(this);
for(i=0;i<8;i++) {
  sprintf(tmp,"%i",i+1);
  fListRod->AddEntry(tmp,i+1);
}
fB3->AddFrame(fListRod, new TGLayoutHints(kLHintsLeft|kLHintsTop,10,5,5,0));
fListRod->Resize(42,140);

fClear = new TGTextButton(fA3, "  Clear  ", M_CLEAR);
fClear->Associate(this);
fClear->SetToolTipText("Clear list boxes");
fA3->AddFrame(fClear, new TGLayoutHints(kLHintsNormal,30,0,10,0));

fZoom = new TGTextButton(fA3, "  Zoom  ", M_ZOOM);
fZoom->Associate(this);
fZoom->SetToolTipText("Make a zoom");
fZoom->AllowStayDown(kTRUE);
fA3->AddFrame(fZoom, new TGLayoutHints(kLHintsNormal,10,0,10,0));
kZOOM = kFALSE;


fConfig = new TGTextButton(fC1, "C&onfig", M_CONFIG);
fConfig->Associate(this);
fConfig->SetToolTipText("Open config window");
fC1->AddFrame(fConfig, new TGLayoutHints(kLHintsNormal,10,5,5,0));

fPrint = new TGTextButton(fC1, " &Print  ", M_PRINT);
fPrint->Associate(this);
fPrint->SetToolTipText("Save picture");
fC1->AddFrame(fPrint, new TGLayoutHints(kLHintsNormal,8,5,5,0));

fClose = new TGTextButton(fC1, "&Close ", M_CLOSE_MAIN);
fClose->Associate(this);
fClose->SetToolTipText("Close window");
fC1->AddFrame(fClose, new TGLayoutHints(kLHintsNormal,8,1,5,0));


fRaw = new TGTextButton(fC4, "  Raw  ", M_RAW);
fRaw->Associate(this);
fRaw->SetToolTipText("Raw histogrammes for a selected sector/module/rod");
fC4->AddFrame(fRaw, new TGLayoutHints(kLHintsNormal,10,5,5,0));

fHit = new TGTextButton(fC4, "  Hit   ", M_HIT);
fHit->Associate(this);
fHit->SetToolTipText("Hit histogrammes for a selected sector/module/rod");
fC4->AddFrame(fHit, new TGLayoutHints(kLHintsNormal,6,5,5,0));

fMultih = new TGTextButton(fC4, " Multi ", M_MULTIH);
fMultih->Associate(this);
fMultih->SetToolTipText("Multihistogrammes");
fC4->AddFrame(fMultih, new TGLayoutHints(kLHintsNormal,6,5,5,0));


fD1 = new TGCompositeFrame(fC2, 100, 100, kHorizontalFrame);
fC2->AddFrame(fD1, new TGLayoutHints(kLHintsNormal, 2,2,0,0));

fD2 = new TGCompositeFrame(fC2, 100, 100, kHorizontalFrame);
fC2->AddFrame(fD2, new TGLayoutHints(kLHintsNormal, 2,2,0,0));

fD3 = new TGCompositeFrame(fC2, 100, 100, kHorizontalFrame);
fC2->AddFrame(fD3, new TGLayoutHints(kLHintsNormal, 2,2,0,0));


fD1->AddFrame(new TGLabel(fD1, new TGHotString("Set events")),
	      new TGLayoutHints(kLHintsNormal,20,0,4,2));
fSetEv = new TGTextEntry(fD1, fSetEvBuf = new TGTextBuffer(100), -1); 
fD1->AddFrame(fSetEv, new TGLayoutHints(kLHintsNormal,15,0,0,0));
fSetEv->Resize(65, fSetEv->GetDefaultHeight());


fStart = new TGTextButton(fD2, "   &Start   ", M_START);
fStart->Associate(this);
fStart->SetToolTipText("Start event loop");
fD2->AddFrame(fStart, new TGLayoutHints(kLHintsNormal,27,0,10,10));

fReset = new TGTextButton(fD2, "   &Reset  ", M_RESET);
fReset->Associate(this);
fReset->SetToolTipText("Reset content of histogrammes");
fD2->AddFrame(fReset, new TGLayoutHints(kLHintsNormal,10,15,10,10));


fD3->AddFrame(new TGLabel(fD3, new TGHotString("Event loop status")),
	      new TGLayoutHints(kLHintsNormal,0,0,3,0));
fLoop = new TGTextButton(fD3, "      ", -1);
toRed(fLoop->GetId());
fD3->AddFrame(fLoop, new TGLayoutHints(kLHintsNormal,15,5,1,0));


fE1 = new TGCompositeFrame(fC3, 100, 100, kHorizontalFrame);
fC3->AddFrame(fE1, new TGLayoutHints(kLHintsNormal));

fE1->AddFrame(new TGLabel(fE1, new TGHotString("Set scale")),
	      new TGLayoutHints(kLHintsNormal,25,0,5,0));
fScText = new TGTextEntry(fE1, fScBuf = new TGTextBuffer(100), -1); 
fE1->AddFrame(fScText, new TGLayoutHints(kLHintsNormal,15,15,1,0));
fScText->Resize(65, fScText->GetDefaultHeight());

fE2 = new TGCompositeFrame(fC3, 100, 100, kHorizontalFrame);
fC3->AddFrame(fE2, new TGLayoutHints(kLHintsNormal));

fDrawSc = new TGTextButton(fE2, "  &Draw  ", M_DRAW_SC);
fDrawSc->Associate(this);
fDrawSc->SetToolTipText("Draw with automatic color scaling");
fE2->AddFrame(fDrawSc, new TGLayoutHints(kLHintsNormal,25,0,9,0));

fScale = new TGTextButton(fE2, "  Scale  ", M_SCALE);
fScale->Associate(this);
fScale->SetToolTipText("Draw with user defined color scaling");
fE2->AddFrame(fScale, new TGLayoutHints(kLHintsNormal,10,0,9,0));

//-----------------------------------------------------------

gTofDisplay = this;
fTofGeom = new HTofGeom;
fHistoCont = 0;
kSector = kFALSE;
kModule = kFALSE;
kRod = kFALSE;
kConfigured = kFALSE;
for(i=0;i<3;i++) listbox[i]=0;
if(readDefaultConfig()) setConfiguration();

MapSubwindows();

SetWindowName("TOF Event Display");

Resize(GetDefaultSize());

MapWindow();

}


HTofEventDisplay::~HTofEventDisplay() {

  delete fScale;
  delete fDrawSc;
  delete fScText;
  delete fLoop;
  delete fStart;
  delete fReset;
  delete fSetEv;
  delete fE2; delete fE1;
  delete fD3; delete fD2; delete fD1;
  delete fClose;
  delete fHit;
  delete fRaw;
  delete fMultih;
  delete fConfig;
  delete fPrint;
  delete fC4; delete fC3; delete fC2; delete fC1;
  delete fCanvas;
  delete fClear;
  delete fZoom;
  delete fListRod;
  delete fListMod;
  delete fListSec;
  delete fB3; delete fB2; delete fB1;
  delete fA3; delete fA2; delete fA1;
  delete fF2; delete fF1;
  delete fFrame;

}


void HTofEventDisplay::CloseWindow() {

  delete this;

}


Bool_t HTofEventDisplay::ProcessMessage(Long_t msg, Long_t parm1, 
					Long_t parm2) {

Int_t j;
Int_t index;
Int_t scale;
Int_t retval;
Int_t buttons;
Color_t color = 10;
HTofHisto *pTofHisto;
char tmp[10];
TGFileInfo fi; // to save a picture
char savefilename[100];
const char *filetypes[] = { "Postscript files",     "*.ps",
                            "Gif files",    "*.gif",
                            0,               0 };
fi.fFileTypes = (char **)filetypes;

 
 switch(GET_MSG(msg)) {

 case kC_COMMAND:

   switch(GET_SUBMSG(msg)) {

   case kCM_BUTTON:
   
     switch(parm1) {

     case M_CLOSE_MAIN:
       retval = 0;
       buttons = 0;
       buttons |= kMBYes;
       buttons |= kMBNo;
       new TGMsgBox(fClient->GetRoot(), this, "Confirm action",
		    "Close Panel 'TOF Event Display' ?",
		    kMBIconQuestion,buttons,&retval);
       if(retval == 1) {
	 if(gHades) delete gHades;
	 CloseWindow();
       }
       break;

     case M_PRINT:
       new TGFileDialog(fClient->GetRoot(), this, kFDSave, &fi);
       sprintf(savefilename,"%s",fi.fFilename);
       if(!strcmp(savefilename,"(null)")) break;
       sprintf(savefilename,"%s%s%s",
	       gSystem->WorkingDirectory(),"/",fi.fFilename);
       canvA->cd();
       canvA->Print(savefilename);
       break;

     case M_CONFIG:
       new HTofConfigPanel(fClient->GetRoot(),1,1);
       break;

     case M_RAW:
       kRaw = kTRUE;
       kHit = kFALSE;
       kMulti = kFALSE;
       new HTofHistAction(fClient->GetRoot(),1,1);
       break;

     case M_HIT:
       kHit = kTRUE;
       kRaw = kFALSE;
       kMulti = kFALSE;
       new HTofHistAction(fClient->GetRoot(),1,1);
       break;

     case M_MULTIH:
       kMulti = kTRUE;
       kHit = kFALSE;
       kRaw = kFALSE;
       new HTofHistAction(fClient->GetRoot(),1,1);
       break;

     case M_CLEAR:
       listbox[0] = 0;
       listbox[1] = 0;
       listbox[2] = 0;
       if(fListSec->GetSelectedEntry()) 
	 fListSec->GetSelectedEntry()->Activate(kFALSE);
       if(fListMod->GetSelectedEntry()) 
	 fListMod->GetSelectedEntry()->Activate(kFALSE);
       if(fListRod->GetSelectedEntry()) 
	 fListRod->GetSelectedEntry()->Activate(kFALSE);
       fListSec->Layout();
       fListMod->Layout();
       fListRod->Layout();
       fTofGeom->setLoopColors();
       kSector = kFALSE;
       kModule = kFALSE;
       kRod = kFALSE;
       canvA->Modified();
       canvA->Update();
       break;

     case M_ZOOM:
       canvA->cd();
       kZOOM = !kZOOM;
       if(kZOOM) {
	 fZoom->SetState(kButtonEngaged);
       }
       else {
	 fZoom->SetState(kButtonUp);
	 canvA->Range(0.,0.,1.,1.);
	 canvA->Modified();
	 canvA->Update();
       }
       break;

     case M_START:
       if(!kConfigured) break;
       SendMessage(this, MK_MSG(kC_COMMAND,kCM_BUTTON),M_CLEAR,0);
       events = atoi(fSetEvBuf->GetString());
       toGreen(fLoop->GetId());
       fClient->NeedRedraw(fLoop);
       gSystem->AddTimer(new HTofDisplayTimer(this,1000));
       break;

     case M_RESET:
       SendMessage(this, MK_MSG(kC_COMMAND,kCM_BUTTON),M_CLEAR,0);
       for(j=0; j<384; j++) {
	 pTofHisto = (HTofHisto*) fHistoCont->At(j);
	 if(pTofHisto) pTofHisto->reset();
       }
       ((TH2F*)fHistoCont->At(384))->Reset();
       ((TH2F*)fHistoCont->At(385))->Reset();
       ((TH2F*)fHistoCont->At(386))->Reset();
       ((TH2F*)fHistoCont->At(387))->Reset();
       ((TH2F*)fHistoCont->At(388))->Reset();
       ((TH2F*)fHistoCont->At(389))->Reset();
       ((TH2F*)fHistoCont->At(390))->Reset();
       ((TH2F*)fHistoCont->At(391))->Reset();
       ((TH2F*)fHistoCont->At(392))->Reset();
       ((TH2F*)fHistoCont->At(393))->Reset();
       ((TH2F*)fHistoCont->At(394))->Reset();
       ((TH2F*)fHistoCont->At(395))->Reset();
       ((TH2F*)fHistoCont->At(396))->Reset();
       ((TH2F*)fHistoCont->At(397))->Reset();
       ((TH2F*)fHistoCont->At(398))->Reset();
       ((TH2F*)fHistoCont->At(399))->Reset();
       ((TH2F*)fHistoCont->At(400))->Reset();
       ((TH2F*)fHistoCont->At(401))->Reset();
       ((TH2F*)fHistoCont->At(402))->Reset();
       ((TH2F*)fHistoCont->At(403))->Reset();
       ((TH2F*)fHistoCont->At(404))->Reset();
       ((TH2F*)fHistoCont->At(405))->Reset();
       ((TH2F*)fHistoCont->At(406))->Reset();
       ((TH2F*)fHistoCont->At(407))->Reset();
       ((TH2F*)fHistoCont->At(408))->Reset();
       fTofGeom->setMaxFlag(kFALSE);
       fTofGeom->setLoopColors();
       sprintf(tmp,"%i",fTofGeom->getEntriesMax());
       fScBuf->Clear();
       fScBuf->AddText(0,tmp);
       fClient->NeedRedraw(fScText);
       canvA->Modified();
       canvA->Update();
       break;

     case M_DRAW_SC:
       fTofGeom->setMaxFlag(kFALSE);
       fTofGeom->setLoopColors();
       sprintf(tmp,"%i",fTofGeom->getEntriesMax());
       fScBuf->Clear();
       fScBuf->AddText(0,tmp);
       fClient->NeedRedraw(fScText);
       canvA->Modified();
       canvA->Update();
       break;

     case M_SCALE:
       scale = atoi(fScBuf->GetString());
       fTofGeom->setEntriesMax(scale);
       fTofGeom->setLoopColors();
       sprintf(tmp,"%i",fTofGeom->getEntriesMax());
       fScBuf->Clear();
       fScBuf->AddText(0,tmp);
       fClient->NeedRedraw(fScText);
       canvA->Modified();
       canvA->Update();
       break;

     default:
       break;
     }

   case kCM_LISTBOX:

     switch(parm1) {

     case M_LIST_SEC:
       listbox[0] = parm2;
       fTofGeom->setLoopColors();
       if(listbox[1]&&listbox[2]) {
	 index = 64 * (parm2-1) + 8 * (listbox[1]-1) + (listbox[2]-1);
	 for(j=0;j<384;j++) {
	   if(fTofGeom->getArray()->At(j)) {
	     if(j==index) ((HTofRod*) fTofGeom->getArray()->At(j))
			    -> SetFillColor(color);
	   }
	 }
	 kSector = kTRUE;
	 kModule = kTRUE;
	 kRod = kTRUE;
       }
       if(!listbox[1]&&!listbox[2]) {
	 for(j=0;j<384;j++) {
	   if(fTofGeom->getArray()->At(j)) { 
	     if(j>=(64*(parm2-1)) && j<(64*parm2))
	       ((HTofRod*) fTofGeom->getArray()->At(j))-> SetFillColor(color);
	   }
	 }
	 kSector = kTRUE;
	 kModule = kFALSE;
	 kRod = kFALSE;
       }
       if(listbox[1]&&!listbox[2]) { 
	 for(j=0;j<384;j++) {
	   if(fTofGeom->getArray()->At(j)) { 
	     if(j >= (64*(parm2-1)+8*(listbox[1]-1)) &&
		j < (64*(parm2-1)+8*listbox[1])) 
	       ((HTofRod*) fTofGeom->getArray()->At(j))-> SetFillColor(color);
	   }
	 }
	 kSector = kTRUE;
	 kModule = kTRUE;
	 kRod = kFALSE;
       }
       /*       if(!listbox[1]&&listbox[2]) { 
	 for(j=0;j<384;j++) {
	   if(fTofGeom->getArray()->At(j)) { 
	     if(j >= (64*(parm2-1)) && j < (64*parm2) &&
		(j%8) == (listbox[2]-1)) 
	       ((HTofRod*) fTofGeom->getArray()->At(j))-> SetFillColor(color);
	   }
	 }
	 }*/
       canvA->Modified();
       canvA->Update();
       break;

     case M_LIST_MOD:
       listbox[1] = parm2;
       fTofGeom->setLoopColors();
       if(listbox[0]&&listbox[2]) {
	 index = 64 * (listbox[0]-1) + 8 * (parm2-1) + (listbox[2]-1);
	 for(j=0;j<384;j++) {
	   if(fTofGeom->getArray()->At(j)) {
	     if(j==index) ((HTofRod*) fTofGeom->getArray()->At(j))
			    -> SetFillColor(color);
	   }
	 }
	 kSector = kTRUE;
	 kModule = kTRUE;
	 kRod = kTRUE;
       }
       /*       if(!listbox[0]&&!listbox[2]) {
	 for(j=0;j<384;j++) {
	   if(fTofGeom->getArray()->At(j)) {
	     if(j >= (64*(j/64)+8*(parm2-1)) && j < (64*(j/64) + 8*parm2)) 
	       ((HTofRod*) fTofGeom->getArray()->At(j))-> SetFillColor(color);
	   }
	 }
       }
       if(!listbox[0]&&listbox[2]) {
	 for(j=0;j<384;j++) {
	   if(fTofGeom->getArray()->At(j)) {
	     if(j >= (64*(j/64)+8*(parm2-1)) && j < (64*(j/64) + 8*parm2) &&
		(j%8) == (listbox[2]-1)) 
	       ((HTofRod*) fTofGeom->getArray()->At(j))-> SetFillColor(color);
	   }
	 }
	 }*/
       if(listbox[0]&&!listbox[2]) {
	 for(j=0;j<384;j++) {
	   if(fTofGeom->getArray()->At(j)) {
	     if(j >= (64*(listbox[0] - 1)+8*(parm2 - 1)) &&
		j < (64*(listbox[0] - 1) +8*parm2)) 
	       ((HTofRod*) fTofGeom->getArray()->At(j))-> SetFillColor(color);
	   }
	 }
	 kSector = kTRUE;
	 kModule = kTRUE;
	 kRod = kFALSE;
       }
       canvA->Modified();
       canvA->Update();
       break;

     case M_LIST_ROD:
       listbox[2] = parm2;
       fTofGeom->setLoopColors();
       if(listbox[0]&&listbox[1]) {
	 index = 64 * (listbox[0]-1) + 8 * (listbox[1]-1) + (parm2-1);
	 for(j=0;j<384;j++) {
	   if(fTofGeom->getArray()->At(j)) {
	     if(j==index) ((HTofRod*) fTofGeom->getArray()->At(j))
			    -> SetFillColor(color);
	   }
	 }
	 kSector = kTRUE;
	 kModule = kTRUE;
	 kRod = kTRUE;
       }
       /*       if(!listbox[0]&&!listbox[1]) {
	 for(j=0;j<384;j++) {
	   if(fTofGeom->getArray()->At(j)) {
	     if((j%8) == (parm2-1)) 
	       ((HTofRod*) fTofGeom->getArray()->At(j))-> SetFillColor(color);
	   }
	 }
       }
       if(listbox[0]&&!listbox[1]) {
	 for(j=0;j<384;j++) {
	   if(fTofGeom->getArray()->At(j)) {
	     if(j >= (64*(listbox[0]-1)) && j < (64*listbox[0]) && 
		(j%8) == (parm2 - 1)) 
	       ((HTofRod*) fTofGeom->getArray()->At(j))-> SetFillColor(color);
	   }
	 }
       }
       if(!listbox[0]&&listbox[1]) {
	 for(j=0;j<384;j++) {
	   if(fTofGeom->getArray()->At(j)) {
	     if(j >= (64*(j/64)+8*(listbox[1]-1)) && 
		j < (64*(j/64) + 8*listbox[1]) && (j%8) == (parm2 - 1)) 
	       ((HTofRod*) fTofGeom->getArray()->At(j))-> SetFillColor(color);
	   }
	 }
	 }*/
       canvA->Modified();
       canvA->Update();
       break;

     default:
       break;
     }

   default:
     break;
   }

 default:
   break;
 }

 return kTRUE;

}



void HTofEventDisplay::setConfigBit(Int_t bit, Bool_t stat) {

  if(stat) config |=  (1 << bit);
  else     config &= ~(1 << bit);

}


Int_t HTofEventDisplay::testConfigBit(Int_t bit) {

  if((config & (1 << bit)) != 0) return 1;
  return 0;

}


Bool_t HTofEventDisplay::setConfiguration() {

  Int_t i;
  Int_t retval;


  if(gHades) delete gHades;

  fTofGeom->getArray()->Delete();

  if(!strcmp(source,"")) {
    new TGMsgBox(fClient->GetRoot(),this,"Error message",
 		 "No input is defined", kMBIconExclamation,kMBOk,&retval);
    return kFALSE;
  }
  if(!strcmp(target,"")) {
    new TGMsgBox(fClient->GetRoot(),this,"Error message",
		 "No output is defined", kMBIconExclamation,kMBOk,&retval);
    return kFALSE;
  }
  if((testConfigBit(B_CAL) == 1) && !strcmp(calib,"")) { 
    new TGMsgBox(fClient->GetRoot(),this,"Error message",
		 "No file with calibration parameters is defined", 
		 kMBIconExclamation,kMBOk,&retval);
    return kFALSE;
  }

  // Make a check that geometry is defined

  if(!testConfigBit(B_SEC1) && !testConfigBit(B_SEC2) && 
     !testConfigBit(B_SEC3) && !testConfigBit(B_SEC4) &&
     !testConfigBit(B_SEC5) && !testConfigBit(B_SEC6)) {
    new TGMsgBox(fClient->GetRoot(),this,"Error message",
		 "No geometry is defined", kMBIconExclamation,kMBOk,&retval);
    return kFALSE;
  }

  // Check if an object of class Hades exists, if not create one

  if(!gHades || !gHades->TestBit(kNotDeleted)) new Hades;
  
  // set geometry

  HTofDetector *tofdet = new HTofDetector;
  Int_t modules[22] = { 1, 2, 3, 4, 5, 6, 7, 8 };
  for(i=0; i<6; i++) {  
    if(testConfigBit(i+6) == 1) { 
      tofdet->setModules(i,modules);
      fTofGeom->addSector(i);
      addHist(i);
    }
  }

  //  SendMessage(this, MK_MSG(kC_COMMAND,kCM_BUTTON), M_RESET, 0);

  gHades->getSetup()->addDetector(tofdet);

  tofdet->print();

  canvA->cd();
  fTofGeom->Draw();
  canvA->Modified();
  canvA->Update();

  // set runtime database
  
  if(testConfigBit(B_ROOT) == 1 || testConfigBit(B_HLD) == 1) 
    gHades->getRuntimeDb()->addEventFile(source);

  // set reconstruction tree

  HTask *nulltask = 0;
  if(testConfigBit(B_CAL) == 0) { // monitoring 
    gHades->getTask()->connect(new HTofMonitor("tof.hist","tof.hist"));
    gHades->getTask()->connect(nulltask,"tof.hist");
  }
  else {
    if(testConfigBit(B_BEAM) == 1) { //hit finding and monitoring
      gHades->getTask()->connect(new HTofHitF("tof.hitf","tof.hitf"));
      gHades->getTask()->
	connect(new HTofMonitor("tof.hist","tof.hist"),"tof.hitf");
      gHades->getTask()->connect(nulltask,"tof.hist");
    }
    else { //calibration and monitoring
      gHades->getTask()->connect(new HTofCalibrater("tof.cal","tof.cal"));
      gHades->getTask()->
	connect(new HTofMonitor("tof.hist","tof.hist"),"tof.cal");
      gHades->getTask()->connect(nulltask,"tof.hist");
    }      
  }

  // set split level

  gHades->setSplitLevel(2);

  // set data source

  if(testConfigBit(B_ROOT) == 1) {
    HRootSource *rootinput = new HRootSource;
    gHades->setDataSource(rootinput);
  }
  if(testConfigBit(B_HLD) == 1) { 
    HldFileSource *hldinput = new HldFileSource;
    hldinput->addUnpacker(new HTofUnpacker);
    gHades->setDataSource(hldinput);
  }
  if(testConfigBit(B_DAQ) == 1) {
    HldRemoteSource *daqinput = new HldRemoteSource(source);
    daqinput->addUnpacker(new HTofUnpacker);
    gHades->setDataSource(daqinput);
  }

  gHades->getRuntimeDb()->setCurrentEventFile(-1);

  if(!gHades->init()) {
    new TGMsgBox(fClient->GetRoot(),this,"Error message",
		 "Error during init", kMBIconExclamation,kMBOk,&retval);
    return kFALSE;
  }

  // set output file

  gHades->setOutputFile(target,"RECREATE","Test",2);

  // build output tree

  gHades->makeTree();

  kConfigured = kTRUE;

  return kTRUE;

}


Bool_t HTofEventDisplay::addHist(Int_t sec) {

  Int_t mod;
  Int_t rod;
  Int_t pos;
  Int_t det = 384;

  if(!fHistoCont) fHistoCont = new TObjArray(450);

  for(mod = 0; mod < 8; mod++) {
    for(rod = 0; rod < 8; rod++) {

      pos = 64*sec + 8*mod + rod;
      if(!(fHistoCont->operator [] (pos)))
      fHistoCont->AddAt(new HTofHisto(sec,mod,rod),pos);

    }
  }
  
  if(!(fHistoCont->operator [] (384))) {

    fHistoCont->AddAt(new TH2F("left vs. right","TOF data",200,0.,4090.,
			       200,0.,4090.), 384);
    ((TH1F*) fHistoCont->At(384))->GetXaxis()->SetTitle("right time [ch]");
    ((TH1F*) fHistoCont->At(384))->GetYaxis()->SetTitle("left time [ch]");
  }
  if(!(fHistoCont->operator [] (385))) {

    fHistoCont->AddAt(new TH2F("left vs. tof","TOF data",200,0.,20.,
			       200,0.,4090.), 385);
    ((TH1F*) fHistoCont->At(385))->GetXaxis()->SetTitle("tof [ns]");
    ((TH1F*) fHistoCont->At(385))->GetYaxis()->SetTitle("left time [ch]");
  }
  if(!(fHistoCont->operator [] (386))) {

    fHistoCont->AddAt(new TH2F("left vs. xpos","TOF data",200,-150.,150.,
			       200,0.,4090.), 386);
    ((TH1F*) fHistoCont->At(386))->GetXaxis()->SetTitle("x [cm]");
    ((TH1F*) fHistoCont->At(386))->GetYaxis()->SetTitle("left time [ch]");
  }
  if(!(fHistoCont->operator [] (387))) {

    fHistoCont->AddAt(new TH2F("det vs. left","TOF data",200,0.,4090.,
			       det,1.,385.), 387);
    ((TH1F*) fHistoCont->At(387))->GetXaxis()->SetTitle("left time [ch]");
    ((TH1F*) fHistoCont->At(387))->GetYaxis()->SetTitle("detector");
  }
  if(!(fHistoCont->operator [] (388))) {

    fHistoCont->AddAt(new TH2F("right vs. tof","TOF data",200,0.,20.,
			       200,0.,4090.), 388);
    ((TH1F*) fHistoCont->At(388))->GetXaxis()->SetTitle("tof [ns]");
    ((TH1F*) fHistoCont->At(388))->GetYaxis()->SetTitle("right time [ch]");
  }
  if(!(fHistoCont->operator [] (389))) {

    fHistoCont->AddAt(new TH2F("right vs. xpos","TOF data",200,-150.,150.,
			       200,0.,4090.), 389);
    ((TH1F*) fHistoCont->At(389))->GetXaxis()->SetTitle("x [cm]");
    ((TH1F*) fHistoCont->At(389))->GetYaxis()->SetTitle("right time [ch]");
  }
  if(!(fHistoCont->operator [] (390))) {

    fHistoCont->AddAt(new TH2F("det vs. right","TOF data",200,0.,4090.,
			       det,1.,385.), 390);
    ((TH1F*) fHistoCont->At(390))->GetXaxis()->SetTitle("right time [ch]");
    ((TH1F*) fHistoCont->At(390))->GetYaxis()->SetTitle("detector");
  }
  if(!(fHistoCont->operator [] (391))) {

    fHistoCont->AddAt(new TH2F("tof vs. xpos","TOF data",200,-150.,150.,
			       200,0.,20.), 391);
    ((TH1F*) fHistoCont->At(391))->GetXaxis()->SetTitle("x [cm]");
    ((TH1F*) fHistoCont->At(391))->GetYaxis()->SetTitle("tof [ns]");
  }
  if(!(fHistoCont->operator [] (392))) {

    fHistoCont->AddAt(new TH2F("det vs. tof","TOF data",200,0.,20.,
			       det,1.,385.), 392);
    ((TH1F*) fHistoCont->At(392))->GetXaxis()->SetTitle("tof [ns]");
    ((TH1F*) fHistoCont->At(392))->GetYaxis()->SetTitle("detector");
  }
  if(!(fHistoCont->operator [] (393))) {

    fHistoCont->AddAt(new TH2F("det vs. xpos","TOF data",200,-150.,150.,
			       det,1.,385.), 393);
    ((TH1F*) fHistoCont->At(393))->GetXaxis()->SetTitle("x [cm]");
    ((TH1F*) fHistoCont->At(393))->GetYaxis()->SetTitle("detector");
  }

  //mult and pattern

  if(!(fHistoCont->operator [] (394))) {

    fHistoCont->AddAt(new TH1F("left mult_3","TOF data",64,1.,65.), 394);
    ((TH1F*) fHistoCont->At(394))->GetXaxis()->SetTitle("multiplicity");
  }

  if(!(fHistoCont->operator [] (395))) {

    fHistoCont->AddAt(new TH1F("right mult_3","TOF data",64,1.,65.), 395);
    ((TH1F*) fHistoCont->At(395))->GetXaxis()->SetTitle("multiplicity");
  }

  if(!(fHistoCont->operator [] (396))) {

    fHistoCont->AddAt(new TH1F("total mult_3","TOF data",64,1.,65.), 396);
    ((TH1F*) fHistoCont->At(396))->GetXaxis()->SetTitle("multiplicity");
  }

  if(!(fHistoCont->operator [] (397))) {

    fHistoCont->AddAt(new TH1F("left mult_6","TOF data",64,1.,65.), 397);
    ((TH1F*) fHistoCont->At(397))->GetXaxis()->SetTitle("multiplicity");
  }

  if(!(fHistoCont->operator [] (398))) {

    fHistoCont->AddAt(new TH1F("right mult_6","TOF data",64,1.,65.), 398);
    ((TH1F*) fHistoCont->At(398))->GetXaxis()->SetTitle("multiplicity");
  }

  if(!(fHistoCont->operator [] (399))) {

    fHistoCont->AddAt(new TH1F("total mult_6","TOF data",64,1.,65.), 399);
    ((TH1F*) fHistoCont->At(399))->GetXaxis()->SetTitle("multiplicity");
  }

  if(!(fHistoCont->operator [] (400))) {

    fHistoCont->AddAt(new TH1F("left pat_3","TOF data",64,1.,65.), 400);
    ((TH1F*) fHistoCont->At(400))->GetXaxis()->SetTitle("pattern");
  }

  if(!(fHistoCont->operator [] (401))) {

    fHistoCont->AddAt(new TH1F("right pat_3","TOF data",64,1.,65.), 401);
    ((TH1F*) fHistoCont->At(401))->GetXaxis()->SetTitle("pattern");
  }

  if(!(fHistoCont->operator [] (402))) {

    fHistoCont->AddAt(new TH1F("total pat_3","TOF data",64,1.,65.), 402);
    ((TH1F*) fHistoCont->At(402))->GetXaxis()->SetTitle("pattern");
  }

  if(!(fHistoCont->operator [] (403))) {

    fHistoCont->AddAt(new TH1F("left pat_6","TOF data",64,1.,65.), 403);
    ((TH1F*) fHistoCont->At(403))->GetXaxis()->SetTitle("pattern");
  }

  if(!(fHistoCont->operator [] (404))) {

    fHistoCont->AddAt(new TH1F("right pat_6","TOF data",64,1.,65.), 404);
    ((TH1F*) fHistoCont->At(404))->GetXaxis()->SetTitle("pattern");
  }

  if(!(fHistoCont->operator [] (405))) {

    fHistoCont->AddAt(new TH1F("total pat_6","TOF data",64,1.,65.), 405);
    ((TH1F*) fHistoCont->At(405))->GetXaxis()->SetTitle("pattern");
  }

  if(!(fHistoCont->operator [] (406))) {

    fHistoCont->AddAt(new TH1F("left mult_all","TOF data",128,1.,129.), 406);
    ((TH1F*) fHistoCont->At(406))->GetXaxis()->SetTitle("multiplicity");
  }

  if(!(fHistoCont->operator [] (407))) {

    fHistoCont->AddAt(new TH1F("right mult_all","TOF data",128,1.,129.), 407);
    ((TH1F*) fHistoCont->At(407))->GetXaxis()->SetTitle("multiplicity");
  }

  if(!(fHistoCont->operator [] (408))) {

    fHistoCont->AddAt(new TH1F("total mult_all","TOF data",128,1.,129.), 408);
    ((TH1F*) fHistoCont->At(408))->GetXaxis()->SetTitle("multiplicity");
  }

  return kTRUE;

}


Bool_t HTofEventDisplay::readDefaultConfig() {

  char message[100];
  Int_t retval;
  Int_t check;
  Int_t ncol;
  Int_t i;
  char first[20], second[20], third[20];
  char nullstring[100];

  FILE *fp = fopen("tofdef.cfg","r");
  if(!fp) {
    sprintf(message,"%s","File tofdef.cfg not found");
    new TGMsgBox(fClient->GetRoot(),this,"Error message",
		 message, kMBIconExclamation,kMBOk,&retval);
    return kFALSE;
  }

  if(fp) {
    ncol = fscanf(fp, "%d %s%s%s", &check, first,second,third);
    if(check != 111) {
      sprintf(message,"%s","Not a file with Tof configuration");
      new TGMsgBox(fClient->GetRoot(),this,"Error message",
		   message, kMBIconExclamation,kMBOk,&retval);
      return kFALSE;
    }
    
    for(i=0; i<12; i++) { 
      ncol = fscanf(fp, "%d %s%s%s", &check, first,second,third);
      if(check == 1) setConfigBit(i);
      else setConfigBit(i,kFALSE);
    }

    ncol = fscanf(fp, "%s %s%s%s", source,first,second,third);

    if(ncol < 0)  {
      setInput(nullstring);
      setOutput(nullstring);
      setCalib(nullstring);
    }
    else {
      ncol = fscanf(fp, "%s %s%s%s", target,first,second,third);
      if(ncol < 0) {
	setOutput(nullstring);
	setCalib(nullstring);
      }
      else {
	ncol = fscanf(fp, "%s %s%s%s", calib,first,second,third);
	if(ncol < 0) setCalib(nullstring);
      } 
    } 
  }

  if(fp) fclose(fp);
   
  return kTRUE;

}


Bool_t HTofEventDisplay::toGreen(Window_t id) {

  SetWindowAttributes_t wattr;
  wattr.fMask = kWABackPixel;
  gClient->GetColorByName("green", wattr.fBackgroundPixel);
  gGXW->ChangeWindowAttributes(id, &wattr);
  return kTRUE;

}


Bool_t HTofEventDisplay::toRed(Window_t id) {

  SetWindowAttributes_t wattr;
  wattr.fMask = kWABackPixel;
  gClient->GetColorByName("red", wattr.fBackgroundPixel);
  gGXW->ChangeWindowAttributes(id, &wattr);
  return kTRUE;

}


Bool_t HTofEventDisplay::HandleTimer(TTimer *) {

  char tmp[10];

  gHades->eventLoop(events);
  toRed(fLoop->GetId());
  fClient->NeedRedraw(fLoop);
  fTofGeom->setMaxFlag(kFALSE);
  fTofGeom->setLoopColors();
  sprintf(tmp,"%i",fTofGeom->getEntriesMax());
  fScBuf->Clear();
  fScBuf->AddText(0,tmp);
  fClient->NeedRedraw(fScText);
  canvA->Modified();
  canvA->Update();
  return kTRUE;

}


HTofEventDisplay *gTofDisplay;















