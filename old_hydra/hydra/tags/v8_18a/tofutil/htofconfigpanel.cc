using namespace std;
#include "htofconfigpanel.h"
#include "htofeventdisplay.h"
#include "htofsector.h"
#include <iostream> 
#include <iomanip>
#include <TGLabel.h>
#include <TList.h>
#include <TCanvas.h>
#include <TGFileDialog.h>
#include <TGMsgBox.h>
#include <TSystem.h>

//-------------------------------------------------------------
//
// Author : Dmitry Vasiliev (LNS, Catania)
//
//-------------------------------------------------------------
//-------------------------------------------------------------
//
// HTofConfigPanel class
//
// a panel to set configuration for the analysis session
//
//-------------------------------------------------------------

ClassImp(HTofConfigPanel)

TCanvas *canv;
Bool_t kColor[6];
const Int_t mask[6] = { 5, 0, 1, 4, 3, 2 };

HTofConfigPanel::HTofConfigPanel(const TGWindow *p, UInt_t w, UInt_t h) :
                               TGMainFrame(p, w, h) {

Char_t tmp[30];
Int_t j;

fFrame = new TGCompositeFrame(this, 700, 600, kVerticalFrame);
AddFrame(fFrame, new TGLayoutHints(kLHintsNormal));

fF1 = new TGCompositeFrame(fFrame, 700, 300, kVerticalFrame);
fFrame->AddFrame(fF1, new TGLayoutHints(kLHintsNormal));

fF2 = new TGGroupFrame(fFrame, "geometry", kHorizontalFrame);
fFrame->AddFrame(fF2, new TGLayoutHints(kLHintsNormal,5,5,0,0));

fF3 = new TGCompositeFrame(fFrame, 700, 100, kHorizontalFrame);
fF3->SetLayoutManager(new TGMatrixLayout(fF3,0,4,15));
fFrame->AddFrame(fF3, new TGLayoutHints(kLHintsNormal|kLHintsCenterX));

//----------------------------------------------------------------

fA1 = new TGCompositeFrame(fF1, 700, 300, kHorizontalFrame);
fF1->AddFrame(fA1, new TGLayoutHints(kLHintsNormal));

fA2 = new TGCompositeFrame(fF1, 700, 100, kHorizontalFrame);
fF1->AddFrame(fA2, new TGLayoutHints(kLHintsNormal));

//------------------------------

fC1 = new TGGroupFrame(fA1, "run", kVerticalFrame);
fA1->AddFrame(fC1, new TGLayoutHints(kLHintsNormal, 5,5,2,2));

fC2 = new TGGroupFrame(fA1, "calibration", kVerticalFrame);
fA1->AddFrame(fC2, new TGLayoutHints(kLHintsNormal, 5,5,2,2));

fB1 = new TGCompositeFrame(fA2, 500, 200, kVerticalFrame);
fA2->AddFrame(fB1, new TGLayoutHints(kLHintsNormal, 5,5,2,2));

fC3 = new TGGroupFrame(fB1, "input", kVerticalFrame);
fB1->AddFrame(fC3, new TGLayoutHints(kLHintsNormal));

fC4 = new TGGroupFrame(fB1, "output", kHorizontalFrame);
fB1->AddFrame(fC4, new TGLayoutHints(kLHintsNormal));

//-------------task list panel------------------

fB2 = new TGGroupFrame(fA2, "task list", kVerticalFrame);
fA2->AddFrame(fB2, new TGLayoutHints(kLHintsNormal, 5,5,2,0));

fTask = new TGListBox(fB2, -1);
fB2->AddFrame(fTask, new TGLayoutHints(kLHintsNormal,0,0,3,0));
fTask->Resize(88,140);

//-------------run panel----------------------

fBeam = new TGRadioButton(fC1, "beam run", M_BEAM);
fBeam->Associate(this);
fC1->AddFrame(fBeam, new TGLayoutHints(kLHintsNormal, 5,5,13,5));

fLaser = new TGRadioButton(fC1, "laser run", M_LASER);
fLaser->Associate(this);
fC1->AddFrame(fLaser, new TGLayoutHints(kLHintsNormal, 5,5,5,4));

//-------------input panel---------

fD1 = new TGCompositeFrame(fC3, 500, 300, kHorizontalFrame);
fC3->AddFrame(fD1, new TGLayoutHints(kLHintsNormal, 5,5,5,5));

fD2 = new TGCompositeFrame(fC3, 500, 300, kHorizontalFrame);
fC3->AddFrame(fD2, new TGLayoutHints(kLHintsNormal, 5,5,5,5));

fRoot = new TGRadioButton(fD1, "root", M_ROOT);
fRoot->Associate(this);
fD1->AddFrame(fRoot, new TGLayoutHints(kLHintsNormal, 15,5,5,5));

fHld = new TGRadioButton(fD1, "hld", M_HLD);
fHld->Associate(this);
fD1->AddFrame(fHld, new TGLayoutHints(kLHintsNormal, 5,5,5,5));

fDaq = new TGRadioButton(fD1, "daq", M_DAQ);
fDaq->Associate(this);
fD1->AddFrame(fDaq, new TGLayoutHints(kLHintsNormal, 5,5,5,5));

fD2->AddFrame(new TGLabel(fD2, new TGHotString("source name")),
              new TGLayoutHints(kLHintsNormal,0,0,4,0));

fStext = new TGTextEntry(fD2, fSbuf = new TGTextBuffer(100), -1);
fD2->AddFrame(fStext, new TGLayoutHints(kLHintsNormal, 10,2, 2, 2));
fStext->Resize(100, fStext->GetDefaultHeight());

//-------------output panel------------------

fC4->AddFrame(new TGLabel(fC4, new TGHotString("file name")),
              new TGLayoutHints(kLHintsNormal,27,0,10,0));

fTtext = new TGTextEntry(fC4, fTbuf = new TGTextBuffer(100), -1);
fC4->AddFrame(fTtext, new TGLayoutHints(kLHintsNormal, 10, 7, 8, 0));
fTtext->Resize(100, fTtext->GetDefaultHeight());

//-------------calibration panel-----------------

fE1 = new TGCompositeFrame(fC2, 500, 300, kHorizontalFrame);
fC2->AddFrame(fE1, new TGLayoutHints(kLHintsNormal));

fE2 = new TGCompositeFrame(fC2, 500, 300, kHorizontalFrame);
fC2->AddFrame(fE2, new TGLayoutHints(kLHintsNormal,0,0,7,0));

fCalib = new TGCheckButton(fE1, "calibration parameters", M_CALIB);
fCalib->Associate(this);
fE1->AddFrame(fCalib, new TGLayoutHints(kLHintsNormal, 27,25,5,5));

fE2->AddFrame(new TGLabel(fE2, new TGHotString("file name")),
              new TGLayoutHints(kLHintsNormal,15,0,8,0));

fCtext = new TGTextEntry(fE2, fCbuf = new TGTextBuffer(100), -1);
fE2->AddFrame(fCtext, new TGLayoutHints(kLHintsNormal, 10,0, 5, 0));
fCtext->Resize(100, fCtext->GetDefaultHeight());
fCtext->SetState(kFALSE);

//--------------geometry panel------------

fCanvas = new TRootEmbeddedCanvas("canvas", fF2, 100, 100);
fF2->AddFrame(fCanvas, new TGLayoutHints(kLHintsNormal, 2,2,5,0));
canv = fCanvas->GetCanvas();
canv->SetFillColor(1);
canv->cd();
canv->Update();
canv->Modified();

fG1 = new TGGroupFrame(fF2, "sectors", kVerticalFrame);
fG1->SetLayoutManager(new TGMatrixLayout(fG1,0,3,14));
fF2->AddFrame(fG1, new TGLayoutHints(kLHintsNormal, 5,0,0,0));

SetWindowAttributes_t wattr;
wattr.fMask = kWABackPixel;
gClient->GetColorByName("white", wattr.fBackgroundPixel);

for(Int_t i=0; i<6; i++) {
  j = mask[i];
  sprintf(tmp,"%s%i","sector &",j+1);
  fSec[j] = new TGTextButton(fG1,tmp,101+j);
  fSec[j]->Associate(this);
  fG1->AddFrame(fSec[j]);
  fSec[j]->Resize(55,fSec[j]->GetDefaultHeight());
  gVirtualX->ChangeWindowAttributes(fSec[j]->GetId(), &wattr);
  kColor[j] = kFALSE;
}

//-------------------------------------

fSet = new TGTextButton(fF3, "&OK", M_SET);
fSet->Associate(this);
fSet->SetToolTipText("Set configuration");
fF3->AddFrame(fSet);
fSet->Resize(60, fSet->GetDefaultHeight());

fRead = new TGTextButton(fF3, "&Read", M_READ);
fRead->Associate(this);
fRead->SetToolTipText("Read configuration");
fF3->AddFrame(fRead);
fRead->Resize(60, fRead->GetDefaultHeight());

fSave = new TGTextButton(fF3, "S&ave as", M_SAVE);
fSave->Associate(this);
fSave->SetToolTipText("Save configuration");
fF3->AddFrame(fSave);
fSave->Resize(60, fSave->GetDefaultHeight());

fClose = new TGTextButton(fF3, "&Cancel", M_CLOSE);
fClose->Associate(this);
fClose->SetToolTipText("Close window");
fF3->AddFrame(fClose);
fClose->Resize(60, fClose->GetDefaultHeight());

//------------------------------------------------------------

if(gTofDisplay->isConfigured()) {

  setFields();
  setTaskList();
}

else { 

  gTofDisplay->setConfigBit(B_BEAM); // beam run
  gTofDisplay->setConfigBit(B_LASER,kFALSE); // laser run
  gTofDisplay->setConfigBit(B_ROOT,kFALSE); // root source
  gTofDisplay->setConfigBit(B_HLD); // hld source
  gTofDisplay->setConfigBit(B_DAQ,kFALSE); // daq source
  gTofDisplay->setConfigBit(B_CAL,kFALSE); // calib param

  setFields();
  setTaskList();

}

MapSubwindows();

SetWindowName("HADES TOF Configuration Panel");

Resize(GetDefaultSize());
MapWindow();

}


