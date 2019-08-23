#if !defined HONLOBJBROWSER_H
#define HONLOBJBROWSER_H

//-------------------------------------------------------------------------
//
// File name:       honlobjbrowser.h
// Subject:         Manager and browser of online objects.
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            March 2000
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the declaration of the class HOnlObjBrowser.
// HOnlObjBrowser is a GUI class which maintains the panel for managing
// and browsing the online objects of HTOP.
//
//-------------------------------------------------------------------------

#include <TGButton.h>
#include <TGListBox.h>
#include <TGComboBox.h>
#include <TGTab.h>
#include <TGTextEntry.h>
#include <TGTextBuffer.h>
#include <TRootEmbeddedCanvas.h>

enum CommandsBrowserId {

  M__CLOSE = 100,

  //**************************************
  //----- det set ------------------------

  //----- TOF ----------------------------

  M__TO_SINGLE,
  M__TO_MULTI,
  M__TO_CATEGBOX,
  M__TO_SECBOX,
  M__TO_MODBOX,
  M__TO_RODBOX,
  M__TO_SINLAY,
  M__TO_RECLAY,
  M__TO_ADDLAY,
  M__TO_SUBLAY,

  //----- MDC ----------------------------

  M__MD_SINGLE,
  M__MD_MULTI,
  M__MD_CATEGBOX,
  M__MD_SECBOX,
  M__MD_MODBOX,
  M__MD_LAYBOX,
  M__MD_CELLBOX,
  M__MD_MODLIST,
  M__MD_SINLAY,
  M__MD_RECLAY,
  M__MD_ADDLAY,
  M__MD_SUBLAY,
  M__MD_UP,
  M__MD_DOWN,

  //----- RICH ---------------------------

  M__RI_SINGLE,
  M__RI_MULTI,
  M__RI_CATEGBOX,
  M__RI_SECBOX,
  M__RI_ROWBOX,
  M__RI_COLBOX,
  M__RI_MODLIST,
  M__RI_SINLAY,
  M__RI_RECLAY,
  M__RI_ADDLAY,
  M__RI_SUBLAY,
  M__RI_UP,
  M__RI_DOWN,

  //----- Shower -------------------------

  M__SH_SINGLE,
  M__SH_MULTI,
  M__SH_CATEGBOX,
  M__SH_SECBOX,
  M__SH_MODBOX,
  M__SH_ROWBOX,
  M__SH_COLBOX,
  M__SH_MODLIST,
  M__SH_SINLAY,
  M__SH_RECLAY,
  M__SH_ADDLAY,
  M__SH_SUBLAY,
  M__SH_UP,
  M__SH_DOWN,

  //----- end of det set -----------------
  //**************************************

  //**************************************
  //----- local conditions ---------------

  M__LC_DSCB,
  M__LC_UNDOEXP,
  M__LC_CLEAREXP,
  M__LC_NEWPAREXP,
  M__LC_PARAMLB,
  M__LC_ALWAYSTRUE,
  M__LC_XPARCB,
  M__LC_YPARCB,
  M__LC_CONTOURCB,
 
  //----- end of local conditions --------
  //**************************************

  //**************************************
  //----- global conditions --------------

  M__GC_UNDOEXP,
  M__GC_CLEAREXP,
  M__GC_PARAMLIST,
  M__GC_DS1CB,
  M__GC_DS2CB,
  M__GC_XPARCB,
  M__GC_YPARCB,
  M__GC_CONTOURCB,
  
  //----- end of global conditions -------
  //**************************************

  //**************************************
  //----- histogrammes -------------------

  M__HI_1DIMRB,
  M__HI_2DIMRB,
  M__HI_MULRB,
  M__HI_CORRB,
  M__HI_NOCRB,
  M__HI_CRB,
  M__HI_LOCALRB,
  M__HI_GLOBALRB,
  M__HI_CCB,
  M__HI_DS1CB,
  M__HI_DS2CB,
  M__HI_PAR1CB,
  M__HI_PAR2CB,
  M__HI_PAUSE,

