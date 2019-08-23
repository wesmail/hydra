#include "htreedrawpanel.h"
#include "htformulacomposer.h"
#include <TBranch.h>
#include <TGTextEntry.h>
#include <TGLabel.h>
#include <TGMsgBox.h>
#include <TCanvas.h>
#include <TGToolBar.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>

//*-- Author : M. Sanchez
//*-- Modified : 13/01/2000

///////////////////////////////////////////////////////////////////////
//                                                                   //
//                         Tree Draw Panel                           //
//This class presents a panel to ease the use of TTree::Draw.
//On the left side of the panel there is a view of the tree being 
//browsed. This view is realized following the same conventions as in
//the ROOT browser.
//
//On the right side there are three areas. The upper one is the formula
//editor. Here we see four text entries corresponding to the X, Y and
//Z axis to be drawn as well as an entry for a condition (selection). By the
//left of each entry there is a radio button which can be active or not,
//the active entry is that with the active radiobutton on the left.
//
//In order to edit the formulae one can use the keyboard or double-click
//on a branch to make it appear in the active formula.
//
//The other two areas contain drawing options for 1D, 2D and general 
//histograms, and options to generate custom histograms (changing the
//binning, for example). This should be self-explanatory.
///////////////////////////////////////////////////////////////////////

class HAxisParam {
private:
  Int_t fNBins;
  Float_t fMin,fMax;
  Bool_t fCorrect;
public:
  HAxisParam(void) : fNBins(0),fMin(0),fMax(0),fCorrect(kFALSE) {}
  ~HAxisParam(void) {}
  Bool_t isCorrect(void) { return fCorrect; }
  void setCorrect(Bool_t f) { fCorrect=f; }
  Int_t getNBins(void) { return fNBins;}
  Float_t getMin(void) { return fMin;}
  Float_t getMax(void) { return fMax;}
  Bool_t readNBins(const Text_t *data) { 
    return ( (sscanf(data,"%i",&fNBins)==1)?kTRUE:kFALSE ); 
  }
  Bool_t readMin(const Text_t *data) { 
    return ( (sscanf(data,"%f",&fMin)==1)?kTRUE:kFALSE ); 
  }
  Bool_t readMax(const Text_t *data) { 
    return ( (sscanf(data,"%f",&fMax)==1)?kTRUE:kFALSE ); 
  }
  void setNBins(Int_t n) { fNBins=n; }
  void setMin(Float_t f) { fMin=f; }
  void setMax(Float_t f) { fMax=f; }
};

enum {kCEditX=0,kCEditY=1,kCEditZ=2,kCEditSel=3,kCbX=4,kCbY=5,kCbZ=6,kCbSel=7,kCDraw=8,kCClose=9,kCClear=10,kRbCustom=11,kRbDefault=12};

HTreeDrawPanel::GraphOption 
HTreeDrawPanel::fCommonOpts[]={ {"SAME","Superimpose"}, 
				{"CYL","Cylindrical Coordinates"},
				{"POL","Polar Coordinates"},
				{"SPH","Spherical Coordinates"},
				{"PSR","PseudoRapidity/Phi Coord."},
				{"LEGO","LEGO plot"},
				{"LEGO1","Lego plot + hidden surf. removal"},
				{"LEGO2","Lego plot with colors"},
				{"SURF","Surface plot"},
				{"SURF1","Surf. plot + hidden surf. removal"},
				{"SURF2","Surface plot with colors"},
				{"SURF3","Surface plot + contour plot on top"},
				{"SURF4","Surface with Goraud Shading"},
				{NULL,NULL}
};

HTreeDrawPanel::GraphOption 
HTreeDrawPanel::f1DOpts[]={ {"AH","Don't draw axis"},
			    {"B","Barchart"},
			    {"C","Smooth curve"},
			    {"E","Error bars"},
			    {"E0","Error bars also for 0 bins"},
			    {"E1","Err. bars with lines"},
			    {"E2","Err. bars with rectangles"},
			    {"E3","Fill area covered by err. bars"},
			    {"E4","Smoothed fill area for err. bars"},
			    {"L","Draw line"},
			    {"P","Draw markers"},
			    {"*H","Use * as marker"},
			    {NULL,NULL}
};

