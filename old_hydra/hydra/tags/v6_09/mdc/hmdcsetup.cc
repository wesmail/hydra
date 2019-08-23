//*-- AUTHOR : J. Markert

////////////////////////////////////////////////////////////////////////////
// HMdcSetup
//
// Container class for the parameters for HMdcTaskset
// Setup objects for the tasks of HMdcTaskSet are created:
// HMdcDitizerSetup,HMdcCalibrater1Setup,HMdcTrackFinderSetup,
// HMdcClusterHitSetup,HMdcFitterSetup and HMdcCommonSetup.
// These setup objects hold all parameters to configure the tasks
// in all possible ways. The objects are initialized with some non valid
// values (-99 for Int_t variables and 3 for Bool_t).
//
// The parameters contain (example):
// ############################################################################################
// HMdcSetup:
// --------------------------------------------------------------------------------------------
// HMdcCommonSetup:
// isSimulation    =  0 :  0 = real data, 1 = simulation, 2 = merge
// analysisLevel   =  4 :  0 = raw, 1 = cal1, 2 = cal2, 3 = hit, 4 = fit
// tracker         =  1 :  1 = dubna, 2 = santiago
// --------------------------------------------------------------------------------------------
// HMdcCalibrater1Setup:
// ModeFlagCal1    =  2 :  1 = NoStartandCal, 2 = StartandCal, 3 = NoStartandNoCal
// TimeCutFlagCal1 =  1 :  0 = noTimeCut    , 1 = TimeCut
// --------------------------------------------------------------------------------------------
// HMdcTrackFinderSetup:
// MagnetStatus    =  0 :  0 = MagnetOn,  1 = MagnetOff
// ClustFinderType =  0 :  0 = combined chamber clusters, 1 = single chamber clusters
// LevelsSegment1  =  10,50  : max. occupancy of MDC for level of cl.finding 4 & 5 layers
// LevelsSegment2  =  10,30  : max. occupancy of MDC for level of cl.finding 4 & 5 layers
// NumLayersPerMod = {{6,6,6,6},
//                    {6,6,6,6},
//                    {6,6,6,6},
//                    {6,6,6,6},
//                    {6,6,6,6},
//                    {6,6,6,6}}
// --------------------------------------------------------------------------------------------
// HMdcClusterToHitSetup:
// ModForSeg1      =  1 : -1 = don't fill HMdcSeg for segment 1,
//                         0 = fill HMdcSeg by cluster in MDC1 if typeClFinder=1
//                             and sector has mdc1&2, else by cluster in existing MDC,
//                         1 =  -/- in mdc2,
// ModForSeg2      =  3 : -1 = don't fill HMdcSeg for segment 2,
//                         2 = fill HMdcSeg by cluster in MDC3 if typeClFinder=1
//                             and sector has mdc3&4 and MagnetOff,
//                             else by cluster in existing MDC,
//                         3 =  -/- in MDC4
// --------------------------------------------------------------------------------------------
// HMdc12FitSetup:
// FitAuthor       =  1 :  0 = Hejdar     , 1 = Alexander\n",
// DrTimeCalcVers  =  1 :  0 = polynom    , 1 = HMdCal2ParSim::calcTime\n"
// FitType         =  0 :  0 = segment fit, 1 = independent mdc fit
// DebugCategorie  =  0 :  1 = create and fill catMdcClusFit&catMdcWireFit, 0 = don't cr.
// DebugPrint      =  0 :  1 = print debug information, 0 = don't print
// --------------------------------------------------------------------------------------------
// HMdcDigiSetup:
// tdcModeDigi     =  2 :  1 = two leading edges, 2 = leading and trailing edge
// NtupleDigi      =  1 :  0 = noNtuple, 1 = digitizer.root
// useTofDigi      =  1 :  0 = NoTof in cal1, 1 = Tof in cal1
// 
// offsetsOnDigi   =  1 :  0 = global offsets off, 1 = global offsets on
// offsetsDigi     =  1.5   2.5    4.5   5.5 ns offset per plane (substracted from (drift time + tof))
// 
// noiseModeDigi   =  1 :  1 = override geant by noise, 2 = keep geant cells
// noiseOnDigi     =  0 :  0 = noise off, 1 = noise on
// noiseLevelDigi  =  5.0%  5.0%   5.0%  5.0% noise level per plane
// noiseRangeDigi  = -700  -700  -700  -700  1000  1000  1000  1000 ns lower/upper limit of noise
// 
// cellEffOnDigi   =  1 :  0 = cellEff off, 1 = cellEff
// cellEffDigi     =  82.0% 82.0%  88.0% 88.0% level of cellEff per plane
// ############################################################################################ 
////////////////////////////////////////////////////////////////////////////
#include <stdlib.h>

