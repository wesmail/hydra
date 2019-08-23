/***************************************************************************
 *   Author :  Tiago Perez <tiago.perez@exp2.physik.uni-giessen.de>        *
 *             II Physikalisches Institut                                  *
 *             Universitaet Giessen                                        *
 *           								   *
 *   Title  :  HHypBaseAlgorithm.cc                                        *
 *   Project:  HYDRA Event Hypothesys  (HADES)                             *
 *   Description:   						           *
 *            								   *
 *   Created  :   2005-01-05 Tiago Perez                                   *
 *   Modified :                                                            *
 *            								   *
 ***************************************************************************/

using namespace std;
#include <iostream>
#include "hhypbasealgorithm.h"

ClassImp(HHypBaseAlgorithm)

HHypBaseAlgorithm::HHypBaseAlgorithm(char *name_i)
{
  algoName = name_i;
  mylist = NULL;
  beam = NULL;
}

HHypBaseAlgorithm::~HHypBaseAlgorithm()
{
}

HHypList * HHypBaseAlgorithm::GetList(void){
  if (mylist) return mylist;
  if (exitIdx < 0) {
    cerr << "ERROR: HypList requested, but input is empty" << endl;
    return NULL;
  }
    
  HCategory* listCat=gHades->getCurrentEvent()->getCategory(catHypList);
  mylist = (HHypList*)listCat->getObject(exitIdx);

  return mylist;

}


Bool_t HHypBaseAlgorithm::SetSourceChannelAlgIdx(Int_t i_id)
{
    sourceIdx = i_id;
    return kTRUE;
}

Bool_t HHypBaseAlgorithm::SetExitChannelAlgIdx(Int_t e_id)
{
    exitIdx = e_id;
    return kTRUE;
}

Bool_t HHypBaseAlgorithm::SetExitList(Int_t e_list)
{
    exitList = e_list;
    return kTRUE;
}

Bool_t HHypBaseAlgorithm::SetInitList(Int_t i_list)
{
    initList = i_list;
    return kTRUE;
}


Int_t HHypBaseAlgorithm::GetSourceChannelAlgIdx()
{
    return(sourceIdx);
}

Int_t HHypBaseAlgorithm::GetExitChannelAlgIdx()
{
    return(exitIdx);
}

Char_t * HHypBaseAlgorithm::GetName()
{
  return(algoName);
}

Bool_t HHypBaseAlgorithm::base_execute()
{
  //cout << "execute: some basic stuff for: " << algoName<< endl;

  if ((sourceIdx >= 0) && (exitList >= 0))// algo based on other algo
    {
      HLocation locDummy;

      //cout << "making clone out of HypList" << endl;
      HCategory* listCat=gHades->getCurrentEvent()->getCategory(catHypList);
      HHypList* sourcelist = (HHypList*)listCat->getObject(sourceIdx);

      mylist = (HHypList*) listCat->getNewSlot(locDummy,&exitIdx);
      if ( mylist != 0 ) mylist = new(mylist) HHypList(sourcelist);

      else return kFALSE;

      mylist->setListId(exitList);

    }
  return kTRUE;
}


Bool_t HHypBaseAlgorithm::base_init()
{
  cout << "init: some basic stuff: " << algoName<< endl;
  return kTRUE;
}
Bool_t HHypBaseAlgorithm::base_reinit()
{
  cout << "reinit: some basic stuff: " << algoName<< endl;
  return kTRUE;
}

Bool_t HHypBaseAlgorithm::base_finalize()
{
  cout << "finalize: some basic stuff: " << algoName<< endl;
  return kTRUE;
}

Bool_t HHypBaseAlgorithm::init()
{
  cerr << "WARNING: init is abstract" << endl;
  return kFALSE;
}

Bool_t HHypBaseAlgorithm::reinit()
{
  cerr << "WARNING: reinit is abstract" << endl;
  return kFALSE;
}

Bool_t HHypBaseAlgorithm::execute()
{
  cerr << "WARNING: execute is abstract" << endl;
  return kFALSE;
}

Bool_t HHypBaseAlgorithm::finalize()
{
  cerr << "WARNING: finalize is abstract" << endl;
  return kFALSE;
}
