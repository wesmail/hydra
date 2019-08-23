//*-- Author : Dmitry Vasiliev (vassili@lns.infn.it)
/* 
 * Copyright P.Finocchiaro and D.Vasiliev (INFN-LNS, Catania) HADES collaboration
*/

//-------------------------------------------------------------------------
//
// File name:       hconversion.cc
// Subject:         utility GUI class to provide a conversion from the 
//                  "global"scintillator number (1-384) to 
//                  the set of indexes sector/module/strip.
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            November 2000
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the source code for the member functions of
// the class HConvertion.
// This is a small utility GUI class destined to help a user in converting
// scintillator numbers to their indexes in the structure sector/module/strip.
//
//------------------------------------------------------------------------

#include "hconversion.h"
#include "hindextransform.h"

#include "TGLabel.h"
#include <stdlib.h>

//_HADES_CLASS_DESCRIPTION 
/////////////////////////////////////////////////////////////////////
// This is a small utility GUI class destined to help a user in converting
// scintillator numbers to their indexes in the structure sector/module/strip.
////////////////////////////////////////////////////////////////////
ClassImp(HConversion)

//---------------------------------------------------------------------
//******  HConversion::HConversion(const TGWindow *p, UInt_t w, UInt_t h)
//
// Constructor of the class HConversion.
//
//---------------------------------------------------------------------

HConversion::HConversion(const TGWindow *p, UInt_t w, UInt_t h) :
  TGMainFrame(p, w, h) {
//
// Constructor of the class HConversion.
//

fFrame = new TGCompositeFrame(this, 500, 400, kVerticalFrame);
AddFrame(fFrame, new TGLayoutHints(kLHintsNormal));

fF1 = new TGCompositeFrame(fFrame, 500, 100, kHorizontalFrame);
fFrame->AddFrame(fF1, new TGLayoutHints(kLHintsNormal,0,0,5,2));

fF2 = new TGCompositeFrame(fFrame, 500, 100, kHorizontalFrame);
fFrame->AddFrame(fF2, new TGLayoutHints(kLHintsNormal,0,0,10,5));

//------------------------------------------------------------------

fF1->AddFrame(new TGLabel(fF1, new TGHotString("strip number (1-384)")),
	      new TGLayoutHints(kLHintsNormal, 2,2,4,2));

fNumber = new TGTextEntry(fF1, fNumberBuf = new TGTextBuffer(100), -1);
fNumberBuf->AddText(0,"0");
fNumber->Associate(this);
fF1->AddFrame(fNumber, new TGLayoutHints(kLHintsNormal,5,5,3,3));
fNumber->Resize(40,fNumber->GetDefaultHeight());
 
fConvert = new TGTextButton(fF1,"Convert",100);
fConvert->Associate(this);
fConvert->SetToolTipText("Convert the index");
fF1->AddFrame(fConvert, new TGLayoutHints(kLHintsNormal,10,2,3,3));

//------------------------------------------------------------------

fF2->AddFrame(new TGLabel(fF2, new TGHotString("sector")),
	      new TGLayoutHints(kLHintsNormal, 2,2,3,2));
fSecList = new TGListBox(fF2,-1);
fSecList->Associate(this);
fSecList->AddEntry("0",1);
fF2->AddFrame(fSecList, new TGLayoutHints(kLHintsNormal,2,2,2,2));
fSecList->Resize(35,20); 

fF2->AddFrame(new TGLabel(fF2, new TGHotString("module")),
	      new TGLayoutHints(kLHintsNormal, 6,2,3,2));
fModList = new TGListBox(fF2,-1);
fModList->Associate(this);
fModList->AddEntry("0",1);
fF2->AddFrame(fModList, new TGLayoutHints(kLHintsNormal,2,2,2,2));
fModList->Resize(35,20); 

fF2->AddFrame(new TGLabel(fF2, new TGHotString("strip")),
	      new TGLayoutHints(kLHintsNormal, 6,2,3,2));
fRodList = new TGListBox(fF2,-1);
fRodList->Associate(this);
fRodList->AddEntry("0",1);
fF2->AddFrame(fRodList, new TGLayoutHints(kLHintsNormal,2,2,2,2));
fRodList->Resize(35,20); 

countConvert++;

MapSubwindows();

SetWindowName("TOF Index Conversion");

Resize(GetDefaultSize());
MapWindow();

}


//---------------------------------------------------------------------
//******  HConversion::~HConversion()
//
// Destructor of the class HConversion.
//
//---------------------------------------------------------------------

HConversion::~HConversion() {
//
// Destructor of the class HConversion.
//

delete fRodList;
delete fModList;
delete fSecList;
delete fNumber;
delete fF2;
delete fF1;
delete fFrame;

countConvert--;

}


//---------------------------------------------------------------------
//******  void HConversion::~CloseWindow()
//
// This function closes the window.
//
//---------------------------------------------------------------------

void HConversion::CloseWindow() {
//
// This function closes the window.
//

delete this;

}


//---------------------------------------------------------------------
//******  Bool_t HConversion::ProcessMessage(Long_t msg, Long_t parm1,
//                                                       Long_t parm2)
//
// This function processes all GUI widgets.
//
//---------------------------------------------------------------------

Bool_t HConversion::ProcessMessage(Long_t msg,Long_t parm1,Long_t parm2) {
//
// This function processes all GUI widgets.
//

Int_t index;
Int_t maxind[3] = {6,8,8};
Int_t idx[3];
Char_t tmp[10];

switch(GET_MSG(msg)) {

 case kC_COMMAND:

  switch(GET_SUBMSG(msg)) {

  case kCM_BUTTON:

    switch(parm1) {

    case 100:
      index = atoi(fNumber->GetText());
      index--;
      if(index>=0 && index<384) {
	HIndexTransform::lin2mult(3,maxind,index,idx);
	sprintf(tmp,"%d",idx[0]+1);
	fSecList->RemoveEntry(1);
	fSecList->AddEntry(tmp,1);
	fSecList->MapSubwindows();
	fSecList->Layout();
	sprintf(tmp,"%d",idx[1]+1);
	fModList->RemoveEntry(1);
	fModList->AddEntry(tmp,1);
	fModList->MapSubwindows();
	fModList->Layout();
	sprintf(tmp,"%d",idx[2]+1);
	fRodList->RemoveEntry(1);
	fRodList->AddEntry(tmp,1);
	fRodList->MapSubwindows();
	fRodList->Layout();
      }
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










