#ifndef __HShowerGeantHit_H
#define __HShowerGeantHit_H

#include "hdataobject.h"
#include <iostream.h>

class HShowerGeantHit : public HDataObject {
public:
        HShowerGeantHit(void);
        ~HShowerGeantHit(void);

        void Clear(void);

        void getXY(Float_t* pfX, Float_t* pfY);
        void getAngles(Float_t* pfTheta, Float_t* pfPhi);
        Float_t getEnerLos();
        Float_t getBeta();
        Float_t getMom();
        Int_t getDetector();
        Int_t getSector();
        Int_t getTrack();

        void setXY(Float_t fX, Float_t fY);
        void setAngles(Float_t fTheta, Float_t fPhi);
        void setEnerLos(Float_t fEner);
        void setBeta(Float_t fBeta);
        void setMom(Float_t fMom);
        void setDetector(Int_t nDet);
        void setSector(Int_t nSect);
        void setTrack(Int_t nTrack);

        friend ostream& operator<< (ostream& output, HShowerGeantHit& element);
        ClassDef(HShowerGeantHit,1) //ShowerGeantHit data

public:
   Float_t         fBetaHit;
   Float_t         fMomHit;
   Float_t         fELosHit;
   Int_t           nDetHit;
   Int_t           nSectHit;
   Float_t         fPhiHit;
   Float_t         fThetaHit;
   Float_t         fXHit;
   Float_t         fYHit;

   Int_t 	   nTrack;
};

#endif

