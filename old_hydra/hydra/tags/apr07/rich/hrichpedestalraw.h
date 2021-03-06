// File: hrichpedestalraw.h
//
// Author: Angelika Elhardt  <angelika.elhardt@physik.tu-muenchen.de>
// Last update by Angelika Elhardt: 00/11/08 22:59:25
// last modified by W.Schoen: 2000/11/08


#ifndef HRICHPEDESTALRAW_H
#define HRICHPEDESTALRAW_H

#include "hreconstructor.h"
#include "hades.h"
#include "hevent.h"
#include "haddef.h"
#include "hparset.h"
#include "hrichcalpar.h"
#include <TVector.h>
#include <TArrayS.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TFile.h>
#include <TNtuple.h>

class HCategory;
class HIterator;
class HRichPedestalRaw : public HReconstructor {

public:
  ClassDef(HRichPedestalRaw,0)
    HRichPedestalRaw(Text_t *name,Text_t *title, char *, char*);
  ~HRichPedestalRaw();
  Bool_t init(void);
  Bool_t finalize(void);
  Int_t execute(void);
  int getCol(int);
  int getRow(int);
  HParSet* getCalPar(){return fCalPar;}
  void setCalPar(HParSet* pCalPar){ if (fCalPar) delete fCalPar;
  fCalPar= pCalPar;}
  void createPedestal();
  void createCalibration();
  
  TFile *dataFile;
  double (* pSum)[96][96];
  double (* pSum2)[96][96];
  int (* pStat)[96][96];
  TH2F *meanHisto[6];
  TH2F *sigmaHisto[6];
  TH1F *mean1dHisto[6];
  TH1F *sigma1dHisto[6];
  TH2F *statHisto[6];
  
  
 private:
  HCategory * pRichRaw;
  HIterator* pIter; 
  HParSet* fCalPar; //Pointer to the cal data category
  HParSet* fMappingPar;
  void createSums();
  void initializeSums();
  void deleteSums();
  void fillMask();
  Bool_t fillCalParCntr();
  void printCalParCntr();
  void initCalPar();
  void initMappingPar();
  void bookHistos();
  void fill2DHistos();
  void fill1DHistos();
  void bookCanvases();
  void calculate();
  void outputFile();
  void uipFile();
  Bool_t checkSectorList();
  char sectorList[7];
  Int_t numSec;
  Int_t rowMax, colMax, sectorMax, hardwareMax;
  int mask[6][2];
  int nCounter;
  char * pedPathName;
  TVector  *vec3,*vec5,*vec6;
  Bool_t normalizedSums, checkInput;
  
};

#endif /* !HRICHPEDESTALRAW_H */





