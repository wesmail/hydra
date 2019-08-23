//*-- Author : Dmitry Vasiliev (vassili@lns.infn.it)
/* 
 * Copyright P.Finocchiaro and D.Vasiliev (INFN-LNS, Catania) HADES collaboration
*/

//-------------------------------------------------------------------------
//
// File name:       hcontourmanager.cc
// Subject:         GUI class for contour management.
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            April 2000
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the source code for the member functions of 
// the class HContourManager.
// This is a GUI class which provides interface for contour definition.
//
//-------------------------------------------------------------------------

#include "hcontourmanager.h"
#include "honlinecontrol.h"
#include "hcontourcont.h"
#include "hcontour.h"
#include "honlmainwindow.h"
#include "honlobjbrowser.h"
#include "herror.h"
#include <TGLabel.h>
#include <TPad.h>
#include <TGMsgBox.h>
#include <TIterator.h>
#include <TList.h>
#include <TH2.h>

//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////
// This is a GUI class which provides the interface for the contour definition.
///////////////////////////////////////////////////////////////////////
ClassImp(HContourManager)


//---------------------------------------------------------------------
//******  HContourManager::HContourManager(const TGWindow,UInt_t w,UInt_t h,
//                                         TGMainFrame *pp = 0)
//
// Constructor of the class HContourManager.
//
//---------------------------------------------------------------------

