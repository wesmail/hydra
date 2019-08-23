// File: hlocalshift.h
//
// Author: Laura Fabbietti <L.Fabbietti@physik.tu-muenchen.de>
// Last update by Laura Fabbietti: 02/04/08 20:37:56
//
#ifndef HLOCALSHIFT_H
#define HLOCALSHIFT_H
#include "hreconstructor.h"
#include "hlocation.h"
#include "hparset.h"
#include "hrichpad.h"
#include "TFile.h"
#include "TH2.h"
#include "TNtuple.h"

class HCategory;
class HIterator;
class HRichCal;
class HRichLocal;

class HLocalShift : public  HReconstructor {
public:
    HLocalShift(const Text_t *name,const Text_t *title,Float_t shiftThetaMgF2[6],Float_t shifThetaSiO2[6],const Char_t energyFileName[128]);
    ~HLocalShift(void);
    Bool_t init();
    Int_t execute();
    Bool_t finalize(){return kTRUE;};
    void setGeometryPar(HParSet* pPar) {fGeometryPar = pPar;}
    HParSet* getGeometryPar() {return fGeometryPar;}
    void recalculateLocal(HRichPad * p,HRichLocal* pL);
    //    Float_t energyset(Int_t nRow, Int_t nCol, Int_t nSector);
    Bool_t energyfile();
    void shiftTheta(HRichLocal* pL);
  Float_t energyset(Int_t nRow, Int_t nCol, Int_t nSector);

    HCategory* getLocalCat() {return fLocalCat;}
    ClassDef(HLocalShift,1)
private :
    Float_t sThetaMgF2[6];
    Float_t sThetaSiO2[6];
    HIterator* fIter1; //!
    HCategory *fLocalCat;//!
    HParSet* fGeometryPar; //! Pointer to geometry parameters
    TH2F * pEnergySim[6];//!
    TFile * pEnergyLockup;//!
    Char_t filename[128];
    Char_t energyName[16];
};
#endif /* !HLOCALSHIFT_H */
