// File: hrichlocalmaxcal.h
#ifndef HCLUSTERSELECTOR_H
#define HCLUSTERSELECTOR_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hparset.h"
#include "TFile.h"
#include "TH2.h"
#include "TNtuple.h"

class HCategory;
class HIterator;
class HRichCal;
class HPhotonCluster;
class HRichGeometryPar;

class HClusterSelector : public HReconstructor {
public:
    HClusterSelector(Text_t *name, Text_t *title,  Int_t hitMin, 
		     Int_t secShadow,
		     Float_t minTheta1, Float_t maxTheta1, Float_t minTheta2,
		     Float_t maxTheta2, 
		     Int_t secEdge1,Int_t secEdge2,Int_t minHitsecEdge1,
		     Int_t minHitsecEdge2);
    ~HClusterSelector();
    HClusterSelector(void){}
    Bool_t init();
    Bool_t reinit();
    Bool_t finalize();
    Bool_t resetmatrix();
    Bool_t hitcontrol();
    Bool_t isMaximumInCentre(HPhotonCluster *pPhotC);
    Int_t execute();
    Bool_t fillmatrix(HLocation &fLoc);
    void formCluster(Int_t secNum);
    void chainPads(Int_t iSec,Int_t iRow,Int_t iCol,HPhotonCluster* pClus);
    void calcClusProp(HPhotonCluster *pPhotClus);
    virtual Bool_t multihit(Int_t nRow, Int_t nCol, Int_t nSector, HLocation& fLoc);
    Bool_t thereIsOnlyOneMax(HPhotonCluster *pPhotC);
    
    
    HCategory* getPhotClusCat() {return fPhotClusCat;}
   
    HCategory* getCalCat() {return fCalCat;}
    HRichGeometryPar* getGeometryPar() {return pGeomPar;}
    
  
    
    void setCalCat(HCategory* pCalCat) {fCalCat = pCalCat;}
    void setGeometryPar(HRichGeometryPar* pPar) {pGeomPar = pPar;}
    void updateHeaders(Float_t t1, Float_t t2,Int_t i,Float_t *secM,Float_t *meanCS,Int_t evtNum);

   

protected:
    Int_t shadowSector,eventNr;
    Int_t edgesector1;
    Int_t edgesector2;
    Int_t minHitedgesector1;
    Int_t minHitedgesector2;
    Int_t minHit;
    Int_t padCounter;
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
    Float_t sumCharge;
    Float_t sumTheta;
    Float_t sumPhi;
    Int_t nPad;

    Int_t mHit2[6][90][92];//!
    Int_t mHit[6][90][92];//!
    Float_t mCharge[6][90][92];//!
    Float_t mTheta[6][90][92];//!
    Float_t mPhi[6][90][92];//!
    Int_t mPadLock[6][90][92];//!
    Float_t fCharge[9];

    Int_t maxCols, maxRows;
    Short_t * pLeftBorder; //! left border of active pad area
    Short_t * pRightBorder; //! dito right side (largest active col in row)
    Int_t count1, count2;
    Float_t fTheta1, fTheta2;

    Int_t padTotNr;
    Float_t chargeTot ;
    TNtuple *tCharge; 
    TNtuple *tCharge1;

    HRichGeometryPar *pGeomPar; //! Pointer to geometry parameters
    HCategory *fCalCat; //! Pointer to the cal data category
    HCategory * fPhotClusCat;//!
    HCategory *fLocalCatHr; //!
    HIterator* fIter; //!
    HIterator* fIter1; //!
    ClassDef(HClusterSelector,1) //ROOT extension
	};
#endif /* !HCLUSTERSELECTOR_H */








