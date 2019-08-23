//*-- Author : Dan Magestro (magestro@gsi.de)
//*-- Modified : 24/01/2002
#ifndef HQAOUTPUTPS_H
#define HQAOUTPUTPS_H

#include <TPostScript.h>

class TCanvas;
class TPad;

class HQAOutputPS : public TPostScript {

public:
  HQAOutputPS() {}
  HQAOutputPS(TString fname);
  ~HQAOutputPS();

  void writeHist(TList *h);
  void makeHist(TList *h);
  void makeText(TList *h);

  void generatePDF(Bool_t kpdf) {kPDF = kpdf;}

  void closePS();

  void setNHistPerPage(Int_t nh) {nHistPerPage = nh;}

  inline void setStats(Int_t nEvents, Int_t nQAProcessed = 0);
  inline void setDSTFileName(const char *file) {fnameDST = file;}

private:
  TCanvas *fCanvas;       //! canvas
  TPad    *fHistPad;      //! histogram pad

  Int_t pageCount;        //! counts pages of a certain type
  Int_t histCount;        //! counts histograms for page breaks
  Int_t nProcessed;       //! number of events QA'd
  Int_t nHistPerPage;     //! user-supplied parameter (default=6)
  Int_t nEvent;           //! total number of events in output file

  Bool_t kFIRST;          //! needed for one-time formatting
  Bool_t kPDF;            //! generate PDF file using ps2pdf; default = kTRUE

  TString fnamePS;        //! PS filename
  TString fnameDST;       //! DST filename (used in page header)

  void makeNewPage(TString type);
  void setStyle();
  void makePDF();

  ClassDef(HQAOutputPS,1) // QA Postscript file generator
};

inline void HQAOutputPS::setStats(Int_t nEvents, Int_t nQAProcessed) {
  nEvent = nEvents;
  nProcessed = nQAProcessed;
}

#endif
