// File: hrichpedcalnew.cc
//
// Author: Angelika Elhardt  <angelika.elhardt@physik.tu-muenchen.de>
// Last update by Angelika Elhardt: 00/11/18 16:16:41
// last modified by W.Schoen: 2000/11/08


#include "hrichpedcalnew.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hrichdetector.h"
#include "hcategory.h"
#include "hmatrixcatiter.h"
#include "hrichcal.h"
#include "richdef.h"
#include "hrichgeometrypar.h"
#include "hdetgeompar.h"
#include "hruntimedb.h"
#include <TFile.h>
#include <string.h>
#include <math.h>
#include "hrichcalparcell.h"
#include "hlocation.h"
#include "hrichcalpar.h"

ClassImp(HRichPedCalNew)
    
HRichPedCalNew::HRichPedCalNew(Text_t *name, Text_t *title,char * 
				       path ,char * pString): HReconstructor(name,title) {    
    
    pedPathName = path;
    
    strcpy(sectorList,pString);
    numSec = strlen(sectorList);
   
    pRichCal = 0;
    pIter = 0;

    sectorMax = 6;
    rowMax = 90;
    colMax = 92;
    hardwareMax = 17464;

    size = 6.;  //   for finding runaways in meanAndSigma!
    epsilon = 0.001; 
    channels = 2;  
  
    sigmaTransformation = false;
    
    for (int i=0;i<numSec;i++){
	sectorList[i]=sectorList[i]-1;
    }
    checkInput = true;
    if (numSec > sectorMax) {
	cout<<"!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
	cout<<"We have only 6 sectors!"<<endl;
	cout<<"!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
	checkInput = false;
    }
    
    if (checkInput){
	createStatistics();
	createMeans();
	initializeStatistics();
	initializeMeans();
	checkSectorList();
    }
}

HRichPedCalNew::~HRichPedCalNew(void) {
    
    deleteStatistics();
    deleteMeans();
}

void HRichPedCalNew::createStatistics(){
    
    pStat = new int[numSec][92][90];
    pRunAway = new int[numSec][92][90];
   
}

void HRichPedCalNew::createMeans(){

    pDiff = new double[numSec][92][90];
    pMean = new double[numSec][92][90];
    pSigma = new double[numSec][92][90];

}


void HRichPedCalNew::initializeStatistics(){
    
    for (int i=0;i<numSec;i++){
	for (int j=0; j<colMax; j++){
	    for (int k=0; k<rowMax; k++){
		
		pStat[i][j][k] = 0;
		pRunAway[i][j][k] = 0;		
		countArray[i][j][k] = 0;

	    }
	}
    }   
}

void HRichPedCalNew::initializeMeans(){
    
    for (int i=0;i<numSec;i++){
	for (int j=0; j<colMax; j++){
	    for (int k=0; k<rowMax; k++){
		
		pDiff[i][j][k] = 0.;
		pMean[i][j][k] = 0.;
		pSigma[i][j][k] = 0.;		
	    }
	}
    }   
}

void HRichPedCalNew::deleteStatistics(){
    
    delete [] pStat;
    delete [] pRunAway;
}

void HRichPedCalNew::deleteMeans(){
    
    delete [] pDiff;
    delete [] pMean;
    delete [] pSigma;
}

Bool_t HRichPedCalNew::checkSectorList(){

    //  cout<<"you want to analyse the following sectors:"<<endl;
//      for (int i = 0; i<numSec; i++){
//  	cout<<sectorList[i]<<endl;
//      }
    
    for (int i = 0; i<numSec; i++){
	char b = sectorList[i];
	if (!(b>47 && b<55)){
	    cout<<"Attention!!!  "<<sectorList[i]+1<<" is not a valid input!"<<endl;
	    checkInput = false;
	    cout<<"checkInput =  "<<checkInput<<endl;
	    exit(1);
	}
    }
    return checkInput;
}

void HRichPedCalNew::fillMask(){
    int iX = 0;
    for (int i=0; i<sectorMax; i++) {	
	mask[i][0] = 0;
	mask[i][1] = 0;
       }
    
    for (int i=0; i<numSec; i++){
	int a = sectorList[i]-48;     // Typkonvertierung char->int
	mask[a][0] = 1;
	mask[a][1] = iX;
	iX++;	
    }
}

