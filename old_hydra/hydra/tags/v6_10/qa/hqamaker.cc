#include <iostream.h>
#include <math.h>
//---- Root includes
#include <TH2.h>
#include <TProfile.h>
//---- Hydra base includes
#include "hades.h"
#include "htree.h"
#include "hcategory.h"
#include "hevent.h"
#include "hiterator.h"
#include "hruntimedb.h" //asd
#include "hmessagemgr.h"
//---- Data container includes
#include "heventheader.h"
#include "hstartcal.h"
#include "hrichcal.h"
#include "hrichhit.h"
#include "hmdcraw.h"
#include "hmdccal1.h"
#include "hmdchit.h"
#include "hmdctimecut.h" //asd
#include "hmdccutstat.h" //asd
#include "htofhit.h"
#include "htofinocal.h"
#include "hshowerhittof.h"
#include "hkicktrack.h"
//---- Category definition includes
#include "hstartdef.h"
#include "richdef.h"
#include "hmdcdef.h"
#include "tofdef.h"
#include "tofinodef.h"
#include "showerdef.h"
#include "showertofinodef.h"
#include "kickdef.h"
//---- QA includes
#include "hqaoutputps.h"
#include "hqamaker.h"
#include "hqahistograms.h"
#include "hqavariations.h"
#include "hqascalers.h"

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-//
//*-- Author : Dan Magestro (magestro@gsi.de)
//*-- Modified : 26/04/2002 Joern Wuestenfeld
//*-- Modified : 24/01/2002
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-//

//////////////////////////////////////////////////////////////////////////////
//
// HQAMaker
//
// HQAMaker is a Hydra event-loop task which iterates over events and fills
// histograms and scalers, defined in separate classes.
//
// BEGIN_HTML<b><font color="maroon">For more information, see the <a href="http://www-hades.gsi.de/computing/qa/">QA home page</a>.</font></b> END_HTML
//
//////////////////////////////////////////////////////////////////////////////

Int_t HQAMaker::cutResults[4]={0,0,0,0};

ClassImp(HQAMaker)

HQAMaker::HQAMaker(void) {
// Default constructor
  nEvent = 0;
  hists = 0;
  varHists = 0;
  kFIRST = kTRUE;
  kPDF = kTRUE;

  samplingRate = 1;
  intervalSize = 1000;
  nInterval = 0;

  initIteratorPointers();
  cutStat=0;          //asd
  setParContainers(); //asd
}

HQAMaker::HQAMaker(Text_t *n,Text_t *t) : HReconstructor(n,t) {
// Constructor: Name, title
  nEvent = 0;
  hists = 0;
  varHists = 0;
  kFIRST = kTRUE;
  kPDF = kTRUE;

  samplingRate = 1;
  intervalSize = 1000;
  nInterval = 0;

  initIteratorPointers();
  cutStat=0;          //asd
  setParContainers(); //asd
}

HQAMaker::~HQAMaker(void) { 
  if (cutStat) HMdcCutStat::deleteHMdcCutStat(); 
}

void HQAMaker::initIteratorPointers() {
// Set iterator pointers to 0
  iterStCal = 0;
  iterRichCal = 0;
  iterRichHit = 0;
  iterMdcRaw = 0;
  iterMdcCal1 = 0;
  iterMdcHit = 0;
  iterShoHit = 0;
  iterShoHitTof = 0;
  iterTofHit = 0;
  iterTfnCal = 0;
  iterKickTrk = 0;
}

Bool_t HQAMaker::init(void) {
  // Create and book QA components, get iterator pointers if the categories
  // exist in current event

  hists    = new HQAHistograms();
  varHists = new HQAVariations(intervalSize);
  scalers  = new HQAScalers();

  hists->bookHist();
  varHists->bookHist();
  scalers->bookScalers();

  cutStat=HMdcCutStat::getObject();
  if (!cutStat) {
    ERROR_msg(DET_QA,"RETRIEVED 0 POINTER FOR HMDCCUTSTAT!");
    //Error("HQA:init()","RETRIEVED 0 POINTER FOR HMDCCUTSTAT!");
     exit(1);
  }

  return getIteratorPointers();
}

