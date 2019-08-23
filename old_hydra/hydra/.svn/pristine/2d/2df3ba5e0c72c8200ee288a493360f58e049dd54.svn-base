//*-- AUTHOR : I. Froehlich
//*-- Modified : 21/04/2005 by I. Froehlich
////////////////////////////////////////////////////////////////////////
//
// HHypQA
//
// This class should collect some basic numbers for determing the
// efficiency of the algorithm chain.
//
// Numbers could be: # of events
//                   mean combinatial multiplicity
//                   number of tracks (all/positive/negative)
//
////////////////////////////////////////////////////////////////////////

using namespace std;

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "hevent.h"
#include "heventheader.h"
#include "hdetector.h"
#include "hratreeext.h"
#include "hcategory.h"
#include "hmatrixcategory.h"
#include "hlinearcategory.h"
#include "hlinearcatiter.h"
#include "hlocation.h"
#include "hiterator.h"
#include "hdebug.h"
#include "hades.h"

#include "hhypQA.h"
#include "hhyplist.h"
#include "hhypchannel.h"
#include "hypinfodef.h"

ClassImp(HHypQA)

HHypQA::HHypQA(char *name_i, Option_t par[])
  :HHypBaseAlgorithm(name_i, par)
{
}

HHypQA::~HHypQA()
{
}

Bool_t HHypQA::execute()
{

  UInt_t cmult = 0, ntracks = 0, npos = 0, nneg = 0;
  HEventHeader *evHeader = gHades->getCurrentEvent()->getHeader();
  Int_t order=0, status=0;
  Int_t geant_tracknr, geant_id;		

  if (evHeader) {
    seqnr = evHeader->getEventSeqNumber();
    date = (evHeader->getDate() & 0xff)
      + ((evHeader->getDate() >> 8) & 0xff) * 100
      + ((evHeader->getDate() >> 16) & 0xff) * 10000;
    time = (evHeader->getTime() & 0xff) //seconds
      + ((evHeader->getTime() >> 8) & 0xff) * 60        //minutes
      + ((evHeader->getTime() >> 16) & 0xff) * 60 * 60; //hours

  }

  if (mylist) {
    // Input is existing
    mylist->CombIteratorReset();
    ntracks = mylist->getNpart();

    while (mylist->CombIterator()) {

      if (mylist->getIterStatus() == kTRUE) {
        cmult++;
	order = mylist->getOrder();
	status = 0;

        if (use_trqa) {

	  //first look for artefacts...pions with mom>1000
	  //THIS FOR DEBUGGUNG DELTATOF
	  for (UInt_t j = 0; j < ntracks; j++)
	    if ((mylist->getPid(j) == 8) && ((mylist->getMomentum(j)).Mag() > 1000))
	      status = 1;
	  //END DEBUG DTOF....


          //Track QA: This is my idea [Ingo]
          for (UInt_t j = 0; j < ntracks; j++) {
            //look if we have the particles         
            HPidParticle *track = mylist->getPidParticle(j);

            if (track) {
              Int_t pid = mylist->getPid(j);
              // Double_t beta = track->getBetaExp() * track->getCharge();
	      Double_t beta = mylist->getBeta(j) * track->getCharge();
              Double_t mom = (mylist->getMomentum(j)).Mag();


	      if (simuflag == 1) {
		//OK, lets compare this with simulation
		HPidParticleSim *my =
		  (HPidParticleSim *) CatPartSim->getObject(mylist->
							    getIdxPidPart(j));
		if (my != NULL) {
		  HPidGeantTrackSet *GeantSet = my->getGeantTrackSet();
		  Int_t geant_track = GeantSet->getGeantTrackID(0);
		  if (geant_track >= 0) {
		    HGeantKine *geantkine =
		      (HGeantKine *) GeantSet->getGeantKine(geant_track);
		    geantkine->getParticle(geant_tracknr, geant_id);
		    // cout << tr << ":" << id << endl;
		  }           //END geant_track
		}             //END my
	      }               //END simuflag
	      
	      Int_t my_order = order;
	      if (status) my_order =  -order;
	      if (simuflag == 1)
		trqa->Fill(pid, mom, beta, mylist->getProbAlg(), my_order, geant_id);
	      else
		trqa->Fill(pid, mom, beta, mylist->getProbAlg(), my_order);
            }
            //lines for the standard PID see below
          }
        }                       //END if (use_trqa)

      }
    }
  }                             // END if mylist
  else                          // Use standard PID input
  {

    Int_t counter = 0;
    Int_t Charge[20];

    cmult = 1;
    order = 0;

    HPidParticle   *pidPart = NULL;
    HPidTrackCand  *pidTrackCand = NULL;

    m_pContItPart->Reset();
    while (((pidPart = (HPidParticle *) m_pContItPart->Next()) != NULL)
           && (counter < 20)) {
      Charge[counter] = pidPart->getCharge();

      if (Charge[counter] == 1)
        npos++;
      else if (Charge[counter] == -1)
        nneg++;

      //Track QA: This is my idea [Ingo]
      if (use_trqa) {
        Int_t pid = pidPart->getPid();
        //Double_t beta = pidPart->getBetaExp() * pidPart->getCharge();
	Double_t beta = 0;
	pidTrackCand = pidPart->getTrackCand();
	if (pidTrackCand) beta = fabs(pidTrackCand->getTrackData()->getBeta(pidPart->getMomAlg())) * pidPart->getCharge();
        Double_t mom = pidPart->P();

        trqa->Fill(pid, mom, beta, 1., order);
      }

      counter++;
    }
    //========= Counting Number of ch. positive and negative particles END =====
    ntracks = counter;

  }
  qa->Fill(date, time, seqnr, cmult, ntracks, npos, nneg);

  return kTRUE;
}

