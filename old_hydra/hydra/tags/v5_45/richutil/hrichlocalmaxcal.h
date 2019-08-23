// File: hrichlocalmaxcal.h
//
// Author: Soenke Schroeder <soenke.schroeder@physik.tu-muenchen.de>
// Last update by Soenke Schroeder: 01/07/04 12:56:23
// based on hydra532 and root 2.25-03
//
#ifndef HRICHLOCALMAXCAL_H
#define HRICHLOCALMAXCAL_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hparset.h"
#include <TFile.h>
#include <TH2.h>

class HCategory;
class HIterator;
class HRichCal;
class HRichLocal;
class HRichGlobal;

class HRichLocalMaxCal : public HReconstructor {
public:
    HRichLocalMaxCal(Text_t *name, Text_t *title, Int_t pads, Float_t charMin,
		     Float_t charMax, Int_t hitMin, Int_t hitMax, Int_t sector,
		     Float_t minTheta1, Float_t maxTheta1, Float_t minTheta2,
		     Float_t maxTheta2, Char_t energyFileName[128]);
    ~HRichLocalMaxCal();
    
    Bool_t init();
    Bool_t finalize();
    Bool_t resetmatrix();
    Bool_t hitcontrol();
    Bool_t globalreset();
    Int_t execute();
    Bool_t fillmatrix(HLocation &fLoc);
    Bool_t localmaxcal(HLocation &fLoc);
    Bool_t localangle(HLocation& fLoc);
    Bool_t localanglecut(HLocation& fLoc);
    Int_t checkLocalMax(Int_t sec,Int_t i,Int_t j,Int_t a);
    virtual Bool_t energyfile();
    virtual Bool_t energyout();
    virtual Bool_t multihit(Int_t nRow, Int_t nCol, Int_t nSector, HLocation& fLoc);
    virtual Float_t energyset(Int_t nRow, Int_t nCol, Int_t nSector, HLocation& fLoc);
    
    HRichLocalMaxCal &operator=(HRichLocalMaxCal &c);
    
    HCategory* getLocalCat() {return fLocalCat;}
    HCategory* getGlobalCat() {return fGlobalCat;}
    HCategory* getCalCat() {return fCalCat;}
    HParSet* getGeometryPar() {return fGeometryPar;}
    
    void setLocalCat(HCategory* pLocalCat) {fLocalCat = pLocalCat;}
    void setGlobalCat(HCategory* pGlobalCat) {fGlobalCat = pGlobalCat;}
    void setCalCat(HCategory* pCalCat) {fCalCat = pCalCat;}
    void setGeometryPar(HParSet* pPar) {fGeometryPar = pPar;}
    
    ClassDef(HRichLocalMaxCal,1) //ROOT extension

protected:
    Int_t shadowSector;
    Int_t padNumber;
    Int_t minHit;
    Int_t maxHit;
    Int_t localEventNr;
    Int_t ringEventNr;
    Int_t eventNr;
    Int_t i, j;
    Int_t n;
    Bool_t hitControl;
    Int_t sumHit[6];
    Int_t doubleHit[6];
    Int_t jCol;
    Int_t jRow;
    Int_t iSector;
    Int_t iCol;
    Int_t iRow;
    Int_t i1;//MgF2
    Int_t i2;//SiO2
    Int_t n1;//MgF2
    Int_t n2;//SiO2
    Int_t thetaCount1;//MgF2
    Int_t thetaCount2;//SiO2
    Int_t phiCount1[6];//MgF2
    Int_t phiCount2[6];//SiO2
    Float_t thetaMin1;//MgF2
    Float_t thetaMax1;//MgF2
    Float_t thetaMin2;//SiO2
    Float_t thetaMax2;//SiO2
    Float_t globalTheta1;//MgF2
    Float_t globalTheta2;//SiO2
    Float_t globalPhi1[6];//MgF2
    Float_t globalPhi2[6];//SiO2
    Float_t minCharge;
    Float_t maxCharge;
    Float_t sumCharge;
    Float_t sumTheta;
    Float_t sumPhi;
    Float_t mHit2[6][90][92];
    Float_t mHit[6][90][92];
    Float_t mCharge[6][90][92];
    Float_t mTheta[6][90][92];
    Float_t mPhi[6][90][92];
    Float_t mLocalTheta[6][90][92];
    Float_t mLocalPhi[6][90][92];
    Float_t sumTheta1;//MgF2
    Float_t sumTheta2;//SiO2
    Float_t sumPhi1[6];//MgF2
    Float_t sumPhi2[6];//SiO2
    Float_t theta2;//SiO2
    Float_t theta1;//MgF2
    Float_t phi2;//SiO2
    Float_t phi1;//MgF2
    Float_t energy;
    Char_t energyName[16];
    Char_t filename[128];
    Bool_t localMaxControl;
    
    TFile * pEnergyLockup;
    TH2F * pEnergySim[6];
    
    HParSet* fGeometryPar; //! Pointer to geometry parameters
    HCategory *fCalCat; //! Pointer to the cal data category
    HCategory *fLocalCat;//!
    HCategory *fGlobalCat;//!
    
    HIterator* fIter; //!
    HIterator* fIter1; //!
};

#endif // HRICHLOCALMAXCAL_H
