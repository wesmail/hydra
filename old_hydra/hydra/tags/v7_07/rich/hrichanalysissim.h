// File: hrichanalysissim.h
//
// Author: Laura Fabbietti <L.Fabbietti@physik.tu-muenchen.de>
// Last update by Laura Fabbietti: 01/09/12 15:36:34
//
#ifndef HRICHANALYSISSIM_H
#define HRICHANALYSISSIM_H

#include "hrichanalysis.h"
#include "hrichringfindsim.h"
#include "hrichhitsim.h"
#include "hrichcalsim.h"

class HRichRingFindSim;
class HCategory;
class HRichAnalysisSim: public HRichAnalysis  {
public:
 HRichAnalysisSim(void);
 HRichAnalysisSim(Text_t *name, Text_t *title, Bool_t kSkip=kFALSE);
 ~HRichAnalysisSim(void); 
 Int_t getPadsNTrack1(Int_t padx, Int_t pady, Int_t sec);
 Int_t getPadsNTrack2();
 Int_t getTrack(Int_t index);
 Int_t getFlag(Int_t index);
 void updateHits(Int_t nSec);
 Bool_t init(void);
 Bool_t reinit(void);
 Bool_t initParameters(void);
 Int_t execute();  
 HRichHitSim *pRings;
 Int_t cont;
 Int_t secCount;

private:
    Bool_t  kSkipEvtIfNoRing;
   HRichRingFindSim *pRingFindSim;
   HCategory * m_pTrackCat;//Pointer to the track container
   HRichCalSim * pRichCalSim;
 ClassDef(HRichAnalysisSim,0)
};

#endif /* !HRICHANALYSISSIM_H */

