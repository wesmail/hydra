#if !defined HPREFERENCES_H
#define HPREFERENCES_H

//-------------------------------------------------------------------------
//
// File name:       hpreferences.h
// Subject:         Preferences (canvas options, memory limits).
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            April 2000
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the declaration of the class HPreferences.
// This is a GUI class which allows the user to define and save 
// the preferences.
// That means canvas dimensions, background color etc.
//
//-------------------------------------------------------------------------

#include <TGTab.h>
#include <TGComboBox.h>
#include <TGButton.h>

enum PrefComId {

  M_PREF_CLOSE = 100,
  M_PREF_APPLY,
  M_PREF_SET,
  M_PREF_MENUBAR,
  M_PREF_NOMENUBAR,
  M_PREF_COLOR,
  M_PREF_WIDTH,
  M_PREF_HEIGHT,
  M_PREF_MEM

};

class HPreferences : public TGMainFrame {

 protected:

  TGCompositeFrame    *fFrame;
  TGTab               *fTab;
  TGCompositeFrame    *fMemFrame;
  TGCompositeFrame    *fCanvFrame;
  TGComboBox          *fMemCB;
  TGComboBox          *fCanvColorCB;
  TGComboBox          *fCanvWidthCB, *fCanvHeightCB;
  TGRadioButton       *fMenuBarRB, *fNoMenuBarRB;
  TGButton            *fClose, *fApply, *fSet;
  TGCompositeFrame    *fF1, *fF2, *fF3;
  TGCompositeFrame    *fF0, *fF00;

 protected:

  TGMainFrame         *fParent;

 public:

  static Int_t         countPref;

 public:

  HPreferences(const TGWindow *p, UInt_t w, UInt_t h, TGMainFrame *p2 = 0);
  virtual ~HPreferences();
  virtual void CloseWindow();
  virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);
  void readPref();
  void setPref();
  void applyPref();

 public:

  ClassDef(HPreferences,0) //Preferences (canvas options, memory limits)

};

#endif


