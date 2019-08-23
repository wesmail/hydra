#include <stdlib.h>
#include "hmdcgeomstruct.h"
#include "hmdcefficiency.h"
#include "hmdcdef.h"
#include "hdebug.h"
#include "hades.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hevent.h"
#include "hlocation.h"
#include "hgeomvector.h"
#include "hgeantmdc.h"
#include "hgeantkine.h"
#include "heventheader.h"
#include "../start/hstartunpacker.h"
#include "TMath.h"
#include "TLine.h"
#include "TText.h"
#include "TStyle.h"
#include "TMarker.h"
#include "TPaveLabel.h"
#include "TColor.h"
#include "hmdcgetcontainers.h"
#include "hmdccal1sim.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TSlider.h"
#include "hmdclookupgeom.h"
#include "hmdctimecut.h"
#include "hcategory.h"
#include "hmdclookuptb.h"
#include "hmdcrawstruct.h"
#include "hmdcdetector.h"
#include "hmdcgeompar.h"
#include "hmdcdetector.h"
#include "hmdcsizescells.h"

//*-- Author : V.Pechenov
//*-- Modified : 10.05.2001 by V. Pechenov
//*-- Modified : 10.05.2000 by V. Pechenov

///////////////////////////////////////////////////////////////
//  HMdcEfficiency
//
// Estimation of MDC efficiency.
// Use macros mdcEfficiency.C
// 
///////////////////////////////////////////////////////////////


ClassImp(HMdcEff)
ClassImp(HMdcEffMod)
ClassImp(HMdcEfficiency)
ClassImp(HMdcEfficiencySec)
ClassImp(HMdcEfficiencyMod)
ClassImp(HMdcEfficiencyLayer)

// !!! mishen', dlya magn.polya budet ...
//

void HMdcEff::calcEff(Double_t iN0,     Double_t eN0,
                      Double_t iN1,     Double_t eN1,
                      Double_t iN2m,    Double_t eN2m,
		      Double_t iN2b=0., Double_t eN2b=0.) {
  n0=iN0;
  n1=iN1;
  n2m=iN2m;
  n2b=iN2b;
  errN0=eN0;
  errN1=eN1;
  errN2m=eN2m;
  errN2b=eN2b;
  totNumTr=n0+n1+n2m+n2b;

  calculation();
}

Double_t HMdcEff::efficiency(Double_t n0T, Double_t n1T, 
                             Double_t n2mT, Double_t n2bT=0.) {
  Double_t eC=n2mT+0.5*n2bT;
  Double_t eB=n1T+2.*eC;
  Double_t eA=n0T+n1T+eC;
  if( eA == 0. ) return 0.;
  Double_t sq=eB*eB-4*eA*eC;
  if( sq < 0. ) sq=0.;
  return (eB+sqrt(sq))/(2.*eA);
}

Double_t HMdcEff::fractSingle(Double_t effT, Double_t n0T, Double_t n1T, 
                             Double_t n2mT, Double_t n2bT=0.) { 
  Double_t b=n1T+2.*n2mT+n2bT;
  if( b == 0.0 ) return 0.;
  Double_t tmp=effT*b-n2mT;
  if( tmp < n2mT+n2bT ) return 0.;
  return 1.-(n2mT+n2bT)/tmp; 
}

void HMdcEff::calculation() {
      
  // Determination of M coefficient
  eM=1.;
  errM=0.;
  if(n2m+n2b>0) {
    eM=1.-0.5*n2b/(n2m+n2b);
    Double_t dMdN2m=+0.5*n2b/((n2m+n2b)*(n2m+n2b));
    Double_t dMdN2b=-0.5*n2m/((n2m+n2b)*(n2m+n2b));
    errM=sqrt(dMdN2b*errN2b*dMdN2b*errN2b+dMdN2m*errN2m*dMdN2m*errN2m);
  }
 
  eff=efficiency(n0,n1,n2m,n2b);
  single=fractSingle(eff,n0,n1,n2m,n2b);
  p0=single*(1.-eff)+(1.-single)*(1.-eff)*(1.-eff)*eM;
  p1=single*eff+(1.-single)*eff*(1.-eff)*2.*eM;
  p2=(1.-single)*eff*eff;
  errEff=0.;
  errSingle=0.;
  errP0=errP1=errP2=0.;
  Double_t dEn[4],dSn[4],dP0[4],dP1[4],dP2[4];
  for(Int_t i=0; i<4; i++) dEn[i]=dSn[i]=dP0[i]=dP1[i]=dP2[i]=0.;
  if(n0+n1+n2m+n2b > 0) {
    for(Int_t i0=-1; i0<=1; i0++) {
      for(Int_t i1=-1; i1<=1; i1++) {
        for(Int_t i2m=-1; i2m<=1; i2m++) {
          for(Int_t i2b=-1; i2b<=1; i2b++) {
            Double_t shN0=n0+errN0*i0;
            if(shN0<0.) continue;
            Double_t shN1=n1+errN1*i1;
            if(shN1<0.) continue;
            Double_t shN2m=n2m+errN2m*i2m;
            if(shN2m<0.) continue;
            Double_t shN2b=n2b+errN2b*i2b;
            if(shN2b<0.) continue;
            //Maximal error:
            Double_t shEff=efficiency(shN0,shN1,shN2m,shN2b);
            errEff=TMath::Max(TMath::Abs(shEff-eff),errEff);
            Double_t shSingle=fractSingle(shEff,shN0,shN1,shN2m,shN2b);
            if(shN1+shN2m+shN2b > 0.) {
              errSingle=TMath::Max(TMath::Abs(shSingle-single),errSingle);
            }
            Double_t shM=(shN2m+shN2b>0) ? shM=1.-0.5*shN2b/(shN2m+shN2b) : 1.;
            Double_t shP0=shSingle*(1.-shEff)+
                          (1.-shSingle)*(1.-shEff)*(1.-shEff)*shM;
            errP0=TMath::Max(TMath::Abs(shP0-p0),errP0);
            Double_t shP1=shSingle*shEff+(1.-shSingle)*shEff*(1.-shEff)*2.*shM;
            errP1=TMath::Max(TMath::Abs(shP1-p1),errP1);
            Double_t shP2=(1.-shSingle)*shEff*shEff;
            errP2=TMath::Max(TMath::Abs(shP2-p2),errP2);
            
            Int_t n=abs(i0)+abs(i1)+abs(i2m)+abs(i2b);
            if(n==1) {
              Int_t i=abs(i0)*0+abs(i1)*1+abs(i2m)*2+abs(i2b)*3;
              dEn[i]=TMath::Max((shEff-eff)*(shEff-eff),dEn[i]);
              dSn[i]=TMath::Max((shSingle-single)*(shSingle-single),dSn[i]);
              dP0[i]=TMath::Max((shP0-p0)*(shP0-p0),dP0[i]);
              dP1[i]=TMath::Max((shP1-p1)*(shP1-p1),dP1[i]);
              dP2[i]=TMath::Max((shP2-p2)*(shP2-p2),dP2[i]);
            }
          }
        }
      }
    }
  }
  
  Double_t errEffN=sqrt(dEn[0]+dEn[1]+dEn[2]+dEn[3]);
  Double_t errSingleN=sqrt(dSn[0]+dSn[1]+dSn[2]+dSn[3]);
  Double_t errP0N=sqrt(dP0[0]+dP0[1]+dP0[2]+dP0[3]);
  Double_t errP1N=sqrt(dP1[0]+dP1[1]+dP1[2]+dP1[3]);
  Double_t errP2N=sqrt(dP2[0]+dP2[1]+dP2[2]+dP2[3]);
  if(errEffN<errEff) errEff=(errEff+errEffN)*0.5;
  if(errSingleN<errSingle) errSingle=(errSingle+errSingleN)*0.5;
  if(errP0N<errP0) errP0=(errP0+errP0N)*0.5;
  if(errP1N<errP1) errP1=(errP1+errP1N)*0.5;
  if(errP2N<errP2) errP2=(errP2+errP2N)*0.5;
  
/*  errEff=TMath::Min(errEff, sqrt(dEn[0]+dEn[1]+dEn[2]+dEn[3]));
  errSingle=TMath::Min(errSingle,sqrt(dSn[0]+dSn[1]+dSn[2]+dSn[3]));
  errP0=TMath::Min(errP0,sqrt(dP0[0]+dP0[1]+dP0[2]+dP0[3]));
  errP1=TMath::Min(errP1,sqrt(dP1[0]+dP1[1]+dP1[2]+dP1[3]));
  errP2=TMath::Min(errP2,sqrt(dP2[0]+dP2[1]+dP2[2]+dP2[3])); */
  if(n2b > 0.) totNumTr=totNumTr/(p0+p1+p2);
}

Double_t HMdcEffMod::efficiency(Double_t n0T, Double_t n1T, 
                             Double_t n2T, Double_t n2bT=0.) {
  Double_t eA=(n1T+n2T)*(n1T+n2T-4.*n0T);
  if( eA == 0. ) return 0.;
  Double_t eB=(2.*n2T+n1T)*(n1T+n2T-5.*n0T);
  Double_t eC=n2T*(n1T+n2T-5.*n0T);
  Double_t sq=eB*eB-4*eA*eC;
  if( sq < 0. ) sq=0.;
  return (eB+sqrt(sq))/(2.*eA);
}

void HMdcEffMod::calcEff(Double_t iN0,     Double_t eN0,
			 Double_t iN1,     Double_t eN1,
 			 Double_t iN2m,    Double_t eN2m,
 			 Double_t iN2b=0., Double_t eN2b=0.) {
  n0=iN0;
  n1=iN1;
  n2m=iN2m;
  n2b=iN2b;
  errN0=eN0;
  errN1=eN1;
  errN2m=eN2m;
  errN2b=eN2b;

  totNumTr=(n0+n1+n2m)/6.;

  calculation();

  Double_t p12=p1+p2;
  p6l=p12 *p12 *p12 *p12 *p12 *p12;
  p5l=p12 *p12 *p12 *p12 *p12 *p0   *6.;
  p4l=p12 *p12 *p12 *p12 *p0  *p0  *15.;
  p3l=p12 *p12 *p12 *p0  *p0  *p0  *20.;
  p2l=p0  *p0  *p0  *p0  *p12 *p12 *15.;
  p1l=p0  *p0  *p0  *p0  *p0  *p12  *6.;
  p0l=p0  *p0  *p0  *p0  *p0  *p0;
}


HMdcEfficiencyLayer::HMdcEfficiencyLayer(Short_t sec, Short_t mod,
 					 Short_t lay) {
  // constructor creates an array of pointers of type
  // HMdcEfficiencyLayer
  sector=sec;
  module=mod;
  layer=lay;
  is=0;
  // Geting of pointers to par. cont.
  HMdcGetContainers* fGetCont=HMdcGetContainers::getObject();
  if( !fGetCont ) return;
  HMdcGeomStruct* fGeomStr=fGetCont->getMdcGeomStruct();
  if ( fGeomStr ) {
    nCells=((*fGeomStr)[sector][module])[layer];
    if( nCells > 0 ) {
      is=1;
      n0=new Int_t [nCells];
      n1=new Int_t [nCells];
      n2=new Int_t [nCells];
      nCellTot=new Int_t [nCells];
      nCellCutS=new Int_t [nCells];
      nCellStat=new Int_t [nCells];
      nCellsClus=new Int_t [nCells];
      nClustCell=new UChar_t [nCells];
      clear();
      createHists();
      for(Int_t nP=0; nP<6; nP++) listPads[nP]=1;
      nPads=6;
      sprintf(nameCanv,"Cn_S%iM%iL%iEff",sector+1,module+1,layer+1);
      sprintf(titleCanv,"S%i M%i L%i Efficiency",sector+1,module+1,layer+1);
      for(Int_t nPlXY=0; nPlXY<3; nPlXY++) {
        titlePadXY[nPlXY].SetX1(0.05);
        titlePadXY[nPlXY].SetX2(0.6);
        titlePadXY[nPlXY].SetY1(0.955);
        titlePadXY[nPlXY].SetY2(0.995);
        titlePadXY[nPlXY].SetTextAlign(22);
        titlePadXY[nPlXY].SetTextSize(0);
        titlePadXY[nPlXY].SetBorderSize(1);
        titlePadXY[nPlXY].SetFillColor(7);
      }
    }
  }
}

