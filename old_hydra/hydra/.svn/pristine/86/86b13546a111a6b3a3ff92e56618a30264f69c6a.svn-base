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
#include "hrichmappingpar.h"
#include <TFile.h>
#include <string.h>

ClassImp(HRichPedestalUpdate)
    
HRichPedestalUpdate::HRichPedestalUpdate(Text_t *name, Text_t *title,
		  char * path ,char * pString, float alpha_orig): HReconstructor(name,title) {    
    
    pedPathName = path;

    alphaorig=alpha_orig;
    
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

//NO CHANGE
HRichPedestalUpdate::~HRichPedestalUpdate(void) {
  
  deleteSums();    
}

//NO CHANGE
void HRichPedestalUpdate::createSums(){
  
  pStat = new int[numSec][96][96];   
  pFrac = new float[numSec][96][96];
}

//NEW
void HRichPedestalUpdate::initializeErfTable(){
  Int_t i;
    
  for(i=0;i<601;i++)
    {
      
      alpha[i]= -4.0 + (float)i * 6.0/600.0 ;
      phi[i]=0.5*(1.0 - erf(alpha[i]/sqrt(2)) );
    }
}

//NO CHANGE
void HRichPedestalUpdate::initializeSums(){
    
  for (int i=0;i<numSec;i++){
    
    for (int j=0; j<colMax; j++){
      
      for (int k=0; k<rowMax; k++){
	
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

//NO CHANGE
void HRichPedestalUpdate::fillMask(){
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

//NO CHANGE
void HRichPedestalUpdate::bookHistos(){

    char * Drift[6];
    char * stat[6];

    char temp1[7];
    char temp2[7];

    for (int i = 0; i<sectorMax; i++){
      
      sprintf(temp1,"MeanDrift_%d",i+1);
      sprintf(temp2,"stat_%d",i+1);
      Drift[i] = temp1;     	
      stat[i] = temp2;

      driftHisto[i] = new TH2F (Drift[i], Drift[i], colMax,1,colMax,rowMax,1,rowMax);
      statHisto[i] = new TH2F (stat[i], stat[i], colMax,1,colMax,rowMax,1,rowMax);
    }
    
}

void HRichPedestalUpdate::normalizeStatistics()
{
  int sec, col, row;
  for(sec=0;sec<sectorMax;sec++)
    {
      if (mask[sec][0]==1){ 
	for(row=0;row<rowMax;row++)
	  {
	    for(col=0;col<colMax;col++)
	      {
		int sIx=mask[sec][1];
		pFrac[sIx][col][row]=(float)pStat[sIx][col][row] / ((float) (nCounter*10));
	      }
	  }
      }
    }
}

//CHANGE !!!!!!!!!!!!!!!
void HRichPedestalUpdate::fill2DHistos(){
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
		}
	    }
	}
    }
    normalizedSums = true;
}

//NO CHANGE
int HRichPedestalUpdate::getCol(int hardwarenumber){
  fMappingPar = gHades->getRuntimeDb()->getContainer("RichMappingParameters");
  vec5 = ((HRichMappingPar*)fMappingPar)->padsy;
  int x = (int) (*vec5)(hardwarenumber);
  return x;
}

//NO CHANGE
int HRichPedestalUpdate::getRow(int hardwarenumber){
  fMappingPar=gHades->getRuntimeDb()->getContainer("RichMappingParameters");
  vec6 = ((HRichMappingPar*)fMappingPar)->uiplog;
  int y = (int) (*vec6)(hardwarenumber);
  return y;  
}



void HRichPedestalUpdate::calculate(){

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
		pStat[sIx][iCol][iRow]= pStat[sIx][iCol][iRow]+1;
	    }else{
		cout<<"Warning: impossible Pad numbers: row: "
		    <<iRow<<"  col: "<<iCol<<"   charge: "<<fCharge<<endl;
	    }   
	}	
    }
    if (kData) nCounter++;
}

