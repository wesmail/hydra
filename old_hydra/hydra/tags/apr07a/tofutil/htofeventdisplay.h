#if !defined HTOFEVENTDISPLAY_H
#define HTOFEVENTDISPLAY_H

#include <TObjArray.h>
#include <TH1.h>
#include <TH2.h>
#include <TRootEmbeddedCanvas.h>
#include <TGTextEntry.h>
#include <TGTextBuffer.h>
#include <TGButton.h>
#include <TGListBox.h>
#include <TTimer.h>

enum ConfigBits {

  B_BEAM = 0,
  B_LASER,
  B_ROOT,
  B_HLD,
  B_DAQ,
  B_CAL,
  B_SEC1,
  B_SEC2,
  B_SEC3,
  B_SEC4,
  B_SEC5,
  B_SEC6

};


enum MainCommandsId {

  M_CLOSE_MAIN = 100,
  M_RAW,
  M_HIT,
  M_MULTIH,
  M_PRINT,
  M_CONFIG,
  M_START,
  M_RESET,
  M_CLEAR,
  M_ZOOM,
  M_DRAW_SC,
  M_SCALE,
  M_LIST_SEC,
  M_LIST_MOD,
  M_LIST_ROD
 
};


class HTofHisto : public TObject {
 public:
  TH1F *left;  // left time
  TH1F *right; // right time
  TH1F *tof;   // time-of-flight
  TH1F *xpos;  // x position
  TH1F *halfs; // half sum
  TH1F *halfd; // half diff		    
  //  TH2F *lr;    // left vs. right
  //  TH2F *lt;    // left vs. tof
  //  TH2F *lx;    // left vs. xpos
  //  TH2F *rt;    // right vs. tof
  //  TH2F *rx;    // right vs. xpos
  //  TH2F *tx;    // tof vs. xpos
 public:
  HTofHisto() : TObject() {}
  HTofHisto(Int_t sec, Int_t mod, Int_t rod);
  void reset();
  virtual ~HTofHisto() {}

  ClassDef(HTofHisto,1)
};


class HTofGeom;

class HTofEventDisplay : public TGMainFrame {

 protected:
  TGCompositeFrame        *fFrame;
  TGCompositeFrame        *fF1, *fF2;
  TGCompositeFrame        *fA1, *fA2, *fA3;
  TGCompositeFrame        *fB1, *fB2, *fB3;
  TGCompositeFrame        *fC1, *fC2, *fC3, *fC4;
  TGCompositeFrame        *fD1, *fD2, *fD3;
  TGCompositeFrame        *fE1, *fE2;
  TGListBox               *fListSec, *fListMod, *fListRod;
  TRootEmbeddedCanvas     *fCanvas;
  TGButton                *fConfig, *fRaw, *fHit, *fMultih, *fClose;
  TGButton                *fClear, *fZoom, *fPrint;
  TGButton                *fLoop;
  TGButton                *fDrawSc, *fScale;
  TGTextEntry             *fSetEv, *fScText;
  TGTextBuffer            *fSetEvBuf, *fScBuf;
  TGButton                *fStart, *fReset;

 protected:
  TObjArray        *fHistoCont;   // histogram container
  Char_t            source[100];  // input data source
  Char_t            target[100];  // output data file
  Char_t            calib[100];   // calibration parameters file
  Short_t           config;       // 12 bit configuration word
  Bool_t            kConfigured;
  Bool_t            kRaw;         // to pick separately raw and hit histos
  Bool_t            kHit;
  Bool_t            kMulti;
  HTofGeom          *fTofGeom;
  Bool_t            kZOOM;
  Bool_t            kSector, kModule, kRod;
  //-------------------------------------------------
  /*  config:  bit 0  - beam run
               bit 1  - laser run
               bit 2  - root source
               bit 3  - hld source
               bit 4  - daq source
               bit 5  - calibration parameters
               bit 6  - sector 1
               bit 7  - sector 2
	       bit 8  - sector 3
	       bit 9  - sector 4
	       bit 10 - sector 5
	       bit 11 - sector 6
  */
  //--------------------------------------------------

 public:
  HTofEventDisplay(const TGWindow *p, UInt_t w, UInt_t h);
  virtual ~HTofEventDisplay();
  virtual void CloseWindow();
  virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);
  TGListBox *getListSec() const { return fListSec; }
  TGListBox *getListMod() const { return fListMod; }
  TGListBox *getListRod() const { return fListRod; }
  void setConfigBit(Int_t bit, Bool_t stat = kTRUE);
  Int_t testConfigBit(Int_t bit); 
  Bool_t readDefaultConfig();
  Bool_t setConfiguration();
  TObjArray *getHistoCont() const {return fHistoCont;}
  Bool_t addHist(Int_t sec);
  void setInput(const char *name) { if(name) sprintf(source,"%s",name); }
  void setOutput(const char *name) { if(name) sprintf(target,"%s",name); }
  void setCalib(const char *name) { if(name) sprintf(calib,"%s",name); }
  const char *getInput() const { return source; }
  const char *getOutput() const { return target; } 
  const char *getCalib() const { return calib; }
  Bool_t isConfigured() { return kConfigured; }
  TGClient *getClient() { return fClient; }
  HTofGeom *getTofGeom() { return fTofGeom; } 
  virtual Bool_t HandleTimer(TTimer *t);
  Bool_t toGreen(Window_t id);
  Bool_t toRed(Window_t id);
  Bool_t isRaw() { return kRaw; } 
  Bool_t isHit() { return kHit; }
  Bool_t isMulti() { return kMulti; }
  Bool_t isZoom() { return kZOOM; } 
  Bool_t isSector() { return kSector; }
  Bool_t isModule() { return kModule; }
  Bool_t isRod() { return kRod; }

  ClassDef(HTofEventDisplay,0)

};


class HTofDisplayTimer: public TTimer {

private:

  HTofEventDisplay  *fTimer;

public:

  HTofDisplayTimer(HTofEventDisplay *p, Long_t ms) : 
    TTimer(ms,kTRUE) { fTimer = p; }
  Bool_t Notify();

};

R__EXTERN HTofEventDisplay *gTofDisplay;

#endif









