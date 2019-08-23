#ifndef __HRichGeantCherHit_H
#define __HRichGeantCherHit_H

#include "hdataobject.h"
#include <iostream.h>

class HRichGeantCherHit : public HDataObject {
public:
        HRichGeantCherHit(void);
        ~HRichGeantCherHit(void);

        void Clear(void);

        void getXY(Float_t* pfX, Float_t* pfY);
        Float_t getEner();
        Float_t getTrkLen();
        Int_t getStatFlag();

        void setXY(Float_t fX, Float_t fY);
        void setEner(Float_t fEner);
        void setTrkLen(Float_t fTrkLen);
        void setStatFlag(Int_t nFlag);

        friend ostream& operator<< (ostream& output, HRichGeantCherHit& element);
        ClassDef(HRichGeantCherHit,1) //Rich GeantCherHit data

public:
   Float_t         fXCer;
   Float_t         fYCer;
   Float_t         fECer;
   Float_t         fTrkLength;
   Int_t           nMediumNum;
   Int_t           nStatFlag;
   Int_t           nPartrkCer;
   Int_t           nParentCer;
   Int_t           nProcesCer;

};

#endif