const void HQAMaker::Print(Option_t *) {
// Print box showing QA summary

  Int_t nHists=0, nVarHists=0;

  if(hists) nHists = hists->getHistList()->GetSize();
  if(varHists) nVarHists = varHists->getHistList()->GetSize();

  SEPERATOR_msg("-",70);
  INFO_msg(10,DET_QA,"HQAMaker: HADES data quality assessment");
  gHades->getMsg()->info(10,DET_QA,this->GetName(),"%-35s%s","QA statistics","Histogram summary");
  gHades->getMsg()->info(10,DET_QA,this->GetName(),"%-35s%s","-------------","-----------------");
  gHades->getMsg()->info(10,DET_QA,this->GetName(),"%-23s%-5i%9i%s","Event sampling rate: 1/",samplingRate,nHists," general hists");
  gHades->getMsg()->info(10,DET_QA,this->GetName(),"%-15s%-5i%5s%9i%s","Interval size:",intervalSize,"events  ",nVarHists," run var hists");
  gHades->getMsg()->info(10,DET_QA,this->GetName(),"%5i%-41s",intervalSize/samplingRate,"QA'd events/interval");
  gHades->getMsg()->info(10,DET_QA,this->GetName(),"%s%-8i","Max # events for var hists (=200*intervalSize): ",200*intervalSize);
  INFO_msg(10,DET_QA,"*extra events are added to overflow bins*");
  if(kPDF) {
    TString pdfFile = psFile(0,psFile.Last('.'))+".pdf";
    gHades->getMsg()->info(10,DET_QA,this->GetName(),"%s%-57s","PDF file:",(const char*)pdfFile);
    INFO_msg(10,DET_QA,"*ps2pdf will be used to generate pdf file*");
    TString txtFile = psFile(0,psFile.Last('.'))+".txt";
    gHades->getMsg()->info(10,DET_QA,this->GetName(),"%s%-57s","scalers:",(const char*)txtFile);
  }
  else
    gHades->getMsg()->info(10,DET_QA,this->GetName(),"%s%-57s","PS file:",(const char*)psFile);

  //  printf("\n");
  //  printf("    --------------- HQAMaker: HADES data quality assessment --------------- \n");
  //  printf("   |                                                                       |\n");
  //  printf("   |    QA statistics                             Histogram summary        |\n");
  // printf("   |    -------------                             -----------------        |\n");
  //  printf("   |    Event sampling rate: 1/%-5i            %5i general hists        |\n",samplingRate,nHists);
  //  printf("   |    Interval size:%5i events              %5i run var hists        |\n",intervalSize,nVarHists);
  //  printf("   |    %5i QA'd events/interval                                         |\n",intervalSize/samplingRate);
  //  printf("   |                                                                       |\n");
  //  printf("   |    Max # events for var hists (=200*intervalSize): %-8i           |\n",200*intervalSize);
  //  printf("   |      *extra events are added to overflow bins*                        |\n");
  //  printf("   |                                                                       |\n");
  //  if(kPDF) {
  //    TString pdfFile = psFile(0,psFile.Last('.'))+".pdf";
  //    printf("   |   PDF file: %-57s |\n",(const char*)pdfFile);
  //    printf("   |     *ps2pdf will be used to generate pdf file*                        |\n");
  //    TString txtFile = psFile(0,psFile.Last('.'))+".txt";
  //    printf("   |    scalers: %-57s |\n",(const char*)txtFile);
  //  }
  //  else
  //    printf("   |    PS file: %-57s |\n",(const char*)psFile);
  SEPERATOR_msg("-",70);

  //  printf("    ----------------------------------------------------------------------- \n");
}