HTofConfigPanel::~HTofConfigPanel() {

delete fClose;
delete fSave;
delete fRead;
delete fSet;
delete fSec[5]; delete fSec[4]; delete fSec[3];
delete fSec[2]; delete fSec[1]; delete fSec[0];
delete fCanvas;
delete fCalib;
delete fRoot;
delete fHld;
delete fDaq;
delete fBeam;
delete fLaser;
delete fCtext;
delete fTtext;
delete fStext;
delete fTask;
delete fG1;
delete fE2; delete fE1;
delete fD2; delete fD1;
delete fC4; delete fC3; delete fC2; delete fC1;
delete fA2; delete fA1;
delete fB2; delete fB1;
delete fF3;
delete fF2;
delete fF1;
delete fFrame;

}


void HTofConfigPanel::toGreen(Window_t id) {

  SetWindowAttributes_t wattr;
  wattr.fMask = kWABackPixel;
  gClient->GetColorByName("green", wattr.fBackgroundPixel);
  gVirtualX->ChangeWindowAttributes(id, &wattr);
}


void HTofConfigPanel::toWhite(Window_t id) {

  SetWindowAttributes_t wattr;
  wattr.fMask = kWABackPixel;
  gClient->GetColorByName("white", wattr.fBackgroundPixel);
  gVirtualX->ChangeWindowAttributes(id, &wattr);
}


