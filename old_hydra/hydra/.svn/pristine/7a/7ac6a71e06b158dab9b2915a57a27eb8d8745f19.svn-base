#ifndef HRICHUNPACKERCURRENT_H
#define HRICHUNPACKERCURRENT_H

#include "hldunpack.h"
#include "TString.h"

#define DEBUG  0x3f //in scaler format jan04
#define RICH_ADC_CRATE 4
#define RICH_ADC_SLOT  13
#define RICH_ADC_MIN   0
#define RICH_ADC_MAX   5

class HIterator;
class HCategory;
class TH2F;
class TCanvas;

class HRichUnpackerCurrent: public HldUnpack {
private:
  void HRichUnpackerCurrent::setStyle();

  Int_t startEvt;             //!
  Int_t iEventNr;             //!
  Int_t histEvt;              //! number of events in control histos (multiplier of 2000)
  Int_t subEvtId;             //! subevent id
  HCategory* m_pHitHdrCat;    //! Rich header
  HIterator* fIterHitHeader;  //!
  TString psFileName;         //!

  TCanvas *c;
  TH2F *h1;
  TH2F *h2;
  TH2F *h3;
  TH2F *h4;
  TH2F *h5;
  TH2F *h6;

public:
  HRichUnpackerCurrent(Int_t id = 416, Int_t nrEvtHist = 1000000,
		       Int_t strtEvt = 0, TString psName = "tmp");
  ~HRichUnpackerCurrent(void) {}
  Int_t getSubEvtId() const { return subEvtId; }
  Int_t execute();
  Bool_t init(void);
  Bool_t finalize(void);
  Bool_t writePdfFile(void);

  ClassDef(HRichUnpackerCurrent,1)         // unpack RICH current data
};

#endif /* !HRICHUNPACKERCURRENT_H */