  //----- end of histogrammes ------------
  //**************************************

  M_ADD,
  M_MODIFY,
  M_REMOVE,
  M_LIST,
  M_NEXT,
  M_PREV

};


enum TabsIdx {

  DETSET_TOF = 1,
  DETSET_MDC,
  DETSET_RICH,
  DETSET_SHOWER,
  DETSET_START,
  DETSET_TOFINO,
  HISTO,
  LC,
  GC

};


class HBrowserDB;
class HDetSet;
class HHistogram;
class HLocalCondition;
class HGlobalCondition;

class HOnlObjBrowser : public TGMainFrame {

 protected:

  HBrowserDB           *fDB;

 protected:

  TGCompositeFrame     *fFrame;
  TGCompositeFrame     *fF00, *fF01;
  TGTab                *fMainTab, *fDSTab;
  TGCompositeFrame     *fF0;
  TGCompositeFrame     *fDSFrame;
  TGCompositeFrame     *fTofFrame, *fMdcFrame, *fRichFrame, *fShFrame;
  TGCompositeFrame     *fB1, *fB2, *fB3;
  TGButton             *fAdd, *fModify, *fRemove;
  TGButton             *fNext, *fPrev;
  TGListBox            *fObjList;

  //------- TOF det set frames definition ----------------------------

  TGCompositeFrame     *fTof1, *fTof2;
  TGCompositeFrame     *fTofRBFrame, *fTofCatFrame;
  TGCompositeFrame     *fTofSingleFrame, *fTofMultiFrame;
  TGCompositeFrame     *fTofRBLayFrame;
  TGCompositeFrame     *fTof21, *fTof22, *fTof23, *fTof24, *fTof25, *fTof26;
  TGComboBox           *fTofSecBox, *fTofModBox, *fTofRodBox;
  TGRadioButton        *fTofSingleRB, *fTofMultiRB;
  TGRadioButton        *fTofSinDefRB, *fTofRecDefRB;
  TGRadioButton        *fTofAddDefRB, *fTofSubDefRB;
  TGComboBox           *fTofCatBox;
  TGTextEntry          *fTofName;
  TGTextBuffer         *fTofNameBuf;
  TGButton             *fTofSecModBut[48];
  Bool_t                kTofColor[48];
  Int_t                 idxiTof[2], idxjTof[2];     // for display
  Int_t                 countTof;                   // layout

  //------- end of TOF det set frames definition ---------------------

  //------- MDC det set frames definition ----------------------------

  TGCompositeFrame     *fMdc1, *fMdc2;
  TGCompositeFrame     *fMdc21, *fMdc22, *fMdc23, *fMdc24, *fMdc25;
  TGCompositeFrame     *fMdc26, *fMdc27, *fMdc28, *fMdc29, *fMdc30;
  TGCompositeFrame     *fMdc31, *fMdc32;
  TGCompositeFrame     *fMdcStep1Frame, *fMdcStep2Frame;
  TGCompositeFrame     *fMdcSingleFrame, *fMdcMultiFrame;
  TGCompositeFrame     *fMdcRBLayFrame;
  TGComboBox           *fMdcSecBox, *fMdcModBox, *fMdcLayBox, *fMdcCellBox;
  TGCompositeFrame     *fMdcRBFrame, *fMdcCatFrame;
  TGRadioButton        *fMdcSingleRB, *fMdcMultiRB;
  TGRadioButton        *fMdcSinDefRB, *fMdcRecDefRB;
  TGRadioButton        *fMdcAddDefRB, *fMdcSubDefRB;
  TGComboBox           *fMdcCatBox;
  TGTextEntry          *fMdcName;
  TGTextBuffer         *fMdcNameBuf;
  TGButton             *fMdcSecModBut[24];
  TGButton             *fMdcCellsBut[48];
  TGPictureButton      *fMdcUp, *fMdcDown;
  TGListBox            *fMdcModList;
  Bool_t                kMdcColor1[24];
  Bool_t                kMdcColor2[48];
  Int_t                 fMdcStep12[24][48];
  Int_t                 idxiMdcS1[2], idxiMdcS2[2]; // for 
  Int_t                 idxjMdcS1[2], idxjMdcS2[2]; // display
  Int_t                 countMdcS1, countMdcS2;     // layout


