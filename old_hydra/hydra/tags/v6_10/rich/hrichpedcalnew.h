// File: hrichpedcalnew.h
//
// Author: Angelika Elhardt  <angelika.elhardt@physik.tu-muenchen.de>
// Last update by W.Schoen: 00/11/08 22:00:32
//
#ifndef HRICHPEDCALNEW_H
#define HRICHPEDCALNEW_H

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
class HRichPedCalNew : public HReconstructor {

public:
    ClassDef(HRichPedCalNew,0)
    HRichPedCalNew(Text_t *name,Text_t *title, char *, char*);
    ~HRichPedCalNew();
    Bool_t init(void);
    Bool_t finalize(void);
    Int_t execute(void);
    int getCol(int);
    int getRow(int);
    
    TFile *dataFile;
    double (* pDiff)[92][90];
    double (* pMean)[92][90];
    double (* pSigma)[92][90];
    int (* pStat)[92][90];
    int (*pRunAway)[92][90];
    TH2F *meanHisto[6];
    TH2F *sigmaHisto[6];
    TH1F *mean1dHisto[6];
    TH1F *sigma1dHisto[6];
    TH2F *statHisto[6];
    TH2F *runAwayHisto[6];
    TH1F *count;


private:
    HCategory * pRichCal;
    HIterator* pIter; 
    HParSet* fMappingPar;
    void createStatistics();
    void createMeans();
    void initializeStatistics();
    void initializeMeans();
    void deleteStatistics();
    void deleteMeans();
    void fillMask();
    void initMappingPar();
    void bookHistos();
    void bookCounterHisto();
    void fill2DHistos();
    void fill1DHistos();
    void fillCounterHisto();
    void bookCanvases();
    void calculate();
    void initializeMeanSigma();
    void meanAndSigma(int,int,int,float);
    void outputFile();
    void uipFile();
    Bool_t checkSectorList();
    char sectorList[7];
    Int_t numSec;
    Int_t rowMax, colMax, sectorMax, hardwareMax;
    int mask[6][2];
    int countArray[6][92][90];
    int nCounter;
    float size;
    float epsilon;
    int channels;
    char * pedPathName;
    TVector  *vec3,*vec5,*vec6;
    Bool_t sigmaTransformation, checkInput;
   
};

#endif /* !HRICHPEDCALNEW_H */















