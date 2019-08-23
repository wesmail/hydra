//*-- AUTHOR : B. Spruck
//*-- Modified : 20/4/2005
//*-- Modified : 22 Dec 2005

#ifndef HHYP_PP_PROJECTOR_H
#define HHYP_PP_PROJECTOR_H

#include <TObject.h>
#include <TArrayI.h>
#include <TNtuple.h>
#include <TFile.h>
#include "hhypbasealgorithm.h"
#include <hcategory.h>
#include <hpidparticlesim.h>
#include <hpidgeanttrackset.h>
#include <hgeantkine.h>
#include <piddef.h>

class HHypPPProjector:public HHypBaseAlgorithm
{

private:

  TNtuple        *miss;
  TNtuple        *header;    // where we store header info like downscaling...
  Int_t           simuflag;
  HCategory      *simCat;
  HCategory      *CatPartSim;

  // For counting Pid Fakes
  HIterator      *m_pContItPart;        //!Pointer to the hit data category HPidPArticle

protected:

public:

  HHypPPProjector(char *name_i = NULL, Option_t par[] = NULL);
  
  virtual ~ HHypPPProjector(void);

  Bool_t          execute();
  Bool_t          init();
  Bool_t          reinit();
  Bool_t          finalize();

  ClassDef(HHypPPProjector, 0)  // creates ntuples for PP reactions
};

//============================================================================

#endif