void HRichPedestalUpdate::outputFile(){

  char fileName[128];
    char * extPed = "ped_update";
    char * extRoot = ".root";

    strcpy(fileName,pedPathName);
    strcat(fileName,"_");
    strcat(fileName,extPed);
    strcat(fileName,extRoot);

    dataFile = new TFile(fileName, "RECREATE");

    for (int i = 0; i<sectorMax; i++){
	
	if (mask[i][0] == 1){
	    
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
	cout << " calling init calpar" << endl;
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
      
    fill2DHistos();
    createPedestal();
    createCalibration();
    normalizeStatistics();
    fillCalParCntr();
    outputFile();
  }
  return kTRUE;
}

//NO CHANGE
void HRichPedestalUpdate::initCalPar() {
  HRuntimeDb* rtdb=gHades->getRuntimeDb();
  fCalPar = rtdb->getContainer("RichCalPar");
  cout << "done" << endl;
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
  float fraction, alphaprime;
  Int_t n=0;
  Float_t shift;
  
  for(Int_t k =0; k<sectorMax; k++){
    for (Int_t i = 0; i<colMax; i++){
      for (Int_t j = 0; j<rowMax; j++){
	
	if(mask[k][0] == 1){//valid sectors
	  // cout<<"valid Sector "<<k<<endl;
	  int sIx = mask[k][1];
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
	      fraction=pFrac[sIx][i][j];
	      //handle extreme shifts outside the erftable loop
	      if(fraction>=0.9987)
		{
		  cout << "Fraction of detected noise events is almost 1.0, assuming position -3.0 sigma" << endl;
		  alphaprime = -3.0;
		}
	      else if(fraction<=0.0013)
		{
		  cout << "Fraction of detected noise events is almost 0.0, assuming position 3.0 sigma" << endl;
		  alphaprime = 3.0;
		}
	      else{
		alphaprime = getalpha(fraction);
	      }
	      shift=(alphaorig-alphaprime) * pCell->getSigma();
	      if(pCell->getOffset()+shift<0.0)
		{
		  pCell->setOffset(0.0);
		  cout << "Mean is set to 0 in sector: " << k << " row: " << j << " col: " << i  << endl;
		  driftHisto[k]->Fill(i,j, -pCell->getOffset());
		}
	      if(pCell->getOffset()+shift>1023){
		pCell->setOffset(1023);
		driftHisto[k]->Fill(i,j, 1023-pCell->getOffset());
		cout << "Mean is set to 1023 in sector: " << k << " row: " << j << " col: " << i  << endl;
	      }
	      if(pCell->getOffset()+shift>0.0 && pCell->getOffset()+shift<1023){
		pCell->setOffset( pCell->getOffset() + shift);
		driftHisto[k]->Fill(i,j, shift);
	      }
	      pCell->setSigma(pCell->getSigma());
	      n++;
	    }else{
	      pCell->setOffset( pCell->getOffset());
	      pCell->setSigma(pCell->getSigma());
	    }
	    
	  }else{
	    cerr<<"Error in HRichPedestalUpdate::fillCalParCntr"<<endl;
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
  
  return kTRUE;
}

//NEW
float HRichPedestalUpdate::getalpha(float fraction)
{
  int i=0;
  float newalpha;

  while( i<601 && !(phi[i]>fraction && phi[i+1]<fraction) )
    {
      i++;
    }
  if(i<=601)
    {
      newalpha=alpha[i] + (fraction-phi[i]) * ( (alpha[i+1]-alpha[i]) / (phi[i+1]-phi[i]) );
      return newalpha;
    }else{
      cout <<"error in calculation of mean-shift! No value of the tabulated error 
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
  char pedestalFile[128];      // filename and path!
    char * appSec[6] = {"1","2","3","4","5","6"};
       
    Int_t rc,tmp,port,mod,ch,x,y;
    rc=tmp=port=mod=ch=x=y=0;

    Int_t iCount , hardware;
    iCount=hardware=0;

    HRuntimeDb* rtdb=gHades->getRuntimeDb();
    fMappingPar = rtdb->getContainer("RichMappingParameters");
    TVector *pUncVec = ((HRichMappingPar*)fMappingPar)->uncuip;
    vec3 = ((HRichMappingPar*)fMappingPar)->padsx;

    HLocation loc;
    HRichCalParCell *calparcell = NULL;
    loc.setNIndex(3);
        
    
    for (Int_t i = 0; i<sectorMax; i++){  // max. 6 sectors
	
	if(mask[i][0] == 1){ 

	  //int sIx = mask[i][1];
	    
	    strcpy(pedestalFile,pedPathName);
	    strcat(pedestalFile,"_update");
	    
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
			  
			  if(mask[i][0] == 1){//valid sectors
			    
			    loc[0] = i;
			    loc[1] = y;
			    loc[2] = x; }
			  else{cout << "invalid sector information in writing calpar file" << endl;}
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

    char * extPed = ".txt";

    char calFile[128];       // filename and path!
    strcpy(calFile,pedPathName);
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
		  //int sIx = mask[k][1];
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
 














