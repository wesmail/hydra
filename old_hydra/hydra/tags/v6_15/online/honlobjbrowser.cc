//*-- Author : Dmitry Vasiliev (vassili@lns.infn.it)
/* 
 * Copyright P.Finocchiaro and D.Vasiliev (INFN-LNS, Catania) HADES collaboration
*/


//-------------------------------------------------------------------------
//
// File name:       honlobjbrowser.cc
// Subject:         Manager and browser of online objects.
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            March 2000
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the source code for the member functions of 
// the class HOnlObjBrowser.
// HOnlObjBrowser is a GUI class which maintains the panel for managing
// and browsing the online objects of HTOP.
//
//-------------------------------------------------------------------------
using namespace std;
#include "honlobjbrowser.h"
#include "honlinecontrol.h"
#include "hconsistency.h"
#include "hdetsetcont.h"
#include "hdetset.h"
#include "hhistocont.h"
#include "hhistogram.h"
#include "hlocalcont.h"
#include "hlocalcondition.h"
#include "hglobalcont.h"
#include "hglobalcondition.h"
#include "hparameter.h"
#include <TGLabel.h>
#include <TCanvas.h>
#include "hcontour.h"
#include "hcontourcont.h"
#include "herror.h"
#include "hexpstring.h"
#include "hbrowserdb.h"
#include <TStyle.h>
#include "hhistovisual.h"
#include "hmemorycheck.h"
//#include "hprojection.h"

#include <stdlib.h>
#include <ctype.h>
#include <iostream> 
#include <iomanip>

///////////////////////////////////////////////////////////////////////
// HOnlObjBrowser is a GUI class which maintains the panel for managing
// and browsing the online objects of HTOP.
///////////////////////////////////////////////////////////////////////
ClassImp(HOnlObjBrowser)


HExpString expStringL;
HExpString expStringG;

char wordpart[50];

//---------------------------------------------------------------------
//******  HOnlObjBrowser::HOnlObjBrowser(const TGWindow,UInt_t w,UInt_t h)
//
// Constructor of the class HOnlObjBrowser.
//
//---------------------------------------------------------------------

