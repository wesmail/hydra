//====================================================================
Char_t *file="n99_089.hld";
 //----------Another way: 
 // root [0] .x mdcTrackD.C("file_name");

Int_t mdcUnpacker=513;
//Int_t mdcUnpacker=6;
//====================================================================
Char_t *lookup="mdc_lookup.txt"; 
//====================================================================

// Cut for drift time:
 Float_t cutTime1L=0.;
 Float_t cutTime1R=200.;
 Float_t cutTime2L=0.;
 Float_t cutTime2R=200.;
 Float_t cutLeft=20.;       // cutLeft < driftTime1-driftTime2 < cutRight
 Float_t cutRight=200.;      // If cutLeft=cutRight=0. the cut will be ignore
//====================================================================


//------------------------------------------------
 Int_t level4=5;         
// Int_t level5=50;
// Int_t level4=0;
 Int_t level5=100000;
//------------------------------------------------


HMdcEfficiency* mdcEff;

//======================================================================

void mdcEfficiency(Char_t* fileI=file){

  Hades* myHades = new Hades();
  Int_t splitLevel=2;
  gHades->setSplitLevel(splitLevel);  

  HSpectrometer* spec=gHades->getSetup();
  HMdcDetector *mdc=new HMdcDetector();
  Int_t mod[4]={0,1,0,0};
//  for(Int_t i=0;i<6;i++) mdc.setModules(i,mod);
  mdc->setModules(2,mod);
  spec->addDetector(mdc);
  
  HRuntimeDb* rtdb=gHades->getRuntimeDb();
  HRun* run=rtdb->addRun(944451190); 
  
  

//Set the data source (here from Lmd file)
  HldFileSource *source=new HldFileSource;
//  source->addUnpacker(new HMdcUnpacker(6,kTRUE));
  source->addUnpacker(new HMdcUnpacker(mdcUnpacker,kFALSE));
//  source->addUnpacker(new HStartUnpacker);
  gHades->setDataSource(source);
  
  source->setDirectory("/d/hades/nov99"); 
  source->addFile(fileI);


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
  tasks->connect((mdcEff=new HMdcEfficiency("MdcEff","MdcEff")),"MdcTrackD");
  tasks->connect(NULL,"MdcEff");
  
  gHades->getTaskSet("real")->connect(tasks); 
  gHades->getTaskSet("real")->print();
  
//  gHades->getRuntimeDb()->setCurrentEventFile(-1);
//  printf("gHades->init()\n");

  if (!gHades->init()) {
    rtdb->closeFirstInput();
    rtdb->closeSecondInput();
    return;
  }
  
//  rtdb->closeFirstInput();
//  rtdb->closeSecondInput();
  
    
// Set output file
//Int_t compLevel = 1; 
//gHades->setOutputFile("/u/nekhaev/anal/Hydra/cur/macros/Mdc/nov99.root",
//                       "RECREATE","Nov99",compLevel); 

//  gHades->makeTree();


//=========================================================
  trackFinder->setLevel(level4, level5);
//========================================================= 

  doEvents(1);
  menu();
  gROOT->SaveContext();

//  delete gHades;  // this takes very long because of ~TGeometry()
} 




HMdcEfficiencySec* feffSec;
HMdcEfficiencyMod* feffMod;
HMdcEfficiencyLayer* feffLay;
TDialogCanvas *dialog;
TButton *bSecMod[6][4];
TButton *bLayer[6];
TButton *bGroupMdcLay[2];
TButton *button;
TButton *bPlot[2];
TButton *bHist[5];
TButton *bHistMb;
Int_t indMod[6][4];
Int_t indLay[6]={1,1,1,1,1,1};          //for all selected modules
Int_t numLay=6;
Int_t indPlot[2]={0,0};
Int_t indHist[5]={0,0,1,0,0};
Int_t indHistMb=0;
Int_t numHists=1;
Int_t indGroup[2]={1,0};
Int_t sumBn=1;
TPaveLabel* pvLSum;
Char_t buf1[100];
Char_t buf2[100];
TText* t=new TText();


void selectingSecMod(Int_t s, Int_t m){
 indMod[s][m]=abs(indMod[s][m]-1);
 bSecMod[s][m]->SetTextColor(indMod[s][m]*5+1); 
 doDraw(indMod[s][m]);
 doPlots(indMod[s][m]);
}

void selectingLayer(Int_t l){
 indLay[l]=abs(indLay[l]-1);
 bLayer[l]->SetTextColor(indLay[l]*5+1);
 numLay+=indLay[l]*2-1;
 setListLayer();
 if(indGroup[1]) doDraw(indLay[l]);
}

