using namespace std;
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
#include "hmdcwirestat.h"
#include "hmessagemgr.h"
#include "hmdcsizescells.h"
#include "hmdcgeomstruct.h"
#include <iostream> 
#include <iomanip>
#include <TMath.h>
#include <TFile.h>
#include <stdlib.h>

//*-- Author : A.Nekhaev
//*-- Modified: 17/01/2001 by Ilse Koenig
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
//  setOffsets(1.5,2.5,4.5,5.5, 0 or 1)sets offsets for each type of module which will be
//                                     substracted from the calculated time1 and time2, the
//                                     last flag switches the use of offsets on/off (1=on(default)).
//  setEffLevel(90.,90.,90.,90., 0 or 1) sets level of cell efficiency cut for each type of module, the
//                                       last flag switches the use of efficiency on/off (1=on(default)).
//  setOffsets(1.5,2.5,4.5,5.5, 0 or 1)  offsets (ns), which will be substracted from drif time + tof
//  setTofUse(kFALSE) time of flight will not be added to drift times
//           (kTRUE ) time of flight will be added to drift time
//  setErrorUse(kTRUE)  apply smearing of drift times with errors
//             (kFALSE) do not apply smearing of drift times with errors
//  setWireStatUse(kTRUE)  take into account dead wires
//                (kFALSE) assume all wires working (default)
//  setNoiseLevel(5.,5.,5.,5.) sets level of noise for each type of module
//  setNoiseRange(low1,low2,low3,low4,hi1,hi2,hi3,hi4) sets the time window of the noise
//                                                     for each module type
//  setNoiseBandWidth(width)       (time-above threshold bump 0-width ns)
//  setNoiseWhiteWidth(upperrange) (upper range in time-above threshold for white noise outside the bump)
//  setNoiseWhiteRatio(ratio);     (ratio between bump/white noise
//  setNoiseMode(1)(default)GEANT cells will be overwritten by noise (if timenoise<time)
//              (2)         GEANT cells will not be touched
//  setEmbeddingMode(Int_t) (default)1=realistic merging og REAL and GEANT data (default)
//                                   2=keep Geant data
//  setSignalSpeed(0.004) sets the speed of the signal on the wire used to calulate the time offset
//                        by the signal propagation on the wire (ns/mm)
//------------------------------------------------------------------------------------------------------
// SHORT INTRODUCTION to HMdcDigitizer:
//
// SCHEME OF DIGITAZATION:
// 1. Reading input from HGeantMdc
// 2. Evaluation of fired cells
// 3. Calulation of drift times / wire offsets
// 4. Storing in HMdcGeantCell category (not persistent)
// 5. Filling arrays of all hits for one cell from HMdcGeantCell
// 6. set efficiency flags to the hits (cell efficiency/layer efficiency/dead wires)
// 7. Filling real data into arrays if digitizer runs in embedding mode
// 8. sorting arrays with increasing arrival time of the signal (drift + error + tof + wireoffset)
// 9. selecting 1. and 2. valid hit in cell
// 10.filling output to HMdcCal1Sim
// 11.generation of noise
//
// INPUT DATA TO THE DIGITIZER:
// The Digitizer retrieves the GEANT data from
// HGeantMdc (sector,module,layer, hit: xcoord,ycoord,tof,ptot, incidence:theta,phi,tracknumber).
// Evaluation of fired cells:
// HMdcDigitizer::transform(Float_t xcoord, Float_t ycoord, Float_t theta,
//                          Float_t phi   , Float_t tof   , Int_t trkNum) calculates
// which cells have been hit by the track. The Information about the layer geometry
// (pitch, cathod distance, wire orientation, number of wires per layer and number of central wire)
// is taken from HMdcLayerGeomPar. Output of the calculation is the minimum distance from the wire
// and the impact angle in the coordinate system of the cell. All values are stored in the
// HMdcGeantCell for a maximum number of 15 hits per cell via
// HMdcDigitizer::storeCell(Float_t per, Float_t tof, Float_t myangle, Int_t trkNum,
//			    Bool_t flagCutEdge,Floa_t wireOffset).
//
// CALCULATION OF DRIFT TIMES:
// For each cell the drift time1 and time2 are calculated by corresponding functions of
// the HMdcCal2ParSim container which holds the calibration parameters for the "distance->drift time"
// calculation:
// HMdcCal2ParSim::calcTimeDigitizer (sector,module,angle,minDist,&time1,&time1Error) and
// HMdcCal2ParSim::calcTime2Digitizer(sector,module,angle,minDist,&time2,&time2Error).

