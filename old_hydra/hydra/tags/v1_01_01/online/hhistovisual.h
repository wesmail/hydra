#if !defined HHISTOVISUAL_H
#define HHISTOVISUAL_H

//-------------------------------------------------------------------------
//
// File name:       hhistovisual.h
// Subject:         GUI panel for histogram display.
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            April 2000
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the declaration of the class HHistoVisual.
// This is a GUI class for histogram display.
//
//-------------------------------------------------------------------------

#include <TGButton.h>
#include <TGListBox.h>
#include <TGComboBox.h>
#include <TGTextEntry.h>
#include <TGTextBuffer.h>
#include <TGMenu.h>

enum CommandsHistoVisualId {

  M__HIV_CLOSE = 100,
  M__HIV_DRAW,
  M__HIV_SELECTALL,
  M__HIV_SAVEPIC,
  M__HIV_EDITPIC,
  M__HIV_SAVEHIST,
  M__HIV_RESETALL,
  M__HIV_RESET,
  M__HIV_NEXT,
  M__HIV_PREV,
  M__HIV_MULTISELECT,
  M__HIV_HISTOLIST,
  M__HIV_SYSDEFAULT,
  M__HIV_HISDEFAULT,
  M__HIV_SAVEOPT,
  M__HIV_FIX1,
  M__HIV_FIX2,
  M__HIV_FIX3,
  M__HIV_SLICE,
  M__HIV_UPDATE

};


class HHistogram;
class TCanvas;
class HDrawingOpt;

class HHistoVisual : public TGMainFrame {

 protected:

  TGMainFrame         *fParent;
  TCanvas             *fCanvas;
  HDrawingOpt         *fBuffer;

 protected:

  TGCompositeFrame    *fFrame;
  TGCompositeFrame    *fF2, *fF3;
  TGCompositeFrame    *fF21, *fF22;
  TGCompositeFrame    *fF221, *fF222, *fF223;
  TGCompositeFrame    *fF31, *fF32;
  TGCompositeFrame    *fF311, *fF312, *fF313, *fF314;
  TGCompositeFrame    *fF3111, *fF3112;
  TGCompositeFrame    *fF31111, *fF31112;

  TGMenuBar           *fMenuBar;
  TGPopupMenu         *fMenuFile, *fMenuPicture;

  TGButton            *fSelAllB, *fResetAllB, *fResetB, *fDrawB;
  TGButton            *fNextB, *fPrevB;
  TGButton            *fLayoutB[16];
  TGButton            *fSysDefaultB, *fHisDefaultB, *fSaveB;
  TGButton            *fFix1B, *fFix2B, *fFix3B;
  TGButton            *fSliceB;
  TGButton            *fUpdateB;

  TGCheckButton       *fMultipleCB;

  TGListBox           *fHistoLB;
  
  TGTextEntry         *fXStart, *fXStop, *fYStart, *fYStop;
  TGTextBuffer        *fXStartBuf, *fXStopBuf, *fYStartBuf, *fYStopBuf;

  TGTextEntry         *fScale;
  TGTextBuffer        *fScaleBuf;

  TGComboBox          *fOptionCB;

 protected:

  Int_t xDiv;
  Int_t yDiv;
  Int_t histIdx[16]; // indexes of histogrammes in pads
  Bool_t kColor[16]; // flags of highlighted buttons in display layout
  Int_t indi[2]; // for 
  Int_t indj[2]; // display
  Int_t count;   // layout

 public:

  static Int_t         countVisual;

 public:

  HHistoVisual(const TGWindow *p, UInt_t w, UInt_t h, TGMainFrame *p2 = 0);
  virtual ~HHistoVisual();
  virtual void CloseWindow();
  virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);
  Bool_t toDefault(Window_t id);
  Bool_t toGreen(Window_t id);
  void drawHist(HHistogram *hist);
  void setCanvasDivision(Int_t number);
  void automaticDraw(); //draws a histo in case of automatic display layout
  void userDraw(); //draws a histo in case of user defined display layout 
  void clearTF();
  Bool_t isLayout();
  void highlight(TGComboBox *p, const char* name);
  const char* isSelected(TGComboBox *p);
  void updateList();
  void calcPad(Int_t index, Float_t *x1,Float_t *y1,Float_t *x2,Float_t *y2);
  void displayLayout();
  Bool_t createCanvas();
  void drawing(HHistogram *hist, Int_t mode);
  void saveToBuffer();
  void restoreBuffer();

 public:

  ClassDef(HHistoVisual,0)
  
};

#endif

 
  
