using namespace std;
#include "TRandom.h"
#include <time.h>
#include "hrpcdigitizer.h"
#include "rpcdef.h"
#include "hdebug.h"
#include "hades.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hrpcdetector.h"
#include "hgeantrpc.h"
#include "hgeantkine.h"
#include "hevent.h"
#include "hcategory.h"
#include "hlocation.h"
#include "hrpcdigipar.h"
#include "hrpcgeomcellpar.h"
#include "hrpccalsim.h"
#include "hrpccal.h"
#include <iostream>
#include <iomanip>

//*-- Author   : D. Gonzalez-Diaz 08/06/2006
//*-- Modified : D. Gonzalez-Diaz 12/06/2006

/////////////////////////////////////////////////////////////////////
//
//  HRpcDigitizer digitizes data, puts output values into
//  cal data category
//
//
/////////////////////////////////////////////////////////////////////

void HRpcDigitizer::initParContainer() {
  fGeomCellPar=(HRpcGeomCellPar *)gHades->getRuntimeDb()->getContainer("RpcGeomCellPar");
  fDigiPar    =(HRpcDigiPar *)gHades->getRuntimeDb()->getContainer("RpcDigiPar");
}

HRpcDigitizer::HRpcDigitizer(void) {

  fGeantRpcCat=0;
  fCalCat=0;
  fKineCat=0;
  fGeomCellPar=0;
  fDigiPar=0;
  fLoc.set(3,0,0,0);
  iterGeantRpc=0;
  iterRpcCal=0;
}

HRpcDigitizer::HRpcDigitizer(Text_t *name,Text_t *title) :
  HReconstructor(name,title) {

  fGeantRpcCat=0;
  fCalCat=0;
  fKineCat=0;
  fGeomCellPar=0;
  fDigiPar=0;
  fLoc.set(3,0,0,0);
  iterGeantRpc=0;
  iterRpcCal=0;
}

HRpcDigitizer::~HRpcDigitizer(void) {
}

Bool_t HRpcDigitizer::init(void) {

  time_t curtime;

  initParContainer();

  HRpcDetector* rpc=(HRpcDetector*)(gHades->getSetup()->getDetector("Rpc"));   

  if(!rpc){
    Error("init","No Rpc Detector found");
    return kFALSE;
  }

  if (!fGeomCellPar){
    Error("init","No RpcGeomCellPar Parameters");
    return kFALSE;
  }
  if (!fDigiPar){
    Error("init","No RpcDigiPar Parameters");
    return kFALSE;
  }

  fKineCat = gHades->getCurrentEvent()->getCategory(catGeantKine);  
  if (!fKineCat) {
    Error("HRpcDigitizer::init()","HGeant kine input missing");
    return kFALSE;
  }

  fGeantRpcCat = gHades->getCurrentEvent()->getCategory(catRpcGeantRaw);  
  if (!fGeantRpcCat) {
    Error("HRpcDigitizer::init()","HGeant RPC input missing");
    return kFALSE;
  }

  //Build the Calibration category

  fCalCat = gHades->getCurrentEvent()->getCategory(catRpcCal);  
  if (!fCalCat) {
    fCalCat=rpc->buildMatrixCategory("HRpcCalSim",0.5);  
    gHades->getCurrentEvent()->addCategory(catRpcCal,fCalCat,"Rpc");
  }

  iterGeantRpc = (HIterator *)fGeantRpcCat->MakeIterator("native");
  iterRpcCal   = (HIterator *)fCalCat->MakeIterator("native");
  
  time(&curtime);

  return kTRUE;
}

