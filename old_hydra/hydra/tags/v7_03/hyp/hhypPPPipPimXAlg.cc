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
#include "hhypPPPipPimXAlg.h"
#include "hypinfodef.h"

ClassImp(HHypPPPipPimXAlg)
  
  HHypPPPipPimXAlg::HHypPPPipPimXAlg(char *name_i) 
{
  algoName = name_i;
  filler = NULL;
}


HHypPPPipPimXAlg::~HHypPPPipPimXAlg()
{
  if (filler) delete filler;
}


Bool_t HHypPPPipPimXAlg::execute()
{
  base_execute();

  exitIdx = filler->execute();
 
  cout << algoName << ":" << exitIdx << endl;

  if (exitIdx > -1) return kTRUE;
  return kFALSE;
}

Bool_t HHypPPPipPimXAlg::init()
{
  base_init();

  filler = new HHypListFiller("aaa","bbb");
  filler->SetExitList(exitList);
 
  /* needed: say filler which particles I want to have */

  return filler->init();
}

Bool_t HHypPPPipPimXAlg::reinit()
{
  return kTRUE;
}

Bool_t HHypPPPipPimXAlg::finalize()
{
  return kTRUE;
}