Bool_t HQAMaker::getIteratorPointers() {
// Search current event for relevant categories... make an iterator
// if category exists

  HEvent *event = gHades->getCurrentEvent();
  if(!event) {
    ERROR_msg(DET_QA,"QA maker needs a Hydra event structure");
    //Error("init","QA maker needs a Hydra event structure");
    return kFALSE;
  }

  // Start
  HCategory *catStCal = event->getCategory(catStartCal);
  if (catStCal)
       iterStCal = (HIterator *)catStCal->MakeIterator("native");

  // Rich
  HCategory *catRicCal = event->getCategory(catRichCal);
  if (catRicCal)
       iterRichCal = (HIterator *)catRicCal->MakeIterator("native");
  HCategory *catRicHit = event->getCategory(catRichHit);
  if (catRicHit)
       iterRichHit = (HIterator *)catRicHit->MakeIterator("native");

  // Mdc
  HCategory *catMdRaw = event->getCategory(catMdcRaw);
  if (catMdRaw)
       iterMdcRaw = (HIterator *)catMdRaw->MakeIterator("native");
  HCategory *catMdCal1 = event->getCategory(catMdcCal1);
  if (catMdCal1)
       iterMdcCal1 = (HIterator *)catMdCal1->MakeIterator("native");
  HCategory *catMdHit = event->getCategory(catMdcHit);
  if (catMdHit)
       iterMdcHit = (HIterator *)catMdHit->MakeIterator("native");

  // Tof
  HCategory *catTfHit = event->getCategory(catTofHit);
  if (catTfHit)
       iterTofHit = (HIterator *)catTfHit->MakeIterator("native");

  // Tofino
  HCategory *catTfnCal = event->getCategory(catTofinoCal);
  if (catTfnCal)
       iterTfnCal = (HIterator *)catTfnCal->MakeIterator("native");

  // Shower
  HCategory *catShoHit = event->getCategory(catShowerHit);
  if (catShoHit)
       iterShoHit = (HIterator *)catShoHit->MakeIterator("native");
  HCategory *catShoHitTof = event->getCategory(catShowerHitTof);
  if (catShoHitTof)
       iterShoHitTof = (HIterator *)catShoHitTof->MakeIterator("native");

  // Kickplane
  HCategory *catKicTrk = event->getCategory(catKickTrack);
  if (catKicTrk)
       iterKickTrk = (HIterator *)catKicTrk->MakeIterator("native");

  return kTRUE;
}

void HQAMaker::setParContainers() { //asd
    // creates the parameter containers MdcLookupGeom/MdcLookupRaw/MdcTimeCut if they
    // exist and adds them to the list of parameter containers in the runtime
    // database
    //lookup   =(HMdcLookupGeom*)gHades->getRuntimeDb()->getContainer("MdcLookupGeom");
    //lookupRaw=(HMdcLookupRaw*) gHades->getRuntimeDb()->getContainer("MdcLookupRaw");
    timecut  =(HMdcTimeCut*)   gHades->getRuntimeDb()->getContainer("MdcTimeCut");
}



Int_t HQAMaker::execute() {
// Fill histograms for events with a frequency of 1/samplingRate

  if(kFIRST) {
     kFIRST = kFALSE;
     buildPSFilename();
     Print();
  }

  if(nEvent%intervalSize == 0 ) nInterval++;

  if( nEvent%samplingRate == 0 ) {

    Int_t eventSize = gHades->getCurrentEvent()->getHeader()->getEventSize();
    varHists->evtHeader_eventSize_Var->Fill( nEvent,eventSize );

    fillHistStart();
    fillHistRich();
    fillHistMdc();
    fillHistTof();
    fillHistTofino();
    fillHistShower();
    fillHistKick();

    nProcessed++;
  }
  nEvent++;

  return 0;
}