void setListPads(){
  for(Int_t sec=0; sec<6; sec++) {
  if(!&(*mdcEff)[sec]) continue;
  feffSec=&(*mdcEff)[sec];
  for(Int_t mod=0; mod<4; mod++) {
   feffMod=&(*feffSec)[mod];
   if(!feffMod) continue;
   for(Int_t lay=0; lay<6; lay++) {
    feffLay=&(*feffMod)[lay];
    feffLay->setListPads(indHist);
   }
  }
 }
}

void setListLayer(){
 for(Int_t sec=0; sec<6; sec++) {
  if(!&(*mdcEff)[sec]) continue;
  feffSec=&(*mdcEff)[sec];
  for(Int_t mod=0; mod<4; mod++) {
   feffMod=&(*feffSec)[mod];
   if(feffMod) feffMod->setListLayers(indLay);
  }
 }
}

void selectingPlot(Int_t n){
 indPlot[n]=abs(indPlot[n]-1);
 bPlot[n]->SetTextColor(indPlot[n]*5+1);
 doPlots(indPlot[n]);
}

void selectingHist(Int_t n){
 indHist[n]=abs(indHist[n]-1);
 bHist[n]->SetTextColor(indHist[n]*5+1);
 numHists+=indHist[n]*2-1;
 setListPads();
 doDraw(indHist[n]);
}

void selectingHistMb(){
 indHistMb=abs(indHistMb-1);
 bHistMb->SetTextColor(indHistMb*5+1);
 doDraw(indHistMb);
}

void selectingGroup(Int_t n) {
 indGroup[n]=abs(indGroup[n]-1);
 bGroupMdcLay[n]->SetTextColor(indGroup[n]*5+1);
 doDraw(indGroup[n]);
}

