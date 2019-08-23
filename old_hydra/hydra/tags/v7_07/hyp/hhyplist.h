//*-- AUTHOR : Marcin Wisniowski 
//*-- Modified : 09.02.2005 by Marcin Wisniowski 
//*-- Modified : 21/04/2005 by I. Froehlich

#ifndef HHypListCat_H
#define HHypListCat_H

#include "hhypcomb.h"
#include "hhypkine.h"
#include "hypinfodef.h"

#include <TROOT.h>
#include <TArrayF.h>
#include <TArrayI.h>
#include <TObject.h>
#include <hcategory.h>
#include <TLorentzVector.h>
#include <hpidparticle.h>
#include <hlinearcategory.h>
#include <hphysicsconstants.h>

class HHypList:public TObject
{
public:
  HHypList ()     {};

  HHypList (HHypList * HypList, Int_t IdxHypKine = -1);
  HHypList (HHypComb * HypComb, Int_t IdxHypKine = -1);
  HHypList (Int_t Idx_HypComb, Int_t IdxHypKine = -1);
  ~HHypList ()    {};

  void            Restore (void);

  Int_t           getNcomb ();
  Int_t           getNpart ();
  Int_t           getPid (Int_t Icomb, Int_t Ipart);
  Int_t           getPid (Int_t Ipart);
  Int_t           getIdxPidPart (Int_t Icomb, Int_t Ipart);
  Int_t           getIdxPidPart (Int_t Ipart);
  Int_t           getIdxPidPart (char *, Int_t);
  Float_t         getProbComb (Int_t Icomb);

  TLorentzVector  getTLorentzVector (Int_t Icomb, Int_t Ipart);
  TLorentzVector  getTLorentzVector (char *, Int_t);
  TLorentzVector  getTLorentzVector (Int_t Ipart);

  TVector3        getMomentum (Int_t Ipart);
  TVector3        getMomentum (Int_t Icomb, Int_t Ipart);

  Bool_t          setMomentum (Int_t Icomb, Int_t Ipart, TVector3 vect);
  Bool_t          setMomentum (Int_t Ipart, TVector3 vect);

  HPidParticle   *getPidParticle (Int_t Ipart);

  Bool_t          setListId (Int_t Id_list);
  Int_t           getListId ();

  Bool_t          setIdxHypComb (Int_t Idx);
  Int_t           getIdxHypComb ();

  Bool_t          setIdxHypKine (Int_t Idx);
  Int_t           getIdxHypKine ();

  Bool_t          setProbAlg (Int_t Icomb, Float_t Prob);
  Float_t         getProbAlg (Int_t Icomb);
  Float_t         getProbAlg (void);
  Int_t           getOrder   (void);  //returns the order number of current comb. (starting from 1...)
  Bool_t          resetProbAlg (Float_t Prob);	// Update the probability inside the algorithm
  void            removeComb (void);
  void            KeepBestComb (void);

  void            setUserValue (Int_t valueID, Float_t value, Int_t my_iter = -1);	//This allows to store any user info
  Bool_t          writeUserValues (void);	//copy user values from cache table to final array, should be called after execute() of each alg.
  Bool_t          getUserValue (Int_t valueID, Float_t &value, Int_t my_iter = -1);	//get user value back
  HHypList       *getParent ();	//gets parent object (or NULL...)

  Bool_t          prepareHypList ();
  Bool_t          print ();

  static HCategory *buildLinearCat (Text_t * Calssname);

  Bool_t          CombIteratorReset (void);
  Bool_t          CombIterator (void);
  Bool_t          getIterStatus (void);

private:
  Int_t idx_hypkine;		// index of HHypKine container ( -1 -> don't use kine ) 
  Int_t           idx_hypcomb;	// index of HHypComb container  
  Int_t           idx_hyplist;	// index of "parent" HHypList container ( -1 -> no parent ) 
  Int_t           id_list;	// channel list ID (see hypchannel)
  TArrayF         fprobalg;	// probability calculated by algorythm for given combination
  TArrayF         fvaluealg;	// storing USER defined values  (up to nvaluealg)
  TArrayI         fuseralg;	// storing USER defined values: row0->ncomb, row1->valueID  (up to 2*nvaluealg)
  Int_t           nvaluealg;	// number of columns of fvaluealg

  Float_t         c_fvaluealg[MAX_USER_VALUES];	//!caching table, avoids delete/new op. of TArray
  Int_t           c_fuseralg[2 * MAX_USER_VALUES];	//!caching table

  HHypComb       *p_hypcomb;	//! pointer to HHypComb container (! -> don't strem p_hypcomb)
  Int_t           comb_iter;	//! for the iterator
  Bool_t          status;	//! exit status
  Bool_t          iterstatus;	//! exit status

  ClassDef (HHypList, 1)	//This is the MAIN container for the HYP framework
};

#endif