Bool_t HHypQA::init()
{

  use_trqa = kFALSE;
  use_trqa = (GetOpt("TRQA") != NULL);

  // Checks if we have sim/exp and book the ntuple
  
  simCat = gHades->getCurrentEvent()->getCategory(catGeantKine);
  
  if (!simCat)
    simuflag = 0;
  else {
    simuflag = 1;
    //cout << "QA uses SIMULATION" << endl;
    
    CatPartSim = NULL;          // Category

    if ((CatPartSim =
         gHades->getCurrentEvent()->getCategory(catPidPart)) == NULL) {
      Error("init", "Cannot get catPidPartSim cat");
      return kFALSE;
    }

  }



  //if (use_trqa) std::cout << "QA uses track information" << std::endl;
  //else std::cout << "normal QA operation" << std::endl;

  //std::cout << "booking ntuples for " << algoName << std::endl;

  // need to get name from channel
  TString input(channel->Get(initList));

  qa =
    new TNtuple(input + TString("_qa"), "QA ntuple",
                "date:time:seqnr:cmult:ntracks:npos:nneg");

  if (use_trqa && !simuflag)
    trqa =
      new TNtuple(input + TString("_trqa"), "Track QA ntuple",
                  "pid:mom:beta:fProbAlg:order");
  if (use_trqa && simuflag)
    trqa =
      new TNtuple(input + TString("_trqa"), "Track QA ntuple",
                  "pid:mom:beta:fProbAlg:order:geant_id");

  //---------- Initialization of HPidParticle Container -----------------------
  m_pContItPart = NULL;         // Iterator
  m_pContCatPart = NULL;        // Category

  if ((m_pContCatPart =
       gHades->getCurrentEvent()->getCategory(catPidPart)) == NULL) {
    Error("init", "Cannot get catPidPart cat");
    return kFALSE;
  }
  m_pContItPart = (HIterator *) m_pContCatPart->MakeIterator();
  //-----------------------------------------------------------------------

  return kTRUE;
}

Bool_t HHypQA::reinit()
{
  return kTRUE;
}

Bool_t HHypQA::finalize()
{

  qa->Write();
  if (use_trqa)
    trqa->Write();
  return kTRUE;
}

void HHypQA::Dump(void)
{

  cout << "Last information from HypQA was:" << endl;
  cout << "Date YYYMMDD:" << endl << "     " << date << endl
    << "Time in seconds after midnight :" << time << endl
    << "SeqNR: " << seqnr << endl;

}
