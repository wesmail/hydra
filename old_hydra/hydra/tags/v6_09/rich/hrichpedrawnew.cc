// File: hrichpedrawnew.cc
//
// Author: Angelika Elhardt  <angelika.elhardt@physik.tu-muenchen.de>
// Last update by Angelika Elhardt: 01/01/09 18:37:00
// last modified by W.Schoen: 2000/11/08


#include <stdlib.h>
#include "hrichpedrawnew.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hrichdetector.h"
#include "hcategory.h"
#include "hmatrixcatiter.h"
#include "hrichraw.h"
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
#include "hrichmappingpar.h"

ClassImp(HRichPedRawNew)
    
    HRichPedRawNew::HRichPedRawNew(Text_t *name, Text_t *title,char * 
				       path ,char * pString): HReconstructor(name,title) {    
    
    pedPathName = path;
    
    strcpy(sectorList,pString);
    numSec = strlen(sectorList);
   
    pRichRaw = 0;
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

HRichPedRawNew::~HRichPedRawNew(void) {
    
    deleteStatistics();
    deleteMeans();
}

void HRichPedRawNew::createStatistics(){
    
    pStat = new int[numSec][92][90];
    pRunAway = new int[numSec][92][90];
   
}

void HRichPedRawNew::createMeans(){

    pDiff = new double[numSec][92][90];
    pMean = new double[numSec][92][90];
    pSigma = new double[numSec][92][90];

}


void HRichPedRawNew::initializeStatistics(){
    
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

void HRichPedRawNew::initializeMeans(){
    
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

void HRichPedRawNew::deleteStatistics(){
    
    delete [] pStat;
    delete [] pRunAway;
}

void HRichPedRawNew::deleteMeans(){
    
    delete [] pDiff;
    delete [] pMean;
    delete [] pSigma;
}

Bool_t HRichPedRawNew::checkSectorList(){

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

void HRichPedRawNew::fillMask(){
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

void HRichPedRawNew::bookHistos(){

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

void HRichPedRawNew::fill2DHistos(){

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
 
void HRichPedRawNew::fill1DHistos(){

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


int HRichPedRawNew::getCol(int hardwarenumber){
  fMappingPar = gHades->getRuntimeDb()->getContainer("RichMappingParameters");
  vec5 = ((HRichMappingPar*)fMappingPar)->padsy;
  int x = (int) (*vec5)(hardwarenumber);
  return x;
}

int HRichPedRawNew::getRow(int hardwarenumber){
  fMappingPar=gHades->getRuntimeDb()->getContainer("RichMappingParameters");
  vec6 = ((HRichMappingPar*)fMappingPar)->uiplog;
  int y = (int) (*vec6)(hardwarenumber);
  return y;
}

void HRichPedRawNew::uipFile(){

    TFile *pFile = new TFile("uipfile_new.root","read","Testfile",1);
    if(pFile){
      vec3 = (TVector*) pFile->Get("uip"); //retrieve object from file
      vec5 = (TVector*) pFile->Get("pads_x"); //and keyname
      vec6 = (TVector*) pFile->Get("pads_y"); 
    }
    else cout << "problems opening uipfile" << endl;
}

void HRichPedRawNew::bookCanvases(){

}

void HRichPedRawNew::calculate(){

    HRichRaw *pRaw=NULL;
    Bool_t kData = kFALSE;
    pIter->Reset();
    
    while( (pRaw = (HRichRaw *)pIter->Next()) ){
	kData = kTRUE;
	
	Int_t sec = pRaw->getSector();
	Float_t fCharge = pRaw->getCharge();
	Int_t iRow = pRaw->getRow();
	Int_t iCol = pRaw->getCol();

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

void HRichPedRawNew::initializeMeanSigma(){

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

void HRichPedRawNew::meanAndSigma(int sIx, int col, int row, float charge){

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


void HRichPedRawNew::outputFile(){

    char fileName[128];
    char * extPed = "ped";
    char * extRoot = ".root";

    strcpy(fileName,pedPathName);
    strcat(fileName,"_");
    strcat(fileName,extPed);
    strcat(fileName,extRoot);

    dataFile = new TFile(fileName, "RECREATE");

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


Bool_t HRichPedRawNew::init() {
    
    if (checkInput){
		
	fillMask();
	bookHistos();
	
	HRichDetector *pRichDet = 
	    (HRichDetector*)gHades->getSetup()->getDetector("Rich");
	pRichRaw=gHades->getCurrentEvent()->getCategory(catRichRaw);
	
	if (!pRichRaw) {
	    pRichRaw=pRichDet->buildCategory(catRichRaw);
	    
	    if (!pRichRaw) return kFALSE;
	    else gHades->getCurrentEvent()
		     ->addCategory(catRichRaw, pRichRaw, "Rich");
	}
	
	pIter = (HMatrixCatIter*)pRichRaw->MakeIterator();
	
	//uipFile();
	initMappingPar();
	initCalPar();
      	nCounter = 0;
    }
    return kTRUE;
    
}

Int_t HRichPedRawNew::execute() {
    
    if (checkInput){    
    calculate();   
    }
    return kTRUE;
}

Bool_t HRichPedRawNew::finalize(void) {
    
    if (checkInput){
	cout<<"We are in finalize!"<<endl;
	fill2DHistos();
	if (sigmaTransformation){
	    fill1DHistos();
	    outputFile();
	    createPedestal();
	    createCalibration();
	    fillCalParCntr();
	}
    }
    return kTRUE;
}
void HRichPedRawNew::initMappingPar() {
    HRuntimeDb* rtdb=gHades->getRuntimeDb();
    fMappingPar = rtdb->getContainer("RichMappingParameters");
}

void HRichPedRawNew::initCalPar() {
  //  HRichDetector *pRichDet = (HRichDetector*)gHades->getSetup()
  //                                                    ->getDetector("Rich");
  
  HRuntimeDb* rtdb=gHades->getRuntimeDb();
  fCalPar = rtdb->getContainer("RichCalPar");
  //  if (fCalPar) {
  //         ((HRichCalPar*)fCalPar)->
  //                setSetup(pRichDet->getSectors(),
  //                         pRichDet->getRows(),
  //                         pRichDet->getColumns());
  //      }
  
}

Bool_t HRichPedRawNew::fillCalParCntr(){
    initCalPar();
    HLocation loc;
    HRichCalParCell *pCell=0;
    loc.setNIndex(3);
    Int_t n=0;
    for(Int_t k =0; k<sectorMax; k++){
	for (Int_t i = 0; i<colMax; i++){
	    for (Int_t j = 0; j<rowMax; j++){
		
		if(mask[k][0] == 1){//valid sectors
		    // cout<<"valid Sector "<<k<<endl;
		    int sIx = mask[k][1];
		    loc[0] = k;
		    loc[1] = j;
		    loc[2] = i;
		    pCell = (HRichCalParCell*) ((HRichCalPar*)getCalPar())
                                         ->getSlot(loc);

		    if (pCell) {
			
			pCell = new(pCell) HRichCalParCell;
			pCell->setSector(k);
			pCell->setRow(j);
			pCell->setCol(i);
			pCell->setSlope(1);
			pCell->setOffset(pMean[sIx][i][j]);
			pCell->setSigma(pSigma[sIx][i][j]);
			n++;
		    }else{
			cerr<<"Error in HRichPedRawNew::fillCalParCntr"<<endl;
			Error("fillCalParCntr(void)",
			      "slot not found:  %i %i %i",loc[0],loc[1],loc[2]);
			return kFALSE;
		    }
		}
	    }	
	}
	//	cout<<"Number of CalParCells allocated : "<<n<<endl;
	n=0;
    }
    
    //printCalParCntr();
	return kTRUE;
}

void HRichPedRawNew::printCalParCntr(){
    HLocation loc;
    HRichCalParCell *calparcell = NULL;
    loc.setNIndex(3);
    for(Int_t k =0; k<sectorMax; k++){
	for (Int_t i = 0; i<colMax; i++){
	    for (Int_t j = 0; j<rowMax; j++){
		
		if(mask[k][0] == 1){//valid sectors
		    
		    loc[0] = k;
		    loc[1] = j;
		    loc[2] = i; 
		    calparcell = (HRichCalParCell*) ((HRichCalPar*)fCalPar)
			->getObject(loc);
		    cout<<"Location: "<<loc[0]<<" "<<loc[1]<<" "<<loc[2]<<" : "<<calparcell->getSlope()<<" : "<<calparcell->getOffset()<<" : "<<calparcell->getSigma()<<endl;
		}
	    }
	}
    }
} 

void HRichPedRawNew::createPedestal(){

    char pedestalFile[128];      // filename and path!
    char * appSec[6] = {"1","2","3","4","5","6"};
       
    Int_t rc,tmp,port,mod,ch,x,y;
    rc=tmp=port=mod=ch=x=y=0;

    Int_t iCount , hardware;
    iCount=hardware=0;

    //TFile *pUnConn = new 
    //TFile("unconnected_pads.root","read",
    //    "file contains pads that are not connected",1);
    //TVector *pUncVec = (TVector*) pUnConn->Get("uip unconnected");

    HRuntimeDb* rtdb=gHades->getRuntimeDb();
    fMappingPar = rtdb->getContainer("RichMappingParameters");
    TVector *pUncVec = ((HRichMappingPar*)fMappingPar)->uncuip;
    vec3 = ((HRichMappingPar*)fMappingPar)->padsx;

    for (Int_t i = 0; i<sectorMax; i++){  // max. 6 sectors
	
	if(mask[i][0] == 1){ 

	    int sIx = mask[i][1];

	    strcpy(pedestalFile,pedPathName);
	    strcat(pedestalFile,"_");
	    strcat(pedestalFile,appSec[i]);
	    strcat(pedestalFile,".dat");    
	    cout<<"------------------------------------------"<<endl;
	    cout<<"pedestal file for sector "<<i+1<<" written: "<<endl; 
	    cout << "fileName: "<< pedestalFile<< endl;
	    
	    ofstream pedestalOut(pedestalFile,ios::out);
	    if (!pedestalOut) {
		cerr<< "Error: unable to open " << pedestalFile << endl;
		exit(2);
	    }
	    
	    for(Int_t j=0; j<hardwareMax; j++){
		
		hardware = j;
		for (Int_t k = 0; k<4; k++){  // 4 gassiplexe per frontend!
		    
		    hardware = j+(k*16);

		    x = getCol(hardware);
		    y = getRow(hardware);

		    rc = hardware/10000;
		    rc = rc + 2*i;     // 2 rc per sector!
		    tmp = hardware % 10000;
		    port = tmp/1000;
		    tmp = tmp % 1000;
		    mod = tmp/100;
		    ch = tmp % 100;

		    if (hardware < hardwareMax){
			if ((*vec3)(hardware)){
			   
			    pedestalOut<<rc<<" "<<port<<" "<<mod
				       <<" "<<ch<<"  "<<pMean[sIx][x][y]
				       <<"  "<<pSigma[sIx][x][y]<<endl;
			    
			}else{ 
			    if((*pUncVec)(hardware)){
				
				pedestalOut<<rc<<" "<<port<<" "<<mod
					   <<" "<<ch<<"  "<<"1023"<<"  "
					   <<"0"<<endl; 
			    }
			}
		    }
		}
		
		iCount++;
		if(iCount == 16){
		    iCount = 0;
		    j = j+84;
		}
	    }
	    pedestalOut.close();
	}
	
    }
    //    pedestalOut.close();
    //pUnConn->Close(); 
    
}

void HRichPedRawNew::createCalibration(){

    char * extPed = ".txt";

    char calFile[128];       // filename and path!

    strcpy(calFile,pedPathName);
    strcat(calFile,"_cal");
    strcat(calFile,extPed);

    cout<<"--------------------------------------------"<<endl;
    cout<<"calibration file written: "<<endl;
    cout << "fileName: "<< calFile<< endl;
    
    ofstream calOut(calFile,ios::out);
    if (!calOut) {
	cerr<< "Error: unable to open " << calFile << endl;
	exit(2);
    }
    calOut<<"# ASCII calibration file"<<endl<<endl<<endl;
    calOut<<"[ Rich Calibrater Parameters Setup ]"<<endl<<endl;
    calOut<<"Sectors	Rows	Columns"<<endl;
    calOut<<sectorMax<<"	"<<rowMax<<"	   "<<colMax<<endl<<endl;

  

    for(Int_t k =0; k<sectorMax; k++){
	cout<<endl;
	calOut<<"[ Rich Calibrater Parameters: Sector "<<k<<" ]"<<endl<<endl;
	calOut<<"Row	Column	Slope	Offset	Sigma"<<endl;
	
	for (Int_t i = 0; i<colMax; i++){
	    for (Int_t j = 0; j<rowMax; j++){
		
		if(mask[k][0] == 1){
		    int sIx = mask[k][1];
		    calOut<<j<<"	"<<i<<"	1"<<"	"<<pMean[sIx][i][j]<<"	"<<pSigma[sIx][i][j]<<endl;
		}else     calOut<<j<<"	"<<i<<"	1"<<"	"<<"0"<<"	"<<"0"<<endl; // zero calibration parameters for nonactive sectors !
	    }
	}	
	calOut<<endl;
    }
    calOut.close();
}




 
 














