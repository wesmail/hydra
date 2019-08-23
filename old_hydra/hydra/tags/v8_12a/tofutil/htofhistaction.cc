#include "htofhistaction.h"
#include <TCollection.h>
#include <TFile.h>
#include <TKey.h>
#include <TROOT.h>
#include <TGMsgBox.h>
#include <TGFileDialog.h>
#include <TSystem.h>
#include <TH1.h>
#include <TCanvas.h>
#include "htofrod.h"
#include "htofgeom.h"
#include "htofeventdisplay.h"

//--------------------------------------------------------------
//
// Author : Dmitry Vasiliev (LNS, Catania)
//
//---------------------------------------------------------------
//---------------------------------------------------------------
//
// HTofHistAction class
//
// histogram actions
//
//---------------------------------------------------------------

#if !defined S_ISDIR
#define S_ISDIR(m) (((m)&(0170000)) == (0040000))
#endif

ClassImp(HTofHistAction)


Bool_t kSTATUS; //to suppress call of drawHist() when no histo is highlighted
                //in ListBoxA
Bool_t kScale;
TCanvas *cA;    //canvas in fCanvasA  
TCanvas *cB;    //canvas in fCanvasB
TH1F *hist;     //clone histo in Scan panel
TH1F *temph;    //bin highlight in canvas cB
Long_t cursA, cursB; //current position in ListBoxA and ListBoxB
Int_t totalA, totalB;  //total number of entries in ListBoxA and ListBoxB
Int_t *array;     //array of non empty bins of a histo under scan
Int_t count;      //number of user highlighted buttons in Display Layout
Int_t indi[2];    //survice arrays for algorithm which calculates
Int_t indj[2];    //display layout