HOnlObjBrowser::HOnlObjBrowser(const TGWindow *p, UInt_t w, UInt_t h) :
  TGMainFrame(p, w, h) {
//
// Constructor of the class HOnlObjBrowser.
//

//---------------------------------------------------------------
//********* main frames
//---------------------------------------------------------------

fFrame = new TGCompositeFrame(this, 800, 700, kVerticalFrame);
AddFrame(fFrame, new TGLayoutHints(kLHintsNormal));

fF0 = new TGCompositeFrame(fFrame, 800, 650, kHorizontalFrame);
fFrame->AddFrame(fF0, new TGLayoutHints(kLHintsNormal));

fF00 = new TGCompositeFrame(fF0, 700, 650, kVerticalFrame);
fF0->AddFrame(fF00, new TGLayoutHints(kLHintsNormal));

fF01 = new TGCompositeFrame(fF0, 100, 650, kVerticalFrame);
fF0->AddFrame(fF01, new TGLayoutHints(kLHintsNormal,10,5,9,0));

fMainTab = new TGTab(fF00, 700, 650);
fMainTab->Associate(this);
fF00->AddFrame(fMainTab, new TGLayoutHints(kLHintsNormal));

//---------------------------------------------------------------
//********* end of main frames
//---------------------------------------------------------------

//---------------------------------------------------------------
//********* detector sets definition
//---------------------------------------------------------------

TGCompositeFrame *tf = fMainTab->AddTab("Detector sets");

fDSFrame = new TGCompositeFrame(tf, 700, 650, kVerticalFrame);
tf->AddFrame(fDSFrame, new TGLayoutHints(kLHintsNormal));

fDSTab = new TGTab(fDSFrame, 700, 650);
fDSTab->Associate(this);
fDSFrame->AddFrame(fDSTab, new TGLayoutHints(kLHintsNormal));

//---------------------------------------------------------------
//********* TOF panel
//---------------------------------------------------------------

TGCompositeFrame *tf2 = fDSTab->AddTab("TOF");

fTofFrame = new TGCompositeFrame(tf2, 700, 650, kVerticalFrame);
tf2->AddFrame(fTofFrame, new TGLayoutHints(kLHintsNormal));

fTof1 = new TGCompositeFrame(fTofFrame,700,100, kHorizontalFrame);
fTofFrame->AddFrame(fTof1, new TGLayoutHints(kLHintsNormal));

fTof2 = new TGCompositeFrame(fTofFrame,700,550, kVerticalFrame);
fTofFrame->AddFrame(fTof2, new TGLayoutHints(kLHintsNormal));

//
//---- content of panel fTof1 -----------------------------------
//

fTofRBFrame = new TGCompositeFrame(fTof1,700,100,kHorizontalFrame);
fTof1->AddFrame(fTofRBFrame, new TGLayoutHints(kLHintsNormal));

fTofCatFrame = new TGCompositeFrame(fTof1,700,100,kHorizontalFrame);
fTof1->AddFrame(fTofCatFrame, new TGLayoutHints(kLHintsNormal));

fTofSingleRB = new TGRadioButton(fTofRBFrame, "single", M__TO_SINGLE);
fTofSingleRB->Associate(this);
fTofSingleRB->SetState(kButtonUp);
fTofRBFrame->AddFrame(fTofSingleRB, new TGLayoutHints(kLHintsNormal,25,2,5,1));

fTofMultiRB = new TGRadioButton(fTofRBFrame, "multi", M__TO_MULTI);
fTofMultiRB->Associate(this);
fTofMultiRB->SetState(kButtonUp);
fTofRBFrame->AddFrame(fTofMultiRB, new TGLayoutHints(kLHintsNormal,2,2,5,1));

fTofCatFrame->AddFrame(new TGLabel(fTofCatFrame,new TGHotString("category")),
		       new TGLayoutHints(kLHintsNormal,20,1,6,1));

fTofCatBox = new TGComboBox(fTofCatFrame, M__TO_CATEGBOX);
fTofCatBox->Associate(this);
char tofcatname[10];
sprintf(tofcatname,"%s","raw");
fTofCatBox->AddEntry(tofcatname,1);
sprintf(tofcatname,"%s","hit");
fTofCatBox->AddEntry(tofcatname,2);
fTofCatBox->Resize(100,20);
fTofCatBox->Select(1);
fTofCatFrame->AddFrame(fTofCatBox, new TGLayoutHints(kLHintsNormal,3,1,5,1));

fTofCatFrame->AddFrame(new TGLabel(fTofCatFrame, new TGHotString("name")),
		       new TGLayoutHints(kLHintsNormal, 20,1,6,1));

fTofName = new TGTextEntry(fTofCatFrame, 
			   fTofNameBuf = new TGTextBuffer(30), -1);
fTofCatFrame->AddFrame(fTofName, new TGLayoutHints(kLHintsNormal,5,1,5,1));
fTofName->Resize(120, fTofName->GetDefaultHeight());

//
//---- end of panel fTof1 ---------------------------------------
//
//
//---- content of panel fTof2 -----------------------------------
//
//
//---- TOF "single" panel ---------------------------------------
//

fTofSingleFrame = new TGGroupFrame(fTof2,"single",kVerticalFrame);
fTof2->AddFrame(fTofSingleFrame, new TGLayoutHints(kLHintsNormal));

fTof21 = new TGCompositeFrame(fTofSingleFrame,700,200,kHorizontalFrame);
fTofSingleFrame->AddFrame(fTof21, new TGLayoutHints(kLHintsNormal));

fTof22 = new TGCompositeFrame(fTofSingleFrame,700,200,kHorizontalFrame);
fTofSingleFrame->AddFrame(fTof22, new TGLayoutHints(kLHintsNormal));

fTof21->AddFrame(new TGLabel(fTof21,new TGHotString("sector")),
		 new TGLayoutHints(kLHintsNormal,130,2,2,2));
fTof21->AddFrame(new TGLabel(fTof21,new TGHotString("module")),
		 new TGLayoutHints(kLHintsNormal,70,2,2,2));
fTof21->AddFrame(new TGLabel(fTof21,new TGHotString("rod")),
		 new TGLayoutHints(kLHintsNormal,70,2,2,2));

fTofSecBox = new TGComboBox(fTof22, M__TO_SECBOX);
fTofSecBox->Associate(this);
fTofSecBox->Resize(100,20);
fTof22->AddFrame(fTofSecBox, new TGLayoutHints(kLHintsNormal, 102,2,0,0));

fTofModBox = new TGComboBox(fTof22, M__TO_MODBOX);
fTofModBox->Associate(this);
fTofModBox->Resize(100,20);
fTof22->AddFrame(fTofModBox, new TGLayoutHints(kLHintsNormal,5,2,0,0));

fTofRodBox = new TGComboBox(fTof22, M__TO_RODBOX);
fTofRodBox->Associate(this);
fTofRodBox->Resize(100,20);
fTof22->AddFrame(fTofRodBox, new TGLayoutHints(kLHintsNormal,5,79,0,0));

//
//---- end of TOF "single" panel --------------------------------
//
//
//---- TOF radio buttons layout frame ---------------------------
//

fTofRBLayFrame = new TGCompositeFrame(fTof2,800,100,kHorizontalFrame);
fTof2->AddFrame(fTofRBLayFrame, new TGLayoutHints(kLHintsNormal));

fTofSinDefRB = new TGRadioButton(fTofRBLayFrame, "single", M__TO_SINLAY);
fTofSinDefRB->Associate(this);
fTofSinDefRB->SetState(kButtonDown);
fTofRBLayFrame->AddFrame(fTofSinDefRB, 
			 new TGLayoutHints(kLHintsNormal,5,2,2,2));

fTofRecDefRB = new TGRadioButton(fTofRBLayFrame, "rectangle", M__TO_RECLAY);
fTofRecDefRB->Associate(this);
fTofRecDefRB->SetState(kButtonUp);
fTofRBLayFrame->AddFrame(fTofRecDefRB, 
			 new TGLayoutHints(kLHintsNormal,5,2,2,2));

fTofAddDefRB = new TGRadioButton(fTofRBLayFrame, "add", M__TO_ADDLAY);
fTofAddDefRB->Associate(this);
fTofAddDefRB->SetState(kButtonDown);
fTofRBLayFrame->AddFrame(fTofAddDefRB, 
			 new TGLayoutHints(kLHintsNormal,40,2,2,2));

fTofSubDefRB = new TGRadioButton(fTofRBLayFrame, "subtract", M__TO_SUBLAY);
fTofSubDefRB->Associate(this);
fTofSubDefRB->SetState(kButtonUp);
fTofRBLayFrame->AddFrame(fTofSubDefRB, 
			 new TGLayoutHints(kLHintsNormal,5,2,2,2));

//
//---- end of TOF radio buttons layout frame --------------------
//
//
//---- TOF "multi" panel ----------------------------------------
//

fTofMultiFrame = new TGGroupFrame(fTof2,"multi",kHorizontalFrame);
fTof2->AddFrame(fTofMultiFrame, new TGLayoutHints(kLHintsNormal));

fTof23 = new TGCompositeFrame(fTofMultiFrame, 100, 200, kVerticalFrame);
fTofMultiFrame->AddFrame(fTof23, new TGLayoutHints(kLHintsNormal));

fTof23->AddFrame(new TGLabel(fTof23, new TGHotString("sec1")),
		 new TGLayoutHints(kLHintsNormal, 65,5,38,2));
fTof23->AddFrame(new TGLabel(fTof23, new TGHotString("sec2")),
		 new TGLayoutHints(kLHintsNormal, 65,5,15,2));
fTof23->AddFrame(new TGLabel(fTof23, new TGHotString("sec3")),
		 new TGLayoutHints(kLHintsNormal, 65,5,15,2));
fTof23->AddFrame(new TGLabel(fTof23, new TGHotString("sec4")),
		 new TGLayoutHints(kLHintsNormal, 65,5,15,2));
fTof23->AddFrame(new TGLabel(fTof23, new TGHotString("sec5")),
		 new TGLayoutHints(kLHintsNormal, 65,5,15,2));
fTof23->AddFrame(new TGLabel(fTof23, new TGHotString("sec6")),
		 new TGLayoutHints(kLHintsNormal, 65,5,15,0));

fTof24 = new TGCompositeFrame(fTofMultiFrame, 300, 200, kVerticalFrame);
fTofMultiFrame->AddFrame(fTof24, new TGLayoutHints(kLHintsNormal));

fTof25 = new TGCompositeFrame(fTof24, 300, 50, kHorizontalFrame);
fTof24->AddFrame(fTof25, new TGLayoutHints(kLHintsNormal));

fTof25->AddFrame(new TGLabel(fTof25, new TGHotString("mod1")),
		 new TGLayoutHints(kLHintsNormal, 12,2,5,3));
fTof25->AddFrame(new TGLabel(fTof25, new TGHotString("mod2")),
		 new TGLayoutHints(kLHintsNormal, 7,2,5,3));
fTof25->AddFrame(new TGLabel(fTof25, new TGHotString("mod3")),
		 new TGLayoutHints(kLHintsNormal, 7,2,5,3));
fTof25->AddFrame(new TGLabel(fTof25, new TGHotString("mod4")),
		 new TGLayoutHints(kLHintsNormal, 7,2,5,3));
fTof25->AddFrame(new TGLabel(fTof25, new TGHotString("mod5")),
		 new TGLayoutHints(kLHintsNormal, 7,2,5,3));
fTof25->AddFrame(new TGLabel(fTof25, new TGHotString("mod6")),
		 new TGLayoutHints(kLHintsNormal, 7,2,5,3));
fTof25->AddFrame(new TGLabel(fTof25, new TGHotString("mod7")),
		 new TGLayoutHints(kLHintsNormal, 7,2,5,3));
fTof25->AddFrame(new TGLabel(fTof25, new TGHotString("mod8")),
		 new TGLayoutHints(kLHintsNormal, 7,83,5,3));

fTof26 = new TGCompositeFrame(fTof24, 400, 200, kVerticalFrame);
fTof24->AddFrame(fTof26, new TGLayoutHints(kLHintsNormal));
fTof26->SetLayoutManager(new TGMatrixLayout(fTof26, 0, 8, 11));

for(Int_t i=0; i<48; i++) {
  fTofSecModBut[i] = new TGTextButton(fTof26, "     ", 500+i);
  fTofSecModBut[i]->Associate(this);
  fTofSecModBut[i]->Resize(30,20);
  fTofSecModBut[i]->AllowStayDown(kTRUE);
  fTofSecModBut[i]->SetState(kButtonEngaged);
  fTof26->AddFrame(fTofSecModBut[i]);
  kTofColor[i] = kFALSE;
  countTof = 2;
}

//
//---- end of TOF "multi" panel ---------------------------------
//
//
//---- end of panel fTof2 ---------------------------------------
//
//---------------------------------------------------------------
//********* end of TOF panel
//---------------------------------------------------------------

//---------------------------------------------------------------
//********* MDC panel
//---------------------------------------------------------------

tf2 = fDSTab->AddTab("MDC");

fMdcFrame = new TGCompositeFrame(tf2, 700, 650, kVerticalFrame);
tf2->AddFrame(fMdcFrame, new TGLayoutHints(kLHintsNormal));

fMdc1 = new TGCompositeFrame(fMdcFrame,600,100, kHorizontalFrame);
fMdcFrame->AddFrame(fMdc1, new TGLayoutHints(kLHintsNormal));

fMdc2 = new TGCompositeFrame(fMdcFrame,600,500, kVerticalFrame);
fMdcFrame->AddFrame(fMdc2, new TGLayoutHints(kLHintsNormal));

//
//---- content of panel fMdc1 -----------------------------------
//

fMdcRBFrame = new TGCompositeFrame(fMdc1,700,100,kHorizontalFrame);
fMdc1->AddFrame(fMdcRBFrame, new TGLayoutHints(kLHintsNormal));

fMdcCatFrame = new TGCompositeFrame(fMdc1,700,100,kHorizontalFrame);
fMdc1->AddFrame(fMdcCatFrame, new TGLayoutHints(kLHintsNormal));

fMdcSingleRB = new TGRadioButton(fMdcRBFrame, "single", M__MD_SINGLE);
fMdcSingleRB->Associate(this);
fMdcSingleRB->SetState(kButtonUp);
fMdcRBFrame->AddFrame(fMdcSingleRB, new TGLayoutHints(kLHintsNormal,25,2,5,1));

fMdcMultiRB = new TGRadioButton(fMdcRBFrame, "multi", M__MD_MULTI);
fMdcMultiRB->Associate(this);
fMdcMultiRB->SetState(kButtonUp);
fMdcRBFrame->AddFrame(fMdcMultiRB, new TGLayoutHints(kLHintsNormal,2,2,5,1));

fMdcCatFrame->AddFrame(new TGLabel(fMdcCatFrame,new TGHotString("category")),
		       new TGLayoutHints(kLHintsNormal,20,1,6,1));

fMdcCatBox = new TGComboBox(fMdcCatFrame, M__MD_CATEGBOX);
fMdcCatBox->Associate(this);
char mdccatname[10];
sprintf(mdccatname,"%s","cal1");
fMdcCatBox->AddEntry(mdccatname,1);
sprintf(mdccatname,"%s","cal2");
fMdcCatBox->AddEntry(mdccatname,2);
sprintf(mdccatname,"%s","hit");
fMdcCatBox->AddEntry(mdccatname,3);
fMdcCatBox->Resize(100,20);
fMdcCatBox->Select(1);

fMdcCatFrame->AddFrame(fMdcCatBox, new TGLayoutHints(kLHintsNormal,3,1,5,1));

fMdcCatFrame->AddFrame(new TGLabel(fMdcCatFrame, new TGHotString("name")),
		       new TGLayoutHints(kLHintsNormal, 20,1,6,1));

fMdcName = new TGTextEntry(fMdcCatFrame, 
			   fMdcNameBuf = new TGTextBuffer(30), -1);
fMdcCatFrame->AddFrame(fMdcName, new TGLayoutHints(kLHintsNormal,5,1,5,1));
fMdcName->Resize(120, fMdcName->GetDefaultHeight());

//
//---- end of panel fMdc1 ---------------------------------------
//
//
//---- content of panel fMdc2 -----------------------------------
//
//
//---- MDC "single" panel ---------------------------------------
//

fMdcSingleFrame = new TGGroupFrame(fMdc2,"single",kVerticalFrame);
fMdc2->AddFrame(fMdcSingleFrame, new TGLayoutHints(kLHintsNormal));

fMdc21 = new TGCompositeFrame(fMdcSingleFrame,700,200,kHorizontalFrame);
fMdcSingleFrame->AddFrame(fMdc21, new TGLayoutHints(kLHintsNormal));

fMdc22 = new TGCompositeFrame(fMdcSingleFrame,700,200,kHorizontalFrame);
fMdcSingleFrame->AddFrame(fMdc22, new TGLayoutHints(kLHintsNormal));

fMdc21->AddFrame(new TGLabel(fMdc21,new TGHotString("sector")),
		 new TGLayoutHints(kLHintsNormal,70,2,2,2));
fMdc21->AddFrame(new TGLabel(fMdc21,new TGHotString("module")),
		 new TGLayoutHints(kLHintsNormal,70,2,2,2));
fMdc21->AddFrame(new TGLabel(fMdc21,new TGHotString("layer")),
		 new TGLayoutHints(kLHintsNormal,70,2,2,2));
fMdc21->AddFrame(new TGLabel(fMdc21,new TGHotString("cell")),
		 new TGLayoutHints(kLHintsNormal,80,2,2,2));

fMdcSecBox = new TGComboBox(fMdc22, M__MD_SECBOX);
fMdcSecBox->Associate(this);
fMdcSecBox->Resize(100,20);
fMdc22->AddFrame(fMdcSecBox, new TGLayoutHints(kLHintsNormal, 38,2,0,0));

fMdcModBox = new TGComboBox(fMdc22, M__MD_MODBOX);
fMdcModBox->Associate(this);
fMdcModBox->Resize(100,20);
fMdc22->AddFrame(fMdcModBox, new TGLayoutHints(kLHintsNormal,5,2,0,0));

fMdcLayBox = new TGComboBox(fMdc22, M__MD_LAYBOX);
fMdcLayBox->Associate(this);
fMdcLayBox->Resize(100,20);
fMdc22->AddFrame(fMdcLayBox, new TGLayoutHints(kLHintsNormal,5,2,0,0));

fMdcCellBox = new TGComboBox(fMdc22, M__MD_CELLBOX);
fMdcCellBox->Associate(this);
fMdcCellBox->Resize(100,20);
fMdc22->AddFrame(fMdcCellBox, new TGLayoutHints(kLHintsNormal,5,38,0,0));

//
//---- end of MDC "single" panel --------------------------------
//
//
//---- MDC radio buttons layout frame ---------------------------
//
fMdcRBLayFrame = new TGCompositeFrame(fMdc2,800,100,kHorizontalFrame);
fMdc2->AddFrame(fMdcRBLayFrame, new TGLayoutHints(kLHintsNormal));

fMdcSinDefRB = new TGRadioButton(fMdcRBLayFrame, "single", M__MD_SINLAY);
fMdcSinDefRB->Associate(this);
fMdcSinDefRB->SetState(kButtonDown);
fMdcRBLayFrame->AddFrame(fMdcSinDefRB, 
			 new TGLayoutHints(kLHintsNormal,5,2,2,2));

fMdcRecDefRB = new TGRadioButton(fMdcRBLayFrame, "rectangle", M__MD_RECLAY);
fMdcRecDefRB->Associate(this);
fMdcRecDefRB->SetState(kButtonUp);
fMdcRBLayFrame->AddFrame(fMdcRecDefRB, 
			 new TGLayoutHints(kLHintsNormal,5,2,2,2));

fMdcAddDefRB = new TGRadioButton(fMdcRBLayFrame, "add", M__MD_ADDLAY);
fMdcAddDefRB->Associate(this);
fMdcAddDefRB->SetState(kButtonDown);
fMdcRBLayFrame->AddFrame(fMdcAddDefRB, 
			 new TGLayoutHints(kLHintsNormal,40,2,2,2));

fMdcSubDefRB = new TGRadioButton(fMdcRBLayFrame, "subtract", M__MD_SUBLAY);
fMdcSubDefRB->Associate(this);
fMdcSubDefRB->SetState(kButtonUp);
fMdcRBLayFrame->AddFrame(fMdcSubDefRB, 
			 new TGLayoutHints(kLHintsNormal,5,2,2,2));

//
//---- end of MDC radio buttons layout frame --------------------
//
//
//---- MDC "multi" panel ----------------------------------------
//

fMdcMultiFrame = new TGGroupFrame(fMdc2,"multi",kHorizontalFrame);
fMdc2->AddFrame(fMdcMultiFrame, new TGLayoutHints(kLHintsNormal));

//
//----- MDC ------ "step1" panel --------------------------------
//

fMdcStep1Frame = new TGGroupFrame(fMdcMultiFrame,"step 1",kHorizontalFrame);
fMdcMultiFrame->AddFrame(fMdcStep1Frame, new TGLayoutHints(kLHintsNormal));

fMdc23 = new TGCompositeFrame(fMdcStep1Frame, 100, 200, kVerticalFrame);
fMdcStep1Frame->AddFrame(fMdc23, new TGLayoutHints(kLHintsNormal,0,0,15,20));

fMdc23->AddFrame(new TGLabel(fMdc23, new TGHotString("sec1")),
		 new TGLayoutHints(kLHintsNormal, 0,0,23,2));
fMdc23->AddFrame(new TGLabel(fMdc23, new TGHotString("sec2")),
		 new TGLayoutHints(kLHintsNormal, 0,0,6,2));
fMdc23->AddFrame(new TGLabel(fMdc23, new TGHotString("sec3")),
		 new TGLayoutHints(kLHintsNormal, 0,0,6,2));
fMdc23->AddFrame(new TGLabel(fMdc23, new TGHotString("sec4")),
		 new TGLayoutHints(kLHintsNormal, 0,0,6,2));
fMdc23->AddFrame(new TGLabel(fMdc23, new TGHotString("sec5")),
		 new TGLayoutHints(kLHintsNormal, 0,0,6,2));
fMdc23->AddFrame(new TGLabel(fMdc23, new TGHotString("sec6")),
		 new TGLayoutHints(kLHintsNormal, 0,0,6,0));

fMdc24 = new TGCompositeFrame(fMdcStep1Frame, 300, 200, kVerticalFrame);
fMdcStep1Frame->AddFrame(fMdc24, new TGLayoutHints(kLHintsNormal,0,0,15,20));

fMdc25 = new TGCompositeFrame(fMdc24, 300, 50, kHorizontalFrame);
fMdc24->AddFrame(fMdc25, new TGLayoutHints(kLHintsNormal));

fMdc25->AddFrame(new TGLabel(fMdc25, new TGHotString("m1")),
		 new TGLayoutHints(kLHintsNormal, 7,3,2,2));
fMdc25->AddFrame(new TGLabel(fMdc25, new TGHotString("m2")),
		 new TGLayoutHints(kLHintsNormal, 10,3,2,2));
fMdc25->AddFrame(new TGLabel(fMdc25, new TGHotString("m3")),
		 new TGLayoutHints(kLHintsNormal, 10,3,2,2));
fMdc25->AddFrame(new TGLabel(fMdc25, new TGHotString("m4")),
		 new TGLayoutHints(kLHintsNormal, 10,3,2,2));

fMdc26 = new TGCompositeFrame(fMdc24, 400, 200, kVerticalFrame);
fMdc24->AddFrame(fMdc26, new TGLayoutHints(kLHintsNormal));
fMdc26->SetLayoutManager(new TGMatrixLayout(fMdc26, 0, 4, 2));

for(Int_t i=0; i<24; i++) {
  fMdcSecModBut[i] = new TGTextButton(fMdc26, "     ", 600+i);
  fMdcSecModBut[i]->Associate(this);
  fMdcSecModBut[i]->Resize(20,15);
  fMdcSecModBut[i]->AllowStayDown(kTRUE);
  fMdcSecModBut[i]->SetState(kButtonEngaged);
  fMdc26->AddFrame(fMdcSecModBut[i]);
  kMdcColor1[i] = kFALSE;
  countMdcS1 = 2;
}

//
//----- Mdc ------ "step2" panel --------------------------------
//

fMdcStep2Frame = new TGGroupFrame(fMdcMultiFrame,"step 2",kHorizontalFrame);
fMdcMultiFrame->AddFrame(fMdcStep2Frame, new TGLayoutHints(kLHintsNormal));

fMdc27 = new TGCompositeFrame(fMdcStep2Frame,300,200,kVerticalFrame);
fMdcStep2Frame->AddFrame(fMdc27, new TGLayoutHints(kLHintsNormal,2,2,5,2));

fMdc27->AddFrame(new TGLabel(fMdc27, new TGHotString("modules")),
		 new TGLayoutHints(kLHintsNormal, 1,0,2,2));

fMdcUp = new TGPictureButton(fMdc27, fClient->GetPicture("arrow_up.xpm"), 
			     M__MD_UP);
fMdcUp->Associate(this);
fMdc27->AddFrame(fMdcUp, new TGLayoutHints(kLHintsNormal,8,1,3,3));

fMdcModList = new TGListBox(fMdc27, M__MD_MODLIST);
fMdcModList->Associate(this);
fMdc27->AddFrame(fMdcModList, new TGLayoutHints(kLHintsNormal,0,0,3,1));
fMdcModList->Resize(40,122);

fMdcDown = new TGPictureButton(fMdc27, fClient->GetPicture("arrow_down.xpm"), 
			       M__MD_DOWN);
fMdcDown->Associate(this);
fMdc27->AddFrame(fMdcDown, new TGLayoutHints(kLHintsNormal,8,1,3,3));

fMdc28 = new TGCompositeFrame(fMdcStep2Frame,300,200,kHorizontalFrame);
fMdcStep2Frame->AddFrame(fMdc28, new TGLayoutHints(kLHintsNormal,1,1,7,2));

fMdc29 = new TGCompositeFrame(fMdc28, 100, 200, kVerticalFrame);
fMdc28->AddFrame(fMdc29, new TGLayoutHints(kLHintsNormal));

fMdc29->AddFrame(new TGLabel(fMdc29, new TGHotString("1")),
		 new TGLayoutHints(kLHintsNormal, 0,1,25,2));
fMdc29->AddFrame(new TGLabel(fMdc29, new TGHotString("2")),
		 new TGLayoutHints(kLHintsNormal, 0,1,4,2));
fMdc29->AddFrame(new TGLabel(fMdc29, new TGHotString("3")),
		 new TGLayoutHints(kLHintsNormal, 0,1,4,2));
fMdc29->AddFrame(new TGLabel(fMdc29, new TGHotString("4")),
		 new TGLayoutHints(kLHintsNormal, 0,1,4,2));
fMdc29->AddFrame(new TGLabel(fMdc29, new TGHotString("5")),
		 new TGLayoutHints(kLHintsNormal, 0,1,4,2));
fMdc29->AddFrame(new TGLabel(fMdc29, new TGHotString("6")),
		 new TGLayoutHints(kLHintsNormal, 0,1,4,2));

fMdc30 = new TGCompositeFrame(fMdc28, 300, 200, kVerticalFrame);
fMdc28->AddFrame(fMdc30, new TGLayoutHints(kLHintsNormal));

fMdc31 = new TGCompositeFrame(fMdc30, 300, 50, kHorizontalFrame);
fMdc30->AddFrame(fMdc31, new TGLayoutHints(kLHintsNormal));

fMdc31->AddFrame(new TGLabel(fMdc31, new TGHotString("25")),
		 new TGLayoutHints(kLHintsNormal, 10,0,2,2));
fMdc31->AddFrame(new TGLabel(fMdc31, new TGHotString("50")),
		 new TGLayoutHints(kLHintsNormal, 10,0,2,2));
fMdc31->AddFrame(new TGLabel(fMdc31, new TGHotString("75")),
		 new TGLayoutHints(kLHintsNormal, 10,0,2,2));
fMdc31->AddFrame(new TGLabel(fMdc31, new TGHotString("100")),
		 new TGLayoutHints(kLHintsNormal, 10,0,2,2));
fMdc31->AddFrame(new TGLabel(fMdc31, new TGHotString("125")),
		 new TGLayoutHints(kLHintsNormal, 9,0,2,2));
fMdc31->AddFrame(new TGLabel(fMdc31, new TGHotString("150")),
		 new TGLayoutHints(kLHintsNormal, 9,0,2,2));
fMdc31->AddFrame(new TGLabel(fMdc31, new TGHotString("175")),
		 new TGLayoutHints(kLHintsNormal, 9,0,2,2));
fMdc31->AddFrame(new TGLabel(fMdc31, new TGHotString("200")),
		 new TGLayoutHints(kLHintsNormal, 9,0,2,2));

fMdc32 = new TGCompositeFrame(fMdc30, 300, 200, kVerticalFrame);
fMdc30->AddFrame(fMdc32, new TGLayoutHints(kLHintsNormal));
fMdc32->SetLayoutManager(new TGMatrixLayout(fMdc32, 0, 8, 0));

for(Int_t i=0; i<48; i++) {
  fMdcCellsBut[i] = new TGTextButton(fMdc32, "     ", 630+i);
  fMdcCellsBut[i]->Associate(this);
  fMdcCellsBut[i]->Resize(20,20);
  fMdcCellsBut[i]->AllowStayDown(kTRUE);
  fMdcCellsBut[i]->SetState(kButtonEngaged);
  fMdc32->AddFrame(fMdcCellsBut[i]);
  kMdcColor2[i] = kFALSE;
  countMdcS2 = 2;
}

//
//---- end of MDC "multi" panel ---------------------------------
//
//
//---- end of panel fMdc2 ---------------------------------------
//
//---------------------------------------------------------------
//********* end of MDC panel
//---------------------------------------------------------------

//---------------------------------------------------------------
//********* RICH panel
//---------------------------------------------------------------

tf2 = fDSTab->AddTab("RICH");

fRichFrame = new TGCompositeFrame(tf2, 700, 650, kVerticalFrame);
tf2->AddFrame(fRichFrame, new TGLayoutHints(kLHintsNormal));

fRich1 = new TGCompositeFrame(fRichFrame,600,100, kHorizontalFrame);
fRichFrame->AddFrame(fRich1, new TGLayoutHints(kLHintsNormal));

fRich2 = new TGCompositeFrame(fRichFrame,600,500, kVerticalFrame);
fRichFrame->AddFrame(fRich2, new TGLayoutHints(kLHintsNormal));

//
//---- content of panel fRich1 -----------------------------------
//

fRichRBFrame = new TGCompositeFrame(fRich1,700,100,kHorizontalFrame);
fRich1->AddFrame(fRichRBFrame, new TGLayoutHints(kLHintsNormal));

fRichCatFrame = new TGCompositeFrame(fRich1,700,100,kHorizontalFrame);
fRich1->AddFrame(fRichCatFrame, new TGLayoutHints(kLHintsNormal));

fRichSingleRB = new TGRadioButton(fRichRBFrame, "single", M__RI_SINGLE);
fRichSingleRB->Associate(this);
fRichSingleRB->SetState(kButtonUp);
fRichRBFrame->AddFrame(fRichSingleRB, 
		       new TGLayoutHints(kLHintsNormal,25,2,5,1));

fRichMultiRB = new TGRadioButton(fRichRBFrame, "multi", M__RI_MULTI);
fRichMultiRB->Associate(this);
fRichMultiRB->SetState(kButtonUp);
fRichRBFrame->AddFrame(fRichMultiRB, new TGLayoutHints(kLHintsNormal,2,2,5,1));

fRichCatFrame->AddFrame(new TGLabel(fRichCatFrame,new TGHotString("category")),
			new TGLayoutHints(kLHintsNormal,20,1,6,1));

fRichCatBox = new TGComboBox(fRichCatFrame, M__RI_CATEGBOX);
fRichCatBox->Associate(this);
char richcatname[10];
sprintf(richcatname,"%s","raw");
fRichCatBox->AddEntry(richcatname,1);
sprintf(richcatname,"%s","cal");
fRichCatBox->AddEntry(richcatname,2);
sprintf(richcatname,"%s","hit");
fRichCatBox->AddEntry(richcatname,3);
fRichCatBox->Resize(100,20);
fRichCatBox->Select(3);

fRichCatFrame->AddFrame(fRichCatBox, 
			new TGLayoutHints(kLHintsNormal,3,1,5,1));

fRichCatFrame->AddFrame(new TGLabel(fRichCatFrame, new TGHotString("name")),
			new TGLayoutHints(kLHintsNormal, 20,1,6,1));

fRichName = new TGTextEntry(fRichCatFrame, 
			    fRichNameBuf = new TGTextBuffer(30), -1);
fRichCatFrame->AddFrame(fRichName, new TGLayoutHints(kLHintsNormal,5,1,5,1));
fRichName->Resize(120, fRichName->GetDefaultHeight());

//
//---- end of panel fRich1 --------------------------------------
//
//
//---- content of panel fRich2 ----------------------------------
//
//
//---- RICH "single" panel --------------------------------------
//

fRichSingleFrame = new TGGroupFrame(fRich2,"single",kVerticalFrame);
fRich2->AddFrame(fRichSingleFrame, new TGLayoutHints(kLHintsNormal));

fRich21 = new TGCompositeFrame(fRichSingleFrame,700,200,kHorizontalFrame);
fRichSingleFrame->AddFrame(fRich21, new TGLayoutHints(kLHintsNormal));

fRich22 = new TGCompositeFrame(fRichSingleFrame,700,200,kHorizontalFrame);
fRichSingleFrame->AddFrame(fRich22, new TGLayoutHints(kLHintsNormal));

fRich21->AddFrame(new TGLabel(fRich21,new TGHotString("sector")),
		  new TGLayoutHints(kLHintsNormal,130,2,2,2));
fRich21->AddFrame(new TGLabel(fRich21,new TGHotString("row")),
		  new TGLayoutHints(kLHintsNormal,70,2,2,2));
fRich21->AddFrame(new TGLabel(fRich21,new TGHotString("col")),
		  new TGLayoutHints(kLHintsNormal,90,2,2,2));

fRichSecBox = new TGComboBox(fRich22, M__RI_SECBOX);
fRichSecBox->Associate(this);
fRichSecBox->Resize(100,20);
fRich22->AddFrame(fRichSecBox, new TGLayoutHints(kLHintsNormal, 102,2,0,0));

fRichRowBox = new TGComboBox(fRich22, M__RI_ROWBOX);
fRichRowBox->Associate(this);
fRichRowBox->Resize(100,20);
fRich22->AddFrame(fRichRowBox, new TGLayoutHints(kLHintsNormal,5,2,0,0));

fRichColBox = new TGComboBox(fRich22, M__RI_COLBOX);
fRichColBox->Associate(this);
fRichColBox->Resize(100,20);
fRich22->AddFrame(fRichColBox, new TGLayoutHints(kLHintsNormal,5,79,0,0));

//
//---- end of RICH "single" panel -------------------------------
//
//
//---- RICH radio buttons layout frame --------------------------
//
fRichRBLayFrame = new TGCompositeFrame(fRich2,800,100,kHorizontalFrame);
fRich2->AddFrame(fRichRBLayFrame, new TGLayoutHints(kLHintsNormal));

fRichSinDefRB = new TGRadioButton(fRichRBLayFrame, "single", M__RI_SINLAY);
fRichSinDefRB->Associate(this);
fRichSinDefRB->SetState(kButtonDown);
fRichRBLayFrame->AddFrame(fRichSinDefRB, 
			  new TGLayoutHints(kLHintsNormal,5,2,2,2));

fRichRecDefRB = new TGRadioButton(fRichRBLayFrame, "rectangle", M__RI_RECLAY);
fRichRecDefRB->Associate(this);
fRichRecDefRB->SetState(kButtonUp);
fRichRBLayFrame->AddFrame(fRichRecDefRB, 
			  new TGLayoutHints(kLHintsNormal,5,2,2,2));

fRichAddDefRB = new TGRadioButton(fRichRBLayFrame, "add", M__RI_ADDLAY);
fRichAddDefRB->Associate(this);
fRichAddDefRB->SetState(kButtonDown);
fRichRBLayFrame->AddFrame(fRichAddDefRB, 
			  new TGLayoutHints(kLHintsNormal,40,2,2,2));

fRichSubDefRB = new TGRadioButton(fRichRBLayFrame, "subtract", M__RI_SUBLAY);
fRichSubDefRB->Associate(this);
fRichSubDefRB->SetState(kButtonUp);
fRichRBLayFrame->AddFrame(fRichSubDefRB, 
			  new TGLayoutHints(kLHintsNormal,5,2,2,2));

//
//---- end of RICH radio buttons layout frame -------------------
//
//
//---- RICH "multi" panel ---------------------------------------
//

fRichMultiFrame = new TGGroupFrame(fRich2,"multi",kHorizontalFrame);
fRich2->AddFrame(fRichMultiFrame, new TGLayoutHints(kLHintsNormal));

//
//----- RICH ------ "step1" panel --------------------------------
//

fRichStep1Frame = new TGGroupFrame(fRichMultiFrame,"step 1",kHorizontalFrame);
fRichMultiFrame->AddFrame(fRichStep1Frame, new TGLayoutHints(kLHintsNormal));

fRich23 = new TGCompositeFrame(fRichStep1Frame, 100, 200, kVerticalFrame);
fRichStep1Frame->AddFrame(fRich23, new TGLayoutHints(kLHintsNormal));

fRich23->AddFrame(new TGLabel(fRich23, new TGHotString("sec1")),
		  new TGLayoutHints(kLHintsNormal, 0,0,27,2));
fRich23->AddFrame(new TGLabel(fRich23, new TGHotString("sec2")),
		  new TGLayoutHints(kLHintsNormal, 0,0,11,2));
fRich23->AddFrame(new TGLabel(fRich23, new TGHotString("sec3")),
		  new TGLayoutHints(kLHintsNormal, 0,0,11,2));
fRich23->AddFrame(new TGLabel(fRich23, new TGHotString("sec4")),
		  new TGLayoutHints(kLHintsNormal, 0,0,11,2));
fRich23->AddFrame(new TGLabel(fRich23, new TGHotString("sec5")),
		  new TGLayoutHints(kLHintsNormal, 0,0,11,2));
fRich23->AddFrame(new TGLabel(fRich23, new TGHotString("sec6")),
		  new TGLayoutHints(kLHintsNormal, 0,0,11,0));

fRich24 = new TGCompositeFrame(fRichStep1Frame, 300, 200, kVerticalFrame);
fRichStep1Frame->AddFrame(fRich24, new TGLayoutHints(kLHintsNormal));

fRich25 = new TGCompositeFrame(fRich24, 300, 50, kHorizontalFrame);
fRich24->AddFrame(fRich25, new TGLayoutHints(kLHintsNormal));

fRich25->AddFrame(new TGLabel(fRich25, new TGHotString("mod1")),
		  new TGLayoutHints(kLHintsNormal, 3,2,2,2));

fRich26 = new TGCompositeFrame(fRich24, 400, 200, kVerticalFrame);
fRich24->AddFrame(fRich26, new TGLayoutHints(kLHintsNormal));
fRich26->SetLayoutManager(new TGMatrixLayout(fRich26, 0, 1, 7));

for(Int_t i=0; i<6; i++) {
  fRichSecModBut[i] = new TGTextButton(fRich26, "     ", 700+i);
  fRichSecModBut[i]->Associate(this);
  fRichSecModBut[i]->Resize(20,15);
  fRichSecModBut[i]->AllowStayDown(kTRUE);
  fRichSecModBut[i]->SetState(kButtonEngaged);
  fRich26->AddFrame(fRichSecModBut[i]);
  kRichColor1[i] = kFALSE;
  countRichS1 = 2;
}

//
//----- Rich ------ "step2" panel ------------------------------
//

fRichStep2Frame = new TGGroupFrame(fRichMultiFrame,"step 2",kHorizontalFrame);
fRichMultiFrame->AddFrame(fRichStep2Frame, new TGLayoutHints(kLHintsNormal));

fRich27 = new TGCompositeFrame(fRichStep2Frame,300,200,kVerticalFrame);
fRichStep2Frame->AddFrame(fRich27, new TGLayoutHints(kLHintsNormal,2,2,2,2));

fRich27->AddFrame(new TGLabel(fRich27, new TGHotString("modules")),
		  new TGLayoutHints(kLHintsNormal, 1,0,2,2));

fRichUp = new TGPictureButton(fRich27, fClient->GetPicture("arrow_up.xpm"), 
			      M__RI_UP);
fRichUp->Associate(this);
fRich27->AddFrame(fRichUp, new TGLayoutHints(kLHintsNormal,8,1,3,3));

fRichModList = new TGListBox(fRich27, M__RI_MODLIST);
fRichModList->Associate(this);
fRich27->AddFrame(fRichModList, new TGLayoutHints(kLHintsNormal,3,0,3,1));
fRichModList->Resize(40,125);

fRichDown = new TGPictureButton(fRich27,fClient->GetPicture("arrow_down.xpm"), 
				M__RI_DOWN);
fRichDown->Associate(this);
fRich27->AddFrame(fRichDown, new TGLayoutHints(kLHintsNormal,8,1,3,3));

fRich28 = new TGCompositeFrame(fRichStep2Frame,300,200,kHorizontalFrame);
fRichStep2Frame->AddFrame(fRich28, new TGLayoutHints(kLHintsNormal,35,35,2,2));

fRich29 = new TGCompositeFrame(fRich28, 100, 200, kVerticalFrame);
fRich28->AddFrame(fRich29, new TGLayoutHints(kLHintsNormal));

fRich29->AddFrame(new TGLabel(fRich29, new TGHotString("12")),
		  new TGLayoutHints(kLHintsNormal, 0,3,25,2));
fRich29->AddFrame(new TGLabel(fRich29, new TGHotString("24")),
		  new TGLayoutHints(kLHintsNormal, 0,3,4,2));
fRich29->AddFrame(new TGLabel(fRich29, new TGHotString("36")),
		  new TGLayoutHints(kLHintsNormal, 0,3,4,2));
fRich29->AddFrame(new TGLabel(fRich29, new TGHotString("48")),
		  new TGLayoutHints(kLHintsNormal, 0,3,4,2));
fRich29->AddFrame(new TGLabel(fRich29, new TGHotString("60")),
		  new TGLayoutHints(kLHintsNormal, 0,3,4,2));
fRich29->AddFrame(new TGLabel(fRich29, new TGHotString("72")),
		  new TGLayoutHints(kLHintsNormal, 0,3,4,2));
fRich29->AddFrame(new TGLabel(fRich29, new TGHotString("84")),
		  new TGLayoutHints(kLHintsNormal, 0,3,4,2));
fRich29->AddFrame(new TGLabel(fRich29, new TGHotString("96")),
		  new TGLayoutHints(kLHintsNormal, 0,3,4,2));

fRich30 = new TGCompositeFrame(fRich28, 300, 200, kVerticalFrame);
fRich28->AddFrame(fRich30, new TGLayoutHints(kLHintsNormal));

fRich31 = new TGCompositeFrame(fRich30, 300, 50, kHorizontalFrame);
fRich30->AddFrame(fRich31, new TGLayoutHints(kLHintsNormal));

fRich31->AddFrame(new TGLabel(fRich31, new TGHotString("12")),
		  new TGLayoutHints(kLHintsNormal, 8,0,2,2));
fRich31->AddFrame(new TGLabel(fRich31, new TGHotString("24")),
		  new TGLayoutHints(kLHintsNormal, 14,0,2,2));
fRich31->AddFrame(new TGLabel(fRich31, new TGHotString("36")),
		  new TGLayoutHints(kLHintsNormal, 14,0,2,2));
fRich31->AddFrame(new TGLabel(fRich31, new TGHotString("48")),
		  new TGLayoutHints(kLHintsNormal, 14,0,2,2));
fRich31->AddFrame(new TGLabel(fRich31, new TGHotString("60")),
		  new TGLayoutHints(kLHintsNormal, 14,0,2,2));
fRich31->AddFrame(new TGLabel(fRich31, new TGHotString("72")),
		  new TGLayoutHints(kLHintsNormal, 14,0,2,2));
fRich31->AddFrame(new TGLabel(fRich31, new TGHotString("84")),
		  new TGLayoutHints(kLHintsNormal, 14,0,2,2));
fRich31->AddFrame(new TGLabel(fRich31, new TGHotString("96")),
		  new TGLayoutHints(kLHintsNormal, 14,0,2,2));

fRich32 = new TGCompositeFrame(fRich30, 300, 200, kVerticalFrame);
fRich30->AddFrame(fRich32, new TGLayoutHints(kLHintsNormal));
fRich32->SetLayoutManager(new TGMatrixLayout(fRich32, 0, 8, 0));

for(Int_t i=0; i<64; i++) {
  fRichCellsBut[i] = new TGTextButton(fRich32, "     ", 730+i);
  fRichCellsBut[i]->Associate(this);
  fRichCellsBut[i]->Resize(20,20);
  fRichCellsBut[i]->AllowStayDown(kTRUE);
  fRichCellsBut[i]->SetState(kButtonEngaged);
  fRich32->AddFrame(fRichCellsBut[i]);
  kRichColor2[i] = kFALSE;
  countRichS2 = 2;
}

//
//---- end of RICH "multi" panel ---------------------------------
//
//
//---- end of panel fRich2 ---------------------------------------
//
//---------------------------------------------------------------
//********* end of RICH panel
//---------------------------------------------------------------

//---------------------------------------------------------------
//********* Shower panel
//---------------------------------------------------------------

tf2 = fDSTab->AddTab("SHOWER");

fShFrame = new TGCompositeFrame(tf2, 700, 650, kVerticalFrame);
tf2->AddFrame(fShFrame, new TGLayoutHints(kLHintsNormal));

fSh1 = new TGCompositeFrame(fShFrame,600,100, kHorizontalFrame);
fShFrame->AddFrame(fSh1, new TGLayoutHints(kLHintsNormal));

fSh2 = new TGCompositeFrame(fShFrame,600,500, kVerticalFrame);
fShFrame->AddFrame(fSh2, new TGLayoutHints(kLHintsNormal));

//
//---- content of panel fSh1 -----------------------------------
//

fShRBFrame = new TGCompositeFrame(fSh1,700,100,kHorizontalFrame);
fSh1->AddFrame(fShRBFrame, new TGLayoutHints(kLHintsNormal));

fShCatFrame = new TGCompositeFrame(fSh1,700,100,kHorizontalFrame);
fSh1->AddFrame(fShCatFrame, new TGLayoutHints(kLHintsNormal));

fShSingleRB = new TGRadioButton(fShRBFrame, "single", M__SH_SINGLE);
fShSingleRB->Associate(this);
fShSingleRB->SetState(kButtonUp);
fShRBFrame->AddFrame(fShSingleRB, new TGLayoutHints(kLHintsNormal,25,2,5,1));

fShMultiRB = new TGRadioButton(fShRBFrame, "multi", M__SH_MULTI);
fShMultiRB->Associate(this);
fShMultiRB->SetState(kButtonUp);
fShRBFrame->AddFrame(fShMultiRB, new TGLayoutHints(kLHintsNormal,2,2,5,1));

fShCatFrame->AddFrame(new TGLabel(fShCatFrame,new TGHotString("category")),
		      new TGLayoutHints(kLHintsNormal,20,1,6,1));

fShCatBox = new TGComboBox(fShCatFrame, M__SH_CATEGBOX);
fShCatBox->Associate(this);
char shcatname[10];
sprintf(shcatname,"%s","raw");
fShCatBox->AddEntry(shcatname,1);
sprintf(shcatname,"%s","cal");
fShCatBox->AddEntry(shcatname,2);
sprintf(shcatname,"%s","hit");
fShCatBox->AddEntry(shcatname,3);
fShCatBox->Resize(100,20);
fShCatBox->Select(3);
fShCatFrame->AddFrame(fShCatBox, new TGLayoutHints(kLHintsNormal,3,1,5,1));

fShCatFrame->AddFrame(new TGLabel(fShCatFrame, new TGHotString("name")),
		      new TGLayoutHints(kLHintsNormal, 20,1,6,1));

fShName = new TGTextEntry(fShCatFrame, fShNameBuf = new TGTextBuffer(30), -1);
fShCatFrame->AddFrame(fShName, new TGLayoutHints(kLHintsNormal,5,1,5,1));
fShName->Resize(120, fShName->GetDefaultHeight());

//
//---- end of panel fSh1 ----------------------------------------
//
//
//---- content of panel fSh2 ------------------------------------
//
//
//---- SHOWER "single" panel ------------------------------------
//

fShSingleFrame = new TGGroupFrame(fSh2,"single",kVerticalFrame);
fSh2->AddFrame(fShSingleFrame, new TGLayoutHints(kLHintsNormal));

fSh21 = new TGCompositeFrame(fShSingleFrame,700,200,kHorizontalFrame);
fShSingleFrame->AddFrame(fSh21, new TGLayoutHints(kLHintsNormal));

fSh22 = new TGCompositeFrame(fShSingleFrame,700,200,kHorizontalFrame);
fShSingleFrame->AddFrame(fSh22, new TGLayoutHints(kLHintsNormal));

fSh21->AddFrame(new TGLabel(fSh21,new TGHotString("sector")),
		new TGLayoutHints(kLHintsNormal,70,2,2,2));
fSh21->AddFrame(new TGLabel(fSh21,new TGHotString("module")),
		new TGLayoutHints(kLHintsNormal,70,2,2,2));
fSh21->AddFrame(new TGLabel(fSh21,new TGHotString("row")),
		new TGLayoutHints(kLHintsNormal,70,2,2,2));
fSh21->AddFrame(new TGLabel(fSh21,new TGHotString("col")),
		new TGLayoutHints(kLHintsNormal,90,2,2,2));

fShSecBox = new TGComboBox(fSh22, M__SH_SECBOX);
fShSecBox->Associate(this);
fShSecBox->Resize(100,20);
fSh22->AddFrame(fShSecBox, new TGLayoutHints(kLHintsNormal, 38,2,0,0));

fShModBox = new TGComboBox(fSh22, M__SH_MODBOX);
fShModBox->Associate(this);
fShModBox->Resize(100,20);
fSh22->AddFrame(fShModBox, new TGLayoutHints(kLHintsNormal,5,2,0,0));

fShRowBox = new TGComboBox(fSh22, M__SH_ROWBOX);
fShRowBox->Associate(this);
fShRowBox->Resize(100,20);
fSh22->AddFrame(fShRowBox, new TGLayoutHints(kLHintsNormal,5,2,0,0));

fShColBox = new TGComboBox(fSh22, M__SH_COLBOX);
fShColBox->Associate(this);
fShColBox->Resize(100,20);
fSh22->AddFrame(fShColBox, new TGLayoutHints(kLHintsNormal,5,38,0,0));

//
//---- end of SHOWER "single" panel -----------------------------
//
//
//---- SHOWER radio buttons layout frame ------------------------
//
fShRBLayFrame = new TGCompositeFrame(fSh2,800,100,kHorizontalFrame);
fSh2->AddFrame(fShRBLayFrame, new TGLayoutHints(kLHintsNormal));

fShSinDefRB = new TGRadioButton(fShRBLayFrame, "single", M__SH_SINLAY);
fShSinDefRB->Associate(this);
fShSinDefRB->SetState(kButtonDown);
fShRBLayFrame->AddFrame(fShSinDefRB, 
			new TGLayoutHints(kLHintsNormal,5,2,2,2));

fShRecDefRB = new TGRadioButton(fShRBLayFrame, "rectangle", M__SH_RECLAY);
fShRecDefRB->Associate(this);
fShRecDefRB->SetState(kButtonUp);
fShRBLayFrame->AddFrame(fShRecDefRB, 
			new TGLayoutHints(kLHintsNormal,5,2,2,2));

fShAddDefRB = new TGRadioButton(fShRBLayFrame, "add", M__SH_ADDLAY);
fShAddDefRB->Associate(this);
fShAddDefRB->SetState(kButtonDown);
fShRBLayFrame->AddFrame(fShAddDefRB, 
			new TGLayoutHints(kLHintsNormal,40,2,2,2));

fShSubDefRB = new TGRadioButton(fShRBLayFrame, "subtract", M__SH_SUBLAY);
fShSubDefRB->Associate(this);
fShSubDefRB->SetState(kButtonUp);
fShRBLayFrame->AddFrame(fShSubDefRB, 
			new TGLayoutHints(kLHintsNormal,5,2,2,2));

//
//---- end of SHOWER radio buttons layout frame -----------------
//
//
//---- SHOWER "multi" panel -------------------------------------
//

fShMultiFrame = new TGGroupFrame(fSh2,"multi",kHorizontalFrame);
fSh2->AddFrame(fShMultiFrame, new TGLayoutHints(kLHintsNormal));

//
//----- SHOWER ------ "step1" panel --------------------------------
//

fShStep1Frame = new TGGroupFrame(fShMultiFrame,"step 1",kHorizontalFrame);
fShMultiFrame->AddFrame(fShStep1Frame, new TGLayoutHints(kLHintsNormal));

fSh23 = new TGCompositeFrame(fShStep1Frame, 100, 200, kVerticalFrame);
fShStep1Frame->AddFrame(fSh23, new TGLayoutHints(kLHintsNormal));

fSh23->AddFrame(new TGLabel(fSh23, new TGHotString("sec1")),
		new TGLayoutHints(kLHintsNormal, 0,0,27,2));
fSh23->AddFrame(new TGLabel(fSh23, new TGHotString("sec2")),
		new TGLayoutHints(kLHintsNormal, 0,0,11,2));
fSh23->AddFrame(new TGLabel(fSh23, new TGHotString("sec3")),
		new TGLayoutHints(kLHintsNormal, 0,0,11,2));
fSh23->AddFrame(new TGLabel(fSh23, new TGHotString("sec4")),
		new TGLayoutHints(kLHintsNormal, 0,0,11,2));
fSh23->AddFrame(new TGLabel(fSh23, new TGHotString("sec5")),
		new TGLayoutHints(kLHintsNormal, 0,0,11,2));
fSh23->AddFrame(new TGLabel(fSh23, new TGHotString("sec6")),
		new TGLayoutHints(kLHintsNormal, 0,0,11,0));

fSh24 = new TGCompositeFrame(fShStep1Frame, 300, 200, kVerticalFrame);
fShStep1Frame->AddFrame(fSh24, new TGLayoutHints(kLHintsNormal));

fSh25 = new TGCompositeFrame(fSh24, 300, 50, kHorizontalFrame);
fSh24->AddFrame(fSh25, new TGLayoutHints(kLHintsNormal));

fSh25->AddFrame(new TGLabel(fSh25, new TGHotString("mod1")),
		new TGLayoutHints(kLHintsNormal, 3,2,2,2));
fSh25->AddFrame(new TGLabel(fSh25, new TGHotString("mod2")),
		new TGLayoutHints(kLHintsNormal, 3,2,2,2));
fSh25->AddFrame(new TGLabel(fSh25, new TGHotString("mod3")),
		new TGLayoutHints(kLHintsNormal, 3,2,2,2));

fSh26 = new TGCompositeFrame(fSh24, 400, 200, kVerticalFrame);
fSh24->AddFrame(fSh26, new TGLayoutHints(kLHintsNormal));
fSh26->SetLayoutManager(new TGMatrixLayout(fSh26, 0, 3, 7));

for(Int_t i=0; i<18; i++) {
  fShSecModBut[i] = new TGTextButton(fSh26, "     ", 800+i);
  fShSecModBut[i]->Associate(this);
  fShSecModBut[i]->Resize(20,15);
  fShSecModBut[i]->AllowStayDown(kTRUE);
  fShSecModBut[i]->SetState(kButtonEngaged);
  fSh26->AddFrame(fShSecModBut[i]);
  kShColor1[i] = kFALSE;
  countShS1 = 2;
}

//
//----- Shower ------ "step2" panel -----------------------------
//

fShStep2Frame = new TGGroupFrame(fShMultiFrame,"step 2",kHorizontalFrame);
fShMultiFrame->AddFrame(fShStep2Frame, new TGLayoutHints(kLHintsNormal));

fSh27 = new TGCompositeFrame(fShStep2Frame,300,200,kVerticalFrame);
fShStep2Frame->AddFrame(fSh27, new TGLayoutHints(kLHintsNormal,2,2,2,2));

fSh27->AddFrame(new TGLabel(fSh27, new TGHotString("modules")),
		new TGLayoutHints(kLHintsNormal, 1,0,2,2));

fShUp = new TGPictureButton(fSh27, fClient->GetPicture("arrow_up.xpm"), 
			    M__SH_UP);
fShUp->Associate(this);
fSh27->AddFrame(fShUp, new TGLayoutHints(kLHintsNormal,8,1,3,3));

fShModList = new TGListBox(fSh27, M__SH_MODLIST);
fShModList->Associate(this);
fSh27->AddFrame(fShModList, new TGLayoutHints(kLHintsNormal,3,0,3,1));
fShModList->Resize(40,125);

fShDown = new TGPictureButton(fSh27, fClient->GetPicture("arrow_down.xpm"), 
			      M__SH_DOWN);
fShDown->Associate(this);
fSh27->AddFrame(fShDown, new TGLayoutHints(kLHintsNormal,8,1,3,3));

fSh28 = new TGCompositeFrame(fShStep2Frame,300,200,kHorizontalFrame);
fShStep2Frame->AddFrame(fSh28, new TGLayoutHints(kLHintsNormal,2,2,2,2));

fSh29 = new TGCompositeFrame(fSh28, 100, 200, kVerticalFrame);
fSh28->AddFrame(fSh29, new TGLayoutHints(kLHintsNormal));

fSh29->AddFrame(new TGLabel(fSh29, new TGHotString("4")),
		new TGLayoutHints(kLHintsNormal, 0,1,25,2));
fSh29->AddFrame(new TGLabel(fSh29, new TGHotString("8")),
		new TGLayoutHints(kLHintsNormal, 0,1,4,2));
fSh29->AddFrame(new TGLabel(fSh29, new TGHotString("12")),
		new TGLayoutHints(kLHintsNormal, 0,1,4,2));
fSh29->AddFrame(new TGLabel(fSh29, new TGHotString("16")),
		new TGLayoutHints(kLHintsNormal, 0,1,4,2));
fSh29->AddFrame(new TGLabel(fSh29, new TGHotString("20")),
		new TGLayoutHints(kLHintsNormal, 0,1,4,2));
fSh29->AddFrame(new TGLabel(fSh29, new TGHotString("24")),
		new TGLayoutHints(kLHintsNormal, 0,1,4,2));
fSh29->AddFrame(new TGLabel(fSh29, new TGHotString("28")),
		new TGLayoutHints(kLHintsNormal, 0,1,4,2));
fSh29->AddFrame(new TGLabel(fSh29, new TGHotString("32")),
		new TGLayoutHints(kLHintsNormal, 0,1,4,2));

fSh30 = new TGCompositeFrame(fSh28, 300, 200, kVerticalFrame);
fSh28->AddFrame(fSh30, new TGLayoutHints(kLHintsNormal));

fSh31 = new TGCompositeFrame(fSh30, 300, 50, kHorizontalFrame);
fSh30->AddFrame(fSh31, new TGLayoutHints(kLHintsNormal));

fSh31->AddFrame(new TGLabel(fSh31, new TGHotString("4")),
		new TGLayoutHints(kLHintsNormal, 15,0,2,2));
fSh31->AddFrame(new TGLabel(fSh31, new TGHotString("8")),
		new TGLayoutHints(kLHintsNormal, 20,0,2,2));
fSh31->AddFrame(new TGLabel(fSh31, new TGHotString("12")),
		new TGLayoutHints(kLHintsNormal, 15,0,2,2));
fSh31->AddFrame(new TGLabel(fSh31, new TGHotString("16")),
		new TGLayoutHints(kLHintsNormal, 15,0,2,2));
fSh31->AddFrame(new TGLabel(fSh31, new TGHotString("20")),
		new TGLayoutHints(kLHintsNormal, 15,0,2,2));
fSh31->AddFrame(new TGLabel(fSh31, new TGHotString("24")),
		new TGLayoutHints(kLHintsNormal, 15,0,2,2));
fSh31->AddFrame(new TGLabel(fSh31, new TGHotString("28")),
		new TGLayoutHints(kLHintsNormal, 15,0,2,2));
fSh31->AddFrame(new TGLabel(fSh31, new TGHotString("32")),
		new TGLayoutHints(kLHintsNormal, 15,0,2,2));

fSh32 = new TGCompositeFrame(fSh30, 300, 200, kVerticalFrame);
fSh30->AddFrame(fSh32, new TGLayoutHints(kLHintsNormal));
fSh32->SetLayoutManager(new TGMatrixLayout(fSh32, 0, 8, 0));

for(Int_t i=0; i<64; i++) {
  fShCellsBut[i] = new TGTextButton(fSh32, "     ", 830+i);
  fShCellsBut[i]->Associate(this);
  fShCellsBut[i]->Resize(20,20);
  fShCellsBut[i]->AllowStayDown(kTRUE);
  fShCellsBut[i]->SetState(kButtonEngaged);
  fSh32->AddFrame(fShCellsBut[i]);
  kShColor2[i] = kFALSE;
  countShS2 = 2;
}

//
//---- end of SHOWER "multi" panel ------------------------------
//
//
//---- end of panel fSh2 ----------------------------------------
//
//---------------------------------------------------------------
//********* end of Shower panel
//---------------------------------------------------------------

tf2 = fDSTab->AddTab("START");
tf2 = fDSTab->AddTab("TOFINO");

//---------------------------------------------------------------
//********* end detector sets definition
//---------------------------------------------------------------

//---------------------------------------------------------------
//********* histogrammes definition
//---------------------------------------------------------------

tf = fMainTab->AddTab("Histogrammes");
fHiFrame = new TGCompositeFrame(tf, 800, 700, kVerticalFrame);
tf->AddFrame(fHiFrame, new TGLayoutHints(kLHintsNormal));

fHi1 = new TGCompositeFrame(fHiFrame, 600, 700, kHorizontalFrame);
fHiFrame->AddFrame(fHi1, new TGLayoutHints(kLHintsNormal));

fHi2 = new TGCompositeFrame(fHiFrame, 200, 700, kHorizontalFrame);
fHiFrame->AddFrame(fHi2, new TGLayoutHints(kLHintsNormal));

fHi11 = new TGCompositeFrame(fHi1, 600, 200, kVerticalFrame);
fHi1->AddFrame(fHi11, new TGLayoutHints(kLHintsNormal));

fHi12 = new TGCompositeFrame(fHi1, 600, 200, kVerticalFrame);
fHi1->AddFrame(fHi12, new TGLayoutHints(kLHintsNormal));

fHi13 = new TGCompositeFrame(fHi1, 600, 200, kVerticalFrame);
fHi1->AddFrame(fHi13, new TGLayoutHints(kLHintsNormal));

fHiStep1 = new TGGroupFrame(fHi11, "Step 1", kVerticalFrame);
fHi11->AddFrame(fHiStep1, new TGLayoutHints(kLHintsNormal));

fHiStep2 = new TGGroupFrame(fHi11, "Step 2", kVerticalFrame);
fHi11->AddFrame(fHiStep2, new TGLayoutHints(kLHintsNormal));

fHiStep3 = new TGGroupFrame(fHi11, "Step 3", kVerticalFrame);
fHi11->AddFrame(fHiStep3, new TGLayoutHints(kLHintsNormal));

fHiStep4 = new TGGroupFrame(fHi12, "Step 4", kVerticalFrame);
fHi12->AddFrame(fHiStep4, new TGLayoutHints(kLHintsNormal));

fHiStep5 = new TGGroupFrame(fHi12, "Step 5", kVerticalFrame);
fHi12->AddFrame(fHiStep5, new TGLayoutHints(kLHintsNormal));

fHiStep6 = new TGGroupFrame(fHi13, "Step 6", kVerticalFrame);
fHi13->AddFrame(fHiStep6, new TGLayoutHints(kLHintsNormal));

fHiStep7 = new TGGroupFrame(fHi13, "Step 7", kVerticalFrame);
fHi13->AddFrame(fHiStep7, new TGLayoutHints(kLHintsNormal));

fHiStep8 = new TGGroupFrame(fHi13, "Step 8", kHorizontalFrame);
fHi13->AddFrame(fHiStep8, new TGLayoutHints(kLHintsNormal));

fHiCanv = new TRootEmbeddedCanvas("histo", fHi2, 220, 175);
fHi2->AddFrame(fHiCanv, new TGLayoutHints(kLHintsNormal, 150,2,2,2));
fHiCanv->GetCanvas()->SetFillColor(10);

/*fHiCanvProj = new TRootEmbeddedCanvas("proj", fHi2, 220, 175);
fHi2->AddFrame(fHiCanvProj, new TGLayoutHints(kLHintsNormal, 10,2,2,2));
fHiCanvProj->GetCanvas()->SetFillColor(10);

fHiCanv->GetCanvas()->cd();
fHiCanv->GetCanvas()->
AddExec("exhi","HProjection::project(((TCanvas*)gPad),1);");*/

//
//----- Step 1 panel --------------------------------------------
//

fHi1dRB = new TGRadioButton(fHiStep1, "1 dim", M__HI_1DIMRB);
fHi1dRB->Associate(this);
fHi1dRB->SetState(kButtonUp);
fHiStep1->AddFrame(fHi1dRB, new TGLayoutHints(kLHintsNormal, 2,41,2,2));

fHi2dRB = new TGRadioButton(fHiStep1, "2 dim", M__HI_2DIMRB);
fHi2dRB->Associate(this);
fHi2dRB->SetState(kButtonUp);
fHiStep1->AddFrame(fHi2dRB, new TGLayoutHints(kLHintsNormal, 2,41,2,2));

//
//----- Step 2 panel --------------------------------------------
//

fHiMulRB = new TGRadioButton(fHiStep2, "multi histo", M__HI_MULRB);
fHiMulRB->Associate(this);
fHiMulRB->SetState(kButtonUp);
fHiStep2->AddFrame(fHiMulRB, new TGLayoutHints(kLHintsNormal, 2,9,2,2));

fHiCorRB = new TGRadioButton(fHiStep2, "correl histo", M__HI_CORRB);
fHiCorRB->Associate(this);
fHiCorRB->SetState(kButtonUp);
fHiStep2->AddFrame(fHiCorRB, new TGLayoutHints(kLHintsNormal, 2,9,2,2));

//
//----- Step 3 panel --------------------------------------------
//

fHiNoCRB = new TGRadioButton(fHiStep3, "no condition", M__HI_NOCRB);
fHiNoCRB->Associate(this);
fHiNoCRB->SetState(kButtonUp);
fHiStep3->AddFrame(fHiNoCRB, new TGLayoutHints(kLHintsNormal, 2,2,4,2));

fHiCRB = new TGRadioButton(fHiStep3, "condition", M__HI_CRB);
fHiCRB->Associate(this);
fHiCRB->SetState(kButtonUp);
fHiStep3->AddFrame(fHiCRB, new TGLayoutHints(kLHintsNormal, 2,2,3,4));

//
//----- Step 4 panel --------------------------------------------
//

fHiStep4RBFrame = new TGCompositeFrame(fHiStep4, 300, 100, kVerticalFrame);
fHiStep4->AddFrame(fHiStep4RBFrame, new TGLayoutHints(kLHintsNormal));

fHiStep4CBFrame = new TGCompositeFrame(fHiStep4, 300, 100, kHorizontalFrame);
fHiStep4->AddFrame(fHiStep4CBFrame, new TGLayoutHints(kLHintsNormal));

fHiLocalRB = new TGRadioButton(fHiStep4RBFrame, "local condition", 
			       M__HI_LOCALRB);
fHiLocalRB->Associate(this);
fHiLocalRB->SetState(kButtonUp);
fHiStep4RBFrame->AddFrame(fHiLocalRB, 
			  new TGLayoutHints(kLHintsNormal, 2,2,2,2));

fHiGlobalRB = new TGRadioButton(fHiStep4RBFrame, "global condition", 
				M__HI_GLOBALRB);
fHiGlobalRB->Associate(this);
fHiGlobalRB->SetState(kButtonUp);
fHiStep4RBFrame->AddFrame(fHiGlobalRB, 
			  new TGLayoutHints(kLHintsNormal, 2,2,2,2));

fHiStep4CBFrame->AddFrame(new TGLabel(fHiStep4CBFrame, 
				      new TGHotString("condition")),
			  new TGLayoutHints(kLHintsNormal, 2,2,10,2));

fHiConCB = new TGComboBox(fHiStep4CBFrame, M__HI_CCB);
fHiConCB->Associate(this);
fHiConCB->Resize(130,20);
fHiStep4CBFrame->AddFrame(fHiConCB,new TGLayoutHints(kLHintsNormal,2,30,6,2));

//
//----- Step 5 panel --------------------------------------------
//

fHiStep5LabFrame = new TGCompositeFrame(fHiStep5, 300, 100, kHorizontalFrame);
fHiStep5->AddFrame(fHiStep5LabFrame, new TGLayoutHints(kLHintsNormal));

fHiStep5CB1Frame = new TGCompositeFrame(fHiStep5, 300, 100, kHorizontalFrame);
fHiStep5->AddFrame(fHiStep5CB1Frame, new TGLayoutHints(kLHintsNormal));

fHiStep5CB2Frame = new TGCompositeFrame(fHiStep5, 300, 100, kHorizontalFrame);
fHiStep5->AddFrame(fHiStep5CB2Frame, new TGLayoutHints(kLHintsNormal));

fHiStep5LabFrame->AddFrame(new TGLabel(fHiStep5LabFrame, 
				       new TGHotString("det set")),
			   new TGLayoutHints(kLHintsNormal,50,2,5,7));
fHiStep5LabFrame->AddFrame(new TGLabel(fHiStep5LabFrame, 
				       new TGHotString("parameter")),
			   new TGLayoutHints(kLHintsNormal,50,2,5,7));

fHiStep5CB1Frame->AddFrame(new TGLabel(fHiStep5CB1Frame,
				       new TGHotString("X axis")),
			   new TGLayoutHints(kLHintsNormal,0,1,5,0));

fHiDS1CB = new TGComboBox(fHiStep5CB1Frame, M__HI_DS1CB);
fHiDS1CB->Associate(this);
fHiDS1CB->Resize(90,20);
fHiStep5CB1Frame->AddFrame(fHiDS1CB, new TGLayoutHints(kLHintsNormal,0,0,5,7));

fHiPar1CB = new TGComboBox(fHiStep5CB1Frame, M__HI_PAR1CB);
fHiPar1CB->Associate(this);
fHiPar1CB->Resize(90,20);
fHiStep5CB1Frame->AddFrame(fHiPar1CB, 
			   new TGLayoutHints(kLHintsNormal,0,1,5,7));

fHiStep5CB2Frame->AddFrame(new TGLabel(fHiStep5CB2Frame,
				       new TGHotString("Y axis")),
			   new TGLayoutHints(kLHintsNormal,0,1,5,0));

fHiDS2CB = new TGComboBox(fHiStep5CB2Frame, M__HI_DS2CB);
fHiDS2CB->Associate(this);
fHiDS2CB->Resize(90,20);
fHiStep5CB2Frame->AddFrame(fHiDS2CB, new TGLayoutHints(kLHintsNormal,0,0,5,8));

fHiPar2CB = new TGComboBox(fHiStep5CB2Frame, M__HI_PAR2CB);
fHiPar2CB->Associate(this);
fHiPar2CB->Resize(90,20);
fHiStep5CB2Frame->AddFrame(fHiPar2CB, 
			   new TGLayoutHints(kLHintsNormal,0,1,5,8));

//
//----- Step 6 panel --------------------------------------------
//

fHiStep6LabFrame = new TGCompositeFrame(fHiStep6, 300, 100, kHorizontalFrame);
fHiStep6->AddFrame(fHiStep6LabFrame, new TGLayoutHints(kLHintsNormal));

fHiStep6T1Frame = new TGCompositeFrame(fHiStep6, 300, 100, kHorizontalFrame);
fHiStep6->AddFrame(fHiStep6T1Frame, new TGLayoutHints(kLHintsNormal));

fHiStep6T2Frame = new TGCompositeFrame(fHiStep6, 300, 100, kHorizontalFrame);
fHiStep6->AddFrame(fHiStep6T2Frame, new TGLayoutHints(kLHintsNormal));

fHiStep6LabFrame->AddFrame(new TGLabel(fHiStep6LabFrame, 
				       new TGHotString("from")),
			   new TGLayoutHints(kLHintsNormal,20,2,2,2));
fHiStep6LabFrame->AddFrame(new TGLabel(fHiStep6LabFrame, 
				       new TGHotString("to")),
			   new TGLayoutHints(kLHintsNormal,15,2,2,2));
fHiStep6LabFrame->AddFrame(new TGLabel(fHiStep6LabFrame, 
				       new TGHotString("bins")),
			   new TGLayoutHints(kLHintsNormal,18,2,2,2));

fHiStep6T1Frame->AddFrame(new TGLabel(fHiStep6T1Frame,
				      new TGHotString("X")),
			  new TGLayoutHints(kLHintsNormal,0,3,2,2));

fHiFrom1 = new TGTextEntry(fHiStep6T1Frame, 
			   fHiFrom1Buf = new TGTextBuffer(20), -1);
fHiStep6T1Frame->AddFrame(fHiFrom1, new TGLayoutHints(kLHintsNormal,0,0,1,1));
fHiFrom1->Resize(38, fHiFrom1->GetDefaultHeight());
fHiFrom1->SetState(kFALSE);

fHiTo1 = new TGTextEntry(fHiStep6T1Frame,fHiTo1Buf = new TGTextBuffer(20), -1);
fHiStep6T1Frame->AddFrame(fHiTo1, new TGLayoutHints(kLHintsNormal,0,0,1,1));
fHiTo1->Resize(38, fHiTo1->GetDefaultHeight());
fHiTo1->SetState(kFALSE);

fHiChan1 = new TGTextEntry(fHiStep6T1Frame,
			   fHiChan1Buf = new TGTextBuffer(20), -1);
fHiStep6T1Frame->AddFrame(fHiChan1, new TGLayoutHints(kLHintsNormal,0,0,1,1));
fHiChan1->Resize(38, fHiChan1->GetDefaultHeight());
fHiChan1->SetState(kFALSE);

fHiStep6T2Frame->AddFrame(new TGLabel(fHiStep6T2Frame,
				      new TGHotString("Y")),
			  new TGLayoutHints(kLHintsNormal,0,3,2,2));

fHiFrom2 = new TGTextEntry(fHiStep6T2Frame, 
			   fHiFrom2Buf = new TGTextBuffer(20), -1);
fHiStep6T2Frame->AddFrame(fHiFrom2, new TGLayoutHints(kLHintsNormal,0,0,2,2));
fHiFrom2->Resize(38, fHiFrom2->GetDefaultHeight());
fHiFrom2->SetState(kFALSE);

fHiTo2 = new TGTextEntry(fHiStep6T2Frame,fHiTo2Buf = new TGTextBuffer(20), -1);
fHiStep6T2Frame->AddFrame(fHiTo2, new TGLayoutHints(kLHintsNormal,0,0,2,2));
fHiTo2->Resize(38, fHiTo2->GetDefaultHeight());
fHiTo2->SetState(kFALSE);

fHiChan2 = new TGTextEntry(fHiStep6T2Frame,
			   fHiChan2Buf = new TGTextBuffer(20), -1);
fHiStep6T2Frame->AddFrame(fHiChan2, new TGLayoutHints(kLHintsNormal,0,0,2,2));
fHiChan2->Resize(38, fHiChan2->GetDefaultHeight());
fHiChan2->SetState(kFALSE);

//
//----- Step 7 panel --------------------------------------------
//

fHiStep7LabFrame = new TGCompositeFrame(fHiStep7, 300, 100, kHorizontalFrame);
fHiStep7->AddFrame(fHiStep7LabFrame, new TGLayoutHints(kLHintsNormal));

fHiStep7LabFrame->AddFrame(new TGLabel(fHiStep7LabFrame, 
				       new TGHotString("histo name")),
			   new TGLayoutHints(kLHintsNormal,2,2,2,2));

fHiName = new TGTextEntry(fHiStep7,fHiNameBuf = new TGTextBuffer(30), -1);
fHiStep7->AddFrame(fHiName, new TGLayoutHints(kLHintsNormal,2,2,0,0));
fHiName->Resize(123, fHiName->GetDefaultHeight());

//
//----- Step 8 panel --------------------------------------------
//

fHiPause = new TGCheckButton(fHiStep8, "pause", M__HI_PAUSE);
fHiPause->Associate(this);
fHiPause->SetState(kButtonUp);
fHiStep8->AddFrame(fHiPause, new TGLayoutHints(kLHintsNormal,40,31,2,0));

//---------------------------------------------------------------
//********* end histogrammes definition
//---------------------------------------------------------------

//---------------------------------------------------------------
//********* local conditions definition
//---------------------------------------------------------------

tf = fMainTab->AddTab("Local conditions");

fLCFrame = new TGCompositeFrame(tf, 800, 700, kHorizontalFrame);
tf->AddFrame(fLCFrame, new TGLayoutHints(kLHintsNormal));

fLC1 = new TGCompositeFrame(fLCFrame, 100, 600, kVerticalFrame);
fLCFrame->AddFrame(fLC1, new TGLayoutHints(kLHintsNormal,15,3,3,1));

fLC2 = new TGCompositeFrame(fLCFrame, 500, 600, kVerticalFrame);
fLCFrame->AddFrame(fLC2, new TGLayoutHints(kLHintsNormal,10,3,3,1));

//
//-------- subframes of fLC1 -------------------------------------
//

fLCAlwTrue = new TGCheckButton(fLC1, "always true", M__LC_ALWAYSTRUE);
fLCAlwTrue->Associate(this);
fLCAlwTrue->SetState(kButtonUp);
fLC1->AddFrame(fLCAlwTrue, new TGLayoutHints(kLHintsNormal,3,1,5,1));

fLC1->AddFrame(new TGLabel(fLC1, new TGHotString("Detector set")),
	       new TGLayoutHints(kLHintsNormal, 3,1,6,1));


fLCDSCB = new TGComboBox(fLC1, M__LC_DSCB);
fLCDSCB->Associate(this);
fLCDSCB->Resize(135, 20);
fLC1->AddFrame(fLCDSCB, new TGLayoutHints(kLHintsNormal,3,1,3,1));
createDSList(fLCDSCB);

fLC1->AddFrame(new TGLabel(fLC1, new TGHotString("Parameters")),
	       new TGLayoutHints(kLHintsNormal, 3,1,7,1));

fLCParLB = new TGListBox(fLC1, M__LC_PARAMLB);
fLCParLB->Associate(this);
fLCParLB->Resize(135,250);
fLC1->AddFrame(fLCParLB, new TGLayoutHints(kLHintsNormal,3,1,3,1));

fLC1->AddFrame(new TGLabel(fLC1, new TGHotString("Condition name")),
	       new TGLayoutHints(kLHintsNormal,1,1,12,1));

fLCName = new TGTextEntry(fLC1, fLCNameBuf = new TGTextBuffer(50), -1);
fLC1->AddFrame(fLCName, new TGLayoutHints(kLHintsNormal,1,1,1,1));
fLCName->Resize(135, fLCName->GetDefaultHeight());

//
//-------- subframes of fLC2 -------------------------------------
//

//
//-------- local conditions expression ---------------------------
//

fLCExpFrame = new TGGroupFrame(fLC2, "expression", kVerticalFrame);
fLC2->AddFrame(fLCExpFrame, new TGLayoutHints(kLHintsNormal,2,2,2,2));

fLCA1 = new TGCompositeFrame(fLCExpFrame, 500,500, kHorizontalFrame);
fLCExpFrame->AddFrame(fLCA1, new TGLayoutHints(kLHintsNormal,2,2,3,2));

fLCA2 = new TGCompositeFrame(fLCExpFrame, 500,200, kVerticalFrame);
fLCExpFrame->AddFrame(fLCA2, new TGLayoutHints(kLHintsNormal,2,2,0,3));

fLCA11 = new TGCompositeFrame(fLCA1, 200, 350, kVerticalFrame);
fLCA1->AddFrame(fLCA11, new TGLayoutHints(kLHintsNormal,5,5,2,2));

fLCA12 = new TGCompositeFrame(fLCA1, 200, 350, kVerticalFrame);
fLCA1->AddFrame(fLCA12, new TGLayoutHints(kLHintsNormal,15,5,2,2));

fLCA111 = new TGCompositeFrame(fLCA11, 200, 350, kVerticalFrame);
fLCA111->SetLayoutManager(new TGMatrixLayout(fLCA111, 0,3,5));
fLCA11->AddFrame(fLCA111, new TGLayoutHints(kLHintsNormal,25,2,2,2));

fLCA112 = new TGCompositeFrame(fLCA11, 200, 350, kVerticalFrame);
fLCA112->SetLayoutManager(new TGMatrixLayout(fLCA112, 0,2,20));
fLCA11->AddFrame(fLCA112, new TGLayoutHints(kLHintsNormal,20,2,10,2));

fLCA121 = new TGCompositeFrame(fLCA12, 200, 350, kVerticalFrame);
fLCA121->SetLayoutManager(new TGMatrixLayout(fLCA121, 0,4,5));
fLCA12->AddFrame(fLCA121, new TGLayoutHints(kLHintsNormal,2,2,2,2));

fLCA122 = new TGCompositeFrame(fLCA12, 200, 350, kVerticalFrame);
fLCA122->SetLayoutManager(new TGMatrixLayout(fLCA122, 0,3,9));
fLCA12->AddFrame(fLCA122, new TGLayoutHints(kLHintsNormal,2,2,2,2));

fLCA123 = new TGCompositeFrame(fLCA12, 200, 350, kVerticalFrame);
fLCA123->SetLayoutManager(new TGMatrixLayout(fLCA123, 0,4,5));
fLCA12->AddFrame(fLCA123, new TGLayoutHints(kLHintsNormal,2,2,2,2));

//----------------------------------------------------------------
// local conditions ************  buttons 0,1 ... 9

Int_t j[10] = { 7,8,9,4,5,6,1,2,3,0 };  // to map the buttons 
char tmp[10];

for(Int_t i=0; i<10; i++) {

  sprintf(tmp,"%s%i%s"," ",j[i]," ");
  fLCToken[i] = new TGTextButton(fLCA111, tmp, j[i]);
  fLCToken[i]->Associate(this);
  fLCToken[i]->Resize(25,25);
  fLCA111->AddFrame(fLCToken[i]);
}  

fLCToken[10] = new TGTextButton(fLCA111, " . ", 10);
fLCToken[10]->Associate(this);
fLCToken[10]->Resize(25,25);
fLCA111->AddFrame(fLCToken[10]);

fLCToken[11] = new TGTextButton(fLCA111, "   ", 11);
fLCToken[11]->Associate(this);
fLCToken[11]->Resize(25,25);
fLCA111->AddFrame(fLCToken[11]);

//----------------------------------------------------------
//  local conditions ********** buttons (  )

fLCGroup4[0] = new TGTextButton(fLCA112, "(", 12);
fLCGroup4[0]->Associate(this);
fLCGroup4[0]->Resize(30,30);
fLCA112->AddFrame(fLCGroup4[0]);

fLCGroup4[1] = new TGTextButton(fLCA112, ")", 13);
fLCGroup4[1]->Associate(this);
fLCGroup4[1]->Resize(30,30);
fLCA112->AddFrame(fLCGroup4[1]);

//----------------------------------------------------------
//  local conditions ********* buttons / * + - ln exp ^ sqrt

const char *bnames[] = { "/", "*", "+", "-", "ln", "exp", "^", "sqrt" };

for(Int_t i=0; i<8; i++) {

  fLCGroup1[i] = new TGTextButton(fLCA121, bnames[i], 14+i);
  fLCGroup1[i]->Associate(this);
  fLCGroup1[i]->Resize(30,25);
  fLCA121->AddFrame(fLCGroup1[i]);
}

//----------------------------------------------------------
//  local conditions ***** buttons sin cos tan asin acos atan

const char *bnames2[] = { "sin", "cos", "tan", "asin", "acos", "atan" };

for(Int_t i=0; i<6; i++) {

  fLCGroup2[i] = new TGTextButton(fLCA122, bnames2[i], 22+i);
  fLCGroup2[i]->Associate(this);
  fLCGroup2[i]->Resize(35,25);
  fLCA122->AddFrame(fLCGroup2[i]);

}

//----------------------------------------------------------
//  local conditions *********   buttons && || == != > < >= <=

const char *bnames3[] = { "and", "or", "!=", "==", ">", "<", ">=", "<=" };

for(Int_t i=0; i<8; i++) {

  fLCGroup3[i] = new TGTextButton(fLCA123, bnames3[i], 28+i);
  fLCGroup3[i]->Associate(this);
  fLCGroup3[i]->Resize(30,25);
  fLCA123->AddFrame(fLCGroup3[i]);

}

//
//-------- subframes of fLCA2 ----------------------------------
//

fLCA21 = new TGCompositeFrame(fLCA2, 200, 350, kVerticalFrame);
fLCA2->AddFrame(fLCA21, new TGLayoutHints(kLHintsNormal,1,1,3,1));

fLCA22 = new TGCompositeFrame(fLCA2, 200, 350, kHorizontalFrame);
fLCA2->AddFrame(fLCA22, new TGLayoutHints(kLHintsNormal,1,1,3,1));

fLCA21->AddFrame(new TGLabel(fLCA21, new TGHotString("expression")),
		 new TGLayoutHints(kLHintsNormal,1,1,1,1));
fLCExpList = new TGListBox(fLCA21, -1);
fLCExpList->Associate(this);
fLCExpList->Resize(320,20);
fLCA21->AddFrame(fLCExpList, new TGLayoutHints(kLHintsNormal,2,2,0,0));

fLCExpUndo = new TGTextButton(fLCA22, "  Undo  ", M__LC_UNDOEXP);
fLCExpUndo->Associate(this);
fLCExpUndo->SetToolTipText("Undo one step");
fLCA22->AddFrame(fLCExpUndo, new TGLayoutHints(kLHintsNormal,1,1,1,1));

fLCExpClear = new TGTextButton(fLCA22, "  Clear  ", M__LC_CLEAREXP);
fLCExpClear->Associate(this);
fLCExpClear->SetToolTipText("Clear expression field");
fLCA22->AddFrame(fLCExpClear, new TGLayoutHints(kLHintsNormal,1,1,1,1));

fLCExpNPar = new TGTextButton(fLCA22, "New par", M__LC_NEWPAREXP);
fLCExpNPar->Associate(this);
fLCExpNPar->SetToolTipText("Create new parameter");
fLCA22->AddFrame(fLCExpNPar, new TGLayoutHints(kLHintsNormal,1,1,1,1));

fLCA22->AddFrame(new TGLabel(fLCA22, new TGHotString("name")),
		 new TGLayoutHints(kLHintsNormal,1,1,4,1));

fLCNParName = new TGTextEntry(fLCA22, 
			      fLCNParNameBuf = new TGTextBuffer(50), -1);
fLCA22->AddFrame(fLCNParName, new TGLayoutHints(kLHintsNormal,1,1,1,1));
fLCNParName->Resize(122, fLCNParName->GetDefaultHeight());

//
//-------- local condition contour --------------------------------
//

fLCContourFrame = new TGGroupFrame(fLC2, "contour", kHorizontalFrame);
fLC2->AddFrame(fLCContourFrame, new TGLayoutHints(kLHintsNormal,2,2,2,2));

fLCC1 = new TGCompositeFrame(fLCContourFrame, 800, 200, kVerticalFrame); 
fLCContourFrame->AddFrame(fLCC1, new TGLayoutHints(kLHintsNormal));

fLCC2 = new TGCompositeFrame(fLCContourFrame, 800, 200, kVerticalFrame); 
fLCContourFrame->AddFrame(fLCC2, new TGLayoutHints(kLHintsNormal,20,10,0,0));

fLCC11 = new TGCompositeFrame(fLCC1, 500, 100, kHorizontalFrame);
fLCC1->AddFrame(fLCC11, new TGLayoutHints(kLHintsNormal));

fLCC12 = new TGCompositeFrame(fLCC1, 500, 100, kHorizontalFrame);
fLCC1->AddFrame(fLCC12, new TGLayoutHints(kLHintsNormal));

fLCC11->AddFrame(new TGLabel(fLCC11, new TGHotString("X axis param")),
		 new TGLayoutHints(kLHintsNormal,0,2,2,2));
fLCXparBox = new TGComboBox(fLCC11, M__LC_XPARCB);
fLCXparBox->Associate(this);
fLCC11->AddFrame(fLCXparBox, new TGLayoutHints(kLHintsNormal,0,0,2,2));
fLCXparBox->Resize(110,20);

fLCC12->AddFrame(new TGLabel(fLCC12, new TGHotString("Y axis param")),
		 new TGLayoutHints(kLHintsNormal,0,2,2,2));
fLCYparBox = new TGComboBox(fLCC12, M__LC_YPARCB);
fLCYparBox->Associate(this);
fLCC12->AddFrame(fLCYparBox, new TGLayoutHints(kLHintsNormal,0,0,2,2));
fLCYparBox->Resize(110,20);

fLCC2->AddFrame(new TGLabel(fLCC2, new TGHotString("contour")),
		new TGLayoutHints(kLHintsNormal, 2,2,2,1));
fLCContourCB = new TGComboBox(fLCC2, M__LC_CONTOURCB);
fLCContourCB->Associate(this);
fLCC2->AddFrame(fLCContourCB, new TGLayoutHints(kLHintsNormal,2,2,2,2));
fLCContourCB->Resize(110,20);

//---------------------------------------------------------------
//********* end local conditions definition
//---------------------------------------------------------------

//---------------------------------------------------------------
//********* global conditions definition
//---------------------------------------------------------------

tf = fMainTab->AddTab("Global conditions");

fGCFrame = new TGCompositeFrame(tf, 800, 700, kVerticalFrame);
tf->AddFrame(fGCFrame, new TGLayoutHints(kLHintsNormal));

fGC1 = new TGCompositeFrame(fGCFrame, 800, 500, kVerticalFrame);
fGCFrame->AddFrame(fGC1, new TGLayoutHints(kLHintsNormal,3,1,1,1));

fGC2 = new TGCompositeFrame(fGCFrame, 800, 100, kHorizontalFrame);
fGCFrame->AddFrame(fGC2, new TGLayoutHints(kLHintsNormal,3,1,1,1));

fGC21 = new TGCompositeFrame(fGC2, 500, 100, kVerticalFrame);
fGC2->AddFrame(fGC21, new TGLayoutHints(kLHintsNormal,3,1,2,1));

fGC22 = new TGCompositeFrame(fGC2, 500, 100, kVerticalFrame);
fGC2->AddFrame(fGC22, new TGLayoutHints(kLHintsNormal,3,1,2,1));

//
//-------- global conditions expression -------------------------
//

fGCExpFrame = new TGGroupFrame(fGC1, "expression", kHorizontalFrame);
fGC1->AddFrame(fGCExpFrame, new TGLayoutHints(kLHintsNormal));

fGCA1 = new TGCompositeFrame(fGCExpFrame, 200,400, kVerticalFrame);
fGCExpFrame->AddFrame(fGCA1, new TGLayoutHints(kLHintsNormal));

fGCA2 = new TGCompositeFrame(fGCExpFrame, 500,500, kVerticalFrame);
fGCExpFrame->AddFrame(fGCA2, new TGLayoutHints(kLHintsNormal,20,10,0,0));

fGCA21 = new TGCompositeFrame(fGCA2, 200, 100, kHorizontalFrame);
fGCA2->AddFrame(fGCA21, new TGLayoutHints(kLHintsNormal,0,0,10,3));

fGCA22 = new TGCompositeFrame(fGCA2, 200, 100, kHorizontalFrame);
fGCA22->SetLayoutManager(new TGMatrixLayout(fGCA22, 0,2,20));
fGCA2->AddFrame(fGCA22, new TGLayoutHints(kLHintsNormal));

fGCA23 = new TGCompositeFrame(fGCA2, 200, 100, kVerticalFrame);
fGCA2->AddFrame(fGCA23, new TGLayoutHints(kLHintsNormal));

fGCA24 = new TGCompositeFrame(fGCA2, 200, 100, kHorizontalFrame);
fGCA2->AddFrame(fGCA24, new TGLayoutHints(kLHintsNormal));

fGCA211 = new TGCompositeFrame(fGCA21, 200, 100, kVerticalFrame);
fGCA211->SetLayoutManager(new TGMatrixLayout(fGCA211, 0,3,5));
fGCA21->AddFrame(fGCA211, new TGLayoutHints(kLHintsNormal,10,3,0,0));

fGCA212 = new TGCompositeFrame(fGCA21, 200, 100, kVerticalFrame);
fGCA212->SetLayoutManager(new TGMatrixLayout(fGCA212, 0,4,10));
fGCA21->AddFrame(fGCA212, new TGLayoutHints(kLHintsNormal,10,3,0,0));

fGCA1->AddFrame(new TGLabel(fGCA1, 
			    new TGHotString("local conditions counters")),
		new TGLayoutHints(kLHintsNormal,3,1,3,2));

fGCListPar = new TGListBox(fGCA1, M__GC_PARAMLIST);
fGCListPar->Associate(this);
fGCListPar->Resize(145,240);
fGCA1->AddFrame(fGCListPar, new TGLayoutHints(kLHintsNormal,3,1,2,0));

//----------------------------------------------------------------
// global conditions ************  buttons 0,1 ... 9

Int_t jj[10] = { 7,8,9,4,5,6,1,2,3,0 };  // to map the buttons 
char tmp2[10];

for(Int_t i=0; i<10; i++) {

  sprintf(tmp2,"%s%i%s"," ",jj[i]," ");
  fGCToken[i] = new TGTextButton(fGCA211, tmp2, jj[i]+1000);
  fGCToken[i]->Associate(this);
  fGCToken[i]->Resize(25,25);
  fGCA211->AddFrame(fGCToken[i]);
}  

fGCToken[10] = new TGTextButton(fGCA211, " . ", 1010);
fGCToken[10]->Associate(this);
fGCToken[10]->Resize(25,25);
fGCA211->AddFrame(fGCToken[10]);

fGCToken[11] = new TGTextButton(fGCA211, "   ", 1011);
fGCToken[11]->Associate(this);
fGCToken[11]->Resize(25,25);
fGCA211->AddFrame(fGCToken[11]);

//----------------------------------------------------------
//  global conditions ********* buttons / * + - and or != ==
//                                      > < >= <=

const char *bnames6[] = { "/", "*", "+", "-", "and", "or", "!=", "==",
                          ">", "<", ">=", "<=" };

for(Int_t i=0; i<12; i++) {

  fGCGroup1[i] = new TGTextButton(fGCA212, bnames6[i], 1014+i);
  fGCGroup1[i]->Associate(this);
  fGCGroup1[i]->Resize(35,30);
  fGCA212->AddFrame(fGCGroup1[i]);

}

//----------------------------------------------------------
//  global conditions ********** buttons (  )

fGCGroup2[0] = new TGTextButton(fGCA22, "(", 1012);
fGCGroup2[0]->Associate(this);
fGCGroup2[0]->Resize(30,30);
fGCA22->AddFrame(fGCGroup2[0]);

fGCGroup2[1] = new TGTextButton(fGCA22, ")", 1013);
fGCGroup2[1]->Associate(this);
fGCGroup2[1]->Resize(30,30);
fGCA22->AddFrame(fGCGroup2[1]);

//
//-------- subframes of fGCA23 ----------------------------------
//

fGCA23->AddFrame(new TGLabel(fGCA23, new TGHotString("expression")),
		 new TGLayoutHints(kLHintsNormal,5,1,2,2));
fGCExpList = new TGListBox(fGCA23, -1);
fGCExpList->Associate(this);
fGCExpList->Resize(310,20);
fGCA23->AddFrame(fGCExpList, new TGLayoutHints(kLHintsNormal,4,2,3,5));

//
//-------- subframes of fGCA24 ----------------------------------
//

fGCExpUndo = new TGTextButton(fGCA24, "  Undo  ", M__GC_UNDOEXP);
fGCExpUndo->Associate(this);
fGCExpUndo->SetToolTipText("Undo one step");
fGCA24->AddFrame(fGCExpUndo, new TGLayoutHints(kLHintsNormal,5,1,2,1));

fGCExpClear = new TGTextButton(fGCA24, "  Clear  ", M__GC_CLEAREXP);
fGCExpClear->Associate(this);
fGCExpClear->SetToolTipText("Clear expression field");
fGCA24->AddFrame(fGCExpClear, new TGLayoutHints(kLHintsNormal,5,1,2,1));

//
//-------- contour -----------------------------------------------
//

fGCContourFrame = new TGGroupFrame(fGC21, "contour", kVerticalFrame);
fGC21->AddFrame(fGCContourFrame, new TGLayoutHints(kLHintsNormal));

fGCC1 = new TGCompositeFrame(fGCContourFrame, 500, 500, kHorizontalFrame);
fGCContourFrame->AddFrame(fGCC1, new TGLayoutHints(kLHintsNormal));

fGCC2 = new TGCompositeFrame(fGCContourFrame, 500,300, kHorizontalFrame);
fGCContourFrame->AddFrame(fGCC2, new TGLayoutHints(kLHintsNormal));

fGCC3 = new TGCompositeFrame(fGCContourFrame, 500,300, kHorizontalFrame);
fGCContourFrame->AddFrame(fGCC3, new TGLayoutHints(kLHintsNormal));

fGCC1->AddFrame(new TGLabel(fGCC1, new TGHotString("det set 1")),
		new TGLayoutHints(kLHintsNormal,2,2,2,2));

fGCDS1CB = new TGComboBox(fGCC1, M__GC_DS1CB);
fGCDS1CB->Associate(this);
fGCC1->AddFrame(fGCDS1CB, new TGLayoutHints(kLHintsNormal,0,0,2,2));
fGCDS1CB->Resize(105,20);

fGCC1->AddFrame(new TGLabel(fGCC1, new TGHotString("X axis param")),
		new TGLayoutHints(kLHintsNormal,5,2,2,2));
fGCXparBox = new TGComboBox(fGCC1, M__GC_XPARCB);
fGCXparBox->Associate(this);
fGCC1->AddFrame(fGCXparBox, new TGLayoutHints(kLHintsNormal,0,0,2,2));
fGCXparBox->Resize(105,20);


fGCC2->AddFrame(new TGLabel(fGCC2, new TGHotString("det set 2")),
		new TGLayoutHints(kLHintsNormal,2,2,2,2));

fGCDS2CB = new TGComboBox(fGCC2, M__GC_DS2CB);
fGCDS2CB->Associate(this);
fGCC2->AddFrame(fGCDS2CB, new TGLayoutHints(kLHintsNormal,0,0,2,2));
fGCDS2CB->Resize(105,20);

fGCC2->AddFrame(new TGLabel(fGCC2, new TGHotString("Y axis param")),
		new TGLayoutHints(kLHintsNormal,5,2,2,2));
fGCYparBox = new TGComboBox(fGCC2, M__GC_YPARCB);
fGCYparBox->Associate(this);
fGCC2->AddFrame(fGCYparBox, new TGLayoutHints(kLHintsNormal,0,0,2,2));
fGCYparBox->Resize(105,20);


fGCC3->AddFrame(new TGLabel(fGCC3, new TGHotString("contour")),
		new TGLayoutHints(kLHintsNormal, 80,2,7,1));
fGCContourCB = new TGComboBox(fGCC3, M__GC_CONTOURCB);
fGCContourCB->Associate(this);
fGCC3->AddFrame(fGCContourCB, new TGLayoutHints(kLHintsNormal,2,2,6,0));
fGCContourCB->Resize(110,20);

//
//-------- subframes of fGC3 -------------------------------------
//

fGC22->AddFrame(new TGLabel(fGC22, new TGHotString("Condition name")),
		new TGLayoutHints(kLHintsNormal,10,1,25,1));

fGCName = new TGTextEntry(fGC22, fGCNameBuf = new TGTextBuffer(50), -1);
fGC22->AddFrame(fGCName, new TGLayoutHints(kLHintsNormal,10,1,3,1));
fGCName->Resize(110, fGCName->GetDefaultHeight());

//---------------------------------------------------------------
//********* end global conditions definition
//---------------------------------------------------------------

//---------------------------------------------------------------
//********* add, modify remove buttons, list of objects, next prev buttons
//---------------------------------------------------------------

fB1 = new TGCompositeFrame(fF01, 200, 700, kHorizontalFrame);
fF01->AddFrame(fB1, new TGLayoutHints(kLHintsNormal));

fB2 = new TGCompositeFrame(fF01, 200, 700, kVerticalFrame);
fF01->AddFrame(fB2, new TGLayoutHints(kLHintsNormal));

fB3 = new TGCompositeFrame(fF01, 200, 700, kHorizontalFrame);
fF01->AddFrame(fB3, new TGLayoutHints(kLHintsNormal));

fAdd = new TGTextButton(fB1, "   Add   ", M_ADD);
fAdd->Associate(this);
fAdd->SetToolTipText("Add object to the list");
fB1->AddFrame(fAdd, new TGLayoutHints(kLHintsNormal,1,1,1,1));

fModify = new TGTextButton(fB1, " Modify ", M_MODIFY);
fModify->Associate(this);
fModify->SetToolTipText("Modify object in the list");
fB1->AddFrame(fModify, new TGLayoutHints(kLHintsNormal,1,1,1,1));

fRemove = new TGTextButton(fB1, "Remove", M_REMOVE);
fRemove->Associate(this);
fRemove->SetToolTipText("Remove object from the list");
fB1->AddFrame(fRemove, new TGLayoutHints(kLHintsNormal,1,1,1,1));

fObjList = new TGListBox(fB2, M_LIST);
fObjList->Associate(this);
fObjList->Resize(170,380);
fB2->AddFrame(fObjList, new TGLayoutHints(kLHintsNormal));

fPrev = new TGTextButton(fB3, "Previous", M_PREV);
fPrev->Associate(this);
fPrev->SetToolTipText("Previous object in the list");
fB3->AddFrame(fPrev, new TGLayoutHints(kLHintsNormal,20,1,1,1));

fNext = new TGTextButton(fB3, "   Next   ", M_NEXT);
fNext->Associate(this);
fNext->SetToolTipText("Next object in the list");
fB3->AddFrame(fNext, new TGLayoutHints(kLHintsNormal,1,1,1,1));

//---------------------------------------------------------------

fDB = new HBrowserDB();

countBrowser++;

MapSubwindows();

SetWindowName("Online Objects Manager");

Resize(GetDefaultSize());
MapWindow();

SendMessage(this,MK_MSG(kC_COMMAND,kCM_TAB),0,0);
createContourList(fLCContourCB);
createContourList(fGCContourCB);
highlight(fLCContourCB,"no contour");
highlight(fGCContourCB,"no contour");

}


