//*-- Author : A.Sadovsky <sadovski@fz-rossendorf.de> (04.11.2004); Special thanks to A.Ivashkin, V.Pechenov and A.Rustamov
//*-- Last modified : 10/08/2005 by Ilse Koenig

#include <iostream>
#include "hrktrackBF.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "hdetector.h"
#include "hiterator.h"
#include "hcategory.h"
#include "hmdctrkcand.h"
#include "hmdcgetcontainers.h"
#include "hrktrackB.h"
#include "hrungekutta.h"
#include "hmetamatch.h"
#include "htofcluster.h"
#include "htofclustersim.h"
#include "hgeomvolume.h"
#include "hspectrometer.h"
#include "hmdcdetector.h"
#include "hmdcgeompar.h"
#include "htofinogeompar.h"
#include "htofgeompar.h"
#include "hshowergeometry.h"
#include "hspecgeompar.h"
#include "tofdef.h"
#include "hmdcseg.h"
#include "hmdchit.h"
#include "showerdef.h"
#include "hmdctrackgfieldpar.h"
#include "hmatrixcategory.h"
#include "showertofinodef.h"
#include "hgeantmdc.h"
#include "hkickplane2.h"
#include "hgeantkine.h"
#include "hgeomtransform.h"
#include "hmdctrackddef.h"
#include "hmdctrackgdef.h"
#include "hmdctrackgspline.h"
#include "hgeomvector.h"
#include "hsplinetrack.h"
#include "hmagnetpar.h"
#include "hshowerhittoftrack.h"
#include "hmdcsizescells.h"

using namespace std;

//_HADES_CLASS_DESCRIPTION 
/////////////////////////////////////////////////////////////////////////////
//
// This class implements Runge Kutta method of momentum calculation
// into Hydra framework. Output is compartible with General Tracking Scheme
// User valuable output is HRKTrackB class in a ROOT tree
// Now it works with all 4 MDCs present
//
/////////////////////////////////////////////////////////////////////////////

ClassImp(HRKTrackBF)

HRKTrackBF::HRKTrackBF() {
  // DUMMY constructor:
  // Initial momentum is taken as a self guess (mode=0)
  mode=0;
  clear();
}


HRKTrackBF::HRKTrackBF(Text_t *name, Short_t m) : HReconstructor(name,name) {
  // Mode m = {0,1,2} Take initial momentum from: 0-self guess, 1-from KickPlane, 2-from SplineTrack
  mode=m;
  clear();
}

void HRKTrackBF::clear() {
  // sets all pointers to NULL
  pMagnet           =0;
  field             =0;
  fieldFactor       =0.;
  fSpecGeomPar      =0;
  fGetCont          =0;
  fTofGeometry      =0;
  fTofinoGeometry   =0;
  fShowerGeometry   =0;
  fCatMetaMatch     =0;
  fMetaMatchIter    =0;
  fCatMdcTrkCand    =0;
  fCatMdcSeg        =0;
  fCatMdcHit        =0;
  fSplineTrack      =0;
  fKickTrack        =0;
  fCatKine          =0;
  fCatShower        =0;
  fCatTof           =0;
  fCatTofCluster    =0;
  fCatRKTrack       =0;
  pMetaMatch        =0;
  pMdcTrkCand       =0;
  pSplineTrack      =0;
  pKickTrack        =0;
  pTofHit           =0;
  pTofCluster       =0;
  pShowerHitTofTrack=0;
  pRungeKutta       =0;

  for(Int_t m=0;m<4;m++){
      for(Int_t s=0;s<6;s++){
	  mdcInstalled[m][s]=kFALSE;
      }
  }
}

HRKTrackBF::~HRKTrackBF() {
  // destructor
  if (pRungeKutta) {
    delete pRungeKutta;
    pRungeKutta=0;
  }
  HMdcSizesCells::deleteCont();
}


