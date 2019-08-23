//////////////////////////////////////////////////////////////////////////////
//
// File: hrichringfindsim.h
//
// $Id: hrichringfindsim.h,v 1.4 2009-01-28 12:26:26 jurkovic Exp $
//
//*-- Author: Laura Fabbietti <L.Fabbietti@physik.tu-muenchen.de>
//*-- Last update by Laura Fabbietti: 00/05/03 14:02:24
//*-- Updated Oct./Dec 2000 by Wolfgang Koenig
//
//////////////////////////////////////////////////////////////////////////////


#ifndef HRICHRINGFINDSIM_H
#define HRICHRINGFINDSIM_H

#include "hrichringfind.h"

class HRichAnalysisSim;
class HRichHitSim;


class HRichRingFindSim: public HRichRingFind
{

 public:
  HRichRingFindSim();
  virtual ~HRichRingFindSim();

  Bool_t init(HRichAnalysisSim* showMe);
  Int_t  Execute(HRichAnalysisSim *giveMe);

 private:
  void LookForTrack(HRichAnalysisSim *showMe);

  HRichHitSim* pRings;

  ClassDef(HRichRingFindSim, 0)

};

#endif /* !HRICHRINGFINDSIM_H */
