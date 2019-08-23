// File: HRichHitHeader.h
//
// Author: Walter Karig <W.Karig@gsi.de>
// Last update by Thomas Eberl: 01/08/17 13:16:46
// Last update: 98/01/20 21:25:08
//

#ifndef __HRichHitHeader_H
#define __HRichHitHeader_H

#include "TObject.h"
#include "TArrayI.h"

class HRichHitHeader: public TObject
{
public:
  HRichHitHeader();
  virtual ~HRichHitHeader();

  Float_t getCurrentSec(){return detCurrentSec;}
  Int_t   getEventNr(){return iEventNr;}
  Int_t   getExecutedAnalysis(void) {return iExecutedAnalysis;}
  Int_t   getSector(void) {return m_nSector;}
  void    setCurrentSec(Float_t Charge){detCurrentSec=Charge;}
  void    setEventNr(Int_t iNr){iEventNr = iNr;}
  void    setExecutedAnalysis(Int_t i){iExecutedAnalysis = i;}
  void    setSector(Int_t s) {m_nSector = s;}

public:
  Int_t iEventNr;
  Int_t iPadFiredNr;
  Int_t iPadCleanedNr;
  Int_t iClusterCleanedNr;
  TArrayI iClustersCleaned; //! get rid of this little bugger
  Int_t iRingNr;
  Int_t iExecutedAnalysis;

  Int_t iFakePad;
  Int_t iFakeLocalMax4;
  Int_t iFakeLocalMax8;

  Int_t m_nSector;
  Float_t detCurrentSec;

  ClassDef(HRichHitHeader, 2)
};

#endif /* !__HRichHitHeader_H */