HTofHistAction::HTofHistAction(const TGWindow *p, UInt_t w, UInt_t h) :
                               TGMainFrame(p, w, h) {


//--------------------------------------------------------------------
// Constructor for the class HTofHistAction. Draws a control panel, divides it
// into sub areas and maps all subwindows
//--------------------------------------------------------------------


Int_t i;
for(i=0; i<1000; i++) flags[i] = kFALSE;

for(i=0; i<16; i++) {
  pads[i]= NULL;
  histInd[i]=-1;
}
fHisto = new TObjArray(1000);
position = 0;
totalA = 0;

//---------------------------------------------------------------

fF0 = new TGCompositeFrame(this, 200, 20, kVerticalFrame);
AddFrame(fF0, new TGLayoutHints(kLHintsLeft | kLHintsTop));


fFA = new TGCompositeFrame(fF0, 200, 20, kHorizontalFrame);
fF0->AddFrame(fFA, new TGLayoutHints(kLHintsTop | kLHintsLeft));


fTab = new TGTab(fF0,200,20);
fF0->AddFrame(fTab, new TGLayoutHints(kLHintsLeft | kLHintsTop));


//-------------------------------------------------------------------


fCanvasA = new TRootEmbeddedCanvas("canvasA",fFA,440,400);
fFA->AddFrame(fCanvasA, new TGLayoutHints(kLHintsTop | kLHintsLeft,5,5,5,5));
cA = fCanvasA->GetCanvas();
//cA->SetCanvasSize(396,396);
cA->SetFillColor(10);


fA1 = new TGCompositeFrame(fFA, 100, 20, kVerticalFrame);
fFA->AddFrame(fA1, new TGLayoutHints(kLHintsTop | kLHintsLeft));


fA2 = new TGGroupFrame(fA1, "Control Buttons", kHorizontalFrame);
fA2->SetLayoutManager(new TGMatrixLayout(fA2,0,3,10));
fA1->AddFrame(fA2, new TGLayoutHints(kLHintsTop | kLHintsLeft,5, 5, 5, 5));


fA3 = new TGCompositeFrame(fA1, 200, 50, kVerticalFrame);
fA1->AddFrame(fA3, new TGLayoutHints(kLHintsTop | kLHintsLeft, 5, 0, 5, 5));

fZ1 = new TGCompositeFrame(fA3, 200, 50, kHorizontalFrame);
fA3->AddFrame(fZ1, new TGLayoutHints(kLHintsNormal));

fZ2 = new TGGroupFrame(fA3, "Drawing", kVerticalFrame);
fA3->AddFrame(fZ2, new TGLayoutHints(kLHintsNormal));


fListBoxA = new TGListBox(fZ1, M_LIST_A);
fListBoxA->Associate(this);
fZ1->AddFrame(fListBoxA, new TGLayoutHints(kLHintsTop | kLHintsLeft));
fListBoxA->Resize(140,197);


fA4 = new TGCompositeFrame(fZ1, 100, 20, kVerticalFrame);
fZ1->AddFrame(fA4, new TGLayoutHints(kLHintsTop | kLHintsLeft));


fMultiButton = new TGCheckButton(fA4, "Multiple selection", M_MULTI);
fMultiButton->Associate(this);
fMultiButton->SetState(kButtonUp);
fA4->AddFrame(fMultiButton, new TGLayoutHints(kLHintsNormal,5,0,3,3));


fPrevButtonA = new TGTextButton(fA4,"Previous", M_PREV_A);
fPrevButtonA->Associate(this);
fA4->AddFrame(fPrevButtonA, new TGLayoutHints(kLHintsNormal,10,5,3,3));


fNextButtonA = new TGTextButton(fA4,"   Next   ", M_NEXT_A);
fNextButtonA->Associate(this);
fA4->AddFrame(fNextButtonA, new TGLayoutHints(kLHintsNormal,10,5,3,3));


fA5 = new TGGroupFrame(fA4, "Display Layout", kVerticalFrame);
fA4->AddFrame(fA5, new TGLayoutHints(kLHintsTop|kLHintsLeft,5,5,3,3));
fA5->SetLayoutManager(new TGMatrixLayout(fA5,0,4,2));


for(i=0;i<16;i++) {
  fLayoutButton[i] = new TGTextButton(fA5,"     ",500+i);
  fLayoutButton[i]->Associate(this);
  fA5->AddFrame(fLayoutButton[i], new TGLayoutHints(kLHintsTop|kLHintsLeft,
						    5,0,5,5));
}


fCloseButton = new TGTextButton(fA2, "&Close", M_CLOSE_HIST);
fCloseButton->Associate(this);
fCloseButton->SetToolTipText("Close window");
fCloseButton->Resize(65, fCloseButton->GetDefaultHeight());
fA2->AddFrame(fCloseButton);

fSelectButton = new TGTextButton(fA2, "Select All", M_SELECT);
fSelectButton->Associate(this);
fSelectButton->SetToolTipText("Highlight all histogrammes in the list box");
fSelectButton->Resize(65, fSelectButton->GetDefaultHeight());
fA2->AddFrame(fSelectButton);


fClearButtonA = new TGTextButton(fA2, "Clear", M_CLEAR_A);
fClearButtonA->Associate(this);
fClearButtonA->SetToolTipText("Clear the canvas");
fClearButtonA->Resize(65, fClearButtonA->GetDefaultHeight());
fA2->AddFrame(fClearButtonA);


fSavePic = new TGTextButton(fA2, "&Save Pic", M_SAVE_PIC);
fSavePic->Associate(this);
fSavePic->SetToolTipText("Save content of the canvas");
fSavePic->Resize(65, fSavePic->GetDefaultHeight());
fA2->AddFrame(fSavePic);


fEditButton = new TGTextButton(fA2, "&Edit Pic", M_EDIT);
fEditButton->Associate(this);
fEditButton->SetToolTipText("Envoke graphical editor");
fEditButton->Resize(65, fEditButton->GetDefaultHeight());
fA2->AddFrame(fEditButton);


fSaveHist = new TGTextButton(fA2, "S&ave Hist", M_SAVE_HIST);
fSaveHist->Associate(this);
fSaveHist->SetToolTipText("Save histogrammes to a root file");
fSaveHist->Resize(65, fSaveHist->GetDefaultHeight());
fA2->AddFrame(fSaveHist);


fX1 = new TGCompositeFrame(fZ2, 100, 50, kHorizontalFrame);
fZ2->AddFrame(fX1, new TGLayoutHints(kLHintsNormal));

fY1 = new TGCompositeFrame(fZ2, 100, 50, kHorizontalFrame);
fZ2->AddFrame(fY1, new TGLayoutHints(kLHintsNormal));

fY2 = new TGCompositeFrame(fZ2, 100, 50, kHorizontalFrame);
fZ2->AddFrame(fY2, new TGLayoutHints(kLHintsNormal));

//fX1->AddFrame(new TGLabel(fX1, new TGHotString("x start")),
//	      new TGLayoutHints(kLHintsNormal,5,1,1,1));
fXstart = new TGTextEntry(fX1, fXstartBuf = new TGTextBuffer(100), -1);
fX1->AddFrame(fXstart, new TGLayoutHints(kLHintsNormal, 5, 0, 0, 0));
fXstart->Resize(60, fXstart->GetDefaultHeight());

fX1->AddFrame(new TGLabel(fX1, new TGHotString("start <- x -> stop")),
	      new TGLayoutHints(kLHintsNormal,5,2,2,0));
fXstop = new TGTextEntry(fX1, fXstopBuf = new TGTextBuffer(100), -1);
fX1->AddFrame(fXstop, new TGLayoutHints(kLHintsNormal, 7, 0, 0, 0));
fXstop->Resize(60, fXstop->GetDefaultHeight());

//fY1->AddFrame(new TGLabel(fY1, new TGHotString("y start")),
//	      new TGLayoutHints(kLHintsNormal,5,1,1,1));
fYstart = new TGTextEntry(fY1, fYstartBuf = new TGTextBuffer(100), -1);
fY1->AddFrame(fYstart, new TGLayoutHints(kLHintsNormal, 5, 0, 0, 0));
fYstart->Resize(60, fYstart->GetDefaultHeight());

fY1->AddFrame(new TGLabel(fY1, new TGHotString("start <- y -> stop")),
	      new TGLayoutHints(kLHintsNormal,5,2,2,0));
fYstop = new TGTextEntry(fY1, fYstopBuf = new TGTextBuffer(100), -1);
fY1->AddFrame(fYstop, new TGLayoutHints(kLHintsNormal, 6, 0, 0, 0));
fYstop->Resize(60, fYstop->GetDefaultHeight());

fDrawButton = new TGTextButton(fY2, " &Draw ", M_DRAW);
fDrawButton->Associate(this);
fDrawButton->SetToolTipText("Draw a histogram with automatic settings");
fY2->AddFrame(fDrawButton, new TGLayoutHints(kLHintsNormal, 5,0,5,0));

fScaleStat = new TGTextButton(fY2, "    ", M_SCALE_STAT);
fScaleStat->Associate(this);
fScaleStat->SetToolTipText("Set a scale status");
fY2->AddFrame(fScaleStat, new TGLayoutHints(kLHintsNormal, 4,0,5,0));

fScaleButton = new TGTextButton(fY2, " Sca&le ", M_SCALE_HIST);
fScaleButton->Associate(this);
fScaleButton->SetToolTipText("Draw a histogram with user defined settings");
fY2->AddFrame(fScaleButton, new TGLayoutHints(kLHintsNormal, 5,0,5,0));


fY2->AddFrame(new TGLabel(fY2, new TGHotString("scale")),
	      new TGLayoutHints(kLHintsNormal,7,0,7,0));
fScale = new TGTextEntry(fY2, fScaleBuf = new TGTextBuffer(100), -1);
fY2->AddFrame(fScale, new TGLayoutHints(kLHintsNormal, 9,0,5,0));
fScale->Resize(60, fScale->GetDefaultHeight());


//---------------------------------------------------------------------


TGCompositeFrame *tf = fTab->AddTab("Import");
fC1 = new TGCompositeFrame(tf,500,50,kHorizontalFrame);
tf->AddFrame(fC1, new TGLayoutHints(kLHintsLeft|kLHintsTop,5,5,5,5));


fC2 = new TGCompositeFrame(tf,500,300,kVerticalFrame);
tf->AddFrame(fC2, new TGLayoutHints(kLHintsLeft|kLHintsTop,5,5,5,5));


fDir = new TGListBox(fC1,-1);
//fDir->Associate(this);
fDir->Resize(400,20);
char tmp[50];
sprintf(tmp,"%s",gSystem->WorkingDirectory());
fDir->AddEntry(tmp,1);
fC1->AddFrame(fDir, new TGLayoutHints(kLHintsLeft|kLHintsTop,2,2,2,2));


fPcdup = fClient->GetPicture("tb_uplevel.xpm");
fCdup = new TGPictureButton(fC1,fPcdup,M_CDUP);
fCdup->SetToolTipText("Up one level");
fCdup->Associate(this);
fC1->AddFrame(fCdup, new TGLayoutHints(kLHintsLeft|kLHintsTop,5,5,5,5));


fPlist = fClient->GetPicture("tb_list.xpm");
fListMode = new TGPictureButton(fC1,fPlist,M_LIST_MODE);
fListMode->SetToolTipText("List mode");
fListMode->Associate(this);
fListMode->SetState(kButtonUp);
fListMode->AllowStayDown(kTRUE);
fC1->AddFrame(fListMode, new TGLayoutHints(kLHintsLeft|kLHintsTop,5,5,5,5));


fPdetail = fClient->GetPicture("tb_details.xpm");
fDetailMode = new TGPictureButton(fC1,fPdetail,M_DETAIL_MODE);
fDetailMode->SetToolTipText("Details mode");
fDetailMode->Associate(this);
fDetailMode->SetState(kButtonEngaged);
fDetailMode->AllowStayDown(kTRUE);
fC1->AddFrame(fDetailMode, new TGLayoutHints(kLHintsLeft|kLHintsTop,
						5,5,5,5));


fC1->AddFrame(new TGLabel(fC1, new TGHotString("Filter:")),
	      new TGLayoutHints(kLHintsLeft|kLHintsTop,10,5,5,5));


fFilterBox = new TGComboBox(fC1, M_FILTER);
fFilterBox->Associate(this);
char filt[50];
sprintf(filt,"%s","All files  (*)");
fFilterBox->AddEntry(filt,1);
sprintf(filt,"%s","ROOT files  (*.root)");
fFilterBox->AddEntry(filt,2);
fFilterBox->Resize(150,20);
fFilterBox->Select(1);
fC1->AddFrame(fFilterBox, new TGLayoutHints(kLHintsLeft|kLHintsTop,5,5,5,5));


fFileView = new TGListView(fC2,540,180);
fFileCont = new TGFileContainer(fFileView->GetViewPort(),100,100,
                                kVerticalFrame,fgWhitePixel);
fFileCont->Associate(this);
fFileView->GetViewPort()->SetBackgroundColor(fgWhitePixel);
fFileView->SetContainer(fFileCont);
fFileCont->SetFilter("*");
fFileCont->ChangeDirectory(".");
fFileView->SetViewMode(kLVDetails);
fFileCont->Sort(kSortByName);
fC2->AddFrame(fFileView, new TGLayoutHints(kLHintsLeft|kLHintsTop,
                                               2,2,2,2));



//---------------------------------------------------------------------


tf = fTab->AddTab("Scan");
fFB = new TGGroupFrame(tf,"Scan Panel", kHorizontalFrame);
tf->AddFrame(fFB, new TGLayoutHints(kLHintsTop | kLHintsLeft));


fB1 = new TGGroupFrame(fFB, "General Histo Info", kVerticalFrame);
fB1->SetLayoutManager(new TGMatrixLayout(fB1,0,2,10));
fFB->AddFrame(fB1, new TGLayoutHints(kLHintsTop | kLHintsLeft | 
				     kLHintsExpandY));


fCanvasB = new TRootEmbeddedCanvas("canvasB",fFB,200,200);
fFB->AddFrame(fCanvasB, new TGLayoutHints(kLHintsTop | kLHintsLeft |
					 kLHintsExpandX | kLHintsExpandY,
					 5, 5, 5, 5));
cB = fCanvasB->GetCanvas();
cB->SetFillColor(10);


fB2 = new TGCompositeFrame(fFB, 100, 100, kVerticalFrame);
fFB->AddFrame(fB2, new TGLayoutHints(kLHintsTop | kLHintsLeft | 
				     kLHintsExpandY));


fB2->AddFrame(new TGLabel(fB2, new TGHotString("Non empty bins")),
	      new TGLayoutHints(kLHintsLeft | kLHintsTop));


fB4 = new TGCompositeFrame(fB2, 220, 100, kHorizontalFrame); 
fB2->AddFrame(fB4, new TGLayoutHints(kLHintsTop|kLHintsLeft));


fListBoxB = new TGListBox(fB4, M_LIST_B);
fListBoxB->Associate(this);
fB4->AddFrame(fListBoxB, new TGLayoutHints(kLHintsTop | kLHintsLeft,5,0,5,0));
fListBoxB->Resize(70,100);


fB5 = new TGGroupFrame(fB4, "Control Buttons", kVerticalFrame);
fB5->SetLayoutManager(new TGMatrixLayout(fB5,0,2,10));
fB4->AddFrame(fB5, new TGLayoutHints(kLHintsTop | kLHintsLeft, 20,0,0,0));


fPrevButtonB = new TGTextButton(fB5,"Previous", M_PREV_B);
fPrevButtonB->Associate(this);
fPrevButtonB->Resize(55, fPrevButtonB->GetDefaultHeight());
fB5->AddFrame(fPrevButtonB, new TGLayoutHints(kLHintsLeft | kLHintsTop,
					      10,1,5,5));


fGetButton = new TGTextButton(fB5, "Import", M_IMPORT);
fGetButton->Associate(this);
fGetButton->SetToolTipText("Import a histogram");
fGetButton->Resize(55, fGetButton->GetDefaultHeight());
fB5->AddFrame(fGetButton, new TGLayoutHints(kLHintsTop | kLHintsLeft,
					     1, 2, 5, 5));


fNextButtonB = new TGTextButton(fB5,"Next", M_NEXT_B);
fNextButtonB->Associate(this);
fNextButtonB->Resize(55, fNextButtonB->GetDefaultHeight());
fB5->AddFrame(fNextButtonB, new TGLayoutHints(kLHintsLeft | kLHintsTop,
					      10,1,5,5));


fClearButtonB = new TGTextButton(fB5, "Clear", M_CLEAR_B);
fClearButtonB->Associate(this);
fClearButtonB->SetToolTipText("Clear all fields in this panel");
fClearButtonB->Resize(55, fClearButtonB->GetDefaultHeight());
fB5->AddFrame(fClearButtonB, new TGLayoutHints(kLHintsTop | kLHintsLeft,
					       1, 2, 5, 5));


fB3 = new TGCompositeFrame(fB2, 100, 20, kVerticalFrame);
fB3->SetLayoutManager(new TGMatrixLayout(fB3,0,2,10));
fB2->AddFrame(fB3, new TGLayoutHints(kLHintsTop | kLHintsLeft));


fB1->AddFrame(new TGLabel(fB1,new TGHotString("Name")),
	      new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX));


