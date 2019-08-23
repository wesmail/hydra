//====================================================================
//Char_t *file="n99_089.hld";
//  Char_t *file="xx00318043306.hld
  Char_t *file="xx00319033031.hld";
 //----------Another way:
 // root [0] .x mdcTrackD.C("file_name");

//Int_t numUnp=1;
//Int_t mdcUnpacker[1]={513};
Int_t numUnp=10;
Int_t mdcUnpacker[10]={201,203,205,207,208,209,210,211,213,214};
//Int_t mdcUnpacker[10]={201,202,203,205,208,209,210,211,213,214};
//====================================================================
  Char_t *fileInp1="/u/pechenov/Hydra/v5_20beem/test/lookup_mdc1_mdc2_mdc3_Nov00_geom.txt";
  Char_t *fileInp2="/u/pechenov/Hydra/v5_20beem/test/add.txt";
//Char_t *lookup="mdc_lookup.txt";
//====================================================================
// isCoilOff=kTRUE - the prog. will drow wires from MDC3&4 (
// isCoilOff=kTRUE
  Bool_t isCoilOff=kTRUE;   // The hits in Mdc3&4 will drowing
//  Bool_t isCoilOff=kFALSE;  // The hits in Mdc3&4 willn't  drowing

// Cut for drift time:
 Float_t cutTime1L=700.;
 Float_t cutTime1R=1700.;
 Float_t cutTime2L=700.;
 Float_t cutTime2R=1700.;
 Float_t cutLeft=30.;       // cutLeft < driftTime1-driftTime2 < cutRight
 Float_t cutRight=500.;      // If cutLeft=cutRight=0. the cut will be ignore
//====================================================================

// working layers: lLayers[sec][mod]=num. of layers
 Int_t nLayers[6][4]={ {3,6,6,6}, {6,6,6,6}, {6,6,6,6},
                       {4,6,6,6}, {6,6,6,6}, {5,6,6,6} };
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
  gHades->setQuietMode();
  Int_t splitLevel=2;
  gHades->setSplitLevel(splitLevel);

  HSpectrometer* spec=gHades->getSetup();
  HMdcDetector *mdc=new HMdcDetector();

   // MDC configuration
 HMdcDetector *mdc=new HMdcDetector;
 Int_t mod[6][4]=
 {
     {0,1,0,0},   // moduledef for sector 1       //  201 208
     {0,1,0,0},   // moduledef for sector 2       //  202 209
     {1,1,0,0},   // ...                          //  203 210
     {1,1,1,0},                                   //  205 211 214
     {0,0,0,0},                                   //  206 212
     {1,1,0,0}                                    //  207 213
 };
/* {
     {1,1,0,0},   // moduledef for sector 1       //  201 208
     {1,1,0,0},   // moduledef for sector 2       //  202 209
     {1,1,0,0},   // ...                          //  203 210
     {1,1,1,0},                                   //  205 211 214
     {0,0,0,0},                                   //  206 212
     {0,1,0,0}                                    //  207 213
 };*/
      
 for(Int_t i=0;i<6;i++) mdc->setModules(i, mod[i]);

  spec->addDetector(mdc);

  HRuntimeDb* rtdb=gHades->getRuntimeDb();
  HRun* run=rtdb->addRun(0);



//Set the data source (here from Lmd file)
  HldFileSource *source=new HldFileSource;
  for(Int_t n=0; n<numUnp; n++)
      source->addUnpacker(new HMdcUnpacker(mdcUnpacker[n],kFALSE));
//  source->addUnpacker(new HStartUnpacker);
  gHades->setDataSource(source);

//  source->setDirectory("/d/hades/nov99");
  source->setDirectory("/d/hades/nov00/hld/");
  source->addFile(fileI);
/*  source->addFile("n99_078.hld");
  source->addFile("n99_079.hld");
  source->addFile("n99_080.hld");
  source->addFile("n99_081.hld");
  source->addFile("n99_082.hld");
  source->addFile("n99_083.hld");
  source->addFile("n99_084.hld");
  source->addFile("n99_085.hld");
  source->addFile("n99_086.hld");
  source->addFile("n99_087.hld");
  source->addFile("n99_088.hld");
  source->addFile("n99_089.hld");*/


  HParAsciiFileIo *inp1=new HParAsciiFileIo;
  HParAsciiFileIo *inp2=new HParAsciiFileIo;
  inp2->open(fileInp1,"in");
  inp1->open(fileInp2,"in");
  gHades->getRuntimeDb()->setFirstInput(inp1);
  gHades->getRuntimeDb()->setSecondInput(inp2);

  HMdcGeomStruct *geomstruct=new HMdcGeomStruct();
  rtdb->addContainer(geomstruct);
  geomstruct.init();

  HMdcRawStruct *rawstruct = new HMdcRawStruct();
  rtdb->addContainer(rawstruct);
  rawstruct.init();

