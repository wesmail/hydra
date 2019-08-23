//*-- AUTHOR : Marcin Wisniowski 
//*-- Modified : 09.02.2005 by Marcin Wisniowski 
//*-- Modified : 21/04/2005 by I. Froehlich
////////////////////////////////////////////////////////////////////// 
//                                                        
// HHypList 
//
// container class for HADES EVENT HYPOTHESIS
//
// Class which keeps HHypComb and table of probabilities (given by algorithms)
// coresponding to each combination of particles
// (index to HHypComb and fprobalg[] )
// HHypList object is created for every new algorithm
//
// User Alg's should use the list and NEVER hypcomb or hypkine
// -> This is the front-end class for all HYP actions!
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

#include "hypinfodef.h"

#include "hhyplist.h"
#include "hhypcomb.h"
#include <iostream>

using namespace std;

ClassImp(HHypList)
//--------------------------- constructors -------------------------------
  HHypList::HHypList(HHypList * OldList, Int_t IdxHypKine)
{
  p_hypcomb = NULL;

  if (OldList != NULL) {
    setIdxHypComb(OldList->getIdxHypComb());
    HCategory *combCat = gHades->getCurrentEvent()->getCategory(catHypComb);

    if (combCat != NULL) {
      p_hypcomb = (HHypComb *) combCat->getObject(idx_hypcomb);
      if (p_hypcomb != NULL) {
        prepareHypList();
      } else {
        cout << "\n HHypList::HHypList ERROR (p_hypcomb is NULL) \n";
      }
    } else {
      cout << "\n HHypList::HHypList ERROR (combCat is NULL) \n";
    }

    // activate kine container 
    if (IdxHypKine == -1)
      setIdxHypKine(OldList->getIdxHypKine());
    else
      setIdxHypKine(IdxHypKine);

    Int_t Ncomb = getNcomb();

    if (Ncomb != 0) {
      //now I copy the old fProbAlg to the new one
      //this I do after prepareHypList(), since here fProbComb has been
      //used [Ingo]
      for (Int_t Icomb = 0; Icomb < Ncomb; Icomb++) {
        setProbAlg(Icomb, OldList->getProbAlg(Icomb));
      }
      // set the pointer to the oldlist
      HCategory *combHyp = gHades->getCurrentEvent()->getCategory(catHypList);

      //cout << combHyp->getIndex(OldList) <<  endl;
      idx_hyplist = combHyp->getIndex(OldList);
    }

  } else {
    cout << "\n HHypList::HHypList ERROR (OldList is NULL) \n";
  }
}

//-------------------------------------------------------------------------
HHypList::HHypList(HHypComb * HypComb, Int_t IdxHypKine)
{
  p_hypcomb = HypComb;
  setIdxHypKine(IdxHypKine);

  if (p_hypcomb != NULL) {
    prepareHypList();
  } else {
    cout << "\n ERROR: HHypList::HHypList (p_hypcomb is NULL) \n";
  }
}

//-------------------------------------------------------------------------
HHypList::HHypList(Int_t Idx_HypComb, Int_t IdxHypKine)
{
  p_hypcomb = NULL;
  setIdxHypComb(Idx_HypComb);
  setIdxHypKine(IdxHypKine);

  HCategory *combCat = gHades->getCurrentEvent()->getCategory(catHypComb);

  if (combCat != NULL) {
    p_hypcomb = (HHypComb *) combCat->getObject(idx_hypcomb);
    if (p_hypcomb != NULL) {
      prepareHypList();
    } else {
      cout << "ERROR: HHypList::HHypList (p_hypcomb is NULL) \n";
    }
  } else {
    cout << "ERROR: HHypList::HHypList (combCat is NULL) \n";
  }
}

void HHypList::Clear(Option_t *opt) {
  fprobalg.Set(0);
  fvaluealg.Set(0);
  fuseralg.Set(0);
}


void HHypList::Restore(void)
{
  //restore pointer after reading from root file

  HCategory *combCat = gHades->getCurrentEvent()->getCategory(catHypComb);

  if (combCat != NULL) {
    p_hypcomb = (HHypComb *) combCat->getObject(idx_hypcomb);
    if (p_hypcomb == NULL) {
      cout << "\n HHypList::HHypList ERROR (p_hypcomb is NULL) \n";
    }
  } else {
    cout << "\n HHypList::HHypList ERROR (combCat is NULL) \n";
  }
}

