#include "hgeantmdc.h"
#include "hmdcdigitizer.h"
#include "hmdcdef.h"
#include "hdebug.h"
#include "hades.h"
#include "hmdcgeantcell.h"
#include "hmdccal1sim.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hmdcdetector.h"
#include "hevent.h"
#include "hcategory.h"
#include "hlocation.h"
#include "hmdclayergeompar.h"
#include "hmdcdigitpar.h"
#include "hmdccal2parsim.h"
#include "hmdccelleff.h"
#include <iostream.h>
#include <TMath.h>
#include <TFile.h>

//*-- Author : A.Nekhaev
//*-- Modified: 05/11/2001 by J.Markert
//*-- Modified: 1/12/2000 by R. Holzmann
//*-- Modified: 30/07/99 by Ilse Koenig
//*-- Modified: 28/06/99 by Alexander Nekhaev

////////////////////////////////////////////////////////////////
//
//  HMdcDigitizer digitizes data and puts output values into 
//  CAL1 category for simulated data
//  OPTIONS:
//
//  HMdcDigitizer("","",option,flag)
//
//  option=1   : TDC mode for two measured leading edges of two signals
//  option=2   : TDC mode for  measured leading and trailing edge of one signal
//  flag=kTRUE : NTuple with internal variables of the digitizer will be filled and written
//               to the outputfile digitizer.root inside the working directory
//  flag=kFALSE: No NTuple is filled
//------------------------------------------------------------------------------------------------------
//  setNTuple(kFALSE) no nTuple with internal data is filled and written out
//           (kTRUE ) a NTuple with internal data is filled and written to digitizer.root
//  setTdcMode(1) TDC mode for two measured leading edges of two signals
//            (2) TDC mode for  measured leading and trailing edge of one signal
//  setOffsets(1.5,2.5,4.5,5.5) sets offsets for each type of module which will be
//                              substracted from the calculated time1 and time2
//  setEffLevel(90.,90.,90.,90.) sets level of cell efficiency cut for each type of module
//  setTofUse(kFALSE) time of flight will not be added to drift times
//           (kTRUE ) time of flight will be added to drift time
//  setNoiseLevel(5.,5.,5.,5.) sets level of noise for each type of module
//  setNoiseRange(low1,low2,low3,low4,hi1,hi2,hi3,hi4) sets the time window of the noise
//                                                     for each module type
//  setNoiseMode(1)(default)GEANT cells will be overwritten by noise (if timenoise<time)
//              (2)         GEANT cells will not be touched
//
//------------------------------------------------------------------------------------------------------
// SHORT INTRODUCTION to HMdcDigitizer:
//
// The Digitizer retrieves the GEANT data from
// HGeantMdc (sector,module,layer, hit: xcoord,ycoord,tof,ptot, incidence:theta,phi,tracknumber).
// HMdcDigitizer::transform(Float_t xcoord, Float_t ycoord, Float_t theta,
//                          Float_t phi   , Float_t tof   , Int_t trkNum) calculates
// which cells have been hit by the track. The Information about the layer geometry
// (pitch, cathod distance, wire orientation, number of wires per layer and number of central wire)
// is taken from HMdcLayerGeomPar. Output of the calculation is the minimum distance from the wire
// and the impact angle in the coordinate system of the cell. All values are stored in the
// HMdcGeantCell for a maximum number of 15 hits per cell via
// HMdcDigitizer::storeCell(Float_t per, Float_t tof, Float_t myangle, Int_t trkNum,
//			    Bool_t flagCutEdge).
// For each cell the drift time1 and time2 are calculated by corresponding functions of
// the HMdcCal2ParSim container which holds the calibration parameters for the "distance->drift time"
// calculation:
// HMdcCal2ParSim::calcTimeDigitizer (sector,module,angle,minDist,&time1,&time1Error) and
// HMdcCal2ParSim::calcTime2Digitizer(sector,module,angle,minDist,&time2,&time2Error).
// For each cell the efficiency cuts are calculated by a function of
// HMdcCellEff::calcEfficiency(module,minDist,angle,Level) which holds
// the information for the efficiency cuts on cell level. The level of the cuts can be specified
// by  HMdcDigitizer::setEffLevel(90.,90.,90.,90.) per module. The cut is done on the basis of GARFIELD
// simulations which give information on the charge which is collected on the sense wire of
// each cell for all combinations of distance and impact angle. The numbers which have to be set
// are the percentage of maximum charge required to make a signal. Hits which are on the edge of
// the drift cell will not create a big amount of charge and therefore will be kicked out first.
// The second cut on the layer level is an overall layer efficiency (e.g. 0.98 for 98% Efficiency)
// and will reduce the overall number of fired cells. This value is taken from HMdcDigiPar container.
// According to the the cuts a list of status flags for each recorded track is filled.
// After all calculations the list of Tracks is sorted by the arrival time (tof + drift time) by
// HMdcDigitizer::select(Int_t nHits) because only the first track will create a signal.
// With HMdcDigitizer::setNoiseLevel(5.,5.,5.,5.) the simulation of noise can be switched on.
// HMdcDigitizer::fillNoise() loops over all cells existing in the actual setup and will randomly
// pick cells according to the specified probability per module (5.==5%) and set the statusflag to 2.
// If a cell is selected for noise production a second random process generates the time in the
// range of the specified window set by HMdcDigitizer::setNoiseRange(low1,low2,low3,low4,hi1,hi2,hi3,hi4).
// With HMdcDigitizer::setNoiseMode(1)(default) a given time in one of the original GEANT cells
// will be overwritten by the the noise time, if timenoise < time.In this case the statusflag of
// the cell will be set to 2 (valid hit but noise).In the case of HMdcDigitizer::setNoiseMode(2) the
// original GEANT cells will not be touched.
// The first valid hit (status=1) inside the track list will be found by
// HMdcDigitizer::findFirstValidHit(Int_t* firsthit, Float_t* firsttime2, Int_t* endlist1)
// which returns the index number of the first valid hit and the the last hit which falls
// into the given time window defined by time2 of first valid hit.
// HMdcDigitizer::findSecondValidHit(Int_t endlist1,Int_t* secondhit)
// finds a second valid hit starting with the last entry of the list of the first valid hit.
// All variables will return -999 if no valid hits are found.
// According to the two different TDC modes the HMdcCal1Sim category is filled.
// If HMdcDigitizer::setTofUse(kFALSE) is selected, the time of flight will be substracted and
// only the drift time is written to the output. With HMdcDigitizer::setOffsets(1.5,2.5,4.5,5.5)
// a common minimum offset (fast particles) per module type can be set and will be subtracted
// from the calculated time to be closer to the real measurement situation.
//
//------------------------------------------------------------------------------------------------------
// MODE 1 (two times LEADING EDGE of the TDC signal)
//           nHits ==  -2 for 2 valid hits
//                 ==  -1 for 1 valid hit
//                 ==   0 for a not filled hit (e.g. 1 hit was kicked out by efficiency)
//         status1 ==   1 for a valid first hit
//                 ==   2 for a valid first hit caused by noise
//                 ==  -3 for a not valid hit
//         status2 ==   1 for a valid second hit
//                 ==   2 for a valid second hit caused by noise
//                 ==  -3 for a not valid hit
//         nTrack1 ==   track number of the first valid hit
//                 ==  -99 if not filled
//         nTrack2 ==   track number of the second valid hit
//                 ==  -99 if not filled
//         time1   ==   drift time1 of the first valid hit
//                 ==  -999 if not filled
//         time2   ==   drift time1 of the second valid hit
//                 ==  -999 if not filled
//     listTrack[5] :   contains the track number of the first 5 hits per cell
//                      == -99 if no hit was filled
//     listStatus[5]:   contains the status flags of the first 5 hits per cell
//                      == -1 if hit was kicked out by cell efficiency cut
//                      == -2 if hit was kicked out by layer efficiency cut
//                      ==  1 if hit is valid
//                      ==  2 if hit is noise
//                      == -99 if no hit was filled
//     both lists will be filled even if no vaild hit was found
//
// MODE 2 (LEADING AND TRAILING EDGE of the TDC signal)
//           nHits ==  +2 for 2 valid hits
//                 ==   0 for not filled hit (e.g. 1 hit was kicked out by efficiency)
//         status1 ==   1 for a valid first hit
//                 ==   2 for a valid first hit caused by noise
//                 ==  -3 for a not valid hit
//         status2 ==   1 for a valid first hit
//                 ==   2 for a valid first hit caused by noise
//                 ==  -3 for a not valid hit
//         nTrack1 ==   track number of first valid hit
//                 ==  -99 if not filled
//         nTrack2 ==   track number of first valid hit
//                 ==  -99 if not filled
//         time1   ==   drift time1 of the first valid hit
//                 ==  -999 if not filled
//         time2   ==   drift time2 of the first valid hit
//                 ==  -999 if not filled
//     listTrack[5] :   contains the track number of the first 5 hits per cell
//                      == -99 if no hit was filled
//     listStatus[5]:   contains the status flags of the first 5 hits per cell
//                      == -1 if hit was kicked out by cell efficiency cut
//                      == -2 if hit was kicked out by layer efficiency cut
//                      ==  1 if hit is valid
//                      ==  2 if hit is noise
//                      ==  0 if no hit was filled
//     both lists will be filled even if no vaild hit was found
//------------------------------------------------------------------------------------------------------
//     EXAMPLES :
//     In general: if nHits<0 ->TDC MODE=1
//                 if nHits>0 ->TDC MODE=2
//                 if status1/status2>0 -> valid hit (1: normal, 2: noise)
//                 if status1/status2<0 -> no valid hit (-3)
//
//     TDC MODE 1 (2 leading edges)
//     no valid hit:  status1=status2=-3 rest will be filled like normal hits
//     1 valid hit :  time1!=-999,time2=-999,status1=1,status2=-3,nHits=-1,nTrack1!=-99,nTrack2=-99
//     2 valid hits:  time1!=-999,time2!=-999,status1=status2=1,nHits=-2,nTrack1!=-99,nTrack2!=-99
//     noise hit   :  if GEANT hit was overwritten:status1/status2 =2 rest filled like normal hits
//                    if a noise cell was added to GEANT cells:
//                    time1!=-999,time2=-999,status1=2,status2=-3,nHits=-1,
//                    nTrack1=nTrack2=-99
//
//     TDC MODE 2 (leading and trailing edge)
//     no valid hit:  status1=status2=-3 rest will be filled like normal hit
//     1 valid hit :  time1!=-999,time2!=-999,status1=status2=1,nHits=2,nTrack1=nTrack2!=-99
//     noise hit   :  if GEANT hit was overwritten:status1=status2=2 rest filled like normal hits
//                    if a noise cell was added to GEANT cells:
//                    time1!=-999,time2!=-999,status1=status2=2,nHits=2,
//                    nTrack1=nTrack2=-99
//
//    MODE1 and MODE2 :
//
//    if status1/status2=-3 looking to the StatusList[5]: -1 cut on Cell efficiency
//                                                        -2 cut on Layer efficiency
//    The TrackList[5] will be filled with the GEANT track numbers no matter if the hit was
//    valid or not or overwritten by noise!
/////////////////////////////////////////////////////////////////

