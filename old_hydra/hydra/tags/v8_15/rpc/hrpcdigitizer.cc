using namespace std;
#include <TRandom.h>
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
#include "hevent.h"
#include "hcategory.h"
#include "hlocation.h"
#include "hrpcdigipar.h"
#include "hrpccalsim.h"
#include <iostream> 
#include <iomanip>

//*-- Author : D. Gonzalez Diaz 08/06/2006

/////////////////////////////////////////////////////////////////////
//
//  HRpcDigitizer digitizes data, puts output values into
//  cal data category
//
//
/////////////////////////////////////////////////////////////////////

void HRpcDigitizer::initParContainer() {
  fDigiPar=(HRpcDigiPar *)gHades->getRuntimeDb()->getContainer("RpcDigiPar");
}

HRpcDigitizer::HRpcDigitizer(void) {

  fGeantCat=0;
  fCalCat=0;
  fDigiPar=0;
  fLoc.set(3,0,0,0);
  iterGeant=0;
  iterRpcCal=0;
}

HRpcDigitizer::HRpcDigitizer(Text_t *name,Text_t *title) :
  HReconstructor(name,title) {

  fGeantCat=0;
  fCalCat=0;
  fDigiPar=0;
  fLoc.set(3,0,0,0);
  iterGeant=0;
  iterRpcCal=0;
}

HRpcDigitizer::~HRpcDigitizer(void) {
}

Bool_t HRpcDigitizer::init(void) {

  time_t curtime;

  initParContainer();

  fGeantCat = gHades->getCurrentEvent()->getCategory(catRpcGeantRaw);  
  if (!fGeantCat) {
    Error("HRpcDigitizer::init()","HGeant RPC input missing");
    return kFALSE;
    }

  fCalCat = gHades->getCurrentEvent()->getCategory(catRpcCal);  
  if (!fCalCat) {
    HRpcDetector* rpc=(HRpcDetector*)(gHades->getSetup()->getDetector("Rpc"));   
    fCalCat=rpc->buildMatrixCategory("HRpcCalSim",0.5);  
    if (!fCalCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catRpcCal,fCalCat,"Rpc");
  }

  iterGeant = (HIterator *)fGeantCat->MakeIterator("native");
  iterRpcCal = (HIterator *)fCalCat->MakeIterator("native");
  
  time(&curtime);

  return kTRUE;
}

Int_t HRpcDigitizer::execute(void) {

  HGeantRpc* geant = 0;
  HRpcCalSim* cal = 0;

  // FIXME: Parameters like vprop, sigma_T, t_o I think they must be stored 
  // in this 'digipar' parameter container, actually. While the cell sizes and
  // positions must be probably part of a geometry file.

  Float_t D, c=300, vprop=2.0/3*c; //velocity in mm/ns
  

  Int_t   NTrackL, NTrackR, NTrack,      //track numbers
    prevNTrackL=-2, prevNTrackR=-2;      //Initialization meant to debug
  Float_t timeL, timeR, totL, totR;
  Float_t prevTimeL, prevTimeR, 
  prevTotL, prevTotR;
    
  Float_t sigma_T = 0.065, t_o = 1.05, T_smearing=0, sigma_el = 0.05/sqrt(2.), Pgap=0.75;
  //resolution = 65 ps for time and 50 ps for position resolution (5 mm).
  //these parameters must be given by the parameter container of the digitizer.

  Float_t geaTof   = 0.;
  Float_t geaEner  = 0.;
  Float_t geaX     = 0.;
  Float_t geaY     = 0.;     
  Float_t geaZ     = 0.;     
  Float_t geaMom   = 0.;
  Float_t geaXm    = 0.;     
  Float_t geaYm    = 0.;
  Float_t geaTheta = 0.;     
  Float_t geaPhi   = 0.;     
  Float_t geaLen   = 0.;     

  iterGeant->Reset();   
  
  // this works only in split mode=2 (as written in Tof digitizer). 
  // FIXME: What is this?? (for 0 and 1 the iterator must be recreated)


  //loop over the HGeantrpc objects, i.e, over the gaps

   while ((geant=(HGeantRpc *)iterGeant->Next())!=0) {

     if(gRandom->Uniform(1)>Pgap) continue; //efficiency/gap: the particle was not detected!

     geant->getHit(geaEner, geaX, geaY, geaZ, geaXm, geaYm, geaTheta,
		   geaPhi, geaTof, geaMom, geaLen); //[mm], [ns], [MeV], [deg]

     if(geaLen<10)               continue; // secondary coming from the last 1 cm are removed
                                           // this distance must be carefully determined

     fLoc[0] = geant->getSector();
     fLoc[1] = geant->getModule();
     fLoc[2] = geant->getCell();

     cal = (HRpcCalSim*) fCalCat->getObject(fLoc);   // test if cell in use

     if(cal) {
       prevNTrackL    = cal->getNTrackL();
       prevNTrackR    = cal->getNTrackR();
       prevTimeL      = cal->getLeftTime();
       prevTimeR      = cal->getRightTime();
       prevTotL       = cal->getLeftTot();
       prevTotR       = cal->getRightTot();
     }

     else {
       prevTimeL = prevTimeR = 100000.;
       cal = (HRpcCalSim*) fCalCat->getSlot(fLoc);  
       if(!cal) cout<<"Error: could not allocate a new slot in HRpcCalSim!"<<endl;
       cal = new(cal) HRpcCalSim;
     }

     HRpcDigiParCell& cell=(*fDigiPar)[fLoc[0]][fLoc[1]][fLoc[2]];
    
     D      = cell.getLength();                    // Cell length in mm             

     NTrack = geant->getTrack();

     NTrackL=NTrack;
     NTrackR=NTrack;

     T_smearing = gRandom->Gaus(t_o, sigma_T); //Common to left and right!. Intrinsic RPC fluctuations

     timeL  = geaTof + T_smearing + (D/2 - geaX)/vprop + gRandom->Gaus(0,sigma_el);   
     timeR  = geaTof + T_smearing + (D/2 + geaX)/vprop + gRandom->Gaus(0,sigma_el);

     //Subtracting the dependence of the time of flight on the cell length 
     timeL  = timeL-D/2/vprop; 
     timeR  = timeR-D/2/vprop;

     //FIXME: subtract also t_o

     //This is crucial. Takes the fastest in the left and in the right

     if(timeL > prevTimeL) {
       timeL   = prevTimeL;
       NTrackL = prevNTrackL;
     }
     if(timeR > prevTimeR) {
       timeR   = prevTimeR;
       NTrackR = prevNTrackR; 
     }

     // It must be decided if ToT must generated here or not!. Dependence with the physics??.
     // Could be also possible to add an intermediate Cal level

     totL=0.0;
     totR=0.0;

     cal->setNTrackL(NTrackL);
     cal->setNTrackR(NTrackR);
     cal->setLeftTime(timeL);
     cal->setRightTime(timeR);
     cal->setLeftTot(totL);
     cal->setRightTot(totR);
     cal->setSector((Char_t) fLoc[0]);
     cal->setModule((Char_t) fLoc[1]);
     cal->setCell((Char_t) fLoc[2]);

   }
  
   return 0;
}

ClassImp(HRpcDigitizer)
