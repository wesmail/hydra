#include "hmdcmonitor.h"
#include "hmdcdef.h"
#include "hdebug.h"
#include "hades.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hevent.h"
#include "hcategory.h"
#include "hlocation.h"
#include "hmdcraw.h"
#include "hmdccalpar.h"
#include "hmdclookupgeom.h"
#include "hmdcdigitpar.h"
#include "heventheader.h"
#include "hmdcunpacker.h"
#include <iostream.h>

////////////////////////////////
//HMdcMonitoring
//
// Storing histograms in TMapFile
// 
////////////////////////////////

Int_t HMdcMonitor::evtNum=0;
ClassImp(HMdcMonitor)

HMdcMonitor::HMdcMonitor(void) {
  mfile=0;
  rawCat=0;
  iter=0;
}

HMdcMonitor::HMdcMonitor(Text_t* name,Text_t* title) :
  HReconstructor(name,title)
{
  rawCat=0;
  iter=0;
}


HMdcMonitor::~HMdcMonitor(void) {
  //  mfile->Update();
  mfile->Close();
}

Bool_t HMdcMonitor::init(void) {
  rawCat=gHades->getCurrentEvent()->getCategory(catMdcRaw);
  if (!rawCat) {
    rawCat=gHades->getSetup()->getDetector("Mdc")->buildCategory(catMdcRaw);
    if (!rawCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catMdcRaw,rawCat,"Mdc");
  }
  iter=(HIterator *)rawCat->MakeIterator();
  loc.set(4,0,0,0,0);
  fActive=kTRUE;
  Char_t mapfile[50];
  sprintf(mapfile,"%s%s",getenv("HADDIR"),"/mdcmon/mdcmon.map");
  mfile = TMapFile::Create(mapfile,"RECREATE", 100000000,"Memory mapped file with MDC-Raw-Data");

  Float_t xmin, xmax, ymin, ymax;
  Char_t title[20],tmp1[20],tmp2[20],tmp3[20],tmp4[20],tmp5[20] ;
  xmin = 0;
  xmax = 2048;
  ymin = 0;
  ymax = 600;
  Int_t mboName[2][16] = // should be taken from runtime database!
  { {111,112,121,122,123,124,211,212,221,222,223,224,311,312,321,322},
    {111,112,121,122,123,124,211,212,221,222,223,224,311,312,321,322} };

  // Distinguish cal and real data


  for (i=0;i<2;i++) {        // 2 Modules
    for (j=0;j<16;j++) {     // 16 MBO

     sprintf(title,"%s%i","MBO ",mboName[i][j]);
     sprintf(tmp1, "%s%i%s%i%s", "pHistTDCDT[",i,"][",j,"]");
     sprintf(tmp2, "%s%i%s%i%s", "pHistTDC[",i,"][",j,"]");
     sprintf(tmp3, "%s%i%s%i%s", "pHistTD[",i,"][",j,"]");
     sprintf(tmp4, "%s%i%s%i%s", "pHistT1[",i,"][",j,"]");
     sprintf(tmp5, "%s%i%s%i%s", "pHistT2[",i,"][",j,"]");
     pHistTDCDT[i][j] = 
	new TH2S(tmp1,title,256,xmin,xmax,600,ymin,ymax);
      pHistTDC[i][j] = 
	new TH1S(tmp2,title,100,0,100);
      pHistTD[i][j] = 
	new TH2S(tmp3,title,256,0,2048,100,0,100);
      pHistT1[i][j] = 
	new TH1S(tmp4,title,2048,0.,2048);
      pHistT2[i][j] = 
	new TH1S(tmp5,title,2048,0.,2048);
   }
  }
  pHistMult[0] = new TH1S("pHistMult[0]","Module1",150,0,150);
  pHistMult[1] = new TH1S("pHistMult[1]","Module1",150,0,150);
  return kTRUE;
}

Int_t HMdcMonitor::execute(void) {
  int mult = 0;
  //  Int_t id= ((HEventHeader*) gHades->getCurrentEvent()->getHeader())->getId();
  HMdcRaw* raw=0;
  iter->Reset();
  while ((raw=(HMdcRaw *)iter->Next())!=0) {
    pHistTDCDT[raw->getModule()][raw->getMbo()]->
      Fill(raw->getTime(1),raw->getTdc(),1.);
    pHistTDCDT[raw->getModule()][raw->getMbo()]->
      Fill(raw->getTime(2),raw->getTdc()+100,1.);
    pHistTDCDT[raw->getModule()][raw->getMbo()]->
      Fill(raw->getTime(3),raw->getTdc()+200,1.);
    pHistTDCDT[raw->getModule()][raw->getMbo()]->
      Fill(raw->getTime(4),raw->getTdc()+300,1.);
    pHistTDCDT[raw->getModule()][raw->getMbo()]->
      Fill(raw->getTime(5),raw->getTdc()+400,1.);
    pHistTDCDT[raw->getModule()][raw->getMbo()]->
      Fill(raw->getTime(6),raw->getTdc()+500,1.);
    pHistTD[raw->getModule()][raw->getMbo()]->
      Fill(raw->getTime(1)-raw->getTime(2),raw->getTdc(),1.);
    pHistT1[raw->getModule()][raw->getMbo()]->
      Fill(raw->getTime(1),1.);
    pHistT2[raw->getModule()][raw->getMbo()]->
      Fill(raw->getTime(2),1.);
    pHistTDC[raw->getModule()][raw->getMbo()]->
      Fill(raw->getTdc(),1.);    
   //      Fill(raw->getTime(1)-raw->getTime(2),.1);
    if(raw->getModule()==1) mult++;
  }

  pHistMult[1]->Fill(mult);
  //  mfile->Update();
  evtNum++;
  //  if (!(evtNum % 100)) cout << " Event Number : " << evtNum << endl;
  if (!(evtNum % 100)) {
    cout << "File updated!!!" << endl;
    mfile->Update();
  }
  return 1;
}