HContourManager::HContourManager(const TGWindow *p, UInt_t w, UInt_t h,
				 TGMainFrame *pp) : 
  TGMainFrame(p, w, h) {
//
// Constructor of the class HContourManager.
//

//---------------------------------------------------------------
//********* frames
//---------------------------------------------------------------

fFrame = new TGCompositeFrame(this, 500, 500, kHorizontalFrame);
AddFrame(fFrame, new TGLayoutHints(kLHintsNormal,2,2,10,10));

fF1 = new TGCompositeFrame(fFrame, 250, 500, kVerticalFrame);
fFrame->AddFrame(fF1, new TGLayoutHints(kLHintsNormal));

fF2 = new TGCompositeFrame(fFrame, 250, 500, kVerticalFrame);
fFrame->AddFrame(fF2, new TGLayoutHints(kLHintsNormal, 10,0,0,0));

fF10 = new TGGroupFrame(fF1, "contour definition", kVerticalFrame);
fF1->AddFrame(fF10, new TGLayoutHints(kLHintsNormal));

fF11 = new TGCompositeFrame(fF10, 250, 500, kVerticalFrame);
fF10->AddFrame(fF11, new TGLayoutHints(kLHintsNormal,0,3,0,0));
fF11->SetLayoutManager(new TGMatrixLayout(fF11,0,2,15));

fF12 = new TGCompositeFrame(fF1, 250, 100, kHorizontalFrame);
fF1->AddFrame(fF12, new TGLayoutHints(kLHintsNormal));

fF13 = new TGCompositeFrame(fF1, 250, 100, kHorizontalFrame);
fF1->AddFrame(fF13, new TGLayoutHints(kLHintsNormal));

fF14 = new TGCompositeFrame(fF1, 250, 100, kHorizontalFrame);
fF1->AddFrame(fF14, new TGLayoutHints(kLHintsNormal));

fF21 = new TGCompositeFrame(fF2, 250, 100, kHorizontalFrame);
fF2->AddFrame(fF21, new TGLayoutHints(kLHintsNormal,5,0,0,0));
fF21->SetLayoutManager(new TGMatrixLayout(fF21,0,2,3));

fF22 = new TGGroupFrame(fF2, "list of contours", kHorizontalFrame);
fF2->AddFrame(fF22, new TGLayoutHints(kLHintsNormal));

fF23 = new TGCompositeFrame(fF2, 250, 100, kHorizontalFrame);
fF2->AddFrame(fF23, new TGLayoutHints(kLHintsNormal,5,0,5,0));
fF23->SetLayoutManager(new TGMatrixLayout(fF23,0,2,3));

//---------------------------------------------------------------
//********* help, undo, set and clear buttons
//---------------------------------------------------------------

fInit = new TGTextButton(fF11, "Init", M__C_INIT);
fInit->Associate(this);
fInit->SetToolTipText("Catch the current active pad");
fF11->AddFrame(fInit, new TGLayoutHints(kLHintsNormal,30,30,3,5)); 
fInit->Resize(50,20);

fClear = new TGTextButton(fF11, "Clear", M__C_CLEAR);
fClear->Associate(this);
fClear->SetToolTipText("Remove contour from the pad");
fF11->AddFrame(fClear, new TGLayoutHints(kLHintsNormal,30,30,3,1)); 
fClear->Resize(50,20);

fUndo = new TGTextButton(fF11, "Undo", M__C_UNDO);
fUndo->Associate(this);
fUndo->SetToolTipText("One step back");
fF11->AddFrame(fUndo, new TGLayoutHints(kLHintsNormal,30,30,3,5)); 
fUndo->Resize(50,20);

fSet = new TGTextButton(fF11, "Set", M__C_SET);
fSet->Associate(this);
fSet->SetToolTipText("Connect the first and last points of the contour");
fF11->AddFrame(fSet, new TGLayoutHints(kLHintsNormal,30,30,3,5)); 
fSet->Resize(50,20);

fHelp = new TGTextButton(fF11, "Help", M__C_HELP);
fHelp->Associate(this);
fHelp->SetToolTipText("Click me to learn how to work with this panel");
fF11->AddFrame(fHelp, new TGLayoutHints(kLHintsNormal,30,30,3,5)); 
fHelp->Resize(50,20);

//---------------------------------------------------------------
//********* contour name
//---------------------------------------------------------------

fF12->AddFrame(new TGLabel(fF12, new TGHotString("contour name")),
	       new TGLayoutHints(kLHintsNormal,5,2,6,2));

fName = new TGTextEntry(fF12, fNameBuf = new TGTextBuffer(100), -1);
fF12->AddFrame(fName, new TGLayoutHints(kLHintsNormal,4,0,5,2));
fName->Resize(70,fName->GetDefaultHeight());

//---------------------------------------------------------------
//********* inside and outside hits
//---------------------------------------------------------------

fF13->AddFrame(new TGLabel(fF13, new TGHotString("hits inside")),
	       new TGLayoutHints(kLHintsNormal,5,2,5,2));
fInLB = new TGListBox(fF13, -1);
fF13->AddFrame(fInLB, new TGLayoutHints(kLHintsNormal,24,0,4,2)); 
fInLB->Resize(70,20);

fF14->AddFrame(new TGLabel(fF14, new TGHotString("hits outside")),
	       new TGLayoutHints(kLHintsNormal,5,2,5,2));
fOutLB = new TGListBox(fF14, -1);
fF14->AddFrame(fOutLB, new TGLayoutHints(kLHintsNormal,17,0,4,2)); 
fOutLB->Resize(70,20);

//---------------------------------------------------------------
//********* add and remove buttons
//---------------------------------------------------------------

fAdd = new TGTextButton(fF21, "Add", M__C_ADD);
fAdd->Associate(this);
fAdd->SetToolTipText("Add new contour to the list");
fAdd->Resize(55,20);
fF21->AddFrame(fAdd);

fRemove = new TGTextButton(fF21, "Remove", M__C_REMOVE);
fRemove->Associate(this);
fRemove->SetToolTipText("Remove contour from the list");
fRemove->Resize(55,20);
fF21->AddFrame(fRemove);

//---------------------------------------------------------------
//********* list of contours
//---------------------------------------------------------------

fList = new TGListBox(fF22, M__C_LIST);
fList->Associate(this);
fF22->AddFrame(fList, new TGLayoutHints(kLHintsNormal,2,2,2,2));
fList->Resize(100,145);

//---------------------------------------------------------------
//********* previous and next buttons
//---------------------------------------------------------------

fPrev = new TGTextButton(fF23, "Previous", M__C_PREV);
fPrev->Associate(this);
fPrev->SetToolTipText("One entry up in the list");
fPrev->Resize(60,20);
fF23->AddFrame(fPrev);

fNext = new TGTextButton(fF23, "Next", M__C_NEXT);
fNext->Associate(this);
fNext->SetToolTipText("One entry down in the list");
fNext->Resize(60,20);
fF23->AddFrame(fNext);

fWindow = pp;
countIn = countOut = 0;

countContour++;

MapSubwindows();

SetWindowName("Contour Manager");

Resize(GetDefaultSize());
MapWindow();

updateList();
SendMessage(this,MK_MSG(kC_COMMAND,kCM_BUTTON),M__C_CLEAR,0);

}


