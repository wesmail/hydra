// File: hrichpedestalraw.cc
//
// Author: Angelika Elhardt  <angelika.elhardt@physik.tu-muenchen.de>
// Last update by Angelika Elhardt: 01/01/13 15:14:06
// last modified by W.Schoen: 2000/11/08

#include <stdlib.h>
#include "hrichpedestalraw.h"
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
#include "hrichcalparcell.h"
#include "hlocation.h"
#include "hrichcalpar.h"
#include "hrichmappingpar.h"
#include <TFile.h>
#include <string.h>

ClassImp(HRichPedestalRaw)
    
HRichPedestalRaw::HRichPedestalRaw(Text_t *name, Text_t *title,
		  char * path ,char * pString): HReconstructor(name,title) {    
    
    pedPathName = path;
    
    strcpy(sectorList,pString);
    numSec = strlen(sectorList);
    for (int i=0;i<numSec;i++){
	sectorList[i]=sectorList[i]-1;
    }
    pRichRaw = 0;
    pIter = 0;

    sectorMax = 6;
    rowMax = 90;
    colMax = 92;
    hardwareMax = 17464;
  
    normalizedSums = false;
    checkInput = true;
    if (numSec > sectorMax) {
	cout<<"!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
	cout<<"We have only 6 sectors!"<<endl;
	cout<<"!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
	checkInput = false;
    }
    
    if (checkInput){
	createSums();
	initializeSums();
	checkSectorList();
    }
}

HRichPedestalRaw::~HRichPedestalRaw(void) {
    
    deleteSums();    
}

void HRichPedestalRaw::createSums(){
    
    pSum = new double[numSec][96][96];
    pSum2 = new double[numSec][96][96];
    pStat = new int[numSec][96][96];   
}

void HRichPedestalRaw::initializeSums(){
    
    for (int i=0;i<numSec;i++){
	
	for (int j=0; j<colMax; j++){
	    
	    for (int k=0; k<rowMax; k++){
		
		pSum[i][j][k] = 0;
		pSum2[i][j][k] = 0;
		pStat[i][j][k] = 0;		
		
	    }
	}
    }   
}

void HRichPedestalRaw::deleteSums(){
    
    delete [] pSum;
    delete [] pSum2;
    delete [] pStat;
}

Bool_t HRichPedestalRaw::checkSectorList(){
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

void HRichPedestalRaw::fillMask(){
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

void HRichPedestalRaw::bookHistos(){

    char * mean[6];
    char * mean1d[6];
    char * sigma[6];
    char * sigma1d[6];
    char * stat[6];
    char temp1[7];
    char temp2[9];
    char temp3[8];
    char temp4[10];
    char temp5[7];

    for (int i = 0; i<sectorMax; i++){

	sprintf(temp1,"mean_%d",i+1);
	sprintf(temp2,"mean1d_%d",i+1);
	sprintf(temp3,"sigma_%d",i+1);
	sprintf(temp4,"sigma1d_%d",i+1);
	sprintf(temp5,"stat_%d",i+1);

  	mean[i] = temp1;
  	mean1d[i] = temp2;
  	sigma[i] = temp3;
  	sigma1d[i] = temp4;
  	stat[i] = temp5;     	
   
	meanHisto[i] = new TH2F (mean[i], mean[i], colMax,1,colMax,rowMax,1,rowMax);
	sigmaHisto[i] = new TH2F (sigma[i], sigma[i], colMax,1,colMax,rowMax,1,rowMax);
	mean1dHisto[i] = new TH1F (mean1d[i], mean1d[i], 17464,1,17464);
	sigma1dHisto[i] = new TH1F (sigma1d[i], sigma1d[i], 17464,1,17464);
	statHisto[i] = new TH2F (stat[i], stat[i], colMax,1,colMax,rowMax,1,rowMax);
    }

}

void HRichPedestalRaw::fill2DHistos(){
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
		    
		    pSum[sIx][j][k] = pSum[sIx][j][k]/nCounter;
		    pSum2[sIx][j][k] = pSum2[sIx][j][k]/nCounter;
		    Float_t radikand = pSum2[sIx][j][k]-pSum[sIx][j][k]*pSum[sIx][j][k];

		    if (radikand >= 0){
			pSum2[sIx][j][k] = sqrt(radikand);
			meanHisto[i]->Fill(j,k,pSum[sIx][j][k]);
			sigmaHisto[i]->Fill(j,k,pSum2[sIx][j][k]);
		    }else{
			cout<<"!2D!numeric prob occured, nr of events proc: "
			    <<nCounter<<" , pedestal generation aborted !"<<endl;
			cout<<" radikand   :"<<radikand<<"  col : "<<j
			    <<"    row : "<<k<<endl;
			exit(1);
			//	    return kFALSE;
		    }
		}
	    }
	}
    }
    normalizedSums = true;
}

