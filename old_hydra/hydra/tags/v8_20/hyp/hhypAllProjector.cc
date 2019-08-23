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

HHypAllProjector::HHypAllProjector(Char_t *name_i, Option_t par[])
  :HHypBaseAlgorithm(name_i,par)
{
}

HHypAllProjector::~HHypAllProjector()
{
}


Bool_t HHypAllProjector::execute()
{
	Int_t npart;
	Int_t src[HYP_MAX_PARTICLES];

	npart=mylist->getNpart();// get MAXIMUM what could happen
	if(npart>HYP_MAX_PARTICLES) return kFALSE;// could not happen -> maximum for Filler

	mylist->CombIteratorReset();
	while (mylist->CombIterator()) {
		Int_t creation;
		Int_t dummy;
		Float_t event_genweight;
		event_genweight=1.0;// unless found other

		npart=mylist->getNvalid();// Getting the VALID particles
		// if(npart>HYP_MAX_PARTICLES) continue;// could not happen -> maximum for Filler

		for(Int_t i=0; i<npart; i++){
			pidofPart[i]=mylist->getPid(i);
			momentum[i]=mylist->getMomentum(i);

			creation=0;
			if(simuflag && CatTrackCandSim){
				HPidTrackCandSim *my = (HPidTrackCandSim *)
						CatTrackCandSim->getObject(mylist->getIdxPidTrackCand(i));

				if ((my != NULL) && (mylist->getIterStatus() == kTRUE)) {
					HPidGeantTrackSet *GeantSet = my->getGeantTrackSet();
					Int_t geant_track = GeantSet->getGeantTrackID(0);
					if (geant_track >= 0) {
						Float_t genweight, dummy2;
						HGeantKine *geantkine =
							(HGeantKine *) GeantSet->getGeantKine(geant_track);
						geantkine->getCreator(dummy,dummy,creation);
						geantkine->getGenerator(dummy2, genweight);
						if(genweight>0.0) event_genweight=genweight;
					}
				}
			}
			src[i]=creation;
		}

		Float_t fakes;
		fakes=mylist->getNrOfFakes();

		Float_t mpid;
		if (!mylist->getUserValue(FILLER_MISSING_PID, mpid)) mpid = -999;

		Float_t chi2;
		if (!mylist->getUserValue(KINEFIT_CHI2, chi2)) chi2 = -1;

		Float_t channel_code_lo;
		if (!mylist->getUserValue(KINEFIT_CHANNEL_LO, channel_code_lo)) channel_code_lo = -1;

		Float_t channel_code_hi;
		if (!mylist->getUserValue(KINEFIT_CHANNEL_HI, channel_code_hi)) channel_code_hi = -1;

		Float_t dtof, dtof2, dtof_chi2, dtof_offset;
		if (!mylist->getUserValue(DELTATOF_DTOF, dtof)) dtof = -1;
		if (!mylist->getUserValue(DELTATOF_DTOF2, dtof2)) dtof2 = -1;
		if (!mylist->getUserValue(DELTATOF_CHI2, dtof_chi2)) dtof_chi2 = -1;
		if (!mylist->getUserValue(DELTATOF_OFFSET, dtof_offset)) dtof_offset = -1;
		// if (!mylist->getUserValue(DELTATOF_SIGMA, sigmaDeltaTof)) sigmaDeltaTof = -1;

		if (histofile) {
			Float_t tmp[10+HYP_MAX_PARTICLES*5];
			Int_t ii;
			ii=0;
			tmp[ii++]=chi2;
			tmp[ii++]=channel_code_lo;// lo
			tmp[ii++]=channel_code_hi;// hi
			tmp[ii++]=mpid;
			tmp[ii++]=fakes;// Valid PID - Npart
			tmp[ii++]=dtof;// Delta tof
			tmp[ii++]=dtof2;// Delta tof^2
			tmp[ii++]=dtof_chi2;// Delta tof
			tmp[ii++]=dtof_offset;// Delta tof
			tmp[ii++]=event_genweight;// Weight (Geant,Pluto)
			for(Int_t i=0; i<npart; i++){
				tmp[ii++]=pidofPart[i];
				tmp[ii++]=momentum[i].Px();
				tmp[ii++]=momentum[i].Py();
				tmp[ii++]=momentum[i].Pz();
				tmp[ii++]=src[i];
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

  simCat = gHades->getCurrentEvent()->getCategory(catGeantKine);
  CatTrackCandSim = NULL;          // Category

  if (!simCat){
    simuflag = 0;
  }else{
    simuflag = 1;


    if ((CatTrackCandSim =
	 gHades->getCurrentEvent()->getCategory(catPidTrackCand)) == NULL) {
      Error("init", "Cannot get catPidTrackCandSim cat");
      return kFALSE;
    }

  }

	if (histofile){
		TString tmpstr(
			"chi2:channel_code_lo:channel_code_hi:miss_pid:fakes"
			":dtof_dtof:dtof_dtof2:dtof_chi2:dtof_offset:weight");
		for(Int_t i=0; i<HYP_MAX_PARTICLES; i++) {
			Char_t tmp2[30];
			sprintf(tmp2,":pid%d:px_%d:py_%d:pz_%d:src_%d",i,i,i,i,i);
			tmpstr+=tmp2;
		}

		cout << "*********"<< tmpstr <<"*********"<< endl;
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