//--------------------------------------------------------------------
//******  HContourManager::~HContourManager()
//
// Destructor of the class HContourManager.
//
//--------------------------------------------------------------------

HContourManager::~HContourManager() {
//
// Destructor of the class HContourManager.
//

clearContour();
delete fNext; delete fPrev;
delete fList;
delete fRemove; delete fAdd;
delete fOutLB; delete fInLB;
delete fName;
delete fHelp; delete fSet; delete fUndo; delete fClear; delete fInit;
delete fF23; delete fF22; delete fF21; 
delete fF14; delete fF13; delete fF12; delete fF11; delete fF10;
delete fF2; delete fF1;
delete fFrame;

countContour--;

}


//--------------------------------------------------------------------
//******  void HContourManager::updateList()
//
// This function updates the content of the list of contours.
//
//--------------------------------------------------------------------

void HContourManager::updateList() {
//
// This function updates the content of the list of contours.
//

  if(!gOnline) return;
  TGTextLBEntry *pEntry = 0;
  HContour *pContour = 0;
  Int_t i=1;

  while((pEntry = (TGTextLBEntry*) fList->Select(i,kFALSE))) {

    fList->RemoveEntry(i);
    i++;
  }

  Int_t j = 0;
  for(i=0; i<gOnline->getContour()->getEntries(); i++) {

    pContour = gOnline->getContour()->at(i);
    if(pContour) fList->AddEntry(pContour->GetName(),++j);
  }

  fList->MapSubwindows();
  fList->Layout();

}


//--------------------------------------------------------------------
//******  void HContourManager::updateCountBoxes()
//
// This function updates the content of the list boxes fInLB and fOutLB.
//
//--------------------------------------------------------------------

void HContourManager::updateCountBoxes() {
//
// This function updates the content of the list boxes fInLB and fOutLB.
//

  fInLB->RemoveEntry(1);
  fOutLB->RemoveEntry(1);
  Char_t entry[20];

  sprintf(entry,"%i",countIn);
  fInLB->AddEntry(entry,1);

  sprintf(entry,"%i",countOut);
  fOutLB->AddEntry(entry,1);

  fInLB->MapSubwindows();
  fInLB->Layout();

  fOutLB->MapSubwindows();
  fOutLB->Layout();

}


//--------------------------------------------------------------------
//******  void HContourManager::clearContour()
//
// This function removes the contour from the active pad.
//
//--------------------------------------------------------------------

void HContourManager::clearContour() {
//
// This function removes the contour from the active pad.
//

  TIterator *it = 0;
  TObject *select = 0;
  if(gPad) { 
    TList *list = ((TPad*)gPad)->GetListOfPrimitives();
    if(list) {
      it = list->MakeIterator();
      while((select = it->Next()) != NULL) {
	if(!strcmp(select->ClassName(),"HContour")) {
	  list->Remove(select);
	  if(select->IsOnHeap()) { delete select; select = 0; }
	}
      }
    }
    gPad->Modified();
    gPad->Update();
  }

}


//--------------------------------------------------------------------
//******  void HContourManager::countHits(TH2F *hist, HContour *cont)
//
// This function counts the number of hits inside and outside the contour.
//
//--------------------------------------------------------------------

