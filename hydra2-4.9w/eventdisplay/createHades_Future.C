#ifndef __CREATEHADES__
#define __CREATEHADES__


#include "hades.h"
#include "hruntimedb.h"
#include "hdatasource.h"
#include "hrootsource.h"
#include "hparasciifileio.h"
#include "hparrootfileio.h"
#include "htask.h"
#include "htaskset.h"
#include "hdst.h"
#include "hreconstructor.h"

#include "hspectrometer.h"
#include "hstart2detector.h"
#include "htboxdetector.h"
#include "hrichdetector.h"
#include "hmdcdetector.h"
#include "htofdetector.h"
#include "hshowerdetector.h"
#include "hrpcdetector.h"
#include "hwalldetector.h"

#include "hstarttaskset.h"
#include "hrichtaskset.h"
#include "hrich700taskset.h"
#include "hmdctaskset.h"
#include "htoftaskset.h"
#include "hshowertaskset.h"
#include "hemctaskset.h"
#include "hrpctaskset.h"
#include "hsplinetaskset.h"
#include "hwalltaskset.h"

#include "hparticlecandfiller.h"
#include "hparticletrackcleaner.h"
#include "hparticleevtinfofiller.h"
#include "hparticlet0reco.h"

#include "hmdcsetup.h"
#include "hmdclayercorrpar.h"
#include "hmdctrackdset.h"
#include "hmdc34clfinder.h"
#include "hmdcidealtracking.h"
#include "hmdcdigitizer.h"
#include "hrichdigitizer.h"
#include "hrich700digitizer.h"

#include "hstart2hitfsim.h"
#include "hmetamatchF2.h"

#include "hmdcsizescells.h"
#include "hmdcgetcontainers.h"
#include "hmagnetpar.h"

#include "hedhelpers.h"

#include "TOrdCollection.h"



