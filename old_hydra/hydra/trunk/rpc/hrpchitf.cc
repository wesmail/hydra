//*-- Author   : Pablo Cabanelas
//*-- Created  : 30/08/07
//*-- Modified : 18/12/07 Diego Gonzalez-Diaz

//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////
//
//  HRpcHitF: RPC detector hit finder
//
//  Gets calibrated data from RpcCal(Sim) and writes to RpcHit(Sim)
//
///////////////////////////////////////////////////////////////////

using namespace std;
#include "TRandom.h"
#include <time.h>
#include "hrpcdetector.h"
#include "hrpchitf.h"
#include "rpcdef.h"
#include "hrpccal.h"
#include "hrpccalsim.h"
#include "hrpchitsim.h"
#include "hgeantrpc.h"
#include "hrpcgeomcellpar.h"
#include "hrpcdigipar.h"
#include "hdebug.h"
#include "hades.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hevent.h"
#include "hgeantkine.h"
#include "hcategory.h"
#include <iostream>
#include <iomanip>

void HRpcHitF::initParContainer() {
  pGeomCellPar=(HRpcGeomCellPar*)(gHades->getRuntimeDb()->getContainer("RpcGeomCellPar"));
  pDigiPar=(HRpcDigiPar*)(gHades->getRuntimeDb()->getContainer("RpcDigiPar"));  
}

HRpcHitF::HRpcHitF(void)
{
  pCalCat=NULL;
  pHitCat=NULL;
  iter=NULL;
  pGeomCellPar=NULL;
  pDigiPar=NULL;
}

HRpcHitF::HRpcHitF(const Text_t *name,const Text_t *title) :
  HReconstructor(name,title)
{
  pCalCat=NULL;
  pHitCat=NULL;
  iter=NULL;
  pGeomCellPar=NULL;
  pDigiPar=NULL;
}

HRpcHitF::~HRpcHitF(void)
{
  if (iter) delete iter;
  iter=NULL;
}

Bool_t HRpcHitF::init(void)
{

  initParContainer();

  if (!pGeomCellPar){
    Error("init","No RpcGeomCellPar Parameters");
    return kFALSE;
  }
  if (!pDigiPar){
    Error("init","No RpcDigiPar Parameters");
    return kFALSE;
  }

  HRpcDetector *rpc;
  rpc=(HRpcDetector *)gHades->getSetup()->getDetector("Rpc");

  if(!rpc){
    Error("init","No Rpc Detector found");
    return kFALSE;
  }

  // Create category for calibrated data
  pCalCat=gHades->getCurrentEvent()->getCategory(catRpcCal);
  if (!pCalCat) {
    Error("init","No RpcCal Category");
    return kFALSE;
  }

  // Decide whether we are running for simulation or real data
  HCategory* catKin=gHades->getCurrentEvent()->getCategory(catGeantKine);
  
  if(catKin) simulation=kTRUE;
  else       simulation=kFALSE;

  // Create category for hit data
  pHitCat=gHades->getCurrentEvent()->getCategory(catRpcHit);
  
  if (!pHitCat) {

    if (simulation) pHitCat=rpc->buildMatrixCategory("HRpcHitSim",0.5);  
    else            pHitCat=rpc->buildMatrixCategory("HRpcHit",0.5);  
    
    gHades->getCurrentEvent()->addCategory(catRpcHit,pHitCat,"Rpc");
  }
  
  if(simulation) {
    pGeantRpcCat = gHades->getCurrentEvent()->getCategory(catRpcGeantRaw);  
    if (!pGeantRpcCat) {
      Error("HRpcClusterF::init()","No HGeant RPC Category");
      return kFALSE;
    }  
  }

  iter=(HIterator *)pCalCat->MakeIterator();
  loc.set(3,0,0,0);

  return kTRUE;
}