void HRichPedCalNew::bookHistos(){

    char * mean[6];
    char * mean1d[6];
    char * sigma[6];
    char * sigma1d[6];
    char * stat[6];
    char * runAway[6];
    char temp1[7];
    char temp2[9];
    char temp3[8];
    char temp4[10];
    char temp5[7];
    char temp6[10];

    for (int i = 0; i<sectorMax; i++){

	sprintf(temp1,"mean_%d",i+1);
	sprintf(temp2,"mean1d_%d",i+1);
	sprintf(temp3,"sigma_%d",i+1);
	sprintf(temp4,"sigma1d_%d",i+1);
	sprintf(temp5,"stat_%d",i+1);
	sprintf(temp6,"runAway_%d",i+1);

  	mean[i] = temp1;
  	mean1d[i] = temp2;
  	sigma[i] = temp3;
  	sigma1d[i] = temp4;
  	stat[i] = temp5;
	runAway[i] = temp6;     	
   
	meanHisto[i] = new TH2F (mean[i],mean[i],colMax,1,colMax,rowMax,1,rowMax);
	sigmaHisto[i] = new TH2F (sigma[i],sigma[i],colMax,1,colMax,rowMax,1,rowMax);
	mean1dHisto[i] = new TH1F (mean1d[i],mean1d[i],17464,0,17464);
	sigma1dHisto[i] = new TH1F (sigma1d[i],sigma1d[i],17464,0,17464);
	statHisto[i] = new TH2F (stat[i],stat[i],colMax,1,colMax,rowMax,1,rowMax);
	runAwayHisto[i] = new TH2F (runAway[i],runAway[i],colMax,1,colMax,rowMax,1,rowMax);
    }

}

void HRichPedCalNew::fill2DHistos(){

    if (nCounter <=0){
	cout<<"No Data found in any of the requested sectors !! Aborting !!"<<endl;
	exit(1);
    }

    for (int i = 0; i<sectorMax; i++){   // max. 6 sectors!
	if (mask[i][0]==1){    // wichtig!
	    for (int j = 0; j<colMax; j++){
		for (int k = 0; k<rowMax; k++){
		    
		    int sIx = mask[i][1];
 
					    
		    statHisto[i]->Fill(j,k,pStat[sIx][j][k]);
		    runAwayHisto[i]->Fill(j,k,pRunAway[sIx][j][k]);
		    meanHisto[i]->Fill(j,k,pMean[sIx][j][k]);
		    
		    float radikand = pSigma[sIx][j][k]*countArray[sIx][j][k];
		    
		    if(radikand < 0.){
			sigmaHisto[i]->Fill(j,k,20000.);
			cout<<" !2D! numerical problem occured "<<endl;
			cout<<" radikand = "<<radikand<<"  col = "<<j<<" row = "<<k<<endl;
			
		    }else{

			pSigma[sIx][j][k] = sqrt(radikand);   // Varianz to sigma transformation

			if(pStat[sIx][j][k]==1){
			    pSigma[sIx][j][k]=0.01;
			}
			sigmaHisto[i]->Fill(j,k,pSigma[sIx][j][k]);
		    }
		}
	    }
	}
    }
    sigmaTransformation = true;
}
 
void HRichPedCalNew::fill1DHistos(){

    for (int i = 0; i<sectorMax; i++){   // max. 6 sectors!
	if (mask[i][0] == 1){
	    int sIx = mask[i][1];
	    for (int j = 0; j<hardwareMax; j++){

		int x = getCol(j);
		int y = getRow(j);

		mean1dHisto[i]->Fill(j,pMean[sIx][x][y]);
		Float_t radikand = pSigma[sIx][x][y] * pSigma[sIx][x][y];

		if(radikand < 0.){

		    sigma1dHisto[i]->Fill(j,20000.);
		    cout<<" !1D! numerical problem occured "<<endl;
		    cout<<" radikand = "<<radikand<<"  col = "<<x<<" row = "<<y<<endl;
		}else{

		    pSigma[sIx][x][y] = sqrt(radikand);

		    if(pStat[sIx][x][y]==1){
			pSigma[sIx][x][y]=0.01;
		    }
		    sigma1dHisto[i]->Fill(j,pSigma[sIx][x][y]);
		}
	    }
	}
    }
}


int HRichPedCalNew::getCol(int hardwarenumber){

    int x = (int) (*vec5)(hardwarenumber);
    return x;
}

int HRichPedCalNew::getRow(int hardwarenumber){

    int y = (int) (*vec6)(hardwarenumber);
    return y;
}

void HRichPedCalNew::uipFile(){

    TFile *pFile = new TFile("uipfile_new.root","read","Testfile",1);
    if(pFile){
        vec3 = (TVector*) pFile->Get("uip"); //retrieve object from file
	vec5 = (TVector*) pFile->Get("pads_x"); //and keyname
	vec6 = (TVector*) pFile->Get("pads_y"); 
    }
    else cout << "problems opening upifile" << endl;
}

void HRichPedCalNew::bookCanvases(){

}