Bool_t HRKTrackBF::init(){
  // sets pointers to all categories and the parameter containers
  if (gHades) {
    HRuntimeDb *rtdb=gHades->getRuntimeDb();
    HSpectrometer *spec=gHades->getSetup();
    HEvent *event=gHades->getCurrentEvent();
    if (!event) return kFALSE;

    field=(HMdcTrackGFieldPar*)(rtdb->getContainer("MdcTrackGFieldPar"));
    fSpecGeomPar=(HSpecGeomPar*)(rtdb->getContainer("SpecGeomPar"));
    pMagnet=(HMagnetPar*)( rtdb->getContainer("MagnetPar"));
    fGetCont=HMdcGetContainers::getObject();
    pMSizesCells = HMdcSizesCells::getObject();

    // TOF-geometry-for-metaEnergy loss calculation
    if (spec->getDetector("Tof")) { // has the user set up a TOF?
      fTofGeometry = (HTofGeomPar *)rtdb->getContainer("TofGeomPar");
    }
    // TOFINO/Shower-geometry-for-metaEnergy loss calculation
    if (spec->getDetector("Shower")&&spec->getDetector("Tofino")) {
      fTofinoGeometry = (HTofinoGeomPar *)rtdb->getContainer("TofinoGeomPar");
      fShowerGeometry = (HShowerGeometry *)rtdb->getContainer("ShowerGeometry");
    }

    // Categories
    fCatMetaMatch=event->getCategory(catMetaMatch);
    if (!fCatMetaMatch) return kFALSE;
    fMetaMatchIter=(HIterator*)fCatMetaMatch->MakeIterator();
    if (!fMetaMatchIter) return kFALSE;
    fCatMdcTrkCand=event->getCategory(catMdcTrkCand);
    if (!fCatMdcTrkCand) return kFALSE;
    fCatMdcSeg=event->getCategory(catMdcSeg);
    if (!fCatMdcSeg) return kFALSE;
    fCatMdcHit=event->getCategory(catMdcHit);
    if (!fCatMdcHit) return kFALSE;

    // momentum guess if any
    if (mode==1) { //- use KickTrack as initial guess for momentum
      fKickTrack=event->getCategory(catKickTrack123B);
      if(!fKickTrack) {
        Error("init",
              "KickTRack123 is used as initial momentum guess, but was not initialized before Runge-Kutta");
        return kFALSE;
      }
    } else if (mode==2) { //- use SplineTrack as initial guess for momentum
      fSplineTrack=event->getCategory(catSplineTrack);
      if (!fSplineTrack) {
        Error("init",
              "Spline is used as initial momentum guess, but was not initialized before Runge-Kutta");
        return kFALSE;
      }
    }

    //- META detectors part
    fCatTof=event->getCategory(catTofHit);
    if (!fCatTof) return kFALSE;
    fCatTofCluster=event->getCategory(catTofCluster);
    if (!fCatTofCluster) Warning("init","No catTofCluster in input!");

    if (!event->getCategory(catGeantKine)) {
      fCatShower=event->getCategory(catShowerHitTof);
      if (!fCatShower) return kFALSE;
    } else {
      fCatShower=event->getCategory(catShowerHitTofTrack);
      if(!fCatShower) return kFALSE;
    }

    //- Here we get category HRKTrackB to use it as an output in "execute()"
    fCatRKTrack=event->getCategory(catRKTrackB);
    if (!fCatRKTrack) {
      Int_t size[2]={6,600};
      fCatRKTrack=new HMatrixCategory("HRKTrackB",2,size,0.5);
      if (fCatRKTrack) event->addCategory(catRKTrackB,fCatRKTrack,"RKTrackB");
    }
    return kTRUE;
  } // end if (gHades)
  return kFALSE;
}


