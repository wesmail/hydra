// File: hrichphotonraw.cc
//
// Author: Soenke Schroeder <soenke.schroeder@physik.tu-muenchen.de>
// Last update by Soenke Schroeder: 01/07/02 16:22:19
// based on hydra532 and root 2.25-03
//
#include "hrichphotonraw.h"
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
#include "hrichraw.h"
#include "hrichlocal.h"
#include "hdebug.h"
#include "hades.h"
#include "richdef.h"
#include "hrichgeometrypar.h"

ClassImp(HRichPhotonRaw)
    
Bool_t HRichPhotonRaw::init() {
    printf("initialization of rich photon\n");
    HRichDetector *pRichDet = (HRichDetector*) gHades -> getSetup()
	-> getDetector("Rich");
    fRawCat = gHades -> getCurrentEvent() -> getCategory(catRichRaw);
    if (!fRawCat) {
	fRawCat = pRichDet -> buildCategory(catRichRaw);
	if (!fRawCat) return kFALSE;
	else gHades -> getCurrentEvent() -> addCategory(catRichRaw,
							fRawCat, "Rich");
    }
    fIter = (HMatrixCatIter*) getRawCat() -> MakeIterator();
    fLocalCat = gHades -> getCurrentEvent() -> getCategory(catRichLocal);
    if (!fLocalCat) {
	fLocalCat = pRichDet -> buildCategory(catRichLocal);
	if (!fLocalCat) return kFALSE;
	else gHades -> getCurrentEvent() -> addCategory(catRichLocal, 
							fLocalCat, "Rich");
    }
    tCharge = new TNtuple("tCharge","tCharge","photNr:q0:q1:q2:q3:q4:q5:q6:q7:q8:qSum:iSym:iSec");
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