// CALCULATION OF WIRE OFFSET:
// calcWireOffset(xcoor,ycoor,wOrient) calulates the time the signal of a given cell would take
// to popagate from the hit point to the readout electronis. The speed of the signal is taken from
// HMdcDigitPar::getSignalSpeed()(ns/mm). Internal functions of HMdcSizesCells are called to calulate
// the path length of the signal.
//
// EFFICIENCY CUTS:
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
//
// SIMULATION OF DEAD WIRES:
// With setWireStatUse(kTRUE) the dead wires of the real data are correctly taken into account.
// This cut is handled in the same way as the effciency cuts and has the top priority
// (wire stat -> cell efficieny -> layer efficiency). In all cases the drift time will be set to -999.
// To get the correct result one has to analyze the status flags! The Information about the status
// of the wire is taken from HMdcWireStat.
//
// SELECTING 1. AND 2. VALID HIT:
// According to the the cuts a list of status flags for each recorded track is filled.
// After all calculations the list of Tracks is sorted by the arrival time (tof + drift time + wire offset) by
// HMdcDigitizer::select(Int_t nHits) because only the first track will create a signal.
// The first valid hit (status=1) inside the track list will be found by
// HMdcDigitizer::findFirstValidHit(Int_t* firsthit, Float_t* firsttime2, Int_t* endlist1)
// which returns the index number of the first valid hit and the the last hit which falls
// into the given time window defined by time2 of first valid hit.
// HMdcDigitizer::findSecondValidHit(Int_t endlist1,Int_t* secondhit)
// finds a second valid hit starting with the last entry of the list of the first valid hit.
// All variables will return -999 if no valid hits are found.
//
// FILLING OUTPUT:
// According to the two different TDC modes the HMdcCal1Sim category is filled.
// If HMdcDigitizer::setTofUse(kFALSE) is selected, the time of flight will be substracted and
// only the drift time is written to the output. With HMdcDigitizer::setOffsets(1.5,2.5,4.5,5.5, 0 or 1)
// a common minimum offset (fast particles) per module type can be set and will be subtracted
// from the calculated time to be closer to the real measurement situation.
// With setErrorUse(kFALSE) the digitizer can be forced to write the drift times without error smearing to the
// output.
//
// NOISE SIMULATION:
// Noise simulation should be used only in TDC mode 2 (leading + trailing edge)
// and not in embedding mode!
// The noise simulation is done after output data have been already filled to HMdcCal1 category.
// a loop over the category is performed and the noise is filled in 2 ways.
// 1. a loop over all existing cells in the setup of Mdc is done and randomly picked cells
//    are filled width noise and added to the category ("only noise" cells).
// 2. For the existing cells filled with GEANT data a comparison between the randomly created noise
//    time and the real time is done (if setNoiseMode(1)). The earlier of both is taken and if the noise wins
//    the status flags are changed accordingly.
// With HMdcDigitizer::setNoiseLevel(5.,5.,5.,5.) the simulation of noise can be switched on.
// HMdcDigitizer::fillNoise() loops over all cells existing in the actual setup and will randomly
// pick cells according to the specified probability per module (5.==5%) and set the statusflag to 2.
// If a cell is selected for noise production a second random process generates the time in the
// range of the specified window set by HMdcDigitizer::setNoiseRange(low1,low2,low3,low4,hi1,hi2,hi3,hi4).
// The behaviour of the noise generation can be specified with
// 1. setNoiseBandWidth(width)       (time-above threshold bump 0-width ns)
// 2. setNoiseWhiteWidth(upperrange) (upper range in time-above threshold for white noise outside the bump)
// 3. setNoiseWhiteRatio(ratio);     (ratio between bump/white noise
// With HMdcDigitizer::setNoiseMode(1)(default) a given time in one of the original GEANT cells
// will be overwritten by the the noise time, if timenoise < time.In this case the statusflag of
// the cell will be set to 2 (valid hit but noise).In the case of HMdcDigitizer::setNoiseMode(2) the
// original GEANT cells will not be touched.
//
// EVENT EMBEDDING:
// In the embedding case of GEANT data into REAL data, the digitizer looks to the HMdcCal1Sim and gets the
// data words filled with the REAL data by the HMdcCalibrater1 which would fall in the same Cells as the
// GEANT data. If the embedding mode is set to 1 (default) the digitizer will do a "realistic" merging,
// that means, the first hit from REAL or GEANT data will be accepted. In embedding mode 2 the GEANT data
// will be allways kept and the coresponding REAL data will be overwritten by GEANT data. The embedding
// mode can be switched by HMdcDigitizer::setEmbeddingMode(Int_t)(1=realistic,2=keep Geant data).
// The status flag of REAL data will be 0, where as in the listStatus[5] the status flag will be 3 for
// REAL data hits which are merged into GEANT cells.
//------------------------------------------------------------------------------------------------------
// MODE 1 (two times LEADING EDGE of the TDC signal)
//           nHits ==  -2 for 2 valid hits
//                 ==  -1 for 1 valid hit
//                 ==   0 for a not filled hit (e.g. 1 hit was kicked out by efficiency)
//         status1 ==   1 for a valid first hit
//                 ==   2 for a valid first hit caused by noise
//                 ==  -3 for a not valid hit
//                 ==   0 for REAL data (embedding) or no hit
//         status2 ==   1 for a valid second hit
//                 ==   2 for a valid second hit caused by noise
//                 ==  -3 for a not valid hit
//                 ==   0 for REAL data (embedding) or no hit
//         angle1  ==   impact of track1 or -99 if not filled
//         angle2  ==   impact of track2 or -99 if not filled
//         minDist1==   minimum distance of track1 or -99 if not filled
//         minDist2==   minimum distance of track2 or -99 if not filled
//         error1  ==   error of time1
//         error2  ==   error of time1 of the second valid hit or -99 if not filled
//         tof1    ==   tof of time1
//         tof2    ==   tof of time1 of the second valid hit or -99 if not filled
//         wireOff1==   signal time on wire of track1 or -99 if not filled
//         wireOff2==   signal time on wire of track2 or -99 if not filled
//         nTrack1 ==   track number of the first valid hit
//                 ==  -99 if not filled
//         nTrack2 ==   track number of the second valid hit
//                 ==  -99 if not filled
//         time1   ==   drift time1 of the first valid hit
//                 ==  -999 if not filled
//         time2   ==   drift time1 of the second valid hit
//                 ==  -999 if not filled
//     listTrack[5] :   contains the track number of the first 5 hits per cell
//                      == -99 if no hit was filled or noise
//     listStatus[5]:   contains the status flags of the first 5 hits per cell
//                      == -1 if hit was kicked out by cell efficiency cut
//                      == -2 if hit was kicked out by layer efficiency cut
//                      == -3 if hit was kicked out by wire stat
//                      == -4 if hit was kicked out by noise
//                      ==  1 if hit is valid
//                      ==  2 if hit is noise
//                      ==  3 if hit is real data (embedding)
//                      ==  0 if no hit was filled
//     both lists will be filled even if no vaild hit was found
//
// MODE 2 (LEADING AND TRAILING EDGE of the TDC signal)
//           nHits ==  +2 for 2 valid hits
//                 ==   0 for not filled hit (e.g. 1 hit was kicked out by efficiency)
//         status1 ==   1 for a valid first hit
//                 ==   2 for a valid first hit caused by noise
//                 ==  -3 for a not valid hit
//                 ==   0 for REAL data (embedding) or no hit
//         status2 ==   1 for a valid first hit
//                 ==   2 for a valid first hit caused by noise
//                 ==  -3 for a not valid hit
//                 ==   0 for REAL data (embedding) or no hit
//         angle1  ==   impact of track1 or -99 if not filled
//         angle2  ==   impact of track1 or -99 if not filled
//         minDist1==   minimum distance of track1 or -99 if not filled
//         minDist2==   minimum distance of track1 or -99 if not filled
//         error1  ==   error of time1 or -99 if not filled
//         error2  ==   error of time2 or -99 if not filled
//         tof1    ==   tof of time1 or -99 if not filled
//         tof2    ==   tof of time2 or -99 if not filled
//         wireOff1==   signal time on wire of track1 or -99 if not filled
//         wireOff2==   signal time on wire of track1 or -99 if not filled
//         nTrack1 ==   track number of first valid hit
//                 ==  -99 if not filled
//         nTrack2 ==   track number of first valid hit
//                 ==  -99 if not filled
//         time1   ==   drift time1 of the first valid hit
//                 ==  -999 if not filled
//         time2   ==   drift time2 of the first valid hit
//                 ==  -999 if not filled
//     listTrack[5] :   contains the track number of the first 5 hits per cell
//                      == -99 if no hit was filled or noise
//     listStatus[5]:   contains the status flags of the first 5 hits per cell
//                      == -1 if hit was kicked out by cell efficiency cut
//                      == -2 if hit was kicked out by layer efficiency cut
//                      == -3 if hit was kicked out by wire stat
//                      == -4 if hit was kicked out by noise
//                      ==  1 if hit is valid
//                      ==  2 if hit is noise
//                      ==  3 if hit is real data (embedding)
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

