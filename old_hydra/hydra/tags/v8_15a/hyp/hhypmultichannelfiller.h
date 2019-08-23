//*-- AUTHOR :
//*-- Modified :

#ifndef HHypMultiChannelFiller_H
#define HHypMultiChannelFiller_H

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

class           HHypMultiChannelFiller:public HReconstructor
{
public:
  HHypMultiChannelFiller();
  HHypMultiChannelFiller(Text_t * name, Text_t * title);
  ~HHypMultiChannelFiller();

  Bool_t          SetExitList(Int_t e_list);

  Bool_t          init(void);
  Int_t           execute(void);
  Bool_t          finalize(void);

  void            setContCatList(HCategory * pContCat)
  {
    m_pContCatList = pContCat;
  };
  void            setContCatComb(HCategory * pContCat)
  {
    m_pContCatComb = pContCat;
  };
  void            setNPlus(Int_t np)
  {
    nPlus = np;
  };
  void            setNMinus(Int_t nm)
  {
    nMinus = nm;
  };

private:
  Int_t exitList;
  Int_t           nPlus, nMinus, numberOfParticles;     // Total Number of Particles

  Int_t           numberOfCombinations, numberOfNegCombinations,
    numberOfPosCombinations;
  Int_t         **pid_real_array;       // only real particles
  Int_t          *missing_array;        // missing particles

  TArrayI         particleArrayPlus, particleArrayMinus;
  HCategory      *m_pContCatList;       //!Pointer to the hit data category HHypList
  HCategory      *m_pContCatComb;       //!Pointer to the hit data category HHypComb
  HCategory      *m_pContCatPart;       //!Pointer to the hit data category HPidPArticle
  HIterator      *m_pContItPart;        //!Pointer to the hit data category HPidPArticle

  Bool_t          generateNumberOfCombinations();       // compute number of combinations
  void            create_pid_table(void);       // create permutations -> pid_full_array

  void            fill_pid_table(HHypComb * hypcomb);   // copy pid_real_array to hypcomb
  void            fill_pid_idx(HHypComb * hypcomb);     // fill pid idx into hypcomb
  Bool_t          fill_pid_fprob(HHypComb * hypcomb, Int_t * numpidtracks);     // fill pid fprob into hypcomb

  //Alexander: add helper function as needed

  ClassDef(HHypMultiChannelFiller, 0)   // Fills the HypMultiChannel with all possible particle combinations
};

#endif
