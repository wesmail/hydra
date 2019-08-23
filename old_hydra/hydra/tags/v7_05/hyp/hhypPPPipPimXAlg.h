/***************************************************************************
 *   Author :                                                              *
 *             II Physikalisches Institut                                  *
 *             Universitaet Giessen                                        *
 *            								   *
 *   Project:  HYDRA Event Hypothesys  (HADES)                             *
 *   Description:   						           *
 *            								   *
 *   Created  :   2005-01-05 Tiago Perez                                   *
 *   Modified :                                                            *
 *            								   *
 ***************************************************************************/
#ifndef HHYP_PP_PIP_PIM_X_H
#define HHYP_PP_PIP_PIM_X_H

#include <TObject.h>
#include <TArrayI.h>
#include "hhypbasealgorithm.h"
#include "hhyplistfiller.h"


// ***************************************************************************

class HHypPPPipPimXAlg : public HHypBaseAlgorithm
{

  // ***************************************************************************
 private:
  HHypListFiller * filler;

protected:

  

public:

  HHypPPPipPimXAlg(char *name_i=NULL); // This give warning on compilation
  
  virtual ~HHypPPPipPimXAlg(void);

  Bool_t execute();
  Bool_t init();
  Bool_t reinit();
  Bool_t finalize();
  
  ClassDef(HHypPPPipPimXAlg,0)
};

//============================================================================

#endif  

