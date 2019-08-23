///////////////////////////////////////////////////////////////////////////
//  HGeantRpc
//
//  GEANT RPC event data
//
//  last modified on 29/12/2004 by D. Gonzalez  (USC)
///////////////////////////////////////////////////////////////////////////
#ifndef HGEANTRPC_H
#define HGEANTRPC_H

#include "hlinkeddataobject.h"

class HGeantRpc : public HLinkedDataObject
{
private:
  Int_t    trackNumber;    // GEANT track number
  Float_t  eHit;           // energy deposited  (in MeV)
  Float_t  xHit;           // x local of hit at RPC gap (in mm)
  Float_t  yHit;           // y local of hit at RPC gap (in mm)
  Float_t  zHit;           // z local of hit at RPC gap (in mm)
  Float_t  xHitM;          // x lab   of hit at RPC gap (in mm)
  Float_t  yHitM;          // y lab   of hit at RPC gap (in mm)
  Float_t  tofHit;         // time of flight of hit  (in ns)
  Float_t  momHit;         // momentum of particle at hit  (in MeV/c)
  Float_t  thetaHit;       // polar angle
  Float_t  phiHit;         // azimutal angle
  Float_t  tlength;        // track length
  Char_t   sector;         // sector number        (0-5)
  Char_t   module;         // module/column number (0-5)
  Char_t   cell;           // cell number          (0-29)
  Char_t   gap;            // gap  number          (0-3)
  
public:
  HGeantRpc(void);
  HGeantRpc(HGeantRpc &aRpc);
  ~HGeantRpc(void);

  inline void setTrack(Int_t aTrack) {trackNumber = aTrack;}
  void setHit(Float_t ae, Float_t ax, Float_t ay, Float_t az, Float_t axm, Float_t aym, 
	      Float_t thetaHit, Float_t phiHit, Float_t atof, Float_t amom, Float_t tlength);
  inline void setAddress (Char_t s, Char_t m, Char_t c, Char_t g);
  inline Int_t getTrack(void) {return trackNumber;}
  void getHit(Float_t& ae, Float_t& ax, Float_t& ay, Float_t& az, Float_t& axm, Float_t& aym, 
	      Float_t& atheta, Float_t& aphi, Float_t& atof, Float_t& amom, Float_t& tlength);
  inline Char_t getSector(void)          {return sector;}
  inline Char_t getModule(void)          {return module;}
  inline Char_t getCell(void)            {return cell;}
  inline Char_t getGap(void)             {return gap;}
  inline Int_t getNLocationIndex(void)   {return 3;}  //FIXME
  inline Int_t getLocationIndex(Int_t i); 

  ClassDef(HGeantRpc,3) // Geant RPC event data class FIXME!!
};

inline void HGeantRpc::setAddress(Char_t s, Char_t m, Char_t c, Char_t g) {
  sector = s;
  module = m;  
  cell   = c;
  gap    = g;
}

inline Int_t HGeantRpc::getLocationIndex(Int_t i) {
  switch (i) {
    case 0 : return sector;
    case 1 : return module;
    case 2 : return cell;
    case 3 : return gap;
  }
  return -1;
}


#endif  /*! HGEANTRPC_H */