HTreeDrawPanel::GraphOption 
HTreeDrawPanel::f2DOpts[]={ {"ARR","Arrow mode"},
			    {"BOX","Box mode"},
			    {"COL","Draw colored box for each cell"},
			    {"COLZ","Colored box + scale"},
			    {"CONT","Draw a contour plot"},
			    {"CONT1","Contour plot with line styles"},
			    {"CONT2","Contour plot with lines"},
			    {"CONT3","Contour plot with colors"},
			    {"FB","Supress front box"},
			    {"BB","Suppress back box"},
			    {"SCAT","Draw scatter plot"},
			    {NULL,NULL}
};

HTreeDrawPanel::HTreeDrawPanel(TTree *t,const TGWindow *p,
			       UInt_t w, UInt_t h) : 
  TGMainFrame(p,w,h) 
{
  //Initializes internal variables and GUI
  fTree=t;
  fWidgets=new TList;
  fCb1DOptions=new TList;
  fCbCommon=new TList;
  fCb2DOptions=new TList;
  fFormulas=new TList;
  fTxX=new TGTextBuffer(512);
  fTxY=new TGTextBuffer(512);
  fTxZ=new TGTextBuffer(512);
  fTxSelection=new TGTextBuffer(512);
  fHTitle=new TGTextBuffer(255);
  for (Int_t i=0;i<3;i++) {
    for (Int_t j=0;j<3;j++) {
      fHistOptions[i][j]=new TGTextBuffer(100);
    }
  }
  fPrevHistogram=0;

  initGUI(); 
}

HTreeDrawPanel::~HTreeDrawPanel(void) {
  //Cleans up
  fWidgets->Delete();
  delete fWidgets;
  fFormulas->Delete();
  delete fFormulas;
  delete fTxX; delete fTxY; delete fTxZ; delete fTxSelection;
  fCb1DOptions->Delete();
  fCb2DOptions->Delete();
  fCbCommon->Delete();
  delete fCb1DOptions; delete fCb2DOptions; delete fCbCommon;
}


void HTreeDrawPanel::setupFormulaLine(Text_t* txt,TGRadioButton *&cb,
				      TGTextBuffer *b,
				      TGCompositeFrame *parent,Int_t id,
				      TGLayoutHints *la) {
  //Builds the GUI for a formula line. Including a radio button, a
  //text entry and one extra button to invoke a formula composer.
  TGLayoutHints *lnormal=new TGLayoutHints();
  TGHorizontalFrame *frame=0;
  TGTextButton *button=0;
  TGTextEntry *entry=0;

  fWidgets->Add(lnormal);
  frame=new TGHorizontalFrame(parent,10,10);
  fWidgets->Add(frame);
  
  cb=new TGRadioButton(frame,txt,id+4); //Check box id = buton id+4
  fWidgets->Add(cb);
  cb->Associate(this);
  frame->AddFrame(cb,lnormal);
  entry=new TGTextEntry(frame,b);
  fFormulas->Add(entry);
  cb->SetUserData(entry);
  frame->AddFrame(entry,la);
  button=new TGTextButton(frame,"...",id);
  button->Associate(this);
  fWidgets->Add(button);
  frame->AddFrame(button,lnormal);

  parent->AddFrame(frame,la);
}

Bool_t HTreeDrawPanel::buildHistoOptions(TGCompositeFrame *frame) {
  //Builds interface to customize histogram created by the Draw command
  TGRadioButton *button=0;
  TGLayoutHints *lnormal=new TGLayoutHints;
  TGLabel *label=0;
  TGLayoutHints *lExpandX=new TGLayoutHints(kLHintsExpandX);
  TGCompositeFrame *iframe=0;
  TGTextEntry *entry=0;
  TGLayoutManager *layout=0;
  TGHorizontal3DLine *separator=0;
  Int_t i=0,j=0;
  char *cols[4]={"","Num. Bins","Minimum","Maximum"};
  char *rows[3]={"X","Y","Z"};

  fWidgets->Add(lnormal);
  fWidgets->Add(lExpandX);
  button=new TGRadioButton(frame,"Default",kRbDefault);
  fRbDefault=button;
  button->Associate(this);
  button->SetState(kButtonDown); fCustomHistogram=kFALSE;
  fWidgets->Add(button);
  frame->AddFrame(button,lnormal);
  button=new TGRadioButton(frame,"Custom",kRbCustom);
  fRbCustom=button;
  button->Associate(this);
  fWidgets->Add(button);
  frame->AddFrame(button,lnormal); 
  
  iframe=new TGHorizontalFrame(frame,10,10); fWidgets->Add(iframe);
  label=new TGLabel(iframe,"Title: "); fWidgets->Add(label);
  iframe->AddFrame(label,lnormal);
  entry=new TGTextEntry(iframe,fHTitle); fWidgets->Add(entry);
  entry->SetWidth(120);
  iframe->AddFrame(entry,lExpandX);
  frame->AddFrame(iframe,lExpandX);

  // Options grid
  separator=new TGHorizontal3DLine(frame); fWidgets->Add(separator);
  frame->AddFrame(separator,lExpandX);
  iframe=new TGCompositeFrame(frame,10,10); fWidgets->Add(iframe);
  layout=new TGMatrixLayout(iframe,4,4); fWidgets->Add(layout);
  iframe->SetLayoutManager(layout);
  for (j=0;j<4;j++) {
    label=new TGLabel(iframe,cols[j]); fWidgets->Add(label);
    iframe->AddFrame(label,lExpandX);
  }
  for (i=1;i<4;i++) {
    label=new TGLabel(iframe,rows[i-1]);
    iframe->AddFrame(label,lnormal);
    for (j=1;j<4;j++) {
      entry=new TGTextEntry(iframe,fHistOptions[i-1][j-1]);
      fWidgets->Add(entry);
      entry->Resize(80,entry->GetDefaultHeight());
      iframe->AddFrame(entry,lExpandX);
    }
  }
  frame->AddFrame(iframe,lExpandX);  
  //! Options grid

  return kTRUE;
}

