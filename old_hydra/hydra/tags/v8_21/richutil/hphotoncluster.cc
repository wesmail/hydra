// File: hphotoncluster.cc
//
// Author: Laura Fabbietti <L.Fabbietti@physik.tu-muenchen.de>
// Last update by Laura Fabbietti: 02/08/09 17:34:02
//

#pragma implementation

#include <iostream> 
#include <iomanip>
#include <stdlib.h>
#include "hphotoncluster.h"
#include "htool.h"

using namespace std;

ClassImp(HPhotonCluster)

void HPhotonCluster::clear(){
    iPadNr = 0;
    fThetaMax = 0;
    fPhiMax = 0;
    iClass = 0;
    iXDimension = 0;
    iYDimension = 0;
    fTotalCharge = 0;
    iSector = -1;
    fPads->Clear();
}
HPhotonCluster::HPhotonCluster(){
    fPads =  new TList;
}
HPhotonCluster::~HPhotonCluster(){
    fPads->Delete();
    delete fPads;
}
Bool_t HPhotonCluster::isInPadList(Int_t col, Int_t row){
    HRichCal *dummyPad =NULL;
    Int_t iCol,iRow;
    Int_t listSize = fPads->GetSize();

    for(Int_t i = 0;i<listSize;i++){
	dummyPad = (HRichCal*)fPads->At(i);
	iCol = dummyPad->getCol();
	iRow = dummyPad->getRow();
	if(iCol==col&&iRow==row) return kTRUE;
    }
    return kFALSE;
}
void HPhotonCluster::calculatemaxmin(){

    HRichCal * dummyPad = NULL;
    Int_t listSize = fPads->GetSize();
    Int_t * xPad = new Int_t[listSize];
    Int_t * yPad = new Int_t[listSize];
   
    for(Int_t i = 0;i<listSize;i++){
	dummyPad = (HRichCal*)fPads->At(i);
	xPad[i] = dummyPad->getCol();
	yPad[i] = dummyPad->getRow();
	//cout<<" pads charge "<<dummyPad->getCharge()<<endl;
	//cout<<"xPad[i]" <<xPad[i]<<endl;
	//cout<<"yPad[i]" <<yPad[i]<<endl;
    }
   
    Int_t * sortedxPad = new Int_t[listSize];
    Int_t * sortedyPad = new Int_t[listSize];
   
    HTool::sort(listSize,xPad ,sortedxPad ,kFALSE,kFALSE); //increasing order, not change directly
    HTool::sort(listSize,yPad ,sortedyPad ,kFALSE,kFALSE); //increasing order, not change directly
    
   for(Int_t i = 0;i<listSize;i++){
       //    cout<<" sorted index "<<sortedxPad[i]<<"  at i "<<i<<endl;
       //cout<<" x pad "<<xPad[sortedxPad[i]]<<"  at i "<<i<<endl;
       //cout<<" y pad "<<yPad[sortedyPad[i]]<<"  at i "<<i<<endl;
 
   }
    iXDimension = TMath::Abs(xPad[sortedxPad[0]] - xPad[sortedxPad[listSize-1]])+1;
    iYDimension = TMath::Abs(yPad[sortedyPad[0]] - yPad[sortedyPad[listSize-1]])+1;
   
    //cout<<" xDim "<<iXDimension<<"  yDim  "<<iYDimension<<endl;
    delete [] xPad;
    delete [] yPad;
    delete [] sortedxPad;
    delete [] sortedyPad;
}
void HPhotonCluster::setfPhiMaxThetaMax(HRichGeometryPar* pGeomPar){
    HRichCal * dummyPad = NULL;
    Int_t sector = 0;
    dummyPad = (HRichCal*)fPads->At(0);
    sector = dummyPad->getSector();
    fThetaMax = ((HRichGeometryPar*) pGeomPar) -> 
    getPadsPar()->getPad(dummyPad->getCol(),dummyPad->getRow())->getTheta();
    fPhiMax =((HRichGeometryPar*) pGeomPar) -> 
    getPadsPar()->getPad(dummyPad->getCol(),dummyPad->getRow())->getPhi(sector);
    
}