Bool_t HRKTrackBF::reinit(){
  // creates The Runge Kutta objects and sets the field and MDC geometry parameters
  // calculates norm vectors on TOF and Tofino modules
  if (!pRungeKutta) {
    pRungeKutta = new HRungeKutta();
  }
  if (pMagnet->hasChanged()) {
    fieldFactor=pMagnet->getScalingFactor();
    if(pMagnet->getPolarity()<0){ fieldFactor=-fieldFactor; } // changed!
    pRungeKutta->setField(field->getPointer(),fieldFactor);
  }

  if (!pMSizesCells->initContainer()) {
    Error("reinit()","HMdcSizesCells is not initialzed!");
    return kFALSE;
  }
  if (pMSizesCells->hasChanged()) {
    // Geometry transformation from module to sector coord system for MDCs
    HGeomTransform gtrMod2Sec;
    for(Int_t is=0; is<6; is++) {
      HMdcSizesCellsSec& fSCSec = (*pMSizesCells)[is];
      if(&fSCSec == 0) continue; // sector is not active
      secTrans[is]=*(fSCSec.getLabTrans());
      for(Int_t im=0; im<4; im++) {
        HMdcSizesCellsMod& fSCMod=fSCSec[im];
        if (&fSCMod) {
          gtrMod2Sec = *(fSCMod.getSecTrans());
	  pRungeKutta->setMdcPosition(is,im,gtrMod2Sec);
	  mdcInstalled[im][is]=kTRUE;
        }
      }
    }
  }

  //Geometry of the TOF and TOFINO/SHOWER-Hit (needed by MetaEloss calculation)
  // (normale on each module in the sector coordinate system) 
  for(Int_t is=0; is<6; is++) {
    if (fTofinoGeometry&&fTofinoGeometry->hasChanged()) {
      HModGeomPar *module = fTofinoGeometry->getModule(is,0);
      HGeomTransform modTrans( module->getLabTransform());
      modTrans.transTo(secTrans[is]);
      HGeomVector r0_mod(0.0, 0.0, 0.0);
      centerTofino[is]=modTrans.transFrom(r0_mod);
      HGeomVector rz_mod(0.0, 0.0, 1.0);
      normVecTofino[is] = modTrans.transFrom(rz_mod) - centerTofino[is];
    }
    if (fShowerGeometry&&fShowerGeometry->hasChanged()) {
      HModGeomPar *module = fShowerGeometry->getModule(is,0);
      HGeomTransform modTrans( module->getLabTransform());
      modTrans.transTo(secTrans[is]);
      HGeomVector r0_mod(0.0, 0.0, 0.0);
      HGeomVector rz_mod(0.0, 0.0, 1.0);
      normVecShower[is] = modTrans.transFrom(rz_mod) - modTrans.transFrom(r0_mod);
    }
    if (fTofGeometry&&fTofGeometry->hasChanged()) {
      for(Int_t im=0; im<8; im++) {
        HModGeomPar *module = fTofGeometry->getModule(is,im);
        HGeomTransform modTrans(module->getLabTransform());
        modTrans.transTo(secTrans[is]);
        HGeomVector r0_mod(0.0, 0.0, 0.0);
        HGeomVector rz_mod(0.0, 0.0, 1.0);
        normVecTof[is][im] = modTrans.transFrom(rz_mod) - modTrans.transFrom(r0_mod);
      }
    }
  }

  return kTRUE;
}