Bool_t HTreeDrawPanel::buildGraphOptions(GraphOption *opt,
					 TGCompositeFrame *parent,
					 TList *checkList) {
  //Builds the notebook holding the different graphical options
  TGLayoutHints *lnormal=new TGLayoutHints;
  Int_t i=0;
  TGCheckButton *box;

  fWidgets->Add(lnormal);
  while (opt[i].fOption!=NULL) {
    box=new TGCheckButton(parent,opt[i].fComment);
    box->SetUserData(opt[i].fOption);
    checkList->Add(box);
    parent->AddFrame(box,lnormal);
    i++;
 }
  
  return kTRUE;
}

void HTreeDrawPanel::insertText(Text_t *txt) {
  //Inserts the text "txt" in the current active formula.
  TGTextEntry *entry=0;
  if (fCbX->GetState()==kButtonDown) {
    entry=(TGTextEntry *)fCbX->GetUserData();
    entry->InsertText(txt,entry->GetCursorPosition());
  }
  if (fCbY->GetState()==kButtonDown) {
    entry=(TGTextEntry *)fCbY->GetUserData();
    entry->InsertText(txt,entry->GetCursorPosition());
  }
  if (fCbZ->GetState()==kButtonDown) {
    entry=(TGTextEntry *)fCbZ->GetUserData();
    entry->InsertText(txt,entry->GetCursorPosition());
  }
  if (fCbSel->GetState()==kButtonDown) {
    entry=(TGTextEntry *)fCbSel->GetUserData();
    entry->InsertText(txt,entry->GetCursorPosition());
  }
}

void HTreeDrawPanel::addSubBranches(TBranch *b,TGListTreeItem *parent) {
  //Add subbranches to a given branch b to the node "parent" in 
  //the GUI tree. This is used to create the GUI to navigate the
  //ROOT tree currently browsed. It is a recursive function
  TGListTreeItem *item;
  TBranch *branch=NULL;
  TIter next(b->GetListOfBranches());
  const Text_t *name;
  Text_t *nameEnd;

  while ( (branch=(TBranch *)next())!=NULL) {
    name=branch->GetName();
    nameEnd=name+strlen(name)-1;
    while (*nameEnd!='.' && nameEnd!=name) nameEnd--;
    if (*nameEnd=='.') nameEnd++;
    item=fBranchTree->AddItem(parent,nameEnd,branch);
    item->SetUserData(name);
    addSubBranches(branch,item);
  }  
}

Bool_t HTreeDrawPanel::buildBranchesTree(void) {
  //Creates the TGListTree used to interact with a ROOT tree.
  TGListTreeItem *item;
  TBranch *branch=NULL;
  TIter next(fTree->GetListOfBranches());

  while ( (branch=(TBranch *)next())!=NULL) {
    item=fBranchTree->AddItem(NULL,branch->GetTitle(),branch);
    item->SetUserData(branch->GetTitle());
    addSubBranches(branch,item);
  }
  return kTRUE;
}

