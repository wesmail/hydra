/***************************************************************************
 *   Author :                                                              *
 *             II Physikalisches Institut                                  *
 *             Universitaet Giessen                                        *
 *           								   *
 *   Project:  HYDRA Event Hypothesys  (HADES)                             *
 *   Description:   						           *
 *            								   *
 *   Created  :   2005-01-05 Tiago Perez                                   *
 *   Modified :                                                            *
 *            								   *
 ***************************************************************************/

using namespace std;
#include <stdlib.h>
#include <iostream>
#include "hhypPPPipPimPi0Alg.h"
#include "hypinfodef.h"

ClassImp(HHypPPPipPimPi0Alg)
  
  HHypPPPipPimPi0Alg::HHypPPPipPimPi0Alg(char *name_i) 
{
  algoName = name_i;
  filler = NULL;
}


HHypPPPipPimPi0Alg::~HHypPPPipPimPi0Alg()
{
  if (filler) delete filler;
}


Bool_t HHypPPPipPimPi0Alg::execute()
{
  base_execute();

  // can modify something here...
  //mylist->...

  //  cout << algoName << ":" << exitIdx << endl;

  if (exitIdx > -1) return kTRUE;
  return kFALSE;
}

Bool_t HHypPPPipPimPi0Alg::init()
{
  base_init();

  /* needed: say filler which particles I want to have */

  return kTRUE;
}

Bool_t HHypPPPipPimPi0Alg::reinit()
{
  return kTRUE;
}

Bool_t HHypPPPipPimPi0Alg::finalize()
{
  return kTRUE;
}
