/*
------------------------------------------------------------------------

 ------------------------        --------------------    -----------------------
 |       fF0            |        |                  |    |         fFD         |
 |----------------------|        |------------------|    |---------------------|
 |                      |        |     fFMain       |    |                     |
 | REAL  CALIBRATION    |        |------------------|    | Refresh  Last N evt | 
 |                      |        |                  |    |                     | 
 ------------------------        --------------------    -----------------------
                                     
 ------------------------      -------------------  -------------------
 |       fFA            |      |       fFB       |  |       fFC       |
 |----------------------|      |-----------------|  |-----------------|
 |                      |      |                 |  |                 |
 | Sec Mob MBInd MBName |      | Draw Stop Close |  | Cuts for X-axis |
 |                      |      |                 |  |                 |
 ------------------------      -------------------  -------------------

------------------------------------------------------------------------
*/
#include "mdcgui.h"
#include <TCollection.h>
#include <TPaveText.h>
#include <TROOT.h>
#include <TGMsgBox.h>
#include <TH2.h>
#include <iostream.h>

ClassImp(mdcGUI)
  
  mdcGUI::mdcGUI(const TGWindow *p, UInt_t w, UInt_t h) :
    TGMainFrame(p, w, h) {
  

  fFMain = new TGCompositeFrame(this,1800,1400,kVerticalFrame);
  AddFrame(fFMain, new TGLayoutHints(kLHintsNormal));

  fFC10 = new TGCompositeFrame(fFMain,800,100,kHorizontalFrame);
  fFMain->AddFrame(fFC10, new TGLayoutHints(kLHintsNormal));


  
  fF0 = new TGGroupFrame(fFC10," Data Type ",kHorizontalFrame);
  //  fF0 = new TGCompositeFrame(fFMain,400,50,kHorizontalFrame);
  //  fFC10->AddFrame(fF0, new TGLayoutHints(kLHintsNormal,5,15,5,0));
  fFC10->AddFrame(fF0, new TGLayoutHints(kLHintsNormal,45,15,5,3));
  fF0->AddFrame(fRad2 = new TGRadioButton(fF0, "REAL", 82), new TGLayoutHints(kLHintsNormal,12,10,12,2));
  fF0->AddFrame(fRad1 = new TGRadioButton(fF0, "CALIBRATION", 81), new TGLayoutHints(kLHintsNormal,30,5,12,2));
   fRad1->Associate(this);
   fRad2->Associate(this);
   fRad2->SetState(kButtonDown);
   fRad1->SetState(kButtonUp);
   fData = kTRUE;

   fFD = new TGGroupFrame(fFC10,"Time Refresh ",kHorizontalFrame);
   fFC10->AddFrame(fFD, new TGLayoutHints(kLHintsNormal,35,15,5,3));

   fTimeRefr = new TGCheckButton(fFD, "  Refresh in", M_TR);
   fTimeRefr->Associate(this);
   fFD->AddFrame(fTimeRefr, new TGLayoutHints(kLHintsNormal, 10,5,12,2));
   fRefreshTime = new TGTextEntry(fFD, fRefreshTimeBuf = new TGTextBuffer(20), -1);
   fRefreshTime->Resize(30, fRefreshTime->GetDefaultHeight()); 
   fRefreshTime->SetState(kFALSE);
   fFD->AddFrame(fRefreshTime, new TGLayoutHints(kLHintsNormal, 0, 5, 7, 2)); 
   fFD->AddFrame(new TGLabel(fFD,new TGHotString("seconds")), 
		new TGLayoutHints(kLHintsNormal, 0, 10, 13, 2 )); 

   // Plot hists only for N last events, disactivated now

   /*   fFE = new TGGroupFrame(fFC10,"Last Events ",kHorizontalFrame);
      fFC10->AddFrame(fFE, new TGLayoutHints(kLHintsNormal,5,15,5,0));

   fLastEvt = new TGCheckButton(fFE, "  Plot last", M_LE);
   fLastEvt->Associate(this);
   fFE->AddFrame(fLastEvt, new TGLayoutHints(kLHintsNormal, 10,5,12,2));
   fLastEvtNum = new TGTextEntry(fFE, fLastEvtBuf = new TGTextBuffer(20), -1);
   fLastEvtNum->Resize(30, fLastEvtNum->GetDefaultHeight()); 
   fLastEvtNum->SetState(kFALSE);
   fFE->AddFrame(fLastEvtNum, new TGLayoutHints(kLHintsNormal, 0, 5, 7, 2)); 
   fFE->AddFrame(new TGLabel(fFE,new TGHotString("events")), 
   new TGLayoutHints(kLHintsNormal, 0, 10, 13, 2 ));  */
  


   //  fFA = new TGCompositeFrame(fFMain,400,50,kHorizontalFrame);
  fFA = new TGGroupFrame(fFMain," Detector ",kHorizontalFrame);
  fFMain->AddFrame(fFA, new TGLayoutHints(kLHintsTop|kLHintsLeft,5,5,5,5));
  
  // Composite Frame for X and Y cuts
  fCuts = new TGCompositeFrame(fFMain,800,100,kHorizontalFrame);
  fFMain->AddFrame(fCuts, new TGLayoutHints(kLHintsNormal));

  fXCuts = new TGGroupFrame(fCuts," Cuts for X-axis ",kHorizontalFrame);
  fCuts->AddFrame(fXCuts, new TGLayoutHints(kLHintsNormal, 5, 5, 5, 0 ));

  fYCuts = new TGGroupFrame(fCuts," Cuts for Y-axis ",kHorizontalFrame);
  fCuts->AddFrame(fYCuts, new TGLayoutHints(kLHintsNormal, 5, 5, 5, 0 ));

  fFB = new TGGroupFrame(fFMain," Functions ",kHorizontalFrame);
  //      fFB = new TGCompositeFrame(fFMain,400,50,kHorizontalFrame);
  fFB->SetLayoutManager(new TGMatrixLayout(fFB,0,5,25));
  fFMain->AddFrame(fFB, new TGLayoutHints(kLHintsNormal, 5, 5, 5, 5 ));
  
  fF1 = new TGCompositeFrame(fFA,400,50,kHorizontalFrame);
  fF1->SetLayoutManager(new TGMatrixLayout(fF1,0,1,2));
  fFA->AddFrame(fF1, new TGLayoutHints(kLHintsNormal,0,0,5,0));
  
  fF2 = new TGCompositeFrame(fFA,400,50,kHorizontalFrame);
  fF2->SetLayoutManager(new TGMatrixLayout(fF2,0,1,2));
  fFA->AddFrame(fF2, new TGLayoutHints(kLHintsLeft|kLHintsTop,2,2,5,0));
  
  fF3 = new TGCompositeFrame(fFA,400,50,kHorizontalFrame);
  fF3->SetLayoutManager(new TGMatrixLayout(fF3,0,1,2));
  fFA->AddFrame(fF3, new TGLayoutHints(kLHintsNormal, 2, 2, 5, 0 ));
  
  fF4 = new TGCompositeFrame(fFA,400,50,kHorizontalFrame);
  fF4->SetLayoutManager(new TGMatrixLayout(fF4,0,1,2));
  fFA->AddFrame(fF4, new TGLayoutHints(kLHintsLeft|kLHintsTop,2,2,5,0));
  
  fF5 = new TGCompositeFrame(fFA,400,50,kHorizontalFrame);
  fF5->SetLayoutManager(new TGMatrixLayout(fF5,0,1,2));
  fFA->AddFrame(fF5, new TGLayoutHints(kLHintsLeft|kLHintsTop,2,2,5,0));
  
  fF1->AddFrame(new TGLabel(fF1, new TGHotString("Sector                ")),
		new TGLayoutHints(kLHintsNormal, 0, 0, 0, 0));
  fCombo1 = new TGComboBox(fF1,M_SECTOR);
  fCombo1->Associate(this);
  char tmp1[20];
  sprintf(tmp1,"%s","3");
  fCombo1->AddEntry(tmp1,1);
  sprintf(tmp1,"%s","6");
  fCombo1->AddEntry(tmp1,2);
  fCombo1->Resize(70,20);
  fCombo1->Select(1);
  fF1->AddFrame(fCombo1,new TGLayoutHints(kLHintsNormal, 0, 0, 10, 0));
  
  fF2->AddFrame(new TGLabel(fF2, new TGHotString("Module                ")),
		new TGLayoutHints(kLHintsNormal, 0, 0, 0, 0));
  fCombo2 = new TGComboBox(fF2,M_MODULE);
  fCombo2->Associate(this);
  char tmp2[20];
  sprintf(tmp2,"%s","1");
  fCombo2->AddEntry(tmp2,1);
  sprintf(tmp2,"%s","2");
  fCombo2->AddEntry(tmp2,2);
  fCombo2->Resize(70,20);
  fCombo2->Select(2);
  fF2->AddFrame(fCombo2);
  
  fF3->AddFrame(new TGLabel(fF3, new TGHotString("MBO Index             " )),
		new TGLayoutHints(kLHintsNormal, 0, 0, 0, 0));
  fCombo3 = new TGComboBox(fF3,M_MBIND);
  fCombo3->Associate(this);
  char tmp3[20];
  sprintf(tmp3, "%s", "all");
  fCombo3->AddEntry(tmp3, 0);
  for (i=0; i<16; i++) {
    sprintf(tmp3, "%i", i);
    fCombo3->AddEntry(tmp3, i+1);
  }
  fCombo3->Resize(70,20);
  fCombo3->Select(0);
  fF3->AddFrame(fCombo3);
  
  fF4->AddFrame(new TGLabel(fF4, new TGHotString("MBO Name              ")),
		new TGLayoutHints(kLHintsNormal, 0, 0, 0, 0));
  fCombo4 = new TGComboBox(fF4,M_MBNAME);
  fCombo4->Associate(this);
  char tmp4[20];
  sprintf(tmp4, "%s", "all");
  fCombo4->AddEntry(tmp4, 0);
  // Not so good, should be initilised automatically
  Int_t table[16]={111,112,121,122,123,124,211,212,
		  221,222,223,224,311,312,321,322};
  for (i=0; i<16; i++) {
    sprintf(tmp4, "%i", table[i]);
    fCombo4->AddEntry(tmp4, i+1);
  }
  fCombo4->Resize(70,20);
  fCombo4->Select(0);
  fF4->AddFrame(fCombo4);

  // Combo for TDC number

  fF5->AddFrame(new TGLabel(fF5, new TGHotString("TDC                   ")),
		new TGLayoutHints(kLHintsNormal, 0, 0, 0, 0));
  fCombo5 = new TGComboBox(fF5,M_TDCNUM);
  fCombo5->Associate(this);
  sprintf(tmp1, "%s", "  ");
  fCombo5->AddEntry(tmp1, 0);
  fCombo5->Resize(70,20);
  fCombo5->Select(0);
  fF5->AddFrame(fCombo5);




  // fXCuts

  fXCuts->AddFrame(new TGLabel(fXCuts,new TGHotString("X-Min")), 
		new TGLayoutHints(kLHintsNormal, 10, 2, 8, 2 )); 
  
  fRangeXMin = new TGTextEntry(fXCuts, fRangeBufXMin = new TGTextBuffer(100), -1); 
  fXCuts->AddFrame(fRangeXMin, new TGLayoutHints(kLHintsNormal, 16, 2, 8, 2)); 
  fRangeXMin->Resize(60, fRangeXMin->GetDefaultHeight()); 

  fXCuts->AddFrame(new TGLabel(fXCuts,new TGHotString("X-Max")), 
		new TGLayoutHints(kLHintsNormal, 17,2, 8, 2 )); 
  
  fRangeXMax = new TGTextEntry(fXCuts, fRangeBufXMax = new TGTextBuffer(100), -1); 
  fXCuts->AddFrame(fRangeXMax, new TGLayoutHints(kLHintsNormal, 16, 2, 8, 2)); 
  fRangeXMax->Resize(60, fRangeXMax->GetDefaultHeight()); 


  // fYCuts

  fYCuts->AddFrame(new TGLabel(fYCuts,new TGHotString("Y-Min")), 
		new TGLayoutHints(kLHintsNormal, 10, 2, 8, 2 )); 
  
  fRangeYMin = new TGTextEntry(fYCuts, fRangeBufYMin = new TGTextBuffer(100), -1); 
  fYCuts->AddFrame(fRangeYMin, new TGLayoutHints(kLHintsNormal, 16, 2, 8, 2)); 
  fRangeYMin->Resize(60, fRangeYMin->GetDefaultHeight()); 

  fYCuts->AddFrame(new TGLabel(fYCuts,new TGHotString("Y-Max")), 
		new TGLayoutHints(kLHintsNormal, 17,2, 8, 2 )); 
  
  fRangeYMax = new TGTextEntry(fYCuts, fRangeBufYMax = new TGTextBuffer(100), -1); 
  fYCuts->AddFrame(fRangeYMax, new TGLayoutHints(kLHintsNormal, 16, 2, 8, 2)); 
  fRangeYMax->Resize(60, fRangeYMax->GetDefaultHeight()); 

  // TDC:DriftTime button

  fTDCDTButton = new TGTextButton(fFB, "TDC:DriftTime", M_TDCDT);
  fTDCDTButton->Associate(this);
  fTDCDTButton->SetToolTipText("TDC chip*8 + TDC channel : Drift Time");
  fFB->AddFrame(fTDCDTButton,new TGLayoutHints(kLHintsNormal,30,4,4,4));
  fTDCDTButton->Resize(95, 25);


  // Menu for Drift Times

  fTDMenu = new TGPopupMenu(fClient->GetRoot());
  fTDMenu->AddEntry("Drift Time1", M_T1);
  fTDMenu->AddEntry("Drift Time2", M_T2);
  fTDMenu->AddEntry("Time1 - Time2", M_T1_T2);
  fTDMenu->Associate(this);
  //  fTDMenu->DisableEntry(M_T1);
  //  fTDMenu->DisableEntry(M_T2);
  //  fTDMenu->DisableEntry(M_T1_T2);

  fMenuBar = new TGMenuBar(fFB, 0, 0, kHorizontalFrame);
  fMenuBar->AddPopup("   Drift Time   ", fTDMenu, 
		     new TGLayoutHints(kLHintsNormal, 2, 0, -1, 0));
  //  fMenuBar->SetToolTipText("Time1, Time2 or Time1-Time2");
  fMenuBar->ChangeOptions(48);
  fFB->AddFrame(fMenuBar,new TGLayoutHints(kLHintsNormal, 0, 0, 0, 0));
  fMenuBar->Resize(95,25);

  /*
  fCascadeMenu = new TGPopupMenu(fClient->GetRoot());
  fCascadeMenu->AddEntry("Drift Time1", M_T1);
  fCascadeMenu->AddEntry("Drift Time2", M_T2);
  fCascadeMenu->AddEntry("Drift Time1 - Time2", M_T1_T2);
  fCascadeMenu->Associate(this);
  fFB->AddFrame(fCascadeMenu, new TGLayoutHints(kLHintsNormal,30, 4, 4, 4));
  */

  // TDC Menu

  fTDCMenu = new TGPopupMenu(fClient->GetRoot());
  fTDCMenu->AddEntry("All chips", M_ALLCH);
  fTDCMenu->AddEntry("Selected chip", M_SELCH);
  fTDCMenu->Associate(this);
  fTDCMenu->DisableEntry(M_SELCH);
  fMenuBarTDC = new TGMenuBar(fFB, 0, 0, kHorizontalFrame);
  fMenuBarTDC->AddPopup("      TDC      ", fTDCMenu, 
		     new TGLayoutHints(kLHintsNormal, 2, 0, -1, 0));
  fMenuBarTDC->ChangeOptions(48);
  fFB->AddFrame(fMenuBarTDC,new TGLayoutHints(kLHintsNormal, 0, 0, 0, 0));
  fMenuBarTDC->Resize(95,25);

 

  // TDC Button

  /*  fTdcButton = new TGTextButton(fFB, "TDC", M_TDC);
  fTdcButton->Resize(90, 25);
  fTdcButton->Associate(this);
  fTdcButton->SetToolTipText("TDC chip*8 + TDC channel");
  fFB->AddFrame(fTdcButton,new TGLayoutHints(kLHintsNormal,30,4,4,4));
  */
  fMultButton = new TGTextButton(fFB, "MULT", M_MULT);
  fMultButton->Associate(this);
  fMultButton->Resize(95, 25);
  fMultButton->SetToolTipText("Multiplicity");
  fFB->AddFrame(fMultButton,new TGLayoutHints(kLHintsNormal,30,4,4,4));

  fCloseButton = new TGTextButton(fFB, "Close Panel", M_CLOSE);
  fCloseButton->Associate(this);
  fCloseButton->SetToolTipText("Close GUI Interface");
  fFB->AddFrame(fCloseButton, new TGLayoutHints(kLHintsNormal,30, 4, 4, 4));
  fCloseButton->Resize(95, 25);


  fAllMbo = kTRUE;
  secNum = 2; // Starting values for beamtime-99, 3 sector, 1 module, all mbo;
  modNum = 1;
  mboNum = 0;
  chipNum = 1;
  xmin = 0.;
  xmax = 2048.;
  ymin = 0.;
  ymax = 200.;
  refrTime = 99999999;
  sprintf(tmp,"%.1f",xmin);
  fRangeBufXMin->Clear();
  fRangeBufXMin->AddText(0,tmp);
  fClient->NeedRedraw(fRangeXMin);
  sprintf(tmp,"%.1f",xmax);
  fRangeBufXMax->Clear();
  fRangeBufXMax->AddText(0,tmp);
  fClient->NeedRedraw(fRangeXMax);

  sprintf(tmp,"%.1f",ymin);
  fRangeBufYMin->Clear();
  fRangeBufYMin->AddText(0,tmp);
  fClient->NeedRedraw(fRangeYMin);
  sprintf(tmp,"%.1f",ymax);
  fRangeBufYMax->Clear();
  fRangeBufYMax->AddText(0,tmp);
  fClient->NeedRedraw(fRangeYMax);
 

  MapSubwindows();
  
  SetWindowName("MDC Beamtime, Nov 23rd - Dec 6th, 1999 ");
  
  Resize(GetDefaultSize());
  MapWindow();
  //  fClient->WaitFor(this); 
}