//------------------------------- class methods -------------------------------

Bool_t HHypList::setListId(Int_t Id_list)
{
  id_list = Id_list;
  return kTRUE;
}

//-------------------------------------------------------------------------
Int_t HHypList::getListId()
{
  return id_list;
}

//-------------------------------------------------------------------------
HHypList *HHypList::getParent()
{
  HCategory *listCat = gHades->getCurrentEvent()->getCategory(catHypList);

  if (listCat != NULL) {
    return (HHypList *) listCat->getObject(idx_hyplist);
  }
  return NULL;
}

//-------------------------------------------------------------------------
Bool_t HHypList::prepareHypList()
{
  Int_t Ncomb = getNcomb();

  idx_hyplist = -1;
  nvaluealg = 0;

  if (Ncomb != 0) {
    fprobalg.Set(Ncomb);
    for (Int_t Icomb = 0; Icomb < Ncomb; Icomb++) {
      //by default, using fprobcomb
      //this can changed later by the algorithms
      setProbAlg(Icomb, p_hypcomb->getProbComb(Icomb));
    }
    return kTRUE;
  } else {
    cout << "ERROR: HHypList::prepareHypList (Ncomb is 0) \n";
    return kFALSE;
  }
}

//-------------------------------------------------------------------------
void HHypList::setUserValue(Int_t valueID, Float_t value, Int_t my_iter)
{

  if (my_iter == -1)
    my_iter = comb_iter;

  if (nvaluealg >= MAX_USER_VALUES) {
    std::cerr << " setUserValue > MAX_USER_VALUES" << std::endl;
    return;
  }
  // cout << "setting " << valueID << " to " << value << endl;
  c_fvaluealg[nvaluealg] = value;
  c_fuseralg[2 * nvaluealg] = my_iter;
  c_fuseralg[2 * nvaluealg + 1] = valueID;
  nvaluealg++;
}

//-------------------------------------------------------------------------
Bool_t HHypList::getUserValue(Int_t valueID, Float_t & value, Int_t my_iter)
{
  //This returns user values from this list or any parent list
  //Int_t valueID: see hypinfodef.h
  //Float_t & value: return user value
  //Int_t my_iter: Iteration (comb_iter by default, to be used in Iterator)
  //return Bool_t: success or not...

  if (my_iter == -1)
    my_iter = comb_iter;

  for (int i = 0; i < nvaluealg; i++) {
    // cout << fuseralg[2*i] << ":" << fuseralg[2*i+1] << endl;
    if ((fuseralg[2 * i] == my_iter) && (fuseralg[2 * i + 1] == valueID)) {
      value = fvaluealg[i];
      // cout << "reading " << fvaluealg[i] << endl;
      return kTRUE;
    }
  }
  if (idx_hyplist >= 0) {
    HHypList *p = getParent();

    if (p) {
      // cout << "looking for parent" << endl;
      Bool_t retval = p->getUserValue(valueID, value, my_iter);

      return retval;
    }
  }
  return kFALSE;
}

//-------------------------------------------------------------------------
Bool_t HHypList::writeUserValues(void)
{
  if (nvaluealg == 0)
    return kFALSE;
  fvaluealg.Set(nvaluealg);
  fuseralg.Set(nvaluealg * 2);
  for (int i = 0; i < nvaluealg; i++) {
    fvaluealg[i] = c_fvaluealg[i];
    // cout << "copy " << fvaluealg[i] << endl;
    fuseralg[2 * i] = c_fuseralg[2 * i];
    fuseralg[2 * i + 1] = c_fuseralg[2 * i + 1];
  }
  return kTRUE;
}

//-------------------------------------------------------------------------
Int_t HHypList::getIdxPidPart(Int_t Icomb, Int_t Ipart)
{
  return p_hypcomb->getIdxPidPart(Icomb, Ipart);
}

//-------------------------------------------------------------------------
Int_t HHypList::getIdxPidPart(Int_t Ipart)
{
  if ((comb_iter >= p_hypcomb->getNcomb()) || (comb_iter < 0)) {
    iterstatus = kFALSE;
    return -1;
  }
  return getIdxPidPart(comb_iter, Ipart);
}

//-------------------------------------------------------------------------
HPidParticle *HHypList::getPidParticle(Int_t Ipart)
{
  Int_t idx = getIdxPidPart(Ipart);

  if (idx < 0)
    return NULL;
  HPidParticle *PidPart = NULL;
  HCategory *pidpartCat = gHades->getCurrentEvent()->getCategory(catPidPart);

  PidPart = (HPidParticle *) pidpartCat->getObject(idx);
  return PidPart;
}

