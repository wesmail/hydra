//*-- AUTHOR : Tiago Perez & Marcin Wisniowski
//*-- Modified : 20/04/2005 B. Spruck

#ifndef HHypListFiller_H
#define HHypListFiller_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hhyplist.h"
#include "TArrayI.h"
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

  // introduced only for debugging
//      inline Int_t getNumberOfCombinations(void){ return(numberOfCombinations);};
//      inline Int_t getNumberOfParticles(void){ return(numberOfParticles);};
//  void            just_a_test_for_debugging(void);

  // end debugging

private:
  Int_t           exitList;
  Int_t           nPlus, nMinus, numberOfParticles;     //particleArrayIndex,
  Int_t           numberOfCombinations, numberOfNegCombinations,
    numberOfPosCombinations;
  Int_t         **pid_array;

  TArrayI         particleArrayPlus, particleArrayMinus;
  HCategory      *m_pContCatList;       //!Pointer to the hit data category HHypList
  HCategory      *m_pContCatComb;       //!Pointer to the hit data category HHypComb
  HCategory      *m_pContCatPart;       //!Pointer to the hit data category HPidPArticle
  HIterator      *m_pContItPart;        //!Pointer to the hit data category HPidPArticle

  Bool_t          generateNumberOfCombinations();       // compute number of combinations
  void            create_pid_table(void);               // create permutations -> pid_array
  void            fill_pid_table(HHypComb * hypcomb);   // copy pid_array to hypcomb

  //      Permutating helper function
  void            print_array(Int_t * array, Int_t count);
  void            PermSwapper(Int_t * array, Int_t a, Int_t b);
  void            PermReverser(Int_t * array, Int_t a, Int_t b);
  Bool_t          FindNextPermutation(Int_t * array, Int_t count);

  ClassDef(HHypListFiller, 0)   // Fills the HypList with all possible particle combinations
};

#endif