Float_t HMdcDigitizer::dTime [15]={0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
Float_t HMdcDigitizer::dTime2[15]={0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
Float_t HMdcDigitizer::dTimeErr [15]={0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
Float_t HMdcDigitizer::dTime2Err[15]={0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
Float_t HMdcDigitizer::minimumdist[15]={0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
Int_t HMdcDigitizer::track[15]={-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99};
Float_t HMdcDigitizer::timeOfFlight[15]={0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
Float_t HMdcDigitizer::angle[15]={0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
Int_t HMdcDigitizer::statusflag[15]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
Bool_t HMdcDigitizer::cutEdge[15]={kFALSE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE};


HMdcDigitizer::HMdcDigitizer(void) {
  fGeantCellCat = 0;
  fCalCat = 0;
  fDigitGeomPar = 0;
  fDigitPar = 0;
  fCal=0;
  fCalnoise=0;
  fCell=0;
  iterin = 0;
  itercell = 0;
  fEventId = 0;
  pi = acos(-1.)/180;
  time1=0;
  time1Error=0;
  time2=0;
  time2Error=0;
  setTdcMode(1);
  setEffLevel(90,90,90,90);
  setNoiseLevel(5.,5.,5.,5.);
  setOffsets(1.5,2.5,4.5,5.5);
  useOffsets=kFALSE;
  useCellEff=kFALSE;
  useNoise=kFALSE;
  useTof=kTRUE;
  fntuple = kFALSE;
  setTime1Noise(0.);
  setTime2Noise(0.);
  resetListVariables();
  setNoiseMode(1);
  for(Int_t i=0;i<5;i++)
  {
      arrayNoise[i]=0;
  }
  setNoiseRange(-500,-500,-500,-500,
		1500,1500,1500,1500);
}
HMdcDigitizer::HMdcDigitizer(Text_t *name,Text_t *title):
               HReconstructor(name,title){
  fGeantCellCat = 0;
  fCalCat = 0;
  fDigitGeomPar = 0;
  fDigitPar = 0;
  fCal=0;
  fCalnoise=0;
  fCell=0;
  iterin = 0;
  itercell = 0;
  fEventId = 0;
  pi = acos(-1.)/180;
  time1=0;
  time1Error=0;
  time2=0;
  time2Error=0;
  setTdcMode(1);
  setEffLevel(90,90,90,90);
  setNoiseLevel(5.,5.,5.,5.);
  setOffsets(1.5,2.5,4.5,5.5);
  useOffsets=kFALSE;
  useCellEff=kFALSE;
  useNoise=kFALSE;
  useTof=kTRUE;
  fntuple = kFALSE;
  setTime1Noise(0.);
  setTime2Noise(0.);
  resetListVariables();
  setNoiseMode(1);
  for(Int_t i=0;i<5;i++)
  {
      arrayNoise[i]=0;
  }
  setNoiseRange(-500,-500,-500,-500,
		1500,1500,1500,1500);

}

HMdcDigitizer::HMdcDigitizer(Text_t *name,Text_t *title,Int_t TDCMODE,Bool_t NTUPLE) :
               HReconstructor(name,title) {
     	   // TDCMODE sets the simulation mode to the two different
       	   // possibilities of the TDC: 1 == to leading edges
       	   //                           2 == leading and trailing edge
       	   // NTUPLE switches the NTuple with the internal information of
       	   // the digitier ON or OFF:   kFALSE == no NTuple filled
           //                           kTRUE  == NTuple filled.
  fGeantCellCat = 0;
  fCalCat = 0;
  fDigitGeomPar = 0;
  fDigitPar = 0;
  fCal=0;
  fCalnoise=0;
  fCell=0;
  iterin = 0;
  itercell = 0;
  fEventId = 0;
  pi = acos(-1.)/180;
  time1=0;
  time1Error=0;
  time2=0;
  time2Error=0;
  setTdcMode(1);
  setEffLevel(90,90,90,90);
  setNoiseLevel(5.,5.,5.,5.);
  setOffsets(1.5,2.5,4.5,5.5);
  useOffsets=kFALSE;
  useCellEff=kFALSE;
  useNoise=kFALSE;
  useTof=kTRUE;
  fntuple = kFALSE;
  setTime1Noise(0.);
  setTime2Noise(0.);
  resetListVariables();
  setNoiseMode(1);
  for(Int_t i=0;i<5;i++)
  {
      arrayNoise[i]=0;
  }
  setNoiseRange(-500,-500,-500,-500,
		1500,1500,1500,1500);

}

HMdcDigitizer::~HMdcDigitizer(void) {
  

  if(iterin) delete iterin;
  if(itercell) delete itercell;
}

void HMdcDigitizer::setParContainers() {
    // Get pointers to the needed containers.The containers are
    // created and added to the runtime Database if the are not existing

    fDigitGeomPar = (HMdcLayerGeomPar*)gHades->getRuntimeDb()
	->getContainer("MdcLayerGeomPar");
    if (!fDigitGeomPar) {
	fDigitGeomPar=new HMdcLayerGeomPar;
	gHades->getRuntimeDb()->addContainer(fDigitGeomPar);
    }
    fDigitPar=(HMdcDigitPar*)gHades->getRuntimeDb()->getContainer("MdcDigitPar");
    if (!fDigitPar) {
	fDigitPar=new HMdcDigitPar;
	gHades->getRuntimeDb()->addContainer(fDigitPar);
    }
    fCal2ParSim=(HMdcCal2ParSim*)gHades->getRuntimeDb()->getContainer("MdcCal2ParSim");
    if (!fCal2ParSim) {
	fCal2ParSim=new HMdcCal2ParSim;
	gHades->getRuntimeDb()->addContainer(fCal2ParSim);
    }
    if(useCellEff)
    {
	fCellEff=(HMdcCellEff*)gHades->getRuntimeDb()->getContainer("MdcCellEff");
	if (!fCellEff) {
	    fCellEff=new HMdcCellEff;
	    gHades->getRuntimeDb()->addContainer(fCellEff);
	}
    }
}

void HMdcDigitizer::setNTuples(void) {
    // Creates an NTuple for the internal infomation of the digitizer
    // which is stored in the file digitizer.root in the working directory.
    // The NTuple contains sec,mod,lay,cell,dist,angle,
    // time1, time1Err,time2,time2Err,tof,cutEdge,status and track.
    myoutput->cd();
    distance_time = new TNtuple("cal2sim", "cal2sim", "sec:mod:lay:cell:dist:angle:time1:time1Err:time2:time2Err:tof:cutEdge:status:track");
}

Bool_t HMdcDigitizer::init(void) {
    // The parameter containers and the iterators over
    // the categorys MdcGeanRaw, MdcGeantCell MdcCal1Sim are created.
    // The actual setup of the Mdc detector in the running analysis
    // is retrieved.
    setParContainers();
    getMdcSetup();

    fGeantMdcCat = (gHades->getCurrentEvent()->getCategory(catMdcGeantRaw));
  if(!fGeantMdcCat) {
      Error("HMdcDigitizer::init()","HGeant MDC input missing");
      return kFALSE;
  }
  iterin = (HIterator*)fGeantMdcCat->MakeIterator("native");

  fGeantCellCat = gHades->getCurrentEvent()->getCategory(catMdcGeantCell);
  if (!fGeantCellCat) {
      fGeantCellCat = gHades->getSetup()->getDetector("Mdc")
	  ->buildCategory(catMdcGeantCell);
      if (!fGeantCellCat) return kFALSE;
      else gHades->getCurrentEvent()->
	  addCategory(catMdcGeantCell,fGeantCellCat,"Mdc");
  }
  fGeantCellCat->setPersistency(kFALSE);   // We don't want to write this one
  itercell = (HIterator*)fGeantCellCat->MakeIterator("native");

  fCalCat=gHades->getCurrentEvent()->getCategory(catMdcCal1);
  if (!fCalCat) {
      HMdcDetector* mdc=(HMdcDetector*)(gHades->getSetup()->getDetector("Mdc"));
      fCalCat=mdc->buildMatrixCategory("HMdcCal1Sim",0.5F);
      if (!fCalCat) return kFALSE;
      else gHades->getCurrentEvent()->addCategory(catMdcCal1,fCalCat,"Mdc");
  } else {
      if (fCalCat->getClass()!=HMdcCal1Sim::Class()) {
	  Error("HMdcDigitizer::init()","Misconfigured output category");
	  return kFALSE;
      }
  }

  
  if(fntuple)
  {
      // create output file and NTuple
      myoutput=new TFile("digitizer.root","RECREATE");
      myoutput->cd();
      setNTuples();
  }
  printStatus();
  return kTRUE;
}
void HMdcDigitizer::printStatus()
{
    // Prints the Options, default settings and
    // actual configuration of HMdcDigitizer.

  printf("****************************HMdcDigitizer****************************\n");
  printf("*  DEFAULT SETTINGS                                                 *\n");
  printf("*-------------------------------------------------------------------*\n");
  printf("*  Options input 1 (default)      two leading edges                 *\n");
  printf("*                2                leading and trailing edge         *\n");
  printf("*  NTuple        kFALSE (default) no NTuple filled                  *\n");
  printf("*                kTRUE            NTuple in digitizer.root filled   *\n");
  printf("*  Use Offsets   kFALSE (default)                                   *\n");
  printf("*  Use Tof       kTRUE  (default) cal1sim = drift time + tof        *\n");
  printf("*  Use Cell Eff  kFALSE (default)                                   *\n");
  printf("*  Use Noise     kFALSE (default)                                   *\n");
  printf("*  Noise mode    1 (default) GEANT hits will be replaced by noise   *\n");
  printf("*-------------------------------------------------------------------*\n");
  printf("*  ACTUAL CONFIGURATION:                                            *\n");
  if(modetdc==1) //two times leading edge
  {
      printf("*  TDC mode: two leading edges                                      *\n");
  }
  if(modetdc==2) //leading and trailing edge
  {
      printf("*  TDC mode: leading and trailing edge                              *\n");
  }
  if(fntuple)
  {
      printf("*  NTuple in output file digitizer.root will be filled              *\n");
  }
  if(!fntuple)
  {
      printf("*  No NTuple will be filled                                         *\n");
  }
  if(useOffsets)
  {
      printf("*  Offsets for fastest particles will be subtracted                 *\n");
      printf("*  Offsets: MDC1= %3.1f ns, MDC2= %3.1f ns, MDC3= %3.1f ns, MDC4= %3.1f ns  *\n",
	     getOffset(0),getOffset(1),getOffset(2),getOffset(3));
  }
  if(!useOffsets)
  {
      printf("*  No Offsets will be subtracted (default)                          *\n");
  }
  if(!useTof)
  {
      printf("*  Time of Flight will be substracted                               *\n");
  }
  if(useTof)
  {
      printf("*  Time of Flight will be included  (default)                       *\n");

  }
    if(useCellEff)
  {
      printf("*  Cell efficiency Level  : %4.1f%% , %4.1f%% , %4.1f%% , %4.1f%%           *\n",
	    100-getCellEffLevel(0), 100-getCellEffLevel(1),100-getCellEffLevel(2),100-getCellEffLevel(3));
  }
  if(!useCellEff)
  {
      printf("*  Cell efficiency Level  :  no Cell Efficiency cut used            *\n");
  }
  if(useNoise)
  {
      printf("*  Noise Level            : %4.1f%% , %4.1f%% , %4.1f%% , %4.1f%%           *\n",
	     100-(100*getNoiseLevel(0)),100-(100*getNoiseLevel(1)),
	     100-(100*getNoiseLevel(2)),100-(100*getNoiseLevel(3)));
      printf("*  Noise Range  :                                                   *\n");
      printf("*     MDC1 Lo  : %5i ns  Hi  : %5i ns                           *\n",
	     getNoiseRangeLo(0),getNoiseRangeHi(0));
      printf("*     MDC2 Lo  : %5i ns  Hi  : %5i ns                           *\n",
	     getNoiseRangeLo(1),getNoiseRangeHi(1));
      printf("*     MDC3 Lo  : %5i ns  Hi  : %5i ns                           *\n",
	     getNoiseRangeLo(2),getNoiseRangeHi(2));
      printf("*     MDC4 Lo  : %5i ns  Hi  : %5i ns                           *\n",
	     getNoiseRangeLo(3),getNoiseRangeHi(3));
  }
  if(!useNoise)
  {
      printf("*  Noise Level  :  no Noise simulation used                         *\n");
  }
  if(useNoise&&noisemode==1)
  {
      printf("*  Noise mode  : 1 GEANT cells will be replaced by noise            *\n");
 

  }
  if(useNoise&&noisemode==2)
  {
      printf("*  Noise mode  : 2 GEANT cells will not be replaced by noise        *\n");
 

  }
  printf("*********************************************************************\n");




}


Bool_t HMdcDigitizer::finalize(void)
{
    // If NTuple exist it will be written to digitizer.root.
    if(fntuple)
    {
        // The NTuple is written to the output file
	myoutput->cd();
	distance_time->Write();
	myoutput->Save();
	myoutput->Close();
    }
    return kTRUE;
}

Int_t HMdcDigitizer::execute(void) {
    // GEANT data are retrieved from HGeantMdc.
    // The GEANT hits will be transformed to the
    // layer coordinate system of the Mdc to find
    // the fired cells. For the fired cells the drift time
    // calulation, noise generation and efficiency cuts will
    // be performed according to the settings and the results
    // will be stored in HMdcCal1Sim.

  Float_t xcoord, ycoord, tof, theta, phi, ptot;
  Int_t trkNum;
  myalpha=0;
  HGeantMdc* fGeant;
  loc.set(4,0,0,0,0);   // location used to fill the HMdcGeantCell category

  iterin->Reset();
  while((fGeant=(HGeantMdc*)iterin->Next()) != NULL) {// loop over HGeant input
      loc[0] = (Int_t)(fGeant->getSector());
      loc[1] = (Int_t)(fGeant->getModule());

      if(!testMdcSetup(loc[0],loc[1]) ) continue; // checks if the module is present in the setup

      loc[2] = (Int_t)(fGeant->getLayer());
      //   loc[3] is filled in transform(...) with the cell number
      fGeant->getHit(xcoord, ycoord, tof, ptot);
      fGeant->getIncidence(theta, phi);
      trkNum = fGeant->getTrack();
      if(loc[2]<6) transform(xcoord,ycoord,theta,phi,tof,trkNum);// transform and store
  }

  fCell = 0;
  fCal = 0;

  initArrays();

  itercell->Reset();

  setLoopVariables(0,0,0,0);

  while ((fCell=(HMdcGeantCell *)itercell->Next()) != NULL) {
     initArrays();
     loc[0] = fCell->getSector();
     loc[1] = fCell->getModule();
     loc[2] = fCell->getLayer();
     loc[3] = fCell->getCell();

    if(useNoise)
    {
	// loop over all cells in actual existing Setup
	// and pick randomly cells.If a cell is selected
	// a random time is filled and the cell is stored
        // in the output
	fillNoise(firstsec,firstmod,firstlay,firstcell,
		  loc[0],loc[1],loc[2],loc[3]);

	setLoopVariables(loc[0],loc[1],loc[2],loc[3]+1);
    }

    fCal = (HMdcCal1Sim*)fCalCat->getSlot(loc);
    if (fCal) {
	fCal = new(fCal) HMdcCal1Sim;
	fCal->setAddress(loc[0],loc[1],loc[2],loc[3]);

	// Digitisation procedure starts here:


	// First TDC signal
	Int_t nHits = fCell->getNumHits();
	for(Int_t ihit = 0;ihit<nHits;ihit++)
	{
	    fillArrays(ihit,loc[0],loc[1],fCell); // fill arrays with all variables needed
	    setEfficiencyFlags(ihit,loc[0],loc[1],loc[2]);// checks for efficiency cuts and sets the propper statusflags
	}
        if (nHits>1) select(nHits);  // sort all hits by arrival time (tof + drifttime)

	resetListVariables();

	//fCell->clear();      // clear the container


	if(useNoise && noisemode==1 && ( gRandom->Rndm()>getNoiseLevel(loc[1])) )
	{
	    // check if the time1 of the noise hit is smaller than time1 of the real hit
	    // if this case is fullfilled the real hit will be overwritten by the noise hit.
	    // if no valid hit was found the noise hit will be written.

	    fillNoiseToGeantCells(loc[1]);
	}
	if(fntuple)
	{
	    // fill Ntuple with internal information of the Digitizer
	    for(Int_t hit=0;hit<15;hit++)
	    {
		if(getStatus(hit)==0)continue;
		fillNTuple(loc[0],loc[1],loc[2],loc[3],hit,
			   fCell,distance_time);

	    }
	}

	findFirstValidHit();

	if(modetdc==1||modetdc==2)  // both TDC modes
	{
	    if(getFirstHit()!=-999) // if a valid hit was found
	    {
		if(useOffsets)
		{
		    if(!useTof)fCal->setTime1(getDTime1(getFirstHit())-getTof(getFirstHit())-getOffset(loc[1])); // drift times - offsets are stored
		    if(useTof) fCal->setTime1(getDTime1(getFirstHit())-getOffset(loc[1])); //  drift times + tof - offsets are stored
		}
		if(!useOffsets)
		{
		    if(!useTof)fCal->setTime1(getDTime1(getFirstHit())-getTof(getFirstHit())); // only drift times are stored
		    if(useTof) fCal->setTime1(getDTime1(getFirstHit())); //  drift times + tof are stored
		}
		fCal->setNTrack1(getTrackN(getFirstHit()));
		fCal->setStatus1(getStatus(getFirstHit()));
	    }
	    else
	    {
		fCal->setStatus1(-3); // no vaild hit1 found
	    }

	    fillTrackList(fCal); // fill list of tracks and statusflags
	                                          // even if no valid hit was in
	}

	if(modetdc==2)  // leading and trailing edge
	{
	    if(getFirstHit()!=-999) // if a valid hit was found
	    {
		if(useOffsets)
		{
		    if(!useTof)fCal->setTime2(getDTime2(getFirstHit())-getTof(getFirstHit())-getOffset(loc[1])); // drift times - offsets are stored
		    if(useTof) fCal->setTime2(getDTime2(getFirstHit())-getOffset(loc[1])); // drift times + tof - offsets are stored
		}
		if(!useOffsets)
		{
		    if(!useTof)fCal->setTime2(getDTime2(getFirstHit())-getTof(getFirstHit())); // only drift times are stored
		    if(useTof) fCal->setTime2(getDTime2(getFirstHit())); // drift times + tof are stored
		}
		fCal->setNTrack2(getTrackN(getFirstHit())); // fill same track number as for time1
		fCal->setNHits(2);   // second hit = trailing edge
		fCal->setStatus2(getStatus(getFirstHit())); // status is ok
	    }
	    else
	    {
		fCal->setStatus2(-3); // no vaild hit2 found
	    }
	}

	if (nHits==1 && modetdc==1) fCal->setNHits(-1);  // if only one hit was detected
	else
	{
	    if(nHits>1 && modetdc==1 && getFirstHit()!=-999)  // two times leading edge
	    {
		findSecondValidHit();

		if (getSecondHit()==-999 )
		{
		    fCal->setNHits(-1); // if no valid hit2 was found
		}
		else
		{
		    if(useOffsets)
		    {
			if(!useTof)fCal->setTime2(getDTime1(getSecondHit())-getTof(getSecondHit())-getOffset(loc[1]));  // only drift times - offsets are stored
			if(useTof) fCal->setTime2(getDTime1(getSecondHit())-getOffset(loc[1]));  //  drift times + tof - offsets are stored
		    }
		    if(!useOffsets)
		    {
			if(!useTof)fCal->setTime2(getDTime1(getSecondHit())-getTof(getSecondHit()));  // only drift times are stored
			if(useTof) fCal->setTime2(getDTime1(getSecondHit()));  // drift times + tof are stored
		    }
		    fCal->setNTrack2(getTrackN(getSecondHit())); // number of second track is stored
		    fCal->setNHits(-2);   // second valid hit was found
		    fCal->setStatus2(getStatus(getSecondHit())); // status of hit2 is ok
		}
	    }
	    else if(nHits>1 && modetdc==1)
	    {
		fCal->setStatus2(-3); // no valid second hit was found
	    }
	}
    }
  }
  if(useNoise)
  {
      fillNoise(firstsec,firstmod,firstlay,firstcell, // fill rest of noise cells
		5,3,5,999);
  }
  return 0;
}

void HMdcDigitizer::fillArrays(Int_t ihit,Int_t sec,Int_t mod,HMdcGeantCell* fCell)
{
    // All needed Arrays for the calculations are
    // filled (minimumdist,angle,tracknumber,tof,time1,time2,time1err,time2err)

    setMinimumDist(ihit,fCell->getMinDist(ihit));
    setAngle(ihit,fCell->getImpactAngle(ihit));
    setTrackN(ihit,fCell->getNTrack(ihit));
    setTof(ihit,fCell->getTimeFlight(ihit));

    fCal2ParSim->calcTimeDigitizer(sec,mod,getAngle(ihit),(getMinimumDist(ihit)),&time1,&time1Error);
    setDTime1(ihit,time1 + time1Error + fCell->getTimeFlight(ihit));
    setDTime1Err(ihit,time1Error);

    fCal2ParSim->calcTime2Digitizer(sec,mod,getAngle(ihit),(getMinimumDist(ihit)),&time2,&time2Error);
    setDTime2(ihit,time2 + time2Error + fCell->getTimeFlight(ihit));
    setDTime2Err(ihit,time2Error);

    setCutEdge(ihit,fCell->getFlagCutEdge(ihit));
}
void HMdcDigitizer::setEfficiencyFlags(Int_t ihit,Int_t sec,Int_t mod,Int_t lay)
{
    // Cuts for cell efficiency and layer efficiency are checked
    // and the statusflag of the cells are set correspondingly

    if(getAngle(ihit)<=180&&   getAngle(ihit)>90)   setAngle(ihit,180-getAngle(ihit));
    if(useCellEff)
    {
	setStatus(ihit,fCellEff->calcEfficiency(mod,getMinimumDist(ihit),getAngle(ihit),getCellEffLevel(mod)));
    }
    else
    {
	setStatus(ihit,1);
    }

    // Efficiency of MDC can be less then 100%...
    HMdcDigitParLayer& layer = (*fDigitPar)[sec][mod][lay];
    if(gRandom->Rndm() >layer.getMdcEfficiency())
    {
	switch (getStatus(ihit))
	{

	case   1: setStatus(ihit,-2); // if it is kicked out by layer efficiency
	break;
	case  -1: setStatus(ihit,-1); // if it was kicked out by cell efficiency
	break;
	default : setStatus(ihit,-3); // just control
	break;
	}

    }
}
void HMdcDigitizer::fillNTuple(Int_t sec,Int_t mod,Int_t lay,Int_t cell,Int_t ihit,
			       HMdcGeantCell* fCell, TNtuple* distance_time)
{
    // The NTuple is filled with internal data of the digitizer

    if(getAngle(ihit)<=90 && getAngle(ihit)>=0)
    {
	distance_time->Fill(sec,mod,lay,cell,
			    getMinimumDist(ihit),
			    getAngle(ihit),
			    getDTime1(ihit),
			    getDTime1Err(ihit),
			    getDTime2(ihit),
			    getDTime2Err(ihit),
			    getTof(ihit),
                            getCutEdge(ihit),
			    getStatus(ihit),
			    getTrackN(ihit)
			   );
    }
}
void HMdcDigitizer::fillNTuple(Int_t sec,Int_t mod,Int_t lay,Int_t cell,
			       Float_t time, Float_t time2,
			       Int_t status)
{
    // The NTuple is filled with internal data of the digitizer

    distance_time->Fill(sec,mod,lay,cell,
			-1,
			-1,
			time,0,
			time2,0,
			0,
			0,
			status,
			-99);
}

Float_t HMdcDigitizer::fillTime1Noise(Int_t mod)
{
    // A random time in a specified time window
    // for the noise simulation is calculated
    Float_t time1Noise=-999;
    time1Noise= (gRandom->Rndm()*(getNoiseRangeHi(mod)-getNoiseRangeLo(mod))) + getNoiseRangeLo(mod);
    return  time1Noise;
}
void HMdcDigitizer::fillNoiseToGeantCells(Int_t mod)
{
    // If timenoise < time of the first valid GEANT hit
    // the time is replaced by timenoise and the
    // statusflag is changed from 1 (valid) to 2 (valid but noise)

    findFirstValidHit();

    setTime1Noise(fillTime1Noise(mod));
    setTime2Noise(getTime1Noise() + 20);

    if(getFirstHit() !=-999)   // valid first hit was found
    {
	if(getTime1Noise() < getDTime1(getFirstHit()))
	{
	    setStatus(getFirstHit(),2);       // noise hit
	    setDTime1(getFirstHit(),getTime1Noise());  // old time1 is replaced by noise
	    setDTime2(getFirstHit(),getTime2Noise());  // old time2 is replaced by noise
	    setTof(getFirstHit(),0);     // tof->0 to put the correct noise time to the output
	}
    }
    else if(getFirstHit()==-999) // no valid first hit was found
    {
	setStatus(0,2);       // noise hit
	setDTime1(0,getTime1Noise());  // old time1 is replaced by noise
	setDTime2(0,getTime2Noise());  // old time2 is replaced by noise
	setTof(0,0);     // tof->0 to put the correct noise time to the output
    }

    resetListVariables();

}
void HMdcDigitizer::fillNoise(Int_t firstsec, Int_t firstmod, Int_t firstlay, Int_t firstcell,
			      Int_t lastsec, Int_t lastmod, Int_t lastlay, Int_t lastcell)
{
    // Fills the noise cells to Cal1Sim up to the next GEANT Cell
    // and the rest of the noise cells after the last GEANT Cell (if
    // input lastcell==-999).

    fCalnoise = 0;
    locnoise.set(4,0,0,0,0);

    for (Int_t sec=firstsec;sec<=lastsec;sec++)
    {
	for (Int_t mod=firstmod;mod<=lastmod;mod++)
	{
            // test if module is existing in current setup
	    if(!testMdcSetup(sec,mod) )continue;

	    for (Int_t lay=firstlay;lay<=lastlay;lay++)
	    {
                if(lastcell!=999)
		{
		    for (Int_t cell=firstcell;cell<lastcell;cell++)
		    {
                        if(gRandom->Rndm()>getNoiseLevel(mod))
			{
			    locnoise[0]=sec;
			    locnoise[1]=mod;
			    locnoise[2]=lay;
			    locnoise[3]=cell;

			    fCalnoise = (HMdcCal1Sim*)fCalCat->getSlot(locnoise);
			    if (fCalnoise)
			    {
				fCalnoise = new(fCalnoise) HMdcCal1Sim;
				fCalnoise->setAddress(sec,mod,lay,cell);
				if (modetdc==1)
				{
				    fCalnoise->setNHits(-1);
				    fCalnoise->setStatus1(2);
				    fCalnoise->setStatus2(-3);
				    setTime1Noise((gRandom->Rndm()*(getNoiseRangeHi(mod)-getNoiseRangeLo(mod))) + getNoiseRangeLo(mod));
                                    setTime2Noise(-999);
				    fCalnoise->setTime1(getTime1Noise());
				    arrayNoise[0]=2;
				    fCalnoise->setStatusList(arrayNoise); // store array in cal1sim level
				    if(fntuple)
				    {
					fillNTuple(locnoise[0],locnoise[1],locnoise[2],locnoise[3],
						   getTime1Noise(),getTime2Noise(),2);
				    }
				}
				else if(modetdc==2)
				{
				    fCalnoise->setNHits(2);
				    fCalnoise->setStatus1(2);
				    fCalnoise->setStatus2(2);
				    setTime1Noise((gRandom->Rndm()*(getNoiseRangeHi(mod)-getNoiseRangeLo(mod))) + getNoiseRangeLo(mod));
                                    setTime2Noise(getTime1Noise()+20);
				    fCalnoise->setTime1(getTime1Noise());
				    fCalnoise->setTime2(getTime2Noise());
				    arrayNoise[0]=2;
				    fCalnoise->setStatusList(arrayNoise); // store array in cal1sim level
				    if(fntuple)
				    {
					fillNTuple(locnoise[0],locnoise[1],locnoise[2],locnoise[3],
						   getTime1Noise(),getTime2Noise(),2);
				    }
				}
			    }
			}
		    }
		}
                if(lastcell==999)
		{
		    HMdcLayerGeomParLay& layernoise = (*fDigitGeomPar)[sec][mod][lay];
		    Int_t   nWires   = layernoise.getNumWires(); // number of wires per layer

		    for (Int_t cell=firstcell;cell<nWires;cell++)
		    {
                        if(gRandom->Rndm()>getNoiseLevel(mod))
			{
			    locnoise[0]=sec;
			    locnoise[1]=mod;
			    locnoise[2]=lay;
			    locnoise[3]=cell;

			    fCalnoise = (HMdcCal1Sim*)fCalCat->getSlot(locnoise);
			    if (fCalnoise)
			    {
				fCalnoise = new(fCalnoise) HMdcCal1Sim;
				fCalnoise->setAddress(sec,mod,lay,cell);
				if (modetdc==1)
				{
				    fCalnoise->setNHits(-1);
				    fCalnoise->setStatus1(2);                        
				    fCalnoise->setStatus2(-3);
				    setTime1Noise((gRandom->Rndm()*(getNoiseRangeHi(mod)-getNoiseRangeLo(mod))) + getNoiseRangeLo(mod));
                                    setTime2Noise(-999);
				    fCalnoise->setTime1(getTime1Noise());
				    arrayNoise[0]=2;
				    fCalnoise->setStatusList(arrayNoise); // store array in cal1sim level
				    if(fntuple)
				    {
					fillNTuple(locnoise[0],locnoise[1],locnoise[2],locnoise[3],
						   getTime1Noise(),getTime2Noise(),2);
				    }
				}
				else if(modetdc==2)
				{
				    fCalnoise->setNHits(2);                           
				    fCalnoise->setStatus1(2);
				    fCalnoise->setStatus2(2);                    
				    setTime1Noise((gRandom->Rndm()*(getNoiseRangeHi(mod)-getNoiseRangeLo(mod))) + getNoiseRangeLo(mod));
                                    setTime2Noise(getTime1Noise()+20);
				    fCalnoise->setTime1(getTime1Noise());
				    fCalnoise->setTime2(getTime2Noise());
				    arrayNoise[0]=2;
				    fCalnoise->setStatusList(arrayNoise); // store array in cal1sim level
				    if(fntuple)
				    {
					fillNTuple(locnoise[0],locnoise[1],locnoise[2],locnoise[3],
						   getTime1Noise(),getTime2Noise(),2);

				    }
				}
			    }
			}
		    }
		}
	    }
	}
    }

}

void HMdcDigitizer::fillTrackList( HMdcCal1Sim* fCal)
{
    // fills track list for tracks in both tdc modes

    Int_t array[5];
    Int_t array1[5];

    for(Int_t i=0;i<5;i++)
    {
	array[i]=getTrackN(i);
        array1[i]=getStatus(i);
    }
    fCal->setTrackList (array);  // store array in cal1sim level
    fCal->setStatusList(array1); // store array in cal1sim level
}
void HMdcDigitizer::findFirstValidHit()
{
    // Function to find the first valid hit (statusflag > 0) inside the
    // array for both tdc modes. Returns the number of the element of the
    // first valid hit and last hit inside the time  window to variables.

    Int_t hit=0;
    Int_t lasthit=0;
    while(getStatus(hit)<=0) // find first valid hit
    {
	lasthit++;
	hit++;
	if(hit==15)
	{
	    // if last element is reached without found
	    // condtion set flags and break
            resetListVariables();
	    break;
	}
    }
    if(hit<15)
    {
	while(getDTime1(lasthit)<=getDTime2(hit) && lasthit<15)
	{
	    lasthit++; // last hist which falls into window of first hit
	}
	// set output values if condition was true
	setFirstHit(hit);
	setFirstTime2(getDTime2(hit));

	if(lasthit<15)
	{
	    setEndList1(lasthit);
	}
	else
	{
	    setEndList1(-999);
	}
    }

}
void HMdcDigitizer::findSecondValidHit()
{
    // Function to find the second valid hit (statusflag > 0) inside the
    // array for tdc mode1. Returns the number of the element of the second
    // valid hit

    Int_t hit=getEndList1()+1;
    if(hit<15 && getEndList1()!=-999) // make sure that it is not last element and a valid first hit exist
    {
	while(getStatus(hit)<=0)
	{
	    // stop if status=1 and second hit starts after first hit ends
	    hit++;
	    if(hit==15)
	    {
		// if last element is reached without found
		// condtion set flags and break
		setSecondHit(-999);
		break;
	    }
	}
	if(hit<15)
	{
            // set output values if condition was true
	    setSecondHit(hit);
	}
    }
    else
    {
	// function was called with las element
        // and skipped to end => no valid secon hit found
	setSecondHit(-999);
    }
}
void HMdcDigitizer::select(Int_t nHits)
{
    // Puts the drift time values into increasing order.
    // Orders the corresponding track number, time of flight, statusflag
    // and impact angle accordingly


    register Int_t a,b,c;
    Int_t exchange;
    Float_t t;
    Float_t t2;
    Float_t tErr;
    Float_t t2Err;

    Float_t flight;
    Float_t angleLocal;
    Int_t statlocal;
    Int_t tracklocal;
    Float_t mindistlocal;
    Bool_t cutEdgelocal;

    for(a=0;a<nHits-1;++a)
    {
	exchange = 0;
	c = a;

	t = dTime[a];
	tErr = dTimeErr[a];
	t2 = dTime2[a];
	t2Err = dTime2Err[a];
	tracklocal = track[a];
	flight=timeOfFlight[a];
	angleLocal=angle[a];
	statlocal=statusflag[a];
	mindistlocal=minimumdist[a];
        cutEdgelocal=cutEdge[a];

	for(b=a+1;b<nHits;++b)
	{
	    if(dTime[b]<t)
	    {
		c = b;

		t = dTime[b];
		tErr = dTimeErr[b];
		t2 = dTime2[b];
		t2Err = dTime2Err[b];
		tracklocal = track[b];
		flight=timeOfFlight[b];
		angleLocal=angle[b];
		statlocal=statusflag[b];
		mindistlocal=minimumdist[b];
                cutEdgelocal=cutEdge[b];

		exchange = 1;
	    }
	}
	if(exchange)
	{
	    dTime[c] = dTime[a];
	    dTime[a] = t;
	    dTimeErr[c] = dTimeErr[a];
	    dTimeErr[a] = tErr;
	    dTime2[c] = dTime2[a];
	    dTime2[a] = t2;
	    dTime2Err[c] = dTime2Err[a];
	    dTime2Err[a] = t2Err;
	    track[c] = track[a];
	    track[a] = tracklocal;
	    timeOfFlight[c] = timeOfFlight[a];
	    timeOfFlight[a] = flight;
	    angle[c] = angle[a];
	    angle[a] = angleLocal;
	    statusflag[c]=statusflag[a];
	    statusflag[a]=statlocal;
	    minimumdist[c]=minimumdist[a];
	    minimumdist[a]=mindistlocal;
	    cutEdge[c]=cutEdge[a];
	    cutEdge[a]=cutEdgelocal;

	}
    }
}
void HMdcDigitizer::getMdcSetup()
{
    // Gets Mdc detector setup

    HDetector *mdcDet = gHades->getSetup()->getDetector("Mdc");
    if (!mdcDet)
    {
	Error("HMdcDigitizer::getMdcSetup()","Mdc-Detector setup (gHades->getSetup()->getDetector(\"Mdc\")) missing.");
    }
    for(Int_t s=0; s<6; s++) {
	for(Int_t m=0; m<4; m++) {
	    if (!mdcDet->getModule(s, m)) setup[s][m]=0;
	    if ( mdcDet->getModule(s, m)) setup[s][m]=1;
	}
    }
}

Bool_t HMdcDigitizer::testMdcSetup(Int_t s, Int_t m)
{
    // Tests the Mdc setup if the modules are present
    // in the running analysis

    Bool_t result=kFALSE;
    if(setup[s][m]==0) result=kFALSE;
    if(setup[s][m]==1) result=kTRUE;
    return result;
}
Bool_t HMdcDigitizer::transform(Float_t xcoor,Float_t ycoor,Float_t theta,
                                Float_t phi  ,Float_t tof  ,Int_t trkNum) {
    // Gets the x,y coordinates, theta and phi, time of flight and track number.
    // From the coordinates and angles the hits in the cells are calculated.
    // All needed parameters are taken from DigiPar container.

    HMdcLayerGeomParLay& layer = (*fDigitGeomPar)[loc[0]][loc[1]][loc[2]];
    Float_t pitch = layer.getPitch();                // pitch of the sense wires
    Float_t halfPitch   = 0.5*pitch;
    Float_t halfCatDist = 0.5*layer.getCatDist();    // distance of the cathode planes
    Int_t   nWires   = layer.getNumWires();          // number of wires per layer
    Float_t wOrient  = layer.getWireOrient()*pi;     // inclination of the wires
    Float_t centWire = layer.getCentralWireNr()-1.;  // wire number in physical center

    // Transformation from MDC's coordinate system to layer's coordinate system:
    Float_t y_wire = ycoor*cos(wOrient)-xcoor*sin(wOrient);

    Float_t ctanalpha = tan(theta*pi)*sin(phi*pi-wOrient);

    // recalculate the angle to the coordinatessystem of HMdcCal2ParSim
    myalpha=90-(atan((pi/2.)-ctanalpha)*180./acos(-1.));


    Float_t dY = halfCatDist*ctanalpha;
    dY<0.0 ? dY = -dY : dY;
    Float_t tmp = (centWire*pitch + y_wire+dY+halfPitch)/pitch;
    if(tmp < 0.0) return kFALSE;
    Int_t nCmax = (Int_t)tmp;
    tmp = (centWire*pitch + y_wire-dY+halfPitch)/pitch;
    Int_t nCmin;
    tmp < 0.0 ? nCmin=0 : nCmin=(Int_t)tmp;
    if(nCmin >= nWires) return kFALSE;
    nCmax>=nWires ? nCmax=nWires-1 : nCmax;

    for (loc[3]=nCmin; loc[3]<=nCmax; loc[3]++) {
	Float_t yDist = y_wire-(loc[3]*pitch-centWire*pitch);
	Float_t sinAlpha = sqrt(1./(1.+ctanalpha*ctanalpha));
	Float_t cosAlpha = sqrt(1.-sinAlpha*sinAlpha);

	Float_t per = fabs(yDist*sinAlpha);// minimum distance of track to the wire

	Bool_t flagCutEdge=kFALSE;
	if(per*sinAlpha > halfPitch) {  // check if per is inside cell (y)

	    flagCutEdge=kTRUE;

	} else if(per*cosAlpha > halfCatDist) { // check if per is inside cell (z)

	    flagCutEdge=kTRUE;
	}
	storeCell(per,tof,myalpha,trkNum,flagCutEdge);//store the final values in container
    }
    return kTRUE;
}

void HMdcDigitizer::storeCell(Float_t per,Float_t tof,Float_t myangle,Int_t trkNum
			      ,Bool_t flagCutEdge)
{
    // Puts the data (minimum distance, time of flight, impact angle,
    // track number, flagCutEdge) to HMdcGeantCell Category

    hit = (HMdcGeantCell*)fGeantCellCat->getObject(loc);
    if (!hit) {
	hit = (HMdcGeantCell*)fGeantCellCat->getSlot(loc);
	hit = new(hit) HMdcGeantCell;
    }
    Int_t nHit;
    nHit = hit->getNumHits();
    if (nHit < 15 ) {  // only the first 15 hits are stored
	hit->setSector(loc[0]);
	hit->setModule(loc[1]);
	hit->setLayer(loc[2]);
	hit->setCell(loc[3]);
	hit->setNumHits(nHit+1);
	hit->setMinDist(per,nHit);
	hit->setTimeFlight(tof,nHit);
	hit->setImpactAngle(myangle,nHit);
	hit->setNTrack(trkNum,nHit);
	hit->setFlagCutEdge(flagCutEdge,nHit);
    }
}

ClassImp(HMdcDigitizer)
