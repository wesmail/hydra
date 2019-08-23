#include "hmdceffmenu.h"
#include "hades.h"
#include "TStyle.h"

HMdcEffMenu* HMdcEffMenu::fEffMenu=0;

HMdcEffMenu* HMdcEffMenu::makeMenu(HMdcEfficiency *feff) {
  if(fEffMenu) deleteMenu();
  fEffMenu=new HMdcEffMenu(feff);
  return fEffMenu;
}

void HMdcEffMenu::deleteMenu(void) {
  if(fEffMenu) delete fEffMenu;
}

HMdcEffMenu::HMdcEffMenu(HMdcEfficiency *feff) {
  mdcEff=feff;
  for(Int_t i=0; i<6; i++) indLay[i]=1;
  indPlot[0]=indPlot[1]=indPlot[2]=0;
  indHist[0]=1;
  indHist[1]=indHist[2]=indHist[3]=indHist[4]=0;
  typePlot=indHistMb=indXYlayers=indTimes=sumBnXY=totModsSel=nTotEv=0;
  numLay=6;
  numHists=1;
  sumBn=sumBnTimes=1;
  indGroup[0]=1;
  indGroup[1]=0;
  text=new TText();
  gStyle->ToggleEventStatus();
  menu();
}

HMdcEffMenu::~HMdcEffMenu() {
  if(!fEffMenu) return;
//   for(Int_t s=0;s<6;s++) for(Int_t m=0;m<4;m++) if(bSecMod[s][m]) delete bSecMod[s][m];
//   for(Int_t s=0; s<6; s++) if(bLayer[s]) delete bLayer[s];
//   for(Int_t n=0; n<2; n++)if(bGroupMdcLay[n]) delete bGroupMdcLay[n];
//   for(Int_t n=0; n<30; n++) if(button[n]) delete button[n];
//   for(Int_t n=0; n<3; n++) if(bPlot[n]) delete bPlot[n];
//   for(Int_t n=0; n<3; n++)  if(bTypePlot[n]) delete bTypePlot[n];
//   for(Int_t n=0; n<5; n++) if(bHist[n]) delete bHist[n];
//   if(bHistMb) delete bHistMb;
//   if(bXYlayer) delete bXYlayer;
//   if(bTimes) delete bTimes;
//   if(text) delete text;
//   if(pvLSum) delete pvLSum;
//   if(pvLSumXY) delete pvLSumXY;
//   if(pvLSumTimes) delete pvLSumTimes;
//   if(dialog) {
//     dialog->Close();
//     delete dialog;
//   }
}

void HMdcEffMenu::selectingSecMod(Int_t s, Int_t m){
  indMod[s][m]=abs(indMod[s][m]-1);
  (indMod[s][m]) ? totModsSel++:totModsSel--;
  Int_t indSecOld=indSec[s];
  indSec[s]+=(indMod[s][m]) ? +1:-1;
  bSecMod[s][m]->SetTextColor(indMod[s][m]*5+1);
  doDraw(indMod[s][m]);
  if(indSec[s]<=1 && indSecOld<=1) doPlots(indMod[s][m]);
}

void HMdcEffMenu::selectingLayer(Int_t l){
  indLay[l]=abs(indLay[l]-1);
  bLayer[l]->SetTextColor(indLay[l]*5+1);
  numLay+=indLay[l]*2-1;
  setListLayer();
  if(indGroup[0] || indGroup[1] || indXYlayers) doDraw(indLay[l]);
}

