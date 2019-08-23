#if !defined HISTDRAW_H
#define HISTDRAW_H

//-------------------------------------------------------
//
// Draw and unpdate the control histogramms
//
//
//-------------------------------------------------------
 

enum HistId {
  TDCDT,
  DT,
  TDC,
  MULT
};

#include <TTimer.h>	
#include <TSystem.h>	
#include <TMapFile.h>
#include <TH2.h>
#include <TCanvas.h>


class histDraw : public TTimer {
 private:
  TMapFile                  *mfile;
  TH2S                      *pHistTDCDT[2][16], *pHistTD[2][16];
  TH1S                      *pHistTime1[2][16], *pHistTime2[2][16], 
                            *pHistTDC[2][16], *pHistMult[2],
                            *pHistT1[2][16], *pHistT2[2][16];
  TH1D                      *htest[2][8];
  TCanvas                   *c1,*c2;
  Int_t                     mboMax[2],mboName[16];
  Char_t                    tmp1[20],tmp2[20],tmp3[20],mapfile[50];
  Int_t                     i,j;
  Int_t                     histNum, secNum, modNum, mboNum, param;
  Bool_t                    allMbo;
  Float_t                   xMin, xMax, yMin, yMax;
  Long_t                    millisec;
 public:
  histDraw();
  histDraw(Int_t name, Bool_t fAllMbo, Float_t xmin, Float_t xmax, 
	   Float_t ymin, Float_t ymax, Long_t ms, Int_t secNum, 
  	   Int_t modNum, Int_t mboNum, Int_t timeFlag );
  virtual ~histDraw();
  void histDraw::drawing(void);
  void  histDraw::stop(void);
  Bool_t Notify(); 
 public:
  ClassDef(histDraw,0)
};

R__EXTERN histDraw *gMdcGUI;

#endif