void menu(){
  for(Int_t s=0; s<6; s++){
   for(Int_t m=0; m<4; m++){
    indMod[s][m]=0;
   }
  }
  setListLayer();
  setListPads();
   
  dialog=new TDialogCanvas("EffMdc","The Efficiency of MDC",300,500);
  dialog->SetFillColor(16);
  dialog->cd();
  t->SetTextFont(132);
  t->SetTextSize(0.07);
  t->SetTextColor(1);
  t->SetTextAlign(11); 
  t->DrawText(.03,0.95,"Sector:");
  Float_t x1,x2,y1,y2;
  for(Int_t s=0; s<6; s++) {
   x1=0.4+((0.99-0.4)/6.)*s;
   x2=x1+(0.99-0.4)/6.-0.01;
   sprintf(buf1,"%i",s+1);
   t->DrawText(x1+0.03,0.95,buf1);
   if(s==0) t->DrawText(0.03,0.75+((0.93-0.75)/4.)*3+0.01,"Module:");
   Int_t ind=0;
   if(!&(*mdcEff)[s]) continue;
   HMdcEfficiencySec& effSec=(*mdcEff)[s];
   for(Int_t m=0; m<4; m++) {
    if(&effSec[m]) {
     y1=0.75+((0.93-0.75)/4.)*(3-m);
     y2=y1+(0.93-0.75)/4.-0.005;
     sprintf(buf1,"%i",m+1);
     sprintf(buf2,"selectingSecMod(%i,%i);",s,m);
     bSecMod[s][m]=new TButton(buf1,buf2,x1,y1,x2,y2);
     drawButton(bSecMod[s][m],42,indMod[s][m]*5+1);
    }
   }
  }
  y1=0.75-(0.93-0.75)/4.-0.01;
  y2=y1+(0.93-0.75)/4.-0.005;
  t->DrawText(0.03,y1+0.01,"Layer:");
  for(Int_t l=0; l<6; l++) {
   x1=0.4+((0.99-0.4)/6.)*l;
   x2=x1+(0.99-0.4)/6.-0.01;
   sprintf(buf1,"%i",l+1);
   sprintf(buf2,"selectingLayer(%i);",l);
   bLayer[l]=new TButton(buf1,buf2,x1,y1,x2,y2);
   drawButton(bLayer[l],42,indLay[l]*5+1);
  }
  
  Float_t dY=0.045;
  Float_t hY=0.04;
  Float_t oY=0.01;
  Float_t dX,wX,oX;
 
  
  // Buttons for histograms:
  dX=0.96/4.; wX=dX-0.01; oX=0.02;

  t->DrawText(0.03,oY+dY*14+0.01,"Select by:");  
  bGroupMdcLay[0]=new TButton("MDC","selectingGroup(0)",oX+dX*2,oY+dY*14,oX+dX*2+wX,oY+dY*14+hY);
  drawButton(bGroupMdcLay[0],42,indGroup[0]*5+1);
  bGroupMdcLay[1]=new TButton("Layer","selectingGroup(1)",oX+dX*3,oY+dY*14,oX+dX*3+wX,oY+dY*14+hY);
  drawButton(bGroupMdcLay[1],42,indGroup[1]*5+1);
  
  t->DrawText(0.03,oY+dY*13+0.01,"Histog.:");
  bHist[0]=new TButton("TotHits","selectingHist(0)",oX+dX*2,oY+dY*13,oX+dX*2+wX,oY+dY*13+hY);
  drawButton(bHist[0],42,indHist[0]*5+1);
  bHist[1]=new TButton("Tracks|Hits","selectingHist(1)",oX+dX*3,oY+dY*13,oX+dX*3+wX,oY+dY*13+hY);
  drawButton(bHist[1],42,indHist[1]*5+1);
  bHist[2]=new TButton("Efficiency","selectingHist(2)",oX+dX*1,oY+dY*13,oX+dX*1+wX,oY+dY*13+hY);
  drawButton(bHist[2],42,indHist[2]*5+1);
  bHist[3]=new TButton("Single","selectingHist(3)",oX+dX*0,oY+dY*12,oX+dX*0+wX,oY+dY*12+hY);
  drawButton(bHist[3],42,indHist[3]*5+1);
  bHist[4]=new TButton("Probability","selectingHist(4)",oX+dX*1,oY+dY*12,oX+dX*1+wX,oY+dY*12+hY);
  drawButton(bHist[4],42,indHist[4]*5+1);
  bHistMb=new TButton("Eff.vs.Mb","selectingHistMb()",oX+dX*0,oY+dY*10.5,oX+dX*0+wX,oY+dY*10.5+hY);
  drawButton(bHistMb,42,indHistMb*5+1);
  
  dX=0.96/10.; wX=dX-0.01; oX=0.02;
  button=new TButton("-5","selectingSum(-5)",oX+dX*5,oY+dY*12,oX+dX*5+wX,oY+dY*12+hY);
  drawButton(button, 42,4);
  button=new TButton("-1","selectingSum(-1)",oX+dX*6,oY+dY*12,oX+dX*6+wX,oY+dY*12+hY);
  drawButton(button,42,4);
  sprintf(buf1,"%i",sumBn);
  pvLSum=new TPaveLabel(oX+dX*7,oY+dY*12,oX+dX*7+wX,oY+dY*12+hY,buf1,"L");
  pvLSum->SetTextSize(1.2);
  pvLSum->SetTextColor(4);
  pvLSum->SetFillColor(10);
  pvLSum->SetTextFont(22);
  pvLSum->SetTextAlign(22); 
  pvLSum->SetBorderSize(5);
  pvLSum->Draw();
  button=new TButton("+1","selectingSum(+1)",oX+dX*8,oY+dY*12,oX+dX*8+wX,oY+dY*12+hY);
  drawButton(button,42,4);
  button=new TButton("+5","selectingSum(+5)",oX+dX*9,oY+dY*12,oX+dX*9+wX,oY+dY*12+hY);
  drawButton(button,42,4);
  
  // Buttons for plots:
  dX=0.28; wX=0.27; oX=0.02;
  t->DrawText(oX+dX*0+0.02,oY+dY*4+dY/5.,"Plots:");
  bPlot[0]=new TButton("Projections","selectingPlot(0)",oX+dX*1,oY+dY*4,oX+dX*1+wX,oY+dY*4+hY);
  drawButton(bPlot[0],42,indPlot[0]*5+1);
  bPlot[1]=new TButton("MDC hits","selectingPlot(1)",oX+dX*2,oY+dY*4,oX+dX*2+wX,oY+dY*4+hY);
  drawButton(bPlot[1],42,indPlot[1]*5+1);
  button=new TButton("Print clusters","printClusters()",oX+dX*1,oY+dY*3,oX+dX*1+wX+wX/2.,oY+dY*3+hY);
  drawButton(button,42,1);
  
  // Buttons for event loop:
  dX=0.16; wX=0.15; oX=0.02;
  t->DrawText(oX+dX*0+0.02,oY+dY*1+dY/5.,"Event loop:");
  button=new TButton("Next","doEvents(1)",oX+dX*2,oY+dY*1,oX+dX*2+wX,oY+dY*1+hY);
  drawButton(button);
  button=new TButton("All","doEvents(-1)",oX+dX*3,oY+dY*1,oX+dX*3+wX,oY+dY*1+hY);
  drawButton(button);
  button=new TButton("10","doEvents(10)",oX+dX*0,oY+dY*0,oX+dX*0+wX,oY+dY*0+hY);
  drawButton(button);
  button=new TButton("100","doEvents(100)",oX+dX*1,oY+dY*0,oX+dX*1+wX,oY+dY*0+hY);
  drawButton(button);
  button=new TButton("1000","doEvents(1000)",oX+dX*2,oY+dY*0,oX+dX*2+wX,oY+dY*0+hY);
  drawButton(button);
  button=new TButton("10000","doEvents(10000)",oX+dX*3,oY+dY*0,oX+dX*3+wX,oY+dY*0+hY);
  drawButton(button);
  
  button=new TButton("Refresh","mdcEff->calcEfficiency(sumBn)",0.80,oY+dY*2,0.99,oY+dY*2+hY);
  drawButton(button);
  
  button=new TButton("Quit",".q",0.85,oY+dY*0,0.99,oY+dY*0+hY);
  drawButton(button);
  
  dialog->cd();
}