void HTofConfigPanel::setTaskList() {

  if(!gTofDisplay) return;

  Char_t *names[] =  {"unpacking", "calibration", "hit finding", "monitoring"}; 
  Char_t tmp[30];
  fTask->RemoveEntries(1,3);
  Int_t zero, one, two, three, four, five;
  zero = (fBeam->GetState()) ? 1 : 0;
  one = (fLaser->GetState()) ? 1 : 0;
  two = (fRoot->GetState()) ? 1 : 0;
  three = (fHld->GetState()) ? 1 : 0;
  four = (fDaq->GetState()) ? 1 : 0;
  five = (fCalib->GetState()) ? 1 : 0;

  Int_t number = 100000*zero + 10000*one + 1000*two + 100*three +
    10*four + five;

  switch(number) {

  case 100100: // only unpacking and montoring (beam run, 
               // data source - hld file)
    sprintf(tmp,"%s",names[0]);
    fTask->AddEntry(tmp,1);
    sprintf(tmp,"%s",names[3]);
    fTask->AddEntry(tmp,2);
    break;

  case 100010: // only unpacking and monitoring (beam run, data source - daq)
    sprintf(tmp,"%s",names[0]);
    fTask->AddEntry(tmp,1);
    sprintf(tmp,"%s",names[3]);
    fTask->AddEntry(tmp,2);
    break;

  case 10100: // only unpacking and monitoring 
              // (laser run, data source - hld file)
    sprintf(tmp,"%s",names[0]);
    fTask->AddEntry(tmp,1);
    sprintf(tmp,"%s",names[3]);
    fTask->AddEntry(tmp,2);
    break;

  case 10010: // only unpacking and monitoring (laser run, data source - daq)
    sprintf(tmp,"%s",names[0]);
    fTask->AddEntry(tmp,1);
    sprintf(tmp,"%s",names[3]);
    fTask->AddEntry(tmp,2);
    break;

  case 101001: // only hit finding and monitoring (data source - root file)
    sprintf(tmp,"%s",names[2]);
    fTask->AddEntry(tmp,1);
    sprintf(tmp,"%s",names[3]);
    fTask->AddEntry(tmp,2);
    break;

  case 11001: // only calibration (data source - root file)
    sprintf(tmp,"%s",names[1]);
    fTask->AddEntry(tmp,1);
    break;

  case 10101: // unpacking and calibration (data source - hld file)
    sprintf(tmp,"%s",names[0]);
    fTask->AddEntry(tmp,1);
    sprintf(tmp,"%s",names[1]);
    fTask->AddEntry(tmp,2);
    break;

  case 10011: // unpacking and calibration (data source - daq)
    sprintf(tmp,"%s",names[0]);
    fTask->AddEntry(tmp,1);
    sprintf(tmp,"%s",names[1]);
    fTask->AddEntry(tmp,2);
    break;

  case 100101: //unpacking, hit finding and monitoring (data source - hld file)
    sprintf(tmp,"%s",names[0]);
    fTask->AddEntry(tmp,1);
    sprintf(tmp,"%s",names[2]);
    fTask->AddEntry(tmp,2);
    sprintf(tmp,"%s",names[3]);
    fTask->AddEntry(tmp,3);
    break;

  case 100011: // unpacking, hit finding and monitoring (data source - daq)
    sprintf(tmp,"%s",names[0]);
    fTask->AddEntry(tmp,1);
    sprintf(tmp,"%s",names[2]);
    fTask->AddEntry(tmp,2);
    sprintf(tmp,"%s",names[3]);
    fTask->AddEntry(tmp,3);
    break;
    
  default:
    if(gTofDisplay->testConfigBit(0) == 1) {
      sprintf(tmp,"%s",names[3]);
      fTask->AddEntry(tmp,1);
    }   
    else fTask->RemoveEntries(1,3);
    break;
  }

  fTask->MapSubwindows();
  fTask->Layout();

}
    