Bool_t createHades()
{
    HEDTransform::setIsNewRich(kTRUE);    // switch between new/old rich for eventdisplay
    HEDTransform::setIsEmc(kTRUE);        // switch between shower/emc   for eventdisplay

    TString inFile  = "/misc/kempter/projects/proposal2017/hgeant/test_1.root";

    Int_t refId        = 14100;    // aug14; // params for aug14. GEOM MDC,TOF,RPC + params. Target+newRich+Emc+Wall geom from ascii
    TString beamtime = "apr12";
    Bool_t doExtendedFit     = kTRUE; // switch on/off fit for initial params of segment fitter (10 x slower!)
    Bool_t doStartCorrection = kTRUE;  // kTRUE (default)=  use run by run start corrections
    Bool_t doMetaMatch       = kFALSE;
    Bool_t doMetaMatchScale  = kTRUE;
    Bool_t useWireStat       = kTRUE;
    Float_t metaScale        = 1.5;
    Bool_t doMDCDeltaElectron = kTRUE;
    Bool_t doRICHDeltaElectron= kFALSE; // kFALSE : suppress inserted delta electrons
    Bool_t doReverseField     = kFALSE; // kTRUE : switch magnet polarity

    TString asciiParFile = "/misc/kempter/projects/proposal2017/dst/params/all_pars_ag1650ag_21042017.txt";
    TString rootParFile = "/u/hadesdst/GE/aug14/scripts/dst/allParam_AUG14_gen2_26022016_170632_refID_14100.root";
    TString paramSource  = "ascii,root"; // oracle, ascii, root

    cout<<"-----------------------------------------------------------------------------"<<endl;
    cout<<"EVENT DISPLAY : INPUT FILE: "<<inFile.Data()<<endl;
    cout<<"-----------------------------------------------------------------------------"<<endl;
    Hades* myHades = new Hades();
    gHades->setTreeBufferSize(8000);
    gHades->setQuietMode(2);
    gHades->makeCounter(1);
    HRuntimeDb* rtdb = gHades->getRuntimeDb();

    //------------- Operations on the filenames --------------------------
    TString rootSuffix =".root";
    TString nFile;    // needed to build outputfile
    TString dirname;  // needed for datasource
    TString filename; // needed for datasource
    TString outFile;

    Int_t sourcetype = 3; // root source

    if(inFile.Contains(",")){ // comma seperated list for geant merge source
	sourcetype = 4;
	inFile.ReplaceAll(" ","");
	TObjArray* ar = inFile.Tokenize(",");
	TString firstfile;
	if(ar){
	    if(ar->GetEntries()>0) {
		firstfile = ((TObjString*)ar->At(0))->GetString();
	    }
	    delete ar;
	}
	nFile     = gSystem->BaseName(firstfile.Data());
	filename  = inFile;
	dirname   = "";

    }  else {  // root source

	nFile     = gSystem->BaseName(inFile.Data());
	filename  = gSystem->BaseName(inFile.Data());
	dirname   = gSystem->DirName(inFile.Data());
    }
    //--------------------------------------------------------------------


    Int_t mdcMods[6][4]=
    { {1,1,1,1},
    {1,1,1,1},
    {1,1,1,1},
    {1,1,1,1},
    {1,1,1,1},
    {1,1,1,1} };

    // recommendations from Vladimir+Olga
    // according to params from 28.04.2011
    Int_t nLayers[6][4] = {
	{6,6,5,6},
	{6,6,5,6},
	{6,6,5,6},
	{6,6,5,6},
	{6,6,5,6},
	{6,6,5,6} };
    Int_t nLevel[4] = {10,50000,10,5000};

    HDst::setupSpectrometer(beamtime,mdcMods,"rich,mdc,tof,rpc,emc,wall");
    // beamtime mdcMods_apr12, mdcMods_full
    // Int_t mdcset[6][4] setup mdc. If not used put NULL (default).
    // if not NULL it will overwrite settings given by beamtime
    // detectors (default)= rich,mdc,tof,rpc,shower,wall,tbox,start

    HDst::setupParameterSources(paramSource,asciiParFile,rootParFile,"now");
    //HDst::setupParameterSources("oracle",asciiParFile,rootParFile,"now"); // use to create param file
    // parsource = oracle,ascii,root (order matters)
    // if source is "ascii" a ascii param file has to provided
    // if source is "root" a root param file has to provided
    // The histDate paramter (default "now") is used wit the oracle source

    HDst::setDataSource(sourcetype,dirname,filename, refId );
    // datasource 0 = hld, 1 = hldgrep 2 = hldremote, 3 root
    // like "lxhadeb02.gsi.de"  needed by dataosoure = 2
    // inputDir needed by dataosoure = 1,2
    // inputFile needed by dataosoure = 1,3




    // ----------- Build TASK SETS (using H***TaskSet::make) -------------

    HStartTaskSet        *startTaskSet        = new HStartTaskSet();
    HRich700TaskSet      *richTaskSet         = new HRich700TaskSet();
    HRpcTaskSet          *rpcTaskSet          = new HRpcTaskSet();
    HEmcTaskSet          *emcTaskSet          = new HEmcTaskSet();
    HTofTaskSet          *tofTaskSet          = new HTofTaskSet();
    HWallTaskSet         *wallTaskSet         = new HWallTaskSet();
    HMdcTaskSet          *mdcTaskSet          = new HMdcTaskSet();

    if(doReverseField){

	HMagnetPar* magnet = (HMagnetPar*)rtdb->getContainer("MagnetPar");
	rtdb->initContainers(refId);
	magnet->setStatic();
        Int_t current = magnet->getCurrent();
	magnet->setCurrent(-1*current);
        magnet->printParams();

    }


    HTask *startTasks         = startTaskSet       ->make("simulation");
    HTask *richTasks;
    if(!HEDTransform::isNewRich()){
	richTasks          = richTaskSet        ->make("simulation","noiseon");
	//richTasks          = richTaskSet        ->make("simulation","noiseoff");
    } else {
	richTasks          = richTaskSet        ->make("simulation");
    }
    HTask *tofTasks           = tofTaskSet         ->make("simulation");
    HTask *rpcTasks           = rpcTaskSet         ->make("simulation");
    HTask *wallTasks          = wallTaskSet        ->make("simulation");
    HTask *emcTasks           = emcTaskSet         ->make("simulation");


    //  ----------- MDC ------------------------------------------------------
    HMdcSetup* mysetup = (HMdcSetup*)rtdb->getContainer("MdcSetup");
    mysetup->getMdcCommonSet()->setAnalysisLevel(4);                // fit
    mysetup->getMdcCalibrater1Set()->setMdcCalibrater1Set(1, 1);    // 1 = NoStartandCal, 2 = StartandCal, 3 = NoStartandNoCal ::  0 = noTimeCut, 1 = TimeCut
    mysetup->getMdcTrackFinderSet()->setIsCoilOff(kFALSE);          // field is on
    mysetup->getMdcTrackFinderSet()->setNLayers(nLayers[0]);
    mysetup->getMdcTrackFinderSet()->setNLevel(nLevel);
    mysetup->getMdc12FitSet()->setMdc12FitSet(2,1,0,kFALSE,kFALSE); // tuned fitter, seg


    rtdb->initContainers(refId);
    mysetup->setStatic();
    //setup->getMdcCommonSet()->setAnalysisLevel(1);



    HTask *mdcTasks           = mdcTaskSet         ->make("rtdb","");
    HMdcIdealTracking *mdcidealtrackTasks       =  new HMdcIdealTracking("idealtrack","idealtrack");
    // mdcidealtrackTasks->fillParallelCategories();

    //----------------SPLINE and RUNGE TACKING----------------------------------------
    HSplineTaskSet         *splineTaskSet       = new HSplineTaskSet("","");
    HTask *splineTasks=splineTaskSet->make("","spline,runge");

    //-----------------------------------------------------------------------
    //HParticleCandFiller  *pParticleCandFiller = new HParticleCandFiller("particlecandfiller","particlecandfiller","debug,goodseg0,goodseg1,goodmeta,goodrk");
    HParticleCandFiller   *pParticleCandFiller = new HParticleCandFiller  ("particlecandfiller","particlecandfiller","");
    HParticleTrackCleaner *pParticleCleaner    = new HParticleTrackCleaner("particlecleaner"   ,"particlecleaner");
    HParticleEvtInfoFiller *pParticleEvtInfo   = new HParticleEvtInfoFiller("particleevtinfo"  ,"particleevtinfo",beamtime);

    HMdcDigitizer* digi = mdcTaskSet->getDigitizer();
    if(digi){
	digi->setDeltaElectronUse(doMDCDeltaElectron,kFALSE,109,-750.,600.,20.);
	digi->setDeltaElectronMinMomCut(2.,2.,4.5,2.,2.,4.5);  // take care of glass mirrors in sec 2+5
	digi->setTimeCutUse(kTRUE);
    }

    HRich700Digitizer* richdigi = HRich700Digitizer::getDigitizer();
    if(richdigi){
	richdigi->setDeltaElectronUse(doRICHDeltaElectron,kFALSE,109,20.,0.66); // 1 - prob  0.5 Mus RICH / 1.5 mus MDC
	richdigi->setDeltaElectronMinMomCut(0.,0.,0.,0.,0.,0.);
    }
    //------------------------ Master task set --------------------------

    HTaskSet *masterTaskSet = gHades->getTaskSet("simulation");

    masterTaskSet->add(startTasks);
    masterTaskSet->add(tofTasks);

    masterTaskSet->add(rpcTasks);

    masterTaskSet->add(richTasks);
    masterTaskSet->add(emcTasks);
    masterTaskSet->add(mdcTasks);
    masterTaskSet->add(wallTasks);
    //masterTaskSet->add(mdcidealtrackTasks);

    masterTaskSet->add(splineTasks);
    masterTaskSet->add(pParticleCandFiller);
    masterTaskSet->add(pParticleCleaner);
    masterTaskSet->add(pParticleEvtInfo);
    masterTaskSet->add(new HParticleT0Reco("T0","T0",beamtime));


    //--------------------------------------------------------------------
    // Get Parameter containers for transformations
    // This taks is needed to get the parameter
    // containers for coordinate transformations
    // of the detector hits for the event display
    masterTaskSet->add(new HEDMakeContainers());
    //--------------------------------------------------------------------

    masterTaskSet->isTimed(kTRUE);

    Bool_t hasTracking = kFALSE;

    HTaskSet* set = (HTaskSet*) gHades->getTaskSet("simulation")->getSetOfTask()->FindObject("Mdc");
    if(set && set->getSetOfTask()->FindObject("mdc.hitf")) hasTracking = kTRUE;

    if(!hasTracking){
        set = (HTaskSet*) gHades->getTaskSet("real")->getSetOfTask()->FindObject("Mdc");
        if(set && set->getSetOfTask()->FindObject("mdc.hitf")) hasTracking = kTRUE;
    }

    HStart2HitFSim* starthitf = HStart2HitFSim::getHitFinder() ;
    if(starthitf) starthitf->setResolution(0.06);    // 60 ps start res

    if(hasTracking){
	cout<<"EVENT DISPLAY : TRACKING IS RUNNING #########################################"<<endl;
	cout<<"EVENT DISPLAY : SETTING TRACKING PARAMETERS FOR "<<beamtime<<"############"<<endl;
	HMdcTrackDSet::setTrackParam(beamtime);
	//--------------------------------------------------------------------
	// find best initial params for segment fit (takes long!)
	if(!doMetaMatch)HMdcTrackDSet::setMetaMatchFlag(kFALSE,kFALSE);  //do not user meta match in clusterfinder
        if(doMetaMatchScale)HMetaMatchF2::setScaleCut(metaScale,metaScale,metaScale); // (tof,rpc,shower) increase matching window, but do not change normalization of MetaQA
	if(doExtendedFit) {
	    HMdcTrackDSet::setCalcInitialValue(1);  // -  1 : for all clusters 2 : for not fitted clusters
	}

	//--------------------------------------------------------------------
    }

    //--------------------------------------------------------------------
    if (!gHades->init())
    {
	cerr<<"EVENT DISPLAY : ERROR IN INIT, EXITING! #####################################"<<endl;
	delete myHades;
	return kFALSE;
    }
    //--------------------------------------------------------------------


    return kTRUE;
}

#endif

