// File: hrichphotoncal.cc
//
// Author: Soenke Schroeder <soenke.schroeder@physik.tu-muenchen.de>
// Last update by Soenke Schroeder: 01/07/03 19:50:36
// based on hydra532 and root 2.25-03
//
#include "hrichphotoncal.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hrichdetector.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hmatrixcatiter.h"
#include "hlocation.h"
#include "hrichcal.h"
#include "hrichlocal.h"
#include "hdebug.h"
#include "hades.h"
#include "richdef.h"
#include "hrichgeometrypar.h"

ClassImp(HRichPhotonCal)
    
HRichPhotonCal::HRichPhotonCal(Text_t *name, Text_t *title, Float_t charMin,
			       Float_t charMax, Int_t hitMin, Int_t hitMax,
			       Float_t thetaMin, Float_t thetaMax) :
HReconstructor(name, title) {
    minCharge = charMin;
    maxCharge = charMax;
    minHit = hitMin;
    maxHit = hitMax;
    minTheta = thetaMin;
    maxTheta = thetaMax;
    fIter = NULL;
    eventNr = 0;
    localEventNr = 1;
   
    i = 0;
    j = 0;
}
HRichPhotonCal::~HRichPhotonCal(void) {
    if (fIter) delete fIter;
}
Bool_t HRichPhotonCal::init() {
    printf("initialization of rich photon\n");
    HRichDetector *pRichDet = (HRichDetector*) gHades -> getSetup()
	-> getDetector("Rich");
    fCalCat = gHades -> getCurrentEvent() -> getCategory(catRichCal);
    if (!fCalCat) {
	fCalCat = pRichDet -> buildCategory(catRichCal);
	if (!fCalCat) return kFALSE;
	else gHades -> getCurrentEvent() -> addCategory(catRichCal,
							fCalCat, "Rich");
    }
    fIter = (HIterator*) getCalCat() -> MakeIterator();
    fLocalCat = gHades -> getCurrentEvent() -> getCategory(catRichLocal);
    if (!fLocalCat) {
	fLocalCat = pRichDet -> buildCategory(catRichLocal);
	if (!fLocalCat) return kFALSE;
	else gHades -> getCurrentEvent() -> addCategory(catRichLocal, 
							fLocalCat, "Rich");
    }
    tCharge = new TNtuple("tCharge","tCharge","padNr:q0:q1:q2:q3:q4:q5:q6:q7:q8:qSum:iSec");
    tCharge -> SetAutoSave();
    HRuntimeDb* rtdb = gHades -> getRuntimeDb();
    HRichGeometryPar *pGeomPar = (HRichGeometryPar*) rtdb -> 
	getContainer("RichGeometryParameters");
//     if (pGeomPar == NULL) {
// 	pGeomPar = new HRichGeometryPar;
// 	rtdb -> addContainer(pGeomPar);
//     }
    setGeometryPar(pGeomPar);
    if (pGeomPar == NULL) return kFALSE;
    return kTRUE;
}
Bool_t HRichPhotonCal::finalize() {
    tCharge -> Write();
    return kTRUE;
}
HRichPhotonCal& HRichPhotonCal::operator=(HRichPhotonCal &c) {
    return c;
}
Int_t HRichPhotonCal::execute() {
    //subroutine to get the active pads and cut two pads at the edge
    padmatrix();
    //reset the matrixes
    resetmatrix();
    HRichCal *pCal;
    HLocation loc;
    fIter -> Reset();
    i = 0;
    //fill the matrixes with eventdata
    while ((pCal = (HRichCal *)fIter -> Next())) {
	loc = fIter -> getLocation();
	fillmatrix(loc);
	i++;
    }
    //if the counter i == 0 there is no event copied to the matrix 
    //cal must be empty!!!
    if (i == 0) {
	cout<<"fillmatrix false !!!"<<endl;
    }
    //count the hits for each sector
    j++;
    if (j == 10) {
	cout<<"EventNr: "<<eventNr<<endl<<endl;
	j = 0;
    }
    fIter -> Reset();
    nPad = 0;
    localControl = kFALSE;
    //select the singlephotons and save them in local container
    while ((pCal = (HRichCal*) fIter -> Next())) {
	loc = fIter -> getLocation();
	photon(loc);
    }
    //count the events in local container
    //cout<<"LocalEventNr: "<<LocalEventNr<<endl;
    if (localControl == kTRUE) {
	localEventNr++;
    }
    //count the events in cal container
    return 0;
}
Bool_t HRichPhotonCal::resetmatrix() {
    for (iSector = 0; iSector < 6; iSector++) {
	sumHit[iSector] = 0;
	for (iRow = 0; iRow < 90; iRow++) {
	    for (iCol = 0; iCol < 92; iCol++) {
		mHit[iSector][iRow][iCol] = 0;
		mCharge[iSector][iRow][iCol] = 0.;
		mTheta[iSector][iRow][iCol] = 0.;
		mPhi[iSector][iRow][iCol] = 0.;
	    }
	}
    }
    return kTRUE;
}
Bool_t HRichPhotonCal::fillmatrix(HLocation& fLoc) {
    //getting the data from cal container
    HRichCal *cal = NULL;
    cal = (HRichCal*) fCalCat -> getObject(fLoc);
    iCol = cal -> getCol();
    iRow = cal -> getRow();
    iSector = cal -> getSector();
    eventNr = cal -> getEventNr();
    HRichPad *pPad = ((HRichGeometryPar*) fGeometryPar) -> 
	fPads.getPad(iCol,iRow);
    if ((cal) && (cal -> getCharge() != 0)) {
	mHit[iSector][iRow][iCol] = 1;
    	mCharge[iSector][iRow][iCol] = cal -> getCharge();
	mTheta[iSector][iRow][iCol] = pPad -> getTheta();
	mPhi[iSector][iRow][iCol] = pPad -> getPhi(iSector);
	sumHit[iSector] = sumHit[iSector]++;
    }
    return kTRUE;
}
Bool_t HRichPhotonCal::centralpad(Int_t nRow, Int_t nCol, Int_t nSector)
{
    for (jRow = nRow-1; jRow < nRow+2; jRow++) {
	for (jCol = nCol-1; jCol < nCol+2; jCol++) {
	    if (mCharge[nSector][jRow][jCol] > 
		mCharge[nSector][nRow][nCol]) {
		return kFALSE;
	    }
	}
    }
    return kTRUE;
}
Float_t HRichPhotonCal::sumcharge(Int_t nRow, Int_t nCol, Int_t nSector) {
    sumCharge = 0;
    for (jRow = nRow-1; jRow < nRow+2; jRow++) {
	for (jCol = nCol-1; jCol < nCol+2; jCol++) {
	    sumCharge = sumCharge + mCharge[nSector][jRow][jCol];
	}
    }
    return sumCharge;
}
Bool_t HRichPhotonCal::checkIsolation(Int_t nRow, Int_t nCol, Int_t nSector) {
    // this method checks that the photon hit candidate is isolated from other hits.
    for (Int_t m = -4;m<0;m++){
	if((mCharge[nSector][nRow+2+m][nCol-2]!=0)) return kFALSE;
	if((mCharge[nSector][nRow-2][nCol+2+m]!=0)) return kFALSE;
    }
    for (Int_t m = 0;m<5;m++){
	if((mCharge[nSector][nRow-2+m][nCol+2]!=0)) return kFALSE;
	if((mCharge[nSector][nRow+2][nCol-2+m]!=0)) return kFALSE;
    }
    return kTRUE;
}
Bool_t HRichPhotonCal::photon(HLocation& fLoc) {
    //getting the data from cal container
    HRichCal *cal = NULL;
    cal = (HRichCal*) fCalCat -> getObject(fLoc);
    HRichLocal *local = NULL;
    iCol = cal -> getCol();
    iRow = cal -> getRow();
   
    iSector = cal -> getSector();
    HRichPad *pPad = ((HRichGeometryPar*) fGeometryPar) -> 
	fPads.getPad(iCol,iRow);
    //select events with hitnumber in one sector like the specifications
    if ((sumHit[iSector] > minHit) && (sumHit[iSector] < maxHit) &&
	//select pads with theta like the specifications
	(pPad -> getTheta() > minTheta) && (pPad -> getTheta() < maxTheta) &&
	//select if pads is centralpad
	(centralpad(iRow, iCol, iSector) == kTRUE) &&
	//select if sum of charge for the local group is like the specification
	(sumcharge(iRow, iCol, iSector) < maxCharge) && 
	(sumcharge(iRow, iCol, iSector) > minCharge) &&
	//charge on sentral pad is more then half minimum specification
	(mCharge[iSector][iRow][iCol] > minCharge / 2) &&
	//select if al 16 pads outside the lokal group are zero
	(checkIsolation(iRow, iCol, iSector) == kTRUE) &&
	//if the pad is no edgepad
	(mPad2[iRow][iCol] == 1)) {
	//count the photons
	
	sumCharge = sumcharge(iRow, iCol, iSector);
	i = 0;
	nPad = 0;
	sumTheta = 0;
	sumPhi = 0;
	//calculates the sum of the charges and the emphasisangles
	for (jRow = iRow-1; jRow < iRow+2; jRow++) {
	    for (jCol = iCol-1; jCol < iCol+2; jCol++) {
		sumTheta = sumTheta + mCharge[iSector][jRow][jCol] *
		    mTheta[iSector][jRow][jCol];
		sumPhi = sumPhi + mCharge[iSector][jRow][jCol] *
		    mPhi[iSector][jRow][jCol];
		nPad = nPad + mHit[iSector][jRow][jCol];
		//add the charge
		fCharge[i] = 0;
		fCharge[i] = mCharge[iSector][jRow][jCol];
		
		i++;
	    }
	}
	sumTheta = sumTheta / sumCharge;
	sumPhi = sumPhi / sumCharge;
	//save data of local group to local container
	HLocation loc1;
	loc1.set(3,0,0,0);
	loc1.setOffset(iCol);
	loc1.setIndex(1,iRow);
	loc1.setIndex(0,iSector);
	if ((loc1.getIndex(1) >= 0) && (loc1.getOffset() >= 0)) {
	    local = (HRichLocal*) fLocalCat -> getSlot(loc1);
	    if (local != NULL) {
		local = new (local) HRichLocal;
		local -> setRow(iRow);
		local -> setCol(iCol);
		local -> setSector(iSector);
		local -> setEventNr(eventNr);
		local -> setAddress(cal -> getAddress());
		local -> setLocalEventNr(localEventNr);
		local -> setLocalCharge(sumCharge);
		local -> setLocalTheta(sumTheta);
		local -> setLocalPhi(sumPhi);
		local -> setRing(nPad);
	    }
	}
	//differentiate between left or right wire 
	
	tCharge -> Fill(nPad,fCharge[0],fCharge[1],fCharge[2],
			fCharge[3],fCharge[4],fCharge[5],fCharge[6],
			fCharge[7],fCharge[8],sumCharge,iSector);
	localControl = kTRUE;
    }
    return kTRUE;
}
Bool_t HRichPhotonCal::padmatrix() {
    for (iRow = 0; iRow < 90; iRow++) {
	for (iCol = 0; iCol < 92; iCol++) {
	    mPad1[iRow][iCol] = 0;
	    mPad2[iRow][iCol] = 0;
	    HRichPad *pPad = ((HRichGeometryPar*) fGeometryPar) -> 
		fPads.getPad(iCol,iRow);
	    if (pPad -> getPadActive()) {
		mPad1[iRow][iCol] = 1;
		mPad2[iRow][iCol] = 1;
	    }
	}
    }
    for (iRow = 0; iRow < 90; iRow++) {
	for (iCol = 0; iCol < 92; iCol++) {
	    if ((mPad1[iRow][iCol] == 1) && (mPad1[iRow-2][iCol] == 0)) {
		mPad2[iRow][iCol] = 0;
		mPad2[iRow-1][iCol] = 0;
	    }
	    if ((mPad1[iRow][iCol] == 1) && (mPad1[iRow+2][iCol] == 0)) {
		mPad2[iRow][iCol] = 0;
		mPad2[iRow+1][iCol] = 0;
	    }
	    if ((mPad1[iRow][iCol] == 1) && (mPad1[iRow][iCol-2] == 0)) {
		mPad2[iRow][iCol] = 0;
		mPad2[iRow][iCol-1] = 0;
	    }
	    if ((mPad1[iRow][iCol] == 1) && (mPad1[iRow][iCol+2] == 0)) {
		mPad2[iRow][iCol] = 0;
		mPad2[iRow][iCol+1] = 0;
	    }
	}
    }
    return kTRUE;
}
