//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 05/13/2005
//*-- Version: $Id: hrichanalysisIPU.h,v 1.6 2005-09-30 14:41:08 wuestenf Exp $

#ifndef HRICHANALYSISIPU_H
#define HRICHANALYSISIPU_H

#include "hreconstructor.h"

class HCategory;
class HIterator;
class HRichDetector;
class HRichRingFindIPU;
class HSpectrometer;
class HRuntimeDb;
class HRichHitIPU;
class HRichGeometryPar;
class HParSet;
class TObjArray;
// ***************************************************************************

class HRichAnalysisIPU : public HReconstructor {

// ***************************************************************************


protected:
  HCategory *m_pRawCat;             //! Pointer to the raw data category
  HCategory *m_pHardHitCat;         //! Pointer to the hard hit data category
  HRichRingFindIPU *pHardRingFind;  //! Pointer to the ring finder
  HParSet *fpGeomPar;               //! Pointer to the geometry parameter container
  HLocation loc;                    //! Address used to store objects in output category


  TObjArray *pRings;                //! Pointer to IPU rings
  TObjArray *psortRings;            //! Pointer to IPU rings (sorted)
  HIterator* fIter;                 //! Iterator over category (which?)

  Int_t iEmulationContext;          //! Is the IPU emulated?

  Int_t iRingNr;                    //! Number of Rings
  Int_t iRingNrTot;                 //!

  void updateHits(Int_t nSec);

public:
  HRichAnalysisIPU();
  HRichAnalysisIPU(Text_t *name, Text_t *title);
  virtual ~HRichAnalysisIPU();
  HRichAnalysisIPU(const HRichAnalysisIPU& source);

  HRichAnalysisIPU& operator=(const HRichAnalysisIPU& source);

  virtual Bool_t init();
  virtual Bool_t finalize();
  virtual Int_t execute();

  HCategory* getRawCat() { return m_pRawCat; };
  HCategory* getHardHitCat() { return m_pHardHitCat; };
  void setRawCat(HCategory* pRawCat) { m_pRawCat = pRawCat; };
  void setHardHitCat(HCategory* pHardHitCat) { m_pHardHitCat = pHardHitCat; };

  HRichGeometryPar* getGeometryPar() { return ((HRichGeometryPar*)fpGeomPar); };
  HParSet* getGeomPar() { return fpGeomPar; };
  void setGeomPar(HParSet* pPar) { fpGeomPar = pPar; };
  void DataWordCut(Int_t nSec);

  TObjArray *getRings(void){return pRings;};
  void setRings(TObjArray **p){pRings=*p;};

  ClassDef(HRichAnalysisIPU,1)
};

//============================================================================

#endif  // HRICHANALYSISIPU_H
