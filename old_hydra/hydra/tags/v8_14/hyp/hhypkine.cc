//*-- AUTHOR : Marcin Wisniowski
//*-- Modified : 09/02/2005 by Marcin Wisniowski
//*-- Modified : 21/04/2005 by I. Froehlich
//_HADES_CLASS_DESCRIPTION 
//////////////////////////////////////////////////////////////////////
//
// HHypKine 
// 
// container class for HADES EVENT HYPOTHESIS
// Class which keeps all possible particle combinations 
// with all kinematicks ( kinematical variables for every particle can be changed )
// ( Foreseen for Kinematical refit )
//
//////////////////////////////////////////////////////////////////////

#pragma implementation
#include "hevent.h"
#include "heventheader.h"
#include "hdetector.h"
#include "hratreeext.h"
#include "hcategory.h"
#include "hmatrixcategory.h"
#include "hlinearcategory.h"
#include "hlinearcatiter.h"
#include "hlocation.h"
#include "hiterator.h"
#include "hdebug.h"
#include "hades.h"
#include <hpidparticle.h>
#include <hphysicsconstants.h>

#include "hypinfodef.h"

#include "hhypkine.h"
#include "hhypcomb.h"
#include <iostream>

using namespace std;

ClassImp(HHypKine)
//-------------------------- constructors ------------------------------
  HHypKine::HHypKine(Int_t Ncomb, Int_t Npart)
{
  clear();
  ncomb = Ncomb;
  npart = Npart;
  prepareHypKine();
}

HHypKine::HHypKine(HHypKine * src)
{
  clear();
  ncomb = src->getNcomb();
  npart = src->getNpart();
//  prepareHypKine();// not needed
  pid = src->getArrayPid();
  px = src->getArrayPx();
  py = src->getArrayPy();
  pz = src->getArrayPz();
}

HHypKine::HHypKine(Int_t Idx_HypComb)
{
  HHypComb       *p_hypcomb = NULL;
  HPidParticle   *PidPart = NULL;

  HCategory      *combCat = gHades->getCurrentEvent()->getCategory(catHypComb);
  HCategory      *pidpartCat =
    gHades->getCurrentEvent()->getCategory(catPidPart);

  if ((combCat != 0) && (pidpartCat != NULL)) {
    p_hypcomb = (HHypComb *) combCat->getObject(Idx_HypComb);

    if (p_hypcomb != NULL) {
      setNcomb(p_hypcomb->getNcomb());
      setNpart(p_hypcomb->getNpart());
      prepareHypKine();

      for (Int_t icomb = 0; icomb < (p_hypcomb->getNcomb()); icomb++) {
        for (Int_t ipart = 0; ipart < (p_hypcomb->getNpart()); ipart++) {
          Int_t idx;
          idx=p_hypcomb->getIdxPidPart(icomb, ipart);
//          cout << ipart << ":" << icomb << ":"<<idx<<endl;
//          if( idx<0) p_hypcomb->print();
          if( idx>=0){// normal algos would ask for hyplist.isValidPart()
          PidPart =
              (HPidParticle *) pidpartCat->getObject(idx);

          if (PidPart != NULL) {
            setPid(icomb, ipart, p_hypcomb->getPid(icomb, ipart));
//              TVector3 Vect3;
//              Vect3 = ((TLorentzVector) (*PidPart)).Vect();
//              setMomentum(icomb, ipart, Vect3);
              setMomentum(icomb, ipart, PidPart->Vect());
          }                     //END if (PidPart != NULL)
          else {
            cout << "ERROR: HHypKine::HHypKine (PidPart is NULL) \n";
          }
          }else{// Pid idx <0 
            // set empty "dummy" momentum
            // well, this might not be needed at all
            // TVector3 Vect3;
            // setMomentum(icomb, ipart, Vect3);
          }
        }
      }

    } else {
      cout << "\n Error : HHypKine::HHypKine ( p_hypcomb set to NULL ) \n";
    }
  }
}

//---------------------------------------------------------------------

//---------------------------- functions ------------------------------
Bool_t HHypKine::clear()
{
  ncomb = 0;
  npart = 0;
  return kTRUE;
}

void HHypKine::Clear(Option_t * opt)
{
  px.Set(0);
  py.Set(0);
  pz.Set(0);
  pid.Set(0);
}

Bool_t HHypKine::setNcomb(Int_t Ncomb)
{
  ncomb = Ncomb;
  return kTRUE;
}

Int_t HHypKine::getNcomb()
{
  return ncomb;
}

Bool_t HHypKine::setNpart(Int_t Npart)
{
  npart = Npart;
  return kTRUE;
}

Int_t HHypKine::getNpart()
{
  return npart;
}

