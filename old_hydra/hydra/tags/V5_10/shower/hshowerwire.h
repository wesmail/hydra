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
   Int_t nWireNr;  //wire identification
   Float_t fYwir;  //position in local coordinates


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
   Float_t fDistWire;  //distance between wires

   TObjArray m_WiresArr;  //table of inforamtion of one wire
   Int_t nWires;          //number of wires
   Int_t m_nModuleID;     //module identifiction

   ClassDef(HShowerWireTab, 1)
};


#endif
