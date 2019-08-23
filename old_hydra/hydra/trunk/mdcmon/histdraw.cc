#include "histdraw.h"
#include <iostream.h>

ClassImp(histDraw)

histDraw::~histDraw(){}

histDraw::histDraw() : TTimer (999,kTRUE){}
histDraw::histDraw(Int_t name, Bool_t fAllMbo, Float_t xmin, Float_t xmax, 
		   Float_t ymin, Float_t ymax, Long_t ms, Int_t secnum, 
		   Int_t modnum, Int_t mbonum, Int_t chipnum, 
		   Int_t timehistnum ) : TTimer (ms,kTRUE) {

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
  chipNum = chipnum;
  param = timehistnum;
  //  gSystem->AddTimer(this); 
  drawing();
} 


void histDraw::drawing(void) {

  // Hardwired!! Should be changed!
  // LookUp array for transformation MBO index to MBO name
  //  Int_t tabl[16]={111,112,121,122,123,124,211,212,221,222,223,224,311,312,321,322};
  //  Int_t tabl[16]={111,112,113,121,122,211,212,213,221,222,311,312,321,322,0,0};
 
  mboMax = 16;
  //  for (i=0;i<16;i++) mboName[i]=tabl[i];
  //  sprintf(mapfile,"%s%s",getenv("HADDIR"),"/mdcmon/mdcmon.map");	
  //  sprintf(mapfile,"%s%s%s","/tmp/",getenv("USER"),"/mdcmon.map");
  sprintf(mapfile,"%s%s","/tmp/","mdcmon.map");
  //  sprintf(mapfile,"%s","/tmp/mdcmon.map");
  mfile = TMapFile::Create(mapfile);
  mfile->Print();
 
  switch (histNum) {
  case TDCDT :
    if(allMbo) { 
      for(j=0;j<mboMax;j++) {  // plots all MBO for current module
	sprintf(tmp1, "%s%i%s", "pHistTDCDT[",j,"]");
	pHistTDCDT[j] = 
	  (TH2S *) mfile->Get(tmp1,pHistTDCDT[j]);	  
      }
      c1 = new TCanvas("c1","TDC:DriftTime",200,10,700,780);    
      c1->Divide(4,4);
      if (!gROOT->IsBatch()) {
	for (int i=0;i<mboMax;i++) {
	  c1->cd(i+1);
	  pHistTDCDT[i]->GetXaxis()->SetRange(xMin/4,xMax/4);
	  pHistTDCDT[i]->GetYaxis()->SetRange(yMin,yMax);
	  //	  c1->SetLogz(); 
	  //	  pHistTDCDT[modNum][i]->Draw("box");
	  gPad->SetLogz(); 
	  pHistTDCDT[i]->Draw("colz");
	}
	c1->Modified();
	c1->Update();
      }
    }	    
    
    else {
      sprintf(tmp1, "%s%i%s", "pHistTDCDT[",mboNum-1,"]");
      pHistTDCDT[mboNum-1] = (TH2S *) 
	mfile->Get(tmp1,pHistTDCDT[mboNum-1] );      
      c1 = new TCanvas("c1","TDC:DriftTime",200,10,600,700); 
      c1->Divide(1,1);
      pHistTDCDT[mboNum-1]->GetXaxis()->SetRange(xMin/4,xMax/4);
      pHistTDCDT[mboNum-1]->GetYaxis()->SetRange(yMin,yMax);
      //      c1->SetLogz(); 
      //      pHistTDCDT[modNum][mboNum-1]->Draw("box");
      pHistTDCDT[mboNum-1]->Draw();
      c1->Modified();
      c1->Update();
    }
    mfile->Close();
    break;
  case DT :
    if(allMbo) {
      for(j=0;j<mboMax;j++) {  // plots all MBO for current module
	for(k=0;k<12;k++) {  // loop over all 8 chips
	  for(l=0;l<8;l++) {  // loop over all 8 channels for current chip
	    sprintf(tmp1, "%s%i%s%i%s%i%s", "pHistT1[",j,"][",k,"][",l,"]");
	    sprintf(tmp2, "%s%i%s%i%s%i%s", "pHistT2[",j,"][",k,"][",l,"]");
	    pHistT1[j][k][l] =
	      (TH1S *) mfile->Get(tmp1,pHistT1[j][k][l]);
	    pHistT2[j][k][l] =
	      (TH1S *) mfile->Get(tmp2,pHistT2[j][k][l]);	  
	    if(!(k==0&&l==0)) {
	      pHistT1[j][0][0]->Add(pHistT1[j][0][0],pHistT1[j][k][l],1.,1.);	  
	      pHistT2[j][0][0]->Add(pHistT2[j][0][0],pHistT2[j][k][l],1.,1.); 
	    }
	  }
	}
	sprintf(tmp3, "%s%i%s", "pHistTD[",j,"]");
	
	pHistTD[j] =
	  (TH2S *) mfile->Get(tmp3,pHistTD[j]);	  
      }

      switch(param) {  // param 1 - time1, 2 - time2, 12 - time1-time2
      case 1:
	c1 = new TCanvas("c1","Drift Time1",200,10,700,780);    
	c1->Divide(4,4); 
	for (i=0;i<mboMax;i++) {
	  c1->cd(i+1);
    	  pHistT1[i][0][0]->GetXaxis()->SetRange(xMin,xMax);
	  pHistT1[i][0][0]->GetYaxis()->SetRange(yMin,yMax);
	  pHistT1[i][0][0]->Draw();
	}
	break;
      case 2:
      c1 = new TCanvas("c1","Drift Time2",200,10,700,780);    
      c1->Divide(4,4);
	for (int i=0;i<mboMax;i++) {
	  c1->cd(i+1);
	  pHistT2[i][0][0]->GetXaxis()->SetRange(xMin,xMax);
	  pHistT2[i][0][0]->GetYaxis()->SetRange(yMin,yMax);
	  pHistT2[i][0][0]->Draw();
	}
	break;
      case 12:
	c1 = new TCanvas("c1","Time1 - Time2",200,10,700,780);    
	c1->Divide(4,4);
	for (int i=0;i<mboMax;i++) {
	  c1->cd(i+1);
	  pHistTD[i]->GetXaxis()->SetRange(xMin/4,xMax/4);
	  pHistTD[i]->GetYaxis()->SetRange(yMin,yMax);
	  //	  c1->SetLogz(); 
	  //	  pHistTD[modNum][i]->Draw("box");
	  pHistTD[i]->Draw();
	}
	break;
      }
    }

    else{
      for(l=0;l<8;l++) { // loop over all channels in the current chip
	sprintf(tmp1, "%s%i%s%i%s%i%s", "pHistT1[",mboNum-1,"][",chipNum,"][",l,"]");
	sprintf(tmp2, "%s%i%s%i%s%i%s", "pHistT2[",mboNum-1,"][",chipNum,"][",l,"]");
	sprintf(tmp3, "%s%i%s", "pHistTD[",mboNum-1,"]");
	pHistT1[mboNum-1][chipNum][l] = 
	  (TH1S *) mfile->Get(tmp1,pHistT1[mboNum-1][chipNum][l] );
	pHistT2[mboNum-1][chipNum][l] = 
	  (TH1S *) mfile->Get(tmp2,pHistT2[mboNum-1][chipNum][l] );
      }
      pHistTD[mboNum-1] = 
	(TH2S *) mfile->Get(tmp3,pHistTD[mboNum-1] );
      mfile->Close();
      switch(param) {
      case 1:
	c2 = new TCanvas("c2","Time1",200,10,500,600); 
	c2->Divide(3,3);
	for(l=0;l<8;l++) {
	  c2->cd(l+1);
	  pHistT1[mboNum-1][chipNum][l]->GetXaxis()->SetRange(xMin,xMax);
	  pHistT1[mboNum-1][chipNum][l]->GetYaxis()->SetRange(yMin,yMax);
	  pHistT1[mboNum-1][chipNum][l]->Draw();
	}
	break;
      case 2:
	c2 = new TCanvas("c2","Time2",200,10,500,600);
 	c2->Divide(3,3);
	for(l=0;l<8;l++) {
	  c2->cd(l+1);
	  pHistT2[mboNum-1][chipNum][l]->GetXaxis()->SetRange(xMin,xMax);
	  pHistT2[mboNum-1][chipNum][l]->GetYaxis()->SetRange(yMin,yMax);
	  pHistT2[mboNum-1][chipNum][l]->Draw();
	}
	break;
      case 12:
	c2 = new TCanvas("c2","Time1-Time2",200,10,500,600); 
	pHistTD[mboNum-1]->GetXaxis()->SetRange(xMin/4,xMax/4);
	pHistTD[mboNum-1]->GetYaxis()->SetRange(yMin,yMax);
	//	c1->SetLogz(); 
	//	pHistTD[modNum][mboNum-1]->Draw("box");
	pHistTD[mboNum-1]->Draw();
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
	for(j=0;j<mboMax;j++) {
	  sprintf(tmp1, "%s%i%s", "pHistTDC[",j,"]");
	  pHistTDC[j] = (TH1S *) mfile->Get(tmp1,pHistTDC[j] ); 
	}
	c1 = new TCanvas("c1","TDC",200,10,700,780);
	
	c1->Divide(4,4);
	if (!gROOT->IsBatch()) {
	  for (int i=0;i<mboMax;i++) {
	    c1->cd(i+1);
	    //	pHist[modNum][i]->GetXaxis()->SetLimits(xmin,xmax);
	    pHistTDC[i]->Draw();
	  }
	  c1->Modified();
	  c1->Update();
	}
      }	    
      else {
	sprintf(tmp1, "%s%i%s", "pHistTDC[",mboNum-1,"]");
	pHistTDC[mboNum-1] = (TH1S *) 
	  mfile->Get(tmp1,pHistTDC[mboNum-1] ); 
	c1 = new TCanvas("c1","TDC",200,10,600,700); 
	pHistTDC[mboNum-1]->Draw();
	c1->Modified();
	c1->Update();
      }
      //      mfile->Close();
    }
    else {  // 1-dim histograms fot time1&time2 for selected TDC
      sprintf(tmp1, "%s%i%s%i%s", "pHistTDCDT[",modNum,"][",mboNum-1,"]");
      pHistTDCDT[mboNum-1] = (TH2S *) 
	mfile->Get(tmp1,pHistTDCDT[mboNum-1] );
      for(j=0;j<8;j++) {
	htest[j]=pHistTDCDT[mboNum-1]->
	  ProjectionX("htest",100*i+param*8+j+1,100*i+param*8+j+1);
      }
      sprintf(tmp1,"%s%i","TDC ",param+1);
      c2 = new TCanvas("c2",tmp1,200,10,700,780);
      c2->Divide(4,4);
      for(j=0;j<8;j++) {
	c2->cd(8+j+1); 
	sprintf(tmp1,"%s%i%s%i%s%i","TDC ",param+1,"  chip ",j+1, 
		"  time",1);
	htest[j]->SetTitle(tmp1);
	htest[j]->Draw();
	}
      
      c2->Modified();
      c2->Update();
    }    
    break;
    
  case MULT :
    pHistMult  = (TH1S *) mfile->Get("pHistMult",pHistMult );
    c2 = new TCanvas("c2","Number of hits per event",200,10,600,500);
    pHistMult->Draw();
    c2->Modified();
    c2->Update();
    break;
  }
  
  //  this->SetTime(ms); // SetTime doesn't work?? 
  
}

Bool_t histDraw::Notify() {
  TTimer::Reset();
  cout << "From Notify() " << endl;
  drawing();
  return kFALSE;
}

void histDraw::stop() {
  TTimer::TurnOff();
  if (c1) delete c2;
  else delete c1;
}

histDraw *gMdcGUI;
