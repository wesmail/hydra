// File: hcreatecorrpar.cc
//
// Author: Laura Fabbietti <L.Fabbietti@physik.tu-muenchen.de>
// Last update by Laura Fabbietti: 03/02/05 18:02:53
//
// This class calculated the possible Dtheta Dphi cuts between different 
// detectors, using the data contained in the HHitMatch class 
//
//
#include "hcreatecorrpar.h"
#include "hspectrometer.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "hdetector.h"
#include "hcategory.h"
#include "hiterator.h"
#include <iostream.h>
#include "hdebug.h"
#include "hades.h"
#include "richdef.h"
#include "hhitmatchheader.h"
#include "hhitmatch.h"
#include "hlinearcategory.h"
#include "hrichutilfunc.h"
#include "hrichcut.h"
#include "hrichpaircreator.h"

ClassImp(HCreateCorrPar)

HCreateCorrPar::HCreateCorrPar(Text_t *name,Text_t *title) :
  HReconstructor(name,title)
{

}
HCreateCorrPar::HCreateCorrPar(Text_t *name,Text_t *title, char* filename,Int_t iRichQual) :
  HReconstructor(name,title)
{
    iRichQualThreshold =  iRichQual;
    pFileName  = filename;// output filename for diagnostic histos
}

HCreateCorrPar::~HCreateCorrPar(void) {

}
Bool_t HCreateCorrPar::init() {
        if (gHades) {
	HEvent *event=gHades->getCurrentEvent();
	HRuntimeDb *rtdb=gHades->getRuntimeDb();
	HSpectrometer *spec=gHades->getSetup();
	if (event && rtdb) {
	    HDetector *rich = spec->getDetector("Rich");
	    
	    if (rich) { //Has the user set up a RICH?

		pHitMatchCat=event->getCategory(catMatchHit);
		if (!pHitMatchCat) 
		{
		    pHitMatchCat=rich->buildCategory(catMatchHit);
		    
		    if (!pHitMatchCat) {
			Error("init","No HIT MATCH category defined");
			return kFALSE;
		    }
		    else event->addCategory(catMatchHit, pHitMatchCat, "Rich");
		}
		
		pIterMatchHit = (HIterator*)getHitMatchCat()->MakeIterator("native");
		
		

	    }
	}
	}
	
	pFileOut = new TFile(pFileName,"RECREATE");
	bin1 = 80;
	fPhiCut = 3.;
	iniHistos();
	count = 0;
	
	
	return kTRUE;
}
Bool_t HCreateCorrPar::isGoodRing(HHitMatch *pHM){

    if(pHM->getRingPatMat()>iRichQualThreshold) return kTRUE;
    else return kFALSE;

}
Int_t HCreateCorrPar::execute(){

    HHitMatch *pHM=0;
    Int_t nNbCorrObjs=0;//nb of HHitMatch objs in evt
    pIterMatchHit->Reset();
    while(( pHM= (HHitMatch *)pIterMatchHit->Next())) nNbCorrObjs++;
    //    cout<<" execute evt "<<count<<endl;
    count++;
    // helper arrays to determine number of diff indexes of hits
    Int_t *tRichInd = new Int_t[nNbCorrObjs];//cannot be bigger than nb of corrs
    Int_t *tRichMdcInd = new Int_t[nNbCorrObjs];
    Int_t *tRichTofInd = new Int_t[nNbCorrObjs];
    Int_t *tRichTofinoInd = new Int_t[nNbCorrObjs];
    Int_t *tRichShowerInd = new Int_t[nNbCorrObjs];

    Float_t *tToftofAccep = new Float_t[nNbCorrObjs];
    Float_t *tToftofRej = new Float_t[nNbCorrObjs];
    Float_t *dThetaRM = new Float_t[nNbCorrObjs];
    Float_t *dPhiRM = new Float_t[nNbCorrObjs];
    for (Int_t i=0;i<nNbCorrObjs;i++) 
    {
	tRichMdcInd[i] =  tRichTofInd[i] =  tRichTofinoInd[i] =
	    tRichShowerInd[i] = tToftofAccep[i] =  tToftofRej[i]  = -2.;
	dThetaRM[i] =  dPhiRM[i] = -100;
    }

    // ***************************************************************
    // ***** loop over HHitMatch *************************************
    // ***************************************************************
    pIterMatchHit->Reset();
    Int_t m =0;
    while(( pHM= (HHitMatch *)pIterMatchHit->Next()))
    {
	
	Int_t nRInd = pHM->getRichInd();
	Int_t nMInd = pHM->getMdcInd();
	Int_t nTInd = pHM->getTofInd();
	if(nTInd==-1) nTInd =pHM->getShowInd();
	Int_t nSInd = pHM->getShowInd();
	//cout<<"rich Ind "<<nRInd<<" mdc INd "<<nMInd<<" TOf index "<<
	//nTInd<<" Shower index "<<nSInd<<endl;
	if(nRInd>-1 && nMInd>-1&&isGoodRing(pHM)){
	    Bool_t kRichMdc = HRichCut::isNew2Tuple(nRInd,nMInd,tRichMdcInd,nNbCorrObjs);
	    //  if(kRichMdc) cout<<" is RM TRUE"<<endl;
	    //else cout<<" is RM FALSE"<<endl;
	    Float_t dPrm=(pHM->getRichPhi()-pHM->getMdcPhi())*
		(TMath::Sin(pHM->getMdcTheta()/57.3));
	    Float_t dTrm = (pHM->getRichTheta()-pHM->getMdcTheta());
		
	    if(kRichMdc){
		pH_RichMdcdPhiTheta->Fill(pHM->getMdcTheta(),dPrm,1);
		pH_RichMdcdThetadPhi->Fill(dTrm,dPrm,1);
		pH_RichMdcdTheta->Fill(dTrm);
		pH_RichMdcdPhi->Fill(dPrm);

		if(TMath::Abs(dPrm)<fPhiCut){
		    pH_RichMdcdThetaPhiCut->Fill(dTrm);
		}	
		m++;
	    }
	    else{
		
		tToftofRej[m] =  pHM->getTofTof();
		if( tToftofRej[m]==-1) tToftofRej[m] =  pHM->getTofinoTof();
		//cout<<" double tof for one rm "<<tToftofRej[m]<<" at " <<m<<endl;
		m++;
	    }
	    if(nTInd>-1){
		Bool_t kRichTof = HRichCut::isNew2Tuple(nRInd,nTInd,tRichTofInd,nNbCorrObjs);
		if(kRichMdc&&kRichTof){
		    tToftofAccep[m-1] =  pHM->getTofTof();
		    if(tToftofAccep[m-1]==-1) tToftofAccep[m-1] =  
						pHM->getTofinoTof();
		    //cout<<" one tof for one rm "<<tToftofAccep[m-1]<<" at " <<m-1<<endl;
		    //cout<<" theta "<<pHM->getRichTheta()<<" cos Theta "<<
		    //TMath::Cos(pHM->getRichTheta()/(57.3))<<endl;
		    dThetaRM[m-1] = dTrm;
		    dPhiRM[m-1] = dPrm;
		    // for each RMT correlation the following histos 
		    // are filled only once
		    if(isTOFBetween(tToftofAccep[m-1],5,9)){
			pH_RichMdcdThetaTofCorr->Fill(dThetaRM[m-1]);
			pH_RichMdcdPhiTofCorr->Fill(dPhiRM[m-1]);
		    }
		}
	    }    
	}
	
    }

    if(m>-1){
    for(int j= 0 ;j<nNbCorrObjs;j++){
	for(int i= j ;i<nNbCorrObjs;i++){
	    
	    if(tRichMdcInd[j]==tRichMdcInd[i]&&tToftofRej[i]>0
	       &&tToftofAccep[j]>0){
		if(isTOFBetween(tToftofRej[i],5,9)&&
		   isTOFBetween(tToftofAccep[j],5,9)){
		    //    cout<<" tof Accep "<<tToftofAccep[j]<<" tof Rej "<<tToftofRej[i]<<
		    //" RM ind Accep "<<tRichMdcInd[j]<<" RM ind Rej "<<
		    //tRichMdcInd[i]<<endl;
		    //cout<< " j "<<j<<"  i "<<i<<endl;
	    //for each RM correlation that matches with 2 Tof hits that
	    // have both a "lepton like " t.o.f, the following histos are 
	    // filled. These hits should correspond to the close lepton
	    //pairs that are unresolved in Rich-Mdc and open up( because
	    // of MS) in the tof
		    //		    cout<<" theta Phi "<<dThetaRM[j]<<"  "<<dPhiRM[j]<<endl;
		    pH_RichMdcdThetaTof2Hit->Fill(dThetaRM[j]);
		    pH_RichMdcdPhiTof2Hit->Fill(dPhiRM[j]);
		}
		else {
		    if(isTOFBetween(tToftofRej[i],5,9)||
		       isTOFBetween(tToftofAccep[j],5,9)){
			// for each RM that matches with 2 tof hits, one 
			// entry is stored if one of the 2 hits has a
			// "lepton like" t.o.f.
			//cout<<" 2 ************tof Accep "<<tToftofAccep[j]<<" tof Rej "<<tToftofRej[i]<<
			//	" RM ind Accep "<<tRichMdcInd[j]<<" RM ind Rej "<<
			//tRichMdcInd[i]<<endl;
			//cout<< " j "<<j<<"  i "<<i<<endl;
			pH_RichMdcdThetaTofCorr->Fill(dThetaRM[j]);
			pH_RichMdcdPhiTofCorr->Fill(dPhiRM[j]);
		    }
		 
		}
		tToftofAccep[j]= -2;    
	    }
	}
    }
    for(int j= 0 ;j<nNbCorrObjs;j++){
	//cout<<" 2 ************tof Accep "<<tToftofAccep[j]<<" at  "<<
	//  j<< " RM ind Accep "<<tRichMdcInd[j]<<endl;
	if(isTOFBetween(tToftofAccep[j],5,9)){
	    pH_RichMdcdThetaTof1Hit->Fill(dThetaRM[j]);
	    pH_RichMdcdPhiTof1Hit->Fill(dPhiRM[j]);
	    if((TMath::Abs(dPhiRM[j]))<fPhiCut){
		pH_RichMdcdThetaTof1HitPhiCut->Fill(dThetaRM[j]);
	    }
	}
    }
    }

    delete []  tRichInd;
    delete []  tRichMdcInd;
    delete []  tRichTofInd;
    delete []  tRichTofinoInd;
    delete []  tRichShowerInd;
    delete []  tToftofAccep;
    delete []  tToftofRej;
    delete []  dThetaRM;
    delete []  dPhiRM;
    return 0;
}
Bool_t HCreateCorrPar::isTOFBetween(Float_t tof,Float_t low,Float_t high) {

    if (tof>-1 && tof>=low && tof<=high) return kTRUE;
    else return kFALSE;

}
void HCreateCorrPar::fitGausHistos(TH1F* histo,TH1F *histoFit){
    //TString *name = (TString*)histo->GetName();
    //    name+="FitFc";
    const char* name = histo->GetName();
    char file1[40] ;
    strcpy(file1,name);
    strcat(file1,"_left");
    char file2[40] ;
    strcpy(file2,name);
    strcat(file2,"_right");
    Float_t back1LowLim = -10.;
    Float_t back1UpLim = -4;
    Float_t xMin = -20.;
    Float_t xMax = 20.;
   

    TF1 *f1 = new TF1("f1","gaus",back1LowLim,back1UpLim);
    Double_t par[18];
    histo->Fit("f1","R0");
    f1->GetParameters(&par[0]);

    TF1 *f2 = new TF1("f2","gaus",-back1UpLim,-back1LowLim);
    histo->Fit("f2","R0");
    f2->GetParameters(&par[3]);
    
    TF1 *f5 = new TF1(file1,"gaus",xMin,0);
    par[6] = par[0];
    par[7] = par[1];
    par[8] = par[2];
  
    f5->SetParameter(0,par[6]);
    f5->SetParameter(1,par[7]);
    f5->SetParameter(2,par[8]);
   
    f5->SetLineColor(4);

    TF1 *f10 = new TF1(file2,"gaus",0,xMax);
    par[9] = par[3];
    par[10] = par[4];
    par[11] = par[5];

    f10->SetParameter(0,par[9]);
    f10->SetParameter(1,par[10]);
    f10->SetParameter(2,par[11]);
    f10->SetLineColor(2);//RED


    pHistArray->Add(f5);
    pHistArray->Add(f10);

    // histoFit = (TH1F*)histo->Clone();
    histoFit->Reset();
    for (Int_t bin=1;bin<=bin1;bin++) {
	Float_t x = histo->GetBinCenter(bin);
	Double_t fval = f5->Eval(x);
	Double_t fval2 = f10->Eval(x);
	Double_t diff =0;
	if(x<=0) diff = (histo->GetBinContent(bin) - fval);
	else  diff = (histo->GetBinContent(bin) - fval2);
	if(diff<0) diff =0;
	histoFit->Fill(x,diff);
	
    }

   
}
void HCreateCorrPar::fitLinHistos(TH1F* histo,TH1F *histoFit){
    //TString *name = (TString*)histo->GetName();
    //    name+="FitFc";
    const char* name = histo->GetName();
    char file1[40] ;
    strcpy(file1,name);
    strcat(file1,"_left");
    char file2[40] ;
    strcpy(file2,name);
    strcat(file2,"_right");
    Float_t back1LowLim = -10.;
    Float_t back1UpLim = -4.;
    Float_t xMin = -20.;
    Float_t xMax = 20.;
   

    TF1 *f1 = new TF1("f1","[0]*x+[1]",back1LowLim,back1UpLim);
    Double_t par[18];
    histo->Fit("f1","R0");
    f1->GetParameters(&par[0]);

    TF1 *f2 = new TF1("f2","[0]*x+[1]",-back1UpLim,-back1LowLim);
    histo->Fit("f2","R0");
    f2->GetParameters(&par[2]);
    
    TF1 *f5 = new TF1(file1,"[0]*x+[1]",xMin,0);
    par[4] = par[0];
    par[5] = par[1];
  
    f5->SetParameter(0,par[4]);
    f5->SetParameter(1,par[5]);
   
    f5->SetLineColor(4);

    TF1 *f10 = new TF1(file2,"[0]*x+[1]",0,xMax);
    par[6] = par[2];
    par[7] = par[3];
   
    f10->SetParameter(0,par[6]);
    f10->SetParameter(1,par[7]);
    f10->SetLineColor(2);//RED


    pHistArray->Add(f5);
    pHistArray->Add(f10);

    // histoFit = (TH1F*)histo->Clone();
    histoFit->Reset();
    for (Int_t bin=1;bin<=bin1;bin++) {
	Float_t x = histo->GetBinCenter(bin);
	Double_t fval = f5->Eval(x);
	Double_t fval2 = f10->Eval(x);
	Double_t diff =0;
	if(x<=0) diff = (histo->GetBinContent(bin) - fval);
	else  diff = (histo->GetBinContent(bin) - fval2);
	if(diff<0) diff =0;
	histoFit->Fill(x,diff);
	
    }

   
}
void HCreateCorrPar::iniHistos(){

    pHistArray = new TObjArray(30);// all the histos are put in this array in order to be able later on to write out all the histos at the same time.

    //theta histos
    pH_RichMdcdThetadPhi = new TH2F("pH_RichMdcdThetadPhi","pH_RichMdcdThetadPhi",bin1,-20,20,bin1,-20,20);
   
    pH_RichMdcdPhiTheta = new TH2F("pH_RichMdcdPhiTheta","pH_RichMdcdPhiTheta",bin1,0,90,bin1,-20,20);
    
    pH_RichMdcdTheta = new TH1F("pH_RichMdcdTheta","pH_RichMdcdTheta",bin1,-20,20);
    pHistArray->Add(pH_RichMdcdTheta);

    pH_RichMdcdThetaPhiCut = new TH1F("pH_RichMdcdThetaPhiCut","pH_RichMdcdThetaPhiCut",bin1,-20,20);
    pHistArray->Add(pH_RichMdcdThetaPhiCut);

    pH_RichMdcdThetaTofCorr = new TH1F("pH_RichMdcdThetaTofCorr","pH_RichMdcdThetaTofCorr",bin1,-20,20);
    pHistArray->Add(pH_RichMdcdThetaTofCorr);
 
    pH_RichMdcdThetaTof1Hit = new TH1F("pH_RichMdcdThetaTof1Hit","pH_RichMdcdThetaTof1Hit",bin1,-20,20);
    pHistArray->Add(pH_RichMdcdThetaTof1Hit);
    
    pH_RichMdcdThetaTof1HitPhiCut = new TH1F("pH_RichMdcdThetaTof1HitPhiCut","pH_RichMdcdThetaTof1HitPhiCut",bin1,-20,20);
    pHistArray->Add(pH_RichMdcdThetaTof1HitPhiCut);

    pH_RichMdcdThetaTof2Hit = new TH1F("pH_RichMdcdThetaTof2Hit","pH_RichMdcdThetaTof2Hit",bin1,-20,20);
    pHistArray->Add(pH_RichMdcdThetaTof2Hit);

    /// phi histos

    pH_RichMdcdPhi = new TH1F("pH_RichMdcdPhi","pH_RichMdcdPhi",bin1,-20,20);
    pHistArray->Add(pH_RichMdcdPhi);

    pH_RichMdcdPhiTofCorr = new TH1F("pH_RichMdcdPhiTofCorr","pH_RichMdcdPhiTofCorr",bin1,-20,20);
    pHistArray->Add(pH_RichMdcdPhiTofCorr);
 
    pH_RichMdcdPhiTof1Hit = new TH1F("pH_RichMdcdPhiTof1Hit","pH_RichMdcdPhiTof1Hit",bin1,-20,20);
    pHistArray->Add(pH_RichMdcdPhiTof1Hit);

   
    pH_RichMdcdPhiTof2Hit= new TH1F("pH_RichMdcdPhiTof2Hit","pH_RichMdcdPhiTof2Hit",bin1,-20,20);
    pHistArray->Add(pH_RichMdcdPhiTof2Hit);

    // theta fit histos

    pH_RichMdcdThetaFit = new TH1F("pH_RichMdcdThetaFit","pH_RichMdcdThetaFit",bin1,-20,20);
    pHistArray->Add(pH_RichMdcdThetaFit);

  pH_RichMdcdThetaPhiCutFit = new TH1F("pH_RichMdcdThetaPhiCutFit","pH_RichMdcdThetaPhiCutFit",bin1,-20,20);
    pHistArray->Add(pH_RichMdcdThetaPhiCutFit);

    pH_RichMdcdThetaTofCorrFit = new TH1F("pH_RichMdcdThetaTofCorrFit","pH_RichMdcdThetaTofCorrFit",bin1,-20,20);
    pHistArray->Add(pH_RichMdcdThetaTofCorrFit);
 
    pH_RichMdcdThetaTof1HitFit = new TH1F("pH_RichMdcdThetaTof1HitFit","pH_RichMdcdThetaTof1HitFit",bin1,-20,20);
    pHistArray->Add(pH_RichMdcdThetaTof1HitFit);

    pH_RichMdcdThetaTof1HitPhiCutFit = new TH1F("pH_RichMdcdThetaTof1HitPhiCutFit","pH_RichMdcdThetaTof1HitPhiCutFit",bin1,-20,20);
    pHistArray->Add(pH_RichMdcdThetaTof1HitPhiCutFit);

    pH_RichMdcdThetaTof2HitFit = new TH1F("pH_RichMdcdThetaTof2HitFit","pH_RichMdcdThetaTof2HitFit",bin1,-20,20);
    pHistArray->Add(pH_RichMdcdThetaTof2HitFit);

    //phi fit histos

 pH_RichMdcdPhiFit = new TH1F("pH_RichMdcdPhiFit","pH_RichMdcdPhiFit",bin1,-20,20);
    pHistArray->Add(pH_RichMdcdPhiFit);

    pH_RichMdcdPhiTofCorrFit = new TH1F("pH_RichMdcdPhiTofCorrFit","pH_RichMdcdPhiTofCorrFit",bin1,-20,20);
    pHistArray->Add(pH_RichMdcdPhiTofCorrFit);
 
    pH_RichMdcdPhiTof1HitFit = new TH1F("pH_RichMdcdPhiTof1HitFit","pH_RichMdcdPhiTof1HitFit",bin1,-20,20);
    pHistArray->Add(pH_RichMdcdPhiTof1HitFit);

   
    pH_RichMdcdPhiTof2HitFit= new TH1F("pH_RichMdcdPhiTof2HitFit","pH_RichMdcdPhiTof2HitFit",bin1,-20,20);
    pHistArray->Add(pH_RichMdcdPhiTof2HitFit);
    
}

Bool_t HCreateCorrPar::finalize() {
    pFileOut->cd();
     for (Int_t i=0;i<10;i++)
    {
	fitGausHistos(( (TH1F*)(*pHistArray)[i] ),( (TH1F*)(*pHistArray)[i+10] ));
	//fitLinHistos(( (TH1F*)(*pHistArray)[i] ),( (TH1F*)(*pHistArray)[i+10] ));
    }
     pHistArray->Add(pH_RichMdcdThetadPhi);
     pHistArray->Add(pH_RichMdcdPhiTheta);
    HRichUtilFunc::saveHistos(pFileOut,pHistArray);

    return kTRUE;
}
