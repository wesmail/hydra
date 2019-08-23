//*-- Modified : 05/01/2005 by Tiago Perez
//*-- Modified : 04/07/2005 by I. Froehlich
//*-- Modified : 22/Dec/2005 by B. Spruck
//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////
//
// HHypPPPipPimXAlg
//
// This is a FILLER for the channel P P Pip Pim + something
//
// This Algorythm will select 4, 5 and 6 particle evens with the following
// conditions: 3p+1n, 4p+1n 3p+2n, for the 5 particle events one
// particle is threaten as FAKE, and removed from the list
// => Output is in ANY case a 3p+1n event!!!
//
// 1 Fake events are only taken if ONEFAKE is set.
// 2 Fake events are only taken if TWOFAKES is set.
// Note: TWOFAKES will not add ONEFAKE option!
//
// Note for adding more fillers:
// Make sure you cover ALL cases!
// F.e. 3fakes: 4+p3 4+p2n1 4+p1n2 4+n3
//
////////////////////////////////////////////////////////////////////////

using namespace std;

#include <stdlib.h>
#include <iostream>
#include "hhypPPPipPimXAlg.h"
#include "hypinfodef.h"

ClassImp(HHypPPPipPimXAlg)

HHypPPPipPimXAlg::HHypPPPipPimXAlg(Char_t *name_i, Option_t par[])
  :HHypBaseAlgorithm(name_i,par)
{
  // 4 Particles
  filler4 = NULL;
  // 4 + 1 fake
  filler4p1 = NULL;
  filler4m1 = NULL;
  // 4 + 2 fakes
  filler4p2 = NULL;
  filler4p1m1 = NULL;
  filler4m2 = NULL;
}

HHypPPPipPimXAlg::~HHypPPPipPimXAlg()
{
  if (filler4)
    delete filler4;

  if (filler4p1)
    delete filler4p1;
  if (filler4m1)
    delete filler4m1;
  
  if (filler4p2)
    delete filler4p2;
  if (filler4p1m1)
    delete filler4p1m1;
  if (filler4m2)
    delete filler4m2;
}

Bool_t HHypPPPipPimXAlg::execute()
{
	// 4 Particles
	exitIdx = filler4->execute();

	// 4+1 fake
	if( use_4_1){
		// if no 3p 1n then try next filler
		if(exitIdx==ERROR_IDX) exitIdx = filler4m1->execute();
		// if no 3p 2n then try next filler
		if(exitIdx==ERROR_IDX) exitIdx = filler4p1->execute();
		// if no 4p 1n then forget about it
	}

	// 4+2 fakes
	if( use_4_2){
		if(exitIdx==ERROR_IDX) exitIdx = filler4m2->execute();
		if(exitIdx==ERROR_IDX) exitIdx = filler4p1m1->execute();
		if(exitIdx==ERROR_IDX) exitIdx = filler4p2->execute();
	}
	
	if (exitIdx > -1) return kTRUE;
	
	return kFALSE;
}

