// File: hclusterselector.cc
//
// Author: Laura Fabbietti <L.Fabbietti@physik.tu-muenchen.de>
// Last update by Laura Fabbietti: 02/10/10 13:14:01
//
// File: 
//hclusterselector.cc
// 
//
//*****************************************************************************

//*****************************************************************************
//
#include "hclusterselector.h"
#include "hruntimedb.h"
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
#include "hphotoncluster.h"
#include "hdebug.h"
#include "hades.h"
#include "richdef.h"
#include "hrichgeometrypar.h"
#include "hrichlocalmaxheader.h"


ClassImp(HClusterSelector)
    
HClusterSelector::HClusterSelector(Text_t *name, Text_t *title, 
				   Int_t hitMin, Int_t secShadow,
				   Float_t minTheta1, Float_t maxTheta1,
				   Float_t minTheta2, Float_t maxTheta2,
				   Int_t secEdge1,
				   Int_t secEdge2,Int_t minHitsecEdge1,
				   Int_t minHitsecEdge2) :
HReconstructor(name,title) {
    edgesector1 = secEdge1;
    edgesector2 = secEdge2;
    minHitedgesector1 =minHitsecEdge1;
    minHitedgesector2 = minHitsecEdge2;
    minHit = hitMin;
    fIter = NULL;
    fIter1 = NULL;
    thetaMin1 = minTheta1;
    thetaMax1 = maxTheta1;
    thetaMin2 = minTheta2;
    thetaMax2 = maxTheta2;
    eventNr = 1;
    shadowSector = secShadow;
    padCounter = 0;

}
HClusterSelector::~HClusterSelector(void) {
    if (fIter) delete fIter;
    if (fIter1) delete fIter1;
    if(pLeftBorder) delete [] pLeftBorder;
    if(pRightBorder) delete [] pRightBorder;
}
Bool_t HClusterSelector::init() {
    printf("initialization of rich localmaxcal\n");
    HRichDetector *pRichDet = (HRichDetector*) gHades -> getSetup() ->
	getDetector("Rich");

    fLocalCatHr=gHades->getCurrentEvent()->getCategory(catRichLocalMaxHeader);
    if (!fLocalCatHr) {
	fLocalCatHr=pRichDet->buildCategory(catRichLocalMaxHeader);

	if (!fLocalCatHr) return kFALSE;
	else gHades->getCurrentEvent()
		 ->addCategory(catRichLocalMaxHeader,fLocalCatHr, "Rich");
    }
    fCalCat = gHades -> getCurrentEvent() -> getCategory(catRichCal);
    if (!fCalCat) {
	fCalCat = pRichDet -> buildCategory(catRichCal);
	if (!fCalCat) return kFALSE;
	else gHades -> getCurrentEvent() -> addCategory(catRichCal,
							fCalCat,"Rich");
    }
    fIter = (HMatrixCatIter*) getCalCat() -> MakeIterator();
     fPhotClusCat= gHades -> getCurrentEvent() -> getCategory(catRichPhotClus);
    if (!fPhotClusCat) {
	 fPhotClusCat= pRichDet -> buildCategory(catRichPhotClus);
	if (!fPhotClusCat) return kFALSE;
	else gHades -> getCurrentEvent() -> addCategory(catRichPhotClus,
							 fPhotClusCat ,"Rich");
    }
    fIter1 = (HMatrixCatIter*) getPhotClusCat() -> MakeIterator();

    HRuntimeDb* rtdb = gHades -> getRuntimeDb();
    pGeomPar = (HRichGeometryPar*) rtdb -> 
	getContainer("RichGeometryParameters");
    if (pGeomPar == NULL) {
	pGeomPar = new HRichGeometryPar;
	rtdb -> addContainer(pGeomPar);
    }
    setGeometryPar(pGeomPar);
    if (pGeomPar == NULL) return kFALSE;

    tCharge = new TNtuple("tCharge","tCharge","padNr:q0:q1:q2:q3:q4:q5:q6:q7:q8:qSum:iSec:iClass:theta:phi"); 
    tCharge1 = new TNtuple("tCharge1","tCharge1","padNr:q0:q1:q2:q3:q4:q5:q6:q7:q8:qSum:iSec:iClass:xDim:yDim"); 
    tCharge -> SetAutoSave();
    tCharge1 -> SetAutoSave();

    return kTRUE;
}
Bool_t HClusterSelector::reinit(){
     // maxCols = pGeomPar->getColumns();
    maxCols = 92;
    maxRows = 90;
    //    cout<<maxCols<<"  "<<maxRows<<endl;
    if(pLeftBorder) delete [] pLeftBorder;
    pLeftBorder = new short[maxRows];
    if(pRightBorder) delete [] pRightBorder;
    pRightBorder = new short[maxRows];
    
    for (int row=0;row<maxRows;++row) {
	Int_t col=0;
	Int_t padOffset = row*maxCols;
	while(!pGeomPar->fPads.getPad(col+padOffset)->getPadActive() && col<maxCols) ++col;
	if(col==maxCols) {
	    maxRows=row;
	    break;
	}
	pLeftBorder[row]=col;
	while(pGeomPar->fPads.getPad(col+padOffset)->getPadActive() && col<maxCols) ++col;
	pRightBorder[row]=col-1;
	//    cout<<"row="<<row<<" min,max="<<pLeftBorder[row]<<", "<<pRightBorder[row]<<"\n";
    }
    cout<<maxCols<<"  "<<maxRows<<endl;
    return kTRUE;
}
Bool_t HClusterSelector::finalize() {
    tCharge -> Write();
    tCharge1 -> Write();
    return kTRUE;
}