//-------------------------------------------------------------------------
Int_t HHypList::getNcomb()
{
  return p_hypcomb->getNcomb();
}

//-------------------------------------------------------------------------
Int_t HHypList::getNpart()
{
  return p_hypcomb->getNpart();
}

//-------------------------------------------------------------------------
Float_t HHypList::getProbComb(Int_t Icomb)
{
  return p_hypcomb->getProbComb(Icomb);
}

//-------------------------------------------------------------------------
Int_t HHypList::getOrder(void)
{
  Int_t better=0;
  if ((comb_iter >= p_hypcomb->getNcomb()) || (comb_iter < 0)) {
    iterstatus = kFALSE;
    return -1;
  }
  for (int i = 0; i < p_hypcomb->getNcomb(); i++) 
    if (getProbAlg(i) > getProbAlg()) 
      better++;
  return better+1;
}

//-------------------------------------------------------------------------
Int_t HHypList::getPid(Int_t num)
{
  if ((comb_iter >= p_hypcomb->getNcomb()) || (comb_iter < 0)) {
    iterstatus = kFALSE;
    return -1;
  }
  return getPid(comb_iter, num);
}

//-------------------------------------------------------------------------
TLorentzVector HHypList::getTLorentzVector(Int_t num)
{
  if ((comb_iter >= p_hypcomb->getNcomb()) || (comb_iter < 0)) {
    iterstatus = kFALSE;
    return TLorentzVector();
  }
  return getTLorentzVector(comb_iter, num);
}

//-------------------------------------------------------------------------
Bool_t HHypList::setMomentum(Int_t Ipart, TVector3 vect)
{
  if ((comb_iter >= p_hypcomb->getNcomb()) || (comb_iter < 0)) {
    iterstatus = kFALSE;
    return kFALSE;
  }
  return setMomentum(comb_iter, Ipart, vect);
}

//-------------------------------------------------------------------------
Bool_t HHypList::setMomentum(char *name, Int_t num, TVector3 vect)
{
  if ((comb_iter >= p_hypcomb->getNcomb()) || (comb_iter < 0)) {
    iterstatus = kFALSE;
    return kFALSE;
  }

  Int_t part_counter = -1;
  Int_t part_num = 0;
  Int_t part_id = HPidPhysicsConstants::pid(name);

  //    cout << "looking for " << name <<  endl;

  while ((part_num != num) && (part_counter < (getNpart() - 1))) {
    part_counter++;
    //        cout << getPid(comb_iter, part_counter) << endl;
    if (part_id == getPid(comb_iter, part_counter))
      part_num++;
  }

  if (part_num == num) {
    //       cout << "particle " << name << " is on position " <<  part_counter << endl;
	  return setMomentum(comb_iter, part_counter, vect);
  }
  iterstatus = kFALSE;
  return kFALSE;
}

//-------------------------------------------------------------------------
TVector3 HHypList::getMomentum(Int_t num)
{
  if ((comb_iter >= p_hypcomb->getNcomb()) || (comb_iter < 0)) {
    iterstatus = kFALSE;
    return TVector3();
  }
  return getMomentum(comb_iter, num);
}

//-------------------------------------------------------------------------
Float_t HHypList::getBeta(Int_t num)
{
  if ((comb_iter >= p_hypcomb->getNcomb()) || (comb_iter < 0)) {
    iterstatus = kFALSE;
    return 0;
  }
  
  return getBeta(comb_iter, num);
}

//-------------------------------------------------------------------------
TLorentzVector HHypList::getTLorentzVector(char *name, Int_t num)
{
  if ((comb_iter >= p_hypcomb->getNcomb()) || (comb_iter < 0)) {
    iterstatus = kFALSE;
    return TLorentzVector();
  }

  Int_t part_counter = -1;
  Int_t part_num = 0;
  Int_t part_id = HPidPhysicsConstants::pid(name);

  //    cout << "looking for " << name <<  endl;

  while ((part_num != num) && (part_counter < (getNpart() - 1))) {
    part_counter++;
    //        cout << getPid(comb_iter, part_counter) << endl;
    if (part_id == getPid(comb_iter, part_counter))
      part_num++;
  }

  if (part_num == num) {
    //       cout << "particle " << name << " is on position " <<  part_counter << endl;
    return getTLorentzVector(comb_iter, part_counter);
  }
  iterstatus = kFALSE;
  return TLorentzVector();
}

