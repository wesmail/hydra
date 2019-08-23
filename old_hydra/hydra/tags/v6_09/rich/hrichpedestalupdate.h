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
    HRichPedestalUpdate(Text_t *name,Text_t *title, char *, char*, float alpha_orig);
    ~HRichPedestalUpdate();
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
    int (* pStat)[96][96];
    float (* pFrac)[96][96];
    TH2F *statHisto[6];
    TH2F *driftHisto[6];

private:
    HCategory * pRichRaw;
    HIterator* pIter; 
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
    void initMappingPar();
    void bookHistos();
    void fill2DHistos();
    void fill1DHistos();
    void bookCanvases();
    void calculate();
    void normalizeStatistics();
    void outputFile();
    void uipFile();
    float getalpha(float fraction);
    Bool_t checkSectorList();
    char sectorList[7];
    Int_t numSec;
    Int_t rowMax, colMax, sectorMax, hardwareMax;
    int mask[6][2];
    int nCounter;
    float alphaorig;
    float alpha[601];
    float phi[601];
    char * pedPathName;
    TVector  *vec3,*vec5,*vec6;
    Bool_t normalizedSums, checkInput;

};

#endif /* !HRICHPEDESTALUPDATE_H */