Int_t HClusterSelector::execute() {
    //    cout<<" executing hclusterselector  "<<endl;
    //resets the matrixes
    resetmatrix();
    HRichCal *pCal;
    HLocation loc;
    fIter -> Reset();
    count1 = count2 = 0;
    fTheta1 = fTheta2 = 0;
    padCounter = 0;
    //fill the matrixes with data from cal container
    while ((pCal = (HRichCal *) fIter -> Next())) {
	loc = fIter -> getLocation();
	fillmatrix(loc);
	padCounter++;
    }
    //if the counter i == 0 there is no event copied to the matrix 
    //cal must be empty!!!
    if ( padCounter== 0) {
	cout<<"fillmatrix false !!!"<<endl;
    }
    //counts the hits in each sector and looks for for afterburning hits
    hitcontrol(); 
    count1 = count2 = 0;
    fTheta1 = fTheta2 = 0.;
    if (hitControl == kTRUE) {
	//calculates the local maxima for a group of 9 or 5 pads
	//theta and phi angle is not longer the centre of one pad
	//safes the data to local container
	//the charge of the local group of pads is added to the central pad
	    for(Int_t i =0;i<6;i++){
		formCluster(i);
	    }	
	    //	    cout<<" all cluster formed "<<endl;    
	    updateHeaders(fTheta1/count1,fTheta2/count2,1,NULL,NULL,eventNr);

	    
	}///remove me

    
    eventNr++;
    return 0;
}
void HClusterSelector::formCluster(Int_t secNum){
    HPhotonCluster * photClus = NULL;
     HLocation fLoc;
     fLoc.set(1,0);
     //cout<<" **************************************"<<endl;

     //     cout<<" ******************* new pad   in sector  ************"<<secNum<<endl;
     //cout<<" **************************************"<<endl;
    for(int row=0;row<maxRows;++row) {
	for(int col=0;col<maxCols;++col) {
	    if(mPadLock[secNum][row][col]== 1&&mCharge[secNum][row][col]>0){
		//	cout<<" ******************* new pad   in sector  ************"<<secNum<<endl;
		
		photClus = ((HPhotonCluster*) (fPhotClusCat) -> getNewSlot(fLoc));
		photClus = new(photClus) HPhotonCluster;

		padTotNr = 0;
		chargeTot = 0.;
		chainPads(secNum,row,col,photClus);

		photClus->setiPadNr(padTotNr);
		photClus->setSector(secNum);
		photClus->setfTotalCharge(chargeTot);
		calcClusProp(photClus);
	    }
    }
    }
}

