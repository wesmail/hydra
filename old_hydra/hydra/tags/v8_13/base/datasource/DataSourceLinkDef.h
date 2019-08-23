#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class HldSource;
#pragma link C++ class HDataSource;
#pragma link C++ class HRootSource;

#ifdef RFIO_SUPPORT
#pragma link C++ class HldRFIOFileSource;
#pragma link C++ class HldRFIOFilEvt;
#pragma link C++ class HRFIORootSource;
#pragma link C++ class HRFIOFile;
#endif

#pragma link C++ class HldFileSourceBase;
#pragma link C++ class HldFileDesc;
#pragma link C++ class HldFileSource;
#pragma link C++ class HldGrepFileSource;
#pragma link C++ class HldRemoteSource;
#pragma link C++ class HldFileOutput;
#pragma link C++ class HGeantSource;
#pragma link C++ class HDirectSource;

//#pragma link C++ class HldBase;
//#pragma link C++ class HldEvt;
//#pragma link C++ class HldRemotEvt;
//#pragma link C++ class HldFilEvt;
#pragma link C++ class HldUnpack;

#pragma link C++ class HTrbLookupChan;
#pragma link C++ class HTrbLookupBoard;
#pragma link C++ class HTrbLookup;

#pragma link C++ class HTrbBaseUnpacker;

#pragma link C++ class HGeantReader;
#pragma link C++ class HKineGeantReader;
#pragma link C++ class HSimulGeantReader;

#endif

