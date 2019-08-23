#if !defined HTOFCONFIGPANEL_H
#define HTOFCONFIGPANEL_H

#include <TRootEmbeddedCanvas.h>
#include <TGButton.h>
#include <TGListBox.h>
#include <TGTextEntry.h>
#include <TGTextBuffer.h>

enum CommandsId {

  M_CLOSE = 100,
  M_SECTOR1,
  M_SECTOR2,
  M_SECTOR3,
  M_SECTOR4,
  M_SECTOR5,
  M_SECTOR6,
  M_SET,
  M_READ,
  M_SAVE,
  M_BEAM,
  M_LASER,
  M_ROOT,
  M_HLD,
  M_DAQ,
  M_CALIB
};


class HTofConfigPanel : public TGMainFrame {

 protected:
  void toGreen(Window_t id);
  void toWhite(Window_t id);
  void setTaskList();
  void setConfigParam();
  void setFields();

 protected:
  TGCompositeFrame       *fFrame, *fF1, *fF2, *fF3;
  TGCompositeFrame       *fA1, *fA2;
  TGCompositeFrame       *fB1, *fB2;
  TGCompositeFrame       *fC1, *fC2, *fC3, *fC4;
  TGCompositeFrame       *fD1, *fD2;
  TGCompositeFrame       *fE1, *fE2;
  TGCompositeFrame       *fG1;
  TRootEmbeddedCanvas    *fCanvas;
  TGButton               *fClose, *fSet, *fRead, *fSave;
  TGButton               *fSec[6];
  TGRadioButton          *fBeam, *fLaser;
  TGRadioButton          *fRoot, *fHld, *fDaq;
  TGCheckButton          *fCalib;
  TGTextEntry            *fStext, *fTtext, *fCtext;
  TGTextBuffer           *fSbuf, *fTbuf, *fCbuf;
  TGListBox              *fTask;

 public:
  HTofConfigPanel(const TGWindow *p, UInt_t w, UInt_t h);
  virtual ~HTofConfigPanel();
  virtual void CloseWindow();
  virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);
  void readConfig();
  void saveConfig();
  Bool_t setConfig();

 public:
  ClassDef(HTofConfigPanel,0)
};

#endif





























