// File: hrichlocalmaxcal.cc
//
// Author: Soenke Schroeder <soenke.schroeder@physik.tu-muenchen.de>
// Last update by Soenke Schroeder: 01/07/04 14:14:25
// based on hydra532 and root 2.25-03
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
#include "hrichglobal.h"
#include "hdebug.h"
#include "hades.h"
#include "richdef.h"
#include "hrichgeometrypar.h"

ClassImp(HRichLocalMaxCal)
    
HRichLocalMaxCal::HRichLocalMaxCal(Text_t *name, Text_t *title, Int_t pads,
				   Float_t charMin, Float_t charMax,
				   Int_t hitMin, Int_t hitMax, Int_t sector,
				   Float_t minTheta1, Float_t maxTheta1,
				   Float_t minTheta2, Float_t maxTheta2,
				   Char_t energyFileName[128]) :
HReconstructor(name,title) {
    padNumber = pads;
    minCharge = charMin;
    maxCharge = charMax;
    minHit = hitMin;
    maxHit = hitMax;
    fIter = NULL;
    fIter1 = NULL;
    thetaMin1 = minTheta1;
    thetaMax1 = maxTheta1;
    thetaMin2 = minTheta2;
    thetaMax2 = maxTheta2;
    for (iSector = 0; iSector < 6; iSector++) {
	sumPhi1[iSector] = 0.;
	sumPhi2[iSector] = 0.;
	phiCount1[iSector] = 0;
	phiCount2[iSector] = 0;
    }
    sumTheta1 = 0;
    sumTheta2 = 0;
    thetaCount1 = 0;
    thetaCount2 = 0;
    shadowSector = sector;
    eventNr = 0;
    localEventNr = 1;
    ringEventNr = 1;
    strcpy(filename, energyFileName);
    for (iSector = 0; iSector < 6; iSector++) {
	for (iRow = 0; iRow < 90; iRow++) {
	    for (iCol = 0; iCol < 92; iCol++) {
		mHit2[iSector][iRow][iCol] = 0;
	    }
	}
    }
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
    fCalCat = gHades -> getCurrentEvent() -> getCategory(catRichCal);
    if (!fCalCat) {
	fCalCat = pRichDet -> buildCategory(catRichCal);
	if (!fCalCat) return kFALSE;
	else gHades -> getCurrentEvent() -> addCategory(catRichCal,
							fCalCat,"Rich");
    }
    fIter = (HMatrixCatIter*) getCalCat() -> MakeIterator();
    //fIter = (HIterator*) getCalCat() -> MakeIterator();
    fLocalCat = gHades -> getCurrentEvent() -> getCategory(catRichLocal);
    if (!fLocalCat) {
	fLocalCat = pRichDet -> buildCategory(catRichLocal);
	if (!fLocalCat) return kFALSE;
	else gHades -> getCurrentEvent() -> addCategory(catRichLocal,
							fLocalCat,"Rich");
    }
    fIter1 = (HMatrixCatIter*) getLocalCat() -> MakeIterator();
    //fIter1 = (HIterator*) getLocalCat() -> MakeIterator();
    fGlobalCat = gHades -> getCurrentEvent() -> getCategory(catRichGlobal);
    if (!fGlobalCat) {
	fGlobalCat = pRichDet -> buildCategory(catRichGlobal);
	if (!fGlobalCat) return kFALSE;
	else gHades -> getCurrentEvent() -> addCategory(catRichGlobal,
							fGlobalCat,"Rich");
    }
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
    energyout();
    cout<<"************************************************************"<<endl;
    if (thetaCount1 > 0) {
	sumTheta1 = sumTheta1 / thetaCount1;
    }
    if (thetaCount2 > 0) {
	sumTheta2 = sumTheta2 / thetaCount2;
    }
    cout<<"theta MgF2: "<<sumTheta1<<"; theta SiO2: "<<sumTheta2<<endl;
    for (iSector = 0; iSector < 6; iSector++) {
	if (phiCount1[iSector] > 0) {
	    sumPhi1[iSector] = sumPhi1[iSector] / phiCount1[iSector];
	}
	if (phiCount2[iSector] > 0) {
	    sumPhi2[iSector] = sumPhi2[iSector] / phiCount2[iSector];
	}
	cout<<"phi"<<iSector<<" MgF2: "<<sumPhi1[iSector]<<"; phi"<<
	    iSector<<" SiO2: "<<sumPhi2[iSector]<<endl;
    }
    cout<<endl;
    cout<<"number of events in cal/calsim/raw container: "<<eventNr<<endl;
    cout<<"number of events in local container: "<<localEventNr - 1<<endl;
    cout<<"number of ring events in local container: "<<ringEventNr - 1<<endl;
    cout<<endl;
    return kTRUE;
}
HRichLocalMaxCal& HRichLocalMaxCal::operator=(HRichLocalMaxCal &c) {
    return c;
}
Int_t HRichLocalMaxCal::execute() {

    //resets the matrixes
    resetmatrix();
    HRichCal *pCal;
    HRichLocal *pLocal;
    HLocation loc;
    fIter -> Reset();
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
    //counts the hits in each sector and surches for afterburning hits
    hitcontrol();
    fIter -> Reset();
    i = 0;
    if (hitControl == kTRUE) {

	//calculates the local maxima for a group of 9 or 5 pads
	//theta and phi angle is not longer the centre of one pad
	//safes the data to local container
	//the charge of the local group of pads is added to the central pad
	while ((pCal = (HRichCal*) fIter -> Next())) {
	 
	    loc = fIter -> getLocation();
	    localmaxcal(loc);
	    i++;
	}
	//if the counter i == 0 cal must be empty
	if (i == 0) {
	    cout<<"local false !!!"<<endl;
	}
	//counts the events in local container
	if (i > 0) {
	    //cout<<"localEventNr: "<<localEventNr<<endl;
	    localEventNr++;
	}
	//resets the global angles
	globalreset();
	HLocation loc1;
	localangle(loc1);
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
	if (i > 0) {
	    //cout<<"RingEventNr: "<<ringEventNr<<endl;
	    ringEventNr++;
	}
    }

    j++;
    if (j == 10) {
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
	fPads.getPad(iCol,iRow);
    if (cal) {
	mHit[iSector][iRow][iCol] = 1;
	mCharge[iSector][iRow][iCol] = cal -> getCharge();
	mTheta[iSector][iRow][iCol] = pad -> getTheta();
	mPhi[iSector][iRow][iCol] = pad -> getPhi(iSector);
	
    }
    return kTRUE;
}
Int_t HRichLocalMaxCal::checkLocalMax(Int_t sec,Int_t i,Int_t j,Int_t a){

    sumTheta = 0;
    sumPhi = 0;
    sumCharge = 0;
    Float_t charge =  mCharge[sec][i][j]; 
    for(Int_t m =-1;m<2;m++){
	for(Int_t n =-1;n<2;n++){
	    if (a==5){ 
		if(m*n==0){
		    if( mCharge[sec][i+m][j+n]<=charge){
			sumCharge = sumCharge + mCharge[sec][i+m][j+n];
			sumTheta = sumTheta + mCharge[sec][i+m][j+n] *
			    mTheta[sec][i+m][j+n];
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
		} 
		else return 0;
	    }
	}
    }
    sumTheta = sumTheta/sumCharge;
    sumPhi = sumPhi /sumCharge;
   
    localMaxControl = kTRUE;
    // return sumCharge;
    return 1;
}
Bool_t HRichLocalMaxCal::localmaxcal(HLocation& fLoc) {

    HRichCal *cal = NULL;
    cal = (HRichCal*) fCalCat -> getObject(fLoc);
    HRichLocal *local = NULL;
    iCol = cal -> getCol();
    iRow = cal -> getRow();
    iSector = cal -> getSector();
    eventNr = cal -> getEventNr();
    localMaxControl = kFALSE;
    //selects if the charge on the pad is like the specifications
    //selects if the central pad is the local maximum for 9 or 5 pads
    if ((mCharge[iSector][iRow][iCol] > minCharge) &&
	(mCharge[iSector][iRow][iCol] < maxCharge)) {

	//selects if the central pad is the local maximum for 9 pads
	checkLocalMax(iSector,iRow,iCol,padNumber);
    }
    if (localMaxControl == kTRUE) {
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
	    local -> setLocalEnergy(energyset(iRow,iCol,iSector,fLoc));
	}
	if (local) {
	    mLocalTheta[local -> getSector()][local -> getRow()]
		[local -> getCol()] = local -> getLocalTheta();
	    mLocalPhi[local -> getSector()][local -> getRow()]
		[local -> getCol()] = local -> getLocalPhi();
	    multihit(iRow,iCol,iSector,fLoc);
	}
    }
    return kTRUE;
}
Bool_t HRichLocalMaxCal::localangle(HLocation& fLoc) {
    for (iSector = 0; iSector < 6; iSector++) {
	theta1 = 0;
	theta2 = 0;
	phi1 = 0;
	phi2 = 0;
	n1 = 0;
	n2 = 0;
	for (iRow = 0; iRow < 90; iRow++) {
	    for (iCol = 0; iCol < 92; iCol++) {
		//if the thetaangle of the pad is like the specifications for 
		//the first ring
		if ((mLocalTheta[iSector][iRow][iCol] < thetaMax1)&&
		    (mLocalTheta[iSector][iRow][iCol] > thetaMin1)) {
		    n1++;
		    theta1 = theta1 + mLocalTheta[iSector][iRow][iCol];
		    phi1 = phi1 + mLocalPhi[iSector][iRow][iCol];
		}
		//if the thetaangle of the pad is like the specifications for 
		//the second ring
		if ((mLocalTheta[iSector][iRow][iCol] > thetaMin2)&&
		    (mLocalTheta[iSector][iRow][iCol] < thetaMax2)) {
		    n2++;
		    theta2 = theta2 + mLocalTheta[iSector][iRow][iCol];
		    phi2 = phi2 + mLocalPhi[iSector][iRow][iCol];
		}
	    }
	}
	if (n1 > 0) {
	    theta1 = theta1 / n1;
	    phi1 = phi1 / n1;
	}
	if (n2 > 0) {
	    theta2 = theta2 / n2;
	    phi2 = phi2 / n2;
	}
	//no angle calculation for the shadowsector
	if ((iSector != shadowSector) && (n1 > 0) && (iSector != 1)) {//$$
	    i1++;
	    globalTheta1 = globalTheta1 + theta1;
	}
	if ((iSector != shadowSector) && (n2 > 0) && (iSector != 1)) {//$$
	    i2++;
	    globalTheta2 = globalTheta2 + theta2;
	}
	//safe the global angles of the rings to global container
	if (n1 + n2 > 0) {
	    HRichGlobal *global = NULL;
	    global = (HRichGlobal*) ((HLinearCategory*) fGlobalCat) ->
		getNewSlot(fLoc);
	    if (global) {
		global = new (global) HRichGlobal;
		global -> setSector(iSector);
		global -> setEventNr(eventNr);
		global -> setGlobalEventNr(localEventNr);
		if (n1 > 0) {
		    global -> setGlobalTheta1(theta1);
		    global -> setGlobalPhi1(phi1);
		    global -> setGlobalN1(n1);
		}
		if (n2 > 0) {
		    global -> setGlobalTheta2(theta2);
		    global -> setGlobalPhi2(phi2);
		    global -> setGlobalN2(n2);
		}
	    }
	}
	globalPhi1[iSector] = phi1;
	globalPhi2[iSector] = phi2;
    }
    for (iSector = 0; iSector < 6; iSector++) {
	if (globalPhi1[iSector] > 0) {
	    phiCount1[iSector]++;
	    sumPhi1[iSector] = sumPhi1[iSector] + globalPhi1[iSector];
	}
	if (globalPhi2[iSector] > 0) {
	    phiCount2[iSector]++;
	    sumPhi2[iSector] = sumPhi2[iSector] + globalPhi2[iSector];
	}
    }
    if ((i1 > 0) && (globalTheta1 > 0)) {
	thetaCount1++;
	globalTheta1 = globalTheta1 / i1;
	sumTheta1 = sumTheta1 + globalTheta1;
    }
    if ((i2 > 0) && (globalTheta1 > 0)) {
	thetaCount2++;
	globalTheta2 = globalTheta2 / i2;
	sumTheta2 = sumTheta2 + globalTheta2;
    }
    return kTRUE;
}
Bool_t HRichLocalMaxCal::localanglecut(HLocation& fLoc) {
    HRichLocal *local = NULL;
    local = (HRichLocal *)fLocalCat -> getObject(fLoc);
    local -> setRingEventNr(ringEventNr);
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
    return kTRUE;
}
Bool_t HRichLocalMaxCal::resetmatrix() {
    for (iSector = 0; iSector < 6; iSector++) {
	for (iRow = 0; iRow < 90; iRow++) {
	    for (iCol = 0; iCol < 92; iCol++) {
		mHit[iSector][iRow][iCol] = 0;
		mCharge[iSector][iRow][iCol] = 0;
		mTheta[iSector][iRow][iCol] = 0;
		mPhi[iSector][iRow][iCol] = 0;
		mLocalTheta[iSector][iRow][iCol] = 0;
		mLocalPhi[iSector][iRow][iCol] = 0;
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
		    //counts the hits were pads where active at the event bevor
		    if (mHit2[iSector][iRow][iCol] == 1) {
			doubleHit[iSector]++;
		    }
		}
		mHit2[iSector][iRow][iCol] = mHit[iSector][iRow][iCol];
	    }
	}
    }
    //selects events with more than half afterburning pads in one sector
    for (iSector = 0; iSector < 6; iSector++) {
	if ((sumHit[iSector] != 0) && (iSector != shadowSector) && 
	    (doubleHit[iSector] > sumHit[iSector] / 2) && (iSector != 1)) {//$$
	    cout<<"doublehit false in Sector: "<<iSector<<" with: "
		<<doubleHit[iSector]<<" hits !!!"<<endl;
	}
    }
    i = 0;
    hitControl = kFALSE;
    for (iSector = 0; iSector < 6; iSector++) {
	if (((sumHit[iSector] > minHit) && (sumHit[iSector] < maxHit) &&
	     (doubleHit[iSector] <= sumHit[iSector] / 2)) ||
	    (sumHit[iSector] == 0) || (iSector == shadowSector)
	    || (iSector == 1)) {//$$
	    i++;
	}
    }
    if (i == 6) {
        hitControl = kTRUE;
    }
    //cout<<"hitControl: "<<hitControl<<endl;
    return kTRUE;
}
Bool_t HRichLocalMaxCal::globalreset() {
    iSector = 0;
    globalTheta1 = 0;
    globalTheta2 = 0;
    i1 = 0;
    i2 = 0;
    for (iSector = 0; iSector < 6; iSector++) {
	globalPhi1[iSector] = 0;
	globalPhi2[iSector] = 0;
    }
    return kTRUE;
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
Float_t HRichLocalMaxCal::energyset(Int_t nRow, Int_t nCol, Int_t nSector,
				    HLocation& fLoc) {
    energy = 0.;
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
