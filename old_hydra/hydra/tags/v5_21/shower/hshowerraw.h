#ifndef __HShowerRaw_H
#define __HShowerRaw_H
#pragma interface

#include "hlocateddataobject.h"

class HShowerRaw : public HLocatedDataObject {
public:
        HShowerRaw() : m_nSector(-1), m_nModule(-1), m_nRow(-1), m_nCol(-1),
                       m_fCharge(0.0), m_lEventNr(-1) {}
///	initialization could be used e.g. to clear the data element
        HShowerRaw(const Float_t fCharge) : 
                       m_nSector(-1), m_nModule(-1), m_nRow(-1), m_nCol(-1),
                       m_fCharge(fCharge), m_lEventNr(-1) {};
	~HShowerRaw() {}

        Int_t clear();
	Float_t getCharge() const {return m_fCharge;}
        Float_t addCharge(Float_t q);
	void setCharge(Float_t fCharge){m_fCharge = fCharge;}

        Int_t getAddress(void) {return calcAddress();}
        Char_t getSector(void) {return m_nSector;}
        Char_t getModule(void) {return m_nModule;}
        Char_t getRow(void) {return m_nRow;}
        Char_t getCol(void) {return m_nCol;}
        Char_t getEventType(void) {return m_nEventType;}

        void setSector(Char_t s) {m_nSector = s;}
        void setModule(Char_t m) {m_nModule = m;}
        void setRow(Char_t r) {m_nRow = r;}
        void setCol(Char_t c) {m_nCol = c;}
        void setEventType(Char_t nEvtT) {m_nEventType = nEvtT;}

        void setEventNr(Int_t lNr){m_lEventNr = lNr;}
        Int_t getEventNr(){return m_lEventNr;}

        Int_t getNLocationIndex(void);
        Int_t getLocationIndex(Int_t i);

        void getLocation(HLocation& loc);

        ClassDef(HShowerRaw,1) // SHOWER RAW data

protected:
        Int_t calcAddress();
        Byte_t m_nEventType; 

        Char_t m_nSector;
        Char_t m_nModule;
        Char_t m_nRow;
        Char_t m_nCol;

	Float_t m_fCharge;
        Int_t m_lEventNr;
};

class HShowerRawMatr : public HShowerRaw {
public:
   HShowerRawMatr(){}

   ClassDef(HShowerRawMatr, 1)  // SHOWER RAW data
};

#endif