void HQAMaker::buildPSFilename() {
// Build Postscript file name, either from user-supplied name or from DST filename

     if(fDir.IsNull() && (!psFile.Contains("/")))
       WARNING_msg(DET_QA,10,"dir not specified, QA output written to present dir");
     //Warning("makePS","dir not specified, QA output written to present dir");

     // If psFile is not given, build PS filename from gHades filename
     dstFile = gHades->getOutputFile()->GetName();
     if(psFile.IsNull())
       psFile  = dstFile( dstFile.Last('/')+1,dstFile.Last('.')-dstFile.Last('/')-1 )+".ps";

     // Add "/" at end of directory
     if( !fDir.IsNull()) {
       if(!(fDir[fDir.Length()-1] == '/')) fDir += "/";
     }
     if(!psFile.Contains("/")) psFile = fDir + psFile;
}

Bool_t HQAMaker::finalize(void) {
// Finalize histograms and fill scalers

  fillScalers();

  finalizeHistStart();
  finalizeHistRich();
  finalizeHistMdc();
  finalizeHistTof();
  finalizeHistTofino();
  finalizeHistShower();
  finalizeHistKick();

  return makePS();
}

Bool_t HQAMaker::makePS() {
// After finalizing, generate PS file using HQAOutputPS

  HQAOutputPS *outPS = new HQAOutputPS(psFile);

  outPS->setDSTFileName(dstFile);
  outPS->setStats(nEvent,nProcessed);
  outPS->generatePDF(kPDF);

  outPS->makeHist( hists->getHistList());
  outPS->writeHist(hists->getHistList(), varHists->getHistList(), psFile ); //asd

  outPS->setNHistPerPage(3);
  outPS->makeHist(varHists->getHistList());

  outPS->makeText(scalers->getScalerList());
  outPS->saveScal(scalers->getScalerList(), psFile); //asd

  // outPS->write(varHists->getHistList());

  delete outPS;
  return kTRUE;
}

void HQAMaker::fillHistStart() {

  HStartCal *stCal = 0;

  if(iterStCal) {
    iterStCal->Reset();
    while((stCal=(HStartCal*) iterStCal->Next())!=0) {

      varHists->stCal_meanStrip_Var->Fill(nEvent,stCal->getStrip());

      hists->stCal_strip->Fill(stCal->getStrip());
      hists->stCal_tof->Fill(stCal->getTime());
    }
  }
}

void HQAMaker::fillHistRich() {

  HRichCal *richCal = 0;
  Int_t nDataObjs;

  if(iterRichCal) {
    iterRichCal->Reset();
    nDataObjs = ((TObjArray*)iterRichCal->GetCollection())->GetEntries();
    varHists->richCal_n_Var->Fill(nEvent,nDataObjs);
    while((richCal=(HRichCal*) iterRichCal->Next())!=0) {

      hists->richCal_row->Fill( richCal->getRow() );
      hists->richCal_column->Fill( richCal->getCol() );
      hists->richCal_nSec->Fill( richCal->getSector() );

    }
  }

  HRichHit *richHit = 0;
  Float_t theta,phi,r2d=57.29578;

  if(iterRichHit) {
    iterRichHit->Reset();
    nDataObjs = ((TObjArray*)iterRichHit->GetCollection())->GetEntries();
    varHists->richHit_n_Var->Fill(nEvent,nDataObjs);
    while((richHit=(HRichHit*) iterRichHit->Next())!=0) {

      theta = richHit->getTheta();
      phi = richHit->getPhi();

      hists->richHit_theta->Fill(theta);
      hists->richHit_phi->Fill(phi);
      hists->richHit_nSec->Fill( richHit->getSector() );

      hists->richHit_scat->Fill(sin(theta/r2d)*sin((phi-90)/r2d),sin(theta/r2d)*cos((phi-90)/r2d));

      hists->richHit_radius->Fill( richHit->getRadius() );
      hists->richHit_centroid->Fill( richHit->getCentroid() );
      hists->richHit_chargeAmpl->Fill( richHit->getRingAmplitude() );
      hists->richHit_ringCol->Fill( richHit->getRingCenterX() );
      hists->richHit_ringRow->Fill( richHit->getRingCenterY() );
      hists->richHit_ringLocMax4->Fill( richHit->getRingLocalMax4() );
      hists->richHit_houTraVsPatMat->Fill( richHit->getRingHouTra() , richHit->getRingPatMat() );
      hists->richHit_patMatVsTheta->Fill( richHit->getRingPatMat() , theta );
      hists->richHit_houTraVsTheta->Fill( richHit->getRingHouTra() , theta );
      hists->richHit_chargeAmplVsTheta->Fill( richHit->getRingAmplitude() , theta );
      hists->richHit_radiusVsTheta->Fill( richHit->getRadius() , theta );

    }
  }
}

