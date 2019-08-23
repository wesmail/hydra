//*-- AUTHOR : B. Spruck
//*-- Modified : 24 Apr 2008

#ifndef HHYP_FWVARS_H
#define HHYP_FWVARS_H

#include "TObject.h"
#include "TArrayI.h"
#include "hhypbasealgorithm.h"
#include "hwallhit.h"
#include "hhyplist.h"

// ***************************************************************************

class HHypFWVars:public HHypBaseAlgorithm
{

  // ***************************************************************************
private:
    HIterator  *m_pContItWallHit;
    HCategory  *m_pContCatWallHit;


protected:

public:

  HHypFWVars(Char_t *name_i = NULL, Option_t par[] = NULL);

  virtual ~ HHypFWVars(void);

  Bool_t          execute();
  Bool_t          init();
  Bool_t          reinit();
  Bool_t          finalize();

  ClassDef(HHypFWVars, 0)// Adding FW variables for every Combination
};

//============================================================================

#endif
