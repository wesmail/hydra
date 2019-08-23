//*-- Author : Stefano Spataro 19/07/2005 
//*-- Modified: Stefano Spataro 05/08/2005 - Now a identified lepton cannot be even an hadron
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidAlgHardCuts                                                            //
//                                                                            //    
// Algorythm for Particle Identification using graphical cuts for hadrons,    //
// and only ring quality selection on leptons.                                //
// Two possible uses: as PID algorythm, and as a function.                    //
//                                                                            //
// *** PID Algorythm ***                                                      //
//                                                                            //
//  HPidAlgHardCuts *pidAlgHardCuts = new HPidAlgHardCuts(inputFile);         //
//  HPidReconstructor *pidReconstructor = new HPidReconstructor(options);     //
//  pidReconstructor->addAlgorithm(pidAlgHardCuts);                           //
//  Short_t pidPart[5]={2,3,8,9,14};                                          //
//  pidReconstructor->setParticleIds(pidPart,sizeof(pidPart)/sizeof(Short_t));//
//  HPidParticleFiller *pidParticleFiller = new HPidParticleFiller(options);  //
//  pidParticleFiller->setAlgorithm(9);                                       //
//                                                                            //
// *** Function ***                                                           //
//                                                                            //
//  HPidAlgHardCuts *pidAlgHardCuts = new HPidAlgHardCuts(inputFile);         //
//  Int_t pid = pidAlgHardCut->getPid(pidTrackCand, nAlgorythm);              //
//                                                                            //
// Where inputFile is a ROOT file with the graphical cuts for protons         //
// (name of the TCutG "cut_14"), pi+ ("cut_8") and pi- ("cut_9"). The lepton  //
// ID is made using HPidAlgStandCutsPar selections on RICH ring quality.      //
// If it is used as function it has to be created with the input namefile;    //
// when the function is called, with the HPidTrackCand object and the number  //	       
// of the tracking algorythm, the output is the particle index (0 if the      //
// PID failed). No lepton ID when it's used as a function.                    // 
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#pragma implementation

#include "hpidalghardcuts.h"
#include "hpidtrackcand.h"
#include "hpidreconstructor.h"
#include "hpidphysicsconstants.h"

#include "hades.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "hpartialevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hpidgeanttrackset.h"
#include "hgeantheader.h"

// -----------------------------------------------------------------------------

ClassImp(HPidAlgHardCuts)

