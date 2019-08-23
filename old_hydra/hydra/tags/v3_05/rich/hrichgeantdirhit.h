#ifndef __HRichGeantDirHit_H
#define __HRichGeantDirHit_H

#include "hdataobject.h"
#include <iostream.h>

class HRichGeantDirHit : public HDataObject {
public:
        HRichGeantDirHit(void);
        ~HRichGeantDirHit(void);

        void Clear(void);

        void getXYZ(Float_t* pfX, Float_t* pfY, Float_t* pfZ);
        void getAngles(Float_t* pfTheta, Float_t* pfPhi);
        Float_t getEnerLos();
        Float_t getTLen();
        Int_t IsMirror();

        void setXYZ(Float_t fX, Float_t fY, Float_t fZ);
        void setAngles(Float_t fTheta, Float_t fPhi);
        void setEnerLos(Float_t fEner);
        void setTLen(Float_t fTLen);
        void setMirror(Int_t nMir);

        friend ostream& operator<< (ostream& output, HRichGeantDirHit& element);
        ClassDef(HRichGeantDirHit,1) //Rich GeantDirHit data

public:
   Float_t         fXHit;
   Float_t         fYHit;
   Float_t         fZHit;
   Int_t           nPartHit;
   Float_t         fTrkLenHit;
   Float_t         fMomHit;
   Float_t         fThetaHit;
   Float_t         fPhiHit;
   Float_t         fELosHit;
   Float_t         fTLenHit;
   Int_t           nMirHit;
   Int_t           nTrkHit;
   Int_t           nParentHit;
   Int_t           nProcesHit;
};

#endif

