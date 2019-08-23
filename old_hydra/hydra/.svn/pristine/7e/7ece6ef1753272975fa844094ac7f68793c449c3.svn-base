//*-- Author : B. Spruck
//*-- Modified : 24 Apr 2008
////////////////////////////////////////////////////////////////////////
//
// HHypFWVars
//
//
////////////////////////////////////////////////////////////////////////

using namespace std;

#include <stdlib.h>
#include <iostream>
#include "hhypFWVars.h"
#include "hypinfodef.h"
#include "walldef.h"
#include "hwallhit.h"

ClassImp(HHypFWVars)

HHypFWVars::HHypFWVars(Char_t *name_i, Option_t par[])
:HHypBaseAlgorithm(name_i,par)
{

}

HHypFWVars::~HHypFWVars()
{
}

Bool_t HHypFWVars::execute()
{
	// Resetting the list and start looping over the combinations
	// Loop is only done over the VALID combinations
	mylist->CombIteratorReset();
	while (mylist->CombIterator())
	{

		HWallHit   *pWallHit = NULL;

		while (((pWallHit = (HWallHit *) m_pContItWallHit->Next()) != NULL)) {
			mylist->setUserValue(FW_THETA, pWallHit->getTheta());
			mylist->setUserValue(FW_PHI, pWallHit->getPhi());
		}

	}                             //END Iterator

	if (exitIdx > -1)
		return kTRUE;
	return kFALSE;
}

Bool_t HHypFWVars::init()
{

  //---------- Initialization of HWallHit Container -----------------------
  m_pContItWallHit = NULL;         // Iterator
  m_pContCatWallHit = NULL;        // Category

  if ((m_pContCatWallHit =
       gHades->getCurrentEvent()->getCategory(catWallHit)) == NULL) {
    Error("init", "Cannot get catWallHit cat");
    return kFALSE;
  }
  m_pContItWallHit = (HIterator *) m_pContCatWallHit->MakeIterator();
  if (m_pContItWallHit == NULL) {
    Error("init", "Cannot get catWallHit Iterator");
    return kFALSE;
  }
  //-----------------------------------------------------------------------

	return kTRUE;
}

Bool_t HHypFWVars::reinit()
{
	return kTRUE;
}

Bool_t HHypFWVars::finalize()
{
	return kTRUE;
}
