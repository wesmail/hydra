#ifndef __HSHOWERDIGI_WIRE
#define __HSHOWERDIGI_WIRE

#include <TObject.h>
#include <TObjArray.h>
#include "hparset.h"
#include "hshowergeantwire.h"
#include "hadioascii.h"

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


ClassDef(HShowerWire, 0)
};

class HShowerWireTab : public HParSet{
public:
    HShowerWireTab();
    virtual ~HShowerWireTab();

    HShowerWire* getWire(int nWire);
    Int_t addWire(HShowerWire*);
    Int_t setWire(HShowerWire* pWire, Int_t n);
    Int_t getWireNr(){return nWires;}

    void setVoltage(Float_t fVolt){fVoltage = fVolt;}
    void setDistWire(Float_t fDist){fDistWire = fDist;}

    Float_t getVoltage(){return  fVoltage;}
    Float_t getDistWire(){return fDistWire;}

    Int_t lookupWire(float y);

    Bool_t init();
    void reset();

    void setIO(HadIoAscii *pReadParam){m_pReadParam = pReadParam;}
    HadIoAscii *getIO(){return m_pReadParam;}
private:

   Float_t fVoltage;
   Float_t fDistWire;
   Int_t nTotalNr;

   TObjArray m_WiresArr;
   Int_t nWires;

   Float_t fNewX, fNewY, fQ;
   Int_t nFiredWire;

  HadIoAscii *m_pReadParam;

  ClassDef(HShowerWireTab, 0)
};


#endif
