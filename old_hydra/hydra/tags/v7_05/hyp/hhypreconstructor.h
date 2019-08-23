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

#include <TObject.h>
#include <TClass.h>
#include <TFile.h>
#include <TArrayI.h>
#include "hades.h"
//#include "hmatrixcatiter.h"
#include "hypinfodef.h"
#include "hhypchannel.h"
#include "hhyplist.h"
#include "hhypcomb.h"
#include "hreconstructor.h"

#include "hhypbasealgorithm.h"
#include "hhypPPPipPimXAlg.h"
#include <iostream>
#include <iomanip>

class HCategory;
class HIterator; 
/* class HSpectrometer; */
/* class HRuntimeDb; */

// ***************************************************************************

#define   hHypMaxNumOfAlgos  50

class HHypReconstructor : public HReconstructor
{

    // ***************************************************************************
protected:

  // Variables Int
  Int_t numOfAlgorithms;
  HHypBaseAlgorithm *hHypAlgList[hHypMaxNumOfAlgos];
  TFile *histofiles[hHypMaxNumOfAlgos];
/*   Int_t init_Channel_Alg_id[hHypMaxNumOfAlgos]; */
/*   Int_t exit_Channel_Alg_id[hHypMaxNumOfAlgos]; */
/*   Int_t exit_idx[hHypMaxNumOfAlgos]; */
  // Matrix[hHypMaxNumOfAlgos][4]:: Alg,TheoInt,TheoOut,RealOut
  //                                                0  = no exec 
  //                                                -1 = exec NoOutput or Error 
  //                                                N  = Output Id (should be = TheoOut 
  //                                                -2 = No exec<= no input found 

  Int_t controlMatrix[hHypMaxNumOfAlgos][4];
  
  // Methods internal
  Bool_t executeInt();
  Bool_t orderBeforeExecute();

public:

    HHypReconstructor();
    ~HHypReconstructor();
    Bool_t AddAlgorithm(Int_t listIn, HHypBaseAlgorithm *hHypAlgoIn, Int_t listOut);
    Bool_t AddAlgorithm(Int_t listIn, HHypBaseAlgorithm *hHypAlgoIn, Int_t listOut, TFile * file);
    Int_t execute();
    Bool_t finalize();
    Bool_t init();
    Bool_t reinit();

    void SetBeam(TLorentzVector * mybeam) {beam=mybeam;};
    TLorentzVector * beam;  //!
    
    ClassDef(HHypReconstructor,0)
};

//============================================================================

#endif  // HHYPRECONSTRUCTOR_H

