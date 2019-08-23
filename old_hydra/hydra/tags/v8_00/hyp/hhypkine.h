//*-- AUTHOR : Marcin Wisniowski
//*-- Modified : 09/02/2005 by Marcin Wisniowski
//*-- Modified : 21/04/2005 by I. Froehlich

#ifndef HHypKineCat_H
#define HHypKineCat_H

#include <TROOT.h>
#include <TArrayF.h>
#include <TArrayI.h>
#include <TObject.h>
#include <hcategory.h>
#include <TLorentzVector.h>
#include <hlinearcategory.h>
#include <hphysicsconstants.h>

class HHypKine:public TObject
{

public:
  HHypKine(Int_t Ncomb, Int_t Npart);
  HHypKine(Int_t Idx_HypComb);
  HHypKine()     {clear();}
  ~HHypKine()    {}
  void            Clear(Option_t *opt = "");  
  Bool_t          clear();

  Bool_t          setNcomb(Int_t Ncomb);
  Int_t           getNcomb();

  Bool_t          setNpart(Int_t Npart);
  Int_t           getNpart();

  Bool_t          prepareHypKine();

  Bool_t          setTLorentzVector(Int_t Icomb, Int_t Ipart,
                                    TLorentzVector Vect);
  TLorentzVector  getTLorentzVector(Int_t Icomb, Int_t Ipart);
  Bool_t          setMomentum(Int_t Icomb, Int_t Ipart, TVector3 vect);
  TVector3        getMomentum(Int_t Icomb, Int_t Ipart);

  Bool_t          setPid(Int_t Icomb, Int_t Ipart, Int_t Pid);
  Int_t           getPid(Int_t Icomb, Int_t Ipart);

  Bool_t          print();

  //void Streamer(TBuffer &R__b);

  static HCategory *buildLinearCat(Text_t * calssname);

private:

  TArrayF         px;           // size = npart*ncomb table of x_momentum for each particle
  TArrayF         py;           // size = npart*ncomb table of y_momentum for each particle
  TArrayF         pz;           // size = npart*ncomb table of z_momentum for each particle
  TArrayI         pid;          // size = npart*ncomb table of ID number  for each particle
  Int_t           ncomb;        // 0,1,2, .... ,k
  Int_t           npart;        // 0,1,2, .... ,j

  ClassDef(HHypKine, 1)         //ROOT extension
};

#endif