  //------- end of MDC det set frames definition ---------------------

  //------- RICH det set frames definition ---------------------------

  TGCompositeFrame     *fRich1, *fRich2;
  TGCompositeFrame     *fRich21, *fRich22, *fRich23, *fRich24, *fRich25;
  TGCompositeFrame     *fRich26, *fRich27, *fRich28, *fRich29, *fRich30;
  TGCompositeFrame     *fRich31, *fRich32;
  TGCompositeFrame     *fRichStep1Frame, *fRichStep2Frame;
  TGCompositeFrame     *fRichSingleFrame, *fRichMultiFrame;
  TGComboBox           *fRichSecBox, *fRichRowBox, *fRichColBox;
  TGCompositeFrame     *fRichRBFrame, *fRichCatFrame;
  TGRadioButton        *fRichSingleRB, *fRichMultiRB;
  TGRadioButton        *fRichSinDefRB, *fRichRecDefRB;
  TGRadioButton        *fRichAddDefRB, *fRichSubDefRB;
  TGCompositeFrame     *fRichRBLayFrame;
  TGComboBox           *fRichCatBox;
  TGTextEntry          *fRichName;
  TGTextBuffer         *fRichNameBuf;
  TGButton             *fRichSecModBut[6];
  TGButton             *fRichCellsBut[64];
  TGPictureButton      *fRichUp, *fRichDown;
  TGListBox            *fRichModList;
  Bool_t                kRichColor1[6];
  Bool_t                kRichColor2[64];
  Int_t                 fRichStep12[6][64];
  Int_t                 idxiRichS1[2], idxiRichS2[2]; // for 
  Int_t                 idxjRichS1[2], idxjRichS2[2]; // display
  Int_t                 countRichS1, countRichS2;     // layout

  //------- end of RICH det set frames definition --------------------

  //------- Shower det set frames definition -------------------------

  TGCompositeFrame     *fSh1, *fSh2;
  TGCompositeFrame     *fSh21, *fSh22, *fSh23, *fSh24, *fSh25;
  TGCompositeFrame     *fSh26, *fSh27, *fSh28, *fSh29, *fSh30;
  TGCompositeFrame     *fSh31, *fSh32;
  TGCompositeFrame     *fShStep1Frame, *fShStep2Frame;
  TGCompositeFrame     *fShSingleFrame, *fShMultiFrame;
  TGComboBox           *fShSecBox, *fShModBox, *fShRowBox, *fShColBox;
  TGCompositeFrame     *fShRBFrame, *fShCatFrame;
  TGRadioButton        *fShSingleRB, *fShMultiRB;
  TGRadioButton        *fShSinDefRB, *fShRecDefRB;
  TGRadioButton        *fShAddDefRB, *fShSubDefRB;
  TGCompositeFrame     *fShRBLayFrame;
  TGComboBox           *fShCatBox;
  TGTextEntry          *fShName;
  TGTextBuffer         *fShNameBuf;
  TGButton             *fShSecModBut[18];
  TGButton             *fShCellsBut[64];
  TGPictureButton      *fShUp, *fShDown;
  TGListBox            *fShModList;
  Bool_t                kShColor1[18];
  Bool_t                kShColor2[64];
  Int_t                 fShStep12[18][64];
  Int_t                 idxiShS1[2], idxiShS2[2]; // for 
  Int_t                 idxjShS1[2], idxjShS2[2]; // display
  Int_t                 countShS1, countShS2;     // layout

  //------- end of Shower det set frames definition ------------------

  //------- Global conditions frames definition --------------------