//  HMdcCalParRaw *calparraw = new HMdcCalParRaw();
//  rtdb->addContainer(calparraw);
//  calparraw->init();
//  calparraw->setChanged();
  //  calparraw->printParam();
//  calparraw->setStatic();

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
//  mdcEff=new HMdcEfficiency("MdcEff","MdcEff"); //!!!!!!!!!!!!
  HTaskSet* tasks = new HTaskSet("MDC","Mdc Track Finding Display");
  tasks->connect(new HMdcCalibrater1("MdcCali1","MdcCali1",3));
  tasks->connect((trackFinder=new HMdcTrackFinder("MdcTrackD","MdcTrackD",isCoilOff)),"MdcCali1");
  tasks->connect((mdcEff=new HMdcEfficiency("MdcEff","MdcEff")),"MdcTrackD");
//  tasks->connect((new HMdcClusterToHit("MdcClToHit","MdcClToHit")),"MdcEff");
  tasks->connect((HTask*)NULL,"MdcEff");

  gHades->getTaskSet("real")->connect(tasks);
  gHades->getTaskSet("real")->connect(NULL,tasks);
  gHades->getTaskSet("real")->print();

//  gHades->getRuntimeDb()->setCurrentEventFile(-1);
//  printf("gHades->init()\n");

  if (!gHades->init()) {
//    rtdb->closeFirstInput();
//    rtdb->closeSecondInput();
    return;
  }

//  rtdb->closeFirstInput();
//  rtdb->closeSecondInput();


// Set output file
Int_t compLevel = 2;
gHades->setOutputFile("nov2000.root",
                       "RECREATE","Nov99",compLevel);

//  gHades->makeTree();


//=========================================================
  trackFinder->setLevel(level4, level5);
  trackFinder->setNLayers(nLayers[0]);
//=========================================================

//  nTotEv=gHades->eventLoop(20000);
  doEvents(1);
  gStyle->ToggleEventStatus();
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
TButton *bPlot[3];
TButton *bTypePlot[3];
TButton *bHist[5];
TButton *bHistMb;
TButton *bXYlayer;
TButton *bTimes;
Int_t indSec[6];
Int_t indMod[6][4];
Int_t indLay[6]={1,1,1,1,1,1};          //for all selected modules
Int_t numLay=6;
Int_t indPlot[3]={0,0,0};
Int_t typePlot=0;
Int_t indHist[5]={1,0,0,0,0};
Int_t indHistMb=0;
Int_t numHists=1;
Int_t indXYlayers=0;
Int_t indTimes=0;
Int_t indGroup[2]={1,0};
Int_t sumBn=1;
Int_t sumBnXY=0;
Int_t sumBnTimes=1;
TPaveLabel* pvLSum;
TPaveLabel* pvLSumXY;
TPaveLabel* pvLSumTimes;
Char_t buf1[100];
Char_t buf2[100];
TText* t=new TText();


void selectingSecMod(Int_t s, Int_t m){
 indMod[s][m]=abs(indMod[s][m]-1);
 Int_t indSecOld=indSec[s];
 indSec[s]+=(indMod[s][m]) ? +1:-1;
 bSecMod[s][m]->SetTextColor(indMod[s][m]*5+1);
 doDraw(indMod[s][m]);
 if(indSec[s]<=1 && indSecOld<=1) doPlots(indMod[s][m]);
}

void selectingLayer(Int_t l){
 indLay[l]=abs(indLay[l]-1);
 bLayer[l]->SetTextColor(indLay[l]*5+1);
 numLay+=indLay[l]*2-1;
 setListLayer();
 if(indGroup[1] || indXYlayers) doDraw(indLay[l]);
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
 if(n<2) doPlots(indPlot[n]);
 else doEventPlots(indPlot[n]);
}

void selectingTypePlot(Int_t n){
 if(n==typePlot) return;
 bTypePlot[typePlot]->SetTextColor(1);
 bTypePlot[typePlot]->Modified();
 typePlot=n;
 bTypePlot[typePlot]->SetTextColor(3);
 if(indPlot[0]) doPlots(indPlot[0]);
 if(indPlot[2]) doEventPlots(indPlot[2]);
}

void selectingHist(Int_t n){
 indHist[n]=abs(indHist[n]-1);
 bHist[n]->SetTextColor(indHist[n]*5+1);
 numHists+=indHist[n]*2-1;
 setListPads();
 doDraw(indHist[n]);
}

