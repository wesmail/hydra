//*-- AUTHOR : B. Spruck
//*-- Modified : 19 Sep 2006

#ifndef HHYP_COPY_KINE_CHI2_TO_PROB_H
#define HHYP_COPY_KINE_CHI2_TO_PROB_H

#include <TObject.h>
#include <TArrayI.h>
#include "hhypbasealgorithm.h"

// ***************************************************************************

class HHypCopyKineChi2ToProb:public HHypBaseAlgorithm
{

  // ***************************************************************************
private:

protected:

public:

  HHypCopyKineChi2ToProb(char *name_i = NULL, Option_t par[] = NULL);

  virtual ~ HHypCopyKineChi2ToProb(void);

  Bool_t          execute();
  Bool_t          init();
  Bool_t          reinit();
  Bool_t          finalize();

  ClassDef(HHypCopyKineChi2ToProb, 0)// "Copy" Kine Chi24 to Probability
};

//============================================================================

#endif
