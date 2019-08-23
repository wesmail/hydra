//*-- Author : B. Spruck
//*-- Modified : 7 May 2008 B. Spruck
//_HADES_CLASS_DESCRIPTION
////////////////////////////////////////////////////////////////////////
//
// HHypAllFillerAlg
//
// This Algorythm allows for easy selection of reactions in your macro
// because reaction can be given by option string.
//
// Example:
//    hyp->AddAlgorithm("empty",new HHypAllFillerAlg("Test of All Filler",
//			standardHypOption + "Reactions=(p p e+ e-;g) "
//			"(p p pi+ pi-;pi0 g dummy) (p p pi+ pi-) ,"
//			" someotheroption=false"),"AllFill");
//
// Syntax:
// Reactions=(reaction1)(reaction2)(reaction3)
// where reaction1 is consits of a part with the wanted particles,
// followed by an semicolon and the missing particles
// Example:   ( p p e+ e- ; g pi0 dummy )
// particle names are seperated by spaces
// the "dummy" particle tells the filler that no missing particle is
// expected (needed for kinematic fitting). If there a several missing
// particles defined, this will result in several reactions, the upper
// example is a shortcut for
// ( p p e+ e- ; g)
// ( p p e+ e- ; pi0)
// ( p p e+ e- ) which is equivalent to  ( p p e+ e- ; dummy)
//
// If you dont follow these syntax rules, result might be "interessting"
// wrong particle names will result in non-fatal errors, therefore
// watch output carefully
//
// Number of reactions is limited to 20 but can be increased easily
// Number of partciles is limited to 10, this is limited by Filler
//
////////////////////////////////////////////////////////////////////////

using namespace std;

#include <stdlib.h>
#include <iostream>
#include "hhypAllFillerAlg.h"
#include "hypinfodef.h"

ClassImp(HHypAllFillerAlg)

HHypAllFillerAlg::HHypAllFillerAlg(Char_t *name_i, Option_t par[])
  :HHypBaseAlgorithm(name_i,par)
{
	for(Int_t i=0; i<HHypAllFillerAlg_ANZ_FILLER; i++) filler[i] = NULL;
}

HHypAllFillerAlg::~HHypAllFillerAlg()
{
	for(Int_t i=0; i<HHypAllFillerAlg_ANZ_FILLER; i++){
		if( filler[i]) delete filler[i];
	}
}

Bool_t HHypAllFillerAlg::execute()
{
	// In principle, the first question should be:
	// How many particles are in that events!
	// Depending on that the appopriate filler should be called.
	// Doing it in the following way is a waste of cpu time ...

// 	cout << "Filler Exec: ";
	Int_t nrs[HHypAllFillerAlg_ANZ_FILLER], nr=0;
	for(Int_t i=0; i<HHypAllFillerAlg_ANZ_FILLER; i++){
		if( filler[i]){
			nrs[nr] = filler[i]->execute();
// 		cout <<nrs[nr]<<" ";
			if( nrs[nr]>=0) nr++;
		}
	}

	if (nr ==0) return kFALSE;

//	cout <<endl<< "-> ";
	HHypList *hyplist_s = NULL;
   HCategory *listCat = gHades->getCurrentEvent()->getCategory(catHypList);
	if(!listCat){
		Error("execute","listCat!=0");
	}
	hyplist_s = (HHypList *) listCat->getObject(nrs[0]);
	if( hyplist_s==0){
		Error("execute","hyplist_s==0");
		exit(0);
	}
//	cout <<nrs[0] << " ";
	for(Int_t i=1; i<nr; i++){
		HHypList *hyplist_j = NULL;
//		cout  <<nrs[i] << " ";
		hyplist_j = (HHypList *) listCat->getObject(nrs[i]);
		if(hyplist_j){
			hyplist_s->Join(hyplist_j);
		}else {
			Error("execute","hyplist_j==0");
			exit(0);
		}
	}
//	cout << endl;

	exitIdx=nrs[0];
//	cout << "Filler Exit List ID " <<exitIdx<<endl;
//	hyplist_s->print();

	return kTRUE;
}

