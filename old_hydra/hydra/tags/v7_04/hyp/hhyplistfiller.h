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

class HHypListFiller : public HReconstructor {
 public:
  HHypListFiller();
  HHypListFiller(Text_t *name,Text_t *title);
  ~HHypListFiller();

  Bool_t SetExitList(Int_t e_list);

  Bool_t init(void);
  Int_t  execute(void);
  Bool_t finalize(void);
		
  void setContCatList(HCategory* pContCat){m_pContCatList = pContCat;}
  void setContCatComb(HCategory* pContCat){m_pContCatComb = pContCat;}

  Bool_t AddTrack(Int_t particle);

 private:
  Int_t exitList;
  Int_t  nPlus, nMinus, numberOfParticles; //particleArrayIndex,
  Int_t numberOfCombinations;
  Int_t CombMatrix[6][3][1];
  //Ex::
  //  _|  0     |  1     |  2     |   3   |
  // 0_ pi+/Prob p/Prob   p/prob   pi-/prob  
  // 1_ p/Prob   pi+/Prob p/prob   pi-/prob  
  // 2_ p/Prob   p/Prob   pi+/prob pi-/prob
  //
  //  TArrayI particleArray;     // Array of particles INTEGERS
  TArrayI particleArrayPlus, particleArrayMinus;
  HCategory *m_pContCatList; //!Pointer to the hit data category HHypList
  HCategory *m_pContCatComb; //!Pointer to the hit data category HHypComb
  HCategory *m_pContCatPart; //!Pointer to the hit data category HPidPArticle
  HIterator *m_pContItPart; //!Pointer to the hit data category HPidPArticle

  Bool_t generateNumberOfCombinations();
  Bool_t generateMatrix();

  ClassDef(HHypListFiller,0) //ROOT extension
    };

#endif
