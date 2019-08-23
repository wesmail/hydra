//*-- AUTHOR : Jochen Markert

//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////////////
//
// HMdcDeDxMaker
//
// This transformation class calculates the pseudo dEdx from t2-t1 (time above threshold)
// of all fired drift cells included in one HMdcSeg. The transformation is performed using
// the parameter container HMdcDEdX, which holds the parameters for normalization of the
// measured t2-t1 with impact angle and minimum distance to wire (CAL2 parametrization)
// and the algorithm to normalize the single measurements and average over all cells included
// in the segment. Inside the execute() function a loop over the category of HMdcTrkCand is
// performed and the dEdx calculation is done for each inner and outer HMdcSeg separately.
// The result is filled to HMdcTrkCand.
// OPTIONS:
// setDeDxMethod(Int_t vers) : vers=0 (default) calculate from HMdcSeg
//                             vers=1           calculate from HMdcClusFit
// Explanations can be looked up in HMdcDeDX.
//
///////////////////////////////////////////////////////////////////////////////
using namespace std;
#include "hmdcdedxmaker.h"
#include "hmdcdedx.h"
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

ClassImp(HMdcDeDxMaker)

HMdcDeDxMaker::HMdcDeDxMaker(void) {
  // Default constructor .
  initParameters();
}
HMdcDeDxMaker::HMdcDeDxMaker(const Text_t* name,const Text_t* title)
                 :  HReconstructor(name,title) {
  // Constructor calls the constructor of class HReconstructor with the name
  // and the title as arguments.
  initParameters();
}

HMdcDeDxMaker::~HMdcDeDxMaker(void) {
  // destructor deletes the iterator
  if (trkcanditer) delete trkcanditer;
  trkcanditer=0;
}
void HMdcDeDxMaker::initParameters()
{
    trkcandCat =0;
    segCat     =0;
    trkcanditer=0;
    mdcdedx    =0;
    hasPrinted =kFALSE;
    method=0;
}
void HMdcDeDxMaker::setParContainers() {
  // creates the parameter containers MdcDeDx if they do not
  // exist and adds them to the list of parameter containers in the runtime
  // database
    mdcdedx=(HMdcDeDx*)(((HRuntimeDb*)(gHades->getRuntimeDb()))->getContainer("MdcDeDx"));
    if(!mdcdedx) {
	    Error("HMdcDeDxMaker:setParContainers()","RETRIEVED 0 POINTER FOR HMDCDEDX!");
	    exit(1);
    }else{
	Bool_t ok=mdcdedx->initContainer();
	if(!ok){
	    Error("HMdcDeDxMaker:setParContainers()","INIT OF HMDCDEDX FAILED!");
	    exit(1);
	}
    }
}
Bool_t HMdcDeDxMaker::init(void) {
    // gets the pointers to HMdcTrkC and HMdcSeg categories
    // calls the function setParContainers()
  setParContainers();
  trkcandCat=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catMdcTrkCand));
  if (!trkcandCat) {
      Error("HMdcDeDxMaker:init()","HMdcTrkCand Category not available!");
      exit(1);
  }
  segCat=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catMdcSeg));
  if (!segCat) {
      Error("HMdcDeDxMaker:init()","HMdcSeg Category not available!");
      exit(1);
  }

  trkcanditer =(HIterator *)((HCategory*)trkcandCat)->MakeIterator("native");

  if(!hasPrinted)printStatus();
  fActive=kTRUE;
  return kTRUE;
}
void HMdcDeDxMaker::printStatus()
{
    // prints the parameters to the screen
    SEPERATOR_msg("-",60);
    INFO_msg(10,HMessageMgr::DET_MDC,"HMdcDeDxMakerSetup:");

    gHades->getMsg()->info(10,HMessageMgr::DET_MDC,GetName()
        		   ,"Input Mode      =  %i :  0 = Fill Input from HMdcSeg, 1 = Fill Input from HMdcClusFit",method);

    SEPERATOR_msg("-",60);
    hasPrinted=kTRUE;
}
Int_t HMdcDeDxMaker::execute(void)
{

    // calculates dEdx for all Segments contained in HMdcTrkCand.
    // The result is filled to HMdctrk.


    HMdcTrkCand* trkcand;
    HMdcSeg*     seg;
    Float_t sig1,sigold1,sig2,sigold2;
    Float_t dedx1,dedx2,dedx1old,dedx2old;
    UChar_t nw1,nwcut1,nw2,nwcut2;
    Int_t indseg1,indseg2;
    trkcanditer->Reset();
    while ((trkcand=(HMdcTrkCand *)trkcanditer->Next())!=0)
    {
        indseg1=trkcand->getSeg1Ind();
	if(indseg1!=-1)
        {
            seg=(HMdcSeg*)segCat->getObject(indseg1);
	    if(seg)
	    {
		dedx1=mdcdedx->calcDeDx(seg,&dedx1old,&sigold1,&nw1,&sig1,&nwcut1,method);
		if(dedx1>-98.)
		{   // function return -99 if nothing has been calculated
		    trkcand->setdedxInnerSeg(dedx1);
		    trkcand->setSigmadedxInnerSeg(sig1);
		    trkcand->setNWirededxInnerSeg(nw1);
		    trkcand->setNWireCutdedxInnerSeg(nwcut1);
		}
	    }
	    else
	    {
		Warning("HMdcDeDxMaker:execute()","ZERO POINTER RECEIVED FOR HMdcSegment 1!");
	    }
	}

        indseg2=trkcand->getSeg2Ind();
        if(indseg2!=-1)
        {
            seg=(HMdcSeg*)segCat->getObject(indseg2);
	    if(seg)
	    {
		dedx2=mdcdedx->calcDeDx(seg,&dedx2old,&sigold2,&nw2,&sig2,&nwcut2,method);
		if(dedx2>-98.)
		{   // function return -99 if nothing has been calculated
		    trkcand->setdedxOuterSeg(dedx2);
		    trkcand->setSigmadedxOuterSeg(sig2);
		    trkcand->setNWirededxOuterSeg(nw2);
		    trkcand->setNWireCutdedxOuterSeg(nwcut2);
		}
	    }
	    else
	    {
		Warning("HMdcDeDxMaker:execute()","ZERO POINTER RECEIVED FOR HMdcSegment 2!");
	    }
	}
    }

    return 0;
}
Bool_t HMdcDeDxMaker::finalize()
{
    return kTRUE;
}