void HRichPedestalRaw::fill1DHistos(){

    for (int i = 0; i<sectorMax; i++){   // max. 6 sectors!
	if (mask[i][0] == 1){
	    int sIx = mask[i][1];
	    for (int j = 0; j<hardwareMax; j++){

		int x = getCol(j);
		int y = getRow(j);

		Float_t radikand = pSum2[sIx][x][y]*pSum2[sIx][x][y];

		if (radikand >= 0){
		    mean1dHisto[i]->Fill(j,pSum[sIx][x][y]);
		    sigma1dHisto[i]->Fill(j,pSum2[sIx][x][y]);
		}else{
	  	    cout<<"!1D!numeric prob occured @ event no "
			<<nCounter<<" , pedestal generation aborted !"<<endl;
		    cout<<" radikand   :"<<radikand<<" col : "<<x
			<<" row : "<<y<<endl;
		    //	    return kFALSE;
		}
	    }
	}
    }
}


int HRichPedestalRaw::getCol(int hardwarenumber){
  fMappingPar = gHades->getRuntimeDb()->getContainer("RichMappingParameters");
  vec5 = ((HRichMappingPar*)fMappingPar)->padsx;
  int x = (int) (*vec5)(hardwarenumber);
  return x;
}

int HRichPedestalRaw::getRow(int hardwarenumber){
  fMappingPar=gHades->getRuntimeDb()->getContainer("RichMappingParameters");
  vec6 = ((HRichMappingPar*)fMappingPar)->padsy;
  int y = (int) (*vec6)(hardwarenumber);
  return y;
}

void HRichPedestalRaw::uipFile(){
  //cout << "using uipfile information" << endl;
  TFile *pFile = new TFile("uipfile_new.root","read","Testfile",1);
  if(pFile){
    vec3 = (TVector*) pFile->Get("uip"); //retrieve object from file
    vec5 = (TVector*) pFile->Get("pads_x"); //and keyname
    vec6 = (TVector*) pFile->Get("pads_y"); 
  }
  else cout << "problems opening upifile" << endl;
}

void HRichPedestalRaw::bookCanvases(){

}

void HRichPedestalRaw::calculate(){

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
		
//if ((iCol==0 && iRow==0) ||(iCol==1 && iRow==0)||(iCol==0 && iRow==1) )
//cout << "decoded a wrong address: Col= "<< iCol << " Row= " << iRow << endl;

//  if(sec==1 && iCol==44 && iRow==55){
//  		    cout<<"(2,44,55): "<<fCharge<<endl;
//  		}
//  		if(sec==1 && iCol==63 && iRow==67){
//  		    cout<<"(2,63,67): "<<fCharge<<endl;
//  		}
//  		if(sec==0 && iCol==55 && iRow==41){
//  		    cout<<"(1,55,41): "<<fCharge<<endl;
//  		}
//  		if(sec==0 && iCol==27 && iRow==62){
//  		    cout<<"(1,27,62): "<<fCharge<<endl;
//  		}
//  		if(sec==2 && iCol==35 && iRow==52){
//  		    cout<<"(3,35,52): "<<fCharge<<endl;
//  		}
//  		if(sec==2 && iCol==77 && iRow==71){
//  		    cout<<"(3,77,71): "<<fCharge<<endl;
//  		}
//  		if(sec==5 && iCol==16 && iRow==75){
//  		    cout<<"(6,16,75): "<<fCharge<<endl;
//  		}
//  		if(sec==5 && iCol==53 && iRow==12){
//  		    cout<<"(6,53,12): "<<fCharge<<endl;
//  		}

		pSum[sIx][iCol][iRow]= pSum[sIx][iCol][iRow]+fCharge;
		pSum2[sIx][iCol][iRow]= pSum2[sIx][iCol][iRow]+fCharge*fCharge;
		pStat[sIx][iCol][iRow]= pStat[sIx][iCol][iRow]+1;
	    }else{
		cout<<"Warning: impossible Pad numbers: row: "
		    <<iRow<<"  col: "<<iCol<<"   charge: "<<fCharge<<endl;
	    }   
	}	
    }
    if (kData) nCounter++;
}

void HRichPedestalRaw::outputFile(){

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
	}
    }
    dataFile->Close();
    cout<<"Histos are written to: "<<fileName<<endl;
}


Bool_t HRichPedestalRaw::init() {
    
    if (checkInput){
	//	cout<<"We are in init!"<<endl;
	//cout<<endl;
	
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
	//There should no longer be any uipfile in use. 
	//We are supposed to handle this viw mapping par
	//containers!
	//uipFile();

	initMappingPar();
	initCalPar();

      	nCounter = 0;
    }
    return kTRUE;
    
}

