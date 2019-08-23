// File: hrichpedestalraw.cc
//
//Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
//Date: Nov 14 1999
//
// HRichPedestal is a task class that is used to calculate the average and the sigma for each channel (pad) of a sector of the RICH photon detector. Furthermore, a file with Offset Info for the HRichCalibrater class and a file with pedestal and sigma info for the DAQ is created.   
// this class reads from the RAW container and should therefore be used to produce 
// pedestal and calibration files.
#include "hrichpedestalraw.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hrichdetector.h"
#include "hcategory.h"
#include "hmatrixcatiter.h"
//#include "hlocation.h"
#include "hrichraw.h"
//#include "hrichcal.h"
//#include "hrichcalibrater.h"
//#include "hdebug.h"
#include "richdef.h"
#include "hrichgeometrypar.h"
#include "hdetgeompar.h"
#include "hruntimedb.h"
#include <TFile.h>

HRichPedestalRaw::HRichPedestalRaw(Text_t *name, Text_t *title, Int_t numSec) : 
  HReconstructor(name,title) { 
  iSec = numSec;
  pRichRaw = 0;
}
HRichPedestalRaw::HRichPedestalRaw(){
  
  pRichRaw = 0;
}
HRichPedestalRaw::~HRichPedestalRaw(void) {
}

Bool_t HRichPedestalRaw::init() {
  
  HRichDetector *pRichDet = 
    (HRichDetector*)gHades->getSetup()->getDetector("Rich");
  
  pRichRaw=gHades->getCurrentEvent()->getCategory(catRichRaw);
  
  nCounter= 0 ;
  
  HRuntimeDb* rtdb=gHades->getRuntimeDb();
  HRichGeometryPar *pGeomPar = (HRichGeometryPar*)rtdb->
    getContainer("RichGeometryParameters");
  
  rowMax= pGeomPar->getRows(); 
  colMax = pGeomPar->getColumns();
  
  //cout<<" rowMax  : "<<rowMax<<endl;
  //cout<<" colMax    : "<<colMax<<endl;
  if (!pRichRaw) {
    pRichRaw=pRichDet->buildCategory(catRichRaw);
    
    if (!pRichRaw) return kFALSE;
    else gHades->getCurrentEvent()
	   ->addCategory(catRichRaw, pRichRaw, "Rich");
  }
  TFile *pFile = new TFile("uipfile_new.root","read","Testfile",1);
  if(pFile){
    
    vec3 = (TVector*) pFile->Get("uip");    //retrieve object from file
    vec5 = (TVector*) pFile->Get("pads_x"); //and keyname
    vec6 = (TVector*) pFile->Get("pads_y"); 
    
  }
  else cout << "problems opening upifile" << endl;
  
  for (int i=0;i<colMax;i++){
    for (int j=0;j<rowMax ;j++){
      sum[i][j] = 0;
      sum2[i][j] = 0;
    }
  }
  return kTRUE;
}