Int_t HRKTrackBF::execute(){
  // 
  success = kTRUE;
  fMetaMatchIter->Reset();
  while ((pMetaMatch=(HMetaMatch*)(fMetaMatchIter->Next()))!=0) {
    sector = pMetaMatch->getSector();
    system = pMetaMatch->getSystem();
    pMdcTrkCand=(HMdcTrkCand*)fCatMdcTrkCand->getObject(pMetaMatch->getTrkCandInd());
    if(!pMdcTrkCand) { continue; }
    pMdcSeg1 = (HMdcSeg*)fCatMdcSeg->getObject(pMdcTrkCand->getSeg1Ind());
    pMdcSeg2 = (HMdcSeg*)fCatMdcSeg->getObject(pMdcTrkCand->getSeg2Ind());
    if (!pMdcSeg1 || !pMdcSeg2) { continue; }

    sectorloc.set(1,sector);

    // We calculate the initial point and direction for each segment.
    // This vector is used in Runge Kutta to calculate the hit points in each MDC plane.
    Double_t u1pos[3], u1dir[3], u2pos[3], u2dir[3];

    // we have to define if a missing hit point should be artificially
    // calculate to define at least 3 points for the RK algorithm. This
    // is done to keep the efficiency high (in special if the the MDC
    // cluster finder is running in "mixed mode").
    // The additional point will be always added in the inner MDC if this
    // chamber is in the setup as the influence of the magnetic field is
    // stronger in the outer chambers. In case the inner chamber is missing
    // in the setup the outer segment will be used.
    Bool_t doFakeHit=kFALSE;

    Int_t nHitsCheck[2]={0};
    if(pMdcSeg1->getHitInd(0)!=-1) nHitsCheck[0]++;
    if(pMdcSeg1->getHitInd(1)!=-1) nHitsCheck[0]++;
    if(pMdcSeg2->getHitInd(0)!=-1) nHitsCheck[1]++;
    if(pMdcSeg2->getHitInd(1)!=-1) nHitsCheck[1]++;
    

    if(mdcInstalled[0][pMdcSeg1->getSec()] &&
       mdcInstalled[1][pMdcSeg1->getSec()] &&
       nHitsCheck[0]==1 && nHitsCheck[1]==1)
    {   // if both inner chambers are installed
	// and the sum of inner and outer MDCHits is
        // equal 2
	doFakeHit=kTRUE;
    }
    Int_t nHits=calcPosDirFromSegment(pMdcSeg1,0,&u1pos[0],&u1dir[0],doFakeHit);    // inner segment

    if(nHits==1                            &&
       mdcInstalled[2][pMdcSeg2->getSec()] &&
       mdcInstalled[3][pMdcSeg2->getSec()] &&
       nHitsCheck[0]==1 && nHitsCheck[1]==1)
    {   // We could not fix the number of hits in inner segment.
	// Now we try for the outer segment. If both outer chambers
	// are installed and the sum of inner and outer MDCHits is
        // equal 2
	doFakeHit=kTRUE;
    }
    else {
	doFakeHit=kFALSE;
    }

    nHits     +=calcPosDirFromSegment(pMdcSeg2,1,&u2pos[0],&u2dir[0],doFakeHit);  // outer segment
    if (nHits<3) {
        Warning("execute()","Less than 3 points for RK detected. This should never happen!");
	continue;
    }

    Int_t splineTrackIndex=-1;
    Int_t kickTrackIndex123=-1;
    momentumGuess=-1000000.0; //indicates the absence of initial guess
    pRK=-1000000.0;
    qRK=0;

    if (mode==2) {
      // Initial momentum guess from SplineTrack
      splineTrackIndex = pMetaMatch->getSplineInd();
      if (splineTrackIndex>=0) { 
        pSplineTrack = (HSplineTrack*)fSplineTrack->getObject(splineTrackIndex);
        pRK=pSplineTrack->getP();
        qRK=pSplineTrack->getPolarity();
      } else {
        Warning("execute()","SplineTrack did not provide information for RK");
      }
    } else if (mode==1 || momentumGuess==-1000000.0) {
      // Initial momentum guess from KickTrack123
      kickTrackIndex123 = pMetaMatch->getKick123Ind();
      if (kickTrackIndex123>=0) {
        pKickTrack = (HBaseTrack*)fKickTrack->getObject(kickTrackIndex123);
        pRK=pKickTrack->getP();
        qRK=pKickTrack->getPolarity();
      } else {
        Warning("execute()","KickTrack did not provide information for RK");
      }
    }

    if (pRK>50.) {
      momentumGuess=qRK*pRK;
    } else if (pRK!=-1000000.0) {
      momentumGuess=qRK*50.;
    }

    // Now do Runge-Kutta momentum tracking and fitting with momentum guess as start value
    if (nHits==4) {
      success=pRungeKutta->fit4Hits(u1pos,u1dir,u2pos,u2dir,multSig,sector,momentumGuess);
    } else {
      if (momentumGuess!=-1000000.0) {
        success=pRungeKutta->fit3Hits(u1pos,u1dir,u2pos,u2dir,multSig,sector,momentumGuess);
      } else {
        //Runge Kutta does not work with MDC123 in case no initial guess was specified
        Warning("execute()",
                "HRKTrackF: MDC123 mode, but user did not provide momentum guess - RK failed!");
        success=kFALSE; // this prevents from writing garbage into HRKTrackB
      }
    }
    if (!success) {      
      fillData(pMdcSeg1,pMdcSeg2,pSplineTrack); //fill HRKTrack with the initial segment information
      continue;
    }

    chiqRK=pRungeKutta->getChi2();
    pRK=pRungeKutta->getPfit();
    if (pRK>0) {
      qRK=+1;
    } else {
      pRK*=-1.;
      qRK=-1;
    }
    if (pRK>10000.) pRK=10000.0;

    // Now propogate it till vertex
    pRungeKutta->traceToVertex(pMSizesCells);

    // Now we take into account META information if any
    Int_t metaInd = pMetaMatch->getMetaHitInd();
    Float_t  xTof,yTof,zTof;      // meta hit
    HGeomVector *pathCorrPos=0, *pathCorrNorm=0;
    if (metaInd<0) {
      // If META does not exist, we just write tracking part into HRKTrackB
      tof          = -1.0;
      trackLength  = -1.0;
      beta         = -1.0;
      mass2        = -1.0;
      metaeloss    = -1.0;
      RKxyzMETA[0] = -1000.0;
      RKxyzMETA[1] = -1000.0;
      RKxyzMETA[2] = -1000.0;
      pRungeKutta->traceToMETA(centerTofino[sector],normVecTofino[sector],pathCorrPos,pathCorrNorm);
    } else {    // META exists
       if (system==0) { // for TOFINO/Shower
         pShowerHitTofTrack=(HShowerHitTofTrack*)fCatShower->getObject(metaInd);
         tof=pShowerHitTofTrack->getTof();
         metaeloss=pShowerHitTofTrack->getADC();
	 pShowerHitTofTrack->getLabXYZ(&xTof,&yTof,&zTof);
         metaNormVec=normVecShower[sector];
         pathCorrPos=&centerTofino[sector];
         pathCorrNorm=&normVecTofino[sector];
       } //-end-if-system==0--
       if (system==1) { // for TOF
         if (pMetaMatch->getTofClusterSize()<=0) { //no TofCluster was found
           pTofHit=(HTofHit*)fCatTof->getObject(metaInd);
           tof=pTofHit->getTof();
           metaeloss=pTofHit->getEdep();
	   pTofHit->getXYZLab(xTof,yTof,zTof);
           metaNormVec=normVecTof[sector][(Int_t)pTofHit->getModule()];
         } else { // RK-is in TofCluster mode and TofCluster was found
           pTofCluster=(HTofCluster*)fCatTofCluster->getObject(metaInd);
           tof=pTofCluster->getTof();
           metaeloss=pTofCluster->getEdep();
           pTofCluster->getXYZLab(xTof,yTof,zTof);
           metaNormVec=normVecTof[sector][(Int_t)pTofCluster->getModule()];
         }
       }

       // We collected META hit coordinates
       // and transform it into the sector coordinate system
       pointMeta.setXYZ(xTof,yTof,zTof);
       pointMeta=secTrans[sector].transTo(pointMeta);
       xTof=pointMeta.getX();
       yTof=pointMeta.getY();
       zTof=pointMeta.getZ();

       // Trace with Runge Kutta from outer MDC to META
       pRungeKutta->traceToMETA(pointMeta,metaNormVec,pathCorrPos,pathCorrNorm);
       RKxyzMETA[0] = pRungeKutta->getXtrackOnMETA() - xTof;
       RKxyzMETA[1] = pRungeKutta->getYtrackOnMETA() - yTof;
       RKxyzMETA[2] = pRungeKutta->getZtrackOnMETA() - zTof;

       // Calculate beta=length/tof
       trackLength = pRungeKutta->getTrackLength(); 
       beta        = 1.0e6*trackLength/tof/TMath::C(); //dist [mm], tof[ns]
       mass2       = pRK*pRK*(1-beta*beta)/beta/beta;

    } // end if META exists
    fillData(pMdcSeg1,pMdcSeg2,pSplineTrack); //finally we fill HRKTrackB
  } // end while pMetaMatch loop
  return 0;
}


