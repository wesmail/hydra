#ifndef HRICHRINGFINDIPU_H
#define HRICHRINGFINDIPU_H

#include <TObject.h>
#include <TArrayI.h>
#include <TArrayS.h>
#include "richutildef.h"

class HRichAnalysisIPU;
class HRichHitIPU;
// ****************************************************************************

class HRichRingFindIPU: public TObject {

// ****************************************************************************

 private:
  Bool_t pads[6][96][96]; // this should not be hard coded!!!
  Int_t Sector;
  
  Int_t lr, lc;
  Int_t labelpattern[24][2];
  Int_t label[96][96]; //!
  Int_t centers[96][96];
  Int_t veto[96][96];
  Int_t patternCoord[29][5][2];

 public:
  Int_t iRingNr;
  HRichHitIPU *pRings;
  
  HRichRingFindIPU();
  virtual ~HRichRingFindIPU();
  
  
  Bool_t init(HRichAnalysisIPU*);
  
  HRichRingFindIPU(const HRichRingFindIPU& source);
  HRichRingFindIPU& operator=(const HRichRingFindIPU& source);
  
  Int_t Execute(void);
  
  void ZeroArray(void);
  inline void FiredPad(Int_t s, Int_t r, Int_t c) 
    { pads[s][r][c]=1; }
  inline void SetSector(Int_t s)
    { Sector = s; }
  
  void RingSearch();
  void MaxSearch();
  
  ClassDef(HRichRingFindIPU,1)
};

//============================================================================


#endif // HRICHRINGFINDIPU_H



























