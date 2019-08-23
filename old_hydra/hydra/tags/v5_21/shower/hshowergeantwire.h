#ifndef __HShowerGeantWire_H
#define __HShowerGeantWire_H

#include "hdataobject.h"
#include <iostream.h>

class HShowerGeantWire : public HDataObject {
public:
        HShowerGeantWire(void);
//      initialization could be used e.g. to clear the data element

        ~HShowerGeantWire(void) {}
        void Clear(void);

        Float_t getCharge(void) const {return fCharge;}
        void getXY(Float_t* pfX, Float_t* pfY);
        Int_t getWireNr(){return nWireNr;}
        Int_t getModule(){return nModule;}
        Int_t getSector(){return nSector;}
        Int_t getTrack(){return nTrack;}
	void setCharge(Float_t q) {fCharge=q;}
        void setXY(Float_t fX, Float_t fY);
        void setWireNr(Int_t nNr){nWireNr = nNr;}
        void setModule(Int_t nMod){nModule = nMod;}
        void setSector(Int_t nSect){nSector = nSect;}
        void setTrack(Int_t nTrk){nTrack = nTrk;}
        friend ostream& operator<< (ostream& output, HShowerGeantWire& element);

protected:
        float fCharge; // Charge for each hit on the wire
        float fXWire;  // X position of hit on the wire 
        float fYWire;  // Y position of hit on the wire
        int nWireNr;   // number of the fired wire
        int nModule;   // module number
        int nSector;   // sector number
        int nTrack;    // track number from Kine
public:
        ClassDef(HShowerGeantWire,1) //Shower raw data
};

#endif