void HQAMaker::fillHistMdc() {

  HMdcRaw *mdcRaw = 0;

  if(iterMdcRaw) {
    iterMdcRaw->Reset();
    while((mdcRaw=(HMdcRaw*) iterMdcRaw->Next())!=0) {

      Int_t sector = mdcRaw->getSector();
      Int_t module = mdcRaw->getModule();

      // mdcRaw: sector vs. mbo mult.
      Int_t mbo = mdcRaw->getMbo();
      if(module == 0) hists->mdcRaw_mboVsSector_m0->Fill(mbo,sector);
      else if(module == 1) hists->mdcRaw_mboVsSector_m1->Fill(mbo,sector);
      else if(module == 2) hists->mdcRaw_mboVsSector_m2->Fill(mbo,sector);
      else if(module == 3) hists->mdcRaw_mboVsSector_m3->Fill(mbo,sector);

    }
  }

  HMdcCal1 *mdcCal1 = 0;

  if(iterMdcCal1) {
    iterMdcCal1->Reset();
    Int_t n[4][6];
    for(Int_t i=0;i<4;i++) for(Int_t j=0;j<6;j++) n[i][j] = 0;
    while((mdcCal1=(HMdcCal1*) iterMdcCal1->Next())!=0) {

      Int_t sector = mdcCal1->getSector();
      Int_t module = mdcCal1->getModule();
      n[module][sector]++;

      Float_t time1 = mdcCal1->getTime1();
      Float_t time2 = mdcCal1->getTime2();

      // mdcCal1: t2-t1
      hists->mdcCal1_t2mt1_vs_t1[sector][module]->Fill(time2-time1, time1);
      hists->mdcCal1_t2mt1_vs_t1_plane[  module]->Fill(time2-time1, time1);

      // mdcCal1: sector vs. t1
      if(module == 0) hists->mdcCal1_time1VsSector_m0->Fill(time1,sector);
      else if(module == 1) hists->mdcCal1_time1VsSector_m1->Fill(time1,sector);
      else if(module == 2) hists->mdcCal1_time1VsSector_m2->Fill(time1,sector);
      else if(module == 3) hists->mdcCal1_time1VsSector_m3->Fill(time1,sector);

      if(module == 0) varHists->mdcCal1_time1_m0_Var->Fill(nEvent,time1);
      else if(module == 1) varHists->mdcCal1_time1_m1_Var->Fill(nEvent,time1);
      else if(module == 2) varHists->mdcCal1_time1_m2_Var->Fill(nEvent,time1);
      else if(module == 3) varHists->mdcCal1_time1_m3_Var->Fill(nEvent,time1);

      // mdcCal1: sector vs. t.a.t.
      Float_t tat = mdcCal1->getTime2()-mdcCal1->getTime1();
      if(module == 0) hists->mdcCal1_tatVsSector_m0->Fill(tat,sector);
      else if(module == 1) hists->mdcCal1_tatVsSector_m1->Fill(tat,sector);
      else if(module == 2) hists->mdcCal1_tatVsSector_m2->Fill(tat,sector);
      else if(module == 3) hists->mdcCal1_tatVsSector_m3->Fill(tat,sector);

    }
    for(Int_t sector=0;sector<6;sector++) {
      hists->mdcCal1_nVsSector_m0->Fill(n[0][sector],sector);
      hists->mdcCal1_nVsSector_m1->Fill(n[1][sector],sector);
      hists->mdcCal1_nVsSector_m2->Fill(n[2][sector],sector);
      hists->mdcCal1_nVsSector_m3->Fill(n[3][sector],sector);
    }
  }

  HMdcHit *mdcHit = 0;

  if(iterMdcHit) {
    iterMdcHit->Reset();
    while((mdcHit=(HMdcHit*) iterMdcHit->Next())!=0) {

      Int_t sector,module;
      Float_t x1,y1,x2,y2,angle;
      mdcHit->getSecMod(sector,module);

      // Scatter plots (made difficult by chamber coordinate system)
      x1 = mdcHit->getX();
      y1 = mdcHit->getY();

      if(module == 0) y1 = (y1 + 650)*.65;
      if(module == 1) y1 = (y1 + 950)*.58;
      if(module == 2) y1 = (y1 + 1800)*.58;
      if(module == 3) y1 = (y1 + 1900)*.68;

      angle = ((float) sector)*60./57.2967;
      x2 = x1*cos(angle) + y1*sin(angle);
      y2 = -x1*sin(angle) + y1*cos(angle);

      if(module == 0) hists->mdcHit_scat_m0->Fill(x2,y2);
      if(module == 1) hists->mdcHit_scat_m1->Fill(x2,y2);
      if(module == 2) hists->mdcHit_scat_m2->Fill(x2,y2);
      if(module == 3) hists->mdcHit_scat_m3->Fill(x2,y2);

    }
  }

}


