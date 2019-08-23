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
        Float_t getTrackLength(void){return m_fTrackLength;}
		
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
        void setTrackLength(Float_t TrackLength){m_fTrackLength = TrackLength;}

        Bool_t  IsSortable() const { return kTRUE; }
        inline Int_t   Compare(const TObject *obj) const;

        ClassDef(HShowerTofPID ,2) //ROOT extension

private:
       Float_t m_fRad;    // radius [mm] in sphere coordinate system  
       Float_t m_fPhi;    // azimuthal angle [deg] in sphere coordinate system
       Float_t m_fTheta;  // polar angle [deg] in sphere coordinate system

       Float_t m_fFact10; // shower multiplication factor sum_post1/sum_pre 
       Float_t m_fFact20; // shower multiplication factor sum_post2/sum_pre

       Char_t m_nSector;  // sector number
       Char_t m_nModule;  // shower module (0,1,2) 
       Char_t m_nRow;     // shower row number (0-31)
       Char_t m_nCol;     // shower column number (0-31)    

       Int_t m_nTofinoCell; // tofino cell (0-3)
       Int_t m_nTofinoMult; // tofino cell multiplicity  
       Float_t m_fTof;      // time of flight 
	   
       Int_t m_nOuterHit;   // kicktrack outer hit index
       Int_t m_nAddress;    // address of HShowerTofPID object
       Float_t m_fTrackLength; // particle track lenght
	   
};

inline Int_t   HShowerTofPID ::Compare(const TObject *obj) const {
   if (m_nAddress==((HShowerTofPID *)obj)->getAddress()) return 0;
   return (m_nAddress > ((HShowerTofPID *)obj)->getAddress() ? 1 : -1);
}

#endif