Int_t HRichPedestalRaw::execute() {
    
    if (checkInput){    
    calculate();  // mean and sigma  
    }
    return kTRUE;
}

Bool_t HRichPedestalRaw::finalize(void) {
    
    if (checkInput){
	//cout<<"We are in finalize!"<<endl;
	fill2DHistos();
	if (normalizedSums) fill1DHistos();
	outputFile();
	createPedestal();
	createCalibration();
	fillCalParCntr();
    }
    return kTRUE;
}

void HRichPedestalRaw::initCalPar() {

  HRuntimeDb* rtdb=gHades->getRuntimeDb();
  fCalPar = rtdb->getContainer("RichCalPar");
}

void HRichPedestalRaw::initMappingPar() {
  cout << "init mappar" << endl;
  HRuntimeDb* rtdb=gHades->getRuntimeDb();
  fMappingPar = rtdb->getContainer("RichMappingParameters");
}

Bool_t HRichPedestalRaw::fillCalParCntr(){
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
		      pCell->setOffset(pSum[sIx][i][j]);
		      pCell->setSigma(pSum2[sIx][i][j]);
		      n++;
		    }else{
			cerr<<"Error in HRichPedestalRaw::fillCalParCntr"<<endl;
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

void HRichPedestalRaw::printCalParCntr(){
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
void HRichPedestalRaw::createPedestal(){

    char pedestalFile[128];      // filename and path!
    char * appSec[6] = {"1","2","3","4","5","6"};
       
    Int_t rc,tmp,port,mod,ch,x,y;
    rc=tmp=port=mod=ch=x=y=0;

    Int_t iCount , hardware;
    iCount=hardware=0;

    //This should not be used any longer as we are not supposed to
    //rely on supplemental root files, but on our container initialization
    //procedure
    //TFile *pUnConn = new 
    //TFile("unconnected_pads.root","read",
    //  "file contains pads that are not connected",1);
    //TVector *pUncVec = (TVector*) pUnConn->Get("uip unconnected");

    HRuntimeDb* rtdb=gHades->getRuntimeDb();
    fMappingPar = rtdb->getContainer("RichMappingParameters");
    TVector *pUncVec = ((HRichMappingPar*)fMappingPar)->uncuip;
    vec3 = ((HRichMappingPar*)fMappingPar)->uiplog;

    for (Int_t i = 0; i<sectorMax; i++){  // max. 6 sectors
	
	if(mask[i][0] == 1){ 

	    int sIx = mask[i][1];

	    strcpy(pedestalFile,pedPathName);
	    strcat(pedestalFile,"_");
	    
	    strcat(pedestalFile,appSec[i]);
	    strcat(pedestalFile,".dat");    
	    cout<<"------------------------------------------"<<endl;
	    cout<<"pedestal file for sector "<<i+1<<" written: "<<endl; 
	    cout << "fileName: "<<pedestalFile<< endl;
	    
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
				       <<" "<<ch<<"  "<<pSum[sIx][x][y]
				       <<"  "<<pSum2[sIx][x][y]<<endl;
			    
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

void HRichPedestalRaw::createCalibration(){

    char * extPed = ".txt";

    char calFile[128];       // filename and path!
    strcpy(calFile,pedPathName);
    strcat(calFile,"_cal");
    strcat(calFile,extPed);
    //cout<<calFile<<endl;
    cout<<"--------------------------------------------"<<endl;
    cout<<"calibration file written: "<<endl;
    cout<< "fileName: "<<calFile<<endl;
    
    ofstream calOut(calFile,ios::out);
    if (!calOut) {
	cerr<< "Error: unable to open " << calFile << endl;
	exit(2);
    }
    calOut<<"# ASCII calibration file"<<endl<<endl<<endl;
    calOut<<"[ Rich Calibrater Parameters Setup ]"<<endl<<endl;
    calOut<<"Sector	Row	Column"<<endl;
    calOut<<sectorMax<<"	"<<rowMax<<"	"<<colMax<<endl<<endl;

    for(Int_t k =0; k<sectorMax; k++){
	cout<<endl;
	calOut<<"[ Rich Calibrater Parameters: Sector "<<k<<" ]"<<endl<<endl;
	calOut<<"Row	Column	Slope	Offset	Sigma"<<endl;
	
	for (Int_t i = 0; i<colMax; i++){
	    for (Int_t j = 0; j<rowMax; j++){
		
		if(mask[k][0] == 1){
		    int sIx = mask[k][1];
		    calOut<<j<<"	"<<i<<"	1"<<"	"<<pSum[sIx][i][j]<<"	"<<pSum2[sIx][i][j]<<endl;
		}else     calOut<<j<<"	"<<i<<"	1"<<"	"<<"0"<<"	"<<"0"<<endl; // zero calibration parameters for nonactive sectors !
	    }
	}	
	calOut<<endl;
    }
    calOut.close();
}
 














