//*-- Author : Dmitry Vasiliev (vassili@lns.infn.it)
/* 
 * Copyright P.Finocchiaro and D.Vasiliev (INFN-LNS, Catania) HADES collaboration
*/

//-------------------------------------------------------------------------
//
// File name:       hconsistencywarn.cc
// Subject:         GUI class to provide a communication channel between 
//                  a user and the program on the subject of inconsistent 
//                  objects.
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            March 2000
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the source code for the member functions of
// the class HConsistencyWarn.
// This is a GUI class destined to warn a user about the presence of
// inconsistent HTOP objects.
//
//------------------------------------------------------------------------

#include "hconsistencywarn.h"
#include "hconsistency.h"
#include "honlinecontrol.h"
#include "hlocalcont.h"
#include "hglobalcont.h"
#include "hhistocont.h"
#include "hlocalcondition.h"
#include "hglobalcondition.h"
#include "hhistogram.h"

#include <TGLabel.h>

//_HADES_CLASS_DESCRIPTION 
/////////////////////////////////////////////////////////////////////
// This is a GUI class destined to warn a user about the presence of
// inconsistent HTOP (HADES TOF Online Project) objects.
////////////////////////////////////////////////////////////////////
ClassImp(HConsistencyWarn)

//---------------------------------------------------------------------
//******  HConsistencyWarn::HConsistencyWarn(const TGWindow *p, UInt_t w,
//                                           UInt_t h, Int_t* retcode)
//
// Constructor of the class HConsistencyWarn.
//
//---------------------------------------------------------------------

HConsistencyWarn::HConsistencyWarn(const TGWindow *p, UInt_t w, UInt_t h,
				   Int_t* retcode) :
  TGMainFrame(p, w, h) {
//
// Constructor of the class HConsistencyWarn.
//

fFrame = new TGCompositeFrame(this, 500, 500, kVerticalFrame);
AddFrame(fFrame, new TGLayoutHints(kLHintsNormal));

fF1 = new TGCompositeFrame(fFrame, 500, 80, kVerticalFrame);
fFrame->AddFrame(fF1, new TGLayoutHints(kLHintsNormal));

fF2 = new TGCompositeFrame(fFrame, 500, 300, kHorizontalFrame);
fFrame->AddFrame(fF2, new TGLayoutHints(kLHintsNormal));

fF3 = new TGCompositeFrame(fFrame, 500, 100, kHorizontalFrame);
fFrame->AddFrame(fF3, new TGLayoutHints(kLHintsNormal));

//------------------------------------------------------------------

fF1->AddFrame(new TGLabel(fF1, new TGHotString("following")),
	      new TGLayoutHints(kLHintsNormal|kLHintsCenterX, 2,2,2,2));
fF1->AddFrame(new TGLabel(fF1, 
new TGHotString("local conditions        global conditions       histogrammes")),
	      new TGLayoutHints(kLHintsNormal|kLHintsCenterX, 20,2,2,2));
fF1->AddFrame(new TGLabel(fF1, new TGHotString("will be inconsistent")),
	      new TGLayoutHints(kLHintsNormal|kLHintsCenterX, 20,2,2,2));

//------------------------------------------------------------------

fLCList = new TGListBox(fF2,-1);
fLCList->Associate(this);
fF2->AddFrame(fLCList, new TGLayoutHints(kLHintsNormal,2,2,2,2));
fLCList->Resize(110,150); 

fGCList = new TGListBox(fF2,-1);
fGCList->Associate(this);
fF2->AddFrame(fGCList, new TGLayoutHints(kLHintsNormal,5,5,2,2));
fGCList->Resize(110,150); 

fHiList = new TGListBox(fF2,-1);
fHiList->Associate(this);
fF2->AddFrame(fHiList, new TGLayoutHints(kLHintsNormal,2,2,2,2));
fHiList->Resize(110,150); 

//------------------------------------------------------------------

fDelete = new TGTextButton(fF3,"Delete ",10);
fDelete->Associate(this);
fDelete->SetToolTipText("Delete all inconsistent objects");
fF3->AddFrame(fDelete, new TGLayoutHints(kLHintsNormal,120,2,8,8));

fCancel = new TGTextButton(fF3,"Cancel",11);
fCancel->Associate(this);
fCancel->SetToolTipText("Close the window");
fF3->AddFrame(fCancel, new TGLayoutHints(kLHintsNormal,10,2,8,8));

//------------------------------------------------------------------

fRetCode = retcode;
fillList();

//------------------------------------------------------------------

MapSubwindows();

SetWindowName("Consistency Check Results");

Resize(GetDefaultSize());
MapWindow();

fClient->WaitFor(this);

}


