#ifndef __HRichGeantWire_H
#define __HRichGeantWire_H

#include "hdataobject.h"
#include <iostream.h>

class HRichGeantWire : public HDataObject {
public:
        HRichGeantWire(void);
//      initialization could be used e.g. to clear the data element

        ~HRichGeantWire(void) {}
        void Clear(void);

        Float_t getCharge(void) const {return fCharge;}
        void getXY(Float_t* pfX, Float_t* pfY);
        Int_t getWireNr(){return nWireNr;}
        Int_t getSrcQ(){return nSrcQ;}
	void setCharge(Float_t q) {fCharge=q;}
        void setXY(Float_t fX, Float_t fY);
        void setWireNr(Int_t nNr){nWireNr = nNr;}
        void setSrcQ(Int_t nS){nSrcQ = nS;}
        friend ostream& operator<< (ostream& output, HRichGeantWire& element);

protected:
        float fCharge; // Charge for each cell
        float fXWire; 
        float fYWire; 
        int nWireNr;
        int nSrcQ;
public:
        ClassDef(HRichGeantWire,1) //Rich raw data
};

#endif

