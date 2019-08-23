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
#include <TGFileDialog.h>
#include <TH2.h>
#include <TMapFile.h>
#include <TFile.h>
#include "mdcmaingui.h"
#include "hparset.h"

class HParIo;
class HMdcRawStruct;


class mdcGUI : public TGTransientFrame {
  //class mdcGUI : public TGMainFrame {
  HParSet* raws;
  histDraw                 *histdraw;
  TGCompositeFrame         *fFMain, *fF0, *fFA, *fFB, *fCuts,  *fXCuts, *fYCuts, 
                           *fFD, *fFE, *fFC10, 
                           *fF3, *fF4, *fF5;
  TGComboBox               *fCombo3, *fCombo4, *fCombo5;
  TGButton                 *fCloseButton;
  TGButton                 *fRad1, *fRad2;
  TGButton                 *fTDCDTButton, *fTDButton, *fTdcButton, *fMultButton;
  TGRadioButton            *fRadio1, *fRadio2;
  TGPopupMenu              *pMenuFile;
  TGLayoutHints            *pMenuBarLayout,*pMenuBarItemLayout,*pMenuBarHelpLayout;
  TGMenuBar                *pMenuBar;
  Bool_t                    fAllMbo, fData;
  Int_t                     mboName[16], secNum, modNum, mboNum, chipNum, chipMax;
  char                      filter[100], histName[100], tmp[20];
  TPaveText                 *title;
  TGTextEntry               *fRangeXMin,*fRangeXMax,*fRangeYMin,*fRangeYMax, 
                            *fRefreshTime, *fLastEvtNum ;
  TGTextBuffer              *fRangeBufXMin,*fRangeBufXMax,*fRangeBufYMin,
                            *fRangeBufYMax, *fRefreshTimeBuf, *fLastEvtBuf;
  TGMenuBar                 *fMenuBar, *fMenuBarTDC;
  TGPopupMenu               *fTDMenu, *fTDCMenu, *fCascadeMenu;
  TGCheckButton             *fTimeRefr, *fLastEvt;
  TH2S                      *pHistTDCDT[16], *pHistTD[16];
  TMapFile                  *mfile;
  TFile                      file;

  Float_t                    xmin, xmax, ymin, ymax;
  Char_t                     tmp1[30],tmp2[30],tmp3[30],tmp4[30],mapfile[100];
  Char_t                     time;
  Int_t                      i,j,flag;
  Long_t                     refrTime;


 public:
  mdcGUI(const TGWindow *p, const TGWindow *main, UInt_t w, UInt_t h,
	 Int_t secnum, Int_t modnum, UInt_t options = kMainFrame | kVerticalFrame);
  //  mdcGUI(const TGWindow *p, UInt_t w, UInt_t h);
  virtual ~mdcGUI();
  virtual void clearTDCComb();
  virtual void setTDCComb(Int_t);
  virtual void CloseWindow();
  virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);
  
 public:
  ClassDef(mdcGUI,0)
};

#endif
