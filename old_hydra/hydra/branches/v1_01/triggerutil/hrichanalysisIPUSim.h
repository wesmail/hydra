#ifndef HRICHANALYSISIPUSIM_H
#define HRICHANALYSISIPUSIM_H

#include <TObject.h>
#include <TArrayI.h> 
#include "hreconstructor.h" 
#include "hrichdetector.h"
#include "hmatrixcatiter.h"
#include "hrichhitIPUSim.h"
#include "hrichpad.h"
#include "hrichgeometrypar.h"


class HCategory;
class HIterator;
class HRichDetector;
class HRichRingFindIPUSim;


// ***************************************************************************

class HRichAnalysisIPUSim : public HReconstructor {

// ***************************************************************************


 protected:
  HCategory *m_pCalCat; //! Pointer to the raw data category 
  HCategory *m_pHardHitCat; //! Pointer to the hard hit data category
 

  HRichRingFindIPUSim *pHardRingFind;

  HParSet *fpGeomPar;
 
  HLocation loc;

  void updateHits(Int_t nSec);

 public:
  HRichAnalysisIPUSim();
  HRichAnalysisIPUSim(Text_t *name, Text_t *title);
  ~HRichAnalysisIPUSim();
  HRichAnalysisIPUSim(const HRichAnalysisIPUSim& source);
  HRichAnalysisIPUSim& operator=(const HRichAnalysisIPUSim& source);
  void SetActiveSector(Int_t sectornr);
  Int_t GetActiveSector() { return iActiveSector; }
  
  Int_t iActiveSector;
  Int_t nCounter;
  Int_t nEvtCounter;
  Int_t nCounterH;
  Int_t nEvtCounterH;
  HRichHitIPUSim *pRings; //!
  HIterator* fIter; //!
  HIterator* fIterH; //!

  Bool_t init();
  Bool_t finalize();
  Int_t execute();
  

  HCategory* getCalCat() { return m_pCalCat; }
  HCategory* getHardHitCat() { return m_pHardHitCat; }
  void setcalCat(HCategory* pCalCat) { m_pCalCat = pCalCat; }
  void setHardHitCat(HCategory* pHardHitCat) { m_pHardHitCat = pHardHitCat; }
  
  Int_t iRingNr;
  Int_t iRingNrTot;
  

 HRichGeometryPar* getGeometryPar() { return ((HRichGeometryPar*)fpGeomPar); }
 HParSet* getGeomPar() { return fpGeomPar; }
 void setGeomPar(HParSet* pPar) { fpGeomPar = pPar; }

  ClassDef(HRichAnalysisIPUSim,1) 
};

//============================================================================
 
#endif  // HRICHANALYSISIPUSIM_H
             






