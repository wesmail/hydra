//*-- AUTHOR : B. Spruck
//*-- Modified : 10/nov/2006 by B. Spruck
//_HADES_CLASS_DESCRIPTION
////////////////////////////////////////////////////////////////////////
//
// HHypAllProjector
//
// This Algorythm projects a Ntuple with
// Hyp-Pid and Momentum information for all particles in that combination
// The number fo particles is free!
//
////////////////////////////////////////////////////////////////////////

using namespace std;
//
////////////////////////////////////////////////////////////////////////

#include "hhypAllProjector.h"

using namespace std;

ClassImp(HHypAllProjector)

HHypAllProjector::HHypAllProjector(char *name_i, Option_t par[])
  :HHypBaseAlgorithm(name_i,par)
{
}

HHypAllProjector::~HHypAllProjector()
{
}


Bool_t HHypAllProjector::execute()
{
	Int_t npart;
	npart=mylist->getNpart();
	if(npart>HYP_MAX_PARTICLES) return kFALSE;// could not happen -> maximum for Filler
	
	mylist->CombIteratorReset();
	while (mylist->CombIterator()) {

		// Getting the particles 
		for(Int_t i=0; i<npart; i++){
			pidofPart[i]=mylist->getPid(i);
			momentum[i]=mylist->getMomentum(i);
		}

		Float_t fakes;
		if (!mylist->getUserValue(FILLER_VALID_PIDTRACKS, fakes)) fakes=npart;
		fakes-=npart;

		Float_t mpid;
		if (!mylist->getUserValue(FILLER_MISSING_PID, mpid)) mpid = -999;

		Float_t chi2;
		if (!mylist->getUserValue(KINEFIT_CHI2, chi2)) chi2 = -1;
		
		Float_t channel_code_lo;
		if (!mylist->getUserValue(KINEFIT_CHANNEL_LO, channel_code_lo)) channel_code_lo = -1;

		Float_t channel_code_hi;
		if (!mylist->getUserValue(KINEFIT_CHANNEL_HI, channel_code_hi)) channel_code_hi = -1;
		
		if (histofile) {
			Float_t tmp[4+HYP_MAX_PARTICLES*4];
			Int_t ii;
			ii=0;
			tmp[ii++]=chi2;
			tmp[ii++]=channel_code_lo;// lo
			tmp[ii++]=channel_code_hi;// hi
			tmp[ii++]=mpid;
			tmp[ii++]=fakes;// Valid PID - Npart
			for(Int_t i=0; i<npart; i++){
				tmp[ii++]=pidofPart[i];
				tmp[ii++]=momentum[i].Px();
				tmp[ii++]=momentum[i].Py();
				tmp[ii++]=momentum[i].Pz();
			}
			for(Int_t i=npart; i<HYP_MAX_PARTICLES; i++){
				tmp[ii++]=0;// dummy
				tmp[ii++]=0;
				tmp[ii++]=0;
				tmp[ii++]=0;
			}

			qa->Fill(tmp);
		}
	}                             //END Iterator
	
	if (exitIdx > -1)
		return kTRUE;
	return kFALSE;
}

Bool_t HHypAllProjector::init()
{
	// I see no way in getting the max number of particles here...
	// therefor we assume the maximum what the filler can handle
	// and put static arrays in the .h file!
	// this is clearly a waste of mem and discspace; solutions?
	// npart=mylist->getNpart();// not possible as mylist==0
	
	// need to get name from channel
	TString input(channel->Get(initList));
	
	if (histofile){
		TString tmpstr;
		tmpstr="chi2:channel_code_lo:channel_code_hi:miss_pid:fakes";
		for(Int_t i=0; i<HYP_MAX_PARTICLES; i++) {
			Char_t tmp2[30];
			sprintf(tmp2,":pid%d:px_%d:py_%d:pz_%d",i,i,i,i);
			tmpstr+=tmp2;
		}

		qa = new TNtuple(input + TString("_all_proj"), "project all", tmpstr);
	}

	return kTRUE;
}

Bool_t HHypAllProjector::reinit()
{
	return kTRUE;
}

Bool_t HHypAllProjector::finalize()
{
	if (histofile) qa->Write();

	return kTRUE;
}
