//*-- AUTHOR : Marcin Wisniowski
//*-- Modified : 04/07/2005 by I. Froehlich
////////////////////////////////////////////////////////////////////////
//
// HHypComb
//
// container class for HADES EVENT HYPOTHESIS
// Class which keeps all possible particle combinations 
// (indeces to HPidParticle/HPidParticleSim)
//
// Example: p   p   pi+  pi-
//          p   pi+ p    pi- 
//          pi+ p   p    pi-  
//
////////////////////////////////////////////////////////////////////////

#pragma implementation
#include "hhypcomb.h"
#include <iostream>

using namespace std;

ClassImp(HHypComb)
//-------------------------- constructor ------------------------------
  HHypComb::HHypComb(Int_t Ncomb, Int_t Npart)
{
  clear();
  ncomb = Ncomb;
  npart = Npart;
  prepareHypComb();
}

//---------------------------- class methods ------------------------------
Bool_t HHypComb::clear()
{
  ncomb = 0;
  npart = 0;
  return kTRUE;
}

//---------------------------------------------------------------------
Bool_t HHypComb::setCombId(Int_t Cid)
{
  cid = Cid;
  return kTRUE;
}

//---------------------------------------------------------------------
Int_t HHypComb::getCombId()
{
  return cid;
}

//---------------------------------------------------------------------
Bool_t HHypComb::setNcomb(Int_t Ncomb)
{
  ncomb = Ncomb;
  return kTRUE;
}

//---------------------------------------------------------------------
Int_t HHypComb::getNcomb()
{
  return ncomb;
}

//---------------------------------------------------------------------
Bool_t HHypComb::setNpart(Int_t Npart)
{
  npart = Npart;
  return kTRUE;
}

//---------------------------------------------------------------------
Int_t HHypComb::getNpart()
{
  return npart;
}

//---------------------------------------------------------------------
Bool_t HHypComb::setProbComb(Int_t Icomb, Float_t Prob)
{
  if (Icomb < ncomb) {
    fprobcomb[Icomb] = Prob;
    return kTRUE;
  } else {
    cout << "Error: HHypComb::setProbComb (Icomb out of bounds)" << endl;
    return kFALSE;
  }
}

//---------------------------------------------------------------------
Float_t HHypComb::getProbComb(Int_t Icomb)
{
  if (Icomb < ncomb) {
    return fprobcomb[Icomb];
  } else {
    cout << "Error: HHypComb::getProbComb (Icomb out of bounds)" << endl;
    return -1;
  }
}

//---------------------------------------------------------------------
Bool_t HHypComb::prepareHypComb()
{
  if ((ncomb != 0) && (npart != 0)) {
    idx.Set(ncomb * npart);
    pid.Set(ncomb * npart);
    fprobcomb.Set(ncomb);
    for (Int_t Icomb = 0; Icomb < ncomb; Icomb++) {
      setProbComb(Icomb, 1);
      for (Int_t Ipart = 0; Ipart < npart; Ipart++) {
        setIdxPidPart(Icomb, Ipart, -1);
      }
    }
    return kTRUE;
  } else {
    cout << "Error: HHypComb::prepareHypComb (ncomb or npart equal 0)" << endl;
    return kFALSE;
  }
}

//---------------------------------------------------------------------
Bool_t HHypComb::setIdxPidPart(Int_t Icomb, Int_t Ipart, Int_t Idx)
{
  if ((Icomb < ncomb) && (Ipart < npart)) {
    idx[Icomb * npart + Ipart] = Idx;
    return kTRUE;
  } else {
    cout << "Error: HHypComb::setIdxPidPart (Icomb or Ipart out of bounds)" <<
      endl;
    return kFALSE;
  }
}

//---------------------------------------------------------------------
Int_t HHypComb::getIdxPidPart(Int_t Icomb, Int_t Ipart)
{
  if ((Icomb < ncomb) && (Ipart < npart)) {
    return idx[Icomb * npart + Ipart];
  } else {
    cout << "ERROR: HHypComb::getIdxPidPart (Icomb or Ipart out of bounds)" <<
      endl;
    return -1;
  }
}

//---------------------------------------------------------------------
Bool_t HHypComb::setPid(Int_t Icomb, Int_t Ipart, Int_t Pid)
{
  if ((Icomb < ncomb) && (Ipart < npart)) {
    pid[Icomb * npart + Ipart] = Pid;
    return kTRUE;
  } else {
    cout << "ERROR: HHypComb::setPid (Icomb or Ipart out of bounds)" << endl;
    return kFALSE;
  }
}

//---------------------------------------------------------------------
Int_t HHypComb::getPid(Int_t Icomb, Int_t Ipart)
{
  if ((Icomb < ncomb) && (Ipart < npart)) {
    return pid[Icomb * npart + Ipart];
  } else {
    cout << "ERROR: HHypComb::getPid (Icomb or Ipart out of range)" << endl;
    return -1;
  }
}

//---------------------------------------------------------------------
Bool_t HHypComb::print()
{
  cout << "\t\t[Debug] HHypComb::print()" << endl;
  cout << "\t\t[Debug] Number of Comb : " << ncomb << endl;
  cout << "\t\t[Debug] Number of Part : " << npart << endl;

  for (Int_t i = 0; i < ncomb; i++) {
    for (Int_t j = 0; j < npart; j++) {
      cout << "\t" << getIdxPidPart(i, j);
    }
    cout << " !";
    for (Int_t j = 0; j < npart; j++) {
      cout << "\t" << getPid(i, j);
    }
    cout << " !\t" << getProbComb(i) << endl;
  }
  cout << "\n";
  return kTRUE;
}

//---------------------------------------------------------------------
HCategory *HHypComb::buildLinearCat(Text_t * classname)
{
  HLinearCategory *category = NULL;
  Int_t size = 1000;

  category = new HLinearCategory(classname, size);
  category->setDynamicObjects(kTRUE);

  return category;
}

// -----------------------------------------------------------------------------


void HHypComb::Streamer(TBuffer & R__b)
{
  // Stream an object of class HHypComb.

  if (R__b.IsReading())         // read 
  {
    Version_t R__v = R__b.ReadVersion();

    if (R__v == 1) {
      Version_t R__v;

      R__v = R__b.ReadVersion();
    }
    TObject::Streamer(R__b);
    R__b >> ncomb;
    R__b >> npart;
    for (Int_t i = 0; i < getNcomb(); i++) {
      R__b >> fprobcomb[i];
    }
    for (Int_t i = 0; i < idx.GetSize(); i++) {
      R__b >> idx[i];
      R__b >> pid[i];
    }
  } else                        // write 
  {
    R__b.WriteVersion(HHypComb::IsA());
    TObject::Streamer(R__b);
    R__b << ncomb;
    R__b << npart;
    for (Int_t i = 0; i < getNcomb(); i++) {
      R__b << fprobcomb[i];
    }
    for (Int_t i = 0; i < idx.GetSize(); i++) {
      R__b << idx[i];
      R__b << pid[i];
    }
  }
}