void HClusterSelector::chainPads(Int_t iSec,Int_t iRow,Int_t iCol,HPhotonCluster* pClus){
    //for each fired pad it is checked if the 4 (up-down-sides) neighbours have fired, as long as no fired neighbours is left. IN this way the cluster is built, while collecting the pads belongning to the clusters the pClus is updated.
    
    HLocation loc;
 for(Int_t m =-1;m<2;m++){
	for(Int_t n =-1;n<2;n++){
	    if(m*n==0){
		//cout<<"charge "<<mCharge[iSec][iRow+m][iCol+n]<< "iRow+ m "<<iRow+m<<" jCol+ n "<<iCol+n<<endl;
		//checks if the pad is fired and if it has already beeen included in the cluster (lock=-1 in this case), if the fired pad doesnt yet belong to the cluster, it is added.
		if(mCharge[iSec][iRow+m][iCol+n]>0&&mPadLock[iSec][iRow+m][iCol+n]==1){
		 
		    // cout<<"charge "<<mCharge[iSec][iRow+m][iCol+n]<< "iRow+ m "<<iRow+m<<" jCol+ n "<<iCol+n<<endl;
		    mPadLock[iSec][iRow+m][iCol+n]=-1;
		    padTotNr++;
		    chargeTot += mCharge[iSec][iRow+m][iCol+n];
		   
		    //cout<<" cal obj "<<(HRichCal*)(((HMatrixCategory*)getCalCat())->getObject(loc))<<endl;
		    loc.set(3,iSec,iRow+m,iCol+n);
		    if((HRichCal*)(((HMatrixCategory*)getCalCat())->getObject(loc)))
  		    pClus->addToTheList((HRichCal*)(((HMatrixCategory*)getCalCat())->getObject(loc)));
		    chainPads(iSec,iRow+m,iCol+n,pClus);
		}

	    }
	}
 }
 /// cout<<" out of the loop "<<pClus<<" e i pads "<<padTotNr<<" e la carica "<<chargeTot<<endl;
}
void HClusterSelector::calcClusProp(HPhotonCluster *pPhotClus){
   
    pPhotClus->sortListByCharge();
    pPhotClus->calculatemaxmin();
    pPhotClus->setfPhiMaxThetaMax(pGeomPar);
    Int_t nPad = pPhotClus->getiPadNr();
     if (nPad<3){
	pPhotClus->setiClass(1);
     }
    else {
	if(nPad==3){
	    if (isMaximumInCentre(pPhotClus)) pPhotClus->setiClass(1);
	    else pPhotClus->setiClass(2);
	}
	else {
	    if(nPad==4 && isMaximumInCentre(pPhotClus)&&
	       thereIsOnlyOneMax(pPhotClus)
	       && pPhotClus->getiXDimension()==2
	       &&pPhotClus->getiYDimension()==3) pPhotClus->setiClass(1);
	    
	    else pPhotClus->setiClass(3);
	}
    }

     //The charge distribution of the pads around the local maxima is checked
     // only those pads belonging to the cluster are considered
 
     Int_t iRow = ((HRichCal*)pPhotClus->getPadInList(0))->getRow();
     Int_t iCol = ((HRichCal*)pPhotClus->getPadInList(0))->getCol();
     Int_t iSector = pPhotClus->getSector();
     Int_t k = 0;
     for (Int_t jRow = iRow-1; jRow < iRow+2; jRow++) {
	    for (Int_t jCol = iCol-1; jCol < iCol+2; jCol++) {
		
		fCharge[k] = 0.;
		if(pPhotClus->isInPadList(jCol,jRow)) fCharge[k] = mCharge[iSector][jRow][jCol];
		k++;
	    }
	}
    
     tCharge -> Fill(nPad,fCharge[0],fCharge[1],fCharge[2],
		     fCharge[3],fCharge[4],fCharge[5],fCharge[6],
		     fCharge[7],fCharge[8],pPhotClus->getfTotalCharge()
		     ,iSector,pPhotClus->getiClass(),
		     pPhotClus->getfThetaMax(),pPhotClus->getfPhiMax());
     tCharge1 -> Fill(nPad,fCharge[0],fCharge[1],fCharge[2],
		     fCharge[3],fCharge[4],fCharge[5],fCharge[6],
		     fCharge[7],fCharge[8],pPhotClus->getfTotalCharge()
		     ,iSector,pPhotClus->getiClass(),
		      pPhotClus->getiXDimension(),pPhotClus->getiYDimension());
     //    cout<<" cluster class "<<pPhotClus->getiClass()<<endl;
     //cout<<" col is  "<<iCol<<"  row is "<<
     //	 iRow<<" number of pads  "<<pPhotClus->getiPadNr()
     //<<endl;
     //cout<< " sector "<<pPhotClus->getSector()<<endl;
}

