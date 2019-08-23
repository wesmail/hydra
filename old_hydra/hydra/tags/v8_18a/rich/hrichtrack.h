// File: hrichtrack.h
//
//
// Author: Laura Fabbietti <L.Fabbietti@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 01/08/17 13:12:50
// Last update by Laura Fabbietti: 01/07/09 14:57:21
// Modified by Wolfgang Koenig Dec. 2000 (event number removed)
//
#ifndef HRICHTRACK_H
#define HRICHTRACK_H
#pragma interface


#include "TObject.h"

class HRichTrack : public TObject {
public:
    HRichTrack() {}
    ~ HRichTrack() {}
    Int_t clear();
    
    Int_t getTrack(void) {return m_nTrack;}
    Int_t getEventNr() {return m_lEventNr;}
    Int_t getFlag(void) {return m_Flag;} 
    Int_t getAddress(void) {return m_Address;} 
    
    void setTrack(Int_t nTrack) {m_nTrack = nTrack;}
    void setEventNr(Int_t lNr) {m_lEventNr = lNr;}
    void setFlag(Int_t nFlag) {m_Flag = nFlag;}
    void setAddress(Int_t nAd) {m_Address = nAd;}
         
    Bool_t IsSortable() const {return kTRUE;}
    Int_t  Compare(const TObject *obj) const {
       // HRichTrack is sorted by the pad address
       if (m_Address == ((HRichTrack*)obj)->m_Address) return 0;
       else if (m_Address > ((HRichTrack*)obj)->m_Address) return 1;
            else return -1;
    }
    
private:
    Int_t m_nTrack;   // Track number of RICH hit
    Int_t m_lEventNr; // Event number taken from CAL<-RAW<-Unpacker
    Int_t m_Flag;     // Flag to distinguish photons (0) and direct hits (1)
    Int_t m_Address;  // pad address, cf. HRichCal::calcAddress()
 
    ClassDef(HRichTrack,1) // RICH HGeant track class	
};

#endif /* !HRICHTRACK_H */






