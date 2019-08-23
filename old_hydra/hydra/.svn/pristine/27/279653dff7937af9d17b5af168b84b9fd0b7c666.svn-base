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
  s = par;
 
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

  st.ToUpper();
  TStringTokenize * s_t = (TStringTokenize*) &s;

  TObjArray * bla = s_t->TokenizeI(TString(","));
  //cout << bla -> GetEntriesFast() << endl;
  
  for (Int_t i=0; i<bla->GetEntriesFast(); i++) {
    TObjString * delme = (TObjString *)(*bla)[i];
    //cout << delme->GetString() << endl;
    TString str = delme->GetString();
    TStringTokenize * str_t = (TStringTokenize*) &str;

    //extract option
    TObjArray * bla2 = str_t->TokenizeI(TString("="));
    TObjString * option = (TObjString *)(*bla2)[0];
    //cout << option->GetString() << endl;
    TString optionstr = option->GetString();
    
    //check if option matches
    optionstr.ToUpper();
    if (strstr(st.Data(), optionstr.Data()) != NULL) {
      //heureka, it matches...
      if (bla2->GetEntriesFast() == 1) { //no parameter
	static TString empty("");
	//cout << optionstr.Data() << endl;
	return &empty;
      }
      if (bla2->GetEntriesFast() == 2) { //parameter
	//cout << "found " << ((TObjString *)(*bla2)[1])->GetString() << endl;
	static TString val(((TObjString *)(*bla2)[1])->GetString());
	return &val;
      }
      Error("GetOpt", "error in option list");
      return NULL;
    }
  }
  return NULL;
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

 TObjArray *TStringTokenize::TokenizeI(const TString &delim) const
{
   // This function is used to isolate sequential tokens in a TString.
   // These tokens are separated in the string by at least one of the
   // characters in delim. The returned array contains the tokens
   // as TObjString's. The returned array is the owner of the objects,
   // and must be deleted by the user.

   std::list<Int_t> splitIndex;

   Int_t i, start, nrDiff = 0;
   for (i = 0; i < delim.Length(); i++) {
      start = 0;
      while (start < Length()) {
         Int_t pos = Index(delim(i), start);
         if (pos == kNPOS) break;
         splitIndex.push_back(pos);
         start = pos + 1;
      }
      if (start > 0) nrDiff++;
   }
   splitIndex.push_back(Length());

   if (nrDiff > 1)
      splitIndex.sort();

   TObjArray *arr = new TObjArray();
   arr->SetOwner();

   start = -1;
   std::list<Int_t>::const_iterator it;
#ifndef R__HPUX
   for (it = splitIndex.begin(); it != splitIndex.end(); it++) {
#else
   for (it = splitIndex.begin(); it != (std::list<Int_t>::const_iterator) splitIndex.end(); it++) {
#endif
      Int_t stop = *it;
      if (stop - 1 >= start + 1) {
         TString tok = (*this)(start+1, stop-start-1);
         TObjString *objstr = new TObjString(tok);
         arr->Add(objstr);
      }
      start = stop;
   }

   return arr;
}
