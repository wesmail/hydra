// File: hrichlocalmaxcal.h
//
//
//
#ifndef HRICHLOCALMAXCAL_H
#define HRICHLOCALMAXCAL_H

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


class HRichLocalMaxCal : public HReconstructor {
public:
    HRichLocalMaxCal(const Text_t *name,const  Text_t *title, Int_t pads,
		     Int_t hitMin, Int_t sector,
		     Float_t minTheta1, Float_t maxTheta1, Float_t minTheta2,
		     Float_t maxTheta2, const Char_t energyFileName[128],
		     Int_t secEdge1,Int_t secEdge2,Int_t minHitsecEdge1,
		     Int_t minHitsecEdge2);
    ~HRichLocalMaxCal();
    HRichLocalMaxCal(void){;};
    Bool_t init();
    Bool_t finalize();
    Bool_t resetmatrix();
    Bool_t hitcontrol();
    Int_t execute();
    Bool_t fillmatrix(HLocation &fLoc);
    Bool_t localmaxcal(HLocation &fLoc);
    Bool_t localanglecut(HLocation& fLoc);
    Int_t checkLocalMax(Int_t sec,Int_t i,Int_t j,Int_t a);

    Bool_t checkIsolation(Int_t nRow, Int_t nCol, Int_t nSector,Int_t padNum);
    Float_t sumcharge(Int_t nRow, Int_t nCol, Int_t nSector);
    virtual Bool_t energyfile();
    virtual Bool_t energyout();
    virtual Bool_t multihit(Int_t nRow, Int_t nCol, Int_t nSector, HLocation& fLoc);
    virtual Float_t energyset(Int_t nRow, Int_t nCol, Int_t nSector);
    
    HRichLocalMaxCal &operator=(HRichLocalMaxCal &c);
    
  HCategory* getLocalCat() {return fLocalCat;}
   
    HCategory* getCalCat() {return fCalCat;}
    HParSet* getGeometryPar() {return fGeometryPar;}
    
  
      void setLocalCat(HCategory* pLocalCat) {fLocalCat = pLocalCat;}
    void setCalCat(HCategory* pCalCat) {fCalCat = pCalCat;}
    void setGeometryPar(HParSet* pPar) {fGeometryPar = pPar;}
    void updateHeaders(Float_t t1, Float_t t2,Int_t i,Float_t *secM,Float_t *meanCS,Int_t evtNum);

    ClassDef(HRichLocalMaxCal,1) //ROOT extension

protected:
    Int_t shadowSector;
    Int_t edgesector1;
    Int_t edgesector2;
    Int_t minHitedgesector1;
    Int_t minHitedgesector2;
    Int_t padNumber;
    Int_t minHit;
    Int_t localEventNr;
    Int_t eventNr;
    Int_t i, j;
    Int_t n;
    Bool_t hitControl;
    Float_t sumHit[6];
    Float_t meanCharge[6];
    Int_t doubleHit[6];

    Int_t jCol;
    Int_t jRow;
    Int_t iSector;
    Int_t iCol;
    Int_t iRow;
    Float_t thetaMin1;//MgF2
    Float_t thetaMax1;//MgF2
    Float_t thetaMin2;//SiO2
    Float_t thetaMax2;//SiO2
    Float_t minCharge;
    Float_t maxCharge;
    Float_t sumCharge;
    Float_t sumTheta;
    Float_t sumPhi;
    Int_t nPad;
    Int_t mHit2[6][90][92];
    Int_t mHit[6][90][92];
    Float_t mCharge[6][90][92];
    Float_t mTheta[6][90][92];
    Float_t mPhi[6][90][92];
    Float_t energy;
    Float_t fCharge[9];
    Char_t energyName[16];
    Char_t filename[128];
    Int_t count1, count2;
    Float_t fTheta1, fTheta2;

    TFile * pEnergyLockup;
    TH2F * pEnergySim[6];
    TNtuple *tCharge; //!

    HParSet* fGeometryPar; //! Pointer to geometry parameters
    HCategory *fCalCat; //! Pointer to the cal data category
    HCategory *fLocalCat;//!
     HCategory *fLocalCatHr; //!
    HIterator* fIter; //!
    HIterator* fIter1; //!
};

#endif // HRICHLOCALMAXCAL_H






