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
  TH2S                      *pHistTDCDT[16], *pHistTD[16];
  TH1S                      *pHistTDC[16], *pHistMult,
                            *pHistT1[16][12][8], *pHistT2[16][12][8];
  TH1D                      *htest[8];
  TCanvas                   *c1,*c2;
  Int_t                      mboMax,mboName[16];
  Char_t                     tmp1[20],tmp2[20],tmp3[20],mapfile[100];
  Int_t                      i,j,k,l;
  Int_t                      histNum, secNum, modNum, mboNum, chipNum, param;
  Bool_t                     allMbo;
  Float_t                    xMin, xMax, yMin, yMax;
  Long_t                     millisec;
 public:
  histDraw();
  histDraw(Int_t name, Bool_t fAllMbo, Float_t xmin, Float_t xmax, 
	   Float_t ymin, Float_t ymax, Long_t ms, Int_t secNum, 
  	   Int_t modNum, Int_t mboNum, Int_t chipNum, Int_t timeFlag );
  virtual ~histDraw();
  void histDraw::drawing(void);
  void  histDraw::stop(void);
  Bool_t Notify(); 
 public:
  ClassDef(histDraw,0)
};

R__EXTERN histDraw *gMdcGUI;

#endif

