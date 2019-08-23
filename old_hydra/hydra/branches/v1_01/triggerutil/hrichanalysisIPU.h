#ifndef HRICHANALYSISIPU_H
#define HRICHANALYSISIPU_H

#include <TObject.h>
#include <TArrayI.h> 
#include "hreconstructor.h" 
#include "hrichdetector.h"
#include "hmatrixcatiter.h"
#include "hrichhitIPU.h"
#include "hrichpad.h"
#include "hrichgeometrypar.h"

class HCategory;
class HIterator;
class HRichDetector;
class HRichRingFindIPU;


// ***************************************************************************

class HRichAnalysisIPU : public HReconstructor {

// ***************************************************************************


 protected:
  HCategory *m_pRawCat; //! Pointer to the raw data category 
  HCategory *m_pHardHitCat; //! Pointer to the hard hit data category
 
  HRichRingFindIPU *pHardRingFind;

  HParSet *fpGeomPar;
 
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
  

  HCategory* getRawCat() { return m_pRawCat; }
  HCategory* getHardHitCat() { return m_pHardHitCat; }
  void setRawCat(HCategory* pRawCat) { m_pRawCat = pRawCat; }
  void setHardHitCat(HCategory* pHardHitCat) { m_pHardHitCat = pHardHitCat; }
  
  Int_t iRingNr;
  Int_t iRingNrTot;
  

 HRichGeometryPar* getGeometryPar() { return ((HRichGeometryPar*)fpGeomPar); }
 HParSet* getGeomPar() { return fpGeomPar; }
 void setGeomPar(HParSet* pPar) { fpGeomPar = pPar; }

  ClassDef(HRichAnalysisIPU,1) 
};

//============================================================================
 
#endif  // HRICHANALYSISIPU_H
             






