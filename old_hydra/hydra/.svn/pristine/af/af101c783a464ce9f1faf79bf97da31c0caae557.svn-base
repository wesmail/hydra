#ifndef __HShowerHit_H
#define __HShowerHit_H
#pragma interface

#include "hdataobject.h"

class HShowerHit : public HDataObject {
public:
	HShowerHit(): m_fCharge(0.0){}
//	initialization could be used e.g. to clear the data element
	HShowerHit(const Float_t fCharge) : m_fCharge(fCharge) {}
	~HShowerHit() {}

        Int_t clear();

        Int_t getAddress(void) {return m_nAddress;}
        Char_t getSector(void) {return m_nSector;}
        Char_t getModule(void) {return m_nModule;}
        Char_t getRow(void) {return m_nRow;}
        Char_t getCol(void) {return m_nCol;}

        float getSum(int i);
        float getSum25(){return m_fSum2_25;}
        float getSumN(){return m_fSum2_N;}
        float getVar(int i);
        int   getClusterSize(int i);
        float getLocalMax(){return m_fLocalMax;}
        void getXY(float *px, float *py){*px = m_fX; *py = m_fY;}
        float getCharge(){return m_fCharge;}
        float getShower(){return m_fShower;}
        
	void setSector(Char_t s) {m_nSector = s;calcAddress();}
        void setModule(Char_t m) {m_nModule = m;calcAddress();}
        void setRow(Char_t r) {m_nRow = r;calcAddress();}
        void setCol(Char_t c) {m_nCol = c;calcAddress();}

        void setSum(int i, float fSum);
        void setVar(int i, float fVar);
        void setClusterSize(int i, int cs);
        void setSum25(float fSum);
        void setLocalMax(float lmax){m_fLocalMax = lmax;}
        void setXY(float x, float y){m_fX = x; m_fY = y;}
	void setCharge(Float_t fCharge){m_fCharge = fCharge;}

        void setEventNr(Int_t lNr){m_lEventNr = lNr;}
        Int_t getEventNr(){return m_lEventNr;}

        virtual void updateCalc();
        virtual void calcShower();

        ClassDef(HShowerHit,1) //ROOT extension

private:
       float m_fX;  //spatial coord
       float m_fY;

       float m_fSum0;  //sum of charge
       float m_fSum1;  //sum of charge
       float m_fSum2;  //sum of charge
       float m_fSum2_25;  //sum of charge
       float m_fSum2_N;  //sum of charge
       float m_fVar0;  //var. of sum of charge
       float m_fVar1;  //var. of sum of charge
       float m_fVar2;  //var. of sum of charge
       float m_fLocalMax; //charge in local max

       float m_fSumDiff01;
       float m_fSumDiff02;
       float m_fSumDiff12;
       float m_fShower; //shower - diff m_fSum2 - m_fSum1 if > 0

       int m_nClusterSize0;
       int m_nClusterSize1;
       int m_nClusterSize2;

       Int_t calcAddress();

       Char_t m_nSector;
       Char_t m_nModule;
       Char_t m_nRow;
       Char_t m_nCol;

       Float_t m_fCharge;
       Int_t m_nAddress;
       Int_t m_lEventNr;

};

#endif
