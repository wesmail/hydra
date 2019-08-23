/////////////////////////////////////////////////////////// 
//
//     HHypList - container class for HADES EVENT HYPOTHESIS
//
//     Author: Marcin Wisniowski
//     Date:   09.02.2005
//////////////////////////////////////////////////////////////////////


#ifndef HHypListCat_H
#define HHypListCat_H

#include "hhypcomb.h"
#include "hhypkine.h"
#include <TROOT.h>
#include <TArrayF.h>
#include <TArrayI.h>
#include <TObject.h>
#include <hcategory.h>
#include <TLorentzVector.h>
#include <hpidparticle.h>
#include <hlinearcategory.h>
#include <hphysicsconstants.h>

class HHypList : public TObject{
public:
    HHypList() {}
    HHypList(HHypList *HypList, Int_t IdxHypKine=-1);
    HHypList(HHypComb *HypComb, Int_t IdxHypKine=-1);
    HHypList(Int_t Idx_HypComb, Int_t IdxHypKine=-1);
   ~HHypList() {}

    Int_t   getNcomb(); 
    Int_t   getNpart();
    Int_t   getPid(Int_t Icomb, Int_t Ipart);  
    Int_t   getIdxPidPart(Int_t Icomb, Int_t Ipart);  
    Float_t getProbComb(Int_t Icomb);
		
    TLorentzVector getTLorentzVector(Int_t Icomb, Int_t Ipart);
    TLorentzVector getTLorentzVector(char *,Int_t);

    Bool_t  setListId(Int_t Id_list);
    Int_t   getListId();

    Bool_t  setIdxHypComb(Int_t Idx);
    Int_t   getIdxHypComb();
		
    Bool_t  setIdxHypKine(Int_t Idx);
    Int_t   getIdxHypKine();
		
    Bool_t  setProbAlg(Int_t Icomb, Float_t Prob);
    Float_t getProbAlg(Int_t Icomb);
    Float_t getProbAlg(void);

    Bool_t  prepareHypList();
    Bool_t  print();

    static HCategory *buildLinearCat(Text_t *Calssname) ;

    Bool_t CombIteratorReset(void);
    Bool_t CombIterator(void);
    Bool_t getIterStatus(void);

private:
    Int_t     idx_hypkine;   // index of HHypKine container ( -1 -> don't use kine ) 
    Int_t     idx_hypcomb;   // index of HHypComb container  
    Int_t     id_list;       // channel list ID (see hypchannel)
    TArrayF   fprobalg;      // probability calculated by algorythm for given combination

    HHypComb *p_hypcomb;     //! pointer to HHypComb container (! -> don't strem p_hypcomb)
    Int_t     comb_iter;     //! for the iterator
    Bool_t    status;        //! exit status
    Bool_t    iterstatus;    //! exit status

    ClassDef(HHypList,1)     //ROOT extension  ( 1 -> streamed )
};

#endif
