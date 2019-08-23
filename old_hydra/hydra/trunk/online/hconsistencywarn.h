#if !defined HCONSISTENCYWARN_H
#define HCONSISTENCYWARN_H

//-------------------------------------------------------------------------
//
// File name:       hconsistencywarn.h
// Subject:         GUI class to provide a communication channel between 
//                  a user and the program on the subject of inconsistent 
//                  objects.
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            March 2000
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the declaration of the class HConsistencyWarn.
// This is a GUI class destined to warn a user about the presence of
// inconsistent HTOP objects.
//
//------------------------------------------------------------------------

#include "TGListBox.h"
#include "TGButton.h"

class HConsistency;

class HConsistencyWarn : public TGMainFrame {

 protected:

  TGCompositeFrame    *fFrame, *fF1, *fF2, *fF3;
  TGListBox           *fLCList, *fGCList, *fHiList;
  TGButton            *fDelete, *fCancel;
  Int_t               *fRetCode; // 1 - Delete; 2 - Cancel

 protected:

  void fillList();

 public:

  HConsistencyWarn(const TGWindow *p, UInt_t w, UInt_t h, Int_t* retcode);
  virtual ~HConsistencyWarn();
  virtual void CloseWindow();
  virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);

 public:

  ClassDef(HConsistencyWarn,0) //GUI class for consistency check operations

};

#endif




