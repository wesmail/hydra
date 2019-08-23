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
       
    
    //The ShowerTrack objects are made "sortable" by the pad address          
        Bool_t  IsSortable() const { return kTRUE; }
        Int_t   Compare(TObject *obj) { if ( m_nAddress> ((HShowerTrack*)obj)->m_nAddress || m_nAddress== ((HShowerTrack*)obj)->m_nAddress )
                                       return 1;
                                   else 
                                      return -1;
                                     }
    //-------------------------------------------------------

        ClassDef(HShowerTrack,1) //ROOT extension

private:
	Int_t m_nAddress;  //pad address
    Int_t m_nTrack;     // Track number
       Int_t m_lEventNr;

};

#endif
