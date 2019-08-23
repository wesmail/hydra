/***************************************************************************
 * description see .h                                                      *
 * THIS IMPLEMENTATION IS A TEMPORARY SOLUTION                             *
 ***************************************************************************/

using namespace std;
#include <iostream>
#include "hhypchannel.h"

ClassImp(HHypChannel)

HHypChannel::HHypChannel(void) 
{}

HHypChannel::~HHypChannel(void) 
{}

Int_t HHypChannel::Get (char * name) {
  if (strcmp(name,"empty") == 0)            return -1;
  if (strcmp(name,"ntuple") == 0)           return -1;
  /* negative number means no input needed */

  if (strcmp(name,"dummy1") == 0)           return 1;
  if (strcmp(name,"dummy2") == 0)           return 2;
  if (strcmp(name,"dummy3") == 0)           return 3;
  if (strcmp(name,"dummy4") == 0)           return 4;
  if (strcmp(name,"dummy5") == 0)           return 5;
  if (strcmp(name,"dummy6") == 0)           return 6;
  if (strcmp(name,"dummy7") == 0)           return 7;
  if (strcmp(name,"dummy8") == 0)           return 8;
  if (strcmp(name,"dummy9") == 0)           return 9;

  /* channel definitions */
  /* spaces are for versions */

  /**************************
   * pion production
   **************************/

  /* semi-inclusive 2 pion production in pp reaction */
  if (strcmp(name,"pp_pip_pim_X") == 0)     return 16 * 10;

  /* exclusive 3 pion production in pp reaction */
  if (strcmp(name,"pp_pip_pim_pi0") == 0)   return 16 * 11;

  /* exclusive 2 pion production in pp reaction */
  if (strcmp(name,"pp_pip_pim") == 0)       return 16 * 12;

  /**************************
   * dilepton production
   **************************/

  /* semi-inclusive dilepton production in pp reaction */
  if (strcmp(name,"pp_ep_em_X") == 0)       return 16 * 20;

  /* exclusive dalitz decay in pp reaction */
  if (strcmp(name,"pp_ep_em_g") == 0)       return 16 * 21;

  /* exclusive dilepton in pp reaction */
  if (strcmp(name,"pp_ep_em") == 0)         return 16 * 22;


  /* numbers > 160000 are reserved for dynamic allocation */

  cout << "WARNING: Dynamic allocation for channels is not yet supported" << endl;
  return -2;

}

char * HHypChannel::Get (Int_t num) {

  if (num == -1) return "empty";

  if (num == 16 * 10) return "pp_pip_pim_X";
  if (num == 16 * 11) return "pp_pip_pim_pi0";
  if (num == 16 * 12) return "pp_pip_pim";
  return "unknown";
}