Bool_t HHypAllFillerAlg::InitFillers(Int_t &fnr,Int_t particles_anz,Int_t *particles,Int_t missing_anz,Int_t *missing)
{
	Bool_t flag=false;
	if(particles_anz>0){
		cout << "HypAllFillerAlg "<<fnr<<": Parsed as:";
		filler[fnr] = new HHypListFiller("filler", "filler");
		filler[fnr]->SetExitList(0x7FFFF000+fnr);
		for(Int_t pi=0; pi<particles_anz; pi++){
			cout << " "<< particles[pi];
			filler[fnr]->AddTrack(particles[pi]);
		}
		cout << " :";
		for(Int_t mi=0; mi<missing_anz; mi++){
			cout << " " << missing[mi];
			filler[fnr]->AddOneMissing(missing[mi]);
		}
		cout << endl;
		flag|=filler[fnr]->init();
		fnr++;
	}else{
		cerr << "HypAllFillerAlg: No Particles defined!"<<endl;
	}
	return(flag);
}

Bool_t HHypAllFillerAlg::init()
{
	Bool_t flag=false;
	Int_t fnr=0;

	TString *abc;
	abc=GetOpt("Reactions");
	if(abc==NULL){
		Error("HHypAllFillerAlg","No Reactions defined in Option String by Reactions=()()()");
		return(false);
	}
	TObjArray *allreac_arr;
	allreac_arr=abc->Tokenize("()");

	Int_t arr_nr;
	arr_nr=allreac_arr->GetEntries();

	// Format: "( , , , ;)(, , , , ;)"
	for(Int_t j=0; j<arr_nr; j++){
		TString reac_str;
		reac_str=((TObjString *)allreac_arr->At(j))->String();
		reac_str.Remove(TString::kBoth,' ');
		if(reac_str.Length()<2) continue;

		TObjArray *partmissdiv_arr;
		partmissdiv_arr=reac_str.Tokenize(";");
		if(partmissdiv_arr->GetEntries()>2){
			cerr<<"HypAllFillerAlg: Parse Error in Reaction >>"<<reac_str<< "<< !!!"<<endl;
			delete partmissdiv_arr;
			break;
		}
		cout << "HypAllFillerAlg: Reaction String "<<j<<": <<"<<reac_str<<">>"<<endl;

		Int_t particles[11], missing[11], particles_anz, missing_anz;
		particles_anz=0;
		missing_anz=0;

		if(partmissdiv_arr->GetEntries()>=1){
			{
				TObjArray *part_arr;
				part_arr=((TObjString *)partmissdiv_arr->At(0))->String().Tokenize(" ");

				for(Int_t k=0; k<part_arr->GetEntries(); k++){
					TString part_str;
					part_str=((TObjString *)part_arr->At(k))->String();
					part_str.Remove(TString::kBoth,' ');
					particles[particles_anz]=HPidPhysicsConstants::pid(part_str);
					if(particles[particles_anz]!=0){
						particles_anz++;
						if(particles_anz==11){
							cerr << "HypAllFillerAlg: number of particles exceeds 10!"<<endl;
							break;
						}
					}
				}

				delete part_arr;
			}

			if(partmissdiv_arr->GetEntries()==2){
				TObjArray *miss_arr;
				miss_arr=((TObjString *)partmissdiv_arr->At(1))->String().Tokenize(" ");

				for(Int_t k=0; k<miss_arr->GetEntries(); k++){
					TString miss_part;
					miss_part=((TObjString *)miss_arr->At(k))->String();
					miss_part.Remove(TString::kBoth,' ');
					if(miss_part.Length()>0){
					}
					missing[missing_anz]=HPidPhysicsConstants::pid(miss_part);
					if(missing[missing_anz]!=0 || miss_part=="dummy"){
						missing_anz++;
						if(missing_anz==11){
							cerr << "HypAllFillerAlg: number of missing particles exceeds 10!"<<endl;
							break;
						}
					}
				}
				delete miss_arr;
			}

			flag|=InitFillers(fnr,particles_anz,particles,missing_anz,missing);
			if(fnr==HHypAllFillerAlg_ANZ_FILLER) break;
		}else{
			cerr << "HypAllFillerAlg: Problem: Empty string!"<<endl;
		}
		delete partmissdiv_arr;
	}
	delete allreac_arr;

	return flag;
}

Bool_t HHypAllFillerAlg::reinit()
{
  return kTRUE;
}

Bool_t HHypAllFillerAlg::finalize()
{
  return kTRUE;
}