Int_t HRpcDigitizer::execute(void) {

  HGeantRpc*  geantrpc = 0;
  HRpcCalSim* cal      = 0;
  HGeantKine* kine     = 0;
  
  Int_t   RefL=-1, RefR=-1, Ref=-1,
    prevRefL=-1, prevRefR=-1;
  
  Float_t tofL=-999, tofR=-999, QL=-999, QR=-999;
  Float_t prevTofL=-999, prevTofR=-999, 
  prevQL=-999, prevQR=-999;
    

  Float_t geaTof   = 0.;
  Float_t geaEner  = 0.;
  Float_t geaX     = 0.;
  Float_t geaXLoc  = 0.;
  Float_t geaY     = 0.;
  Float_t geaZ     = 0.;     
  Float_t geaMom   = 0.;
  Float_t geaLen   = 0.;     
  Float_t geaLocLen= 0.;     

  iterGeantRpc->Reset();   
  
  //loop over the HGeantrpc objects, i.e, over the gaps

   while ((geantrpc=(HGeantRpc *)iterGeantRpc->Next())!=0) {

     if(geantrpc->getDetectorID()<0) continue; //Hit in virtual volume EBOX

     geantrpc->getHit(geaX, geaY, geaZ, geaTof, geaMom, geaEner); //[mm], [ns], [MeV] in module ref system
     geantrpc->getTLength(geaLen, geaLocLen); 

     fLoc[0] = geantrpc->getSector();
     fLoc[1] = geantrpc->getColumn();
     fLoc[2] = geantrpc->getCell();


     //Load the relevant parameters

     HRpcGeomCellParCell& cell  = (*fGeomCellPar)[fLoc[0]][fLoc[1]][fLoc[2]];
     HRpcDigiParCell& digi_cell = (*fDigiPar)[fLoc[0]][fLoc[1]][fLoc[2]];

     Float_t D;
    
     //Change to detector coordinates (origin at the cell center)

     D       =  cell.getLength();                    // Cell length in mm             
     geaXLoc =  geaX - (cell.getX()) - D/2;          
     //Note that cell.getX() corresponds to the left corner

     //Get parameters from digitizer
     Float_t sigma_T, sigma_el, vprop, Pgap, t_o = 1.05-0.968, T_smearing=0;

     //Factor 1.182 in sigma_T is 'ad hoc', so that after the algorithm
     //the time resolution results indeed to be sigma_T

     vprop    =  digi_cell.getVprop();                       //[mm/ns]   [def: 200 mm/ns]  
     sigma_el = (digi_cell.getSigmaX())*2/vprop/sqrt(2.);    //[ns]      [def: 0.08/sqrt(2)]
     sigma_T  = 1.5*(digi_cell.getSigmaT())/1000;            //[ns]      [def: 0.065]     
     //FIXME: sigma_T is not correct in parameter container. FIX this to 80 ps and 8 mm sigma
     Pgap     = 1 - sqrt(sqrt(1-digi_cell.getEff()));        //          [def: 0.75]  

     if(gRandom->Uniform(1)>Pgap) continue; //efficiency/gap: the particle was not detected!

     cal = (HRpcCalSim*) fCalCat->getObject(fLoc);   // test if cell in use

     if(cal) {
       prevRefL       = cal->getRefLDgtr();
       prevRefR       = cal->getRefRDgtr();
       prevTofL       = cal->getLeftTime();
       prevTofR       = cal->getRightTime();
       prevQL         = cal->getLeftCharge();
       prevQR         = cal->getRightCharge();
     }

     else {
       prevTofL = prevTofR = 100000.;
       cal = (HRpcCalSim*) fCalCat->getSlot(fLoc);  
       if(!cal) cout<<"Error: could not allocate a new slot in HRpcCalSim!"<<endl;
       cal = new(cal) HRpcCalSim;
     }

     Ref = fGeantRpcCat->getIndex(geantrpc);

     RefL=Ref;
     RefR=Ref;

     T_smearing = gRandom->Gaus(t_o, sigma_T); //Common to left and right!. Intrinsic RPC fluctuations

     tofL  = geaTof + T_smearing + (D/2 - geaXLoc)/vprop + gRandom->Gaus(0,sigma_el);   
     tofR  = geaTof + T_smearing + (D/2 + geaXLoc)/vprop + gRandom->Gaus(0,sigma_el);

     //Subtracting the dependence of the time of flight on the cell length 
     tofL  = tofL-D/2/vprop; 
     tofR  = tofR-D/2/vprop;

     //FIXME: subtract also t_o

     //This is crucial. Takes the fastest in the left and in the right

     if(tofL > prevTofL) {
       tofL    = prevTofL;
       RefL    = prevRefL;
     }
     if(tofR > prevTofR) {
       tofR    = prevTofR;
       RefR    = prevRefR; 
     }

     // It must be decided if Charge must be generated here or not!. Dependence with the physics??.
     // Could be also possible to add an intermediate Cal level

     QL=0.0;
     QR=0.0;

     cal->setRefLDgtr(RefL);
     cal->setRefRDgtr(RefR);
     cal->setTrackLDgtr(((HGeantRpc*)fGeantRpcCat->getObject(RefL))->getTrack());
     cal->setTrackRDgtr(((HGeantRpc*)fGeantRpcCat->getObject(RefR))->getTrack());

     cal->setLeftTime(tofL);
     cal->setRightTime(tofR);
     cal->setLeftCharge(QL);
     cal->setRightCharge(QR);
     cal->setAddress(fLoc[0], fLoc[1], fLoc[2]);

     //Loop over tracks for finding the impact of the mother at the entrance of the RPC (EBOX) 
     //First: Left side

     Int_t index        = -1;

     Int_t detectorID   =  0;
     Int_t track_mother = -1;
     
     track_mother = ((HGeantRpc*)fGeantRpcCat->getObject(RefL))->getTrack();

     while(detectorID>=0){

       if (track_mother == 0) {
	 index = -999;
	 //this means that the algorithm could not find the mother. It should not happen
	 break;
       }

       kine         = (HGeantKine*)fKineCat->getObject(track_mother-1);

       if(kine->getNRpcHits()==0) {
	 track_mother = kine->getParentTrack();
	 continue;
	 //This means that the mother was a secondary produced inside the RPC structure but has no 
	 //hit on it. Unlikely, but happens, and geantrpc would not exist in that case [!]. Therefore,
	 //the loop must go on and search for the mother of the mother.
       }
       
       index        =  kine->getFirstRpcHit(); //Look for the first hit of the track
       geantrpc     = (HGeantRpc*)fGeantRpcCat->getObject(index);
       detectorID   = geantrpc->getDetectorID();
       track_mother = kine->getParentTrack();
     }
   
     cal->setRefL(index);
     cal->setTrackL(((HGeantRpc*)fGeantRpcCat->getObject(index))->getTrack());

     //Second: Right side

     index        = -1;
     detectorID   = 0;

     track_mother = ((HGeantRpc*)fGeantRpcCat->getObject(RefR))->getTrack();

     while(detectorID>=0){
       
       if (track_mother == 0) {
	 index = -999;
	 break;
       }

       kine         = (HGeantKine*)fKineCat->getObject(track_mother-1);

       if(kine->getNRpcHits()==0) {
	 track_mother = kine->getParentTrack();
	 continue;
       }

       index        =  kine->getFirstRpcHit();
       geantrpc     = (HGeantRpc*)fGeantRpcCat->getObject(index);
       detectorID   = geantrpc->getDetectorID();
       track_mother = kine->getParentTrack();
     }
     
     cal->setRefR(index);
     cal->setTrackR(((HGeantRpc*)fGeantRpcCat->getObject(index))->getTrack());
     //End of algorithm for finding mother particle

   }
   
   return 0;
}

ClassImp(HRpcDigitizer)
