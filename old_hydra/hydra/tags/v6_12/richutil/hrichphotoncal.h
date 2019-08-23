// File: hrichphotoncal.h
//
// Author: Soenke Schroeder <soenke.schroeder@physik.tu-muenchen.de>
// Last update by Soenke Schroeder: 01/07/02 17:39:22
// based on hydra532 and root 2.25-03
//
#ifndef HRICHPHOTONCAL_H
#define HRICHPHOTONCAL_H
using namespace std;
#include "hreconstructor.h"
#include "hlocation.h"
#include "hparset.h"
#include <fstream> 
#include <TNtuple.h>

class HCategory;
class HIterator;
class HRichCal;
class HRichLocal;

class HRichPhotonCal : public HReconstructor {
public:
    HRichPhotonCal(Text_t *name, Text_t *title, Float_t charMin, 
		   Float_t charMax, Int_t hitMin, Int_t hitMax,
		   Float_t thetaMin, Float_t thetaMax);
    ~HRichPhotonCal();
    
    Bool_t init();
    Bool_t finalize();
    Int_t execute();
    Bool_t fillmatrix(HLocation &fLoc);
    Bool_t photon(HLocation &fLoc);
    Bool_t padmatrix();
    Bool_t resetmatrix();
    inline Float_t sumcharge(Int_t nRow, Int_t nCol, Int_t nSector);
    inline Bool_t centralpad(Int_t nRow, Int_t nCol, Int_t nSector);
    inline Bool_t checkIsolation(Int_t nRow, Int_t nCol, Int_t nSector);

    HRichPhotonCal &operator=(HRichPhotonCal &c);
    
    HCategory* getCalCat() {return fCalCat;}
    HCategory* getLocalCat() {return fLocalCat;}
    HParSet* getGeometryPar() { return fGeometryPar;}
    
    void setCalCat(HCategory* pCalCat) {fCalCat = pCalCat;}
    void setLocalCat(HCategory* pLocalCat) {fLocalCat = pLocalCat;}
    void setGeometryPar(HParSet* pPar) { fGeometryPar = pPar;}
    
    ClassDef(HRichPhotonCal, 1) //ROOT extension
	
protected:
    Int_t minHit;
    Int_t maxHit;
    Int_t localEventNr;
    Int_t eventNr;
    Float_t minTheta;
    Float_t maxTheta;
    Float_t minCharge;
    Float_t maxCharge;
    Float_t sumHit[6];
    Float_t maxPhi;
    Float_t mPhi[6][90][92];
    Float_t mTheta[6][90][92];
    Float_t mCharge[6][90][92];
    Int_t mHit[6][90][92];
    Int_t nPad;
    Int_t iSector;
    Int_t iCol;
    Int_t iRow;
    Int_t jCol;
    Int_t jRow;
    Int_t i, j;
    Int_t mPad1[90][92];
    Int_t mPad2[90][92];
    Bool_t localControl;
    Float_t sumCharge;
    Float_t sumPhi;
    Float_t sumTheta;
    Int_t iSym;
    Float_t fCharge[9];
    
    TNtuple *tCharge; //!
    HParSet* fGeometryPar; //!
    HCategory *fCalCat; //!
    HCategory *fLocalCat; //!
    HIterator *fIter; //!
};

#endif // HRICHPHOTONCAL_H
