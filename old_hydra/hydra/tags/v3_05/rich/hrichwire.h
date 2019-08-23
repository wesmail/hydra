#ifndef HRICHWIRE_H
#define HRICHWIRE_H

#include <TObject.h>
#include <TObjArray.h>
#include "hrichparset.h"
#include "hrichgeantwire.h"


class HRichWire : public TObject{

public:

   HRichWire();
   virtual ~HRichWire();

   Float_t getXWire() { return fXwire; }
   Int_t getNrWire() { return fNrWire; }

   void setXWire(Float_t fX) { fXwire = fX; }
   void setNrWire(Int_t nr) { fNrWire = nr; }

private:

   Int_t fNrWire;
   Float_t fXwire;

ClassDef(HRichWire, 0)
};


class HRichWireTab : public HRichParSet{

public:

    HRichWireTab();
    virtual ~HRichWireTab();

    HRichWire* getWire(Int_t nWire);
    Int_t addWire(HRichWire*);
    Int_t setWire(HRichWire* pWire, Int_t n);
    Int_t getWiresNr(){return nWires;}
    Float_t getDistWire(){return fDistWire;}

//    Int_t lookupWire(float x);

    Bool_t initAscii(HParHadAsciiFileIo* pHadAsciiFile);
    void reset();


private:

   Float_t fDistWire;

   TObjArray m_WiresArr;
   Int_t nWires;


  ClassDef(HRichWireTab, 0)
};


#endif // HRICHWIRE_H
