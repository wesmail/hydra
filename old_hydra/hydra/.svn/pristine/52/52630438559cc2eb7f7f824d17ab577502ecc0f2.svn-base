// File: hrichanalysis.h
// ****************************************************************************
//
// HRichAnalysis
//
// Author: Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
// Modified: 1999/10/08 by Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
// Modified: 00/01/19 by Laura Fabbietti
// ----------------------------------------------------------------------------
//
// ****************************************************************************

#ifndef HRICHANALYSIS_H
#define HRICHANALYSIS_H

#include "TObject.h"
#include "TArrayI.h"
#include "hreconstructor.h"
#include "hrichpad.h"
#include "hrichpadsignal.h"
#include "hrichlabel.h"
#include "hrichhit.h"
#include "hrichanalysispar.h"
#include "hrichgeometrypar.h"


class HRichDisplay;
class HCategory;
class HIterator;
class HDetector;
class HRichPadClean;
class HRichPadLabel;
class HRichRingFind;


// ***************************************************************************

class HRichAnalysis: public HReconstructor {

// ***************************************************************************

public:

 Int_t iActiveAnalysis;
 Int_t iEventNr;
 Int_t nSecCounter; //sector counter to know how many times a special sector was fired
 Int_t iActiveSector;

 HRichDisplay *disp;
 
 HRichPadSignal **pPads;
 HRichLabel *pLabelArea;
 HRichHit *pRings;
 HIterator* fIter; //!

 Int_t iPadFiredNr;
 Int_t fPadFired[6];
 Int_t iPadCleanedNr;
 Int_t iClusterCleanedNr;
 TArrayI iClustersCleaned;
 Int_t iLabelNr;
 Int_t iRingNr;

 Int_t iFakePad;
 Int_t iFakeLocalMax4;
 Int_t iFakeLocalMax8;
 

private:

 HRichPadClean *pPadClean;
 HRichPadLabel *pPadLabel;
 HRichRingFind *pRingFind;
 Int_t* pads; 

 HCategory *m_pCalCat; //Pointer to the cal data category
 HCategory *m_pPadCat; //Pointer to the pad auxiliary data category
 HCategory *m_pHitCat; //Pointer to the hit data category
 HCategory *m_pHitHdrCat; //Pointer to the hit header data category

 HParSet *fpAnalysisPar;
 HParSet *fpGeomPar;

 HLocation loc;

 HDetector *pRichDet;

public:

 HRichAnalysis();
 HRichAnalysis(Text_t *name, Text_t *title);
 virtual ~HRichAnalysis();
 
 HRichAnalysis(const HRichAnalysis& source);
 HRichAnalysis& operator=(const HRichAnalysis& source);

 Bool_t init();
 Bool_t finalize();
 Int_t execute();
 Bool_t initParameters();

 void Reset();

 Int_t GetPadsXNr();
 Int_t GetPadsYNr();

 void SetActiveSector(Int_t sectornr);
 Int_t GetActiveSector();
 Int_t SetNextSector();

 Int_t GetPadNr();
 HRichPadSignal* GetPad(Int_t padnr);
 HRichPadSignal* GetPad(Int_t padx, Int_t pady);

 Int_t GetLabelNr();
 HRichLabel* GetLabel(Int_t labelnr);

 Int_t GetRingNr();
 HRichHit* GetRing(Int_t ringnr);

 void SetActive();
 Int_t IsActive();
 Int_t IsOut(Int_t nowPad, Int_t dx = 0, Int_t dy = 0);
 Int_t IsOut(Int_t x, Int_t y, Int_t dx, Int_t dy); 
 
 HCategory* getCalCat() { return m_pCalCat; }
 HCategory* getHitCat() { return m_pHitCat; }
 HCategory* getHitHdrCat() { return m_pHitHdrCat; }
 void setCalCat(HCategory* pCalCat) { m_pCalCat = pCalCat; }
 void setHitCat(HCategory* pHitCat) { m_pHitCat = pHitCat; }
 void setHitHdrCat(HCategory* pHitHdrCat) { m_pHitHdrCat = pHitHdrCat; } 

 HRichAnalysisPar *getParams() { return ((HRichAnalysisPar*)fpAnalysisPar); }
 HParSet* getAnalysisPar() { return fpAnalysisPar; }
 HParSet* getGeomPar() { return fpGeomPar; }
 void setAnalysisPar(HParSet* pPar) { fpAnalysisPar = pPar; }
 void setGeomPar(HParSet* pPar) { fpGeomPar = pPar; }

 void calcLabCoordinates(HRichHit* hit, HLocation &fLoc);

private:

 void clear();
 void updateHeaders(Int_t nSec, Int_t nEvNr);
 void updateHits(Int_t nSec, Int_t nEvNr);
 
 Int_t getPadsIndex(Int_t nSec, Int_t nRow, Int_t nCol);
 Int_t* getPadsPointer(Int_t* pPads, Int_t nSec);


 ClassDef(HRichAnalysis,1)
};

//============================================================================

#endif // HRICHANALYSIS_H
