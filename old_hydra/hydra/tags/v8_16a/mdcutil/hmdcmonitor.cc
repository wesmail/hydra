using namespace std;
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
// File: hmdcmonitor.cc
//
// Last update: 20/10/2000 A.Nekhaev
// Author: A.Nekhaev
//

#include "heventheader.h"
#include "hmdcunpacker.h"
#include "hmdcrawstruct.h"
#include <iostream> 
#include <iomanip>

//_HADES_CLASS_DESCRIPTION 
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


HMdcMonitor::HMdcMonitor(Text_t* name,Text_t* title, Int_t secnum, Int_t modnum) :
  HReconstructor(name,title)
{
  rawCat=0;
  iter=0;
  secNum = secnum;
  modNum = modnum;
}


HMdcMonitor::~HMdcMonitor(void) {
  //  mfile->Update();
  mfile->Close();
}

void HMdcMonitor::setParContainers() {
   raws=(HMdcRawStruct*)gHades->getRuntimeDb()->getContainer("MdcRawStruct");
}

Bool_t HMdcMonitor::init(void) {
  setParContainers();
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
  sprintf(mapfile,"%s","/tmp/mdcmon.map"); 
  mfile = TMapFile::Create(mapfile,"RECREATE", 50000000,"Memory mapped file with MDC-Raw-Data");
  
  Float_t xmin, xmax, ymin, ymax;
  Char_t title[25],tmp1[25],tmp2[25],tmp3[25],tmp4[25],tmp5[25] ;
  xmin = 0;
  xmax = 2048;
  ymin = 0;
  //  ymax = 600;
  ymax = 200;
  Int_t mboName[16]; // should be taken from runtime database!
    //  {111,112,121,122,123,124,211,212,221,222,223,224,311,312,321,322};
    //  {111,112,113,121,122,211,212,213,221,222,311,312,321,322,0,0};
  HMdcRawModStru &mod= (*raws)[secNum][modNum];
  for (j=0;j<mod.getNMotherboards();j++) {
    HMdcRawMothStru &moth=(*raws)[secNum][modNum][j];
    mboName[j] = atoi(moth.GetName());
    //    cout << " mboName[" << j << "] = " <<  mboName[j] << endl;
     }

  // Distinguish cal and real data

  for (j=0;j<16;j++) {     // loop over 16 MBO
    
    sprintf(title,"%s%i","MBO ",mboName[j]);
    sprintf(tmp1, "%s%i%s", "pHistTDCDT[",j,"]");
    sprintf(tmp2, "%s%i%s", "pHistTDC[",j,"]");
    sprintf(tmp3, "%s%i%s", "pHistTD[",j,"]");
    for (k=0;k<12;k++) {     // 8 TDC chips
      for (l=0;l<8;l++) {     // 8 TDC channels in one TDC chip
	sprintf(tmp4, "%s%i%s%i%s%i%s", "pHistT1[",j,"][",k,"][",l,"]");
	sprintf(tmp5, "%s%i%s%i%s%i%s", "pHistT2[",j,"][",k,"][",l,"]");
	pHistT1[j][k][l] = new TH1S(tmp4,title,2048,0.,2048);
	pHistT2[j][k][l] = new TH1S(tmp5,title,2048,0.,2048);
      }
    }
    pHistTDCDT[j] = 
      new TH2S(tmp1,title,128,xmin,xmax,200,ymin,ymax);
    //    new TH2S(tmp1,title,128,xmin,xmax,600,ymin,ymax);
    pHistTDC[j] = 
      new TH1S(tmp2,title,100,0,100);
    pHistTD[j] = 
      new TH2S(tmp3,title,128,0,2048,100,0,100);
  }
  pHistMult = new TH1S("pHistMult","Module1",250,0,250);
  return kTRUE;
}

Int_t HMdcMonitor::execute(void) {
  int mult = 0;
  //  Int_t id= ((HEventHeader*) gHades->getCurrentEvent()->getHeader())->getId();
  HMdcRaw* raw=0;
  iter->Reset();
  while ((raw=(HMdcRaw *)iter->Next())!=0) {
    if(raw->getSector()==secNum&&raw->getModule()==modNum) {
      pHistTDCDT[raw->getMbo()]->
	Fill(raw->getTime(1),raw->getTdc(),1.);
      pHistTDCDT[raw->getMbo()]->
	Fill(raw->getTime(2),raw->getTdc()+100,1.);
      pHistTDCDT[raw->getMbo()]->
	Fill(raw->getTime(3),raw->getTdc()+200,1.);
      pHistTDCDT[raw->getMbo()]->
	Fill(raw->getTime(4),raw->getTdc()+300,1.);
      pHistTDCDT[raw->getMbo()]->
	Fill(raw->getTime(5),raw->getTdc()+400,1.);
      pHistTDCDT[raw->getMbo()]->
	Fill(raw->getTime(6),raw->getTdc()+500,1.);
      pHistTD[raw->getMbo()]->
	Fill(raw->getTime(1)-raw->getTime(2),raw->getTdc(),1.);
	tdcChip = Int_t(raw->getTdc()/8);
	tdcChan = raw->getTdc() - tdcChip*8;
      pHistT1[raw->getMbo()][tdcChip][tdcChan]->
	Fill(raw->getTime(1),1.);
      pHistT2[raw->getMbo()][tdcChip][tdcChan]->
	Fill(raw->getTime(2),1.);
      pHistTDC[raw->getMbo()]->
	Fill(raw->getTdc(),1.);    
      //      Fill(raw->getTime(1)-raw->getTime(2),.1);
      mult++;
    }
  }
  pHistMult->Fill(mult);
  //  mfile->Update();
  evtNum++;
  //  if (!(evtNum % 100)) cout << " Event Number : " << evtNum << endl;
  if (!(evtNum % 100)) {
    cout << "File updated!!!" << endl;
    if(evtNum==100) showWin();
    mfile->Update();
  }
  return 1;
}

void HMdcMonitor::showWin(void) {
  Char_t tmp[200];
 sprintf(tmp,"%s%s%s","xv -wait 15 ",getenv("HADDIR"),"/mdcmon/param/up.gif&");
 cout << gSystem->Exec(tmp) << endl;
}


Bool_t HMdcMonitor::finalize(void) {
  cout << "File updated finaly!!!" << endl;
  mfile->Update();
  if(evtNum<100) {
    Char_t tmp[200];
    sprintf(tmp,"%s%s%s","xv -wait 15 ",getenv("HADDIR"),"/mdcmon/param/up.gif&");
    gSystem->Exec(tmp);
  }
  return kTRUE;
}