HMdcEfficiencyLayer::~HMdcEfficiencyLayer() {
  delete [] n0;
  delete [] n1;
  delete [] n2;
  delete [] nCellTot;
  delete [] nCellCutS;
  delete [] nCellStat;
  delete [] nClustCell;
  delete [] nCellsClus;
  TSeqCollection* fSegColl=gROOT->GetListOfCanvases();
  canvEff=(TCanvas*)fSegColl->FindObject(nameCanv);
  if( canvEff ) {
    delete canvEff;
    canvEff=0;
  }
  for(Int_t i=0; i<3; i++) {
    canvXY[i]=(TCanvas*)fSegColl->FindObject(nameCanvXY[i]);
    if( canvXY[i] ) {
      delete canvXY[i];
      canvXY[i]=0;
    }
  }
}

void HMdcEfficiencyLayer::clear() {
  for (Short_t i=0; i<nCells; i++) {
    n0[i]=n1[i]=n2[i]=0;
    nCellStat[i]=nCellCutS[i]=nCellTot[i]=nCellsClus[i]=0;
  }
  for (Short_t xy=0; xy<256; xy++) n0xy[xy]=n1xy[xy]=n2xy[xy]=0;
}

void HMdcEfficiencyLayer::createHists() {
  Char_t* titH[13];
  Char_t* nmH[13];
  titH[0]="The total num. of hits";
  nmH[0] ="TNhits";
  titH[1]="The total num. of hits (no cut)";
  nmH[1] ="TNhitsNoCut";
  titH[2]="The num. of selected tracks crossing cell(s) vs. cell";
  nmH[2] ="TNtracks";
  titH[3]="The num. of \"zero\" hits";
  nmH[3] ="N0hits";
  titH[4]="The num. of \"single\" hits";
  nmH[4] ="N1hits";
  titH[5]="The num. of \"double\" hits";
  nmH[5] ="N2hits";
  titH[6]="The efficiency of wires";
  nmH[6] ="Eff";
  titH[7]="The fraction of single hits";
  nmH[7] ="Sing";
  titH[8]="The probability to have \"zero\" hit";
  nmH[8] ="P0";
  titH[9]="The probability to have \"single\" hit";
  nmH[9] ="P1";
  titH[10]="The probability to have \"double\" hit";
  nmH[10] ="P2";
  titH[11]="The total num. of hits (cut time1&time2)";
  nmH[11] ="TNhitsCutT1T2"; 
  titH[12]="The hits from clusters";
  nmH[12] ="TNhitsClust";
  Float_t minimum[13] ={  0.,  0., 0., 0., 0., 0., .0,  .0, .0, .0, .0,  0.,  0.};
  Float_t maximim[13] ={ -1., -1.,-1.,-1.,-1.,-1.,1.1, 1.1,1.1,1.1,1.1, -1., -1.};
  Int_t markStyle[13] ={  -1,  -1, -1, -1, -1, -1, 21,  21, 21, 21, 21,  -1,  -1};
  Float_t markSize[13]={ 0.4, 0.4,0.4,0.4,0.4,0.4,0.4, 0.4,0.4,0.4,0.4, 0.4, 0.4};
  Int_t markColor[13] ={  -1,  -1, -1, -1, -1, -1,  4,   4,  2,  4,  3,  -1,  -1};
  Int_t fillStyle[13] ={1001,1001,  0,  0,  0,  0,  0,   0,  0,  0,  0,1001,   0};
  Text_t name[20];
  Text_t title[100];
  for(Int_t nHist=0; nHist<13; nHist++) {
    sprintf(name,"S%iM%iL%i%s",sector+1,module+1,layer+1,nmH[nHist]);
    sprintf(title,"S%i M%i L%i %s",sector+1,module+1,layer+1,titH[nHist]);
    hists[nHist]=new TH1F(name,title,10,0.,1.);
    hists[nHist]->SetMinimum(minimum[nHist]);
    if(maximim[nHist]>0.) hists[nHist]->SetMaximum(maximim[nHist]);
    if(markStyle[nHist]>0) {
      hists[nHist]->SetMarkerStyle(markStyle[nHist]);
      hists[nHist]->SetMarkerSize(markSize[nHist]);
      hists[nHist]->SetMarkerColor(markColor[nHist]);
    }
    if(nHist>4&&nHist<11) hists[nHist]->SetOption("e1p");
    hists[nHist]->SetFillStyle(fillStyle[nHist]);
  }
  sprintf(nameCanvXY[0],"S%iM%iL%inHitsXY",sector+1,module+1,layer+1);
  sprintf(nameCanvXY[1],"S%iM%iL%iEffXY",sector+1,module+1,layer+1);
  sprintf(nameCanvXY[2],"S%iM%iL%iSinXY",sector+1,module+1,layer+1);
  sprintf(titleCanvXY[0],"S%i M%i L%i The num. of hits vs XY",
                               sector+1,module+1,layer+1);
  sprintf(titleCanvXY[1],"S%i M%i L%i The efficiency vs XY",
                               sector+1,module+1,layer+1);
  sprintf(titleCanvXY[2],"S%i M%i L%i The fraction of single hits vs XY",
                               sector+1,module+1,layer+1);  
  setBiningXY(1);
}

void HMdcEfficiencyLayer::clearClCells() {
  for (Short_t i=0; i<nCells; i++) nClustCell[i]=0;
}

void HMdcEfficiencyLayer::addCellInClust(Short_t cell) {
  if(cell>=0 && cell<nCells) nClustCell[cell]++;
}

void HMdcEfficiencyLayer::addCellTot(Short_t cell) {
  if(cell>=0 && cell<nCells) nCellTot[cell]++;
}

void HMdcEfficiencyLayer::addCellCutS(Short_t cell) {
  if(cell>=0 && cell<nCells) nCellCutS[cell]++;
}

void HMdcEfficiencyLayer::addCellClus(Short_t cell) {
  if(cell>=0 && cell<nCells) nCellsClus[cell]++;
}

void HMdcEfficiencyLayer::addCell(Short_t cell) {
  if(cell>=0 && cell<nCells) {
   nClustCell[cell]++;
   nCellStat[cell]++;
  }
}

Int_t HMdcEfficiencyLayer::getCellStat(Short_t cell) {
  if(cell>=0 && cell<nCells) return nCellStat[cell];
  return 0;
}

void HMdcEfficiencyLayer::add0Hit(Short_t cell, Int_t nB) {
  if(cell>=0 && cell<nCells) n0[cell]++;
  n0xy[nB]++;
}

void HMdcEfficiencyLayer::add1Hit(Short_t cell, Int_t nB) {
  if(cell>=0 && cell<nCells) n1[cell]++;
  n1xy[nB]++;
}

void HMdcEfficiencyLayer::add2Hit(Short_t cell1, Short_t cell2, Int_t nB) {
  if(cell1>=0 && cell1<nCells && cell2>=0 && cell2<nCells) {
   n2[(cell1>cell2) ? cell1 : cell2]++;
  }
  n2xy[nB]++;
}

void HMdcEfficiencyLayer::getEffCells(HMdcEff* efficiency,
                                      Short_t fCell=-1000, Short_t lCell=1000) {
  // Calculating of avarage value of efficiency (eff +/- errEff)
  // and fraction of singles hits (singles +/- errSingles)
  // for cells  fCell, fCell+1, ... lCell (for lCell-fCell+1 cells).
  //
  // getEfficiency(fCell) -
  // - for cell number fCell
  //
  // getEfficiency() -
  // - for all cells in layers.


  // Finding of working wires region
  Short_t fCellN=0;
  Short_t lCellN=nCells-1;
  for(Short_t cell=0; cell<nCells; cell++) {
    if(nCellStat[cell]>0) {
      if(nCellStat[cell]>10) fCellN=cell;
      break;
    }
  }
  for(Short_t cell=lCellN; cell>=0; cell--) {
    if(nCellStat[cell]>0) {
      if(nCellStat[cell]>10) lCellN=cell;
      break;
    }
  }

  Short_t fC=(fCell<fCellN) ? fCellN : fCell;
  Short_t lC=(lCell>lCellN) ? lCellN : lCell;
  if(fCell!=-1000 && lCell == 1000) lC=fC=fCell;
  Int_t nHits,nHitsNoCut,nHitsCutS,nHitsClus,nC0,nC1,nC2b,nC2m;
  nHits=nHitsNoCut=nHitsCutS=nHitsClus=nC0=nC1=nC2b=nC2m=0;
  Double_t eN0,eN1,eN2m,eN2b;
  eN0=eN1=eN2m=eN2b=0.;
  if(fC>=fCellN && lC<=lCellN) {
    for (Int_t cell=fC; cell<=lC; cell++) {
      nHits+=nCellStat[cell];
      nHitsNoCut+=nCellTot[cell];
      nHitsCutS+=nCellCutS[cell];
      nHitsClus+=nCellsClus[cell];
      nC0+=n0[cell];
      nC1+=n1[cell];
      if(cell>fC) nC2m+=n2[cell];
    }
    if(fC>fCellN) nC2b+=n2[fC];
    if(lC<lCellN) nC2b+=n2[lC+1];
    eN0=(nC0>0) ? sqrt((Double_t)nC0) : 1.;
    eN1=(nC1>0) ? sqrt((Double_t)nC1) : 1.;
    if(fC != lC) eN2m=(nC2m>0) ? sqrt((Double_t)nC2m) : 1.;
    if(fC>fCellN || lC<lCellN) eN2b=(nC2b>0) ? sqrt((Double_t)nC2b) : 1.;
  }

  efficiency->setTotNhits((Float_t)nHits);
  efficiency->setTotNhitsCutS((Float_t)nHitsCutS);
  efficiency->setTotNhitsNoCut((Float_t)nHitsNoCut);
  efficiency->setTotNhitsClus((Float_t)nHitsClus);
  efficiency->calcEff((Double_t)nC0, eN0,  (Double_t)nC1,  eN1,
 		      (Double_t)nC2m,eN2m, (Double_t)nC2b,eN2b);
}

void HMdcEfficiencyLayer::getEffXY(HMdcEff* efficiency) {
  // Calculation of efficiency of MDC layer.
  getEffXY(efficiency,0,15,0,15);
}

void HMdcEfficiencyLayer::getEffXY(HMdcEff* efficiency,
                              Short_t xBin, Short_t yBin){
  // Calculation of efficiency of MDC for bin (xBin:yBin).
  getEffXY(efficiency,xBin,xBin,yBin,yBin);
}

void HMdcEfficiencyLayer::getEffXY(HMdcEff* efficiency,
                                   Short_t fXbin, Short_t lXbin,
                                   Short_t fYbin, Short_t lYbin){
  // Calculation of efficiency of MDC ragion
  // (MDC is divided on 16x16 bins).
  // Region: fXbin <= x <= lXbin, fYbin <= y <= lYbin
  Int_t nC0,nC1,nC2;
  nC0=nC1=nC2=0;
  Double_t eN0,eN1,eN2;
  eN0=eN1=eN2=0.;

  Int_t fXb=(fXbin < 0 ) ?  0 : fXbin;
  Int_t lXb=(lXbin > 15) ? 15 : lXbin;
  Int_t fYb=(fYbin < 0 ) ?  0 : fYbin;
  Int_t lYb=(lYbin > 15) ? 15 : lYbin;
  for(Int_t y=fYb; y<=lYb; y++) {
    Int_t nBin=y*16;
    for(Int_t x=fXb; x<=lXb; x++){
      nC0+=n0xy[nBin+x];
      nC1+=n1xy[nBin+x];
      nC2+=n2xy[nBin+x];
    }
  }
  if(nC0+nC1+nC2 > 0) {
    eN0=(nC0>0) ? sqrt((Double_t)nC0) : 1.;
    eN1=(nC1>0) ? sqrt((Double_t)nC1) : 1.;
    eN2=(nC2>0) ? sqrt((Double_t)nC2) : 1.;
  }
  efficiency->calcEff((Double_t)nC0,eN0, (Double_t)nC1,eN1, (Double_t)nC2,eN2);
}

