// File: hrichpedestalupdate.h
//
// Author: Tassilo Christ <tassilo.christ@physik.tu-muenchen.de>
// Last update by Tassilo Christ: 15.05.02


#ifndef HRICHPEDESTALUPDATE_H
#define HRICHPEDESTALUPDATE_H

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
class HRichPedestalUpdate : public HReconstructor {

public:
    ClassDef(HRichPedestalUpdate,0)
    HRichPedestalUpdate(Text_t *name,Text_t *title, Char_t* path ,Char_t * pString);
    ~HRichPedestalUpdate();
    Bool_t init(void);
    Bool_t finalize(void);
    Int_t execute(void);
    Int_t getCol(int);
    Int_t getRow(int);
    HParSet* getCalPar(){return fCalPar;}
    void setCalPar(HParSet* pCalPar){ if (fCalPar) delete fCalPar;
    fCalPar= pCalPar;}
    void createPedestal();
    void createCalibration();
    void calculateShifts();
    TFile *dataFile;
    Int_t (* pStat)[96][96];
    Float_t (* pFrac)[96][96];
    Float_t (* pShift)[96][96];
    


    //diagnostic plots
    TH2F *driftHisto[6];
    TH2F *meanHisto[6];
    TH2F *sigmaHisto[6];
    TH1F *mean1dHisto[6];
    TH1F *sigma1dHisto[6];
    TH2F *statHisto[6];
    TH2F *runAwayHisto[6];

private:
    HCategory * pRichRaw;
    HIterator* pIter; 
    HParSet* fThresholdPar;
    HParSet* fCalPar; //Pointer to the cal data category
    HParSet* fMappingPar;
    void createSums();
    void initializeErfTable();
    void initializeSums();
    void deleteSums();
    void fillMask();
    Bool_t fillCalParCntr();
    void printCalParCntr();
    void initCalPar();
    void initThresholdPar();
    void initMappingPar();
    void bookHistos();
    void fill2DHistos();
    void fill1DHistos();
    void bookCanvases();
    void calculate();
    void normalizeStatistics();
    void outputFile();
    void uipFile();
    Float_t getNewCoordinate(Float_t countedFraction);
    Bool_t checkSectorList();
    Char_t sectorList[7];
    Int_t numSec;
    Int_t rowMax, colMax, sectorMax, hardwareMax;
    Int_t mask[6][2];
    Int_t nCounter;
    Float_t coordinate[901];
    Float_t gaussInt[901];
    Char_t * pedPathName;
    TVector  *vec3,*vec5,*vec6;
    Bool_t normalizedSums, checkInput;

};

#endif /* !HRICHPEDESTALUPDATE_H */





