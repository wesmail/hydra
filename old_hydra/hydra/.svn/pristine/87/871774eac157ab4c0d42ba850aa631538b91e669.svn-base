{
  // ROOT-macro to merge parameter files

// To merge the parameterfiles with root 3.02 and hydra 6
// Only edit the first section with the filenames and the directories! 
  
  Hades* myHades=new Hades;
  HRuntimeDb* rtdb=gHades->getRuntimeDb();
  HSpectrometer* spec=gHades->getSetup();

HRichDetector* rich=new HRichDetector;
Int_t richMods[6][1]={ {1},{1},{1},{1},{1},{1} };
for(Int_t i=0;i<6;i++) rich->setModules(i,richMods[i]);
spec->addDetector(rich);
TString richParamDir = "/u/hadesdst/nov01/gen2/params/detPar/";
TString richParamName = "richParNov01Day1339MT.root";


HMdcDetector* mdc=new HMdcDetector;
Int_t mdcMods[6][4]=
{ {1,1,1,1},
  {1,1,0,0},
    {1,1,1,0},
      {1,1,1,0},
        {1,1,0,0},
	  {1,1,1,0} };
for(Int_t i=0;i<6;i++) mdc->setModules(i,mdcMods[i]);
spec->addDetector(mdc);
TString mdcParamDir = "/u/hadesdst/nov01/gen2/params/detPar/";
TString mdcParamName = "mdcNov01_341_080202.root";

HTofDetector* tof=new HTofDetector;
Int_t tofMods[6][22]=
{ {1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	  {1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0} };
for(Int_t i=0;i<6;i++) tof->setModules(i,tofMods[i]);
spec->addDetector(tof);
TString tofParamDir = "/u/hadesdst/nov01/gen2/params/detPar/";
TString tofParamName = "tof_params_nov01_day340nofield_gen2.root";


HTofinoDetector* tofino=new HTofinoDetector;
Int_t tofinoMods[6][1]={ {1},{1},{1},{1},{1},{1} };
for(Int_t i=0;i<6;i++) tofino->setModules(i,tofinoMods[i]);
spec->addDetector(tofino);
TString tofinoParamDir = "/u/hadesdst/nov01/gen2/params/detPar/";
TString tofinoParamName = "tofinoParams_Nov01_150201.root";

HShowerDetector* shower=new HShowerDetector;
Int_t showerMods[6][3]=
{ {1,1,1},
  {1,1,1},
    {1,1,1},
      {1,1,1},
        {1,1,1},
	  {1,1,1} };
for(Int_t i=0;i<6;i++) shower->setModules(i,showerMods[i]);
spec->addDetector(shower);
TString showerParamDir = "/u/hadesdst/nov01/gen2/params/detPar/";
//TString showerParamName = "showerParams_08_Dec_01.root";
TString showerParamName = "showerParams_Nov01_150201.root";

HStartDetector* start=new HStartDetector;
Int_t startMods[2]={1,1};
start->setModules(-1,startMods);
spec->addDetector(start);
TString startParamDir = "/u/hadesdst/nov01/gen2/params/detPar/";
TString startParamName = "start_params_nov01_day339.root";

TString kickParamDir = "/u/hadesdst/nov01/gen2/params/detPar/";
TString kickParamName = "common.nov01.z-27.6.150202.root";

HParRootFileIo* inp1=new HParRootFileIo;
rtdb->setFirstInput(inp1);

HParRootFileIo* inp2=new HParRootFileIo;
rtdb->setSecondInput(inp2);

HParAsciiFileIo* inpascii=new HParAsciiFileIo;

HParRootFileIo* output=new HParRootFileIo;
TString outputDir = "/u/hadesdst/nov01/gen2/params/detPar/";
TString outputName = "allParamTest341.root";
output->open((outputDir+outputName).Data(),"RECREATE");
rtdb->setOutput(output);

////////////////////////////////  RICH  ////////////////////////////////////////

if (!inp1->open((richParamDir+richParamName).Data())) {
  printf("*** File %s not found ****\n",(richParamDir+richParamName).Data());
  return; }
// Entry date: 12-DEC-2001   Hydra version: nov2001
inp1->setInputNumber(1);

rtdb->getContainer("RichAnalysisParameters");
rtdb->getContainer("RichCalPar");
rtdb->getContainer("RichDigitisationParameters");
rtdb->getContainer("RichGeometryParameters");
rtdb->getContainer("RichMappingParameters");

if (!rtdb->initContainers(1007756140,1)) return;

rtdb->writeContainers();
rtdb->setContainersStatic();

/////////////////////////////////  MDC  ////////////////////////////////////////

if (!inp1->open((mdcParamDir+mdcParamName).Data())) {
  printf("*** File %s not found ****\n",(mdcParamDir+mdcParamName).Data());
  return; }
// Entry date: 03-DEC-2001   Hydra version: nov2001
inp1->setInputNumber(1);

rtdb->getContainer("MdcGeomStruct");
rtdb->getContainer("MdcRawStruct");

if (!rtdb->initContainers(1007756140,1)) return;

rtdb->getContainer("MdcCal2Par");
rtdb->getContainer("MdcCal2ParSim");
rtdb->getContainer("MdcCalParRaw");
rtdb->getContainer("MdcCellEff");
rtdb->getContainer("MdcDigitPar");
rtdb->getContainer("MdcGeomPar");
rtdb->getContainer("MdcLayerGeomPar");
rtdb->getContainer("MdcLookupGeom");
rtdb->getContainer("MdcMboReadout");
rtdb->getContainer("MdcTimeCut");

if (!rtdb->initContainers(1007756140,1)) return;

rtdb->writeContainers();
rtdb->setContainersStatic();

/////////////////////////////////  TOF  ////////////////////////////////////////
//preliminary file

//calibration made on the base of the first dsts

if (!inp1->open((tofParamDir+tofParamName).Data())) { 
  printf("*** File %s not found ****\n",(tofParamDir+tofParamName).Data());
  return; }
// Entry date: 11-DEC-2001   Hydra version: nov2001
inp1->setInputNumber(1);

rtdb->getContainer("TofCalPar");
//rtdb->getContainer("TofGeomPar");
rtdb->getContainer("TofLookup");
rtdb->getContainer("TofClusterFPar");

if (!rtdb->initContainers(1007756140,1)) return;

rtdb->writeContainers();
rtdb->setContainersStatic();

//////////////////////////////  TOFINO  ////////////////////////////////////////

if (!inp1->open((tofinoParamDir+tofinoParamName).Data())) {
  printf("*** File %s not found ****\n",(tofinoParamDir+tofinoParamName).Data());
  return; }
// Entry date: 09-NOV-2001   Hydra version: nov2001
inp1->setInputNumber(1);

rtdb->getContainer("TofinoCalPar");
rtdb->getContainer("TofinoGeomPar");
rtdb->getContainer("TofinoLookup");
rtdb->getContainer("TofinoShowerMap");

if (!rtdb->initContainers(1007756140,1)) return;

rtdb->writeContainers();
rtdb->setContainersStatic();

//////////////////////////////  SHOWER  ////////////////////////////////////////

if (!inp1->open((showerParamDir+showerParamName).Data())) {
  printf("*** File %s not found ****\n",(showerParamDir+showerParamName).Data());
  return; }
// Entry date: 08-DEC-2001   Hydra version: v5.42
inp1->setInputNumber(1);

rtdb->getContainer("ShowerCalPar");
rtdb->getContainer("ShowerGeometry");
rtdb->getContainer("ShowerHitFinderParams");
rtdb->getContainer("ShowerUnpackParam");

if (!rtdb->initContainers(1007756140,1)) return;

rtdb->writeContainers();
rtdb->setContainersStatic();

///////////////////////////////  START  ////////////////////////////////////////

if (!inp1->open((startParamDir+startParamName).Data())) {
  printf("*** File %s not found ****\n",(startParamDir+startParamName).Data());
  return; }
// Entry date: 05-DEC-2001   Hydra version: nov2001
inp1->setInputNumber(1);

rtdb->getContainer("StartCalPar");
rtdb->getContainer("StartLookup");
rtdb->getContainer("StartSelfCoPar");

if (!rtdb->initContainers(1007756140,1)) return;

rtdb->writeContainers();
rtdb->setContainersStatic();

//////////////////////////////  COMMON  ////////////////////////////////////////
                  
if (!inp1->open((kickParamDir+kickParamName).Data())) {
  printf("*** File %s not found ****\n",(kickParamDir+kickParamName).Data());
  return; }
// Entry date: 07-DEC-2001   Hydra version: nov2001
inp1->setInputNumber(1);

HKickPlane *kickmdc3=HKickPlane::getMDC3();
HKickPlane *kickmeta=HKickPlane::getMeta();
rtdb->getContainer("KickTrackFPar");
//rtdb->getContainer("SpecGeomPar");

if (!rtdb->initContainers(1007756140,1)) return;

rtdb->writeContainers();
rtdb->setContainersStatic();

////////////////////////////////////////////////////////////////////////////////

rtdb->removeRun("1");

rtdb->saveOutput();
rtdb->print();
delete myHades;
}
