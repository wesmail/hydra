///////////////////////////////////////////////////////////////////////////
//  HGeantTof
//
//  GEANT TOF event data
//
//  last modified on 18/11/99 by R.Holzmann  (GSI)
///////////////////////////////////////////////////////////////////////////
#ifndef HGEANTTOF_H
#define HGEANTTOF_H

#include "hlinkeddataobject.h"

class HGeantTof : public HLinkedDataObject
{
private:
  Int_t trackNumber;        // GEANT track number
  Float_t eHit;             // energy deposited  (in MeV)
  Float_t xHit;             // x of hit along scintillator (in cm)
  Float_t yHit;             // y of hit along scintillator (in cm)
  Float_t tofHit;           // time of flight of hit  (in ns)
  Char_t sector;            // sector number  (0-5)
  Char_t module;            // module number  (0-21) + (22-25 for Tofino)
  Char_t cell;              // cell number  (0-7)
public:
  HGeantTof(void);
  HGeantTof(HGeantTof &aTof);
  ~HGeantTof(void);
  inline void setTrack(Int_t aTrack) {trackNumber = aTrack;}
  void setHit(Float_t ae, Float_t ax, Float_t ay, Float_t atof);
  void setAddress (Char_t s, Char_t m, Char_t c);
  inline Int_t getTrack(void) {return trackNumber;}
  void getHit(Float_t& ae, Float_t& ax, Float_t& ay, Float_t& atof);
  inline Char_t getSector(void) {return sector;}
  inline Char_t getModule(void) {return module;}
  inline Char_t getCell(void) {return cell;}
  inline Int_t getNLocationIndex(void) {return 3;}
  inline Int_t getLocationIndex(Int_t i); 

  ClassDef(HGeantTof,2) // Geant TOF event data class
};

#endif  /*! HGEANTTOF_H */











