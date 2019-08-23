//*-- AUTHOR : B. Spruck
//*-- Modified : 24 Apr 2008

#ifndef HHYP_USERFUNC_H
#define HHYP_USERFUNC_H

#include <TObject.h>
#include <TArrayI.h>
#include "hhypbasealgorithm.h"
#include "hhyplist.h"

// ***************************************************************************

class HHypUserFunc:public HHypBaseAlgorithm
{

  // ***************************************************************************
private:
	void (*userfunc)(HHypList *l,TLorentzVector *beam);

protected:

public:

  HHypUserFunc(Char_t *name_i = NULL, Option_t par[] = NULL);

  virtual ~ HHypUserFunc(void);

	void SetUserFunc(void (*uf)(HHypList *l,TLorentzVector *beam)){userfunc=uf;};

  Bool_t          execute();
  Bool_t          init();
  Bool_t          reinit();
  Bool_t          finalize();

  ClassDef(HHypUserFunc, 0)// call Userfunc for every Combination
};

//============================================================================

#endif
