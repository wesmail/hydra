#ifndef HRICHRINGFINDIPUSIM_H
#define HRICHRINGFINDIPUSIM_H

#include <TObject.h>
#include <TArrayI.h>
#include <TArrayS.h>
#include "richutildef.h"
#include "hrichringfindIPU.h"
#include "hrichhitIPUSim.h"
#include "hrichanalysisIPUSim.h"

class HRichAnalysisIPUSim;
class HRichAnalysisIPU;
/* class HRichHitIPUSim; */
// --------------------------------------------------------------------------

class HRichRingFindIPUSim: public HRichRingFindIPU {

// --------------------------------------------------------------------------

 public:
  HRichHitIPUSim *pRings;
  
  HRichRingFindIPUSim();
  virtual ~HRichRingFindIPUSim();
    
  Bool_t init(HRichAnalysisIPUSim*);
  
  HRichRingFindIPUSim(const HRichRingFindIPUSim& source);
  HRichRingFindIPUSim& operator=(const HRichRingFindIPUSim& source);
  
  Int_t Execute(HRichAnalysisIPUSim *giveMe);
  void LookForTrack(HRichAnalysisIPUSim *showMe);

  ClassDef(HRichRingFindIPUSim,1)
};

//============================================================================


#endif // HRICHRINGFINDIPUSIM_H



