#include "hmdcsetup.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hmdcdetector.h"
#include "hpario.h"
#include "hdetpario.h"
#include "hparamlist.h"
#include "hmdcdigitizer.h"
#include "hmdccalibrater1.h"
#include "hmdctrackdset.h"

#include <iostream.h>

ClassImp(HMdcSetup)
ClassImp(HMdcDigitizerSetup)
ClassImp(HMdcCalibrater1Setup)
ClassImp(HMdcTrackFinderSetup)
ClassImp(HMdcClusterToHitSetup)
ClassImp(HMdc12FitSetup)
ClassImp(HMdcCommonSetup)

HMdcSetup::HMdcSetup(const char* name,const char* title,
                       const char* context)
    : HParCond(name,title,context)
{
    // creates setup objects for the tasks of HMdcTaskSet:
    // HMdcDitizerSetup,HMdcCalibrater1Setup,HMdcTrackFinderSetup,
    // HMdcClusterHitSetup,HMdc12FitSetup and HMdcCommonSetup.
    // These setup objects hold all parameters to configure the tasks
    // in all possible ways. The objects are initialized with some non valid
    // values (-99 for Int_t variables and 3 for Bool_t).
  strcpy(detName,"Mdc");
  if (gHades) {
      fMdc = (HMdcDetector*)gHades->getSetup()->getDetector("Mdc");
  } else {
      fMdc = 0;
  }
  digiset        = new HMdcDigitizerSetup("MdcDigitizerSetup",""      ,this);
  cal1set        = new HMdcCalibrater1Setup("MdcCalibrater1Setup",""  ,this);
  trackfinderset = new HMdcTrackFinderSetup("MdcTrackFinderSetup",""  ,this);
  clustertohitset= new HMdcClusterToHitSetup("MdcClusterToHitSetup","",this);
  fitterset      = new HMdc12FitSetup("Mdc12FitSetup",""              ,this);
  commonset      = new HMdcCommonSetup("MdcCommonSetup",""            ,this);
  clear();
}

