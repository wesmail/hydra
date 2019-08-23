//*-- AUTHOR : Tiago Perez & Marcin Wisniowski
//*-- Modified : 20/04/2005 B. Spruck
//*-- Modified : 15.04.2008 B. Spruck
//*-- Modified : 15.07.2008 B. Spruck

#ifndef HHypListFiller_H
#define HHypListFiller_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hhyplist.h"
#include "TArrayI.h"
#include "TNtuple.h"
#include "hevent.h"
#include "heventheader.h"
#include "hdetector.h"
#include "hratreeext.h"
#include "hmatrixcategory.h"
#include "hlinearcategory.h"
#include "hlinearcatiter.h"
#include "hlocation.h"
#include "hiterator.h"

#include "hcategory.h"
#include "hdebug.h"
#include "hades.h"
#include "hypinfodef.h"
#include "hpidphysicsconstants.h"

#include <cmath>
#include <iostream>

#define HYP_MAX_TRACKCLEAN	1000

class HHypListFiller:public HReconstructor
{
public:
  HHypListFiller();
  HHypListFiller(Text_t * name, Text_t * title);
  ~HHypListFiller();

  Bool_t          SetExitList(Int_t e_list);

  Bool_t          init(void);
  Int_t           execute(void);
  Bool_t          finalize(void);

  void            setContCatList(HCategory * pContCat) {m_pContCatList = pContCat;};
  void            setContCatComb(HCategory * pContCat) {m_pContCatComb = pContCat;};

  Bool_t          AddTrack(Int_t particle);
  Bool_t          AddOneMissing(Int_t particle);

  // introduced only for debugging
#if 1
  inline Int_t getNumberOfCombinations(void){ return(numberOfCombinations);};
  inline Int_t getNumberOfParticles(void){ return(numberOfParticles);};
  void            just_a_test_for_debugging(void);
#endif
  // end debugging

	Bool_t is_valid[HYP_MAX_TRACKCLEAN];

private:
  Int_t           exitList;
  Int_t           nPlus, nMinus, numberOfParticles;// Total Number of Particles including Fakes
  Int_t           nPlusReal, nMinusReal, numberOfRealParticles;// Number of REAL Particles (without Fakes)
  Int_t           numberOfCombinations, numberOfNegCombinations,
    numberOfPosCombinations;
  Int_t           numberOfPossibleMissing;
  Int_t         **pid_real_array;// only real particles
  Int_t         **pid_full_array;// real and fake particles

  TArrayI         particleArrayPlus, particleArrayMinus, particleArrayMissing;
  HCategory      *m_pContCatList;       //! Pointer to the hit data category HHypList
  HCategory      *m_pContCatComb;       //! Pointer to the hit data category HHypComb
  HCategory      *m_pContCatTrackCand;  //! Pointer to the hit data category HPidTrackCand
  HIterator      *m_pContItTrackCand;   //! Iterator for category HPidTrackCand

  Bool_t          generateNumberOfCombinations();       // compute number of combinations
  void            create_pid_table(void);               // create permutations -> pid_full_array
  void            remove_fakes_pid_table(void);         // copy pid_full_array to pid_real_array, removing fakes
  void            fill_pid_table(HHypComb * hypcomb);   // copy pid_real_array to hypcomb
  void            fill_pid_idx(HHypComb * hypcomb);     // fill pid idx into hypcomb
  Bool_t          fill_pid_fprob(HHypComb * hypcomb,Int_t *numpidtracks);   // fill pid fprob into hypcomb


  //      Permutating helper function
  //void            print_array(Int_t * array, Int_t count);// Debug output
  void            PermSwapper(Int_t * array, Int_t a, Int_t b);
  void            PermReverser(Int_t * array, Int_t a, Int_t b);
  Bool_t          FindNextPermutation(Int_t * array, Int_t count);

  ClassDef(HHypListFiller, 0)   // Fills the HypList with all possible particle combinations
};

#endif
