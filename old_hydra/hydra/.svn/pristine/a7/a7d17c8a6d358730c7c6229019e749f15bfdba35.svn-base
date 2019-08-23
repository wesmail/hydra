// File: hrichpedestalupdate.cc
//
// Author: Tassilo Christ <tassilo.christ@physik.tu-muenchen.de>
// Last update by Tassilo Christ: 15.05.02 


#include <math.h>
#include <stdlib.h>
#include "hrichpedestalraw.h"
#include "hrichpedestalupdate.h"
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
#include "hrichthresholdpar.h"
#include "hrichmappingpar.h"
#include "TFile.h"
#include <string.h>

ClassImp(HRichPedestalUpdate)
    
HRichPedestalUpdate::HRichPedestalUpdate(const Text_t *name,const Text_t *title,
		  const Char_t * path ,const Char_t * pString): HReconstructor(name,title) {
    
    pedPathName = path;

    strcpy(sectorList,pString);
    numSec = strlen(sectorList);
    for (Int_t i=0;i<numSec;i++){
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

//NO CHANGE
HRichPedestalUpdate::~HRichPedestalUpdate(void) {
  
  deleteSums();    
}

//NO CHANGE
void HRichPedestalUpdate::createSums(){
  
  pStat = new int[numSec][96][96];   
  pFrac = new float[numSec][96][96];
  pShift = new float[numSec][96][96];
}

//NEW
void HRichPedestalUpdate::initializeErfTable(){
  cout << "initializing erftable" <<endl;
  for(Int_t i=0;i<901;i++)
    {
      //coordinate is the coordinate along which the gaussian error integral is changing
      //it is discretized in 900 steps between -4.5 and +4.5
      coordinate[i]= -4.5 + (float)i * 9.0/900.0 ;

      //gaussInt[i] is the integrated value of a normalized gaussian centered at zero at position i
      gaussInt[i]=0.5*(1.0 + erf(coordinate[i]/sqrt(2.)) );
      //cout << "coordinate: " << coordinate[i] << endl;
      //cout << "gaussInt: " << gaussInt[i] << endl;
    }
}

//NO CHANGE
void HRichPedestalUpdate::initializeSums(){
    
  for (Int_t i=0;i<numSec;i++){
    
    for (Int_t j=0; j<colMax; j++){
      
      for (Int_t k=0; k<rowMax; k++){
	
	pStat[i][j][k] = 0;		
	
      }
    }
  }   
}

//NO CHANGE
void HRichPedestalUpdate::deleteSums(){
    
  delete [] pStat;
}

//NO CHANGE
Bool_t HRichPedestalUpdate::checkSectorList(){
  for (Int_t i = 0; i<numSec; i++){
    Char_t b = sectorList[i];
    if (!(b>47 && b<55)){
      cout<<"Attention!!!  "<<sectorList[i]+1<<" is not a valid input!"<<endl;
      checkInput = false;
      cout<<"checkInput =  "<<checkInput<<endl;
      exit(1);
    }
  }
  return checkInput;
}

//NO CHANGE
void HRichPedestalUpdate::fillMask(){
    Int_t iX = 0;
    for (Int_t i=0; i<sectorMax; i++) {	
	mask[i][0] = 0;
	mask[i][1] = 0;
       }
    
    for (Int_t i=0; i<numSec; i++){
 Int_t a = sectorList[i]-48;     // Typkonvertierung char->int
	mask[a][0] = 1;
	mask[a][1] = iX;
	iX++;	
    }
}

//NO CHANGE
void HRichPedestalUpdate::bookHistos(){

  Char_t * mean[6];
  Char_t * mean1d[6];
  Char_t * sigma[6];
  Char_t * sigma1d[6];
  Char_t * stat[6];
  Char_t * Drift[6];
  Char_t temp1[7];
  Char_t temp2[9];
  Char_t temp3[8];
  Char_t temp4[10];
  Char_t temp5[7];
  Char_t temp6[8];
  
  for (Int_t i = 0; i<sectorMax; i++){
    
    sprintf(temp1,"mean_%d",i+1);
    sprintf(temp2,"mean1d_%d",i+1);
    sprintf(temp3,"sigma_%d",i+1);
    sprintf(temp4,"sigma1d_%d",i+1);
    sprintf(temp5,"stat_%d",i+1);
    sprintf(temp6,"Drift_%d",i+1);    

    mean[i] = temp1;
    mean1d[i] = temp2;
    sigma[i] = temp3;
    sigma1d[i] = temp4;
    stat[i] = temp5;     	
    Drift[i] = temp6;     	
    
    meanHisto[i] = new TH2F (mean[i], mean[i], colMax,1,colMax,rowMax,1,rowMax);
    sigmaHisto[i] = new TH2F (sigma[i], sigma[i], colMax,1,colMax,rowMax,1,rowMax);
    mean1dHisto[i] = new TH1F (mean1d[i], mean1d[i], 17464,1,17464);
    sigma1dHisto[i] = new TH1F (sigma1d[i], sigma1d[i], 17464,1,17464);
    statHisto[i] = new TH2F (stat[i], stat[i], colMax,1,colMax,rowMax,1,rowMax);
    driftHisto[i] = new TH2F (Drift[i], Drift[i], colMax,1,colMax,rowMax,1,rowMax);
  }
}


void HRichPedestalUpdate::normalizeStatistics()
{
  Int_t sec, col, row;
  for(sec=0;sec<sectorMax;sec++)
    {
      if (mask[sec][0]==1){ 
	for(row=0;row<rowMax;row++)
	  {
	    for(col=0;col<colMax;col++)
	      {
	 Int_t sIx=mask[sec][1];
		pFrac[sIx][col][row]=(float)pStat[sIx][col][row] / ((float) (nCounter));
		//cout << "Pad : "<< sIx <<" " << col <<" " << row << endl;
		//cout <<"has " << pStat[sIx][col][row] << " entries " << endl;
	      }
	  }
      }
    }
}

void HRichPedestalUpdate::fill1DHistos(){
  
  HLocation loc;
  HRichCalParCell *pCell=0;
  loc.setNIndex(3);

  for (Int_t i = 0; i<sectorMax; i++){   // max. 6 sectors!
    if (mask[i][0] == 1){
      for (Int_t j = 0; j<hardwareMax; j++){
 Int_t x = getCol(j);
 Int_t y = getRow(j);
	loc[0]=i;
	loc[1]=y;
	loc[2]=x;
	pCell = (HRichCalParCell*) ((HRichCalPar*)getCalPar())->getSlot(loc);
	mean1dHisto[i]->Fill(j,pCell->getOffset());
	sigma1dHisto[i]->Fill(j,pCell->getSigma());
      }
    }
  }
}


void HRichPedestalUpdate::fill2DHistos(){
  if (nCounter <=0){
	cout<<"No Data found in any of the requested sectors !! Aborting !!"<<endl;
	exit(1);
    }
  HLocation loc;
  HRichCalParCell *pCell=0;
  loc.setNIndex(3);
  for (Int_t i = 0; i<sectorMax; i++){   // max. 6 sectors!
    if (mask[i][0]==1){    // wichtig! (if sector exists?)
      for (Int_t j = 0; j<colMax; j++){
	for (Int_t k = 0; k<rowMax; k++){
	  Int_t sIx = mask[i][1];		
	  statHisto[i]->Fill(j,k,pStat[sIx][j][k]);
	  loc[0]=i;
	  loc[1]=k;
	  loc[2]=j;
	  pCell = (HRichCalParCell*) ((HRichCalPar*)getCalPar())->getSlot(loc);
	  meanHisto[i]->Fill(j,k,pCell->getOffset());
	  sigmaHisto[i]->Fill(j,k,pCell->getSigma());
	}
      }
    }
  }
  normalizedSums = true;
}

//NO CHANGE
Int_t HRichPedestalUpdate::getCol(Int_t hardwarenumber){
  fMappingPar = gHades->getRuntimeDb()->getContainer("RichMappingParameters");
  vec5 = ((HRichMappingPar*)fMappingPar)->padsx;
  Int_t x = (int) (*vec5)(hardwarenumber);
  return x;
}

//NO CHANGE
Int_t HRichPedestalUpdate::getRow(Int_t hardwarenumber){
  fMappingPar=gHades->getRuntimeDb()->getContainer("RichMappingParameters");
  vec6 = ((HRichMappingPar*)fMappingPar)->padsy;
  Int_t y = (int) (*vec6)(hardwarenumber);
  return y;  
}


void HRichPedestalUpdate::calculateShifts(){
  // Int_t wait; // unused
  HLocation loc;
  HRichCalParCell *pCell=0;
  loc.setNIndex(3);
  Float_t countedFraction, newCoordinate;
  cout << "Calculating shift of mean values!" << endl;
  
  for(Int_t k =0; k<sectorMax; k++){
    Float_t multiply_sigma_orig = ( (HRichThresholdPar*)fThresholdPar )->getMultiply_sigma(k); //gamma
    Float_t multiply_mean_orig = ( (HRichThresholdPar*)fThresholdPar )->getMultiply_offset(k); //beta
    Float_t additional_offset_orig = ( (HRichThresholdPar*)fThresholdPar )->getShift(k); //alpha
    for (Int_t i = 0; i<colMax; i++){
      for (Int_t j = 0; j<rowMax; j++){
	
	if(mask[k][0] == 1){//valid sectors
	  Int_t sIx = mask[k][1];
	  loc[0] = k;
	  loc[1] = j;
	  loc[2] = i;
	  pCell = (HRichCalParCell*) ((HRichCalPar*)getCalPar())->getSlot(loc);
	  
	  if (pCell) {
	    
	    //pCell = new(pCell) HRichCalParCell;
	    pCell->setSector(k);
	    pCell->setRow(j);
	    pCell->setCol(i);
	    pCell->setSlope(1);
	    if(pCell->getSigma()>0.0){
	      countedFraction=pFrac[sIx][i][j];
	      //handle extreme shifts outside the erftable loop
	      if(countedFraction>=0.999997)
		{
		    cout << "Number of detected noise events is " <<pStat[sIx][i][j] << endl;
		    cout << "Fraction of detected noise events is " <<countedFraction <<", assuming position -4.5 sigma" << endl;
		    cout << "Pad postion: Sector=" << k <<" Row=" << j << " Column=" << i <<" ."<<endl;
		    cout << endl;
		  newCoordinate = -4.5;
		}
	      else if(countedFraction<=0.000003)
		{
		    cout << "Number of detected noise events is " <<pStat[sIx][i][j] << endl;
		    cout << "Fraction of detected noise events is " <<countedFraction <<",  assuming position 4.5 sigma" << endl;
		    cout << "Pad postion: Sector=" << k <<" Row=" << j << " Column=" << i <<" ."<<endl;
		    cout << endl;
		  newCoordinate = 4.5;
		}
	      else{
		  //cout << "Number of detected noise events is " <<pStat[sIx][i][j] << endl;
		  //cout << "Fraction of detected noise events is " <<countedFraction << endl;
		  //cout << "Pad postion: Sector=" << k <<" Row=" << j << " Column=" << i <<" ."<<endl;
		  newCoordinate = getNewCoordinate(countedFraction);
		  //cout << "Returned new position is: " << newCoordinate <<endl;
		  //cout << endl; 
		  //cout << "sigma original is: " << pCell->getSigma() << endl;
		  //cin >> wait;
		//THIS IS WRONG!!!
		//newCoordinate=sqrt(2.0) * erfc(2.0*countedFraction-1.0);
	      }
	      pShift[sIx][i][j]= (multiply_sigma_orig-newCoordinate) * pCell->getSigma() + pCell->getOffset() * (multiply_mean_orig - 1. ) + additional_offset_orig;
	      cout << "Calculated absoulte shift is: " << pShift[sIx][i][j] << endl;
	      if(pShift[sIx][i][j]>0)
	      {
		  //cout << "positive shift: " << pShift[sIx][i][j] << endl;
		  //cout << "sector: " << sIx << endl;
		  //cout << "col: " << i << endl;
		  //cout << "row: " << j << endl;
		  //cin >> wait;
	      }
	      if(pShift[sIx][i][j]<-1)
	      {
		  cout << "huge negative shift: " << pShift[sIx][i][j] << endl;
		  cout << "sector: " << sIx << endl;
		  cout << "col: " << i << endl;
		  cout << "row: " << j << endl;
		  cout << "original sigma: " <<pCell->getSigma() << endl;
		  cout << "original mean: " <<pCell->getOffset() << endl;
		  cout << "number of firings" <<pStat[sIx][i][j] << endl;
		  cout << "fraction of firings" <<pFrac[sIx][i][j] << endl;
		  cout << "MEASURED gamma"  << newCoordinate<< endl;
		  //cin >> wait;
	      }
	      if(pShift[sIx][i][j]<-1) pShift[sIx][i][j]=-1; //limit the down shift artificially!
	    } // if getsigma > 0
	  } //if pCell
	}// if mask
      }// row
    }// col
  }// sector
}

void HRichPedestalUpdate::calculate(){
    Int_t npads=0;
  HRichRaw *pRaw=NULL;
    Bool_t kData = kFALSE;
    pRichRaw=gHades->getCurrentEvent()->getCategory(catRichRaw);
    //cout << "number of entries : " << pRichRaw->getEntries() << endl;

    pIter->Reset();
    while( (pRaw = (HRichRaw *)pIter->Next()) ){
	kData = kTRUE;
	npads++;
	Int_t sec = pRaw->getSector();
	Float_t fCharge = pRaw->getCharge();
	Int_t iRow = pRaw->getRow();
	Int_t iCol = pRaw->getCol();

	if(mask[sec][0]==1){
	    if(iRow < rowMax  && iCol < colMax){
	 Int_t sIx = mask[sec][1];
		pStat[sIx][iCol][iRow] = pStat[sIx][iCol][iRow]+1;
	    }else{
		cout<<"Warning: impossible Pad numbers: row: "
		    <<iRow<<"  col: "<<iCol<<"   charge: "<<fCharge<<endl;
	    }   
	}	
    }
    //cout << "unpacked " << npads << endl;
    if (kData) nCounter++;
}

void HRichPedestalUpdate::outputFile(){

  Char_t fileName[128];
    const Char_t * extPed = "ped_update";
    const Char_t * extRoot = ".root";

    strcpy(fileName,pedPathName.Data());
    strcat(fileName,"_");
    strcat(fileName,extPed);
    strcat(fileName,extRoot);

    dataFile = new TFile(fileName, "RECREATE");

    for (Int_t i = 0; i<sectorMax; i++){
	
	if (mask[i][0] == 1){

	  meanHisto[i]->Write();
	  mean1dHisto[i]->Write();
	  sigmaHisto[i]->Write();
	  sigma1dHisto[i]->Write();  
	  statHisto[i]->Write();	    
	  driftHisto[i]->Write();
			    
	}
    }
    dataFile->Close();
    cout<<"Histos are written to: "<<fileName<<endl;
}

//CHANGED
Bool_t HRichPedestalUpdate::init() {
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

	initMappingPar();
	cout << "calling init calpar during init function"<<endl;
	initThresholdPar();
	initCalPar();
	initializeErfTable();
      	nCounter = 0;
    }
    return kTRUE;
    
}

Int_t HRichPedestalUpdate::execute() {
    
  if (checkInput){    
    calculate();  // mean and sigma  
  }
  return kTRUE;
}

//CHANGED!!!!!!!!!!!!!!!!!
Bool_t HRichPedestalUpdate::finalize(void) {
    
  if (checkInput){

    normalizeStatistics();      
    calculateShifts();
    fillCalParCntr();

    fill2DHistos();
    if(normalizedSums) fill1DHistos();
    createPedestal();
    createCalibration();

    outputFile();
  }
  return kTRUE;
}

//NO CHANGE
void HRichPedestalUpdate::initCalPar() {
  HRuntimeDb* rtdb=gHades->getRuntimeDb();
  fCalPar = rtdb->getContainer("RichCalPar");
  cout << "calpar container made" << endl;
}

void HRichPedestalUpdate::initThresholdPar() {
  HRuntimeDb* rtdb=gHades->getRuntimeDb();
  fThresholdPar = rtdb->getContainer("RichThresholdPar");
  cout << "threshold parameter container made!" << endl;
}

//NO CHANGE
void HRichPedestalUpdate::initMappingPar() {
  HRuntimeDb* rtdb=gHades->getRuntimeDb();
  fMappingPar = rtdb->getContainer("RichMappingParameters");
}

//CHANGE!!!!!!!!!!!!!!!!!!!!
Bool_t HRichPedestalUpdate::fillCalParCntr(){
  
  HLocation loc;
  HRichCalParCell *pCell=0;
  loc.setNIndex(3);
  Int_t n=0;
  
  for(Int_t k =0; k<sectorMax; k++){
    for (Int_t i = 0; i<colMax; i++){
      for (Int_t j = 0; j<rowMax; j++){
	
	if(mask[k][0] == 1){//valid sectors
	  // cout<<"valid Sector "<<k<<endl;
	  Int_t sIx = mask[k][1];
	  loc[0] = k;
	  loc[1] = j;
	  loc[2] = i;
	  pCell = (HRichCalParCell*) ((HRichCalPar*)getCalPar())->getSlot(loc);
	  
	  if (pCell) {
	    
	    //pCell = new(pCell) HRichCalParCell;
	    pCell->setSector(k);
	    pCell->setRow(j);
	    pCell->setCol(i);
	    pCell->setSlope(1);
	    if(pCell->getSigma()>0.0){
	      if(pCell->getOffset()+pShift[sIx][i][j]<0.0)
		{
		  driftHisto[k]->Fill(i,j, -pCell->getOffset());
		  cout <<"Warning, mean is set to 0 in sector: " << k << " row: " << j << " col: " << i  << endl;	  
		  cout <<"Original offset was: " <<pCell->getOffset() <<endl;
		  cout <<"calculated shift is: " << pShift[sIx][i][j] << endl;
		  pCell->setOffset(0.0);
		}
	      if(pCell->getOffset()+pShift[sIx][i][j]>1023){
		driftHisto[k]->Fill(i,j, 1023-pCell->getOffset());
		pCell->setOffset(1023);
		cout << "Mean is set to 1023 in sector: " << k << " row: " << j << " col: " << i  << endl;
	      }

	      if(pCell->getOffset()+pShift[sIx][i][j]>0.0 && pCell->getOffset()+pShift[sIx][i][j]<1023){
		driftHisto[k]->Fill(i,j, pShift[sIx][i][j]);
		pCell->setOffset( pCell->getOffset() + pShift[sIx][i][j]);
	      }
	      pCell->setSigma(pCell->getSigma());      
	      n++;
	    }else{ //if getSigma!>0
	      pCell->setOffset( pCell->getOffset());
	      pCell->setSigma(pCell->getSigma());
	    }
	  }else{
	    cerr<<"Error in HRichPedestalUpdate::fillCalParCntr"<<endl;
	    Error("fillCalParCntr(void)",
		  "slot not found:  %i %i %i",loc[0],loc[1],loc[2]);
	    return kFALSE;
	  }
	}//if mask
      }//for row
    }//for col
    //	cout<<"Number of CalParCells allocated : "<<n<<endl;
    n=0;
  }//for sector
  
  return kTRUE;
}

//NEW
Float_t HRichPedestalUpdate::getNewCoordinate(Float_t countedFraction)
{
  Int_t i=0;
  Float_t missingFraction = 1.0-countedFraction;
  Float_t newCoordinate;

  while( i<901 && (gaussInt[i] < missingFraction && gaussInt[i+1] < missingFraction) )
    {
      //We are in a bin of our tabulated function which is below the one we are searching.
      i++;
    }
  if(i<=901)
    {
      //newCoordinate=coordinate[i] + (missingFraction-gaussInt[i]) * ( (coordinate[i+1]-coordinate[i]) / (gaussInt[i+1]-gaussInt[i]) );
      newCoordinate = (coordinate[i]+coordinate[i+1])/2.0;
      return newCoordinate;
    }else{
      cout <<"error in calculation of mean-shift! No value of the tabulated error \
              function corresponding to the given fraction" << endl;
      exit(1);
    }
}

//NO CHANGE
void HRichPedestalUpdate::printCalParCntr(){
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

//CHANGED!!!!!!!!!!!!!!!!!!!!!!!
void HRichPedestalUpdate::createPedestal(){
  Char_t pedestalFile[128];      // filename and path!
  const Char_t * appSec[6] = {"1","2","3","4","5","6"};
       
    Int_t rc,tmp,port,mod,ch,x,y;
    rc=tmp=port=mod=ch=x=y=0;

    Int_t iCount , hardware;
    iCount=hardware=0;

    HRuntimeDb* rtdb=gHades->getRuntimeDb();
    fMappingPar = rtdb->getContainer("RichMappingParameters");
    TVector *pUncVec = ((HRichMappingPar*)fMappingPar)->uncuip;
    vec3 = ((HRichMappingPar*)fMappingPar)->uiplog;

    HLocation loc;
    HRichCalParCell *calparcell = NULL;
    loc.setNIndex(3);
        
    
    for (Int_t i = 0; i<sectorMax; i++){  // max. 6 sectors
	
	if(mask[i][0] == 1){ 

	  //int sIx = mask[i][1];
	    
	    strcpy(pedestalFile,pedPathName.Data());
	    strcat(pedestalFile,"_update_");
	    
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
			  loc[0] = i;
			  loc[1] = y;
			  loc[2] = x; 
			  
			  calparcell = (HRichCalParCell*) ((HRichCalPar*)fCalPar)->getObject(loc);
			  
			  pedestalOut<<rc<<" "<<port<<" "<<mod
				     <<" "<<ch<<"  "<< calparcell->getOffset()
				     <<"  "<<calparcell->getSigma()<<endl;
			  
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
}

//CHANGED!!!!!!!!!!!!!!!!!!!!!!!!!!!
void HRichPedestalUpdate::createCalibration(){

    const Char_t * extPed = ".txt";

    Char_t calFile[128];       // filename and path!
    strcpy(calFile,pedPathName.Data());
    strcat(calFile,"_update_cal");
    strcat(calFile,extPed);
    //cout<<calFile<<endl;
    cout<<"--------------------------------------------"<<endl;
    cout<<"calibration file written: "<<endl;
    cout<< "fileName: "<<calFile<<endl;

    HLocation loc;
    HRichCalParCell *calparcell = NULL;
    loc.setNIndex(3);
    
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
		  loc[0] = k;
		  loc[1] = j;
		  loc[2] = i;
		  calparcell = (HRichCalParCell*) ((HRichCalPar*)getCalPar())->getSlot(loc);
		  calOut<<j<<"	"<<i<<"	1"<<"	"<<calparcell->getOffset()<<"	"<< calparcell->getSigma() <<endl;
		}else     calOut<<j<<"	"<<i<<"	1"<<"	"<<"0"<<"	"<<"0"<<endl; // zero calibration parameters for nonactive sectors !
	    }
	}	
	calOut<<endl;
    }
    calOut.close();
}
 