void HMdcEfficiencyLayer::fillHists(Int_t iBinSum=1) {
  nBinSum=(iBinSum>0) ? iBinSum : 1;
  Int_t nBtot=(nCells-1)/nBinSum+1;
  for(Int_t nH=0; nH<13; nH++) {
    if(nBtot != hists[nH]->GetNbinsX())
 			hists[nH]->SetBins(nBtot,0.,(Float_t)(nBtot*nBinSum));
  }
  Float_t max0,max1;
  max0=max1=0.;
  for (Int_t bin=0; bin<nBtot; bin++) {
    getEffCells(&eff, bin*nBinSum, bin*nBinSum+nBinSum-1);
    hists[0]->SetBinContent(bin+1,eff.getTotNhits());
    hists[1]->SetBinContent(bin+1,eff.getTotNhitsNoCut());
    hists[11]->SetBinContent(bin+1,eff.getTotNhitsCutS());
    hists[12]->SetBinContent(bin+1,eff.getTotNhitsClus());
    hists[2]->SetBinContent(bin+1,eff.getTotNumTracks());
    hists[3]->SetBinContent(bin+1,eff.getN0());
    hists[4]->SetBinContent(bin+1,eff.getN1());
    hists[5]->SetBinContent(bin+1,eff.getN2());
    hists[6]->SetBinContent(bin+1,eff.getEfficiency());
    hists[6]->SetBinError(bin+1,eff.getErrEfficiency());
    hists[7]->SetBinContent(bin+1,eff.getFracSingle());
    hists[7]->SetBinError(bin+1,eff.getErrFracSingle());
    hists[8]->SetBinContent(bin+1,eff.getP0());
    hists[8]->SetBinError(bin+1,eff.getErrP0());
    hists[9]->SetBinContent(bin+1,eff.getP1());
    hists[9]->SetBinError(bin+1,eff.getErrP1());
    hists[10]->SetBinContent(bin+1,eff.getP2());
    hists[10]->SetBinError(bin+1,eff.getErrP2());
//    if(eff.getTotNhitsNoCut()>max0) max0=eff.getTotNhitsNoCut();
    if(eff.getTotNhitsCutS()>max0) max0=eff.getTotNhitsCutS(); // max after cut "common stop"
    if(eff.getTotNumTracks()>max1) max1=eff.getTotNumTracks();
  }
  hists[1]->SetMaximum(max0+0.1*max0);
  for(Int_t n=2; n<6; n++) hists[n]->SetMaximum(max1+0.1*max1);
  getEffCells(&eff);
}


void HMdcEfficiencyLayer::setTitle(TH1F* &hist, Char_t* nm){
  Char_t title[50];
  sprintf(title,"%s vs %i cell(s)",nm,nBinSum);
  hist->SetYTitle(title);
  hist->GetYaxis()->CenterTitle(1);
  hist->SetTitleOffset(0.6,"Y");
  hist->GetXaxis()->SetLabelSize(0.07);
  hist->GetYaxis()->SetLabelSize(0.07);
  hist->GetYaxis()->SetTitleSize(0.06);
  gStyle->SetOptStat(0);
}


void HMdcEfficiencyLayer::drawTotNhits() {
  hists[0]->SetFillColor(3);
  if(TMath::Abs(hists[0]->GetSumOfWeights()-hists[1]->GetSumOfWeights())>0.5) { 
   Text_t title[90];
   Float_t h0=hists[0]->GetSumOfWeights();
   Float_t h1=hists[1]->GetSumOfWeights();
   Float_t h11=hists[11]->GetSumOfWeights();
   Float_t prCut=(h1-h0)/h1*100.;
   Float_t prCutS=(h1-h11)/h1*100.;
   Float_t prCutTD=(h11-h0)/h1*100.;
   sprintf(title,"S%i M%i L%i: Nhits (Com.stop cut(%6.2f%c)+times diff. cut(%6.2f%c)=%6.2f%c)",
       sector+1,module+1,layer+1,prCutS,'%',prCutTD,'%',prCut,'%');
   setTitle(hists[1],"Nhits");
   hists[1]->SetTitle(title);
   hists[1]->SetFillColor(4);
   hists[1]->Draw();
   hists[11]->SetFillColor(5);
   hists[11]->Draw("same");
   hists[0]->Draw("same");
   hists[12]->SetLineColor(6);
   hists[12]->Draw("same");
 }
  else {
    setTitle(hists[0],"Nhits");
    hists[0]->Draw();
  }
}

void HMdcEfficiencyLayer::drawNHits() {
  setTitle(hists[2],"N0,N1,N2,Ntr");
  Char_t title[80];
  sprintf(title,
      "S%i M%i L%i:  N0[r]=%.0f  N1[b]=%.0f N2[g]=%.0f Ntr[black]=%.0f",
      sector+1,module+1,layer+1,
      eff.getN0(),eff.getN1(),eff.getN2(),eff.getTotNumTracks());
  hists[2]->SetTitle(title);
  hists[2]->Draw();
  hists[3]->SetLineColor(2);
  hists[3]->Draw("same");
  hists[4]->SetLineColor(4);
  hists[4]->Draw("same");
  hists[5]->SetLineColor(3);
  hists[5]->Draw("same");
}

void HMdcEfficiencyLayer::drawHistEff() {
  // Drawing of the efficiency.
  setTitle(hists[6],"E");
  Char_t title[60];
  sprintf(title,"S%i M%i L%i:  E=%.3f+/-%.4f",sector+1,module+1,layer+1,
                       eff.getEfficiency(), eff.getErrEfficiency());
  hists[6]->SetTitle(title);
  hists[6]->Draw();
  drawLineTot(hists[6],2, eff.getEfficiency(), eff.getErrEfficiency());
}

void HMdcEfficiencyLayer::drawHistSing() {
  // Drawing of the fraction of single hits.
  setTitle(hists[7],"S");
  Char_t title[60];
  sprintf(title,"S%i M%i L%i:  S=%.3f+/-%.4f",sector+1,module+1,layer+1,
                       eff.getFracSingle(), eff.getErrFracSingle());
  hists[7]->SetTitle(title);
  hists[7]->Draw();
  drawLineTot(hists[7], 2, (Float_t)eff.getFracSingle(), 
                           (Float_t)eff.getErrFracSingle());
}

void HMdcEfficiencyLayer::drawProbabilitis() {
  // Drawing of the probabilities.
  setTitle(hists[8],"P0,P1,P2");
  Char_t title[80];
  sprintf(title,
      "S%i M%i L%i:  P0=%.3f+/-%.4f(r) P1=%.3f+/-%.4f(b) P2=%.3f+/-%.4f(g)",
      sector+1,module+1,layer+1,eff.getP0(),eff.getErrP0(),
      eff.getP1(),eff.getErrP1(),eff.getP2(),eff.getErrP2());
  hists[8]->SetTitle(title);
  hists[8]->Draw();
  drawLineTot(hists[8], 2, (Float_t)eff.getP0(), (Float_t)eff.getErrP0());
  hists[9]->Draw("same");
  drawLineTot(hists[9], 4, (Float_t)eff.getP1(), (Float_t)eff.getErrP1());
  hists[10]->Draw("same");
  drawLineTot(hists[10], 3, (Float_t)eff.getP2(), (Float_t)eff.getErrP2());
}

void HMdcEfficiencyLayer::drawLineTot(TH1F* hst, Int_t color, Float_t eff, 
                                                              Float_t errEff){
  Float_t xf=hst->GetXaxis()->GetBinLowEdge(hst->GetXaxis()->GetFirst());
  Float_t xl=hst->GetXaxis()->GetBinUpEdge(hst->GetXaxis()->GetLast());
  TLine lnEffLay;
  lnEffLay.SetLineStyle(1);
  lnEffLay.SetLineColor(color);
  lnEffLay.DrawLine(xf,eff,xl,eff);
  lnEffLay.SetLineStyle(2);
  lnEffLay.DrawLine(xf, eff+errEff, xl, eff+errEff);
  lnEffLay.DrawLine(xf, eff-errEff, xl, eff-errEff);
}

void HMdcEfficiencyLayer::setListPads(Int_t* listPad) {
  nPads=0;
  for(Int_t pad=0; pad<5; pad++) {
    if(listPad[pad]) {
      nPads++;
      listPads[pad]=1;
    }
    else listPads[pad]=0;
  }
  
  TSeqCollection* fSegColl=gROOT->GetListOfCanvases();
  TCanvas* canv=(TCanvas*)fSegColl->FindObject(nameCanv);
  if(canv) {
    if(nPads>0) {
      Int_t nPd=0;
      if(nPads != (nPd=canv->GetListOfPrimitives()->GetSize())) {
       canv->Clear();
       if(nPd<3||nPads<3) canv->SetWindowSize(700,(nPads>3) ? 900 : nPads*300);
       setPads();
      }
      drawCanvas();
    }
    else canv->Clear();
  }
  for(Int_t nPl=0; nPl<3; nPl++) {
    if( listPad[nPl+1] == 1 ) continue;
    TCanvas* canv=(TCanvas*)fSegColl->FindObject(nameCanvXY[nPl]);
    if( canv ) delete canv;
    for(Int_t i=0; i<256; i++) indDrawPv[nPl][i]=1;
    indDrawTitXY[nPl]=1;
  }
}

void HMdcEfficiencyLayer::creatCanvas() {
  if(gROOT->GetListOfCanvases()->FindObject(nameCanv)) return;
  if(nPads==0) return;
  canvEff=new TCanvas(nameCanv,titleCanv,1,1,700,(nPads>3) ? 900 : nPads*300);
  setPads();
}

void HMdcEfficiencyLayer::setPads() {
  canvEff->Divide(1,nPads,0.0001,0.0001);
  for(Int_t nPad=0; nPad<nPads; nPad++) {
    canvEff->cd(nPad+1);   
    TPad* pad = (TPad*)gPad;
    pad->SetRightMargin(0.03);
    pad->SetLeftMargin(0.08);
    pad->SetTopMargin(0.1);
    pad->SetBottomMargin(0.1);   
  }
}

void HMdcEfficiencyLayer::updateCanv(void) {
  canvEff=(TCanvas*)gROOT->GetListOfCanvases()->FindObject(nameCanv);
  if(!canvEff) return;
  drawCanvas();
}

void HMdcEfficiencyLayer::drawCanvas() {
  creatCanvas();
  Int_t nPad=1;
  for(Short_t pad=0; pad<6; pad++) {
    if(!listPads[pad]) continue;
    canvEff->cd(nPad++);
    if(pad==0) drawTotNhits();
    else if(pad==1) drawNHits();
    else if(pad==2) drawHistEff();
    else if(pad==3) drawHistSing();
    else if(pad==4) drawProbabilitis();
  }
  canvEff->cd();
  canvEff->Modified();
  canvEff->Update();
}

void HMdcEfficiencyLayer::delCanvas(){
  canvEff=(TCanvas*)gROOT->GetListOfCanvases()->FindObject(nameCanv);
  if(canvEff) {
    delete canvEff;
    canvEff=0;
  }
}

void HMdcEfficiencyLayer::setPavesXY() {
  Int_t nPavesSh=16/nBinsSumXY;
  Float_t bordX=0.05;
  Float_t bordY=0.05;
  Float_t dX=(1.-bordX*2)/nPavesSh;
  Float_t dY=(1.-bordY*2)/nPavesSh;
  for(Int_t yp=0; yp<16; yp++) {
    Float_t y1=bordY+yp*dY;
    Float_t y2=bordY+(yp+1)*dY;
    for(Int_t xp=0; xp<16; xp++) {
      for(Int_t i=0; i<3; i++) {
        pvTxEffXY[i][xp+yp*16].SetX1(bordX+xp*dX);
        pvTxEffXY[i][xp+yp*16].SetX2(bordX+(xp+1)*dX);
        pvTxEffXY[i][xp+yp*16].SetY1(y1);
        pvTxEffXY[i][xp+yp*16].SetY2(y2);
        pvTxEffXY[i][xp+yp*16].SetX1NDC(bordX+xp*dX);
        pvTxEffXY[i][xp+yp*16].SetX2NDC(bordX+(xp+1)*dX);
        pvTxEffXY[i][xp+yp*16].SetY1NDC(y1);
        pvTxEffXY[i][xp+yp*16].SetY2NDC(y2);
        pvTxEffXY[i][xp+yp*16].SetTextAlign(22);
        pvTxEffXY[i][xp+yp*16].SetTextSize(0);
        pvTxEffXY[i][xp+yp*16].SetBorderSize(1);
      }
    }
  }
}

