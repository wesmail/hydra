// File: hrichpedrawnew.h
//
// Author: Angelika Elhardt  <angelika.elhardt@physik.tu-muenchen.de>
// Last update by W.Schoen: 00/11/08 22:00:32
// Modified by martin.jurkovic@ph.tum.de 2005/09/11

#ifndef HRICHPEDRAWNEW_H
#define HRICHPEDRAWNEW_H

#include "TArrayS.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TH2.h"
#include "TNtuple.h"
#include "TStyle.h"
#include "TVector.h"

#include "haddef.h"
#include "hades.h"
#include "hevent.h"
#include "hparset.h"
#include "hreconstructor.h"
#include "hrichcalpar.h"

class HCategory;
class HIterator;
class HRichPedRawNew : public HReconstructor {

 public:
  HRichPedRawNew(Text_t *name,Text_t *title, Char_t *, Char_t*);
  ~HRichPedRawNew();

  void createCalibration();
  void createPedestal();
  void setCalPar(HParSet* pCalPar) {if(fCalPar) delete fCalPar; fCalPar = pCalPar;}

  Bool_t finalize(void);
  Bool_t init(void);

  Int_t execute(void);
  Int_t getCol(int);
  Int_t getRow(int);

  HParSet *getCalPar() {return fCalPar;}

  TFile *dataFile;

  TH2F *meanHisto[6];
  TH1F *mean1dHisto[6];
  TH2F *sigmaHisto[6];
  TH1F *sigma1dHisto[6];
  TH2F *statHisto[6];
  TH2F *runAwayHisto[6];


 private:
  void bookCanvases();
  void bookHistos();
  void calculate();
  void createMeans();
  void createStatistics();
  void deleteMeans();
  void deleteStatistics();
  void initializeMeans();
  void initializeMeanSigma();
  void initializeStatistics();
  void initCalPar();
  void initMappingPar();
  void fillMask();
  void fill2DHistos();
  void fill1DHistos();
  void meanAndSigma(int,int,int,float);
  void outputFile();
  void printCalParCntr();
  void uipFile();

  Bool_t fillCalParCntr();
  Bool_t checkSectorList();

  Double_t (*pDiff)      [92][90];
  Double_t (*pMean)      [92][90];
  Double_t (*pSigma)     [92][90];

  Int_t    (*countArray) [92][90];
  Int_t    (*pRunAway)   [92][90];
  Int_t    (*pStat)      [92][90];

  Bool_t checkInput;
  Bool_t sigmaTransformation;

  Char_t *pedPathName;
  Char_t sectorList[7];

  Int_t channels;
  Int_t colMax;
  Int_t hardwareMax;
  Int_t mask[6][2];
  Int_t nCounter;
  Int_t numSec;
  Int_t rowMax;
  Int_t sectorMax;

  Float_t epsilon;
  Float_t size;

  HCategory *pRichRaw;

  HIterator *pIter; 

  HParSet *fCalPar; //Pointer to the cal data category
  HParSet *fMappingPar;

  TVector *vec3;
  TVector *vec5;
  TVector *vec6;

  ClassDef(HRichPedRawNew,0)
};

#endif /* !HRICHPEDRAWNEW_H */
