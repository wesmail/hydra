/***************************************************************************
 *   Author :  Tiago Perez <tiago.perez@exp2.physik.uni-giessen.de>        *
 *             II Physikalisches Institut                                  *
 *             Universitaet Giessen                                        *
 *            								   *
 *   Title  :  HHypReconstructor.h                                         *
 *   Project:  HYDRA Event Hypothesys  (HADES)                             *
 *   Description:   						           *
 *            								   *
 *   Modified :  2005-01-04 Created: Tiago Perez                           *
 *            								   *
 ***************************************************************************/

#ifndef HHYPRECONSTRUCTOR_H
#define HHYPRECONSTRUCTOR_H

#include "TObject.h"
#include "TClass.h"
#include "TFile.h"
#include "TArrayI.h"
#include "hades.h"
//#include "hmatrixcatiter.h"
#include "hypinfodef.h"
#include "hhypchannel.h"
#include "hhyplist.h"
#include "hhypcomb.h"
#include "hreconstructor.h"

#include "hhypbasealgorithm.h"
#include "hhypQA.h"


class           HCategory;
class           HIterator;

/* class HSpectrometer; */
/* class HRuntimeDb; */

// ***************************************************************************

#define   hHypMaxNumOfAlgos  150

class           HHypReconstructor:public HReconstructor
{

  // ***************************************************************************
protected:

  Int_t numOfAlgorithms;
  HHypBaseAlgorithm *hHypAlgList[hHypMaxNumOfAlgos];
  TFile          *histofiles[hHypMaxNumOfAlgos];
  HHypChannel     channel;      //official AND private channel list

  Bool_t          skip_event_enable;    //If this is true, EMPTY hyp events are skipped
  Int_t           num_accepted_events;  //! How many events have been selected by SKIPEMPTY option

  Int_t           controlMatrix[hHypMaxNumOfAlgos][5];
  //meaning of control matrix
  //field 0: number of algorithm
  //      1: Input channel number
  //      2: output channel number
  //      3: output index (after execution)
  //      4: from_tree/current_event

  HCategory      *m_pContCatComb, *m_pContCatList, *m_pContCatKine;
  HHypQA         *hypqa;

  HHypRecPar     *pParams;      // pointer to container with params

  HIterator      *pitHypList;

  // Methods internal

  Bool_t          orderBeforeExecute();
  Bool_t          has_build_cat;

  void            Dump(Int_t, TString); //Dumps ana tree

public:

                  HHypReconstructor(const Text_t * name,const Text_t * title, TString opt);
                  HHypReconstructor();
                 ~HHypReconstructor();

  Bool_t          AddAlgorithm(const Char_t *in, HHypBaseAlgorithm * hHypAlgoIn,
                               const Char_t *out);
  Bool_t          AddAlgorithm(const Char_t *in, HHypBaseAlgorithm * hHypAlgoIn,
                               const Char_t *out, TFile * file);

  Bool_t          AddQA(TFile * file, const Option_t par[] = NULL);

  Int_t           execute();
  Bool_t          finalize();
  Bool_t          init();
  Bool_t          reinit();

  void            SetBeam(TLorentzVector * mybeam) {beam = mybeam;};
  void            SetTargetPID(Int_t target){ target_pid=target;};
  void            SetBeamPID(Int_t projectile){ beam_pid=projectile;};

  TLorentzVector *beam;         //!
  Int_t beam_pid, target_pid;   //!

  ClassDef(HHypReconstructor, 0)        // The HYP reconstructor (which calls the Algo's)
};

//============================================================================

#endif // HHYPRECONSTRUCTOR_H
