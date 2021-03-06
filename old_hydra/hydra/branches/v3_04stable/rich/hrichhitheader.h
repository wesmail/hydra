// File: HRichHitHeader.h
//
// Author: Walter Karig <W.Karig@gsi.de>
// Last update: 98/01/20 21:25:08
//

#ifndef __HRichHitHeader_H
#define __HRichHitHeader_H

#include "TObject.h"
#include "hdataobject.h"

class HRichHitHeader: public HDataObject
{
public:
  HRichHitHeader();
  virtual ~HRichHitHeader();

  Char_t getSector(void) {return m_nSector;}
  void setSector(Char_t s) {m_nSector = s;}
 
  Int_t getEventNr(){return iEventNr;}
  void setEventNr(Int_t iNr){iEventNr = iNr;}

public:
  Int_t iEventNr;
  Int_t iPadFiredNr;
  Int_t iPadCleanedNr;
  Int_t iClusterCleanedNr;
  TArrayI iClustersCleaned;
  Int_t iRingNr;

  Int_t iFakePad;
  Int_t iFakeLocalMax4;
  Int_t iFakeLocalMax8;

  Int_t m_nSector;

  ClassDef(HRichHitHeader, 1)
};

#endif /* !__HRichHitHeader_H */
