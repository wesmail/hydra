//////////////////////////////////////////////////////////////////////////////
//
// File: hrichanalysissim.h
//
// $Id: hrichanalysissim.h,v 1.7 2009-04-09 16:26:28 jurkovic Exp $
//
//*-- Author: Laura Fabbietti <L.Fabbietti@physik.tu-muenchen.de>
//*-- Last update by Laura Fabbietti: 01/09/12 15:36:34
//
//////////////////////////////////////////////////////////////////////////////


#ifndef HRICHANALYSISSIM_H
#define HRICHANALYSISSIM_H

#include "hrichanalysis.h"

class HCategory;
class HRichCalSim;
class HRichHitSim;
class HRichRingFindSim;


class HRichAnalysisSim: public HRichAnalysis
{
 public:
  HRichAnalysisSim(void);
  HRichAnalysisSim(Text_t *name, Text_t *title, Bool_t kSkip = kFALSE);
  ~HRichAnalysisSim(void); 

  Bool_t init(void);
  Bool_t reinit(void);
  Int_t  execute();  

  Int_t  getPadsNTrack1(Int_t padx, Int_t pady, Int_t sec);
  Int_t  getPadsNTrack2(void);
  Int_t  getTrack(Int_t index);
  Int_t  getFlag(Int_t index);

  HRichHitSim *pRings;

 private:
  void   sortTracks(HRichHitSim *);
  void   updateHits(Int_t nSec);
  Bool_t initParameters(void);

  Bool_t kSkipEvtIfNoRing;

  HRichRingFindSim*  pRingFindSim;  // Pointer to ringfinder
  HCategory*         m_pTrackCat;   // Pointer to the track container
  HRichCalSim*       pRichCalSim;   // Pointer to cal container

  ClassDef(HRichAnalysisSim, 0)

};

#endif /* !HRICHANALYSISSIM_H */

