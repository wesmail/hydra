//*-- AUTHOR : B. Spruck
//*-- Modified : 19 Sep 2006

#ifndef HHYP_REMOVE_ALL_BUT_BEST_H
#define HHYP_REMOVE_ALL_BUT_BEST_H

#include <TObject.h>
#include <TArrayI.h>
#include "hhypbasealgorithm.h"

// ***************************************************************************

class HHypRemoveAllButBest:public HHypBaseAlgorithm
{

  // ***************************************************************************
private:

protected:

public:

  HHypRemoveAllButBest(char *name_i = NULL, Option_t par[] = NULL);

  virtual ~ HHypRemoveAllButBest(void);

  Bool_t          execute();
  Bool_t          init();
  Bool_t          reinit();
  Bool_t          finalize();

  ClassDef(HHypRemoveAllButBest, 0)// Removes all but one Combination based on Kine Chi2/Chi24
};

//============================================================================

#endif