//--------------------------------------------------------------------
//******  HOnlObjBrowser::~HOnlObjBrowser()
//
// Destructor of the class HOnlObjBrowser.
//
//--------------------------------------------------------------------

HOnlObjBrowser::~HOnlObjBrowser() {
//
// Destructor of the class HOnlObjBrowser.
//

expStringL.clear();
expStringG.clear();

delete fDB;

delete fNext; delete fPrev;
delete fObjList;
delete fRemove; delete fModify; delete fAdd;
delete fB3; delete fB2; delete fB1;

//-------  delete Global Conditions stuff ----------------------------

delete fGCName;
delete fGCContourCB;
delete fGCYparBox; delete fGCDS2CB;
delete fGCXparBox; delete fGCDS1CB;
delete fGCC3; delete fGCC2; delete fGCC1;
delete fGCContourFrame; 
delete fGCExpClear; delete fGCExpUndo;
delete fGCExpList;
for(Int_t i=2;i--;delete fGCGroup2[i]);
for(Int_t i=12;i--;delete fGCGroup1[i]);
for(Int_t i=12;i--;delete fGCToken[i]);
delete fGCListPar;
delete fGCA212; delete fGCA211;
delete fGCA24; delete fGCA23; delete fGCA22; delete fGCA21;
delete fGCA2; delete fGCA1;
delete fGCExpFrame;
delete fGC22; delete fGC21; delete fGC2; delete fGC1;
delete fGCFrame;

//-------  end of delete Global Conditions stuff ---------------------

//-------  delete Local Conditions stuff -----------------------------

delete fLCContourCB;
delete fLCYparBox; delete fLCXparBox;
delete fLCC12; delete fLCC11;
delete fLCC2; delete fLCC1;
delete fLCContourFrame;
delete fLCNParName;
delete fLCExpNPar; delete fLCExpClear; delete fLCExpUndo;
delete fLCExpList;
delete fLCA22; delete fLCA21;
for(Int_t i=8;i--;delete fLCGroup3[i]);
for(Int_t i=6;i--;delete fLCGroup2[i]);
for(Int_t i=8;i--;delete fLCGroup1[i]);
for(Int_t i=2;i--;delete fLCGroup4[i]);
for(Int_t i=12;i--;delete fLCToken[i]);
delete fLCA123; delete fLCA122; delete fLCA121;
delete fLCA112; delete fLCA111;
delete fLCA12; delete fLCA11;
delete fLCA2; delete fLCA1;
delete fLCExpFrame;
delete fLCName;
delete fLCParLB;
delete fLCDSCB;
delete fLCAlwTrue;
delete fLC2; delete fLC1;
delete fLCFrame;

//-------  end of delete Local Conditions stuff ----------------------

//-------  delete Histogrammes stuff ---------------------------------

delete fHiPause;
delete fHiName;
delete fHiStep7LabFrame;
delete fHiChan2; delete fHiTo2; delete fHiFrom2; 
delete fHiChan1; delete fHiTo1; delete fHiFrom1;
delete fHiStep6T2Frame; delete fHiStep6T1Frame; delete fHiStep6LabFrame;
delete fHiPar2CB; delete fHiDS2CB;
delete fHiPar1CB; delete fHiDS1CB;
delete fHiStep5CB2Frame; delete fHiStep5CB1Frame; delete fHiStep5LabFrame;
delete fHiConCB;
delete fHiGlobalRB; delete fHiLocalRB;
delete fHiStep4CBFrame; delete fHiStep4RBFrame;
delete fHiCRB; delete fHiNoCRB;
delete fHiCorRB; delete fHiMulRB;
delete fHi2dRB; delete fHi1dRB;
//fHiCanv->GetCanvas()->DeleteExec("exhi");
//delete fHiCanvProj;
fHiCanv->GetCanvas()->Clear();
delete fHiCanv;
delete fHiStep8; delete fHiStep7; delete fHiStep6; delete fHiStep5;  
delete fHiStep4; delete fHiStep3; delete fHiStep2; delete fHiStep1;  
delete fHi13; delete fHi12; delete fHi11;
delete fHi2; delete fHi1;
delete fHiFrame;

//-------  end of delete Histogrammes stuff --------------------------

//-------  delete SHOWER det set stuff -------------------------------

for(Int_t i=64;i--;delete fShCellsBut[i]);
delete fSh32; delete fSh31; delete fSh30; delete fSh29; delete fSh28;
delete fShDown;
delete fShModList;
delete fShUp;
delete fSh27;
delete fShStep2Frame;
for(Int_t i=18;i--;delete fShSecModBut[i]);
delete fSh26; delete fSh25; delete fSh24; delete fSh23;
delete fShStep1Frame;
delete fShMultiFrame;
delete fShSubDefRB;
delete fShAddDefRB;
delete fShRecDefRB;
delete fShSinDefRB;
delete fShRBLayFrame;
delete fShColBox; delete fShRowBox; delete fShModBox; delete fShSecBox;
delete fSh22; delete fSh21;
delete fShSingleFrame;
delete fShName;
delete fShCatBox;
delete fShMultiRB; delete fShSingleRB;
delete fShCatFrame; delete fShRBFrame;
delete fSh2; delete fSh1;
delete fShFrame;

//-------  end of delete SHOWER det set stuff ------------------------

//-------  delete RICH det set stuff ---------------------------------

for(Int_t i=64;i--;delete fRichCellsBut[i]);
delete fRich32; delete fRich31; delete fRich30; delete fRich29; delete fRich28;
delete fRichDown;
delete fRichModList;
delete fRichUp;
delete fRich27;
delete fRichStep2Frame;
for(Int_t i=6;i--;delete fRichSecModBut[i]);
delete fRich26; delete fRich25; delete fRich24; delete fRich23;
delete fRichStep1Frame;
delete fRichMultiFrame;
delete fRichSubDefRB;
delete fRichAddDefRB;
delete fRichRecDefRB;
delete fRichSinDefRB;
delete fRichRBLayFrame;
delete fRichColBox; delete fRichRowBox; delete fRichSecBox;
delete fRich22; delete fRich21;
delete fRichSingleFrame;
delete fRichName;
delete fRichCatBox;
delete fRichMultiRB; delete fRichSingleRB;
delete fRichCatFrame; delete fRichRBFrame;
delete fRich2; delete fRich1;
delete fRichFrame;

//-------  end of delete RICH det set stuff --------------------------

//-------  delete MDC det set stuff ----------------------------------

for(Int_t i=48;i--;delete fMdcCellsBut[i]);
delete fMdc32; delete fMdc31; delete fMdc30; delete fMdc29; delete fMdc28;
delete fMdcDown;
delete fMdcModList;
delete fMdcUp;
delete fMdc27;
delete fMdcStep2Frame;
for(Int_t i=24;i--;delete fMdcSecModBut[i]);
delete fMdc26; delete fMdc25; delete fMdc24; delete fMdc23;
delete fMdcStep1Frame;
delete fMdcMultiFrame;
delete fMdcSubDefRB;
delete fMdcAddDefRB;
delete fMdcRecDefRB;
delete fMdcSinDefRB;
delete fMdcRBLayFrame;
delete fMdcCellBox; delete fMdcLayBox; delete fMdcModBox; delete fMdcSecBox;
delete fMdc22; delete fMdc21;
delete fMdcSingleFrame;
delete fMdcName;
delete fMdcCatBox;
delete fMdcMultiRB; delete fMdcSingleRB;
delete fMdcCatFrame; delete fMdcRBFrame;
delete fMdc2; delete fMdc1;
delete fMdcFrame;

//-------  end of delete MDC det set stuff ---------------------------

//-------  delete TOF det set stuff ----------------------------------

for(Int_t i=48;i--;delete fTofSecModBut[i]);
delete fTof26; delete fTof25; delete fTof24; delete fTof23;
delete fTofMultiFrame;
delete fTofSubDefRB;
delete fTofAddDefRB;
delete fTofRecDefRB;
delete fTofSinDefRB;
delete fTofRBLayFrame;
delete fTofRodBox; delete fTofModBox; delete fTofSecBox;
delete fTof22; delete fTof21;
delete fTofSingleFrame;
delete fTofName;
delete fTofCatBox;
delete fTofMultiRB; delete fTofSingleRB;
delete fTofCatFrame; delete fTofRBFrame;
delete fTof2; delete fTof1;
delete fTofFrame;

//-------  end of delete TOF det set stuff ---------------------------

delete fDSTab;
delete fDSFrame;
delete fMainTab;
delete fF01; delete fF00;
delete fF0;
delete fFrame;

countBrowser--;

}