void HTofConfigPanel::setConfigParam() { 

  if(!gTofDisplay) return;

  Char_t nullstring[100] = "";

  if(fBeam->GetState()) gTofDisplay->setConfigBit(B_BEAM);
  else gTofDisplay->setConfigBit(B_BEAM,kFALSE);
  if(fLaser->GetState()) gTofDisplay->setConfigBit(B_LASER);
  else gTofDisplay->setConfigBit(B_LASER,kFALSE);
  if(fRoot->GetState()) gTofDisplay->setConfigBit(B_ROOT);
  else gTofDisplay->setConfigBit(B_ROOT,kFALSE);
  if(fHld->GetState()) gTofDisplay->setConfigBit(B_HLD);
  else gTofDisplay->setConfigBit(B_HLD,kFALSE);
  if(fDaq->GetState()) gTofDisplay->setConfigBit(B_DAQ);
  else gTofDisplay->setConfigBit(B_DAQ,kFALSE);
  if(fCalib->GetState()) gTofDisplay->setConfigBit(B_CAL);
  else gTofDisplay->setConfigBit(B_CAL,kFALSE);
  if(kColor[0]) gTofDisplay->setConfigBit(B_SEC1);
  else gTofDisplay->setConfigBit(B_SEC1,kFALSE);
  if(kColor[1]) gTofDisplay->setConfigBit(B_SEC2);
  else gTofDisplay->setConfigBit(B_SEC2,kFALSE);
  if(kColor[2]) gTofDisplay->setConfigBit(B_SEC3);
  else gTofDisplay->setConfigBit(B_SEC3,kFALSE);
  if(kColor[3]) gTofDisplay->setConfigBit(B_SEC4);
  else gTofDisplay->setConfigBit(B_SEC4,kFALSE);
  if(kColor[4]) gTofDisplay->setConfigBit(B_SEC5);
  else gTofDisplay->setConfigBit(B_SEC5,kFALSE);
  if(kColor[5]) gTofDisplay->setConfigBit(B_SEC6);
  else gTofDisplay->setConfigBit(B_SEC6,kFALSE);

  gTofDisplay->setInput(fSbuf->GetString());
  gTofDisplay->setOutput(fTbuf->GetString());
  if(fCtext->IsEnabled()) gTofDisplay->setCalib(fCbuf->GetString());
  else gTofDisplay->setCalib(nullstring);

}