Int_t HRKTrackBF::calcPosDirFromSegment(HMdcSeg* pSeg,Int_t ioseg, Double_t* pos, Double_t* dir, Bool_t flag) {
  // calculates the initial point and direction from a MDC track segment
  // If a hit index in the outer segment is -1, the error is set to -1.
  // This hit will then be discarded in Runge Kutta. For not fitted outer
  // segments, the hits get larger errors. 
  Int_t index[2]={pSeg->getHitInd(0),pSeg->getHitInd(1)};
  Int_t nHits=2;
  if (ioseg==0) {                         // inner segment
    for (Int_t i=0;i<2;i++) {
      if (index[i]==-1 && flag==kFALSE) { // missing hit
        multSig[2*i] = multSig[2*i+1] = -1.;
        nHits--;
      } else multSig[2*i] = multSig[2*i+1] = 1.;
    }
  } else {                                // outer segment
    for (Int_t i=0;i<2;i++) {
      if (index[i]==-1) {                 // missing hit
	  if (flag==kFALSE) {             // normal mode
	      multSig[4+2*i] = multSig[4+2*i+1] = -1.;
	      nHits--;
	  } else {                        // create fake hit
              if(pSeg->getChi2()>=0) multSig[4+2*i] = multSig[4+2*i+1] = 1.;
              else                   multSig[4+2*i] = multSig[4+2*i+1] = 5.;
	  }

      } else if (pSeg->getChi2()>=0) {    // fitted single hit
	  multSig[4+2*i] = multSig[4+2*i+1] = 1.;
      } else {                            // not fitted single hit
	  multSig[4+2*i] = multSig[4+2*i+1] = 5.;
      }
    }
  }
  if (nHits>0) {
    Double_t hpi=TMath::Pi()/2.;
    pos[0] = pSeg->getR()*cos(pSeg->getPhi()+hpi); // 1st-tracklet's initial point
    pos[1] = pSeg->getR()*sin(pSeg->getPhi()+hpi);
    pos[2] = pSeg->getZ();
    dir[0] = cos(pSeg->getPhi())*sin(pSeg->getTheta()); // 1st-tracklet's direction
    dir[1] = sin(pSeg->getPhi())*sin(pSeg->getTheta());
    dir[2] = cos(pSeg->getTheta());
  }
  return nHits;
}


