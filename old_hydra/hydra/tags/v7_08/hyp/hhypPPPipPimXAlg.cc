//*-- AUTHOR : Tiago Perez
//*-- Modified : 05/01/2005 by Tiago Perez
//*-- Modified : 04/07/2005 by I. Froehlich
//*-- Modified : 10/Nov/2005 by B. Spruck
////////////////////////////////////////////////////////////////////////
//
// HHypPPPipPimXAlg
//
// This is a FILLER for the channel P P Pip Pim + something
//
////////////////////////////////////////////////////////////////////////

using namespace std;

#include <stdlib.h>
#include <iostream>
#include "hhypPPPipPimXAlg.h"
#include "hypinfodef.h"

ClassImp(HHypPPPipPimXAlg)

HHypPPPipPimXAlg::HHypPPPipPimXAlg(char *name_i, Option_t par[])
  :HHypBaseAlgorithm(name_i,par)
{
	filler4 = NULL;
	fillerfp = NULL;
	fillerfm = NULL;
}

HHypPPPipPimXAlg::~HHypPPPipPimXAlg()
{
	if (filler4)
		delete filler4;
	if (fillerfp)
		delete fillerfp;
	if (fillerfm)
		delete fillerfm;
}

Bool_t HHypPPPipPimXAlg::execute()
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

Bool_t HHypPPPipPimXAlg::init()
{
	Bool_t flag;

	use_fakes = kFALSE;
	use_fakes = (GetOpt("ONEFAKE") != NULL);

	filler4 = new HHypListFiller("aaa", "bbb");
	filler4->SetExitList(exitList);

	filler4->AddTrack(HPidPhysicsConstants::pid("p"));
	filler4->AddTrack(HPidPhysicsConstants::pid("p"));
	filler4->AddTrack(HPidPhysicsConstants::pid("pi+"));
	filler4->AddTrack(HPidPhysicsConstants::pid("pi-"));

	flag=filler4->init();

	if(use_fakes){
		cout << "----------------------" << endl
			<< "HHypPPPipPimXAlg will use option ONEFAKE" << endl
			<< "----------------------" << endl;
	
		fillerfp = new HHypListFiller("ccc", "ddd");
		fillerfp->SetExitList(exitList);
		
		fillerfp->AddTrack(HPidPhysicsConstants::pid("p"));
		fillerfp->AddTrack(HPidPhysicsConstants::pid("p"));
		fillerfp->AddTrack(HPidPhysicsConstants::pid("pi+"));
		fillerfp->AddTrack(HPidPhysicsConstants::pid("pi-"));
		fillerfp->AddTrack(HPidPhysicsConstants::fakePos());
		
		flag &= fillerfp->init();
		
		fillerfm = new HHypListFiller("eee", "fff");
		fillerfm->SetExitList(exitList);
		
		fillerfm->AddTrack(HPidPhysicsConstants::pid("p"));
		fillerfm->AddTrack(HPidPhysicsConstants::pid("p"));
		fillerfm->AddTrack(HPidPhysicsConstants::pid("pi+"));
		fillerfm->AddTrack(HPidPhysicsConstants::pid("pi-"));
		fillerfm->AddTrack(HPidPhysicsConstants::fakeNeg());
		
		flag &= fillerfm->init();
	}
	
	return flag;
}

Bool_t HHypPPPipPimXAlg::reinit()
{
	return kTRUE;
}

Bool_t HHypPPPipPimXAlg::finalize()
{
	return kTRUE;
}
