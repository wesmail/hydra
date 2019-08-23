//====================================================================
Char_t *file="m00_hv0000_II3_3105_1755.hld";
 //----------Another way: 
 // root [0] .x mdcTrackD.C("file_name");

Int_t mdcUnpacker=513;
//Int_t mdcUnpacker=6;
//====================================================================
Char_t *lookup="mdc_lookup.txt"; 
//====================================================================
// Float_t cutLeft=27.;
// Float_t cutRight=300.;

// Cut for drift time:
 Float_t cutTime1L=0.;
 Float_t cutTime1R=0.;
 Float_t cutTime2L=0.;
 Float_t cutTime2R=0.;
 Float_t cutLeft=0.;       // cutLeft < driftTime1-driftTime2 < cutRight
 Float_t cutRight=0.;      // If cutLeft=cutRight=0. the cut will be ignore
//====================================================================


//------------------------------------------------
 Int_t level4=5.;         
 Int_t level5=50.;
// Int_t level4=0.;
// Int_t level5=100000.;
//------------------------------------------------

void mdcTrackD(Char_t* fileI=file){
#include <stdio.h>
  Hades* myHades = new Hades();
  Int_t splitLevel=2;
  gHades->setSplitLevel(splitLevel);  

  HSpectrometer* spec=gHades->getSetup();
  HMdcDetector* mdc=new HMdcDetector();
  Int_t mod[4]={0,1,0,0};
//  for(Int_t i=0;i<6;i++) mdc.setModules(i,mod);
  mdc->setModules(2,mod);
//  mdc.setModules(5,mod);
  spec->addDetector(mdc);

  HRuntimeDb* rtdb=gHades->getRuntimeDb();
  HRun* run=rtdb->addRun(944451190);

  //Set the data source (here from Lmd file)
  HldFileSource *source=new HldFileSource;
//  source->addUnpacker(new HMdcUnpacker(6,kTRUE));
  source->addUnpacker(new HMdcUnpacker(mdcUnpacker,kFALSE));
//  source->addUnpacker(new HStartUnpacker);
  gHades->setDataSource(source);
  
  source->setDirectory("/d/hades/mdc/data"); 
  source->addFile(fileI);

// Remark: At the moment it is not advised to add the paths in the name of
//         the event files in the runtime database

  HParAsciiFileIo *inp1=new HParAsciiFileIo;
//  inp1->open("D_mdc_lookup.txt","in");
  inp1->open(lookup,"in");
  gHades->getRuntimeDb()->setFirstInput(inp1); 

  HMdcGeomStruct *geomstruct=new HMdcGeomStruct();
  rtdb->addContainer(geomstruct);
  geomstruct.init();
  
  HMdcRawStruct *rawstruct = new HMdcRawStruct();
  rtdb->addContainer(rawstruct);
  rawstruct.init();
  
  HMdcCalParRaw *calparraw = new HMdcCalParRaw();
  rtdb->addContainer(calparraw);
  calparraw->init();
  calparraw->setStatic();
  
  HMdcConnectedWires *connWires = new HMdcConnectedWires();
  rtdb->addContainer(connWires);
  
  HMdcLookupRaw *lookupMbo = new HMdcLookupRaw();
  rtdb->addContainer(lookupMbo);
      
  HMdcLayerGeomPar *layergeom = new HMdcLayerGeomPar();
  rtdb->addContainer(layergeom);
  if (layergeom.init()==kFALSE) return; 
  
  HMdcGeomPar* geom=(HMdcGeomPar*)(gHades->getSetup()->createDetGeomPar("Mdc"));
  geom->init(); 
  
  HSpecGeomPar* specGeom=(HSpecGeomPar*)(gHades->getSetup()->createSpecGeomPar());
  specGeom->init();
  
  HMdcTimeCut* fcut=new HMdcTimeCut(cutTime1L,cutTime1R,cutTime2L,cutTime2R,
                                    cutLeft,cutRight);

  rtdb->addContainer(fcut);
//  HMdcMboReadout readout;
//  rtdb->addContainer(&readout);

  HMdcTrackFinder* trackFinder;  
  HTaskSet* tasks = new HTaskSet("MDC","Mdc Track Finding Display");
  tasks->connect(new HMdcCalibrater1("MdcCali1","MdcCali1"));
  tasks->connect((trackFinder=new HMdcTrackFinder("MdcTrackD","MdcTrackD")),"MdcCali1");
  tasks->connect((beamTest=new HMdcBeamTest("MdcTDbeam","MdcTDbeam")),"MdcTrackD");
  tasks->connect(NULL,"MdcTDbeam");
  
  gHades->getTaskSet("real")->connect(tasks); 
  gHades->getTaskSet("real")->print();

//  gHades->getRuntimeDb()->setCurrentEventFile(-1);
//  printf("gHades->init()\n");

  if (!gHades->init()) {
    rtdb->closeFirstInput();
    rtdb->closeSecondInput();
    return;
  }




// gHades->setOutputFile("test.root","RECREATE","Test",2); 

//  gHades->makeTree();

//  rtdb->closeFirstInput();
//  rtdb->closeSecondInput();
  
//=========================================================
  trackFinder->setLevel(level4, level5);
//=========================================================
  
//==================================================================
  for(Int_t i=0; i<20; i++) {drawCanv[i]=0; canv[i]=0;}
//  drawCanv[0]=1;
  doEvents(1);
  setPlots(1,0);

  TControlBar *menu = new TControlBar("vertical","MDC Event Display",800,650);
  menu->AddButton("    1 Event  : Draw  ","doEvents(1)","Do 1 event");
  menu->AddButton("   10 Events : Draw ","doEvents(10)","Do 10 events"); 
  menu->AddButton("  100 Events : Draw ","doEvents(100)","Do 10 events");
  menu->AddButton(" 1000 Events : Draw ","doEvents(1000)","Do 10 events");
  menu->AddButton("  All events ","doEvents(-1)","Do all events"); 
  menu->AddButton("","","D");
  menu->AddButton("Plot: Sec. 3"               ,"setPlots(1,0)","Display plot Sec.3");
  menu->AddButton("Plot: All clusters"         ,"setPlots(2,0)","Allplots");
  menu->AddButton("Hist.: nClusters(Theta,Phi)","setPlots(3,0)","nHits");
  menu->AddButton("Hist.: nHits(N.wire)"       ,"setPlots(4,0)","nHits(nWire)");
  menu->AddButton("Hist.: drift time 1,2,1-2"  ,"setPlots(5,0)","times");
  menu->AddButton("Hist.: dX, dY"  ,"setPlots(6,0)","dXdY");
  menu->AddButton("","","D");  
  menu->AddButton("Print clusters","beamTest->printClusters(sec)","Display plot Sec.3");
  menu->AddButton("Write current plots and hist. to root file","setPlots(-1,1)","Display plot Sec.3");
  menu->AddButton("Quit",".q","Quit Root");
  gROOT->SaveContext();
  menu->Show();  
 
//  gHades->eventLoop(1);

//  delete gHades;  // this takes very long because of ~TGeometry()

} 