//---------------------------------------------------------------------
//******  HConsistencyWarn::~HConsistencyWarn()
//
// Destructor of the class HConsistencyWarn.
//
//---------------------------------------------------------------------

HConsistencyWarn::~HConsistencyWarn() {
//
// Destructor of the class HConsistencyWarn.
//

delete fCancel;
delete fDelete;
delete fHiList;
delete fGCList;
delete fLCList;
delete fF3;
delete fF2;
delete fF1;
delete fFrame;

}


//---------------------------------------------------------------------
//******  void HConsistencyWarn::~CloseWindow()
//
// This function closes the window.
//
//---------------------------------------------------------------------

void HConsistencyWarn::CloseWindow() {
//
// This function closes the window.
//

delete this;

}


//---------------------------------------------------------------------
//******  Bool_t HConsistencyWarn::ProcessMessage(Long_t msg, Long_t parm1,
//                                                            Long_t parm2)
//
// This function processes all GUI widgets.
//
//---------------------------------------------------------------------

Bool_t HConsistencyWarn::ProcessMessage(Long_t msg,Long_t parm1,Long_t parm2) {
//
// This function processes all GUI widgets.
//

switch(GET_MSG(msg)) {

 case kC_COMMAND:

  switch(GET_SUBMSG(msg)) {

  case kCM_BUTTON:

    switch(parm1) {

    case 10:
      if(fRetCode) *fRetCode = 1;
      CloseWindow();
      break;

    case 11:
      if(fRetCode) *fRetCode = 2;
      CloseWindow();
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


//---------------------------------------------------------------------
//******  void HConsistencyWarn::fillList()
//
// This function fills internal arrays of the class HConsistency with
// the indexes of inconsistent objects.
//
//---------------------------------------------------------------------

void HConsistencyWarn::fillList() {
//
// This function fills internal arrays of the class HConsistency with
// the indexes of inconsistent objects.
//

  if(!gOnline) return;

  Int_t i;
  Int_t j;
  HLocalCondition *pLocal = 0;
  HGlobalCondition *pGlobal = 0;
  HHistogram *pHisto = 0;

  j = 0;
  for(i=0; i < (*gOnline->getConsistency()).fLCIdx.fN; i++) {

    if((*gOnline->getConsistency()).fLCIdx[i] == 1) {
      pLocal = gOnline->getLocal()->at(i);
      if(pLocal) fLCList->AddEntry(pLocal->GetName(),++j);
    }
  } 
  j = 0;
  for(i=0; i < (*gOnline->getConsistency()).fGCIdx.fN; i++) {

    if((*gOnline->getConsistency()).fGCIdx[i] == 1) {
      pGlobal = gOnline->getGlobal()->at(i);
      if(pGlobal) fGCList->AddEntry(pGlobal->GetName(),++j);
    }
  } 
  j = 0;
  for(i=0; i < (*gOnline->getConsistency()).fHiIdx.fN; i++) {

    if((*gOnline->getConsistency()).fHiIdx[i] == 1) {
      pHisto = gOnline->getHisto()->at(i);
      if(pHisto) fHiList->AddEntry(pHisto->GetName(),++j);
    }
  } 

}