void selectingXYlayer(){
 indXYlayers=abs(indXYlayers-1);
 bXYlayer->SetTextColor(indXYlayers*5+1);
 doDraw(indXYlayers);
}

void drawTimes(){
 indTimes=abs(indTimes-1);
 bTimes->SetTextColor(indTimes*5+1);
 doDraw(indTimes);
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
    indSec[s]=0;
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

  // Buttons for XY plots:
  bXYlayer=new TButton("Eff(xy)","selectingXYlayer()",oX+dX*0,oY+dY*9,oX+dX*0+wX,oY+dY*9+hY);
  drawButton(bXYlayer, 42,indXYlayers*5+1);

  dX=0.96/10.; wX=dX-0.01; oX=0.02;
  button=new TButton("-1","selectingSumXY(-1)",oX+dX*6,oY+dY*9,oX+dX*6+wX,oY+dY*9+hY);
  drawButton(button,42,4);
  sprintf(buf1,"%i",sumBn);
  pvLSumXY=new TPaveLabel(oX+dX*7,oY+dY*9,oX+dX*7+wX,oY+dY*9+hY,buf1,"L");
  pvLSumXY->SetTextSize(1.2);
  pvLSumXY->SetTextColor(4);
  pvLSumXY->SetFillColor(10);
  pvLSumXY->SetTextFont(22);
  pvLSumXY->SetTextAlign(22);
  pvLSumXY->SetBorderSize(5);
  pvLSumXY->Draw();
  button=new TButton("+1","selectingSumXY(+1)",oX+dX*8,oY+dY*9,oX+dX*8+wX,oY+dY*9+hY);
  drawButton(button,42,4);


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

  // Buttons for times:
  dX=0.96/4; wX=0.27; oX=0.02;
  bTimes=new TButton("Time 1,2,2-1","drawTimes()",oX+dX*0,oY+dY*7,oX+dX*0+wX,oY+dY*7+hY);
  drawButton(bTimes, 42,indTimes*5+1);

  dX=0.96/10.; wX=dX-0.01; oX=0.02;
  button=new TButton("-5","selectingSumTimes(-5)",oX+dX*5,oY+dY*7,oX+dX*5+wX,oY+dY*7+hY);
  drawButton(button, 42,4);
  button=new TButton("-1","selectingSumTimes(-1)",oX+dX*6,oY+dY*7,oX+dX*6+wX,oY+dY*7+hY);
  drawButton(button,42,4);
  sprintf(buf1,"%i",sumBn);
  pvLSumTimes=new TPaveLabel(oX+dX*7,oY+dY*7,oX+dX*7+wX,oY+dY*7+hY,buf1,"L");
  pvLSumTimes->SetTextSize(1.2);
  pvLSumTimes->SetTextColor(4);
  pvLSumTimes->SetFillColor(10);
  pvLSumTimes->SetTextFont(22);
  pvLSumTimes->SetTextAlign(22);
  pvLSumTimes->SetBorderSize(5);
  pvLSumTimes->Draw();
  button=new TButton("+1","selectingSumTimes(+1)",oX+dX*8,oY+dY*7,oX+dX*8+wX,oY+dY*7+hY);
  drawButton(button,42,4);
  button=new TButton("+5","selectingSumTimes(+5)",oX+dX*9,oY+dY*7,oX+dX*9+wX,oY+dY*7+hY);
  drawButton(button,42,4);

  // Buttons for plots:
  dX=0.28; wX=0.27; oX=0.02;
  t->DrawText(oX+dX*0+0.02,oY+dY*4+dY/5.,"Plots:");
  bTypePlot[0]=new TButton("A","selectingTypePlot(0)",oX+dX*1,oY+dY*5,oX+dX*1+dX/3.-0.01,oY+dY*5+hY);
  drawButton(bTypePlot[0],42,(typePlot==0)?3:1);
  bTypePlot[1]=new TButton("B","selectingTypePlot(1)",oX+dX*1+dX/3.,oY+dY*5,oX+dX*1+dX*2./3.-0.01,oY+dY*5+hY);
  drawButton(bTypePlot[1],42,(typePlot==1)?3:1);
  bTypePlot[2]=new TButton("C","selectingTypePlot(2)",oX+dX*1+dX*2./3.,oY+dY*5,oX+dX*1+dX-0.01,oY+dY*5+hY);
  drawButton(bTypePlot[2],42,(typePlot==2)?3:1);
  bPlot[2]=new TButton("Event","selectingPlot(2)",oX+dX*2,oY+dY*5,oX+dX*2+wX,oY+dY*5+hY);
  drawButton(bPlot[2],42,indPlot[0]*5+1);
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

  button=new TButton("Quit","delete gHades; dialog->Close(); gROOT->DeleteAll();",0.75,oY+dY*1,0.99,oY+dY*1+hY);
  drawButton(button);
  button=new TButton("Quit ROOT",".q", 0.75,oY+dY*0,0.99,oY+dY*0+hY);
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

void selectingSumTimes(Int_t n){
 if(sumBnTimes==1 && n<0) return;
 sumBnTimes+=n;
 if(sumBnTimes<1) sumBnTimes=1;
 if(sumBnTimes>100) sumBnTimes=100;
 sprintf(buf1,"%i",sumBnTimes);
 pvLSumTimes->SetLabel(buf1);
 dialog->Modified();
 doDraw();
}

void selectingSumXY(Int_t n) {
  if((sumBnXY==0 && n<0) || (sumBnXY==3 && n>0)) return;
  sumBnXY+=n;
  Int_t sBn=1;
  for(Int_t i=0; i<sumBnXY; i++) sBn*=2;
  sprintf(buf1,"%i",sBn);
  pvLSumXY->SetLabel(buf1);
  dialog->Modified();
  for(Int_t sec=0; sec<6; sec++) {
    if(!&(*mdcEff)[sec]) continue;
    feffSec=&(*mdcEff)[sec];
    for(Int_t mod=0; mod<4; mod++) {
     if(!&(*feffSec)[mod]) continue;
     feffMod=&(*feffSec)[mod];
     for(Int_t lay=0; lay<6; lay++) {
       feffLay=&(*feffMod)[lay];
       feffLay->setBiningXY(sBn);
     }
   }
  }
// doDraw();
}

void printClusters(){
 for(Int_t sec=0; sec<6; sec++) {
  if(indSec[sec])  mdcEff->printClusters(sec); // mod ???
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
   if( ind && indMod[sec][mod] ) {
    if( indHistMb ) feffMod->drawCanvEffMb();
    if( indTimes ) feffMod->drawCanvTimes(sumBnTimes);
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
     if(indXYlayers) {
       for(Int_t lay=0; lay<6; lay++) {
        if(!indLay[lay]) continue;
        feffLay=&(*feffMod)[lay];
        if(numHists) feffLay->drawCanvXYlist();
       }
     }
    }
   }
   else {
    if( !indHistMb || !indMod[sec][mod] )  feffMod->delCanvMb();
    if( !indTimes || !indMod[sec][mod] ) feffMod->delCanvTimes();
    Int_t indCls=(indMod[sec][mod]>0 && numLay>0) ? 0 : 1;
    if(!indHist[0] || indCls || !indGroup[0]) feffMod->delCanvTotNhits();
    if(!indHist[1] || indCls || !indGroup[0]) feffMod->delCanvNHits();
    if(!indHist[2] || indCls || !indGroup[0]) feffMod->delCanvHistEff();
    if(!indHist[3] || indCls || !indGroup[0]) feffMod->delCanvHistSing();
    if(!indHist[4] || indCls || !indGroup[0]) feffMod->delCanvProbabilitis();
    for(Int_t lay=0; lay<6; lay++) {
     feffLay=&(*feffMod)[lay];
     if(indMod[sec][mod]==0 || indLay[lay]==0 || numHists==0 || indGroup[1]==0) feffLay->delCanvas();
     if(!indXYlayers || indCls || indLay[lay]==0) feffLay->delCanvXYlist();
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
    Int_t ind1=(indPlot[0] && indSec[sec]) ? 1 : 0;
    Int_t ind2=(indPlot[1] && indSec[sec]) ? 1 : 0;
    if(ind) {
      if(ind1) {feffSec->drawCanvProj(); mdcEff->drawProjPlotsSec(sec,typePlot);}
      if(ind2) {feffSec->drawCanvHits(); mdcEff->drawHitsPlots(sec);}
    }
    else {
      if(!ind1) feffSec->delCanvProj();
      if(!ind2) feffSec->delCanvHits();
    }
  }
}

void doEventPlots(Int_t ind=1){
 //ind=1 - draw canvas
 //   =0 - close canvas
  if(ind) {mdcEff->drawCanvEvent(); mdcEff->drawPrPlEvent(typePlot);}
  else mdcEff->delCanvEvent();
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
  if(evN > 0) printf("--- Total num. of events processed: %i ---\n",nTotEv);
 }
 if(evN > 0) printf("======== Total num. of events processed: %i ========\n",nTotEv);
 else if(evN < 0) printf("======== The error of reading event. evN=%i\n",evN);
 if(evN>=0 && evN<nEvDo) printf(
 "======== End of file! Total num. of events processed: %i ========\n",nTotEv);
}