Bool_t HTofConfigPanel::setConfig() {

  if(!gTofDisplay) return kFALSE;
  return gTofDisplay->setConfiguration();

}


void HTofConfigPanel::saveConfig() {

  if(!gTofDisplay) return;
  const Char_t *filetypes[] = { "All files",     "*",
                              "ROOT files",    "*.root",
                              "ROOT macros",   "*.C",
			      "Config files",  "*.cfg",
                               0,               0 };

  TGFileInfo fi;
  fi.fFileTypes = (const Char_t **)filetypes;  
  Char_t filename[100];
  Char_t message[200];
  Int_t retval;
  Int_t cfgpar;
  Char_t cfgname[100];

  new TGFileDialog(fClient->GetRoot(), this, kFDSave, &fi);

  sprintf(filename,"%s",fi.fFilename);
  if(!strcmp(filename,"(null)")) return;
  sprintf(filename,"%s%s%s",gSystem->WorkingDirectory(),"/",fi.fFilename);
  FILE *fp = fopen(filename,"w");
  if(!fp) { 
    sprintf(message,"%s%s","Unable to open file ",filename);
    new TGMsgBox(fClient->GetRoot(),this,"Error message",
		 message, kMBIconExclamation,kMBOk,&retval);
    return;
  }

  fprintf(fp,"%d%s%s%s\n",111,"   //TOF"," configuration ","flag");
  cfgpar = (fBeam->GetState()) ? 1 : 0;
  fprintf(fp,"%d%s%s%s\n",cfgpar,"   //bit0"," beam ","run");
  cfgpar = (fLaser->GetState()) ? 1 : 0;
  fprintf(fp,"%d%s%s%s\n",cfgpar,"   //bit1"," laser ","run");
  cfgpar = (fRoot->GetState()) ? 1 : 0;
  fprintf(fp,"%d%s%s%s\n",cfgpar,"   //bit2"," root ","source");
  cfgpar = (fHld->GetState()) ? 1 : 0;
  fprintf(fp,"%d%s%s%s\n",cfgpar,"   //bit3"," hld ","source");
  cfgpar = (fDaq->GetState()) ? 1 : 0;
  fprintf(fp,"%d%s%s%s\n",cfgpar,"   //bit4"," daq ","source");
  cfgpar = (fCalib->GetState()) ? 1 : 0;
  fprintf(fp,"%d%s%s%s\n",cfgpar,"   //bit5"," calib ","param");
  cfgpar = (kColor[0]) ? 1 : 0;
  fprintf(fp,"%d%s%s%s\n",cfgpar,"   //bit6"," sector1 ","geom");
  cfgpar = (kColor[1]) ? 1 : 0;
  fprintf(fp,"%d%s%s%s\n",cfgpar,"   //bit7"," sector2 ","geom");
  cfgpar = (kColor[2]) ? 1 : 0;
  fprintf(fp,"%d%s%s%s\n",cfgpar,"   //bit8"," sector3 ","geom");
  cfgpar = (kColor[3]) ? 1 : 0;
  fprintf(fp,"%d%s%s%s\n",cfgpar,"   //bit9"," sector4 ","geom");
  cfgpar = (kColor[4]) ? 1 : 0;
  fprintf(fp,"%d%s%s%s\n",cfgpar,"   //bit10"," sector5 ","geom");
  cfgpar = (kColor[5]) ? 1 : 0;
  fprintf(fp,"%d%s%s%s\n",cfgpar,"   //bit11"," sector6 ","geom");

  sprintf(cfgname,"%s",fSbuf->GetString());
  if(!strcmp(cfgname,""));
  else { 
    fprintf(fp,"%s%s%s%s\n",cfgname,"   //input"," data ","source");
    sprintf(cfgname,"%s",fTbuf->GetString());
    if(!strcmp(cfgname,""));
    else { 
      fprintf(fp,"%s%s%s%s\n",cfgname,"   //output"," data ","file");
      sprintf(cfgname,"%s",fCbuf->GetString());
      if(!strcmp(cfgname,"")); 
      else fprintf(fp,"%s%s%s%s\n",cfgname,"  //calibration"," param ","file");
    }
  }
  fclose(fp);

}


