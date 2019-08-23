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

  mfile = TMapFile::Create("mdcmonit.map","RECREATE", 100000000,"Memory mapped file with MDC-Raw-Data");

  char *hist[2][16]={ {"pHist[0][0]","pHist[0][1]","pHist[0][2]","pHist[0][3]",
		     "pHist[0][4]","pHist[0][5]","pHist[0][6]","pHist[0][7]",
		     "pHist[0][8]","pHist[0][9]","pHist[0][10]","pHist[0][11]",
		     "pHist[0][12]","pHist[0][13]","pHist[0][14]","pHist[0][15]"},
		     {"pHist[1][0]","pHist[1][1]","pHist[1][2]","pHist[1][3]",
		     "pHist[1][4]","pHist[1][5]","pHist[1][6]","pHist[1][7]",
		     "pHist[1][8]","pHist[1][9]","pHist[1][10]","pHist[1][11]",
		     "pHist[1][12]","pHist[1][13]","pHist[1][14]","pHist[1][15]"}};
  
  char *hist_TDC[2][16]={ {"pHist_TDC[0][0]","pHist_TDC[0][1]","pHist_TDC[0][2]",
			   "pHist_TDC[0][3]","pHist_TDC[0][4]","pHist_TDC[0][5]",
			   "pHist_TDC[0][6]","pHist_TDC[0][7]","pHist_TDC[0][8]",
			   "pHist_TDC[0][9]","pHist_TDC[0][10]","pHist_TDC[0][11]",
			   "pHist_TDC[0][12]","pHist_TDC[0][13]","pHist_TDC[0][14]",
			   "pHist_TDC[0][15]"},{"pHist_TDC[1][0]","pHist_TDC[1][1]",
			   "pHist_TDC[1][2]","pHist_TDC[1][3]","pHist_TDC[1][4]",
			   "pHist_TDC[1][5]","pHist_TDC[1][6]","pHist_TDC[1][7]",
		     "pHist_TDC[1][8]","pHist_TDC[1][9]","pHist_TDC[1][10]","pHist_TDC[1][11]",
		     "pHist_TDC[1][12]","pHist_TDC[1][13]","pHist_TDC[1][14]","pHist_TDC[1][15]"}};
  
  char *hist_name[2][16]={{"MBO 111","MBO 112","MBO 121","MBO 122","MBO 123",
			  "MBO 124","MBO 211","MBO 212","MBO 221","MBO 222",
			  "MBO 223","MBO 224","MBO 311","MBO 312","MBO 321",
			  "MBO 322"},{"MBO 111","MBO 112","MBO 121","MBO 122",
			  "MBO 123","MBO 124","MBO 211","MBO 212","MBO 221",
			  "MBO 222","MBO 223","MBO 224","MBO 311","MBO 312",
			  "MBO 321","MBO 322"}};
  char *hist_timeDiff[2][16]={{"pHistTimeDiff[0][0]","pHistTimeDiff[0][1]",
			      "pHistTimeDiff[0][2]","pHistTimeDiff[0][3]",
			      "pHistTimeDiff[0][4]","pHistTimeDiff[0][5]",
			      "pHistTimeDiff[0][6]","pHistTimeDiff[0][7]",
			      "pHistTimeDiff[0][8]","pHistTimeDiff[0][9]",
			      "pHistTimeDiff[0][10]","pHistTimeDiff[0][11]",
			      "pHistTimeDiff[0][12]","pHistTimeDiff[0][13]",
			      "pHistTimeDiff[0][14]","pHistTimeDiff[0][15]"},
			      {"pHistTimeDiff[1][0]","pHistTimeDiff[1][1]",
			      "pHistTimeDiff[1][2]","pHistTimeDiff[1][3]",
			      "pHistTimeDiff[1][4]","pHistTimeDiff[1][5]",
			      "pHistTimeDiff[1][6]","pHistTimeDiff[1][7]",
			      "pHistTimeDiff[1][8]","pHistTimeDiff[1][9]",
			      "pHistTimeDiff[1][10]","pHistTimeDiff[1][11]",
			      "pHistTimeDiff[1][12]","pHistTimeDiff[1][13]",
			      "pHistTimeDiff[1][14]","pHistTimeDiff[1][15]"}};
 

  Float_t xmin, xmax, ymin, ymax;
  xmin = 0.;
  xmax = 2500.;
  ymin = -10.;
  ymax = 200.;

  for (j=0;j<2;j++) {
   for (i=0;i<16;i++) {

     pHist[j][i] = 
	new TH2F(hist[j][i],hist_name[j][i],500,xmin,xmax,70,ymin,ymax);
      pHistTimeDiff[j][i] = 
	new TH1F(hist_timeDiff[j][i],hist_name[j][i],500,-2500.,2500.);
      pHistTDC[j][i] = 
	new TH1F(hist_TDC[j][i],hist_name[j][i],600,-10.,100.);
   }
  }
  pHistMult[0] = new TH1F("pHistMult[0]","Module1",150,0,150);
  pHistMult[1] = new TH1F("pHistMult[1]","Module1",150,0,150);
  return kTRUE;
}

Int_t HMdcMonitor::execute(void) {
  int mult = 0;
  HMdcRaw* raw=0;
  iter->Reset();
  while ((raw=(HMdcRaw *)iter->Next())!=0) {
    pHist[raw->getModule()][raw->getMbo()]->
      Fill(raw->getTime(1),raw->getTdc(),1.);
    pHist[raw->getModule()][raw->getMbo()]->
      Fill(raw->getTime(2),raw->getTdc()+100,1.);
    pHistTimeDiff[raw->getModule()][raw->getMbo()]->
      Fill(raw->getTime(1)-raw->getTime(2),.1);
    pHistTDC[raw->getModule()][raw->getMbo()]->
      Fill(raw->getTdc(),1.);    
    //      Fill(raw->getTime(1)-raw->getTime(2),.1);
    if(raw->getModule()==1) mult++;
  }
  pHistMult[1]->Fill(mult);
  mfile->Update();
  
  //  if (!(evtNum % 50)) {
  //    cout << "File updated!!!" << endl;
  //    mfile->Update();
  //  }
  return 1;
}

