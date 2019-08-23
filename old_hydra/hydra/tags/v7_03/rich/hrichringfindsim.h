// File: hrichringfindsim.h
//
// Author: Laura Fabbietti <L.Fabbietti@physik.tu-muenchen.de>
// Last update by Laura Fabbietti: 00/05/03 14:02:24
// Updated Oct./Dec 2000 by Wolfgang Koenig
//
#ifndef HRICHRINGFINDSIM_H
#define HRICHRINGFINDSIM_H

#include "hrichringfind.h"
#include "hrichhitsim.h"
#include "hrichanalysissim.h"

class HRichAnalysis;
class HRichAnalysisSim;
class HRichRingFindSim: public HRichRingFind{
 public:
 HRichRingFindSim();
 virtual ~HRichRingFindSim();
 void LookForTrack(HRichAnalysisSim *showMe);
 HRichHitSim * pRings;
 Bool_t init(HRichAnalysisSim* showMe);
 Int_t Execute(HRichAnalysisSim *giveMe);

 ClassDef(HRichRingFindSim,1)
};

#endif /* !HRICHRINGFINDSIM_H */
