// File: hlocalselection.h
//
// Author: Laura Fabbietti <L.Fabbietti@physik.tu-muenchen.de>
// Last update by Laura Fabbietti: 02/05/02 20:10:58
//
#ifndef HLOCALSELECTION_H
#define HLOCALSELECTION_H
#include "hreconstructor.h"
#include "hlocation.h"
#include "hparset.h"
#include "TFile.h"
#include "TH2.h"
#include "TNtuple.h"

class HCategory;
class HIterator;
class HRichCal;
class HRichLocal;

class HLocalSelection : public HReconstructor {
public:
    HLocalSelection(Text_t *name, Text_t *title,Float_t lowTheta[6],Float_t highTheta[6],Float_t *minEvt,Float_t *maxEvt,Int_t intSize);
    ~HLocalSelection(void);
    Int_t execute();
    Bool_t  init();
    void checkPhoton(Float_t theta,Int_t sec);
    Bool_t finalize(){return kTRUE;};
    HCategory* getLocalCat() {return fLocalCat;}
    ClassDef(HLocalSelection,1)

protected:
    Int_t countSec[6];
    Float_t thetaLow[6];
    Float_t thetaHigh[6];
    Float_t *minEvent; //!
    Float_t *maxEvent; //!
    Int_t evtIntSize;
    Int_t evtCount;
    HIterator* fIter1; //!
    HCategory *fLocalCat;//!
};
#endif /* !HLOCALSELECTION_H */

