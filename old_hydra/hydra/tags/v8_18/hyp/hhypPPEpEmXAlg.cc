//*-- Author : B. Spruck
//*-- Modified : 2005-04-25
//*-- Modified : 22/Dec/2005 B. Spruck
//_HADES_CLASS_DESCRIPTION
////////////////////////////////////////////////////////////////////////
//
// HHypPPEpEmXAlg
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
#include "hhypPPEpEmXAlg.h"
#include "hypinfodef.h"

ClassImp(HHypPPEpEmXAlg)

HHypPPEpEmXAlg::HHypPPEpEmXAlg(Char_t *name_i, Option_t par[])
  :HHypBaseAlgorithm(name_i,par)
{
  // 4 Particles
  filler  = NULL;
  filler4 = NULL;
  // 4 + 1 fake
  filler4p1 = NULL;
  filler4m1 = NULL;
  // 4 + 2 fakes
  filler4p2 = NULL;
  filler4p1m1 = NULL;
  filler4m2 = NULL;
}

HHypPPEpEmXAlg::~HHypPPEpEmXAlg()
{
  if (filler)
    delete filler;

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

Bool_t HHypPPEpEmXAlg::execute()
{
	// In principle, the first question should be:
	// How many particles are in that events!
	// Depending on that the appopriate filler should be called.
	// Doing it in the following way is a waste of cpu time ...
	
	// 4 Particles
    
	if( use_InclusiveFiller){
		if(exitIdx!=ERROR_IDX) exitIdx = filler->execute();
		else cerr <<  "HHypPEpEmXAlg::execute() : exitIdx = "<< exitIdx;
		if (exitIdx > -1) return kTRUE;

		return kFALSE;
	}
	
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

Bool_t HHypPPEpEmXAlg::init()
{
	Bool_t flag;
	
	f=0;
//			  f=new TFile("meintest.root","recreate");

	miss_none=kFALSE;
	miss_none = (GetOpt("miss_none") != NULL);
	miss_gamma=kFALSE;
	miss_gamma = (GetOpt("miss_gamma") != NULL);
	miss_pi0=kFALSE;
	miss_pi0 = (GetOpt("miss_pi0") != NULL);
	
	use_InclusiveFiller = kFALSE;
	use_InclusiveFiller = (GetOpt("use_InclusiveFiller") != NULL);
	
    if( use_InclusiveFiller ){
	    filler = new HHypListFillerInclusive("filler", "filler");
		filler->AddTrack(HPidPhysicsConstants::pid("p"));
		filler->AddTrack(HPidPhysicsConstants::pid("p"));
		filler->AddTrack(HPidPhysicsConstants::pid("e+"));
		filler->AddTrack(HPidPhysicsConstants::pid("e-"));
        flag = filler->init();
	    return flag;
	}

	use_4_1 = kFALSE;
	use_4_1 = (GetOpt("ONEFAKE") != NULL);
	
	use_4_2 = kFALSE;
	use_4_2 = (GetOpt("TWOFAKES") != NULL);

	// Normal 4 part filler
	filler4 = new HHypListFiller("filler4", "filler4");
	filler4->SetExitList(exitList);
	
	filler4->AddTrack(HPidPhysicsConstants::pid("p"));
	filler4->AddTrack(HPidPhysicsConstants::pid("p"));
	filler4->AddTrack(HPidPhysicsConstants::pid("e+"));
	filler4->AddTrack(HPidPhysicsConstants::pid("e-"));
	if(miss_gamma) filler4->AddOneMissing(HPidPhysicsConstants::pid("g"));
	if(miss_pi0) filler4->AddOneMissing(HPidPhysicsConstants::pid("pi0"));
	if(miss_none) filler4->AddOneMissing(HPidPhysicsConstants::pid("dummy"));
	
	flag=filler4->init();

	// 4 + 1 part filler
	if(use_4_1){
		cout << "HHypPPEpEmXAlg will use option ONEFAKE" << endl;
	
		filler4p1 = new HHypListFiller("filler4p1", "filler4p1");
		filler4p1->SetExitList(exitList);
		
		filler4p1->AddTrack(HPidPhysicsConstants::pid("p"));
		filler4p1->AddTrack(HPidPhysicsConstants::pid("p"));
		filler4p1->AddTrack(HPidPhysicsConstants::pid("e+"));
		filler4p1->AddTrack(HPidPhysicsConstants::pid("e-"));
		filler4p1->AddTrack(HPidPhysicsConstants::fakePos());
		if(miss_gamma) filler4p1->AddOneMissing(HPidPhysicsConstants::pid("g"));
		if(miss_pi0) filler4p1->AddOneMissing(HPidPhysicsConstants::pid("pi0"));
		if(miss_none) filler4p1->AddOneMissing(HPidPhysicsConstants::pid("dummy"));
		
		flag &= filler4p1->init();
		
		filler4m1 = new HHypListFiller("filler4m1", "filler4m1");
		filler4m1->SetExitList(exitList);
		
		filler4m1->AddTrack(HPidPhysicsConstants::pid("p"));
		filler4m1->AddTrack(HPidPhysicsConstants::pid("p"));
		filler4m1->AddTrack(HPidPhysicsConstants::pid("e+"));
		filler4m1->AddTrack(HPidPhysicsConstants::pid("e-"));
		filler4m1->AddTrack(HPidPhysicsConstants::fakeNeg());
		if(miss_gamma) filler4m1->AddOneMissing(HPidPhysicsConstants::pid("g"));
		if(miss_pi0) filler4m1->AddOneMissing(HPidPhysicsConstants::pid("pi0"));
		if(miss_none) filler4m1->AddOneMissing(HPidPhysicsConstants::pid("dummy"));
		
		flag &= filler4m1->init();
	}
	
	// 4 + 2 part filler
	if(use_4_2){
		cout << "HHypPPEpEmXAlg will use option TWOFAKES" << endl;
	
		filler4p2 = new HHypListFiller("filler4p2", "filler4p2");
		filler4p2->SetExitList(exitList);
		
		filler4p2->AddTrack(HPidPhysicsConstants::pid("p"));
		filler4p2->AddTrack(HPidPhysicsConstants::pid("p"));
		filler4p2->AddTrack(HPidPhysicsConstants::pid("e+"));
		filler4p2->AddTrack(HPidPhysicsConstants::pid("e-"));
		filler4p2->AddTrack(HPidPhysicsConstants::fakePos());
		filler4p2->AddTrack(HPidPhysicsConstants::fakePos());
		if(miss_gamma) filler4p2->AddOneMissing(HPidPhysicsConstants::pid("g"));
		if(miss_pi0) filler4p2->AddOneMissing(HPidPhysicsConstants::pid("pi0"));
		if(miss_none) filler4p2->AddOneMissing(HPidPhysicsConstants::pid("dummy"));
		
		flag &= filler4p2->init();

		filler4p1m1 = new HHypListFiller("filler4p1m1", "filler4p1m1");
		filler4p1m1->SetExitList(exitList);
		
		filler4p1m1->AddTrack(HPidPhysicsConstants::pid("p"));
		filler4p1m1->AddTrack(HPidPhysicsConstants::pid("p"));
		filler4p1m1->AddTrack(HPidPhysicsConstants::pid("e+"));
		filler4p1m1->AddTrack(HPidPhysicsConstants::pid("e-"));
		filler4p1m1->AddTrack(HPidPhysicsConstants::fakePos());
		filler4p1m1->AddTrack(HPidPhysicsConstants::fakeNeg());
		if(miss_gamma) filler4p1m1->AddOneMissing(HPidPhysicsConstants::pid("g"));
		if(miss_pi0) filler4p1m1->AddOneMissing(HPidPhysicsConstants::pid("pi0"));
		if(miss_none) filler4p1m1->AddOneMissing(HPidPhysicsConstants::pid("dummy"));
		
		flag &= filler4p1m1->init();
				
		filler4m2 = new HHypListFiller("filler4m2", "filler4m2");
		filler4m2->SetExitList(exitList);
		
		filler4m2->AddTrack(HPidPhysicsConstants::pid("p"));
		filler4m2->AddTrack(HPidPhysicsConstants::pid("p"));
		filler4m2->AddTrack(HPidPhysicsConstants::pid("e+"));
		filler4m2->AddTrack(HPidPhysicsConstants::pid("e-"));
		filler4m2->AddTrack(HPidPhysicsConstants::fakeNeg());
		filler4m2->AddTrack(HPidPhysicsConstants::fakeNeg());
		if(miss_gamma) filler4m2->AddOneMissing(HPidPhysicsConstants::pid("g"));
		if(miss_pi0) filler4m2->AddOneMissing(HPidPhysicsConstants::pid("pi0"));
		if(miss_none) filler4m2->AddOneMissing(HPidPhysicsConstants::pid("dummy"));
		
		flag &= filler4m2->init();
	}
	
	return flag;
}

Bool_t HHypPPEpEmXAlg::reinit()
{
  return kTRUE;
}

Bool_t HHypPPEpEmXAlg::finalize()
{
	if(f){
				f->Write();
				f->Close();
				delete f;
				f=0;
	}
  return kTRUE;
}