void HTofConfigPanel::readConfig() {

  if(!gTofDisplay) return;
  const Char_t *filetypes[] = { "All files",     "*",
                              "ROOT files",    "*.root",
                              "ROOT macros",   "*.C",
			      "Config files",  "*.cfg",
                               0,               0 };

  TGFileInfo fi;
  fi.fFileTypes = (const Char_t **)filetypes;  
  Char_t filename[100];
  Char_t message[200];
  Char_t first[20], second[20], third[20];
  Int_t retval;
  Int_t check;
  Char_t cfgname[100];
  Int_t i;
  Int_t ncol;

  new TGFileDialog(fClient->GetRoot(), this, kFDOpen, &fi);
  sprintf(filename,"%s%s%s",gSystem->WorkingDirectory(),"/",fi.fFilename);

  FILE *fp = fopen(filename,"r");
  if(!fp) { 
    sprintf(message,"%s%s","Unable to open file ",filename);
    new TGMsgBox(fClient->GetRoot(),this,"Error message",
		 message, kMBIconExclamation,kMBOk,&retval);
    return;
  }

  if(fp) {
    ncol = fscanf(fp, "%d %s%s%s", &check, first,second,third);
    if(check != 111) {
      sprintf(message,"%s","Not a file with Tof configuration");
      new TGMsgBox(fClient->GetRoot(),this,"Error message",
		   message, kMBIconExclamation,kMBOk,&retval);
      return;
    }

    for(i=0; i<12; i++) { 
      ncol = fscanf(fp, "%d %s%s%s", &check, first,second,third);
      if(i==0) { 
	if(check == 1) fBeam->SetState(kButtonDown);
	else fBeam->SetState(kButtonUp);
      }
      if(i==1) { 
	if(check == 1) fLaser->SetState(kButtonDown);
	else fLaser->SetState(kButtonUp);
      }
      if(i==2) { 
	if(check == 1) fRoot->SetState(kButtonDown);
	else fRoot->SetState(kButtonUp);
      }
      if(i==3) { 
	if(check == 1) fHld->SetState(kButtonDown);
	else fHld->SetState(kButtonUp);
      }
      if(i==4) { 
	if(check == 1) fDaq->SetState(kButtonDown);
	else fDaq->SetState(kButtonUp);
      }
      if(i==5) { 
	if(check == 1) fCalib->SetState(kButtonDown);
	else fCalib->SetState(kButtonUp);
	fCtext->SetState(fCalib->GetState());
      }
      if(i > 5) {
	if(check == 1) kColor[i-6] = kFALSE;
	else kColor[i-6] = kTRUE;
	SendMessage(this,MK_MSG(kC_COMMAND,kCM_BUTTON),101+i-6,0);
      }
    }

    ncol = fscanf(fp, "%s %s%s%s", cfgname,first,second,third);
    if(ncol < 0)  {
      fSbuf->Clear();
      fTbuf->Clear();
      fCbuf->Clear();
    }
    else {
      fSbuf->Clear();
      fSbuf->AddText(0,cfgname);
      ncol = fscanf(fp, "%s %s%s%s", cfgname,first,second,third);
      if(ncol < 0) {
	fTbuf->Clear();
	fCbuf->Clear();
      }
      else {
	fTbuf->Clear();
	fTbuf->AddText(0,cfgname);
	ncol = fscanf(fp, "%s %s%s%s", cfgname,first,second,third);
	if(ncol < 0) fCbuf->Clear();
	else {
	  fCbuf->Clear();
	  fCbuf->AddText(0,cfgname); 
	}
      } 
    } 
    fClient->NeedRedraw(fStext);
    fClient->NeedRedraw(fTtext);
    fClient->NeedRedraw(fCtext);
  }

  if(fp) fclose(fp);
  setTaskList();

}