fName = new TGTextEntry(fB1, fNameBuf = new TGTextBuffer(100), -1);
fB1->AddFrame(fName, new TGLayoutHints(kLHintsTop | kLHintsLeft, 10, 2, 2, 2));
fName->Resize(100, fName->GetDefaultHeight());


fB1->AddFrame(new TGLabel(fB1,new TGHotString("Title")),
	      new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX));


fTitle = new TGTextEntry(fB1, fTitleBuf = new TGTextBuffer(100), -1);
fB1->AddFrame(fTitle, new TGLayoutHints(kLHintsTop | kLHintsLeft, 10,2, 2, 2));
fTitle->Resize(100, fTitle->GetDefaultHeight());


fB1->AddFrame(new TGLabel(fB1,new TGHotString("Channels")),
	      new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX));


fChan = new TGTextEntry(fB1, fChanBuf = new TGTextBuffer(100), -1);
fB1->AddFrame(fChan, new TGLayoutHints(kLHintsTop | kLHintsLeft, 10, 2, 2, 2));
fChan->Resize(100, fChan->GetDefaultHeight());


fB1->AddFrame(new TGLabel(fB1,new TGHotString("Range")),
	      new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX));


fRange = new TGTextEntry(fB1, fRangeBuf = new TGTextBuffer(100), -1);
fB1->AddFrame(fRange, new TGLayoutHints(kLHintsTop | kLHintsLeft, 10,2, 2, 2));
fRange->Resize(100, fRange->GetDefaultHeight());


fB1->AddFrame(new TGLabel(fB1,new TGHotString("Entries")),
	      new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX));


fEntries = new TGTextEntry(fB1, fEntriesBuf = new TGTextBuffer(100), -1);
fB1->AddFrame(fEntries, new TGLayoutHints(kLHintsTop | kLHintsLeft, 10,2,2,2));
fEntries->Resize(100, fEntries->GetDefaultHeight());


fB3->AddFrame(new TGLabel(fB3,new TGHotString("Bin content")),
	      new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX));


fBinCont = new TGTextEntry(fB3, fBinContBuf = new TGTextBuffer(100), -1);
fB3->AddFrame(fBinCont, new TGLayoutHints(kLHintsTop | kLHintsLeft, 
					  0,2, 2, 2));
fBinCont->Resize(110, fBinCont->GetDefaultHeight());


fB3->AddFrame(new TGLabel(fB3,new TGHotString("Bin range")),
	      new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX));


fBinRange = new TGTextEntry(fB3, fBinRangeBuf = new TGTextBuffer(100), -1);
fB3->AddFrame(fBinRange, new TGLayoutHints(kLHintsTop | kLHintsLeft, 
					   0,2, 2, 2));
fBinRange->Resize(110, fBinRange->GetDefaultHeight());

//---------------------------------------------------------------

importDefault();
kScale = kFALSE;

//----------------------------------------------------------------

MapSubwindows();

SetWindowName("HISTO ACTION");

Resize(GetDefaultSize());
MapWindow();

}



HTofHistAction::~HTofHistAction() {

//----------------------------------------------------------
//
//    Destructor for the class HTofHistAction
//
//----------------------------------------------------------
delete fBinRange;
delete fBinCont;
delete fEntries;
delete fRange;
delete fChan;
delete fTitle;
delete fName;
delete fB3;
delete fClearButtonB;
delete fNextButtonB;
delete fGetButton;
delete fPrevButtonB;
delete fB5;
delete fListBoxB;
delete fB4;
delete fB2;
delete fCanvasB;
delete fB1;
delete fFB;
delete fFileView;
delete fFilterBox;
delete fDetailMode;
delete fListMode;
delete fCdup;
delete fDir;
delete fC2;
delete fC1;
delete fScaleStat;
delete fScaleButton;
delete fDrawButton;
delete fScale;
delete fYstop; 
delete fYstart;
delete fXstop; 
delete fXstart;
delete fY2; 
delete fY1;
delete fX1;
delete fSaveHist;
delete fEditButton;
delete fSavePic;
delete fClearButtonA;
delete fSelectButton;
delete fCloseButton;
delete fLayoutButton[15];
delete fLayoutButton[14];
delete fLayoutButton[13];
delete fLayoutButton[12];
delete fLayoutButton[11];
delete fLayoutButton[10];
delete fLayoutButton[9];
delete fLayoutButton[8];
delete fLayoutButton[7];
delete fLayoutButton[6];
delete fLayoutButton[5];
delete fLayoutButton[4];
delete fLayoutButton[3];
delete fLayoutButton[2];
delete fLayoutButton[1];
delete fLayoutButton[0];
delete fA5;
delete fNextButtonA;
delete fPrevButtonA;
delete fMultiButton;
delete fA4;
delete fListBoxA;
delete fZ2; 
delete fZ1;
delete fA3;
delete fA2;
delete fA1;
delete fCanvasA;
delete fTab;
delete fFA;
delete fF0;

}



void HTofHistAction::CloseWindow() {

//-----------------------------------------------------
//
//           CloseWindow()
//
//     Closes the panel "HISTO ACTION"
//
//-----------------------------------------------------

delete this;

}


