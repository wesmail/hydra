#include "hconfigure.h"
#include "hades.h"
#include "htofdetector.h"
#include "hmdcdetector.h"
#include "hrichdetector.h"
#include "hshowerdetector.h"
#include "hruntimedb.h"

ClassImp(HConfigure)

Bool_t HConfigure::configure() {

  if(gHades) {
    delete gHades;
    gHades = 0;
  }
  if(!gHades) new Hades();
  if(!gHades) return kFALSE;

  return kTRUE;

}
/* Int_t i;

  //--- geometry

  HTofDetector *tof=new HTofDetector();
  Int_t tof_mods[22]={1,2,3,4,5,6,7,8};
  for(i=3;i<9;i++) {
    if(fConfig[i] == 1) tof->setModules(3-i,tof_mods);
  }
  gHades->getSetup()->addDetector(tof);

  HMdcDetector *mdc=new HMdcDetector();
  Int_t mdc_mods[4]={0,1,0,0}; 
  for(i=9;i<15;i++) {
    if(fConfig[i] == 1) mdc->setModules(9-i,mdc_mods);   
  }
  gHades->getSetup()->addDetector(mdc);
  
  HRichDetector *rich = new HRichDetector();
  Int_t rich_mods[1] = {1};
  for(i=15;i<21;i++) {
    if(fConfig[i] == 1) rich->setModules(16-i,rich_mods);   
  }
  gHades->getSetup()->addDetector(rich);

  
  HShowerDetector *shower = new HShowerDetector();
  Int_t sho_mods[3]={1,1,1}; 
  for(i=21;i<27;i++) {
    if(fConfig[i] == 1) shower->setModules(21-i,sho_mods);   
  }
  gHades->getSetup()->addDetector(shower);

  //--- runtime db

  HRuntimeDb *rtdb=gHades->getRuntimeDb();
  rtdb->addEventFile(input);

  //-- mdc

  HParAsciiFileIo *inp1=new HParAsciiFileIo;
  inp1->open("mdc_lookup.txt","in");
  gHades->getRuntimeDb()->setFirstInput(inp1);

  HMdcGeomStruct *geomstruct = new HMdcGeomStruct();
  rtdb->addContainer(geomstruct);
  geomstruct->init();

  HMdcCalPar *calpar = new HMdcCalPar();
  calpar->setStatic();
  rtdb->addContainer(calpar);

  HMdcRawStruct *rawstruct = new HMdcRawStruct();
  rtdb->addContainer(rawstruct);
  rawstruct->init();

  HMdcConnectedWires *connWires = new HMdcConnectedWires();
  rtdb->addContainer(connWires);

  HMdcLookupRaw *lookupMbo = new HMdcLookupRaw;
  rtdb->addContainer(lookupMbo);

  //--- reconstruction tree 
  
  HTaskSet *toftasks= new HTaskSet("Tof","Tof");
  toftasks->connect(new HTofHitF("tof.hitf","tof.hitf"));
  toftasks->connect(NULL, "tof.hitf");

  HTaskSet *shotasks= new HTaskSet("Shower","Shower");
  shotasks->connect(new HShowerCalibrater("shower.cal","shower.cal"));
  shotasks->connect(new HShowerHitFinder("shower.hitf","shower.hitf"),
		    "shower.cal");
  shotasks->connect(NULL, "shower.hitf");

  HTaskSet *mdctasks = new HTaskSet("Mdc","Mdc");
  mdctasks->connect(new HMdcCalibrater1("mdc.cal1","mdc.cal1"));
  mdctasks->connect(NULL,"mdc.cal1");
  
  HTaskSet *richtasks  = new HTaskSet("Rich","Rich");
//  richtasks->connect(new HRichCalibrater("RichCal","rich.cal"));
//  richtasks->connect(NULL, "RichCal");
  richtasks->connect(new HRichAnalysis("RichAnalysis","Rich hit finder"));
  richtasks->connect(NULL, "RichAnalysis");

  HTaskSet *onltask = new HTaskSet("Online","Online");
  onltasks->connect(new HOnlProcessEvent("online","online"));
  onltasks->connect(NULL,"online");

  gHades->getTaskSet("real")->connect(mdctasks);
  gHades->getTaskSet("real")->connect(toftasks,mdctasks);
  //  gHades->getTaskSet("real")->connect(shotasks,toftasks);
  //  gHades->getTaskSet("real")->connect(richtasks,shotasks);
  gHades->getTaskSet("real")->connect(onltasks,toftasks);

  // split level

  gHades->setSplitLevel(2);

  // data source

  if(fConfig[0] == 1) { // hld file

    HldFileSource *hldinput = new HldFileSource;
    //    hldinput->addUnpacker(new HShowerUnpackerReal);
    //    hldinput->addUnpacker(new HShowerUnpackerReal2);
    hldinput->addUnpacker(new HTofUnpacker);
    hldinput->addUnpacker(new HStartUnpacker);
    hldinput->addUnpacker(new HMdcUnpacker(513,kFALSE)); //November'99 beam
    //    hldinput->addUnpacker(new HRichUnpackerCal(100));
    //    hldinput->addUnpacker(new HRichUnpackerCal(101));
    gHades->setDataSource(hldinput);
    gHades->getRuntimeDb()->setCurrentEventFile(-1);

  }    

  if(fConfig[1] == 1) { // daq input

    HldRemoteSource *daqinput = new HldRemoteSource(input);
    //    daqinput->addUnpacker(new HShowerUnpackerReal);
    //    daqinput->addUnpacker(new HShowerUnpackerReal2);
    daqinput->addUnpacker(new HTofUnpacker);
    daqinput->addUnpacker(new HStartUnpacker);
    daqinput->addUnpacker(new HMdcUnpacker(513,kFALSE)); //November'99 beam
    //    daqinput->addUnpacker(new HRichUnpackerCal(100));
    //    daqinput->addUnpacker(new HRichUnpackerCal(101));
    gHades->setDataSource(daqinput);
    gHades->getRuntimeDb()->setCurrentEventFile(-1);
    
  }

  if(!gHades->init()) {
    kConfigured = kFALSE;
    return kFALSE;
  }

  //--- output

  //  gHades->getCurrentEvent()->getCategory(catShowerRaw)->setPersistency(0);
  gHades->setOutputFile(output,"RECREATE","Test",2);

  //--- build output tree

  gHades->makeTree();

  return kTRUE;

}
*/
  





