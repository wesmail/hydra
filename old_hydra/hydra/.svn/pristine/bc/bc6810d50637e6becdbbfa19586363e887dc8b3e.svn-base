//-----------------------------------------------------------------------------
// Example of the executable program which projects HPidTrackCand/HPidTrackCandSim  
// category to an ntuple (TNtuple ROOT object). It uses functionality of 
// HSUExeMacro. As the input it requries the DST *.root file 
// with HPidTrackCand or HPidTrackCandSim category. 
//
// To run the program user must select "simulation" or "real" analysis context.
//
// The program must be compiled and linked with HYDRA and ROOT libraries. 
//
// To run the program use:
// ./program -i [input_file.root] -o [output_file.root]
// or
// ./program -i [input_file.root] -o [output_file.root] -e [events]
//
// All options are descibed by using help:
// ./program -h
//
//*--Author: Jacek Otwinowski 06/02/2007
// ----------------------------------------------------------------------------

#include <TNtuple.h>

#include "hcategory.h"
#include "hsuexemacro.h"
#include "hpidtrackcandsim.h"
#include "piddef.h"

//-----------------------------------------------------------------------------
// main function
//-----------------------------------------------------------------------------
int main(int argc, char **argv)
{
  HCategory 		*pPidTrackCandCat = 0;	
  TNtuple   		*pNtuple = 0;
  HPidTrackCandSim 	*pTrack = 0;
  HPidTrackData 	*pTrackData = 0;
  HPidHitData  		*pHitData = 0; 

//-- user settings

  // select "real" or "simulation" events 
  TString context = "simulation";

//-- end of user settings
 
  // create eEM object
  HSUExeMacro eEM;

  // load arguments and set input and output files
  eEM.loadArgs(argc, argv);
  eEM.openInput();
  eEM.openOutput();

  // get category from HTree 
  if(context.CompareTo("simulation") == 0) {
     pPidTrackCandCat = eEM.getHCategory("HPidTrackCandSim");	  
  } 
  else if(context.CompareTo("real") == 0) {
     pPidTrackCandCat = eEM.getHCategory("HPidTrackCand");	  
  }
  else {
	 ::Error("projectProgram","HPidTrackCand/HPidTrackCandSim categories missing in the input");
  }

  // create output ntuple
  const TString kTable="pmat:acharge:npad:rcent:beta_k:mom_k:pull:pol_k:mass_k:beta_s:mom_s:chi2_s:pol_s:mass_s:beta_r:mom_r:chi2_r:pol_r:mass_r:maxF:F10:F20:sum0:sum1:sum2:tof:theta:phi:sec:sys:chi2_inmdc:chi2_outmdc:showerId:tofId:richId:kickId:splineId:rungeId:isRichKick:isRichSpline:isRichRungeKutta";

  pNtuple = new TNtuple("Ntuple","PidTrackCand",kTable.Data());

  // create timer object to benchmark this loop
  TStopwatch timer;
  Int_t i,evN=0;

  Float_t F10=0., F20=0., maxF=0.; 
  Float_t avrCharge=0.;

  timer.Reset();
  timer.Start();

  printf("Processing events...\n");

  // event loop
  while(eEM.nextEvent() >= 0)
  {
	  // event counter
      evN++;
	  if((evN%1000) == 0) printf("evN: %d \n",evN);
	   
	  // hit loop
	  for(i=0; i<pPidTrackCandCat->getEntries(); i++)
	  {
		  if(0 != (pTrack=(HPidTrackCandSim*)pPidTrackCandCat->getObject(i))) {
			
			   pHitData   = pTrack->getHitData();
			   pTrackData = pTrack->getTrackData();

			   // only leptons
			   if(pHitData->iIndRICH < 0) continue; // broad Rich-MDC window 

			   // calculate shower observables
			   if( pHitData->fShowerSum[0]>0)
			   {
				   F10 = pHitData->fShowerSum[1]/pHitData->fShowerSum[0];
				   F20 = pHitData->fShowerSum[2]/pHitData->fShowerSum[0];
				   maxF = TMath::Max(F10,F20);
			   }

			   // calculate rich observables
			   if(pHitData->nRingPadNr > -1)
			   avrCharge = ((Float_t)pHitData->nRingAmplitude)/((Float_t)pHitData->nRingPadNr);

			   // table for ntuple 
			   // ALG_KICK, ALG_SPLINE, ALG_RUNGEKUTTA from piddef.h
			   Float_t vTable[] = 
			   {
                  	// rich
					pHitData->nRingPatMat            // pattern matrix
					,avrCharge                       // avarage charge
					,pHitData->nRingPadNr            // pad number
					,pHitData->fRingCentroid         // ring centroid
					
					// kicktrack
					,pTrackData->fCorrectedBeta[ALG_KICK]   // beta kick
					,pTrackData->fMomenta[ALG_KICK]         // momentum kick
					,pTrackData->fPull               // quality parameters (pull) kick
					,pTrackData->nPolarity[ALG_KICK]        // charge kick
					,pTrackData->fMassSquared[ALG_KICK]     // squered mass kick
					
					//spline
					,pTrackData->fCorrectedBeta[ALG_SPLINE]   // beta spline
					,pTrackData->fMomenta[ALG_SPLINE]         // momentum spline
					,pTrackData->fSplineChiSquare         // quality parameter (chi2) spline
					,pTrackData->nPolarity[ALG_SPLINE]        // charge spline
					,pTrackData->fMassSquared[ALG_SPLINE]     // squered mass spline
					
					//rungekutta
					,pTrackData->fCorrectedBeta[ALG_RUNGEKUTTA]   // beta rungekutta
					,pTrackData->fMomenta[ALG_RUNGEKUTTA]         // momentum rungekutta
					,pTrackData->fRKChiSquare                 // quality parameter (chi2) runge kutta
					,pTrackData->nPolarity[ALG_RUNGEKUTTA]        // charge rungekutta
					,pTrackData->fMassSquared[ALG_RUNGEKUTTA]     // squered mass rungekutta
					
					// shower
					,maxF                            // maximum multiplication factor
					,F10							 // multiplication factor post1/pre
					,F20                             // multiplication factor post2/pre
					,pHitData->fShowerSum[0]         // sum of charge in pre
					,pHitData->fShowerSum[1] 		 // sum of charge in post1
					,pHitData->fShowerSum[2]         // sum of charge in post2

					// common
					,pHitData->getTof()              // time of flight
					,pHitData->fMdcTheta             // theta
					,pHitData->fMdcPhi               // phi
					,pHitData->nSector               // sector
					,pHitData->iSystem               // system
					,pHitData->fInnerMdcChiSquare    // chi2 from inner MDC
					,pHitData->fOuterMdcChiSquare    // chi2 from outer MDC
					,pHitData->iIndShower // index of HShowerHitTof object (-1 no object)
					,pHitData->iIndTOF    // index of HTofHit object (-1 no object)
					,pHitData->iIndRICH   // index of HRichHit object (-1 no object) 

					,pTrackData->nKickTrackInd 	 // index of HKickTrack object (-1 no object) 
					,pTrackData->nSplineTrackInd // index of HSplineTrack object (-1 no object)
					,pTrackData->nRKTrackInd 	 // index of HRKTack  object (-1 no object) 
					,pHitData->hasRingCorrelation[ALG_KICK] // Rich ring and Kick track correlated in the momentum dependent window (1 yes)
					,pHitData->hasRingCorrelation[ALG_SPLINE] // Rich ring and Spline track correlated in the momentum dependent window (1 yes)
					,pHitData->hasRingCorrelation[ALG_RUNGEKUTTA] // Rich ring and RK track correlated in the momentum dependent window (1 yes)
			   }; 

			   // fill ntuple
			   pNtuple->Fill(vTable);
		  }
	  } // end hit loop
  } // end event loop

  timer.Stop();
  
  // benchmark
  printf("------------------------------------------------------\n");
  printf("Events processed: %i\n",evN);
  printf("Real time: %f\n",timer.RealTime());
  printf("Cpu time: %f\n",timer.CpuTime());
  if (evN) printf("Performance: %f s/ev\n",timer.CpuTime()/evN);
  
  // write output file
  eEM.writeOutput();
  
  return 0;
}