//---------------------------------------------------------------------
//******  HOnlObjBrowser::CloseWindow()
//
// This function closes the browser.
//
//---------------------------------------------------------------------

void HOnlObjBrowser::CloseWindow() {
//
// This function closes the browser.
//

delete this;

}


//---------------------------------------------------------------------
//******  Int_t HOnlObjBrowser::calcTabsIdx()
//
// This function calculates positions of tabs.
//
//---------------------------------------------------------------------

Int_t HOnlObjBrowser::calcTabsIdx() {
//
// This function calculates positions of tabs.
//

  Int_t mainIdx = fMainTab->GetCurrent();
  Int_t DSIdx   = fDSTab->GetCurrent();

  switch(mainIdx) {

  case 0: // det set

    switch(DSIdx) {

    case 0: return 1; // TOF
    case 1: return 2; // MDC
    case 2: return 3; // RICH
    case 3: return 4; // SHOWER
    case 4: return 5; // START
    case 5: return 6; // TOFINO
    default: break;
    }
    break;

  case 1: return 7; // HISTO
  case 2: return 8; // local conditions
  case 3: return 9; // global condition

  default:
    break;
  }

  return 0;

}


//---------------------------------------------------------------------
//******  void HOnlObjBrowser::displayObjList()
//
// This function displays list fObjList.
//
//---------------------------------------------------------------------

void HOnlObjBrowser::displayObjList() {
//
// This function displays list fObjList.
//

  
  Int_t tab = fDB->tabpos;
  Int_t i = 1;
  HDetSet *pSet = 0;
  HHistogram *pHisto = 0;
  HLocalCondition *pLocal = 0;
  HGlobalCondition *pGlobal = 0;

  TGTextLBEntry *pEntry;
  while((pEntry = (TGTextLBEntry*) fObjList->Select(i,kFALSE))) {
    fObjList->RemoveEntry(i++);
  }

  if(tab>0 && tab<=6) {
    for(i=1; i<= fDB->cursor; i++) {
      pSet = gOnline->getDetSet()->at((*fDB).aridx[i]);
      fObjList->AddEntry(pSet->GetName(),i);
    }
    createDSList(fLCDSCB);
    clearLB(fLCParLB);
    createDSList(fGCDS1CB);
    createDSList(fGCDS2CB);
  }

  if(tab == 7) {
    for(i=1; i<= fDB->cursor; i++) {
      pHisto = gOnline->getHisto()->at((*fDB).aridx[i]);
      fObjList->AddEntry(pHisto->GetName(),i);
    }
  }

  if(tab == 8) {
    for(i=1; i<= fDB->cursor; i++) {
      pLocal = gOnline->getLocal()->at((*fDB).aridx[i]);
      fObjList->AddEntry(pLocal->GetName(),i);
    }
    createCountsList();
  }

  if(tab == 9) {
    for(i=1; i<= fDB->cursor; i++) {
      pGlobal = gOnline->getGlobal()->at((*fDB).aridx[i]);
      fObjList->AddEntry(pGlobal->GetName(),i);
    }
  }

  fObjList->MapSubwindows();
  fObjList->Layout();

}


//---------------------------------------------------------------------
//******  void HOnlObjBrowser::displayHisto()
//
// This function displays the highlighted histogram.
//
//---------------------------------------------------------------------

void HOnlObjBrowser::displayHisto() {
//
// This function displays the highlighted histogram.
//

  if(!gOnline) return;
  if(!gOnline->getLocal()) return;

  Int_t idx = fObjList->GetSelected();
  HHistogram *pHisto = gOnline->getHisto()->at((*fDB).aridx[idx]);
  HLocalCondition *pLocal = 0;
  char highname[50];

  if(!pHisto) return;

  fHi1dRB->SetState(kButtonUp);
  fHi2dRB->SetState(kButtonUp);
  if(pHisto->getDim() == 1) fHi1dRB->SetState(kButtonDown);
  if(pHisto->getDim() == 2) fHi2dRB->SetState(kButtonDown);

  fHiMulRB->SetState(kButtonUp);
  fHiCorRB->SetState(kButtonUp);  
  if(pHisto->getDim()==2 && pHisto->isMulti()) fHiMulRB->SetState(kButtonDown);
  if(pHisto->getDim()==2 &&!pHisto->isMulti()) fHiCorRB->SetState(kButtonDown);
  
  fHiNoCRB->SetState(kButtonUp);
  fHiCRB->SetState(kButtonUp);
  if(!pHisto->isConditioned()) fHiNoCRB->SetState(kButtonDown);
  if(pHisto->isConditioned()) fHiCRB->SetState(kButtonDown);

  fHiLocalRB->SetState(kButtonUp);
  fHiGlobalRB->SetState(kButtonUp);
  if(pHisto->isConditioned() && pHisto->getCType() == 1) {
    fHiLocalRB->SetState(kButtonDown);
    createHiCList(1);
    highlight(fHiConCB,gOnline->getLocal()->at(pHisto->getCIdx())->GetName());
  }
  if(pHisto->isConditioned() && 
     !pHisto->isMulti() && pHisto->getCType() == 2) {
    fHiGlobalRB->SetState(kButtonDown);
    createHiCList(2);
    highlight(fHiConCB,gOnline->getGlobal()->at(pHisto->getCIdx())->GetName());
  }
  if(!pHisto->isConditioned()) createHiCList();

  HDetSet *pSet = 0;
  pSet = gOnline->getDetSet()->at(pHisto->getDSIdx(1));
  if(pSet) {
    createDSList(fHiDS1CB,pSet);
    highlight(fHiDS1CB,pSet->GetName());
    createParList(fHiPar1CB,pSet);
    if(pHisto->getKey() == 10 || pHisto->getKey() == 11) {
      pLocal = gOnline->getLocal()->at(pHisto->getLCondition(1));
      if(pLocal) {
	addCounterPar(fHiPar1CB, pLocal);
	if(pHisto->getLCount(1) == 1) 
	  sprintf(highname,"%s%s",pLocal->GetName(),"_true");
	if(pHisto->getLCount(1) == 2) 
	  sprintf(highname,"%s%s",pLocal->GetName(),"_false");
	if(pHisto->getLCount(1) == 3) 
	  sprintf(highname,"%s%s",pLocal->GetName(),"_sum");
	highlight(fHiPar1CB,highname);
      }
    }
    else highlight(fHiPar1CB,pHisto->getParameter(1)->GetName());
  }
  else {
    clearCB(fHiDS1CB);
    clearCB(fHiPar1CB);
  }
  pSet = 0;
  pSet = gOnline->getDetSet()->at(pHisto->getDSIdx(2));
  if(pSet && !fHiMulRB->GetState()) {
    createDSList(fHiDS2CB,pSet);
    highlight(fHiDS2CB,pSet->GetName());
    createParList(fHiPar2CB,pSet);
    if(pHisto->getKey() == 1 || pHisto->getKey() == 11) {
      pLocal = gOnline->getLocal()->at(pHisto->getLCondition(2));
      if(pLocal) {
	addCounterPar(fHiPar2CB, pLocal);
	if(pHisto->getLCount(2) == 1) 
	  sprintf(highname,"%s%s",pLocal->GetName(),"_true");
	if(pHisto->getLCount(2) == 2) 
	  sprintf(highname,"%s%s",pLocal->GetName(),"_false");
	if(pHisto->getLCount(2) == 3) 
	  sprintf(highname,"%s%s",pLocal->GetName(),"_sum");
	highlight(fHiPar2CB,highname);
      }
    }
    else highlight(fHiPar2CB,pHisto->getParameter(2)->GetName());
  }
  else {
    clearCB(fHiDS2CB);
    clearCB(fHiPar2CB);
  }

  Float_t from = 0.;
  Float_t to = 0.;
  Int_t chan = 0;
  char tmp[10] = "";

  if(pHisto->getDim() == 1 || (pHisto->getDim()==2 && fHiMulRB->GetState())) {

    fHiFrom1->SetState(kTRUE);
    fHiTo1->SetState(kTRUE);
    fHiChan1->SetState(kTRUE);

    if(pHisto->getDim() == 1) {
      fHiFrom2->SetState(kFALSE);
      fHiTo2->SetState(kFALSE);
      fHiChan2->SetState(kFALSE);
    }
    fHiFrom1Buf->Clear();
    fHiTo1Buf->Clear();
    fHiChan1Buf->Clear();

    if(pHisto->getDim() == 1) {
      from = pHisto->getHist1()->GetXaxis()->GetXmin();
      to = pHisto->getHist1()->GetXaxis()->GetXmax();
      chan = pHisto->getHist1()->GetXaxis()->GetNbins();
    }
    if(pHisto->getDim() == 2) {
      from = pHisto->getHist2()->GetXaxis()->GetXmin();
      to = pHisto->getHist2()->GetXaxis()->GetXmax();
      chan = pHisto->getHist2()->GetXaxis()->GetNbins();
    }
    sprintf(tmp,"%.1f",from);
    fHiFrom1Buf->AddText(0,tmp);
    sprintf(tmp,"%.1f",to);
    fHiTo1Buf->AddText(0,tmp);
    sprintf(tmp,"%i",chan);
    fHiChan1Buf->AddText(0,tmp);

    fClient->NeedRedraw(fHiFrom1);
    fClient->NeedRedraw(fHiTo1);
    fClient->NeedRedraw(fHiChan1);
  }

  if(pHisto->getDim() == 2) {

    fHiFrom1->SetState(kTRUE);
    fHiTo1->SetState(kTRUE);
    fHiChan1->SetState(kTRUE);

    fHiFrom1Buf->Clear();
    fHiTo1Buf->Clear();
    fHiChan1Buf->Clear();

    from = pHisto->getHist2()->GetXaxis()->GetXmin();
    to = pHisto->getHist2()->GetXaxis()->GetXmax();
    chan = pHisto->getHist2()->GetXaxis()->GetNbins();
    
    sprintf(tmp,"%.1f",from);
    fHiFrom1Buf->AddText(0,tmp);
    sprintf(tmp,"%.1f",to);
    fHiTo1Buf->AddText(0,tmp);
    sprintf(tmp,"%i",chan);
    fHiChan1Buf->AddText(0,tmp);

    fClient->NeedRedraw(fHiFrom1);
    fClient->NeedRedraw(fHiTo1);
    fClient->NeedRedraw(fHiChan1);

    fHiFrom2->SetState(kTRUE);
    fHiTo2->SetState(kTRUE);
    fHiChan2->SetState(kTRUE);

    fHiFrom2Buf->Clear();
    fHiTo2Buf->Clear();
    fHiChan2Buf->Clear();

    from = pHisto->getHist2()->GetYaxis()->GetXmin();
    to = pHisto->getHist2()->GetYaxis()->GetXmax();
    chan = pHisto->getHist2()->GetYaxis()->GetNbins();
    
    sprintf(tmp,"%.1f",from);
    fHiFrom2Buf->AddText(0,tmp);
    sprintf(tmp,"%.1f",to);
    fHiTo2Buf->AddText(0,tmp);
    sprintf(tmp,"%i",chan);
    fHiChan2Buf->AddText(0,tmp);

    fClient->NeedRedraw(fHiFrom2);
    fClient->NeedRedraw(fHiTo2);
    fClient->NeedRedraw(fHiChan2);
  }
  
  fHiName->SetState(kTRUE);
  fHiNameBuf->Clear();
  fHiNameBuf->AddText(0,pHisto->GetName());
  fClient->NeedRedraw(fHiName);

  if(pHisto->isPause()) fHiPause->SetState(kButtonDown);
  else fHiPause->SetState(kButtonUp);

  fHiCanv->GetCanvas()->cd();
  gStyle->SetOptStat(1);
  pHisto->setDrawingOpt();
  if(pHisto->getDim() == 1) pHisto->getHist1()->Draw();
  if(pHisto->getDim() == 2) pHisto->getHist2()->Draw();
  fHiCanv->GetCanvas()->Modified();
  fHiCanv->GetCanvas()->Update();
  /*  fHiCanvProj->GetCanvas()->Clear();
  fHiCanvProj->GetCanvas()->Modified();
  fHiCanvProj->GetCanvas()->Update();*/

}


//---------------------------------------------------------------------
//******  void HOnlObjBrowser::displayLC()
//
// This function displays the highlighted local condition.
//
//---------------------------------------------------------------------

void HOnlObjBrowser::displayLC() {
//
// This function displays the highlighted local condition.
//

  if(!gOnline) return;
  Int_t idx = fObjList->GetSelected();
  HLocalCondition *pLC = gOnline->getLocal()->at((*fDB).aridx[idx]);
  if(!pLC) return;
  HDetSet *pSet = 0;

  if(pLC->isAlwaysTrue()) fLCAlwTrue->SetState(kButtonDown);
  else fLCAlwTrue->SetState(kButtonUp);

  createDSList(fLCDSCB);

  pSet = gOnline->getDetSet()->at(pLC->getDetSet());

  if(pSet) { 
    highlight(fLCDSCB,pSet->GetName());
    createParList(pSet);
    createParList(fLCXparBox,pSet);
    createParList(fLCYparBox,pSet);
  }
  
  fLCNameBuf->Clear();
  fLCNameBuf->AddText(0,pLC->GetName());
  fClient->NeedRedraw(fLCName);
  
  if(pLC->getExpString()) {
    pLC->getExpString()->copy(expStringL);
    expStringL.makeExp();
  }
  else expStringL.clear();
  expStringL.updateList(fLCExpList);

  if(pLC->getContour()) {

    createContourList(fLCContourCB);
    HUserParameter *pPar1 = (HUserParameter*) pLC->getContList()->First();
    HUserParameter *pPar2 = 0;
    if(pPar1) pPar2 = (HUserParameter*) pLC->getContList()->After(pPar1);

    highlight(fLCXparBox,pPar1->GetName());
    highlight(fLCYparBox,pPar2->GetName());
    highlight(fLCContourCB,pLC->getContour()->GetName());
  }
  else {
    createContourList(fLCContourCB);
    fLCContourCB->Select(1);
  }

}


//---------------------------------------------------------------------
//******  void HOnlObjBrowser::displayGC()
//
// This function displays the highlighted global condition.
//
//---------------------------------------------------------------------

void HOnlObjBrowser::displayGC() {
//
// This function displays the highlighted global condition.
//
  
  if(!gOnline) return;
  Int_t idx = fObjList->GetSelected();
  HGlobalCondition *pGC = gOnline->getGlobal()->at((*fDB).aridx[idx]);

  if(!pGC) return;

  fGCNameBuf->Clear();
  fGCNameBuf->AddText(0,pGC->GetName());
  fClient->NeedRedraw(fGCName);
  
  if(pGC->getExpString()) {
    pGC->getExpString()->copy(expStringG);
    expStringG.makeExp();
  }
  else expStringG.clear();
  expStringG.updateList(fGCExpList);

  clearCB(fGCDS1CB);
  clearCB(fGCDS2CB);
  clearCB(fGCXparBox);
  clearCB(fGCYparBox);

  if(pGC->getContour()) {

    createContourList(fGCContourCB);
    HDetSet *pSet1 = gOnline->getDetSet()->at(pGC->getDSIdx(1));
    HDetSet *pSet2 = gOnline->getDetSet()->at(pGC->getDSIdx(2));

    if(pSet1 && pSet2) {
      createDSList(fGCDS1CB);
      createDSList(fGCDS2CB);
      highlight(fGCDS1CB,pSet1->GetName());
      highlight(fGCDS2CB,pSet2->GetName());
      createParList(fGCXparBox,pSet1);
      createParList(fGCYparBox,pSet2);
    }

    HUserParameter *pPar1 = (HUserParameter*) pGC->getContList()->First();
    HUserParameter *pPar2 = 0;
    if(pPar1) pPar2 = (HUserParameter*) pGC->getContList()->After(pPar1);

    highlight(fGCXparBox,pPar1->GetName());
    highlight(fGCYparBox,pPar2->GetName());
    highlight(fGCContourCB,pGC->getContour()->GetName());
  }

  else {
    createContourList(fGCContourCB);
    fGCContourCB->Select(1);
  }

}



//---------------------------------------------------------------------
//******  void HOnlObjBrowser::createDSList(TGComboBox *p, HDetSet *pSet)
//
// This function sets the content of a selected combo box with detector
// sets geometrically identical to pSet. If pSet=0, puts all available
// det sets.
//
//---------------------------------------------------------------------

void HOnlObjBrowser::createDSList(TGComboBox *p, HDetSet *pSet) {
//
// This function sets the content of a selected combo box with detector
// sets geometrically identical to pSet. If pSet=0, puts all available
// det sets.
//

  if(!gOnline) return;
  if(!p) return;
  clearCB(p);
  HDetSet *pS;
  Int_t j = 1;
  Int_t i = 0;
  Int_t cat = -1;

  if(!pSet) {
    j=1;
    for(i=0; i<gOnline->getDetSet()->getEntries(); i++) {

      pS = gOnline->getDetSet()->at(i);
      if(pS) p->AddEntry(pS->GetName(),j++);
    }
  }

  if(pSet) {
    j=1;
    if(pSet->getCategory() == 1 || pSet->getCategory() == 33) { 
      p->AddEntry(pSet->GetName(),j);
      return;
    }
    for(i=0; i<gOnline->getDetSet()->getEntries(); i++) {

      pS = gOnline->getDetSet()->at(i);
      if(pS) {
	cat = pS->getCategory();
	if(pS->isGeomEqual(pSet) && cat!=1 && cat!=33) 
	  p->AddEntry(pS->GetName(),j++);
      }
    }
  }

}


//---------------------------------------------------------------------
//******  void HOnlObjBrowser::createContourList(TGComboBox *p = 0)
//
// This function sets the content of a selected combo box with available
// contours.
//
//---------------------------------------------------------------------

void HOnlObjBrowser::createContourList(TGComboBox *p) {
//
// This function sets the content of a selected combo box with available
// contours.
//

  if(!gOnline) return;
  Bool_t kBoth = kFALSE;
  if(!p) {
    p = fLCContourCB;
    kBoth = kTRUE;
  }
  clearCB(p);
  p->AddEntry("no contour",1);
  HContour *pCont;

  Int_t j = 1;
  for(Int_t i=0; i<gOnline->getContour()->getEntries(); i++) {

    pCont = gOnline->getContour()->at(i);
    if(pCont) p->AddEntry(pCont->GetName(),++j);
  }

  if(kBoth) p = fGCContourCB;
  clearCB(p);
  p->AddEntry("no contour",1);

  j = 1;
  for(Int_t i=0; i<gOnline->getContour()->getEntries(); i++) {

    pCont = gOnline->getContour()->at(i);
    if(pCont) p->AddEntry(pCont->GetName(),++j);
  }

}


//---------------------------------------------------------------------
//******  void HOnlObjBrowser::createParList(TGComboBox *p, HDetSet *pSet)
//
// This function sets the content of a selected combo box with parameters
// corresponding to the category of the detector set. Used for the histogram
// subpanel.
//
//---------------------------------------------------------------------

void HOnlObjBrowser::createParList(TGComboBox *p, HDetSet *pSet) {
//
// This function sets the content of a selected combo box with parameters
// corresponding to the category of the detector set. Used for the histogram
// subpanel.
//

  if(!p || !pSet) return;
  clearCB(p);
  Int_t cat = pSet->getCategory();
  HUserParameter *pParam;
  Int_t j=1;
  for(Int_t i=0; i<fDB->getEntries(); i++) {

    pParam = fDB->at(i);
    if(pParam) {
      if(pParam->getCategory() == cat) p->AddEntry(pParam->GetName(),j++);
    }
  }

}


//---------------------------------------------------------------------
//******  void HOnlObjBrowser::addCounterPar(TGComboBox *p)
//
// This function adds to the content of a selected combo box 3 entries:
// Count_true, Count_false and Count_sum.
// Used for multiplicity histogrammes (histogram parameter is a condition 
// counter).
//
//---------------------------------------------------------------------

void HOnlObjBrowser::addCounterPar(TGComboBox *p) {
//
// This function adds to the content of a selected combo box 3 entries:
// Count_true, Count_false and Count_sum.
// Used for multiplicity histogrammes (histogram parameter is a condition 
// counter).
//

  if(!p) return;

  TGListBox *pListBox = const_cast <TGListBox*> (p->GetListBox());
  if(!pListBox) return;

  TGTextLBEntry *pEntry = 0;
  Int_t i = 1;

  while((pEntry = (TGTextLBEntry*) pListBox->Select(i,kFALSE))) i++;
  
  p->AddEntry("Count_true",i);
  p->AddEntry("Count_false",i+1);
  p->AddEntry("Count_sum",i+2);

  pListBox = 0;

}


//---------------------------------------------------------------------
//******  void HOnlObjBrowser::addCounterPar(TGComboBox *p, HDetSet *pSet,
//                                           HGlobalCondition *pGC)
//
// This function adds to the content of a selected combo box a certain 
// number of entries:
// lname_true, lname_false and lname_sum,
// where lname - names of the local conditions which contain the detector
// set pSet and are present in the list of pGC 
// Used for multiplicity histogrammes (histogram parameter is a condition 
// counter).
//
//---------------------------------------------------------------------

void HOnlObjBrowser::addCounterPar(TGComboBox *p, HDetSet *pSet, 
				   HGlobalCondition *pGC) {
//
// This function adds to the content of a selected combo box a certain 
// number of entries:
// lname_true, lname_false and lname_sum,
// where lname - names of the local conditions which contain the detector
// set pSet and are present in the list of pGC.
// Used for multiplicity histogrammes (histogram parameter is a condition 
// counter).
//

  if(!p || !pSet || !pGC) return; 
  if(!gOnline) return;
  if(!gOnline->getLocal()) return;
  if(!gOnline->getGlobal()) return;
  if(!gOnline->getDetSet()) return;

  HLocalCondition *pLC;
  HNameIdx *pNameIdx = 0;
  Int_t idx = -1;

  TIter iter(pGC->getLCList());
 
  while((pNameIdx = (HNameIdx*) iter())) {
 
    pLC = gOnline->getLocal()->at(pNameIdx->fLCIndex);                         
    if(pLC) {
      idx = pLC->getDetSet();
      if(gOnline->getDetSet()->at(idx)) {
	if(!strcmp(pSet->GetName(),gOnline->getDetSet()->at(idx)->GetName()))
	  addCounterPar(p,pLC);
      }
    }
  }

}


//---------------------------------------------------------------------
//******  void HOnlObjBrowser::addCounterPar(TGComboBox *p, HLocalCondition 
//                                                          *pLC)
//
// This function adds to the content of a selected combo box a certain 
// number of entries:
// lname_true, lname_false and lname_sum,
// where lname - name of the local condition pLC
// Used for multiplicity histogrammes (histogram parameter is a condition 
// counter).
//
//---------------------------------------------------------------------

void HOnlObjBrowser::addCounterPar(TGComboBox *p, HLocalCondition *pLC) {
//
// This function adds to the content of a selected combo box a certain 
// number of entries:
// lname_true, lname_false and lname_sum,
// where lname - name of the local condition pLC
// Used for multiplicity histogrammes (histogram parameter is a condition 
// counter).
//

  if(!p || !pLC) return; 
  if(!gOnline) return;
  if(!gOnline->getLocal()) return;

  TGListBox *pListBox = const_cast <TGListBox*> (p->GetListBox());
  if(!pListBox) return;

  TGTextLBEntry *pEntry = 0;
  Int_t i = 1;

  while((pEntry = (TGTextLBEntry*) pListBox->Select(i,kFALSE))) i++;
  
  char entry[30];
  sprintf(entry,"%s%s",pLC->GetName(),"_true");
  p->AddEntry(entry,i);
  sprintf(entry,"%s%s",pLC->GetName(),"_false");
  p->AddEntry(entry,i+1);
  sprintf(entry,"%s%s",pLC->GetName(),"_sum");
  p->AddEntry(entry,i+2);

  pListBox = 0;

}


//---------------------------------------------------------------------
//******  void HOnlObjBrowser::createParList(HDetSet *pSet = 0)
//
// This function sets the content of the list box fLCParLB with parameters
// corresponding to the category of the detector set. Used for the local
// conditions subpanel.
//
//---------------------------------------------------------------------

void HOnlObjBrowser::createParList(HDetSet *pSet) {
//
// This function sets the content of the list box fLCParLB with parameters
// corresponding to the category of the detector set. Used for the local
// conditions subpanel.
//

  TGTextLBEntry *pEntry = 0;
  Int_t i = 1;
  while((pEntry = (TGTextLBEntry*) fLCParLB->Select(i,kFALSE))) {

    fLCParLB->RemoveEntry(i++);

  }

  if(!pSet) {
    fLCParLB->MapSubwindows();
    fLCParLB->Layout();
    return;
  }

  Int_t cat = pSet->getCategory();
  HUserParameter *pParam;
  Int_t j=1;
  char tmp[50] = "";

  for(Int_t i=0; i<fDB->getEntries(); i++) {

    pParam = fDB->at(i);
    if(pParam) {
      if(pParam->getCategory() == cat) {
	if(!pParam->getExpression()) {

	  sprintf(tmp,"%s%i%s%s","[",pParam->getIndex()-1,"] - ",
		  pParam->GetName());
	  fLCParLB->AddEntry(tmp,j++);
	}
	else fLCParLB->AddEntry(pParam->GetName(),j++);
      }
    }
  }

  fLCParLB->MapSubwindows();
  fLCParLB->Layout();
  
}



//---------------------------------------------------------------------
//******  void HOnlObjBrowser::clearCB(TGComboBox *p)
//
// This function clears the combo box p.
//
//---------------------------------------------------------------------

void HOnlObjBrowser::clearCB(TGComboBox *p) {
//
// This function clears the combo box p.
//

  if(!p) return;
  TGTextLBEntry *pEntry;
  TGListBox *pListBox;
  Int_t i = 1;

  pListBox = const_cast <TGListBox*> (p->GetListBox());
  if(!pListBox) return;

  while((pEntry = (TGTextLBEntry*) pListBox->Select(i,kFALSE))) {
    if(i==1) {
      pEntry->SetText(new TGString(""));
      p->Select(i);
    }
    p->RemoveEntry(i++);
  }
  pListBox = 0;
  
}


//---------------------------------------------------------------------
//******  void HOnlObjBrowser::clearLB(TGListBox *p)
//
// This function clears the list box p.
//
//---------------------------------------------------------------------

void HOnlObjBrowser::clearLB(TGListBox *p) {
//
// This function clears the list box p.
//

  if(!p) return;
  TGTextLBEntry *pEntry;
  Int_t i = 1;

  while((pEntry = (TGTextLBEntry*) p->Select(i,kFALSE))) {
    p->RemoveEntry(i++);
  }
  p->MapSubwindows();
  p->Layout();
  
}


//---------------------------------------------------------------------
//******  void HOnlObjBrowser::createHiCList(Int_t mode)
//
// This function sets the content of the combo box fHiConCB.
//
// mode = 0 - empty list
//        1 - local conditions
//        2 - global conditions
//
//---------------------------------------------------------------------

void HOnlObjBrowser::createHiCList(Int_t mode) {
//
// This function sets the content of the combo box fHiConCB.
//
// mode = 0 - empty list
//        1 - local conditions
//        2 - global conditions
//

  if(!gOnline) return;

  clearCB(fHiConCB);
  Int_t i=0;
  Int_t j=1;

  if(mode==1) {

    HLocalCondition *pLocal = 0;
    j = 1;
    for(i=0; i < gOnline->getLocal()->getEntries(); i++) {

      pLocal = gOnline->getLocal()->at(i);
      if(pLocal) fHiConCB->AddEntry(pLocal->GetName(),j++);
    }
  }

  if(mode==2) {

    HGlobalCondition *pGlobal = 0;
    j = 1;
    for(i=0; i < gOnline->getGlobal()->getEntries(); i++) {

      pGlobal = gOnline->getGlobal()->at(i);
      if(pGlobal) fHiConCB->AddEntry(pGlobal->GetName(),j++);
    }
  }

}


//---------------------------------------------------------------------
//******  void HOnlObjBrowser::highlight(TGComboBox *p, const char* name)
//
// This function highlights the named entry in the combo box.
//
//---------------------------------------------------------------------

