#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class HMdcDetector;
#pragma link C++ class HMdcTaskSet;
#pragma link C++ class HMdcContFact;

#pragma link C++ class HMdcParRootFileIo;
#pragma link C++ class HMdcParAsciiFileIo;

#pragma link C++ class HMdcGeomStruct-;
#pragma link C++ class HMdcGeomSecStru-;
#pragma link C++ class HMdcGeomModStru-;

#pragma link C++ class HMdcRawStruct;
#pragma link C++ class HMdcRawSecStru;
#pragma link C++ class HMdcRawModStru;
#pragma link C++ class HMdcRawMothStru;

#pragma link C++ class HMdcMboReadout;
#pragma link C++ class HMdcMboReadoutSec;
#pragma link C++ class HMdcMboReadoutMod;
#pragma link C++ class HMdcMboReadoutMoth;

#pragma link C++ class HMdcEvReadout;
#pragma link C++ class HMdcEvReadoutSam;
#pragma link C++ class HMdcEvReadoutRoc;
#pragma link C++ class HMdcEvReadoutBusPos;
#pragma link C++ class HMdcEvReadoutBus;

#pragma link C++ class HMdcCalPar;
#pragma link C++ class HMdcCalParRaw-;
#pragma link C++ class HMdcCalParSec;
#pragma link C++ class HMdcCalParRawSec;
#pragma link C++ class HMdcCalParRawMod;
#pragma link C++ class HMdcCalParMod;
#pragma link C++ class HMdcCalParMbo;
#pragma link C++ class HMdcCalParTdc;
#pragma link C++ class HMdcCal2Par;
#pragma link C++ class HMdcCal2ParSec;
#pragma link C++ class HMdcCal2ParMod;
#pragma link C++ class HMdcCal2ParAngle;
#pragma link C++ class HMdcCal2ParSim-;
#pragma link C++ class HMdcCal2ParSecSim;
#pragma link C++ class HMdcCal2ParModSim;
#pragma link C++ class HMdcCal2ParAngleSim;
#pragma link C++ class HMdcCellEff;
#pragma link C++ class HMdcCellEffMod;
#pragma link C++ class HMdcCellEffAngle;
#pragma link C++ class HMdcTdcThreshold;
#pragma link C++ class HMdcTdcThresholdSec;
#pragma link C++ class HMdcTdcThresholdMod;
#pragma link C++ class HMdcTdcThresholdMbo;
#pragma link C++ class HMdcTdcThresholdDbo;
#pragma link C++ class HMdcTimeCut;
#pragma link C++ class HMdcTimeCutSec;
#pragma link C++ class HMdcTimeCutMod;
#pragma link C++ class HMdcCutStat;
#pragma link C++ class HMdcCutStatSec;
#pragma link C++ class HMdcCutStatMod;

#pragma link C++ class HMdcSetup;
#pragma link C++ class HMdcDigitizerSetup;
#pragma link C++ class HMdcCalibrater1Setup;
#pragma link C++ class HMdcTrackFinderSetup;
#pragma link C++ class HMdcClusterToHitSetup;
#pragma link C++ class HMdc12FitSetup;
#pragma link C++ class HMdcCommonSetup;

#pragma link C++ class HMdcVertexFind;

#pragma link C++ class HMdcCalParLayer;
#pragma link C++ class HMdcCalParCell;

#pragma link C++ class HMdcLookupGeom;
#pragma link C++ class HMdcLookupGSec;
#pragma link C++ class HMdcLookupGMod;
#pragma link C++ class HMdcLookupMoth;
#pragma link C++ class HMdcLookupChan;


#pragma link C++ class HMdcLookupRaw;
#pragma link C++ class HMdcLookupRSec;
#pragma link C++ class HMdcLookupRMod;
#pragma link C++ class HMdcLookupLayer;
#pragma link C++ class HMdcLookupCell;

#pragma link C++ class HMdcLayerGeomPar;
#pragma link C++ class HMdcLayerGeomParSec;
#pragma link C++ class HMdcLayerGeomParMod;
#pragma link C++ class HMdcLayerGeomParLay;

#pragma link C++ class HMdcDigitPar;
#pragma link C++ class HMdcWireStat-;
#pragma link C++ class HMdcGeomPar;

#pragma link C++ class HMdcRaw;
#pragma link C++ class HMdcCal1;
#pragma link C++ class HMdcCal2;
#pragma link C++ class HMdcCellGroup;
#pragma link C++ class HMdcCellGroup6;
#pragma link C++ class HMdcCellGroup12;
#pragma link C++ class HMdcHit-;
#pragma link C++ class HMdcSeg-;
#pragma link C++ class HMdcTrk;
#pragma link C++ class HMdcTrkCand-;
#pragma link C++ class HMdcXY;

#pragma link C++ class HMdcUnpacker;
#pragma link C++ class HMdcDataWord;
#pragma link C++ class HMdcGeantReader;

#pragma link C++ class HMdcDigitizer;

#pragma link C++ class HMdcCalibrater1;
#pragma link C++ class HMdcCalibrater2;

#pragma link C++ class HMdcGeantCell;
#pragma link C++ class HMdcCal1Sim-;
#pragma link C++ class HMdcCal2Sim;
#pragma link C++ class HMdcHitSim;
#pragma link C++ class HMdcSegSim;

#pragma link C++ class HMdcCalibrater2Sim;

#pragma link C++ global catMdcRaw;
#pragma link C++ global catMdcHit;
#pragma link C++ global catMdcSeg;
#pragma link C++ global catMdcTrkCand;
#pragma link C++ global catMdcCal1;
#pragma link C++ global catMdcCal2;
#pragma link C++ global catMdcGeantCell;
#pragma link C++ class HMdcClosePairCutsParCut;
#pragma link C++ class HMdcClosePairCutsParEf;
#pragma link C++ class HMdcClosePairCutsParPhi;
#pragma link C++ class HMdcClosePairCutsParTheta;
#pragma link C++ class HMdcClosePairCutsPar;  

#pragma link C++ class HMdcRawEventHeader;
#pragma link C++ global catMdcRawEventHeader;
#pragma link C++ global catMdcDataWord;

#pragma link C++ class HMdcTdcChannel;
#pragma link C++ class HMdcTdcChannelSec;
#pragma link C++ class HMdcTdcChannelMod;
#pragma link C++ class HMdcTdcChannelMbo;
#pragma link C++ class HMdcTdcChannelTdc;
#pragma link C++ class HMdcConnectedWires;

#pragma link C++ class HMdcPPEvtFilter;

#endif