Int_t HHypList::getIdxPidPart(char *name, Int_t num)
{
  if ((comb_iter >= p_hypcomb->getNcomb()) || (comb_iter < 0)) {
    iterstatus = kFALSE;
    return -1;
  }

  Int_t part_counter = -1;
  Int_t part_num = 0;
  Int_t part_id = HPidPhysicsConstants::pid(name);

  while ((part_num != num) && (part_counter < (getNpart() - 1))) {
    part_counter++;
    if (part_id == getPid(comb_iter, part_counter))
      part_num++;
  }

  if (part_num == num) {
    return getIdxPidPart(comb_iter, part_counter);
  }
  iterstatus = kFALSE;
  return -1;
}

//-------------------------------------------------------------------------
Bool_t HHypList::setMomentum(Int_t Icomb, Int_t Ipart, TVector3 vect)
{
  if (getIdxHypKine() == -1)    //MAKE Kine
  {
    HHypKine *HypKine = NULL;

    HCategory *hypkineCat = gHades->getCurrentEvent()->getCategory(catHypKine);

    if (hypkineCat != NULL) {
      HLocation locDummy;

      HypKine = (HHypKine *) hypkineCat->getNewSlot(locDummy, &idx_hypkine);
      if (HypKine != 0)
        HypKine = new(HypKine) HHypKine(idx_hypcomb);
      else
        return kFALSE;
      HypKine->setMomentum(Icomb, Ipart, vect);
    } else {
      cout << "ERROR: HHypList::setMomentum (hypkineCat is NULL) \n";
    }
  } else                        //USE kine
  {
    HHypKine *HypKine = NULL;

    HCategory *hypkineCat = gHades->getCurrentEvent()->getCategory(catHypKine);

    if (hypkineCat != NULL) {
      HypKine = (HHypKine *) hypkineCat->getObject(getIdxHypKine());

      if (HypKine != NULL) {
        HypKine->setMomentum(Icomb, Ipart, vect);
      } else {
        cout << "ERROR: HHypList::setMomentum (HypKine is NULL) \n";
      }
    } else {
      cout << "ERROR: HHypList::setMomentum (hypkineCat is NULL) \n";
    }
  }
  return kTRUE;
}

//-------------------------------------------------------------------------
TVector3 HHypList::getMomentum(Int_t Icomb, Int_t Ipart)
{
  TVector3 Vect3;

  if (getIdxHypKine() == -1) {
    HPidParticle *PidPart = NULL;

    HCategory *pidpartCat = gHades->getCurrentEvent()->getCategory(catPidPart);

    if (pidpartCat != NULL) {
      PidPart =
        (HPidParticle *) pidpartCat->getObject(getIdxPidPart(Icomb, Ipart));

      if (PidPart != NULL) {
        Vect3 = ((TLorentzVector) (*PidPart)).Vect();
      }
    }
  } else {
    HHypKine *HypKine = NULL;

    HCategory *hypkineCat = gHades->getCurrentEvent()->getCategory(catHypKine);

    if (hypkineCat != NULL) {
      HypKine = (HHypKine *) hypkineCat->getObject(getIdxHypKine());

      if (HypKine != NULL) {
        Vect3 = (HypKine->getTLorentzVector(Icomb, Ipart)).Vect();
      }
    }
  }
  return Vect3;
}

//-------------------------------------------------------------------------
Float_t HHypList::getBeta(Int_t Icomb, Int_t Ipart)
{
  Float_t beta = 0;

  HPidParticle *PidPart = NULL;
  HPidTrackCand  *pidTrackCand = NULL;
  
  HCategory *pidpartCat = gHades->getCurrentEvent()->getCategory(catPidPart);
  

  if (pidpartCat != NULL) {
    PidPart =
      (HPidParticle *) pidpartCat->getObject(getIdxPidPart(Icomb, Ipart));
    
    if (PidPart != NULL) {
      pidTrackCand = PidPart->getTrackCand();
      if (pidTrackCand) { 

	beta = fabs(pidTrackCand->getTrackData()->getBeta(PidPart->getMomAlg()));
	Float_t offset;
	if (getUserValue(DELTATOF_OFFSET, offset, Icomb)) {
	  //Recalculate the beta according to DeltaTof
	  Double_t path = pidTrackCand->getTrackData()->getPathLength(PidPart->getMomAlg());
	  //Remember OFFSET was defined to ExpBeta
	  Double_t t_exp = path/(PidPart->getBetaExp() * 299.792458);
	  beta = path/((t_exp+offset) * 299.792458);
	  //cout << "path: " <<path << " offset : " << offset << " beta new: " << beta << " beta old: "<<PidPart->getBetaExp() << endl;
	  //beta = path/((t_exp) * 299.792458);
	}
      }
    }
  }
  return beta;
}


