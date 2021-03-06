// $Id: hrichrecalibrater.h,v 1.5 2009-07-15 11:39:22 halo Exp $
// Last update by Thomas Eberl: 02/09/25 18:14:52
//
#ifndef HRICHRECALIBRATER_H
#define HRICHRECALIBRATER_H

#include "hreconstructor.h"
#include "hades.h"
#include "hevent.h"
#include "haddef.h"
#include "hparset.h"
#include "hrichcalpar.h"
#include "TVector.h"
#include "TArrayS.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TFile.h"
#include "TNtuple.h"
#include "TString.h"

class HCategory;
class HIterator;
class HRichReCalibrater : public HReconstructor {

public:
    ClassDef(HRichReCalibrater,0)
    HRichReCalibrater(const Text_t *name,const Text_t *title, const Char_t *, const Char_t*, Float_t);
    ~HRichReCalibrater();
    Bool_t init(void);
    Bool_t finalize(void);
    Int_t execute(void);
    Int_t getCol(int);
    Int_t getRow(int);
    
    TFile *dataFile;
    Double_t (* pDiff)[92][90];
    Double_t (* pMean)[92][90];
    Double_t (* pSigma)[92][90];
    Int_t (* pStat)[92][90];
    Int_t (*pRunAway)[92][90];
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
    void createStatistics();
    void createMeans();
    void initializeStatistics();
    void initializeMeans();
    void deleteStatistics();
    void deleteMeans();
    void fillMask();
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
    Char_t sectorList[7];
    Int_t numSec;
    Int_t rowMax, colMax, sectorMax, hardwareMax;
    Int_t mask[6][2];
    Int_t countArray[6][92][90];
    Int_t nCounter;
    Float_t fNoiseLim;
    Float_t size;
    Float_t epsilon;
    Int_t channels;
    TString pedPathName;
    TVector  *vec3,*vec5,*vec6;
    Bool_t sigmaTransformation, checkInput;
   
};

#endif /* !HRICHRECALIBRATER_H */















