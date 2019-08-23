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

        Int_t getAddress(void) {return calcAddress();}
        Char_t getSector(void) {return m_nSector;}
        Char_t getModule(void) {return m_nModule;}
        Char_t getRow(void) {return m_nRow;}
        Char_t getCol(void) {return m_nCol;}

        Float_t getSum(Int_t i);
        Float_t getSum25(){return m_fSum2_25;}
        Float_t getSumN(){return m_fSum2_N;}
        Float_t getVar(Int_t i);
        Int_t   getClusterSize(Int_t i);
        Float_t getLocalMax(){return m_fLocalMax;}
        void getXY(Float_t *px, Float_t *py){*px = m_fX; *py = m_fY;}
        void getLabXYZ(Float_t *px, Float_t *py, Float_t *pz)
                                {*px = m_fLabX; *py = m_fLabY;*pz = m_fLabZ;}
        void getSphereCoord(float *pr, float *pphi, float *ptheta)
                          {*pr = m_fRad; *pphi = m_fPhi ; *ptheta = m_fTheta;}

        Float_t getCharge(){return m_fCharge;}
        Float_t getShower(){return m_fShower;}
        Float_t getSumDiff01(){return m_fSumDiff01;}
        Float_t getSumDiff02(){return m_fSumDiff02;}
        
	void setSector(Char_t s) {m_nSector = s;}
        void setModule(Char_t m) {m_nModule = m;}
        void setRow(Char_t r) {m_nRow = r;}
        void setCol(Char_t c) {m_nCol = c;}

        void setSum(Int_t i, Float_t fSum);
        void setVar(Int_t i, Float_t fVar);
        void setClusterSize(Int_t i, Int_t cs);
        void setSum25(Float_t fSum);
        void setLocalMax(Float_t lmax){m_fLocalMax = lmax;}
        void setXY(Float_t x, Float_t y){m_fX = x; m_fY = y;}
        void setLabXYZ(Float_t x, Float_t y, Float_t z)
                                    {m_fLabX = x; m_fLabY = y;m_fLabZ = z;}
        void setSphereCoord(float r, float phi, float theta)
                                    {m_fRad = r; m_fPhi = phi ; m_fTheta=theta;}
        void setCharge(Float_t fCharge){m_fCharge = fCharge;}
        void setShower(Float_t fShower){m_fShower = fShower;}

        void setEventNr(Int_t lNr){m_lEventNr = lNr;}
        Int_t getEventNr(){return m_lEventNr;}

        virtual void updateCalc();
        Int_t calcAddress(); //calculation of position of pad


        Bool_t  IsSortable() const { return kTRUE; }
        Int_t   Compare(TObject *obj);

        ClassDef(HShowerHit,1) //ROOT extension

private:
       Float_t m_fX;  //local coord
       Float_t m_fY;
 
       Float_t m_fLabX;  //spatial coord in lab
       Float_t m_fLabY;
       Float_t m_fLabZ;

       float m_fRad; // sphere coord in lab
       float m_fPhi;
       float m_fTheta;

       Float_t m_fSum0;  //sum of charge of nine pads in pre 
       Float_t m_fSum1;  //sum of charge of nine pads in post
       Float_t m_fSum2;  //sum of charge  of nine pads in post2
       Float_t m_fSum2_25;  //sum of charge of 25 pads in post2
       Float_t m_fSum2_N;  //normalized sum of charge of 25 pads to 9 pads 
       Float_t m_fVar0;  //var. of sum of charge in pre
       Float_t m_fVar1;  //var. of sum of charge in post
       Float_t m_fVar2;  //var. of sum of charge in post2
       Float_t m_fLocalMax; //charge in local max - not used 

       Float_t m_fSumDiff01;  //:= m_fSum1 - m_fSum0
       Float_t m_fSumDiff02;  //:= m_fSum2 - m_fSum0
       Float_t m_fSumDiff12;  //:= m_fSum2 - m_fSum1
       Float_t m_fShower; //value of shower definition. it's defined 
                          //in HShowerCriterium

       //number of fired cells around local max and local max
       Int_t m_nClusterSize0; 
       Int_t m_nClusterSize1;  
       Int_t m_nClusterSize2;

       Char_t m_nSector; //position of pad
       Char_t m_nModule;
       Char_t m_nRow;
       Char_t m_nCol;

       Float_t m_fCharge; //charge in local max
       Int_t m_nAddress;  //combined address of position pads 
       Int_t m_lEventNr;  //internal event number
};

inline Int_t  HShowerHit::Compare(TObject *obj) {
   if (m_nAddress==((HShowerHit*)obj)->getAddress()) return 0;
   return (m_nAddress > ((HShowerHit*)obj)->getAddress()) ? 1 : -1;
}

#endif