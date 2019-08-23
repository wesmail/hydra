//*-- AUTHOR : I. Froehlich
//*-- Modified : ?? by ??
////////////////////////////////////////////////////////////////////////
//
// HHypHardCutsAlg
//
// Algorithm which removes combinations acconding to hard cuts
//
////////////////////////////////////////////////////////////////////////

using namespace std;

#include <stdlib.h>
#include <iostream>
#include "hhypHardCutsAlg.h"
#include "hpidalghardcuts.h"
#include "hypinfodef.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "hpartialevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hpidgeanttrackset.h"
#include "hgeantheader.h"

ClassImp(HHypHardCutsAlg)

HHypHardCutsAlg::HHypHardCutsAlg(char *name_i, Option_t par[])
  :HHypBaseAlgorithm(name_i,par)
{

  TString * path = GetOpt("cutfile");
  if (path == NULL) {
    Error("HHypHardCutsAlg" , "no cutfile given");
  }


  //hardcuts = new HPidAlgHardCuts(*path);
  // constructor
  sInFileName = *path;
  //sOutFileName = outFileName;
  
  pidParams = NULL;
  
  bSim = kFALSE;
  bCut8 = kFALSE;
  bCut9 = kFALSE;
  bCut14 = kFALSE;

  if (sInFileName.IsNull())
    {
      Error("HPidAlgHardCuts::init","No input file");
    }
  else
    { 
      TFile *fFile;
      // Load param file for graphical cut.
      Info("HPidAlgHardCuts::init","Load file: "+sInFileName);
      fFile = TFile::Open(sInFileName,"READ");
      
      GCut_8 = (TCutG*)fFile->Get("pid_8");
      if (!GCut_8)
	Warning("HPidAlgHardCuts::init","No pi+ cut");
      else bCut8 = kTRUE;
      GCut_9 = (TCutG*)fFile->Get("pid_9");
      if (!GCut_9)
	Warning("HPidAlgHardCuts::init","No input file");
      else bCut9 = kTRUE;
      GCut_14 = (TCutG*)fFile->Get("pid_14");
      if (!GCut_14)
	Warning("HPidAlgHardCuts::init","No input file");
      else bCut14 = kTRUE;
    }


}

HHypHardCutsAlg::~HHypHardCutsAlg()
{
}

Bool_t HHypHardCutsAlg::execute()
{

  if (!beam) {
    cerr << algoName << " needs beam particle! " << endl;
    return kFALSE;
  }
  // Resetting the list and start looping over the combinations
  // Loop is only done over the VALID combinations
  mylist->CombIteratorReset();
  while (mylist->CombIterator()) {
    //cout << "CombIterator" << endl;
    Bool_t removeComb = kFALSE;

    for (int i = 0; i < mylist->getNpart(); i++) {
      //loop over all particles
      Bool_t isLep = kFALSE;

      Int_t hyppid = mylist->getPid(i);
      HPidParticle *particle = mylist->getPidParticle(i);
      
      HPidHitData * pHitData = particle->getTrackCand()->getHitData();
      //First check if we have a shitty lepton :-)
      // Check RICH ring quality by using standard cuts   
      Int_t iSelectedMomAlg = particle->getMomAlg();
      if (pHitData->hasRingCorrelation[iSelectedMomAlg] && ((isLepInRich(2, pHitData) || (isLepInRich(3, pHitData)))))
	isLep=kTRUE;

      if (bCut14 && hyppid == 14 && (!GCut_14->IsInside(mylist->getBeta(i),(mylist->getMomentum(i)).Mag() ))) { 
	removeComb = kTRUE;
	//cout << "bCut14" << endl;
      }
      if (bCut8 && hyppid == 8 && (!GCut_8->IsInside(mylist->getBeta(i),(mylist->getMomentum(i)).Mag()) )) {
	removeComb = kTRUE;
	//cout << "bCut8" << endl;
      }
      if (bCut9 && hyppid == 9 && (!GCut_9->IsInside(mylist->getBeta(i),(-1)*(mylist->getMomentum(i)).Mag()) )) {
	removeComb = kTRUE;
	//cout << "bCut9" << endl;
      }

      if ((hyppid == 2) || (hyppid == 3)) {
	if (!isLep) removeComb = kTRUE;
	//cout << "no lep" << endl;
      }
      
      if ((hyppid != 2) && (hyppid != 3)) {
	if (isLep) removeComb = kTRUE;
	//cout << "lep veto" << endl;
      }

    } // end loop over all particles

#if 0
    if ((removeComb) && (mylist->getProbAlg() > 0.1))  { //remove good combination
      cout << "remove good combination" << endl;
      //exit(1);
    }
    else if ((!removeComb) && (mylist->getProbAlg() < 0.1)) {
      cout << "keep bad combination" << endl;
      //exit(1);
    }
#endif

    if (removeComb) {
      //cout << "remove comb " << mylist->getProbAlg() << endl;
      mylist->removeComb();
    }
      

  } //END Iterator
  if (exitIdx > -1)
    return kTRUE;
  return kFALSE;
}

Bool_t HHypHardCutsAlg::init()
{

  // init parameters
  if((pidParams = (HPidAlgStandCutsPar *)gHades->getRuntimeDb()
      ->getContainer(PIDALGSTANDCUTSPAR_NAME)) == NULL)
    {
      Error("HHypHardCutsAlg::init", "Cannot get parameters: %s", PIDALGSTANDCUTSPAR_NAME);
      return kFALSE;
    }

  return kTRUE;
}

Bool_t HHypHardCutsAlg::reinit()
{
  return kTRUE;
}

Bool_t HHypHardCutsAlg::finalize()
{
  return kTRUE;
}

Bool_t HHypHardCutsAlg::isLepInRich(Int_t iPid,HPidHitData *pHitData) 
{
  // Check RICH ring quality by using standard cuts   
  
  Int_t sec = pHitData->getSector(); 
  
  Stat_t iRingPatMatrThresh 	= pidParams->getValue(iPid,0,sec,0); 
  // particle id, 0 - hist offset, sector, 0 bin value 
  Float_t fRingCentroidThresh = pidParams->getValue(iPid,0,sec,1);
  Stat_t 	iRingPadNrThres    	= pidParams->getValue(iPid,0,sec,2);
  Float_t fRingAvChargeThres  = pidParams->getValue(iPid,0,sec,3);
  Float_t fRingAvChrg = ((Float_t)pHitData->nRingAmplitude)/((Float_t)pHitData->nRingPadNr);
  
  if(pHitData->nRingPatMat <= iRingPatMatrThresh )  return kFALSE;
  if(pHitData->fRingCentroid >= fRingCentroidThresh) return  kFALSE;	
  if(pHitData->nRingPadNr <= iRingPadNrThres ) return  kFALSE;
  if(fRingAvChrg <= fRingAvChargeThres ) return  kFALSE; 	
  
  return kTRUE;
}

