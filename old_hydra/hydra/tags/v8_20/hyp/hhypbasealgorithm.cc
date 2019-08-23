//*-- AUTHOR : Tiago Perez
//*-- Modified : 20/04/2005 by I. Froehlich
//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////
//
// HHypBaseAlgorithm
//
// This is an abstract base class for all the algorithms which
// are used in the HYP project. It privides some basic
// data members, common functions and the standard methods
// init, reinit, execute and finalize.
//
// In addition, base_init,  base_reinit,  base_execute and base_finalize
// are provided which should be called by the HHypReconstructor for
// common features.
//
////////////////////////////////////////////////////////////////////////

using namespace std;

#include <iostream>
#include <list>

#include "TObjString.h"

#include "hhypbasealgorithm.h"

ClassImp(HHypBaseAlgorithm)

  HHypBaseAlgorithm::HHypBaseAlgorithm(Char_t *name_i, Option_t par[])
{
  // Constructor
  // ===========
  //
  // Input parameters:
  //     name_i: Pointer to the algorithm name
  //     par:    Option, which can be parsed by the implementation

  algoName = name_i;
  mylist = NULL;
  beam = NULL;
  histofile = NULL;
  options = par;
 
  // standard option: If skip is valid for this alg (Micro-Hyp-DST)
  skip = kFALSE;

  skip = (GetOpt("SKIPEVENIFEXIST") != NULL);
}

HHypBaseAlgorithm::~HHypBaseAlgorithm()
{
}

TString *HHypBaseAlgorithm::GetOpt(TString st) 
{
  //scan option list
  //try to extract option paramter
  //returns NULL if option was not set

  TString *retval=NULL;

  st.ToUpper();
  TObjArray * bla = options.Tokenize(",");
  
  for (Int_t i=0; i<bla->GetEntriesFast(); i++) {
    //cout << delme->GetString() << endl;
    TString str = ((TObjString *)bla->At(i))->String();

    //extract option
    TObjArray * bla2 = str.Tokenize("=");
    //cout << option->GetString() << endl;
    TString optionstr = ((TObjString *)bla2->At(0))->String();
    
    //check if option matches
    optionstr.ToUpper();
    if (strstr(st.Data(), optionstr.Data()) != NULL) {
      //heureka, it matches...
      if (bla2->GetEntriesFast() == 1) { //no parameter
        static TString empty("");
        //cout << optionstr.Data() << endl;
        retval=&empty;
      }else if (bla2->GetEntriesFast() == 2) { //parameter
        //cout << "found " << ((TObjString *)(*bla2)[1])->GetString() << endl;
        static TString val;
        val=((TObjString *)bla2->At(1))->String();
        retval=&val;
      }else{
      Error("GetOpt", "error in option list");
    }
      delete bla2;
      break;
  }
    delete bla2;
  }
  delete bla;
  
  return retval;
}

HHypList *HHypBaseAlgorithm::GetList(void)
{
  // Return the result HHypList of the specific algorithm

  if (mylist)
    return mylist;
  if (exitIdx < 0)
    return NULL;

  HCategory *listCat = gHades->getCurrentEvent()->getCategory(catHypList);

  mylist = (HHypList *) listCat->getObject(exitIdx);

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
  return (sourceIdx);
}

Int_t HHypBaseAlgorithm::GetExitChannelAlgIdx()
{
  return (exitIdx);
}

Char_t *HHypBaseAlgorithm::GetName()
{
  return (algoName);
}

Bool_t HHypBaseAlgorithm::base_execute()
{
  // base_execute should be executed by the reconstructor
  // BEFORE execute is called
  // In the case, when:
  //    1. the algorithm is based on another algorithm AND 
  //    2. it should have an exit object
  // the exit object HHypList is created
  //
  // Return code: kFALSE or kTRUE

  //cout << "exe " << algoName << endl;



  if ((sourceIdx >= 0) && (exitList >= 0))
    // algorithm IS based on other algo
  {
    //return kTRUE;

    HLocation locDummy;

    // making clone out of HypList
    // this means HypComb and HypKine are kept (see HypList constructor for details

    HCategory *listCat = gHades->getCurrentEvent()->getCategory(catHypList);
    HHypList *sourcelist = (HHypList *) listCat->getObject(sourceIdx);

    mylist = (HHypList *) listCat->getNewSlot(locDummy, &exitIdx);
    if (mylist != 0)
      mylist = new(mylist) HHypList(sourcelist);

    else
      return kFALSE;

    mylist->setListId(exitList);

  } else if ((sourceIdx >= 0) && (exitList < 0)) {
    // this is the case for e.g. the projector
    mylist = NULL;

    HCategory *listCat = gHades->getCurrentEvent()->getCategory(catHypList);

    mylist = (HHypList *) listCat->getObject(sourceIdx);

    if (!mylist)
      return kFALSE;
  } else {
    // no source... I can do nothing!
    mylist = NULL;
  }

  return kTRUE;
}

Bool_t HHypBaseAlgorithm::base_init()
{
  return kTRUE;
}

Bool_t HHypBaseAlgorithm::base_reinit()
{
  return kTRUE;
}

Bool_t HHypBaseAlgorithm::base_finalize()
{
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
