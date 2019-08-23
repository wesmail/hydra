//*-- Author : B. Spruck
//*-- Modified : 2005-04-25
//*-- Modified : 22/Dec/2005 B. Spruck
//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////
//
// HHypPEpEmXAlg
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
#include "hhypPEpEmXAlg.h"
#include "hypinfodef.h"

ClassImp(HHypPEpEmXAlg)

HHypPEpEmXAlg::HHypPEpEmXAlg(Char_t *name_i, Option_t par[])
  :HHypBaseAlgorithm(name_i,par)
{
  // 4 Particles
  filler  = NULL;
}

HHypPEpEmXAlg::~HHypPEpEmXAlg()
{
  if (filler)
    delete filler;
}

Bool_t HHypPEpEmXAlg::execute()
{
	// In principle, the first question should be:
	// How many particles are in that events!
	// Depending on that the appopriate filler should be called.
	// Doing it in the following way is a waste of cpu time ...
	
	// 4 Particles
    
	if( use_InclusiveFiller){
		if(exitIdx!=ERROR_IDX)  exitIdx = filler->execute();
		else cerr <<  "HHypPEpEmXAlg::execute() : exitIdx = "<< exitIdx;
		if (exitIdx > -1) return kTRUE;

		return kFALSE;
	}
	return kFALSE;
}

Bool_t HHypPEpEmXAlg::init()
{
	Bool_t flag;
	
	//use_InclusiveFiller = kFALSE;
	use_InclusiveFiller = (GetOpt("use_InclusiveFiller") != NULL);
	
    //if( use_InclusiveFiller ){
	    filler = new HHypListFillerInclusive("filler", "filler");
		filler->AddTrack(HPidPhysicsConstants::pid("p"));
		filler->AddTrack(HPidPhysicsConstants::pid("e+"));
		filler->AddTrack(HPidPhysicsConstants::pid("e-"));
        flag = filler->init();
	    return flag;
	//}
	
	//return flag;
}

Bool_t HHypPEpEmXAlg::reinit()
{
  return kTRUE;
}

Bool_t HHypPEpEmXAlg::finalize()
{
  return kTRUE;
}
