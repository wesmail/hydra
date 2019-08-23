#ifndef HRICHRINGFINDIPUSIM_H
#define HRICHRINGFINDIPUSIM_H

#include <TObject.h>
#include <TArrayI.h>
#include <TArrayS.h>
#include "richutildef.h"

class HRichAnalysisIPUSim;
class HRichHitIPUSim;
// ****************************************************************************

class HRichRingFindIPUSim: public TObject {

// ****************************************************************************

 private:
  Bool_t pads[6][96][96]; // this should not be hard coded!!!
  Int_t Sector;
  
  Int_t lr, lc;
  Int_t labelpattern[32][2];
  Int_t label[96][96]; //!
  Int_t centers[96][96];
  Int_t ringsum[96][96];
  Int_t vetosum[96][96];
  Int_t nPads[96][96];
  Int_t patternCoord[29][5][2];

 public:
  Int_t iRingNr;
  HRichHitIPUSim *pRings;
  
  HRichRingFindIPUSim();
  virtual ~HRichRingFindIPUSim();
  
  
  Bool_t init(HRichAnalysisIPUSim*);
  
  HRichRingFindIPUSim(const HRichRingFindIPUSim& source);
  HRichRingFindIPUSim& operator=(const HRichRingFindIPUSim& source);
  
  Int_t Execute(void);
  
  void ZeroArray(void);
  inline void FiredPad(Int_t s, Int_t r, Int_t c) 
    { pads[s][r][c]=1; }
  inline void SetSector(Int_t s)
    { Sector = s; }
  
  void RingSearch();
  void MaxSearch();
  
  ClassDef(HRichRingFindIPUSim,1)
};

//============================================================================


#endif // HRICHRINGFINDIPUSIM_H



























