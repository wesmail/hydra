//*-- AUTHOR : Marcin Wisniowski
//*-- Modified : 09.02.2005 by Marcin Wisniowski
//*-- Modified : 21/04/2005 by I. Froehlich
//*-- Modified : 15.04.2008 by B. Spruck
//_HADES_CLASS_DESCRIPTION
//////////////////////////////////////////////////////////////////////
//
// HHypList
//
// container class for HADES EVENT HYPOTHESIS
//
//       This is the MAIN container for the HYP framework
//
// Class keeps HHypComb and table of probabilities (given by algorithms)
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
#include "hhypchannel.h"
#include <iostream>
#include "hpidphysicsconstants.h"

using namespace std;

ClassImp(HHypList)
//--------------------------- constructors -------------------------------
  HHypList::HHypList(HHypList * OldList, Int_t IdxHypKine)
{
  // Constructor
  // ===========
  //
  // Input parameters:
  //       OldList    : pointer to the parent HHypList object.
  //                    New HHypList inherits HHypComb container from OldList
  //                    also fprobalg values are copied
  //       IdxHypKine : Index to HHypKine container; put -1 to have the same as the OldList has
  //                    (if does not have the new HHypList will not have too)
  //
  p_hypcomb = NULL;
  c_nvaluealg = 0;

  if (OldList != NULL) {
    // set HHypComb
    setIdxHypComb(OldList->getIdxHypComb());
    HCategory      *combCat =
      gHades->getCurrentEvent()->getCategory(catHypComb);

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

    Int_t           Ncomb = getNcomb();

    if (Ncomb != 0) {
      //now I copy the old fProbAlg to the new one
      //this I do after prepareHypList(), since here fProbComb has been
      //used [Ingo]
      for (Int_t Icomb = 0; Icomb < Ncomb; Icomb++) {
        setProbAlg(Icomb, OldList->getProbAlg(Icomb));
      }
      // set the pointer to the oldlist
      HCategory      *combHyp =
        gHades->getCurrentEvent()->getCategory(catHypList);

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
  // Constructor
  // ===========
  //
  // Input parameters:
  //       HypComb    : pointer to the HHypComb object.
  //                    New HHypList base on the HypComb
  //       IdxHypKine : Index to HHypKine container; put -1 to have the same as the OldList has
  //                    (if does not have the new HHypList will not have too)

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
  // Constructor
  // ===========
  //
  // Input parameters:
  //             Idx_HypComb : index to HHypComb container
  //             IdxHypKine  : index to HHypKine container
  //             see previous constructors

  p_hypcomb = NULL;
  setIdxHypComb(Idx_HypComb);
  setIdxHypKine(IdxHypKine);

  HCategory      *combCat = gHades->getCurrentEvent()->getCategory(catHypComb);

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

//-------------------------------------------------------------------------
Bool_t HHypList::Join(HHypList * addme)
{
#warning "Join can ONLY be used DIRECTLY AFTER the Filler!!!!!"
  if (getNpart() != addme->getNpart()) {
    Error("Join", "The NUMBER of particles has to be the same!");
    return kFALSE;
  }
  if (addme->getNcomb() <= 0) {
    Error("Join", "Nr of Combination <=0!");
    return kFALSE;
  }
  //
  Int_t oldncomb, ncomb;

  oldncomb = getNcomb();        // old Ncomb (comes from Comb)

  // Join the Combs
  HCategory      *combCat = gHades->getCurrentEvent()->getCategory(catHypComb);

//  cout << "join combs:" << endl;
  if (combCat != NULL) {
    HHypComb       *p_hypcombadd;

    p_hypcombadd = (HHypComb *) combCat->getObject(addme->getIdxHypComb());
    if (p_hypcombadd != NULL) {
      p_hypcomb->Join(p_hypcombadd);
    } else {
      cout << "\n HHypList::Join ERROR (p_hypcombadd is NULL) \n";
    }
  } else {
    cout << "\n HHypList::Join ERROR (combCat is NULL) \n";
  }
  // Joined!

  ncomb = getNcomb();           // new Ncomb (comes from Comb)

  fprobalg.Set(ncomb);

//  cout << "join probs:" << endl;

  // Probabilities...
  for (Int_t Icomb = 0; Icomb < addme->getNcomb(); Icomb++) {
    Int_t nci;

    nci = Icomb + oldncomb;
    setProbAlg(nci, addme->getProbAlg(Icomb));
  }

//  cout << "join UserValues:" << endl;

  // To make things easy we now make a writeUserValues()
  // this means no "cached" UserValues exist any more in the addme
  addme->writeUserValues();
  for (Int_t i = 0;; i++) {
    Int_t my_iter, valueID;
    Float_t value;

    if (!addme->ReadNumberedUserValues(i, my_iter, valueID, value))
      break;
    setUserValue(valueID, value, my_iter + oldncomb);
  }

//  print();
  return kTRUE;
}

void HHypList::Clear(Option_t * opt)
{
  // opt -> for future :)
  // set size of 3 TArray objects to 0
  fprobalg.Set(0);
  fvaluealg.Set(0);
  fuseralg.Set(0);
  nvaluealg = 0;
  c_nvaluealg = 0;
}

void HHypList::Restore(void)
{
  //restore pointer after reading from root file

  HCategory      *combCat = gHades->getCurrentEvent()->getCategory(catHypComb);

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
  // set id of the HHypList

  id_list = Id_list;
  return kTRUE;
}

//-------------------------------------------------------------------------
Int_t HHypList::getListId()
{
  // returns id of the HHypList

  return id_list;
}

//-------------------------------------------------------------------------
HHypList       *HHypList::getParent()
{
  // returns index of parent HHypList if exist
  // if does not returns NULL

  HCategory      *listCat = gHades->getCurrentEvent()->getCategory(catHypList);

  if (listCat != NULL) {
    return (HHypList *) listCat->getObject(idx_hyplist);
  }
  return NULL;
}

//-------------------------------------------------------------------------
Bool_t HHypList::prepareHypList()
{
  // set variables : Ncomb, fprobalg size
  // copy fprobalg from old HHypComb container to the actual one

  Int_t Ncomb = getNcomb();

  idx_hyplist = -1;
  nvaluealg = 0;
  c_nvaluealg = 0;

  if (Ncomb > 0) {
    // set the size of array for probabilieties
    fprobalg.Set(Ncomb);
    for (Int_t Icomb = 0; Icomb < Ncomb; Icomb++) {
      //by default, using fprobcomb
      //this can changed later by the algorithms
      setProbAlg(Icomb, p_hypcomb->getProbComb(Icomb));
    }
    return kTRUE;
  } else {
    cout << "ERROR: HHypList::prepareHypList (Ncomb is <=0) \n";
    return kFALSE;
  }
}

//-------------------------------------------------------------------------
void HHypList::setUserValue(Int_t valueID, Float_t value, Int_t my_iter)
{
  // set user value

  if (my_iter == -1)
    my_iter = comb_iter;

  if (c_nvaluealg >= MAX_USER_VALUES) {
    std::cerr << " setUserValue > MAX_USER_VALUES" << std::endl;
    return;
  }
  // cout << "setting " << valueID << " to " << value << endl;
  c_fvaluealg[c_nvaluealg] = value;
  c_fuseralg[2 * c_nvaluealg] = my_iter;
  c_fuseralg[2 * c_nvaluealg + 1] = valueID;
  c_nvaluealg++;
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

  // first read cached variables
  for (Int_t i = 0; i < c_nvaluealg; i++) {
    // cout << fuseralg[2*i] << ":" << fuseralg[2*i+1] << endl;
    if ((c_fuseralg[2 * i] == my_iter) && (c_fuseralg[2 * i + 1] == valueID)) {
      value = c_fvaluealg[i];
      // cout << "reading " << fvaluealg[i] << endl;
      return kTRUE;
    }
  }

  // now the arrayed vars
  for (Int_t i = 0; i < nvaluealg; i++) {
    // cout << fuseralg[2*i] << ":" << fuseralg[2*i+1] << endl;
    if ((fuseralg[2 * i] == my_iter) && (fuseralg[2 * i + 1] == valueID)) {
      value = fvaluealg[i];
      // cout << "reading " << fvaluealg[i] << endl;
      return kTRUE;
    }
  }
  if (idx_hyplist >= 0) {
    HHypList       *p = getParent();

    if (p) {
      // cout << "looking for parent" << endl;
      Bool_t retval = p->getUserValue(valueID, value, my_iter);

      return retval;
    }
  }
  return kFALSE;
}

Bool_t HHypList::ReadNumberedUserValues(Int_t i, Int_t & my_iter,
                                        Int_t & valueID, Float_t & value)
{
  // now the arrayed vars
  if (i < 0 || i >= nvaluealg)
    return kFALSE;

  my_iter = fuseralg[2 * i];
  valueID = fuseralg[2 * i + 1];
  value = fvaluealg[i];

  return kTRUE;
}

//-------------------------------------------------------------------------
Bool_t HHypList::writeUserValues(void)
{
  // copy values from c_fuseralg to fuseralg
  //         and from c_fvaluealg to fuseralg
  // from "cached" variables to dynamic Arrays

  if (c_nvaluealg == 0)
    return kFALSE;

  if (nvaluealg + c_nvaluealg > MAX_USER_VALUES) {
    std::cerr << " writeUserValues > MAX_USER_VALUES" << std::endl;
    return kFALSE;
  }

  fvaluealg.Set(nvaluealg + c_nvaluealg);
  fuseralg.Set((nvaluealg + c_nvaluealg) * 2);
  for (Int_t i = 0; i < c_nvaluealg; i++) {
    fvaluealg[nvaluealg + i] = c_fvaluealg[i];
    // cout << "copy " << fvaluealg[i] << endl;
    fuseralg[2 * (nvaluealg + i)] = c_fuseralg[2 * i];
    fuseralg[2 * (nvaluealg + i) + 1] = c_fuseralg[2 * i + 1];
  }
  nvaluealg += c_nvaluealg;
  c_nvaluealg = 0;

  return kTRUE;
}

//-------------------------------------------------------------------------
HPidTrackCand   *HHypList::getPidTrackCand(Int_t Ipart)
{
  // returns HPidTrackCand for certain particle position
  // iterates over combination !!!

  Int_t idx = getIdxPidTrackCand(Ipart);

  if (idx < 0)
    return NULL;
  HPidTrackCand   *pTrackCand = NULL;
  HCategory      *pidtrackcandCat =
    gHades->getCurrentEvent()->getCategory(catPidTrackCand);

  pTrackCand = (HPidTrackCand *) pidtrackcandCat->getObject(idx);
  return pTrackCand;
}

//-------------------------------------------------------------------------
HPidTrackCand   *HHypList::getPidTrackCand(Char_t *name, Int_t num)
{
  // returns HPidTrackCand for certain particle position
  // iterates over combination !!!

  Int_t idx = getIdxPidTrackCand(name,num);

  if (idx < 0)
    return NULL;
  HPidTrackCand   *pTrackCand = NULL;
  HCategory      *pidtrackcandCat =
    gHades->getCurrentEvent()->getCategory(catPidTrackCand);

  pTrackCand = (HPidTrackCand *) pidtrackcandCat->getObject(idx);
  return pTrackCand;
}

//-------------------------------------------------------------------------
Int_t HHypList::getNcomb()
{
  // returns number of possible combination
  if( p_hypcomb==0){
    Error("getNcomb","Internal Error: HypList with HypComb==0");
    print();
    return(0);
  }
  return p_hypcomb->getNcomb();
}

//-------------------------------------------------------------------------
Int_t HHypList::getNpart()
{
  // returns number of particles in each single combination
  // this is the size of the array, not the number of real valid tracks!
  // use getNvalid() to get that number

  return p_hypcomb->getNpart();
}

//-------------------------------------------------------------------------
Int_t HHypList::getNvalid(Int_t Icomb)
{
  // returns number of VALID particles in this combination
  // this can be smaller that size of the array given by getNpart()

  return p_hypcomb->getNvalid(Icomb);
}

//-------------------------------------------------------------------------
Int_t HHypList::getNvalid(void)
{
  // returns number of VALID particles in this combination
  // this can be smaller that size of the array given by getNpart()

  if ((comb_iter >= p_hypcomb->getNcomb()) || (comb_iter < 0)) {
    iterstatus = kFALSE;
    return -1;
  }
  return getNvalid(comb_iter);
}

//-------------------------------------------------------------------------
Float_t HHypList::getProbComb(Int_t Icomb)
{
  // returns probability of given combination

  return p_hypcomb->getProbComb(Icomb);
}

//-------------------------------------------------------------------------
Int_t HHypList::getOrder(void)
{
  // returns integer from 1 to NConb
  // 1 - for the combination with the highest probability
  // Note: same Probability will result in same order number!!!

  Int_t better = 0;

  if ((comb_iter >= p_hypcomb->getNcomb()) || (comb_iter < 0)) {
    iterstatus = kFALSE;
    return -1;
  }
  for (Int_t i = 0; i < p_hypcomb->getNcomb(); i++) {
    if (getProbAlg(i) > getProbAlg())
      better++;
  }
  return better + 1;
}

//-------------------------------------------------------------------------
TLorentzVector HHypList::getTLorentzVector(Int_t Icomb, Int_t Ipart)
{
  // returns TLorentzVector for given cell
  //      x,y,z momentum components are taken from HPidTrackCand object
  //      mass taken according to assumed pid of the particle
  //     if kine container exist take momenta from this container

  TLorentzVector Vect;

  if (getIdxHypKine() == -1) {
    HPidTrackCand   *pTrackCand = NULL;

    HCategory      *pidtrackcandCat =
      gHades->getCurrentEvent()->getCategory(catPidTrackCand);

    if (pidtrackcandCat != NULL) {
      pTrackCand =
        (HPidTrackCand *) pidtrackcandCat->getObject(getIdxPidTrackCand(Icomb, Ipart));

      if (pTrackCand != NULL) {
        Double_t Mass = HPidPhysicsConstants::mass(getPid(Icomb, Ipart));

//        TVector3 Vect3;
//        Vect = (TLorentzVector) (*pTrackCand);
//        Vect3 = Vect.Vect();
//        Vect.SetVectM(Vect3, Mass);
        TVector3 Vect3;
				Vect3.SetMagThetaPhi(
					pTrackCand->getTrackData()->getMomenta(ALG_RUNGEKUTTA),
					pTrackCand->getTrackData()->getRKTheta()*TMath::DegToRad(),
					pTrackCand->getTrackData()->getRKPhi()*TMath::DegToRad()
				);

        Vect.SetVectM(Vect3, Mass);
      } else {
        cout << "ERROR: HHypList::getTLorentzVector (pTrackCand is NULL) \n";
      }
    } else {
      cout << "ERROR: HHypList::getTLorentzVector (pidtrackcandCat is NULL) \n";
    }
  } else {
    HHypKine       *HypKine = NULL;

    HCategory      *hypkineCat =
      gHades->getCurrentEvent()->getCategory(catHypKine);

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
TLorentzVector HHypList::getTLorentzVector(Int_t num)
{
  // returns TLorentzVector for given num (number of particle)
  // iterates over combinations
  //      x,y,z momentum components are taken from HPidTrackCand object
  //      mass taken according to assumed pid of the particle
  //      if kine container exist take momenta from this container

  if ((comb_iter >= p_hypcomb->getNcomb()) || (comb_iter < 0)) {
    iterstatus = kFALSE;
    return TLorentzVector();
  }
  return getTLorentzVector(comb_iter, num);
}

//-------------------------------------------------------------------------
TLorentzVector HHypList::getTLorentzVector(Char_t *name, Int_t num)
{
  // returns TLorentzVector for given name of particle (i.e "p")
  // num is number in order : use 1 for first "p" , 2 for secound ....
  // iterates over combinations
  //      x,y,z momentum components are taken from HPidTrackCand object
  //      mass taken according to assumed pid of the particle
  //      if kine container exist take momenta from this container

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

//-------------------------------------------------------------------------
Int_t HHypList::getIdxPidTrackCand(Int_t Icomb, Int_t Ipart)
{
  // returns PidTrackCand object index for certain cell

  return p_hypcomb->getIdxPidTrackCand(Icomb, Ipart);
}

//-------------------------------------------------------------------------
Int_t HHypList::getIdxPidTrackCand(Int_t Ipart)
{
  // returns index to HPidTrackCand for certain particle position
  // iterates over combination

  if ((comb_iter >= p_hypcomb->getNcomb()) || (comb_iter < 0)) {
    iterstatus = kFALSE;
    return -1;
  }
  return getIdxPidTrackCand(comb_iter, Ipart);
}

//-------------------------------------------------------------------------
Int_t HHypList::getIdxPidTrackCand(Char_t *name, Int_t num)
{
  // returns index to HPidTrackCand for given particle name and number in order
  // iterates over combination

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
    return getIdxPidTrackCand(comb_iter, part_counter);
  }
  iterstatus = kFALSE;
  return -1;
}

//-------------------------------------------------------------------------
Bool_t HHypList::isValidPart(Int_t Icomb, Int_t Ipart)
{
  // returns if PidTrackCand Index is valid >=0

  return (p_hypcomb->getIdxPidTrackCand(Icomb, Ipart)>=0);
}

//-------------------------------------------------------------------------
Bool_t HHypList::isValidPart(Int_t Ipart)
{
  // returns index to HPidTrackCand for certain particle position
  // iterates over combination

  if ((comb_iter >= p_hypcomb->getNcomb()) || (comb_iter < 0)) {
    iterstatus = kFALSE;
    return false;
  }
  return isValidPart(comb_iter, Ipart);
}

Bool_t HHypList::initcopyMomentum(void)
{
  // Inits (and if necs. copies) momentum
  // This fcuntion should be called at the beginning of every
  // algo which uses setMomentum
  // if not there is the *possibility* that one algo is
  // overwriting the momentum another algo might need

  HHypKine       *HypKine = NULL;
  HCategory      *hypkineCat =
    gHades->getCurrentEvent()->getCategory(catHypKine);

  if (hypkineCat != NULL) {
    HLocation locDummy;

    if (getIdxHypKine() == -1) {        //MAKE Kine
//      cout << ">>> initcopyMomentum: MAKE Kine" << endl;

      HypKine = (HHypKine *) hypkineCat->getNewSlot(locDummy, &idx_hypkine);
      if (HypKine != NULL)
        HypKine = new(HypKine) HHypKine(idx_hypcomb);
      else
        return kFALSE;

    } else {                    // COPY kine
//      cout << ">>> initcopyMomentum: COPY Kine" << endl;
      HHypKine       *OldHypKine = NULL;

      OldHypKine = (HHypKine *) hypkineCat->getObject(getIdxHypKine());

      if (OldHypKine != NULL) {
        HypKine = (HHypKine *) hypkineCat->getNewSlot(locDummy, &idx_hypkine);
        if (HypKine != NULL)
          HypKine = new(HypKine) HHypKine(OldHypKine);  // Init and Copy
        else
          return kFALSE;

        // Now copy EVERYTHING(!)
        // HypKine->setMomentum(Icomb, Ipart, OldHypKine->getMomentum(Icomb, Ipart));
      } else {
        cout << "ERROR: HHypList::initCopyMomentum (OldHypKine is NULL) \n";
        return kFALSE;
      }
    }
  } else {
    cout << "ERROR: HHypList::initcopyMomentum (hypkineCat is NULL) \n";
    return kFALSE;
  }
  return kTRUE;
}

//-------------------------------------------------------------------------
Bool_t HHypList::setMomentum(Int_t Icomb, Int_t Ipart, TVector3 vect)
{
  // set momentum of particle for given cell (in HHypKine)
  // vect=(px,py,pz) (HHypKine is created if does not exist)

  if (getIdxHypKine() == -1)    //MAKE Kine
  {

// Remark by Bjoern Spruck:
// This behaviour is NOT what we want, because if the momentum is
// set in another algorithm (f.e. first eloss and then kinefit), it is
// overwritten/modified for ALL trees which evolute from the point of
// the first modification of momentum
// Therefor a "set momentum" HAS TO create new objects every time!!!
//
// One bad thing... waste of memory/disk
// if an object is not used anymore it will still survive because
// i cannot check here if it is still in use by another analysis branch

    Error("HHypList::setMomentum",
          "Please use initcopyMomentum BEFORE calling any setMomentum. Please update Code from CVS!");
    exit(0);
/*
    HHypKine *HypKine = NULL;

    HCategory *hypkineCat = gHades->getCurrentEvent()->getCategory(catHypKine);

    if (hypkineCat != NULL) {
      HLocation locDummy;

      HypKine = (HHypKine *) hypkineCat->getNewSlot(locDummy, &idx_hypkine);
      if (HypKine != NULL)
        HypKine = new(HypKine) HHypKine(idx_hypcomb);
      else
        return kFALSE;
      HypKine->setMomentum(Icomb, Ipart, vect);
    } else {
      cout << "ERROR: HHypList::setMomentum (hypkineCat is NULL) \n";
    }*/
  } else                        //USE kine
  {
    HHypKine       *HypKine = NULL;

    HCategory      *hypkineCat =
      gHades->getCurrentEvent()->getCategory(catHypKine);

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
Bool_t HHypList::setMomentum(Int_t Ipart, TVector3 vect)
{
  // set momentum of particle for given particle number (in HHypKine)
  // vect=(px,py,pz) (HHypKine is created if does not exist)
  // iterate over combinations

  if ((comb_iter >= p_hypcomb->getNcomb()) || (comb_iter < 0)) {
    iterstatus = kFALSE;
    return kFALSE;
  }
  return setMomentum(comb_iter, Ipart, vect);
}

//-------------------------------------------------------------------------
Bool_t HHypList::setMomentum(Char_t *name, Int_t num, TVector3 vect)
{
  // set momentum of particle for given particle name (in HHypKine) and number in order
  // i.e num=1 for first "p" , num=2 for secound "p"
  // vect=(px,py,pz) (HHypKine is created if does not exist)
  // iterate over combinations

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
TVector3 HHypList::getMomentum(Int_t Icomb, Int_t Ipart)
{
  // return TVector3 (px,py,pz) - 3-momentum components
  // if HHypKine exist take TVector3 from HHypKine

  TVector3 Vect3;

  if (getIdxHypKine() == -1) {
    HPidTrackCand   *pTrackCand = NULL;

    HCategory      *pidtrackcandCat =
      gHades->getCurrentEvent()->getCategory(catPidTrackCand);

    if (pidtrackcandCat != NULL) {
      pTrackCand =
        (HPidTrackCand *) pidtrackcandCat->getObject(getIdxPidTrackCand(Icomb, Ipart));

      if (pTrackCand != NULL) {
//        Vect3 = ((TLorentzVector) (*pTrackCand)).Vect();
					Vect3.SetMagThetaPhi(
						pTrackCand->getTrackData()->getMomenta(ALG_RUNGEKUTTA),
						pTrackCand->getTrackData()->getRKTheta()*TMath::DegToRad(),
						pTrackCand->getTrackData()->getRKPhi()*TMath::DegToRad()
					);
      }
    }
  } else {
    HHypKine       *HypKine = NULL;

    HCategory      *hypkineCat =
      gHades->getCurrentEvent()->getCategory(catHypKine);

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
TVector3 HHypList::getMomentum(Int_t num)
{
  // return TVector3 (px,py,pz) - 3-momentum components
  // if HHypKine exist take TVector3 from HHypKine
  // num : x position of paticle in HypComb table
  // iterate over combination

  if ((comb_iter >= p_hypcomb->getNcomb()) || (comb_iter < 0)) {
    iterstatus = kFALSE;
    return TVector3();
  }
  return getMomentum(comb_iter, num);
}

//-------------------------------------------------------------------------
TVector3 HHypList::getOrigMomentum(Int_t Icomb, Int_t Ipart)
{
  // return TVector3 (px,py,pz) - 3-momentum components

  TVector3 Vect3;

  {
    HPidTrackCand   *pTrackCand = NULL;

    HCategory      *pidtrackcandCat =
      gHades->getCurrentEvent()->getCategory(catPidTrackCand);

    if (pidtrackcandCat != NULL) {
      pTrackCand =
        (HPidTrackCand *) pidtrackcandCat->getObject(getIdxPidTrackCand(Icomb, Ipart));

      if (pTrackCand != NULL) {
				Vect3.SetMagThetaPhi(
					pTrackCand->getTrackData()->getMomenta(ALG_RUNGEKUTTA),
					pTrackCand->getTrackData()->getRKTheta()*TMath::DegToRad(),
					pTrackCand->getTrackData()->getRKPhi()*TMath::DegToRad()
				);
      }
    }
  }
  return Vect3;
}

//-------------------------------------------------------------------------
TVector3 HHypList::getOrigMomentum(Int_t num)
{
  // return TVector3 (px,py,pz) - 3-momentum components
  // if HHypKine exist take TVector3 from HHypKine
  // num : x position of paticle in HypComb table
  // iterate over combination

  if ((comb_iter >= p_hypcomb->getNcomb()) || (comb_iter < 0)) {
    iterstatus = kFALSE;
    return TVector3();
  }
  return getOrigMomentum(comb_iter, num);
}

//-------------------------------------------------------------------------
Float_t HHypList::getBeta(Char_t *name, Int_t num)
{
  // returns beta for given name of particle (i.e "p")
  // num is number in order : use 1 for first "p" , 2 for secound ....
  // iterates over combinations

  if ((comb_iter >= p_hypcomb->getNcomb()) || (comb_iter < 0)) {
    iterstatus = kFALSE;
    return 0;
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
    return getBeta(comb_iter, part_counter);
  }
  iterstatus = kFALSE;
  return 0;
}

//-------------------------------------------------------------------------
Float_t HHypList::getBeta(Int_t Icomb, Int_t Ipart)
{
  // return beta of given particle according to applied algorithm
  // for particle on position Ipart in combination Icomb

  Float_t beta = 0;

  HPidTrackCand  *pidTrackCand = NULL;

  HCategory      *pidtrackcandCat =
    gHades->getCurrentEvent()->getCategory(catPidTrackCand);

  if (pidtrackcandCat != NULL) {
    pidTrackCand =
      (HPidTrackCand *) pidtrackcandCat->getObject(getIdxPidTrackCand(Icomb, Ipart));
		if (pidTrackCand) {

			beta =
				fabs(pidTrackCand->getTrackData()->getBeta(ALG_RUNGEKUTTA));
			Float_t offset;

			if (getUserValue(DELTATOF_OFFSET, offset, Icomb)) {
				//Recalculate the beta according to DeltaTof
				Double_t path =
					pidTrackCand->getTrackData()->getPathLength(ALG_RUNGEKUTTA);
				//Remember OFFSET was defined to ExpBeta
				Double_t t_exp = path / (beta * 299.792458);

				beta = path / ((t_exp + offset) * 299.792458);
				//cout << "path: " <<path << " offset : " << offset << " beta new: " << beta << " beta old: "<<pTrackCand->getTrackData()->getBeta(ALG_RUNGEKUTTA) << endl;
				//beta = path/((t_exp) * 299.792458);
			}
		}
  }
  return beta;
}

//-------------------------------------------------------------------------
Float_t HHypList::getBeta(Int_t num)
{
  // return beta of given particle according to applied algorithm
  // for particle on position Ipart
  // iterate over combination

  if ((comb_iter >= p_hypcomb->getNcomb()) || (comb_iter < 0)) {
    iterstatus = kFALSE;
    return 0;
  }

  return getBeta(comb_iter, num);
}

//-------------------------------------------------------------------------
Bool_t HHypList::setProbAlg(Int_t Icomb, Float_t Prob)
{
  // set probability Prob given by algorithm for combination number Icomb

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
  // returns probability given by algotrythm for  combination number Icomb

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
  // returns probability given by algorythm for combination
  // iterate over combination

  if ((comb_iter >= p_hypcomb->getNcomb()) || (comb_iter < 0)) {
    iterstatus = kFALSE;
    return -1.;
  }
  return getProbAlg(comb_iter);
}

//-------------------------------------------------------------------------
void HHypList::removeComb(void)
{
  // Use this function inside a selector to remove current combination.

  resetProbAlg(-1.);
}

//-------------------------------------------------------------------------
void HHypList::KeepBestComb(void)
{
  // Remove all combinations, keep only the best one

  Double_t bestprob = -1, bestcomb = -1;

  for (Int_t i = 0; i < p_hypcomb->getNcomb(); i++) {
    if (getProbAlg(i) > bestprob) {
      bestprob = getProbAlg(i);
      bestcomb = i;
    }
  }
  if (bestcomb < 0)
    return;
  for (Int_t i = 0; i < p_hypcomb->getNcomb(); i++) {
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
  // returns assumed pid for given cell

  return p_hypcomb->getPid(Icomb, Ipart);
}

//-------------------------------------------------------------------------
Int_t HHypList::getPid(Int_t num)
{
  // returns assumed pid for given num (number of particle)
  // iterates over combinations

  if ((comb_iter >= p_hypcomb->getNcomb()) || (comb_iter < 0)) {
    iterstatus = kFALSE;
    return -1;
  }
  return getPid(comb_iter, num);
}

//-------------------------------------------------------------------------
Bool_t HHypList::setIdxHypComb(Int_t Idx)
{
  // set index to the HHypComb

  idx_hypcomb = Idx;
  return kTRUE;
}

//-------------------------------------------------------------------------
Int_t HHypList::getIdxHypComb()
{
  // returns index to the HHypComb

  return idx_hypcomb;
}

//-------------------------------------------------------------------------
Bool_t HHypList::setIdxHypKine(Int_t Idx)
{
  // set index to the HHypKine

  idx_hypkine = Idx;
  return kTRUE;
}

//-------------------------------------------------------------------------
Int_t HHypList::getIdxHypKine()
{
  // returns index to the HHypKine

  return idx_hypkine;
}

//-------------------------------------------------------------------------
Bool_t HHypList::CombIteratorReset(void)
{
  // reset comb_iter

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
void HHypList::resetIterStatus(void)
{
 	iterstatus = kTRUE;
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

  if ((comb_iter < p_hypcomb->getNcomb()) && (getProbAlg(comb_iter) >= 0)){
  	resetIterStatus();
		// set iterstatus = kTRUE seems to be very important!
		// else: if one combination fails, it will not be reset for the next one
		// very important for mixed lists
    return kTRUE;
  }else
    return kFALSE;
}

//-------------------------------------------------------------------------
Bool_t HHypList::print()
{
// use while debuging to print HHypComb table with indexes and pids
  cout << "HypList::Print()" << endl;
  HHypChannel a;
  cout<<"id_list: "<<id_list << " : " << a.Get(id_list) <<endl;
  cout<<"idx_hyplist: "<<idx_hyplist << endl;
  cout<<"idx_hypcomb: "<<idx_hypcomb << endl;
  cout<<"idx_hypkine: "<<idx_hypkine << endl;
  cout << "Index to pTrackCand and ProbComb and ProbAlg\n";        // Idx table
  for (Int_t i = 0; i < getNcomb(); i++) {
    for (Int_t j = 0; j < getNpart(); j++) {
      cout << "\t" << getIdxPidTrackCand(i, j);
    }
    cout << "\t" << getProbComb(i) << "\t" << getProbAlg(i) << endl;
  }

  cout << "only PIDs and missing pid\n";        // Pid table
  for (Int_t i = 0; i < getNcomb(); i++) {
    for (Int_t j = 0; j < getNpart(); j++) {
      cout << "\t" << getPid(i, j);
    }
    Float_t mpid;

    if (!getUserValue(FILLER_MISSING_PID, mpid, i))
      mpid = -999;
    cout << "\t" << mpid << "\n";
  }
  cout << "\n";
  if(p_hypcomb) p_hypcomb->print();
  return kTRUE;
}

Float_t HHypList::getNrOfFakes(void)
{
  Float_t fakes;
#warning "Calculation of Fakes only correct, if UserValue FILLER_VALID_PIDTRACKS is filled correctly, which was not the case before and needs to be checked in inclusive filler/multi channel filler"
  // fakes is "valid PID tracks" minus "valid" particles in list
  if (getUserValue(FILLER_VALID_PIDTRACKS, fakes)){
		fakes-=getNvalid();
  }else{
    fakes=-1;// if FILLER_VALID_PIDTRACKS is not set I have no chance to get # of fakes
  }
  return(fakes);
}

//-------------------------------------------------------------------------
HCategory      *HHypList::buildLinearCat(Text_t * classname)
{
  // build linear category

  HLinearCategory *category = NULL;
  Int_t size = 1000;

  category = new HLinearCategory(classname, size);
  category->setDynamicObjects(kTRUE);

  return category;
}

#if 1
//-------------------------------------------------------------------------
void HHypList::Streamer(TBuffer & R__b)
{
  // Stream an object of class HHypList.
  if (R__b.IsReading()) {
    Version_t R__v = R__b.ReadVersion();

    if (R__v == 1) {            // buggy old version, no ncomb
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
    } else if (R__v == 2) {

      TObject::Streamer(R__b);
      Int_t ncomb = 0;

      R__b >> idx_hypcomb;
      R__b >> idx_hypkine;
      R__b >> idx_hyplist;
      R__b >> id_list;
      R__b >> nvaluealg;
      R__b >> ncomb;
      fprobalg.Set(ncomb);
      fvaluealg.Set(nvaluealg);
      fuseralg.Set(2 * nvaluealg);
      for (Int_t i = 0; i < ncomb; i++) {
        R__b >> fprobalg[i];
      }
      for (Int_t i = 0; i < nvaluealg; i++) {
        R__b >> fvaluealg[i];
      }
      for (Int_t i = 0; i < (2 * nvaluealg); i++) {
        R__b >> fuseralg[i];
      }
    } else {
      cout << "HHypList::Streamer: version is not 1 or 2" << endl;
    }
  } else {
    R__b.WriteVersion(HHypList::IsA());
    TObject::Streamer(R__b);
    R__b << idx_hypcomb;
    R__b << idx_hypkine;
    R__b << idx_hyplist;
    R__b << id_list;
    R__b << nvaluealg;
    R__b << getNcomb();         //important to know this
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
#endif