Float_t HMdcDigitizer::dTime [15]             ={0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
Float_t HMdcDigitizer::dTime2[15]             ={0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
Float_t HMdcDigitizer::dTimeErr [15]          ={0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
Float_t HMdcDigitizer::dTime2Err[15]          ={0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
Float_t HMdcDigitizer::minimumdist[15]        ={0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
Int_t   HMdcDigitizer::track[15]              ={-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99};
Float_t HMdcDigitizer::timeOfFlight[15]       ={0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
Float_t HMdcDigitizer::angle[15]              ={0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
Int_t   HMdcDigitizer::statusflag[15]         ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
Float_t HMdcDigitizer::fractionOfmaxCharge[15]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
Bool_t  HMdcDigitizer::cutEdge[15]            ={kFALSE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE};
Float_t HMdcDigitizer::wireOffset[15]         ={0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};


HMdcDigitizer::HMdcDigitizer(void)
{
    initVariables();
}
HMdcDigitizer::HMdcDigitizer(Text_t *name,Text_t *title):
HReconstructor(name,title)
{
    initVariables();
}

HMdcDigitizer::HMdcDigitizer(Text_t *name,Text_t *title,Int_t TDCMODE,Bool_t NTUPLE) :
HReconstructor(name,title)
{
    // TDCMODE sets the simulation mode to the two different
    // possibilities of the TDC: 1 == to leading edges
    //                           2 == leading and trailing edge
    // NTUPLE switches the NTuple with the internal information of
    // the digitizer ON or OFF:   kFALSE == no NTuple filled
    //                            kTRUE  == NTuple filled.

    initVariables();
    setTdcMode(TDCMODE);
    setNTuple(NTUPLE);
}

HMdcDigitizer::~HMdcDigitizer(void) {
  

  if(iterin) delete iterin;
  if(itercell) delete itercell;
  if(itercal1) delete itercal1;
}
void HMdcDigitizer::initVariables()
{
    // sets all used variables to the initial values

    fGeantCellCat = 0;
    fCalCat = 0;
    fDigitGeomPar = 0;
    fDigitPar = 0;
    fCal2ParSim = 0;
    fCellEff = 0;
    fWireStat = 0;
    fsizescells = 0;
    fCal=0;
    fCalnoise=0;
    fCell=0;
    iterin = 0;
    itercell = 0;
    itercal1 = 0;
    fEventId = 0;
    pi = acos(-1.)/180;
    time1=0;
    time1Error=0;
    time2=0;
    time2Error=0;
    setTdcMode(2);
    setEffLevel(90,90,90,90);
    setNoiseLevel(5.,5.,5.,5.);
    setOffsets(1.5,2.5,4.5,5.5);
    setOffsetsUse(kFALSE);
    setCellEffUse(kTRUE);
    setWireStatUse(kFALSE);
    setNoiseUse(kFALSE);
    setTofUse(kTRUE);
    setErrorUse(kTRUE);
    setWireOffsetUse(kTRUE);
    setNTuple(kFALSE);
    hasPrinted=kFALSE;
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
    setNoiseBandWidth(20.);
    setNoiseWhiteWidth(500.);
    setNoiseWhiteRatio(0.1);
    setEmbeddingMode(1);
    setSignalSpeed(0.004);// ns/mm
}
void HMdcDigitizer::setParContainers() {
    // Get pointers to the needed containers.The containers are
    // created and added to the runtime Database if the are not existing

    fDigitGeomPar =(HMdcLayerGeomPar*)(((HRuntimeDb*)(gHades->getRuntimeDb()))->getContainer("MdcLayerGeomPar"));
    if(!fDigitGeomPar)
    {
	Error("HMdcDigitizer:init()","ZERO POINTER FOR HMDCLAYERGEOMPAR RECIEVED!");
	exit(1);
    }
    fDigitPar =(HMdcDigitPar*)(((HRuntimeDb*)(gHades->getRuntimeDb()))->getContainer("MdcDigitPar"));
    if(!fDigitPar)
    {
	Error("HMdcDigitizer:init()","ZERO POINTER FOR HMDCDIGITPAR RECIEVED!");
	exit(1);
    }
    fCal2ParSim  =(HMdcCal2ParSim*)(((HRuntimeDb*)(gHades->getRuntimeDb()))->getContainer("MdcCal2ParSim"));
    if(!fCal2ParSim)
    {
	Error("HMdcDigitizer:init()","ZERO POINTER FOR HMDCCAL2PARSIM RECIEVED!");
	exit(1);
    }
    geomstruct =(HMdcGeomStruct*)(((HRuntimeDb*)(gHades->getRuntimeDb()))->getContainer("MdcGeomStruct"));
    if(!geomstruct)
    {
	Error("HMdcDigitizer:init()","ZERO POINTER FOR HMDCGEOMSTRUCT RECIEVED!");
	exit(1);
    }
    if(getCellEffUse())
    {
	fCellEff =(HMdcCellEff*)(((HRuntimeDb*)(gHades->getRuntimeDb()))->getContainer("MdcCellEff"));
	if(!fCellEff)
	{
	    Error("HMdcDigitizer:init()","ZERO POINTER FOR HMDCCELLEFF RECIEVED!");
	    exit(1);
	}
    }
    if(getWireStatUse())
    {
	fWireStat =(HMdcWireStat*)(((HRuntimeDb*)(gHades->getRuntimeDb()))->getContainer("MdcWireStat"));
	if(!fWireStat)
	{
	    Error("HMdcDigitizer:init()","ZERO POINTER FOR HMDCWIRESTAT RECIEVED!");
	    exit(1);
	}
    }
    if(getWireOffsetUse())
    {
	fsizescells=(HMdcSizesCells*)HMdcSizesCells::getObject();
	if(!fsizescells)
	{
	    Error("HMdcDigitizer:init()","ZERO POINTER FOR HMDCSIZESCELLS RECIEVED!");
	    exit(1);
	}
    }
}

void HMdcDigitizer::setNTuples(void) {
    // Creates an NTuple for the internal infomation of the digitizer
    // which is stored in the file digitizer.root in the working directory.
    // The NTuple contains
    // sec,mod,lay,cell: the "software address" of a single cell
    // dist,angle: minimum distance of the track to the wire
    //             and impact angle in coordinates of the cell
    // time1, time1Err,time2,time2Err: values for the drift times
    // tof: time of flight
    // cutEdge: 0 if minimum distance smaler than cell bounderies, 1 if larger
    // status: 1 if valid hit, 2 if noise, 3 for REAL data
    //        -1 if cut by cell efficiency
    //        -2 if cut by layer efficiency
    // track: track number (-99 if no real track)
    // eff:   efficiency value which would correspond to a cut on this minimum dist

    myoutput->cd();
    distance_time = new TNtuple("cal2sim", "cal2sim", "sec:mod:lay:cell:dist:angle:time1:time1Err:time2:time2Err:tof:cutEdge:status:track:eff");

}

Bool_t HMdcDigitizer::init(void) {
    // The parameter containers and the iterators over
    // the categorys MdcGeanRaw, MdcGeantCell and MdcCal1Sim are created.
    // The actual setup of the Mdc detector in the running analysis
    // is retrieved.
    setParContainers();
    getMdcSetup();

    fGeantMdcCat=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catMdcGeantRaw));
    if(!fGeantMdcCat) {
	Error("HMdcDigitizer::init()","HGeant MDC input missing");
	return kFALSE;
    }
    iterin = (HIterator*)((HCategory*)fGeantMdcCat)->MakeIterator("native");

    fGeantCellCat =(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catMdcGeantCell));
    if (!fGeantCellCat) {
	fGeantCellCat =(HCategory*)((HMdcDetector*)(((HSpectrometer*)(gHades->getSetup()))->getDetector("Mdc"))->buildCategory(catMdcGeantCell));
	if (!fGeantCellCat) return kFALSE;
	else ((HEvent*)(gHades->getCurrentEvent()))->addCategory(catMdcGeantCell,fGeantCellCat,"Mdc");
    }
    fGeantCellCat->setPersistency(kFALSE);   // We don't want to write this one
    itercell = (HIterator*)((HCategory*)fGeantCellCat)->MakeIterator("native");

    fCalCat=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catMdcCal1));
    if (!fCalCat) {
	HMdcDetector* mdc=(HMdcDetector*)(((HSpectrometer*)(gHades->getSetup()))->getDetector("Mdc"));
	fCalCat=(HCategory*)(((HMdcDetector*)mdc)->buildMatrixCategory("HMdcCal1Sim",0.5F));
	if (!fCalCat) return kFALSE;
	else ((HEvent*)(gHades->getCurrentEvent()))->addCategory(catMdcCal1,fCalCat,"Mdc");
        itercal1= (HIterator*)fCalCat->MakeIterator("native");

    } else {
	if (fCalCat->getClass()!=HMdcCal1Sim::Class()) {
	    Error("HMdcDigitizer::init()","Misconfigured output category");
	    return kFALSE;
	}
    }
    if(getNTuple())
    {
	// create output file and NTuple
	myoutput=new TFile("digitizer.root","RECREATE");
	myoutput->cd();
	setNTuples();
    }
    if(!hasPrinted)printStatus();
    return kTRUE;
}
Bool_t HMdcDigitizer::reinit(void)
{
    if(getWireOffsetUse())
    {
	setSignalSpeed(fDigitPar->getSignalSpeed());
	return fsizescells->initContainer();
    }
    else return kTRUE;
}
void HMdcDigitizer::printStatus()
{
    // Prints the Options, default settings and
    // actual configuration of HMdcDigitizer.

  SEPERATOR_msg("*",60);
  INFO_msg(10,DET_MDC,"DEFAULT SETTINGS");
  SEPERATOR_msg("-",60);
  INFO_msg(10,DET_MDC,"Options input 1 (default)      two leading edges");
  INFO_msg(10,DET_MDC,"              2                leading and trailing edge");
  INFO_msg(10,DET_MDC,"NTuple        kFALSE (default) no NTuple filled");
  INFO_msg(10,DET_MDC,"              kTRUE            NTuple in digitizer.root filled");
  INFO_msg(10,DET_MDC,"Use Offsets   kFALSE (default)");
  INFO_msg(10,DET_MDC,"Use Tof       kTRUE  (default) cal1sim = drift time + tof");
  INFO_msg(10,DET_MDC,"Use Cell Eff  kFALSE (default)");
  INFO_msg(10,DET_MDC,"Use Noise     kFALSE (default)");
  INFO_msg(10,DET_MDC,"Noise mode    1 (default) GEANT hits will be replaced by noise");
  SEPERATOR_msg("-",60);
  INFO_msg(10,DET_MDC,"ACTUAL CONFIGURATION");

  SEPERATOR_msg("*",60);
  gHades->getMsg()->info(10,DET_MDC,GetName(),"HMdcDigiSetup:");
  gHades->getMsg()->info(10,DET_MDC,GetName()
			 ,"tdcModeDigi       =  %i :  1 = two leading edges, 2 = leading and trailing edge",getTdcMode());
  gHades->getMsg()->info(10,DET_MDC,GetName()
			 ,"NtupleDigi        =  %i :  0 = noNtuple, 1 = digitizer.root",(Int_t)getNTuple());
  gHades->getMsg()->info(10,DET_MDC,GetName()
			 ,"useTofDigi        =  %i :  0 = NoTof in cal1, 1 = Tof in cal1 \n",(Int_t)getTofUse());
  gHades->getMsg()->info(10,DET_MDC,GetName()
			 ,"useErrorDigi      =  %i :  0 = NoErr in cal1, 1 = Err in cal1 \n",(Int_t)getErrorUse());
  gHades->getMsg()->info(10,DET_MDC,GetName()
			 ,"useWireOffsetDigi =  %i :  1 = add wireOffset to drift time, 0 = don't add wireOffsets"
			 , getWireOffsetUse());
  gHades->getMsg()->info(10,DET_MDC,GetName()
			 ,"useWireStatDigi   =  %i :  1 = use wirestat container, 0 = don't use wirestat container"
			 , getWireStatUse());
  gHades->getMsg()->info(10,DET_MDC,GetName()
			 ,"offsetsOnDigi     =  %i :  0 = global offsets off, 1 = global offsets on",(Int_t)getOffsetsUse());
  gHades->getMsg()->info(10,DET_MDC,GetName()
			 ,"offsetsDigi       = %4.1f  %4.1f   %4.1f  %4.1f ns offset per plane (substracted from (drift time + tof))\n"
			 ,getOffset(0),getOffset(1),getOffset(2),getOffset(3));
  gHades->getMsg()->info(10,DET_MDC,GetName()
			 ,"noiseModeDigi     =  %i :  1 = override geant by noise, 2 = keep geant cells",getNoiseMode());
  gHades->getMsg()->info(10,DET_MDC,GetName()
			 ,"noiseOnDigi       =  %i :  0 = noise off, 1 = noise on",(Int_t)getNoiseUse());
  gHades->getMsg()->info(10,DET_MDC,GetName()
			 ,"noiseLevelDigi    = %4.1f%% %4.1f%%  %4.1f%% %4.1f%% noise level per plane"
			 ,100*getNoiseLevel(0),100*getNoiseLevel(1),100*getNoiseLevel(2),100*getNoiseLevel(3));
  gHades->getMsg()->info(10,DET_MDC,GetName()
			 ,"noiseRangeDigi    =%5i %5i %5i %5i %5i %5i %5i %5i ns lower/upper limit of noise"
			 ,getNoiseRangeLo(0),getNoiseRangeLo(1),getNoiseRangeLo(2),getNoiseRangeLo(3)
			 ,getNoiseRangeHi(0),getNoiseRangeHi(1),getNoiseRangeHi(2),getNoiseRangeHi(3));
  gHades->getMsg()->info(10,DET_MDC,GetName()
			 ,"noiseBandWidth    =  %5.1f ns : width of the t2-t1 noise band",getNoiseBandWidth());
  gHades->getMsg()->info(10,DET_MDC,GetName()
			 ,"noiseWhiteWidth   =  %5.1f ns : width of the t2-t1 white noise region",getNoiseWhiteWidth());
  gHades->getMsg()->info(10,DET_MDC,GetName()
			 ,"noiseWhiteRatio   =  %5.1f    : ratio between t2-t1 band/white noise\n",getNoiseWhiteRatio());
  gHades->getMsg()->info(10,DET_MDC,GetName()
			 ,"cellEffOnDigi     =  %i :  0 = cellEff off, 1 = cellEff",(Int_t)getCellEffUse());
  gHades->getMsg()->info(10,DET_MDC,GetName()
			 ,"cellEffDigi       =  %4.1f%% %4.1f%%  %4.1f%% %4.1f%% level of cellEff per plane"
			 ,getCellEffLevel(0),getCellEffLevel(1),getCellEffLevel(2),getCellEffLevel(3));
  hasPrinted=kTRUE;
}

Bool_t HMdcDigitizer::finalize(void)
{
    // If NTuple exist it will be written to digitizer.root.

    if(getNTuple())
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

  while ((fCell=(HMdcGeantCell *)itercell->Next()) != NULL)
  {
     initArrays();
     loc[0] = fCell->getSector();
     loc[1] = fCell->getModule();
     loc[2] = fCell->getLayer();
     loc[3] = fCell->getCell();


    //######################### CHECK IF OBJECT EXISTS (EMBEDDING) #####################
    fCal=0;
    fCal=(HMdcCal1Sim*)fCalCat->getObject(loc);

    resetCal1Real(); // reset digitizers variables, not cal1!

    if (fCal)
    {  // if object exists before

	getCal1Real();  // copy real cal1 data to digitizers variables

	// consistency check

	if( (getNHitsReal()==2&&getTdcMode()==1) || (getNHitsReal()<0&&getTdcMode()==2) )
	{
	    Warning("HMdcDigitizer:execute()","HMdcCalibater1 and HMdcDigitizer running in different tdc modes!");
	}
    }

    if(!fCal)
    {   // if object did not exist before allocate a new object
	fCal = (HMdcCal1Sim*)fCalCat->getSlot(loc);
	if (fCal)
	{
	    fCal = new(fCal) HMdcCal1Sim;
	    fCal->setAddress(loc[0],loc[1],loc[2],loc[3]);
	}
	else
	{
	    Warning("HMdcDigitizer:execute()","CAL1SIM:getSlot(loc) failed!");
	}
    }
    //##################################################################################

    if (fCal) {

	// Digitisation procedure starts here:


	// First TDC signal
	Int_t nHits = fCell->getNumHits();
	for(Int_t ihit = 0;ihit<nHits;ihit++)
	{
	    fillArrays(ihit,loc[0],loc[1],fCell);         // fill arrays with all variables needed
	    setEfficiencyFlags(ihit,loc[0],loc[1],loc[2]);// checks for efficiency cuts and sets the propper statusflags
	}


	//########################### FILLING REAL DATA ##################################
	if(getEmbeddingMode()==1&&getTime1Real()!=-999)
	{   // if embedding mode and a cell was filled before
	    fillArraysReal(nHits);                 // fill the real data into the working arrays
	    if(getTdcMode()==2)     nHits=nHits+1; // count up the number of hits (1 Hit)
            else if(getTdcMode()==1)nHits=nHits+2; // count up the number of hits (2 Hits)
	}
        //################################################################################

	if (nHits>1) select(nHits);  // sort all hits by arrival time (tof + drifttime + wireOffset)

	resetListVariables();

	if(getNTuple())
	{
	    // fill Ntuple with internal information of the Digitizer
	    for(Int_t hit=0;hit<15;hit++)
	    {
		if(getStatus(hit)==0)continue;
		fillNTuple(loc[0],loc[1],loc[2],loc[3],hit,
			   fCell,distance_time);
	    }
	}

	//#################################### FILLING OUTPUT #######################################

	findFirstValidHit();

        //----------------------------------------- MODE1 AND MODE2 ---------------------------------------------------
	if(getTdcMode()==1||getTdcMode()==2)
	{ // both TDC modes
	    if(getFirstHit()!=-999) 
	    {
		// if a valid hit was found

                //--------------------- switch table ----------------------------------------------------------
		// DTime1/2 contains drift time + error + tof + wireoffset:
		//
		// useTof        :  0  +((Int_t)useTof-1  )      -> -1 tof will be substracted
                //                  1  +((Int_t)useTof-1  )      ->  0 tof will not be substracted
                // useError      :  0  +((Int_t)useError-1)      -> -1 error will be substracted
                //                  1  +((Int_t)useError-1)      ->  0 error will not be substracted
                // useOffsets    :  0  -((Int_t)useOffsets)      ->  0 no offsets will be substracted
                //                  1  -((Int_t)useOffsets)      -> -1 offsets will be substracted
                // useWireOffset :  0  +((Int_t)useWireOffset-1) -> -1 wire offsets will be substracted
                //                  1  +((Int_t)useWireOffset-1) ->  0 wire offsets will be included
                //---------------------------------------------------------------------------------------------

		fCal->setTime1(
			       getDTime1(getFirstHit())
			       + ( ((Int_t)getTofUse()-1  )     *getTof(getFirstHit()) )
			       + ( ((Int_t)getErrorUse()-1)     *getDTime1Err(getFirstHit()) )
			       - ( ((Int_t)getOffsetsUse())     *getOffset(loc[1]) )
			       + ( ((Int_t)getWireOffsetUse()-1)*getWireOffset(getFirstHit()) ) );

		fCal->setNTrack1(getTrackN(getFirstHit()));
		if(getStatus(getFirstHit())==3)fCal->setStatus1(0); // is real data
		else                           fCal->setStatus1(getStatus(getFirstHit()));
		fCal->setAngle1(getAngle(getFirstHit()));
		fCal->setMinDist1(getMinimumDist(getFirstHit()));
		fCal->setError1(getDTime1Err(getFirstHit()));
		fCal->setTof1(getTof(getFirstHit()));
                fCal->setWireOffset1(getWireOffset(getFirstHit()));
	    }
	    else
	    {
		fCal->setStatus1(-3); // no valid hit1 found
	    }
	}

	//----------------------------------------- MODE2 --------------------------------------------------------------------
	if(getTdcMode()==2)
	{ // leading and trailing edge
	    if(getFirstHit()!=-999) 
	    { // if a valid hit was found

		fCal->setTime2(
			       getDTime2(getFirstHit())
			       + ( ((Int_t)getTofUse()-1  )     *getTof(getFirstHit()) )
                               + ( ((Int_t)getErrorUse()-1)     *getDTime2Err(getFirstHit()) )
			       - ( ((Int_t)getOffsetsUse())     *getOffset(loc[1]) )
			       + ( ((Int_t)getWireOffsetUse()-1)*getWireOffset(getFirstHit()) ) );

		fCal->setNTrack2(getTrackN(getFirstHit()));         // fill same track number as for time1
		fCal->setNHits(2);                                  // second hit = trailing edge
                if(getStatus(getFirstHit())==3)fCal->setStatus2(0); // is real data
		else                           fCal->setStatus2(getStatus(getFirstHit())); // status is ok
		fCal->setAngle2(getAngle(getFirstHit()));
                fCal->setMinDist2(getMinimumDist(getFirstHit()));
		fCal->setError2(getDTime2Err(getFirstHit()));
                fCal->setTof2(getTof(getFirstHit()));
                fCal->setWireOffset2(getWireOffset(getFirstHit()));
	    }
	    else
	    {
		fCal->setStatus2(-3); // no vaild hit2 found
	    }
	}

	//----------------------------------------- MODE1 -------------------------------------------------------------------------
     
	if (nHits==1 && getTdcMode()==1) fCal->setNHits(-1);  // if only one hit was detected
	else
	{
	    if(nHits>1 && getTdcMode()==1 && getFirstHit()!=-999)
	    { // two times leading edge
		findSecondValidHit();

		if (getSecondHit()==-999 )
		{
		    fCal->setNHits(-1); // if no valid hit2 was found
		}
		else
		{
		    fCal->setTime2(
				   getDTime1(getSecondHit())
				   + ( ((Int_t)getTofUse()-1  )     *getTof(getSecondHit()) )
                                   + ( ((Int_t)getErrorUse()-1)     *getDTime1Err(getSecondHit()) )
				   - ( ((Int_t)getOffsetsUse())     *getOffset(loc[1]) )
				   + ( ((Int_t)getWireOffsetUse()-1)*getWireOffset(getSecondHit()) ) );
   
		    fCal->setNTrack2(getTrackN(getSecondHit()));          // number of second track is stored
		    fCal->setNHits(-2);                                   // second valid hit was found
                    if(getStatus(getSecondHit())==3)fCal->setStatus2(0);  // is real data
		    else                            fCal->setStatus2(getStatus(getSecondHit())); // status of hit2 is ok
		    fCal->setAngle2(getAngle(getSecondHit()));
		    fCal->setMinDist2(getMinimumDist(getSecondHit()));
		    fCal->setError2(getDTime1Err(getSecondHit()));
		    fCal->setTof2(getTof(getSecondHit()));
                    fCal->setWireOffset2(getWireOffset(getSecondHit()));
		}
	    }
	    else if(nHits>1 && getTdcMode()==1)
	    {
		fCal->setStatus2(-3); // no valid second hit was found
	    }
	}

	//---------------------------------- FILL LIST OF TRACKS / STATUSFLAGS -----------------------------------------------

	fillTrackList(fCal); // fill list of tracks and statusflags
	                     // even if no valid hit was in
    }
    else
    {
	Warning("HMdcDigitizer:execute()","ZERO POINTER FOR CAL1SIM OBSERVED!");
    }
  }

  //###################################### NOISE #########################################
  if(getNoiseUse())
  {
      // loop over all cells in actual existing Setup
      // and pick randomly cells.If a cell is selected
      // a random time is filled and the cell is stored
      // in the output. For cells filled by Geant hits
      // the noise time will be compared to the measured
      // time and if the noise comes earlier the real time
      // will be replaced (if noise mode 1 is selected)

      setLoopVariables(0,0,0,0);

      itercal1->Reset();
      Int_t sec,mod,lay,cell;

      while ((fCal=(HMdcCal1Sim *)itercal1->Next()) != NULL)
      {
	  fCal->getAddress(sec,mod,lay,cell);                               // get existing object
	  if(getNoiseMode()==1 && ( gRandom->Rndm()<getNoiseLevel(mod)) )
	  {
	      // check if the time1 of the noise hit is smaller than time1 of the real hit
	      // if this case is fullfilled the real hit will be overwritten by the noise hit.
	      // if no valid hit was found the noise hit will be written.

	      fillNoiseToGeantCells(mod,fCal);                              // fill this cell with noise if noise comes earlier
	  }
	  fillNoise(firstsec,firstmod,firstlay,firstcell,sec,mod,lay,cell); // fill randomly all cells before the object
	  setLoopVariables(sec,mod,lay,cell+1,kTRUE);                       // set the loop start no the actual value
      }
      fillNoise(firstsec,firstmod,firstlay,firstcell,5,3,5,999);            // fill rest of noise cells after last real object
  }
  //######################################################################################

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
    setWireOffset(ihit,fCell->getWireOffset(ihit));
    setCutEdge(ihit,fCell->getFlagCutEdge(ihit));

    fCal2ParSim->calcTimeDigitizer(sec,mod,getAngle(ihit),(getMinimumDist(ihit)),&time1,&time1Error);
    setDTime1(ihit,(Float_t)time1 + time1Error + getTof(ihit) + getWireOffset(ihit));
    setDTime1Err(ihit,time1Error);

    fCal2ParSim->calcTime2Digitizer(sec,mod,getAngle(ihit),(getMinimumDist(ihit)),&time2,&time2Error);
    Float_t mytime2=time2 + time2Error + getTof(ihit);

    if(mytime2<getDTime1(ihit))
    {   // make shure that time2 > time1
	setDTime2(ihit,time1 + time1Error + 20 + (10*gRandom->Gaus(0,1)) + getTof(ihit) + getWireOffset(ihit));
    }
    else
    {
	setDTime2(ihit,time2 + time2Error + getTof(ihit) + getWireOffset(ihit));
    }
    setDTime2Err(ihit,time2Error);
}
void HMdcDigitizer::fillArraysReal(Int_t i)
{
    // All needed Arrays for the calculations are
    // filled for the REAL hits (minimumdist,angle,tracknumber,tof,time1,time2,time1err,time2err)

    if(getTdcMode()==2 && i<14)
    {   // leading and trailing edge
	setMinimumDist(i,-99);
	setAngle(i,-99);
	setTrackN(i,-99);
	setTof(i,-99);

	setDTime1(i,getTime1Real());
	setDTime1Err(i,-99);

	if(getTime2Real()>-998)
	{   // if trailing edge has been measured
	    setDTime2(i,getTime2Real());
	}
	else
	{   // create dummy trailing edge
            setDTime2(i,getTime1Real()+80);
	}
	setDTime2Err(i,-99);

	setCutEdge(i,kFALSE);
        setWireOffset(i,0.);
	setStatus(i,3);
	setFractionOfmaxCharge(i,0);
    }
    else if(getTdcMode()==1 && i<13)
    {   // two leading edges
        // first hit
	setMinimumDist(i,-99);
	setAngle(i,-99);
	setTrackN(i,-99);
	setTof(i,-99);

	setDTime1(i,getTime1Real());
	setDTime1Err(i,-99);

	setDTime2(i,-999);
	setDTime2Err(i,-99);

	setCutEdge(i,kFALSE);
	setWireOffset(i,0.);
	setStatus(i,3);
	setFractionOfmaxCharge(i,0);

        // second hit
	setMinimumDist(i+1,-99);
	setAngle(i+1,-99);
	setTrackN(i+1,-99);
	setTof(i+1,-99);

	if(getTime2Real()>-998)
	{   // if there has been a real second hit
	    setDTime1(i+1,getTime2Real());
	}
	else
	{   // if there has been no second hit
            setDTime1(i+1,1000);
	}
	setDTime1Err(i+1,-99);

	setDTime2(i+1,-999);
	setDTime2Err(i+1,-99);

	setCutEdge(i+1,kFALSE);
	setWireOffset(i+1,0.);
	setStatus(i+1,3);
	setFractionOfmaxCharge(i+1,0);
    }
    else
    {
	Warning("HMdcDigitizer:fillArraysReal()","real hit could not be stored to array,\n because the maximum has been exceeded!");
    }
}
void HMdcDigitizer::setEfficiencyFlags(Int_t ihit,Int_t sec,Int_t mod,Int_t lay)
{
    // Cuts for cell efficiency layer efficiency and wire status are checked
    // and the statusflag of the cells are set correspondingly
    if(evalWireStat(sec,mod,lay,loc[3]))
    { // if the wire is connected and working or wireStatUse=kFALSE
        if(getCellEffUse())
        {
            setStatus(ihit,fCellEff->calcEfficiency(mod,getMinimumDist(ihit),getAngle(ihit),100.-getCellEffLevel(mod)));
            setFractionOfmaxCharge(ihit,fCellEff->calcEffval(mod,getMinimumDist(ihit),getAngle(ihit),100.-getCellEffLevel(mod)));
        }
        else
        {
            setStatus(ihit,1);
            setFractionOfmaxCharge(ihit,100);
        }

        // Efficiency of MDC layers can be less then 100%...
        if(gRandom->Rndm() >fDigitPar->getLayerEfficiency(sec,mod,lay))
        {
            switch (getStatus(ihit))
            {
            case   1: setStatus(ihit,-2); // if it is kicked out by layer efficiency
            break;
            case  -1: setStatus(ihit,-1); // if it was kicked out by cell efficiency
            break;
            default : setStatus(ihit,-7); // just control
            break;
            }
        }
    }
    else
    { // if wire is not connected or dead
        setStatus(ihit,-3);
        setFractionOfmaxCharge(ihit,0);
    }
}
Bool_t HMdcDigitizer::evalWireStat(Int_t sec,Int_t mod,Int_t lay,Int_t cell)
{
    // gets the status of the wire from HMdcWireStat.
    // returns kFALSE if wire is dead or not connected.

    Bool_t result=kTRUE;
    Int_t stat=-99;
    if(getWireStatUse())
    {
        stat=fWireStat->getStatus(sec,mod,lay,cell);
        (stat>0)? result=kTRUE:result=kFALSE;
    }
    return result;
}
void HMdcDigitizer::fillNTuple(Int_t sec,Int_t mod,Int_t lay,Int_t cell,Int_t ihit,
			       HMdcGeantCell* fCell, TNtuple* distance_time)
{
    // The NTuple is filled with internal data of the digitizer

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
			getTrackN(ihit),
			getFractionOfmaxCharge(ihit)
		       );
}
void HMdcDigitizer::fillNTuple(Int_t sec,Int_t mod,Int_t lay,Int_t cell,
			       Float_t time, Float_t time2,
			       Int_t status)
{
    // The NTuple is filled with internal data of the digitizer
    // in the case of noise generation
    distance_time->Fill(sec,mod,lay,cell,
			-1,
			-1,
			time,0,
			time2,0,
			0,
			0,
			status,
			-99,
		        100);
}

Float_t HMdcDigitizer::fillTime1Noise(Int_t mod)
{
    // A random time in a specified time window
    // for the noise simulation is calculated
    Float_t time1Noise=-999;
    time1Noise= (gRandom->Rndm()*(getNoiseRangeHi(mod)-getNoiseRangeLo(mod))) + getNoiseRangeLo(mod);
    return  time1Noise;
}
Float_t HMdcDigitizer::fillTime2Noise(Int_t mod)
{
    // A random time in a specified time window
    // for the noise simulation is calculated
    Float_t time2Noise=-999;
    if(gRandom->Rndm()<getNoiseWhiteRatio())
    {
	time2Noise= getTime1Noise() + gRandom->Rndm()*getNoiseWhiteWidth();
    }
    else {
	time2Noise= getTime1Noise() + gRandom->Rndm()*getNoiseBandWidth();
    }
    return  time2Noise;
}
void HMdcDigitizer::fillNoiseLists(HMdcCal1Sim* cal1,Int_t statval,Int_t geant)
{

    // sets the lists of status and tracks
    // statval is put to the first entry of the status list
    // if geant==1 (cell was Geant hit before), the original
    // track numbers and the status flags (except the first) are kept

    if(geant==0)
    {   // noise only
	Int_t dummystat [5]={0,0,0,0,0};
	Int_t dummytrack[5]={-99,-99,-99,-99,-99};
	dummystat[0]=statval;

	cal1->setStatusList(dummystat);       // store array in cal1sim level
	cal1->setTrackList(dummytrack);       // store array in cal1sim level
    }
    else
    {   // noise into Geant cell
	Int_t* mylist=cal1->getStatusList();
	mylist[0]=statval;
    }

}
void HMdcDigitizer::fillNoiseToGeantCells(Int_t mod,HMdcCal1Sim* cal1)
{
    // If timenoise < time of the first valid GEANT hit
    // the time is replaced by timenoise and the
    // statusflag is changed from 1 (valid) to 2 (valid but noise)


    setTime1Noise(fillTime1Noise(mod));
    setTime2Noise(fillTime2Noise(mod));

    if(getTime1Noise() < cal1->getTime1())
    {
	    cal1->setStatus1(2);              // noise hit
	    cal1->setTime1(getTime1Noise());  // old time1 is replaced by noise
	    cal1->setTime2(getTime2Noise());  // old time2 is replaced by noise
            cal1->setError1(-99);             // noise has no error
	    cal1->setError2(-99);             // noise has no error
	    cal1->setTof1(-99);               // tof->0 to put the correct noise time to the output
            cal1->setTof2(-99);               // tof->0 to put the correct noise time to the output
	    cal1->setWireOffset1(-99);        // noise has no wire offset
	    cal1->setWireOffset2(-99);        // noise has no wire offset
            cal1->setStatus1(2);              // noise has no wire offset
            cal1->setStatus2(2);              // noise has no wire offset
            cal1->setAngle1(-99);             // no impact angle
            cal1->setAngle2(-99);             // no impact angle
            cal1->setMinDist1(-99);           // no min dist
            cal1->setMinDist2(-99);           // no min mindist
            cal1->setNTrack1(-99);            // no track id
            cal1->setNTrack2(-99);            // no track id
            fillNoiseLists(cal1,-4,1);        // fill list of status/tracks
    }
}
void HMdcDigitizer::setLoopVariables(Int_t s,Int_t m,Int_t l,Int_t c,Bool_t check)
{
    if(check)handleOverFlow(s,m,l,c);
    else {
	firstsec =s;
	firstmod =m;
	firstlay =l;
        firstcell=c;
    }
}
void HMdcDigitizer::handleOverFlow(Int_t firsts, Int_t firstm, Int_t firstl, Int_t firstc)
{
    // if last cell in Layer has been reached
    // the next valid cell in setup has to be taken

    if(firstc>(*geomstruct)[firsts][firstm][firstl])
    {
	// find next valid module

	if(firstl<5)
	{ // just switch to next layer
	    setLoopVariables(firsts,firstm,firstl+1,0);
	}
	else
	{ // if last layer, we have to move to next valid module
	    Int_t found=0;
	    for(Int_t i=firsts;i<6;i++)
	    {
		for(Int_t j=firstm;j<4;j++)
		{
		    if(testMdcSetup(i,j))
		    {   // if a valid module in this sector has been found
			found++;
			if(found==1) setLoopVariables(i,j,0,0);
		    }
		}
	    }
	    // if no valid module found switch to last ( will be skipped : 500 < lastcell)
	    if(found==0) setLoopVariables(firsts,firstm,firstl,500);
	}
    } else setLoopVariables(firsts,firstm,firstl,firstc);
}

void HMdcDigitizer::fillNoise(Int_t firsts, Int_t firstm, Int_t firstl, Int_t firstc,
			      Int_t lastsec, Int_t lastmod, Int_t lastlay, Int_t lastcell)
{
    // Fills the noise cells to Cal1Sim up to the next GEANT Cell
    // and the rest of the noise cells after the last GEANT Cell (if
    // input lastcell==999).

    fCalnoise = 0;
    locnoise.set(4,0,0,0,0);

    if(lastcell!=999) handleOverFlow(firsts,firstm,firstl,firstc);

    for (Int_t sec=firstsec;sec<=lastsec;sec++)
    {
        if(sec>5) continue;
	for (Int_t mod=firstmod;mod<=lastmod;mod++)
	{
            if(mod>3) continue;
	    // test if module is existing in current setup
	    if(!testMdcSetup(sec,mod) )continue;

	    for (Int_t lay=firstlay;lay<=lastlay;lay++)
	    {
                if(lay>5) continue;
		if(lastcell!=999)
		{
		    for (Int_t cell=firstcell;cell<lastcell;cell++)
		    {
                        if(cell>(*geomstruct)[sec][mod][lay]) continue;

			if(evalWireStat(sec,mod,lay,cell))
                        { // if wire is connected and working
                            if(gRandom->Rndm()<getNoiseLevel(mod))
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
                                    if (getTdcMode()==1)
                                    {
                                        fCalnoise->setNHits(-1);
                                        fCalnoise->setStatus1(2);
                                        fCalnoise->setStatus2(-3);


					setTime1Noise(fillTime1Noise(locnoise[1]));
                                        setTime2Noise(-999);
                                        fCalnoise->setTime1(getTime1Noise());

					if(getNTuple())
                                        {
                                            fillNTuple(locnoise[0],locnoise[1],locnoise[2],locnoise[3],
                                                       getTime1Noise(),getTime2Noise(),2);
                                        }
                                    }
                                    else if(getTdcMode()==2)
                                    {
					fCalnoise->setNHits(2);
                                        fCalnoise->setStatus1(2);
                                        fCalnoise->setStatus2(2);
                                        setTime1Noise(fillTime1Noise(locnoise[1]));
                                        setTime2Noise(fillTime2Noise(locnoise[1]));
                                        fCalnoise->setTime1(getTime1Noise());
                                        fCalnoise->setTime2(getTime2Noise());

					if(getNTuple())
                                        {
                                            fillNTuple(locnoise[0],locnoise[1],locnoise[2],locnoise[3],
                                                       getTime1Noise(),getTime2Noise(),2);
                                        }
                                    }
                                    fillNoiseLists(fCalnoise,2,0);
				}
                            }
                        }
                    }
                }
                if(lastcell==999)
                {   // fill up to the last existing wire
                    HMdcLayerGeomParLay& layernoise = (*fDigitGeomPar)[sec][mod][lay];
                    Int_t   nWires   = layernoise.getNumWires(); // number of wires per layer

                    for (Int_t cell=firstc;cell<nWires;cell++)
                    {
                       if(evalWireStat(sec,mod,lay,cell))
                        { // if wire is connected and working
                            if(gRandom->Rndm()<getNoiseLevel(mod))
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
                                    if (getTdcMode()==1)
                                    {
                                        fCalnoise->setNHits(-1);
                                        fCalnoise->setStatus1(2);
                                        fCalnoise->setStatus2(-3);
                                        setTime1Noise(fillTime1Noise(locnoise[1]));
                                        setTime2Noise(-999);
                                        fCalnoise->setTime1(getTime1Noise());

					if(getNTuple())
                                        {
                                            fillNTuple(locnoise[0],locnoise[1],locnoise[2],locnoise[3],
                                                       getTime1Noise(),getTime2Noise(),2);
                                        }
                                    }
                                    else if(getTdcMode()==2)
                                    {
					fCalnoise->setNHits(2);
                                        fCalnoise->setStatus1(2);
                                        fCalnoise->setStatus2(2);
                                        setTime1Noise(fillTime1Noise(locnoise[1]));
                                        setTime2Noise(fillTime2Noise(locnoise[1]));
                                        fCalnoise->setTime1(getTime1Noise());
                                        fCalnoise->setTime2(getTime2Noise());

					if(getNTuple())
                                        {
                                            fillNTuple(locnoise[0],locnoise[1],locnoise[2],locnoise[3],
                                                       getTime1Noise(),getTime2Noise(),2);

                                        }
                                    }
				    fillNoiseLists(fCalnoise,2,0);
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
    // fills track list and status list for hits in both tdc modes

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
        // and skipped to end => no valid second hit found
	setSecondHit(-999);
    }
}
void HMdcDigitizer::select(Int_t nHits)
{
    // Puts the drift time values into increasing order.
    // Orders the corresponding track number, time of flight, statusflag
    // and impact angle etc accordingly

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
    Float_t fractionlocal;
    Float_t wireOffsetlocal;

    if(nHits<=15)
    {
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
	    fractionlocal=fractionOfmaxCharge[a];
            wireOffsetlocal=wireOffset[a];

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
		    fractionlocal=fractionOfmaxCharge[b];
		    wireOffsetlocal=wireOffset[b];

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
		fractionOfmaxCharge[c]=fractionOfmaxCharge[a];
		fractionOfmaxCharge[a]=fractionlocal;
		wireOffset[c]=wireOffset[a];
		wireOffset[a]=wireOffsetlocal;
	    }
	}

    }
    else
    {
	Warning("HMdcDigitizer:select(nHits)","nHits > 15! Entries >15 skipped! ");
    }
}
void HMdcDigitizer::getMdcSetup()
{
    // Gets Mdc detector setup

    HMdcDetector* mdcDet=(HMdcDetector*)(((HSpectrometer*)(gHades->getSetup()))->getDetector("Mdc"));
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
    // All needed parameters are taken from HMdcLayerGeomPar and HMdcDigitPar
    // containers.

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

    // in HMdcCal2ParSim:
    // Input is the angle of the track (alphaTrack:90 degree for perpendicular impact),which
    // has to be recalculated to the angle of minimum drift distance (alphaDrDist:0 degree for
    // perpendicular impact).
    //   y ^
    //     |  |------------*----|              cathod  plane
    //     |  | cell        *   |
    //     |  |            / *  |
    //     |  |           /90 * |
    //     |  | driftDist/     *|
    //     |  |         / |     *
    //   --|--|--------*^-|-----|*--------->   sense/potential plane
    //     |  |           |     | *        x
    //     |  |      alphaDrDist|  *
    //        |                 |/  *          alphaDriftDist=90-alphaTrack
    //        |      alphaTrack /    *
    //        |-----------------|     * track  cathod plane
    //
    // angles must be between 0 and 90 degree, all other angles have to be shifted
    // before calling the function.


    // convert to range 0-90 degree
    if(myalpha<=90  && myalpha>=0)
    {  // normal case
        myalpha=myalpha;
    }
    else
    {
	if(myalpha<=180 && myalpha> 90 )
	{  //reflect upper left to upper right
	    myalpha=(180-myalpha);
	}
	else
	{   // reflect lower left to upper right
	    if(myalpha<=270 && myalpha> 180) myalpha=(270-myalpha);
            // reflect lower right to upper right
	    if(myalpha< 360 && myalpha> 270) myalpha=(360-myalpha);
	}
    }

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

	Float_t wireOffset=0;

	if(getWireOffsetUse())wireOffset=calcWireOffset(xcoor,ycoor,wOrient);

	storeCell(per,tof,myalpha,trkNum,flagCutEdge,wireOffset);//store the final values in container
    }
    return kTRUE;
}
Float_t HMdcDigitizer::calcWireOffset(Float_t xcoor,Float_t ycoor,Float_t wOrient)
{
    // calculate the time needed by the signal to propagate to the tdc
    Float_t x_wire = xcoor*cos(wOrient)+ycoor*sin(wOrient);

    Float_t pathwire=0;

    HMdcSizesCellsCell& mycell=(*fsizescells)[loc[0]][loc[1]][loc[2]][loc[3]];

    if(!&mycell || mycell.getReadoutSide()=='0')
    {
	if(loc[3]>3&& loc[3]<(((*geomstruct)[loc[0]][loc[1]][loc[2]])-4))
	{
	    Warning("HMdcDigitizer:transform()","Not connected wire detected s %i m %i l %i c %i !",loc[0],loc[1],loc[2],loc[3]);
	}
    }
    else
    {
	pathwire= (mycell.getReadoutSide()=='1') ? mycell.getXReadout()-x_wire : x_wire-mycell.getXReadout();
    }

    return getSignalSpeed()*pathwire;
}
void HMdcDigitizer::storeCell(Float_t per,Float_t tof,Float_t myangle,Int_t trkNum
			      ,Bool_t flagCutEdge,Float_t wireOffset)
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
        hit->setWireOffset(wireOffset,nHit);
    }
    else
    {
	Warning("HMdcDigitizer:storeCell()","hit could not be stored in HMdcGeantCell ,\n because number of hits exceeded the maximum!");
    }
}

ClassImp(HMdcDigitizer)
