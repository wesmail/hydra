// File: hrichphotonfinder.h
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 01/08/20 13:17:31
//
#ifndef HRICHPHOTONFINDER_H
#define HRICHPHOTONFINDER_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hparset.h"

class HCategory;
class HIterator;
class HRichHit;
class HRichPhotonHit;

class HRichPhotonFinder : public HReconstructor 
{
public:

        HRichPhotonFinder(){};
        HRichPhotonFinder(Text_t *name,Text_t *title);
        ~HRichPhotonFinder();

        Bool_t init();
        Bool_t finalize();
        Int_t execute();
        Bool_t findLocMax(Int_t,Int_t*,Int_t*,Float_t*,
			  Float_t*,Int_t*,HRichHit*);
        Bool_t isLocalMaxOnBorder(Int_t,Int_t,Int_t,Int_t);
        Bool_t isLocalMaxOfFive(Int_t*,Int_t*,Float_t*,Int_t);
        HCategory* getCalCat(){return pCalCat;};
        HCategory* getHitCat(){return pHitCat;};
        HCategory* getPhotonHitCat(){return pPhotonHitCat;};
        HCategory* getPhotonHitHeaderCat(){return pPhotonHitHeaderCat;};
        HParSet*   getGeometryPar(){return pGeomPar;};
        void       initCalPar();
        void       printCalParCell(HLocation &);
        void       setGeometryPar(HParSet* pPar){ pGeomPar = pPar;};
        HParSet*   getCalPar(){return pCalPar;};
        void       setCalPar(HParSet* pPar){ pCalPar = pPar;};
        Bool_t     isPhoton(){return kTRUE;};
        void       setThetaPhi(HRichPhotonHit*);
    //        void subtractThreshold(void);
    
        Float_t* calcImpactByGravity(Int_t*,Int_t*,Float_t*,Float_t*,Int_t);
        void dumpPhotonParameters(void);
 
       ClassDef(HRichPhotonFinder,1) //ROOT extension

private:
        HIterator* pIterCal; //!
        HIterator* pIterHit; //!
        HCategory* pCalCat;//!
        HCategory* pHitCat; //!
        HCategory* pPhotonHitCat; //!
        HCategory* pPhotonHitHeaderCat; //!
        HParSet* pGeomPar;
        HParSet* pCalPar;
    // FIXME: input parameters to algo should come from RTDB
       Int_t fs; // size of square around hit center from recognition
    // HELPER
       Int_t evtcounter;
};

#endif // HRICHPHOTONFINDER_H
