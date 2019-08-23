//*-- AUTHOR : B. Spruck
//*-- Modified : 20/4/2005

#ifndef HHYP_PP_ELASTIC_H
#define HHYP_PP_ELASTIC_H

#include "TObject.h"
#include "TArrayI.h"
#include "hhypbasealgorithm.h"
#include "hhyplistfiller.h"

class HHypPPElasticAlg:public HHypBaseAlgorithm
{

private:

protected:

public:

  HHypPPElasticAlg(Char_t *name_i = NULL, Option_t par[] = NULL);

  virtual ~ HHypPPElasticAlg(void);

  Bool_t          execute();
  Bool_t          init();
  Bool_t          reinit();
  Bool_t          finalize();

  ClassDef(HHypPPElasticAlg, 0) //  PP elastic selection
};

//============================================================================

#endif
