#if !defined HSLICE_H
#define HSLICE_H

//-------------------------------------------------------------------------
//
// File name:       hslice.h
// Subject:         Slicing of 2dim histogrammes.
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            March 2000
//
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the declaration of the class HSlice.
// This is a GUI class which allows to slice 2dim histogrammes.
//
//-------------------------------------------------------------------------

#include <TRootEmbeddedCanvas.h>
#include <TGButton.h>
#include <TGSlider.h>
#include <TGListBox.h>
#include <TGComboBox.h>
#include <TGTextEntry.h>
#include <TGTextBuffer.h>

enum SliderComId {

  M_VSLIDER = 100,
  M_HSLIDER,
  M_XSLICE,
  M_YSLICE,
  M_2DHIST,
  M_RESCALE,
  M_FIX,
  M_REDRAW

};


class TPad;
class TH2F;
class TObjArray;
class TArrayI;

class HSlice : public TGMainFrame {

 protected:

  TGCompositeFrame             *fFrame;
  TGCompositeFrame             *fF1, *fF2, *fF3;
  TGCompositeFrame             *fF21, *fF22;
  TGCompositeFrame             *fF31, *fF32;
  TGCompositeFrame             *fF221, *fF222;
  TGCompositeFrame             *fF311, *fF312, *fF313, *fF314;
  TGCompositeFrame             *fF321, *fF322;
  TGRadioButton                *fXSlice, *fYSlice, *f2dHist;
  TRootEmbeddedCanvas          *fSliceCanv;
  TGHSlider                    *fHSlider;
  TGVSlider                    *fVSlider;
  TGListBox                    *fVLB1, *fVLB2, *fVLB3;
  TGListBox                    *fHLB1, *fHLB2, *fHLB3;
  TGTextEntry                  *fMax;
  TGTextBuffer                 *fMaxBuf;
  TGButton                     *fFix, *fRescale;
  TGComboBox                   *fOptionCB;
  TGButton                     *fRedraw;

 protected:

  TPad                         *fCurPad;
  Int_t                         fVMin;
  Int_t                         fVMax;
  Int_t                         fHMin;
  Int_t                         fHMax;
  Int_t                         fVPos;
  Int_t                         fHPos;
  TObjArray                    *fArray;
  TArrayI                      *fXArray;
  TArrayI                      *fYArray;
  Int_t                        *fMode;

 public:

  static Int_t                  countSlice;

 public:

  HSlice(const TGWindow *p, UInt_t w, UInt_t h, TObjArray *p1 = 0,
	 TArrayI *p2 = 0, TArrayI *p3 = 0, Int_t *p4 = 0);
  virtual ~HSlice();
  virtual void CloseWindow();
  virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);
  void makeXslice(Bool_t kRescale = kFALSE);
  void makeYslice(Bool_t kRescale = kFALSE);
  void drawHist();
  void setSliderRange(TH2F *hist);
  void setCB(Bool_t kClear = kTRUE);
  void setState(Bool_t kEnable = kTRUE);
  void restoreStatus();
  void setCursors(const char *name);

 public:

  ClassDef(HSlice,0)

};

#endif
