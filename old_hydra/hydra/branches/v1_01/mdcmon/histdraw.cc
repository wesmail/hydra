#include "histdraw.h"
#include <iostream.h>

ClassImp(histDraw)

histDraw::~histDraw(){}

histDraw::histDraw() : TTimer (999,kTRUE){}
histDraw::histDraw(Int_t name, Bool_t fAllMbo, Float_t xmin, Float_t xmax, 
		   Float_t ymin, Float_t ymax, Long_t ms, Int_t secnum, 
		   Int_t modnum, Int_t mbonum, Int_t timehistnum ) 
                   : TTimer (ms,kTRUE) {

  gMdcGUI=this;
  histNum = name;
  allMbo = fAllMbo;
  xMin = xmin;
  xMax = xmax;
  yMin = ymin;
  yMax = ymax;
  millisec = ms;
  secNum = secnum;
  modNum = modnum;
  mboNum = mbonum;
  param = timehistnum;
  sprintf(mapfile,"%s%s",getenv("HADDIR"),"/mdcmon/mdcmon.map");	
  gSystem->AddTimer(this); 
  drawing();
}

void histDraw::drawing(void) {

  // Hardwired!! Should be changed!
  // LookUp array for transformation MBO index to MBO name
  Int_t tabl[16]={111,112,121,122,123,124,211,212,
		  221,222,223,224,311,312,321,322};
  //  Int_t tabl[2][14]={111,112,113,121,122,211,212,213,221,222,311,
  //312,321,322,111,112,121,122,123,124,211,212,221,222,223,224,311,312};

  mboMax[0] = mboMax[1] = 16;
  for (i=0;i<16;i++) mboName[i]=tabl[i];

  mfile = TMapFile::Create(mapfile);
  mfile->Print();
 
  switch (histNum) {
  case TDCDT :
    if(allMbo) { 
      for(j=0;j<mboMax[modNum];j++) {  // plots all MBO for current module
	sprintf(tmp1, "%s%i%s%i%s", "pHistTDCDT[",modNum,"][",j,"]");
	pHistTDCDT[modNum][j] = 
	  (TH2S *) mfile->Get(tmp1,pHistTDCDT[modNum][j]);	  
      }
      c1 = new TCanvas("c1","TDC:DriftTime",200,10,700,780);    
      c1->Divide(4,4);
      if (!gROOT->IsBatch()) {
	for (int i=0;i<mboMax[modNum];i++) {
	  c1->cd(i+1);
	  pHistTDCDT[modNum][i]->GetXaxis()->SetRange(xMin/4,xMax/4);
	  pHistTDCDT[modNum][i]->GetYaxis()->SetRange(yMin,yMax);
	  //	  c1->SetLogz(); 
	  //	  pHistTDCDT[modNum][i]->Draw("box");
	  pHistTDCDT[modNum][i]->Draw();
	}
	c1->Modified();
	c1->Update();
      }
    }	    
    
    else {
      sprintf(tmp1, "%s%i%s%i%s", "pHistTDCDT[",modNum,"][",mboNum-1,"]");
      pHistTDCDT[modNum][mboNum-1] = (TH2S *) 
	mfile->Get(tmp1,pHistTDCDT[modNum][mboNum-1] );      
      c1 = new TCanvas("c1","TDC:DriftTime",200,10,600,700); 
      c1->Divide(1,1);
      pHistTDCDT[modNum][mboNum-1]->GetXaxis()->SetRange(xMin/4,xMax/4);
      pHistTDCDT[modNum][mboNum-1]->GetYaxis()->SetRange(yMin,yMax);
      //      c1->SetLogz(); 
      //      pHistTDCDT[modNum][mboNum-1]->Draw("box");
      pHistTDCDT[modNum][mboNum-1]->Draw();
      c1->Modified();
      c1->Update();
    }
    mfile->Close();
    
    
    break;
  case DT :
    if(allMbo) {
      for(j=0;j<mboMax[modNum];j++) {  // plots all MBO for current module
	sprintf(tmp1, "%s%i%s%i%s", "pHistT1[",modNum,"][",j,"]");
	sprintf(tmp2, "%s%i%s%i%s", "pHistT2[",modNum,"][",j,"]");
	sprintf(tmp3, "%s%i%s%i%s", "pHistTD[",modNum,"][",j,"]");
	pHistT1[modNum][j] =
	  (TH1S *) mfile->Get(tmp1,pHistT1[modNum][j]);	  
	pHistT2[modNum][j] =
	  (TH1S *) mfile->Get(tmp2,pHistT2[modNum][j]);	  
	pHistTD[modNum][j] =
	  (TH2S *) mfile->Get(tmp3,pHistTD[modNum][j]);	  
      }
      switch(param) {  // param 1 - time1, 2 - time2, 12 - time1-time2
      case 1:
	c1 = new TCanvas("c1","Drift Time1",200,10,700,780);    
      c1->Divide(4,4);
	for (int i=0;i<mboMax[modNum];i++) {
	  c1->cd(i+1);
	  pHistT1[modNum][i]->GetXaxis()->SetRange(xMin,xMax);
	  pHistT1[modNum][i]->GetYaxis()->SetRange(yMin,yMax);
	  pHistT1[modNum][i]->Draw();
	}
	break;
      case 2:
      c1 = new TCanvas("c1","Drift Time2",200,10,700,780);    
      c1->Divide(4,4);
	for (int i=0;i<mboMax[modNum];i++) {
	  c1->cd(i+1);
	  pHistT2[modNum][i]->GetXaxis()->SetRange(xMin,xMax);
	  pHistT2[modNum][i]->GetYaxis()->SetRange(yMin,yMax);
	  pHistT2[modNum][i]->Draw();
	}
	break;
      case 12:
	c1 = new TCanvas("c1","Time1 - Time2",200,10,700,780);    
	c1->Divide(4,4);
	for (int i=0;i<mboMax[modNum];i++) {
	  c1->cd(i+1);
	  pHistTD[modNum][i]->GetXaxis()->SetRange(xMin/4,xMax/4);
	  pHistTD[modNum][i]->GetYaxis()->SetRange(yMin,yMax);
	  //	  c1->SetLogz(); 
	  //	  pHistTD[modNum][i]->Draw("box");
	  pHistTD[modNum][i]->Draw();
	}
	break;
      }
    }

    else{
      sprintf(tmp1, "%s%i%s%i%s", "pHistT1[",modNum,"][",mboNum-1,"]");
      sprintf(tmp2, "%s%i%s%i%s", "pHistT2[",modNum,"][",mboNum-1,"]");
      sprintf(tmp3, "%s%i%s%i%s", "pHistTD[",modNum,"][",mboNum-1,"]");
      pHistT1[modNum][mboNum-1] = 
	(TH1S *) mfile->Get(tmp1,pHistT1[modNum][mboNum-1] );
      pHistT2[modNum][mboNum-1] = 
	(TH1S *) mfile->Get(tmp2,pHistT2[modNum][mboNum-1] );
      pHistTD[modNum][mboNum-1] = 
	(TH2S *) mfile->Get(tmp3,pHistTD[modNum][mboNum-1] );
      mfile->Close();
      switch(param) {
      case 1:
	c2 = new TCanvas("c2","Time1",200,10,500,600); 
	pHistT1[modNum][mboNum-1]->GetXaxis()->SetRange(xMin,xMax);
	pHistT1[modNum][mboNum-1]->GetYaxis()->SetRange(yMin,yMax);
	pHistT1[modNum][mboNum-1]->Draw();
	break;
      case 2:
	c2 = new TCanvas("c2","Time2",200,10,500,600); 
	pHistT2[modNum][mboNum-1]->GetXaxis()->SetRange(xMin,xMax);
	pHistT2[modNum][mboNum-1]->GetYaxis()->SetRange(yMin,yMax);
	pHistT2[modNum][mboNum-1]->Draw();
	break;
      case 12:
	c2 = new TCanvas("c2","Time1-Time2",200,10,500,600); 
	pHistTD[modNum][mboNum-1]->GetXaxis()->SetRange(xMin/4,xMax/4);
	pHistTD[modNum][mboNum-1]->GetYaxis()->SetRange(yMin,yMax);
	//	c1->SetLogz(); 
	//	pHistTD[modNum][mboNum-1]->Draw("box");
	pHistTD[modNum][mboNum-1]->Draw();
	break;
      default:
	break;
      }
      c2->Modified();
      c2->Update();    
    }   

    break;
  case TDC :
    if(param==0) { // param corespodns to the number of TDC channel
      if(allMbo) { 
	for(j=0;j<mboMax[modNum];j++) {
	  sprintf(tmp1, "%s%i%s%i%s", "pHistTDC[",modNum,"][",j,"]");
	  pHistTDC[modNum][j] = (TH1S *) mfile->Get(tmp1,pHistTDC[modNum][j] ); 
	}
	c1 = new TCanvas("c1","TDC",200,10,700,780);
	
	c1->Divide(4,4);
	if (!gROOT->IsBatch()) {
	  for (int i=0;i<mboMax[modNum];i++) {
	    c1->cd(i+1);
	    //	pHist[modNum][i]->GetXaxis()->SetLimits(xmin,xmax);
	    pHistTDC[modNum][i]->Draw();
	  }
	  c1->Modified();
	  c1->Update();
	}
      }	    
      else {
	sprintf(tmp1, "%s%i%s%i%s", "pHistTDC[",modNum,"][",mboNum-1,"]");
	pHistTDC[modNum][mboNum-1] = (TH1S *) 
	  mfile->Get(tmp1,pHistTDC[modNum][mboNum-1] ); 
	c1 = new TCanvas("c1","TDC",200,10,600,700); 
	pHistTDC[modNum][mboNum-1]->Draw();
	c1->Modified();
	c1->Update();
      }
      //      mfile->Close();
    }
    else {  // 1-dim histograms fot time1&time2 for selected TDC
      sprintf(tmp1, "%s%i%s%i%s", "pHistTDCDT[",modNum,"][",mboNum-1,"]");
      pHistTDCDT[modNum][mboNum-1] = (TH2S *) 
	mfile->Get(tmp1,pHistTDCDT[modNum][mboNum-1] );
      for(i=0;i<2;i++) {
	for(j=0;j<8;j++) {
	  htest[i][j]=pHistTDCDT[modNum][mboNum-1]->
	    ProjectionX("htest",100*i+param*8+j+1,100*i+param*8+j+1);
	}
      }
      sprintf(tmp1,"%s%i","TDC ",param+1);
      c2 = new TCanvas("c2",tmp1,200,10,700,780);
      c2->Divide(4,4);
      for(i=0;i<2;i++) {
	for(j=0;j<8;j++) {
	  c2->cd(i*8+j+1); 
	  sprintf(tmp1,"%s%i%s%i%s%i","TDC ",param+1,"  chip ",j+1, 
		  "  time",i+1);
	  htest[i][j]->SetTitle(tmp1);
	  htest[i][j]->Draw();
	}
      }
      c2->Modified();
      c2->Update();
    }    
    break;
    
  case MULT :
    pHistMult[0]  = (TH1S *) mfile->Get("pHistMult[0]",pHistMult[0] );
    pHistMult[1]  = (TH1S *) mfile->Get("pHistMult[1]",pHistMult[1] );
    c2 = new TCanvas("c2","Number of hits per event",200,10,600,500); 
    pHistMult[modNum]->Draw();
    c2->Modified();
    c2->Update();
    break;
  }
  
  //  this->SetTime(ms); // SetTime doesn't work?? 
  
}

Bool_t histDraw::Notify() {
  TTimer::Reset();
  drawing();
  return kFALSE;
}

void histDraw::stop() {
  TTimer::TurnOff();
  if (c1) delete c2;
  else delete c1;
}



histDraw *gMdcGUI;
