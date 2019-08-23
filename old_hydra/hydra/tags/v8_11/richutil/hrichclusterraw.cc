// File: hrichclusterraw.cc
//
// Author: Soenke Schroeder <soenke.schroeder@physik.tu-muenchen.de>
// Last update by Soenke Schroeder: 01/06/29 13:15:22
// based on hydra531 and root 2.25-03
//
#include "hrichclusterraw.h"
#include "hrichclustercal.h"
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

ClassImp(HRichClusterRaw)
    
Bool_t HRichClusterRaw::init() {
    printf("initialization of rich cluster\n");
    HRichDetector *pRichDet = (HRichDetector*) gHades -> getSetup()
	-> getDetector("Rich");
    fRawCat = gHades -> getCurrentEvent() -> getCategory(catRichRaw);
    if (!fRawCat) {
	fRawCat = pRichDet -> buildCategory(catRichRaw);
	if (!fRawCat) return kFALSE;
	else gHades -> getCurrentEvent() -> addCategory(catRichRaw,
							fRawCat, "Rich");
    }
    fLocalCat = gHades -> getCurrentEvent() -> getCategory(catRichLocal);
    if (!fLocalCat) {
	fLocalCat = pRichDet -> buildCategory(catRichLocal);
	if (!fLocalCat) return kFALSE;
	else gHades -> getCurrentEvent() -> addCategory(catRichLocal, 
							fLocalCat, "Rich");
    }
    fIter = (HMatrixCatIter*) getRawCat() -> MakeIterator();
    return kTRUE;
}
