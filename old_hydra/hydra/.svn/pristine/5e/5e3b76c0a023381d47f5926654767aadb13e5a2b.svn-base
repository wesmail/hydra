//*-- AUTHOR :
//*-- Modified : 

#ifndef HHYP_MULTICHANNEL_P3N1_ALG_H
#define HHYP_MULTICHANNEL_P3N1_ALG_H

#include "TObject.h"
#include "TArrayI.h"
#include "hhypbasealgorithm.h"
#include "hhypmultichannelfiller.h"

class           HHypMultiChannelP3N1Alg:public HHypBaseAlgorithm
{

private:
  HHypMultiChannelFiller * filler;      // 4 Particles, no fakes

protected:

public:

  HHypMultiChannelP3N1Alg(Char_t *name_i = NULL, Option_t par[] = NULL);

                  virtual ~ HHypMultiChannelP3N1Alg(void);

  Bool_t          execute();
  Bool_t          init();
  Bool_t          reinit();
  Bool_t          finalize();

                  ClassDef(HHypMultiChannelP3N1Alg, 0)  // The wrapper for MC filling
};

//============================================================================

#endif