void HTofHistAction::importDefault() {

  if(!gTofDisplay) return;

  HTofRod *pRod;
  HTofHisto *pTofHisto;
  Int_t i;
  Color_t color;
  Int_t sec, mod, rod;
  Int_t index;
  Bool_t kBooked = kFALSE;
  char name[100];
  Int_t count = 0;
  TH2F *l_d = 0;
  TH2F *r_d = 0;
  TH2F *t_d = 0;
  TH2F *x_d = 0;
  Int_t detmin;
  Int_t detmax;
  Int_t k;
  Float_t lr_ratio = 4096./1024.;
  //  Float_t t_ratio = 60./200.;
  //  Float_t x_ratio = 300./200.;

  for(i=0; i<384; i++) {
    pRod = (HTofRod*) gTofDisplay->getTofGeom()->getArray()->At(i);
    if(pRod) {
      color = pRod->GetFillColor();
      if(color == 10) {

	if(!kBooked) {
	  if(gTofDisplay->isSector() && !gTofDisplay->isModule()) {

	    detmin = 64 * (pRod->getSector() - 1) + 1;
	    detmax = detmin + 63;

	    sprintf(name,"%s%i%s%i","left multi_",detmin,":",detmax);
	    l_d = new TH2F(name,"TOF data",1024,0.,4096.,64,(Float_t)detmin,
			   (Float_t)(detmax+1));
	    sprintf(name,"%s%i%s%i","right multi_",detmin,":",detmax);
	    r_d = new TH2F(name,"TOF data",1024,0.,4096.,64,(Float_t)detmin,
			   (Float_t)(detmax+1));
	    sprintf(name,"%s%i%s%i","tof multi_",detmin,":",detmax);
	    t_d = new TH2F(name,"TOF data",200,-20.,40.,64,(Float_t)detmin,
			   (Float_t)(detmax+1));
	    sprintf(name,"%s%i%s%i","xpos multi_",detmin,":",detmax);
	    x_d = new TH2F(name,"TOF data",200,-150.,150.,64,(Float_t)detmin,
			   (Float_t)(detmax+1));
	    kBooked = kTRUE;
	  }

	  if(gTofDisplay->isSector() && gTofDisplay->isModule() && 
	     !gTofDisplay->isRod()) {

	    detmin = 64 * (pRod->getSector() - 1) + 
	      8 * (pRod->getModule() - 1) + 1;
	    detmax = detmin + 7;

	    //cout << "detmin = " << detmin << "  detmax = " << detmax << endl;

	    sprintf(name,"%s%i%s%i","left multi_",detmin,":",detmax);
	    l_d = new TH2F(name,"TOF data",1024,0.,4096.,8,(Float_t)detmin,
			   (Float_t)(detmax+1));
	    sprintf(name,"%s%i%s%i","right multi_",detmin,":",detmax);
	    r_d = new TH2F(name,"TOF data",1024,0.,4096.,8,(Float_t)detmin,
			   (Float_t)(detmax+1));
	    sprintf(name,"%s%i%s%i","tof multi_",detmin,":",detmax);
	    t_d = new TH2F(name,"TOF data",200,-20.,40.,8,(Float_t)detmin,
			   (Float_t)(detmax+1));
	    sprintf(name,"%s%i%s%i","xpos multi_",detmin,":",detmax);
	    x_d = new TH2F(name,"TOF data",200,-150.,150.,8,(Float_t)detmin,
			   (Float_t)(detmax+1));
	    kBooked = kTRUE;
	  }
	}

	if(kBooked) {
	  l_d->GetXaxis()->SetTitle("left time [ch]");
	  l_d->GetYaxis()->SetTitle("detector");
	  r_d->GetXaxis()->SetTitle("right time [ch]");
	  r_d->GetYaxis()->SetTitle("detector");
	  t_d->GetXaxis()->SetTitle("tof [ns]");
	  t_d->GetYaxis()->SetTitle("detector");
	  x_d->GetXaxis()->SetTitle("x [cm]");
	  x_d->GetYaxis()->SetTitle("detector");
	}
	
	sec = pRod->getSector() - 1;
	mod = pRod->getModule() - 1;
	rod = pRod->getStrip() - 1;

	index = 64*sec + 8*mod + rod;

	pTofHisto = (HTofHisto*) gTofDisplay->getHistoCont()->At(index);

	if(pTofHisto) {
	  if(gTofDisplay->isRaw()) {
	    importHist(pTofHisto->left);
	    importHist(pTofHisto->right);
	    //       	    importHist(pTofHisto->halfs);
       	    //importHist(pTofHisto->halfd);
	  }
	  if(gTofDisplay->isHit()) { 
	    importHist(pTofHisto->tof);
	    importHist(pTofHisto->xpos);
	  }
	  if(gTofDisplay->isMulti() && kBooked) {
	    for(k=1;k<=1024;k++) {
	      l_d->Fill((Float_t)(k-1)*lr_ratio,index+1,
			pTofHisto->left->GetBinContent(k));
	      r_d->Fill((Float_t)(k-1)*lr_ratio,index+1,
			pTofHisto->right->GetBinContent(k));
	    }
	    for(k=1;k<=200;k++) {
	      //	      t_d->Fill((Float_t)(k-21)*t_ratio,index+1,
	      //	pTofHisto->tof->GetBinContent(k));
	      t_d->Fill((Float_t)(t_d->GetXaxis()->GetBinCenter(k)),index+1,
			pTofHisto->tof->GetBinContent(k));
	      //	      x_d->Fill((Float_t)(k-151)*x_ratio,index+1,
	      //			pTofHisto->xpos->GetBinContent(k));
	      x_d->Fill((Float_t)(x_d->GetXaxis()->GetBinCenter(k)),index+1,
			pTofHisto->xpos->GetBinContent(k));
	    }
	    count++;
	  }
	  if((count == 8 && gTofDisplay->isModule() && 
	      gTofDisplay->isSector() && !gTofDisplay->isRod()) ||
	     (count == 64 && gTofDisplay->isSector() && 
	      !gTofDisplay->isModule())) {
	    importHist((TH1F*)l_d);
	    importHist((TH1F*)r_d);
	    importHist((TH1F*)t_d);
	    importHist((TH1F*)x_d);
	  }
	}
      }
    }
  }

  if(gTofDisplay->isMulti() && !gTofDisplay->isSector() && 
     !gTofDisplay->isModule() && !gTofDisplay->isRod()) {
    for(i=394; i<409; i++) {
      //    for(i=384; i<394; i++) {
      importHist((TH1F*)gTofDisplay->getHistoCont()->At(i));
    }
  }

}


Bool_t HTofHistAction::importHist(TH1F *hist) {

  if(!hist) return kFALSE;
  if(position==1000) return kFALSE;
  fHisto->AddAt(hist,position++);
  fListBoxA->AddEntry(hist->GetName(),++totalA);
  fListBoxA->MapSubwindows();
  fListBoxA->Layout();
  return kTRUE;

}



Bool_t HTofHistAction::importFromFile(char *filename) {

//-------------------------------------------------------------------
//
//                 importFromFile(char *filename)
//
// Imports histogrammes from a file with the name "filename".
// Opens the file, scans it, if finds an object of the class TH1F or TH2F, 
// imports it.
// All the other objects are ignored.
// In case of not a ROOT file returns an error message and takes no further 
// action.
// This function is called when a user doubly clicks on the file icon
// in the file list view.
//
//-------------------------------------------------------------------- 

  Int_t retval;
  Int_t l;
  TFile *f;
  TH1F *fH;
  f = new TFile(filename);
  if(!f) return kFALSE;
  if(f->IsZombie()) {
    new TGMsgBox(fClient->GetRoot(), this, "Error Message",
		 "You have chosen not a ROOT file. Please, be attentive.",
		 kMBIconExclamation,kMBOk,&retval);
    delete f;
    f = NULL;
    return kFALSE;
  }
  TKey *key;
  TIter it(f->GetListOfKeys());
  while((key = (TKey*) it())) {
    if(!strcmp(key->GetClassName(),"TH1F") || 
       !strcmp(key->GetClassName(),"TH2F")) {
      fH = (TH1F*) key->ReadObj();
      if(fH && position<1000) fHisto->AddAt(fH,position++);
    }
  }
  for(l=totalA; l<position; l++) {
    fListBoxA->AddEntry(fHisto->At(l)->GetName(),l+1);
  }
  fListBoxA->MapSubwindows();
  fListBoxA->Layout();
  totalA = position;
  return kTRUE;
}



