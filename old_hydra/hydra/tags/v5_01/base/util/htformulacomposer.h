#ifndef HTFORMULACOMPOSER_H
#define HTFORMULACOMPOSER_H

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

const Long_t kUpdateFormula=100;

class HTFormulaComposer : public TGTransientFrame, public TGWidget {
private:
  static char *fSymbols[];
  TList *fWidgets;                // Widgets used.
  TTree *fTree;                   // Tree being drawn.
  TGListTreeItem *fSelectedItem;  // Selected branch
  TGListTree *fBranchTree;        // Branches tree
  TGTextBuffer *fText;            // Formula
  TGTextEntry *fTextEntry;
  
  void insertText(Text_t *txt);
  TGCanvas *buildTreeView(TGFrame *);
  TGFrame *buildOperatorFrame(TGFrame *);
  Bool_t buildBranchesTree(void);
  void addSubBranches(TBranch *b,TGListTreeItem *parent);
  void initGUI(void);
public:
  HTFormulaComposer(TTree *t,TGTextBuffer *,const TGWindow *p,
		    const TGWindow *main,Int_t id,UInt_t w, UInt_t h);
  ~HTFormulaComposer(void);
  Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);
};

#endif