void HMdcEfficiencyLayer::setBiningXY(Int_t nBinsSum) {
  Int_t nbnSm=8;
  if( nBinsSum <= 1 ) nbnSm=1;
  else if( nBinsSum < 4 ) nbnSm=2;
  else if( nBinsSum < 8 ) nbnSm=4;
  if(nbnSm == nBinsSumXY) return;
  nBinsSumXY=nbnSm;
  for(Int_t nPl=0; nPl<3; nPl++) {
    canvXY[nPl]=(TCanvas*)gROOT->GetListOfCanvases()->
        FindObject(nameCanvXY[nPl]);
    if( canvXY[nPl] ) {
      canvXY[nPl]->Clear();
      for(Int_t i=0; i<256; i++) {
        pvTxEffXY[nPl][i].Clear();
        indDrawPv[nPl][i]=1;
      }
      indDrawTitXY[nPl]=1;
    }
  }
  setPavesXY();
  drawCanvXY(1);
}

void HMdcEfficiencyLayer::drawCanvXYlist() {
  drawCanvXY(0);
}

void HMdcEfficiencyLayer::updateCanvXYlist(void) {
  drawCanvXY(1);
}

void HMdcEfficiencyLayer::delCanvXYlist() {
  TSeqCollection* fSegColl=gROOT->GetListOfCanvases();
  for(Int_t i=0; i<3; i++) {
    if( !listPads[i+1] ) continue;
    canvXY[i]=(TCanvas*)fSegColl->FindObject(nameCanvXY[i]);
    if(canvXY[i]) {
      delete canvXY[i];
      canvXY[i]=0;
    }
  }
}

void HMdcEfficiencyLayer::fillPaveXYnHits(Int_t nPv) {
  Float_t edgeCol[10]={.2,.4,.5,.7,.79,.85,.89,.92,.94,.951};
  Text_t buf[20];
  Int_t color=0;
  sprintf(buf,"N0:%.0f",effXY.getN0());
  pvTxEffXY[0][nPv].InsertText(buf);
  sprintf(buf,"N1:%.0f",effXY.getN1());
  pvTxEffXY[0][nPv].InsertText(buf);
  sprintf(buf,"N2:%.0f",effXY.getN2());
  pvTxEffXY[0][nPv].InsertText(buf);
  Float_t eff=effXY.getEfficiency();
  if(eff >= 0.95) {
    color=Int_t((eff-0.95)/0.005)+10;
    if(color > 19) color=19;
  }
  else {
    color=0;
    while(eff > edgeCol[color]) color++;
  }
  pvTxEffXY[0][nPv].SetFillColor(color+31);  
}

void HMdcEfficiencyLayer::fillPaveXYeff(Int_t nPv) {
  Float_t edgeCol[10]={.2,.4,.5,.7,.79,.85,.89,.92,.94,.951};
  Text_t buf[20];
  Int_t color=0;
  Float_t eff=effXY.getEfficiency();
  sprintf(buf,"%5.3f",eff);
  pvTxEffXY[1][nPv].InsertText(buf);
  sprintf(buf,"+-%5.3f",effXY.getErrEfficiency());
  pvTxEffXY[1][nPv].InsertText(buf);
  if(eff >= 0.95) {
    color=Int_t((eff-0.95)/0.005)+10;
    if(color > 19) color=19;
  }
  else {
    color=0;
    while(eff > edgeCol[color]) color++;
  }
  pvTxEffXY[1][nPv].SetFillColor(color+31);  
}

void HMdcEfficiencyLayer::fillPaveXYsing(Int_t nPv) {
  Text_t buf[20];
  Float_t frS=effXY.getFracSingle();
  sprintf(buf,"%5.3f",frS);
  pvTxEffXY[2][nPv].InsertText(buf);
  sprintf(buf,"+-%5.3f",effXY.getErrFracSingle());
  pvTxEffXY[2][nPv].InsertText(buf);
  Int_t color=Int_t(frS*20.)+1;
  if(color > 20) color=20;
  pvTxEffXY[2][nPv].SetFillColor(color+30);  
}


void HMdcEfficiencyLayer::drawCanvXY(Int_t indUpDr=0) {
  // indUpDr=0 - draw canv
  // indUpDr=1 - update
  TSeqCollection* fSegColl=gROOT->GetListOfCanvases();
  for(Int_t nPl=0; nPl<3; nPl++) {
    canvXY[nPl]=(TCanvas*)fSegColl->FindObject(nameCanvXY[nPl]);
    if( indUpDr == 0 && canvXY[nPl] == 0 && listPads[nPl+1] ) {
      canvXY[nPl]=new TCanvas(nameCanvXY[nPl],titleCanvXY[nPl],1,1,600,600);
      for(Int_t i=0; i<256; i++) indDrawPv[nPl][i]=1;
      indDrawTitXY[nPl]=1;
    }
  }
  setPavesXY();  
  for(Short_t xp=0; xp<16; xp+=nBinsSumXY) {
    for(Short_t yp=0; yp<16; yp+=nBinsSumXY) {
      getEffXY(&effXY,xp,xp+nBinsSumXY-1,yp,yp+nBinsSumXY-1);
      if(effXY.getTotNumTracks() > 0.) {
        Int_t nPv=xp/nBinsSumXY+(yp/nBinsSumXY)*16;
        for(Int_t nPl=0; nPl<3; nPl++) {
          if( canvXY[nPl] == 0 ) continue;
          canvXY[nPl]->cd();
          pvTxEffXY[nPl][nPv].Clear(); 
          if( nPl == 0) fillPaveXYnHits(nPv);
          else if( nPl == 1) fillPaveXYeff(nPv);
          else fillPaveXYsing(nPv); 
          if( indDrawPv[nPl][nPv] ) {
            pvTxEffXY[nPl][nPv].Draw();
            indDrawPv[nPl][nPv]=0;
          }
//          else pvTxEffXY[nPl][nPv].Modified();
        }
      }
    }
  }
  getEffXY(&effXY); 
  for(Int_t nPl=0; nPl<3; nPl++) {
    if( canvXY[nPl] == 0 ) continue;
    canvXY[nPl]->cd();
    Text_t buf[80]; 
    if( nPl == 0 ) sprintf(buf,"S%i M%i L%i:  N0=%.0f  N1=%.0f N2=%.0f",
      sector+1,module+1,layer+1,
      effXY.getN0(),effXY.getN1(),effXY.getN2());
    else if( nPl == 1 ) sprintf(buf,"S%i M%i L%i:  E=%.3f+/-%.4f",sector+1,
      module+1,layer+1,effXY.getEfficiency(), effXY.getErrEfficiency());
    else sprintf(buf,"S%i M%i L%i:  S=%.3f+/-%.4f",sector+1,module+1,layer+1,
                       effXY.getFracSingle(), effXY.getErrFracSingle());
    titlePadXY[nPl].Clear();
    titlePadXY[nPl].InsertText(buf);
    if( indDrawTitXY[nPl] ) {
      titlePadXY[nPl].Draw();
      indDrawTitXY[nPl]=0;
    }
//    else titlePadXY[nPl].Modified();
    canvXY[nPl]->Modified();
    canvXY[nPl]->Update();
  }
}

//------------------------------

HMdcEfficiencyMod::HMdcEfficiencyMod(Short_t sec, Short_t mod) {
  // constructor creates an array of pointers of type HMdcEfficiencyLayer
  sector=sec;
  module=mod;

  // Geting of pointers to par. cont.
  HMdcGetContainers* fGetCont=HMdcGetContainers::getObject();
  if( !fGetCont ) return;
  HMdcLookUpTb* fLookUpTb= HMdcLookUpTb::getExObject();
  HMdcLookupGeom* flookupGeom=fGetCont->getMdcLookupGeom();
  HMdcRawStruct* frawStruct=fGetCont->getMdcRawStruct();
  if( !fLookUpTb ) return;
  
  // Geting of proj. plane sizes
  if( &((*fLookUpTb)[sector]) ) {
    HMdcLookUpTbSec& flookutsec = (*fLookUpTb)[sec];
    xLow=flookutsec.getXlow();
    xUp=flookutsec.getXup();
    yLow=flookutsec.getYlow();
    yUp=flookutsec.getYup();
    dX=(xUp-xLow)/16;
    dY=(yUp-yLow)/16;
    // creating of arrays
    clear();
  }
  
  // geting of pointers to HMdcLookupGeom[sec][mod] and HMdcLookupRaw[sec][mod]
  flookupGMod=(flookupGeom) ? &(*flookupGeom)[sector][module] :0;
  frawModStru=(frawStruct) ? &(*frawStruct)[sector][module] : 0;
   
  hsEffMb=0;
  nChannels=0;
  for(Int_t i=0; i<9; i++) hsTimes[i]=0;
  for(Int_t i=0; i<15; i++) drHsTimes[i]=0;
  minTimeBin[0]=minTimeBin[1]=minTimeBin[2]=1000000.;
  maxTimeBin[0]=maxTimeBin[1]=maxTimeBin[2]=-100000.0;
  Char_t name[20];
  if(flookupGMod && frawStruct) {
    Int_t nMoth=frawModStru->getNMotherboards();
    for(Int_t nMb=0; nMb<nMoth; nMb++) {
      HMdcRawMothStru* fMb=&(*frawModStru)[nMb];
      if(fMb) nChannels+=fMb->getNTdcs();
    }
    sprintf(name,"S%iM%iEffMb",sector+1,module+1);
    hsEffMb=new TH1F(name,"",nChannels,0.,nChannels/8);
    hsEffMb->SetOption("e1p");
    hsEffMb->SetMarkerStyle(21);
    hsEffMb->SetMarkerSize(0.3);
    hsEffMb->SetMarkerColor(4);
    hsEffMb->SetMaximum(1.1);
    hsEffMb->SetMinimum(0.0);
    hsEffMb->SetYTitle("Eff. vs. 1 chan. TDC");
    hsEffMb->SetXTitle("TDC");
    hsEffMb->SetNdivisions(0);
    sprintf(name,"S%iM%iEffMbS4",sector+1,module+1);
    hsEffMb4=new TH1F(name,"",nChannels/4,0.,nChannels/8);
    hsEffMb4->SetOption("e1p");
    hsEffMb4->SetMarkerStyle(21);
    hsEffMb4->SetMarkerSize(0.3);
    hsEffMb4->SetMarkerColor(4);
    hsEffMb4->SetMaximum(1.1);
    hsEffMb4->SetMinimum(0.0);
    hsEffMb4->SetYTitle("Eff. vs. 4 chan. TDC");
    hsEffMb4->SetXTitle("TDC");
    hsEffMb4->SetNdivisions(0);
  }
  Char_t* titT[25];
  Char_t* nmT[9];
  titT[0]="Time1";    
  titT[1]="Time2";
  titT[2]="Time2-Time1";
  titT[3]="Time1 (clust.)";    
  titT[4]="Time2 (clust.)";
  titT[5]="Time2-Time1 (clust.)";
  titT[6]="Time1 (sel.clust.)";    
  titT[7]="Time2 (sel.clust.)";
  titT[8]="Time2-Time1 (sel.clust.)";
  nmT[0]="Time1a";    
  nmT[1]="Time2a";
  nmT[2]="Time2m1a";
  nmT[3]="Time1c";    
  nmT[4]="Time2c";
  nmT[5]="Time2m1c";
  nmT[6]="Time1sc";    
  nmT[7]="Time2sc";
  nmT[8]="Time2m1sc";
  biningTimes=1;
  Char_t title[50];
  Float_t x1,x2;
  Int_t nBHs=0;
  Int_t colors[3]={4,5,3};
  for(Int_t i=0; i<9; i++) {
    x1=-1000.;
    x2=3200.;
    nBHs=840;
    if( (i+1)%3 == 0 ) { 
      x1=-100.;
      x2=900.;
       nBHs=500;
    }
    sprintf(name,"S%iM%i%s",sector+1,module+1,nmT[i]);
    sprintf(title,"S%i M%i: %s",sector+1,module+1,titT[i]);
    hsTimes[i]=new TH1F(name,title,nBHs,x1,x2);
    hsTimes[i]->SetFillColor(colors[i/3]);
    hsTimes[i]->SetLineColor(colors[i/3]);
  }
  
  // creating of HMdcEfficiencyLayer array
  array = new TObjArray(6);
  for (Short_t layer = 0; layer < 6; layer++) {
    (*array)[layer] = new HMdcEfficiencyLayer(sector,module,layer);
  }
  setNameCanv();
}

