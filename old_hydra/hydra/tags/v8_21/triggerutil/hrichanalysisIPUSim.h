//*-- Autor: Alberica Toia
//*-- Modified: Joern Wuestenfeld 05/18/2005
//*-- Version: $Id: hrichanalysisIPUSim.h,v 1.7 2009-07-15 11:34:10 halo Exp $

#ifndef HRICHANALYSISIPUSIM_H
#define HRICHANALYSISIPUSIM_H

#include "hrichanalysisIPU.h"

class HCategory;
class HRichCalSim;
class HCategory;
class HRichRingFindIPUSim;
class HRichHitIPUSim;

// ***************************************************************************//

class HRichAnalysisIPUSim : public HRichAnalysisIPU
{

// ***************************************************************************//

 private:
  HRichRingFindIPUSim *pHardRingFindSim; //!
  HCategory *m_pTrackCat;                //! Pointer to the track container
  HRichCalSim *pRichCalSim;              //!

 public:
  HRichAnalysisIPUSim(void);
  HRichAnalysisIPUSim(const Text_t *name,const Text_t *title);
  ~HRichAnalysisIPUSim(void);

  Bool_t init(void);
  Bool_t finalize(void);
  Int_t execute(void);  

  void updateHits(Int_t nSec);

  Int_t getPadsNTrack1(Int_t padx, Int_t pady, Int_t sec);
  Int_t getPadsNTrack2(void);
  Int_t getTrack(Int_t index);
  Int_t getFlag(Int_t index);
  TObjArray *getRings(void){return pRings;};
  void setRings(TObjArray *ring){pRings = ring;};

  ClassDef(HRichAnalysisIPUSim,1) // RICH IPU emulation for simulated data
};

//============================================================================
 
#endif  // HRICHANALYSISIPUSIM_H