//***---------------------------------------------------------


void HQAMaker::finalizeHistMdc(){
  Int_t c[7];
  char buf[500];

  DEBUG_msg(1,DET_QA,"MYOUTPUT----MYOUTPUT----MYOUTPUT----MYOUTPUT----");

  Int_t ib;
  for(Int_t m=0; m<4; m++) { //better to have first all sectors of the same module
     for(Int_t s=0; s<6; s++) {
          cutStat->getCal1StatCut(s,m,&c[0],&c[1],&c[2],&c[3],&c[4],&c[5],&c[6]);
          sprintf(buf,"%i %i %i %i %i %i %i %i %i \n",
          s,m,
          c[0],c[1],c[2],c[3],c[4],c[5],c[6] 
          );
	  DEBUG_msg(1,DET_QA,buf);

          ib = m*6+s+1;  //before I have an error here: ib = m*4+s+1; which caused lost of sector0 Dubna && Rossendorf for nov01-gen3 DST

          // 0   1    2     3     4          5        6 
          //t1  t2  t1t2  wire  t1+t12  cutall  allhits

          hists->mdcCutCounts[0]->SetBinContent(ib, c[6]); //allhits
          hists->mdcCutCounts[1]->SetBinContent(ib, c[0]); //t1
          hists->mdcCutCounts[2]->SetBinContent(ib, c[1]); //t2
          hists->mdcCutCounts[3]->SetBinContent(ib, c[2]); //t1t2
          hists->mdcCutCounts[4]->SetBinContent(ib, c[3]); //wire
          hists->mdcCutCounts[5]->SetBinContent(ib, c[4]); //t1+t12
          hists->mdcCutCounts[6]->SetBinContent(ib, c[5]); //cutall
     }
  }
  DEBUG_msg(1,DET_QA,buf);

  if (cutStat) cutStat->printParam();
}


void HQAMaker::fillHistTof() {

  HTofHit *tofHit = 0;
  Float_t x,y,z,theta,phi;
  Int_t nDataObjs;

  if(iterTofHit) {
    iterTofHit->Reset();
    nDataObjs = ((TObjArray*)iterTofHit->GetCollection())->GetEntries();
    hists->tofHit_n->Fill(nDataObjs);
    varHists->tofHit_n_Var->Fill(nEvent,nDataObjs);

    while((tofHit=(HTofHit*) iterTofHit->Next())!=0) {

      tofHit->getTheta(theta);
      tofHit->getPhi(phi);

      hists->tofHit_nSec->Fill( tofHit->getSector() );
      tofHit->getXYZLab(x,y,z);
      hists->tofHit_scat->Fill(-x,y);
      hists->tofHit_tof->Fill(tofHit->getTof());
      hists->tofHit_phi->Fill(phi);
      hists->tofHit_theta->Fill(theta);
    }
  }
}