HMdcSetup::~HMdcSetup()
{
  // destructor
}
void HMdcSetup::clear()
{
    digiset        ->clear();
    cal1set        ->clear();
    trackfinderset ->clear();
    clustertohitset->clear();
    fitterset      ->clear();
    commonset      ->clear();

    status=kFALSE;
    resetInputVersions();
    changed=kFALSE;
}
Bool_t HMdcSetup::check()
{
    // all parameters are checked for non valid values
    if(!digiset        ->check()) {Error("HMdcSetup:check()","HMdcDitizerSetup not fully initialized");     return kFALSE;}
    if(!cal1set        ->check()) {Error("HMdcSetup:check()","HMdcCalibrater1Setup not fully initialized"); return kFALSE;}
    if(!trackfinderset ->check()) {Error("HMdcSetup:check()","HMdcTrackFinderSetup not fully initialized"); return kFALSE;}
    if(!clustertohitset->check()) {Error("HMdcSetup:check()","HMdcClusterToHitSetup not fully initialized");return kFALSE;}
    if(!fitterset      ->check()) {Error("HMdcSetup:check()","HMdc12FitSetup not fully initialized");       return kFALSE;}
    if(!commonset      ->check()) {Error("HMdcSetup:check()","HMdcCommonSetup not fully initialized");      return kFALSE;}

    return kTRUE;
}
void HMdcSetup::printParam(void)
{
    // prints the parameters of HMdcSetup to the screen.
    // The printParam() of the subobjects are called to print the
    // parameters of the corresponding subobject.
    printf ("############################################################################################\n");
    printf ("HMdcSetup:\n");
    commonset      ->printParam();
    cal1set        ->printParam();
    trackfinderset ->printParam();
    clustertohitset->printParam();
    fitterset      ->printParam();
    digiset        ->printParam();
    printf ("############################################################################################\n");
}
Bool_t HMdcSetup::init(HParIo* inp,Int_t* set)
{
  // intitializes the container from an input
  HDetParIo* input=inp->getDetParIo("HMdcParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}
Int_t HMdcSetup::write(HParIo* output)
{
  // writes the container to an output
  HDetParIo* out=output->getDetParIo("HMdcParIo");
  if (out) return out->write(this);
  return -1;
}
void HMdcSetup::putParams(HParamList* l)
{
    // Puts all params of HMdcSetup to the parameter list of
    // HParamList (which ist used by the io);
    // The  putParams() functions of all subobjects are called to add the
    // parameters of the corresponding subobject.
    if (!l) return;
    commonset      ->putParams(l);
    cal1set        ->putParams(l);
    trackfinderset ->putParams(l);
    clustertohitset->putParams(l);
    fitterset      ->putParams(l);
    digiset        ->putParams(l);
}
Bool_t HMdcSetup::getParams(HParamList* l)
{
    if (!l) return kFALSE;
    if(!commonset      ->getParams(l)) return kFALSE;
    if(!cal1set        ->getParams(l)) return kFALSE;
    if(!trackfinderset ->getParams(l)) return kFALSE;
    if(!clustertohitset->getParams(l)) return kFALSE;
    if(!fitterset      ->getParams(l)) return kFALSE;
    if(!digiset        ->getParams(l)) return kFALSE;
    return kTRUE;
}
//################################## DIGISETUP #############################
HMdcDigitizerSetup::HMdcDigitizerSetup(const char* name="",const char* title="",HMdcSetup* set=0)
    : TNamed(name,title)
{
    // Constructor of the setup object for HMdcDigitizer
    setup=set;
    offsetsDigi     .Set(4);
    cellEffDigi     .Set(4);
    noiseLevelDigi  .Set(4);
    noiseRangeLoDigi.Set(4);
    noiseRangeHiDigi.Set(4);
    clear();
}
HMdcDigitizerSetup::~HMdcDigitizerSetup()
{
  // destructor of the setup object for HMdcDigitizer
}
void HMdcDigitizerSetup::clear()
{
    // all parameters of the object are initialized with
    // non valid values.
    tdcModeDigi       =-99;    // 1,2 :default 2(leading and trailing edge)
    NtupleDigi        =-99;    //default kFALSE
    useTofDigi        =-99;    //default kTRUE
    useErrorDigi      =-99;    //default kTRUE
    useWireOffsetDigi =-99;    //default kTRUE
    embeddingModeDigi =-99;    //default 1
    noiseModeDigi     =-99;    // 1,2: default 1 override geant

    offsetsOnDigi =-99;    // switch on/off this option
    cellEffOnDigi =-99;    // switch on/off this option
    noiseOnDigi   =-99;    // switch on/off this option

    for(Int_t i=0;i<4;i++)
    {
	noiseLevelDigi[i]  =-99; // 0.,0.,0.,0.
	offsetsDigi[i]     =-99; // default 1.5,2.5,4.5,5.5
	cellEffDigi[i]     =-99; // default 82.,82.,88.,88.
	noiseRangeLoDigi[i]=-99; // -700,-700,-700,-700
	noiseRangeHiDigi[i]=-99; // 1000,1000,1000,1000
    }
}
Bool_t HMdcDigitizerSetup::check()
{
    // all parameters of the object are checked for non valid values.
    if(tdcModeDigi        ==-99) return kFALSE;
    if(NtupleDigi         ==-99) return kFALSE;
    if(useTofDigi         ==-99) return kFALSE;
    if(useErrorDigi       ==-99) return kFALSE;
    if(useWireOffsetDigi  ==-99) return kFALSE;
    if(embeddingModeDigi  ==-99) return kFALSE;
    if(noiseModeDigi      ==-99) return kFALSE;

    if(offsetsOnDigi      ==-99) return kFALSE;
    if(cellEffOnDigi      ==-99) return kFALSE;
    if(noiseOnDigi        ==-99) return kFALSE;

    for(Int_t i=0;i<4;i++)
    {
	if(noiseLevelDigi[i]  ==-99) return kFALSE;
	if(offsetsDigi[i]     ==-99) return kFALSE;
	if(cellEffDigi[i]     ==-99) return kFALSE;
	if(noiseRangeLoDigi[i]==-99) return kFALSE;
	if(noiseRangeHiDigi[i]==-99) return kFALSE;
    }
   return kTRUE;
}
void HMdcDigitizerSetup::printParam(void)
{
    // prints the parameters to the screen
    printf ("--------------------------------------------------------------------------------------------\n");
    printf ("HMdcDigitizerSetup:\n");
    printf ("tdcModeDigi       =  %i :  1 = two leading edges, 2 = leading and trailing edge\n",        tdcModeDigi);
    printf ("NtupleDigi        =  %i :  0 = noNtuple, 1 = digitizer.root\n",                            NtupleDigi);
    printf ("useTofDigi        =  %i :  0 = No Tof in cal1, 1 = Tof in cal1 \n",                      useTofDigi);
    printf ("useErrorDigi      =  %i :  0 = No Err in cal1, 1 = Err in cal1 \n",                      useErrorDigi);
    printf ("useWireOffsetDigi =  %i :  1 = add wireOffset to drift time, 0 = don't add wireOffsets \n",useWireOffsetDigi);
    printf ("embeddingModeDigi =  %i :  1 = override geant by real, 2 = keep geant cells \n\n",           embeddingModeDigi);
    printf ("offsetsOnDigi     =  %i :  0 = global offsets off, 1 = global offsets on\n",       offsetsOnDigi);
    printf ("offsetsDigi       = %4.1f  %4.1f   %4.1f  %4.1f ns offset per plane (substracted from (drift time + tof))\n\n"
	    ,offsetsDigi[0],offsetsDigi[1],offsetsDigi[2],offsetsDigi[3]);

    printf ("noiseModeDigi     =  %i :  1 = override geant by noise, 2 = keep geant cells \n",  noiseModeDigi);
    printf ("noiseOnDigi       =  %i :  0 = noise off, 1 = noise on\n",                         noiseOnDigi);
    printf ("noiseLevelDigi    = %4.1f%% %4.1f%%  %4.1f%% %4.1f%% noise level per plane\n"
	    ,noiseLevelDigi[0],noiseLevelDigi[1],noiseLevelDigi[2],noiseLevelDigi[3]);
    printf ("noiseRangeDigi    =%5i %5i %5i %5i %5i %5i %5i %5i ns lower/upper limit of noise\n\n"
	    ,noiseRangeLoDigi[0],noiseRangeLoDigi[1],noiseRangeLoDigi[2],noiseRangeLoDigi[3]
            ,noiseRangeHiDigi[0],noiseRangeHiDigi[1],noiseRangeHiDigi[2],noiseRangeHiDigi[3]);

    printf ("cellEffOnDigi     =  %i :  0 = cellEff off, 1 = cellEff\n",cellEffOnDigi);
    printf ("cellEffDigi       =  %4.1f%% %4.1f%%  %4.1f%% %4.1f%% level of cellEff per plane \n"
	    ,cellEffDigi[0],cellEffDigi[1],cellEffDigi[2],cellEffDigi[3]);
}
Bool_t HMdcDigitizerSetup::getParams(HParamList* l)
{
   // gets the parameters from HParamList, which is used for io.
   if(!( l->fill("tdcModeDigi",      &tdcModeDigi)))       return kFALSE;
   if(!( l->fill("NtupleDigi",       &NtupleDigi) ))       return kFALSE;
   if(!( l->fill("useTofDigi",       &useTofDigi) ))       return kFALSE;
   if(!( l->fill("useErrorDigi",     &useErrorDigi) ))     return kFALSE;
   if(!( l->fill("useWireOffsetDigi",&useWireOffsetDigi) ))return kFALSE;
   if(!( l->fill("embeddingModeDigi",&embeddingModeDigi) ))return kFALSE;
   if(!( l->fill("offsetsOnDigi",    &offsetsOnDigi)))     return kFALSE;
   if(!( l->fill("offsetsDigi",      &offsetsDigi)))       return kFALSE;
   if(!( l->fill("cellEffOnDigi",    &cellEffOnDigi)))     return kFALSE;
   if(!( l->fill("cellEffDigi",      &cellEffDigi)))       return kFALSE;
   if(!( l->fill("noiseModeDigi",    &noiseModeDigi)))     return kFALSE;
   if(!( l->fill("noiseOnDigi",      &noiseOnDigi)))       return kFALSE;
   if(!( l->fill("noiseLevelDigi",   &noiseLevelDigi)))    return kFALSE;
   if(!( l->fill("noiseRangeLoDigi", &noiseRangeLoDigi)))  return kFALSE;
   if(!( l->fill("noiseRangeHiDigi", &noiseRangeHiDigi)))  return kFALSE;

   return kTRUE;
}
void HMdcDigitizerSetup::putParams(HParamList* l)
{
    // puts the parameters of HMdcDigiSetup to
    // HParamList, which is used for io.
    l->add("tdcModeDigi",      tdcModeDigi);
    l->add("NtupleDigi",       NtupleDigi);
    l->add("useTofDigi",       useTofDigi);
    l->add("useErrorDigi",     useErrorDigi);
    l->add("useWireOffsetDigi",useWireOffsetDigi);
    l->add("embeddingModeDigi",embeddingModeDigi);
    l->add("offsetsOnDigi",    offsetsOnDigi);
    l->add("offsetsDigi",      offsetsDigi);
    l->add("cellEffOnDigi",    cellEffOnDigi);
    l->add("cellEffDigi",      cellEffDigi);
    l->add("noiseOnDigi",      noiseOnDigi);
    l->add("noiseModeDigi",    noiseModeDigi);
    l->add("noiseLevelDigi",   noiseLevelDigi);
    l->add("noiseRangeLoDigi", noiseRangeLoDigi);
    l->add("noiseRangeHiDigi", noiseRangeHiDigi);
}
void HMdcDigitizerSetup::setupMdcDigitizer(HMdcDigitizer* digi)
{
    // calls the Set functions of HMdigitizer to set the parameters
    // from HMdcDigiSetup.

    digi->setTdcMode      (tdcModeDigi);
    digi->setNTuple       ((Bool_t)NtupleDigi);
    digi->setTofUse       ((Bool_t)useTofDigi);
    digi->setErrorUse     ((Bool_t)useErrorDigi);
    digi->setWireOffsetUse((Bool_t)useWireOffsetDigi);
    digi->setEmbeddingMode(embeddingModeDigi);
    digi->setOffsets   (offsetsDigi[0]     ,offsetsDigi[1]     ,offsetsDigi[2]     ,offsetsDigi[3]   ,offsetsOnDigi);
    digi->setEffLevel  (cellEffDigi[0]     ,cellEffDigi[1]     ,cellEffDigi[2]     ,cellEffDigi[3]   ,cellEffOnDigi);
    digi->setNoiseLevel(noiseLevelDigi[0]  ,noiseLevelDigi[1]  ,noiseLevelDigi[2]  ,noiseLevelDigi[3],noiseOnDigi);
    digi->setNoiseRange(noiseRangeLoDigi[0],noiseRangeLoDigi[1],noiseRangeLoDigi[2],noiseRangeLoDigi[3],
			noiseRangeHiDigi[0],noiseRangeHiDigi[1],noiseRangeHiDigi[2],noiseRangeHiDigi[3]);
    digi->setNoiseMode (noiseModeDigi);
}
//################################## CALIBRATER1SETUP #############################
HMdcCalibrater1Setup::HMdcCalibrater1Setup(const char* name="",const char* title="",HMdcSetup* set=0)
    : TNamed(name,title)
{
    // constructor for HMdcCalibrater1Setup
    // The parameters are initialized with non valid values.
    setup=set;
    clear();
}
HMdcCalibrater1Setup::~HMdcCalibrater1Setup()
{
  // destructor of HMdcCalibrater1Setup
}
void HMdcCalibrater1Setup::clear()
{
    // The parameters are initialized with non valid values.
    ModeFlagCal1   =-99; //1,2,3
    TimeCutFlagCal1=-99; //0,1
}
Bool_t HMdcCalibrater1Setup::check()
{
    // The parameters are checked for non valid values.
    if(ModeFlagCal1   ==-99) return kFALSE;
    if(TimeCutFlagCal1==-99) return kFALSE;
    return kTRUE;
}
void HMdcCalibrater1Setup::printParam(void)
{
    // prints the parameters to the screen
    printf ("--------------------------------------------------------------------------------------------\n");
    printf ("HMdcCalibrater1Setup:\n");
    printf ("ModeFlagCal1    =  %i :  1 = NoStartandCal, 2 = StartandCal, 3 = NoStartandNoCal\n",ModeFlagCal1);
    printf ("TimeCutFlagCal1 =  %i :  0 = noTimeCut    , 1 = TimeCut\n",                         TimeCutFlagCal1);
}
Bool_t HMdcCalibrater1Setup::getParams(HParamList* l)
{
    // gets the parameters from HParamList, which is used for io.
    if(!(l->fill("ModeFlagCal1",   &ModeFlagCal1   ))) return kFALSE;
    if(!(l->fill("TimeCutFlagCal1",&TimeCutFlagCal1))) return kFALSE;
    return kTRUE;
}
void HMdcCalibrater1Setup::putParams(HParamList* l)
{
    // puts the parameters of HMdcCalibrater1Setup to
    // HParamList, which is used for io.
    l->add("ModeFlagCal1",   ModeFlagCal1);
    l->add("TimeCutFlagCal1",TimeCutFlagCal1);
}
void HMdcCalibrater1Setup::setupMdcCalibrater1(HMdcCalibrater1* calibrater1,Int_t merge=0)
{
    if(setup)
    {
	if(setup->getMdcCommonSet()->getIsSimulation()==2) merge=1;
    }
    calibrater1->switchArguments(ModeFlagCal1,TimeCutFlagCal1, merge);
}
//################################## TRACKFINDERSETUP #############################
HMdcTrackFinderSetup::HMdcTrackFinderSetup(const char* name="",const char* title="",HMdcSetup* set=0)
    : TNamed(name,title)
{
    // constructor of HMdcTrackFinderSetup
    // The parameters are initialized with non valid values
    setup=set;
    nLayers.Set(24);
    nLevel.Set(4);
    clear();
}
HMdcTrackFinderSetup::~HMdcTrackFinderSetup()
{
  // destructor HMdcTrackFinderSetup
}
void HMdcTrackFinderSetup::clear()
{
    // The parameters are initialized with non valid values
    isCoilOff      =-99;   // kTRUE=magnet off
    typeClustFinder=-99; // 0 segment, 1 mdc
    for(Int_t i=0;i<24;i++){nLayers[i]=-99;}
    for(Int_t i=0;i<4;i++) {nLevel [i]=-99;}
}
Bool_t HMdcTrackFinderSetup::check()
{
    // The parameters are checked for non valid values
    if(isCoilOff      ==-99) return kFALSE;
    if(typeClustFinder==-99) return kFALSE;
    for(Int_t i=0;i<24;i++)
    {
	if(nLayers[i]==-99)  return kFALSE;
    }
    for(Int_t i=0;i<4;i++)
    {
	if(nLevel [i]==-99)  return kFALSE;
    }
    return kTRUE;
}
void HMdcTrackFinderSetup::printParam(void)
{
    // prints the parameters to the screen
    printf ("--------------------------------------------------------------------------------------------\n");
    printf ("HMdcTrackFinderSetup:\n");
    printf ("MagnetStatus    =  %i :  0 = MagnetOn,  1 = MagnetOff\n",                              isCoilOff);
    printf ("ClustFinderType =  %i :  0 = combined chamber clusters, 1 = single chamber clusters\n",typeClustFinder);
    printf ("LevelsSegment1  =  %2i,%2i  : max. occupancy of MDC for level of cl.finding 4 & 5 layers\n" ,nLevel[0],nLevel[1]);
    printf ("LevelsSegment2  =  %2i,%2i  : max. occupancy of MDC for level of cl.finding 4 & 5 layers\n" ,nLevel[2],nLevel[3]);
    printf ("NumLayersPerMod = {{%i,%i,%i,%i}, \n",nLayers[0] ,nLayers[1] ,nLayers[2] ,nLayers[3] );
    printf ("                   {%i,%i,%i,%i}, \n",nLayers[4] ,nLayers[5] ,nLayers[6] ,nLayers[7] );
    printf ("                   {%i,%i,%i,%i}, \n",nLayers[8] ,nLayers[9] ,nLayers[10],nLayers[11]);
    printf ("                   {%i,%i,%i,%i}, \n",nLayers[12],nLayers[13],nLayers[14],nLayers[15]);
    printf ("                   {%i,%i,%i,%i}, \n",nLayers[16],nLayers[17],nLayers[18],nLayers[19]);
    printf ("                   {%i,%i,%i,%i}} \n",nLayers[20],nLayers[21],nLayers[22],nLayers[23]);
}
Bool_t HMdcTrackFinderSetup::getParams(HParamList* l)
{
   // gets the parameters from HParamList, which is used for io.
    if(!(l->fill("isCoilOff",      &isCoilOff      ))) return kFALSE;
    if(!(l->fill("typeClustFinder",&typeClustFinder))) return kFALSE;
    if(!(l->fill("nLevel" ,        &nLevel         ))) return kFALSE;
    if(!(l->fill("nLayers",        &nLayers        ))) return kFALSE;
    return kTRUE;
}
void HMdcTrackFinderSetup::putParams(HParamList* l)
{
    // puts the parameters of HMdcTrackFinderSetup to
    // HParamList, which is used for io.
    l->add("isCoilOff",      isCoilOff);
    l->add("typeClustFinder",typeClustFinder);
    l->add("nLevel" ,        nLevel);
    l->add("nLayers",        nLayers);
}
void HMdcTrackFinderSetup::setupMdcTrackFinder()
{
    Int_t myLayers[6][4];
    getNLayers(&myLayers[0][0]);
    HMdcTrackDSet::setMdcTrackFinder((Bool_t)isCoilOff,typeClustFinder);
    HMdcTrackDSet::setTrFnNLayers(&myLayers[0][0]);
    HMdcTrackDSet::setTrFnLevel(nLevel[0],nLevel[1],nLevel[2],nLevel[3]);
}
//################################## CLUSTERTOSEGSETUP #############################
HMdcClusterToHitSetup::HMdcClusterToHitSetup(const char* name="",const char* title="",HMdcSetup* set=0)
    : TNamed(name,title)
{
    // constructor of HMdcClusterToHitSetup
    // The parameters are initialized with non valid values
    setup=set;
    clear();
}
HMdcClusterToHitSetup::~HMdcClusterToHitSetup()
{
  // destructor HMdcClusterHitSetup
}
void HMdcClusterToHitSetup::clear()
{
    // The parameters are initialized with non valid values
    mdcForSeg1=-99;
    mdcForSeg2=-99;
}
Bool_t HMdcClusterToHitSetup::check()
{
    // The parameters are checked for non valid values
    if(mdcForSeg1==-99) return kFALSE;
    if(mdcForSeg2==-99) return kFALSE;
    return kTRUE;
}
void HMdcClusterToHitSetup::printParam(void)
{
    // prints the parameters to the screen
    printf ("--------------------------------------------------------------------------------------------\n");
    printf ("HMdcClusterToHitSetup:\n");
    printf ("ModForSeg1      =  %i : -1 = don't fill HMdcSeg for segment 1,\n",mdcForSeg1);
    printf ("                        0 = fill HMdcSeg by cluster in MDC1 if typeClFinder=1\n");
    printf ("                            and sector has mdc1&2, else by cluster in existing MDC,\n");
    printf ("                        1 =  -/- in mdc2,\n");
    printf ("ModForSeg2      =  %i : -1 = don't fill HMdcSeg for segment 2,\n",mdcForSeg2);
    printf ("                        2 = fill HMdcSeg by cluster in MDC3 if typeClFinder=1\n");
    printf ("                            and sector has mdc3&4 and MagnetOff,\n");
    printf ("                            else by cluster in existing MDC,\n");
    printf ("                        3 =  -/- in MDC4\n");
}
Bool_t HMdcClusterToHitSetup::getParams(HParamList* l)
{
    // gets the parameters from HParamList, which is used for io.
    if(!(l->fill("mdcForSeg1",&mdcForSeg1))) return kFALSE;
    if(!(l->fill("mdcForSeg2",&mdcForSeg2))) return kFALSE;
    return kTRUE;
}
void HMdcClusterToHitSetup::putParams(HParamList* l)
{
    // puts the parameters of HMdcClusterHitSetup to
    // HParamList, which is used for io.
    l->add("mdcForSeg1",mdcForSeg1);
    l->add("mdcForSeg2",mdcForSeg2);
}
void HMdcClusterToHitSetup::setupMdcClusterToHit()
{
    HMdcTrackDSet::setMdcClusterToHit(mdcForSeg1,mdcForSeg2);
}
//################################## FITTERSETUP #############################
HMdc12FitSetup::HMdc12FitSetup(const char* name="",const char* title="",HMdcSetup* set=0)
    : TNamed(name,title)
{
    // constructor of HMdc12FitSetup
    // The parameters are initialized with non valid values
    setup=set;
    clear();
}
HMdc12FitSetup::~HMdc12FitSetup()
{
  // destructor HMdc12FitSetup
}
void HMdc12FitSetup::clear()
{
    // The parameters are initialized with non valid values
    fitauthor =-99;  // 0 Hejdar, 1 Alexander
    fitVersion=-99;  // 0 polynom 1 cal2parsim
    fitIndep  =-99;  // 0 segment 1 mdc
    fitNTuple =-99;    // kTRUE fill NTuple
    fitPrint  =-99;    // kTRUE print debug
}
Bool_t HMdc12FitSetup::check()
{
    // The parameters are checked for non valid values
    if(fitauthor ==-99) return kFALSE;
    if(fitVersion==-99) return kFALSE;
    if(fitIndep  ==-99) return kFALSE;
    if(fitNTuple ==-99) return kFALSE;
    if(fitPrint  ==-99) return kFALSE;
    return kTRUE;
}
void HMdc12FitSetup::printParam(void)
{
    // prints the parameters to the screen
    printf ("--------------------------------------------------------------------------------------------\n");
    printf ("HMdc12FitSetup:\n");
    printf ("FitAuthor       =  %i :  0 = Hejdar     , 1 = Alexander\n",                                fitauthor);
    printf ("DrTimeCalcVers  =  %i :  0 = polynom    , 1 = HMdCal2ParSim::calcTime\n",                  fitVersion);
    printf ("FitType         =  %i :  0 = segment fit, 1 = independent mdc fit\n",                      fitIndep);
    printf ("DebugCategorie  =  %i :  1 = create and fill catMdcClusFit&catMdcWireFit, 0 = don't cr.\n",fitNTuple);
    printf ("DebugPrint      =  %i :  1 = print debug information, 0 = don't print\n",                  fitPrint);
}
Bool_t HMdc12FitSetup::getParams(HParamList* l)
{
    // gets the parameters from HParamList, which is used for io.
    if(!(l->fill("fitAuthor", &fitauthor ))) return kFALSE;
    if(!(l->fill("fitVersion",&fitVersion))) return kFALSE;
    if(!(l->fill("fitIndep",  &fitIndep  ))) return kFALSE;
    if(!(l->fill("fitNTuple", &fitNTuple ))) return kFALSE;
    if(!(l->fill("fitPrint",  &fitPrint  ))) return kFALSE;
    return kTRUE;
}
void HMdc12FitSetup::putParams(HParamList* l)
{
    // puts the parameters of HMdc12FitSetup to
    // HParamList, which is used for io.
    l->add("fitAuthor", fitauthor);
    l->add("fitVersion",fitVersion);
    l->add("fitIndep",  fitIndep);
    l->add("fitNTuple", fitNTuple);
    l->add("fitPrint",  fitPrint);
}
void HMdc12FitSetup::setupMdc12Fit()
{
    HMdcTrackDSet::setMdc12Fit(fitauthor,fitVersion,fitIndep, (Bool_t)fitNTuple,(Bool_t)fitPrint);
}
//################################## COMMONSETUP #############################
HMdcCommonSetup::HMdcCommonSetup(const char* name="",const char* title="",HMdcSetup* set=0)
    : TNamed(name,title)
{
    // constructor of HMdcCommonSetup
    // The parameters are initialized with non valid values
    setup=set;
    clear();
}
HMdcCommonSetup::~HMdcCommonSetup()
{
  // destructor of HMdcCommonSetup
}
void HMdcCommonSetup::clear()
{
    // The parameters are initialized with non valid values
    isSimulation =-99;
    analysisLevel=-99;
    tracker      =-99;          // 1,2 1:dubna
}
Bool_t HMdcCommonSetup::check()
{
    // The parameters are are checked for non valid values
    if(isSimulation ==-99) return kFALSE;
    if(analysisLevel==-99) return kFALSE;
    if(tracker      ==-99) return kFALSE;
    return kTRUE;
}
void HMdcCommonSetup::printParam(void)
{
    // prints the parameters to the screen
    printf ("--------------------------------------------------------------------------------------------\n");
    printf ("HMdcCommonSetup:\n");
    printf ("isSimulation    =  %i :  0 = real data, 1 = simulation, 2 = merge\n",     isSimulation);
    printf ("analysisLevel   =  %i :  0 = raw, 1 = cal1, 2 = cal2, 3 = hit, 4 = fit\n",analysisLevel);
    printf ("tracker         =  %i :  1 = dubna, 2 = santiago\n",                      tracker);
}
Bool_t HMdcCommonSetup::getParams(HParamList* l)
{
    // gets the parameters from HParamList, which is used for io.
    if(!(l->fill("isSimulation", &isSimulation ))) return kFALSE;
    if(!(l->fill("analysisLevel",&analysisLevel))) return kFALSE;
    if(!(l->fill("tracker",      &tracker      ))) return kFALSE;
    return kTRUE;
}
void HMdcCommonSetup::putParams(HParamList* l)
{
    // puts the parameters of HMdcCommonSetup to
    // HParamList, which is used for io.
    l->add("isSimulation", isSimulation);
    l->add("analysisLevel",analysisLevel);
    l->add("tracker",      tracker);
}