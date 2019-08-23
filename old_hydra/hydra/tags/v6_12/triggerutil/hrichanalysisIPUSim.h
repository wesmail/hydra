#ifndef HRICHANALYSISIPUSIM_H
#define HRICHANALYSISIPUSIM_H

#include <TObject.h>
#include <TArrayI.h> 
#include "hreconstructor.h" 
#include "hrichdetector.h"
#include "hmatrixcatiter.h"
#include "hrichhitIPUSim.h"
#include "hrichringfindIPUSim.h"
#include "hrichpad.h"
#include "hrichgeometrypar.h"
#include "hrichanalysisIPU.h"
#include "hrichcalsim.h"

class HCategory;
class HRichRingFindIPUSim;

// ***************************************************************************//

class HRichAnalysisIPUSim : public HRichAnalysisIPU {

// ***************************************************************************//

 private:
  HRichRingFindIPUSim *pHardRingFindSim;
  HCategory *m_pTrackCat;//Pointer to the track container
  HRichCalSim *pRichCalSim;

 public:
  HRichAnalysisIPUSim();
  HRichAnalysisIPUSim(Text_t *name, Text_t *title);
  ~HRichAnalysisIPUSim();

  Bool_t init();
  Bool_t finalize();
  Int_t execute();  

  void updateHits(Int_t nSec);
  HRichHitIPUSim *pRings; //!

  Int_t getPadsNTrack1(Int_t padx, Int_t pady, Int_t sec);
  Int_t getPadsNTrack2();
  Int_t getTrack(Int_t index);
  Int_t getFlag(Int_t index);

  ClassDef(HRichAnalysisIPUSim,1) 
};

//============================================================================
 
#endif  // HRICHANALYSISIPUSIM_H
             






