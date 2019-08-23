//*-- AUTHOR : Tiago Perez & Marcin Wisniowski
//*-- Modified : 20/04/2005 B. Spruck

#ifndef HHypListFillerInclusive_H
#define HHypListFillerInclusive_H

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

#define HYP_FILLINCL_LIMIT	20
class HHypListFillerInclusive:public HReconstructor
{
public:
  HHypListFillerInclusive();
  HHypListFillerInclusive(const Text_t * name,const Text_t * title);
  ~HHypListFillerInclusive();

  Bool_t          SetExitList(Int_t e_list);

  Bool_t          init(void);
  Int_t           execute(void);
  Bool_t          finalize(void);

  void            setContCatList(HCategory * pContCat) {m_pContCatList = pContCat;};
  void            setContCatComb(HCategory * pContCat) {m_pContCatComb = pContCat;};

  Bool_t          AddTrack(Int_t particle);


 private:
  Int_t           exitList;

  Int_t           nHadronPlus, nHadronMinus, nLeptonMinus, nLeptonPlus, numberOfParticles, nLepton;
  Int_t           nElectron, nPositron, nPosPion, nNegPion, nProton;
  Int_t           numberOfCombinations;

  Int_t          counterHadronPlus  ;
  Int_t          counterHadronMinus ;
  Int_t          counterLeptonPlus  ;
  Int_t          counterLeptonMinus ;
  Int_t          counterLepton  ;

  Int_t           PosHadronIdx[HYP_FILLINCL_LIMIT];
  Int_t           NegHadronIdx[HYP_FILLINCL_LIMIT];
  Int_t           PositronIdx[HYP_FILLINCL_LIMIT];
  Int_t           ElectronIdx[HYP_FILLINCL_LIMIT];
  Int_t           LeptonIdx[HYP_FILLINCL_LIMIT];

  TArrayI         particleArrayPlus, particleArrayMinus;
  HCategory      *m_pContCatList;       //!Pointer to the hit data category HHypList
  HCategory      *m_pContCatComb;       //!Pointer to the hit data category HHypComb
  HCategory      *m_pContCatPart;       //!Pointer to the hit data category HPidPArticle
  HIterator      *m_pContItPart;        //!Pointer to the hit data category HPidPArticle

  Bool_t          generateNumberOfCombinations();       // compute number of combinations
  void            create_pid_table(void);               // create permutations -> pid_full_array
  void            remove_fakes_pid_table(void);         // copy pid_full_array to pid_real_array, removing fakes
  void            fill_pid_table(HHypComb * hypcomb);   // copy pid_real_array to hypcomb
  Int_t           fill_pid_idx(HHypComb * hypcomb);     // fill pid idx into hypcomb
  Int_t           fill_pid_idx_double_part_first(HHypComb * hypcomb,
                             Int_t nTracks, Int_t iParPosition, Int_t iStartComb, Int_t Step, Int_t *pidIdx);
  Int_t           fill_pid_idx_double_part_secound(HHypComb * hypcomb,
                             Int_t nTracks, Int_t iParPosition, Int_t iStartComb, Int_t Step, Int_t *pidIdx);
  Int_t           fill_pid_idx_double_part(HHypComb * hypcomb,
                             Int_t nTracks, Int_t iParPosition, Int_t iStartComb, Int_t Step, Int_t *pidIdx);
  Int_t           fill_pid_idx_single_part(HHypComb * hypcomb, Int_t nTracks, Int_t iParPosition,
                             Int_t iStartComb, Int_t iNComb,Int_t *pidIdx );

  Bool_t          fill_pid_fprob(HHypComb * hypcomb,Int_t *numpidtracks);   // fill pid fprob into hypcomb

  Int_t           CalculateNComb(Int_t nTracksPlus,Int_t nPartPlus,Int_t nSamePartPlus);

  //      Permutating helper function
  //void            print_array(Int_t * array, Int_t count);// Debug output
  void            PermSwapper(Int_t * array, Int_t a, Int_t b);
  void            PermReverser(Int_t * array, Int_t a, Int_t b);
  Bool_t          FindNextPermutation(Int_t * array, Int_t count);

  ClassDef(HHypListFillerInclusive, 0)   // Fills the HypList with all possible particle combinations
};

#endif
