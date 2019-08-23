//*-- AUTHOR : Marcin Wisniowski
//*-- Modified : 09.02.2005 by Marcin Wisniowski
//*-- Modified : 21/04/2005 by I. Froehlich

#ifndef HHypListCat_H
#define HHypListCat_H

#include "hhypcomb.h"
#include "hhypkine.h"
#include "hypinfodef.h"

#include "TROOT.h"
#include "TArrayF.h"
#include "TArrayI.h"
#include "TObject.h"
#include <hcategory.h>
#include "TLorentzVector.h"
#include <hpidtrackcand.h>
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
  void            Clear(const Option_t *opt = "");

  Int_t           getNcomb ();                                  // returns number of combination
  Int_t           getNpart ();                                  // returns (maximum) number of particle in one combination
  Int_t           getPid (Int_t Icomb, Int_t Ipart);            // returns assumed PID for cartain cell
  Int_t           getPid (Int_t Ipart);                         // returns assumed PID via iterator (Icomb++)
  Int_t           getIdxPidTrackCand (Int_t Icomb, Int_t Ipart);     // returns PidTrackCand index for cartain cell
  Int_t           getIdxPidTrackCand (Int_t Ipart);                  // returns PidTrackCand index via iterator (Icomb++)
  Int_t           getIdxPidTrackCand (const Char_t *name, Int_t);            // returns PidTrackCand index for given particle (via name)
  Float_t         getProbComb (Int_t Icomb);                    // returns probability of combination

  Float_t         getNrOfFakes(void);                         // return number of fakes (=valid but ignored tracks)
  Bool_t          isValidPart (Int_t Icomb, Int_t Ipart);     // returns false if Pid Idx<0 for cartain cell
  Bool_t          isValidPart (Int_t Ipart);                  // returns false if Pid Idx<0 via iterator (Icomb++)
  Int_t           getNvalid(void);                            // returns number of VALID particle in this combination
  Int_t           getNvalid(Int_t Icomb);                     // returns number of VALID particle in this combination

  TLorentzVector  getTLorentzVector (Int_t Icomb, Int_t Ipart); // returns TLorentzVector (px,py,pz,E)
  TLorentzVector  getTLorentzVector (const Char_t *, Int_t);            // returns TLorentzVector for given name (p,pi+...)
  TLorentzVector  getTLorentzVector (Int_t Ipart);

  TVector3        getOrigMomentum (Int_t Ipart);                    // returns original momentum vector
  TVector3        getOrigMomentum (Int_t Icomb, Int_t Ipart);       // returns original momentum vector

  TVector3        getMomentum (Int_t Ipart);                    // returns original or modified momentum vector
  TVector3        getMomentum (Int_t Icomb, Int_t Ipart);       // returns original or modified momentum vector

  Float_t         getBeta(const Char_t *name, Int_t num);
  Float_t         getBeta (Int_t Icomb, Int_t Ipart);
  Float_t         getBeta (Int_t Ipart);

  Bool_t          initcopyMomentum (void);                      // Inits (and if necs. copies) momentum
  Bool_t          setMomentum (Int_t Icomb, Int_t Ipart, TVector3 vect);// sets a new (modified) momentum vector
  Bool_t          setMomentum (Int_t Ipart, TVector3 vect);             // sets a new (modified) momentum vector
  Bool_t          setMomentum (const Char_t *name, Int_t num, TVector3 vect);   // sets a new (modified) momentum vector

  HPidTrackCand   *getPidTrackCand (Int_t Ipart);
  HPidTrackCand   *getPidTrackCand(const Char_t *name, Int_t num);

  Bool_t          setListId (Int_t Id_list);
  Int_t           getListId ();

  Bool_t          setIdxHypComb (Int_t Idx);
  Int_t           getIdxHypComb ();

  Bool_t          setIdxHypKine (Int_t Idx);
  Int_t           getIdxHypKine ();

  Bool_t          setProbAlg (Int_t Icomb, Float_t Prob);
  Float_t         getProbAlg (Int_t Icomb);
  Float_t         getProbAlg (void);
  Int_t           getOrder   (void);  //returns the order number of current comb. (starting from 1... numComb), 1 is best
  Bool_t          resetProbAlg (Float_t Prob);	// Update the probability inside the algorithm
  void            removeComb (void);
  void            KeepBestComb (void);

  Bool_t          ReadNumberedUserValues(Int_t i,Int_t &my_iter,Int_t &valueID,Float_t &value);
  void            setUserValue (Int_t valueID, Float_t value, Int_t my_iter = -1);	//This allows to store any user info
  Bool_t          writeUserValues (void);	//copy user values from cache table to final array, should be called after execute() of each alg.
  Bool_t          getUserValue (Int_t valueID, Float_t &value, Int_t my_iter = -1);	//get user value back
  HHypList       *getParent ();	//gets parent object (or NULL...)

  Bool_t          Join(HHypList *addthis);// Joins two lists (adds addthis to this)
  Bool_t          prepareHypList ();
  Bool_t          print ();

  static HCategory *buildLinearCat (const Text_t * Calssname);

  Bool_t          CombIteratorReset (void);
  Bool_t          CombIterator (void);
  Bool_t          getIterStatus (void);
  void            resetIterStatus(void);// set back to kTRUE

private:
  Int_t           idx_hypkine;  // index of HHypKine container ( -1 -> don't use kine )
  Int_t           idx_hypcomb;	// index of HHypComb container
  Int_t           idx_hyplist;	// index of "parent" HHypList container ( -1 -> no parent )
  Int_t           id_list;	    // channel list ID (see hypchannel)
  TArrayF         fprobalg;	    // probability calculated by algorythm for given combination
  TArrayF         fvaluealg;	// storing USER defined values  (up to nvaluealg)
  TArrayI         fuseralg;	    // storing USER defined values: row0->ncomb, row1->valueID  (up to 2*nvaluealg)
  Int_t           nvaluealg;	// number of columns of fvaluealg

  Float_t         c_fvaluealg[MAX_USER_VALUES];	    //!caching table, avoids delete/new op. of TArray
  Int_t           c_fuseralg[2 * MAX_USER_VALUES];	//!caching table
  Int_t           c_nvaluealg;	//! number of columns of c_fvaluealg

  HHypComb       *p_hypcomb;	//! pointer to HHypComb container (! -> don't strem p_hypcomb)
  Int_t           comb_iter;	//! for the iterator
  Bool_t          status;	    //! exit status
  Bool_t          iterstatus;	//! internal status of the particle iterator

  ClassDef(HHypList, 2)         //This is the MAIN container for the HYP framework
};

#endif
