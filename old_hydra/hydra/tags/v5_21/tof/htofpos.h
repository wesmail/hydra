///////////////////////////////////////////////////////////////////////////
//  HTofPos
//
//  GEANT TOF event data
//
//  last modified on 11/04/00 by M.Benovic  (IP SAS)
///////////////////////////////////////////////////////////////////////////
#ifndef HTOFPOS_H
#define HTOFPOS_H

#include "hlinkeddataobject.h"
#include "TMath.h"

//class HTofPos : public HLinkedDataObject
class HTofPos : public TObject
{
private:
  Float_t tofLabX;             // x position in lab. system (in mm)
  Float_t tofLabY;             // y position in lab. system (in mm)
  Float_t tofLabZ;             // z position in lab. system (in mm)
  Float_t position[4];         // vector of the lab. position
  Int_t s;                     // sector number  (0-5)
  Int_t m;                     // module number  (0-7)
  Int_t c;                     // cell number  (0-7)
  Float_t xhit;                // x-position of hit in scintilator (in mm)

 void HTofPos::toftranslab(Int_t &s, Int_t &m, Int_t &c, Float_t &xhit);
    
public:
  HTofPos(void);
  ~HTofPos(void);
  double HTofPos::getTofLabX(Int_t sec, Int_t mod, Int_t cell, Float_t xhitpos);
  double HTofPos::getTofLabY(Int_t sec, Int_t mod, Int_t cell, Float_t xhitpos);
  double HTofPos::getTofLabZ(Int_t sec, Int_t mod, Int_t cell, Float_t xhitpos);

  ClassDef(HTofPos,0) //Lab. TOF position data class
};

R__EXTERN HTofPos *gTofPos;

#endif  /*! HTOFPOS_H */











