#ifndef __HShowerTrack_H
#define __HShowerTrack_H
#pragma interface

#include "hdataobject.h"

class HShowerTrack : public HDataObject {
public:
	HShowerTrack(){}
//	initialization could be used e.g. to clear the data element
	~HShowerTrack() {}

        Int_t clear();

        Int_t getAddress(void) {return m_nAddress;}
        Int_t getTrack(void) {return m_nTrack;}

        void setAddress(Int_t nAddr) {m_nAddress = nAddr;}
        void setTrack(Int_t nTrack) {m_nTrack = nTrack;}

        void setEventNr(Int_t lNr){m_lEventNr = lNr;}
        Int_t getEventNr(){return m_lEventNr;}

        ClassDef(HShowerTrack,1) //ROOT extension

private:
       Int_t m_nAddress;
       Int_t m_nTrack;
       Int_t m_lEventNr;

};

#endif
