//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 05/17/2005
//*-- Version: $Id: hrichringfindIPUSim.h,v 1.3 2005-07-15 09:02:50 wuestenf Exp $

#ifndef HRICHRINGFINDIPUSIM_H
#define HRICHRINGFINDIPUSIM_H

#include "hrichringfindIPU.h"

class HRichAnalysisIPUSim;
class HRichHitIPUSim;
// --------------------------------------------------------------------------

class HRichRingFindIPUSim: public HRichRingFindIPU
{

// --------------------------------------------------------------------------

  protected:
    void LookForTrack(HRichAnalysisIPUSim *showMe);
    virtual void fillHit(Int_t row, Int_t column, Int_t quality, Int_t vetoQuality, Int_t nPads);

  public:
    HRichRingFindIPUSim(void);
    virtual ~HRichRingFindIPUSim(void);

    Bool_t init(HRichAnalysisIPUSim*);

    HRichRingFindIPUSim(const HRichRingFindIPUSim& source);
    HRichRingFindIPUSim& operator=(const HRichRingFindIPUSim& source);

    Int_t Execute(HRichAnalysisIPUSim *giveMe);

    ClassDef(HRichRingFindIPUSim,1)   // RICH ring finder emulation for simulated data
};

//============================================================================


#endif // HRICHRINGFINDIPUSIM_H
