///////////////////////////////////////////////////////////////////////////
//  HGeantTof
//
//  GEANT TOF event data
//
//  last modified on 22/11/2000 by R.Holzmann  (GSI)
///////////////////////////////////////////////////////////////////////////
#ifndef HGEANTTOF_H
#define HGEANTTOF_H

#include "hlinkeddataobject.h"

class HGeantTof : public HLinkedDataObject
{
private:
  Int_t trackNumber;      // GEANT track number
  Float_t trackLength;    // track length  (in mm)
  Float_t eHit;           // energy deposited  (in MeV)
  Float_t xHit;           // x of hit along scintillator  (in mm)
  Float_t yHit;           // y of hit along scintillator  (in mm)
  Float_t tofHit;         // time of flight of hit  (in ns)
  Float_t momHit;         // momentum of particle at hit  (in MeV/c)
  Char_t sector;          // sector number (0-5)
  Char_t module;          // module number (0-21 in->out) + (22-25 for Tofino)
  Char_t cell;            // cell number (0-7 in->out)
public:
  HGeantTof(void);
  HGeantTof(HGeantTof &aTof);
  ~HGeantTof(void);
  inline void setTrack(Int_t aTrack) {trackNumber = aTrack;}
  void setHit(Float_t ae, Float_t ax, Float_t ay, Float_t atof,
              Float_t amom, Float_t alen);
  void setAddress (Char_t s, Char_t m, Char_t c);
  inline Int_t getTrack(void) {return trackNumber;}
  void getHit(Float_t& ae, Float_t& ax, Float_t& ay, Float_t& atof,
              Float_t& amom, Float_t& alen);
  inline Char_t getSector(void) {return sector;}
  inline Char_t getModule(void) {return module;}
  inline Char_t getCell(void) {return cell;}
  inline Int_t getNLocationIndex(void) {return 3;}
  inline Int_t getLocationIndex(Int_t i); 

  ClassDef(HGeantTof,3) // Geant TOF event data class
};

#endif  /*! HGEANTTOF_H */