// -----------------------------------------------------------------------------

  HPidAlgHardCuts::HPidAlgHardCuts(TString InParamFile, TString outFileName)
    : HPidAlgorithm("HardCuts", algHardCuts)
{
  // constructor
  sInFileName = InParamFile;
  sOutFileName = outFileName;
  
  pParams = NULL;
  pOutFile =NULL;
  pNtuple =NULL;
  
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

// -----------------------------------------------------------------------------

HPidAlgHardCuts::HPidAlgHardCuts(Float_t fWeight, TString InParamFile, TString outFileName)
  : HPidAlgorithm("HardCuts", algHardCuts, fWeight)
{
  // constructor with weight
  sInFileName = InParamFile;
  sOutFileName = outFileName;
  
  pParams = NULL;
  pOutFile =NULL;
  pNtuple =NULL;
  
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

// -----------------------------------------------------------------------------

Bool_t HPidAlgHardCuts::init(void)
{
  // init parameters
  if((pParams = (HPidAlgStandCutsPar *)gHades->getRuntimeDb()
      ->getContainer(PIDALGSTANDCUTSPAR_NAME)) == NULL)
    {
      Error("HPidAldHardCuts::init", "Cannot get parameters: %s", PIDALGSTANDCUTSPAR_NAME);
      return kFALSE;
    }
  pParams->setContext(pRec->iSelectedMomAlg);
  if (bSim) Info("HPidAldHardCuts::init", "Simulation mode ON");

  // build output file and ntuple
  if (sOutFileName.IsNull() == 0)	
    {
      Info("HPidAlgHardCuts::init","Output NTuple file: "+sOutFileName);
      if(openOutFile() == 0) 
	{ 
	  ::Error("HPidAlgHardCuts::init","Cannot open output file");
	  return kFALSE ;
	}
      if(buildOutNtuple() == 0) 
	{
	  ::Error("HPidAlgHardCuts::init","Cannot build output ntuple");
	  return kFALSE;
	}
    }
  
  return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HPidAlgHardCuts::openOutFile()
{
  // open output ntuple file
  pOutFile = new TFile(sOutFileName.Data(),"recreate");
  if(pOutFile != NULL)  
    {
      pOutFile->cd();
      return kTRUE;
    }
  return kFALSE;	
}

// -----------------------------------------------------------------------------

Bool_t HPidAlgHardCuts::buildOutNtuple()
{
  
  // bild output ntuple for sim/exp
  if( bSim == kFALSE )
    {
      pNtuple = new TNtuple("output","track properties",
			    "pid:beta:mom:charge:mass2:theta:phi:sec:sys:tof:rich:chi2inmdc:chi2outmdc:tofrec");
    }
  else
    {
      pNtuple = new TNtuple("output","Track properties",
			    "pid:beta:mom:charge:mass2:theta:phi:sec:sys:tof:rich:chi2inmdc:chi2outmdc:tofrec:geantpid:geantflag");
    }
  
  if (pNtuple != NULL) return kTRUE;
  
  return kFALSE;
}

// -----------------------------------------------------------------------------

Bool_t HPidAlgHardCuts::writeOutFile(void)
{
  // write output ntuple to file
  if(pOutFile == NULL)
    {
      Error("writeOutFile", "No output file");
      return kFALSE;
    }
	
  if(pOutFile != NULL) 
    {
      Warning("writeOutFile","writing...");

      pOutFile->cd();
      pOutFile->Write();

      delete pOutFile;
      pOutFile = NULL;

      Warning("writeOutFile","ok");
    }
  return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HPidAlgHardCuts::reinit(void)
{
  //Check if parameter context corresponds to the
  //appropriate momentum algorithm index
  //If not -> return kFALSE (analysis can not be done in such case)
  //and print error message
  
  return pParams->checkContext(pRec->iSelectedMomAlg);
}

// -----------------------------------------------------------------------------

Bool_t HPidAlgHardCuts::finalize(void)
{
  // write output file
	
  if(sOutFileName.IsNull()==0)
    {
      return writeOutFile();
    }

  return kTRUE;
}
// -----------------------------------------------------------------------------

Bool_t HPidAlgHardCuts::isLepInRich(Int_t iPid,HPidHitData *pHitData) 
{
  // Check RICH ring quality by using standard cuts   
  
  Int_t sec = pHitData->getSector(); 
  
  Stat_t iRingPatMatrThresh 	= pParams->getValue(iPid,0,sec,0); 
  // particle id, 0 - hist offset, sector, 0 bin value 
  Float_t fRingCentroidThresh = pParams->getValue(iPid,0,sec,1);
  Stat_t 	iRingPadNrThres    	= pParams->getValue(iPid,0,sec,2);
  Float_t fRingAvChargeThres  = pParams->getValue(iPid,0,sec,3);
  Float_t fRingAvChrg = ((Float_t)pHitData->nRingAmplitude)/((Float_t)pHitData->nRingPadNr);
  
  if(pHitData->nRingPatMat <= iRingPatMatrThresh )  return kFALSE;
  if(pHitData->fRingCentroid >= fRingCentroidThresh) return  kFALSE;	
  if(pHitData->nRingPadNr <= iRingPadNrThres ) return  kFALSE;
  if(fRingAvChrg <= fRingAvChargeThres ) return  kFALSE; 	
  
  return kTRUE;
}
// -----------------------------------------------------------------------------

Bool_t HPidAlgHardCuts::isProton(Int_t iPid, HPidTrackData *pTrackData, Int_t iSelectedMomAlg) 
{
  // serach for tracks inside the proton graphical selection
  if (bCut14 && iPid==14 && (GCut_14->IsInside(pTrackData->getBeta(iSelectedMomAlg),pTrackData->fMomenta[iSelectedMomAlg]*pTrackData->nPolarity[iSelectedMomAlg])))
    return kTRUE;
  else
    return kFALSE;
}
// -----------------------------------------------------------------------------

Bool_t HPidAlgHardCuts::isPiP(Int_t iPid, HPidTrackData *pTrackData, Int_t iSelectedMomAlg) 
{
  // serach for tracks inside the pi+ graphical selection
  if (bCut8 && iPid==8 && (GCut_8->IsInside(pTrackData->getBeta(iSelectedMomAlg),pTrackData->fMomenta[iSelectedMomAlg]*pTrackData->nPolarity[iSelectedMomAlg])))
    return kTRUE;
  else
    return kFALSE;
}
// -----------------------------------------------------------------------------

Bool_t HPidAlgHardCuts::isPiM(Int_t iPid, HPidTrackData *pTrackData, Int_t iSelectedMomAlg) 
{
  // serach for tracks inside the pi- graphical selection
  if (bCut9 && iPid==9 && (GCut_9->IsInside(pTrackData->getBeta(iSelectedMomAlg),pTrackData->fMomenta[iSelectedMomAlg]*pTrackData->nPolarity[iSelectedMomAlg])))
    return kTRUE;
  else
    return kFALSE;
}
// -----------------------------------------------------------------------------

Bool_t HPidAlgHardCuts::fillOutNtuple(Int_t iPid,HPidTrackCandSim *pTrack)
{ 
  //fill ntuple in experiment and in simulation
  HPidTrackData *pTrackData = NULL;
  HPidHitData *pHitData = NULL;
  HPidGeantTrackSet *pGeantSet = NULL;
  
  if((pTrackData = pTrack->getTrackData()) == NULL)
    {
      ::Error("HPidAlgHardCuts::fillOutNtuple", "Cannot get pTrackData");
      return kFALSE;
    }
  
  if((pHitData = pTrack->getHitData()) == NULL)
    {
      ::Error("HPidAlgHardCuts::fillOutNtuple", "Cannot get pHitData");
      return kFALSE;
    }
  
  if(bSim == kTRUE)
    {
      if((pGeantSet = pTrack->getGeantTrackSet()) == NULL)
	{
	  ::Error("HPidAlgHardCuts::fillOutNtuple", "Cannot get pGeantSet");
	  return kFALSE;
	}
    }
  
  Int_t   iSelectedMomAlg = pRec->iSelectedMomAlg;
  //pidtrack data
  Float_t charge = pTrackData->nPolarity[iSelectedMomAlg];
  Float_t beta   = pTrackData->getBeta(iSelectedMomAlg);
  Float_t mom    = pTrackData->fMomenta[iSelectedMomAlg];
  Float_t mass2  = pTrackData->getMass2(iSelectedMomAlg);
  Float_t tofrec  = pTrackData->nTofRecFlag[iSelectedMomAlg];
  //pid hit data
  Float_t theta = pHitData->fMdcTheta;
  Float_t phi = pHitData->fMdcPhi;
  Int_t sec = pHitData->getSector(); 
  Int_t sys = pHitData->iSystem;
  Float_t chi2inmdc = pHitData->fInnerMdcChiSquare;
  Float_t chi2outmdc = pHitData->fOuterMdcChiSquare;
  Float_t tof = pHitData->getTof();
  Float_t rich = pHitData->hasRingCorrelation[iSelectedMomAlg];
  
  // fill ntuple
  if(bSim == kFALSE) //experiment
    { 
      Float_t val[14] = {iPid,beta,mom,charge,mass2,
			 theta,phi,sec,sys,tof,
			 rich,chi2inmdc,chi2outmdc,tofrec};
      pNtuple->Fill(val);  
    }
  else
    {
      Float_t val[16] = {iPid,beta,mom,charge,mass2,
			 theta,phi,sec,sys,tof,
			 rich,chi2inmdc,chi2outmdc,tofrec,pGeantSet->getGeantPID(),
			 pGeantSet->getCorrelationFlag()};
      pNtuple->Fill(val);
    }
  
  return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HPidAlgHardCuts::calculatePDF(HPidTrackCand *pTrack,
				     Float_t afReturn[], Short_t &nCatIndex)
{
  // Calculate PDF from data (return 1 in case of lepton in the window)  
  HPidTrackData *pTrackData = NULL;
  HPidHitData *pHitData = NULL;
  
  if((pTrackData = pTrack->getTrackData()) == NULL)
    {
      ::Error("HPidAlgHardCuts::calculatePDF", "Cannot get pTrackData");
      return kFALSE;
    }
  
  if((pHitData = pTrack->getHitData()) == NULL)
    {
      ::Error("HPidAlgHardCuts::calculatePDF", "Cannot get pHitData");
      return kFALSE;
    }
  
  Int_t   iSelectedMomAlg = pRec->iSelectedMomAlg; 
  Int_t iId = 0;
  Int_t Pid = 0;
  Bool_t isLep = kFALSE;

  // loop over predefined particles
  for(Int_t i = 0; i < pRec->particlesNumber(); i++)
    {
      iId = pRec->getParticleId(i);       
      if(iId != 2 && iId != 3 && iId != 8 && iId != 9 && iId != 14) continue;

      afReturn[i] = 0.0f;
      // compare the particle charge
      if(pTrackData->nPolarity[iSelectedMomAlg] != HPidPhysicsConstants::charge(iId))
	continue;
      
      // No particles without "good" tome-of-flight
      if(pTrackData->nTofRecFlag[iSelectedMomAlg] == -1)
	continue;
      
      // calculate PDFs
      if(! HPidPhysicsConstants::isFake(iId))
	{
	  if ((pHitData->hasRingCorrelation[iSelectedMomAlg]) && ((iId==2) || (iId==3)))
            if (isLepInRich(iId, pHitData))
              {
	        afReturn[i] = 1.0f;
                isLep = kTRUE;
              }
	  if (!isLep)
	    {
	      if (iId==14 && isProton(iId, pTrackData, pRec->iSelectedMomAlg)) afReturn[i] = 1.0f;
	      if (iId==8 && isPiP(iId, pTrackData, pRec->iSelectedMomAlg)) afReturn[i] = 1.0f;
	      if (iId==9 && isPiM(iId, pTrackData, pRec->iSelectedMomAlg)) afReturn[i] = 1.0f;
	    }
	}
      if (afReturn[i]==1.0f) Pid = iId;
    }
  
  if((pNtuple != NULL)) // fill control ntuple
    {
      // only correlated lepton tracks in the ntuple
      fillOutNtuple(Pid,(HPidTrackCandSim*)pTrack);
    }
  
  
  return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HPidAlgHardCuts::calculateCL(HPidTrackCand *pTrack,
				    Float_t afReturn[], Short_t &nCatIndex)
{
  return kTRUE;
}

// -----------------------------------------------------------------------------

Int_t HPidAlgHardCuts::getPid(HPidTrackCand *pTrack, Int_t nAlg)
{
  // function which get the pid of a given HPidTrackCand
  Int_t Pid = 0;

  if (isProton(14, pTrack->getTrackData(), nAlg)) Pid = 14;
  if (isPiP(8, pTrack->getTrackData(), nAlg)) Pid = 8;
  if (isPiM(9, pTrack->getTrackData(), nAlg)) Pid = 9;
  return Pid;
}

// -----------------------------------------------------------------------------

void HPidAlgHardCuts::print(void) const
{
  // Empty function for printing info of the algorithm

  printf("\tHardCuts algorithm\n");
}