void HMdcEfficiencyMod::setNameCanv(void){
  for(Int_t nCanv=0; nCanv<5; nCanv++) canvases[nCanv]=0;
  sprintf(nameCanv[0],"Cn_S%iM%iTNhits",sector+1,module+1);
  sprintf(nameCanv[1],"Cn_S%iM%iNtrNhits",sector+1,module+1);
  sprintf(nameCanv[2],"Cn_S%iM%iEffC",sector+1,module+1);
  sprintf(nameCanv[3],"Cn_S%iM%iSingC",sector+1,module+1);
  sprintf(nameCanv[4],"Cn_S%iM%iProbC",sector+1,module+1);
  sprintf(titleCanv[0],"S%i M%i The total num. of hits",sector+1,module+1);
  sprintf(titleCanv[1],"S%i M%i The num. of tracks and zero, single and double hits",sector+1,module+1);
  sprintf(titleCanv[2],"S%i M%i The efficiency",sector+1,module+1);
  sprintf(titleCanv[3],"S%i M%i The fraction of single hits",sector+1,module+1);
  sprintf(titleCanv[4],"S%i M%i The probability to have zero, single and double hit",sector+1,module+1);
  nPads=0;
  for(Int_t nLay=0; nLay<6; nLay++) listLayers[nLay]=0;
  canvasMb=0;
  sprintf(nameCanvMb,"Cn_S%iM%iEffMb",sector+1,module+1);
  sprintf(titleCanvMb,"S%i M%i The efficiency of wires vs. chanels of matherboard",sector+1,module+1);
  sprintf(nameSliderMb,"Sl_S%iM%iEffMb",sector+1,module+1);
  sprintf(nameCanvTimes,"Cn_S%iM%iTimes",sector+1,module+1);
  sprintf(titleCanvTimes,"S%i M%i time1, time2, time2-time1 ",
                 sector+1,module+1);
}

HMdcEfficiencyMod::~HMdcEfficiencyMod() {
  // destructor
  TSeqCollection* fSegColl=gROOT->GetListOfCanvases();
  canvasMb=(TCanvas*)fSegColl->FindObject(nameCanvMb);
  if( canvasMb ) delete canvasMb;
  TCanvas* canv=(TCanvas*)fSegColl->FindObject(nameCanvTimes);
  if( canv ) delete canv;  
  for(Int_t i=0; i<5; i++) {
    canvases[i]=(TCanvas*)fSegColl->FindObject(nameCanv[i]);
    if( canvases[i] ) delete canvases[i];
  }
  if( array ) {
    array->Delete();
    delete array;
  }
}

void HMdcEfficiencyMod::clear() {
  for (Short_t xy=0; xy<256; xy++) n0xy[xy]=n1xy[xy]=n2xy[xy]=0;
}

Int_t HMdcEfficiencyMod::getBinNum(Float_t x, Float_t y) {
  Int_t nX=(Int_t)((x-xLow+dX)/dX)-1;
  Int_t nY=(Int_t)((y-yLow+dY)/dY)-1;
  if(nX<0 || nX>=16 || nY<0 || nY>=16) return -1;
  return nY*16+nX;
}

void HMdcEfficiencyMod::getEffXY(HMdcEffMod* efficiency) {
  // Calculation of efficiency of MDC layer.
  getEffXY(efficiency,0,15,0,15);
}

void HMdcEfficiencyMod::getEffXY(HMdcEffMod* efficiency,
                              Short_t xBin, Short_t yBin){
  // Calculation of efficiency of MDC for bin (xBin:yBin).
  getEffXY(efficiency,xBin,xBin,yBin,yBin);
}
void HMdcEfficiencyMod::getEffXY(HMdcEffMod* efficiency,
                                   Short_t fXbin, Short_t lXbin,
                                   Short_t fYbin, Short_t lYbin){
  // Calculation of efficiency of MDC ragion
  // (MDC is divided on 16x16 bins).
  // Region: fXbin <= x <= lXbin, fYbin <= y <= lYbin
  Int_t nC0,nC1,nC2;
  nC0=nC1=nC2=0;
  Double_t eN0,eN1,eN2;
  eN0=eN1=eN2=0.;

  Int_t fXb=(fXbin < 0 ) ?  0 : fXbin;
  Int_t lXb=(lXbin > 15) ? 15 : lXbin;
  Int_t fYb=(fYbin < 0 ) ?  0 : fYbin;
  Int_t lYb=(lYbin > 15) ? 15 : lYbin;
  for(Int_t y=fYb; y<=lYb; y++) {
    Int_t nBin=y*16;
    for(Int_t x=fXb; x<=lXb; x++){
      nC0+=n0xy[nBin+x];
      nC1+=n1xy[nBin+x];
      nC2+=n2xy[nBin+x];
    }
  }
  if(nC0+nC1+nC2 > 0) {
    eN0=(nC0>0) ? sqrt((Double_t)nC0) : 1.;
    eN1=(nC1>0) ? sqrt((Double_t)nC1) : 1.;
    eN2=(nC2>0) ? sqrt((Double_t)nC2) : 1.;
  }
  efficiency->calcEff((Double_t)nC0,eN0, (Double_t)nC1,eN1, (Double_t)nC2,eN2);
}

void HMdcEfficiencyMod::setListLayers(Int_t* listLay) {
  nPads=0;
  for(Int_t lay=0; lay<6; lay++) {
    listLayers[lay]=(listLay[lay]) ? 1:0;
    if(listLayers[lay]) nPads++;
  }
  TSeqCollection* fSegColl=gROOT->GetListOfCanvases();
  for(Int_t nCanv=0; nCanv<5; nCanv++) {
    TCanvas* canv=(TCanvas*)fSegColl->FindObject(nameCanv[nCanv]);
    if(!canv) continue;
    if(nPads>0) {
      Int_t nPd=0;
      if(nPads != (nPd=canv->GetListOfPrimitives()->GetSize())) {
        canv->Clear();
        if(nPd<3 || nPads<3) canv->SetWindowSize(700,(nPads>3) ? 900:nPads*300);
        setPads(canv);
      }
      if(nCanv==0) drawCanvTotNhits();
      else if(nCanv==1) drawCanvNHits();
      else if(nCanv==2) drawCanvHistEff();
      else if(nCanv==3) drawCanvHistSing();
      else if(nCanv==4) drawCanvProbabilitis();
    }
    else delete canv;
  }
}

void HMdcEfficiencyMod::creatCanvas(Int_t nCanv) {
  if(gROOT->GetListOfCanvases()->FindObject(nameCanv[nCanv])) return;
  if(nPads==0) return;
  canvases[nCanv]=new TCanvas(nameCanv[nCanv],titleCanv[nCanv],
 			    1,1,700,(nPads>3) ? 900 : nPads*300);
  setPads(canvases[nCanv]);
}

void HMdcEfficiencyMod::setPads(TCanvas* canv) {
  canv->Divide(1,nPads,0.0001,0.0001);
  for(Int_t nPad=0; nPad<nPads; nPad++) {
    canv->cd(nPad+1);
    canv->SetLeftMargin(0.05);
    canv->SetRightMargin(0.02);
    canv->SetBottomMargin(0.1);
    canv->SetTopMargin(0.15);
  }
  gStyle->SetOptStat(0);
}

void HMdcEfficiencyMod::updateCanv(void) {
  TSeqCollection* fSegColl=gROOT->GetListOfCanvases();
  for(Int_t nCanv=0; nCanv<5; nCanv++){
    TCanvas* canv=(TCanvas*)fSegColl->FindObject(nameCanv[nCanv]);
    if(!canv) continue;
    drawHists(nCanv);
  }
  if(fSegColl->FindObject(nameCanvMb)) drawCanvEffMb();
  if(fSegColl->FindObject(nameCanvTimes)) drawCanvTimes();
}

void HMdcEfficiencyMod::drawHists(Int_t nCanv) {
  Int_t nPad=1;
  for(Short_t lay=0; lay<6; lay++) {
    if(!listLayers[lay]) continue;
    canvases[nCanv]->cd(nPad++);
    if(nCanv==0) (*this)[lay].drawTotNhits();
    else if(nCanv==1) (*this)[lay].drawNHits();
    else if(nCanv==2) (*this)[lay].drawHistEff();
    else if(nCanv==3) (*this)[lay].drawHistSing();
    else if(nCanv==4) (*this)[lay].drawProbabilitis();
  }
  canvases[nCanv]->Modified();
  canvases[nCanv]->Update();
}

void HMdcEfficiencyMod::drawCanvTotNhits() {
  creatCanvas(0);
  drawHists(0);
}

void HMdcEfficiencyMod::drawCanvNHits() {
  creatCanvas(1);
  drawHists(1);
}

void HMdcEfficiencyMod::drawCanvHistEff() {
  creatCanvas(2);
  drawHists(2);
}

void HMdcEfficiencyMod::drawCanvHistSing() {
  creatCanvas(3);
  drawHists(3);
}

void HMdcEfficiencyMod::drawCanvProbabilitis() {
  creatCanvas(4);
  drawHists(4);
}

void HMdcEfficiencyMod::delCanv(Int_t nCanv){
  if(gROOT->GetListOfCanvases()->FindObject(nameCanv[nCanv])) 
    delete canvases[nCanv];
  canvases[nCanv]=0;
}

void HMdcEfficiencyMod::fillHistMb() {
  if( !flookupGMod || !frawModStru) return;
  Int_t bin=0;
  Int_t bin4=0;
  Int_t nMoth=frawModStru->getNMotherboards();
  for(Int_t nMb=0; nMb<nMoth; nMb++) {
    HMdcRawMothStru* fRMb=&(*frawModStru)[nMb];
    if(!fRMb) continue;
    Int_t nChan=fRMb->getNTdcs();
    HMdcLookupMoth* fMb=&(*flookupGMod)[nMb];
    for(Int_t nCh4=0; nCh4<nChan/4; nCh4++) {
      Int_t nLay4,nCell1,nCell2;
      nLay4=nCell1=nCell2=-1;
      bin4++;
      for(Int_t i4=0; i4<4; i4++) {
        Int_t nCh=nCh4*4+i4;
        bin++;
        if( !fMb ) continue;
        HMdcLookupChan* fchan=&(*fMb)[nCh];
        if( !fchan ) continue;
        Int_t layer=fchan->getNLayer();
        Int_t cell=fchan->getNCell();
        if(cell<0 || layer<0) continue;
        HMdcEfficiencyLayer& flay=(*this)[layer];
        flay.getEffCells(&eff, cell);
        hsEffMb->SetBinContent(bin,eff.getEfficiency());
        hsEffMb->SetBinError(bin,eff.getErrEfficiency());
        if(nLay4==-1) {
          nCell1=nCell2=cell;
          nLay4=layer;
        } else {
          if(nLay4 != layer) Error("fillHistMb","nLay4 != layer");
          nCell2=cell;
        }
      }
      if(nCell1>nCell2) {
        Int_t tmp=nCell1;
        nCell1=nCell2;
        nCell2=tmp;
      }
      if(nLay4>=0 && nCell1>=0 && nCell2-nCell1<4) {
        HMdcEfficiencyLayer& flay=(*this)[nLay4];
        flay.getEffCells(&eff, nCell1, nCell2);
        hsEffMb4->SetBinContent(bin4,eff.getEfficiency());
        hsEffMb4->SetBinError(bin4,eff.getErrEfficiency()); 
      }
    }
  }
  getEffXY(&effMod);
  Char_t title[70];
  sprintf(title,"Sec.%i Mod.%i :     E=%.4f +/- %.5f",sector+1,module+1,
             effMod.getEfficiency(),effMod.getErrEfficiency());
  hsEffMb->SetTitle(title);
}


