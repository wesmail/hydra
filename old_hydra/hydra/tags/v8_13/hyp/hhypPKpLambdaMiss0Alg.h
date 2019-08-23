//*-- AUTHOR : Marco Destefanis
//*-- Modified : 26/07/2005 by Marco Destefanis



#ifndef HHYP_P_KP_LAMBDA_MISS0_H
#define HHYP_P_KP_LAMBDA_MISS0_H

#include <TObject.h>
#include <TArrayI.h>
#include "hhypbasealgorithm.h"
#include "hhyplistfiller.h"


// ***************************************************************************

class HHypPKpLambdaMiss0Alg : public HHypBaseAlgorithm
{

  // ***************************************************************************
 private:


protected:

  

public:

  HHypPKpLambdaMiss0Alg(char *name_i=NULL, Option_t par[]=NULL); 
  
  virtual ~HHypPKpLambdaMiss0Alg(void);

  Bool_t execute();
  Bool_t init();
  Bool_t reinit();
  Bool_t finalize();
  
  ClassDef(HHypPKpLambdaMiss0Alg,0)
};

//============================================================================

#endif  

