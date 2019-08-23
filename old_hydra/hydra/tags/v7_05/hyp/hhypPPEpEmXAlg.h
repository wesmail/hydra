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
#ifndef HHYP_PP_EP_EM_X_H
#define HHYP_PP_EP_EM_X_H

#include <TObject.h>
#include <TArrayI.h>
#include "hhypbasealgorithm.h"
#include "hhyplistfiller.h"


// ***************************************************************************

class HHypPPEpEmXAlg : public HHypBaseAlgorithm
{

  // ***************************************************************************
 private:
  HHypListFiller * filler;

protected:

  

public:

  HHypPPEpEmXAlg(char *name_i=NULL); // This give warning on compilation
  
  virtual ~HHypPPEpEmXAlg(void);

  Bool_t execute();
  Bool_t init();
  Bool_t reinit();
  Bool_t finalize();
  
  ClassDef(HHypPPEpEmXAlg,0)
};

//============================================================================

#endif  

