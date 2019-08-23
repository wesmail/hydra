//*-- AUTHOR : I. Froehlich
//*-- Modified : 21/04/2005 by I. Froehlich
//*-- Modified : 21/nov/2005 by B. Spruck

#ifndef HHYP_PP_PIP_PIM_PROJECTOR_H
#define HHYP_PP_PIP_PIM_PROJECTOR_H

#include <TObject.h>
#include <TArrayI.h>
#include <TNtuple.h>
#include <TFile.h>
#include "hhypbasealgorithm.h"
#include <hcategory.h>
#include <hpidtrackcandsim.h>
#include <hpidgeanttrackset.h>
#include <hgeantkine.h>
#include <piddef.h>

// ***************************************************************************

class HHypPPPipPimProjector:public HHypBaseAlgorithm
{

  // ***************************************************************************
private:

  TNtuple        *miss;      // where we store the values
  Int_t           simuflag;  // if simulation
  HCategory      *simCat;
  HCategory      *pcatTrackCandSim;

  Bool_t nt_full_lorentz;// verbose flags
  Bool_t nt_dtof_refit;// verbose flags
  Bool_t nt_trigger;// verbose flags
  Bool_t nt_full_geant;// verbose flags

  // For counting Pid Fakes
  HIterator      *m_pContItPart;        //!Pointer to the hit data category HPidPArticle

protected:

public:
     
  HHypPPPipPimProjector(Char_t *name_i = NULL, Option_t par[] = NULL);
  
  virtual ~ HHypPPPipPimProjector(void);

  Bool_t          execute();
  Bool_t          init();
  Bool_t          reinit();
  Bool_t          finalize();

  ClassDef(HHypPPPipPimProjector, 0)    //Projects events to Ntuple
};

//============================================================================

#endif
