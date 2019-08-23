#ifndef __HShowerTofPID_H
#define __HShowerTofPID_H
#pragma interface

#include <TObject.h>

class HShowerTofPID  : public TObject {
public:
        HShowerTofPID (void);
       ~HShowerTofPID (void); 

        Int_t clear(void);
        Int_t getOuterHitId(){return m_nOuterHit;}  
        Int_t getAddress(void) {return m_nAddress;}
        Char_t getSector(void) {return m_nSector;}
        Char_t getModule(void) {return m_nModule;}
        Char_t getRow(void) {return m_nRow;}
        Char_t getCol(void) {return m_nCol;}
						   
        Float_t getRadius(void) {return m_fRad;}
	Float_t getPhi(void) {return m_fPhi;}
	Float_t getTheta(void) {return m_fTheta;}
						   
        Float_t getFact10(void){return m_fFact10;}
        Float_t getFact20(void){return m_fFact20;}
        Float_t getTof(void){return m_fTof;}
        Float_t getTofinoCell(void){return m_nTofinoCell;}
        Float_t getTofinoMult(void){return m_nTofinoMult;}
		
        void setAddress(Int_t nAddress) {m_nAddress = nAddress;}
        void setSector(Char_t s) {m_nSector = s;}
        void setModule(Char_t m) {m_nModule = m;}
        void setRow(Char_t r) {m_nRow = r;}
        void setCol(Char_t c) {m_nCol = c;}

        void setRadius(Float_t rad){m_fRad = rad;}
	void setPhi(Float_t phi) {m_fPhi = phi;}
	void setTheta(Float_t theta) {m_fTheta = theta;}

        void setTof(Float_t fTof){m_fTof = fTof;}
        void setFact10(Float_t fact10){m_fFact10 = fact10;}
        void setFact20(Float_t fact20){m_fFact20 = fact20;}
        void setTofinoCell(Int_t cell){m_nTofinoCell = cell;}
        void setTofinoMult(Int_t mult){m_nTofinoMult = mult;}
        void setOuterHitId(Int_t outerHit){m_nOuterHit = outerHit;}  

        Bool_t  IsSortable() const { return kTRUE; }
        Int_t   Compare(TObject *obj);

        ClassDef(HShowerTofPID ,1) //ROOT extension

private:
       Float_t m_fRad; // sphere coord
       Float_t m_fPhi;
       Float_t m_fTheta;

       Float_t m_fFact10;
       Float_t m_fFact20;

       Char_t m_nSector;
       Char_t m_nModule;
       Char_t m_nRow;
       Char_t m_nCol;

       Int_t m_nTofinoCell;
       Int_t m_nTofinoMult;
       Float_t m_fTof;
	   
       Int_t m_nOuterHit;
       Int_t m_nAddress;
};

inline Int_t   HShowerTofPID ::Compare(TObject *obj) {
   if (m_nAddress==((HShowerTofPID *)obj)->getAddress()) return 0;
   return (m_nAddress > ((HShowerTofPID *)obj)->getAddress() ? 1 : -1);
}

#endif
