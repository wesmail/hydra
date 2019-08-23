//*-- AUTHOR : I. Froehlich
//*-- Modified : 04/07/2005 by I. Froehlich
////////////////////////////////////////////////////////////////////////
//
// HHypPPPipPimPi0Kine
//
// This is 
// i)  an ALGORITHM which updates the momenta using the HypKine Object
// ii) a SELECTOR which removes background using the kine refit
//
////////////////////////////////////////////////////////////////////////

using namespace std;

#include <stdlib.h>
#include <iostream>
#include "hhypPPPipPimPi0Kine.h"
#include "hypinfodef.h"

ClassImp(HHypPPPipPimPi0Kine)

HHypPPPipPimPi0Kine::HHypPPPipPimPi0Kine(char *name_i, Option_t par[])
  :HHypBaseAlgorithm(name_i,par)
{
}

HHypPPPipPimPi0Kine::~HHypPPPipPimPi0Kine()
{
}

Bool_t HHypPPPipPimPi0Kine::execute()
{
  if (mylist->getNpart() != 4)
    return kFALSE;
  //needs 4 particles

  Int_t hyppid[4];
  HPidParticle *particle[4];

  mylist->CombIteratorReset();
  while (mylist->CombIterator()) {
    for (int i = 0; i < (mylist->getNpart()); i++) {
      hyppid[i] = mylist->getPid(i);
      particle[i] = (HPidParticle *) mylist->getPidParticle(i);
    }

    //include kine refit here....
    //You have the array of all 4 tracks

    //DUMMY
    for (int i = 0; i < (mylist->getNpart()); i++) {
      TVector3 p = mylist->getMomentum(i);

      p.SetMag(p.Mag() * 0.5);
      mylist->setMomentum(i, p);
    }

  }                             //END Iterator

  if (exitIdx > -1)
    return kTRUE;
  return kFALSE;
}

Bool_t HHypPPPipPimPi0Kine::init()
{
  return kTRUE;
}

Bool_t HHypPPPipPimPi0Kine::reinit()
{
  return kTRUE;
}

Bool_t HHypPPPipPimPi0Kine::finalize()
{
  return kTRUE;
}
