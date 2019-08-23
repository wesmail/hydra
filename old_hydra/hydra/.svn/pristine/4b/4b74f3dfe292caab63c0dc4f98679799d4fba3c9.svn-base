// File: hrichdigitizer.cc
//
// Author:Author : Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
// Last update by Laura Fabbietti: 02/05/16 20:04:44
//
/////////////////////////////////////////////////////////////////////////////
//_______________________________________________________
//  HRichDigitizer
//  .....(Witeks description)
// For each digitized pad the corresponding track numbers of the 
// direct hits and photons are stored in a linear category
// (catRichTrack<-HRichTrack) that is made sortable by the pad address.
// Each pad object of the HRichCalSim class has 2 members, nTrack1, nTrack2,
// that give the indexes of the first and the last track numbers in the linear
// category (catRichTrack) corresponding to a pad.
// This method allows to store as many particle track numbers as the pad 
// " sees ". 
////////////////////////////////////////////////////////////////////////////
#include "hrichcalparcell.h"
#include "hrichdigitizer.h"
#include <stdlib.h>
#include "hevent.h"
#include "hmatrixcatiter.h"
#include "hgeantrich.h"
#include "hcategory.h"
#include "hlinearcatiter.h"
#include "hlocation.h"
#include "hrichdigitisationpar.h"
#include "hrichgeometrypar.h"
#include "hrichanalysispar.h"
#include "hdebug.h"
#include "hades.h"
#include "hrichpad.h"
#include "hrichwiresignal.h"
#include "hspectrometer.h"
#include "hrichdetector.h"
#include "hruntimedb.h"
#include "hrichcalpar.h"
#include "hrichcalsim.h"
#include "richdef.h"
#include "hrichtrack.h"
#include <TVector.h>
#include <TF1.h>
#include <iostream.h>
#include "hrichpadfilter.h"
#include <TRandom.h>

ClassImp(HRichDigitizer)


//----------------------------------------------------------------------------
HRichDigitizer::HRichDigitizer(Text_t *name, Text_t *title,Bool_t kNoise) : HReconstructor(name,title)
{
  fCerIter = NULL;
  fDirIter = NULL;
  fTrackIter = NULL;
  fCalIter  = NULL;

  fCerInputCat = NULL;
  fDirInputCat = NULL;
  fOutputCat = NULL;
  fTrackOutputCat = NULL;


  for (Int_t i = 0; i < 9; i++) fDigitPadMatrix[i] = 0.0F;

 
  fQupper = 0.0F;
  fChargePerChannel = 0.0F;
  fSigmaValue = 0.0F;
  fYShift = 0.0F;
  isActiveNoise = 0; 

  fFactor1 = 0.0F;
  fFactor2 = 0.0F;
  fParam1 = 0.0F;
  fParam2 = 0.0F;
  fFactor1Sig = 0.0F;
  fFactor2Sig = 0.0F;

  a1 = 0.0F;
  a2 = 0.0F;
  b1 = 0.0F;
  b2 = 0.0F;
  
  cont=0;

  if(kNoise) isActiveNoise = 1;
  else isActiveNoise = 0;
}
//============================================================================

//----------------------------------------------------------------------------
HRichDigitizer::HRichDigitizer()
{
  fCerIter = NULL;
  fDirIter = NULL;
  fTrackIter = NULL;  
  fCalIter = NULL;   

  fCerInputCat = NULL;
  fDirInputCat = NULL;
  fOutputCat = NULL;
  fTrackOutputCat = NULL;

  for (Int_t i = 0; i < 9; i++) fDigitPadMatrix[i] = 0.0F;

  fQupper = 0.0F;
  fChargePerChannel = 0.0F;
  fSigmaValue = 0.0F;
  fYShift = 0.0F;
 

  fFactor1 = 0.0F;
  fFactor2 = 0.0F;
  fParam1 = 0.0F;
  fParam2 = 0.0F;
  fFactor1Sig = 0.0F;
  fFactor2Sig = 0.0F;
  param21 = 0.0F;
  param11 = 0.0F;

  a1 = 0.0F;
  a2 = 0.0F;
  b1 = 0.0F;
  b2 = 0.0F;
}
//============================================================================