Bool_t HClusterSelector::thereIsOnlyOneMax(HPhotonCluster *pPhotC){
    Float_t chargeMax ;
    chargeMax = ((HRichCal *)pPhotC->getPadInList(0))->getCharge();
    if(chargeMax>(pPhotC->getfTotalCharge())*0.5)
	return kTRUE;
    else return kFALSE;
}

Bool_t HClusterSelector::isMaximumInCentre(HPhotonCluster *pPhotC){
    //cout<<" in isMaximumInCentre  "<<endl;
    HRichCal * dummy = NULL;
    dummy = pPhotC->getPadInList(0);
    Int_t rowOfmax = dummy->getRow();
    Int_t colOfmax = dummy->getCol();
    Int_t secOfmax = dummy->getSector();
    Int_t countNeig = 0;
    for(Int_t m =-1;m<2;m++){
	for(Int_t n =-1;n<2;n++){
	    if(m*n==0){
		if(mCharge[secOfmax][rowOfmax+m][colOfmax+n]>0) countNeig++;
	    }
	}
    }
    //cout<<" countNeig "<<countNeig<<endl;
    if (countNeig>=3) return kTRUE;
    else return kFALSE;
}
Bool_t HClusterSelector::fillmatrix(HLocation& fLoc) {
    //writes hit and charge from cal container for active pads into matrix
    //writes theta and phi from pad container for active pads into matrix
   
    HRichCal *cal = NULL;
    cal = (HRichCal*) fCalCat -> getObject(fLoc);
    iCol = cal -> getCol();
    iRow = cal -> getRow();
    iSector = cal -> getSector();
    HRichPad *pad = ((HRichGeometryPar*) pGeomPar) -> 
	fPads.getPad(iCol,iRow);
    //cout<<" in fill matrix pad : "<<iCol<<",  "<<iRow<<endl;
    if (cal) {
	mHit[iSector][iRow][iCol] = 1;
	mPadLock[iSector][iRow][iCol] = 1;
	mCharge[iSector][iRow][iCol] = cal -> getCharge();
	meanCharge[iSector]= meanCharge[iSector]+mCharge[iSector][iRow][iCol];
	//mTheta,mPhi are used to calculate the polar and azymutal angle of the photon clusters.
	mTheta[iSector][iRow][iCol] = pad -> getTheta();
	mPhi[iSector][iRow][iCol] = pad -> getPhi(iSector);
	//cout<<" charge  "<<mCharge[iSector][iRow][iCol]<<"   theta  "<<mTheta[iSector][iRow][iCol]<< "  phi  "<<mPhi[iSector][iRow][iCol]<<endl;
	if ((pad -> getTheta())>thetaMin1&& (pad -> getTheta())<thetaMax1) {
	    fTheta1 +=mTheta[iSector][iRow][iCol];
	    count1++;
	    //	    cout<<" fTheta1 "<<fTheta1<<"  count1 "<<count1<<endl;
	}
	else if ((pad -> getTheta())>thetaMin2&&(pad -> getTheta())<thetaMax2){
	    fTheta2 +=mTheta[iSector][iRow][iCol];
	    count2++;
	    //  cout<<" fTheta2 "<<fTheta2<<"  count2 "<<count2<<endl;
	}
	
    }
    return kTRUE;
}

