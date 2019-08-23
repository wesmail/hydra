//*-- Author : B. Spruck
//*-- Modified : 28/Feb/2007 B. Spruck
//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////
//
// HHypPEpEmPAlg
//
////////////////////////////////////////////////////////////////////////

using namespace std;

#include <stdlib.h>
#include <iostream>
#include "hhypPEpEmPAlg.h"
#include "hypinfodef.h"

ClassImp(HHypPEpEmPAlg)

HHypPEpEmPAlg::HHypPEpEmPAlg(Char_t *name_i, Option_t par[])
  :HHypBaseAlgorithm(name_i,par)
{
  // 3
  filler3 = NULL;
  // 3 + 1 fake
  filler3p1 = NULL;
  filler3m1 = NULL;
  // 3 + 2 fakes
  filler3p2 = NULL;
  filler3p1m1 = NULL;
  filler3m2 = NULL;
}

HHypPEpEmPAlg::~HHypPEpEmPAlg()
{
  if (filler3)
    delete filler3;

  if (filler3p1)
    delete filler3p1;
  if (filler3m1)
    delete filler3m1;
  
  if (filler3p2)
    delete filler3p2;
  if (filler3p1m1)
    delete filler3p1m1;
  if (filler3m2)
    delete filler3m2;
}

Bool_t HHypPEpEmPAlg::execute()
{
	// In principle, the first question should be:
	// How many particles are in that events!
	// Depending on that the appopriate filler should be called.
	// Doing it in the following way is a waste of cpu time ...

	exitIdx = filler3->execute();

	// 3+1 fake
	if( use_3_1){
		// if no 3p 1n then try next filler
		if(exitIdx==ERROR_IDX) exitIdx = filler3m1->execute();
		// if no 3p 2n then try next filler
		if(exitIdx==ERROR_IDX) exitIdx = filler3p1->execute();
		// if no 4p 1n then forget about it
	}

	// 3+2 fakes
	if( use_3_2){
		if(exitIdx==ERROR_IDX) exitIdx = filler3m2->execute();
		if(exitIdx==ERROR_IDX) exitIdx = filler3p1m1->execute();
		if(exitIdx==ERROR_IDX) exitIdx = filler3p2->execute();
	}
	
	if (exitIdx > -1) return kTRUE;
	
	return kFALSE;
}

Bool_t HHypPEpEmPAlg::init()
{
	Bool_t flag;
  
	use_3_1 = kFALSE;
	use_3_1 = (GetOpt("ONEFAKE") != NULL);
	
	use_3_2 = kFALSE;
	use_3_2 = (GetOpt("TWOFAKES") != NULL);

	filler3 = new HHypListFiller("filler3", "filler3");
	filler3->SetExitList(exitList);
	filler3->AddTrack(HPidPhysicsConstants::pid("p"));
	filler3->AddTrack(HPidPhysicsConstants::pid("e+"));
	filler3->AddTrack(HPidPhysicsConstants::pid("e-"));
	filler3->AddOneMissing(HPidPhysicsConstants::pid("p"));
	flag = filler3->init();

	// 3 + 1 part filler
	if(use_3_1){
		cout << "HHypPEpEmPAlg will use option ONEFAKE" << endl;
	
		filler3p1 = new HHypListFiller("filler3p1", "filler3p1");
		filler3p1->SetExitList(exitList);
		
		filler3p1->AddTrack(HPidPhysicsConstants::pid("p"));
		filler3p1->AddTrack(HPidPhysicsConstants::pid("e+"));
		filler3p1->AddTrack(HPidPhysicsConstants::pid("e-"));
		filler3p1->AddTrack(HPidPhysicsConstants::fakePos());
		filler3p1->AddOneMissing(HPidPhysicsConstants::pid("p"));
		
		flag &= filler3p1->init();
		
		filler3m1 = new HHypListFiller("filler3m1", "filler3m1");
		filler3m1->SetExitList(exitList);
		
		filler3m1->AddTrack(HPidPhysicsConstants::pid("p"));
		filler3m1->AddTrack(HPidPhysicsConstants::pid("e+"));
		filler3m1->AddTrack(HPidPhysicsConstants::pid("e-"));
		filler3m1->AddTrack(HPidPhysicsConstants::fakeNeg());
		filler3m1->AddOneMissing(HPidPhysicsConstants::pid("p"));
		
		flag &= filler3m1->init();
	}	
	
	// 3 + 2 part filler
	if(use_3_2){
		cout << "HHypPEpEmPAlg will use option TWOFAKES" << endl;
	
		filler3p2 = new HHypListFiller("filler3p2", "filler3p2");
		filler3p2->SetExitList(exitList);
		
		filler3p2->AddTrack(HPidPhysicsConstants::pid("p"));
		filler3p2->AddTrack(HPidPhysicsConstants::pid("e+"));
		filler3p2->AddTrack(HPidPhysicsConstants::pid("e-"));
		filler3p2->AddTrack(HPidPhysicsConstants::fakePos());
		filler3p2->AddTrack(HPidPhysicsConstants::fakePos());
		filler3p2->AddOneMissing(HPidPhysicsConstants::pid("p"));
		
		flag &= filler3p2->init();

		filler3p1m1 = new HHypListFiller("filler3p1m1", "filler3p1m1");
		filler3p1m1->SetExitList(exitList);
		
		filler3p1m1->AddTrack(HPidPhysicsConstants::pid("p"));
		filler3p1m1->AddTrack(HPidPhysicsConstants::pid("e+"));
		filler3p1m1->AddTrack(HPidPhysicsConstants::pid("e-"));
		filler3p1m1->AddTrack(HPidPhysicsConstants::fakePos());
		filler3p1m1->AddTrack(HPidPhysicsConstants::fakeNeg());
		filler3p1m1->AddOneMissing(HPidPhysicsConstants::pid("p"));
		
		flag &= filler3p1m1->init();
				
		filler3m2 = new HHypListFiller("filler3m2", "filler3m2");
		filler3m2->SetExitList(exitList);
		
		filler3m2->AddTrack(HPidPhysicsConstants::pid("p"));
		filler3m2->AddTrack(HPidPhysicsConstants::pid("e+"));
		filler3m2->AddTrack(HPidPhysicsConstants::pid("e-"));
		filler3m2->AddTrack(HPidPhysicsConstants::fakeNeg());
		filler3m2->AddTrack(HPidPhysicsConstants::fakeNeg());
		filler3m2->AddOneMissing(HPidPhysicsConstants::pid("p"));
		
		flag &= filler3m2->init();
	}	
	
	return flag;
}

Bool_t HHypPEpEmPAlg::reinit()
{
  return kTRUE;
}

Bool_t HHypPEpEmPAlg::finalize()
{
  return kTRUE;
}