Bool_t HRichPedestalRaw::finalize(void) {
  
  char title1[15];
  sprintf(title1,"mean sec:%d",iSec);
  char title2[15];
  sprintf(title2,"sigma sec:%d",iSec);
  char title3[15];
  sprintf(title3,"mean sec:%d 1D",iSec);
  char title4[15];
  sprintf(title4,"sigma sec:%d 1D",iSec);
  
  TH2F *meanHisto = new TH2F(title1,title1,colMax,1,colMax,rowMax,1,rowMax);
  TH2F *sigmaHisto = new TH2F(title2,title2,colMax,1,colMax,rowMax,1,rowMax);
  TH1F *meanHisto1d = new TH1F(title3,title3,17464,1,17464);
  TH1F *sigmaHisto1d = new TH1F(title4,title4,17464,1,17464);
  
  for (int i=0;i<colMax;i++){
    for (int j=0;j<rowMax ;j++){
      
      sum[i][j] = sum[i][j]/nCounter;
      sum2[i][j] = sum2[i][j]/nCounter;
      Float_t radikand = sum2[i][j]-sum[i][j]*sum[i][j];
      
      if (radikand >= 0 ) {
	sum2[i][j] = sqrt(radikand);
	meanHisto->Fill(i,j,sum[i][j]);
	sigmaHisto->Fill(i,j,sum2[i][j]);  
      }else{
	cout<<"!!!!!! A numerical problem has occured at event no "<<nCounter<<" , pedestal generation aborted !!!!!"<<endl;
	cout<<" radikand   :"<<radikand<<"   i : "<<i<<"      j : "<<j<<endl;
	return kFALSE;
      }
      
    }
  }
  cout<<"Real number of events :"<<nCounter<<endl;
  Int_t xCor,yCor;
  xCor=yCor=0;
  for(Int_t uip=0;uip<17464;uip++){
    //int num = 100*i +j;
    xCor = (Int_t)(*vec5)(uip);
    yCor = (Int_t)(*vec6)(uip);
    meanHisto1d->Fill(uip,sum[xCor][yCor]);
    sigmaHisto1d->Fill(uip,sum2[xCor][yCor]);
  } 
  createPedestal();
  createCalibration();
  // visualize average and sigma
  gStyle->SetPalette(1,0); //activate pretty spectral palette
  gStyle->SetStatX(5);// do not draw statistics box
  gStyle->SetStatY(5);
  
  
  
  TCanvas *canvas1 = new TCanvas("Mean","Mean",400,400);
  TPad *pad1 = new TPad("pad1","pad1",.01,.01,.99,.99,0);
  pad1->SetBorderSize(0);
  pad1->SetRightMargin(0.15);
  pad1->Draw();
  
  TCanvas *canvas2 = new TCanvas("Sigma","Sigma",400,400);
  TPad *pad2 = new TPad("pad2","pad2",.01,.01,.99,.99,0);
  pad2->SetBorderSize(0);
  pad2->SetRightMargin(0.15);
  pad2->Draw();
  
  TCanvas *canvas3 = new TCanvas("Mean1d","Mean1d",400,400);
  TPad *pad3 = new TPad("pad3","pad3",.01,.01,.99,.99,0);
  pad3->SetBorderSize(0);
  pad3->SetRightMargin(0.15);
  pad3->Draw();
  
  TCanvas *canvas4 = new TCanvas("Sigma1d","Sigma1d",400,400);
  TPad *pad4 = new TPad("pad4","pad4",.01,.01,.99,.99,0);
  pad4->SetBorderSize(0);
  pad4->SetRightMargin(0.15);
  pad4->Draw();
  
  canvas1->cd();
  pad1->cd();
  meanHisto->SetMaximum(1023);
  meanHisto->Draw("colz");
  canvas1->Update();
  
  canvas2->cd();
  pad2->cd();
  sigmaHisto->SetMaximum(50);
  sigmaHisto->Draw("colz");
  canvas2->Update();
  
  canvas3->cd();
  pad3->cd();
  meanHisto1d->SetMaximum(1023);
  meanHisto1d->Draw();
  canvas3->Update();
  
  canvas4->cd();
  pad4->cd();
  sigmaHisto1d->SetMaximum(50);
  sigmaHisto1d->Draw();
  canvas4->Update();
  
  // ------------------
  return kTRUE;
}

