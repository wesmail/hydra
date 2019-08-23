
// File: hthreseval.cc
//
// Author: Laura Fabbietti <L.Fabbietti@physik.tu-muenchen.de>
// Last update by Laura Fabbietti: 03/02/05 15:11:01
//
using namespace std;
#include "hthreseval.h"
#include "hevent.h"
#include "hruntimedb.h"
#include "hrichhit.h"
#include "hmdcseg.h"
#include "hkicktrack.h"
#include "hades.h"
#include "hhitmatch.h"
#include "hhitmatchsim.h"
#include "hrichutilfunc.h"
#include <fstream>
#include <TMath.h>
#include <TString.h>
#include <TF1.h>
#include "hspectrometer.h"
#include "hdetector.h"
#include "hrichdetector.h"
#include "richdef.h"
#include "hiterator.h"
#include "hrichgeometrypar.h"
#include "hrichcal.h"
#include "hrichanalysis.h"
#include "hlocation.h"
#include "hmatrixcategory.h"
ClassImp(HThreseval);
HThreseval::HThreseval(Text_t *name,Text_t *title,Option_t *opt,Option_t *opt1):
HReconstructor(name,title)
{
    isLepton = opt1;
    whichData =opt;
    maxPadCharge = 0.;

    pHistArray = new TObjArray(30);
    thetaDiffPM =  new (TH1F**)[50];
    thetaDiffHT =  new (TH1F**)[50];
    for (int i =0; i< 50 ; i++){
        thetaDiffPM[i] = new (TH1F*)[8];
	thetaDiffHT[i] = new (TH1F*)[8];
    }
    Int_t histoBin = 100;
    Float_t xMin = -45.;
    Float_t xMax = 45.;
    char hname0[40];
    char hname1[40];
    char hname2[40];
    char hname3[40];
    char hname4[40];
    char hname5[40];
    char hname6[40];
    char hname7[40];
    //cout<<"before loop "<<endl;
    for (int i =0; i< 50; i++){
	sprintf(hname0,"thetaDiffPM_0_GT_%d",i*20);
	sprintf(hname1,"thetaDiffPM_1_GT_%d",i*20);
	sprintf(hname2,"thetaDiffPM_2_GT_%d",i*20);
	sprintf(hname3,"thetaDiffPM_3_GT_%d",i*20);
	sprintf(hname4,"thetaDiffPM_4_GT_%d",i*20);
	sprintf(hname5,"thetaDiffPM_5_GT_%d",i*20);
	sprintf(hname6,"thetaDiffPM_6_GT_%d",i*20);
	sprintf(hname7,"thetaDiffPM_7_GT_%d",i*20);

	thetaDiffPM[i][0]=new TH1F(hname0,hname0,histoBin,xMin,xMax);
	thetaDiffPM[i][1]=new TH1F(hname1,hname1,histoBin,xMin,xMax);
	thetaDiffPM[i][2]=new TH1F(hname2,hname2,histoBin,xMin,xMax);
	thetaDiffPM[i][3]=new TH1F(hname3,hname3,histoBin,xMin,xMax);
	thetaDiffPM[i][4]=new TH1F(hname4,hname4,histoBin,xMin,xMax);
	thetaDiffPM[i][5]=new TH1F(hname5,hname5,histoBin,xMin,xMax);
	thetaDiffPM[i][6]=new TH1F(hname6,hname6,histoBin,xMin,xMax);
	thetaDiffPM[i][7]=new TH1F(hname7,hname7,histoBin,xMin,xMax);
	pHistArray->Add(thetaDiffPM[i][0]);

    }
    for (int i =0; i< 11; i++){
	sprintf(hname0,"thetaDiffHT_0_GT_%d",i*10);
	sprintf(hname1,"thetaDiffHT_1_GT_%d",i*10);
	sprintf(hname2,"thetaDiffHT_3_GT_%d",i*10);
	sprintf(hname3,"thetaDiffHT_4_GT_%d",i*10);
	sprintf(hname4,"thetaDiffHT_5_GT_%d",i*10);
	sprintf(hname5,"thetaDiffHT_6_GT_%d",i*10);
	sprintf(hname6,"thetaDiffHT_7_GT_%d",i*10);
	sprintf(hname7,"thetaDiffHT_8_GT_%d",i*10);

	thetaDiffHT[i][0]=new TH1F(hname0,hname0,histoBin,xMin,xMax);
	thetaDiffHT[i][1]=new TH1F(hname1,hname1,histoBin,xMin,xMax);
	thetaDiffHT[i][2]=new TH1F(hname2,hname2,histoBin,xMin,xMax);
	thetaDiffHT[i][3]=new TH1F(hname3,hname3,histoBin,xMin,xMax);
	thetaDiffHT[i][4]=new TH1F(hname4,hname4,histoBin,xMin,xMax);
	thetaDiffHT[i][5]=new TH1F(hname5,hname5,histoBin,xMin,xMax);
	thetaDiffHT[i][6]=new TH1F(hname6,hname6,histoBin,xMin,xMax);
	thetaDiffHT[i][7]=new TH1F(hname7,hname7,histoBin,xMin,xMax);
	pHistArray->Add(thetaDiffHT[i][0]);
    }
    for (int i =11; i< 50; i++){
	sprintf(hname0,"thetaDiffHT_0_GT_%d",i*20);
	sprintf(hname1,"thetaDiffHT_1_GT_%d",i*20);
	sprintf(hname2,"thetaDiffHT_3_GT_%d",i*20);
	sprintf(hname3,"thetaDiffHT_4_GT_%d",i*20);
	sprintf(hname4,"thetaDiffHT_5_GT_%d",i*20);
	sprintf(hname5,"thetaDiffHT_6_GT_%d",i*20);
	sprintf(hname6,"thetaDiffHT_7_GT_%d",i*20);
	sprintf(hname7,"thetaDiffHT_8_GT_%d",i*20);

	thetaDiffHT[i][0]=new TH1F(hname0,hname0,histoBin,xMin,xMax);
	thetaDiffHT[i][1]=new TH1F(hname1,hname1,histoBin,xMin,xMax);
	thetaDiffHT[i][2]=new TH1F(hname2,hname2,histoBin,xMin,xMax);
	thetaDiffHT[i][3]=new TH1F(hname3,hname3,histoBin,xMin,xMax);
	thetaDiffHT[i][4]=new TH1F(hname4,hname4,histoBin,xMin,xMax);
	thetaDiffHT[i][5]=new TH1F(hname5,hname5,histoBin,xMin,xMax);
	thetaDiffHT[i][6]=new TH1F(hname6,hname6,histoBin,xMin,xMax);
	thetaDiffHT[i][7]=new TH1F(hname7,hname7,histoBin,xMin,xMax);
	pHistArray->Add(thetaDiffHT[i][0]);
    }
 
    effPM = new TH2F("effPM","effPM",500,0,1000,1000,0,120000);
    pHistArray->Add(effPM);
    effHT = new TH2F("effHT","effHT",500,0,1000,1000,0,120000);
    pHistArray->Add(effHT); 
    // here come all the efficiency histogramms for the diffrent cuts
    effPM1 = new TH2F("effPMDens","effPMDens",500,0,1000,1000,0,120000);
    pHistArray->Add(effPM1);
    effHT1 = new TH2F("effHTDens","effHTDens",500,0,1000,1000,0,120000);
    pHistArray->Add(effHT1);

    effPM2 = new TH2F("effPMBoard","effPMDensBoard",500,0,1000,1000,0,120000);
    pHistArray->Add(effPM2);
    effHT2 = new TH2F("effHTBoard","effHTBoard",500,0,1000,1000,0,120000);
    pHistArray->Add(effHT2);

    effPM3 = new TH2F("effPMDyn","effPMDyn",500,0,1000,1000,0,120000);
    pHistArray->Add(effPM3);
    effHT3 = new TH2F("effHTDyn","effHTDyn",500,0,1000,1000,0,120000);
    pHistArray->Add(effHT3); 

    effPM4 = new TH2F("effPMRatio","effPMRatio",500,0,1000,1000,0,120000);
    pHistArray->Add(effPM4);
    effHT4 = new TH2F("effHTRatio","effHTRatio",500,0,1000,1000,0,120000);
    pHistArray->Add(effHT4); 

    effPM5 = new TH2F("effPMAssym","effPMAssym",500,0,1000,1000,0,120000);
    pHistArray->Add(effPM5);
    effHT5 = new TH2F("effHTAssym","effHTAssym",500,0,1000,1000,0,120000);
    pHistArray->Add(effHT5); 

    effPM6 = new TH2F("effPMCharge","effPMCharge",500,0,1000,1000,0,120000);
    pHistArray->Add(effPM6);
    effHT6 = new TH2F("effHTCharge","effHTCharge",500,0,1000,1000,0,120000);
    pHistArray->Add(effHT6); 

    effPM7 = new TH2F("effPMClose","effPMClose",500,0,1000,1000,0,120000);
    pHistArray->Add(effPM7);
    effHT7 = new TH2F("effHTClose","effHTClose",500,0,1000,1000,0,120000);
    pHistArray->Add(effHT7); 

    hTestBorder =new TH2F("fracVSQual","fracVSQual",500,0,1000,100,0,1);
    hTestBorderCorr=new TH2F("fracVSQualCorr","fracVSQualCorr",500,0,1000,100,0,1);
    pHistArray->Add(hTestBorder);
    pHistArray->Add(hTestBorderCorr);

    hTestRatio =new TH1F("inOutDivAll","inOutDivAll",100,0,1);
    hTestRatioCorr =new TH1F("inOutDivAllCorr","inOutDivAllCorr",100,0,1);
    pHistArray->Add(hTestRatio);
    pHistArray->Add(hTestRatioCorr);

    hTestRadius = new TH1F("radius","radius",20,0,10);
    hTestRadiusCorr = new TH1F("radiusCorr","radiusCorr",20,0,10);
    pHistArray->Add(hTestRadius);
    pHistArray->Add(hTestRadiusCorr);

    hTestCentroid = new TH1F("centroid","centroid",50,0,10);
    hTestCentroidCorr = new TH1F("centroidCorr","centroidCorr",50,0,10);
    pHistArray->Add(hTestCentroid);
    pHistArray->Add(hTestCentroidCorr);

    hTestCharge = new TH1F("charge","charge",200,0,200);
    hTestChargeCorr = new TH1F("chargeCorr","chargeCorr",200,0,200);
    pHistArray->Add(hTestCharge);
    pHistArray->Add(hTestChargeCorr);

    hTestClose =new TH2F("dQVScentroid","dQVScentroid",50,0,10,50,0,1);
    hTestCloseCorr =new TH2F("dQVScentroidCorr","dQVScentroidCorr",50,0,10,50,0,1);
    pHistArray->Add(hTestClose);
    pHistArray->Add(hTestCloseCorr);

    pMhT = new TH2F("pMhT","pMhT",500,0,1000,500,0,1000);
    pHistArray->Add(pMhT);
   
    isLepton.ToLower();
    whichData.ToLower();
    TString s ="simul";
    TString l ="lepton";
    TString l1 ="fake";
    if(whichData.CompareTo(s)==0){
	if(isLepton.CompareTo(l)==0){
	    out = new TFile("thresholdLepSim.root","recreate");
	}
	else if(isLepton.CompareTo(l1)==0){
	    out = new TFile("thresholdFakeSim.root","recreate");
	}
	else{
	    out = new TFile("thresholdSim.root","recreate");
	}
    }
    else{
	out = new TFile("thresholdExp.root","recreate");
    }

    allRingPar = new TNtuple("allRingPar","allRingPar","padNr:pmQual:avCharge:radius:centroid:dThetaMdc:dPhiMdc:localMax:deltaQual:fracInSec:totalCharge:maxCharge");
;
    corrRingPar= new TNtuple("corrRingPar","corrRingPar","padNr:pmQual:avCharge:radius:centroid:dThetaMdc:dPhiMdc:localMax:deltaQual:fracInSec:totalCharge:maxCharge");;
   
}
HThreseval::HThreseval(){
}
HThreseval::~HThreseval(){
    delete out;
    for (int i =0; i< 50; i++){
	for (int j =0; j< 8; j++){
	    delete thetaDiffPM[i][j];
	    delete thetaDiffHT[i][j];
	}
    }
    for (int i =0; i< 8; i++){
	delete thetaDiffPM[i];
	delete thetaDiffHT[i];
 }
    delete thetaDiffPM;
    delete thetaDiffHT;
    delete effPM;
    delete effHT;

}
Bool_t HThreseval::init(){
TString s ="simul";
if (gHades) {
	HEvent *event=gHades->getCurrentEvent();
	HRuntimeDb *rtdb=gHades->getRuntimeDb();
	HSpectrometer *spec=gHades->getSetup();
	HDetector *rich = spec->getDetector("Rich");
	if (event && rtdb) {
	    // Setup output category
	    if(whichData.CompareTo(s)==0){
		pRichCalCat=event->getCategory(catRichCal);
		if (!pRichCalCat) {
		    pRichCalCat=rich->buildCategory(catRichCal);
		    
		    if (!pRichCalCat) return kFALSE;
		    else gHades->getCurrentEvent()
			     ->addCategory(catRichCal, pRichCalCat, "Rich");
		}
		pIterRichCal = (HIterator*)pRichCalCat->MakeIterator("native");
		//cout<<" cal category "<<  pRichCalCat<<endl;
		pRichHitCat=event->getCategory(catRichHit);
		if (!pRichHitCat) {
		    pRichHitCat=rich->buildCategory(catRichHit);
		    
		    if (!pRichHitCat) return kFALSE;
		    else gHades->getCurrentEvent()
			     ->addCategory(catRichHit, pRichHitCat, "Rich");
		}
		pIterRichHit1 = (HIterator*)pRichHitCat->MakeIterator("native");
		pIterRichHit2 = (HIterator*)pRichHitCat->MakeIterator("native");
	    }	
	    pHitMatchCat=event->getCategory(catMatchHit);
	    if (!pHitMatchCat) {
		pHitMatchCat=rich->buildCategory(catMatchHit);
		
		if (!pHitMatchCat) {
		    Error("init","No HIT MATCH category defined");
		    return kFALSE;
		}
		else event->addCategory(catMatchHit, pHitMatchCat, "Rich");
	    }
	    
	    pIterMatchHit = (HIterator*)getHitMatchCat()->MakeIterator("native");
	     pIterMatchHit1 = (HIterator*)getHitMatchCat()->MakeIterator("native");
	   
	    HRichGeometryPar *pGeomPar = (HRichGeometryPar*)rtdb->
                                  getContainer("RichGeometryParameters");
	    
	    setGeomPar(pGeomPar);
	    if (pGeomPar == NULL) return kFALSE;

	    HRichAnalysisPar *pAnalysisPar = (HRichAnalysisPar*)rtdb->
		getContainer("RichAnalysisParameters");
	    setAnalysisPar(pAnalysisPar);
	    if (pAnalysisPar == NULL) return kFALSE;
	}
 }
 return kTRUE;
}
Bool_t HThreseval::reinit() {
    Int_t maxRows = 96;
    Int_t maxCols = 96;
    
    iPadActive.Set(maxCols*maxRows);
    for (int i=0 ; i<maxCols*maxRows; i++)
	if (((HRichGeometryPar*)fpGeomPar)->getPadsPar()->getPad(i)->getPadActive() >0)
        iPadActive[i] = 1; else iPadActive[i] = 0;
    

    Int_t iMatrixSurface=0, iPartSurface=0;
    Int_t iMaskSize = ((HRichAnalysisPar*)fpAnalysisPar)->iRingMaskSize;
    Int_t iMaskSizeSquared = iMaskSize*iMaskSize;
    Int_t k,j,i,m,n;
    
    HRichAnalysis ana;
    for (k = 0; k < iMaskSizeSquared; k++)
	if (((HRichAnalysisPar*)fpAnalysisPar)->iRingMask[k] == 1) iMatrixSurface++;
    for (j = 0; j < maxRows; j++)
	for (i = 0; i < maxCols; i++)   {
	    iPartSurface = 0;
	    for (k = 0; k < iMaskSizeSquared; k++) {
		m = (k % iMaskSize) - iMaskSize/2;
		n = (k / iMaskSize) - iMaskSize/2;
		if (!IsOut(i,j,m,n))
		    if (((HRichAnalysisPar*)fpAnalysisPar)->iRingMask[k] == 1) iPartSurface++;
	    }
	    ((HRichGeometryPar*)fpGeomPar)->getPadsPar()->
		getPad(i,j)->setAmplitFraction((Float_t)iPartSurface/(Float_t)iMatrixSurface);
	}
    
    
    return kTRUE;
}
Int_t HThreseval::IsOut(Int_t x, Int_t y, Int_t dx, Int_t dy) {
   
 if ((x+dx) >= 0 && (x+dx) < 96 &&
     (y+dy) >= 0 && (y+dy) < 96 &&
     (x+dx + 96*(y+dy)) <96*96  &&
     (x+dx + 96*(y+dy))>0){
     if(iPadActive[x+dx + 96*(y+dy)]) {
	 return 0; 
     }
     else {
	 return 1; 
     }
 }
 else return 1;
}
Int_t HThreseval::execute(){
    // cout<<" new event "<<endl;
    pIterMatchHit->Reset();
   
    HHitMatchSim * pMatch = NULL;
    HHitMatchSim * pMatch1 = NULL;
    Float_t ringPM = 0;
    Float_t ringHT = 0;
    Float_t dThetaRM = 0;
    Float_t dPhiRM = 0;
    Float_t dPhiRM1 = 0;
    TString s = "0";
    for(Int_t j =0;j<100;j++){
	richIndArray[j] = 0;
    }
    countHitMatchObj = 0;
    HRichHit *pHit = NULL;
    TString l ="lepton";
    TString l1 ="fake";
    TString l2 ="";
    
    while((pMatch= (HHitMatchSim *)pIterMatchHit->Next())){
	dPhiRM = (pMatch->getRichPhi() - pMatch->getMdcPhi())*
	    TMath::Sin(pMatch->getMdcTheta()/57.3);
	pIterMatchHit1->Reset();
	while((pMatch1= (HHitMatchSim *)pIterMatchHit1->Next())){
	    dPhiRM1 = (pMatch1->getRichPhi() - pMatch1->getMdcPhi())*
		TMath::Sin(pMatch1->getMdcTheta()/57.3);
	    //  cout<<" rich ind 1 :  "<<pMatch->getRichInd()<<" rich ind 2 :  "<<
	    //	pMatch1->getRichInd()<<" dPhi 1 :  "<<dPhiRM<<" dPhi 2 :  "<<
	    //	dPhiRM1<<endl;
	    if((pMatch->getRichInd()==pMatch1->getRichInd()) &&
	       (TMath::Abs(dPhiRM))<(TMath::Abs(dPhiRM1))) {
		pMatch1 ->setMatchedRichMdc(0);
		//	break;
	    }
	}
    }
    pIterMatchHit->Reset();
    while((pMatch= (HHitMatchSim *)pIterMatchHit->Next())){
	if(pMatch->getMatchedRichMdc()&& !isRingInArray(pMatch)){
	    if((isLepton.CompareTo(l)==0   && (pMatch->getNumPhot1()>0)   &&
		(pMatch->getGLepInMDC()>0) && (pMatch->isGCLepRing()>0)) ||

	       // a lepton is selected if the number of photons on the mirror 
	       // is greater than 0 and if the lepton has reached the mdc.
	       // The first condition is satisfied if the ring candidate
	       // passes the tests done in the function 
	       // HRichCorrelatorSim::isLepOnMirror(HHitMatchSim *h)
	       // Additionally the geant track number of the ring
	       // must be matched with at least another subdetector
	       // hgeant track number(flag isGCLepRing in HHitMatchSim).

	       (isLepton.CompareTo(l1)==0 && (pMatch->getNumPhot1()<0)) ||

	       // A fake is selected if the ring doesnt match any lepton
	       // contained in the HGeantRichMirror container.
	       // The conditions that must be satisfied for the matching
	       // are set in the function 
	       // HRichCorrelatorSim::isLepOnMirror(HHitMatchSim *h).

	       (isLepton.CompareTo(l2)==0)){

		//  cout<<" countHitMatchObj "<<countHitMatchObj<<endl;
		//	cout<<" number of photon "<<pMatch->getNumPhot()<<endl;
		//filling the dTheta dPhi histos for rings correlated
		// in phi with a kicktrack.
		
		richIndArray[countHitMatchObj] = pMatch->getRichInd();
		countHitMatchObj++;
		//	cout<<" ring index "<<pMatch->getRichInd()<<endl;
		ringPM = pMatch->getRingPatMat();
		ringHT = pMatch->getRingHouTra();
		dThetaRM = pMatch->getRichTheta() -  pMatch->getMdcTheta();
		dPhiRM = (pMatch->getRichPhi() - pMatch->getMdcPhi())*
		    TMath::Sin(pMatch->getMdcTheta()/57.3);
		//	cout<<"  dPhiRM  "<<dPhiRM<<endl;
		fillHistoPM(ringPM,dThetaRM,0);
		fillHistoHt(ringHT,dThetaRM,0);
		if(TMath::Abs(dThetaRM)<3) pMhT->Fill(ringPM,ringHT,1);
		
		//filling the dTheta dPhi histos for rings correlated
		// in phi with a kicktrack and fullfilling the tests conditions.
		s = "0";
		Int_t iTest = pMatch->getRingTestFlags();
		// cout<<" intial value "<<pMatch->getRingTestFlags()<<endl;
		char name[10];
		sprintf(name,"%d",iTest);
		s = name;
		
		//  cout<<"fTest  "<<s<<endl;
		// the following condition is necessary since the experimental
		// data dont contain the richcal and richhit container
		if(whichData.CompareTo("simul")==0){
		    if(isClosePairRejectes(s)){
			fillHistoPM(ringPM,dThetaRM,7);
			fillHistoHt(ringHT,dThetaRM,7);
		    }
		    if(isTestChargeTrue(s)){
			fillHistoPM(ringPM,dThetaRM,6);
			fillHistoHt(ringHT,dThetaRM,6);
		    }
		    if(isTestAssymerTrue(s)){
			fillHistoPM(ringPM,dThetaRM,5);
			fillHistoHt(ringHT,dThetaRM,5);
		    }
		    
		    if(isTestRatioTrue(s)){
			fillHistoPM(ringPM,dThetaRM,4);
			fillHistoHt(ringHT,dThetaRM,4);
		    }
		    if(isTestDynamicTrue(s)){
			fillHistoPM(ringPM,dThetaRM,3);
			fillHistoHt(ringHT,dThetaRM,3);
		    }
		    if(isTestBoarderTrue(s)){
			fillHistoPM(ringPM,dThetaRM,2);
			fillHistoHt(ringHT,dThetaRM,2);
		    }
		    if(isTestDensityTrue(s)){
			fillHistoPM(ringPM,dThetaRM,1);
			fillHistoHt(ringHT,dThetaRM,1);
		    }
		    //  	    if(isTestDensityTrue(s)) cout<<"is Dens [6]"<<endl;
		    //  	    if(isTestBoarderTrue(s)) cout<<"is Boarder  [5]"<<endl;
		    //  	    if(isTestDynamicTrue(s)) cout<<"is Dyna [4]"<<endl;
		    //  	    if(isTestRatioTrue(s)) cout<<"is Ratio [3]"<<endl;
		    //  	    if(isTestAssymerTrue(s)) cout<<"is Assym [2]"<<endl;
		    //  	    if(isTestChargeTrue(s))  cout<<"is Charge [1]"<<endl;
		    //  	    if(isClosePairRejectes(s)) cout<<"is Closepairs [0]"<<endl;
		    
		    // all the correlated rings are labeled in the hrichHit container
		    // they get fLabX=5 (otherwise is always 0)
		    
		    pHit = (HRichHit*)((HMatrixCategory*)getRichHitCat())->
			getObject(pMatch->getRichInd());
		    pHit->setLabXYZ(5,dThetaRM,dPhiRM);
		}
		else{

		    //for the experimental data only the entuple is filled

		    corrRingPar->Fill(pMatch->getRingPadNr(),pMatch->getRingPatMat(),pMatch->getRingAmplitude()/pMatch->getRingPadNr(),pMatch->getRadius(),pMatch->getCentroid(),dThetaRM,dPhiRM,pMatch->getRingLocalMax4(),pMatch->getRichTheta(),pMatch->getRichPhi(),pMatch->getRingAmplitude(),maxPadCharge);
		}
	    }
	}
    }
    if(whichData.CompareTo("simul")==0){
	pIterRichHit1->Reset();
	HRichHit *  pHit2= NULL;
	Float_t qual= 0;
	Float_t deltaQual =0;
	Float_t ratio = 0;
	Float_t flagCorr,dP,dT;
	flagCorr = dP = dT = 0;
	Float_t frac = 0;
	//if the option "lepton" is selected in 
	while((pHit2= (HRichHit *)pIterRichHit1->Next())){
	    // if((isLepton.CompareTo(l)==0 && pIterMatchHit->isLepRing())||
	    // 	   (isLepton.CompareTo(l1)==0 && !pIterMatchHit->isLepRing())||
	    // 	   (isLepton.CompareTo(l2)==0)){
	    //cout<<" new hrichhit "<<endl;
	    qual = pHit2->getRingPatMat();
	    //cout<<" qual "<<qual<<endl;
	    frac = testBorder(pHit2);
	    hTestBorder->Fill(qual,frac);
	    ratio = testRatio(pHit2);
	    hTestRatio->Fill(ratio);
	    hTestRadius->Fill(pHit2->getRadius());
	    hTestCentroid->Fill(pHit2->getCentroid());
	    hTestCharge->Fill(pHit2->getRingAmplitude()/pHit2->getRingPadNr());
	    testCloseRej(pHit2,deltaQual);
	    hTestClose->Fill(pHit2->getCentroid(),deltaQual);
	    allRingPar->Fill(pHit2->getRingPadNr(),qual,pHit2->getRingAmplitude()/pHit2->getRingPadNr(),pHit2->getRadius(),pHit2->getCentroid(),0,0,pHit2->getRingLocalMax4(),deltaQual,frac,pHit2->getRingAmplitude(),maxPadCharge);
	    pHit2->getLabXYZ(&flagCorr,&dT,&dP);
	    //cout<<" ring flag "<<flagCorr<<" dT  "<<dT<<" dP "<<dP<<endl; 
	    if(flagCorr>4.99&&flagCorr<5.01){
		corrRingPar->Fill(pHit2->getRingPadNr(),qual,pHit2->getRingAmplitude()/pHit2->getRingPadNr(),pHit2->getRadius(),pHit2->getCentroid(),dT,dP,pHit2->getRingLocalMax4(),deltaQual,frac,pHit2->getRingAmplitude(),maxPadCharge);
		//  cout<<" correlated ring "<<endl;
		hTestBorderCorr->Fill(qual,frac);
		hTestRatioCorr->Fill(ratio);
		hTestRadiusCorr->Fill(pHit2->getRadius());
		hTestCentroidCorr->Fill(pHit2->getCentroid());
		hTestChargeCorr->Fill(pHit2->getRingAmplitude()/pHit2->getRingPadNr());
		hTestCloseCorr->Fill(pHit2->getCentroid(),deltaQual);
	    }
	    //	}
	}
    }
    return 0;
}
Bool_t HThreseval::isTestDensityTrue(TString st){
    Int_t iLength = st.Length();
    //cout<<" length "<< iLength<<endl;
    if (iLength>0){
	//cout<<" Dense values "<<st[iLength-1]<<endl;
	TString st1 = st[iLength-1];
	if(st1.CompareTo("0")==0) return kFALSE;
	else return kTRUE;
    }
    else return kFALSE;
}
Bool_t HThreseval::isTestBoarderTrue(TString st){
    Int_t iLength = st.Length();
    if (iLength>1){
	//cout<<" Board value "<<st[iLength-2]<<endl;
	TString st1 = st[iLength-2];
	if(st1.CompareTo("0")==0) return kFALSE;
	else return kTRUE;
    }
    else return kFALSE;
}
Bool_t HThreseval::isTestDynamicTrue(TString st){
    Int_t iLength = st.Length();
    if (iLength>2){
	//cout<<" Dynamic value "<<st[iLength-3]<<endl;
	TString st1 = st[iLength-3];
	if(st1.CompareTo("0")==0) return kFALSE;
	else return kTRUE;
    }
    else return kFALSE;
}
Bool_t HThreseval::isTestRatioTrue(TString st){
    Int_t iLength = st.Length();
    // cout<<" length "<< iLength<<endl;
    if (iLength>3){
	//cout<<" Ratio value "<<st[iLength-4]<<endl;
	TString st1 = st[iLength-4];
	if(st1.CompareTo("0")==0) return kFALSE;
	else return kTRUE;
    }
    else return kFALSE;
}
Bool_t HThreseval::isTestAssymerTrue(TString st){
    Int_t iLength = st.Length();

    //cout<<" length "<< iLength<<endl;
    if (iLength>4){
	//cout<<" Assymetry value "<<st[iLength-5]<<endl;
	TString st1 = st[iLength-5];
	if(st1.CompareTo("0")==0) return kFALSE;
	else return kTRUE;
    }
    else return kFALSE;
}
Bool_t HThreseval::isTestChargeTrue(TString st){
    Int_t iLength = st.Length();
    if (iLength>5){
	//cout<<" Charge value "<<st[iLength-6]<<endl;
	TString st1 = st[iLength-6];
	if(st1.CompareTo("0")==0) return kFALSE;
	else return kTRUE;
	
    }
    else return kFALSE;
}
Bool_t HThreseval::isClosePairRejectes(TString st){
    Int_t iLength = st.Length();
    if (iLength>6){
	//cout<<" Close value "<<st[iLength-7]<<endl;
	TString st1 = st[iLength-7];
	if(st1.CompareTo("0")==0) return kFALSE;
	else return kTRUE;
	
    }
    else return kFALSE;
}
Bool_t HThreseval::isRingInArray(HHitMatch *  pM){

    for(Int_t j = 0;j<countHitMatchObj;j++){
	//cout<<"pMatch->getRichInd() "<<pM->getRichInd()<<endl;
	//cout<<"richIndArray[j]  "<<richIndArray[j]<<endl;
	if(pM->getRichInd()==richIndArray[j]) return kTRUE;
    }
    return kFALSE;
}
Float_t HThreseval::testBorder(HRichHit *pHit){
    //  cout<<" X "<<pHit->getRingCenterX()<<" Y "<<pHit->getRingCenterY()<<endl;
    Float_t fraction = getGeometryPar()->getPadsPar()->
	getPad(pHit->getRingCenterX(),pHit->getRingCenterY())->getAmplitFraction();
    //    cout<<" fraction "<<fraction<<endl;
    return fraction;
}