void HMdcEffMenu::setListPads(){
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

void HMdcEffMenu::setListLayer(){
  for(Int_t sec=0; sec<6; sec++) {
    if(!&(*mdcEff)[sec]) continue;
    feffSec=&(*mdcEff)[sec];
    for(Int_t mod=0; mod<4; mod++) {
      feffMod=&(*feffSec)[mod];
      if(feffMod) feffMod->setListLayers(indLay);
    }
  }
}

void HMdcEffMenu::selectingPlot(Int_t n){
  indPlot[n]=abs(indPlot[n]-1);
  bPlot[n]->SetTextColor(indPlot[n]*5+1);
  if(n<2) doPlots(indPlot[n]);
  else doEventPlots(indPlot[n]);
}

void HMdcEffMenu::selectingTypePlot(Int_t n){
  if(n==typePlot) return;
  bTypePlot[typePlot]->SetTextColor(1);
  bTypePlot[typePlot]->Modified();
  typePlot=n;
  bTypePlot[typePlot]->SetTextColor(3);
  if(indPlot[0]) doPlots(indPlot[0]);
  if(indPlot[2]) doEventPlots(indPlot[2]);
}

void HMdcEffMenu::selectingHist(Int_t n){
  indHist[n]=abs(indHist[n]-1);
  bHist[n]->SetTextColor(indHist[n]*5+1);
  numHists+=indHist[n]*2-1;
  setListPads();
  doDraw(indHist[n]);
}

void HMdcEffMenu::selectingXYlayer(){
  indXYlayers=abs(indXYlayers-1);
  bXYlayer->SetTextColor(indXYlayers*5+1);
  doDraw(indXYlayers);
}

void HMdcEffMenu::drawTimes(){
  indTimes=abs(indTimes-1);
  bTimes->SetTextColor(indTimes*5+1);
  doDraw(indTimes);
}

void HMdcEffMenu::selectingHistMb(){
  indHistMb=abs(indHistMb-1);
  bHistMb->SetTextColor(indHistMb*5+1);
  doDraw(indHistMb);
}

void HMdcEffMenu::selectingGroup(Int_t n) {
  indGroup[n]=abs(indGroup[n]-1);
  bGroupMdcLay[n]->SetTextColor(indGroup[n]*5+1);
  doDraw(indGroup[n]);
}

void HMdcEffMenu::menu(){
  gHades->eventLoop(1);
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
  text->SetTextFont(132);
  text->SetTextSize(0.07);
  text->SetTextColor(1);
  text->SetTextAlign(11);
  text->DrawText(.03,0.95,"Sector:");
  Float_t x1,x2,y1,y2;
  for(Int_t s=0; s<6; s++) {
    x1=0.4+((0.99-0.4)/6.)*s;
    x2=x1+(0.99-0.4)/6.-0.01;
    sprintf(buf1,"%i",s+1);
    text->DrawText(x1+0.03,0.95,buf1);
    if(s==0) text->DrawText(0.03,0.75+((0.93-0.75)/4.)*3+0.01,"Module:");
    if(!&(*mdcEff)[s]) continue;
    HMdcEfficiencySec& effSec=(*mdcEff)[s];
    for(Int_t m=0; m<4; m++) {
      if(!&effSec[m]) continue;
      y1=0.75+((0.93-0.75)/4.)*(3-m);
      y2=y1+(0.93-0.75)/4.-0.005;
      sprintf(buf1,"%i",m+1);
      sprintf(buf2,"effMenu->selectingSecMod(%i,%i);",s,m);
      bSecMod[s][m]=new TButton(buf1,buf2,x1,y1,x2,y2);
      drawButton(bSecMod[s][m],42,indMod[s][m]*5+1);
    }
  }
  y1=0.75-(0.93-0.75)/4.-0.01;
  y2=y1+(0.93-0.75)/4.-0.005;
  text->DrawText(0.03,y1+0.01,"Layer:");
  for(Int_t l=0; l<6; l++) {
    x1=0.4+((0.99-0.4)/6.)*l;
    x2=x1+(0.99-0.4)/6.-0.01;
    sprintf(buf1,"%i",l+1);
    sprintf(buf2,"effMenu->selectingLayer(%i);",l);
    bLayer[l]=new TButton(buf1,buf2,x1,y1,x2,y2);
    drawButton(bLayer[l],42,indLay[l]*5+1);
  }

  Float_t dY=0.045;
  Float_t hY=0.04;
  Float_t oY=0.01;
  Float_t dX,wX,oX;


  // Buttons for histograms:
  dX=0.96/4.; wX=dX-0.01; oX=0.02;

  text->DrawText(0.03,oY+dY*14+0.01,"Select by:");
  bGroupMdcLay[0]=new TButton("MDC","effMenu->selectingGroup(0)",oX+dX*2,oY+dY*14,oX+dX*2+wX,oY+dY*14+hY);
  drawButton(bGroupMdcLay[0],42,indGroup[0]*5+1);
  bGroupMdcLay[1]=new TButton("Layer","effMenu->selectingGroup(1)",oX+dX*3,oY+dY*14,oX+dX*3+wX,oY+dY*14+hY);
  drawButton(bGroupMdcLay[1],42,indGroup[1]*5+1);

  text->DrawText(0.03,oY+dY*13+0.01,"Histog.:");
  bHist[0]=new TButton("TotHits","effMenu->selectingHist(0)",oX+dX*2,oY+dY*13,oX+dX*2+wX,oY+dY*13+hY);
  drawButton(bHist[0],42,indHist[0]*5+1);
  bHist[1]=new TButton("Tracks|Hits","effMenu->selectingHist(1)",oX+dX*3,oY+dY*13,oX+dX*3+wX,oY+dY*13+hY);
  drawButton(bHist[1],42,indHist[1]*5+1);
  bHist[2]=new TButton("Efficiency","effMenu->selectingHist(2)",oX+dX*1,oY+dY*13,oX+dX*1+wX,oY+dY*13+hY);
  drawButton(bHist[2],42,indHist[2]*5+1);
  bHist[3]=new TButton("Single","effMenu->selectingHist(3)",oX+dX*0,oY+dY*12,oX+dX*0+wX,oY+dY*12+hY);
  drawButton(bHist[3],42,indHist[3]*5+1);
  bHist[4]=new TButton("Probability","effMenu->selectingHist(4)",oX+dX*1,oY+dY*12,oX+dX*1+wX,oY+dY*12+hY);
  drawButton(bHist[4],42,indHist[4]*5+1);
  bHistMb=new TButton("Eff.vs.Mb","effMenu->selectingHistMb()",oX+dX*0,oY+dY*10.5,oX+dX*0+wX,oY+dY*10.5+hY);
  drawButton(bHistMb,42,indHistMb*5+1);

  // Buttons for XY plots:
  bXYlayer=new TButton("Eff(xy)","effMenu->selectingXYlayer()",oX+dX*0,oY+dY*9,oX+dX*0+wX,oY+dY*9+hY);
  drawButton(bXYlayer, 42,indXYlayers*5+1);

  dX=0.96/10.; wX=dX-0.01; oX=0.02;
  button[0]=new TButton("-1","effMenu->selectingSumXY(-1)",oX+dX*6,oY+dY*9,oX+dX*6+wX,oY+dY*9+hY);
  drawButton(button[0],42,4);
  sprintf(buf1,"%i",sumBn);
  pvLSumXY=new TPaveLabel(oX+dX*7,oY+dY*9,oX+dX*7+wX,oY+dY*9+hY,buf1,"L");
  pvLSumXY->SetTextSize(1.2);
  pvLSumXY->SetTextColor(4);
  pvLSumXY->SetFillColor(10);
  pvLSumXY->SetTextFont(22);
  pvLSumXY->SetTextAlign(22);
  pvLSumXY->SetBorderSize(5);
  pvLSumXY->Draw();
  button[1]=new TButton("+1","effMenu->selectingSumXY(+1)",oX+dX*8,oY+dY*9,oX+dX*8+wX,oY+dY*9+hY);
  drawButton(button[1],42,4);


  dX=0.96/10.; wX=dX-0.01; oX=0.02;
  button[2]=new TButton("-5","effMenu->selectingSum(-5)",oX+dX*5,oY+dY*12,oX+dX*5+wX,oY+dY*12+hY);
  drawButton(button[2], 42,4);
  button[3]=new TButton("-1","effMenu->selectingSum(-1)",oX+dX*6,oY+dY*12,oX+dX*6+wX,oY+dY*12+hY);
  drawButton(button[3],42,4);
  sprintf(buf1,"%i",sumBn);
  pvLSum=new TPaveLabel(oX+dX*7,oY+dY*12,oX+dX*7+wX,oY+dY*12+hY,buf1,"L");
  pvLSum->SetTextSize(1.2);
  pvLSum->SetTextColor(4);
  pvLSum->SetFillColor(10);
  pvLSum->SetTextFont(22);
  pvLSum->SetTextAlign(22);
  pvLSum->SetBorderSize(5);
  pvLSum->Draw();
  button[4]=new TButton("+1","effMenu->selectingSum(+1)",oX+dX*8,oY+dY*12,oX+dX*8+wX,oY+dY*12+hY);
  drawButton(button[4],42,4);
  button[5]=new TButton("+5","effMenu->selectingSum(+5)",oX+dX*9,oY+dY*12,oX+dX*9+wX,oY+dY*12+hY);
  drawButton(button[5],42,4);

  // Buttons for times:
  dX=0.96/4; wX=0.27; oX=0.02;
  bTimes=new TButton("Time 1,2,2-1","effMenu->drawTimes()",oX+dX*0,oY+dY*7,oX+dX*0+wX,oY+dY*7+hY);
  drawButton(bTimes, 42,indTimes*5+1);

  dX=0.96/10.; wX=dX-0.01; oX=0.02;
  button[6]=new TButton("-5","effMenu->selectingSumTimes(-5)",oX+dX*5,oY+dY*7,oX+dX*5+wX,oY+dY*7+hY);
  drawButton(button[6], 42,4);
  button[7]=new TButton("-1","effMenu->selectingSumTimes(-1)",oX+dX*6,oY+dY*7,oX+dX*6+wX,oY+dY*7+hY);
  drawButton(button[7],42,4);
  sprintf(buf1,"%i",sumBn);
  pvLSumTimes=new TPaveLabel(oX+dX*7,oY+dY*7,oX+dX*7+wX,oY+dY*7+hY,buf1,"L");
  pvLSumTimes->SetTextSize(1.2);
  pvLSumTimes->SetTextColor(4);
  pvLSumTimes->SetFillColor(10);
  pvLSumTimes->SetTextFont(22);
  pvLSumTimes->SetTextAlign(22);
  pvLSumTimes->SetBorderSize(5);
  pvLSumTimes->Draw();
  button[8]=new TButton("+1","effMenu->selectingSumTimes(+1)",oX+dX*8,oY+dY*7,oX+dX*8+wX,oY+dY*7+hY);
  drawButton(button[8],42,4);
  button[9]=new TButton("+5","effMenu->selectingSumTimes(+5)",oX+dX*9,oY+dY*7,oX+dX*9+wX,oY+dY*7+hY);
  drawButton(button[9],42,4);

  // Buttons for plots:
  dX=0.28; wX=0.27; oX=0.02;
  text->DrawText(oX+dX*0+0.02,oY+dY*4+dY/5.,"Plots:");
  bTypePlot[0]=new TButton("A","effMenu->selectingTypePlot(0)",oX+dX*1,oY+dY*5,oX+dX*1+dX/3.-0.01,oY+dY*5+hY);
  drawButton(bTypePlot[0],42,(typePlot==0)?3:1);
  bTypePlot[1]=new TButton("B","effMenu->selectingTypePlot(1)",oX+dX*1+dX/3.,oY+dY*5,oX+dX*1+dX*2./3.-0.01,oY+dY*5+hY);
  drawButton(bTypePlot[1],42,(typePlot==1)?3:1);
  bTypePlot[2]=new TButton("C","effMenu->selectingTypePlot(2)",oX+dX*1+dX*2./3.,oY+dY*5,oX+dX*1+dX-0.01,oY+dY*5+hY);
  drawButton(bTypePlot[2],42,(typePlot==2)?3:1);
  bPlot[2]=new TButton("Event","effMenu->selectingPlot(2)",oX+dX*2,oY+dY*5,oX+dX*2+wX,oY+dY*5+hY);
  drawButton(bPlot[2],42,indPlot[0]*5+1);
  bPlot[0]=new TButton("Projections","effMenu->selectingPlot(0)",oX+dX*1,oY+dY*4,oX+dX*1+wX,oY+dY*4+hY);
  drawButton(bPlot[0],42,indPlot[0]*5+1);
  bPlot[1]=new TButton("MDC hits","effMenu->selectingPlot(1)",oX+dX*2,oY+dY*4,oX+dX*2+wX,oY+dY*4+hY);
  drawButton(bPlot[1],42,indPlot[1]*5+1);
  button[10]=new TButton("Print clusters","effMenu->printClusters()",oX+dX*1,oY+dY*3,oX+dX*1+wX+wX/2.,oY+dY*3+hY);
  drawButton(button[10],42,1);

  // Buttons for event loop:
  dX=0.16; wX=0.15; oX=0.02;
  text->DrawText(oX+dX*0+0.02,oY+dY*1+dY/5.,"Event loop:");
  button[11]=new TButton("Next","effMenu->doEvents(1)",oX+dX*2,oY+dY*1,oX+dX*2+wX,oY+dY*1+hY);
  drawButton(button[11]);
  button[12]=new TButton("All","effMenu->doEvents(-1)",oX+dX*3,oY+dY*1,oX+dX*3+wX,oY+dY*1+hY);
  drawButton(button[12]);
  button[13]=new TButton("10","effMenu->doEvents(10)",oX+dX*0,oY+dY*0,oX+dX*0+wX,oY+dY*0+hY);
  drawButton(button[13]);
  button[14]=new TButton("100","effMenu->doEvents(100)",oX+dX*1,oY+dY*0,oX+dX*1+wX,oY+dY*0+hY);
  drawButton(button[14]);
  button[15]=new TButton("1000","effMenu->doEvents(1000)",oX+dX*2,oY+dY*0,oX+dX*2+wX,oY+dY*0+hY);
  drawButton(button[15]);
  button[16]=new TButton("10000","effMenu->doEvents(10000)",oX+dX*3,oY+dY*0,oX+dX*3+wX,oY+dY*0+hY);
  drawButton(button[16]);

  button[17]=new TButton("Refresh","effMenu->calcEff()",0.80,oY+dY*2,0.99,oY+dY*2+hY);
  drawButton(button[17]);

//  button[18]=new TButton("Quit","HMdcEffMenu::deleteMenu(); delete gHades;",0.75,oY+dY*1,0.99,oY+dY*1+hY);
  button[18]=new TButton("Quit","HMdcEffMenu::closeDial(); delete gHades;",0.75,oY+dY*1,0.99,oY+dY*1+hY);
//  button[18]=new TButton("Quit","effMenu->quitMacro()",0.75,oY+dY*1,0.99,oY+dY*1+hY);
  drawButton(button[18]);
  button[19]=new TButton("Quit ROOT",".q", 0.75,oY+dY*0,0.99,oY+dY*0+hY);
  drawButton(button[19]);

  dialog->cd();
}

void HMdcEffMenu::calcEff(void) {
  mdcEff->calcEfficiency(sumBn);
}

void HMdcEffMenu::drawButton(TButton* bt, Int_t nFnt=42, Int_t nTCl=1){
  bt->SetTextFont(nFnt);
  bt->SetTextColor(nTCl);
  bt->SetFillColor(16);
  bt->SetBorderSize(1);
  bt->Draw();
}

void HMdcEffMenu::selectingSum(Int_t n){
 if(sumBn==1 && n<0) return;
 sumBn+=n;
 if(sumBn<1) sumBn=1;
 sprintf(buf1,"%i",sumBn);
 pvLSum->SetLabel(buf1);
 dialog->Modified();
 mdcEff->calcEfficiency(sumBn);
 doDraw();
}

void HMdcEffMenu::selectingSumTimes(Int_t n){
 if(sumBnTimes==1 && n<0) return;
 sumBnTimes+=n;
 if(sumBnTimes<1) sumBnTimes=1;
 if(sumBnTimes>100) sumBnTimes=100;
 sprintf(buf1,"%i",sumBnTimes);
 pvLSumTimes->SetLabel(buf1);
 dialog->Modified();
 doDraw();
}

void HMdcEffMenu::selectingSumXY(Int_t n) {
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

void HMdcEffMenu::printClusters(){
  for(Int_t sec=0; sec<6; sec++) {
    if(indSec[sec]) {
      printf("\n---------------- Sector %i ---------------------------\n",sec+1);
      mdcEff->printClusters(sec); // mod ???
    }
  }
}

void HMdcEffMenu::doDraw(Int_t ind=1){
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

void HMdcEffMenu::doPlots(Int_t ind=1){
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

void HMdcEffMenu::doEventPlots(Int_t ind=1){
 //ind=1 - draw canvas
 //   =0 - close canvas
  if(ind) {mdcEff->drawCanvEvent(); mdcEff->drawPrPlEvent(typePlot);}
  else mdcEff->delCanvEvent();
}

void HMdcEffMenu::doEvents(Int_t nEv) {
 Int_t nEvDo=(nEv/10==0) ? 1 : nEv/10; 
 Int_t nfor=nEv/nEvDo;
 if(nEv==-1) {nEvDo=10000; nfor=1;}
 if(totModsSel == 0 && nEv>0 && indPlot[2]==0) {
   nEvDo=nEv;
   nfor=1;
 }
 Int_t evN=0;
 for(Int_t nTm=0; nTm<nfor; nTm++) {
  evN=gHades->eventLoop(nEvDo);
  mdcEff->calcEfficiency(sumBn);
  if(evN<nEvDo) break;
  if(evN<=0) break;
  if(nEv==-1) nTm=-1;
  if(evN>0) nTotEv+=evN;
  if(evN > 0 && nTm<nfor-1 && nEvDo>100) 
    printf("--- Total num. of events processed: %i ---\n",nTotEv);
 }
 if(evN > 0) printf("======== Total num. of events processed: %i ========\n",nTotEv);
 else if(evN < 0) printf("======== The error of reading event. evN=%i\n",evN);
 if(evN>=0 && evN<nEvDo) printf(
 "======== End of file! Total num. of events processed: %i ========\n",nTotEv);
}

ClassImp(HMdcEffMenu)
