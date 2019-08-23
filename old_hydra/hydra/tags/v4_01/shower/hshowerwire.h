#ifndef __HSHOWERDIGI_WIRE
#define __HSHOWERDIGI_WIRE

#include <TObject.h>
#include <TObjArray.h>
#include "hshowerparset.h"
#include "hshowergeantwire.h"

class HShowerWire : public TObject{
public:
   HShowerWire();
   virtual ~HShowerWire();

   Float_t getYWire(){return fYwir;}
   Int_t getNrWire(){return nWireNr;}

   void setYWire(Float_t fY){fYwir = fY;}
   void setNrWire(Int_t nr){nWireNr = nr;}
private:
   Int_t nWireNr;
   Float_t fYwir;


ClassDef(HShowerWire, 1)
};

class HShowerWireTab : public HShowerParSet{
public:
    HShowerWireTab();
    virtual ~HShowerWireTab();

    HShowerWire* getWire(int nWire);
    Int_t addWire(HShowerWire*);
    Int_t setWire(HShowerWire* pWire, Int_t n);
    Int_t getWireNr(){return nWires;}

    void setDistWire(Float_t fDist){fDistWire = fDist;}
    Float_t getDistWire(){return fDistWire;}

    Int_t lookupWire(float y);

    Bool_t initAscii(HParHadAsciiFileIo* pHadAsciiFile);
    Bool_t writeAscii(HParHadAsciiFileIo* pHadAsciiFile);
    void reset();

    void setModuleID(int id){m_nModuleID = id;}
    int getModuleID(){return m_nModuleID;}

private:

   Float_t fDistWire;
   Int_t nTotalNr;

   TObjArray m_WiresArr;
   Int_t nWires;
   Int_t m_nModuleID;

   Float_t fNewX, fNewY, fQ;
   Int_t nFiredWire;

  ClassDef(HShowerWireTab, 1)
};


#endif
