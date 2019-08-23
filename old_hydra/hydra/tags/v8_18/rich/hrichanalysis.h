// File: hrichanalysis.h
// ****************************************************************************
//
// HRichAnalysis
//
// Author: Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
// Modified: 1999/10/08 by Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
// ----------------------------------------------------------------------------
//Modified Oct. 2000 by W. Koenig
//
// ****************************************************************************

#ifndef HRICHANALYSIS_H
#define HRICHANALYSIS_H

#include "TObject.h"
#include "TArrayI.h"
#include "hiterator.h"
#include "hreconstructor.h"
#include "hrichanalysispar.h"
#include "hrichdetector.h"
#include "hrichgeometrypar.h"
#include "hrichhit.h"
#include "hrichhitheader.h"
#include "hrichlabel.h"
#include "hrichpad.h"
#include "hrichpadsignal.h"

class HRichDisplay;
class HCategory;
class HRichDetector;
class HRichPadClean;
class HRichPadLabel;
class HRichRingFind;


// ***************************************************************************

class HRichAnalysis: public HReconstructor {

// ***************************************************************************

public:

 Int_t iActiveAnalysis;
 Int_t iEventNr;

 Int_t iActiveSector;

 HRichDisplay *disp;
 
 Short_t * pLeftBorder; // left border of active pad area
 Short_t * pRightBorder; // dito right side (largest active col in row)

 HRichPadSignal **pPads; //!
 HRichLabel *pLabelArea;
 HRichHit *pRings;
 HIterator* fIter; //!
 HIterator* fIterHitHeader; //!

 Int_t iPadFiredNr;
 Int_t fPadFired[6];
 Int_t iPadCleanedNr;
 Int_t iClusterCleanedNr;
 TArrayI iClustersCleaned;
 Int_t iLabelNr;
 Int_t iRingNr;
 Int_t iRingNrTot;
 Int_t sectorPairNrTot;
 Int_t allPairNrTot; //2 rings in different sectors

 Int_t iFakePad;
 Int_t iFakeLocalMax4;
 Int_t iFakeLocalMax8;

 Int_t maxFiredSectorPads;
 Int_t maxFiredTotalPads;

protected:

 HRichPadClean *pPadClean;
 HRichPadLabel *pPadLabel;
 HRichRingFind *pRingFind;

 HRichPadSignal * pSectorPads; //points to pads of active sector
 Int_t* pads;
 Int_t maxCols, maxRows,maxPads;

 HCategory *m_pCalCat; //Pointer to the cal data category
 HCategory *m_pPadCat; //Pointer to the pad auxiliary data category
 HCategory *m_pHitCat; //Pointer to the hit data category
 HCategory *m_pHitHdrCat; //Pointer to the hit header data category
    
 HParSet *fpAnalysisPar;
 HParSet *fpGeomPar;

 HLocation loc;

 TArrayI iPadActive;
 HRichHitHeader *hithdrLoop;//!

public:
//ll
    HCategory *mTemp;//Pointer to the category containing local max
    //ll
 HRichAnalysis();
 HRichAnalysis(Text_t *name, Text_t *title, Bool_t kSkip=kFALSE);
 virtual ~HRichAnalysis();
 
 HRichAnalysis(const HRichAnalysis& source);
 HRichAnalysis& operator=(const HRichAnalysis& source);

 Bool_t init();
 Bool_t reinit();
 Bool_t finalize();
 Int_t execute();
 Bool_t initParameters();

 void Reset();

 Int_t GetPadsXNr() { return maxCols; }
 Int_t GetPadsYNr() { return maxRows; }

 void SetActiveSector(Int_t sectornr);
 Int_t GetActiveSector() { return iActiveSector; }
 Int_t SetNextSector();

 Int_t GetPadNr() { return maxPads; }

 HRichPadSignal* GetPad(Int_t padnr) { return &pSectorPads[padnr]; }
 HRichPadSignal* GetPad(Int_t padx, Int_t pady) { return &pSectorPads[padx + pady*maxCols]; }

 Int_t GetLabelNr() { return iLabelNr; }
 HRichLabel* GetLabel(Int_t labelnr) { return &pLabelArea[labelnr]; }

 Int_t GetRingNr() { return iRingNr; }
 HRichHit* GetRing(Int_t ringnr) { return &pRings[ringnr]; }

 void SetActive() { iActiveAnalysis = 1; }
 Int_t IsActive() { return iActiveAnalysis; }

 Int_t IsOut(Int_t x, Int_t y) {
  return (!(y >= 0 && y < maxRows &&
            x >= pLeftBorder[y] && x <= pRightBorder[y]));
 }

 Int_t IsOut(Int_t nowPad, Int_t dx, Int_t dy);
 Int_t IsOut(Int_t x, Int_t y, Int_t dx, Int_t dy);

 HCategory* getCalCat() { return m_pCalCat; }
 HCategory* getHitCat() { return m_pHitCat; }
 HCategory* getHitHdrCat() { return m_pHitHdrCat; }
 void setCalCat(HCategory* pCalCat) { m_pCalCat = pCalCat; }
 void setHitCat(HCategory* pHitCat) { m_pHitCat = pHitCat; }
 void setHitHdrCat(HCategory* pHitHdrCat) { m_pHitHdrCat = pHitHdrCat; } 

 HRichAnalysisPar *getParams() { return ((HRichAnalysisPar*)fpAnalysisPar); }
 HRichGeometryPar* getGeometryPar() { return ((HRichGeometryPar*)fpGeomPar); }
 HParSet* getAnalysisPar() { return fpAnalysisPar; }
 HParSet* getGeomPar() { return fpGeomPar; }
 void setAnalysisPar(HParSet* pPar) { fpAnalysisPar = pPar; }
 void setGeomPar(HParSet* pPar) { fpGeomPar = pPar; }
 Int_t secWithCurrent;

protected:
 Bool_t  kSkipEvtIfNoRing;
 void clear();
 void updateHeaders(const Int_t nSec, Int_t nEvNr);
 void updateHits(Int_t nSec);
 
 Int_t getPadsIndex(Int_t nSec, Int_t nRow, Int_t nCol);
 Int_t* getPadsPointer(Int_t* pPads, Int_t nSec);


 ClassDef(HRichAnalysis,1)
};

//============================================================================

#endif // HRICHANALYSIS_H
