#ifndef HTREEDRAWPANEL_H
#define HTREEDRAWPANEL_H

#include <TGCanvas.h>
#include <TList.h>
#include <TRint.h>
#include <TGListTree.h>
#include <TGButton.h>
#include <TGTab.h>
#include <TGTextEntry.h>
#include <TGWindow.h>
#include <TObjArray.h>
#include <TTree.h>
#include <TGTextBuffer.h>
#include <TGTextEntry.h>
#include <TGCanvas.h>

class HTreeDrawPanel : public TGMainFrame {
private:
  struct GraphOption {
    char *fOption;
    char *fComment;
  };
  static GraphOption fCommonOpts[],f1DOpts[],f2DOpts[];
  TList *fWidgets;                // Widgets used.
  TTree *fTree;                   // Tree being drawn.
  TGTextBuffer *fTxX;             // X formula
  TGTextBuffer *fTxY;             // Y formula
  TGTextBuffer *fTxZ;             // Z formula
  TGTextBuffer *fTxSelection;     // Selection formula.
  TGListTreeItem *fSelectedItem;  // Selected branch
  TGListTree *fBranchTree;        // Branches tree
  TGRadioButton *fCbX,*fCbY,*fCbZ,*fCbSel;
  Bool_t fCustomHistogram;        // Custom or default histo options.
  TGRadioButton *fRbDefault,*fRbCustom;
  TGTextBuffer *fHTitle; //Histogram title
  TGTextBuffer *fHistOptions[3][3]; //Histo. options [coord][value]
  TH1 *fPrevHistogram; // Previous histogram if custom was used.
  TList *fFormulas;
  TList *fCb1DOptions;
  TList *fCbCommon;
  TList *fCb2DOptions;
  
  Bool_t buildGraphOptions(GraphOption *,TGCompositeFrame *,
			   TList *);
  Bool_t buildHistoOptions(TGCompositeFrame *frame);
  void setupFormulaLine(Text_t *,TGRadioButton *&cb,
			TGTextBuffer *,TGCompositeFrame *,
			Int_t,TGLayoutHints *);
  void initGUI(void);
  void drawHistogram(void);
  TGCanvas *buildTreeView(TGFrame *parent);
  Bool_t buildBranchesTree(void);
  void addSubBranches(TBranch *b,TGListTreeItem *parent);
  void insertText(Text_t *txt);
public:
  HTreeDrawPanel(TTree *t,const TGWindow *p,UInt_t w, UInt_t h);
  ~HTreeDrawPanel(void);
  Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);
};

#endif



