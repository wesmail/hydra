//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 05/13/2005
//*-- Version: $Id: hrichhitIPU.h,v 1.6 2008-09-18 13:03:30 halo Exp $

#ifndef HRICHHITIPU_H
#define HRICHHITIPU_H

#include "TObject.h"
#include <iostream>

class HRichHitIPU: public TObject
{

protected:
  Int_t iRingX;        // x coordinate of the ring
  Int_t iRingY;        // y coordinate of the ring
  Int_t iRingQuality;  // quality factor of the ring
  Int_t iVetoQuality;  // quality of the veto region
  Int_t nPads;         // Number of pads that contribute to the ring
  Float_t fTheta;      // Theta of the ring
  Float_t fPhi;        // Phi of the ring
  Int_t DataWord;      // ???
  Int_t nSector;       // Sector of the ring [0..5]

public:
  HRichHitIPU(void);
  virtual ~HRichHitIPU(void){;}

  //HRichHitIPU(const HRichHitIPU& source);
  HRichHitIPU& operator=(const HRichHitIPU& source);

  virtual void Reset(void);

  Int_t getX(void) {return iRingX;}
  Int_t getY(void) {return iRingY;}
  Int_t getQuality(void) {return iRingQuality;}
  Int_t getVetoQuality(void) {return iVetoQuality;}
  Int_t getDataWord(void) {return DataWord;}
  Int_t getSector(void) {return nSector;}
  Int_t getNPads(void) {return nPads;}
  Float_t getTheta(void) {return fTheta;}
  Float_t getPhi(void) {return fPhi;}

  void setX(Int_t px) { iRingX = px; }
  void setY(Int_t py) { iRingY = py; }
  void setDataWord(Int_t dw) {DataWord = dw;}
  void setSector(Int_t s) {nSector = s;}
  void setQuality(Int_t q) { iRingQuality = q; }
  void setVetoQuality(Int_t v) { iVetoQuality = v; }
  void setTheta(Float_t th) { fTheta = th; }
  void setPhi(Float_t ph) { fPhi = ph; }
  void setNPads(Int_t pd){nPads = pd;};

  ClassDef(HRichHitIPU,1)  // RICH IPU hit information
};

class HRichHitIPUCont: public TObject
{
protected:

    HRichHitIPU* hit;            //
    static Bool_t sortIncreasing;// true: with increasing order, false: with decreasing order
public:
    HRichHitIPUCont(void)         {hit=new HRichHitIPU();}
    virtual ~HRichHitIPUCont(void){if(hit){ delete hit;}}
    static void     sortIncreasingOrder(Bool_t direction) {sortIncreasing=direction;}
    virtual Short_t getRingPadNTrack1(Int_t i){Error("getRingPadNTrack1()","calling virtual function!"); return 0;}
    virtual Short_t getRingPadNTrack2(Int_t i){Error("getRingPadNTrack2()","calling virtual function!"); return 0;}
    virtual Short_t getRingTrack(Int_t i)     {Error("getRingTrack()","calling virtual function!"); return 0;}
    virtual Char_t  getRingFlag(Int_t i)      {Error("getRingFlag()" ,"calling virtual function!"); return 0;}
    virtual void setRingPadNTrack1(Int_t idx, Short_t val){Error("setRingPadNTrack1()","calling virtual function!");}
    virtual void setRingPadNTrack2(Int_t idx, Short_t val){Error("setRingPadNTrack2()","calling virtual function!");}
    virtual void setRingTrack(Int_t idx, Short_t val)     {Error("setRingTrack()"     ,"calling virtual function!");}
    virtual void setRingFlag(Int_t idx, Char_t val)       {Error("setRingFlag()"      ,"calling virtual function!");}
    virtual void clearArrays()                            {Error("clearArrays()"      ,"calling virtual function!");}
    virtual HRichHitIPU* getData(){return hit;}
    virtual Int_t  Compare(const TObject *obj) const
    {
	HRichHitIPUCont* exHitCont=(HRichHitIPUCont*)obj;
        HRichHitIPU* exHit=exHitCont->getData();

	if     ( hit->getY() > exHit->getY() ) return  (sortIncreasing)?   1 :-1;
	else if( hit->getY() < exHit->getY() ) return  (sortIncreasing)?  -1 : 1;
	else   return 0;

    }
    virtual Bool_t IsSortable() const { return kTRUE;}
    ClassDef(HRichHitIPUCont,1) // RICH IPU helper container for simulated data
};


#endif // HRICHHITIPU_H
