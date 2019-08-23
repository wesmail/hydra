#if !defined HCONVERSION_H
#define HCONVERSION_H

//-------------------------------------------------------------------------
//
// File name:       hconversion.h
// Subject:         utility GUI class to provide a conversion from the 
//                  "global"scintillator number (1-384) to 
//                  the set of indexes sector/module/strip.
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            November 2000
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the declaration of the class HConvertion.
// This is a small utility GUI class destined to help a user in converting
// scintillator numbers to their indexes in the structure sector/module/strip.
//
//------------------------------------------------------------------------

#include <TGListBox.h>
#include <TGTextEntry.h>
#include <TGTextBuffer.h>
#include <TGButton.h>


class HConversion : public TGMainFrame {

  protected:

  TGCompositeFrame    *fFrame, *fF1, *fF2;
  TGListBox           *fSecList, *fModList, *fRodList;
  TGButton            *fConvert;
  TGTextEntry         *fNumber;
  TGTextBuffer        *fNumberBuf;

  public:
  
  static Int_t         countConvert;

  public:

  HConversion(const TGWindow *p, UInt_t w, UInt_t h);
  virtual ~HConversion();
  virtual void CloseWindow();
  virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);

  public:
  
  ClassDef(HConversion,0) //GUI class for TOF index conversions

};

#endif




