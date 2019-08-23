#if !defined HCONFIGRUN_H
#define HCONFIGRUN_H

//-------------------------------------------------------------------------
//
// File name:       hconfigrun.h
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
// This file contains the declaration of the class HConfigRun.
// This is a GUI class for definition of the configuration for the analysis
// run.
//-------------------------------------------------------------------------

#include "TGTextEntry.h"
#include "TGTextBuffer.h"
#include "TGButton.h"

enum ConfRunId {

  M_CR_HLD = 100,
  M_CR_DAQ,
  M_CR_ROOT,
  M_CR_SET,
  M_CR_READ,
  M_CR_WRITE,
  M_CR_CANCEL

};

class HConfigRun : public TGMainFrame {

 protected:

  TGCompositeFrame         *fFrame;
  TGCompositeFrame         *fF1, *fF2, *fF3, *fF4, *fF5;
  TGCompositeFrame         *fF11, *fF12;
  TGCompositeFrame         *fF31, *fF32;
  TGCompositeFrame         *fF321, *fF322;
  TGButton                 *fGeomB[24];
  TGButton                 *fSetB, *fReadB, *fWriteB, *fCancelB;
  TGRadioButton            *fHldRB, *fDaqRB, *fRootRB;
  TGTextEntry              *fInput, *fOutput;
  TGTextBuffer             *fInputBuf, *fOutputBuf;

 protected:

  TGMainFrame              *fMainWindow;

 public:

  HConfigRun(const TGWindow* p, UInt_t w, UInt_t h, TGMainFrame *pp);
  virtual ~HConfigRun();
  virtual void CloseWindow();
  virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);
  void readConfig() {}
  void saveConfig() {}
  Bool_t setConfig() {return kFALSE;}
  Bool_t toGreen(Window_t id);
  Bool_t toDefault(Window_t id);

 public:

  ClassDef(HConfigRun,0) //GUI panel for configuration of the analysis run

};
  
#endif






