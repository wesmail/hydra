// File: hrichanalysissim.h
//
// Author: Laura Fabbietti <L.Fabbietti@physik.tu-muenchen.de>
// Last update by Laura Fabbietti: 00/05/07 19:29:40
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
 HRichAnalysisSim(Text_t *name, Text_t *title);
 ~HRichAnalysisSim(void); 
 Int_t getPadsNTrack1(Int_t padx, Int_t pady, Int_t sec);
 Int_t getPadsNTrack2();
 Int_t getTrack(Int_t index);
 Int_t getFlag(Int_t index);
 void updateHits(Int_t nSec);
 Bool_t init(void);
 Int_t execute();  
 HRichHitSim *pRings;
 Int_t cont;
 Int_t secCount;

private:
   HRichRingFindSim *pRingFind;
   HCategory * m_pTrackCat;//Pointer to the track container
   HRichCalSim * pRichCalSim;
 ClassDef(HRichAnalysisSim,0)
};

#endif /* !HRICHANALYSISSIM_H */