TGCanvas *HTreeDrawPanel::buildTreeView(TGFrame *parent) {
  //Creates the full GUI for the navigation in the ROOT tree. Internally
  //this method calls buildBranchesTree and addSubBranches
  TGCanvas *treeView=new TGCanvas(parent,10,10,kSunkenFrame | kDoubleBorder);
  
  fBranchTree = new TGListTree(treeView->GetViewPort(), 10, 10, 
	 		       kHorizontalFrame,fgWhitePixel);
  fBranchTree->Associate(this);
  treeView->SetContainer(fBranchTree);
  buildBranchesTree();

  return treeView;
}

void HTreeDrawPanel::initGUI(void) {
  //Creates the GUI of the panel.
  TGLayoutHints *lExpandXY,*lExpandX,*lExpandY,*lo;
  TGVerticalFrame *mainFrame=0;
  TGHorizontalFrame *optFrame=0,*baseFrame=0;
  TGGroupFrame *graphOpts=0;
  TGTextButton *button=0;
  TGCanvas *treeView=0;
  TGTab *notebook=0;

  lExpandXY=new TGLayoutHints(kLHintsExpandX | kLHintsExpandY);
  fWidgets->Add(lExpandXY);
  lExpandX=new TGLayoutHints(kLHintsExpandX);
  fWidgets->Add(lExpandX);
  lExpandY=new TGLayoutHints(kLHintsExpandY);
  fWidgets->Add(lExpandY);

  // Base Frame
  baseFrame=new TGHorizontalFrame(this,10,10);
  fWidgets->Add(mainFrame);
  
  treeView=buildTreeView(baseFrame);
  fWidgets->Add(treeView);
  baseFrame->AddFrame(treeView,lExpandXY);

  // Main Frame
  mainFrame=new TGVerticalFrame(baseFrame,10,10);
  fWidgets->Add(mainFrame);

  setupFormulaLine("&X: ",fCbX,fTxX,mainFrame,kCEditX,lExpandX);
  setupFormulaLine("&Y: ",fCbY,fTxY,mainFrame,kCEditY,lExpandX);
  setupFormulaLine("&Z: ",fCbZ,fTxZ,mainFrame,kCEditZ,lExpandX);
  setupFormulaLine("&Sel: ",fCbSel,fTxSelection,mainFrame,kCEditSel,lExpandX);
  
  //Options Frame
  optFrame=new TGHorizontalFrame(mainFrame,10,10);
  fWidgets->Add(optFrame);
  lo=new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,2,2,2,2);
  fWidgets->Add(lo);
  
  //Options notebook
  graphOpts=new TGGroupFrame(optFrame,"Options");
  fWidgets->Add(graphOpts);
  notebook=new TGTab(graphOpts,10,10);
  fWidgets->Add(notebook);
  
  notebook->AddTab("General");
  buildGraphOptions(fCommonOpts,notebook->GetTabContainer(0),fCbCommon);
  notebook->AddTab("1D");
  buildGraphOptions(f1DOpts,notebook->GetTabContainer(1),fCb1DOptions);
  notebook->AddTab("2D");
  buildGraphOptions(f2DOpts,notebook->GetTabContainer(2),fCb2DOptions);

  graphOpts->AddFrame(notebook,lExpandY);
  optFrame->AddFrame(graphOpts,lExpandY);
  //!Options notebook

  //Histogram configuration
  graphOpts=new TGGroupFrame(optFrame,"Histogram config.");
  fWidgets->Add(graphOpts);
  buildHistoOptions(graphOpts);
  optFrame->AddFrame(graphOpts,lExpandY);
  //!Histogram config
  mainFrame->AddFrame(optFrame,lExpandXY);
  //!Options Frame

  //Buttons
  optFrame=new TGHorizontalFrame(mainFrame,10,10);
  fWidgets->Add(optFrame);
  
  button=new TGTextButton(optFrame,"Draw",kCDraw);
  button->Associate(this);
  fWidgets->Add(button);
  optFrame->AddFrame(button,lo);
  button=new TGTextButton(optFrame,"Clear",kCClear);
  button->Associate(this);
  fWidgets->Add(button);
  optFrame->AddFrame(button,lo);
  button=new TGTextButton(optFrame,"Close",kCClose);
  button->Associate(this);
  fWidgets->Add(button);
  optFrame->AddFrame(button,lo);

  mainFrame->AddFrame(optFrame,lExpandX);
  //!Buttons

  baseFrame->AddFrame(mainFrame,lExpandY);
  //! Main Frame
  
  AddFrame(baseFrame,lExpandXY);
  //! Base Frame

  MapSubwindows();
  SetWindowName("Tree draw panel");
  Resize(GetDefaultSize());
  MapWindow();
}

