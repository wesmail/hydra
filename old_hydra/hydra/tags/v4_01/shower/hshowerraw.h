#ifndef __HShowerRaw_H
#define __HShowerRaw_H
#pragma interface

#include "hlocateddataobject.h"

class HShowerRaw : public HLocatedDataObject {
public:
	HShowerRaw(): m_fCharge(0.0){clear();}
//	initialization could be used e.g. to clear the data element
	HShowerRaw(const Float_t fCharge) {clear(); setCharge(fCharge);}
	~HShowerRaw() {}

        Int_t clear();
	Float_t getCharge() const {return m_fCharge;}
        Float_t addCharge(Float_t q);
	void setCharge(Float_t fCharge){m_fCharge = fCharge;}

        Int_t getAddress(void) {return m_nAddress;}
        Char_t getSector(void) {return m_nSector;}
        Char_t getModule(void) {return m_nModule;}
        Char_t getRow(void) {return m_nRow;}
        Char_t getCol(void) {return m_nCol;}
        Char_t getEventType(void) {return m_nEventType;}

        void setSector(Char_t s) {m_nSector = s;calcAddress();}
        void setModule(Char_t m) {m_nModule = m;calcAddress();}
        void setRow(Char_t r) {m_nRow = r;calcAddress();}
        void setCol(Char_t c) {m_nCol = c;calcAddress();}
        void setEventType(Char_t nEvtT) {m_nEventType = nEvtT;}

        void setEventNr(Int_t lNr){m_lEventNr = lNr;}
        Int_t getEventNr(){return m_lEventNr;}

        Int_t getNLocationIndex(void);
        Int_t getLocationIndex(Int_t i);

        void getLocation(HLocation& loc);

        ClassDef(HShowerRaw,1) //ROOT extension

private:
        Byte_t m_nEventType; 
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