Bool_t HHypPPPipPimXAlg::init()
{
	Bool_t flag;
	
	use_4_1 = kFALSE;
	use_4_1 = (GetOpt("ONEFAKE") != NULL);
	
	use_4_2 = kFALSE;
	use_4_2 = (GetOpt("TWOFAKES") != NULL);

	// Normal 4 part filler
	filler4 = new HHypListFiller("filler4", "filler4");
	filler4->SetExitList(exitList);

	filler4->AddTrack(HPidPhysicsConstants::pid("p"));
	filler4->AddTrack(HPidPhysicsConstants::pid("p"));
	filler4->AddTrack(HPidPhysicsConstants::pid("pi+"));
	filler4->AddTrack(HPidPhysicsConstants::pid("pi-"));
	filler4->AddOneMissing(HPidPhysicsConstants::pid("pi0"));

	flag=filler4->init();

	// 4 + 1 part filler
	if(use_4_1){
		cout <<"HHypPPPipPimXAlg will use option ONEFAKE" << endl;
	
		filler4p1 = new HHypListFiller("filler4p1", "filler4p1");
		filler4p1->SetExitList(exitList);

		filler4p1->AddTrack(HPidPhysicsConstants::pid("p"));
		filler4p1->AddTrack(HPidPhysicsConstants::pid("p"));
		filler4p1->AddTrack(HPidPhysicsConstants::pid("pi+"));
		filler4p1->AddTrack(HPidPhysicsConstants::pid("pi-"));
		filler4p1->AddTrack(HPidPhysicsConstants::fakePos());
		filler4p1->AddOneMissing(HPidPhysicsConstants::pid("pi0"));
		
		flag &= filler4p1->init();
		
		filler4m1 = new HHypListFiller("filler4m1", "filler4m1");
		filler4m1->SetExitList(exitList);
		
		filler4m1->AddTrack(HPidPhysicsConstants::pid("p"));
		filler4m1->AddTrack(HPidPhysicsConstants::pid("p"));
		filler4m1->AddTrack(HPidPhysicsConstants::pid("pi+"));
		filler4m1->AddTrack(HPidPhysicsConstants::pid("pi-"));
		filler4m1->AddTrack(HPidPhysicsConstants::fakeNeg());
		filler4m1->AddOneMissing(HPidPhysicsConstants::pid("pi0"));
		
		flag &= filler4m1->init();
	}
	
	// 4 + 2 part filler
	if(use_4_2){
		cout << "HHypPPPipPimXAlg will use option TWOFAKES" << endl;
	
		filler4p2 = new HHypListFiller("filler4p2", "filler4p2");
		filler4p2->SetExitList(exitList);
		
		filler4p2->AddTrack(HPidPhysicsConstants::pid("p"));
		filler4p2->AddTrack(HPidPhysicsConstants::pid("p"));
		filler4p2->AddTrack(HPidPhysicsConstants::pid("pi+"));
		filler4p2->AddTrack(HPidPhysicsConstants::pid("pi-"));
		filler4p2->AddTrack(HPidPhysicsConstants::fakePos());
		filler4p2->AddTrack(HPidPhysicsConstants::fakePos());
		filler4p2->AddOneMissing(HPidPhysicsConstants::pid("pi0"));
		
		flag &= filler4p2->init();

		filler4p1m1 = new HHypListFiller("filler4p1m1", "filler4p1m1");
		filler4p1m1->SetExitList(exitList);
		
		filler4p1m1->AddTrack(HPidPhysicsConstants::pid("p"));
		filler4p1m1->AddTrack(HPidPhysicsConstants::pid("p"));
		filler4p1m1->AddTrack(HPidPhysicsConstants::pid("pi+"));
		filler4p1m1->AddTrack(HPidPhysicsConstants::pid("pi-"));
		filler4p1m1->AddTrack(HPidPhysicsConstants::fakePos());
		filler4p1m1->AddTrack(HPidPhysicsConstants::fakeNeg());
		filler4p1m1->AddOneMissing(HPidPhysicsConstants::pid("pi0"));
		
		flag &= filler4p1m1->init();
				
		filler4m2 = new HHypListFiller("filler4m2", "filler4m2");
		filler4m2->SetExitList(exitList);
		
		filler4m2->AddTrack(HPidPhysicsConstants::pid("p"));
		filler4m2->AddTrack(HPidPhysicsConstants::pid("p"));
		filler4m2->AddTrack(HPidPhysicsConstants::pid("pi+"));
		filler4m2->AddTrack(HPidPhysicsConstants::pid("pi-"));
		filler4m2->AddTrack(HPidPhysicsConstants::fakeNeg());
		filler4m2->AddTrack(HPidPhysicsConstants::fakeNeg());
		filler4m1->AddOneMissing(HPidPhysicsConstants::pid("pi0"));
		
		flag &= filler4m2->init();
	}	
	
	return flag;
}

Bool_t HHypPPPipPimXAlg::reinit()
{
	return kTRUE;
}

Bool_t HHypPPPipPimXAlg::finalize()
{
    if(filler4) filler4->finalize();
    if(filler4m1) filler4m1->finalize();
    if(filler4p1) filler4p1->finalize();
    if(filler4m2) filler4m2->finalize();
    if(filler4p1m1) filler4p1m1->finalize();
    if(filler4p2) filler4p2->finalize();
	
   return kTRUE;
}