void drawButton(TButton* bt, Int_t nFnt=42, Int_t nTCl=1){
  bt->SetTextFont(nFnt);
  bt->SetTextColor(nTCl);
  bt->SetFillColor(16); 
  bt->SetBorderSize(1);
  bt->Draw();
}

void selectingSum(Int_t n){
 if(sumBn==1 && n<0) return;
 sumBn+=n;
 if(sumBn<1) sumBn=1;
 sprintf(buf1,"%i",sumBn);
 pvLSum->SetLabel(buf1);
 dialog->Modified();
 mdcEff->calcEfficiency(sumBn);
 doDraw();
}

void printClusters(){
 for(Int_t sec=0; sec<6; sec++) {
  if(indMod[sec][0]+indMod[sec][1]) {
   mdcEff->printClusters(sec);
  }
 }
}

void doDraw(Int_t ind=1){
 //ind=1 - draw canvas
 //   =0 - close canvas
 for(Int_t sec=0; sec<6; sec++) {
  if(!&(*mdcEff)[sec]) continue;
  feffSec=&(*mdcEff)[sec];
  for(Int_t mod=0; mod<4; mod++) {
   if(!&(*feffSec)[mod]) continue;
   feffMod=&(*feffSec)[mod];                          //Drawing
   if(ind && indMod[sec][mod]) {
    if(indHistMb) feffMod->drawCanvEffMb();
    if(numLay) {
     if(indGroup[0]) {
      if(indHist[0]) feffMod->drawCanvTotNhits();
      if(indHist[1]) feffMod->drawCanvNHits();
      if(indHist[2]) feffMod->drawCanvHistEff();
      if(indHist[3]) feffMod->drawCanvHistSing();
      if(indHist[4]) feffMod->drawCanvProbabilitis();
     }
     if(indGroup[1]) {
      for(Int_t lay=0; lay<6; lay++) {
       if(!indLay[lay]) continue;
       feffLay=&(*feffMod)[lay];
       if(numHists) feffLay->drawCanvas();
      }
     }
    }
   }
   else {
    if(!indHistMb || !indMod[sec][mod])  feffMod->delCanvMb();
    Int_t indCls=(indMod[sec][mod]>0 && numLay>0) ? 0 : 1;
    if(!indHist[0] || indCls || !indGroup[0]) feffMod->delCanvTotNhits();
    if(!indHist[1] || indCls || !indGroup[0]) feffMod->delCanvNHits();
    if(!indHist[2] || indCls || !indGroup[0]) feffMod->delCanvHistEff();
    if(!indHist[3] || indCls || !indGroup[0]) feffMod->delCanvHistSing();
    if(!indHist[4] || indCls || !indGroup[0]) feffMod->delCanvProbabilitis();
    for(Int_t lay=0; lay<6; lay++) {
     feffLay=&(*feffMod)[lay];
     if(indMod[sec][mod]==0 || indLay[lay]==0 || numHists==0 || indGroup[1]==0) feffLay->delCanvas();
    }
   }
  }
 }
}

void doPlots(Int_t ind=1){
 //ind=1 - draw canvas
 //   =0 - close canvas
 for(Int_t sec=0; sec<6; sec++) {
  if(!&(*mdcEff)[sec]) continue;
  feffSec=&(*mdcEff)[sec];
  Int_t ind1=(indPlot[0] && indMod[sec][0]+indMod[sec][1]>0) ? 1 : 0;
  Int_t ind2=(indPlot[1] && indMod[sec][0]+indMod[sec][1]>0) ? 1 : 0;
  if(ind) {
   if(ind1) {feffSec->drawCanvProj(); mdcEff->drawProjPlots(sec);}
   if(ind2) {feffSec->drawCanvHits(); mdcEff->drawHitsPlots(sec);}
  }
  else {
   if(!ind1) feffSec->delCanvProj();
   if(!ind2) feffSec->delCanvHits();
  }
 }
}

Int_t nTotEv=0;
void doEvents(Int_t nEv) {
 Int_t nEvDo=(nEv/10==0) ? 1 : nEv/10;
 Int_t nfor=nEv/nEvDo;
 if(nEv==-1) {nEvDo=10000; nfor=1;}
 Int_t evN;
 for(Int_t nTm=0; nTm<nfor; nTm++) {
  evN=gHades->eventLoop(nEvDo);
  mdcEff->calcEfficiency(sumBn);
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

