//*-- Author : B. Spruck
//*-- Modified : 2005-04-25
////////////////////////////////////////////////////////////////////////
//
// HHypPPEpEmXAlg
//
// This Algorythm will select 4 and 5 particle evens with the following 
// conditions: 3p+1n, 4p+1n 3p+2n, for the 5 particle events one
// particle is threaten as FAKE, and removed from the list
// 1 Fake evets are only taken if ONEFAKE is set.
// Output is in ANY case a 3p+1n event!!!
//
////////////////////////////////////////////////////////////////////////

using namespace std;

#include <stdlib.h>
#include <iostream>
#include "hhypPPEpEmXAlg.h"
#include "hypinfodef.h"

ClassImp(HHypPPEpEmXAlg)

HHypPPEpEmXAlg::HHypPPEpEmXAlg(char *name_i, Option_t par[])
  :HHypBaseAlgorithm(name_i,par)
{
  filler4 = NULL;
  fillerfp = NULL;
  fillerfm = NULL;
}

HHypPPEpEmXAlg::~HHypPPEpEmXAlg()
{
  if (filler4)
    delete filler4;
  if (fillerfp)
    delete fillerfp;
  if (fillerfm)
    delete fillerfm;
}

Bool_t HHypPPEpEmXAlg::execute()
{
	exitIdx = filler4->execute();
	
	if( use_fakes){
		// if no 3p 1n then try next filler
		if(exitIdx==ERROR_IDX) exitIdx = fillerfm->execute();
		// if no 3p 2n then try next filler
		if(exitIdx==ERROR_IDX) exitIdx = fillerfp->execute();
		// if no 4p 1n then forget about it
	}
	
	if (exitIdx > -1) return kTRUE;
	
	return kFALSE;
}

Bool_t HHypPPEpEmXAlg::init()
{
	Bool_t flag;
	
	use_fakes = kFALSE;
	use_fakes = (GetOpt("ONEFAKE") != NULL);
	
	filler4 = new HHypListFiller("aaa", "bbb");
	filler4->SetExitList(exitList);
	
	filler4->AddTrack(HPidPhysicsConstants::pid("p"));
	filler4->AddTrack(HPidPhysicsConstants::pid("p"));
	filler4->AddTrack(HPidPhysicsConstants::pid("e+"));
	filler4->AddTrack(HPidPhysicsConstants::pid("e-"));
	
	flag=filler4->init();
	
	if(use_fakes){
		cout << "----------------------" << endl
			<< "HHypPPEpEmXAlg will use option ONEFAKE" << endl
			<< "----------------------" << endl;
	
		fillerfp = new HHypListFiller("ccc", "ddd");
		fillerfp->SetExitList(exitList);
		
		fillerfp->AddTrack(HPidPhysicsConstants::pid("p"));
		fillerfp->AddTrack(HPidPhysicsConstants::pid("p"));
		fillerfp->AddTrack(HPidPhysicsConstants::pid("e+"));
		fillerfp->AddTrack(HPidPhysicsConstants::pid("e-"));
		fillerfp->AddTrack(HPidPhysicsConstants::fakePos());
		
		flag &= fillerfp->init();
		
		fillerfm = new HHypListFiller("eee", "fff");
		fillerfm->SetExitList(exitList);
		
		fillerfm->AddTrack(HPidPhysicsConstants::pid("p"));
		fillerfm->AddTrack(HPidPhysicsConstants::pid("p"));
		fillerfm->AddTrack(HPidPhysicsConstants::pid("e+"));
		fillerfm->AddTrack(HPidPhysicsConstants::pid("e-"));
		fillerfm->AddTrack(HPidPhysicsConstants::fakeNeg());
		
		flag &= fillerfm->init();
	}	
	
	return flag;
}

Bool_t HHypPPEpEmXAlg::reinit()
{
  return kTRUE;
}

Bool_t HHypPPEpEmXAlg::finalize()
{
  return kTRUE;
}
