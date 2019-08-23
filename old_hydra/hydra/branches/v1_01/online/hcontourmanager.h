#if !defined HCONTOURMANAGER_H
#define HCONTOURMANAGER_H

//-------------------------------------------------------------------------
//
// File name:       hcontourmanager.h
// Subject:         GUI class for contour management.
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            April 2000
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the declaration of the class HContourManager.
// This is a GUI class which provides interface for contour definition.
//
//-------------------------------------------------------------------------

#include <TGButton.h>
#include <TGTextEntry.h>
#include <TGTextBuffer.h>
#include <TGListBox.h>

enum ContManId {

  M__C_HELP,
  M__C_INIT,
  M__C_UNDO,
  M__C_SET,
  M__C_CLEAR,
  M__C_ADD,
  M__C_REMOVE,
  M__C_PREV,
  M__C_NEXT,
  M__C_LIST

};

class HContour;
class TH2F;

class HContourManager : public TGMainFrame {

 protected:

  TGCompositeFrame            *fFrame;
  TGCompositeFrame            *fF1, *fF2;
  TGCompositeFrame            *fF10, *fF11, *fF12, *fF13, *fF14;
  TGCompositeFrame            *fF21, *fF22, *fF23;
  TGButton                    *fPrev, *fNext;
  TGButton                    *fAdd, *fRemove;
  TGButton                    *fHelp, *fInit, *fUndo, *fSet, *fClear;
  TGListBox                   *fList;
  TGListBox                   *fInLB, *fOutLB;
  TGTextEntry                 *fName;
  TGTextBuffer                *fNameBuf;

 protected:

  TGMainFrame                 *fWindow;

 protected:

  HContour                    *fContour;
  Int_t                        countIn; // number of hits inside contour
  Int_t                        countOut; // number of hits outside contour

 public:

  static Int_t                 countContour;

 public:

  HContourManager(const TGWindow *p, UInt_t w, UInt_t h, TGMainFrame *pp = 0);
  virtual ~HContourManager();
  virtual void CloseWindow() { delete this; }
  virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);
  void updateList();
  void updateCountBoxes();
  void clearContour();
  void countHits(TH2F *hist, HContour *cont);

 public:

  ClassDef(HContourManager,0)

};

#endif
