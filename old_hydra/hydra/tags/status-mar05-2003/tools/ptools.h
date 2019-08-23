#ifndef PTOOLS__H
#define PTOOLS__H

////////////////////////////////////////////////////////////////////////////////
//
// PTools
// Peter's Tools
// 
//            Author: Peter W.Zumbruch
//           Contact: P.Zumbruch@gsi.de
//           Created: Mar 21, 2002
// Last modification: Nov 21, 2002
// 
////////////////////////////////////////////////////////////////////////////////

#include <TPaveText.h>
#include <TStopwatch.h>
#include <TH1.h>
#include <TH2.h>
#include <THStack.h>
#include <TCanvas.h>

class PTools 
{
 public:
  static TPad* getDividedSubPad(TCanvas  * canv, 
				Int_t xpads, 
				Int_t ypads);

  static TPad* getDividedSubPad(TCanvas  * canv, 
				Int_t xpads, 
				Int_t ypads,
				const Char_t * title);
  
  static TPad* getDividedSubPad(TCanvas  * canv, 
				Int_t xpads, 
				Int_t ypads, 
				const Char_t * title, 
				const Char_t * filename);

  static TPad* getDividedSubPad(TCanvas  * canv, 
				Int_t xpads, 
				Int_t ypads, 
				const Char_t * title, 
				const Char_t * filename, 
				Int_t nentries);
  
  static TPad* getDividedSubPad(TCanvas  * canv, 
				Int_t xpads, 
				Int_t ypads, 
				const Char_t * title, 
				const Char_t * filename, 
				Int_t nentries, 
				TPaveText * info);
  
  static TPad* getDividedSubPad(TCanvas  * canv,
				Int_t xpads, 
				Int_t ypads, 
				const Char_t * title, 
				const Char_t * filename,
				Int_t nentries, 
				const Char_t * info);
  
  static TPad* getDividedSubPad(TCanvas  * canv, 
				Int_t xpads, 
				Int_t ypads, 
				const Char_t * title, 
				const Char_t * filename, 
				Int_t nentries, 
				TString info);
  
  static void create4Histogramms(TH1F**, const Char_t *, const Char_t *, const Char_t *, const Char_t *, Int_t ,double ,double );
  static void printHeader(const Char_t *titlehead=0, const Char_t *mytextfilename=0, const Int_t nentries=0, TPad * pad = 0);
  static void evalStopwatch(TStopwatch *timer, Int_t nevts);
  static void setTitleArts( TH1* histogramm, const Char_t *xtitle, const Char_t *ytitle, int color = 4, int type = 42);
  static void setTitleArts( TH1* histogramm, const TString xtitle, const TString ytitle, int color = 4, int type = 42);
  static void setTitleArts( THStack* histogrammStack, const Char_t *xtitle, const Char_t *ytitle, int type = 42);
  static void setTitleArts( THStack* histogrammStack, const TString xtitle, const TString ytitle, int type = 42);

  static TCanvas* drawCanvas(const Char_t * , const Char_t * , TH1 **, Int_t , Int_t );
  static TCanvas* draw32Canvas(const Char_t * , const Char_t * , TH1F **);
  static TCanvas* draw32CanvasTH2(const Char_t * , const Char_t * , TH2F **, const Char_t * option=0);

  static void cutEnd(Char_t * , Char_t * );
  static char* myexec(char * );
  static void mdb(int b=-1);

  static TDirectory *Mkdir(TDirectory *dirOld, const Char_t *newDirName, Int_t i = -99, Int_t precision = 1); 
  static TDirectory *Mkdir(TDirectory *dirOld, const TString newDirName, Int_t i = -99, Int_t precision = 1); 

  ClassDef(PTools,0) // Peters Tools 
};

#endif