Bool_t HClusterSelector::resetmatrix() {
    for (iSector = 0; iSector < 6; iSector++) {
	meanCharge[iSector]=0;
	for (iRow = 0; iRow < maxRows; iRow++) {
	    for (iCol = 0; iCol < maxCols; iCol++) {
		mPadLock[iSector][iRow][iCol] = 0;
		mHit[iSector][iRow][iCol] = 0;
		mCharge[iSector][iRow][iCol] = 0.;
		mTheta[iSector][iRow][iCol] = 0.;
		mPhi[iSector][iRow][iCol] = 0.;
		mHit2[iSector][iRow][iCol] = 0;
		
	    }
	}
    }
    return kTRUE;
}
Bool_t HClusterSelector::hitcontrol() {
    for (iSector = 0; iSector < 6; iSector++) {
	sumHit[iSector] = 0;
	doubleHit[iSector] = 0;
	//counts hits in each sector
	for (iRow = 0; iRow < 90; iRow++) {
	    for (iCol = 0; iCol < 92; iCol++) {
		if (mHit[iSector][iRow][iCol] == 1) {
		    sumHit[iSector]++;
		    //counts the hits were pads where active in the previous evt.
		    if (mHit2[iSector][iRow][iCol] == 1) {
			doubleHit[iSector]++;
		    }
		}
		mHit2[iSector][iRow][iCol] = mHit[iSector][iRow][iCol];
	    }
	}
	if(sumHit[iSector])   meanCharge[iSector] = meanCharge[iSector]/sumHit[iSector];
    }
    //selects events with more than half afterburning pads in one sector
   
  Int_t  i = 0;
    hitControl = kFALSE;
    //the event is rejected if there is an after burning sector with an hit multiplicity higher than the threshold set in the macro. The shadow sector is excluded from the selection.
    for (iSector = 0; iSector < 6; iSector++) {
	if ((doubleHit[iSector] <= sumHit[iSector] / 2 && sumHit[iSector]>minHit) || 
	    (iSector == shadowSector)) {//$$
	    
	    i++;
	}
    }
    //cout<<" index "<<i<<" sumHit[0]  "<<sumHit[0]  <<" sumHit[1]  "<<sumHit[1]<<" sumHit[2]  "<<sumHit[2]<<" sumHit[3]  "<<sumHit[3] <<" sumHit[4]  "<<sumHit[4]<<" sumHit[5]  "<<sumHit[5]<<endl;

    //cout<<" ************************************* "<<endl;
    //cout<<" edgesector1  "<< edgesector1 <<"  minHitedgesector1  "<<
    //minHitedgesector1<< "  edgesector2  " <<edgesector2<<
    //"  minHitedgesector2 "<<minHitedgesector2<<endl;
    if (i >=3 && sumHit[edgesector1]>minHitedgesector1&&
	sumHit[edgesector2]>minHitedgesector2 ) {
        hitControl = kTRUE;
	//	cout<<" update headers "<<endl;
	updateHeaders(fTheta1/count1,fTheta2/count2,1,sumHit,meanCharge,eventNr);
    }
    else {
	//cout<<"doublehit false in Sector: "<<endl;
	updateHeaders(0,0,0,sumHit,meanCharge,eventNr);
    }
    //cout<<"hitControl: "<<hitControl<<endl;
    return kTRUE;
}
void HClusterSelector::updateHeaders(Float_t t1, Float_t t2,Int_t i,Float_t *secM,Float_t *meanCS,Int_t evtNum){
    HRichLocalMaxHeader *localHr = NULL;
    HLocation loc;
    loc.set(1,0);
    localHr = (HRichLocalMaxHeader*)fLocalCatHr->getObject(loc);
    // cout<<" localHr "<<localHr<<endl;
    if(localHr==NULL){
	localHr = (HRichLocalMaxHeader*)fLocalCatHr->getSlot(loc);
	
	if(localHr!=NULL){
	    localHr = new(localHr) HRichLocalMaxHeader;
	    localHr->setEventFlag(i);
	    localHr->setMeanThetaPad1(t1);
	    localHr->setMeanThetaPad2(t2);
	    localHr->setSecMult(secM);
	    localHr->setMeanChargeSec(meanCS);
	    localHr->setEvtNum(evtNum);
	}
    }
    else{
	//	cout<<"update local maxima "<<endl;
	localHr->setMeanThetaLoc1(t1);
	localHr->setMeanThetaLoc2(t2);
    }

}


Bool_t HClusterSelector::multihit(Int_t nRow, Int_t nCol, Int_t nSector,
				  HLocation& fLoc) {
    //this memberfunction is required for hrichlocalmaxsim class
    return kTRUE;
}