//--------------------------------------------------------

Int_t indFT=0;
Int_t nHist=6;
HMdcBeamTest *beamTest;
Int_t sec=2;
Int_t drawCanv[20];
Int_t setStat[20]={0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
TCanvas* canv[20];
TObjArray* hists=new TObjArray(20);
TFile *fh;
char* filename="mdcTrackD.root";
Int_t nTotEv=0;
//--------------------------------------------------

void doEvents(Int_t nEv) {
  Int_t nEvDo=(nEv/10==0) ? 1 : nEv/10;
  Int_t nfor=nEv/nEvDo;
  if(nEv==-1) {nEvDo=10000; nfor=1;}
  Int_t evN;
  for(Int_t nTm=0; nTm<nfor; nTm++) {
    evN=gHades->eventLoop(nEvDo);
    setPlots(-1,0);
    if(evN<nEvDo) break;
    if(evN<=0) break;
    if(nEv==-1) nTm=-1;
    if(evN>0) nTotEv+=evN;
  }
  if(evN > 0) printf("======== Total num. of events processed: %i ========\n",nTotEv);
  else if(evN < 0) printf("======== The error of reading event. evN=%i\n",evN);
  if(evN=>0 && evN<nEvDo) printf(
   "======== End of file! Total num. of events processed: %i ========\n",nTotEv);
}

void setPlots(Int_t npl, Int_t iprn) {
 Int_t n1=0;
 Int_t n2=20;
 if(iprn==1) {
  if(indFT==0) {indFT=1; fh=new TFile(filename,"RECREATE");}
 }
 if(npl>=0) {
  drawCanv[npl-1]=abs(drawCanv[npl-1]-1);
  initCanv(npl-1,drawCanv[npl-1]);
  if(drawCanv[npl-1]==0) return;
  n1=npl-1;
  n2=npl;
 }
 for(Int_t i=n1; i<n2; i++) {
  if(drawCanv[i]==1) {
   gStyle->SetOptStat(setStat[i]);
   
   if (i==0) {
    canv[0]->cd(0);
    canv[0]->cd(1);
    TH2C *plot=beamTest->getPlot(1);
    Int_t pal[6] = {33,34,38,5,6,3};
//    gStyle->SetPalette(19,0);
    if(plot) {
     UInt_t eSN=((HEventHeader*) gHades->getCurrentEvent()->getHeader())->getEventSeqNumber();
     UInt_t eD= ((HEventHeader*) gHades->getCurrentEvent()->getHeader())->getDate();
     UChar_t* ed;
     ed=(UChar_t*) &eD;
     UInt_t eT= ((HEventHeader*) gHades->getCurrentEvent()->getHeader())->getTime();
     UChar_t* et;
     et=(UChar_t*) &eT;
     Char_t title[50];
     sprintf(title,"Event %i (%i.%i.%i %i:%i:%i)%c",
     eSN,ed[0],ed[1],ed[2],et[2],et[1],et[0],0);
     plot->SetTitle(title);
    
     plot->SetXTitle("x, (mm)");
     plot->SetYTitle("y, (mm)");
     plot->SetMinimum(0.5);
     plot->SetMaximum(6.5);
     gStyle->SetPalette(6,pal);
     plot->Draw("COLZ");
     canv[0]->Update();
     if(iprn) plot->Write();
     plot=beamTest->getPlot(2);
     if(plot) {
      plot->SetMarkerColor(2);
      plot->Draw("same");
      canv[0]->Update();
      if(iprn) plot->Write();
     }
    }
    else canv[0]->Clear();
   }
   else if(i==1) {
    canv[1]->cd(0);
    canv[1]->cd(1);
    TH2F *plotF=beamTest->getPlotF(0);
    if(plotF) {
     plotF->SetXTitle("x, (mm)");
     plotF->SetYTitle("y, (mm)");
     plotF->Draw();
     canv[1]->Update();
     if(iprn) plotF->Write();
    }
   }
   else if(i==2) {
    canv[2]->cd(0);
    for (Int_t n=0; n<2; n++) {
     canv[2]->cd(n+1);
     TH1F *hist=beamTest->getHist(11+n);
     hist->SetFillColor(3+n);
     hist->Draw();
     if(iprn) hist->Write();
    }
    canv[2]->cd(3);
    TH1F *hist=beamTest->getHist(6);
    hist->SetFillColor(4);
    if(hist) { hist->Draw();
    if(iprn) hist->Write(); }
    canv[2]->cd(4);
    TH1F *hist=beamTest->getHist(7);
    hist->SetFillColor(4);
    if(hist) { hist->Draw();
    if(iprn) hist->Write(); }   
    canv[2]->Update();  
   }
   else if(i==3) {
    canv[3]->cd(0);
    for (Int_t n=0; n<6; n++) {
     canv[3]->cd(n+1);
     TH1F *hist=beamTest->getHist(n);
     hist->SetFillColor(3);
     gStyle->SetOptStat(2);
     hist->Draw();
     if(iprn) hist->Write();
    }
    canv[3]->Update();
   }
   else if(i==4) {
    canv[4]->cd(0);
    for (Int_t n=0; n<3; n++) {
     canv[4]->cd(n+1);
     TH1F *hist=beamTest->getHist(21+n);
     hist->Draw();
     if(iprn) hist->Write();
    } 
    canv[4]->Update();
   }
   else if(i==5) {
    canv[5]->cd(0);
    for (Int_t n=0; n<2; n++) {
     canv[5]->cd(n+1);
     TH1F *hist=beamTest->getHist(31+n);
     hist->SetFillColor(3);
     hist->Draw();
     if(iprn) hist->Write();
    }
   }
  }
 }
// if(iprn) {
//printf("fh->Close\n");
//  fh->Close();
// }
 return;
}

void initCanv(Int_t npl, Int_t flag) {
 if(flag==0 && canv[npl]!=0) {delete canv[npl]; canv[npl]=0;}
 else {
  if(npl==0) {
   canv[0] = new TCanvas("MdcTrackD", "MdcTrackD",1,1,500,600);
   canv[0]->SetHighLightColor(10);
   canv[0]->Range(-49.375,-41.9375,49.375,42.4375);   
   canv[0]->SetFillColor(10);
   canv[0]->SetBorderSize(0);
  }
  else if(npl==1) {
   canv[1] = new TCanvas("MdcTrackD_all", "MdcTrackD: Hits for all events",510,1,500,600);
   canv[1]->SetHighLightColor(10);
   canv[1]->Range(-49.375,-41.9375,49.375,42.4375);   
   canv[1]->SetFillColor(10);
   canv[1]->SetBorderSize(0);
  }
  else if(npl==2) {
   canv[2] = new TCanvas("MdcTrackD_ThPh", "MdcTrackD: Theta & Phi",1,630,500,470);   
   canv[2]->SetHighLightColor(10);
   canv[2]->SetFillColor(10);
   canv[2]->Divide(2,2);
  }
  else if(npl==3) {
   canv[3] = new TCanvas("Nhinl", "Num. hits in layers",1015,1,570,800);
   canv[3]->SetBorderSize(0);
   canv[3]->SetFillColor(10);
   canv[3]->Divide(1,6);   
  }
  else if(npl==4) {
   canv[4] = new TCanvas("times", "times",17,58,400,935);
   canv[4]->Divide(1,3);
  }
  else if(npl==5) {
   canv[5] = new TCanvas("dXdY", "dXdY",17,58,400,400);
   canv[5]->Divide(2,1);
  }
  
 }
}





