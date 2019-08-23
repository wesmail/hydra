// $Id: hrichchernovringfitter.h,v 1.6 2009-07-15 11:39:21 halo Exp $
// Last update by Thomas Eberl: 03/04/02 16:56:23
//
#ifndef HRICHCHERNOVRINGFITTER_H
#define HRICHCHERNOVRINGFITTER_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hparset.h"
#include "TArrayD.h"

class HCategory;
class HIterator;
class HRichHit;
class HRichHitFit;
class HRichChernovRingFitter : public HReconstructor {
public:

	HRichChernovRingFitter(){};
        HRichChernovRingFitter(const Text_t *name,const Text_t *title,Bool_t);
	~HRichChernovRingFitter();

        Bool_t init();
        Bool_t finalize();
        Int_t execute();
        
        HCategory* getCalCat(){return pCalCat;};
        HCategory* getHitCat(){return pHitCat;};
        HCategory* getHitFitCat(){return pHitFitCat;};
        HCategory* getPhotonHitCat(){return pPhotonHitCat;};
        HCategory* getPhotonHitHeaderCat(){return pPhotonHitHeaderCat;};
        void setFitRadius(Double_t rad){ dFittedRadius=rad;};
        void setFitCenterX(Double_t centx){ dFittedCenterX=centx;};
        void setFitCenterY(Double_t centy){ dFittedCenterY=centy;};
        void setFitVar(Double_t var){ dFitVariance=var;};
        void setFitCenterTheta(Double_t centtheta){ dFittedCenterTheta=centtheta;};
        void setFitCenterPhi(Double_t centphi){ dFittedCenterPhi=centphi;};
        Double_t getFitRadius(){return dFittedRadius;};
        Double_t getFitCenterX(){return dFittedCenterX;};
        Double_t getFitCenterY(){return dFittedCenterY;};
        Double_t getFitVar(){return dFitVariance;};
        Double_t getFitCenterTheta(){return dFittedCenterTheta;};
        Double_t getFitCenterPhi(){return dFittedCenterPhi;};
        HParSet* getGeometryPar(){return pGeomPar;};
        void     setGeometryPar(HParSet* pPar){ pGeomPar = pPar;};
    //Bool_t fit();
        Bool_t fit(TArrayD&,TArrayD&);
        Bool_t fitFoundPhotons(){return kTRUE;};
        Bool_t fitFiredPads();
        Bool_t fitFiredPads2();
        void clearFitParams();
        void unvalidFitParams();
        void switchXtoPhi();
        Double_t* compress(Double_t*,Int_t);
        void dumpFitParameters(Int_t,Int_t,Int_t,Float_t);
        void calcThetaAndPhi(HRichHit*);
        void storeFitinHRichHitFit(Int_t,Int_t,Int_t);
        ClassDef(HRichChernovRingFitter,2) //ROOT extension

private:
        HIterator* pIterCal; //!
        HIterator* pIterHit; //!
        HIterator* pIterPhotonHit; //!
        HIterator* pIterPhotonHitHeader; //!
        HCategory* pCalCat; //!
        HCategory* pHitCat; //!
        HCategory* pHitFitCat; //!
        HCategory* pPhotonHitCat; //!
        HCategory* pPhotonHitHeaderCat; //!
        HParSet* pGeomPar; //Pointer to geometry parameter container
        Bool_t kPhotonFinder; // kTRUE=fit found photon hit coords
                              // kFALSE=fit pad coords in pad units
    // Fit parameters
        Double_t dFittedRadius;
        Double_t dFittedCenterX;
        Double_t dFittedCenterY;
        Double_t dFitVariance;
        Double_t dFittedCenterTheta;
        Double_t dFittedCenterPhi;
        Int_t mRC; // error flag
    // FIXME: input parameters to algo should come from RTDB
        Int_t fs; // size of square around hit center from recognition
        Int_t arraysize;
    // HELPERS
        Int_t evtcounter;
        
};

#endif // HRICHCHERNOVRINGFITTER_H