Bool_t HHypKine::prepareHypKine()
{
  if ((ncomb > 0) && (npart > 0)) {
    pid.Set(ncomb * npart);
    px.Set(ncomb * npart);
    py.Set(ncomb * npart);
    pz.Set(ncomb * npart);

    return kTRUE;
  } else {
    cout << "\tError: ncomb || npart are <=0 !!!\n";
    return kFALSE;
  }
}

Bool_t HHypKine::setTLorentzVector(Int_t Icomb, Int_t Ipart,
                                   TLorentzVector Vect)
{
  if ((Icomb < getNcomb()) && (Ipart < getNpart())) {
    px[Icomb * npart + Ipart] = Vect.Px();
    py[Icomb * npart + Ipart] = Vect.Py();
    pz[Icomb * npart + Ipart] = Vect.Pz();
    return kTRUE;
  } else {
    cout <<
      "\n Error : HHypKine::setTLorentzVector  ( Icomb or Ipart out of range )\n";
    return kFALSE;
  }

}

Bool_t HHypKine::setMomentum(Int_t Icomb, Int_t Ipart, TVector3 vect)
{
  if ((Icomb < getNcomb()) && (Ipart < getNpart())) {
    px[Icomb * npart + Ipart] = vect.Px();
    py[Icomb * npart + Ipart] = vect.Py();
    pz[Icomb * npart + Ipart] = vect.Pz();
    return kTRUE;
  } else {
    cout <<
      "\n Error : HHypKine::setMomentum  ( Icomb or Ipart out of range )\n";
    return kFALSE;
  }
}

TLorentzVector HHypKine::getTLorentzVector(Int_t Icomb, Int_t Ipart)
{
  TLorentzVector Vect;

  if ((Icomb < getNcomb()) && (Ipart < getNpart())) {
    Double_t Mass = HPidPhysicsConstants::mass(getPid(Icomb, Ipart));
    TVector3 Vect3;

    Vect3.SetXYZ(px[Icomb * npart + Ipart], py[Icomb * npart + Ipart],
                 pz[Icomb * npart + Ipart]);
    Vect.SetVectM(Vect3, Mass);
  } else {
    cout <<
      "\n Error : HHypKine::getTLorentzVector  ( Icomb or Ipart out of range )\n";
  }
  return Vect;
}

TVector3 HHypKine::getMomentum(Int_t Icomb, Int_t Ipart)
{
  TVector3 Vect3;

  if ((Icomb < getNcomb()) && (Ipart < getNpart())) {
    Vect3.SetXYZ(px[Icomb * npart + Ipart], py[Icomb * npart + Ipart],
                 pz[Icomb * npart + Ipart]);
  } else {
    cout <<
      "\n Error : HHypKine::getMomentum  ( Icomb or Ipart out of range )\n";
  }
  return Vect3;
}

Bool_t HHypKine::setPid(Int_t Icomb, Int_t Ipart, Int_t Pid)
{
  if ((Icomb < getNcomb()) && (Ipart < getNpart())) {
    pid[Icomb * npart + Ipart] = Pid;
    return kTRUE;
  } else {
    cout << "\n Error : HHypKine::setPid  ( Icomb or Ipart out of range )\n";
    return kFALSE;
  }
}

Int_t HHypKine::getPid(Int_t Icomb, Int_t Ipart)
{
  if ((Icomb < getNcomb()) && (Ipart < getNpart())) {
    return pid[Icomb * npart + Ipart];
  } else {
    cout << "\n Error : HHypKine::getPid  ( Icomb or Ipart out of range )\n";
    return -1;
  }
}

Bool_t HHypKine::print()
{

  return kTRUE;
}

HCategory      *HHypKine::buildLinearCat(Text_t * classname)
{
  HLinearCategory *category = NULL;
  Int_t size = 1000;

  category = new HLinearCategory(classname, size);
  category->setDynamicObjects(kTRUE);

  return category;
}

// -----------------------------------------------------------------------------

void HHypKine::Streamer(TBuffer & R__b)
{
  // Stream an object of class HHypKine.

  if (R__b.IsReading()) {
    Version_t R__v = R__b.ReadVersion();

    if (R__v != 1) {
      cout << "HHypKine::Streamer: version is not 1" << endl;
    }
    TObject::Streamer(R__b);
    R__b >> ncomb;
    R__b >> npart;
    pid.Set(ncomb * npart);
    px.Set(ncomb * npart);
    py.Set(ncomb * npart);
    pz.Set(ncomb * npart);

    for (Int_t i = 0; i < pid.GetSize(); i++) {
      R__b >> pid[i];
      R__b >> px[i];
      R__b >> py[i];
      R__b >> pz[i];
    }
  } else {
    R__b.WriteVersion(HHypKine::IsA());
    TObject::Streamer(R__b);
    R__b << ncomb;
    R__b << npart;

    for (Int_t i = 0; i < pid.GetSize(); i++) {
      R__b << pid[i];
      R__b << px[i];
      R__b << py[i];
      R__b << pz[i];
    }
  }
}