  TGCompositeFrame     *fGCFrame;
  TGCompositeFrame     *fGC1, *fGC2, *fGC21, *fGC22;
  TGTextEntry          *fGCName;
  TGTextBuffer         *fGCNameBuf;

  //----- GC -- define expression subpanel --------------------------

  TGCompositeFrame     *fGCExpFrame;
  TGCompositeFrame     *fGCA1, *fGCA2;
  TGCompositeFrame     *fGCA21, *fGCA22, *fGCA23, *fGCA24;
  TGCompositeFrame     *fGCA211, *fGCA212;
  TGButton             *fGCToken[12];
  TGButton             *fGCGroup1[12];
  TGButton             *fGCGroup2[2];
  TGListBox            *fGCExpList;
  TGListBox            *fGCListPar;
  TGButton             *fGCExpUndo, *fGCExpClear;

  //-----GC -- define contour subpanel --------------------------

  TGCompositeFrame     *fGCContourFrame;
  TGCompositeFrame     *fGCC1, *fGCC2, *fGCC3;
  TGComboBox           *fGCDS1CB, *fGCDS2CB;
  TGComboBox           *fGCXparBox, *fGCYparBox;
  TGComboBox           *fGCContourCB;

  //------- end of Global conditions frames definition ---------------

  //------- Local conditions frames definition -----------------------

  TGCompositeFrame     *fLCFrame;
  TGCompositeFrame     *fLC1, *fLC2;
  TGComboBox           *fLCDSCB;
  TGListBox            *fLCParLB;
  TGCheckButton        *fLCAlwTrue;
  TGTextEntry          *fLCName;
  TGTextBuffer         *fLCNameBuf;

  //---- LC -- define expression subpanel --------------------------

  TGCompositeFrame     *fLCExpFrame;
  TGCompositeFrame     *fLCA1, *fLCA2;
  TGCompositeFrame     *fLCA11, *fLCA12;
  TGCompositeFrame     *fLCA21, *fLCA22;
  TGCompositeFrame     *fLCA111, *fLCA112;
  TGCompositeFrame     *fLCA121, *fLCA122, *fLCA123;
  TGButton             *fLCToken[12];
  TGButton             *fLCGroup1[8];
  TGButton             *fLCGroup2[6];
  TGButton             *fLCGroup3[8];
  TGButton             *fLCGroup4[2];
  TGListBox            *fLCExpList;
  TGButton             *fLCExpUndo, *fLCExpClear, *fLCExpNPar;
  TGTextEntry          *fLCNParName;
  TGTextBuffer         *fLCNParNameBuf;

  //---- LC -- define contour subpanel ---------------------------

  TGCompositeFrame     *fLCContourFrame;
  TGCompositeFrame     *fLCC1, *fLCC2;
  TGCompositeFrame     *fLCC11, *fLCC12;
  TGComboBox           *fLCXparBox, *fLCYparBox;
  TGComboBox           *fLCContourCB;

  //------- end of Local conditions frames definition ----------------

  //------- Histogrammes frames definition -----------------------