void HMdcEfficiencyMod::drawCanvEffMb() {
  if( !flookupGMod || !frawModStru) return;
  canvasMb=(TCanvas*)gROOT->GetListOfCanvases()->FindObject(nameCanvMb);
  if( !canvasMb ) {
    canvasMb = new TCanvas(nameCanvMb,titleCanvMb, 1, 1, 1000, 600);
    canvasMb->Divide(1,2);
    canvasMb->cd(1);
    canvasMb->SetBorderMode(0);
    canvasMb->SetFillColor(10);
    
    TPad *upPad = (TPad*)gPad;
    upPad->SetPad(0.0,0.53,1.,1.);
    upPad->SetRightMargin(0.02);
    upPad->SetLeftMargin(0.05);
    upPad->SetTopMargin(0.1);
    upPad->SetFillColor(10); 
    canvasMb->cd(2);
    TPad *lowPad = (TPad*)gPad;
    lowPad->SetPad(0.0,0.0,1.,0.44);
    lowPad->SetRightMargin(0.02);
    lowPad->SetLeftMargin(0.05);
    lowPad->SetTopMargin(0.02);
    lowPad->SetFillColor(10); 
//    canv->SetBottomMargin(0.1);
  }
  canvasMb->cd();
  gStyle->SetOptStat(0);
  canvasMb->cd(1);
  gPad->SetFrameFillColor(10);
  hsEffMb->GetXaxis()->SetTitleSize(0.06);
  hsEffMb->SetTitleOffset(0.45,"Y");;
  hsEffMb->SetTitleOffset(0.8,"X");
  hsEffMb->GetYaxis()->SetTitleSize(0.05);
  hsEffMb->GetXaxis()->SetTitleSize(0.05);
  hsEffMb->GetYaxis()->CenterTitle(1);
  hsEffMb->GetXaxis()->SetLabelSize(0.05);
  hsEffMb->GetYaxis()->SetLabelSize(0.05);
  hsEffMb->Draw();
  drawLineEffMb(hsEffMb);
  drawLineGrMb(hsEffMb);
  canvasMb->cd(2);
  gPad->SetFrameFillColor(10);
  hsEffMb4->SetTitleOffset(0.45,"Y");
  hsEffMb4->SetTitleOffset(0.8,"X");
  hsEffMb4->GetYaxis()->SetTitleSize(0.05);
  hsEffMb4->GetXaxis()->SetTitleSize(0.05);
  hsEffMb4->GetYaxis()->CenterTitle(1);
  hsEffMb4->GetXaxis()->SetLabelSize(0.05);
  hsEffMb4->GetYaxis()->SetLabelSize(0.05);  
  hsEffMb4->Draw();
  drawLineEffMb(hsEffMb4);
  drawLineGrMb(hsEffMb4);

  if(!(TSlider*)gROOT->FindObject(nameSliderMb)) {
    canvasMb->cd();
    Float_t x1=0.05;
    Float_t x2=0.98;
    mbSlider=new TSlider(nameSliderMb,"", x1,0.49,x2, 0.53);
    mbSlider->SetObject(this);
    
    Int_t nChan=0;
    Int_t nMoth=frawModStru->getNMotherboards();
    for(Int_t nMb=0; nMb<nMoth; nMb++) {
      HMdcRawMothStru* fMb=&(*frawModStru)[nMb];
      if( !fMb ) continue;
      Float_t x1p=Float_t(nChan)/Float_t(nChannels)*(x2-x1)+x1;
      nChan+=fMb->getNTdcs();
      Float_t x2p=Float_t(nChan)/Float_t(nChannels)*(x2-x1)+x1;
      Text_t nameMb[10];
      sprintf(nameMb,"%i : %s",nMb+1,fMb->GetName());
      TPaveLabel* pave = new TPaveLabel(x1p,0.45,x2p,0.48,nameMb);
      pave->SetBorderSize(1);
      pave->Draw();
    }
  }
  mbSlider->SetRange(
   Float_t(hsEffMb->GetXaxis()->GetFirst())/Float_t(nChannels),
   Float_t(hsEffMb->GetXaxis()->GetLast())/Float_t(nChannels));
  gStyle->SetOptStat(0);
  canvasMb->cd(0);
  canvasMb->Modified();
  canvasMb->Update(); 
}

void HMdcEfficiencyMod::drawLineGrMb(TH1F* hst) {
  Float_t ymin=hst->GetMinimum();
  Float_t ymax=hst->GetMaximum(); 
  Float_t xf=hst->GetXaxis()->GetBinLowEdge(hst->GetXaxis()->GetFirst());
  Float_t xl=hst->GetXaxis()->GetBinUpEdge(hst->GetXaxis()->GetLast());
  TLine lineMb;
  lineMb.SetLineColor(3);
  Int_t nChan=0;
  Int_t nMoth=frawModStru->getNMotherboards();
  TText lbMb;
  for(Int_t nMb=0; nMb<nMoth; nMb++) {
    HMdcRawMothStru* fMb=&(*frawModStru)[nMb];
    if( !fMb ) continue;
    Int_t nChMoth=fMb->getNTdcs();
    lineMb.SetLineStyle(2);
    lineMb.SetLineColor(3);
    for(Int_t nTDC=0; nTDC<nChMoth/8; nTDC++) {
      lbMb.SetTextColor(3);
      Float_t x=Float_t(nChan/8+nTDC);
      if(x>xf && x<xl && nTDC>0) lineMb.DrawLine(x, ymin, x, ymax);
      if(x>=xf && x<xl) { 
        Text_t labl[3];
        if( xl-xf <= 56.) {
          sprintf(labl,"%i",nTDC+1);
          lbMb.SetTextAlign(23);
          lbMb.DrawText(x+0.5,ymin-(ymax-ymin)*0.005,labl);
        }
        else if( nTDC==0 || nTDC==nChMoth/8-1 ) {
          sprintf(labl,"%i",nTDC+1);
          Int_t indFL=(nTDC+1)/(nChMoth/8);
          lbMb.SetTextAlign((indFL*2+1)*10+3);
          lbMb.DrawText(x+indFL+(1-indFL*2)*0.3,ymin-(ymax-ymin)*0.005,labl);
        }
        
      }
    }
    Float_t x=Float_t(nChan/8);
    if(x>=xf && x<=xl) {
      Text_t labl[3];
      sprintf(labl,"%i",nMb+1);
      lbMb.SetTextColor(1);
      lbMb.SetTextAlign(23);
      lbMb.DrawText(x,ymin-(ymax-ymin)*0.05,labl);
    }
    nChan+=nChMoth;
    lineMb.SetLineColor(6);
    lineMb.SetLineStyle(1);
    x=Float_t(nChan/8);
    if(x>xf && x<xl) lineMb.DrawLine(x, ymin-(ymax-ymin)*0.05, x, ymax);
  }
}

void HMdcEfficiencyMod::drawLineEffMb(TH1F* hst) {
  Float_t xf=hst->GetXaxis()->GetBinLowEdge(hst->GetXaxis()->GetFirst());
  Float_t xl=hst->GetXaxis()->GetBinUpEdge(hst->GetXaxis()->GetLast());
  TLine lnEffMod;
  lnEffMod.SetLineStyle(1);
  lnEffMod.SetLineColor(2);
  lnEffMod.DrawLine(xf,effMod.getEfficiency(),
                    xl,effMod.getEfficiency());
  lnEffMod.SetLineStyle(2);
  lnEffMod.DrawLine(xf,effMod.getEfficiency()+effMod.getErrEfficiency(),
                    xl,effMod.getEfficiency()+effMod.getErrEfficiency());
  lnEffMod.DrawLine(xf,effMod.getEfficiency()-effMod.getErrEfficiency(),
                    xl,effMod.getEfficiency()-effMod.getErrEfficiency());
}

void HMdcEfficiencyMod::delCanvMb(){
  canvasMb=(TCanvas*)gROOT->GetListOfCanvases()->FindObject(nameCanvMb);
  if( canvasMb ) {
    delete canvasMb;
    canvasMb=0;
  }
}

void HMdcEfficiencyMod::fillTimes(HMdcCal1* cal, Bool_t isGnt, Int_t nHist) {
  if( !hsTimes[nHist] ) return;
  // nHits==0 tested befor call to this function.
  Int_t nHits=abs(cal->getNHits());
  Bool_t tm1=kTRUE;
  Bool_t tm2=(nHits==2) ? kTRUE:kFALSE;
  if(isGnt) {
    HMdcCal1Sim* calS=(HMdcCal1Sim*)cal;
    if(calS->getStatus1()<0) tm1=kFALSE;
    if(nHits==2 && calS->getStatus2()<0) tm2=kFALSE;
  }
  if(tm1) {
    Float_t time=cal->getTime1();
    if(hsTimes[nHist]) hsTimes[nHist]->Fill(time);
    if( nHist == 0 /* && time > -998.*/ ) {
      if( time < minTimeBin[0] ) minTimeBin[0]=time;
      if( time > maxTimeBin[0] ) maxTimeBin[0]=time;
    }
  }
  if( tm2 ) {
    Float_t time=cal->getTime2();
    if(hsTimes[nHist+1]) hsTimes[nHist+1]->Fill(time);
    if( nHist == 0 /* && time > -998.*/ ) {
      if( time < minTimeBin[1] ) minTimeBin[1]=time;
      if( time > maxTimeBin[1] ) maxTimeBin[1]=time;
    }
  }
  if(tm1 && tm2) { 
    Float_t time=cal->getTime2()-cal->getTime1();
    if(hsTimes[nHist+2]) hsTimes[nHist+2]->Fill(time);
    if( nHist == 0 /* && time > -998.*/ ) {
      if( time < minTimeBin[2] ) minTimeBin[2]=time;
      if( time > maxTimeBin[2] ) maxTimeBin[2]=time;
    }
  }
}

void HMdcEfficiencyMod::delCanvTimes() {
  TCanvas* canv=(TCanvas*)gROOT->GetListOfCanvases()->FindObject(nameCanvTimes);
  if(canv) delete canv;
}

void HMdcEfficiencyMod::drawCanvTimes(Int_t bining=-1) {
  if(bining>0 && bining <=100) biningTimes=bining;
  TCanvas* canv=(TCanvas*)gROOT->GetListOfCanvases()->FindObject(nameCanvTimes);
  if( !canv ) {
    canv = new TCanvas(nameCanvTimes,titleCanvTimes, 300, 200, 800, 700);
    canv->Divide(3,2);
    canv->SetBorderMode(0);
    canv->SetFillColor(10);
  }
  Text_t buf[80];
  for(Int_t i=0; i<15; i++) {
//    if(drHsTimes[i]) drHsTimes[i]->Delete();
    if(drHsTimes[i]) delete (drHsTimes[i]);
    if( i < 9 ) {
      drHsTimes[i] = new TH1F(*hsTimes[i]);
      sprintf(buf,"%sR",hsTimes[i]->GetName());
      drHsTimes[i]->SetName(buf);
      if( biningTimes > 1 ) drHsTimes[i]->Rebin(biningTimes);
    }
    else if( i < 12 ) {
      drHsTimes[i] = new TH1F(*drHsTimes[i-3]);  
      drHsTimes[i]->Sumw2();
      drHsTimes[i]->Divide(drHsTimes[i-9]);
      sprintf(buf,"%s (clust.fraction)",hsTimes[i-9]->GetTitle());
      drHsTimes[i]->SetTitle(buf);
      sprintf(buf,"%sdC",hsTimes[i-9]->GetName());
      drHsTimes[i]->SetName(buf);
    }
    else  {
      drHsTimes[i] = new TH1F(*drHsTimes[i-9]);
      drHsTimes[i]->Sumw2();
      drHsTimes[i]->Divide(drHsTimes[i-12]);
      sprintf(buf,"%s (clust.fraction)",hsTimes[i-12]->GetTitle());
      drHsTimes[i]->SetTitle(buf);
      sprintf(buf,"%sdSC",hsTimes[i-12]->GetName());
      drHsTimes[i]->SetName(buf);
    }
  }
  for(Int_t i=0; i<15; i++)  {
    drHsTimes[i]->SetAxisRange(minTimeBin[i%3]-10.0,maxTimeBin[i%3]+10.0);
  }
  for(Int_t i=0; i<3; i++) {
    canv->cd(i+1);
    drHsTimes[i]->SetFillColor(4);
    drHsTimes[i]->SetLineColor(4);
    drHsTimes[i]->Draw(); 
    drHsTimes[i+3]->SetLineColor(6);
    drHsTimes[i+3]->SetFillColor(6);
    drHsTimes[i+3]->Draw("same");
    drHsTimes[i+6]->Draw("same");
    canv->cd(i+4);
    drHsTimes[i+12]->SetLineColor(6);
    drHsTimes[i+12]->SetFillColor(6);
    drHsTimes[i+12]->SetMaximum(1.1);
    drHsTimes[i+12]->Draw("hist");
    drHsTimes[i+9]->SetFillColor(3);
    drHsTimes[i+9]->SetLineColor(3);
    drHsTimes[i+9]->Draw("samehist");
  }
  canv->Modified();
  canv->Update(); 
}