//-----------------------------------------------------------------
//
//                drawTDCDT()
//
//  Draws a branch projection of MDC-Raw tree.  
//
//----------------------------------------------------------------


   

//-----------------------------------------------------
//
//      Destructor for class mdcGUI
//
//----------------------------------------------------


mdcGUI::~mdcGUI() {

  delete fTDCDTButton;
  delete fMenuBar;
  delete fTDMenu;
  delete fTdcButton;
  delete fMultButton;
  delete fCloseButton;
  delete fRangeYMax;
  delete fRangeYMin;
  delete fRangeXMax;
  delete fRangeXMin;
  delete fCombo4;
  delete fCombo3;
  delete fCombo2;
  delete fCombo1;
  //  delete fF4;
  //  delete fF3;
  //  delete fF2;
  //  delete fF1;
  delete fFB;
  delete fXCuts;
  delete fFA;
  delete fF0;
  delete fFMain;

}



//---------------------------------------------------
//
//                CloseWindow() 
//
//    Closes window 'MDC GUI' (apparently)
//
//---------------------------------------------------


void mdcGUI::CloseWindow() {
  
  delete this;
 
}

//------------------------------------------------------------
//
//     ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2)
//
//   Processes information from GUI items of the panel
//
//------------------------------------------------------------


Bool_t mdcGUI::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2) {
  
  int retval; //for class TGMsgBox
  int buttons;//used to construct message panel when Close button is clicked 
  //  int numb;//to update layout of list boxes
  //  int i,j;
  //  int imin, imax;//to calculate display layout
  //  int jmin, jmax;//to calculate display layout
  //  char tmp[50], filename[50], ext[5];//to process information in file container
  //TGFileItem *f;//the same as above
  //  void *p = NULL;//the same as above
  //  char command[50];//the same as above

  xmin = (atoi)(fRangeBufXMin->GetString());
  xmax = (atoi)(fRangeBufXMax->GetString());
  ymin = (atoi)(fRangeBufYMin->GetString());
  ymax = (atoi)(fRangeBufYMax->GetString());
  switch(GET_MSG(msg)) {
  case kC_COMMAND:
    switch(GET_SUBMSG(msg)) {

  case kCM_CHECKBUTTON:

    switch(parm1) {
    case M_TR:
      fRefreshTimeBuf->Clear();
      fClient->NeedRedraw(fRefreshTime);
      if(!fTimeRefr->GetState()) {
	fRefreshTime->SetState(kFALSE);
	refrTime = 99999999;
	if(gMdcGUI) gMdcGUI->TTimer::TurnOff();
      }
      else {
	fRefreshTime->SetState(kTRUE);
      }
      break;
    case M_LE:
      fLastEvtBuf->Clear();
      fClient->NeedRedraw(fLastEvtNum);
      if(!fLastEvt->GetState()) fLastEvtNum ->SetState(kFALSE);
      // Not this option is not activeted
      //else fLastEvtNum ->SetState(kTRUE);
      break;
    default:
      break;
    }

    case kCM_RADIOBUTTON:
      switch (parm1) {
      case 81:
        fData = kFALSE; 
	fRad2->SetState(kButtonUp);
	ymin = 0.;
	ymax = 600.;
	sprintf(tmp,"%.1f",ymin);
	fRangeBufYMin->Clear();
	fRangeBufYMin->AddText(0,tmp);
	fClient->NeedRedraw(fRangeYMin);
	sprintf(tmp,"%.1f",ymax);
	fRangeBufYMax->Clear();
	fRangeBufYMax->AddText(0,tmp);
	fClient->NeedRedraw(fRangeYMax);
	break;
      case 82:
	fRad1->SetState(kButtonUp);
        fData = kTRUE; 
	ymin = 0.;
	ymax = 200.;
	sprintf(tmp,"%.1f",ymin);
	fRangeBufYMin->Clear();
	fRangeBufYMin->AddText(0,tmp);
	fClient->NeedRedraw(fRangeYMin);
	sprintf(tmp,"%.1f",ymax);
	fRangeBufYMax->Clear();
	fRangeBufYMax->AddText(0,tmp);
	fClient->NeedRedraw(fRangeYMax);	
	break;
      default:
	break;
      }
      break;
      
    case kCM_MENU:
      switch (parm1) { 
      case M_T1:
	printf("Drift Time1 button pressed \n");
	if(fTimeRefr->GetState()) 
	  refrTime = 1000*(atoi)(fRefreshTimeBuf->GetString());
	if(gMdcGUI) gMdcGUI->TTimer::TurnOff();
        histdraw = new 
	  histDraw(DT,fAllMbo,xmin,xmax,ymin,ymax,refrTime,secNum,modNum,mboNum,1);
	break;
      case M_T2:
	printf("Drift Time2 button pressed\n");
	if(fTimeRefr->GetState()) 
	  refrTime = 1000*(atoi)(fRefreshTimeBuf->GetString());
	if(gMdcGUI) gMdcGUI->TTimer::TurnOff();
        histdraw = new 
	  histDraw(DT,fAllMbo,xmin,xmax,ymin,ymax,refrTime,secNum,modNum,mboNum,2);
	break;
      case M_T1_T2:
	printf("Drift Time1-Time2 button pressed\n");
	if(fTimeRefr->GetState()) 
	  refrTime = 1000*(atoi)(fRefreshTimeBuf->GetString());
	if(gMdcGUI) gMdcGUI->TTimer::TurnOff();
        histdraw = new 
	  histDraw(DT,fAllMbo,xmin,xmax,ymin,ymax,refrTime,secNum,modNum,mboNum,12);
	break;

      case M_ALLCH:
	if(fTimeRefr->GetState()) 
	  refrTime = 1000*(atoi)(fRefreshTimeBuf->GetString());
	if(gMdcGUI) gMdcGUI->TTimer::TurnOff();
        histdraw = new histDraw(TDC,fAllMbo,xmin,xmax,ymin,ymax,
				refrTime,secNum,modNum,mboNum,0);
	break;
	
      case M_SELCH:
	if(fTimeRefr->GetState()) 
	  refrTime = 1000*(atoi)(fRefreshTimeBuf->GetString());
	if(gMdcGUI) gMdcGUI->TTimer::TurnOff();
	histdraw = new histDraw(TDC,fAllMbo,xmin,xmax,ymin,ymax,
				refrTime,secNum,modNum,mboNum,chipNum);

	break;

      default:
	break;
      }
      
    case kCM_BUTTON:
      switch(parm1) {	
      case M_CLOSE:
	retval = 0;
	buttons = 0;
	buttons |= kMBYes;
	buttons |= kMBNo;
	new TGMsgBox(fClient->GetRoot(), this, "Confirm action",
		     "Close Panel 'MDC GUI' ?",
		     kMBIconQuestion,buttons,&retval);
	if(retval == 1) CloseWindow();
	break;

      case M_TDCDT:
        printf("TDCDT button id %ld pressed\n", parm1);
	if(fTimeRefr->GetState()) refrTime = 1000*(atoi)(fRefreshTimeBuf->GetString());
 	if(gMdcGUI) gMdcGUI->TTimer::TurnOff();
	histdraw = new 
	  histDraw(TDCDT,fAllMbo,xmin,xmax,ymin,ymax,refrTime,secNum,modNum,mboNum,0);
        break;

     case M_TDC:
	printf("TDC pressed\n");
	if(fTimeRefr->GetState()) refrTime = 1000*(atoi)(fRefreshTimeBuf->GetString());
	if(gMdcGUI) gMdcGUI->TTimer::TurnOff();
        histdraw = new 
	  histDraw(TDC,fAllMbo,xmin,xmax,ymin,ymax,refrTime,secNum,modNum,mboNum,0);
	break;

     case M_MULT:
	printf("Mult pressed\n");
	if(fTimeRefr->GetState()) refrTime = 1000*(atoi)(fRefreshTimeBuf->GetString());
 	if(gMdcGUI) gMdcGUI->TTimer::TurnOff();
	histdraw = new 
	  histDraw(MULT,fAllMbo,xmin,xmax,ymin,ymax,refrTime,secNum,modNum,mboNum,0);
	break;
      default:

	break;      
      }
    }      
  case kCM_COMBOBOX:
    switch(parm1) {
    case M_SECTOR:
      (parm2==1) ? secNum=2:secNum=5;
      break;
    case M_MODULE:
      (parm2==1) ? modNum=0:modNum=1;
      if(parm2 == 1) {
	fCombo3->Select(15);
	if(fCombo3->GetSelected()==-1) fCombo3->AddEntry("14",15);
	fCombo3->Select(16);
	if(fCombo3->GetSelected()==-1) fCombo3->AddEntry("15",16);
	fCombo4->Select(15);
	if(fCombo4->GetSelected()==-1) fCombo4->AddEntry("321",15);
	fCombo4->Select(16);
	if(fCombo4->GetSelected()==-1) fCombo4->AddEntry("322",16);
      }
      if(parm2 == 2) {
	if(fCombo3->GetSelected()>14) { 
	  fCombo3->Select(0);
	  fCombo4->Select(0);
	  fAllMbo = kTRUE;
	}
	fCombo3->RemoveEntry(15);
	fCombo3->RemoveEntry(16);
	fCombo4->RemoveEntry(15);
	fCombo4->RemoveEntry(16);
      }
      break;
    case M_MBIND:
      mboNum=parm2;
      if(parm2==0) {
	fAllMbo = kTRUE;
	//	fTDMenu->DisableEntry(M_T1);
	//	fTDMenu->DisableEntry(M_T2);
	//	fTDMenu->DisableEntry(M_T1_T2);
	fCombo4->Select(parm2);
      }
      else {
	fAllMbo = kFALSE;
	//	fTDMenu->EnableEntry(M_T1);
	//	fTDMenu->EnableEntry(M_T2);
	//	fTDMenu->EnableEntry(M_T1_T2);
	fCombo4->Select(parm2);
      }
      if(parm2==0) {  	// Delete entries from TDC Combo
	fCombo5->Select(7);
	if(!(fCombo5->GetSelected()==-1)) {
	  for(i=0;i<8;i++) {
	    fCombo5->RemoveEntry(i);
	  }
	  sprintf(tmp1, "%s", "  ");
	  fCombo5->AddEntry(tmp1,0);
	}
 	fTDCMenu->DisableEntry(M_SELCH);
      }
      else {       // Add entries to TDC Combo
	fCombo5->Select(7);
	if(fCombo5->GetSelected()==-1) {
	  fCombo5->RemoveEntry(0);
	  for(i=0;i<8;i++) {    
	    sprintf(tmp1, "%i", i+1);
	    fCombo5->AddEntry(tmp1,i);
	  }
	}
	fTDCMenu->EnableEntry(M_SELCH);
      }
      fCombo5->Select(0);
      break;
    case M_MBNAME:      
      mboNum=parm2;
      if(parm2==0) 
	fAllMbo = kTRUE;
      else
	fAllMbo = kFALSE;
      fCombo3->Select(parm2);
      if(parm2==0) {  	// Delete entries from TDC Combo
	fCombo5->Select(7);
	if(!(fCombo5->GetSelected()==-1)) {
	  for(i=0;i<8;i++) {
	    fCombo5->RemoveEntry(i);
	  }
	  sprintf(tmp1, "%s", "  ");
	  fCombo5->AddEntry(tmp1,0);
	}
	fTDCMenu->DisableEntry(M_SELCH);
      }
      else {                   // Add entries to TDC Combo
	fCombo5->Select(7);
	if(fCombo5->GetSelected()==-1) {
	  fCombo5->RemoveEntry(0);
	  for(i=0;i<8;i++) {    
	    sprintf(tmp1, "%i", i+1);
	    fCombo5->AddEntry(tmp1,i);
	  }
	}
	fTDCMenu->EnableEntry(M_SELCH);\
      }
      fCombo5->Select(0);
      break;
    case M_TDCNUM:
      chipNum = parm2;
      break;

    default:
      break; 
    }
  default:
    break;
  } 
  return kTRUE;
}



