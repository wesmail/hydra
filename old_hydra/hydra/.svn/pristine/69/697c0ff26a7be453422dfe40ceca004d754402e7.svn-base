#ifndef HSCALERSREADER_H
#define HSCALERSREADER_H

#include "hreconstructor.h"
#include <iostream.h>
#include <hstartdef.h>
#include <tofdef.h>

class HCategory;
class HIterator;
class TH1F;
class TH2F;
class TFile;

class HScalersReader : public HReconstructor {
protected:
  HCategory* fTBoxCat;       //! pointer to the tof raw data
  HIterator* iter_tbox;         //! iterator on tof raw data.

  TH1F *histStartScalerPattern;
  TH1F *histVetoScalerPattern;
  TH1F *histInputScalerPattern;
  TH1F *histReduceScalerPattern;

  TH1F *histReduce[8];
  TH1F *histInput[8];
  TH1F *histStartScaler[8];
  TH1F *histVetoScaler[8];

  Int_t counter_execute;

  Char_t* fNameRootScalersReader;   // file name of root output file
  TFile *ofile;
  void createHist(TFile*);
  void writeHist (TFile*);

public:
  HScalersReader();
  HScalersReader(Text_t* name,Text_t* title);
  ~HScalersReader();
  Bool_t init(void);
  Bool_t finalize(void) {return kTRUE;}
  Int_t execute(void);
  void initParContainer(){;};
  void setOutputRoot(Char_t*);

  ClassDef(HScalersReader,0)
};

#endif /* !HSCALERSREADER_H */