void HMdcEfficiencyMod::ExecuteEvent(Int_t event, Int_t px, Int_t py){
  if(event != 11) return;
  mbSlider=(TSlider*)gROOT->FindObject(nameSliderMb);
  if(!mbSlider) return;
  Int_t nTDC=hsEffMb->GetNbinsX()/32;
  Float_t min=mbSlider->GetMinimum();
  if(min<0.) min=0.;
  Float_t max=mbSlider->GetMaximum();
  Int_t nBmin=Int_t(nTDC*min+0.1);
  if(nBmin==nTDC) nBmin--;
  Int_t nBmax=Int_t(nTDC*max+0.1);
  if(nBmax==nBmin) nBmax++;
  canvasMb->cd(1);
  hsEffMb->SetAxisRange(nBmin*4,nBmax*4-0.0001);
  hsEffMb->Draw();
  drawLineEffMb(hsEffMb);
  drawLineGrMb(hsEffMb);
  mbSlider->SetRange(Float_t(nBmin)/nTDC,Float_t(nBmax)/nTDC);
//  canvasMb->Modified();
  canvasMb->Update();
}

//--Sector--------------------------

HMdcEfficiencySec::HMdcEfficiencySec(Short_t sec) {
  // constructor creates an array of pointers of type
  // HMdcEfficiencyMod
  sector=sec;
  array = new TObjArray(4);
  HMdcGetContainers* fGetCont=HMdcGetContainers::getObject();
  HMdcDetector* fMdcDetector=fGetCont->getMdcDetector();
  for (Short_t mod = 0; mod < 4; mod++) {
   if( fMdcDetector->getModule(sector,mod) ) {
      (*array)[mod] = new HMdcEfficiencyMod(sec,mod);
   }
  }
  sprintf(nameCanvProj,"S%iProjPl",sector+1);
  sprintf(titleCanvProj,"S%i The plots of wires projections.",sector+1);
  sprintf(nameCanvHits,"S%iHitsPl",sector+1);
  sprintf(titleCanvHits,"S%i The plots of MDC hits.",sector+1);  
}

HMdcEfficiencySec::~HMdcEfficiencySec() {
  // destructor
  TSeqCollection* fSegColl=gROOT->GetListOfCanvases();
  canvPlotProj=(TCanvas*)fSegColl->FindObject(nameCanvProj);
  if( canvPlotProj ) {
    delete canvPlotProj;
    canvPlotProj=0;
  }
  canvPlotsHits=(TCanvas*)fSegColl->FindObject(nameCanvHits);
  if( canvPlotsHits ) {
    delete canvPlotsHits;
    canvPlotsHits=0;
  }
  if( array ) {
    array->Delete();
    delete array;
  }
}

void HMdcEfficiencySec::creatPlots(HMdcLookUpTbSec* flookupTbSec) {
  Int_t nBinX=flookupTbSec->getNBinX();
  Int_t nBinY=flookupTbSec->getNBinY();
  Float_t xLow=flookupTbSec->getXlow();
  Float_t xUp=flookupTbSec->getXup();
  Float_t yLow=flookupTbSec->getYlow();
  Float_t yUp=flookupTbSec->getYup();
  Char_t buf[100];
  sprintf(buf,"S%iAllClust.",sector+1);
  plotHits[0]=new TH2F(buf,"",nBinX,xLow,xUp,nBinY,yLow,yUp);
  sprintf(buf,"S%iSelClust.",sector+1);
  plotHits[1]=new TH2F(buf,"",nBinX,xLow,xUp,nBinY,yLow,yUp);
  sprintf(buf,"S%i All clusters",sector+1);
  plotHits[0]->SetTitle(buf);
  sprintf(buf,"S%i Selected clusters",sector+1);
  plotHits[1]->SetTitle(buf);
  plotHits[0]->SetXTitle("x, (mm)");
  plotHits[0]->SetYTitle("y, (mm)      ");
  plotHits[1]->SetXTitle("x, (mm)");
  plotHits[1]->SetYTitle("y, (mm)      ");
  
}

void HMdcEfficiencySec::drawCanvProj() {
  if(gROOT->GetListOfCanvases()->FindObject(nameCanvProj)) return;
  canvPlotProj=new TCanvas(nameCanvProj,titleCanvProj,300,1,500,600);
  canvPlotProj->SetFillColor(10);
}

void HMdcEfficiencySec::delCanvProj(){
  canvPlotProj=(TCanvas*)gROOT->GetListOfCanvases()->FindObject(nameCanvProj);
  if(canvPlotProj) {
    delete canvPlotProj;
    canvPlotProj=0;
  }
}

void HMdcEfficiencySec::drawCanvHits() {
  if(gROOT->GetListOfCanvases()->FindObject(nameCanvHits)) return;
  canvPlotsHits=new TCanvas(nameCanvHits,titleCanvHits,100,300,800,400);
  canvPlotsHits->Divide(2,1,0.0001,0.0001);
  canvPlotsHits->SetFillColor(10);
}

void HMdcEfficiencySec::delCanvHits(){
  canvPlotsHits=(TCanvas*)gROOT->GetListOfCanvases()->FindObject(nameCanvHits);
  if(canvPlotsHits) {
    delete canvPlotsHits;
    canvPlotsHits=0;
  }
}

//------------------------------------------------

HMdcEfficiency::HMdcEfficiency() {
  initSecArray();
}

HMdcEfficiency::HMdcEfficiency(Text_t *name,Text_t *title) : 
                                           HReconstructor(name,title) {
  initSecArray();
}

void HMdcEfficiency::initSecArray() {
  fCalCat=0;
  fLookUpTb=0;
  iter=0;
  eventId=0;
  fMdcDetector=0;
  isCreated=kFALSE;
  array = new TObjArray(6);
}

HMdcEfficiency::~HMdcEfficiency() {
  // destructor
  if( array ) {
    array->Delete();
    delete array;
  }
  canvEventProj=(TCanvas*)gROOT->GetListOfCanvases()->FindObject(nameCanvEvent);
  if( canvEventProj ) {
    delete canvEventProj;
    canvEventProj=0;
  }
//  HMdcTimeCutD::deleteCont();
}

Bool_t HMdcEfficiency::init(void) {
  
  fGetCont=HMdcGetContainers::getObject();
  if( !fGetCont ) return kFALSE;
  fLookUpTb=HMdcLookUpTb::getExObject();
  fSizesCells=HMdcSizesCells::getObject();
  fcut=HMdcTimeCut::getExObject();
  if(fcut && fcut->getIsUsed()) fcut=0;
  fMdcDetector      = fGetCont->getMdcDetector();
  fGeomStr          = fGetCont->getMdcGeomStruct();
  if( !fLookUpTb || !fSizesCells || !fMdcDetector || !fGeomStr) return kFALSE; 

  fCalCat=gHades->getCurrentEvent()->getCategory(catMdcCal1);
  if (!fCalCat) return kFALSE;
  isGeant=HMdcGetContainers::isGeant();
  
  iter=(HIterator *)fCalCat->MakeIterator();
  for(Int_t i=31; i<51; i++) {
    TColor* cl=gROOT->GetColor(i); 
    Float_t gr=(i-30)/20.;
    cl->SetRGB(1.-gr,gr,0.);
  }
  Int_t pal[12] = {17,16,15,14,13,12, 30,7,5,4,6,3};
  gStyle->SetPalette(12,pal);
  
  loc.set(4,0,0,0,0); 
  
  sprintf(nameCanvEvent,"ProjPlEvent");
  sprintf(titleCanvEvent,"The plots of wires projections.");
  
  return kTRUE;
}

Bool_t HMdcEfficiency::reinit(void) {
  if( isCreated ) return kTRUE;
  if( !HMdcGetContainers::isInited(fLookUpTb) ) return kFALSE;  
  for (Short_t sec = 0; sec < 6; sec++) {
    if( (*array)[sec] || !fMdcDetector->isSectorActive(sec) ) continue;
    (*array)[sec] = new HMdcEfficiencySec(sec);
    (*this)[sec].creatPlots(&(*fLookUpTb)[sec]);
  }
  isCreated=kTRUE;
  return kTRUE;
}