//-------------------------------------------------------------------------
TLorentzVector HHypList::getTLorentzVector(Int_t Icomb, Int_t Ipart)
{
  TLorentzVector Vect;

  if (getIdxHypKine() == -1) {
    HPidParticle *PidPart = NULL;

    HCategory *pidpartCat = gHades->getCurrentEvent()->getCategory(catPidPart);

    if (pidpartCat != NULL) {
      PidPart =
        (HPidParticle *) pidpartCat->getObject(getIdxPidPart(Icomb, Ipart));

      if (PidPart != NULL) {
        Double_t Mass = HPidPhysicsConstants::mass(getPid(Icomb, Ipart));

        TVector3 Vect3;

        Vect = (TLorentzVector) (*PidPart);
        Vect3 = Vect.Vect();
        Vect.SetVectM(Vect3, Mass);
      } else {
        cout << "ERROR: HHypList::getTLorentzVector (PidPart is NULL) \n";
      }
    } else {
      cout << "ERROR: HHypList::getTLorentzVector (pidpartCat is NULL) \n";
    }
  } else {
    HHypKine *HypKine = NULL;

    HCategory *hypkineCat = gHades->getCurrentEvent()->getCategory(catHypKine);

    if (hypkineCat != NULL) {
      HypKine = (HHypKine *) hypkineCat->getObject(getIdxHypKine());

      if (HypKine != NULL) {
        Vect = HypKine->getTLorentzVector(Icomb, Ipart);
      } else {
        cout << "ERROR: HHypList::getTLorentzVector (HypKine is NULL) \n";
      }
    } else {
      cout << "ERROR: HHypList::getTLorentzVector (hypkineCat is NULL) \n";
    }
  }
  return Vect;
}

//-------------------------------------------------------------------------
Bool_t HHypList::setProbAlg(Int_t Icomb, Float_t Prob)
{
  if (Icomb < getNcomb()) {
    fprobalg[Icomb] = Prob;
    return kTRUE;
  } else {
    cout << "ERROR HHypList::setProbAlg (Icomb out of bounds)" << endl;
    return kFALSE;
  }
}

//-------------------------------------------------------------------------
Float_t HHypList::getProbAlg(Int_t Icomb)
{
  if (Icomb < getNcomb()) {
    return fprobalg[Icomb];
  } else {
    cout << "ERROR HHypList::getProbAlg (Icomb out of bounds)" << endl;
    return -1.0;
  }
}

//-------------------------------------------------------------------------
Float_t HHypList::getProbAlg(void)
{
  if ((comb_iter >= p_hypcomb->getNcomb()) || (comb_iter < 0)) {
    iterstatus = kFALSE;
    return -1.;
  }
  return getProbAlg(comb_iter);
}

//-------------------------------------------------------------------------
void HHypList::removeComb(void)
{
  // Use this function inside a selector to remoce current combination.
  resetProbAlg(-1.);
}

//-------------------------------------------------------------------------
void HHypList::KeepBestComb(void)
{
  // Remove all combinations, keep only the best one
  Double_t bestprob = -1, bestcomb = -1;

  for (int i = 0; i < p_hypcomb->getNcomb(); i++) {
    if (getProbAlg(i) > bestprob) {
      bestprob = getProbAlg(i);
      bestcomb = i;
    }
  }
  if (bestcomb < 0)
    return;
  for (int i = 0; i < p_hypcomb->getNcomb(); i++) {
    if (i != bestcomb)
      setProbAlg(i, -1);
  }
}

//-------------------------------------------------------------------------
Bool_t HHypList::resetProbAlg(Float_t Prob)
{
  // This function sets a new probability inside the Iterator()
  // [Ingo]
  if ((comb_iter >= p_hypcomb->getNcomb()) || (comb_iter < 0)) {
    iterstatus = kFALSE;
    return kFALSE;
  }
  setProbAlg(comb_iter, Prob);
  return kTRUE;
}