//----------------------------------------------------------------------------
HRichDigitizer::~HRichDigitizer() {

  if (fCerIter) delete fCerIter;
  if (fDirIter) delete fDirIter;
  if (fTrackIter) delete fTrackIter;
  if (fCalIter) delete fCalIter;
  
  fChargeOnWireList.Delete();
  fTrackNumberList.Delete();
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichDigitizer::init() {
   
  lNrEvent = 0;
  fDigitisationPar = NULL;
  fGeometryPar = NULL;

  

  HCategory *pCat;
  HRichDetector *pRichDet = (HRichDetector*)gHades->getSetup()
                                                  ->getDetector("Rich");

  HRuntimeDb* rtdb=gHades->getRuntimeDb();

 
  HRichCalPar *pCalPar =
      (HRichCalPar*)rtdb->getContainer("RichCalPar");
  if (!pCalPar) {
      pCalPar= new HRichCalPar;
      pCalPar->SetName("RichCalPar");
      if (pCalPar) {
	  ((HRichCalPar*)pCalPar)->
	      setSetup(pRichDet->getSectors(),
		       pRichDet->getRows(),
		       pRichDet->getColumns());
	  rtdb->addContainer(pCalPar);
      }
  }
  setCalPar(pCalPar);

  HRichGeometryPar *pGeomPar = 
      (HRichGeometryPar*)rtdb->getContainer("RichGeometryParameters");
  if (pGeomPar == NULL) {
     pGeomPar = new HRichGeometryPar;
     rtdb->addContainer(pGeomPar);
  }
  setGeometryPar(pGeomPar);
  if (pGeomPar == NULL) return kFALSE;

  HRichDigitisationPar *pDigitPar = 
     (HRichDigitisationPar*)rtdb->getContainer("RichDigitisationParameters");
  if (pDigitPar == NULL) {
     pDigitPar = new HRichDigitisationPar;
     rtdb->addContainer(pDigitPar);
  }
  setDigitisationPar(pDigitPar);
  if (pDigitPar == NULL) return kFALSE;

  pCat = gHades->getCurrentEvent()->getCategory(catRichGeantRaw);
  if (pCat == NULL) {
     pCat = pRichDet->buildCategory(catRichGeantRaw);
     if (pCat == NULL) return kFALSE;
     else gHades->getCurrentEvent()
                ->addCategory(catRichGeantRaw, pCat, "Rich");
  }
  setCerInputCat(pCat);
  fCerIter = (HIterator*)getCerInputCat()->MakeIterator();

  pCat = gHades->getCurrentEvent()->getCategory(catRichGeantRaw+1);
  if (pCat == NULL) {
     pCat = pRichDet->buildCategory(catRichGeantRaw+1);
     if (pCat == NULL) return kFALSE;
     else gHades->getCurrentEvent()
                ->addCategory(catRichGeantRaw+1, pCat, "Rich");
  }
  setDirInputCat(pCat);
  fDirIter = (HIterator*)getDirInputCat()->MakeIterator();

  pCat = gHades->getCurrentEvent()->getCategory(catRichTrack);
  if (pCat == NULL) {
     pCat = pRichDet->buildCategory(catRichTrack);
     if (pCat == NULL) return kFALSE;
     else gHades->getCurrentEvent()->addCategory(catRichTrack, pCat, "Rich");
  }
 
  setTrackOutputCat(pCat);

  fTrackIter = (HIterator*)getTrackOutputCat()->MakeIterator();

  pCat = gHades->getCurrentEvent()->getCategory(catRichCal);
  if (pCat == NULL) {
     pCat = pRichDet->buildMatrixCat("HRichCalSim",1); // ETLF 02/23/2000
     if (pCat == NULL) return kFALSE;
     else gHades->getCurrentEvent()->addCategory(catRichCal, pCat, "Rich");
  }
  setOutputCat(pCat);
  fCalIter = (HIterator*)getOutputCat()->MakeIterator();
 
return kTRUE;

}
//============================================================================
Bool_t HRichDigitizer::reinit(){
    fYShift = ((HRichGeometryPar*)fGeometryPar)->getSectorShift();
    fYShift = fYShift/cos(20/57.3);
   
  
    fQupper = ((HRichDigitisationPar*)fDigitisationPar)->getQupper();
    
    fSigmaValue = ((HRichDigitisationPar*)fDigitisationPar)->getSigmaValue();
    fChargePerChannel =
	((HRichDigitisationPar*)fDigitisationPar)->getChargePerChannel();
    
    
     fFactor1 = ((HRichDigitisationPar*)fDigitisationPar)->getFactor1();
    fFactor2 = ((HRichDigitisationPar*)fDigitisationPar)->getFactor2();
    fParam1 = ((HRichDigitisationPar*)fDigitisationPar)->getParameter1();
    fParam2 = ((HRichDigitisationPar*)fDigitisationPar)->getParameter2();
    fFactor1Sig = ((HRichDigitisationPar*)fDigitisationPar)->getFactor1Sig();
    fFactor2Sig = ((HRichDigitisationPar*)fDigitisationPar)->getFactor2Sig();


    param21 = 0.0F;
    param11 = 0.0F;
   
    return kTRUE;
}
//----------------------------------------------------------------------------
HRichDigitizer::HRichDigitizer(const HRichDigitizer& source) {
 // dummy
}
//============================================================================

//----------------------------------------------------------------------------
HRichDigitizer& HRichDigitizer::operator=(HRichDigitizer &source) {
 if (this != &source) {
  // dummy
 }
return *this;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichDigitizer::execute()
{
 
    //
    // At the end of the execute() the TClonesArray that contains
    // the track numbers is sorted by the pad addreses. Then
    // every CalSim object is retrieved from the catRichCal
    // container to assign to each pad the corresponding nTrack1, nTrack2.
    // These 2 numbers are the indexes of the track nbs. in the TClonesArray 
    // HRichTrack.
    //

      cont++;
        if (cont%1000==0) cout<<"Executing Digitizer at evt   "<<cont<<endl;
	//cout<<"Executing Digitizer at evt   "<<cont<<endl;
      HGeantRichPhoton *pCherenkovHit;
      HGeantRichDirect *pDirectHit;
      
      lNrEvent++;
      Int_t fCerNr = 0, fDirNr = 0;
      fChargeOnWireList.Delete();
      fTrackNumberList.Delete();
      // cout<<"   fChargeOnWireList.GetSize()  "<<fChargeOnWireList.GetSize()<<"  fTrackNumberList.GetSize()  "<<fTrackNumberList.GetSize()<<endl;
      fCerIter->Reset();
      while((pCherenkovHit = (HGeantRichPhoton*)fCerIter->Next())) {
	  // cout<<" loop on the photons  : "<<fCerNr <<"  photon  with energy "<<pCherenkovHit->getEnergy()<<endl;
	  digitiseCherenkovHits(pCherenkovHit);
	  fCerNr++;
      }
      
      fDirIter->Reset();
      while((pDirectHit = (HGeantRichDirect*)fDirIter->Next())) {
	  //  cout<<" loop on the photons  : "<<endl;
	  digitiseDirectHits(pDirectHit);
	  fDirNr++;
      }
      
      digitisePads();
      
      // *** added by LF 02/23/2000 ***
      ((HLinearCategory*)getTrackOutputCat())->sort();   // sort track objects
      
      HRichCalSim *calsimobj;
      HRichTrack *trackobj;
      
      fCalIter->Reset();
      
      Int_t temp;
      Int_t indTrack1 = 0;
      
      while((calsimobj = (HRichCalSim*)fCalIter->Next())) { // loop over CalSim
	  // here the cut-off threshold (3 sigma) is applied to each pad.
	  // Previously the electronic noise was calculated and the real
	  // signal added ( with a statistic fluctuation of sigma) for
	  // each pad.
    
    
	  if(!checkPad(calsimobj)) {
	      
	      calsimobj->setCharge(0);//LF
	      
	  }
	  else {//LF
	      temp = 0;
	      Int_t addPad = calsimobj->getAddress();
	      fTrackIter->Reset();
	      
	      // in the updateCharge member for each fired pad the energy of the "hitting" photon is added. At the same time the number of the photons hits is stored, so that after having digizited all pads the average photon energy for each pad can be calculated as follows.
	      
	      if(calsimobj->getNTrack1()) calsimobj->setEnergy(calsimobj->getEnergy()/calsimobj->getNTrack1());
	      
	      calsimobj->setNTrack1(0);
	      while((trackobj = (HRichTrack*)fTrackIter->Next())) { // loop over tracks	    
		  Int_t addTrack = trackobj->getAddress();
		  if (trackobj->getTrack()==0)  cout<<"track null in digitizer  "<<endl;  
		  if(addPad == addTrack){ // 1st track found
		      indTrack1 = ((HLinearCategory*)getTrackOutputCat())->getIndex(trackobj);
		      if (temp!=addPad) {
			  calsimobj->setNTrack1(indTrack1);
			  calsimobj->setNTrack2(indTrack1); 
		      } else {
			  calsimobj->setNTrack2(indTrack1);
		      }
		      temp = addTrack;
		  }
		  else if(temp!=0) break;
	      }
	  }
      } // *** end of LF addition 
      
      // The HRichPadFilter class allows to get rid of the zero we got after the threshold check of all the pads.
      
      HRichPadFilter padFilter;
      ((HMatrixCategory*)getOutputCat())->filter(padFilter);
      
  return 0;
}
//============================================================================


//----------------------------------------------------------------------------
Int_t HRichDigitizer::checkPad(HRichCalSim *calSim){

    // this function applies the Threshold to each pad.
    // The procedure is the same used for real data, the charge 
    // on the pad consists of the coupled signal plus the noise,
    // the mean value is 0. To take into account that the qMean
    // is a float number a random number between 0 and 0,5 is added
    // to both charge on pad and threshold, then both numbers are casted 
    // in integer.
    
    Float_t fSigmaPad = 0;    
    Float_t charge = calSim->getCharge();
    Int_t iRow = calSim->getRow();
    Int_t iCol = calSim->getCol();
    Int_t iSec = calSim->getSector();
    loc.set(3, iSec, iRow,iCol);
    HRichCalParCell* pCell = ((HRichCalParCell*)((HRichCalPar*)getRichCalPar())->getObject(loc));
    if (pCell)
    {
	 fSigmaPad= ((HRichCalPar*)getRichCalPar())->getSigma(loc);
	if(fSigmaPad<0.5) fSigmaPad= 0.6;
    }else  fSigmaPad = 0.6;

    Float_t fDraw ;
    fDraw = rand()/(double)RAND_MAX;
    fDraw= fDraw / 2; 
    //fDraw is introduced to take into account that the mean 
    //pedestal value  is a float in the reality and 0 for simulation.
    //(cfr MakeNoise)
    Int_t iCharge = (Int_t)( charge + fDraw);
    fDraw = rand()/(double)RAND_MAX;
    fDraw= fDraw / 2;
    Int_t iThreshold =   (Int_t) ( fSigmaValue*fSigmaPad + fDraw + 0.5);

    //cout<<" Threshold  "<<iThreshold<<" pad charge "<<calSim->getCharge()<<endl;
    if( iCharge > iThreshold) {
	calSim->setCharge((Float_t)iCharge);
	//cout<<" Threshold  "<<iThreshold<<" pad charge "<<calSim->getCharge()<<endl;
	//cout<<" in checkPad  "<<calSim->getCharge()<<endl;
	return iCharge;
    }
    else return 0;
}

//============================================================================
//----------------------------------------------------------------------------
Float_t HRichDigitizer::GaussFun(Float_t mean, Float_t sigma) {

  Double_t x,y,z;
 
  do {
     y = rand()/(double)RAND_MAX;
  } while (!y);
  z = rand()/(double)RAND_MAX;
  x = z*6.2831853;
  return mean + sigma*float(sin(x)*sqrt(-2.0*log(y)));
}
//============================================================================



//----------------------------------------------------------------------------
Bool_t HRichDigitizer::calcQE(const Float_t photlen) {

 Int_t i;
 Float_t draw;

 Int_t bins = ((HRichDigitisationPar*)fDigitisationPar)->getQEBinsNr();
 Float_t* photlength = ((HRichDigitisationPar*)fDigitisationPar)
                        ->getPhotonLenArray();
 Float_t* photeffic = ((HRichDigitisationPar*)fDigitisationPar)
                        ->getPhotonEffArray();

 draw = rand()/(double)RAND_MAX;

 for (i = 0; i < bins; i++)
  if (i == 0) {
   if (photlen < ((photlength[i]+photlength[i+1])/2))
    if (draw > photeffic[i]) { return kFALSE; } 
     else { return kTRUE; }
  } else if (i == bins-1) {
   if (photlen >= ((photlength[i-1]+photlength[i])/2))
    if (draw > photeffic[i]) { return kFALSE; } 
     else { return kTRUE; }
  } else if (((photlength[i-1]+photlength[i])/2) <= photlen &&
             ((photlength[i]+photlength[i+1])/2) > photlen) {
   if (draw > photeffic[i]) { return kFALSE; } 
    else { return kTRUE; }
  }
 return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichDigitizer::getWireNr(Float_t xhit) {

 Int_t fWiresNr = ((HRichGeometryPar*)fGeometryPar)->getWiresPar()
                                                   ->getWiresNr();
 Float_t fWiresDist = ((HRichGeometryPar*)fGeometryPar)->getWiresPar()
                                                       ->getDistWire();

 Int_t i, nWireNr = -1;
 Float_t Xwir;

 for (i = 0; i < fWiresNr; i++) {
  Xwir = ((HRichGeometryPar*)fGeometryPar)->getWiresPar()->getWire(i)
                                                         ->getXWire();
  if ((xhit >= Xwir - fWiresDist) && (xhit < Xwir + fWiresDist)) {
   nWireNr = i;
   break;
  }
 }

 return nWireNr;
}
//============================================================================

//----------------------------------------------------------------------------

void HRichDigitizer::calcChargeOnWire(Int_t sector, Float_t xhit, Float_t yhit, Float_t nTrack, Float_t nFlag ,Float_t ene) {
    //
    // this function calculates the charge on each wire and saves it in a TList
    // moreover, it saves the track number and the corresponding flag
    // (cf HRichTrack) in a second TList. 
    // The charge is calculated according to two different exponential function
    // obtained fitting the total charge distribution of the photon candidates.
    //( OEM data). 
    // That means that the so calculated charge isn't in reality the 
    // original charge deposited on the wire but the total amount of the charge
    // coupled to the pads. Therefore it isn't any longer necessary to 
    // use an additional coupling factor. ( 75%)

    TVector * gTrack = new  TVector(1,2,nTrack,nFlag,"END"); // change by LF
 Int_t fHitedWireNr;
 
 Float_t fX, fY, fQ;
 HRichFrame *pFrame = ((HRichGeometryPar*)fGeometryPar)->getFramePar();

  fX = fY = fQ = 0.0;
  fHitedWireNr = getWireNr(xhit);
  if (fHitedWireNr == -1) return;


  fX  = ((HRichGeometryPar*)fGeometryPar)->getWiresPar()
             ->getWire(fHitedWireNr)->getXWire();
  
  fY  = yhit;
  //charge amplification for different sectors
  Float_t* a1 = ((HRichDigitisationPar*)fDigitisationPar)->getExp1Nor();
  Float_t* a2 = ((HRichDigitisationPar*)fDigitisationPar)->getExp1Nor();
  Float_t* b1 = ((HRichDigitisationPar*)fDigitisationPar)->getExp2Nor();
  Float_t* b2 = ((HRichDigitisationPar*)fDigitisationPar)->getExp2exp();
  
  Float_t q1,qMax,fac,I1,I2;
  Float_t charge;
  qMax = ((HRichDigitisationPar*)fDigitisationPar)->getQupper();
  q1 = log(b1[sector]/a1[sector])/(a2[sector]-b2[sector]);
  I1 = a1[sector]/a2[sector]*(exp(a2[sector]*q1)-1);
  I2= -b1[sector]/b2[sector]*exp(b2[sector]*q1);
  fac = I1/(I1+I2);
  Float_t fDraw,fDraw1,fDraw2;
  fDraw1 = fDraw2 = fDraw = 0;
  Int_t cTRUE ;
  fDraw = rand()/(double)RAND_MAX;
  cTRUE = 0;
  while(!cTRUE){
      if(fDraw<fac){
	  
	  fDraw1= rand()/(double)RAND_MAX;
	  
	  charge = 1/a2[sector]*(log(fDraw1*( exp(a2[sector]*q1)-1 ) + 1));
	  if(charge<=q1) {
	      
	      cTRUE = 1;
	  }
	  else cTRUE =0;
      }
      else{
	  fDraw2= rand()/(double)RAND_MAX;
	  charge = -q1 + 1/b2[sector]*log(-fDraw2*(exp(b2[sector]*qMax) - exp(b2[sector]*q1) ) );
	  if(charge>q1) {
	      
	      cTRUE = 1;
	  }
	  else cTRUE =0;
      }
  }
    fQ = charge*((HRichDigitisationPar*)fDigitisationPar)->getChargePerChannel();
  
    // cout<<" charge on the wire "<<fQ<<endl;    
  
  if (fQ > 0. && pFrame->isOut(fX, fY) == kFALSE) {
     
   fChargeOnWireList.Add(new HRichWireSignal(sector,fHitedWireNr,fQ,fX,fY,ene));
   fTrackNumberList.Add(gTrack);
  }

}//============================================================================

//----------------------------------------------------------------------------
void HRichDigitizer::digitiseCherenkovHits(HGeantRichPhoton *pCerHit) {
    //
    // for every photon hit on a pad the resulting charge on a wire
    // is calculated and the track number of the photon parent is stored.
    // (cf. calcChargeOnWire).
    // 
 HRichFrame *pFrame = ((HRichGeometryPar*)fGeometryPar)->getFramePar();
     Float_t xHitcm = (pCerHit->getX()/10.0F);//HGeant output is in mm!!!
     Float_t yHitcm = (pCerHit->getY()/10.0F + fYShift);
 if (calcQE(pCerHit->getEnergy()) == kTRUE &&
     // one day it will be additional condition if it is mirror or detector
     pFrame->isOut(xHitcm,yHitcm) == kFALSE){
     //  cout<<" photon energy   "<<pCerHit->getEnergy()<<endl;
      calcChargeOnWire(pCerHit->getSector(),xHitcm,yHitcm,
                       pCerHit->getTrack(),0,pCerHit->getEnergy());
     
 } 


}
//============================================================================

//----------------------------------------------------------------------------
void HRichDigitizer::digitiseDirectHits(HGeantRichDirect *pDirHit) {
    //
    // for every direct hit (charge particle hitting the RICH or ionizing
    // the gas near the surface of the photon detector)  the 
    // resulting charge on the wires is calculated and the track number
    // of the charged particle hitting the pad is stored.
    // (cf. calcChargeOnWire) 

 Int_t i, fStepsNr;
 Float_t fStepLength, fNewX, fNewY;
 HRichFrame *pFrame = ((HRichGeometryPar*)fGeometryPar)->getFramePar();

     Float_t xHitcm = pDirHit->getX()/10.0F;//HGeant output is in mm!!!
     Float_t yHitcm = pDirHit->getY()/10.0F + fYShift ;

 if (pDirHit->getEnergyLoss() > 0.0F &&
     // one day it will be additional condition if it is mirror or detector
     pFrame->isOut(xHitcm, yHitcm) == kFALSE) {

  fStepsNr = (Int_t)(1+((HRichDigitisationPar*)fDigitisationPar)
                         ->getElectronsNr()*pDirHit->getEnergyLoss());
  if (fStepsNr > 5000) {
   fStepsNr = 5000;
   cerr << "Warning in <HRichDigitizer::digitiseDirectHits>: "
        << "Number of maximum steps exceeded!\n";
  }
  fStepLength = pDirHit->getTrackLengthInPhotDet()/((Float_t)fStepsNr);

  for (i = 0; i < fStepsNr; i++) {
   fNewX = xHitcm + (i+1) * fStepLength * 
           sin(pDirHit->getTheta()) * cos(pDirHit->getPhi());
   fNewY =  yHitcm + (i+1) * fStepLength * 
           sin(pDirHit->getTheta()) * sin(pDirHit->getPhi());
   Int_t dTrack, dId;
   pDirHit->getTrack( dTrack,dId);
   calcChargeOnWire(pDirHit->getSector(), fNewX, fNewY,dTrack,1,-1);
  }

 }

}
//============================================================================
//----------------------------------------------------------------------------
HRichPad* HRichDigitizer::translateCorners(HRichPad *pPad, Float_t dx, Float_t dy) {

// check what you want to translate - probably real corners

 Int_t i;
 HRichPad *fpTranslatedPad = new HRichPad(*pPad);

 for (i = 0; i < fpTranslatedPad->getCornersNr(); i++) {
  fpTranslatedPad->getCorner(i)->setX(fpTranslatedPad->getCorner(i)->getX() - dx);
  fpTranslatedPad->getCorner(i)->setY(fpTranslatedPad->getCorner(i)->getY() - dy);
 }
 fpTranslatedPad->setXmin(fpTranslatedPad->getXmin() - dx);
 fpTranslatedPad->setYmin(fpTranslatedPad->getYmin() - dy);
 fpTranslatedPad->setXmax(fpTranslatedPad->getXmax() - dx);
 fpTranslatedPad->setYmax(fpTranslatedPad->getYmax() - dy);

 // the ownership of this pointer is passed to outer variable-pointer
 return fpTranslatedPad;
}
//===========================================================================
//----------------------------------------------------------------------------
void HRichDigitizer::makeNoiseOnPads() {
    // this method produces electronic noise on the pads. It samples for each pad a charge value on a gaussian distribution centred in zero with a width fSigmaValue and assign drawn the charge to the pad.
    Int_t i, j;
    Float_t fCharge;
    HRichCalSim *pCalSim = NULL;
    Float_t fSigmaPad = 0.;
    HRichGeometryPar * geomPar = (HRichGeometryPar *)getGeometryPar();
    Int_t maxPads = geomPar->getPadsNr();
   
    
    for (i = 0; i < 6; i++)
	if (geomPar->getSector(i) > 0) 
	    for (j = 0; j < maxPads; j++)
		if (geomPar->getPadStatus(j)) {
		    loc.set(3, i, geomPar->calcRow(j), geomPar->calcCol(j));
		    HRichCalParCell* pCell = ((HRichCalParCell*)((HRichCalPar*)getRichCalPar())->getObject(loc));
		    if (pCell)
		    {
			fSigmaPad = ((HRichCalPar*)getRichCalPar())->getSigma(loc);
			if(fSigmaPad<0.5) fSigmaPad= 0.6;
		    }else  fSigmaPad= 0.6;
		    fCharge = GaussFun(0.0, fSigmaPad);  
		    
		    pCalSim = (HRichCalSim*)((HMatrixCategory*)getOutputCat())->getObject(loc);
		    if (pCalSim == NULL) {
			pCalSim = (HRichCalSim*)((HMatrixCategory*)getOutputCat())->getSlot(loc);
			if (pCalSim != NULL) {
			    pCalSim = new(pCalSim) HRichCalSim;
			    //        pCalSim->setEventNr(lNrEvent);
			    pCalSim->setSector(loc[0]);
			    pCalSim->setCol(loc[2]);
			    pCalSim->setRow(loc[1]);
			}
			
			if (pCalSim != NULL) pCalSim->addCharge(fCharge);
		    }
		} // eof loop over all pads
}
//============================================================================

//----------------------------------------------------------------------------
void HRichDigitizer::digitisePads() {
    //
    // for each wire on which some charge has been deposited
    // the corresponding coupled pads are calculated.
    // The function updateCharge creates a HRichCalSim obj if
    // the charge on the pad is greater than zero.
    // A cut off threshold is applied to each pad in the execute func.
    // after the digitilasation of all the hits.
    // If the pad is hit twice in the same event, the charges
    // corresponding to the 2 different hits are added.
    // The particle track number is passed to the function 
    // updateCharge, too. 
    //
    // cout<<"  Digitiize Event +++++++++++++++++"<<endl;
 Int_t i, j, xcenter, ycenter;
 Int_t sector;
 Float_t energyPhot =0;
 Int_t seed =0;
 TRandom ran1;
 ran1.SetSeed(0);
 HRichPadTab *pPadsPar = ((HRichGeometryPar*)fGeometryPar)->getPadsPar();
 
 HRichPad *pPad = NULL;
 HRichPad *pTmpPad = NULL;

 TVector * nTrackTemp =NULL;

 HRichWireSignal *pCharge = NULL;
 

 if(isActiveNoise) makeNoiseOnPads();

 for (i = 0; i < fChargeOnWireList.GetSize(); i++) {
  
  for (j = 0; j < 9; j++) fDigitPadMatrix[j] = 0.;
  //  cout<<" digitize new photon "<<endl;
  pCharge = (HRichWireSignal*)fChargeOnWireList.At(i);
  
  nTrackTemp =(TVector*)fTrackNumberList.At(i);

  energyPhot = pCharge->getEnergy();  

  sector = pCharge->getSector();
  pPad = pPadsPar->getPad(pCharge->getX(), pCharge->getY());
  xcenter = pPad->getPadX();
  ycenter = pPad->getPadY();

  Float_t yRel = TMath::Abs((pCharge->getY() - pPad->getYmin())/(pPad->getYmax()-pPad->getYmin()));
  //cout<<"******************************************"<<endl;
  //cout<<" hit on the "<<(pCharge->getWireNr()%2)<<"  wire, at position "<< yRel<<endl;
  pTmpPad = translateCorners(pPad, pCharge->getX(), pCharge->getY());
  // do not forget to delete it at the end of loop



  // Given a certain charge deposited on the wire ( this charge is drawn on the Polya function, that we approsimate with 2 exponential, cfr calcChargeOnWire()), a set of coupling parameters extracted from experimental data (OEM) is used to calculate the response of the pad to the charge.
  // First the charge on the central pad is calculated and then that on the neighbours.

 // ------------------------ central pad (0,0)
  
  
 
  
  param11 = param21 = 0;
  Float_t charge4 = 0;

  if (pTmpPad->getPadFlag() == 2) {
      //First we calculate the charge on the central pad taking as contibutions on the side pads, 0 for the weak coupling and the mean for the strong one.    
       
      //  fDigitPadMatrix[4] = q4Calc(pCharge->getCharge(),yRel,fFactor1,fFactor2);
      // Then we recalculate the coupling parameter for the strong coupling drawing a number on the landau distribution centered in the mean value.

      //param11 = (ran.Landau(fDigitPadMatrix[4]*fFactor1,fDigitPadMatrix[4]*fFactor1Sig))/fDigitPadMatrix[4];
      //  cout<<" fFactor1 "<<fFactor1<<endl;

      param11 = (ran1.Landau(fFactor1,fFactor1Sig));
      //      cout<<"  param11 "<<param11<<endl;
      // The weak coupling is charaterized by a quite broad gaussian with a negative mean. A value is drawn accoring to thi gaussian until a positive number is drawn.
      //The so calculated coupling is used lateron to calculate the charges on the neighbours (cfr. calcIndCharge).   
      //param21 =-1;
      // while( param21<0){  
      //cout<<" fFactor2 "<<fFactor2<<endl;
      //param21 =  GaussFun(fDigitPadMatrix[4]*fFactor2,fDigitPadMatrix[4]*fFactor2Sig)/fDigitPadMatrix[4];
      param21 = GaussFun(fFactor2,fFactor2Sig);
      //cout<<" param21 "<<param21<<endl;
	  //}
	  if(param21<0) param21 = 0;
      //cout<<" weak coupling   :"<<param21<<endl;
      //if(param21>0) cout<<" %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"<<endl;
      charge4 = q4Calc(pCharge->getCharge(),yRel,param11,param21);
      if (charge4 < 0.) fDigitPadMatrix[4] = 0.;
      else{
	  updateCharge(sector, pPad,charge4 ,nTrackTemp,energyPhot);
	  fDigitPadMatrix[4] = charge4;
      }
      
      //cout<<" charge on the central pad "<<charge4<<endl;
  }
  //------------------------ loop on other pads
  Int_t iPadInd = 0;
  for (Int_t n =-1;n<2;n++){
      for (Int_t m =-1;m<2;m++){
	  
	  if(m!=0 || n!=0 ){
	      
	      if (pPadsPar->getPad(xcenter+m, ycenter+n) &&
		  pPadsPar->getPad(xcenter+m, ycenter+n)->getPadActive()) {
		  delete pTmpPad;
		  pTmpPad = translateCorners(pPadsPar->getPad(xcenter+m,ycenter+n),pCharge->getX(), pCharge->getY());
		  if (pTmpPad->getPadFlag() == 2){
		      fDigitPadMatrix[iPadInd] = 
			  calcIndCharge(yRel,fDigitPadMatrix[4],
					iPadInd,pCharge->getWireNr()); 
		      //	      		  
			if(iPadInd!=1&&iPadInd!=7)  fDigitPadMatrix[iPadInd] = 
			  GaussFun(fDigitPadMatrix[iPadInd],fDigitPadMatrix[iPadInd]/4.);//?????
		      if (fDigitPadMatrix[iPadInd] < 0.) 
			  fDigitPadMatrix[iPadInd] = 0.;
		      else{
			  
			  updateCharge(sector, pTmpPad, fDigitPadMatrix[iPadInd],nTrackTemp,energyPhot);
		      }
		  }
	      }
	      
	  }
	  iPadInd ++;      
      }
  }
  
  delete pTmpPad;
 }
}
Float_t HRichDigitizer::calcIndCharge(float yCharge,float q4,int iPdaIndex,int iWireNr){

    // iWireNr is used to determine whether the wire is the left or the right one.
    // Even wire numbers correspond to right.  
   
    switch(iPdaIndex){
	
    case 1: {
	
	return (qX(1-yCharge)*q4);
    }
    case 7: {
	
	return (qX(yCharge)*q4);
    }
    case 3: {
	if(iWireNr%2==0) return ( param21* q4);
	else return (param11 * q4);
	  
    }
    case 5:{
	if(iWireNr%2==0) return (param11 * q4);
	else return ( param21* q4);
    }
    case 2:{
	if(iWireNr%2==0) return ( param11 * q4 * qX(1-yCharge));
	else return  ( param21 * q4 * qX(1-yCharge));
    }
    case 8:{
	if(iWireNr%2==0) return ( param11 * q4 * qX(yCharge));
	else return  ( param21 * q4 * qX(yCharge));
    }
    case 6:{
	if(iWireNr%2==0) return ( param21 * q4 * qX(yCharge));
	else return  ( param11 * q4 * qX(yCharge));
    }
    case 0:{
	if(iWireNr%2==0) return  ( param21 * q4 * qX(1-yCharge));
	else return  ( param11 * q4 * qX(1-yCharge));
    }
    }
    return 0;
    
}; 
Float_t HRichDigitizer::qX(Float_t pos){

    Float_t charge = 0;
    Float_t q0 = -0.03;
    Float_t integral =  1/(fParam1*fParam1*q0 + fParam1*fParam2) -
	1/(fParam1*fParam1 + fParam1*fParam2);
   
   
    charge =((q0+fParam2/fParam1)/(1-(q0+fParam2/fParam1)*fParam1*fParam1*integral*pos))-fParam2/fParam1;
    
    if (charge<0) charge = 0;
    return charge;
}
Float_t HRichDigitizer::q4Calc(float charge,float pos,float par1,float par2){

    //the coupling factor (75%) from the wire to the pad has been discarded since the charge distribution on the wire has been calculated fitting pads responses. 

    Float_t gX = 1 + qX(pos) + qX(1-pos);    
    if(gX) return  charge/ ((1+ 2*par1 + 2*par2)* gX);
   
    else return 0;
}  
//----------------------------------------------------------------------------
void HRichDigitizer::updateCharge(Int_t sector, HRichPad* pPad, Float_t charge,TVector * rTrack, Float_t ene) {
    //
    // this function creates an HRichCalSim obj that corresponds
    // to a fired pad, it calls the function updateTrack, 
    // that stores the corresponding track numbers.
    //
    HLocation locat;
    Int_t fCol, fRow;
    HRichCalSim *pCalSim = NULL;
    
    pPad->getPadXY(&fCol, &fRow);
    
    //    cout<<" pad Col "<<fCol<<" pad Row "<<fRow<<endl;
    //cout<<" charge on Pad "<<charge<<endl;
    locat.set(3, sector, fRow, fCol);

    pCalSim = (HRichCalSim*)((HMatrixCategory*)getOutputCat())->getObject(locat);
    if (pCalSim == NULL) {
	pCalSim = (HRichCalSim*)((HMatrixCategory*)getOutputCat())->getSlot(locat);
	if (pCalSim != NULL) {
	    pCalSim = new(pCalSim) HRichCalSim;
	    pCalSim->setEventNr(lNrEvent);
	    pCalSim->setSector(locat[0]);
	    pCalSim->setCol(locat[2]);
	    pCalSim->setRow(locat[1]);
	    updateTrack(pCalSim,locat,rTrack);
	}
    }
    if (pCalSim != NULL){
	pCalSim->addCharge(charge / fChargePerChannel);
	pCalSim->addHit();
	pCalSim->addEnergy(ene);
	updateTrack(pCalSim,locat,rTrack);
    }
    //  cout<<" charge on pad "<<pCalSim->getCharge()<<endl;
}
//============================================================================

//---------------------------------------------------------------------------
void HRichDigitizer::updateTrack(HRichCalSim *pCal,HLocation& loc,TVector * rTrack){
    //
    // this function stores the track numbers of parent 
    // particles of photons and of direct hits in a linear 
    // category (HRichTrack). This category is set sortable.
    // 
 HRichTrack *pRichTrack = NULL;
 Int_t Ad = pCal->getAddress();
  
 fTrackIter->Reset();

 while((pRichTrack = (HRichTrack*)fTrackIter->Next()))
 if((pRichTrack->getTrack()==(*rTrack)(1)) &&
               pRichTrack->getFlag()==(*rTrack)(2) && pRichTrack->getAddress() == Ad) return;
  
 HLocation loc1;
 loc1.set(1,0);
 pRichTrack = (HRichTrack*)((HLinearCategory*)getTrackOutputCat())->getNewSlot(loc1);
 //pRichTrack = (HRichTrack*)((HLinearCategory*)getTrackOutputCat())->getSlot(loc1);
    if (pRichTrack){
       pRichTrack = new (pRichTrack) HRichTrack;
       pRichTrack->setEventNr(pCal->getEventNr());
       //usage of TVector because of used in TList, explicit cast to integer is needed since TVector converts all arguments into double.
       pRichTrack->setTrack((Int_t)((*rTrack)(1))); 
       pRichTrack->setFlag((Int_t)((*rTrack)(2))); 
       pRichTrack->setAddress(Ad);
      
    }
}




Bool_t HRichDigitizer::finalize(){
return kTRUE;
}



















