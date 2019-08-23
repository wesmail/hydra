// File: hrichlocalmaxsim.cc
//
// Author: Soenke Schroeder <soenke.schroeder@physik.tu-muenchen.de>
// Last update by Soenke Schroeder: 01/07/04 14:14:20
// based on hydra532 and root 2.25-03
//
#include "hrichlocalmaxsim.h"
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
#include "hrichcalsim.h"
#include "hrichlocal.h"
#include "hrichglobal.h"
#include "hdebug.h"
#include "hades.h"
#include "richdef.h"
#include "hrichgeometrypar.h"

ClassImp(HRichLocalMaxSim)

Bool_t HRichLocalMaxSim::init() {
    printf("initialization of rich localmaxsim\n");
    HRichDetector *pRichDet = (HRichDetector*) gHades -> getSetup() ->
	getDetector("Rich");
    fCalCat = gHades -> getCurrentEvent() -> getCategory(catRichCal);
    if (!fCalCat) {
	fCalCat = pRichDet -> buildMatrixCat("HRichCalSim", 1);
	if (!fCalCat) return kFALSE;
	else gHades -> getCurrentEvent() -> addCategory(catRichCal,
							fCalCat, "Rich");
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
	fGlobalCat = pRichDet->buildCategory(catRichGlobal);
	if (!fGlobalCat) return kFALSE;
	else gHades -> getCurrentEvent() -> addCategory(catRichGlobal, 
							fGlobalCat, "Rich");
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
    energyreset();
    energyfile();
    return kTRUE;
}
Bool_t HRichLocalMaxSim::energyreset() {
    for (iSector = 0; iSector < 6; iSector++) {
  	for (iRow = 0; iRow < 90; iRow++) {
  	    for (iCol = 0; iCol < 92; iCol++) {
		mEnergy[iSector][iRow][iCol] = 0;
		mMultiHit[iSector][iRow][iCol] = 0;
	    }
	}
    }
    return kTRUE;
}
Bool_t HRichLocalMaxSim::energyfile() {
    for (iSector = 0; iSector < 6; iSector++) {
	sprintf(energyName,"pEnergySim%d",iSector);
	pEnergySim[iSector] = new TH2F(energyName,energyName,90,0,90,92,0,92);
    }
    pEnergyLockup = new TFile(filename,"RECREATE","pEnergySim",1);
    return kTRUE;
}
Bool_t HRichLocalMaxSim::multihit(Int_t nRow, Int_t nCol, Int_t nSector, 
				  HLocation& fLoc) {
    HRichLocal *local = NULL;
    local = (HRichLocal*) fLocalCat -> getSlot(fLoc);
    if (local -> getLocalEnergy() > 0) {
	mEnergy[nSector][nRow][nCol] = mEnergy[nSector][nRow][nCol]
	    + local -> getLocalEnergy();
	mMultiHit[nSector][nRow][nCol] = mMultiHit[nSector][nRow][nCol] +1;
    }
    return kTRUE;
}
Bool_t HRichLocalMaxSim::energyout() {
    pEnergyLockup -> cd();
    for (iSector = 0; iSector < 6; iSector++) {
  	for (iRow = 0; iRow < 90; iRow++) {
  	    for (iCol = 0; iCol < 92; iCol++) {
		if (mMultiHit[iSector][iRow][iCol] > 0) {
		    mEnergy[iSector][iRow][iCol] = mEnergy[iSector][iRow][iCol]
			/ mMultiHit[iSector][iRow][iCol];
		    pEnergySim[iSector] -> 
			Fill(iRow,iCol,mEnergy[iSector][iRow][iCol]);
		}
  	    }
  	}
	pEnergySim[iSector] -> Write();
    }
    pEnergyLockup -> Close();
    return kTRUE;
}
Float_t HRichLocalMaxSim::energyset(Int_t nRow, Int_t nCol, Int_t nSector, 
				    HLocation& fLoc) {
    energy = 0.;
    //getting the photonenergy for the pad from sim container
    HRichCalSim *sim = NULL;
    sim = (HRichCalSim*) fCalCat -> getObject(fLoc);
    if (sim -> getEnergy() != 0) {
	energy = sim -> getEnergy();
	return energy;
    }
    //if there is no photonenergyinformation for this pad it is
    //calculated by the information out of the nightbourpads
    if (mEnergy[nSector][nRow][nCol] != 0) {
	energy = mEnergy[nSector][nRow][nCol] / mMultiHit[nSector][nRow][nCol];
	return energy;
    }
    if ((energy == 0) && (mEnergy[nSector][nRow][nCol-1] != 0) && 
	(mEnergy[nSector][nRow][nCol+1] != 0)) {
	energy = (mEnergy[nSector][nRow][nCol-1] /
		  mMultiHit[nSector][nRow][nCol-1] +
		  mEnergy[nSector][nRow][nCol+1] /
		  mMultiHit[nSector][nRow][nCol+1]) / 2;
	return energy;
    }
    if ((mEnergy[nSector][nRow-1][nCol] != 0) && 
	(mEnergy[nSector][nRow+1][nCol] != 0)) {
	energy = (mEnergy[nSector][nRow-1][nCol] /
		  mMultiHit[nSector][nRow-1][nCol] +
		  mEnergy[nSector][nRow+1][nCol] /
		  mMultiHit[nSector][nRow+1][nCol]) / 2;
	return energy;
    }
    if (mEnergy[nSector][nRow][nCol-1] != 0) {
	energy = mEnergy[nSector][nRow][nCol-1] /
	    mMultiHit[nSector][nRow][nCol-1];
	return energy;
    }
    if (mEnergy[nSector][nRow][nCol+1] != 0) {
	energy = mEnergy[nSector][nRow][nCol+1] /
	    mMultiHit[nSector][nRow][nCol+1];
	return energy;
    }
    if ((mEnergy[nSector][nRow][nCol-2] != 0) && 
	(mEnergy[nSector][nRow][nCol+2] != 0)) {
	energy = (mEnergy[nSector][nRow][nCol-2] /
		  mMultiHit[nSector][nRow][nCol-2] +
		  mEnergy[nSector][nRow][nCol+2] /
		  mMultiHit[nSector][nRow][nCol+2]) / 2;
	return energy;
    }
    if ((mEnergy[nSector][nRow-1][nCol] != 0) && 
	(mEnergy[nSector][nRow-2][nCol] != 0)) {
	energy = 2 * mEnergy[nSector][nRow-1][nCol] /
	    mMultiHit[nSector][nRow-1][nCol] -
	    mEnergy[nSector][nRow-2][nCol] /
	    mMultiHit[nSector][nRow-2][nCol];
	return energy;
    }
    if ((mEnergy[nSector][nRow+1][nCol] != 0) && 
	(mEnergy[nSector][nRow+2][nCol] != 0)) {
	energy = 2 * mEnergy[nSector][nRow+1][nCol] /
	    mMultiHit[nSector][nRow+1][nCol] -
	    mEnergy[nSector][nRow+2][nCol] /
	    mMultiHit[nSector][nRow+2][nCol];
	return energy;
    }
    if (mEnergy[nSector][nRow][nCol+2] != 0) {
	energy = mEnergy[nSector][nRow][nCol+2] /
	    mMultiHit[nSector][nRow][nCol+2];
	return energy;
    }
    if (mEnergy[nSector][nRow][nCol-2] != 0) {
	energy = mEnergy[nSector][nRow][nCol-2] /
	    mMultiHit[nSector][nRow][nCol-2];
	return energy;
    }
    return 0.;
}