//-------------------------------------------------------------------------
Int_t HHypList::getPid(Int_t Icomb, Int_t Ipart)
{
  return p_hypcomb->getPid(Icomb, Ipart);
}

//-------------------------------------------------------------------------
Bool_t HHypList::setIdxHypComb(Int_t Idx)
{
  idx_hypcomb = Idx;
  return kTRUE;
}

//-------------------------------------------------------------------------
Int_t HHypList::getIdxHypComb()
{
  return idx_hypcomb;
}

//-------------------------------------------------------------------------
Bool_t HHypList::setIdxHypKine(Int_t Idx)
{
  idx_hypkine = Idx;
  return kTRUE;
}

//-------------------------------------------------------------------------
Int_t HHypList::getIdxHypKine()
{
  return idx_hypkine;
}

//-------------------------------------------------------------------------
Bool_t HHypList::CombIteratorReset(void)
{
  comb_iter = -1;
  iterstatus = kTRUE;
  return kTRUE;
}

//-------------------------------------------------------------------------
Bool_t HHypList::getIterStatus(void)
{
  return iterstatus;
}

//-------------------------------------------------------------------------
Bool_t HHypList::CombIterator(void)
{
  // This is the only legal method how to scan the valid 
  // combinations. A combination is valid, if probAlg is equal or more
  // than zero. It is unvalid, if probAlg is SMALLER than zero.
  comb_iter++;

  if (comb_iter >= p_hypcomb->getNcomb()) {
    return kFALSE;
  }

  while ((comb_iter < p_hypcomb->getNcomb()) && (getProbAlg(comb_iter) < 0)) {
    comb_iter++;
  }

  if ((comb_iter < p_hypcomb->getNcomb()) && (getProbAlg(comb_iter) >= 0))
    return kTRUE;
  else
    return kFALSE;
}

//-------------------------------------------------------------------------
Bool_t HHypList::print()
{
  cout << "\n";                 // Idx table
  for (Int_t i = 0; i < getNcomb(); i++) {
    for (Int_t j = 0; j < (getNpart()); j++) {
      cout << "\t" << getIdxPidPart(i, j);
    }
    cout << "\t" << getProbComb(i) << "\t" << getProbAlg(i) << endl;
  }

  cout << "\n";                 // Pid table
  for (Int_t i = 0; i < getNcomb(); i++) {
    for (Int_t j = 0; j < (getNpart()); j++) {
      cout << "\t" << getPid(i, j);
    }
    cout << "\n";
  }
  cout << "\n";
  return kTRUE;
}

//-------------------------------------------------------------------------
HCategory *HHypList::buildLinearCat(Text_t * classname)
{
  HLinearCategory *category = NULL;
  Int_t size = 1000;

  category = new HLinearCategory(classname, size);
  category->setDynamicObjects(kTRUE);

  return category;
}

//-------------------------------------------------------------------------
void HHypList::Streamer(TBuffer & R__b)
{
  // Stream an object of class HHypList.

  if (R__b.IsReading()) {
    Version_t R__v = R__b.ReadVersion();

    if (R__v == 1) {
      Version_t R__v;

      R__v = R__b.ReadVersion();
    }
    TObject::Streamer(R__b);
    R__b >> idx_hypcomb;
    R__b >> idx_hypkine;
    R__b >> idx_hyplist;
    R__b >> id_list;
    R__b >> nvaluealg;
    for (Int_t i = 0; i < getNcomb(); i++) {
      R__b >> fprobalg[i];
    }
    for (Int_t i = 0; i < nvaluealg; i++) {
      R__b >> fvaluealg[i];
    }
    for (Int_t i = 0; i < (2 * nvaluealg); i++) {
      R__b >> fuseralg[i];
    }

  } else {
    R__b.WriteVersion(HHypList::IsA());
    TObject::Streamer(R__b);
    R__b << idx_hypcomb;
    R__b << idx_hypkine;
    R__b << idx_hyplist;
    R__b << id_list;
    R__b << nvaluealg;
    for (Int_t i = 0; i < getNcomb(); i++) {
      R__b << fprobalg[i];
    }
    for (Int_t i = 0; i < nvaluealg; i++) {
      R__b << fvaluealg[i];
    }
    for (Int_t i = 0; i < (2 * nvaluealg); i++) {
      R__b << fuseralg[i];
    }
  }
}