void HContourManager::countHits(TH2F *hist, HContour *cont) {
//
// This function counts the number of hits inside and outside the contour.
//
  
  countIn = countOut = 0;
  if(!hist || !cont) return;

  Float_t xmin = hist->GetXaxis()->GetXmin();
  Float_t xmax = hist->GetXaxis()->GetXmax();
  Float_t ymin = hist->GetYaxis()->GetXmin();
  Float_t ymax = hist->GetYaxis()->GetXmax();

  if(!cont->checkLimits(xmin,xmax,ymin,ymax)) return;

  Int_t binsx = hist->GetXaxis()->GetNbins();
  Int_t binsy = hist->GetYaxis()->GetNbins();

  Int_t i, j;
  Float_t lowx, upx, lowy, upy;

  for(i=1; i<=binsx; i++) {
    for(j=1; j<=binsy; j++) {

      lowx = hist->GetXaxis()->GetBinLowEdge(i);
      upx  = hist->GetXaxis()->GetBinUpEdge(i);
      lowy = hist->GetYaxis()->GetBinLowEdge(j);
      upy  = hist->GetYaxis()->GetBinUpEdge(j);

      if(cont->isInside(lowx,lowy) || cont->isInside(lowx,upy) ||
	 cont->isInside(upx,lowy)  || cont->isInside(upx,upy)) {
	countIn += (Int_t) (hist->GetCellContent(i,j));
      }
      else countOut += (Int_t) (hist->GetCellContent(i,j));

    }
  }

}


//---------------------------------------------------------------------
//******  Bool_t HContourManager::ProcessMessage(Long_t msg, Long_t parm1,
//                                                           Long_t parm2)
//
// This function processes all GUI widgets.
//
//---------------------------------------------------------------------

