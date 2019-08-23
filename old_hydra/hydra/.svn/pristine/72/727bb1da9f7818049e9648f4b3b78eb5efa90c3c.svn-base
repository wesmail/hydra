#ifndef HMDCMONITOR
#define HMDCMONITOR

#include "hreconstructor.h"
#include "hlocation.h"
#include <TMapFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TTree.h>

class HCategory;
class HIterator;
class HMdcDigitPar;
class HEvent;

class HMdcMonitor : public HReconstructor {
private:
  HLocation loc;                       // Location for new object
  HCategory *rawCat;                   // Pointer to MdcRaw data category
  HIterator *iter;                     //! Iterator over Cal2 category
  TMapFile *mfile;                   // Pointer to TMapFile
  HEvent *recEvt;
  TH2F *pHist[2][16];
  TH1F *pHistTimeDiff[2][16];
  TH1F *pHistTDC[2][16];
  TH1F *pHistMult[2];
  Int_t i,j;
  Int_t time1,time2,time3,time4,time5,time6;
  Int_t nHits, sector, module, mbo, tdc;
public:
  HMdcMonitor(void);
  HMdcMonitor(Text_t* name,Text_t* title);
  ~HMdcMonitor(void);
  Bool_t init(void);
  Bool_t finalize(void) {return kTRUE;}
  Int_t execute(void);
  ClassDef(HMdcMonitor,0) //Calibrater of Mdc data.
};

#endif /* !HMDCMONITOR_H */










