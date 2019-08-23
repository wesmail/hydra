// File: hrichlocalmaxcal.cc
//
// 
//
//*****************************************************************************
// this class is build to analyse and select the big ring events from the 
// online efficiency mesurement (OEM) of the rich. It looks for photon hits 
// in cal container and safes them to lokal container. For a local maximum out
// of 5 or 9 neighbours pad it calculates the emphasisangles out of the charges
// and safes it together with the sum of the charges to the central pad. After 
// this the emphasisangles for each big ring is calculated out of the 
// emphasisangle for the photonhits. If the rings in one event is like the 
// specifications, a flag is set to local container. For each pad of one ring
// the energy of the pad out of the simulation with the same beamenergy and 
// particle is saved.
//*****************************************************************************
//
#include "hrichlocalmaxcal.h"
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
#include "hrichlocal.h"
#include "hdebug.h"
#include "hades.h"
#include "richdef.h"
#include "hrichgeometrypar.h"
#include "hrichlocalmaxheader.h"
ClassImp(HRichLocalMaxCal)
    
HRichLocalMaxCal::HRichLocalMaxCal(const Text_t *name,const Text_t *title, Int_t pads,
				   Int_t hitMin, Int_t sector,
				   Float_t minTheta1, Float_t maxTheta1,
				   Float_t minTheta2, Float_t maxTheta2,
				   const Char_t energyFileName[128],Int_t secEdge1,
				   Int_t secEdge2,Int_t minHitsecEdge1,
				   Int_t minHitsecEdge2) :
