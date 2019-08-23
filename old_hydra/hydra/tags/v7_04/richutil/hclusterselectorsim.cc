// File: hclusterselectorsim.cc
//
// Author: Laura Fabbietti <L.Fabbietti@physik.tu-muenchen.de>
// Last update by Laura Fabbietti: 02/08/08 20:41:59
//
#include "hclusterselectorsim.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hrichdetector.h"
#include "hevent.h"
#include "hades.h"
#include "richdef.h"
#include "hrichgeometrypar.h"
#include "hmatrixcatiter.h"
#include "hlinearcatiter.h"
#include "hruntimedb.h"
ClassImp(HClusterSelectorSim)

Bool_t HClusterSelectorSim::init(){
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
	fCalCat = pRichDet -> buildMatrixCat("HRichCalSim", 1);
	if (!fCalCat) return kFALSE;
	else gHades -> getCurrentEvent() -> addCategory(catRichCal,
							fCalCat,"Rich");
    }
    fIter = (HMatrixCatIter*) getCalCat() -> MakeIterator();
    //fIter = (HIterator*) getCalCat() -> MakeIterator();
     fPhotClusCat= gHades -> getCurrentEvent() -> getCategory(catRichPhotClus);
    if (!fPhotClusCat) {
	 fPhotClusCat= pRichDet -> buildCategory(catRichPhotClus);
	if (!fPhotClusCat) return kFALSE;
	else gHades -> getCurrentEvent() -> addCategory(catRichPhotClus,
							 fPhotClusCat ,"Rich");
    }
    fIter1 = (HMatrixCatIter*) getPhotClusCat() -> MakeIterator();
    //fIter1 = (HIterator*) getLocalCat() -> MakeIterator();

  

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
