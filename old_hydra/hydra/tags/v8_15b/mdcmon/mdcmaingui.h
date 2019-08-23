#if !defined MAINMDCGUI_H
#define MAINMDCGUI_H

//-------------------------------------------------------
//
// Main panel for online/offline MDC monitoring
// Allows to set input source (DAQ or hld file) and 
// selecte MDC module
//
//
//-------------------------------------------------------

#include <TGFrame.h>
#include <TGComboBox.h>
#include <TGLabel.h>
#include <TGButton.h>
#include <TGMsgBox.h>
#include <TGLabel.h>
#include <TGTextEntry.h>
#include <TGListBox.h>
#include <TGFSContainer.h>
#include <TROOT.h>
#include <TSystem.h>
#include <iostream.h>
#include <fstream.h>
#include <unistd.h>


enum CommandsId {
  M_CLOSEMAIN = 100,
  M_ONLINE,
  M_HLDFILE,
  M_SECTORMAIN,
  M_MODULEMAIN,
  M_HLDBROWSE,
  M_HLDEXECUTE,
  M_HLDCLEAR,
  M_HLDCLOSE,
  M_LIST_MODE,
  M_DETAIL_MODE,
  M_BROWSERCLOSE,
  M_OK,
  M_CL,
  M_CDUP,

  M_CLOSE = 100,
  M_TDCDT,
  M_TD,
  M_TDC,
  M_MULT,
  M_MBIND,  
  M_MBNAME,
  M_TDCNUM,
  M_T1,
  M_T2,
  M_T1_T2,
  M_ALLCH,
  M_SELCH,
  M_TR,
  M_LE,
  M_FILE_OPEN,
  M_FILE_SAVE,
  M_FILE_SAVEAS,
  M_FILE_EXIT
};

class mdcMainGUI : public TGMainFrame {
  TGCompositeFrame    *pMdcMain, *pMDC, *pInput, *pSector, *pModule;
  TGCompositeFrame    *pHldPanel;
  TGComboBox          *pComboSector, *pComboModule;
  TGButton            *pOnlineButton, *pHldButton, *pCloseButton;

  Int_t                secNum, modNum, evtNum;
  Char_t saveFile[100], hldDir[100], hldFile[100], exec[100];
  Int_t i;
  Char_t tmp[100], buf[100];
 public:
  mdcMainGUI(const TGWindow *p, UInt_t w, UInt_t h);
  ~mdcMainGUI();
  void closeWindow();
  Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);
 public:
  ClassDef(mdcMainGUI,0)
};


class HldMsgBox : public TGMainFrame {

 private:
  TGCompositeFrame     *pHldMain, *pHldMsg, *pHldBrow, *pHldBut, 
                       *pHldEvt, *pHldFile;
  TGButton             *pExecuteButton, *pClearButton, 
                       *pCloseButton, *pBrowseButton;
  TGLabel              *pMsg1, *pMsg2;
  TGTextEntry          *pEvtNum;
  Char_t                saveFile[100], exec[100];
  Char_t                hldFile[100], hldFileOld[100], DirFile[100];
  Char_t                hldDir[100], hldDirOld[100];
 public:
  TGTextBuffer         *pEvtNumBuf, *pHldFileBuf;
  TGTextEntry          *pHldFileName;
  char                  fileName[100], dirName[100]; // hld file&dir name is 
                                                     // passed to executed macro 
  Int_t                 evtNum, evtNumOld, secNum, secNumOld, modNum, modNumOld;      // ---"--- 
 public:
  HldMsgBox(const TGWindow *p, UInt_t w, UInt_t h,
	    Int_t secnum, Int_t modnum, UInt_t options = kMainFrame | kVerticalFrame);
  virtual ~HldMsgBox(); 
  virtual void closeWindow();
  void closeWindowHld();
  virtual void readParam();
  virtual int saveParam();
  virtual void updateParam();
  virtual void fileDirDefine();
  virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);
 public:
   ClassDef(HldMsgBox,0)
};

class HldFileSearch : public TGTransientFrame {

private:
   TGCompositeFrame     *pHldFileSearchMain, *pHldFileSearchBut, 
                        *pHldFileSearchStart, *pHldFileSearchBrow;
   TGButton             *pOKButton,*pCloseButton;
   TGListView           *pFileView;
   TGFileContainer      *pFileCont;
   TGListBox            *pDir;  
   const TGPicture      *pPcdup, *pPlist, *pPdetail;
   TGPictureButton      *pCdup, *pListMode, *pDetailMode;
   Char_t                tmp[100];
public:
   HldFileSearch(const TGWindow *p, const TGWindow *main, UInt_t w, UInt_t h,
		 Char_t *dirName,Char_t *dirFile,UInt_t options = kMainFrame | kVerticalFrame);
   virtual ~HldFileSearch();
   
   virtual void closeWindow();
   virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);
   ClassDef(HldFileSearch,0)

};

R__EXTERN  HldMsgBox  *gHldMsgBox; 

#endif

