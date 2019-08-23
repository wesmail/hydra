///////////////////////////////////////////////////////////////////////////
//  HGeantMdc
//
//  GEANT MDC event data
//
//  last modified on 29/1/99 by R.Holzmann  (GSI)
///////////////////////////////////////////////////////////////////////////
#ifndef HGEANTMDC_H
#define HGEANTMDC_H

#include "hlocateddataobject.h"

class HGeantMdc : public HLocatedDataObject
{
private:
  Int_t trackNumber;        // GEANT track number
  Float_t xHit;             // x of hit  (in cm)
  Float_t yHit;             // y of hit  (in cm)
  Float_t thetaHit;         // theta of hit  (0-180 deg)
  Float_t phiHit;           // phi of hit  (0-360 deg)
  Float_t tofHit;           // time of flight of hit  (in ns)
  Char_t sector;            // sector number  (0-5)
  Char_t module;            // module number  (0-3)
  Char_t layer;             // layer number  (0-5)
public:
  HGeantMdc(void);
  HGeantMdc(HGeantMdc &aMdc);
  ~HGeantMdc(void);
  inline void setTrack(Int_t aTrack) {trackNumber = aTrack;}
  void setHit(Float_t ax, Float_t ay, Float_t atof);
  void setIncidence(Float_t ath, Float_t aph);
  void setAddress (Char_t s, Char_t m, Char_t l);
  inline Int_t getTrack(void) {return trackNumber;}
  void getHit(Float_t& ax, Float_t& ay, Float_t& atof);
  void getIncidence(Float_t &ath, Float_t &aph);
  inline Char_t getSector(void) {return sector;}
  inline Char_t getModule(void) {return module;}
  inline Char_t getLayer(void) {return layer;}
  inline Int_t getNLocationIndex(void) {return 3;}
  inline Int_t getLocationIndex(Int_t i); 

  ClassDef(HGeantMdc,1) // Geant MDC event data class
};

#endif  /*! HGEANTMDC_H */









