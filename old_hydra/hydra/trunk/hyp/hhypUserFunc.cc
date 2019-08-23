//*-- Author : B. Spruck
//*-- Modified : 24 Apr 2008
////////////////////////////////////////////////////////////////////////
//
// HHypUserFunc
//
// calls a user defined funcion for every valid combination in HypList
// purpose: easy and fast testing of new algorithms
//
// usage:
// In fillHypDst.cc add a user function like:
// void TestFunction(HHypList *mylist,TLorentzVector *beam)
// {
//  Int_t npart;
// 	npart=mylist->getNvalid();
// 	cout << "Npart= "<<npart<<" : ";
// 	for(Int_t i=0; i<npart; i++){
// 		cout << mylist->getPid(i)<<", ";
// 	}
// 	cout <<endl;
//  // might also remove or modify combination (like mylist->removeComb();)
// }
//
// and add HHypUserFunc task to hyp-tasklist as follows:
// HHypUserFunc *f;
// f=new HHypUserFunc("test user func","");
// f->SetUserFunc(TestFunction);
// hyp->AddAlgorithm("inputchn",f,"outputchn");
//
////////////////////////////////////////////////////////////////////////

using namespace std;

#include <stdlib.h>
#include <iostream>
#include "hhypUserFunc.h"
#include "hypinfodef.h"

ClassImp(HHypUserFunc)

HHypUserFunc::HHypUserFunc(Char_t *name_i, Option_t par[])
:HHypBaseAlgorithm(name_i,par)
{
	userfunc=NULL;
}

HHypUserFunc::~HHypUserFunc()
{
}

Bool_t HHypUserFunc::execute()
{
	// Resetting the list and start looping over the combinations
	// Loop is only done over the VALID combinations
  if(userfunc){
		mylist->CombIteratorReset();
		while (mylist->CombIterator())
		{
			userfunc(mylist,beam);
		}                             //END Iterator
  }

	if (exitIdx > -1)
		return kTRUE;
	return kFALSE;
}

Bool_t HHypUserFunc::init()
{
  if(!userfunc){
		cerr << "Error in "<< algoName << ": userfunc is missing! " << endl;
		return kFALSE;
  }
	if (!beam) {
		cerr << "Error in "<< algoName << ": needs beam particle! " << endl;
		return kFALSE;
	}

	return kTRUE;
}

Bool_t HHypUserFunc::reinit()
{
	return kTRUE;
}

Bool_t HHypUserFunc::finalize()
{
	return kTRUE;
}
