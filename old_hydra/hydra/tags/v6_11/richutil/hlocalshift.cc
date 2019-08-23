#include "hlocalshift.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hrichdetector.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hmatrixcatiter.h"
#include "hlinearcatiter.h"
#include "hlocation.h"
#include "hrichcal.h"
#include "hrichpad.h"
#include "hrichlocal.h"
#include "hdebug.h"
#include "hades.h"
#include "richdef.h"
#include "hrichgeometrypar.h"
#include "hrichlocalmaxheader.h"
#include "hruntimedb.h"
#include "hrichlocalmaxcal.h"
ClassImp(HLocalShift)

HLocalShift::HLocalShift(Text_t *name, Text_t *title,Float_t shiftThetaMgF2[6],Float_t shifThetaSiO2[6],Char_t energyFileName[128]) : HReconstructor(name,title) {
    strcpy(filename, energyFileName);
    for(Int_t i = 0;i<6;i++){
	
	sThetaMgF2[i] = shiftThetaMgF2[i];
	sThetaSiO2[i] = shifThetaSiO2[i];
    }
}
HLocalShift::~HLocalShift(void){
}
Bool_t HLocalShift::init() {
 printf("initialization of rich localmaxcal\n");
    HRichDetector *pRichDet = (HRichDetector*) gHades -> getSetup() ->
	getDetector("Rich");
    fLocalCat = gHades -> getCurrentEvent() -> getCategory(catRichLocal);
    if (!fLocalCat) {
	fLocalCat = pRichDet -> buildCategory(catRichLocal);
	if (!fLocalCat) return kFALSE;
	else gHades -> getCurrentEvent() -> addCategory(catRichLocal,
							fLocalCat,"Rich");
    }
    fIter1 = (HMatrixCatIter*) getLocalCat() -> MakeIterator();
    HRuntimeDb* rtdb = gHades -> getRuntimeDb();
    HRichGeometryPar *pGeomPar = (HRichGeometryPar*) rtdb -> 
	getContainer("RichGeometryParameters");
    if (pGeomPar == NULL) {
	pGeomPar = new HRichGeometryPar;
	rtdb -> addContainer(pGeomPar);
    }
    setGeometryPar(pGeomPar);
    if (pGeomPar == NULL) return kFALSE;
    energyfile();
    return kTRUE;


}
Int_t HLocalShift::execute() {
    //    cout<<" event "<<endl;
    HRichLocal * pLocal = NULL;
    fIter1->Reset();
    while ((pLocal = (HRichLocal*) fIter1 -> Next())) {
		
	//cout<<" *****************************"<<endl;
	//cout<<" row OLD "<<pLocal->getRow()<<" col OLD "<<pLocal->getCol()<<endl;
	//cout<<" energy OLD "<<pLocal->getLocalEnergy()<<endl;
	//cout<<" *****************************"<<endl;
		shiftTheta(pLocal);
    }

    return 0;
}
void HLocalShift::shiftTheta(HRichLocal* pL){
    Float_t thetaIni =  pL->getLocalTheta();
    //cout<<" *************************************** "<<endl;
    //cout<<" theta ini "<<thetaIni<<endl;
    HRichPad *pPad = ((HRichGeometryPar*) fGeometryPar) -> 
	fPads.getPad(pL->getCol(),pL->getRow());
    HRichPad *padShift = pPad;
    //    cout<<"sThetaMgF2[pL->getSector()]  "<<sThetaMgF2[pL->getSector()]<<endl;
    if(thetaIni<54&&thetaIni>20){
	//cout<<" shifiting  "<<endl;
       while(TMath::Abs(padShift->getTheta()-thetaIni)<TMath::Abs(sThetaMgF2[pL->getSector()])){
	   
	    padShift = (((HRichGeometryPar*) fGeometryPar) ->getPadsPar())->getPad(padShift->getPadX(),padShift->getPadY()+TMath::Sign(-1.0,sThetaMgF2[pL->getSector()])*1);
	    //  cout<<" padShift->getPadX() "<<padShift->getPadX()<<"  padShift->getPadY()"<<padShift->getPadY()<<endl;
	    if((padShift->getTheta())<=20){
		
		break; 
	    }
	    //    cout<< " shifted Theta MgF2 "<<padShift->getTheta()<<endl;
       }
       recalculateLocal(padShift,pL);
    }
    else {
	//cout<<" shifiting  "<<endl;
	while(TMath::Abs(padShift->getTheta()-thetaIni)<TMath::Abs(sThetaSiO2[pL->getSector()])){
	    
	    padShift = (((HRichGeometryPar*) fGeometryPar) 
			->getPadsPar())->getPad(padShift->getPadX(),padShift->getPadY()+TMath::Sign(-1.0,sThetaSiO2[pL->getSector()])*1); 
	    //cout<<" pad out "<<padShift->getPadFlag()<<endl;
	    if((padShift->getPadFlag()) != 2 || padShift->getTheta()==0) {
		//		cout<<" pad out "<<endl; 
		padShift = (((HRichGeometryPar*) fGeometryPar) 
			    ->getPadsPar())->getPad(padShift->getPadX(),92);
		break; 
	    }
	    //cout<< " shifted Theta SiO2 "<<padShift->getTheta()<<endl;
	}
	recalculateLocal(padShift,pL);
    }
    
}
void HLocalShift::recalculateLocal(HRichPad * p,HRichLocal* pL){
    //    cout<<" in recalculateLocal  "<<endl;
    pL->setRow(p->getPadY());
    pL-> setLocalTheta(p->getTheta());
    pL-> setLocalPhi(p->getPhi(pL->getSector()));
    //cout<<" pL Theta "<<pL-> getLocalTheta()<<endl;
   
    //    pL-> setLocalEnergy(energyset(p->getPadX(),p->getPadY(),pL->getSector()));
    pL-> setLocalEnergy(energyset(pL->getRow(),pL->getCol(),pL->getSector()));
    // cout<<"energy of shifted pad  "<<pL->getLocalEnergy() <<"  sector  "<<pL->getSector()<<"  theta "<< pL->getLocalTheta()  <<endl;  
    //cout<<" row NEW "<<pL->getRow()<<" col NEW "<<pL->getCol()<<endl;
}
Bool_t HLocalShift::energyfile() {
    pEnergyLockup = new TFile(filename);
    for (Int_t iSector = 0; iSector < 6; iSector++) {
	sprintf(energyName,"pEnergySim%d",iSector);
	pEnergySim[iSector] = (TH2F*) pEnergyLockup -> Get(energyName);
    }
    return kTRUE;
}
Float_t HLocalShift::energyset(Int_t nRow, Int_t nCol, Int_t nSector) {
    Float_t energy = 0.;
    //getting the photonenergy for the pad out of the simulation
    if (pEnergySim[nSector] -> GetCellContent(nRow,nCol) != 0) {
	energy = pEnergySim[nSector] -> GetCellContent(nRow,nCol);
	//cout<<" ++++++++++++++++++++++++++++++++ "<<endl;
	//cout<<" energy from the file "<<energy<<endl;
	//cout<<" at col "<<nCol <<" and row :"<<nRow <<" sector "<<nSector<<endl;
	//cout<<" ++++++++++++++++++++++++++++++++ "<<endl;
	return energy;
    }
    //if there is no photonenergyinformation for this pad it is
    //calculated by the information out of the nightbourpads
    if ((pEnergySim[nSector] -> GetCellContent(nRow,nCol-1) != 0) &&
	(pEnergySim[nSector] -> GetCellContent(nRow,nCol+1) != 0)) {
	energy = (pEnergySim[nSector] -> GetCellContent(nRow,nCol-1) +
		  pEnergySim[nSector] -> GetCellContent(nRow,nCol+1)) / 2;
	return energy;
    }
    if ((pEnergySim[nSector] -> GetCellContent(nRow-1,nCol) != 0) &&
	(pEnergySim[nSector] -> GetCellContent(nRow+1,nCol) != 0)) {
	energy = (pEnergySim[nSector] -> GetCellContent(nRow-1,nCol) +
		  pEnergySim[nSector] -> GetCellContent(nRow+1,nCol)) / 2;
	return energy;
    }
    if (pEnergySim[nSector] -> GetCellContent(nRow,nCol-1) != 0) {
	energy = pEnergySim[nSector] -> GetCellContent(nRow,nCol-1);
	return energy;
    }
    if (pEnergySim[nSector] ->
 GetCellContent(nRow,nCol+1) != 0) {
	energy = pEnergySim[nSector] -> GetCellContent(nRow,nCol+1);
	return energy;
    }
    if ((pEnergySim[nSector] -> GetCellContent(nRow,nCol-2) != 0) &&
	(pEnergySim[nSector] -> GetCellContent(nRow,nCol+2) != 0)) {
	energy = (pEnergySim[nSector] -> GetCellContent(nRow,nCol-2) +
		  pEnergySim[nSector] -> GetCellContent(nRow,nCol+2)) / 2;
	return energy;
    }
    if ((pEnergySim[nSector] -> GetCellContent(nRow-1,nCol) != 0) &&
     	(pEnergySim[nSector] -> GetCellContent(nRow-2,nCol) != 0)) {
      	energy = 2 * pEnergySim[nSector] -> GetCellContent(nRow-1,nCol) -
      	    pEnergySim[nSector] -> GetCellContent(nRow-2,nCol);
	return energy;
    }
    if ((pEnergySim[nSector] -> GetCellContent(nRow+1,nCol) != 0) &&
     	(pEnergySim[nSector] -> GetCellContent(nRow+2,nCol) != 0)) {
      	energy = 2 * pEnergySim[nSector] -> GetCellContent(nRow+1,nCol) -
      	    pEnergySim[nSector] -> GetCellContent(nRow+2,nCol);
	return energy;
    }
    if (pEnergySim[nSector] -> GetCellContent(nRow,nCol+2) != 0) {
	energy = (pEnergySim[nSector] -> GetCellContent(nRow,nCol+2));
	return energy;
    }
    if (pEnergySim[nSector] -> GetCellContent(nRow,nCol-2) != 0) {
	energy = (pEnergySim[nSector] -> GetCellContent(nRow,nCol-2));
	return energy;
    }
    return 0.;
}