HRKTrackB* HRKTrackBF::fillData(HMdcSeg* segment1,HMdcSeg* segment2,HSplineTrack* spline) {
  // fills the Runge Kutta track object
  TObject* slot=0;
  HRKTrackB* rkt=0;
  Int_t indexRK;
  slot=fCatRKTrack->getNewSlot(sectorloc,&indexRK);
  if (!slot) {
    Error("fillData","No slot available");
  } else {
      //----------------------------------------------------------------------
      #warning locally changed isRungeKuttaAccepted. Has to be removed when meaning of isSplineAccepted is changed
      // work arround : function setRungeKuttaInd() sets
      // isAccepted=kTRUE no matter what it was set by TofClusterF.
      // So, this depends on the order of tasks
      // metamatch -> splinetask -> tofcluster -> RK
      Bool_t isAccepted=kTRUE;
      if(pMetaMatch->getSplineInd()>-1){
	  // spline object already exists isSplineAccepted()
	  // should be always true if there is no overlap in
	  // TOF/TOFINO and the TofClustF runs in front of RK
	  isAccepted=pMetaMatch->isSplineAccepted();
      }
      pMetaMatch->setRungeKuttaInd(indexRK); // add index in METAMATCH
      // work arround : set is accepted to old flag
      // Should work, no matter if TofClusterF runs berfore of not
      if(isAccepted==kFALSE) pMetaMatch->unsetRungeKuttaAccept();
      //----------------------------------------------------------------------
    rkt=new(slot) HRKTrackB;
    rkt->setSector(sector);
    rkt->setShowerHitInd(pMetaMatch->getShowerHitInd());
    rkt->setTofHitInd(pMetaMatch->getTofHitInd());
    rkt->setZ(    segment1->getZ()     ,segment1->getErrZ()     );
    rkt->setR(    segment1->getR()     ,segment1->getErrR()     );
    rkt->setTheta(segment1->getTheta() ,segment1->getErrTheta() );
    rkt->setPhi(  segment1->getPhi()   ,segment1->getErrPhi()   );
    rkt->setP(pRK,0.);
    rkt->setPolarity(qRK);
    rkt->setTof(tof);
    rkt->setMetaEloss(metaeloss);
    rkt->setTarDist(spline->getTarDist());
    rkt->setIOMatching(spline->getIOMatching());

    if (success) {
      rkt->setChiq(chiqRK);
      rkt->setZSeg1RK(pRungeKutta->getZSeg1());
      rkt->setRSeg1RK(pRungeKutta->getRSeg1());
      rkt->setThetaSeg1RK(pRungeKutta->getThetaSeg1());
      rkt->setPhiSeg1RK(pRungeKutta->getPhiSeg1());
      rkt->setZSeg2RK(pRungeKutta->getZSeg2());
      rkt->setRSeg2RK(pRungeKutta->getRSeg2());
      rkt->setThetaSeg2RK(pRungeKutta->getThetaSeg2());
      rkt->setPhiSeg2RK(pRungeKutta->getPhiSeg2());
      rkt->setTofDist(trackLength);
      rkt->setBeta(beta);
      rkt->setMass2(mass2, 0.);
      rkt->setMETAdx(RKxyzMETA[0]);
      rkt->setMETAdy(RKxyzMETA[1]);
      rkt->setMETAdz(RKxyzMETA[2]);
    } else {
      //something failed, or momentum reconstruction was not even called
      rkt->setChiq(1000000.);
      rkt->setZSeg1RK(segment1->getZ());
      rkt->setRSeg1RK(segment1->getR());
      rkt->setThetaSeg1RK(segment1->getTheta());
      rkt->setPhiSeg1RK(segment1->getPhi());
      rkt->setZSeg2RK(segment2->getZ());
      rkt->setRSeg2RK(segment2->getR());
      rkt->setThetaSeg2RK(segment2->getTheta());
      rkt->setPhiSeg2RK(segment2->getPhi());
      rkt->setTofDist(spline->getTofDist());
      rkt->setBeta(   spline->getBeta());
      rkt->setMass2(  spline->getMass2(), 0.);
      rkt->setMETAdx(-10000.);
      rkt->setMETAdy(-10000.);
      rkt->setMETAdz(-10000.);
    }
  }
  return rkt;
}
