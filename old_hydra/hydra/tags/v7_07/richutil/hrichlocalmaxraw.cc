// File: hrichlocalmaxraw.cc
//
// Author: Soenke Schroeder <soenke.schroeder@physik.tu-muenchen.de>
// Last update by Soenke Schroeder: 01/07/02 16:23:12
// based on hydra532 and root 2.25-03
//
//*****************************************************************************
// this class is build to analyse and select the big ring events from the 
// online efficiency mesurement (OEM) of the rich. It surches for photon hits 
// in raw container and safes them to lokal container. For a local maximum out
// of 5 or 9 nightbourpads it calculates the emphasisangles out of the charges
// and safes it together with the sum of the charges to the central pad. After 
// this the emphasisangles for each big ring is calculated out of the 
// emphasisangle for the photonhits. If the rings in one event is like the 
// specifications, a flag is set to local container. For each pad of one ring
// the energy of the pad out of the simulation with the same beamenergy and 
// particle is safed.
//*****************************************************************************
//
#include "hrichlocalmaxraw.h"
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
#include "hrichraw.h"
#include "hrichlocal.h"
#include "hrichglobal.h"
#include "hdebug.h"
#include "hades.h"
#include "richdef.h"
#include "hrichgeometrypar.h"

ClassImp(HRichLocalMaxRaw)
    
Bool_t HRichLocalMaxRaw::init() {
    printf("initialization of rich localmaxraw\n");
    HRichDetector *pRichDet = (HRichDetector*) gHades -> getSetup() ->
	getDetector("Rich");
    fRawCat = gHades -> getCurrentEvent() -> getCategory(catRichRaw);
    if (!fRawCat) {
	fRawCat = pRichDet->buildCategory(catRichRaw);
	if (!fRawCat) return kFALSE;
	else gHades -> getCurrentEvent() -> addCategory(catRichRaw,
							fRawCat,"Rich");
    }
    fIter = (HIterator*) getRawCat() -> MakeIterator();
    fLocalCat = gHades -> getCurrentEvent() -> getCategory(catRichLocal);
    if (!fLocalCat) {
	fLocalCat = pRichDet->buildCategory(catRichLocal);
	if (!fLocalCat) return kFALSE;
	else gHades -> getCurrentEvent() -> addCategory(catRichLocal,
							fLocalCat,"Rich");
    }
    fIter1 = (HIterator*) getLocalCat() -> MakeIterator();
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
//     if (pGeomPar == NULL) {
// 	pGeomPar = new HRichGeometryPar;
// 	rtdb -> addContainer(pGeomPar);
//     }
    setGeometryPar(pGeomPar);
    if (pGeomPar == NULL) return kFALSE;
    energyfile();
    return kTRUE;
}
Bool_t HRichLocalMaxRaw::energyfile() {
    pEnergyLockup = new TFile(filename);
    for (iSector = 0; iSector < 6; iSector++) {
	sprintf(energyName,"pEnergySim%d",iSector);
	pEnergySim[iSector] = (TH2F*) pEnergyLockup -> Get(energyName);
    }
    return kTRUE;
}

