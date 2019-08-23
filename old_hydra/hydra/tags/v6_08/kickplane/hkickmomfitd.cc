#include "hkickmomfitd.h"
#include "hades.h"
#include "hmatrixcategory.h"
#include "hspecgeompar.h"
#include "hspectrometer.h"
#include "hruntimedb.h"
#include "hmdcclus.h"
#include "hmdcdef.h"
#include "hevent.h"
#include "kickdef.h"
#include "hkickmdctrk.h"
#include "hgeomvolume.h"

//*-- Author : M. Sanchez
//*-- Modified : 09/05/2001 by M.Sanchez

///////////////////////////////////////////////////////////
// HKickMomFitD
//
//   Momentum fit for Dubna candidate search using the inner and
// outer lever arms.
//
//   This class prepares the input for HKickMatch using the Dubna candidate
// search. (No calibration parameters are needed)
//
///////////////////////////////////////////////////////////

HKickMomFitD::HKickMomFitD(Text_t name[],Text_t title[]) 
  : HReconstructor(name,title) {
}

HKickMomFitD::~HKickMomFitD(void) {
}

Bool_t HKickMomFitD::init(void) {
  if (gHades) {
    HEvent *event = gHades->getCurrentEvent();
    HRuntimeDb *rtdb = gHades->getRuntimeDb();
    HSpectrometer *spec = gHades->getSetup();
    if ( rtdb && event && spec ) {
      
      fClusterCat = event->getCategory(catMdcClus);
      if (!fClusterCat) return kFALSE;
      fClusterIter = (HIterator *)fClusterCat->MakeIterator();
      
      fKickPlane = (HKickPlane *)rtdb->getContainer("KickPlaneMDC3");

      fSpecGeometry = (HSpecGeomPar *)rtdb->getContainer("SpecgeomPar");

      fKickMdcTrkCat = event->getCategory(catKickMdcTrk);
      if (!fKickMdcTrkCat) {
	Int_t sizes[2] = {6, 100};
	fKickMdcTrkCat = new HMatrixCategory("HKickMdcTrk",2,sizes,1.);
	if (!fKickMdcTrkCat) return kFALSE; 
	event->addCategory(catKickMdcTrk,fKickMdcTrkCat,"Tracks");
      }
      
    } else {
      Error( "init", "rtdb && event && spec not verified");
      return kFALSE;
    }
  } else {
    Error("init","gHades not found");
    return kFALSE;
  }
  fLaLocation.set(2,0,1); //  Second lever arm
  fSecLoc.set(1,0);

  return kTRUE;
}

Int_t HKickMomFitD::execute(void) {
  HMdcClus *cluster=0;
  HGeomVector kick,far,alpha1,alpha2;
  const HGeomVector &zero = fSpecGeometry->getTarget(0)->getTransform().getTransVector();
  Double_t g1,g2,f1,f2,ptg,ptf,err;
  Int_t polarity;
  Double_t p,pull;

  fMaxPull = 100;

  for (fLaLocation[0]=0; fLaLocation[0] < 6; fLaLocation[0]++) {
    fSecLoc[0] = fLaLocation[0];
    fClusterIter->Reset();
    fClusterIter->gotoLocation(fLaLocation);
    
    while ( (cluster=(HMdcClus *)fClusterIter->Next()) != 0 ) {
      kick.setX( cluster->getXTarg() );
      kick.setY( cluster->getYTarg() );
      kick.setZ( cluster->getZTarg() );
      
      far.setX( cluster->getX() );
      far.setY( cluster->getY() );
      far.setZ( cluster->getZ() );
      
      alpha1 = kick - zero;
      alpha1 /= alpha1.length();
      
      alpha2 = far - kick;
      alpha2 /= alpha2.length();

      g1 = TMath::ATan2(alpha1.getZ(), alpha1.getY());
      g2 = TMath::ATan2(alpha2.getZ(), alpha2.getY());
      f1 = TMath::ATan2(alpha1.getY(), alpha1.getX());
      f2 = TMath::ATan2(alpha2.getY(), alpha2.getX());

      if ( g2 - g1 > 0) polarity = 1; else polarity = -1;
      
      ptg = fKickPlane->getPt(kick,err,polarity);
      ptf = fKickPlane->getPtPhi(kick,err,polarity);

      p = ptg / ( 2. * TMath::Sin((g2 - g1) / 2.0) );

      pull = 2 * p * sin( (f2 - f1) / 2. ) -
	fKickPlane->getPtPhi(kick,err,polarity);

      #warning "Implicit cut momentum>60"
      if (TMath::Abs(pull) < fMaxPull && p > 60) { //Extra filtering
	fillData(zero,kick,far,p,pull,polarity,cluster);
      }
    }
  }
  return 0;
}

void HKickMomFitD::fillData(const HGeomVector &zero,HGeomVector &kick,
			    HGeomVector &far, Double_t p, Double_t pull,
			    Int_t pol,HMdcClus *cluster) {
  TObject *slot=0;
  HKickMdcTrk *track=0;

  slot = fKickMdcTrkCat->getNewSlot(fSecLoc);
  if (slot) {
    track = new(slot) HKickMdcTrk;
    track->setZeroPos(zero);
    track->setKickPos(kick);
    track->setOuterPos(far);
    track->setMomentum(p);
    track->setPull(pull);
    track->setCharge(pol);
    track->setSector(cluster->getSec());
  }
}

Bool_t HKickMomFitD::finalize(void) {
  return kTRUE;
}


ClassImp(HKickMomFitD)
