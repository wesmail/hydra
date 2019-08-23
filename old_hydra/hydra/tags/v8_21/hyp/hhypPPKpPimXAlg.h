//*-- AUTHOR : Marco Destefanis
//*-- Modified : 22/07/2005 by Marco Destefanis

#ifndef HHYP_P_P_Kp_Pim_X_H
#define HHYP_P_P_Kp_Pim_X_H

#include "TObject.h"
#include "TArrayI.h"
#include "hhypbasealgorithm.h"
#include "hhyplistfiller.h"


// ***************************************************************************

class HHypPPKpPimXAlg : public HHypBaseAlgorithm
{

  // ***************************************************************************
 private:
  HHypListFiller * filler;

protected:

  

public:

  HHypPPKpPimXAlg(Char_t *name_i=NULL, Option_t par[]=NULL); 
  
  virtual ~HHypPPKpPimXAlg(void);

  Bool_t execute();
  Bool_t init();
  Bool_t reinit();
  Bool_t finalize();
  
  ClassDef(HHypPPKpPimXAlg,0)
};

//============================================================================

#endif  

