//*-- AUTHOR : Marcin Wisniowski
//*-- Modified : 04/07/2005 by I. Froehlich
//*-- Modified : 30.11.2005 by B. Spruck

#ifndef HHypCombCat_H
#define HHypCombCat_H

#include "TROOT.h"
#include "TArrayF.h"
#include "TArrayI.h"
#include "TObject.h"
#include <hcategory.h>
#include <hlinearcategory.h>

class  HHypComb:public TObject
{

public:
  HHypComb(Int_t Ncomb, Int_t Npart);
  HHypComb()     {clear();}
  ~HHypComb()    {}

  Bool_t          clear();
  void            Clear(const Option_t *opt = "");

  Bool_t          setCombId(Int_t Cid);
  Int_t           getCombId();

  Bool_t          setNcomb(Int_t Ncomb);
  Int_t           getNcomb();

  Bool_t          setNpart(Int_t Npart);
  Int_t           getNpart();

  Int_t           getNvalid(Int_t Icomb);   // returns numer of valid particle in this combination 

  Bool_t          setProbComb(Int_t Icomb, Float_t Prob);
  Float_t         getProbComb(Int_t Icomb);

  Bool_t          prepareHypComb();

  Bool_t          setIdxPidTrackCand(Int_t Icomb, Int_t Ipart, Int_t Idx);
  Int_t           getIdxPidTrackCand(Int_t Icomb, Int_t Ipart);

  Bool_t          setPid(Int_t Icomb, Int_t Ipart, Int_t Pid);
  Int_t           getPid(Int_t Icomb, Int_t Ipart);

  Bool_t          Join(HHypComb *addthis);// This adds content of Comb to already existing (same nr of particles!)
  
  Bool_t          print();

  static HCategory *buildLinearCat(const Text_t * calssname);

private:

  TArrayF         fprobcomb;    // probTrack1*probTrack2*.... 
  TArrayI         idx;          // size = npart*ncomb table of Indexes to HPidTrackCand
  TArrayI         pid;          // size = npart*ncomb table of Particle ID
  Int_t           ncomb;        // 0,1,2, .... ,k
  Int_t           npart;        // 0,1,2, .... ,j
  Int_t           cid;          // Combination's ID number (table of combination)

  ClassDef(HHypComb, 1)         // Combination matrix for assignment PID <-> TRACKS
};

#endif