Int_t HMdcEfficiency::execute(void) {
  eventId++;
  //--------Num. clusters have cell ...------------------------
  iter->Reset();
  for(Short_t sec=0; sec<6; sec++) {
    HMdcEfficiencySec& fsec=(*this)[sec];
    if(!&fsec) continue;
    for(Short_t mod=0; mod<4; mod++) {
      HMdcEfficiencyMod& fmod=fsec[mod];
      if(!&fmod) continue;
      for(Short_t lay=0; lay<6; lay++) {
        HMdcEfficiencyLayer& flay=fmod[lay];
        if(&flay) flay.clearClCells();
      }
    }
  }
  
  // Geant data ===========
  if(isGeant) {
    HMdcCal1Sim* cal=0;
    while ((cal=(HMdcCal1Sim *)iter->Next())!=0) {
      Int_t nHits=abs(cal->getNHits());
      if(nHits==0) continue;
      if(nHits==1 && cal->getStatus1()<0) continue;
      if(nHits==2 && cal->getStatus1()<0 && cal->getStatus2()<0) continue;
      Int_t s,m,l,c;
      cal->getAddress(s,m,l,c);
      if(&(*this)[s][m][l]) {
        (*this)[s][m][l].addCell(c);
        (*this)[s][m][l].addCellTot(c);
      }
      if( &(*this)[s][m] ) (*this)[s][m].fillTimesAll(cal,isGeant);
    }
  }
  // Real data ===========
  else {
    HMdcCal1* cal=0;
    while ((cal=(HMdcCal1 *)iter->Next())!=0) {
      Int_t s,m,l,c;
      if(cal->getNHits()==0) continue;
      cal->getAddress(s,m,l,c);
      HMdcEfficiencySec &fsec=(*this)[s];
      if( !&fsec ) continue;
      HMdcEfficiencyMod &fmod=fsec[m];
      if( !&fmod ) continue;
      fmod.fillTimesAll(cal,isGeant);
      HMdcEfficiencyLayer &flay=fmod[l];
      if(c<0 || c >flay.nCells) continue;
//      flay.addCellTot(c);
      flay.nCellTot[c]++;
      if(fcut) {
        if(fcut->cutComStop(cal)) continue;
//        flay.addCellCutS(c);
        flay.nCellCutS[c]++;
        if(fcut->cut(cal)) continue;	 // Cut drift time
      }
//      flay.addCell(c);
      flay.nClustCell[c]++;
      flay.nCellStat[c]++;
    }
  }
  //-----------------------------------------------------------
  for(Int_t sec=0; sec<6; sec++){
    if( ! &((*fLookUpTb)[sec]) ) continue;
    if( ! &((*this)[sec]) ) continue;
    HMdcLookUpTbSec& lookutsec = (*fLookUpTb)[sec];
    //===Selecting of clusters (tracks)===========================
    HMdcEfficiencySec& fsec=(*this)[sec];
    HMdc12Clusters& clusters=lookutsec.getClusters();
    Int_t nclst=clusters.getNClusters();     // Num. of clusters
//    Short_t nLayers[2][nclst];
    Short_t nLayers[4][nclst];
    if(nclst==0) continue;
    for(Int_t nc=0; nc<nclst; nc++) {
      HMdcCluster& clst=clusters[nc];
      nLayers[0][nc]=nLayers[1][nc]=nLayers[2][nc]=nLayers[3][nc]=0;
//      for(Short_t l=0; l<12; l++) 
      for(Short_t l=0; l<24; l++) {
 	Short_t mod=l/6;
        Short_t lay=l%6;
 	Short_t nCells=clst.getNumHits(l);
 	if(nCells<=0) continue;
 	nLayers[mod][nc]++;
 	for(Int_t n=0; n<nCells; n++) {
 	  Short_t cell=clst.getHit(l,n);
 	  fsec[mod][lay].addCellInClust(cell);  // fired wires num.
          if(fsec[mod][lay].nClustCell[cell] == 2) {
            fsec[mod][lay].addCellClus(cell);
            loc[0]=sec; loc[1]=mod; loc[2]=lay; loc[3]=cell;
            HMdcCal1* cal=(HMdcCal1*)fCalCat->getObject(loc);
            fsec[mod].fillTimesCls(cal,isGeant);      //tim1,time2,time2-time1 hist.
          }
 	}
      }
    }
    for(Int_t nc=0; nc<nclst; nc++) {  
//      qual[sec][nc]=1;
      HMdcCluster& clst=clusters[nc];
//      Short_t maxClust=clst.getMaximum();
//      if(maxClust > 6) {
// 	if(maxClust >10) qual[sec][nc]=0;         // Selecting clusters ...
// 	if(nLayers[0][nc] < 5 || nLayers[1][nc] < 5) qual[sec][nc]=0;
//      }
//      else if(maxClust == 6 || (nLayers[0][nc] < 5 && nLayers[1][nc] < 5))
// 							       qual[sec][nc]=0;
//      if(!qual[sec][nc]) continue;
      
//      Int_t nCellsZ[2];
      Int_t nCellsZ[4];
      nCellsZ[0]=nCellsZ[1]=nCellsZ[2]=nCellsZ[3]=-1;
      Short_t cellF=0;
      Short_t cellS=0;
//      for(Short_t mod=0; mod<2; mod++) 
      for(Short_t mod=0; mod<4; mod++) {
        qual[sec][nc][mod]=0;
        if(clst.getMaximum(mod) != 5) continue;
        if(nLayers[mod][nc] == 0) continue;
        if(!&fsec[mod]) continue;
        qual[sec][nc][mod]=1;
 	HMdcEfficiencyMod& fmod=fsec[mod];
 	for(Short_t layer=0; layer<6; layer++) {
 	  HMdcEfficiencyLayer& flay=fmod[layer];
 	  Short_t nCells=clst.getNumHits(mod*6+layer);
 	  //----- Num. fired cells in plane >2
 	  if(nCells>2) qual[sec][nc][mod]=0;
 	  //----- Num. fired cells in plane >0 & <=2
 	  else if(nCells>0){
 	    for(Int_t n=0; n<nCells; n++) {
 	      Short_t cell=clst.getHit(mod*6+layer,n);
 	      if(flay.nClustCell[cell] > 2) qual[sec][nc][mod]=0; //selecting of fired wires in one cluster only
 	      else {
 		// For dubl.hits |n.cell1-n.cell2| must be = 1
 		if(n==0) cellF=cellS=cell;
 		else {
 		  cellS=cell;
 		  if(TMath::Abs(cellF-cellS) != 1) qual[sec][nc][mod]=0;
 		  else {
 		    if(cellF>cellS) {
 		      Short_t tmp=cellF;
 		      cellF=cellS;
 		      cellS=tmp;
 		    }
 		  }
 		}
 	      }
 	    }
 	    // Nearests cells can not be fired
 	    if(qual[sec][nc][mod]) {
 	      if(cellF>0 && flay.nClustCell[cellF-1]>0) qual[sec][nc][mod]=0;
 	      if(flay.nClustCell[cellS+1] >0) qual[sec][nc][mod]=0;
 	    }
 	  }
 	  //----- N. fired cells in plane = 0
 	  else {
 	    HGeomVector trP2(clst.getX(),clst.getY(),clst.getZ());
 	    HGeomVector trP1(0.0,0.0,0.0);  //!!!zamenit' na pol. misheni!!!
 	    HGeomVector pointCros;
            HMdcSizesCellsLayer& fSCLayer = (*fSizesCells)[sec][mod][layer];
            fSCLayer.calcIntersection(trP1,trP2,pointCros);
            pointCros=fSCLayer.getSecTrans()->transTo(pointCros);
 	    Float_t xcoor=(Float_t)pointCros.getX();
 	    Float_t ycoor=(Float_t)pointCros.getY();
            Int_t nZrCell=fSCLayer.calcCellNum(xcoor,ycoor);
 	    if( nZrCell < 0 ) qual[sec][nc][mod]=0;
 	    else {
 	      if(nZrCell>0 && flay.nClustCell[nZrCell-1] >0) 
                qual[sec][nc][mod]=0;
 	      if(flay.nClustCell[nZrCell] >0) qual[sec][nc][mod]=0;
 	      if(nZrCell+1<flay.nCells &&
 		  flay.nClustCell[nZrCell+1] >0) qual[sec][nc][mod]=0;
 	      nCellsZ[mod]=nZrCell;
 	    }
 	  }
 	  if(!qual[sec][nc][mod]) break;
 	}
      // Keeping of the information ========================
        if(qual[sec][nc][mod]) {
          Int_t bNum=fmod.getBinNum(clst.getX(), clst.getY());
          if( bNum<0 ) continue;
 	  //==================================================
 	  for(Int_t lay=0; lay<6; lay++) {
 	    HMdcEfficiencyLayer& flay=fmod[lay];
 	    Short_t nCells=clst.getNumHits(mod*6+lay);
            for(Int_t cell=0; cell<nCells; cell++) {
              loc[0]=sec; loc[1]=mod; loc[2]=lay; 
              loc[3]=clst.getHit(mod*6+lay,cell);
              HMdcCal1* cal=(HMdcCal1*)fCalCat->getObject(loc); 
              fsec[mod].fillTimesSelCls(cal,isGeant);      //tim1,time2,time2-time1 hist.
            }
 	    if(nCells == 0) {
 	      fmod.add0Hit(bNum);
// 	      flay.add0Hit(nCellsZ[mod],bNum);
              flay.n0[nCellsZ[mod]]++;
              flay.n0xy[bNum]++;
 	    } else if(nCells == 1) {
 	      fmod.add1Hit(bNum);
 	      if(nLayers[mod][nc]==6) {
//                flay.add1Hit(clst.getHit(mod*6+lay,0),bNum);
                flay.n1[clst.getHit(mod*6+lay,0)]++;
                flay.n1xy[bNum]++;
              }
 	    } else if(nCells == 2) {
 	      fmod.add2Hit(bNum);
 	      if(nLayers[mod][nc]==6) flay.add2Hit(clst.getHit(mod*6+lay,0),
                clst.getHit(mod*6+lay,1),bNum);
 	    }
 	  }
 	  //==================================================
        }
      }
    }
    for(Int_t nc=0; nc<nclst; nc++) {
      HMdcCluster& clst=clusters[nc];
      Float_t x=clst.getX();
      Float_t y=clst.getY();
      (*this)[sec].getPlotAllHits()->Fill(x,y);
      if(qual[sec][nc][0]+qual[sec][nc][1]+qual[sec][nc][2]+qual[sec][nc][3]>0) 
                     (*this)[sec].getPlotSelHits()->Fill(x,y);
    }
  }
  return 0;
}

void HMdcEfficiency::drawProjPlotsSec(Int_t sec, Int_t type=0) {
  if( ! &((*fLookUpTb)[sec]) ) return;
  if( ! &((*this)[sec]) ) return;
  TCanvas* canv=(*this)[sec].getCanvasProj();
  if( !canv ) return;
  canv->cd();
  canv->cd(1);
  drawProjPlots(sec, type);
  gStyle->SetOptStat(0);
  canv->Modified();
  canv->Update();
}

void HMdcEfficiency::drawPrPlEvent(Int_t type=0) {
  canvEventProj=(TCanvas*)gROOT->GetListOfCanvases()->FindObject(nameCanvEvent);
  if(!canvEventProj) return;
  canvEventProj->cd();
  for(Int_t sec=0; sec<6; sec++) {
    if( ! &((*fLookUpTb)[sec]) ) continue;
    canvEventProj->cd(sec+1);
    drawProjPlots(sec, type);
  }
  canvEventProj->Modified();
  canvEventProj->Update();
}

void HMdcEfficiency::drawCanvEvent() {
  if(gROOT->GetListOfCanvases()->FindObject(nameCanvEvent)) return;
  canvEventProj=new TCanvas(nameCanvEvent,titleCanvEvent,20,1,800,600);
  canvEventProj->SetFillColor(10);
  canvEventProj->Divide(3,2,0.0001,0.0001);
}

void HMdcEfficiency::delCanvEvent(){
  canvEventProj=(TCanvas*)gROOT->GetListOfCanvases()->FindObject(nameCanvEvent);
  if(canvEventProj) {
    delete canvEventProj;
    canvEventProj=0;
  }
  
}

void HMdcEfficiency::drawProjPlots(Int_t sec, Int_t type=0) {
  HMdcLookUpTbSec& lookutsec = (*fLookUpTb)[sec];
  Char_t name[15],title[20];
  sprintf(name,"S%iPlProj",sec+1);
  sprintf(title,"Event %i Sector %i",eventId,sec+1);
  typePrPlot[sec]=type;
  TH2C* plt=lookutsec.fillTH2C(name,title,typePrPlot[sec]);
  plt->SetXTitle("x, (mm)");
  plt->SetYTitle("y, (mm)      ");
  plt->SetMinimum(0.0);
  Int_t modMax=3;
  for(; modMax>=0; modMax--) if(lookutsec.getMaxClus(modMax)) break;
  Float_t mxBn=0;
  if(typePrPlot[sec]==1) for(Int_t mod=0; mod<=modMax; mod++) mxBn+=6*(mod+1);
  else if(typePrPlot[sec]==2) mxBn=4.5;
  else for(Int_t mod=0; mod<4; mod++) if(lookutsec.getMaxClus(mod)) mxBn+=6.0;
  plt->SetMaximum(mxBn);
  plt->SetLabelSize(0.03,"X");
  plt->SetLabelOffset(0.001,"X");
  plt->GetXaxis()->SetTitleSize(0.04);
  plt->SetLabelSize(0.03,"Y");
  plt->SetLabelOffset(0.01,"Y");
  plt->SetTitleOffset(1.2,"Y");
  plt->GetYaxis()->SetTitleSize(0.04);
  plt->Draw("COLZ");
  HMdc12Clusters& clusters=lookutsec.getClusters();
  Int_t nclst=clusters.getNClusters();     // Num. of clusters
  TMarker marker;
//  marker.SetMarkerStyle(24);
  marker.SetMarkerSize(1.4);
  marker.SetMarkerColor(4);
  Int_t mark[4]={24,28,26,25};
  for(Int_t nc=0; nc<nclst; nc++) {
//    if(clusters[nc].getMod()<0) marker.SetMarkerSize(1.4);
//    else marker.SetMarkerSize(1.4+clusters[nc].getMod()*0.3);
//    Int_t qualSeg=0;
//    if(qual[sec][nc][0]+qual[sec][nc][1]+qual[sec][nc][2]+qual[sec][nc][3]>0)
//      qualSeg=1;
//    marker.SetMarkerColor(qualSeg+2);
    Int_t mod=clusters[nc].getMod();
    if(mod<0 || mod>3) marker.SetMarkerStyle(24);
    else marker.SetMarkerStyle(mark[mod]);
    marker.DrawMarker(clusters[nc].getX(),clusters[nc].getY());
  }

  gStyle->SetOptStat(0);
}

void HMdcEfficiency::drawHitsPlots(Int_t sec) {
  if( ! &((*fLookUpTb)[sec]) ) return;
  if( ! &((*this)[sec]) ) return;
  TCanvas* canv=(*this)[sec].getCanvasHits();
  if(canv) {
    canv->cd(1);
    (*this)[sec].getPlotAllHits()->Draw();
    canv->cd(2);
    (*this)[sec].getPlotSelHits()->Draw();
    canv->Modified();
    canv->Update();
  }
}


void HMdcEfficiency::printClusters(Int_t sec) {
  if(&((*fLookUpTb)[sec])==0) return;
  HMdcLookUpTbSec& lookutsec = (*fLookUpTb)[sec];
  HMdc12Clusters& clusters=lookutsec.getClusters();
  clusters.print();
}

void HMdcEfficiency::calcEfficiency(Int_t iBinSum=1) {
  gStyle->SetTitleW(0.5);
  gStyle->SetTitleH(0.09);
  for(Int_t sec=0; sec<6; sec++){
    HMdcEfficiencySec& fsec=(*this)[sec];
    if(!&fsec) continue;
    for(Int_t mod=0; mod<4; mod++){
      HMdcEfficiencyMod& fmod=fsec[mod];
      if(!&fmod) continue;
      for(Int_t lay=0; lay<6; lay++) {
        fmod[lay].fillHists(iBinSum);
        fmod[lay].updateCanv();
        fmod[lay].updateCanvXYlist();
      }
      fmod.fillHistMb();
      fmod.updateCanv();
    }
    drawProjPlotsSec(sec,typePrPlot[sec]);
    drawPrPlEvent(typePrPlot[sec]);
    drawHitsPlots(sec);
  } 
}

Bool_t HMdcEfficiency::finalize(void) {
  // writeHists();
  return kTRUE;
}
