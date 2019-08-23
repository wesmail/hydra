#ifndef HRICHIPUPARTHRESHOLDS_H
#define HRICHIPUPARTHRESHOLDS_H

#include <TObject.h>
#include <TObjArray.h>
#include "hlocation.h"
#include "hparset.h"


class HRichIPUParThresholds : public HParSet {
private: 
  Int_t thresholds[2][6];  

  void printPause(void);
public:
  HRichIPUParThresholds(const char* name="RichIPUParThresholds",
	      const char* title="Parameters for Rich IPU Thresholds ",
	      const char* context="RICHIPUEmulation");
  ~HRichIPUParThresholds(void);

  Int_t getRegionSize() {return 2;}
  Int_t getThresholds(Int_t re, Int_t sec){return thresholds[re][sec];}
  void setThresholds(Int_t re, Int_t sec, Int_t thr){thresholds[re][sec]=thr;}

  Bool_t init(HParIo*,Int_t*);
  Int_t write(HParIo*);
  void clear();
  void printParam();
  void readline(const char*, Int_t*);
  void putAsciiHeader(TString&);
  Bool_t writeline(char*, Int_t, Int_t); 
  void setDefaults(void);

  ClassDef(HRichIPUParThresholds,1)  // Container for the RICH IPU Thresholds parameters
};

#endif /* !HRICHIPUPARTHRESHOLDS_H */











