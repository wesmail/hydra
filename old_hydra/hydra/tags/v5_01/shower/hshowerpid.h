#ifndef __HShowerPID_H
#define __HShowerPID_H
#pragma interface

#include "hdataobject.h"

class HShowerPID : public HDataObject {
public:
	HShowerPID(): m_fCharge(0.0){}
//	initialization could be used e.g. to clear the data element
	HShowerPID(const Float_t fCharge) : m_fCharge(fCharge) {}
	~HShowerPID() {}

        Int_t clear();

        Int_t getAddress(void) {return m_nAddress;}
        Char_t getSector(void) {return m_nSector;}
        Char_t getModule(void) {return m_nModule;}
        Char_t getRow(void) {return m_nRow;}
        Char_t getCol(void) {return m_nCol;}

        void getXYZ(Float_t *px, Float_t *py, Float_t *pz)
                                {*px = m_fX; *py = m_fY;*py = m_fZ;}
        Float_t getCharge(){return m_fCharge;}
        Float_t getShower(){return m_fShower;}
        
	void setSector(Char_t s) {m_nSector = s;}
        void setModule(Char_t m) {m_nModule = m;}
        void setRow(Char_t r) {m_nRow = r;}
        void setCol(Char_t c) {m_nCol = c;}

        void setXYZ(Float_t x, Float_t y, Float_t z)
                               {m_fX = x; m_fY = y;m_fZ = z;}
	void setCharge(Float_t fCharge){m_fCharge = fCharge;}
	void setShower(Float_t fShower){m_fShower = fShower;}

        void setEventNr(Int_t lNr){m_lEventNr = lNr;}
        Int_t getEventNr(){return m_lEventNr;}

        ClassDef(HShowerPID,1) //ROOT extension

private:
       Float_t m_fX;  //spatial coord
       Float_t m_fY;
       Float_t m_fZ;

       Float_t m_fShower; //shower - diff m_fSum2 - m_fSum1 if > 0

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
