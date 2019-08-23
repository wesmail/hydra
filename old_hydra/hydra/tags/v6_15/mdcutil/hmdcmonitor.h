#ifndef HMDCMONITOR
#define HMDCMONITOR

#include "hreconstructor.h"
#include "hlocation.h"
#include "hparset.h"
#include <TMapFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TTree.h>
#include <stdlib.h>

#include <TSystem.h>
#include <TGFrame.h>


class HCategory;
class HIterator;
class HMdcDigitPar;
class HEvent;
class HMdcRawStruct;

class HMdcMonitor : public HReconstructor {
private:
  HLocation loc;                       // Location for new object
  HCategory *rawCat;                   // Pointer to MdcRaw data category
  HIterator *iter;                     //! Iterator over Cal2 category
  HMdcRawStruct *raws;                 // Pointer to MdcRawStruct
  TMapFile *mfile;                     // Pointer to TMapFile
  HEvent *recEvt;
  TH2S *pHistTDCDT[16];
  TH1S *pHistTDC[16];
  TH2S *pHistTD[16];
  TH1S *pHistMult, *pHistT1[16][12][8], *pHistT2[16][12][8];
  Int_t i,j,k,l,tdcChip,tdcChan;
  Int_t time1,time2,time3,time4,time5,time6;
  Int_t nHits, sector, module, mbo, tdc;
  Int_t secNum, modNum;
 public:
static Int_t evtNum;
 public:
  HMdcMonitor(void);
  HMdcMonitor(Text_t* name,Text_t* title);
  HMdcMonitor(Text_t* name,Text_t* title, Int_t secnum, Int_t modnum);
  ~HMdcMonitor(void);
  Bool_t init(void);
  Bool_t finalize(void);
  Int_t execute(void);
  void showWin(void);
  void setParContainers(void);
  ClassDef(HMdcMonitor,0) //Calibrater of Mdc data.
};

#endif /* !HMDCMONITOR_H */










