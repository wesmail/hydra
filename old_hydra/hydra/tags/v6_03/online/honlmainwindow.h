#if !defined HONLMAINWINDOW_H
#define HONLMAINWINDOW_H

//-------------------------------------------------------------------------
//
// File name:       honlmainwindow.h
// Subject:         Main GUI panel.
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            March 2000
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the declaration of the classes HOnlMainWindow and
// HOnlTimer.
// HOnlMainWindow is a GUI class which maintains the main panel of 
// the program HTOP.
// HOnlTimer is a small helper class which introduces a delay between
// the clicking on the Start button and real execution of the event loop.
//
//-------------------------------------------------------------------------

#include <TGMenu.h>
#include <TGComboBox.h>
#include <TGButton.h>
#include <TTimer.h>
#include <TGTextEntry.h>
#include <TGTextBuffer.h>
#include <TObjArray.h>
#include <TArrayI.h>

enum MainComId {

  M__CONFIGRUN = 100,
  M__OBJMANAGER,
  M__CONTMANAGER,
  M__OBJSAVE,
  M__OBJSAVEAS,
  M__EXIT,
  M__HISTOVISUAL,
  M__HISTACTION,
  M__RUN,
  M__HELP,
  M__SLICE,
  M__CANVAS,
  M__PREF,
  M__CONVERT

};

class HOnlObjBrowser;
class HHistoVisual;
class HTofHistAction;
class HContourManager;
class HConversion;
class TCanvas;

class HOnlMainWindow : public TGMainFrame {

 protected:

  TGCompositeFrame     *fFrame, *fF1;
  TGButton             *fEvLoop;
  TGTextEntry          *fEvents;
  TGTextBuffer         *fEventsBuf;
  TGMenuBar            *fMenuBar;
  TGPopupMenu          *fMenuFile, *fMenuHisto, *fMenuHelp;
  TGPopupMenu          *fMenuOptions;
  TGPopupMenu          *fMenuTools;

 protected:

  HOnlObjBrowser       *fBrowser;
  HHistoVisual         *fVisual;
  HTofHistAction       *fHistAction;
  HConversion          *fConversion;
  HContourManager      *fContManager;
  Int_t                 fCanvW, fCanvH, fCanvCol;
  TCanvas              *fCanvas;

 protected:

  TObjArray            *fArray;
  TArrayI               fXArray;
  TArrayI               fYArray;
  Int_t                 fSliceMode; // 1, 2 or 3

 public:

  HOnlMainWindow(const TGWindow *p, UInt_t w, UInt_t h);
  virtual ~HOnlMainWindow();

  virtual void CloseWindow();
  virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t);
  Bool_t toGreen(Window_t id);
  Bool_t toRed(Window_t id);
  Bool_t toDefault(Window_t id);
  HHistoVisual *getVisual() const { return fVisual; }
  HOnlObjBrowser *getBrowser() const { return fBrowser; }
  HContourManager *getContManager() const { return fContManager; }
  HConversion *getConversion() const { return fConversion; }
  virtual Bool_t HandleTimer(TTimer *t);  
  void coffeebreak();
  void getCanvPref(Int_t *p1, Int_t *p2, Int_t *p3);
  void setCanvPref(Int_t w, Int_t h, Int_t c);
  TCanvas* getCanvas() const { return fCanvas; }
  void createCanvas();
  void updateCursorList();

 public:

  ClassDef(HOnlMainWindow,0) //Main GUI class

};



class HOnlTimer: public TTimer {

private:

  HOnlMainWindow  *fTimer;

public:

  HOnlTimer(HOnlMainWindow *p, Long_t ms) : 
    TTimer(ms,kTRUE) { fTimer = p; }
  Bool_t Notify() {  fTimer->HandleTimer(0); TurnOff(); return kFALSE; }

};


#endif






