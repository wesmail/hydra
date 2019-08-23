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
#include "hhypPPEpEmXAlg.h"
#include "hypinfodef.h"

ClassImp(HHypPPEpEmXAlg)
  
  HHypPPEpEmXAlg::HHypPPEpEmXAlg(char *name_i) 
{
  algoName = name_i;
  filler = NULL;
}


HHypPPEpEmXAlg::~HHypPPEpEmXAlg()
{
  if (filler) delete filler;
}


Bool_t HHypPPEpEmXAlg::execute()
{
  base_execute();

  exitIdx = filler->execute();
 
  cout << algoName << ":" << exitIdx << endl;

  if (exitIdx > -1) return kTRUE;
  return kFALSE;
}

Bool_t HHypPPEpEmXAlg::init()
{
  base_init();

  filler = new HHypListFiller("aaa","bbb");
  filler->SetExitList(exitList);
 
  /* needed: say filler which particles I want to have */

  return filler->init();
}

Bool_t HHypPPEpEmXAlg::reinit()
{
  return kTRUE;
}

Bool_t HHypPPEpEmXAlg::finalize()
{
  return kTRUE;
}
