//*-- Author : Dan Magestro (magestro@gsi.de)
//*-- Modified : 24/01/2002
#ifndef HQAMAKER_H
#define HQAMAKER_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "TRandom.h"

class HQAHistograms;
class HQAVariations;
class HQAScalers;
class HEvent;
//class HCategory;      //asd
class HIterator;
//class HMdcLookupGeom; //asd
//class HMdcLookupRaw;  //asd
class HMdcTimeCut;      //asd
class HQAOutputPS;
class HMdcCutStat;      //asd 


class HQAMaker : public HReconstructor {

public:
  HQAMaker(void);
  HQAMaker(Text_t* name,Text_t* title);
  ~HQAMaker(void);

  void setSamplingRate(Int_t rate) {samplingRate = rate;}
  void setIntervalSize(Int_t size) {intervalSize = size;}
  void setPSFileName(char *fname) {psFile = fname;}
  void setOutputDir(char *fdir) {fDir = fdir;}

  void generatePDF(Bool_t kpdf = kTRUE) {kPDF = kpdf;}

  Bool_t getIteratorPointers();
  void initIteratorPointers();
  void setParContainers(); //asd

  const void Print(Option_t *option="");
  void writeToFile(Bool_t kwtf = kTRUE) {kWriteToFile = kwtf;}

  Bool_t init(void);
  Bool_t finalize(void);
  Int_t  execute(void);

private:
  void buildPSFilename();

  void fillHistStart();
  void fillHistRich();
  void fillHistMdc();
  void fillHistTof();
  void fillHistTofino();
  void fillHistShower();
  void fillHistKick();
  void fillHistTrig();

  void finalizeHistStart() {;}
  void finalizeHistRich() {;}
  void finalizeHistMdc();
  void finalizeHistTof();
  void finalizeHistTofino() {;}
  void finalizeHistShower();
  void finalizeHistKick() {;}
  void finalizeHistTrig() {;}

  void fillScalers();

  HIterator *iterStCal;     //! HStartCal iterator
  HIterator *iterRichCal;   //! HRichCal iterator
  HIterator *iterRichHit;   //! HRichHit iterator
  HIterator *iterMdcRaw;    //! HMdcRaw iterator
  HIterator *iterMdcCal1;   //! HMdcCal1 iterator
  HIterator *iterMdcHit;    //! HMdcHit iterator
  HIterator *iterTofHit;    //! HTofHit iterator
  HIterator *iterTfnCal;    //! HTofinoCal iterator
  HIterator *iterShoHit;    //! HShowerHit iterator
  HIterator *iterShoHitTof; //! HShowerHit iterator
  HIterator *iterKickTrk;   //! HKickTrack iterator

  HIterator *iterHMatchURich;   //!HMatchURich iterator 
  HIterator *iterHMatchUTof;    //!HMatchUTof iterator
  HIterator *iterHMatchUShower; //!HMatchUShower iterator
  HIterator *iterHMULeptons;    //!HMULeptons iterator


  Bool_t kFIRST;
  Bool_t kPDF;
  Bool_t kWriteToFile;   //! boolean for writing hists to gHades output file
  Bool_t isSim;          //! boolean for sim/real
  Bool_t makePS();

  Int_t nProcessed;      //! number of events QA-analyzed
  Int_t nEvent;          //! number of events analyzed
  Int_t nInterval;       //! interval number (for variation hists)

  Int_t samplingRate;    //! rate of event sampling
  Int_t intervalSize;    //! number of events per interval (for variation hists)

  TString psFile;        //! PostScript output filename
  TString dstFile;       //! DST filename
  TString fDir;            //! QA output directory

  HQAVariations *varHists; //! Run variation histograms
  HQAHistograms *hists;    //! General QA histograms
  HQAScalers *scalers;     //! Run QA scalers

protected: //asd
  // pointer to used parameter containers
  //HMdcLookupGeom* lookup;   //! lookup table for mapping
  //HMdcLookupRaw*  lookupRaw;//! lookup table for mapping
  //HMdcTimeCut*    timecut;    //! container for time cuts //Not necessary
  HMdcCutStat*    cutStat;    //! container for statistics on cuts //asd

  static Int_t cutResults[4];         // contains 0/1 for cuts in t1,t2 and t2-1 after testTimeCuts()
public:

  ClassDef(HQAMaker,1) // Hydra task for filling QA histograms and scalers
};



#endif
