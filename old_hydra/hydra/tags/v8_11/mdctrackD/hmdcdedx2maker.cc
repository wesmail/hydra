//*-- AUTHOR : Jochen Markert

//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////////////
//
// HMdcDeDx2Maker
//
// This transformation class calculates the pseudo dEdx from t2-t1 (time above threshold)
// of all fired drift cells included in one HMdcSeg. The transformation is performed using
// the parameter container HMdcDEdX2, which holds the parameters for normalization of the
// measured t2-t1 with impact angle and minimum distance to wire (CAL2 parametrization)
// and the algorithm to normalize the single measurements and average over all cells included
// in the segment. Inside the execute() function a loop over the category of HMdcTrkCand is
// performed and the dEdx calculation is done for each inner and outer HMdcSeg separately.
// The result is filled to HMdcTrkCand.
// Explanations can be looked up in HMdcDeDX2.
//
///////////////////////////////////////////////////////////////////////////////
using namespace std;
#include "hmdcdedx2maker.h"
#include "hmdcdedx2.h"
#include "hmdcseg.h"
#include "hmdctrkcand.h"
#include "hmdcdef.h"
#include "hdebug.h"
#include "hades.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "hcategory.h"
#include "hmessagemgr.h"
#include <iostream> 
#include <iomanip>
#include <stdlib.h>

ClassImp(HMdcDeDx2Maker)

HMdcDeDx2Maker::HMdcDeDx2Maker(void) {
  // Default constructor .
  initParameters();
}
HMdcDeDx2Maker::HMdcDeDx2Maker(Text_t* name,Text_t* title)
                 :  HReconstructor(name,title) {
  // Constructor calls the constructor of class HReconstructor with the name
  // and the title as arguments.
  initParameters();
}

HMdcDeDx2Maker::~HMdcDeDx2Maker(void) {
  // destructor deletes the iterator
  if (trkcanditer) delete trkcanditer;
  trkcanditer=0;
}
void HMdcDeDx2Maker::initParameters()
{
    trkcandCat =0;
    segCat     =0;
    trkcanditer=0;
    mdcdedx    =0;
    hasPrinted =kFALSE;
    module     =2;
}
void HMdcDeDx2Maker::setParContainers() {
  // creates the parameter containers MdcDeDx if they do not
  // exist and adds them to the list of parameter containers in the runtime
  // database
    mdcdedx=(HMdcDeDx2*)(((HRuntimeDb*)(gHades->getRuntimeDb()))->getContainer("MdcDeDx2"));
    if(!mdcdedx) {
	    Error("HMdcDeDx2Maker:setParContainers()","RETRIEVED 0 POINTER FOR HMDCDEDX!");
	    exit(1);
    }else{
	Bool_t ok=mdcdedx->initContainer();
	if(!ok){
	    Error("HMdcDeDx2Maker:setParContainers()","INIT OF HMDCDEDX FAILED!");
	    exit(1);
	}
    }
}
Bool_t HMdcDeDx2Maker::init(void) {
    // gets the pointers to HMdcTrkC and HMdcSeg categories
    // calls the function setParContainers()
  setParContainers();
  trkcandCat=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catMdcTrkCand));
  if (!trkcandCat) {
      Error("HMdcDeDx2Maker:init()","HMdcTrkCand Category not available!");
      exit(1);
  }
  segCat=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catMdcSeg));
  if (!segCat) {
      Error("HMdcDeDx2Maker:init()","HMdcSeg Category not available!");
      exit(1);
  }

  trkcanditer =(HIterator *)((HCategory*)trkcandCat)->MakeIterator("native");

  if(!hasPrinted)printStatus();
  fActive=kTRUE;
  return kTRUE;
}
void HMdcDeDx2Maker::printStatus()
{
    // prints the parameters to the screen
    SEPERATOR_msg("-",60);
    INFO_msg(10,HMessageMgr::DET_MDC,"HMdcDeDx2MakerSetup:");

    gHades->getMsg()->info(10,HMessageMgr::DET_MDC,GetName()
        		   ,"Input Mode      =  %i :  0 = from 1. MDC in seg, 1 = from 2. MDC in seg , 2 = from both MDC in seg",module);

    SEPERATOR_msg("-",60);
    hasPrinted=kTRUE;
}
Int_t HMdcDeDx2Maker::execute(void)
{

    // calculates dEdx for all Segments contained in HMdcTrkCand.
    // The result is filled to HMdctrk.


    HMdcTrkCand* trkcand;
    trkcanditer->Reset();

    Float_t dedx[3];
    Float_t dedxold[3];
    Float_t sig[3];
    Float_t sigold[3];
    UChar_t nw[3];
    UChar_t nwcut[3];

    while ((trkcand=(HMdcTrkCand *)trkcanditer->Next())!=0)
    {
        HMdcSeg* seg[2]={0};
	if(trkcand->getSeg1Ind()!=-1) { seg[0]=(HMdcSeg*)segCat->getObject(trkcand->getSeg1Ind()); }
        if(trkcand->getSeg2Ind()!=-1) { seg[1]=(HMdcSeg*)segCat->getObject(trkcand->getSeg2Ind()); }

        dedx[0]=mdcdedx->calcDeDx(seg,&dedxold[0],&sigold[0],&nw[0],&sig[0],&nwcut[0],0,module,kTRUE,-99.);
        dedx[1]=mdcdedx->calcDeDx(seg,&dedxold[1],&sigold[1],&nw[1],&sig[1],&nwcut[1],1,module,kTRUE,-99.);
        dedx[2]=mdcdedx->calcDeDx(seg,&dedxold[2],&sigold[2],&nw[2],&sig[2],&nwcut[2],2,module,kTRUE,-99.);


	for(Int_t i=0;i<3;i++){

	    if(dedx[i]>-98.){
		trkcand->setdedxSeg        (i,dedx [i]);
		trkcand->setSigmadedxSeg   (i,sig  [i]);
		trkcand->setNWirededxSeg   (i,nw   [i]);
		trkcand->setNWireCutdedxSeg(i,nwcut[i]);
	    }
	}
    }

    return 0;
}
Bool_t HMdcDeDx2Maker::finalize()
{
    return kTRUE;
}
