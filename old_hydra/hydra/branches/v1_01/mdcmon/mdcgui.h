#if !defined MDCGUI_H
#define MDCGUI_H

//-------------------------------------------------------
//
// Main panel for MDC graphical interface for September-99
// beam-test
//
//
//-------------------------------------------------------

#include "histdraw.h"
#include <TGFrame.h>
#include <TGLabel.h>
#include <TGComboBox.h>
#include <TGButton.h>
#include <TTree.h>
#include <TSystem.h>
#include <TPaveText.h>
#include <TGTextEntry.h> 
#include <TGTextBuffer.h> 
#include <TGMenu.h>	

enum CommandsId {
  M_CLOSE = 100,
  M_TDCDT,
  M_TD,
  M_TDC,
  M_MULT,
  M_SECTOR,
  M_MODULE,
  M_MBIND,  
  M_MBNAME,
  M_TDCNUM,
  M_T1,
  M_T2,
  M_T1_T2,
  M_ALLCH,
  M_SELCH,
  M_TR,
  M_LE
};
  
class mdcGUI : public TGMainFrame {
  histDraw                 *histdraw;
  TGCompositeFrame         *fFMain, *fF0, *fFA, *fFB, *fCuts,  *fXCuts, *fYCuts, 
                           *fFD, *fFE, *fFC10, 
                           *fF1, *fF2, *fF3, *fF4, *fF5;
  TGComboBox               *fCombo1, *fCombo2, *fCombo3, *fCombo4, *fCombo5;
  TGButton                 *fCloseButton;
  TGButton                 *fRad1, *fRad2;
  TGButton                 *fTDCDTButton, *fTDButton, *fTdcButton, *fMultButton;
  TGRadioButton            *fRadio1, *fRadio2;
  Bool_t                    fAllMbo, fData;
  Int_t                     mboName[16], secNum, modNum, mboNum, chipNum;
  char                      filter[100], histName[100], tmp[20];
  TPaveText                 *title;
  TGTextEntry               *fRangeXMin,*fRangeXMax,*fRangeYMin,*fRangeYMax, 
                            *fRefreshTime, *fLastEvtNum ;
  TGTextBuffer              *fRangeBufXMin,*fRangeBufXMax,*fRangeBufYMin,
                            *fRangeBufYMax, *fRefreshTimeBuf, *fLastEvtBuf;
  TGMenuBar                 *fMenuBar, *fMenuBarTDC;
  TGPopupMenu               *fTDMenu, *fTDCMenu, *fCascadeMenu;
  TGCheckButton             *fTimeRefr, *fLastEvt;
  Float_t                    xmin, xmax, ymin, ymax;
  Char_t                     tmp1[30],tmp2[30],tmp3[30],tmp4[30];
  Char_t                     time;
  Int_t                      i,j;
  Long_t                     refrTime;
 public:
  mdcGUI(const TGWindow *p, UInt_t w, UInt_t h);
  virtual ~mdcGUI();
  virtual void CloseWindow();
  virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);
  
 public:
  ClassDef(mdcGUI,0)
};

#endif