Int_t HRichPedestalRaw::execute() {
  HMatrixCatIter *pIter;
  HRichRaw *pRaw=NULL;
  //  cout<<"bin in execute"<<endl;
  pIter = (HMatrixCatIter*)pRichRaw->MakeIterator(); 
  //cout<<"nCounter ......"<<nCounter<<endl;
  //cout<<" pointer auf Iterator  :"<<pIter<<endl;
  pRaw = (HRichRaw *)pIter->Next();
  //cout<<" pointer auf RAW container  :"<<pRaw<<endl;
  
  Bool_t kData = kFALSE;
while((pRaw = (HRichRaw *)pIter->Next()))
    // !!! for first iteration of execute function, pointer to first element 
    // of RAW container is zero !!!
    {
      kData = kTRUE;
      //  cout<<" in while      "<<nCounter<<endl;
      //cout<<"in while"<<endl;
      Int_t sec = pRaw->getSector();
      if (sec != iSec) break;
      
      Float_t fCharge = pRaw->getCharge();
      
      Int_t iRow = pRaw->getRow();
      Int_t iCol = pRaw->getCol();
      //if (nCounter == 0){
      //cout<<" ******** "<<iRow<<"  "<<iCol<<"   "<<fCharge<<"*******"<<endl;
      //}
      sum[iCol][iRow]=sum[iCol][iRow]+fCharge;
      sum2[iCol][iRow]=sum2[iCol][iRow]+fCharge*fCharge;
      
    }
  if (kData) nCounter++;
// for (int i=0;i<colMax;i++){
//     for (int j=0;j<rowMax ;j++){
      
//       sum[i][j] = sum[i][j]/nCounter;
//       sum2[i][j] = sum2[i][j]/nCounter;
//       Float_t radikand = sum2[i][j]-sum[i][j]*sum[i][j];
      
//       if (radikand >= 0 ) {
// 	sum2[i][j] = sqrt(radikand);
// 	//meanHisto->Fill(i,j,sum[i][j]);
// 	//sigmaHisto->Fill(i,j,sum2[i][j]);  
//       }else{
// 	cout<<"!!!!!! A numerical problem has occured in execute at event no "<<nCounter<<" , pedestal generation aborted !!!!!"<<endl;
// 	cout<<" radikand  in execute :"<<radikand<<" /\ Channel  i : "<<i<<"      j : "<<j<<endl;
// 	//return kFALSE;
//       }
      
  //   }
  //  }
    return kTRUE;
}
void HRichPedestalRaw::createPedestal(){
  char cPedestal1[40];

  sprintf(cPedestal1,"pedestal_%d",iSec);
  strcat(cPedestal1, ".dat");
  Int_t rc,tmp,port,mod,ch,xcor,ycor;
  rc=tmp=port=mod=ch=xcor=ycor=0;
  ofstream pedestalOut(cPedestal1,ios::out);
  Int_t iCount ,iUip1;
  iCount=iUip1=0;
TFile *pUnConn = new TFile("unconnected_pads.root","read","file contains pads that are not connected",1);
TVector *pUncVec = (TVector*) pUnConn->Get("uip unconnected");  
  //***********
  for(Int_t iLauf=0;iLauf<17464;iLauf++){
      
    iUip1=iLauf;
    for (Int_t nCount=0;nCount<4;nCount++){
      
      iUip1 = iLauf+(nCount*16);
      if (iUip1<17464){
	if ((*vec3)(iUip1)){
	  
	  rc = iUip1/10000;
	  tmp = iUip1 % 10000;
	  port = tmp/1000;
	  tmp = tmp % 1000;
	  mod = tmp/100;
	  ch = tmp % 100;
	  
	  xcor = (Int_t)(*vec5)(iUip1);
	  ycor = (Int_t)(*vec6)(iUip1);
	  pedestalOut<<rc<<" "<<port<<" "<<mod<<" "<<ch<<"  "<<sum[xcor][ycor]<<"  "<<sum2[xcor][ycor]<<endl;
	  
	}else{ 
	  if((*pUncVec)(iUip1)){
	  rc = iUip1/10000;
	  tmp = iUip1 % 10000;
	  port = tmp/1000;
	  tmp = tmp % 1000;
	  mod = tmp/100;
	  ch = tmp % 100;
	  pedestalOut<<rc<<" "<<port<<" "<<mod<<" "<<ch<<"  "<<"1023"<<"  "<<"0"<<endl; 
	  }
	}
      }
    }
  
    iCount++;
    if(iCount == 16){
      iCount = 0;
      iLauf=iLauf+84;
    }
  }

  //*************
  pedestalOut.close();
  pUnConn->Close();
  cout<<"pedestal file for sector "<<iSec<<" written"<<endl; 
  
}
void HRichPedestalRaw::createCalibration(){
  char cCalibra[40];
  // Int_t sector =0 ;
  sprintf(cCalibra,"calpar_%d",iSec);
  strcat(cCalibra, ".txt");
  
  ofstream calOut(cCalibra,ios::out);
  calOut<<"# ASCII calibration file"<<endl<<endl<<endl;
  calOut<<"[ Rich Calibrater Parameters Setup ]"<<endl<<endl;
  calOut<<"Sector	Row	Column"<<endl;
  //                                                         Y                 X                               checked in padpin_orig
  calOut<<iSec+1<<"	90	92"<<endl<<endl;
  calOut<<"[ Rich Calibrater Parameters: Sector "<<iSec<<" ]"<<endl<<endl;
  calOut<<"Row	Column	Slope	Offset"<<endl;
  
  for (Int_t i = 0;i<colMax;i++){
    for (Int_t j = 0;j<rowMax;j++){
      //row (Y)     col(X) !!!

      calOut<<j<<"	"<<i<<"	1"<<"	"<<sum[i][j]<<endl;
      //calOut<<j<<"	"<<i<<"	1"<<"	"<<0<<endl;
    }
  }
  calOut.close();
  
  cout<<"calibration file for sector "<<iSec<<" written"<<endl; 
  
}
ClassImp(HRichPedestalRaw)