Int_t HRpcHitF::execute(void)
{
  Float_t tof, charge, xCell, yCell, xSec, ySec, zSec, xMod, yMod, zMod;//[ns], [pC],[mm]
  Float_t deg2pi=3.14159/180, theta_vert = 28.5, yc=1044.34, zc=1902.910;
  Float_t sigma_X, sigma_Y, sigma_Z, sigma_T;
  Float_t vprop, D_layers= 20, Zrpc=12; //[mm/ns], [mm]

  HRpcCalSim *pCal    = NULL; //Only reading, use the same class for HRpcCalSim and HRpcCal objects
  HRpcHitSim *pHitSim = NULL; //Reading and STORING in category, use 2 different classes.
  HRpcHit    *pHit    = NULL; //Reading and STORING in category, use 2 different classes.

  iter->Reset();

  while ((pCal = (HRpcCalSim *)iter->Next())!= NULL) {

    loc[0] = pCal->getSector();
    loc[1] = pCal->getColumn();
    loc[2] = pCal->getCell();

    //Getting cell geometry params

    HRpcGeomCellParCell& pGeoCell  = (*pGeomCellPar)[loc[0]][loc[1]][loc[2]];
    HRpcDigiParCell& digi_cell     = (*pDigiPar)    [loc[0]][loc[1]][loc[2]];

    vprop    =  digi_cell.getVprop();                       

    //Find object RpcHit

    if (simulation) {
      pHitSim = (HRpcHitSim*)pHitCat->getSlot(loc);
      if ( !pHitSim ) {
	Error ("execute()", "Can't get slot in hit finder: sec %i, col %i, cell %i",loc[0],loc[1],loc[2]);
	return EXIT_FAILURE;
      }
      pHitSim = new(pHitSim) HRpcHitSim;
    }
    else {
      pHit = (HRpcHit*)pHitCat->getSlot(loc);
      if ( !pHit ) {
	Error ("execute()", "Can't get slot in hit finder: sec %i, col %i, cell %i",loc[0],loc[1],loc[2]);
	return EXIT_FAILURE;
      }
      pHit = new(pHit) HRpcHit;
    }

    //Calculate the variables for filling the object RpcHit
          
    charge = 0.5*( pCal->getRightCharge() + pCal->getLeftCharge() );
    tof    = 0.5*( pCal->getRightTime()   + pCal->getLeftTime()   );          
    xCell  = 0.5*( pCal->getRightTime()   - pCal->getLeftTime()   )*vprop;

    if(!simulation) xCell  = (pCal->getRightTime()/2)*vprop;  // FIXME: temporal!!!!!!
    
    yCell  = 0.5*( pGeoCell.getWidth() );                              

    xMod   = pGeoCell.getX() + pGeoCell.getLength()/2 + xCell;      
    yMod   = pGeoCell.getY() + yCell;                               
    zMod   = D_layers*(loc[2]%2-0.5);
    
    //FIXME: Rotation to the Lab system
    ySec   = yMod*cos(theta_vert*deg2pi)  + yc;
    zSec   = -yMod*sin(theta_vert*deg2pi) + zc;
    xSec   = xMod;

    sigma_X = 7;          //FIXME. This must be read probably from digipar
    sigma_T = 0.72;

    sigma_Z = Zrpc/sqrt(12.);
    sigma_Y = (pGeoCell.getWidth())/sqrt(12.);

    //Fill the Hit

    if(simulation) {
      pHitSim->setAddress(pCal->getAddress());
      pHitSim->setHit(tof,charge,xMod,yMod,zMod);        
      pHitSim->setXSec(xSec);
      pHitSim->setYSec(ySec);
      pHitSim->setZSec(zSec);
      pHitSim->setRMS(sigma_T, sigma_X, sigma_Y, sigma_Z);
	
      pHitSim->setRefL(pCal->getRefL());
      pHitSim->setRefR(pCal->getRefR());
      pHitSim->setRefLDgtr(pCal->getRefLDgtr());
      pHitSim->setRefRDgtr(pCal->getRefRDgtr());

      pHitSim->setTrackL(((HGeantRpc*)pGeantRpcCat->getObject(pCal->getRefL()))->getTrack());
      pHitSim->setTrackR(((HGeantRpc*)pGeantRpcCat->getObject(pCal->getRefR()))->getTrack());
      pHitSim->setTrackLDgtr(((HGeantRpc*)pGeantRpcCat->getObject(pCal->getRefLDgtr()))->getTrack());
      pHitSim->setTrackRDgtr(((HGeantRpc*)pGeantRpcCat->getObject(pCal->getRefRDgtr()))->getTrack());

    }
    else {
      pHit->setAddress(pCal->getAddress());
      pHit->setLogBit(pCal->getLogBit()); 
      pHit->setHit(tof,charge,xMod,yMod,zMod);        
      pHit->setXSec(xSec);
      pHit->setYSec(ySec);
      pHit->setZSec(zSec);
      pHit->setRMS(sigma_T, sigma_X, sigma_Y, sigma_Z);
    }
  }
  
  return EXIT_SUCCESS;
}

ClassImp(HRpcHitF)