void HQAMaker::fillHistTofino() {

  HTofinoCal *tfnCal = 0;

  if(iterTfnCal) {
    iterTfnCal->Reset();
    hists->tfnCal_n->Fill( ((TObjArray*)iterTfnCal->GetCollection())->GetEntries() );
    while((tfnCal=(HTofinoCal*) iterTfnCal->Next())!=0) {

      hists->tfnCal_tdc->Fill(tfnCal->getTime());

    }
  }
}

void HQAMaker::fillHistShower() {

  Float_t dummy1,phi,theta,charge,x,y,z;
  Int_t row,col,mod,sec;
  Int_t nDataObjs;

  HShowerHit *shoHit = 0;

  if(iterShoHit) {
    iterShoHit->Reset();
    nDataObjs = ((TObjArray*)iterShoHit->GetCollection())->GetEntries();
    varHists->shoHit_n_Var->Fill(nEvent,nDataObjs);

    while((shoHit=(HShowerHit*) iterShoHit->Next())!=0) {

      row = shoHit->getRow();
      col = shoHit->getCol();
      mod = shoHit->getModule();
      sec = shoHit->getSector();
      charge = shoHit->getCharge();

      hists->shoHit_nSec->Fill( sec );
      hists->shoHit_sectorVsModule->Fill( sec,mod );

      if(mod==0) hists->shoHit_chargeVsSector_m0->Fill(charge,sec);
      else if(mod==1) hists->shoHit_chargeVsSector_m1->Fill(charge,sec);
      else if(mod==2) hists->shoHit_chargeVsSector_m2->Fill(charge,sec);

      if(mod==0) hists->shoHit_rowVsSector_m0->Fill(row,sec);
      else if(mod==1) hists->shoHit_rowVsSector_m1->Fill(row,sec);
      else if(mod==2) hists->shoHit_rowVsSector_m2->Fill(row,sec);

      if(mod==0) hists->shoHit_colVsSector_m0->Fill(col,sec);
      else if(mod==1) hists->shoHit_colVsSector_m1->Fill(col,sec);
      else if(mod==2) hists->shoHit_colVsSector_m2->Fill(col,sec);

      shoHit->getSphereCoord(&dummy1,&phi,&theta);
      hists->shoHit_phi->Fill( phi );
      hists->shoHit_theta->Fill( theta );

      shoHit->getLabXYZ(&x,&y,&z);
      hists->shoHit_scat->Fill(-x,y);

    }
  }

  HShowerHitTof *shoHitTof = 0;

  if(iterShoHitTof) {
    iterShoHitTof->Reset();
    nDataObjs = ((TObjArray*)iterShoHitTof->GetCollection())->GetEntries();
    varHists->shoHitTof_n_Var->Fill(nEvent,nDataObjs);

    while((shoHitTof=(HShowerHitTof*) iterShoHitTof->Next())!=0) {

      sec = shoHitTof->getSector();
      hists->shoHitTof_nSec->Fill(sec);

      shoHitTof->getLabXYZ(&x,&y,&z);
      hists->shoHitTof_scat->Fill(-x,y);
    }
  }

}

void HQAMaker::fillHistKick() {

  Float_t mass,theta,phi,r2d=57.29578;
  Int_t charge,sec,nDataObjs;

  HKickTrack *kickTrk = 0;

  if(iterKickTrk) {
    iterKickTrk->Reset();
    nDataObjs = ((TObjArray*)iterKickTrk->GetCollection())->GetEntries();
    varHists->kickTrack_n_Var->Fill(nEvent,nDataObjs);

    while((kickTrk=(HKickTrack*) iterKickTrk->Next())!=0) {

      charge = kickTrk->getCharge();
      mass = sqrt(kickTrk->getMass());
      sec = kickTrk->getSector();

      hists->kickTrack_pullVsSector->Fill( kickTrk->getPull(),sec );
      hists->kickTrack_massCharge->Fill( mass*charge );

      theta = kickTrk->getTheta()*r2d;
      phi = kickTrk->getPhi()*r2d+60.*sec; if(phi>360.) phi-=360.;

      hists->kickTrack_scat->Fill(sin(theta/r2d)*sin((phi-90)/r2d),sin(theta/r2d)*cos((phi-90)/r2d));
      hists->kickTrack_nSec->Fill( kickTrk->getSector() );

    }
  }
}