Bool_t HContourManager::ProcessMessage(Long_t msg,Long_t parm1,Long_t parm2) {
//
// This function processes all GUI widgets.
//

Int_t retval;
HContour *pCont = 0;
TGTextLBEntry *pEntry = 0;
Int_t idx = -1;
TH2F *hist2d = 0;
TIterator *it = 0;
TObject *select = 0;
HOnlMainWindow *pWind;

switch(GET_MSG(msg)) {

 case kC_COMMAND:

  switch(GET_SUBMSG(msg)) {

  case kCM_BUTTON:

    switch(parm1) {

    case M__C_HELP:
      new TGMsgBox(fClient->GetRoot(),this,"Help Message",
		   "A contour can be defined by clicking the left button of the mouse on the current active pad \
(important that in the pad there should be drawn a 2d histogram which serves as a \
coordinate system for the contour).\
\
To make the active pad visible for the panel click 'Init'.\
To connect the first and last points make a Double_t click on the left button or click 'Set'.\
To undo one step click 'Undo'.\
To release the pad and remove the contour from it click 'Clear'.\
\
Important notes:\
\
The program takes care of the selfcross check during the contour definition. If a selfcross \
found the contour is shown in red. To be back one step click 'Undo'.\
\
During the contour definition or display it is not possible to pick up with the mouse \
other objects drawn in the pad. So, to reactivate this facility release the pad by clicking \
'Clear'.",0,0,&retval);
      break;

    case M__C_UNDO:
      if(!gPad || !fContour) break;
      fContour->undo();
      break;

    case M__C_SET:
      if(!gPad || !fContour) break;
      fContour->setAll();
      break;

    case M__C_INIT:
      clearContour();
      countIn = countOut = 0;
      updateCountBoxes();
      fNameBuf->Clear();
      fClient->NeedRedraw(fName);
      if(gPad) {
	fContour = new HContour();
	fContour->Draw();
	gPad->Modified();
	gPad->Update();
      }
      break;

    case M__C_CLEAR:
      clearContour();
      countIn = countOut = 0;
      updateCountBoxes();
      fNameBuf->Clear();
      fClient->NeedRedraw(fName);
      break;

    case M__C_ADD:
      if(!gOnline) {
	HError::message("Container of contours not initialized");
	break;
      }
      if(!fContour) {
	HError::message("No contour defined");
	break;
      }
      if(!strcmp(fNameBuf->GetString(),"")) {
	HError::message("Contour name not defined");
	break;
      }
      pCont = new HContour();
      fContour->copy(pCont);
      pCont->SetName(fNameBuf->GetString());
      gOnline->getContour()->add(pCont);
      SendMessage(this,MK_MSG(kC_COMMAND,kCM_BUTTON),M__C_INIT,0);
      updateList();
      if(fWindow && fWindow->IsOnHeap()) {
	pWind = (HOnlMainWindow*) fWindow;
	if(pWind->getBrowser() && pWind->getBrowser()->IsOnHeap()) {
	  pWind->getBrowser()->createContourList();
	}
      }
      break;      

    case M__C_REMOVE:
      if(!gOnline) {
	HError::message("Container of contours not initialized");
	break;
      }
      pEntry = (TGTextLBEntry*) fList->GetSelectedEntry();
      if(pEntry) {
	idx = gOnline->getContour()->find(pEntry->GetText()->GetString());
	gOnline->getContour()->removeAt(idx);
      }
      SendMessage(this,MK_MSG(kC_COMMAND,kCM_BUTTON),M__C_CLEAR,0);
      updateList();
      if(fWindow && fWindow->IsOnHeap()) {
	pWind = (HOnlMainWindow*) fWindow;
	if(pWind->getBrowser() && pWind->getBrowser()->IsOnHeap()) {
	  pWind->getBrowser()->createContourList();
	}
      }
      break;

    case M__C_NEXT:
      idx = fList->GetSelected();
      if(idx < 0) idx = 0;
      pEntry = (TGTextLBEntry*) fList->Select(++idx,kFALSE);
      if(pEntry) {
	fList->Select(idx,kTRUE);
	SendMessage(this,MK_MSG(kC_COMMAND,kCM_LISTBOX),M__C_LIST,idx);
      }
      else SendMessage(this,MK_MSG(kC_COMMAND,kCM_BUTTON),M__C_CLEAR,0);
      break;

    case M__C_PREV:
      idx = fList->GetSelected();
      if(idx < 0) idx = 0;
      pEntry = (TGTextLBEntry*) fList->Select(--idx,kFALSE);
      if(pEntry) {
	fList->Select(idx,kTRUE);
	SendMessage(this,MK_MSG(kC_COMMAND,kCM_LISTBOX),M__C_LIST,idx);
      }
      else SendMessage(this,MK_MSG(kC_COMMAND,kCM_BUTTON),M__C_CLEAR,0);
      break;

    default:
      break;
    }
    break;

  case kCM_LISTBOX:

    if(parm1 == M__C_LIST) {

      if(!gOnline) break;
      if(!gPad) break;
      pEntry = (TGTextLBEntry*) fList->GetSelectedEntry();
      if(pEntry) {

	idx = gOnline->getContour()->find(pEntry->GetText()->GetString());
	pCont = gOnline->getContour()->at(idx);
	if(pCont) {
	  clearContour();
	  fContour = new HContour();
	  pCont->copy(fContour);
	  fContour->Draw();
	  fNameBuf->Clear();
	  fNameBuf->AddText(0,fContour->GetName());
	  fClient->NeedRedraw(fName);
	  gPad->Modified();
	  gPad->Update();
	}
      }
      if(it) { delete it; it = 0; }
      it = gPad->GetListOfPrimitives()->MakeIterator();
      while((select = it->Next()) != NULL) {
	if(!strcmp(select->ClassName(),"TH2F") && 
	   !strcmp(it->GetOption(),"")) {
	  hist2d = (TH2F*) select;
	  break;
	}
      }
      if(hist2d && pCont) {
	countHits(hist2d,pCont);
	updateCountBoxes();
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




