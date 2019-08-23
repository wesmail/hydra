//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 05/13/2005
//*-- Version: $Id: hrichhitIPUSim.h,v 1.4 2005-10-10 11:32:26 kempter Exp $

#ifndef HRICHHITIPUSIM_H
#define HRICHHITIPUSIM_H

#include "hrichhitIPU.h"

class HRichHitIPUSim: public HRichHitIPU
{
protected:
  Int_t track1;                    //
  Int_t track2;                    //
  Int_t track3;                    //
  Int_t weigTrack1;                //
  Int_t weigTrack2;                //
  Int_t weigTrack3;                //
  Int_t flag1;                     //
  Int_t flag2;                     //
  Int_t flag3;                     //


public:
  HRichHitIPUSim(void);
  virtual ~HRichHitIPUSim(void){;}

 HRichHitIPUSim& operator=(const HRichHitIPUSim& source){

    if (this != &source)
    {
	iRingX = source.iRingX;
	iRingY = source.iRingY;
	iRingQuality = source.iRingQuality;
	iVetoQuality = source.iVetoQuality;
	nSector = source.nSector;
	nPads = source.nPads;
	fTheta =source.fTheta;
	fPhi =source.fPhi;
	DataWord=source.DataWord;

	track1 = source.track1;
	track2 = source.track2;
	track3 = source.track3;
	weigTrack1 = source.weigTrack1;
	weigTrack2 = source.weigTrack2;
	weigTrack3 = source.weigTrack3;
	flag1 = source.flag1;
	flag2 = source.flag2;
	flag3 = source.flag3;
    }
    return *this;
  }
  HRichHitIPUSim& operator=(const HRichHitIPU& source){
    HRichHitIPU::operator=(source);

    return *this;
  }

  Int_t getTrack1(void){return track1;};
  Int_t getTrack2(void){return track2;};
  Int_t getTrack3(void){return track3;};
  Int_t getWeightTrack1(void){return weigTrack1;};
  Int_t getWeightTrack2(void){return weigTrack2;};
  Int_t getWeightTrack3(void){return weigTrack3;};
  Int_t getFlag1(void){return flag1;};
  Int_t getFlag2(void){return flag2;};
  Int_t getFlag3(void){return flag3;};

  void setTrack1(Int_t tr){track1 = tr;};
  void setTrack2(Int_t tr){track2 = tr;};
  void setTrack3(Int_t tr){track3 = tr;};
  void setFlag1(Int_t fl){flag1 = fl;};
  void setFlag2(Int_t fl){flag2 = fl;};
  void setFlag3(Int_t fl){flag3 = fl;};
  void setWeightTrack1(Int_t val){weigTrack1 = val;};
  void setWeightTrack2(Int_t val){weigTrack2 = val;};
  void setWeightTrack3(Int_t val){weigTrack3 = val;};
  void Reset();
  ClassDef(HRichHitIPUSim,1) // RICH IPU for simulated data
};


class HRichHitIPUSimCont: public TObject
{
protected:
    Short_t iRingPadNTrack1[256];    //
    Short_t iRingPadNTrack2[256];    //
    Short_t iRingTrack[256];         //
    Char_t  iRingFlag[256];          //

    HRichHitIPUSim* hit;             //
    static Bool_t sortIncreasing;           // true: with increasing order, false: with decreasing order

public:
    HRichHitIPUSimCont(void)         {clearArrays(); hit=new HRichHitIPUSim();}
    virtual ~HRichHitIPUSimCont(void){delete hit;}
    static void  sortIncreasingOrder(Bool_t direction) {sortIncreasing=direction;}
    Short_t getRingPadNTrack1(Int_t i){return iRingPadNTrack1[i];}
    Short_t getRingPadNTrack2(Int_t i){return iRingPadNTrack2[i];}
    Short_t getRingTrack(Int_t i){return iRingTrack[i];}
    Char_t  getRingFlag(Int_t i){return iRingFlag[i];}
    void setRingPadNTrack1(Int_t idx, Short_t val){iRingPadNTrack1[idx] = val;}
    void setRingPadNTrack2(Int_t idx, Short_t val){iRingPadNTrack2[idx] = val;}
    void setRingTrack(Int_t idx, Short_t val){iRingTrack[idx] = val;}
    void setRingFlag(Int_t idx, Char_t val){iRingFlag[idx] = val;}
    void clearArrays()
    {
	for(Int_t i=0;i<256;i++){
	    iRingPadNTrack1[i]=0;
	    iRingPadNTrack2[i]=0;
	    iRingTrack[i]     =0;
	    iRingFlag[i]      =0;

	}
    }
    HRichHitIPUSim* getData(){return hit;}
    Int_t  Compare(const TObject *obj) const
    {
	HRichHitIPUSimCont* exHitCont=(HRichHitIPUSimCont*)obj;
        HRichHitIPUSim* exHit=exHitCont->getData();


	if     ( hit->getY() > exHit->getY()) return (sortIncreasing)?  1 :-1;
	else if( hit->getY() < exHit->getY()) return (sortIncreasing)? -1 : 1;
	else   return 0;

    }
    Bool_t IsSortable() const { return kTRUE;}

    ClassDef(HRichHitIPUSimCont,1) // RICH IPU helper container for simulated data
};

#endif // HRICHHITIPUSIM_H