HReconstructor(name,title) {
    edgesector1 = secEdge1;
    edgesector2 = secEdge2;
    minHitedgesector1 =minHitsecEdge1;
    minHitedgesector2 = minHitsecEdge2;
    padNumber = pads;
    minHit = hitMin;
    fIter = NULL;
    fIter1 = NULL;
    thetaMin1 = minTheta1;
    thetaMax1 = maxTheta1;
    thetaMin2 = minTheta2;
    thetaMax2 = maxTheta2;
   
    shadowSector = sector;
    eventNr = 0;
    localEventNr = 1;
    strcpy(filename, energyFileName);
    i = 0;
    j = 0;
}
HRichLocalMaxCal::~HRichLocalMaxCal(void) {
    if (fIter) delete fIter;
    if (fIter1) delete fIter1;
}
Bool_t HRichLocalMaxCal::init() {
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
    fLocalCat = gHades -> getCurrentEvent() -> getCategory(catRichLocal);
    if (!fLocalCat) {
	fLocalCat = pRichDet -> buildCategory(catRichLocal);
	if (!fLocalCat) return kFALSE;
	else gHades -> getCurrentEvent() -> addCategory(catRichLocal,
							fLocalCat,"Rich");
    }
    fIter1 = (HMatrixCatIter*) getLocalCat() -> MakeIterator();
 
    tCharge = new TNtuple("tCharge","tCharge","padNr:q0:q1:q2:q3:q4:q5:q6:q7:q8:qSum:iSec:iEvtNr"); 
    tCharge -> SetAutoSave();

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
Bool_t HRichLocalMaxCal::finalize() {
    tCharge -> Write();
    energyout();
   
    return kTRUE;
}
HRichLocalMaxCal& HRichLocalMaxCal::operator=(HRichLocalMaxCal &c) {
    return c;
}
Int_t HRichLocalMaxCal::execute() {
    // cout<<" executing hrichlocalmaxcal  "<<endl;
    //resets the matrixes
    resetmatrix();
    HRichCal *pCal;
    HRichLocal *pLocal;
    HLocation loc;
    fIter -> Reset();
    count1 = count2 = 0;
    fTheta1 = fTheta2 = 0;
    i = 0;
    //fill the matrixes with data from cal container
    while ((pCal = (HRichCal *) fIter -> Next())) {
	loc = fIter -> getLocation();
	fillmatrix(loc);
	i++;
    }
    //if the counter i == 0 there is no event copied to the matrix 
    //cal must be empty!!!
    if (i == 0) {
	cout<<"fillmatrix false !!!"<<endl;
    }
    else localEventNr++;
    //counts the hits in each sector and looks for for afterburning hits
    hitcontrol(); //remove me
    fIter -> Reset();
    i = 0;
    count1 = count2 = 0;
    fTheta1 = fTheta2 = 0;
        if (hitControl == kTRUE) { //remove me

	//calculates the local maxima for a group of 9 or 5 pads
	//theta and phi angle is not longer the centre of one pad
	//safes the data to local container
	//the charge of the local group of pads is added to the central pad
	while ((pCal = (HRichCal*) fIter -> Next())) {
	 
	    loc = fIter -> getLocation();
	    localmaxcal(loc);
	    i++;
	}
	//cout<<" theta1loc  "<<fTheta1/count1<< "  theta2loc  "<<fTheta2/count2<<endl;
	updateHeaders(fTheta1/count1,fTheta2/count2,1,NULL,NULL,eventNr);
	//if the counter i == 0 cal must be empty
	
	
	//calculats the global angles phi and theta from the rings for 
	//each sector and safes it to global container
	//selects events where the global angles are like the specification
	//and the eventnumber in the container is less than the 
	//specification number
	HLocation loc2;
	fIter1 -> Reset();
	i = 0;
	//safes the flag of the rings to local container
	while ((pLocal = (HRichLocal*) fIter1 -> Next())) {
	    loc2 = fIter1 -> getLocation();
	    localanglecut(loc2);
	    i++;
	}
	//if the counter 1 == 0 thre is no event in local container
	if (i == 0) {
	    cout<<"local false !!!"<<endl;
	}
	//counts the ring events in local container
	
	}///remove me

    j++;
    eventNr++;
    if (j == 100) {
	cout<<"EventNr: "<<eventNr<<endl<<endl;
	j = 0;
    }
    return 0;
}
Bool_t HRichLocalMaxCal::fillmatrix(HLocation& fLoc) {
    //writes hit and charge from cal container for active pads into matrix
    //writes theta and phi from pad container for active pads into matrix
   
    HRichCal *cal = NULL;
    cal = (HRichCal*) fCalCat -> getObject(fLoc);
    iCol = cal -> getCol();
    iRow = cal -> getRow();
    iSector = cal -> getSector();
    HRichPad *pad = ((HRichGeometryPar*) fGeometryPar) -> 
    getPadsPar()->getPad(iCol,iRow);
    if (cal) {
	mHit[iSector][iRow][iCol] = 1;
	mCharge[iSector][iRow][iCol] = cal -> getCharge();
	meanCharge[iSector]= meanCharge[iSector]+mCharge[iSector][iRow][iCol];
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
Int_t HRichLocalMaxCal::checkLocalMax(Int_t sec,Int_t i,Int_t j,Int_t a){

    //this method chekcs whether a pad is a local maxima within 
    //5 or 9 neighbours. In this case it calculates the total 
    //charge of the photon hit and the theta, phi angles of the 
    //photon barycenter( the pad angles are weighted with the pad charges).     
    sumCharge= 0;
    sumTheta = 0;
    sumPhi =0; 
    Int_t padCount = 0;
   
    Float_t charge =  mCharge[sec][i][j]; 
    for(Int_t m =-1;m<2;m++){
	for(Int_t n =-1;n<2;n++){
	    if (a==5){ 
		if( mCharge[sec][i+m][j+n]<=charge){
		    //		    cout<<"  charge "<<charge<<" charge on pad "<<mCharge[sec][i+m][j+n]<<endl;
		    padCount++;
		}// the maximum is calculated on 5 pads but an additional chaeck on 9 pads is performed and the local maximum gets a flag equal to 8 if it is a 9 pads max, a flag equal to 1 if it is only a 5 pads local maximum. 
		if(m*n==0){
		    if( mCharge[sec][i+m][j+n]<=charge){
			sumCharge = sumCharge + mCharge[sec][i+m][j+n];
			sumTheta = sumTheta + mCharge[sec][i+m][j+n] *
			    mTheta[sec][i+m][j+n];
			//		cout<<"theta "<<mTheta[sec][i+m][j+n]<<" i" <<i<<"  j"<<j<<" m  "<<m<<" n "<<n<<"  charge "<<mCharge[sec][i+m][j+n]<<endl;
			sumPhi = sumPhi + mCharge[sec][i+m][j+n] *
			    mPhi[sec][i+m][j+n];
			
		    } 
		    else return 0;
		}
		
	    }
	    else {
		if( mCharge[sec][i+m][j+n]<=charge){
		    sumCharge = sumCharge + mCharge[sec][i+m][j+n];
		    sumTheta = sumTheta + mCharge[sec][i+m][j+n] *
			mTheta[sec][i+m][j+n];
		    sumPhi = sumPhi + mCharge[sec][i+m][j+n] *
			mPhi[sec][i+m][j+n];
		    nPad = nPad + mHit[sec][i+m][j+n];
		} 
		else return 0;
	    }
	}
    }
    //    cout<<" sumTheta "<< sumTheta<<"   sumPhi "<< sumPhi<<" sumCharge  "<<sumCharge<<endl;
    sumTheta = sumTheta/sumCharge;
    sumPhi = sumPhi /sumCharge;
    //cout<<" sumTheta "<< sumTheta<<"   sumPhi "<< sumPhi<<endl;
    // return sumCharge;
    //cout<<" padNr min "<<padCount<<endl;
    if(padCount==9) return 9;
    else return 1;
}
Float_t HRichLocalMaxCal::sumcharge(Int_t nRow, Int_t nCol, Int_t nSector) {
    //this method calculates the sum of the charge of all the pads that sorround the central pad between a within of 3 pads. 
    sumCharge = 0;
    for (jRow = nRow-1; jRow < nRow+2; jRow++) {
	for (jCol = nCol-1; jCol < nCol+2; jCol++) {
	    sumCharge = sumCharge + mCharge[nSector][jRow][jCol];
	}
    }
    return sumCharge;
}
Bool_t HRichLocalMaxCal::checkIsolation(Int_t nRow, Int_t nCol, Int_t nSector,Int_t padNum) {
    // this method checks that the photon hit candidate is isolated from other hits.
    if(padNum==9) return kTRUE;
    else{
	for (Int_t m = -4;m<0;m++){
	    if((mCharge[nSector][nRow+2+m][nCol-2]!=0)) return kFALSE;
	    if((mCharge[nSector][nRow-2][nCol+2+m]!=0)) return kFALSE;
	}
	for (Int_t m = 0;m<5;m++){
	    if((mCharge[nSector][nRow-2+m][nCol+2]!=0)) return kFALSE;
	    if((mCharge[nSector][nRow+2][nCol-2+m]!=0)) return kFALSE;
	}
    }
    return kTRUE;
}
Bool_t HRichLocalMaxCal::localmaxcal(HLocation& fLoc) {

    HRichCal *cal = NULL;
    cal = (HRichCal*) fCalCat -> getObject(fLoc);
    HRichLocal *local = NULL;
     iCol = cal -> getCol();
    iRow = cal -> getRow();
    iSector = cal -> getSector();
    //    eventNr = cal -> getEventNr();
    nPad = 0;
    sumTheta = 0;
    sumPhi = 0;
    sumCharge = 0;
    HRichPad *pPad = ((HRichGeometryPar*) fGeometryPar) -> 
    getPadsPar()->getPad(iCol,iRow);
    //    cout<<" new pad analyzed +++++++++++++++++++++++"<<endl;
    //selects if the charge on the pad is like the specifications
    //selects if the central pad is the local maximum for 9 or 5 pads
//charge on central pad is more then half minimum specification
    Int_t flagMax = checkLocalMax(iSector,iRow,iCol,padNumber);
    if ( 
	//if the pad is no edgepad
	(pPad -> getPadActive())&&
	//selects if the central pad is the local maximum for 9 pads
	// or for 5 pads
	 (flagMax>0)){
	Int_t k = 0;
	for (jRow = iRow-1; jRow < iRow+2; jRow++) {
	    for (jCol = iCol-1; jCol < iCol+2; jCol++) {
		nPad = nPad + mHit[iSector][jRow][jCol];
		fCharge[k] = 0;
		fCharge[k] = mCharge[iSector][jRow][jCol];
		k++;
	    }
	}
	//cout<<" photon found "<<endl;
	//safes the data to local container
	local = (HRichLocal*) fLocalCat -> getSlot(fLoc);
	if ((local) && (iCol != 0)) {
	    local = new(local) HRichLocal;
	    local -> setRow(iRow);
	    local -> setCol(iCol);
	    local -> setSector(iSector);
	    local -> setEventNr(eventNr);
	    local -> setAddress(cal -> getAddress());
	    local -> setLocalEventNr(localEventNr);
	    local -> setLocalCharge(sumCharge);
	    local -> setLocalTheta(sumTheta);
	    local -> setLocalPhi(sumPhi);
	    local -> setLocalEnergy(energyset(iRow,iCol,iSector));
	    local -> setPadNr(nPad);
	    local -> setFlagMax(flagMax);
	    // cout<<" col is "<<iCol<<
	    //		" row is "<<iRow<<
	    //	"  number of pads  "<<nPad<<"  flagMax "<<flagMax<<
	    //	endl;
	    //cout<<" sector "<< local ->getSector()<<endl;
	    //if(local->getLocalEnergy()==0) cout<<"energy is 0 ,theta is"<< local ->getLocalTheta()<<endl;
	    //else  cout<<" energy is "<<local->getLocalEnergy()<<" theta is "<<local ->getLocalTheta()<<endl;
	    if (sumTheta>thetaMin1&& sumTheta<thetaMax1) {
		fTheta1 +=sumTheta;
		count1++;
		// cout<<"filling HLocal -> fTheta1 "<<fTheta1<<"  count1 "<<count1<<endl;
	    }
	    else if(sumTheta>thetaMin2&& sumTheta<thetaMax2){
		fTheta2 +=sumTheta;
		count2++;
		//cout<<" fTheta2 "<<fTheta2<<"  count2 "<<count2<<endl;
	    }
	}
	if (local) {
	   
	    multihit(iRow,iCol,iSector,fLoc);
	}
	//cout<<" nPad "<<nPad<<" fCharge[0]  "<< fCharge[0]  <<" fCharge[1] "
	//  <<fCharge[1] <<" fCharge[2] "<< fCharge[2]<< "  fCharge[3] "
	//  <<fCharge[3]<<" fCharge[4] "<< fCharge[4]<< "  fCharge[5] "
	//  <<fCharge[5]<< " fCharge[6] "<<fCharge[6]<<
	//  "  fCharge[7],fCharge[8],sumCharge,iSector "<<
	//  fCharge[7]<<"  "<<fCharge[8]<<"  "<<sumCharge<<"  "
	//  <<iSector<<endl;
	//cout<<" evt Nr "<< localEventNr<<endl;
	tCharge -> Fill(nPad,fCharge[0],fCharge[1],fCharge[2],
			fCharge[3],fCharge[4],fCharge[5],fCharge[6],
			fCharge[7],fCharge[8],sumCharge,iSector,localEventNr);
    }
    return kTRUE;
}


Bool_t HRichLocalMaxCal::localanglecut(HLocation& fLoc) {
    HRichLocal *local = NULL;
    local = (HRichLocal *)fLocalCat -> getObject(fLoc);
  
    if ((local -> getLocalTheta() < thetaMax1) &&
	(local -> getLocalTheta() > thetaMin1)) {
	if ((local) && (local -> getCol() != 0)) {
	    local -> setRing(1);
	}
    }
    if ((local -> getLocalTheta() > thetaMin2) &&
	(local -> getLocalTheta() < thetaMax2)) {
	if ((local) && (local -> getCol() != 0)) {
	    local -> setRing(2);
	}
    }
    else local -> setRing(0);
    return kTRUE;
}
Bool_t HRichLocalMaxCal::resetmatrix() {
    for (iSector = 0; iSector < 6; iSector++) {
	meanCharge[iSector]=0;
	for (iRow = 0; iRow < 90; iRow++) {
	    for (iCol = 0; iCol < 92; iCol++) {
		mHit[iSector][iRow][iCol] = 0;
		mCharge[iSector][iRow][iCol] = 0;
		mTheta[iSector][iRow][iCol] = 0;
		mPhi[iSector][iRow][iCol] = 0;
		mHit2[iSector][iRow][iCol] = 0;
		
	    }
	}
}
    return kTRUE;
}
Bool_t HRichLocalMaxCal::hitcontrol() {
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

    i = 0;
    hitControl = kFALSE;
    //the event is rejected if there is an after burning sector with an hit multiplicity higher than the threshold set in the macro. The shadow sector is excluded from the selection.
    for (iSector = 0; iSector < 6; iSector++) {
	if ((doubleHit[iSector] <= sumHit[iSector] / 2 && sumHit[iSector]>minHit) || 
	    (iSector == shadowSector)) {//$$
	    
	    i++;
	}
    }
    // cout<<" index "<<i<<" sumHit[0]  "<<sumHit[0]  <<" sumHit[1]  "<<sumHit[1]<<" sumHit[2]  "<<sumHit[2]<<" sumHit[3]  "<<sumHit[3] <<" sumHit[4]  "<<sumHit[4]<<" sumHit[5]  "<<sumHit[5]<<endl;

    // cout<<" ************************************* "<<endl;
    //cout<<" edgesector1  "<< edgesector1 <<"  minHitedgesector1  "<<
    //minHitedgesector1<< "  edgesector2  " <<edgesector2<<
    //"  minHitedgesector2 "<<minHitedgesector2<<endl;
    //at least 3 sector must be fired and the multiplicity on the sectors
    // next to the shadow must be greater than a given threshold.
    if (i >=3 && sumHit[edgesector1]>minHitedgesector1&&
	sumHit[edgesector2]>minHitedgesector2 ) {
        hitControl = kTRUE;
	updateHeaders(fTheta1/count1,fTheta2/count2,1,sumHit,meanCharge,eventNr);
    }
    else {
	updateHeaders(0,0,0,sumHit,meanCharge,eventNr);
    }
    return kTRUE;
}
void HRichLocalMaxCal::updateHeaders(Float_t t1, Float_t t2,Int_t i,Float_t *secM,Float_t *meanCS,Int_t evtNum){
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

Bool_t HRichLocalMaxCal::energyfile() {
    pEnergyLockup = new TFile(filename);
    for (iSector = 0; iSector < 6; iSector++) {
	sprintf(energyName,"pEnergySim%d",iSector);
	pEnergySim[iSector] = (TH2F*) pEnergyLockup -> Get(energyName);
    }
    return kTRUE;
}
Bool_t HRichLocalMaxCal::multihit(Int_t nRow, Int_t nCol, Int_t nSector,
				  HLocation& fLoc) {
    //this memberfunction is required for hrichlocalmaxsim class
    return kTRUE;
}
Bool_t HRichLocalMaxCal::energyout() {
    //this memberfunction is required for hrichlocalmaxsim class
    return kTRUE;
}
Float_t HRichLocalMaxCal::energyset(Int_t nRow, Int_t nCol, Int_t nSector) {
    energy = 0.;
    //    cout<<" in exp energyset "<<endl;
    //getting the photonenergy for the pad out of the simulation
    if (pEnergySim[nSector] -> GetCellContent(nRow,nCol) != 0) {
	energy = pEnergySim[nSector] -> GetCellContent(nRow,nCol);
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
    if (pEnergySim[nSector] -> GetCellContent(nRow,nCol+1) != 0) {
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