void HTreeDrawPanel::drawHistogram(void) {
  //This is the function called when the "Draw" button is pressed. It
  //includes the logic to extract the info to generate one histogram from
  //the different GUI elements in the panel, and creates the corresponding
  //histogram.
  Text_t *varexp=0,opts[512]="";
  const Text_t *sel=fTxSelection->GetString();
  Int_t size=fTxX->GetTextLength();
  TGMsgBox *msgbox=0;
  TIterator *iter=0;
  TGCheckButton *b=0;
  HAxisParam hParam[3];
  Int_t i=0;
  TGWindow *curWindow=this;

  iter=fCbCommon->MakeIterator();
  while ( (b=(TGCheckButton *)iter->Next())!=NULL) {
    if (b->GetState()==kButtonDown) strcat(opts,(char *)b->GetUserData());
  }
  delete iter;
  
  if (fCustomHistogram) {
    for (i=0;i<3;i++) {
      if ( ( hParam[i].readNBins(fHistOptions[i][0]->GetString()) && 
	     hParam[i].readMin(fHistOptions[i][1]->GetString()) &&
	     hParam[i].readMax(fHistOptions[i][2]->GetString()) )) {
	hParam[i].setCorrect(kTRUE);
      }
    }
  }

  if (size==0) {
    msgbox=new TGMsgBox(gClient->GetRoot(),curWindow,"Error",
			"No X specified",kMBIconExclamation);
    return;
  } else if (fTxY->GetTextLength()==0) { //Only X
    varexp=new Text_t[size+1];
    strcpy(varexp,fTxX->GetString());
    if (fCustomHistogram) {
      if (hParam[0].isCorrect()) {
	fPrevHistogram=new TH1F("tempHist",fHTitle->GetString(),
				hParam[0].getNBins(),hParam[0].getMin(),
				hParam[0].getMax());
	strcat(varexp,">>tempHist");
      }
    }
    iter=fCb1DOptions->MakeIterator();
    while ( (b=(TGCheckButton *)iter->Next())!=NULL) {
      if (b->GetState()==kButtonDown) strcat(opts,(char *)b->GetUserData());
    }
    delete iter;

  } else if (fTxZ->GetTextLength()==0) { // X vs Y
    size+=fTxY->GetTextLength();
    varexp=new Text_t[size+2];
    strcpy(varexp,fTxY->GetString());
    strcat(varexp,":"); strcat(varexp,fTxX->GetString());
    if (fCustomHistogram) {
      if (hParam[0].isCorrect() && hParam[1].isCorrect()) {
	fPrevHistogram=new TH2F("tempHist",fHTitle->GetString(),
				hParam[0].getNBins(),hParam[0].getMin(),
				hParam[0].getMax(),hParam[1].getNBins(),
				hParam[1].getMin(),hParam[1].getMax());
	strcat(varexp,">>tempHist");
      }
    }
    iter=fCb2DOptions->MakeIterator();
    while ( (b=(TGCheckButton *)iter->Next())!=NULL) {
      if (b->GetState()==kButtonDown) strcat(opts,(char *)b->GetUserData());
    }
    delete iter;

  } else { // X vs Y vs Z
    size=size+fTxY->GetTextLength()+fTxZ->GetTextLength();
    varexp=new Text_t[size+3];
    strcpy(varexp,fTxX->GetString());
    strcat(varexp,":"); strcat(varexp,fTxY->GetString());
    strcat(varexp,":"); strcat(varexp,fTxZ->GetString());
    if (fCustomHistogram) {
      if (hParam[0].isCorrect() && hParam[1].isCorrect()
	  && hParam[2].isCorrect()) {
	fPrevHistogram=new TH3F("tempHist",fHTitle->GetString(),
				hParam[0].getNBins(),hParam[0].getMin(),
				hParam[0].getMax(),hParam[1].getNBins(),
				hParam[1].getMin(),hParam[1].getMax(),
				hParam[2].getNBins(),hParam[2].getMin(),
				hParam[2].getMax());
	strcat(varexp,">>tempHist");
      }
    }
  }

  fTree->Draw(varexp,sel,opts);
  gPad->Update();
}