int HTofHistAction::getNextTrueIndex() {

//---------------------------------------------------------------
//
//                 getNextTrueIndex()
//
// Iterates over array "flags", returns the next "TRUE index".
// In case of no "TRUE index" found returns -1.
// "TRUE index" means the index of a histogram currently highlighted in 
// the large list box (ListBoxA).
//
//---------------------------------------------------------------

  while(cursorIter < 1000) {
    cursorIter++;
    if(flags[cursorIter]) return cursorIter;
  }
  return -1;
}



void HTofHistAction::setCanvasDivision(Int_t number) {

//---------------------------------------------------------------
//
//              setCanvasDivision(Int_t number)
//
// Calculates the xDiv and yDiv parameters which are used to divide
// the main canvas (CanvasA) into subpads.
// This function is used in case of the automatic display layout
// (checkbutton "Multiple selection" is engaged).
// The function takes as an input parameter the total number of histogrammes
// to be displayed.
// Called from drawHist()
//
//---------------------------------------------------------------

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



void HTofHistAction::drawHist() {

//-----------------------------------------------------------------
//
//                drawHist()
//
// Draws a set of histogrammes in the canvas cA in case of the automatic 
// display layout (checkbutton "Multiple selection" is engaged). 
// Called when the button "Draw" is clicked.
//
//----------------------------------------------------------------

  Int_t number;  //number of highlighted histos in ListBoxA
  Int_t i;
  TH1F *histogram;
  number = 0;
  resetIter();
  while(getNextTrueIndex() != -1) number++;
  setCanvasDivision(number);
  cA->Clear();
  cA->Divide(xDiv,yDiv);
  resetIter();
  for(i = 0; i < number; i++) {
    cA->cd(i+1);
    histogram = (TH1F*) fHisto->At(getNextTrueIndex());
    if(histogram->InheritsFrom("TH2F")) histogram->Draw("SURF2");
    else histogram->Draw();
    //    ((TH1F*) fHisto->At(getNextTrueIndex()))->Draw();
  }
  cA->cd();
  cA->Modified();
  cA->Update();
}



void HTofHistAction::toScan() {

//---------------------------------------------------------------
//
//                     toScan()
//
// Makes a clone of a selected histo. 
// Puts the clone object to the scan panel (panel B).
// Called when the button "Import" is clicked.
//
//---------------------------------------------------------------

  Int_t retval;
  Int_t chan;
  Float_t from, to;
  Int_t entries;
  TObject *p;
  Text_t tmp[20];
  Int_t i;
  Float_t bcont;

  clearScan();
  TIter it(gPad->GetListOfPrimitives());
  while((p = it())) {
    if(!strcmp(p->ClassName(),"TH1F")) hist = (TH1F*) p->Clone();
  }
  if(!hist) {
    new TGMsgBox(fClient->GetRoot(), this, "Help message",
  "Choose a histogram by clicking on its area the middle button of the mouse.",
		 kMBIconExclamation,kMBOk,&retval);
    return;
  }
  cB->cd();
  if(hist) {

    hist->Draw();
    chan = hist->GetXaxis()->GetNbins();
    from = hist->GetXaxis()->GetXmin();
    to = hist->GetXaxis()->GetXmax();
    entries = (Int_t) hist->GetEntries();

    sprintf(tmp,"%s",hist->GetName());
    fNameBuf->Clear();
    fNameBuf->AddText(0,tmp);
    fClient->NeedRedraw(fName);

    sprintf(tmp,"%s",hist->GetTitle());
    fTitleBuf->Clear();
    fTitleBuf->AddText(0,tmp);
    fClient->NeedRedraw(fTitle);

    sprintf(tmp,"%i",chan);
    fChanBuf->Clear();
    fChanBuf->AddText(0,tmp);
    fClient->NeedRedraw(fChan);

    sprintf(tmp,"%.1f%s%.1f",from,"...",to);
    fRangeBuf->Clear();
    fRangeBuf->AddText(0,tmp);
    fClient->NeedRedraw(fRange);

    sprintf(tmp,"%i",entries);
    fEntriesBuf->Clear();
    fEntriesBuf->AddText(0,tmp);
    fClient->NeedRedraw(fEntries);

    array = new Int_t[chan];
    for(i=1; i<=chan; i++) {
      bcont = (Float_t) hist->GetBinContent(i);
      if(bcont > 0.0) {
	sprintf(tmp,"%i",i);
	array[totalB] = i;
	fListBoxB->AddEntry(tmp,++totalB);
      }
    }
    fListBoxB->MapSubwindows();
    fListBoxB->Layout();
  }
  cB->Update();
}



void HTofHistAction::processBoxB(Int_t par) {

//-----------------------------------------------------------
//
//        processBoxB(Int_t par)
//
// Processes information from the listbox "Non empty bins" (ListBoxB).
// Updates the fields "Bin content" and "Bin range".
// Higlights the selected bin on the canvas cB.
// Called when an item in the listbox "Non empty bins" is highlighted.
// Input parameter - bin index.
//
//------------------------------------------------------------

  Int_t chan;
  Float_t from, to;
  Float_t bfrom, bto;
  Text_t tmp[20];
  Float_t bcont;

  if(temph) {
    delete temph;
    temph = NULL;
  }
  if(hist) {
    chan = hist->GetXaxis()->GetNbins();
    from = hist->GetXaxis()->GetXmin();
    to = hist->GetXaxis()->GetXmax();
    temph = new TH1F("h","h",chan,from,to);
    temph->SetLineColor(kRed);
    temph->SetFillColor(kRed);
    temph->SetBinContent(array[par-1],hist->GetBinContent(array[par-1]));
    cB->cd();
    temph->Draw("SAME");
    
    bcont = (Float_t) hist->GetBinContent(array[par-1]);
    sprintf(tmp,"%.2f",bcont);
    fBinContBuf->Clear();
    fBinContBuf->AddText(0,tmp);
    fClient->NeedRedraw(fBinCont);
    
    bfrom = hist->GetBinLowEdge(array[par-1]);
    bto = bfrom + hist->GetBinWidth(array[par-1]);
    sprintf(tmp,"%.2f%s%.2f",bfrom,"...",bto);
    fBinRangeBuf->Clear();
    fBinRangeBuf->AddText(0,tmp);
    fClient->NeedRedraw(fBinRange);
  }
  cB->Update();
}



void HTofHistAction::clearScan() {

//-----------------------------------------------------------
//
//              clearScan()
//
// Clears all the fields in the scan panel (panel B).
//
//-----------------------------------------------------------


  TVirtualPad *padsav = gPad;

  fNameBuf->Clear();
  fClient->NeedRedraw(fName);

  fTitleBuf->Clear();
  fClient->NeedRedraw(fTitle);

  fChanBuf->Clear();
  fClient->NeedRedraw(fChan);

  fRangeBuf->Clear();
  fClient->NeedRedraw(fRange);

  fEntriesBuf->Clear();
  fClient->NeedRedraw(fEntries);

  fBinContBuf->Clear();
  fClient->NeedRedraw(fBinCont);

  fBinRangeBuf->Clear();
  fClient->NeedRedraw(fBinRange);

  if(hist) {
    delete hist;
    hist = NULL;
  }

  if(temph) {
    delete temph;
    temph = NULL;
  }

  if(totalB) {
    fListBoxB->RemoveEntries(1,totalB);
    totalB = 0;
    fListBoxB->MapSubwindows();
    fListBoxB->Layout();
  }

  cursB = 0;

  if(array) {
    delete [] array;
    array = NULL;
  }

  cB->Clear();
  cB->Update();

  gPad = padsav;
}



Bool_t HTofHistAction::toDefault(Window_t id) {

//------------------------------------------------------------------
//
//      toDefault(Window_t id)
//
// Changes the color attributes of a window to default values (gray).
// Used to change the colors of the buttons in the panel "Display Layout".
// Does not redraw the button.
// So to visually change the color needs to be followed by
// the function NeedRedraw(TGWindow *).
// Input paramater - ID of the button to which the action must be applied.
//
//------------------------------------------------------------------


  SetWindowAttributes_t wattr;
  wattr.fMask = kWABackPixel;
  wattr.fBackgroundPixel = fgDefaultFrameBackground;
  gVirtualX->ChangeWindowAttributes(id, &wattr);
  return kTRUE;

}



Bool_t HTofHistAction::toGreen(Window_t id) {

//-------------------------------------------------------------
//
//       toGreen(Window_t id)
//
// The same as above except changing the color to green.
//
//-------------------------------------------------------------


  SetWindowAttributes_t wattr;
  wattr.fMask = kWABackPixel;
  gClient->GetColorByName("green", wattr.fBackgroundPixel);
  gVirtualX->ChangeWindowAttributes(id, &wattr);
  return kTRUE;

}



Bool_t HTofHistAction::isOverlap() {

//-------------------------------------------------------------
//
//         isOverlap()
//
// Checks if a selected display layout overlaps with already existing
// pads in the canvas cA.
//
//-------------------------------------------------------------


  Int_t i,j;
  Int_t tmpIndex;

  for(i=0; i<4; i++) {
    for(j=0; j<4; j++) {
      if(verLay[i]&&horLay[j]) {
	tmpIndex = 4*i + j;
	if(histInd[tmpIndex] != -1) return kTRUE;
      }
    }
  }
  return kFALSE;

}



Bool_t HTofHistAction::isLayout() {

//-------------------------------------------------------
//
//        isLayout()
//
// Checks if display layout is set.
//
//--------------------------------------------------------


  Int_t i;
  for(i=0;i<4;i++) {
    if(horLay[i] != 0) return kTRUE;
    if(verLay[i] != 0) return kTRUE;
  }
  return kFALSE;

}




void HTofHistAction::paintHist() {

//--------------------------------------------------------------------
//
//              paintHist()
//
// Draws a histo in the canvas cA in case of the user defined display layout.
// The latest display layout has the highest priority. If an overlap
// with existing pads is detected, they are deleted from cA.
// Algorithm virtually divides cA into subpads with the matrix layout (4x4).
// A real pad in which histo will be drawn is constructed from virtual subpads.
// The number of virtual subpads for the real pad can change in the range 1-16.
// Arrays histInd[16] and pads[16] keep the "id" of the histo and the 
// address of the real pad
//
//            -----------------
//            |   |   |   |   |
//            | 1 | 2 | 3 | 4 |
//            |---|---|---|---|
//            |   |   |   |   |
//            | 5 | 6 | 7 | 8 |
//            |---|---|---|---|
//            |   |   |   |   |
//            | 9 | 10| 11| 12|
//            |---|---|---|---|
//            |   |   |   |   |
//            | 13| 14| 15| 16|
//            -----------------
//
//
// If a histo with id=20 must be drawn in a pad which embraces virtual subpads
// 1,2,5,6 then 
//              histInd[0] = 20        pads[0] = address of the real pad
//              histInd[1] = 20        pads[1] = NULL
//              histInd[4] = 20        pads[4] = NULL
//              histInd[5] = 20        pads[5] = NULL
//
// To search for the pads to be deleted the algorithm uses only array 
// histInd[].
// Only one of the virtual subpads of the real pad keeps the address 
// to avoid double deleting of the same object.
// If there is an overlap between the pads which contain the histo with 
// the same "id", then only the latest version is drawn. 
// All the other pads with this histo (even non overlapping with the current 
// one) will be deleted from the canvas. 
// To have several versions of the same histo drawn in the canvas one has
// to avoid pads overlapping when setting display layout.
//--------------------------------------------------------------------

  Int_t retval;
  Axis_t xstart, xstop, ystart, ystop;
  Int_t binFirst, binLast;
  Float_t max;
  Float_t xmin = 0.0F;
  Float_t xmax = 0.0F;
  Float_t ymin = 0.0F;
  Float_t ymax = 0.0F;
  Int_t i,j,countLocal;
  Int_t ind;
  Int_t tempind;
  TPad *pad;
  TH1F *histogram;
  const Float_t ratio = 0.25;

  if(!isLayout()) {
    new TGMsgBox(fClient->GetRoot(), this, "Message",
		 "Set Display Layout.",
		 kMBIconExclamation,kMBOk,&retval);
    return;
  }
  resetIter(); 
  ind = getNextTrueIndex();
  for(i=0; i<4; i++) {
    if(horLay[i]&&(xmin==0.0)) xmin = i*ratio + 0.01;
    if(horLay[i]&&(xmin!=0.0)) xmax = (i+1)*ratio - 0.01;
  }
  for(i=3; i>-1; i--) {
    if(verLay[i]&&(ymin==0.0)) ymin = (3-i)*ratio + 0.01;
    if(verLay[i]&&(ymin!=0.0)) ymax = (4-i)*ratio - 0.01;
  }
  if(isOverlap()) {
    for(i=0; i<16; i++) {
      if(verLay[i/4] && horLay[i%4]) {
	tempind = histInd[i];
	for(j=0; j<16; j++) {
	  if(histInd[j] == tempind) {
	    histInd[j] = -1;
	    if(pads[j]) {
	      delete pads[j];
	      pads[j] = NULL;
	    }
	  }
	}
      }
    }
  }
  pad = new TPad("pad","pad",xmin,ymin,xmax,ymax);
  pad->SetFillColor(10);
  cA->cd();
  pad->Draw();
  pad->cd();
  histogram = (TH1F*) fHisto->At(ind);
  if(histogram) {
    if(!kScale) automatic(histogram);
    if(histogram->InheritsFrom("TH2F")) {
      xstart = atoi(fXstartBuf->GetString());
      xstop = atoi(fXstopBuf->GetString());
      ystart = atoi(fYstartBuf->GetString());
      ystop = atoi(fYstopBuf->GetString());
      max = atoi(fScaleBuf->GetString());
      binFirst = histogram->GetXaxis()->FindBin(xstart);
      binLast = histogram->GetXaxis()->FindBin(xstop);
      histogram->GetXaxis()->SetRange(binFirst,binLast);
      binFirst = histogram->GetYaxis()->FindBin(ystart);
      binLast = histogram->GetYaxis()->FindBin(ystop);
      histogram->GetYaxis()->SetRange(binFirst,binLast);
      histogram->SetMaximum(max + 0.1*max);
      histogram->Draw("SURF2");
    }
    else {
      xstart = atoi(fXstartBuf->GetString());
      xstop = atoi(fXstopBuf->GetString());
      max = atoi(fScaleBuf->GetString());
      binFirst = histogram->GetXaxis()->FindBin(xstart);
      binLast = histogram->GetXaxis()->FindBin(xstop);
      histogram->GetXaxis()->SetRange(binFirst,binLast);
      histogram->SetMaximum(max + 0.1*max);
      histogram->Draw();
    }
    //    kScale = kFALSE;
  }
  //  if(fHisto->At(ind)) ((TH1F*) fHisto->At(ind))->Draw();
  cA->cd();
  cA->Modified();
  cA->Update();

  countLocal = 0;
  for(i=0; i<4; i++) {
    for(j=0; j<4; j++) {
      if(verLay[i]&&horLay[j]) {
	countLocal++;
	histInd[4*i+j] = ind;
	if(countLocal==1) pads[4*i+j] = pad;
	else pads[4*i+j] = NULL;
      }
    }
  }
  return;
}


void HTofHistAction::automatic(TH1F *hist) {

  if(!hist) return;

  Text_t str[15];

  sprintf(str,"%.1f",hist->GetXaxis()->GetXmin());
  fXstartBuf->Clear();
  fXstartBuf->AddText(0,str);
  fClient->NeedRedraw(fXstart);
  sprintf(str,"%.1f",hist->GetXaxis()->GetXmax());
  fXstopBuf->Clear();
  fXstopBuf->AddText(0,str);
  fClient->NeedRedraw(fXstop);
  sprintf(str,"%.1f",hist->GetYaxis()->GetXmin());
  fYstartBuf->Clear();
  fYstartBuf->AddText(0,str);
  fClient->NeedRedraw(fYstart);
  sprintf(str,"%.1f",hist->GetYaxis()->GetXmax());
  fYstopBuf->Clear();
  fYstopBuf->AddText(0,str);
  fClient->NeedRedraw(fYstop);
  hist->SetMaximum();
  sprintf(str,"%.1f",hist->GetMaximum());
  fScaleBuf->Clear();
  fScaleBuf->AddText(0,str);
  fClient->NeedRedraw(fScale);

}


Bool_t HTofHistAction::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2) {

//------------------------------------------------------------------
//
//    ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2)
//
// Processes information from all GUI items.
// Selecting an item usually generates an event with 4 parameters.
// The first two are packed into msg (first and second bytes).
// The other two are parm1 and parm2.
//
//------------------------------------------------------------------

Int_t retval; //for class TGMsgBox
Int_t buttons;//used to construct message panel when Close button is clicked 
Int_t numb;//to update layout of list boxes
Int_t i,j;
Int_t imin, imax;//to calculate display layout
Int_t jmin, jmax;//to calculate display layout
Text_t tmp[50], filename[50], ext[5];//to process information in file container
TGFileItem *f;//the same as above
void *p = NULL;//the same as above
Text_t command[50];//the same as above
TGFileInfo fi; // to save a picture
TFile *file;
TH1F *hi;
TH1F *his;
Text_t str[10];
char savefilename[100];
const char *filetypes[] = { "Postscript files",     "*.ps",
			    "Gif files",    "*.gif",
			    "Root files",   "*.root",
			    0,               0 };
fi.fFileTypes = (const char **)filetypes;  


switch(GET_MSG(msg)) {
 case kC_COMMAND:

  switch(GET_SUBMSG(msg)) {

  case kCM_BUTTON:

    switch(parm1) {
    case M_DRAW:
      if(!totalA) {
	new TGMsgBox(fClient->GetRoot(), this, "Error message",
		     "Histo container is empty.",
		     kMBIconExclamation,kMBOk,&retval);
	break;
      }

      if(!kSTATUS) {
        new TGMsgBox(fClient->GetRoot(), this, "Help message",
                     "Highlight the name of the histogram to be displayed.",
                     kMBIconExclamation,kMBOk,&retval);
	break;
      }

      if(parm2 != 10) {
	kScale = kFALSE;
	toDefault(fScaleStat->GetId());
	fClient->NeedRedraw(fScaleStat);
      }
      if(fMultiButton->GetState()) drawHist();//Automatic display layout
      else {
	paintHist(); //User defined display layout

// Total number of buttons which can be set in "Display Layout" panel by a user

	count = 2;
      }
      break;

    case M_SCALE_HIST:
      if(fListBoxA->GetMultipleSelections()) break;
      kScale = kTRUE;
      toGreen(fScaleStat->GetId());
      fClient->NeedRedraw(fScaleStat);
      SendMessage(this, MK_MSG(kC_COMMAND,kCM_BUTTON), M_DRAW, 10);
      break;

    case M_SCALE_STAT:
      kScale = !kScale;
      if(kScale) toGreen(fScaleStat->GetId());
      else toDefault(fScaleStat->GetId());
      fClient->NeedRedraw(fScaleStat);
      break;

    case M_CLEAR_A:
      for(Int_t k=0; k<16; k++) {
	histInd[k] = -1;
	if(pads[k]) delete pads[k];
	pads[k] = NULL;
      }
      cA->cd();
      cA->Clear();
      cA->Update();
      break;

    case M_SAVE_PIC:

      new TGFileDialog(fClient->GetRoot(), this, kFDSave, &fi);
      sprintf(savefilename,"%s",fi.fFilename);
      if(!strcmp(savefilename,"(null)")) break;
      sprintf(savefilename,"%s%s%s",
	      gSystem->WorkingDirectory(),"/",fi.fFilename);
      cA->cd();
      cA->Print(savefilename);
      fFileCont->DisplayDirectory();
      break;


    case M_SAVE_HIST:
      new TGFileDialog(fClient->GetRoot(), this, kFDSave, &fi);
      sprintf(savefilename,"%s",fi.fFilename);
      if(!strcmp(savefilename,"(null)")) break;
      sprintf(savefilename,"%s%s%s",
	      gSystem->WorkingDirectory(),"/",fi.fFilename);
      file = new TFile(savefilename,"RECREATE");
      for(i=0; i<1000; i++) {
	hi = (TH1F*) fHisto->At(i);
	if(hi) hi->Write();
      }
      file->Close();
      delete file;
      break;

    case M_EDIT:
      cA->cd();
      cA->EditorBar();
      break;

    case M_IMPORT:

// Put a histo to the Scan panel

      toScan();
      break;

    case M_CLEAR_B:
      clearScan();
      break;

    case M_CLOSE_HIST:
      retval = 0;
      buttons = 0;
      buttons |= kMBYes;
      buttons |= kMBNo;
      //      new TGMsgBox(fClient->GetRoot(), this, "Confirm action",
      //		   "Close Panel 'HISTO ACTION' ?",
      //		   kMBIconQuestion,buttons,&retval);
      retval = 1;
      if(retval == 1) CloseWindow();
      break;

    case M_PREV_B:

// One bin up in ListBoxB (non empty bins)

      if(totalB > 0) {
	if((cursB > 0) && (cursB <= totalB)) cursB--;
	if(cursB < 1) cursB = 1;
	if(cursB > totalB) cursB = totalB;
	fListBoxB->Select(cursB);
	numb = cursB;
	while(((--numb)%5) != 0);
	fListBoxB->SetTopEntry(++numb);
	SendMessage(this,MK_MSG(kC_COMMAND, kCM_LISTBOX),M_LIST_B,cursB);
      }
      break;

    case M_NEXT_B:

// One bin down in ListBoxB (non empty bins)

      if(totalB > 0) {
	if((cursB > 0) && (cursB <= totalB)) cursB++;
	if(cursB < 1) cursB = 1;
	if(cursB > totalB) cursB = totalB;
	fListBoxB->Select(cursB);
	numb = cursB;
	while(((--numb)%5) != 0);
	fListBoxB->SetTopEntry(++numb);
	SendMessage(this,MK_MSG(kC_COMMAND, kCM_LISTBOX),M_LIST_B,cursB);
      }
      break;

    case M_PREV_A:

// One histo up in ListBoxA

      if(!fMultiButton->GetState() && (totalA > 0)) {
	if((cursA > 0) && (cursA <= totalA)) cursA--;
	if(cursA < 1) cursA = 1;
	if(cursA > totalA) cursA = totalA;
	fListBoxA->Select(cursA);
	numb = cursA;
	while(((--numb)%10) != 0);
	fListBoxA->SetTopEntry(++numb);
	SendMessage(this,MK_MSG(kC_COMMAND, kCM_LISTBOX),M_LIST_A,cursA);
	if(kScale) SendMessage(this,MK_MSG(kC_COMMAND, kCM_BUTTON),M_DRAW,10);
	else SendMessage(this,MK_MSG(kC_COMMAND, kCM_BUTTON),M_DRAW,0);
      }
      break;

    case M_NEXT_A:

// One histo down in ListBoxA

      if(!fMultiButton->GetState() && (totalA > 0)) {
	if((cursA > 0) && (cursA <= totalA)) cursA++;
	if(cursA < 1) cursA = 1;
	if(cursA > totalA) cursA = totalA;
	fListBoxA->Select(cursA);
	numb = cursA;
	while(((--numb)%10) != 0);
	fListBoxA->SetTopEntry(++numb);
	SendMessage(this,MK_MSG(kC_COMMAND, kCM_LISTBOX),M_LIST_A,cursA);
	if(kScale) SendMessage(this,MK_MSG(kC_COMMAND, kCM_BUTTON),M_DRAW,10);
	else SendMessage(this,MK_MSG(kC_COMMAND, kCM_BUTTON),M_DRAW,0);
      }
      break;

    case M_SELECT:

// "Select All" button is clicked

      fMultiButton->SetState(kButtonDown);
      SendMessage(this,MK_MSG(kC_COMMAND, kCM_CHECKBUTTON),M_MULTI,0);

// Call twice SetMultipleSelections(kTRUE), otherwise items in the list box 
// are not highlighted (though proper functionality remains)

      fListBoxA->SetMultipleSelections(kTRUE);
      for(i=1; i<=totalA; i++) {
	fListBoxA->Select(i);
	SendMessage(this,MK_MSG(kC_COMMAND, kCM_LISTBOX),M_LIST_A,i);
      }
      break;

    case M_CDUP:

// To the parent directory 

      gSystem->ChangeDirectory("..");
      sprintf(tmp,"%s",gSystem->WorkingDirectory());
      fDir->RemoveEntry(1);
      fDir->AddEntry(tmp,1);
      fDir->MapSubwindows();
      fDir->Layout();
      fFileCont->DisplayDirectory();//otherwise the current directory is
                                    //not always updated
      break;

    case M_LIST_MODE:

// "List" mode of file view

      fFileView->SetViewMode(kLVList);
      fDetailMode->SetState(kButtonUp);
      break;

    case M_DETAIL_MODE:

// "Details" mode of file view

      fFileView->SetViewMode(kLVDetails);
      fListMode->SetState(kButtonUp);
      break;

    default:

// Process the panel "Display Layout"

      if(parm1>=500 && parm1<=515 && !fMultiButton->GetState()) {
	if(count==2) count = 0;
	if(count<2) {
	  toGreen(fLayoutButton[parm1-500]->GetId());
	  verLay[(parm1-500)/4] = 1;
	  horLay[(parm1-500)%4] = 1;
	  fClient->NeedRedraw(fLayoutButton[parm1-500]);
	  indi[count] = (parm1-500)/4;
	  indj[count] = (parm1-500)%4;
	  count++;
	  if(count==2) {
	    imin = (indi[0]<indi[1]) ? indi[0] : indi[1];
	    imax = (indi[0]>indi[1]) ? indi[0] : indi[1];
	    jmin = (indj[0]<indj[1]) ? indj[0] : indj[1];
	    jmax = (indj[0]>indj[1]) ? indj[0] : indj[1];
	    for(i=0;i<4;i++) {
	      for(j=0;j<4;j++) {
		if(i>=imin && i<=imax && j>=jmin && j<=jmax) {
		  toGreen(fLayoutButton[4*i+j]->GetId());
		  verLay[i] = 1;
		  horLay[j] = 1;
		}
		else {
		  toDefault(fLayoutButton[4*i+j]->GetId());
		  if(i<imin || i>imax) verLay[i] = 0;
		  if(j<jmin || j>jmax) horLay[j] = 0;
		}
		fClient->NeedRedraw(fLayoutButton[4*i+j]);
	      }
	    }
	  }
	  if(count==1) {
	    for(i=0;i<16;i++) {
	      if(i != (parm1-500)) {
		toDefault(fLayoutButton[i]->GetId());
		if(i/4 != (parm1-500)/4) verLay[i/4] = 0;
		if(i%4 != (parm1-500)%4) horLay[i%4] = 0;
		fClient->NeedRedraw(fLayoutButton[i]);
	      }
	    }
	  }
	}
      }
      break;
    }

  case kCM_CHECKBUTTON:
    
// Multiple selection

    switch(parm1) {
    case M_MULTI:
      if(!fListBoxA->GetMultipleSelections()) {
	if(fListBoxA->GetSelectedEntry()) 
	  fListBoxA->GetSelectedEntry()->Activate(kFALSE);
      }
      fListBoxA->SetMultipleSelections(fMultiButton->GetState());      
      fXstartBuf->Clear();
      fClient->NeedRedraw(fXstart);
      fXstopBuf->Clear();
      fClient->NeedRedraw(fXstop);
      fYstartBuf->Clear();
      fClient->NeedRedraw(fYstart);
      fYstopBuf->Clear();
      fClient->NeedRedraw(fYstop);
      fScaleBuf->Clear();
      fClient->NeedRedraw(fScale);

      cursA = 0;
      cA->Clear();
      cA->Update();
      for(i=0; i<16; i++) {
	toDefault(fLayoutButton[i]->GetId());
	fClient->NeedRedraw(fLayoutButton[i]);
	verLay[i/4] = 0;
	horLay[i%4] = 0;
      }
      count = 0;
      for(j=0; j<16; j++) {
	pads[j]=NULL;
	histInd[j]=-1;
      }
      for(Int_t g=0; g<1000; g++) flags[g] = kFALSE;
      kSTATUS = kFALSE;
      break;

    default:
      break;
    }

  case kCM_LISTBOX:

    switch(parm1) {
    case M_LIST_A:

// ListBoxA

      cursA = parm2; //necessary for "Previous", "Next" buttons in case of
                     //random jumps in list box window

      if(!fListBoxA->GetMultipleSelections()) {
        for(Int_t k=0; k<1000; k++) flags[k] = kFALSE;
        flags[parm2-1] = kTRUE;
	his = (TH1F*) fHisto->At(parm2-1);
	if(his&&!kScale) {
	  i = his->GetXaxis()->GetFirst();
	  sprintf(str,"%.1f",his->GetXaxis()->GetBinLowEdge(i));
	  fXstartBuf->Clear();
	  fXstartBuf->AddText(0,str);
	  fClient->NeedRedraw(fXstart);
	  i = his->GetXaxis()->GetLast();
	  sprintf(str,"%.1f",his->GetXaxis()->GetBinUpEdge(i));
	  fXstopBuf->Clear();
	  fXstopBuf->AddText(0,str);
	  fClient->NeedRedraw(fXstop);
	  i = his->GetYaxis()->GetFirst();
	  sprintf(str,"%.1f",his->GetYaxis()->GetBinLowEdge(i));
	  fYstartBuf->Clear();
	  fYstartBuf->AddText(0,str);
	  fClient->NeedRedraw(fYstart);
	  i = his->GetYaxis()->GetLast();
	  sprintf(str,"%.1f",his->GetYaxis()->GetBinUpEdge(i));
	  fYstopBuf->Clear();
	  fYstopBuf->AddText(0,str);
	  fClient->NeedRedraw(fYstop);
	  sprintf(str,"%.1f",his->GetMaximum());
	  fScaleBuf->Clear();
	  fScaleBuf->AddText(0,str);
	  fClient->NeedRedraw(fScale);
	}
      } else flags[parm2-1] = !flags[parm2-1];
      kSTATUS = kTRUE;
      break;

    case M_LIST_B:

// ListBoxB (non empty bins)

      cursB = parm2; //for "Previous", "Next" buttons
      processBoxB(parm2);
      break;

    default:
      break;
    }

  case kCM_COMBOBOX:
    switch(parm1) {

    case M_FILTER:

// Set filter on the file type

      if(parm2 == 1) fFileCont->SetFilter("*");
      if(parm2 == 2) fFileCont->SetFilter("*.root");
      fFileCont->DisplayDirectory();
      break;

    default:
      break;
    }

  default:
    break;
  }

 case kC_CONTAINER:
  switch(GET_SUBMSG(msg)) {

  case kCT_ITEMDBLCLICK:

// Process mouse double clicking in file view container

    if (parm1 == kButton1) {
      if (fFileCont->NumSelected() == 1) {
        f = (TGFileItem *) fFileCont->GetNextSelected(&p);
        if (S_ISDIR(f->GetType())) {
          fFileCont->ChangeDirectory(f->GetItemName()->GetString());
          sprintf(tmp,"%s",gSystem->WorkingDirectory());
          fDir->RemoveEntry(1);
          fDir->AddEntry(tmp,1);
          fDir->MapSubwindows();
          fDir->Layout();
        } 
	else {
	  sprintf(filename,f->GetItemName()->GetString());
	  for(Int_t s=0;s<5;s++) ext[s]='\0';
	  for(Int_t s=0; filename[s] != '\0'; s++) {
	    if(filename[s] == '.') {
	      strncpy(ext,&filename[s+1],4);
	      break;
	    }
	  }

// Call gv for postscript and pdf files

	  if(!strcmp(ext,"ps") || !strcmp(ext,"PDF") || !strcmp(ext,"pdf")) {
	    sprintf(command,"%s%s%s%s%s","gv ",
		    gSystem->WorkingDirectory(),
		    "/",
		    filename,
		    "&");
	    gSystem->Exec(command);
	    break;
	  }

// Call ee for jpg, gif and bmp files

	  if(!strcmp(ext,"gif") || !strcmp(ext,"jpg") || !strcmp(ext,"bmp")) {
	    sprintf(command,"%s%s%s%s%s","xv ",
		    gSystem->WorkingDirectory(),
		    "/",
		    filename,
		    "&");
	    gSystem->Exec(command);
	    break;
	  }

// Import root file

          sprintf(filename,"%s%s%s",
		  gSystem->WorkingDirectory(),
		  "/",
		  f->GetItemName()->GetString());
          importFromFile(filename);
        }
      }
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