  TGCompositeFrame     *fHiFrame;
  TGCompositeFrame     *fHi1, *fHi2;
  TGCompositeFrame     *fHi11, *fHi12, *fHi13;
  TGCompositeFrame     *fHiStep1, *fHiStep2, *fHiStep3, *fHiStep4;
  TGCompositeFrame     *fHiStep5, *fHiStep6, *fHiStep7, *fHiStep8;
  TRootEmbeddedCanvas  *fHiCanv;
  //  TRootEmbeddedCanvas  *fHiCanvProj;
  TGRadioButton        *fHi1dRB, *fHi2dRB;
  TGRadioButton        *fHiMulRB, *fHiCorRB;
  TGRadioButton        *fHiNoCRB, *fHiCRB;
  TGCompositeFrame     *fHiStep4RBFrame, *fHiStep4CBFrame;
  TGRadioButton        *fHiLocalRB, *fHiGlobalRB;
  TGComboBox           *fHiConCB;
  TGCompositeFrame     *fHiStep5LabFrame, *fHiStep5CB1Frame, *fHiStep5CB2Frame;
  TGComboBox           *fHiDS1CB, *fHiDS2CB, *fHiPar1CB, *fHiPar2CB;
  TGCompositeFrame     *fHiStep6LabFrame, *fHiStep6T1Frame, *fHiStep6T2Frame;
  TGTextEntry          *fHiFrom1, *fHiFrom2, *fHiTo1, *fHiTo2;
  TGTextEntry          *fHiChan1, *fHiChan2;
  TGTextBuffer         *fHiFrom1Buf, *fHiFrom2Buf, *fHiTo1Buf, *fHiTo2Buf;
  TGTextBuffer         *fHiChan1Buf, *fHiChan2Buf;
  TGCompositeFrame     *fHiStep7LabFrame;
  TGTextEntry          *fHiName;
  TGTextBuffer         *fHiNameBuf;
  TGCheckButton        *fHiPause;

  //------- end of Histogrammes frames definition ----------------

 public:

  static Int_t          countBrowser;

 public:

  HOnlObjBrowser(const TGWindow *p, UInt_t w, UInt_t h);
  virtual ~HOnlObjBrowser();
  virtual void CloseWindow();
  virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);
  Int_t calcTabsIdx(); // this function calculates positions of tabs
  void displayObjList(); // displays fObjList
  void displayHisto();
  void displayLC();
  void displayGC();
  void displayTofDS();
  void displayMdcDS();
  void displayRichDS();
  void displayShDS();
  void createDSList(TGComboBox *p, HDetSet *pSet = 0);
  void createParList(TGComboBox *p, HDetSet *pSet); // for histogrammes
  void createContourList(TGComboBox *p = 0);
  void addCounterPar(TGComboBox *p);
  void createParList(HDetSet *pSet = 0); // for local conditions
  void createCountsList(); // for global conditions
  Int_t extractCountIdx(const char *name);
  void clearCB(TGComboBox *p);
  void clearLB(TGListBox *p);
  void createHiCList(Int_t mode = 0);
  void highlight(TGComboBox *p, const char *name);
  void highlight(TGListBox *p, const char *name);
  void controlHiGui(TGFrame *p);
  void controlTofDSGui(TGFrame *p);
  void controlMdcDSGui(TGFrame *p);
  void controlRichDSGui(TGFrame *p);
  void controlShDSGui(TGFrame *p);
  HHistogram* createHisto();
  HLocalCondition* createLC();
  HGlobalCondition* createGC();
  HDetSet* createTofDS();
  HDetSet* createMdcDS();
  HDetSet* createRichDS();
  HDetSet* createShDS();
  Bool_t changeHisto(Int_t mode = 0);
  Bool_t changeLC(Int_t mode = 0, HLocalCondition **p = 0, Int_t *id = 0);
  Bool_t changeGC(Int_t mode = 0, HGlobalCondition **p = 0, Int_t *id = 0);
  Bool_t changeDetSet(Int_t det, Int_t mode = 0, HDetSet **p = 0, Int_t *id=0);
  void clearHistoFields();
  void clearContFields(Int_t ctype);
  Int_t calcCateg();
  void createTofDSCB(Bool_t kFlag);
  void createMdcDSCB(Bool_t kFlag);
  void createRichDSCB(Bool_t kFlag);
  void createShDSCB(Bool_t kFlag);
  Bool_t toDefault(Window_t id);
  Bool_t toGreen(Window_t id);
  Bool_t toRed(Window_t id);
  void createMdcModList(Int_t id);
  void createRichModList(Int_t id);
  void createShModList(Int_t id);
  void displayMdcLayS1();
  void displayMdcLayS2();
  void displayRichLayS1();
  void displayRichLayS2();
  void displayShLayS1();
  void displayShLayS2();
  void displayTofLay();

 public:

  ClassDef(HOnlObjBrowser,0)
  
};

#endif