Bool_t HTreeDrawPanel::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2) {
  //Internal function of the ROOT GUI classes used to process the messages
  //generated by the GUI code. Read the ROOT documentation for more info.
  HTFormulaComposer *c=0;
  switch (GET_MSG(msg)) {
    
  case kC_TEXTENTRY:
    switch (GET_SUBMSG(msg)) {
    case kTE_TEXTCHANGED:
      if (parm1==kUpdateFormula) {
	TIterator *iter=fFormulas->MakeIterator();
	TGFrame *f=0;
	while ( (f=(TGFrame *)iter->Next())!=NULL) 
	  fClient->NeedRedraw(f);
      }
      break;
    }
    break;
  case kC_COMMAND:
    switch (GET_SUBMSG(msg)) {
    case kCM_BUTTON:
      switch (parm1) {
      case kCEditX:
	c=new HTFormulaComposer(fTree,fTxX,gClient->GetRoot(),this,kCEditX,
				500,300);
	c->Resize(300,300);
	c->Associate(this);
	fClient->WaitFor(c);
	break;
      case kCEditY:
	c=new HTFormulaComposer(fTree,fTxY,gClient->GetRoot(),this,kCEditY,
				500,300);
	c->Associate(this);c->Resize(300,300);
	fClient->WaitFor(c);
	break;
      case kCEditZ:
	c=new HTFormulaComposer(fTree,fTxZ,gClient->GetRoot(),this,kCEditZ,
				500,300);
	c->Associate(this);c->Resize(300,300);
	fClient->WaitFor(c);
	break;
      case kCEditSel:
	c=new HTFormulaComposer(fTree,fTxSelection,gClient->GetRoot(),this,
				kCEditSel,500,300);
	c->Associate(this);c->Resize(300,300);
	fClient->WaitFor(c);
	break;
      case kCDraw:
	drawHistogram();
	break;
      case kCClose:
	CloseWindow();
	break;
      case kCClear:
	((TGTextEntry *)fCbX->GetUserData())->Clear();
	((TGTextEntry *)fCbY->GetUserData())->Clear();
	((TGTextEntry *)fCbZ->GetUserData())->Clear();
	((TGTextEntry *)fCbSel->GetUserData())->Clear();
	break;
      default:
	break;
      }
      break;
    case kCM_RADIOBUTTON:
      fCbX->SetState(kButtonUp);
      fCbY->SetState(kButtonUp);
      fCbZ->SetState(kButtonUp);
      fCbSel->SetState(kButtonUp);
      switch (parm1) {
      case kCbX:
	fCbX->SetState(kButtonDown); 
	((TGTextEntry *)fCbX->GetUserData())->SetFocus();
	break;
      case kCbY:
	fCbY->SetState(kButtonDown); 
	((TGTextEntry *)fCbY->GetUserData())->SetFocus();
	break;
     case kCbZ:
	fCbZ->SetState(kButtonDown); 
	((TGTextEntry *)fCbZ->GetUserData())->SetFocus();
	break;
     case kCbSel:
	fCbSel->SetState(kButtonDown); 
	((TGTextEntry *)fCbSel->GetUserData())->SetFocus();
	break;
      case kRbCustom:
	fRbCustom->SetState(kButtonDown); 
	fRbDefault->SetState(kButtonUp);
	fCustomHistogram=kTRUE;
	break;
      case kRbDefault:
	fRbCustom->SetState(kButtonUp); 
	fRbDefault->SetState(kButtonDown);
	fCustomHistogram=kFALSE;
	break;
      }
      break;
    default:
      break;
    }
    break;
  case kC_LISTTREE:
    switch (GET_SUBMSG(msg)) {      
    case kCT_ITEMCLICK:
      if (parm1 == kButton1 || parm1 == kButton2 || parm1 == kButton3) {
	TGListTreeItem *item;
	if ((item = fBranchTree->GetSelected()) != 0 &&
	    item != fSelectedItem) {
	  fBranchTree->HighlightItem(item);
	  fClient->NeedRedraw(fBranchTree);
	  fSelectedItem=item;
	}
      }
      if (parm1 == kButton2) {
	if (fSelectedItem->GetFirstChild()==0) {
	  insertText((Text_t *)fSelectedItem->GetUserData());
	}
      }
      break;
    case kCT_ITEMDBLCLICK:
      if (parm1 == kButton1) {
	TGListTreeItem *item;
	if ((item=fBranchTree->GetSelected()) != 0) {
	  fClient->NeedRedraw(fBranchTree);
	  fSelectedItem=item;
	  if (fSelectedItem->GetFirstChild()==0) {
	    insertText((Text_t *)fSelectedItem->GetUserData());
	  }
	}
      }
      break;
    default:
      break;
    }
    break;
  default:
    break;
  }
  return kTRUE;
}















