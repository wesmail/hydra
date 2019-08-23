//*-- Author : B. Spruck
//*-- Modified : 19 Sep 2006
//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////
//
// HHypRemoveAllButBest
//
// Removes all but the BEST Combination based on their probability
//
////////////////////////////////////////////////////////////////////////

using namespace std;

#include <stdlib.h>
#include <iostream>
#include "hhypRemoveAllButBest.h"
#include "hypinfodef.h"

ClassImp(HHypRemoveAllButBest)

HHypRemoveAllButBest::HHypRemoveAllButBest(Char_t *name_i, Option_t par[])
:HHypBaseAlgorithm(name_i,par)
{
}

HHypRemoveAllButBest::~HHypRemoveAllButBest()
{
}

Bool_t HHypRemoveAllButBest::execute()
{
	// Its really easy like that!

	mylist->KeepBestComb();

	if (exitIdx > -1)
		return kTRUE;
	return kFALSE;
}

Bool_t HHypRemoveAllButBest::init()
{
	return kTRUE;
}

Bool_t HHypRemoveAllButBest::reinit()
{
	return kTRUE;
}

Bool_t HHypRemoveAllButBest::finalize()
{
	return kTRUE;
}