Float_t HThreseval::testRatio(HRichHit *pHit){
    //cout<<" ratio +++++++++++++++++++++++++++++++++++++"<<endl;
    //cout<<" nr of pads "<< pHit->getRingPadNr()<<endl;
    //cout<<" center X "<< pHit->iRingX<<" center Y "<<pHit->iRingY<<endl;
    Int_t sector = 	pHit->getSector();
    HRichCal *cal = NULL;
    HRichAnalysis ana;
    HLocation locat;
    Int_t k,m,n;
    Int_t iMatrixSurface;
    Float_t  iOutRing = 0., iOnRing = 0., iInRing = 0., iAllRing = 0.;
    maxPadCharge = 0;
    Float_t chargeMax = 0;
    Float_t chargeTemp = 0;

    iMatrixSurface = getAnalysisParams()->iRingMaskSize * getAnalysisParams()->iRingMaskSize;
    
    for (k = 0; k < iMatrixSurface; k++) {
	m = (k % getAnalysisParams()->iRingMaskSize) - getAnalysisParams()->iRingMaskSize/2;
	n = (k / getAnalysisParams()->iRingMaskSize) - getAnalysisParams()->iRingMaskSize/2;
	
	//cout<<"  col "<<pHit->iRingX+m<<"  row "<<pHit->iRingY+n<<endl;
	
	locat.set(3, sector, pHit->iRingY +n, pHit->iRingX +m);
	
	//cout<<" cal category  "<<(HMatrixCategory*)getRichCalCat()<<endl;
	cal = ((HRichCal*)((HMatrixCategory*)getRichCalCat())->getObject(locat));


	//cout<<" cal object "<<cal<<endl;
	//if(cal) cout<<" cal->getCharge() "<<cal->getCharge()<<endl;
	if(cal) {
	    chargeTemp = cal->getCharge();
	    if(chargeMax<chargeTemp){
		chargeMax = chargeTemp;
	    }
	    //cout<<" x+dx "<<pHit->iRingX+m <<" y+dy  "<< pHit->iRingY + n<<"   iPadActive[x+dx + 92*(y+dy) "<<iPadActive[ pHit->iRingX+m + 96*(pHit->iRingY + n)]<<endl; 
	    //cout<<cal <<"  cal "<<endl;
	    if (!IsOut(pHit->iRingX,pHit->iRingY,m,n) &&
		cal->getCharge() > 0){
		//cout<<" in the loop "<<endl;
		if (getAnalysisParams()->iRingMask[k] == 0) iOutRing++;
		else
		    if (getAnalysisParams()->iRingMask[k] == 1) iOnRing++;
		    else
			if (getAnalysisParams()->iRingMask[k] == 2) iInRing++;
	    }
	}
    }
    maxPadCharge = chargeMax;
    iAllRing = iOutRing + iOnRing + iInRing;
    // cout<<" ALl "<<iAllRing<<" out "<<iOutRing<<"  on "<<iOnRing<<"  in "<<iInRing<<endl;
    //cout<<" iOutRing+iInRing/iAllRing "<<iOutRing+iInRing/iAllRing<<endl;
    return (iOutRing+iInRing/iAllRing);
    
}
void HThreseval::testCloseRej(HRichHit *pHit1,Float_t &dq){
    Float_t maxFakeDistSquared = 4.*4.*4.2;
    
    HRichHit *  pHit2= NULL;
    pIterRichHit2->Reset();
    Float_t dQTemp = 0;
    Float_t dQ = 0;
    while((pHit2= (HRichHit *)pIterRichHit2->Next())){
	Int_t dx = pHit1->getRingCenterX() - pHit2->getRingCenterX();
	Int_t dy = pHit1->getRingCenterY()- pHit2->getRingCenterY();
	Float_t distSquared=dx*dx+dy*dy;
	//cout<<" distSquared "<<distSquared <<" maxFakeDistSquared "<< maxFakeDistSquared<<endl;
	//cout<<" qual1 "<<pHit1->iRingPatMat<<" qual2 "<<pHit2->iRingPatMat<<endl;
	  if (distSquared<maxFakeDistSquared && pHit1->getAddress()!=pHit2->getAddress()) {
	     
	      if (pHit1->iRingPatMat+pHit2->iRingPatMat==0) Error("HRichRingFind::CloseMaxRejection","possible division by zero");
	      // cout<<" division  "<<  pHit2->iRingPatMat+pHit1->iRingPatMat<<endl;  
	    dQ =(Float_t)(pHit2->iRingPatMat-pHit1->iRingPatMat)
	      /(Float_t)(pHit2->iRingPatMat+pHit1->iRingPatMat);
	    //cout <<" dQ "<<dQ <<endl;
	    if(dQTemp>dQ){
		dQ = dQTemp;
	    }
	    else{
	    dQTemp = dQ;
	    }
	    
	  }
    }
    dq= TMath::Abs(dQ);
   
}
void HThreseval::fillHistoPM(Float_t pm,Float_t dt,Int_t histoInd){
    Int_t x1,y1;
    x1 = y1 =0;
    if(pm<1000){
	
	x1 = TMath::Nint(pm/20.);
	//cout<<"Integer 1) rings "<< x1 <<endl; 
	y1 = TMath::Min((Int_t)pm,x1*20);
				
	x1=(x1*20.==y1)?x1:x1-1;
	//  if(pm<150){
//  	    cout<<" quality "<<pm<<endl;
//  	    cout<<"Channel "<<x1<<endl;
//  	}
	//cout<<" channel "<<x1<<endl;
	if(x1<50)
	    for(Int_t j=0;j<x1+1;j++){
		thetaDiffPM[j][histoInd]->Fill(dt);
	    }
    }
}
void HThreseval::fillHistoHt(Float_t ht,Float_t dt,Int_t histoInd){
    Int_t x1,y1;
    x1 = y1 =0;
    if(ht<101){
	x1 = TMath::Nint(ht/10.);
				//  cout<<"Integer 1) rings "<< x1 <<endl; 
	y1 = TMath::Min((Int_t)ht,x1*10);
				//cout<<"Min  "<<y1<<endl;
	x1=(x1*10.==y1)?x1:x1-1;
	for(Int_t j=0;j<x1+1;j++){
	   
	thetaDiffHT[j][histoInd]->Fill(dt);
	}
    }
    else if(ht<1200){
	x1 = TMath::Nint(ht/20.);
	// cout<<"Integer 2) fakes"<< x1 <<endl; 
	y1 = TMath::Min((Int_t)ht,x1*20);
	// cout<<"Min  "<<y1<<endl;
	x1=(x1*20.==y1)?x1:x1-1;
	if(x1<50)
	    for(Int_t j=0;j<x1+1;j++){
		//cout<<" dt "<<dt<<" thetaDiffHT[j]  "<<j<<endl;
		thetaDiffHT[j][histoInd]->Fill(dt);
	    }
    }

}
Float_t HThreseval::fitAndSubtractBG(TH1F* histo){
    Float_t back1LowLim = -40.;
    Float_t back1UpLim = -10.;
    TF1 *f1 = new TF1("f1","gaus",back1LowLim,back1UpLim);
    Double_t par[18];
    histo->Fit("f1","R0");
    f1->GetParameters(&par[0]);
    
    Float_t xMin = -45.;
    TF1 *f5 = new TF1("f5","gaus",xMin,0);
    par[9] = par[0];
    par[10] = par[1];
    par[11] = par[2];
    f5->SetParameter(0,par[9]);
    f5->SetParameter(1,par[10]);
    f5->SetParameter(2,par[11]);
    f5->SetLineColor(4);

    Float_t xMax = 45.;
    TF1 *f10 = new TF1("f10","gaus",0,xMax);
    par[6] = par[0];
    par[7] = -par[1];
    par[8] = par[2];
    f10->SetParameter(0,par[6]);
    f10->SetParameter(1,par[7]);
    f10->SetParameter(2,par[8]);
    f10->SetLineColor(2);//RED
    f10->Draw("same");
    Double_t diff = 0;
    TH1F *htemp = (TH1F*)histo->Clone();
    htemp->Reset();
    for (Int_t bin=1;bin<=histo->GetNbinsX();bin++) {
	Float_t x = histo->GetBinCenter(bin);
	Double_t fval = f5->Eval(x);Double_t fval2 = f10->Eval(x);
	if(x<=0) diff = (histo->GetBinContent(bin) - fval);
	else  diff = (histo->GetBinContent(bin) - fval2);
	if(diff<0) diff =0;
	htemp->Fill(x,diff);
	
    }
    htemp->SetLineColor(4);
     pHistArray->Add(htemp);
    return htemp->Integral(42,53);
  
}
Bool_t HThreseval::finalize(){
    out->cd();
    allRingPar->Write();
    corrRingPar->Write();
    for(Int_t j = 0;j<50;j++) {
	effPM->Fill(j*20,fitAndSubtractBG(thetaDiffPM[j][0]));
	effPM1->Fill(j*20,fitAndSubtractBG(thetaDiffPM[j][1]));
	effPM2->Fill(j*20,fitAndSubtractBG(thetaDiffPM[j][2]));
	effPM3->Fill(j*20,fitAndSubtractBG(thetaDiffPM[j][3]));
	effPM4->Fill(j*20,fitAndSubtractBG(thetaDiffPM[j][4]));
	effPM5->Fill(j*20,fitAndSubtractBG(thetaDiffPM[j][5]));
	effPM6->Fill(j*20,fitAndSubtractBG(thetaDiffPM[j][6]));
	effPM7->Fill(j*20,fitAndSubtractBG(thetaDiffPM[j][7]));
	if(j<11) {
	    effHT->Fill(j*10,fitAndSubtractBG(thetaDiffHT[j][0]));
	    effHT1->Fill(j*10,fitAndSubtractBG(thetaDiffHT[j][1]));
	    effHT2->Fill(j*10,fitAndSubtractBG(thetaDiffHT[j][2]));
	    effHT3->Fill(j*10,fitAndSubtractBG(thetaDiffHT[j][3]));
	    effHT4->Fill(j*10,fitAndSubtractBG(thetaDiffHT[j][4]));
	    effHT5->Fill(j*10,fitAndSubtractBG(thetaDiffHT[j][5]));
	    effHT6->Fill(j*10,fitAndSubtractBG(thetaDiffHT[j][6]));
	    effHT7->Fill(j*10,fitAndSubtractBG(thetaDiffHT[j][7]));
	}
	else {
	    effHT->Fill(j*20,fitAndSubtractBG(thetaDiffHT[j][0]));
	    effHT1->Fill(j*20,fitAndSubtractBG(thetaDiffHT[j][1]));
	    effHT2->Fill(j*20,fitAndSubtractBG(thetaDiffHT[j][2]));
	    effHT3->Fill(j*20,fitAndSubtractBG(thetaDiffHT[j][3]));
	    effHT4->Fill(j*20,fitAndSubtractBG(thetaDiffHT[j][4]));
	    effHT5->Fill(j*20,fitAndSubtractBG(thetaDiffHT[j][5]));
	    effHT6->Fill(j*20,fitAndSubtractBG(thetaDiffHT[j][6]));
	    effHT7->Fill(j*20,fitAndSubtractBG(thetaDiffHT[j][7]));
	}
   }
    HRichUtilFunc::saveHistos(out,pHistArray);
    out->Close();
    return kTRUE;
}
