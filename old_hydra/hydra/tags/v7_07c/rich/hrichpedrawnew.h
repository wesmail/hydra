// File: hrichpedrawnew.h
//
// Author: Angelika Elhardt  <angelika.elhardt@physik.tu-muenchen.de>
// Last update by W.Schoen: 00/11/08 22:00:32
//

#ifndef HRICHPEDRAWNEW_H
#define HRICHPEDRAWNEW_H

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
class HRichPedRawNew : public HReconstructor {

public:
    ClassDef(HRichPedRawNew,0)
    HRichPedRawNew(Text_t *name,Text_t *title, char *, char*);
    ~HRichPedRawNew();
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


private:
    HCategory * pRichRaw;
    HIterator* pIter; 
    HParSet* fCalPar; //Pointer to the cal data category
    HParSet* fMappingPar;
    void createStatistics();
    void createMeans();
    void initializeStatistics();
    void initializeMeans();
    void deleteStatistics();
    void deleteMeans();
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

#endif /* !HRICHPEDRAWNEW_H */