void HRichPedCalNew::calculate(){

    HRichCal *pCal=NULL;
    Bool_t kData = kFALSE;
    pIter->Reset();
    
    while( (pCal = (HRichCal *)pIter->Next()) ){
	kData = kTRUE;
	
	Int_t sec = pCal->getSector();
	Float_t fCharge = pCal->getCharge();
	Int_t iRow = pCal->getRow();
	Int_t iCol = pCal->getCol();

	if(mask[sec][0]==1){
	    if(iRow < rowMax  && iCol < colMax){
		int sIx = mask[sec][1]; 
		
		meanAndSigma(sIx,iCol,iRow,fCharge);
		countArray[sIx][iCol][iRow]++;
		pStat[sIx][iCol][iRow]= pStat[sIx][iCol][iRow]+1;
	    }else{
		cout<<"Warning: impossible Pad numbers: row: "
		    <<iRow<<"  col: "<<iCol<<"   charge: "<<fCharge<<endl;
	    }   
	}	
    }
    if (kData) nCounter++;
}

void HRichPedCalNew::initializeMeanSigma(){

    for(int i=0; i<sectorMax; i++){
	for(int j=0; j<colMax; j++){
	    for(int k=0; k<rowMax; k++){
	 
		pDiff[i][j][k] = 0.;
		pMean[i][j][k] = 0.;
		pSigma[i][j][k] = 0.;
	    }
	}
    }
}

void HRichPedCalNew::meanAndSigma(int sIx, int col, int row, float charge){

    // !! das hier berechnete sigma ist die Varianz !!
    // !! in fill2DHistos wird aus der Varianz, die sigma heisst, sigma berechnet
    double nEv = (double)countArray[sIx][col][row];
    double sigma = sqrt(pSigma[sIx][col][row]*nEv);

    if( nEv>5. && fabs(pMean[sIx][col][row] - charge) >= size * sigma + epsilon && fabs(pMean[sIx][col][row] - charge) > channels ){
	// "nEv>5." to avoid numerical problems 
	pRunAway[sIx][col][row]++;
	countArray[sIx][col][row]--;
    }
    else{
	
	pDiff[sIx][col][row] = ((double)charge - pMean[sIx][col][row]) / (nEv+1.);
	pMean[sIx][col][row] = pMean[sIx][col][row] + pDiff[sIx][col][row];
	pSigma[sIx][col][row] = (nEv==0.)?0.:((nEv-1.)/(nEv+1.))*pSigma[sIx][col][row]+pDiff[sIx][col][row]*pDiff[sIx][col][row]; 
	
    }
    
}

void HRichPedCalNew::bookCounterHisto(){

    count = new TH1F("count","count",3,0,3);
}

void HRichPedCalNew::fillCounterHisto(){

    count->Fill(1,nCounter+1);

}

void HRichPedCalNew::outputFile(){

    char fileName[128];
    char * extRoot = ".root";

    strcpy(fileName,pedPathName);
    strcat(fileName,extRoot);

    dataFile = new TFile(fileName, "RECREATE");

    count->Write();
    for (int i = 0; i<sectorMax; i++){
	
	if (mask[i][0] == 1){
	    
	    meanHisto[i]->Write();
	    mean1dHisto[i]->Write();
	    sigmaHisto[i]->Write();
	    sigma1dHisto[i]->Write();
	    statHisto[i]->Write();
	    runAwayHisto[i]->Write();	    
	}
    }
    dataFile->Close();
    cout<<"Histos are written to: "<<fileName<<endl;
}


Bool_t HRichPedCalNew::init() {
    
    if (checkInput){
		
	fillMask();
	bookHistos();
	bookCounterHisto();
	
	HRichDetector *pRichDet = 
	    (HRichDetector*)gHades->getSetup()->getDetector("Rich");
	pRichCal=gHades->getCurrentEvent()->getCategory(catRichCal);
	
	if (!pRichCal) {
	    pRichCal=pRichDet->buildCategory(catRichCal);
	    
	    if (!pRichCal) return kFALSE;
	    else gHades->getCurrentEvent()
		     ->addCategory(catRichCal, pRichCal, "Rich");
	}
	
	pIter = (HMatrixCatIter*)pRichCal->MakeIterator();
	
	uipFile();
      	nCounter = 0;
    }
    return kTRUE;
    
}

Int_t HRichPedCalNew::execute() {
    
    if (checkInput){    
    calculate();   
    }
    return kTRUE;
}

Bool_t HRichPedCalNew::finalize(void) {
    
    if (checkInput){
	cout<<"We are in finalize!"<<endl;
	fill2DHistos();
	if (sigmaTransformation){
	    fill1DHistos();
	    fillCounterHisto();
	    outputFile();
	}
    }
    return kTRUE;
}
 


 
 














