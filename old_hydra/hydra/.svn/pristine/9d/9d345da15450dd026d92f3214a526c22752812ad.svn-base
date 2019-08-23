//*-- Author  : Pablo Cabanelas
//*-- Created : 24/08/07

//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////
//
//  HRpcCalibrater: perform global correction on the data
//
// Calibrates RpcRaw data and writes to RpcCal.
//
////////////////////////////////////////////////////////////////

using namespace std;
#include "hrpcdetector.h"
#include "hrpccalibrater.h"
#include "rpcdef.h"
#include "hrpcraw.h"
#include "hrpccal.h"
#include "hrpccalpar.h"
#include "hdebug.h"
#include "hades.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hevent.h"
#include "hcategory.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>


HRpcCalibrater::HRpcCalibrater(void)
{
  pRawCat=NULL;
  pCalCat=NULL;
  iter=NULL;
  pCalPar=NULL;
}

HRpcCalibrater::HRpcCalibrater(const Text_t *name,const Text_t *title) :
  HReconstructor(name,title)
{
  pRawCat=NULL;
  pCalCat=NULL;
  iter=NULL;
  pCalPar=NULL;
}

HRpcCalibrater::~HRpcCalibrater(void)
{
  if (iter) delete iter;
  iter=NULL;
}

Bool_t HRpcCalibrater::init(void)
{
  HRpcDetector *rpc;
  rpc=(HRpcDetector *)gHades->getSetup()->getDetector("Rpc");

  if(!rpc){
    Error("init","No Rpc Detector found");
    return kFALSE;
  }

  // Categories
  pRawCat=gHades->getCurrentEvent()->getCategory(catRpcRaw);
  if (!pRawCat) {
    Error("init","No RpcRaw Category");
    return kFALSE;
  }
  pCalCat=gHades->getCurrentEvent()->getCategory(catRpcCal);
  if (!pCalCat) {
    pCalCat=gHades->getSetup()->getDetector("Rpc")->buildCategory(catRpcCal);
    if (!pCalCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catRpcCal,pCalCat,"Rpc");
  }

  // Parameters
  pCalPar=(HRpcCalPar*)(gHades->getRuntimeDb()->getContainer("RpcCalPar"));
  if (!pCalPar){
    Error("init","No RpcCalPar Parameters");
    return kFALSE;
  }

  iter=(HIterator *)pRawCat->MakeIterator();
  loc.set(3,0,0,0);
  fActive=kTRUE;

  return kTRUE;
}

Int_t HRpcCalibrater::execute(void)
{
  HRpcRaw *pRaw=0;
  HRpcCal *pCal=0;

  // Do the calibration here and fill RpcCal category
  iter->Reset();
  while ( (pRaw=(HRpcRaw *)iter->Next()) != NULL) {

    loc[0] = pRaw->getSector();
    loc[1] = pRaw->getColumn();
    loc[2] = pRaw->getCell();

    if(loc[2]>31) continue; //FIXME: 31 (number of cells, must be hardcoded)
                            //Note: In raw, also spare TRB channels are stored, but here
                            //only real cells are used

    pCal=(HRpcCal*)pCalCat->getSlot(loc);
    if (pCal) {
      pCal=new (pCal) HRpcCal;
      if ( !pCal ) return EXIT_FAILURE;

      // Filling address (sector, column and cell) and logBit to RpcCal category
      pCal->setAddress(pRaw->getAddress());
      pCal->setLogBit(pRaw->getRightLogBit()); //FIXME: Here this is just a convention

      // Getting calibration parameters from RpcCalPar container
      Float_t rightTimeOffset,leftTimeOffset,rightTotOffset,leftTotOffset,tdc2time,tot2charge,tzero=0.0;

      HRpcCalParCol* pCalParCol=pCalPar->getCol(loc[0],loc[1]);
      if(pCalParCol) {
	HRpcCalParCell* pCalParCell=pCalParCol->getCell(loc[2]);
	if(pCalParCell) {
	  pCalParCell->getAddress(rightTimeOffset,leftTimeOffset,rightTotOffset,leftTotOffset,tdc2time,
				  tot2charge,tzero);

	  // Adding offsets and correcting units
	  Float_t rightTime,leftTime;
	  Float_t rightTot,leftTot,rightTot2,leftTot2;

	  rightTime = ((pRaw->getLeftTime() - pRaw->getRightTime()) + rightTimeOffset ) * tdc2time;  
	  //FIXME: temporal.
	  //rightTime  = (pRaw->getRightTime()+rightTimeOffset) * tdc2time;
	  leftTime   = (pRaw->getLeftTime()+leftTimeOffset) * tdc2time;

	  rightTot  = (pRaw->getRightTot()+rightTotOffset) * tot2charge;
	  leftTot   = (pRaw->getLeftTot()+leftTotOffset) * tot2charge;

	  rightTot2 = (pRaw->getRightTotLast()+rightTotOffset) * tot2charge;
	  leftTot2  = (pRaw->getLeftTotLast()+leftTotOffset) * tot2charge;

	  // Setting values to the RpcCal category
	  pCal->setRightTime(rightTime);
	  pCal->setLeftTime(leftTime);

	  pCal->setRightCharge(rightTot);
	  pCal->setRightCharge2(rightTot2);
	  pCal->setLeftCharge(leftTot);
	  pCal->setLeftCharge2(leftTot2);
	  //By default it stores the TotLast as the second charge: Charge2


	} else { Warning("execute","Can't get CalParCell"); }
      } else { Warning("execute","Can't get CalParCol"); }

    } else {
      Error ("execute()", "Can't get slot in calibrater: sec %i, col %i, cell %i",loc[0],loc[1],loc[2]);
      return -1;
    }
  }
  return EXIT_SUCCESS;
}

ClassImp(HRpcCalibrater)
