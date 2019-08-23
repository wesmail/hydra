#ifndef HRICHANALYSISIPU_H
#define HRICHANALYSISIPU_H

#include <TObject.h>
#include <TArrayI.h> 
#include "hreconstructor.h" 
#include "hrichdetector.h"
#include "hmatrixcatiter.h"
#include "hrichhitIPU.h"

class HCategory;
class HIterator;
class HRichDetector;
class HRichRingFindIPU;


// ***************************************************************************

class HRichAnalysisIPU : public HReconstructor {

// ***************************************************************************


 protected:
  HCategory *m_pCalCat; //! Pointer to the cal data category 
  HCategory *m_pHardHitCat; //! Pointer to the hard hit data category
 
  HRichRingFindIPU *pHardRingFind;
 
  HLocation loc;

  void updateHits(Int_t nSec);

 public:
  HRichAnalysisIPU();
  HRichAnalysisIPU(Text_t *name, Text_t *title);
  ~HRichAnalysisIPU();
  HRichAnalysisIPU(const HRichAnalysisIPU& source);
  HRichAnalysisIPU& operator=(const HRichAnalysisIPU& source);
  void SetActiveSector(Int_t sectornr);
  Int_t GetActiveSector() { return iActiveSector; }
  
  Int_t iActiveSector;
  Int_t nCounter;
  Int_t nEvtCounter;
  Int_t nCounterH;
  Int_t nEvtCounterH;
  HRichHitIPU *pRings; //!
  HIterator* fIter; //!
  HIterator* fIterH; //!

  Bool_t init();
  Bool_t finalize();
  Int_t execute();
  

  HCategory* getCalCat() { return m_pCalCat; }
  HCategory* getHardHitCat() { return m_pHardHitCat; }
  void setCalCat(HCategory* pCalCat) { m_pCalCat = pCalCat; }
  void setHardHitCat(HCategory* pHardHitCat) { m_pHardHitCat = pHardHitCat; }
  
  Int_t iRingNr;
  Int_t iRingNrTot;
  
/*    Int_t GetHardRingNr() { return iRingNr; } */
/*    HardRichHit* GetHardRing(Int_t ringnr) { return &pRings[ringnr]; } */



  ClassDef(HRichAnalysisIPU,1) 
};

//============================================================================
 
#endif  // HRICHANALYSISIPU_H
             