void HQAMaker::finalizeHistTof() {
  if(hists->tofHit_nSec->GetEntries() > 0)
    {
      if(nProcessed == 0)
	hists->tofHit_nSec->Scale(1.);
      else
	hists->tofHit_nSec->Scale(1./nProcessed);
    }
}

void HQAMaker::finalizeHistShower() {
  if(hists->shoHit_nSec->GetEntries() > 0)
    {
      if(nProcessed == 0)
	hists->shoHit_nSec->Scale(1.);
      else
	hists->shoHit_nSec->Scale(1./nProcessed);
    }
  // hists->shoHit_avgCharge_m0->Divide(hists->shoHit_rowVsCol_m0);
}

void HQAMaker::fillScalers() {
// Fill scalar quantities with operations on QA histograms

  TH1D *proj = 0;

  // Sectorwise scalers
  for(Int_t sec=0; sec<6; sec++) {

    //---- Rich scalers
    if(hists->richCal_nSec->GetEntries() > 0)
       (*scalers->richCal_n).fData[sec] = (float) hists->richCal_nSec->GetBinContent(sec+1)/nProcessed;
    if(hists->richHit_nSec->GetEntries() > 0)
       (*scalers->richHit_n)[sec] = 1000. * (float) hists->richHit_nSec->GetBinContent(sec+1)/nProcessed;

    //---- Mdc scalers
    if(hists->mdcCal1_nVsSector_m0->GetEntries() > 0) {
       proj = hists->mdcCal1_nVsSector_m0->ProjectionX("proj",sec+1,sec+1);
       (*scalers->mdcCal1_n_m0)[sec] = proj->GetMean();
       delete proj;
    }
    if(hists->mdcCal1_nVsSector_m1->GetEntries() > 0) {
       proj = hists->mdcCal1_nVsSector_m1->ProjectionX("proj",sec+1,sec+1);
       (*scalers->mdcCal1_n_m1)[sec] = proj->GetMean();
       delete proj;
    }
    if(hists->mdcCal1_nVsSector_m2->GetEntries() > 0) {
       proj = hists->mdcCal1_nVsSector_m2->ProjectionX("proj",sec+1,sec+1);
       (*scalers->mdcCal1_n_m2)[sec] = proj->GetMean();
       delete proj;
    }
    if(hists->mdcCal1_nVsSector_m3->GetEntries() > 0) {
       proj = hists->mdcCal1_nVsSector_m3->ProjectionX("proj",sec+1,sec+1);
       (*scalers->mdcCal1_n_m3)[sec] = proj->GetMean();
       delete proj;
    }

    //---- Tof scalers
    if(hists->tofHit_nSec->GetEntries() > 0)
       (*scalers->tofHit_n)[sec] = (float) hists->tofHit_nSec->GetBinContent(sec+1)/nProcessed;

    // Shower scalers
    if(hists->shoHit_nSec->GetEntries() > 0)
       (*scalers->shoHit_n)[sec] = (float) hists->shoHit_nSec->GetBinContent(sec+1)/nProcessed;
    if(hists->shoHitTof_nSec->GetEntries() > 0)
       (*scalers->shoHitTof_n)[sec] = (float) hists->shoHitTof_nSec->GetBinContent(sec+1)/nProcessed;

    //---- Kick scalers
    if(hists->kickTrack_nSec->GetEntries() > 0)
       (*scalers->kickTrack_n)[sec] = (float) hists->kickTrack_nSec->GetBinContent(sec+1)/nProcessed;

  }

  // Detector-wise scalers

}











