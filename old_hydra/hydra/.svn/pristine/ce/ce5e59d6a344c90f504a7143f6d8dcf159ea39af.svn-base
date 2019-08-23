//*-- AUTHOR : B. Spruck
//*-- Modified : 20/4/2005
//*-- Modified : 21/nov/2005

#ifndef HHYP_PP_EP_EM_PROJECTOR_H
#define HHYP_PP_EP_EM_PROJECTOR_H

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

// ***************************************************************************

class HHypPPEpEmProjector:public HHypBaseAlgorithm
{

  // ***************************************************************************
private:

  TNtuple        *miss;// here we store everything
  Int_t           simuflag;
  HCategory      *simCat;
  HCategory      *CatPartSim;

  Bool_t nt_full_lorentz;// verbose flags
  Bool_t nt_dtof_refit;// verbose flags
  Bool_t nt_trigger;// verbose flags
  Bool_t nt_full_geant;// verbose flags
  
  // For counting Pid Fakes
  HIterator      *m_pContItPart;        //!Pointer to the hit data category HPidPArticle

protected:

public:

     
  HHypPPEpEmProjector(Char_t *name_i = NULL, Option_t par[] = NULL);
  
  virtual ~ HHypPPEpEmProjector(void);

  Bool_t          execute();
  Bool_t          init();
  Bool_t          reinit();
  Bool_t          finalize();

  ClassDef(HHypPPEpEmProjector, 0)      // ntuple creation for PPEmEp
};

//============================================================================

#endif