void HTofConfigPanel::setFields() {

  if(!gTofDisplay) return;
  Int_t i;

  if(gTofDisplay->testConfigBit(B_BEAM))
    SendMessage(this,MK_MSG(kC_COMMAND,kCM_RADIOBUTTON),M_BEAM,0);
  if(gTofDisplay->testConfigBit(B_LASER))
    SendMessage(this,MK_MSG(kC_COMMAND,kCM_RADIOBUTTON),M_LASER,0);
  if(gTofDisplay->testConfigBit(B_ROOT)) 
    SendMessage(this,MK_MSG(kC_COMMAND,kCM_RADIOBUTTON),M_ROOT,0);
  if(gTofDisplay->testConfigBit(B_HLD)) 
    SendMessage(this,MK_MSG(kC_COMMAND,kCM_RADIOBUTTON),M_HLD,0);
  if(gTofDisplay->testConfigBit(B_DAQ)) 
    SendMessage(this,MK_MSG(kC_COMMAND,kCM_RADIOBUTTON),M_DAQ,0);
  if(gTofDisplay->testConfigBit(B_CAL)) {
    fCalib->SetState(kButtonDown);
    fCtext->SetState(kTRUE);
  }
  else  {
    fCalib->SetState(kButtonUp);
    fCtext->SetState(kFALSE);
  }

  canv->cd();
  canv->Clear();
  for(i=0; i<6; i++) {
    if(gTofDisplay->testConfigBit(i+6)) kColor[i] = kFALSE;
    else kColor[i] = kTRUE;
    SendMessage(this,MK_MSG(kC_COMMAND,kCM_BUTTON),101+i,0);
  }  

  fSbuf->Clear();
  fSbuf->AddText(0,gTofDisplay->getInput());
  fClient->NeedRedraw(fStext);

  fTbuf->Clear();
  fTbuf->AddText(0,gTofDisplay->getOutput());
  fClient->NeedRedraw(fTtext);

  if(fCalib->GetState()) {
    fCbuf->Clear();
    fCbuf->AddText(0,gTofDisplay->getCalib());
    fClient->NeedRedraw(fCtext);
  }

}


void HTofConfigPanel::CloseWindow() {

delete this;

}


Bool_t HTofConfigPanel::ProcessMessage(Long_t msg,Long_t parm1,Long_t parm2) {

HTofSector *sec;
TIterator *iter;

switch(GET_MSG(msg)) {
 case kC_COMMAND:

  switch(GET_SUBMSG(msg)) {

  case kCM_BUTTON:

    switch(parm1) {

    case M_CLOSE:
      CloseWindow();
      break;

    case M_SET:
      setConfigParam();
      setTaskList();
      setConfig();
      CloseWindow();
      break;

    case M_READ:
      readConfig();
      break;

    case M_SAVE:
      saveConfig();
      break;

    default:
      if(parm1>100 && parm1<=106) {
	kColor[parm1-101] = !kColor[parm1-101];
	if(kColor[parm1-101]) {
	  toGreen(fSec[parm1-101]->GetId());
	  sec = new HTofSector(parm1-100);
	}
	else {
	  toWhite(fSec[parm1-101]->GetId());
	  iter = canv->GetListOfPrimitives()->MakeIterator();
	  while((sec = (HTofSector*) iter->Next())) {
	    if(sec->getNumber() == (parm1-100)) {
	      sec->Delete();
	      sec = 0;
	      break;
	    }
	  }
	}
	fClient->NeedRedraw(fSec[parm1-101]);
	canv->cd();
	if(sec) sec->Draw();
	canv->Modified();
	canv->Update();
      }
      break;
    }

  case kCM_RADIOBUTTON:

    switch(parm1) {

    case M_BEAM:
      fBeam->SetState(kButtonDown);
      fLaser->SetState(kButtonUp);
      setTaskList();
      break;

    case M_LASER:
      fLaser->SetState(kButtonDown);
      fBeam->SetState(kButtonUp);
      setTaskList();
      break;

    case M_ROOT:
      fRoot->SetState(kButtonDown);
      fHld->SetState(kButtonUp);
      fDaq->SetState(kButtonUp);
      setTaskList();
      break;

    case M_HLD:
      fHld->SetState(kButtonDown);
      fRoot->SetState(kButtonUp);
      fDaq->SetState(kButtonUp);
      setTaskList();
      break;

    case M_DAQ:
      fDaq->SetState(kButtonDown);
      fRoot->SetState(kButtonUp);
      fHld->SetState(kButtonUp);
      setTaskList();
      break;

    default:
      break;
    }

  case kCM_CHECKBUTTON:

    switch(parm1) {

    case M_CALIB:
      fCbuf->Clear();
      fClient->NeedRedraw(fCtext);
      if(!fCalib->GetState()) fCtext->SetState(kFALSE);
      else fCtext->SetState(kTRUE);
   
      setTaskList();
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