void HOnlObjBrowser::highlight(TGComboBox *p, const char* name) {
//
// This function highlights the named entry in the combo box.
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
//******  void HOnlObjBrowser::highlight(TGlistBox *p, const char* name)
//
// This function highlights the named entry in the list box.
//
//---------------------------------------------------------------------

void HOnlObjBrowser::highlight(TGListBox *p, const char* name) {
//
// This function highlights the named entry in the list box.
//

  if(!p || !name) return;
  TGTextLBEntry *pEntry;
  Int_t i = 1;

  while((pEntry = (TGTextLBEntry*) p->Select(i,kFALSE))) {

    if(!strcmp(pEntry->GetText()->GetString(),name)) {
      p->Select(i,kTRUE);
      break;
    }
    i++;
  }

  p->MapSubwindows();
  p->Layout();

}


//---------------------------------------------------------------------
//******  void HOnlObjBrowser::controlHiGui()
//
// This function provides self defence for the histogram panel.
// (experimental function)
//
//---------------------------------------------------------------------

void HOnlObjBrowser::controlHiGui(TGFrame *p) {
//
// This function handles all GUI widgets in the histogram panel.
//

  if(!p) return;
  if(!gOnline) return;
  HDetSet *pSet = 0;
  HLocalCondition *pLocal = 0;
  HGlobalCondition *pGlobal = 0;
  TGTextLBEntry *pEntry = 0;
  TGTextLBEntry *pEntry2 = 0;
  Int_t idx = -1;
  Int_t idx2 = -1;

  // panel "Step 1"  1dim/2dim

  if(p == fHi1dRB) {   // one dimension
    if(fHi1dRB->GetState()) {
      fHi2dRB->SetState(kButtonUp);
      fHiMulRB->SetState(kButtonUp);
      fHiCorRB->SetState(kButtonUp);
    }
    p = fHi2dRB;
  }

  if(p == fHi2dRB) {   // two dimensions
    if(fHi2dRB->GetState()) {
      fHi1dRB->SetState(kButtonUp);      
    }
    p = fHiMulRB;
  }

  // panel "Step 2"  multi/correl

  if(p == fHiMulRB) {   // multi histogram
    if(fHi1dRB->GetState()) fHiMulRB->SetState(kButtonUp);
    if(fHiMulRB->GetState()) {
      fHiCorRB->SetState(kButtonUp);
      fHi1dRB->SetState(kButtonUp);
      fHi2dRB->SetState(kButtonDown);
    }
    p = fHiCorRB;
  }

  if(p == fHiCorRB) {   // correlation histogram
    if(fHi1dRB->GetState()) fHiCorRB->SetState(kButtonUp);
    if(fHiCorRB->GetState()) {
      fHiMulRB->SetState(kButtonUp);
      fHi1dRB->SetState(kButtonUp);
      fHi2dRB->SetState(kButtonDown);
    }
    p = fHiNoCRB;
  }

  // panel "Step 3" condition/no condition

  if(p == fHiNoCRB) {  // no condition
    if(!fHi1dRB->GetState() && !fHi2dRB->GetState()) 
      fHiNoCRB->SetState(kButtonUp);
    if(fHi2dRB->GetState() && !fHiMulRB->GetState() && !fHiCorRB->GetState())
      fHiNoCRB->SetState(kButtonUp);
    if(fHiNoCRB->GetState()) {
      fHiCRB->SetState(kButtonUp);
      fHiLocalRB->SetState(kButtonUp);
      fHiGlobalRB->SetState(kButtonUp);
      clearCB(fHiConCB);
      createDSList(fHiDS1CB);
      clearCB(fHiPar1CB);
      fHiFrom1->SetState(kFALSE);
      fHiTo1->SetState(kFALSE);
      fHiChan1->SetState(kFALSE);
      fHiFrom2->SetState(kFALSE);
      fHiTo2->SetState(kFALSE);
      fHiChan2->SetState(kFALSE);
      if(fHi2dRB->GetState() && !fHiMulRB->GetState()) createDSList(fHiDS2CB);
      else clearCB(fHiDS2CB);
      clearCB(fHiPar2CB);
    }
    p = fHiCRB;
  }

  if(p == fHiCRB) {    // condition
    if(!fHi1dRB->GetState() && !fHi2dRB->GetState()) 
      fHiCRB->SetState(kButtonUp);
    if(fHi2dRB->GetState() && !fHiMulRB->GetState() && !fHiCorRB->GetState())
      fHiCRB->SetState(kButtonUp);
    if(fHiCRB->GetState()) {
      fHiNoCRB->SetState(kButtonUp);
      if(fHiLocalRB->GetState()) createHiCList(1);
      if(fHiGlobalRB->GetState()) createHiCList(2);
      clearCB(fHiDS1CB);
      clearCB(fHiPar1CB);
      clearCB(fHiDS2CB);
      clearCB(fHiPar2CB);
      fHiFrom1->SetState(kFALSE);
      fHiTo1->SetState(kFALSE);
      fHiChan1->SetState(kFALSE);
      fHiFrom2->SetState(kFALSE);
      fHiTo2->SetState(kFALSE);
      fHiChan2->SetState(kFALSE);
    }
    p = fHiLocalRB;
  }

  // panel "Step 4"  local/global condition

  if(p == fHiLocalRB) {   // local condition
    if(!fHi1dRB->GetState() && !fHi2dRB->GetState()) 
      fHiLocalRB->SetState(kButtonUp);
    if(fHi2dRB->GetState() && !fHiMulRB->GetState() && !fHiCorRB->GetState())
      fHiLocalRB->SetState(kButtonUp);
    if(!fHiCRB->GetState()) fHiLocalRB->SetState(kButtonUp);
    if(fHiLocalRB->GetState()) {
      fHiGlobalRB->SetState(kButtonUp);
      createHiCList(1);
      clearCB(fHiDS1CB);
      clearCB(fHiPar1CB);
      clearCB(fHiDS2CB);
      clearCB(fHiPar2CB);
      fHiFrom1->SetState(kFALSE);
      fHiTo1->SetState(kFALSE);
      fHiChan1->SetState(kFALSE);
      fHiFrom2->SetState(kFALSE);
      fHiTo2->SetState(kFALSE);
      fHiChan2->SetState(kFALSE);
    }
    p = fHiGlobalRB;
  }

  if(p == fHiGlobalRB) {
    if(!fHi1dRB->GetState() && !fHi2dRB->GetState()) 
      fHiGlobalRB->SetState(kButtonUp);
    if(fHi2dRB->GetState() && !fHiMulRB->GetState() && !fHiCorRB->GetState())
      fHiGlobalRB->SetState(kButtonUp);
    if(!fHiCRB->GetState()) fHiGlobalRB->SetState(kButtonUp);
    if(fHi2dRB->GetState() && fHiMulRB->GetState()) 
      fHiGlobalRB->SetState(kButtonUp); // forbids global conditions for multi
                                        // histogrammes
    //    if(fHi1dRB->GetState() && fHiCRB->GetState()) 
    //      fHiGlobalRB->SetState(kButtonUp);


    if(fHiGlobalRB->GetState()) {
      fHiLocalRB->SetState(kButtonUp);
      createHiCList(2);
      clearCB(fHiDS1CB);
      clearCB(fHiPar1CB);
      clearCB(fHiDS2CB);
      clearCB(fHiPar2CB);
      fHiFrom1->SetState(kFALSE);
      fHiTo1->SetState(kFALSE);
      fHiChan1->SetState(kFALSE);
      fHiFrom2->SetState(kFALSE);
      fHiTo2->SetState(kFALSE);
      fHiChan2->SetState(kFALSE);
    }
    p = fHiConCB;
  }

  if(p == fHiConCB) {  // combobox with condition name
    if(!fHiLocalRB->GetState() && !fHiGlobalRB->GetState()) clearCB(fHiConCB);
    if(!fHiNoCRB->GetState() && !fHiCRB->GetState()) {
      clearCB(fHiDS1CB);
      clearCB(fHiDS2CB);
      clearCB(fHiPar1CB);
      clearCB(fHiPar2CB);
      fHiFrom1->SetState(kFALSE);
      fHiTo1->SetState(kFALSE);
      fHiChan1->SetState(kFALSE);
      fHiFrom2->SetState(kFALSE);
      fHiTo2->SetState(kFALSE);
      fHiChan2->SetState(kFALSE);
    }
    if(fHiCRB->GetState()) {
      pEntry = (TGTextLBEntry*) fHiConCB->GetSelectedEntry();
      if(pEntry) {
	if(fHiLocalRB->GetState()) {
	  idx = gOnline->getLocal()->find(pEntry->GetText()->GetString());
	  if(idx != -1) {
	    pSet = gOnline->getDetSet()->at(gOnline->getLocal()->
					    at(idx)->getDetSet());
	  }
	  if(pSet) {
	    if(fHi1dRB->GetState()) createDSList(fHiDS1CB, pSet);
	    else clearCB(fHiDS1CB);
	    clearCB(fHiPar1CB);
	    clearCB(fHiPar2CB);
	    if(fHi2dRB->GetState()) {
	      createDSList(fHiDS1CB, pSet);
	      createDSList(fHiDS2CB, pSet);
	    }
	    else {
	      clearCB(fHiDS2CB);
	    }
	    if(fHiMulRB->GetState()) clearCB(fHiDS2CB);
	  }
	  else {
	    clearCB(fHiDS1CB);
	    clearCB(fHiDS2CB);
	    clearCB(fHiPar1CB);
	    clearCB(fHiPar2CB);
	  }
	}
	if(fHiGlobalRB->GetState()) {
	  if(fHi1dRB->GetState()) createDSList(fHiDS1CB);
	  else clearCB(fHiDS1CB);
	  clearCB(fHiPar1CB);
	  clearCB(fHiPar2CB);
	  if(fHi2dRB->GetState()) {
	    createDSList(fHiDS1CB);
	    createDSList(fHiDS2CB);
	  }
	  else {
	    clearCB(fHiDS2CB);
	  }
	}	
      }
      else {
	clearCB(fHiDS1CB);
	clearCB(fHiDS2CB);
	clearCB(fHiPar1CB);
	clearCB(fHiPar2CB);
      }
    }
    p = fHiDS1CB;
    if(!fHiLocalRB->GetState() && !fHiGlobalRB->GetState()) p = 0;
  }

  if(p == fHiDS1CB) { // combobox with first (x axis) detector set
    if(fHi2dRB->GetState() && !fHiMulRB->GetState() && !fHiCorRB->GetState()) {
      clearCB(fHiDS1CB);
      clearCB(fHiPar1CB);
      fHiFrom1->SetState(kFALSE);
      fHiTo1->SetState(kFALSE);
      fHiChan1->SetState(kFALSE);
    }
    if(!fHiNoCRB->GetState() && !fHiCRB->GetState()) {
      clearCB(fHiDS1CB);
      clearCB(fHiPar1CB);
      fHiFrom1->SetState(kFALSE);
      fHiTo1->SetState(kFALSE);
      fHiChan1->SetState(kFALSE);
    }
    pEntry = (TGTextLBEntry*) fHiDS1CB->GetSelectedEntry();
    if(pEntry) {
      idx = gOnline->getDetSet()->find(pEntry->GetText()->GetString());
      pSet = gOnline->getDetSet()->at(idx);
      if(pSet) {
	createParList(fHiPar1CB, pSet);
	if(fHiLocalRB->GetState()) {
	  pEntry2 = (TGTextLBEntry*) fHiConCB->GetSelectedEntry();
	  if(pEntry2) {
	    idx2 = gOnline->getLocal()->find(pEntry2->GetText()->GetString());
	    pLocal = gOnline->getLocal()->at(idx2);
	    if(pLocal) addCounterPar(fHiPar1CB, pLocal);
	  }
	}
	if(fHiGlobalRB->GetState()) {
	  pEntry2 = (TGTextLBEntry*) fHiConCB->GetSelectedEntry();
	  if(pEntry2) {
	    idx2 = gOnline->getGlobal()->find(pEntry2->GetText()->GetString());
	    pGlobal = gOnline->getGlobal()->at(idx2);
	    if(pGlobal) addCounterPar(fHiPar1CB, pSet, pGlobal);
	  }
	}
	fHiFrom1->SetState(kFALSE);
	fHiTo1->SetState(kFALSE);
	fHiChan1->SetState(kFALSE);
	if(fHi2dRB->GetState() && fHiLocalRB->GetState() && 
	   !fHiMulRB->GetState()) {
	  highlight(fHiDS2CB, pSet->GetName());
	  createParList(fHiPar2CB, pSet);
	  if(fHiLocalRB->GetState()) {
	    pEntry2 = (TGTextLBEntry*) fHiConCB->GetSelectedEntry();
	    if(pEntry2) {
	      idx2 = gOnline->getLocal()->
		find(pEntry2->GetText()->GetString());
	      pLocal = gOnline->getLocal()->at(idx2);
	      if(pLocal) addCounterPar(fHiPar2CB, pLocal);
	    }
	  }
	  fHiFrom2->SetState(kFALSE);
	  fHiTo2->SetState(kFALSE);
	  fHiChan2->SetState(kFALSE);
	}
      }
      else clearCB(fHiPar1CB);
    }
    else clearCB(fHiPar1CB);
    p = fHiDS2CB;
  }    

  if(p == fHiDS2CB) {
    if(fHi2dRB->GetState() && !fHiMulRB->GetState() && !fHiCorRB->GetState()) {
      clearCB(fHiDS2CB);
      clearCB(fHiPar2CB);
      fHiFrom2->SetState(kFALSE);
      fHiTo2->SetState(kFALSE);
      fHiChan2->SetState(kFALSE);
    }
    if(!fHiNoCRB->GetState() && !fHiCRB->GetState()) {
      clearCB(fHiDS2CB);
      clearCB(fHiPar2CB);
      fHiFrom2->SetState(kFALSE);
      fHiTo2->SetState(kFALSE);
      fHiChan2->SetState(kFALSE);
    }
    pEntry = (TGTextLBEntry*) fHiDS2CB->GetSelectedEntry();
    if(pEntry) {
      idx = gOnline->getDetSet()->find(pEntry->GetText()->GetString());
      pSet = gOnline->getDetSet()->at(idx);
      if(pSet) {
	createParList(fHiPar2CB, pSet);
	if(fHiLocalRB->GetState()) {
	  pEntry2 = (TGTextLBEntry*) fHiConCB->GetSelectedEntry();
	  if(pEntry2) {
	    idx2 = gOnline->getLocal()->find(pEntry2->GetText()->GetString());
	    pLocal = gOnline->getLocal()->at(idx2);
	    if(pLocal) addCounterPar(fHiPar2CB, pLocal);
	  }
	}
	if(fHiGlobalRB->GetState()) {
	  pEntry2 = (TGTextLBEntry*) fHiConCB->GetSelectedEntry();
	  if(pEntry2) {
	    idx2 = gOnline->getGlobal()->find(pEntry2->GetText()->GetString());
	    pGlobal = gOnline->getGlobal()->at(idx2);
	    if(pGlobal) addCounterPar(fHiPar2CB, pSet, pGlobal);
	  }
	}
	fHiFrom2->SetState(kFALSE);
	fHiTo2->SetState(kFALSE);
	fHiChan2->SetState(kFALSE);
	if(fHi2dRB->GetState() && fHiLocalRB->GetState()) {
	  highlight(fHiDS1CB, pSet->GetName());
	  createParList(fHiPar1CB, pSet);
	  if(fHiLocalRB->GetState()) {
	    pEntry2 = (TGTextLBEntry*) fHiConCB->GetSelectedEntry();
	    if(pEntry2) {
	      idx2 = gOnline->getLocal()->
		find(pEntry2->GetText()->GetString());
	      pLocal = gOnline->getLocal()->at(idx2);
	      if(pLocal) addCounterPar(fHiPar1CB, pLocal);
	    }
	  }
	  fHiFrom1->SetState(kFALSE);
	  fHiTo1->SetState(kFALSE);
	  fHiChan1->SetState(kFALSE);
	}
      }
      else clearCB(fHiPar2CB);
    }
    else clearCB(fHiPar2CB);
    p = fHiPar1CB;
  }    

  if(p == fHiPar1CB) {
    pEntry = (TGTextLBEntry*) fHiPar1CB->GetSelectedEntry();
    if(pEntry) {
      idx = fDB->find(pEntry->GetText()->GetString());
      if(fDB->at(idx)) {
	fHiFrom1->SetState(kTRUE);
	fHiTo1->SetState(kTRUE);
	fHiChan1->SetState(kTRUE);
      }
      if(!strcmp("true",getAfterUnderline(pEntry->GetText()->GetString())) ||
	 !strcmp("false",getAfterUnderline(pEntry->GetText()->GetString())) ||
	 !strcmp("sum",getAfterUnderline(pEntry->GetText()->GetString()))) {
	fHiFrom1->SetState(kTRUE);
	fHiTo1->SetState(kTRUE);
	fHiChan1->SetState(kTRUE);
	}
    }
    else {
      fHiFrom1->SetState(kFALSE);
      fHiTo1->SetState(kFALSE);
      fHiChan1->SetState(kFALSE);
    }
    p = fHiPar2CB;
  }

  if(p == fHiPar2CB) {
    pEntry = (TGTextLBEntry*) fHiPar2CB->GetSelectedEntry();
    if(pEntry) {
      idx = fDB->find(pEntry->GetText()->GetString());
      if(fDB->at(idx)) {
	fHiFrom2->SetState(kTRUE);
	fHiTo2->SetState(kTRUE);
	fHiChan2->SetState(kTRUE);
      }
      if(!strcmp("true",getAfterUnderline(pEntry->GetText()->GetString())) ||
	 !strcmp("false",getAfterUnderline(pEntry->GetText()->GetString())) ||
	 !strcmp("sum",getAfterUnderline(pEntry->GetText()->GetString()))) {
	fHiFrom2->SetState(kTRUE);
	fHiTo2->SetState(kTRUE);
	fHiChan2->SetState(kTRUE);
      }
    }
    else {
      fHiFrom2->SetState(kFALSE);
      fHiTo2->SetState(kFALSE);
      fHiChan2->SetState(kFALSE);
    }
  }
  fHiCanv->GetCanvas()->Clear();
  fHiCanv->GetCanvas()->Modified();
  fHiCanv->GetCanvas()->Update();
  /*  fHiCanvProj->GetCanvas()->Clear();
  fHiCanvProj->GetCanvas()->Modified();
  fHiCanvProj->GetCanvas()->Update();*/
}


//---------------------------------------------------------------------
//******  HHistogram* HOnlObjBrowser::createHisto()
//
// This function scans all widgets of the histogram subpanel, creates an
// object of the class HHistogram and checks its consistency. If the object
// is consistent returns it. If not returns 0.
//
//---------------------------------------------------------------------

HHistogram* HOnlObjBrowser::createHisto() {
//
// This function scans all widgets of the histogram subpanel, creates an
// object of the class HHistogram and checks its consistency. If the object
// is consistent returns it. If not, returns 0.
//

  if(!gOnline) {
    HError::message("Main control class does not exist.");
    return 0;
  }
  if(gOnline->getMemCheck()->isAlarm()) {
    HError::message("Memory limit already exceeded.");
    return 0;
  }

  HHistogram *pHisto = new HHistogram();
  if(!pHisto) return 0;
  Axis_t xfrom, xto;
  Axis_t yfrom, yto = 0.;
  Int_t  xchan, ychan = 0;
  TGTextLBEntry *pEntry = 0;
  HUserParameter *pPar = 0;
  HDetSet *pSet = 0;
  Int_t idx = -1;
  Int_t cat = 0;
  TH1F *hist1 = 0;
  TH2F *hist2 = 0;
  char tmp[50] = "";

  if(fHi1dRB->GetState() && !fHi2dRB->GetState()) pHisto->setDim(1);
  if(!fHi1dRB->GetState() && fHi2dRB->GetState()) pHisto->setDim(2);

  if(fHiMulRB->GetState() && !fHiCorRB->GetState()) pHisto->setMulti();
  if(!fHiMulRB->GetState() && fHiCorRB->GetState()) pHisto->setMulti(kFALSE);

  if(fHiNoCRB->GetState() && !fHiCRB->GetState()) pHisto->setCType(0);
  if(!fHiNoCRB->GetState() && fHiCRB->GetState()) {
    if(fHiLocalRB->GetState() && !fHiGlobalRB->GetState()) pHisto->setCType(1);
    if(!fHiLocalRB->GetState() && fHiGlobalRB->GetState()) pHisto->setCType(2);
  }
  
  if(pHisto->getCType() == 1) { // local condition

    pEntry = (TGTextLBEntry*) fHiConCB->GetSelectedEntry();
    if(pEntry) {
      pHisto->setCName(pEntry->GetText()->GetString());
      pHisto->setCIdx(gOnline->getLocal()->
		      find(pEntry->GetText()->GetString()));
    }
  }

  if(pHisto->getCType() == 2) { // global condition

    pEntry = (TGTextLBEntry*) fHiConCB->GetSelectedEntry();
    if(pEntry) {
      pHisto->setCName(pEntry->GetText()->GetString());
      pHisto->setCIdx(gOnline->getGlobal()->
		      find(pEntry->GetText()->GetString()));
    }
  }

  if(pHisto->getDim() == 1) {

    pEntry = (TGTextLBEntry*) fHiDS1CB->GetSelectedEntry();
    if(pEntry) {
      pHisto->setDSName(pEntry->GetText()->GetString(),1);
      pHisto->setDSIdx(gOnline->getDetSet()->
		       find(pEntry->GetText()->GetString()),1);
    }
    pEntry = (TGTextLBEntry*) fHiPar1CB->GetSelectedEntry();
    if(pEntry) {
      idx = fDB->find(pEntry->GetText()->GetString());
      if(idx>=0) {
	pPar = new HUserParameter();
	if(pPar) fDB->at(idx)->copy(pPar);

	pHisto->setParameter(pPar,1);
      }
      if(!strcmp(getAfterUnderline(pEntry->GetText()->GetString()),"true")) {
	pHisto->setKey(10);
	pHisto->setLCondition(gOnline->getLocal()->
			      find(getBeforeUnderline(pEntry->GetText()->
						      GetString())),1);
	pHisto->setLCount(1,1);
      }
      if(!strcmp(getAfterUnderline(pEntry->GetText()->GetString()),"false")) {
	pHisto->setKey(10);
	pHisto->setLCondition(gOnline->getLocal()->
			      find(getBeforeUnderline(pEntry->GetText()->
						      GetString())),1);
	pHisto->setLCount(2,1);
      }
      if(!strcmp(getAfterUnderline(pEntry->GetText()->GetString()),"sum")) {
	pHisto->setKey(10);
	pHisto->setLCondition(gOnline->getLocal()->
			      find(getBeforeUnderline(pEntry->GetText()->
						      GetString())),1);
	pHisto->setLCount(3,1);
      }
    }
  }

  if(pHisto->getDim() == 2) { // two dimensional histogram

    pEntry = (TGTextLBEntry*) fHiDS1CB->GetSelectedEntry();
    if(pEntry) {
      sprintf(tmp,"%s",pEntry->GetText()->GetString());
      pHisto->setDSName(pEntry->GetText()->GetString(),1);
      pHisto->setDSIdx(gOnline->getDetSet()->
		       find(pEntry->GetText()->GetString()),1);
    }
    pEntry = (TGTextLBEntry*) fHiPar1CB->GetSelectedEntry();
    if(pEntry) {
      idx = fDB->find(pEntry->GetText()->GetString());
      if(idx>=0) {
	pPar = new HUserParameter();
	if(pPar) fDB->at(idx)->copy(pPar);
	pHisto->setParameter(pPar,1);
      }
      if(!strcmp(getAfterUnderline(pEntry->GetText()->GetString()),"true")) {
	pHisto->setKey(10);
	pHisto->setLCondition(gOnline->getLocal()->
			      find(getBeforeUnderline(pEntry->GetText()->
						      GetString())),1);
	pHisto->setLCount(1,1);
      }
      if(!strcmp(getAfterUnderline(pEntry->GetText()->GetString()),"false")) {
	pHisto->setKey(10);
	pHisto->setLCondition(gOnline->getLocal()->
			      find(getBeforeUnderline(pEntry->GetText()->
						      GetString())),1);
	pHisto->setLCount(2,1);
      }
      if(!strcmp(getAfterUnderline(pEntry->GetText()->GetString()),"sum")) {
	pHisto->setKey(10);
	pHisto->setLCondition(gOnline->getLocal()->
			      find(getBeforeUnderline(pEntry->GetText()->
						      GetString())),1);
	pHisto->setLCount(3,1);
      }
    }

    if(pHisto->isMulti() && 
       (pHisto->getCType() == 1 || pHisto->getCType() == 0)) { 

      if(pHisto->getParameter(1)) cat = pHisto->getParameter(1)->getCategory();
      if(cat > 0) {
	pPar = new HUserParameter();
	if(pPar) {
	  pHisto->setDSName(tmp,2);
	  pHisto->setDSIdx(pHisto->getDSIdx(1),2);
	  pPar->setCategory(cat);
	  pPar->setIndex(0);
	  pHisto->setParameter(pPar,2);
	}
      }
    }

    if(!pHisto->isMulti()) {
      pEntry = (TGTextLBEntry*) fHiDS2CB->GetSelectedEntry();
      if(pEntry) {
	pHisto->setDSName(pEntry->GetText()->GetString(),2);
	pHisto->setDSIdx(gOnline->getDetSet()->
			 find(pEntry->GetText()->GetString()),2);
      }
      pEntry = (TGTextLBEntry*) fHiPar2CB->GetSelectedEntry();
      if(pEntry) {
	idx = fDB->find(pEntry->GetText()->GetString());
	if(idx>=0) {
	  pPar = new HUserParameter();
	  if(pPar) fDB->at(idx)->copy(pPar);
	  pHisto->setParameter(pPar,2);
	}
	if(!strcmp(getAfterUnderline(pEntry->GetText()->GetString()),"true")) {
	  if(pHisto->getKey()==10) pHisto->setKey(11);
	  else pHisto->setKey(1);
	  pHisto->setLCondition(gOnline->getLocal()->
				find(getBeforeUnderline(pEntry->GetText()->
							GetString())),2);
	  pHisto->setLCount(1,2);
	}
	if(!strcmp(getAfterUnderline(pEntry->GetText()->GetString()),"false")) 
	  {
	    if(pHisto->getKey()==10) pHisto->setKey(11);
	    else pHisto->setKey(1);
	    pHisto->setLCondition(gOnline->getLocal()->
				  find(getBeforeUnderline(pEntry->GetText()->
							  GetString())),2);
	    pHisto->setLCount(2,2);
	  }
	if(!strcmp(getAfterUnderline(pEntry->GetText()->GetString()),"sum")) {
	  if(pHisto->getKey()==10) pHisto->setKey(11);
	  else pHisto->setKey(1);
	  pHisto->setLCondition(gOnline->getLocal()->
				find(getBeforeUnderline(pEntry->GetText()->
							GetString())),2);
	  pHisto->setLCount(3,2);
	}
      }
    }
    
  }

  if(fHiPause->GetState()) pHisto->setPause();
  else pHisto->setPause(kFALSE);

  if(pHisto->getDim() == 1) {

    xfrom = atof(fHiFrom1->GetText());
    xto   = atof(fHiTo1->GetText());
    xchan = atoi(fHiChan1->GetText());

    if(!gOnline->getMemCheck()->checkVirtual(1,xchan,0)) {
      HError::message("Memory limit will be exceeded.");
      delete pHisto;
      pHisto = 0;
      return 0;
    }

    if((xto > xfrom) && (xchan > 1)) {
      hist1 = new TH1F(fHiName->GetText(),"online",xchan,xfrom,xto);
      if(hist1) pHisto->setHist1(hist1);
    }
  }

  if(pHisto->getDim() == 2) {

    if(pHisto->isMulti() && 
       (pHisto->getCType() == 1 || pHisto->getCType() == 0)) {//multi histogram

      pSet = gOnline->getDetSet()->at(pHisto->getDSIdx(1));
      if(pSet) {
	pSet->calcMinMax();
	xfrom = atof(fHiFrom1->GetText());
	xto   = atof(fHiTo1->GetText());
	xchan = atoi(fHiChan1->GetText());
	yfrom = (Float_t) (pSet->getMinCell());
	yto   = (Float_t) (pSet->getMaxCell());
	ychan = pSet->getMaxCell() - pSet->getMinCell() + 1;

	if(!gOnline->getMemCheck()->checkVirtual(2,xchan,ychan)) {
	  HError::message("Memory limit will be exceeded.");
	  delete pHisto;
	  pHisto = 0;
	  return 0;
	}

	if((xto>xfrom) && (yto>yfrom) && (xchan>1) && (ychan>1)) {
	  hist2 = new TH2F(fHiName->GetText(),"multi",xchan,xfrom,xto,
			   ychan,yfrom,yto);
	  if(hist2) pHisto->setHist2(hist2);
	}
      }
    }
  
    if(!pHisto->isMulti()) {

      xfrom = atof(fHiFrom1->GetText());
      xto   = atof(fHiTo1->GetText());
      xchan = atoi(fHiChan1->GetText());
      yfrom = atof(fHiFrom2->GetText());
      yto   = atof(fHiTo2->GetText());
      ychan = atoi(fHiChan2->GetText());

      if(!gOnline->getMemCheck()->checkVirtual(2,xchan,ychan)) {
	HError::message("Memory limit will be exceeded.");
	delete pHisto;
	pHisto = 0;
	return 0;
      }
      
      if((xto>xfrom) && (yto>yfrom) && (xchan>1) && (ychan>1)) {
	hist2 = new TH2F(fHiName->GetText(),"correl",xchan,xfrom,xto,
			 ychan,yfrom,yto);
	if(hist2) pHisto->setHist2(hist2);
      }
    }
  }

  pHisto->SetName(fHiName->GetText());

  if(pHisto->checkConsistency(kTRUE)) return pHisto;
  //  pHisto->print();
  if(pHisto) { delete pHisto; pHisto = 0; }
  return 0;

}


//---------------------------------------------------------------------
//******  HLocalCondition* HOnlObjBrowser::createLC()
//
// This function scans all widgets of the local conditions subpanel, creates an
// object of the class HLocalCondition and checks its consistency. 
// If the object is consistent returns it. If not returns 0.
//
//---------------------------------------------------------------------

HLocalCondition* HOnlObjBrowser::createLC() {
//
// This function scans all widgets of the local conditions subpanel, creates an
// object of the class HLocalCondition and checks its consistency. 
// If the object is consistent returns it. If not, returns 0.
//

  HLocalCondition *pLC;
  Int_t categ;
  Int_t idx = -1;
  TGTextLBEntry *pEntry1 = 0;
  TGTextLBEntry *pEntry2 = 0;
  HDetSet *pSet = 0;
  HUserParameter *pPar1 = 0;
  HUserParameter *pPar2 = 0;

  if(!gOnline) return 0;

  pLC = new HLocalCondition();
  if(!pLC) return 0;

  if(!pLC->init()) {
    delete pLC;
    return 0;
  }

  if(fLCAlwTrue->GetState()) pLC->setAlwaysTrue();
  else pLC->setAlwaysTrue(kFALSE);

  pEntry1 = (TGTextLBEntry*) fLCDSCB->GetSelectedEntry();
  if(pEntry1) {
    pLC->setDetSet(pEntry1->GetText()->GetString());
    pSet = gOnline->getDetSet()->at(gOnline->getDetSet()->
				    find(pEntry1->GetText()->GetString()));
  }

  if(pSet) {
    categ = pSet->getCategory();
    pLC->setDetSet(pSet->GetName());
  }
  else categ = -1;

  if(categ == -1) {
    HError::message("Data category is -1. Probably det set not defined.");
    delete pLC;
    return 0;
  }

  if(!expStringL.isClear()) {

    expStringL.check();
    if(!expStringL.isValid()) {
      delete pLC;
      HError::message("Invalid expression");
      return 0;
    }

    HExpString *strng = new HExpString();
    if(!strng) {
      delete pLC;
      return 0;
    }
    expStringL.copy(*strng);

    pLC->setExpString(strng);

    if(!pLC->setExpression(categ)) {
      delete pLC; 
      return 0;
    }
  }


  if(fLCContourCB->GetSelected() > 1) {

    pEntry1 = (TGTextLBEntry*) fLCContourCB->GetSelectedEntry();
    if(!pEntry1) { 
      delete pLC;
      return 0;
    }

    idx = gOnline->getContour()->find(pEntry1->GetText()->GetString());
    if(idx < 0) {
      HError::message("Contour not found in the container");
      delete pLC;
      return 0;
    }

    HContour* pLCCont = new HContour();
    if(!pLCCont) {
      delete pLC;
      return 0;
    }
    
    gOnline->getContour()->at(idx)->copy(pLCCont);
    pLC->setContour(pLCCont);

    pPar1 = new HUserParameter();
    if(!pPar1) {
      delete pLC;
      return 0;
    }
    pPar2 = new HUserParameter();
    if(!pPar2) {
      if(pPar1) delete pPar1;
      delete pLC;
      return 0;
    }

    pEntry1 = (TGTextLBEntry*) fLCXparBox->GetSelectedEntry();
    if(!pEntry1) {
      HError::message("First parameter for the contour not defined");
      delete pLC;
      return 0;
    }

    idx = fDB->find(pEntry1->GetText()->GetString());
    if(!fDB->at(idx)) {
      HError::message("First parameter not found in the data base");
      delete pLC;
      delete pPar1;
      delete pPar2;
      return 0;
    }
      
    fDB->at(idx)->copy(pPar1);

    pEntry2 = (TGTextLBEntry*) fLCYparBox->GetSelectedEntry();
    if(!pEntry2) {
      HError::message("Second parameter for the contour not defined");
      delete pLC;
      return 0;
    }

    idx = fDB->find(pEntry2->GetText()->GetString());
    if(!fDB->at(idx)) {
      HError::message("Second parameter not found in the data base");
      delete pLC;
      delete pPar1;
      delete pPar2;
      return 0;
    }

    fDB->at(idx)->copy(pPar2);

    pLC->setContList(pPar1,pPar2);

  }

  pLC->SetName(fLCNameBuf->GetString());

  if(pLC->checkConsistency(kTRUE)) return pLC;

  if(pLC) { delete pLC; pLC = 0; }

  return 0;

}


//---------------------------------------------------------------------
//******  HGlobalCondition* HOnlObjBrowser::createGC()
//
// This function scans all widgets of the global conditions subpanel, 
// creates an object of the class HGlobalCondition and checks its consistency. 
// If the object is consistent returns it. If not, returns 0.
//
//---------------------------------------------------------------------

HGlobalCondition* HOnlObjBrowser::createGC() {
//
// This function scans all widgets of the global conditions subpanel, 
// creates an object of the class HGlobalCondition and checks its consistency. 
// If the object is consistent returns it. If not, returns 0.
//

  HGlobalCondition *pGC = new HGlobalCondition();
  if(!pGC) return 0;

  TGTextLBEntry *pEntry = 0;

  Int_t idx = -1;

  HUserParameter *pPar1 = 0;
  HUserParameter *pPar2 = 0;

  if(!expStringG.isClear()) {

    expStringG.check();
    if(!expStringG.isValid()) {
      delete pGC;
      HError::message("Invalid expression.");
      return 0;
    }

    HExpString *strng = new HExpString();
    if(!strng) {
      delete pGC;
      return 0;
    }
    expStringG.copy(*strng);

    pGC->setExpString(strng);
    if(!pGC->setExpression()) {
      delete pGC; 
      return 0;
    }
  }


  if(fGCContourCB->GetSelected() > 1) {

    pEntry = (TGTextLBEntry*) fGCContourCB->GetSelectedEntry();
    if(!pEntry) { 
      delete pGC;
      return 0;
    }

    idx = gOnline->getContour()->find(pEntry->GetText()->GetString());
    if(idx < 0) {
      HError::message("Contour not found in the container");
      delete pGC;
      return 0;
    }

    HContour* pGCCont = new HContour();
    if(!pGCCont) {
      delete pGC;
      return 0;
    }
    
    gOnline->getContour()->at(idx)->copy(pGCCont);
    pGC->setContour(pGCCont);

    pPar1 = new HUserParameter();
    if(!pPar1) {
      delete pGC;
      return 0;
    }
    pPar2 = new HUserParameter();
    if(!pPar2) {
      if(pPar1) delete pPar1;
      delete pGC;
      return 0;
    }

    pEntry = (TGTextLBEntry*) fGCDS1CB->GetSelectedEntry();
    if(pEntry) {

      pGC->setDSName(pEntry->GetText()->GetString(),1);
      
      pEntry = (TGTextLBEntry*) fGCDS2CB->GetSelectedEntry();
      if(pEntry) {

	pGC->setDSName(pEntry->GetText()->GetString(),2);
	
	pEntry = (TGTextLBEntry*) fGCXparBox->GetSelectedEntry();
	if(pEntry) {

	  idx = fDB->find(pEntry->GetText()->GetString());
	  if(!fDB->at(idx)) {
	    delete pGC;
	    delete pPar1;
	    delete pPar2;
	    return 0;
	  }

	  fDB->at(idx)->copy(pPar1);
	  
	  pEntry = (TGTextLBEntry*) fGCYparBox->GetSelectedEntry();
	  if(pEntry) {
	    
	    idx = fDB->find(pEntry->GetText()->GetString());
	    if(!fDB->at(idx)) {
	      delete pGC;
	      delete pPar1;
	      delete pPar2;
	      return 0;
	    }

	    fDB->at(idx)->copy(pPar2);
	  }
	}
      }
    }

    pGC->setContList(pPar1,pPar2);
  }
  
  pGC->SetName(fGCNameBuf->GetString());


  if(pGC->checkConsistency(kTRUE)) return pGC;

  if(pGC) { delete pGC; pGC = 0; }

  return 0;

}
  


//---------------------------------------------------------------------
//******  Bool_t HOnlObjBrowser::changeHisto(Int_t mode = 0)
//
// This function is called when Add, Modify or Remove button is clicked.
// mode == 0  - add a new histrogram
// mode == 1  - modify existing histogram
// mode == 2  - delete existing histogram
//
//---------------------------------------------------------------------

Bool_t HOnlObjBrowser::changeHisto(Int_t mode) {
//
// This function is called when Add, Modify or Remove button is clicked.
// mode == 0  - add a new histrogram
// mode == 1  - modify existing histogram
// mode == 2  - delete existing histogram
//

  if(!gOnline) return kFALSE;
  TGTextLBEntry *pEntry = 0;
  Int_t idx = -1;

  if(mode==2) { // delete existing histogram

    pEntry = (TGTextLBEntry*) fObjList->GetSelectedEntry();
    if(!pEntry) return kFALSE;
    idx = gOnline->getHisto()->find(pEntry->GetText()->GetString());
    gOnline->getHisto()->removeAt(idx);
    gOnline->getMemCheck()->check();
    return kTRUE;
  }

  HHistogram *pHisto = createHisto();
  HHistogram *pExistingHisto = 0;
  TH1F *h1 = 0;
  TH2F *h2 = 0;

  if(!pHisto) {
    return kFALSE;
  }

  if(mode==0 && !gOnline->getMemCheck()->isAlarm()) { // add histogram

    idx = gOnline->getHisto()->find(pHisto->GetName());
    if(idx != -1) {
      HError::message("Histogram with this name already exists.");
      delete pHisto;
      pHisto = 0;
      return kFALSE;
    }
    if(!gOnline->getHisto()->add(pHisto)) {
      delete pHisto;
      pHisto = 0;
      return kFALSE;
    }
    gOnline->getMemCheck()->check();
  }

  if(mode==1) { // modify existing histogram

    pEntry = (TGTextLBEntry*) fObjList->GetSelectedEntry();
    if(!pEntry) {
      delete pHisto;
      pHisto = 0;
      return kFALSE;
    }

    Int_t  index = gOnline->getHisto()->find(pEntry->GetText()->GetString());
    if(index == -1) {
      delete pHisto;
      pHisto = 0;
      return kFALSE;
    }

    idx = gOnline->getHisto()->find(pHisto->GetName());

    if(idx != -1 && idx != index) {
      HError::message("Histogram with this name already exists.");
      delete pHisto;
      pHisto = 0;
      return kFALSE;
    }

    if(!strcmp(pHisto->GetName(),pEntry->GetText()->GetString())) {
      idx = gOnline->getHisto()->find(pEntry->GetText()->GetString());
      pExistingHisto = gOnline->getHisto()->at(idx);
      if(pExistingHisto) { // check if the histogram itself is the same, then
	                   // do not change it

	if(pHisto->getDim() == pExistingHisto->getDim()) {

	  if(pHisto->getDim() == 1) {

	    if((pHisto->getHist1()->GetXaxis()->GetXmin() == 
		pExistingHisto->getHist1()->GetXaxis()->GetXmin()) &&
	       (pHisto->getHist1()->GetXaxis()->GetXmax() ==
		pExistingHisto->getHist1()->GetXaxis()->GetXmax()) &&
	       (pHisto->getHist1()->GetXaxis()->GetNbins() ==
		pExistingHisto->getHist1()->GetXaxis()->GetNbins())) {

	      h1 = pHisto->getHist1();
	      if(h1) { delete h1; h1 = 0; }
	      h1 = (TH1F*) pExistingHisto->getHist1()->Clone();
	      pHisto->setHist1(h1);
	    }
	  }

	  if(pHisto->getDim() == 2) {

	    if((pHisto->getHist2()->GetXaxis()->GetXmin() == 
		pExistingHisto->getHist2()->GetXaxis()->GetXmin()) &&
	       (pHisto->getHist2()->GetXaxis()->GetXmax() ==
		pExistingHisto->getHist2()->GetXaxis()->GetXmax()) &&
	       (pHisto->getHist2()->GetXaxis()->GetNbins() ==
		pExistingHisto->getHist2()->GetXaxis()->GetNbins()) && 
	       (pHisto->getHist2()->GetYaxis()->GetXmin() == 
		pExistingHisto->getHist2()->GetYaxis()->GetXmin()) &&
	       (pHisto->getHist2()->GetYaxis()->GetXmax() ==
		pExistingHisto->getHist2()->GetYaxis()->GetXmax()) &&
	       (pHisto->getHist2()->GetYaxis()->GetNbins() ==
		pExistingHisto->getHist2()->GetYaxis()->GetNbins())) { 

	      h2 = pHisto->getHist2();
	      if(h2) { delete h2; h2 = 0; }
	      h2 = (TH2F*) pExistingHisto->getHist2()->Clone();
	      pHisto->setHist2(h2);
	    }
	  }
	}
      }
      gOnline->getHisto()->removeAt(idx);
      if(!gOnline->getHisto()->addAt(pHisto,idx)) {
	delete pHisto;
	pHisto = 0;
	return kFALSE;
      }
    }
    else {
      idx = gOnline->getHisto()->find(pHisto->GetName());
      if(idx != -1) {
	delete pHisto;
	pHisto = 0;
	return kFALSE;
      }
      idx = gOnline->getHisto()->find(pEntry->GetText()->GetString());
      pExistingHisto = gOnline->getHisto()->at(idx);
      if(pExistingHisto) { // check if the histogram itself is the same, then
	                   // do not change it

	if(pHisto->getDim() == pExistingHisto->getDim()) {

	  if(pHisto->getDim() == 1) {

	    if((pHisto->getHist1()->GetXaxis()->GetXmin() == 
		pExistingHisto->getHist1()->GetXaxis()->GetXmin()) &&
	       (pHisto->getHist1()->GetXaxis()->GetXmax() ==
		pExistingHisto->getHist1()->GetXaxis()->GetXmax()) &&
	       (pHisto->getHist1()->GetXaxis()->GetNbins() ==
		pExistingHisto->getHist1()->GetXaxis()->GetNbins())) {

	      h1 = pHisto->getHist1();
	      if(h1) { delete h1; h1 = 0; }
	      h1 = (TH1F*) pExistingHisto->getHist1()->Clone();
	      h1->SetName(pHisto->GetName());
	      pHisto->setHist1(h1);
	    }
	  }

	  if(pHisto->getDim() == 2) {

	    if((pHisto->getHist2()->GetXaxis()->GetXmin() == 
		pExistingHisto->getHist2()->GetXaxis()->GetXmin()) &&
	       (pHisto->getHist2()->GetXaxis()->GetXmax() ==
		pExistingHisto->getHist2()->GetXaxis()->GetXmax()) &&
	       (pHisto->getHist2()->GetXaxis()->GetNbins() ==
		pExistingHisto->getHist2()->GetXaxis()->GetNbins()) && 
	       (pHisto->getHist2()->GetYaxis()->GetXmin() == 
		pExistingHisto->getHist2()->GetYaxis()->GetXmin()) &&
	       (pHisto->getHist2()->GetYaxis()->GetXmax() ==
		pExistingHisto->getHist2()->GetYaxis()->GetXmax()) &&
	       (pHisto->getHist2()->GetYaxis()->GetNbins() ==
		pExistingHisto->getHist2()->GetYaxis()->GetNbins())) { 

	      h2 = pHisto->getHist2();
	      if(h2) { delete h2; h2 = 0; }
	      h2 = (TH2F*) pExistingHisto->getHist2()->Clone();
	      h2->SetName(pHisto->GetName());
	      pHisto->setHist2(h2);
	    }
	  }
	}
      }
      gOnline->getHisto()->removeAt(idx);
      if(!gOnline->getHisto()->addAt(pHisto,idx)) {
	delete pHisto;
	pHisto = 0;
	return kFALSE;
      }
    }
  }

  gOnline->getMemCheck()->check();

  return kTRUE;
}


//---------------------------------------------------------------------
//******  Bool_t HOnlObjBrowser::changeLC(Int_t mode = 0, HLocalCondition **p,
//                                        Int_t *id)
//
// This function is called when Add, Modify or Remove button is clicked.
// mode == 0  - add a new local condition
// mode == 1  - modify existing local condition
// mode == 2  - virtually delete existing local condition and return object's
//              address in p 
//
//---------------------------------------------------------------------

Bool_t HOnlObjBrowser::changeLC(Int_t mode, HLocalCondition **p, Int_t *id) {
//
// This function is called when Add, Modify or Remove button is clicked.
// mode == 0  - add a new local condition
// mode == 1  - modify existing local condition
// mode == 2  - virtually delete existing local condition and return object's
//              address in p 
//

  if(!gOnline) return kFALSE;
  TGTextLBEntry *pEntry = 0;
  Int_t idx = -1;

  if(id) *id = -1;
  if(mode==2) { // delete existing local condition

    pEntry = (TGTextLBEntry*) fObjList->GetSelectedEntry();
    if(!pEntry) return kFALSE;
    idx = gOnline->getLocal()->find(pEntry->GetText()->GetString());
    if(idx == -1) return kFALSE;
    if(id) *id = idx;
    if(p) (*p) = gOnline->getLocal()->at(idx);
    gOnline->getLocal()->removeAt(idx,kFALSE);
    gOnline->getMemCheck()->check();
    return kTRUE;
  }

  HLocalCondition *pLC = createLC();

  if(!pLC) {
    return kFALSE;
  }

  if(mode == 1) { // modify existing local condition

    pEntry = (TGTextLBEntry*) fObjList->GetSelectedEntry();
    if(!pEntry) {
      delete pLC;
      pLC = 0;
      return kFALSE;
    }

    Int_t  index = gOnline->getLocal()->find(pEntry->GetText()->GetString());
    if(index == -1) {
      delete pLC;
      pLC = 0;
      return kFALSE;
    }

    idx = gOnline->getLocal()->find(pLC->GetName());

    if(idx != -1 && idx != index) {
      HError::message("Local condition with this name already exists.");
      delete pLC;
      pLC = 0;
      return kFALSE;
    }

    gOnline->getLocal()->removeAt(index);
    gOnline->getLocal()->addAt(pLC,index);
  }

  if(mode == 0 && !gOnline->getMemCheck()->isAlarm()) {
    idx = gOnline->getLocal()->find(pLC->GetName());
    if(idx != -1) {
      HError::message("Local condition with this name already exists.");
      delete pLC;
      pLC = 0;
      return kFALSE;
    }
    gOnline->getLocal()->add(pLC);
  }

  //  gOnline->getMemCheck()->check();
  return kTRUE;

}


//---------------------------------------------------------------------
//******  Bool_t HOnlObjBrowser::changeGC(Int_t mode = 0, HGlobalCondition **p,
//                                        Int_t *id)
//
// This function is called when Add, Modify or Remove button is clicked.
// mode == 0  - add a new global condition
// mode == 1  - modify existing global condition
// mode == 2  - virtually delete existing global condition and return object's
//              address in p 
//
//---------------------------------------------------------------------

Bool_t HOnlObjBrowser::changeGC(Int_t mode, HGlobalCondition **p, Int_t *id) {
//
// This function is called when Add, Modify or Remove button is clicked.
// mode == 0  - add a new global condition
// mode == 1  - modify existing global condition
// mode == 2  - virtually delete existing global condition and return object's
//              address in p 
//

  if(!gOnline) return kFALSE;
  TGTextLBEntry *pEntry = 0;
  Int_t idx = -1;

  if(id) *id = -1;
  if(mode==2) { // delete existing global condition

    pEntry = (TGTextLBEntry*) fObjList->GetSelectedEntry();
    if(!pEntry) return kFALSE;
    idx = gOnline->getGlobal()->find(pEntry->GetText()->GetString());
    if(idx == -1) return kFALSE;
    if(id) *id = idx;
    if(p) (*p) = gOnline->getGlobal()->at(idx);
    gOnline->getGlobal()->removeAt(idx,kFALSE);
    gOnline->getMemCheck()->check();
    return kTRUE;
  }

  HGlobalCondition *pGC = createGC();

  if(!pGC) {
    return kFALSE;
  }

  if(mode == 1) { // modify existing global condition

    pEntry = (TGTextLBEntry*) fObjList->GetSelectedEntry();
    if(!pEntry) {
      delete pGC;
      pGC = 0;
      return kFALSE;
    }

    Int_t  index = gOnline->getGlobal()->find(pEntry->GetText()->GetString());
    if(index == -1) {
      delete pGC;
      pGC = 0;
      return kFALSE;
    }

    idx = gOnline->getGlobal()->find(pGC->GetName());

    if(idx != -1 && idx != index) {
      HError::message("Global condition with this name already exists.");
      delete pGC;
      pGC = 0;
      return kFALSE;
    }

    gOnline->getGlobal()->removeAt(index);
    gOnline->getGlobal()->addAt(pGC,index);
  }

  if(mode == 0 && !gOnline->getMemCheck()->isAlarm()) {
    idx = gOnline->getGlobal()->find(pGC->GetName());
    if(idx != -1) {
      HError::message("Global condition with this name already exists.");
      delete pGC;
      pGC = 0;
      return kFALSE;
    }
    gOnline->getGlobal()->add(pGC);
  }

  return kTRUE;

}



//---------------------------------------------------------------------
//******  void HOnlObjBrowser::clearContFields(Int_t ctype)
//
// This function clears all widgets in the contour subpanel.
// ctype == 1 - local condition
// ctype == 2 - global condition
//
//---------------------------------------------------------------------

void HOnlObjBrowser::clearContFields(Int_t ctype) {
//
// This function clears all widgets in the contour subpanel.
// ctype == 1 - local condition
// ctype == 2 - global condition
//

  if((ctype != 1) && (ctype != 2)) return;

  if(ctype == 1) { // local condition

    clearCB(fLCXparBox);
    clearCB(fLCYparBox);
  }

  if(ctype == 2) {

    clearCB(fGCDS1CB);
    clearCB(fGCDS2CB);
    clearCB(fGCXparBox);
    clearCB(fGCYparBox);
  }
  
}


//---------------------------------------------------------------------
//******  void HOnlObjBrowser::clearHistoFields()
//
// This function clears all widgets in the histogram subpanel.
// Usually called after REMOVE button is clicked.
//
//---------------------------------------------------------------------

void HOnlObjBrowser::clearHistoFields() {
//
// This function clears all widgets in the histogram subpanel.
// Usually called after REMOVE button is clicked.
//

  fHi1dRB->SetState(kButtonUp);
  fHi2dRB->SetState(kButtonUp);
  fHiMulRB->SetState(kButtonUp);
  fHiCorRB->SetState(kButtonUp);
  fHiNoCRB->SetState(kButtonUp);
  fHiCRB->SetState(kButtonUp);
  fHiLocalRB->SetState(kButtonUp);
  fHiGlobalRB->SetState(kButtonUp);
  clearCB(fHiConCB);
  clearCB(fHiDS1CB);
  clearCB(fHiDS2CB);
  clearCB(fHiPar1CB);
  clearCB(fHiPar2CB);
  fHiFrom1Buf->Clear();
  fHiTo1Buf->Clear();
  fHiChan1Buf->Clear();
  fHiFrom2Buf->Clear();
  fHiTo2Buf->Clear();
  fHiChan2Buf->Clear();
  fHiNameBuf->Clear();
  fClient->NeedRedraw(fHiFrom1);
  fClient->NeedRedraw(fHiTo1);
  fClient->NeedRedraw(fHiChan1);
  fClient->NeedRedraw(fHiFrom2);
  fClient->NeedRedraw(fHiTo2);
  fClient->NeedRedraw(fHiChan2);
  fClient->NeedRedraw(fHiName);
  fHiFrom1->SetState(kFALSE);
  fHiTo1->SetState(kFALSE);
  fHiChan1->SetState(kFALSE);
  fHiFrom2->SetState(kFALSE);
  fHiTo2->SetState(kFALSE);
  fHiChan2->SetState(kFALSE);
  fHiPause->SetState(kButtonUp);
  fHiCanv->GetCanvas()->cd();
  fHiCanv->GetCanvas()->Clear();
  fHiCanv->GetCanvas()->Modified();
  fHiCanv->GetCanvas()->Update();
  /*  fHiCanvProj->GetCanvas()->cd();
  fHiCanvProj->GetCanvas()->Clear();
  fHiCanvProj->GetCanvas()->Modified();
  fHiCanvProj->GetCanvas()->Update();*/
}


//---------------------------------------------------------------------
//******  Int_t HOnlObjBrowser::calcCateg()
//
// This function extracts the detector set indicated in fLCDSCB and calculates
// the category.
//
//---------------------------------------------------------------------

Int_t HOnlObjBrowser::calcCateg() {
//
// This function extracts the detector set indicated in fLCDSCB and calculates
// the category.
//

  Int_t categ = -1;
  TGTextLBEntry *pEntry = (TGTextLBEntry*) fLCDSCB->GetSelectedEntry();
  HDetSet *pSet = 0;
  if(pEntry) {
    pSet = gOnline->getDetSet()->
      at(gOnline->getDetSet()->find(pEntry->GetText()->GetString()));
    if(pSet) categ = pSet->getCategory();
  }

  return categ;
}
  


//---------------------------------------------------------------------
//******  void HOnlObjBrowser::createCountsList()
//
// This function creates the list fGCListPar.
//
//---------------------------------------------------------------------

void HOnlObjBrowser::createCountsList() {
//
// This function creates the list fGCListPar.
//

  if(!gOnline) return;
  HLocalCondition *pLocal = 0;
  Int_t j = 1;
  char entry[50] = "";

  clearLB(fGCListPar);

  for(Int_t i=0; i<gOnline->getLocal()->getEntries(); i++) {

    pLocal = gOnline->getLocal()->at(i);
    if(pLocal) {

      if(pLocal->isConsistent()) {

	sprintf(entry,"%s%i%s%s%s","[",i*3,"] - ",pLocal->GetName(),"_true");
	fGCListPar->AddEntry(entry,j++);
	sprintf(entry,"%s%i%s%s%s","[",i*3+1,"] - ",pLocal->GetName(),"_false");
	fGCListPar->AddEntry(entry,j++);
	sprintf(entry,"%s%i%s%s%s","[",i*3+2,"] - ",pLocal->GetName(),"_sum");
	fGCListPar->AddEntry(entry,j++);

      }
    }
  }

  fGCListPar->MapSubwindows();
  fGCListPar->Layout();

}


//---------------------------------------------------------------------
//******  Int_t HOnlObjBrowser::extractCountIdx(const char *name)
//
// This is a service function for the fGCListPar.
//
//---------------------------------------------------------------------

Int_t HOnlObjBrowser::extractCountIdx(const char *name) {
//
// This is a service function for the fGCListPar.
//

  if(!name) return -1;
  while(!isdigit(*name)) ++name;

  return (atoi(name));

}

     
//---------------------------------------------------------------------
//******  Bool_t HOnlObjBrowser::ProcessMessage(Long_t msg, Long_t parm1,
//                                              Long_t parm2)
//
// This function processes all GUI items.
//
//---------------------------------------------------------------------

Bool_t HOnlObjBrowser::ProcessMessage(Long_t msg,Long_t parm1,Long_t parm2) {
//
// This function processes all GUI widgets.
//

if(!gOnline) return kFALSE;
Int_t id = 0;
TGTextLBEntry *pEntry = 0;
HUserParameter *pUP = 0;
HDetSet *pSet = 0;
HLocalCondition *pLocal = 0;
HGlobalCondition *pGlobal = 0;
Int_t categ = -1;
char tmpname[50] = "";
char nulstr[50] = "";
Int_t idx = -1;

switch(GET_MSG(msg)) {

 case kC_COMMAND:

  switch(GET_SUBMSG(msg)) {

  case kCM_BUTTON:

    switch(parm1) {

    case M_ADD:
      if(calcTabsIdx() > 0 && calcTabsIdx() < 7) changeDetSet(calcTabsIdx());
      if(calcTabsIdx() == 7) changeHisto();
      if(calcTabsIdx() == 8) changeLC();
      if(calcTabsIdx() == 9) changeGC();
      fDB->changed(calcTabsIdx());
      displayObjList();
      if(gOnline) gOnline->getMemCheck()->check();
      break;

    case M_MODIFY:
      if(calcTabsIdx() > 0 && calcTabsIdx() < 7) changeDetSet(calcTabsIdx(),1);
      if(calcTabsIdx() == 7) changeHisto(1);
      if(calcTabsIdx() == 8) changeLC(1);
      if(calcTabsIdx() == 9) changeGC(1);
      fDB->changed(calcTabsIdx());
      displayObjList();
      if(gOnline) gOnline->getMemCheck()->check();
      if(gOnline) gOnline->getConsistency()->action();
      break;

    case M_REMOVE:
      if(calcTabsIdx() > 0 && calcTabsIdx() < 7) {
	changeDetSet(calcTabsIdx(),2,&pSet,&idx);
	gOnline->getConsistency()->action();
	if(gOnline->getConsistency()->isConsistent()) {
	  if(pSet) { delete pSet; pSet = 0; }
	}
	else {
	  gOnline->getDetSet()->addAt(pSet,idx);
	  gOnline->getConsistency()->action();
	}
      }
      if(calcTabsIdx() == 7) changeHisto(2);

      if(calcTabsIdx() == 8) {
	changeLC(2,&pLocal,&idx);
	gOnline->getConsistency()->action();
	if(gOnline->getConsistency()->isConsistent()) {
	  if(pLocal) { delete pLocal; pLocal = 0; }
	}
	else {
	  gOnline->getLocal()->addAt(pLocal,idx);
	  gOnline->getConsistency()->action();
	}
      }
      if(calcTabsIdx() == 9) {
	changeGC(2,&pGlobal,&idx);
	gOnline->getConsistency()->action();
	if(gOnline->getConsistency()->isConsistent()) {
	  if(pGlobal) { delete pGlobal; pGlobal = 0; }
	}
	else {
	  gOnline->getGlobal()->addAt(pGlobal,idx);
	  gOnline->getConsistency()->action();
	}
      }
      fDB->changed(calcTabsIdx());
      displayObjList();
      if(calcTabsIdx() == 7) clearHistoFields();
      if(gOnline) gOnline->getMemCheck()->check();
      break;

    case M_NEXT:
      id = fObjList->GetSelected();
      if(id < 0) id = 0;
      pEntry = (TGTextLBEntry*) fObjList->Select(++id,kFALSE);
      if(pEntry) {
	fObjList->Select(id,kTRUE);
	SendMessage(this,MK_MSG(kC_COMMAND,kCM_LISTBOX),M_LIST,id);
      }
      else {
	if(calcTabsIdx() == 7) {
	  fHiCanv->GetCanvas()->Clear();
	  //	  fHiCanvProj->GetCanvas()->Clear();
	  fHiCanv->GetCanvas()->Modified();
	  //	  fHiCanvProj->GetCanvas()->Modified();
	  fHiCanv->GetCanvas()->Update();
	  //	  fHiCanvProj->GetCanvas()->Update();
	}
      }
      break;

    case M_PREV:
      id = fObjList->GetSelected();
      if(id < 0) id = 0;
      pEntry = (TGTextLBEntry*) fObjList->Select(--id,kFALSE);
      if(pEntry) {
	fObjList->Select(id,kTRUE);
	SendMessage(this,MK_MSG(kC_COMMAND,kCM_LISTBOX),M_LIST,id);
      }
      else {
	if(calcTabsIdx() == 7) {
	  fHiCanv->GetCanvas()->Clear();
	  //	  fHiCanvProj->GetCanvas()->Clear();
	  fHiCanv->GetCanvas()->Modified();
	  //	  fHiCanvProj->GetCanvas()->Modified();
	  fHiCanv->GetCanvas()->Update();
	  //	  fHiCanvProj->GetCanvas()->Update();
	}
      }
      break;

    case M__LC_UNDOEXP:
      fLCParLB->SetMultipleSelections(kFALSE);
      expStringL.undo();
      expStringL.updateList(fLCExpList);
      break;

    case M__GC_UNDOEXP:
      fGCListPar->SetMultipleSelections(kFALSE);
      expStringG.undo();
      expStringG.updateList(fGCExpList);
      break;

    case M__LC_CLEAREXP:
      fLCParLB->SetMultipleSelections(kFALSE);
      expStringL.clear();
      expStringL.updateList(fLCExpList);
      break;

    case M__GC_CLEAREXP:
      fGCListPar->SetMultipleSelections(kFALSE);
      expStringG.clear();
      expStringG.updateList(fGCExpList);
      break;

    case M__LC_NEWPAREXP:
      fLCParLB->SetMultipleSelections(kFALSE);
      expStringL.checkParam();
      if(!expStringL.isValid()) {
	HError::message("Invalid expression.");
	break;
      }
      pEntry = (TGTextLBEntry*) fLCDSCB->GetSelectedEntry();
      if(!pEntry) break;
      pSet = gOnline->getDetSet()->at(gOnline->getDetSet()->
				      find(pEntry->GetText()->GetString()));
      categ = calcCateg();
      if(categ != -1) {
	
	pUP = new HUserParameter();
	if(!pUP) break;
	pUP->setCategory(categ);
	pUP->setIndex(-1);
	pUP->setExpression(&expStringL);
	pUP->SetName(fLCNParNameBuf->GetString());
	fDB->add(pUP);
	delete pUP;
	pUP = 0;
      }
      createParList(pSet);
      break;

    case M__MD_DOWN:
      id = fMdcModList->GetSelected();
      if(id < 0) id = 0;
      pEntry = (TGTextLBEntry*) fMdcModList->Select(++id,kFALSE);
      if(pEntry) {
	fMdcModList->Select(id,kTRUE);
	if(id > 6) fMdcModList->SetTopEntry(id);
	else fMdcModList->SetTopEntry(1);
	SendMessage(this,MK_MSG(kC_COMMAND,kCM_LISTBOX),M__MD_MODLIST,id);
      }
      break;

    case M__MD_UP:
      id = fMdcModList->GetSelected();
      if(id < 0) id = 0;
      pEntry = (TGTextLBEntry*) fMdcModList->Select(--id,kFALSE);
      if(pEntry) {
	fMdcModList->Select(id,kTRUE);
	if(id > 6) fMdcModList->SetTopEntry(id);
	else fMdcModList->SetTopEntry(1);
	SendMessage(this,MK_MSG(kC_COMMAND,kCM_LISTBOX),M__MD_MODLIST,id);
      }
      break;

    case M__RI_DOWN:
      id = fRichModList->GetSelected();
      if(id < 0) id = 0;
      pEntry = (TGTextLBEntry*) fRichModList->Select(++id,kFALSE);
      if(pEntry) {
	fRichModList->Select(id,kTRUE);
	if(id > 6) fRichModList->SetTopEntry(id);
	else fRichModList->SetTopEntry(1);
	SendMessage(this,MK_MSG(kC_COMMAND,kCM_LISTBOX),M__RI_MODLIST,id);
      }
      break;

    case M__RI_UP:
      id = fRichModList->GetSelected();
      if(id < 0) id = 0;
      pEntry = (TGTextLBEntry*) fRichModList->Select(--id,kFALSE);
      if(pEntry) {
	fRichModList->Select(id,kTRUE);
	if(id > 6) fRichModList->SetTopEntry(id);
	else fRichModList->SetTopEntry(1);
	SendMessage(this,MK_MSG(kC_COMMAND,kCM_LISTBOX),M__RI_MODLIST,id);
      }
      break;

    case M__SH_DOWN:
      id = fShModList->GetSelected();
      if(id < 0) id = 0;
      pEntry = (TGTextLBEntry*) fShModList->Select(++id,kFALSE);
      if(pEntry) {
	fShModList->Select(id,kTRUE);
	if(id > 6) fShModList->SetTopEntry(id);
	else fShModList->SetTopEntry(1);
	SendMessage(this,MK_MSG(kC_COMMAND,kCM_LISTBOX),M__SH_MODLIST,id);
      }
      break;

    case M__SH_UP:
      id = fShModList->GetSelected();
      if(id < 0) id = 0;
      pEntry = (TGTextLBEntry*) fShModList->Select(--id,kFALSE);
      if(pEntry) {
	fShModList->Select(id,kTRUE);
	if(id > 6) fShModList->SetTopEntry(id);
	else fShModList->SetTopEntry(1);
	SendMessage(this,MK_MSG(kC_COMMAND,kCM_LISTBOX),M__SH_MODLIST,id);
      }
      break;
      

    default:
      if(parm1 >= 0 && parm1 <= 35) {
	fLCParLB->SetMultipleSelections(kFALSE);
	expStringL.addItem(parm1);
	expStringL.updateList(fLCExpList);
      }
      if(parm1 >= 1000 && parm1 <= 1025) {
	fGCListPar->SetMultipleSelections(kFALSE);
	if(parm1 <= 1017) expStringG.addItem(parm1-1000);
	else expStringG.addItem(parm1-1000+10);
	expStringG.updateList(fGCExpList);
      }
      if(parm1>=500 && parm1<=547) {
	if(countTof == 2) { 
	  countTof = 0; 
	  idxiTof[0] = idxiTof[1] = idxjTof[0] = idxjTof[1] = -1;
	}
	countTof++;
	idxiTof[countTof-1] = (parm1-500)/8;
	idxjTof[countTof-1] = (parm1-500)%8;
	displayTofLay();
      }
      if(parm1 >= 600 && parm1 <= 623 && fMdcMultiRB->GetState()) {
	if(countMdcS1 == 2) { 
	  countMdcS1 = 0; 
	  idxiMdcS1[0] = idxiMdcS1[1] = idxjMdcS1[0] = idxjMdcS1[1] = -1;
	}
	countMdcS1++;
	idxiMdcS1[countMdcS1-1] = (parm1-600)/4;
	idxjMdcS1[countMdcS1-1] = (parm1-600)%4;
	controlMdcDSGui(fMdcSecModBut[0]);
      }
      if(parm1 >= 630 && parm1 <= 677 && fMdcMultiRB->GetState()) {
	if(countMdcS2 == 2) { 
	  countMdcS2 = 0; 
	  idxiMdcS2[0] = idxiMdcS2[1] = idxjMdcS2[0] = idxjMdcS2[1] = -1;
	}
	countMdcS2++;
	idxiMdcS2[countMdcS2-1] = (parm1-630)/8;
	idxjMdcS2[countMdcS2-1] = (parm1-630)%8;
	controlMdcDSGui(fMdcCellsBut[0]);
      }
      if(parm1 >= 700 && parm1 <= 705 && fRichMultiRB->GetState()) {
	if(countRichS1 == 2) { 
	  countRichS1 = 0; 
	  idxiRichS1[0] = idxiRichS1[1] = idxjRichS1[0] = idxjRichS1[1] = -1;
	}
	countRichS1++;
	idxiRichS1[countRichS1-1] = parm1-700;
	idxjRichS1[countRichS1-1] = 0;
	controlRichDSGui(fRichSecModBut[0]);
      }
      if(parm1 >= 730 && parm1 <= 793 && fRichMultiRB->GetState()) {
	if(countRichS2 == 2) { 
	  countRichS2 = 0; 
	  idxiRichS2[0] = idxiRichS2[1] = idxjRichS2[0] = idxjRichS2[1] = -1;
	}
	countRichS2++;
	idxiRichS2[countRichS2-1] = (parm1-730)/8;
	idxjRichS2[countRichS2-1] = (parm1-730)%8;
	controlRichDSGui(fRichCellsBut[0]);
      }
      if(parm1 >= 800 && parm1 <= 817 && fShMultiRB->GetState()) {
	if(countShS1 == 2) { 
	  countShS1 = 0; 
	  idxiShS1[0] = idxiShS1[1] = idxjShS1[0] = idxjShS1[1] = -1;
	}
	countShS1++;
	idxiShS1[countShS1-1] = (parm1-800)/3;
	idxjShS1[countShS1-1] = (parm1-800)%3;
	controlShDSGui(fShSecModBut[0]);
      }
      if(parm1 >= 830 && parm1 <= 893 && fShMultiRB->GetState()) {
	if(countShS2 == 2) { 
	  countShS2 = 0; 
	  idxiShS2[0] = idxiShS2[1] = idxjShS2[0] = idxjShS2[1] = -1;
	}
	countShS2++;
	idxiShS2[countShS2-1] = (parm1-830)/8;
	idxjShS2[countShS2-1] = (parm1-830)%8;
	controlShDSGui(fShCellsBut[0]);
      }

    }
    break;

  case kCM_RADIOBUTTON:

    switch(parm1) {

    case M__HI_1DIMRB:
      fHi1dRB->SetState(kButtonDown);
      controlHiGui(fHi1dRB);
      break;
    case M__HI_2DIMRB:
      fHi2dRB->SetState(kButtonDown);
      controlHiGui(fHi2dRB);
      break;
    case M__HI_MULRB:
      fHiMulRB->SetState(kButtonDown);
      controlHiGui(fHiMulRB);
      break;
    case M__HI_CORRB:
      fHiCorRB->SetState(kButtonDown);
      controlHiGui(fHiCorRB);
      break;
    case M__HI_NOCRB:
      fHiNoCRB->SetState(kButtonDown);
      controlHiGui(fHiNoCRB);
      break;
    case M__HI_CRB:
      fHiCRB->SetState(kButtonDown);
      controlHiGui(fHiCRB);
      break;
    case M__HI_LOCALRB:
      fHiLocalRB->SetState(kButtonDown);
      controlHiGui(fHiLocalRB);
      break;
    case M__HI_GLOBALRB:
      fHiGlobalRB->SetState(kButtonDown);
      controlHiGui(fHiGlobalRB);
      break;

    case M__TO_SINGLE:
      fTofSingleRB->SetState(kButtonDown);
      controlTofDSGui(fTofSingleRB);
      break;
    case M__TO_MULTI:
      fTofMultiRB->SetState(kButtonDown);
      controlTofDSGui(fTofMultiRB);
      break;
    case M__TO_SINLAY:
      fTofSinDefRB->SetState(kButtonDown);
      fTofRecDefRB->SetState(kButtonUp);
      break;
    case M__TO_RECLAY:
      fTofRecDefRB->SetState(kButtonDown);
      fTofSinDefRB->SetState(kButtonUp);
      break;
    case M__TO_ADDLAY:
      fTofAddDefRB->SetState(kButtonDown);
      fTofSubDefRB->SetState(kButtonUp);
      countTof = 2;
      break;
    case M__TO_SUBLAY:
      fTofSubDefRB->SetState(kButtonDown);
      fTofAddDefRB->SetState(kButtonUp);
      countTof = 2;
      break;

    case M__MD_SINGLE:
      fMdcSingleRB->SetState(kButtonDown);
      controlMdcDSGui(fMdcSingleRB);
      break;
    case M__MD_MULTI:
      fMdcMultiRB->SetState(kButtonDown);
      controlMdcDSGui(fMdcMultiRB);
      break;
    case M__MD_SINLAY:
      fMdcSinDefRB->SetState(kButtonDown);
      fMdcRecDefRB->SetState(kButtonUp);
      break;
    case M__MD_RECLAY:
      fMdcRecDefRB->SetState(kButtonDown);
      fMdcSinDefRB->SetState(kButtonUp);
      break;
    case M__MD_ADDLAY:
      fMdcAddDefRB->SetState(kButtonDown);
      fMdcSubDefRB->SetState(kButtonUp);
      countMdcS1 = 2;
      countMdcS2 = 2;
      break;
    case M__MD_SUBLAY:
      fMdcSubDefRB->SetState(kButtonDown);
      fMdcAddDefRB->SetState(kButtonUp);
      countMdcS1 = 2;
      countMdcS2 = 2;
      break;

    case M__RI_SINGLE:
      fRichSingleRB->SetState(kButtonDown);
      controlRichDSGui(fRichSingleRB);
      break;
    case M__RI_MULTI:
      fRichMultiRB->SetState(kButtonDown);
      controlRichDSGui(fRichMultiRB);
      break;
    case M__RI_SINLAY:
      fRichSinDefRB->SetState(kButtonDown);
      fRichRecDefRB->SetState(kButtonUp);
      break;
    case M__RI_RECLAY:
      fRichRecDefRB->SetState(kButtonDown);
      fRichSinDefRB->SetState(kButtonUp);
      break;
    case M__RI_ADDLAY:
      fRichAddDefRB->SetState(kButtonDown);
      fRichSubDefRB->SetState(kButtonUp);
      countRichS1 = 2;
      countRichS2 = 2;
      break;
    case M__RI_SUBLAY:
      fRichSubDefRB->SetState(kButtonDown);
      fRichAddDefRB->SetState(kButtonUp);
      countRichS1 = 2;
      countRichS2 = 2;
      break;

    case M__SH_SINGLE:
      fShSingleRB->SetState(kButtonDown);
      controlShDSGui(fShSingleRB);
      break;
    case M__SH_MULTI:
      fShMultiRB->SetState(kButtonDown);
      controlShDSGui(fShMultiRB);
      break;
    case M__SH_SINLAY:
      fShSinDefRB->SetState(kButtonDown);
      fShRecDefRB->SetState(kButtonUp);
      break;
    case M__SH_RECLAY:
      fShRecDefRB->SetState(kButtonDown);
      fShSinDefRB->SetState(kButtonUp);
      break;
    case M__SH_ADDLAY:
      fShAddDefRB->SetState(kButtonDown);
      fShSubDefRB->SetState(kButtonUp);
      countShS1 = 2;
      countShS2 = 2;
      break;
    case M__SH_SUBLAY:
      fShSubDefRB->SetState(kButtonDown);
      fShAddDefRB->SetState(kButtonUp);
      countShS1 = 2;
      countShS2 = 2;
      break;

    default:
      break;
    }
    break;

  case kCM_COMBOBOX:

    switch(parm1) {

    case M__LC_DSCB:
      pEntry = (TGTextLBEntry*) fLCDSCB->GetSelectedEntry();
      if(pEntry) {
	pSet = gOnline->getDetSet()->
	  at(gOnline->getDetSet()->find(pEntry->GetText()->GetString()));
      } 
      createParList(pSet);
      createParList(fLCXparBox,pSet);
      createParList(fLCYparBox,pSet);
      break;

    case M__GC_DS1CB:
      pEntry = (TGTextLBEntry*) fGCDS1CB->GetSelectedEntry();
      if(pEntry) {
	pSet = gOnline->getDetSet()->
	  at(gOnline->getDetSet()->find(pEntry->GetText()->GetString()));
      } 
      createParList(fGCXparBox,pSet);
      break;

    case M__GC_DS2CB:
      pEntry = (TGTextLBEntry*) fGCDS2CB->GetSelectedEntry();
      if(pEntry) {
	pSet = gOnline->getDetSet()->
	  at(gOnline->getDetSet()->find(pEntry->GetText()->GetString()));
      } 
      createParList(fGCYparBox,pSet);
      break;

    case M__LC_XPARCB:
      break;

    case M__LC_YPARCB:
      break;

    case M__HI_CCB:
      controlHiGui(fHiConCB);
      break;
    case M__HI_DS1CB:
      controlHiGui(fHiDS1CB);
      break;
    case M__HI_DS2CB:
      controlHiGui(fHiDS2CB);
      break;
    case M__HI_PAR1CB:
      controlHiGui(fHiPar1CB);
      break;
    case M__HI_PAR2CB:
      controlHiGui(fHiPar2CB);
      break;
    default:
      break;
    }
    break;

  case kCM_TAB:
    pEntry = (TGTextLBEntry*) fObjList->GetSelectedEntry();
    if(pEntry) sprintf(tmpname,"%s",pEntry->GetText()->GetString());
    else sprintf(tmpname,"%s",nulstr);
    fDB->changed(calcTabsIdx());
    displayObjList();
    highlight(fObjList,tmpname);
    break;

  case kCM_LISTBOX:
    
    switch(parm1) {

    case M__MD_MODLIST:
      controlMdcDSGui(fMdcModList);
      break;
    case M__RI_MODLIST:
      controlRichDSGui(fRichModList);
      break;
    case M__SH_MODLIST:
      controlShDSGui(fShModList);
      break;

    case M_LIST:

      if(calcTabsIdx() == 7) displayHisto();
      if(calcTabsIdx() == 8) displayLC();
      if(calcTabsIdx() == 9) displayGC();
      if(calcTabsIdx() == 1) displayTofDS();
      if(calcTabsIdx() == 2) displayMdcDS();
      if(calcTabsIdx() == 3) displayRichDS();
      if(calcTabsIdx() == 4) displayShDS();
      break;

    case M__LC_PARAMLB:
      if(calcTabsIdx() == 8) {
	if(parm2 <= HDefaultParameter::getMaxDefIdx(calcCateg())) {
	  expStringL.addItem(parm2 + 35);
	}
	else {
	  pEntry = (TGTextLBEntry*) fLCParLB->GetSelectedEntry();
	  if(!pEntry) break;
	  if(!fDB) break;
	  pUP = fDB->at(fDB->find(pEntry->GetText()->GetString()));
	  if(!pUP) break;
	  expStringL.addString(pUP->getExpression());
	}
	expStringL.updateList(fLCExpList);
      }
      break;

    case M__GC_PARAMLIST:
      pEntry = (TGTextLBEntry*) fGCListPar->GetSelectedEntry();
      if(!pEntry) break;
      expStringG.addItem(extractCountIdx(pEntry->GetText()->GetString())+36);
      expStringG.updateList(fGCExpList);
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
//******  void HOnlObjBrowser::controlTofDSGui(TGFrame *p)
//
// This function handles GUI widgets in the TOF DS subpanel.
//
//---------------------------------------------------------------------

void HOnlObjBrowser::controlTofDSGui(TGFrame *p) {
//
// This function handles GUI widgets in the TOF DS subpanel.
//

  if(!p) return;

  if(p == fTofSingleRB) {

    if(fTofSingleRB->GetState()) {
      fTofMultiRB->SetState(kButtonUp);
      createTofDSCB(kTRUE);
      for(Int_t i=0; i<48; i++) {
	toDefault(fTofSecModBut[i]->GetId());
	kTofColor[i] = kFALSE;
	fClient->NeedRedraw(fTofSecModBut[i]);      
	fTofSecModBut[i]->AllowStayDown(kTRUE);
	fTofSecModBut[i]->SetState(kButtonEngaged);
	countTof = 2;
      }
    }
    p = fTofMultiRB;
  }

  if(p == fTofMultiRB) {

    if(fTofMultiRB->GetState()) {
      fTofSingleRB->SetState(kButtonUp);
      createTofDSCB(kFALSE);
      for(Int_t i=0; i<48; i++) {
	toDefault(fTofSecModBut[i]->GetId());
	kTofColor[i] = kFALSE;
	fClient->NeedRedraw(fTofSecModBut[i]);      
	fTofSecModBut[i]->AllowStayDown(kFALSE);
	fTofSecModBut[i]->SetState(kButtonUp);
	countTof = 2;
      }
    }
  }

}


//---------------------------------------------------------------------
//******  void HOnlObjBrowser::controlMdcDSGui(TGFrame *p)
//
// This function handles GUI widgets in the MDC DS subpanel.
//
//---------------------------------------------------------------------

void HOnlObjBrowser::controlMdcDSGui(TGFrame *p) {
//
// This function handles GUI widgets in the MDC DS subpanel.
//

  if(!p) return;

  Int_t i = 0;
  Int_t j = 0;
  TGTextLBEntry *pEntry = 0;
  Int_t index = -1;

  if(p == fMdcSingleRB) {

    if(fMdcSingleRB->GetState()) {
      fMdcMultiRB->SetState(kButtonUp);
      createMdcDSCB(kTRUE);
      for(i=0; i<24; i++) {
	toDefault(fMdcSecModBut[i]->GetId());
	kMdcColor1[i] = kFALSE;
	fClient->NeedRedraw(fMdcSecModBut[i]);      
	fMdcSecModBut[i]->AllowStayDown(kTRUE);
	fMdcSecModBut[i]->SetState(kButtonEngaged);
	countMdcS1 = 2;
      }
      for(i=0; i<48; i++) {
	toDefault(fMdcCellsBut[i]->GetId());
	kMdcColor2[i] = kFALSE;
	fClient->NeedRedraw(fMdcCellsBut[i]);      
	fMdcCellsBut[i]->AllowStayDown(kTRUE);
	fMdcCellsBut[i]->SetState(kButtonEngaged);
	countMdcS2 = 2;
      }
      for(i=0;i<24;i++) {
	for(j=0;j<48;j++) fMdcStep12[i][j] = 0;
      }
    }
    p = fMdcMultiRB;
  }

  if(p == fMdcMultiRB) {

    if(fMdcMultiRB->GetState()) {
      fMdcSingleRB->SetState(kButtonUp);
      createMdcDSCB(kFALSE);
      for(i=0; i<24; i++) {
	toDefault(fMdcSecModBut[i]->GetId());
	kMdcColor1[i] = kFALSE;
	fClient->NeedRedraw(fMdcSecModBut[i]);      
	fMdcSecModBut[i]->AllowStayDown(kFALSE);
	fMdcSecModBut[i]->SetState(kButtonUp);
	countMdcS1 = 2;
      }
      for(i=0; i<48; i++) {
	toDefault(fMdcCellsBut[i]->GetId());
	kMdcColor2[i] = kFALSE;
	fClient->NeedRedraw(fMdcCellsBut[i]);      
	fMdcCellsBut[i]->AllowStayDown(kTRUE);
	fMdcCellsBut[i]->SetState(kButtonEngaged);
	countMdcS2 = 2;
      }
      for(i=0;i<24;i++) {
	for(j=0;j<48;j++) fMdcStep12[i][j] = 0;
      }
    }
    createMdcModList(-1);
    p = 0;
  }


  if(fMdcMultiRB->GetState() && (p == fMdcSecModBut[0])) {
    
    displayMdcLayS1();
    createMdcModList(-1);
    p = fMdcModList;
  }


  if(p == fMdcModList) {

    pEntry = (TGTextLBEntry*) fMdcModList->GetSelectedEntry();
    if(pEntry) index = atoi(pEntry->GetText()->GetString()) - 1;

    for(i=0;i<24;i++) {
      if(kMdcColor1[i]) {
	if(i==index) toRed(fMdcSecModBut[i]->GetId());
	else toGreen(fMdcSecModBut[i]->GetId());
      }
      else toDefault(fMdcSecModBut[i]->GetId());
      fClient->NeedRedraw(fMdcSecModBut[i]);
    }

    if(index > -1 && index < 24) {
      for(i=0;i<48;i++) {
	fMdcCellsBut[i]->AllowStayDown(kFALSE);
	fMdcCellsBut[i]->SetState(kButtonUp);
	if(fMdcStep12[index][i] == 1) {
     	  kMdcColor2[i] = kTRUE;
	  toGreen(fMdcCellsBut[i]->GetId());
	}
	else {
       	  kMdcColor2[i] = kFALSE;
	  toDefault(fMdcCellsBut[i]->GetId());
	}
	fClient->NeedRedraw(fMdcCellsBut[i]);
      }
    }

  }

  pEntry = (TGTextLBEntry*) fMdcModList->GetSelectedEntry();
  if(pEntry) {

    index = atoi(pEntry->GetText()->GetString()) - 1;

    if(fMdcMultiRB->GetState() && (p == fMdcCellsBut[0])) {

      displayMdcLayS2();
      for(i=0;i<48;i++) { 
	if(kMdcColor2[i]) fMdcStep12[index][i] = 1;
	else fMdcStep12[index][i] = 0; 
      }
      p = 0;
    }  
  }

}


//---------------------------------------------------------------------
//******  void HOnlObjBrowser::controlRichDSGui(TGFrame *p)
//
// This function handles GUI widgets in the RICH DS subpanel.
//
//---------------------------------------------------------------------

void HOnlObjBrowser::controlRichDSGui(TGFrame *p) {
//
// This function handles GUI widgets in RICH DS subpanel.
//

  if(!p) return;

  Int_t i = 0;
  Int_t j = 0;
  TGTextLBEntry *pEntry = 0;
  Int_t index = -1;

  if(p == fRichSingleRB) {

    if(fRichSingleRB->GetState()) {
      fRichMultiRB->SetState(kButtonUp);
      createRichDSCB(kTRUE);
      for(i=0; i<6; i++) {
	toDefault(fRichSecModBut[i]->GetId());
	kRichColor1[i] = kFALSE;
	fClient->NeedRedraw(fRichSecModBut[i]);      
	fRichSecModBut[i]->AllowStayDown(kTRUE);
	fRichSecModBut[i]->SetState(kButtonEngaged);
	countRichS1 = 2;
      }
      for(i=0; i<64; i++) {
	toDefault(fRichCellsBut[i]->GetId());
	kRichColor2[i] = kFALSE;
	fClient->NeedRedraw(fRichCellsBut[i]);      
	fRichCellsBut[i]->AllowStayDown(kTRUE);
	fRichCellsBut[i]->SetState(kButtonEngaged);
	countRichS2 = 2;
      }
      for(i=0;i<6;i++) {
	for(j=0;j<64;j++) fRichStep12[i][j] = 0;
      }
    }
    p = fRichMultiRB;
  }

  if(p == fRichMultiRB) {

    if(fRichMultiRB->GetState()) {
      fRichSingleRB->SetState(kButtonUp);
      createRichDSCB(kFALSE);
      for(i=0; i<6; i++) {
	toDefault(fRichSecModBut[i]->GetId());
	kRichColor1[i] = kFALSE;
	fClient->NeedRedraw(fRichSecModBut[i]);      
	fRichSecModBut[i]->AllowStayDown(kFALSE);
	fRichSecModBut[i]->SetState(kButtonUp);
	countRichS1 = 2;
      }
      for(i=0; i<64; i++) {
	toDefault(fRichCellsBut[i]->GetId());
	kRichColor2[i] = kFALSE;
	fClient->NeedRedraw(fRichCellsBut[i]);      
	fRichCellsBut[i]->AllowStayDown(kTRUE);
	fRichCellsBut[i]->SetState(kButtonEngaged);
	countRichS2 = 2;
      }
      for(i=0;i<6;i++) {
	for(j=0;j<64;j++) fRichStep12[i][j] = 0;
      }
    }
    createRichModList(-1);
    p = 0;
  }


  if(fRichMultiRB->GetState() && (p == fRichSecModBut[0])) {
    
    displayRichLayS1();
    createRichModList(-1);
    p = fRichModList;
  }

  if(p == fRichModList) {

    pEntry = (TGTextLBEntry*) fRichModList->GetSelectedEntry();
    if(pEntry) index = atoi(pEntry->GetText()->GetString()) - 1;

    for(i=0;i<6;i++) {
      if(kRichColor1[i]) {
	if(i==index) toRed(fRichSecModBut[i]->GetId());
	else toGreen(fRichSecModBut[i]->GetId());
      }
      else toDefault(fRichSecModBut[i]->GetId());
      fClient->NeedRedraw(fRichSecModBut[i]);
    }

    if(index > -1 && index < 6) {
      for(i=0;i<64;i++) {
	fRichCellsBut[i]->AllowStayDown(kFALSE);
	fRichCellsBut[i]->SetState(kButtonUp);
	if(fRichStep12[index][i] == 1) {
	  kRichColor2[i] = kTRUE;
	  toGreen(fRichCellsBut[i]->GetId());
	}
	else {
	  kRichColor2[i] = kFALSE;
	  toDefault(fRichCellsBut[i]->GetId());
	}
	fClient->NeedRedraw(fRichCellsBut[i]);
      }
    }

  }

  pEntry = (TGTextLBEntry*) fRichModList->GetSelectedEntry();
  if(pEntry) {

    index = atoi(pEntry->GetText()->GetString()) - 1;

    if(fRichMultiRB->GetState() && (p == fRichCellsBut[0])) {

      displayRichLayS2();
      for(i=0;i<64;i++) { 
	if(kRichColor2[i]) fRichStep12[index][i] = 1;
	else fRichStep12[index][i] = 0; 
      }
      p = 0;
    }  
  }

}



//---------------------------------------------------------------------
//******  void HOnlObjBrowser::controlShDSGui(TGFrame *p)
//
// This function handles GUI widgets in the SHOWER DS subpanel.
//
//---------------------------------------------------------------------

void HOnlObjBrowser::controlShDSGui(TGFrame *p) {
//
// This function handles GUI widgets in the SHOWER DS subpanel.
//

  if(!p) return;

  Int_t i = 0;
  Int_t j = 0;
  TGTextLBEntry *pEntry = 0;
  Int_t index = -1;

  if(p == fShSingleRB) {

    if(fShSingleRB->GetState()) {
      fShMultiRB->SetState(kButtonUp);
      createShDSCB(kTRUE);
      for(i=0; i<18; i++) {
	toDefault(fShSecModBut[i]->GetId());
	kShColor1[i] = kFALSE;
	fClient->NeedRedraw(fShSecModBut[i]);      
	fShSecModBut[i]->AllowStayDown(kTRUE);
	fShSecModBut[i]->SetState(kButtonEngaged);
	countShS1 = 2;
      }
      for(i=0; i<64; i++) {
	toDefault(fShCellsBut[i]->GetId());
	kShColor2[i] = kFALSE;
	fClient->NeedRedraw(fShCellsBut[i]);      
	fShCellsBut[i]->AllowStayDown(kTRUE);
	fShCellsBut[i]->SetState(kButtonEngaged);
	countShS2 = 2;
      }
      for(i=0;i<18;i++) {
	for(j=0;j<64;j++) fShStep12[i][j] = 0;
      }
    }
    p = fShMultiRB;
  }

  if(p == fShMultiRB) {

    if(fShMultiRB->GetState()) {
      fShSingleRB->SetState(kButtonUp);
      createShDSCB(kFALSE);
      for(i=0; i<18; i++) {
	toDefault(fShSecModBut[i]->GetId());
	kShColor1[i] = kFALSE;
	fClient->NeedRedraw(fShSecModBut[i]);      
	fShSecModBut[i]->AllowStayDown(kFALSE);
	fShSecModBut[i]->SetState(kButtonUp);
	countShS1 = 2;
      }
      for(i=0; i<64; i++) {
	toDefault(fShCellsBut[i]->GetId());
	kShColor2[i] = kFALSE;
	fClient->NeedRedraw(fShCellsBut[i]);      
	fShCellsBut[i]->AllowStayDown(kTRUE);
	fShCellsBut[i]->SetState(kButtonEngaged);
	countShS2 = 2;
      }
      for(i=0;i<18;i++) {
	for(j=0;j<64;j++) fShStep12[i][j] = 0;
      }
    }
    createShModList(-1);
    p = 0;
  }

  if(fShMultiRB->GetState() && (p == fShSecModBut[0])) {
    
    displayShLayS1();
    createShModList(-1);
    p = fShModList;
  }

  if(p == fShModList) {

    pEntry = (TGTextLBEntry*) fShModList->GetSelectedEntry();
    if(pEntry) index = atoi(pEntry->GetText()->GetString()) - 1;

    for(i=0;i<18;i++) {
      if(kShColor1[i]) {
	if(i==index) toRed(fShSecModBut[i]->GetId());
	else toGreen(fShSecModBut[i]->GetId());
      }
      else toDefault(fShSecModBut[i]->GetId());
      fClient->NeedRedraw(fShSecModBut[i]);
    }

    if(index > -1 && index < 18) {
      for(i=0;i<64;i++) {
	fShCellsBut[i]->AllowStayDown(kFALSE);
	fShCellsBut[i]->SetState(kButtonUp);
	if(fShStep12[index][i] == 1) {
	  kShColor2[i] = kTRUE;
	  toGreen(fShCellsBut[i]->GetId());
	}
	else {
	  kShColor2[i] = kFALSE;
	  toDefault(fShCellsBut[i]->GetId());
	}
	fClient->NeedRedraw(fShCellsBut[i]);
      }
    }

  }

  pEntry = (TGTextLBEntry*) fShModList->GetSelectedEntry();
  if(pEntry) {

    index = atoi(pEntry->GetText()->GetString()) - 1;

    if(fShMultiRB->GetState() && (p == fShCellsBut[0])) {

      displayShLayS2();
      for(i=0;i<64;i++) { 
	if(kShColor2[i]) fShStep12[index][i] = 1;
	else fShStep12[index][i] = 0; 
      }
      p = 0;
    }  
  }

}


//---------------------------------------------------------------------
//******  void HOnlObjBrowser::createMdcModList(Int_t id)
//
// This function creates the list of chosen MDC modules.
//
//---------------------------------------------------------------------

void HOnlObjBrowser::createMdcModList(Int_t id) {
//
// This function creates the list of chosen MDC modules.
//

  Int_t i;
  Int_t j;
  Int_t k = 0;
  Int_t possave = 0;
  char entry[5] = "";

  clearLB(fMdcModList);
  Bool_t kListEmpty = kTRUE;

  j = 0;
  for(i=0;i<24;i++) {

    if(kMdcColor1[i]) {
      kListEmpty = kFALSE;
      sprintf(entry,"%i",i+1);
      fMdcModList->AddEntry(entry,++j);
      if(id > -1 && id < 24 && i == id) possave = j;
      if(id == -1) {
	for(k=0;k<48;k++) {
	  kMdcColor2[k] = kFALSE;
	  toDefault(fMdcCellsBut[k]->GetId());
	  fClient->NeedRedraw(fMdcCellsBut[k]);
	  fMdcCellsBut[k]->AllowStayDown(kTRUE);
	  fMdcCellsBut[k]->SetState(kButtonEngaged);
	}
      }
    }
  }
  if(kListEmpty) {
    for(i=0;i<24;i++) {
      for(k=0;k<48;k++) {
	fMdcStep12[i][k] = 0;
	if(i == 0) {
	  kMdcColor2[k] = kFALSE;
	  toDefault(fMdcCellsBut[k]->GetId());
	  fClient->NeedRedraw(fMdcCellsBut[k]);
	  fMdcCellsBut[k]->AllowStayDown(kTRUE);
	  fMdcCellsBut[k]->SetState(kButtonEngaged);
	}
      }
    }
  }

  fMdcModList->MapSubwindows();
  fMdcModList->Layout();
  if(possave != 0) fMdcModList->Select(possave);
  if(j > 10) fMdcModList->SetTopEntry(possave);

  if(possave > 0) {
    for(k=0;k<48;k++) {
      fMdcCellsBut[k]->AllowStayDown(kFALSE);
      fMdcCellsBut[k]->SetState(kButtonUp);
    }
  }

}


//---------------------------------------------------------------------
//******  void HOnlObjBrowser::createRichModList(Int_t id)
//
// This function creates the list of chosen RICH modules.
//
//---------------------------------------------------------------------

void HOnlObjBrowser::createRichModList(Int_t id) {
//
// This function creates the list of chosen RICH modules.
//

  Int_t i;
  Int_t j;
  Int_t k = 0;
  Int_t possave = 0;
  char entry[5] = "";

  clearLB(fRichModList);
  Bool_t kListEmpty = kTRUE;

  j = 0;
  for(i=0;i<6;i++) {

    if(kRichColor1[i]) {
      kListEmpty = kFALSE;
      sprintf(entry,"%i",i+1);
      fRichModList->AddEntry(entry,++j);
      if(id > -1 && id < 6 && i == id) possave = j;
      if(id == -1) {
	for(k=0;k<64;k++) {
	  kRichColor2[k] = kFALSE;
	  toDefault(fRichCellsBut[k]->GetId());
	  fClient->NeedRedraw(fRichCellsBut[k]);
	  fRichCellsBut[k]->AllowStayDown(kTRUE);
	  fRichCellsBut[k]->SetState(kButtonEngaged);
	}
      }
    }
  }
  if(kListEmpty) {
    for(i=0;i<6;i++) {
      for(k=0;k<64;k++) {
	fRichStep12[i][k] = 0;
	if(i == 0) {
	  kRichColor2[k] = kFALSE;
	  toDefault(fRichCellsBut[k]->GetId());
	  fClient->NeedRedraw(fRichCellsBut[k]);
	  fRichCellsBut[k]->AllowStayDown(kTRUE);
	  fRichCellsBut[k]->SetState(kButtonEngaged);
	}
      }
    }
  }

  fRichModList->MapSubwindows();
  fRichModList->Layout();
  if(possave != 0) fRichModList->Select(possave);
  if(j > 10) fRichModList->SetTopEntry(possave);

  if(possave > 0) {
    for(k=0;k<64;k++) {
      fRichCellsBut[k]->AllowStayDown(kFALSE);
      fRichCellsBut[k]->SetState(kButtonUp);
    }
  }

}


//---------------------------------------------------------------------
//******  void HOnlObjBrowser::createShModList(Int_t id)
//
// This function creates the list of chosen SHOWER modules.
//
//---------------------------------------------------------------------

void HOnlObjBrowser::createShModList(Int_t id) {
//
// This function creates the list of chosen SHOWER modules.
//

  Int_t i;
  Int_t j;
  Int_t k = 0;
  Int_t possave = 0;
  char entry[5] = "";

  clearLB(fShModList);
  Bool_t kListEmpty = kTRUE;

  j = 0;
  for(i=0;i<18;i++) {

    if(kShColor1[i]) {
      kListEmpty = kFALSE;
      sprintf(entry,"%i",i+1);
      fShModList->AddEntry(entry,++j);
      if(id > -1 && id < 18 && i == id) possave = j;
      if(id == -1) {
	for(k=0;k<64;k++) {
	  kShColor2[k] = kFALSE;
	  toDefault(fShCellsBut[k]->GetId());
	  fClient->NeedRedraw(fShCellsBut[k]);
	  fShCellsBut[k]->AllowStayDown(kTRUE);
	  fShCellsBut[k]->SetState(kButtonEngaged);
	}
      }
    }
  }
  if(kListEmpty) {
    for(i=0;i<18;i++) {
      for(k=0;k<64;k++) {
	fShStep12[i][k] = 0;
	if(i == 0) {
	  kShColor2[k] = kFALSE;
	  toDefault(fShCellsBut[k]->GetId());
	  fClient->NeedRedraw(fShCellsBut[k]);
	  fShCellsBut[k]->AllowStayDown(kTRUE);
	  fShCellsBut[k]->SetState(kButtonEngaged);
	}
      }
    }
  }

  fShModList->MapSubwindows();
  fShModList->Layout();
  if(possave != 0) fShModList->Select(possave);
  if(j > 10) fShModList->SetTopEntry(possave);

  if(possave > 0) {
    for(k=0;k<64;k++) {
      fShCellsBut[k]->AllowStayDown(kFALSE);
      fShCellsBut[k]->SetState(kButtonUp);
    }
  }

}

    
//---------------------------------------------------------------------
//******  void HOnlObjBrowser::createTofDSCB(Bool_t kFlag)
//
// This function handles combo boxes in the TOF single panel.
//
//---------------------------------------------------------------------

void HOnlObjBrowser::createTofDSCB(Bool_t kFlag) {
//
// This function handles combo boxes in the TOF single panel.
//

  clearCB(fTofSecBox);
  clearCB(fTofModBox);
  clearCB(fTofRodBox);

  if(kFlag) {

    char entry[5];
    Int_t i;

    for(i=0;i<8;i++) {
      sprintf(entry,"%i",i+1);
      if(i<6) fTofSecBox->AddEntry(entry,i+1);
      fTofModBox->AddEntry(entry,i+1);
      fTofRodBox->AddEntry(entry,i+1);
    }
  }

}


//---------------------------------------------------------------------
//******  void HOnlObjBrowser::createMdcDSCB(Bool_t kFlag)
//
// This function handles combo boxes in the MDC single panel.
//
//---------------------------------------------------------------------

void HOnlObjBrowser::createMdcDSCB(Bool_t kFlag) {
//
// This function handles combo boxes in the MDC single panel.
//

  clearCB(fMdcSecBox);
  clearCB(fMdcModBox);
  clearCB(fMdcLayBox);
  clearCB(fMdcCellBox);

  if(kFlag) {

    char entry[5];
    Int_t i;

    for(i=0;i<200;i++) {
      sprintf(entry,"%i",i+1);
      if(i<4) fMdcModBox->AddEntry(entry,i+1);
      if(i<6) {
	fMdcSecBox->AddEntry(entry,i+1);
	fMdcLayBox->AddEntry(entry,i+1);
      }
      fMdcCellBox->AddEntry(entry,i+1);
    }
  }

}


//---------------------------------------------------------------------
//******  void HOnlObjBrowser::createRichDSCB(Bool_t kFlag)
//
// This function handles combo boxes in the RICH single panel.
//
//---------------------------------------------------------------------

void HOnlObjBrowser::createRichDSCB(Bool_t kFlag) {
//
// This function handles combo boxes in the RICH single panel.
//

  clearCB(fRichSecBox);
  clearCB(fRichRowBox);
  clearCB(fRichColBox);

  if(kFlag) {

    char entry[5];
    Int_t i;

    for(i=0;i<96;i++) {
      sprintf(entry,"%i",i+1);
      if(i<6) fRichSecBox->AddEntry(entry,i+1);
      fRichRowBox->AddEntry(entry,i+1);
      fRichColBox->AddEntry(entry,i+1);
    }
  }

}


//---------------------------------------------------------------------
//******  void HOnlObjBrowser::createShDSCB(Bool_t kFlag)
//
// This function handles combo boxes in the SHOWER single panel.
//
//---------------------------------------------------------------------

void HOnlObjBrowser::createShDSCB(Bool_t kFlag) {
//
// This function handles combo boxes in the SHOWER single panel.
//

  clearCB(fShSecBox);
  clearCB(fShModBox);
  clearCB(fShRowBox);
  clearCB(fShColBox);

  if(kFlag) {

    char entry[5];
    Int_t i;

    for(i=0;i<32;i++) {
      sprintf(entry,"%i",i+1);
      if(i<6) fShSecBox->AddEntry(entry,i+1);
      if(i<3) fShModBox->AddEntry(entry,i+1);
      fShRowBox->AddEntry(entry,i+1);
      fShColBox->AddEntry(entry,i+1);
    }
  }

}


//---------------------------------------------------------------------
//******  HDetSet* HOnlObjBrowser::createTofDS()
//
// This function scans subpanel TOF DS and returns an object of HTofDetSet
// class.
//
//---------------------------------------------------------------------

HDetSet* HOnlObjBrowser::createTofDS() {
//
// This function scans subpanel TOF DS and returns an object of HTofDetSet
// class.
//

  char detsetname[100] = "";
  char userword[50] = "";
  HDetSet *pSet = 0;
  Int_t *idx = 0;
  Int_t i = 0;
  TGTextLBEntry *pEntry = 0;
  Bool_t kMultiSet = kFALSE;

  sprintf(userword, "%s", fTofNameBuf->GetString());
  if(!strcmp(userword,"")) return 0;

  if(fTofSingleRB->GetState()) {
  
    idx = new Int_t[3];
    if(!idx) return 0;

    pEntry = (TGTextLBEntry*) fTofSecBox->GetSelectedEntry();
    if(!pEntry) { delete [] idx; return 0; }
    idx[0] = atoi(pEntry->GetText()->GetString()) - 1;
    pEntry = (TGTextLBEntry*) fTofModBox->GetSelectedEntry();
    if(!pEntry) { delete [] idx; return 0; }
    idx[1] = atoi(pEntry->GetText()->GetString()) - 1;
    pEntry = (TGTextLBEntry*) fTofRodBox->GetSelectedEntry();
    if(!pEntry) { delete [] idx; return 0; }
    idx[2] = atoi(pEntry->GetText()->GetString()) - 1;

  }

  if(fTofMultiRB->GetState()) {

    if(idx) { delete [] idx; idx = 0; }
    idx = new Int_t[48];
    if(!idx) return 0;
    
    for(i=0;i<48;i++) {
      if(kTofColor[i]) {
	idx[i] = 1;
	kMultiSet = kTRUE;
      }
      else idx[i] = 0;
    }
    if(!kMultiSet) { delete [] idx; return 0; }
  }

  pSet = new HTofDetSet();
  if(!pSet) {
    if(idx) delete [] idx;
    return 0;
  }

  if(fTofSingleRB->GetState()) pSet->setSingleAddress(idx,3);
  if(fTofMultiRB->GetState()) pSet->setMultiAddress(idx,48);
  if(idx) { delete [] idx; idx = 0; }

  pSet->setDetName(TOF);
  
  pEntry = (TGTextLBEntry*) fTofCatBox->GetSelectedEntry();
  if(!pEntry) { delete pSet; return 0; }

  if(!strcmp(pEntry->GetText()->GetString(),"raw")) {
    pSet->setCategory(96); // TOF raw
    sprintf(detsetname,"%s%s","tofraw_",userword);
    pSet->SetName(detsetname);
  }    
  if(!strcmp(pEntry->GetText()->GetString(),"hit")) {
    pSet->setCategory(97); // TOF hit
    sprintf(detsetname,"%s%s","tofhit_",userword);
    pSet->SetName(detsetname);
  }    
    
  return pSet;

}


//---------------------------------------------------------------------
//******  HDetSet* HOnlObjBrowser::createMdcDS()
//
// This function scans subpanel MDC DS and returns an object of HMdcDetSet
// class.
//
//---------------------------------------------------------------------

HDetSet* HOnlObjBrowser::createMdcDS() {
//
// This function scans subpanel MDC DS and returns an object of HMdcDetSet
// class.
//

  char detsetname[100] = "";
  char userword[50] = "";
  HDetSet *pSet = 0;
  Int_t *idx = 0;
  Int_t i = 0;
  Int_t j = 0;
  TGTextLBEntry *pEntry = 0;
  Bool_t kMultiSet = kFALSE;

  sprintf(userword, "%s", fMdcNameBuf->GetString());
  if(!strcmp(userword,"")) return 0;

  if(fMdcSingleRB->GetState()) {
  
    idx = new Int_t[4];
    if(!idx) return 0;

    pEntry = (TGTextLBEntry*) fMdcSecBox->GetSelectedEntry();
    if(!pEntry) { delete [] idx; return 0; }
    idx[0] = atoi(pEntry->GetText()->GetString()) - 1;
    pEntry = (TGTextLBEntry*) fMdcModBox->GetSelectedEntry();
    if(!pEntry) { delete [] idx; return 0; }
    idx[1] = atoi(pEntry->GetText()->GetString()) - 1;
    pEntry = (TGTextLBEntry*) fMdcLayBox->GetSelectedEntry();
    if(!pEntry) { delete [] idx; return 0; }
    idx[2] = atoi(pEntry->GetText()->GetString()) - 1;
    pEntry = (TGTextLBEntry*) fMdcCellBox->GetSelectedEntry();
    if(!pEntry) { delete [] idx; return 0; }
    idx[3] = atoi(pEntry->GetText()->GetString()) - 1;

  }

  if(fMdcMultiRB->GetState()) {

    if(idx) { delete [] idx; idx = 0; }
    for(i=0;i<24;i++) {
      for(j=0;j<48;j++) {
	if(fMdcStep12[i][j] == 1) {
	  kMultiSet = kTRUE;
	  break;
	}
      }
      if(kMultiSet) break;
    }

    if(!kMultiSet) return 0;

    idx = &fMdcStep12[0][0];
  }

  pSet = new HMdcDetSet();
  if(!pSet) {
    if(fMdcSingleRB->GetState() && idx) delete [] idx;
    return 0;
  }

  if(fMdcSingleRB->GetState()) pSet->setSingleAddress(idx,4);
  if(fMdcMultiRB->GetState()) pSet->setMultiAddress(idx,1152);
  if(fMdcSingleRB->GetState() && idx) { delete [] idx; idx = 0; }

  pSet->setDetName(MDC);
  
  pEntry = (TGTextLBEntry*) fMdcCatBox->GetSelectedEntry();
  if(!pEntry) { delete pSet; return 0; }

  if(!strcmp(pEntry->GetText()->GetString(),"cal1")) {
    pSet->setCategory(2); // MDC cal1
    sprintf(detsetname,"%s%s","mdccal1_",userword);
    pSet->SetName(detsetname);
  }    
  if(!strcmp(pEntry->GetText()->GetString(),"cal2")) {
    pSet->setCategory(4); // MDC cal2
    sprintf(detsetname,"%s%s","mdccal2_",userword);
    pSet->SetName(detsetname);
  }    
  if(!strcmp(pEntry->GetText()->GetString(),"hit")) {
    pSet->setCategory(1); // MDC hit
    sprintf(detsetname,"%s%s","mdchit_",userword);
    pSet->SetName(detsetname);
  }    
    
  return pSet;

}


//---------------------------------------------------------------------
//******  HDetSet* HOnlObjBrowser::createRichDS()
//
// This function scans subpanel RICH DS and returns an object of HRichDetSet
// class.
//
//---------------------------------------------------------------------

HDetSet* HOnlObjBrowser::createRichDS() {
//
// This function scans subpanel RICH DS and returns an object of HRichDetSet
// class.
//

  char detsetname[100] = "";
  char userword[50] = "";
  HDetSet *pSet = 0;
  Int_t *idx = 0;
  Int_t i = 0;
  Int_t j = 0;
  TGTextLBEntry *pEntry = 0;
  Bool_t kMultiSet = kFALSE;

  sprintf(userword, "%s", fRichNameBuf->GetString());
  if(!strcmp(userword,"")) return 0;

  if(fRichSingleRB->GetState()) {
  
    idx = new Int_t[3];
    if(!idx) return 0;

    pEntry = (TGTextLBEntry*) fRichSecBox->GetSelectedEntry();
    if(!pEntry) { delete [] idx; return 0; }
    idx[0] = atoi(pEntry->GetText()->GetString()) - 1;
    pEntry = (TGTextLBEntry*) fRichRowBox->GetSelectedEntry();
    if(!pEntry) { delete [] idx; return 0; }
    idx[1] = atoi(pEntry->GetText()->GetString()) - 1;
    pEntry = (TGTextLBEntry*) fRichColBox->GetSelectedEntry();
    if(!pEntry) { delete [] idx; return 0; }
    idx[2] = atoi(pEntry->GetText()->GetString()) - 1;

  }

  if(fRichMultiRB->GetState()) {

    if(idx) { delete [] idx; idx = 0; }
    for(i=0;i<6;i++) {
      for(j=0;j<64;j++) {
	if(fRichStep12[i][j] == 1) {
	  kMultiSet = kTRUE;
	  break;
	}
      }
      if(kMultiSet) break;
    }

    if(!kMultiSet) return 0;

    idx = &fRichStep12[0][0];
  }

  pSet = new HRichDetSet();
  if(!pSet) {
    if(fRichSingleRB->GetState() && idx) delete [] idx;
    return 0;
  }

  if(fRichSingleRB->GetState()) pSet->setSingleAddress(idx,3);
  if(fRichMultiRB->GetState()) pSet->setMultiAddress(idx,384);
  if(fRichSingleRB->GetState() && idx) { delete [] idx; idx = 0; }

  pSet->setDetName(RICH);
  
  pEntry = (TGTextLBEntry*) fRichCatBox->GetSelectedEntry();
  if(!pEntry) { delete pSet; return 0; }

  if(!strcmp(pEntry->GetText()->GetString(),"raw")) {
    pSet->setCategory(32); // RICH raw
    sprintf(detsetname,"%s%s","riraw_",userword);
    pSet->SetName(detsetname);
  }    
  if(!strcmp(pEntry->GetText()->GetString(),"cal")) {
    pSet->setCategory(34); // RICH cal
    sprintf(detsetname,"%s%s","rical_",userword);
    pSet->SetName(detsetname);
  }    
  if(!strcmp(pEntry->GetText()->GetString(),"hit")) {
    pSet->setCategory(33); // RICH hit
    sprintf(detsetname,"%s%s","rihit_",userword);
    pSet->SetName(detsetname);
  }    
    
  return pSet;

}


//---------------------------------------------------------------------
//******  HDetSet* HOnlObjBrowser::createShDS()
//
// This function scans subpanel SHOWER DS and returns an object of 
// HShowerDetSet class.
//
//---------------------------------------------------------------------

HDetSet* HOnlObjBrowser::createShDS() {
//
// This function scans subpanel SHOWER DS and returns an object of 
// HShowerDetSet class.
//

  char detsetname[100] = "";
  char userword[50] = "";
  HDetSet *pSet = 0;
  Int_t *idx = 0;
  Int_t i = 0;
  Int_t j = 0;
  TGTextLBEntry *pEntry = 0;
  Bool_t kMultiSet = kFALSE;

  sprintf(userword, "%s", fShNameBuf->GetString());
  if(!strcmp(userword,"")) return 0;

  if(fShSingleRB->GetState()) {
  
    idx = new Int_t[4];
    if(!idx) return 0;

    pEntry = (TGTextLBEntry*) fShSecBox->GetSelectedEntry();
    if(!pEntry) { delete [] idx; return 0; }
    idx[0] = atoi(pEntry->GetText()->GetString()) - 1;
    pEntry = (TGTextLBEntry*) fShModBox->GetSelectedEntry();
    if(!pEntry) { delete [] idx; return 0; }
    idx[1] = atoi(pEntry->GetText()->GetString()) - 1;
    pEntry = (TGTextLBEntry*) fShRowBox->GetSelectedEntry();
    if(!pEntry) { delete [] idx; return 0; }
    idx[2] = atoi(pEntry->GetText()->GetString()) - 1;
    pEntry = (TGTextLBEntry*) fShColBox->GetSelectedEntry();
    if(!pEntry) { delete [] idx; return 0; }
    idx[3] = atoi(pEntry->GetText()->GetString()) - 1;

  }

  if(fShMultiRB->GetState()) {

    if(idx) { delete [] idx; idx = 0; }
    for(i=0;i<18;i++) {
      for(j=0;j<64;j++) {
	if(fShStep12[i][j] == 1) {
	  kMultiSet = kTRUE;
	  break;
	}
      }
      if(kMultiSet) break;
    }

    if(!kMultiSet) return 0;

    idx = &fShStep12[0][0];
  }

  pSet = new HShowerDetSet();
  if(!pSet) {
    if(fShSingleRB->GetState() && idx) delete [] idx;
    return 0;
  }

  if(fShSingleRB->GetState()) pSet->setSingleAddress(idx,4);
  if(fShMultiRB->GetState()) pSet->setMultiAddress(idx,1152);
  if(fShSingleRB->GetState() && idx) { delete [] idx; idx = 0; }

  pSet->setDetName(SHOWER);
  
  pEntry = (TGTextLBEntry*) fShCatBox->GetSelectedEntry();
  if(!pEntry) { delete pSet; return 0; }

  if(!strcmp(pEntry->GetText()->GetString(),"raw")) {
    pSet->setCategory(64); // SHOWER raw
    sprintf(detsetname,"%s%s","shraw_",userword);
    pSet->SetName(detsetname);
  }    
  if(!strcmp(pEntry->GetText()->GetString(),"cal")) {
    pSet->setCategory(66); // SHOWER cal
    sprintf(detsetname,"%s%s","shcal_",userword);
    pSet->SetName(detsetname);
  }    
  if(!strcmp(pEntry->GetText()->GetString(),"hit")) {
    pSet->setCategory(65); // SHOWER hit
    sprintf(detsetname,"%s%s","shhit_",userword);
    pSet->SetName(detsetname);
  }    
    
  return pSet;

}


//---------------------------------------------------------------------
//******  Bool_t HOnlObjBrowser::changeDetSet(Int_t det, Int_t mode = 0,
//                                            HDetSet **p, Int_t *id)
//
// This function is called when Add, Modify or Remove button is clicked.
// mode == 0  - add a new det set
// mode == 1  - modify existing det set
// mode == 2  - virtually delete existing det set and return objects's address
//              in p
//
// det == 1 - TOF
// det == 2 - MDC
// det == 3 - RICH
// det == 4 - SHOWER
//
//---------------------------------------------------------------------

Bool_t HOnlObjBrowser::changeDetSet(Int_t det, Int_t mode, HDetSet **p,
				    Int_t *id) {
//
// This function is called when Add, Modify or Remove button is clicked.
// mode == 0  - add a new det set
// mode == 1  - modify existing det set
// mode == 2  - virtually delete existing det set and return objects's address
//              in p
//
// det == 1 - TOF
// det == 2 - MDC
// det == 3 - RICH
// det == 4 - SHOWER
//

  if(!gOnline) return kFALSE;
  TGTextLBEntry *pEntry = 0;
  Int_t idx = -1;

  if(id) *id = -1;
  if(mode == 2) { // delete

    pEntry = (TGTextLBEntry*) fObjList->GetSelectedEntry();
    if(!pEntry) return kFALSE;
    idx = gOnline->getDetSet()->find(pEntry->GetText()->GetString());
    if(idx == -1) return kFALSE;
    if(p) (*p) = gOnline->getDetSet()->at(idx);
    if(id) *id = idx;
    gOnline->getDetSet()->removeAt(idx,kFALSE);
    return kTRUE;
  }

  HDetSet *pSet;
  switch(det) {
  case 1:  { pSet = createTofDS(); break; }
  case 2:  { pSet = createMdcDS(); break; }
  case 3:  { pSet = createRichDS(); break; }
  case 4:  { pSet = createShDS(); break; }
  default: { pSet = 0; break; }
  }
  if(!pSet) return kFALSE;

  if(mode == 1) { // modify

    pEntry = (TGTextLBEntry*) fObjList->GetSelectedEntry();
    if(!pEntry) { delete pSet; return kFALSE; }

    Int_t index = gOnline->getDetSet()->find(pEntry->GetText()->GetString());
    if(index == -1) { delete pSet; return kFALSE; }

    idx = gOnline->getDetSet()->find(pSet->GetName());

    if(idx != -1 && idx != index) {
      HError::message("Detector set with this name already exists.");
      delete pSet;
      pSet = 0;
      return kFALSE;
    }
    gOnline->getDetSet()->removeAt(index);
    gOnline->getDetSet()->addAt(pSet,index);
  }

  if(mode == 0) {
    idx = gOnline->getDetSet()->find(pSet->GetName());
    if(idx != -1) {
      HError::message("Detector set with this name already exists.");
      delete pSet;
      pSet = 0;
      return kFALSE;
    }
    gOnline->getDetSet()->add(pSet);
  }

  return kTRUE;
}


//---------------------------------------------------------------------
//******  void HOnlObjBrowser::displayTofDS()
//
// This function displays the highlighted TOF det set in the subpanel TOF.
//
//---------------------------------------------------------------------

void HOnlObjBrowser::displayTofDS() {
//
// This function displays the highlighted TOF det set in the subpanel TOF.
//

  if(!gOnline) return;

  HDetSet *pSet = 0;
  TArrayI ar;
  Int_t idx = -1;
  Int_t i = 0;
  TGTextLBEntry *pEntry = 0;

  pEntry = (TGTextLBEntry*) fObjList->GetSelectedEntry();
  if(!pEntry) return;

  idx = gOnline->getDetSet()->find(pEntry->GetText()->GetString());
  if(idx == -1) return;

  pSet = gOnline->getDetSet()->at(idx);
  if(!pSet) return;
  if(pSet->getDetName() != TOF) return;

  if(pSet->isMulti()) {
    fTofMultiRB->SetState(kButtonDown);
    controlTofDSGui(fTofMultiRB);
  }
  else {
    fTofSingleRB->SetState(kButtonDown);
    controlTofDSGui(fTofSingleRB);
  }

  if(pSet->getCategory() == 96) highlight(fTofCatBox,"raw");
  if(pSet->getCategory() == 97) highlight(fTofCatBox,"hit");

  fTofNameBuf->Clear();
  fTofNameBuf->AddText(0,pSet->getUserWord());
  fClient->NeedRedraw(fTofName);

  if(fTofSingleRB->GetState()) {
	
    ar = pSet->getSingleAddress();
    if(ar.fN != 3) return;
    fTofSecBox->Select(ar[0]+1);
    fTofModBox->Select(ar[1]+1);
    fTofRodBox->Select(ar[2]+1);
  }
  else {
    ar = pSet->getMultiAddress();
    if(ar.fN != 48) return;
    for(i=0; i<48; i++) {
      if(ar[i] == 1) {
	kTofColor[i] = kTRUE;
	toGreen(fTofSecModBut[i]->GetId());
      }
      else {
	kTofColor[i] = kFALSE;
	toDefault(fTofSecModBut[i]->GetId());
      }
      fClient->NeedRedraw(fTofSecModBut[i]);
    }
  }

}


//---------------------------------------------------------------------
//******  void HOnlObjBrowser::displayMdcDS()
//
// This function displays the highlighted MDC det set in the subpanel MDC.
//
//---------------------------------------------------------------------

void HOnlObjBrowser::displayMdcDS() {
//
// This function displays the highlighted MDC det set in the subpanel MDC.
//

  if(!gOnline) return;

  HDetSet *pSet = 0;
  TArrayI ar;
  Int_t idx = -1;
  Int_t i = 0;
  Int_t j = 0;
  TGTextLBEntry *pEntry = 0;

  pEntry = (TGTextLBEntry*) fObjList->GetSelectedEntry();
  if(!pEntry) return;

  idx = gOnline->getDetSet()->find(pEntry->GetText()->GetString());
  if(idx == -1) return;

  pSet = gOnline->getDetSet()->at(idx);
  if(!pSet) return;
  if(pSet->getDetName() != MDC) return;

  if(pSet->isMulti()) {
    fMdcMultiRB->SetState(kButtonDown);
    controlMdcDSGui(fMdcMultiRB);
  }
  else {
    fMdcSingleRB->SetState(kButtonDown);
    controlMdcDSGui(fMdcSingleRB);
  }

  if(pSet->getCategory() == 2) highlight(fMdcCatBox,"cal1");
  if(pSet->getCategory() == 4) highlight(fMdcCatBox,"cal2");
  if(pSet->getCategory() == 1) highlight(fMdcCatBox,"hit");

  fMdcNameBuf->Clear();
  fMdcNameBuf->AddText(0,pSet->getUserWord());
  fClient->NeedRedraw(fMdcName);

  if(fMdcSingleRB->GetState()) {
	
    ar = pSet->getSingleAddress();
    if(ar.fN != 4) return;
    fMdcSecBox->Select(ar[0]+1);
    fMdcModBox->Select(ar[1]+1);
    fMdcLayBox->Select(ar[2]+1);
    fMdcCellBox->Select(ar[3]+1);
  }
  else {
    ar = pSet->getMultiAddress();
    if(ar.fN != 1152) return;
    for(i=0;i<1152;i++) {
      fMdcStep12[i/48][i%48] = ar[i];
      if(i%48 == 47) {
	kMdcColor1[i/48] = kFALSE;
	for(j=0;j<48;j++) {
	  if(fMdcStep12[i/48][j] == 1) kMdcColor1[i/48] = kTRUE;
	}
      }
    }
  }

  if(fMdcMultiRB->GetState()) {
    for(i=0;i<24;i++) {
      if(kMdcColor1[i]) toGreen(fMdcSecModBut[i]->GetId());
      else toDefault(fMdcSecModBut[i]->GetId());
      fClient->NeedRedraw(fMdcSecModBut[i]);
    }
  }
  createMdcModList(-1);

}


//---------------------------------------------------------------------
//******  void HOnlObjBrowser::displayRichDS()
//
// This function displays the highlighted RICH det set in the subpanel RICH.
//
//---------------------------------------------------------------------

void HOnlObjBrowser::displayRichDS() {
//
// This function displays the highlighted RICH det set in the subpanel RICH.
//

  if(!gOnline) return;

  HDetSet *pSet = 0;
  TArrayI ar;
  Int_t idx = -1;
  Int_t i = 0;
  Int_t j = 0;
  TGTextLBEntry *pEntry = 0;

  pEntry = (TGTextLBEntry*) fObjList->GetSelectedEntry();
  if(!pEntry) return;

  idx = gOnline->getDetSet()->find(pEntry->GetText()->GetString());
  if(idx == -1) return;

  pSet = gOnline->getDetSet()->at(idx);
  if(!pSet) return;
  if(pSet->getDetName() != RICH) return;

  if(pSet->isMulti()) {
    fRichMultiRB->SetState(kButtonDown);
    controlRichDSGui(fRichMultiRB);
  }
  else {
    fRichSingleRB->SetState(kButtonDown);
    controlRichDSGui(fRichSingleRB);
  }

  if(pSet->getCategory() == 32) highlight(fRichCatBox,"raw");
  if(pSet->getCategory() == 34) highlight(fRichCatBox,"cal");
  if(pSet->getCategory() == 33) highlight(fRichCatBox,"hit");

  fRichNameBuf->Clear();
  fRichNameBuf->AddText(0,pSet->getUserWord());
  fClient->NeedRedraw(fRichName);

  if(fRichSingleRB->GetState()) {
	
    ar = pSet->getSingleAddress();
    if(ar.fN != 3) return;
    fRichSecBox->Select(ar[0]+1);
    fRichRowBox->Select(ar[1]+1);
    fRichColBox->Select(ar[2]+1);
  }
  else {
    ar = pSet->getMultiAddress();
    if(ar.fN != 384) return;
    for(i=0;i<384;i++) {
      fRichStep12[i/64][i%64] = ar[i];
      if(i%64 == 63) {
	kRichColor1[i/64] = kFALSE;
	for(j=0;j<64;j++) {
	  if(fRichStep12[i/64][j] == 1) kRichColor1[i/64] = kTRUE;
	}
      }
    }
  }

  if(fRichMultiRB->GetState()) {
    for(i=0;i<6;i++) {
      if(kRichColor1[i]) toGreen(fRichSecModBut[i]->GetId());
      else toDefault(fRichSecModBut[i]->GetId());
      fClient->NeedRedraw(fRichSecModBut[i]);
    }
  }
  createRichModList(-1);

}


//---------------------------------------------------------------------
//******  void HOnlObjBrowser::displayShDS()
//
// This function displays the highlighted SHOWER det set in the subpanel 
// SHOWER.
//
//---------------------------------------------------------------------

void HOnlObjBrowser::displayShDS() {
//
// This function displays the highlighted SHOWER det set in the subpanel 
// SHOWER.
//

  if(!gOnline) return;

  HDetSet *pSet = 0;
  TArrayI ar;
  Int_t idx = -1;
  Int_t i = 0;
  Int_t j = 0;
  TGTextLBEntry *pEntry = 0;

  pEntry = (TGTextLBEntry*) fObjList->GetSelectedEntry();
  if(!pEntry) return;

  idx = gOnline->getDetSet()->find(pEntry->GetText()->GetString());
  if(idx == -1) return;

  pSet = gOnline->getDetSet()->at(idx);
  if(!pSet) return;
  if(pSet->getDetName() != SHOWER) return;

  if(pSet->isMulti()) {
    fShMultiRB->SetState(kButtonDown);
    controlShDSGui(fShMultiRB);
  }
  else {
    fShSingleRB->SetState(kButtonDown);
    controlShDSGui(fShSingleRB);
  }

  if(pSet->getCategory() == 64) highlight(fShCatBox,"raw");
  if(pSet->getCategory() == 66) highlight(fShCatBox,"cal");
  if(pSet->getCategory() == 65) highlight(fShCatBox,"hit");

  fShNameBuf->Clear();
  fShNameBuf->AddText(0,pSet->getUserWord());
  fClient->NeedRedraw(fShName);

  if(fShSingleRB->GetState()) {
	
    ar = pSet->getSingleAddress();
    if(ar.fN != 4) return;
    fShSecBox->Select(ar[0]+1);
    fShModBox->Select(ar[1]+1);
    fShRowBox->Select(ar[2]+1);
    fShColBox->Select(ar[3]+1);
  }
  else {
    ar = pSet->getMultiAddress();
    if(ar.fN != 1152) return;
    for(i=0;i<1152;i++) {
      fShStep12[i/64][i%64] = ar[i];
      if(i%64 == 63) {
	kShColor1[i/64] = kFALSE;
	for(j=0;j<64;j++) {
	  if(fShStep12[i/64][j] == 1) kShColor1[i/64] = kTRUE;
	}
      }
    }
  }

  if(fShMultiRB->GetState()) {
    for(i=0;i<18;i++) {
      if(kShColor1[i]) toGreen(fShSecModBut[i]->GetId());
      else toDefault(fShSecModBut[i]->GetId());
      fClient->NeedRedraw(fShSecModBut[i]);
    }
  }
  createShModList(-1);

}

  
//---------------------------------------------------------------------
//******  Bool_t HOnlObjBrowser::toDefault(Window_t id)
//
// This function changes the button color to default one (gray).
//
//---------------------------------------------------------------------

Bool_t HOnlObjBrowser::toDefault(Window_t id) {
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
//******  Bool_t HOnlObjBrowser::toGreen(Window_t id)
//
// This function changes the button color to green.
//
//---------------------------------------------------------------------

Bool_t HOnlObjBrowser::toGreen(Window_t id) {
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
//******  Bool_t HOnlObjBrowser::toRed(Window_t id)
//
// This function changes the button color to red.
//
//---------------------------------------------------------------------

Bool_t HOnlObjBrowser::toRed(Window_t id) {
//
// This function changes the button color to red.
//

  SetWindowAttributes_t wattr;
  wattr.fMask = kWABackPixel;
  gClient->GetColorByName("red", wattr.fBackgroundPixel);
  gVirtualX->ChangeWindowAttributes(id, &wattr);
  return kTRUE;

}


//---------------------------------------------------------------------
//******  void HOnlObjBrowser::displayMdcLayS1()
//
// This function displays the buttons in the subpanel step1.
//
//---------------------------------------------------------------------

void HOnlObjBrowser::displayMdcLayS1() {
//
// This function displays the buttons in the subpanel step1.
//

  Int_t i = 0;
  Int_t imin, imax, jmin, jmax;
  Int_t j = 0;
  
  imin = imax = jmin = jmax = -1;

  if(fMdcAddDefRB->GetState() && countMdcS1 == 1) {

    i = 4*idxiMdcS1[0] + idxjMdcS1[0];
    kMdcColor1[i] = kTRUE;
    toGreen(fMdcSecModBut[i]->GetId());
    fClient->NeedRedraw(fMdcSecModBut[i]);

  }

  if(fMdcSubDefRB->GetState() && countMdcS1 == 1) {

    i = 4*idxiMdcS1[0] + idxjMdcS1[0];
    kMdcColor1[i] = kFALSE;
    for(j=0; j<48; j++) fMdcStep12[i][j] = 0;
    toDefault(fMdcSecModBut[i]->GetId());
    fClient->NeedRedraw(fMdcSecModBut[i]);

  }

  if(fMdcRecDefRB->GetState() && fMdcAddDefRB->GetState() &&
     countMdcS1 == 2) {

    imin = (idxiMdcS1[0] < idxiMdcS1[1]) ? idxiMdcS1[0] : idxiMdcS1[1];
    imax = (idxiMdcS1[0] > idxiMdcS1[1]) ? idxiMdcS1[0] : idxiMdcS1[1];
    jmin = (idxjMdcS1[0] < idxjMdcS1[1]) ? idxjMdcS1[0] : idxjMdcS1[1];
    jmax = (idxjMdcS1[0] > idxjMdcS1[1]) ? idxjMdcS1[0] : idxjMdcS1[1];

    for(i=0; i<24; i++) {

      if(i/4 >= imin && i/4 <= imax && i%4 >= jmin && i%4 <= jmax) {
	kMdcColor1[i] = kTRUE;
	toGreen(fMdcSecModBut[i]->GetId());
	fClient->NeedRedraw(fMdcSecModBut[i]);
      }
    }

  }

  if(fMdcRecDefRB->GetState() && fMdcSubDefRB->GetState() &&
     countMdcS1 == 2) {

    imin = (idxiMdcS1[0] < idxiMdcS1[1]) ? idxiMdcS1[0] : idxiMdcS1[1];
    imax = (idxiMdcS1[0] > idxiMdcS1[1]) ? idxiMdcS1[0] : idxiMdcS1[1];
    jmin = (idxjMdcS1[0] < idxjMdcS1[1]) ? idxjMdcS1[0] : idxjMdcS1[1];
    jmax = (idxjMdcS1[0] > idxjMdcS1[1]) ? idxjMdcS1[0] : idxjMdcS1[1];

    for(i=0; i<24; i++) {

      if(i/4 >= imin && i/4 <= imax && i%4 >= jmin && i%4 <= jmax) {
	kMdcColor1[i] = kFALSE;
	for(j=0; j<48; j++) fMdcStep12[i][j] = 0;
	toDefault(fMdcSecModBut[i]->GetId());
	fClient->NeedRedraw(fMdcSecModBut[i]);
      }
    }

  }

  if(fMdcSinDefRB->GetState()) countMdcS1 = 2;

}


//---------------------------------------------------------------------
//******  void HOnlObjBrowser::displayMdcLayS2()
//
// This function displays the buttons in the subpanel step2.
//
//---------------------------------------------------------------------

void HOnlObjBrowser::displayMdcLayS2() {
//
// This function displays the buttons in the subpanel step2.
//

  Int_t i = 0;
  Int_t imin, imax, jmin, jmax;
  
  imin = imax = jmin = jmax = -1;

  if(fMdcAddDefRB->GetState() && countMdcS2 == 1) {

    i = 8*idxiMdcS2[0] + idxjMdcS2[0];
    kMdcColor2[i] = kTRUE;
    toGreen(fMdcCellsBut[i]->GetId());
    fClient->NeedRedraw(fMdcCellsBut[i]);

  }

  if(fMdcSubDefRB->GetState() && countMdcS2 == 1) {

    i = 8*idxiMdcS2[0] + idxjMdcS2[0];
    kMdcColor2[i] = kFALSE;
    toDefault(fMdcCellsBut[i]->GetId());
    fClient->NeedRedraw(fMdcCellsBut[i]);

  }

  if(fMdcRecDefRB->GetState() && fMdcAddDefRB->GetState() &&
     countMdcS2 == 2) {

    imin = (idxiMdcS2[0] < idxiMdcS2[1]) ? idxiMdcS2[0] : idxiMdcS2[1];
    imax = (idxiMdcS2[0] > idxiMdcS2[1]) ? idxiMdcS2[0] : idxiMdcS2[1];
    jmin = (idxjMdcS2[0] < idxjMdcS2[1]) ? idxjMdcS2[0] : idxjMdcS2[1];
    jmax = (idxjMdcS2[0] > idxjMdcS2[1]) ? idxjMdcS2[0] : idxjMdcS2[1];

    for(i=0; i<48; i++) {

      if(i/8 >= imin && i/8 <= imax && i%8 >= jmin && i%8 <= jmax) {
	kMdcColor2[i] = kTRUE;
	toGreen(fMdcCellsBut[i]->GetId());
	fClient->NeedRedraw(fMdcCellsBut[i]);
      }
    }

  }

  if(fMdcRecDefRB->GetState() && fMdcSubDefRB->GetState() &&
     countMdcS2 == 2) {

    imin = (idxiMdcS2[0] < idxiMdcS2[1]) ? idxiMdcS2[0] : idxiMdcS2[1];
    imax = (idxiMdcS2[0] > idxiMdcS2[1]) ? idxiMdcS2[0] : idxiMdcS2[1];
    jmin = (idxjMdcS2[0] < idxjMdcS2[1]) ? idxjMdcS2[0] : idxjMdcS2[1];
    jmax = (idxjMdcS2[0] > idxjMdcS2[1]) ? idxjMdcS2[0] : idxjMdcS2[1];

    for(i=0; i<48; i++) {

      if(i/8 >= imin && i/8 <= imax && i%8 >= jmin && i%8 <= jmax) {
	kMdcColor2[i] = kFALSE;
	toDefault(fMdcCellsBut[i]->GetId());
	fClient->NeedRedraw(fMdcCellsBut[i]);
      }
    }

  }

  if(fMdcSinDefRB->GetState()) { countMdcS1 = 2; countMdcS2 = 2; }

}


//---------------------------------------------------------------------
//******  void HOnlObjBrowser::displayRichLayS1()
//
// This function displays the buttons in the subpanel step1.
//
//---------------------------------------------------------------------

void HOnlObjBrowser::displayRichLayS1() {
//
// This function displays the buttons in the subpanel step1.
//

  Int_t i = 0;
  Int_t imin, imax, jmin, jmax;
  Int_t j = 0;
  
  imin = imax = jmin = jmax = -1;

  if(fRichAddDefRB->GetState() && countRichS1 == 1) {

    i = idxiRichS1[0] + idxjRichS1[0];
    kRichColor1[i] = kTRUE;
    toGreen(fRichSecModBut[i]->GetId());
    fClient->NeedRedraw(fRichSecModBut[i]);

  }

  if(fRichSubDefRB->GetState() && countRichS1 == 1) {

    i = idxiRichS1[0] + idxjRichS1[0];
    kRichColor1[i] = kFALSE;
    for(j=0; j<64; j++) fRichStep12[i][j] = 0;
    toDefault(fRichSecModBut[i]->GetId());
    fClient->NeedRedraw(fRichSecModBut[i]);

  }

  if(fRichRecDefRB->GetState() && fRichAddDefRB->GetState() &&
     countRichS1 == 2) {

    imin = (idxiRichS1[0] < idxiRichS1[1]) ? idxiRichS1[0] : idxiRichS1[1];
    imax = (idxiRichS1[0] > idxiRichS1[1]) ? idxiRichS1[0] : idxiRichS1[1];
    jmin = (idxjRichS1[0] < idxjRichS1[1]) ? idxjRichS1[0] : idxjRichS1[1];
    jmax = (idxjRichS1[0] > idxjRichS1[1]) ? idxjRichS1[0] : idxjRichS1[1];

    for(i=0; i<6; i++) {

      if(i >= imin && i <= imax) {
	kRichColor1[i] = kTRUE;
	toGreen(fRichSecModBut[i]->GetId());
	fClient->NeedRedraw(fRichSecModBut[i]);
      }
    }

  }

  if(fRichRecDefRB->GetState() && fRichSubDefRB->GetState() &&
     countRichS1 == 2) {

    imin = (idxiRichS1[0] < idxiRichS1[1]) ? idxiRichS1[0] : idxiRichS1[1];
    imax = (idxiRichS1[0] > idxiRichS1[1]) ? idxiRichS1[0] : idxiRichS1[1];
    jmin = (idxjRichS1[0] < idxjRichS1[1]) ? idxjRichS1[0] : idxjRichS1[1];
    jmax = (idxjRichS1[0] > idxjRichS1[1]) ? idxjRichS1[0] : idxjRichS1[1];

    for(i=0; i<6; i++) {

      if(i >= imin && i <= imax) {
	kRichColor1[i] = kFALSE;
	for(j=0; j<64; j++) fRichStep12[i][j] = 0;
	toDefault(fRichSecModBut[i]->GetId());
	fClient->NeedRedraw(fRichSecModBut[i]);
      }
    }

  }

  if(fRichSinDefRB->GetState()) countRichS1 = 2;

}


//---------------------------------------------------------------------
//******  void HOnlObjBrowser::displayRichLayS2()
//
// This function displays the buttons in the subpanel step2.
//
//---------------------------------------------------------------------

void HOnlObjBrowser::displayRichLayS2() {
//
// This function displays the buttons in the subpanel step2.
//

  Int_t i = 0;
  Int_t imin, imax, jmin, jmax;
  
  imin = imax = jmin = jmax = -1;

  if(fRichAddDefRB->GetState() && countRichS2 == 1) {

    i = 8*idxiRichS2[0] + idxjRichS2[0];
    kRichColor2[i] = kTRUE;
    toGreen(fRichCellsBut[i]->GetId());
    fClient->NeedRedraw(fRichCellsBut[i]);

  }

  if(fRichSubDefRB->GetState() && countRichS2 == 1) {

    i = 8*idxiRichS2[0] + idxjRichS2[0];
    kRichColor2[i] = kFALSE;
    toDefault(fRichCellsBut[i]->GetId());
    fClient->NeedRedraw(fRichCellsBut[i]);

  }

  if(fRichRecDefRB->GetState() && fRichAddDefRB->GetState() &&
     countRichS2 == 2) {

    imin = (idxiRichS2[0] < idxiRichS2[1]) ? idxiRichS2[0] : idxiRichS2[1];
    imax = (idxiRichS2[0] > idxiRichS2[1]) ? idxiRichS2[0] : idxiRichS2[1];
    jmin = (idxjRichS2[0] < idxjRichS2[1]) ? idxjRichS2[0] : idxjRichS2[1];
    jmax = (idxjRichS2[0] > idxjRichS2[1]) ? idxjRichS2[0] : idxjRichS2[1];

    for(i=0; i<64; i++) {

      if(i/8 >= imin && i/8 <= imax && i%8 >= jmin && i%8 <= jmax) {
	kRichColor2[i] = kTRUE;
	toGreen(fRichCellsBut[i]->GetId());
	fClient->NeedRedraw(fRichCellsBut[i]);
      }
    }

  }

  if(fRichRecDefRB->GetState() && fRichSubDefRB->GetState() &&
     countRichS2 == 2) {

    imin = (idxiRichS2[0] < idxiRichS2[1]) ? idxiRichS2[0] : idxiRichS2[1];
    imax = (idxiRichS2[0] > idxiRichS2[1]) ? idxiRichS2[0] : idxiRichS2[1];
    jmin = (idxjRichS2[0] < idxjRichS2[1]) ? idxjRichS2[0] : idxjRichS2[1];
    jmax = (idxjRichS2[0] > idxjRichS2[1]) ? idxjRichS2[0] : idxjRichS2[1];

    for(i=0; i<64; i++) {

      if(i/8 >= imin && i/8 <= imax && i%8 >= jmin && i%8 <= jmax) {
	kRichColor2[i] = kFALSE;
	toDefault(fRichCellsBut[i]->GetId());
	fClient->NeedRedraw(fRichCellsBut[i]);
      }
    }

  }

  if(fRichSinDefRB->GetState()) { countRichS1 = 2; countRichS2 = 2; }

}


//---------------------------------------------------------------------
//******  void HOnlObjBrowser::displayShLayS1()
//
// This function displays the buttons in the subpanel step1.
//
//---------------------------------------------------------------------

void HOnlObjBrowser::displayShLayS1() {
//
// This function displays the buttons in the subpanel step1.
//

  Int_t i = 0;
  Int_t imin, imax, jmin, jmax;
  Int_t j = 0;
  
  imin = imax = jmin = jmax = -1;

  if(fShAddDefRB->GetState() && countShS1 == 1) {

    i = 3*idxiShS1[0] + idxjShS1[0];
    kShColor1[i] = kTRUE;
    toGreen(fShSecModBut[i]->GetId());
    fClient->NeedRedraw(fShSecModBut[i]);

  }

  if(fShSubDefRB->GetState() && countShS1 == 1) {

    i = 3*idxiShS1[0] + idxjShS1[0];
    kShColor1[i] = kFALSE;
    for(j=0; j<64; j++) fShStep12[i][j] = 0;
    toDefault(fShSecModBut[i]->GetId());
    fClient->NeedRedraw(fShSecModBut[i]);

  }

  if(fShRecDefRB->GetState() && fShAddDefRB->GetState() &&
     countShS1 == 2) {

    imin = (idxiShS1[0] < idxiShS1[1]) ? idxiShS1[0] : idxiShS1[1];
    imax = (idxiShS1[0] > idxiShS1[1]) ? idxiShS1[0] : idxiShS1[1];
    jmin = (idxjShS1[0] < idxjShS1[1]) ? idxjShS1[0] : idxjShS1[1];
    jmax = (idxjShS1[0] > idxjShS1[1]) ? idxjShS1[0] : idxjShS1[1];

    for(i=0; i<18; i++) {

      if(i/3 >= imin && i/3 <= imax && i%3 >= jmin && i%3 <= jmax) {
	kShColor1[i] = kTRUE;
	toGreen(fShSecModBut[i]->GetId());
	fClient->NeedRedraw(fShSecModBut[i]);
      }
    }

  }

  if(fShRecDefRB->GetState() && fShSubDefRB->GetState() &&
     countShS1 == 2) {

    imin = (idxiShS1[0] < idxiShS1[1]) ? idxiShS1[0] : idxiShS1[1];
    imax = (idxiShS1[0] > idxiShS1[1]) ? idxiShS1[0] : idxiShS1[1];
    jmin = (idxjShS1[0] < idxjShS1[1]) ? idxjShS1[0] : idxjShS1[1];
    jmax = (idxjShS1[0] > idxjShS1[1]) ? idxjShS1[0] : idxjShS1[1];

    for(i=0; i<18; i++) {

      if(i/3 >= imin && i/3 <= imax && i%3 >= jmin && i%3 <= jmax) {
	kShColor1[i] = kFALSE;
	for(j=0; j<64; j++) fShStep12[i][j] = 0;
	toDefault(fShSecModBut[i]->GetId());
	fClient->NeedRedraw(fShSecModBut[i]);
      }
    }

  }

  if(fShSinDefRB->GetState()) countShS1 = 2;

}


//---------------------------------------------------------------------
//******  void HOnlObjBrowser::displayShLayS2()
//
// This function displays the buttons in the subpanel step2.
//
//---------------------------------------------------------------------

void HOnlObjBrowser::displayShLayS2() {
//
// This function displays the buttons in the subpanel step2.
//

  Int_t i = 0;
  Int_t imin, imax, jmin, jmax;
  
  imin = imax = jmin = jmax = -1;

  if(fShAddDefRB->GetState() && countShS2 == 1) {

    i = 8*idxiShS2[0] + idxjShS2[0];
    kShColor2[i] = kTRUE;
    toGreen(fShCellsBut[i]->GetId());
    fClient->NeedRedraw(fShCellsBut[i]);

  }

  if(fShSubDefRB->GetState() && countShS2 == 1) {

    i = 8*idxiShS2[0] + idxjShS2[0];
    kShColor2[i] = kFALSE;
    toDefault(fShCellsBut[i]->GetId());
    fClient->NeedRedraw(fShCellsBut[i]);

  }

  if(fShRecDefRB->GetState() && fShAddDefRB->GetState() &&
     countShS2 == 2) {

    imin = (idxiShS2[0] < idxiShS2[1]) ? idxiShS2[0] : idxiShS2[1];
    imax = (idxiShS2[0] > idxiShS2[1]) ? idxiShS2[0] : idxiShS2[1];
    jmin = (idxjShS2[0] < idxjShS2[1]) ? idxjShS2[0] : idxjShS2[1];
    jmax = (idxjShS2[0] > idxjShS2[1]) ? idxjShS2[0] : idxjShS2[1];

    for(i=0; i<64; i++) {

      if(i/8 >= imin && i/8 <= imax && i%8 >= jmin && i%8 <= jmax) {
	kShColor2[i] = kTRUE;
	toGreen(fShCellsBut[i]->GetId());
	fClient->NeedRedraw(fShCellsBut[i]);
      }
    }

  }

  if(fShRecDefRB->GetState() && fShSubDefRB->GetState() &&
     countShS2 == 2) {

    imin = (idxiShS2[0] < idxiShS2[1]) ? idxiShS2[0] : idxiShS2[1];
    imax = (idxiShS2[0] > idxiShS2[1]) ? idxiShS2[0] : idxiShS2[1];
    jmin = (idxjShS2[0] < idxjShS2[1]) ? idxjShS2[0] : idxjShS2[1];
    jmax = (idxjShS2[0] > idxjShS2[1]) ? idxjShS2[0] : idxjShS2[1];

    for(i=0; i<64; i++) {

      if(i/8 >= imin && i/8 <= imax && i%8 >= jmin && i%8 <= jmax) {
	kShColor2[i] = kFALSE;
	toDefault(fShCellsBut[i]->GetId());
	fClient->NeedRedraw(fShCellsBut[i]);
      }
    }

  }

  if(fShSinDefRB->GetState()) { countShS1 = 2; countShS2 = 2; }

}


//---------------------------------------------------------------------
//******  void HOnlObjBrowser::displayTofLay()
//
// This function displays the buttons in the TOF subpanel.
//
//---------------------------------------------------------------------

void HOnlObjBrowser::displayTofLay() {
//
// This function displays the buttons in the TOF subpanel.
//

  Int_t i = 0;
  Int_t imin, imax, jmin, jmax;
  
  imin = imax = jmin = jmax = -1;

  if(fTofAddDefRB->GetState() && countTof == 1) {

    i = 8*idxiTof[0] + idxjTof[0];
    kTofColor[i] = kTRUE;
    toGreen(fTofSecModBut[i]->GetId());
    fClient->NeedRedraw(fTofSecModBut[i]);

  }

  if(fTofSubDefRB->GetState() && countTof == 1) {

    i = 8*idxiTof[0] + idxjTof[0];
    kTofColor[i] = kFALSE;
    toDefault(fTofSecModBut[i]->GetId());
    fClient->NeedRedraw(fTofSecModBut[i]);

  }

  if(fTofRecDefRB->GetState() && fTofAddDefRB->GetState() &&
     countTof == 2) {

    imin = (idxiTof[0] < idxiTof[1]) ? idxiTof[0] : idxiTof[1];
    imax = (idxiTof[0] > idxiTof[1]) ? idxiTof[0] : idxiTof[1];
    jmin = (idxjTof[0] < idxjTof[1]) ? idxjTof[0] : idxjTof[1];
    jmax = (idxjTof[0] > idxjTof[1]) ? idxjTof[0] : idxjTof[1];

    for(i=0; i<48; i++) {

      if(i/8 >= imin && i/8 <= imax && i%8 >= jmin && i%8 <= jmax) {
	kTofColor[i] = kTRUE;
	toGreen(fTofSecModBut[i]->GetId());
	fClient->NeedRedraw(fTofSecModBut[i]);
      }
    }

  }

  if(fTofRecDefRB->GetState() && fTofSubDefRB->GetState() &&
     countTof == 2) {

    imin = (idxiTof[0] < idxiTof[1]) ? idxiTof[0] : idxiTof[1];
    imax = (idxiTof[0] > idxiTof[1]) ? idxiTof[0] : idxiTof[1];
    jmin = (idxjTof[0] < idxjTof[1]) ? idxjTof[0] : idxjTof[1];
    jmax = (idxjTof[0] > idxjTof[1]) ? idxjTof[0] : idxjTof[1];

    for(i=0; i<48; i++) {

      if(i/8 >= imin && i/8 <= imax && i%8 >= jmin && i%8 <= jmax) {
	kTofColor[i] = kFALSE;
	toDefault(fTofSecModBut[i]->GetId());
	fClient->NeedRedraw(fTofSecModBut[i]);
      }
    }

  }

  if(fTofSinDefRB->GetState()) { countTof = 2; }

}


//---------------------------------------------------------------------
//******  const char* HOnlObjBrowser::getBeforeUnderline(const char *word)
//
// This function extracts the part of the word before underline.
//
//---------------------------------------------------------------------

const char* HOnlObjBrowser::getBeforeUnderline(const char *word) {
//
// This function extracts the part of the word before underline.
//

  if(!word) return 0;
  char tmp[50];
  char nulstr[50] = " ";
  Int_t i;

  sprintf(tmp,"%s",word);
  strncpy(wordpart,&nulstr[0],50);
  for(i = (strlen(tmp)-1); tmp[i] != '_' && i>-1; i--);
  strncpy(wordpart,&tmp[0],i);
 
  return wordpart;                                               

}


//---------------------------------------------------------------------
//******  const char* HOnlObjBrowser::getAfterUnderline(const char *word)
//
// This function extracts the part of the word after underline.
//
//---------------------------------------------------------------------

const char* HOnlObjBrowser::getAfterUnderline(const char *word) {
//
// This function extracts the part of the word after underline.
//

  if(!word) return 0;
  char tmp[50];
  char nulstr[50] = " ";
  Int_t i;

  sprintf(tmp,"%s",word);
  strncpy(wordpart,&nulstr[0],50);
  for(i = (strlen(tmp)-1); tmp[i] != '_' && i>-1; i--);
  strncpy(wordpart,&tmp[i+1],strlen(tmp)-i);
 
  return wordpart;                                               

}




