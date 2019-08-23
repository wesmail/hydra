#include "htofclusterf.h"
#include "htofcluster.h"
#include "hades.h"
#include "htofhit.h"
#include "hruntimedb.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hdebug.h"
#include "tofdef.h"
#include "hevent.h"
#include "heventheader.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hgeomvolume.h"
#include "hgeomcompositevolume.h"
#include "hgeomtransform.h"
#include "htofgeompar.h"
#include "hdetgeompar.h"
#include "hgeomvector.h"
#include "hspecgeompar.h"
#include <TMath.h>

//*-- Author :  1/12/2001 D.Zovinec

//////////////////////////////////////////////////////////////////////////////////////
// HTofClusterF Tof cluster finder
//
// Iterates over the hit level of tof data and find a clusters.
//
// Iterates over the hit level of Tof data and finds a clusters.
// The cluster is constructed here from the set of hits with following conditions
// satisfied:
// 1.) the hits are close to each other (same sector, same module, adjacent rod)
// 2.) time difference of two hits is less than 1ns
// 3.) xposition differnece of two hits is less than 200mm
//
// Hits are also included here as the clusters with size=1.
// New tof, xpos, xposAdc, lAmp, rAmp, xlab, ylab, zlab is calculated for the
// cluster here as a weighted mean of values of individual hits in cluster.
// The weight is energy deposited in the rod, i.e. edep.
// The edep of the cluster is sum of the edep's of hits.
//
//////////////////////////////////////////////////////////////////////////////////////

void HTofClusterF::initParContainer(HSpectrometer *spec, HRuntimeDb *rtdb) {
  // Adds to the runtime database "rtdb" the containers needed by the Cluster Finder
  //spec is used to get information of the spectrometer setup.

  fTofGeometry=(HTofGeomPar *)rtdb->getContainer("TofGeomPar");
  if (!fTofGeometry) {
    fTofGeometry=(HTofGeomPar *)spec->createDetGeomPar("Tof");
  }
  fSpecGeometry = (HSpecGeomPar *)gHades->getSetup()->createSpecGeomPar();
  if(!fSpecGeometry){
    fSpecGeometry = new HSpecGeomPar;
    rtdb->addContainer(fSpecGeometry);
  }
}

HTofClusterF::HTofClusterF(void) {
  fHitCat = fClusterCat = NULL;
  fLoc.set(3,0,0,0);
  fpLoc.set(3,0,0,0);
  fCLoc.set(3,0,0,0);
  iterh = NULL;
  iterc = NULL;
}

HTofClusterF::HTofClusterF(Text_t *name,Text_t *title) : HReconstructor (name,title) {
  fHitCat = fClusterCat = NULL;
  fLoc.set(3,0,0,0);
  fpLoc.set(3,0,0,0);
  fCLoc.set(3,0,0,0);
  iterh = NULL;
  iterc = NULL;
}

HTofClusterF::~HTofClusterF(void) {
  if(iterh) delete iterh;
  if(iterc) delete iterc;
}

Bool_t HTofClusterF::init(void) {
  Bool_t r=kTRUE; // Function's return value
  HSpectrometer *spec = gHades->getSetup();
  HRuntimeDb *rtdb = gHades->getRuntimeDb();
  HEvent *ev = gHades->getCurrentEvent(); // Event structure

  if (spec && rtdb && ev) {
    HDetector *tof = spec->getDetector("Tof");

    if (tof) {
      // Parameter containers initialization.
      initParContainer(spec,rtdb);

      fHitCat = ev->getCategory(catTofHit);
      if (!fHitCat) {
        fHitCat = tof->buildCategory(catTofHit);
        if (!fHitCat) return kFALSE;
        else ev->addCategory(catTofHit,fHitCat,"Tof");
      }
      iterh=(HIterator*)fHitCat->MakeIterator("native");

      fClusterCat = ev->getCategory(catTofCluster);
      if (!fClusterCat) {
        fClusterCat = tof->buildCategory(catTofCluster);
        if (!fClusterCat) return kFALSE;
        else ev->addCategory(catTofCluster,fClusterCat,"Tof");
      }
      iterc=(HIterator*)fClusterCat->MakeIterator("native");
    } else {
      Error("init","TOF setup is not defined");
      r = kFALSE; // Notify error to task manager
    }
  } else {
    Error("init","Setup, RuntimeDb or Event structure not found");
    r = kFALSE; // Notify error to task manager
  }
  return r;
}

Int_t HTofClusterF::execute(void) {

#if DEBUG_LEVEL>2
  gDebuger->enterFunc("HTofClusterF::execute");
#endif

  HTofHit *hit=NULL;
  HTofCluster *cluster=NULL;

  Float_t tof, xpos;
  Float_t prevTof = 0.0, prevXpos=0.0;
  Float_t absTd=0.0, absXd=0.0;
  Float_t tof1, tof2, tofm, xpos1, xpos2, xposm;
  Float_t xposAdc1, xposAdc2, xposAdcm;
  Float_t lAmp1, lAmp2, lAmpm, rAmp1, rAmp2, rAmpm;
  Float_t xlab1, xlab2, xlabm, ylab1, ylab2, ylabm, zlab1, zlab2, zlabm;
  Float_t edep1, edep2, edepm, w1, w2;
  Int_t clsize = 0;
  HGeomVector  r;
  Float_t  d, theta, phi;
  Int_t flagAdc, flagAdc1, flagAdc2;
  Float_t rad2deg = 180./TMath::Pi();

  fpLoc[0] = -1;
  fpLoc[1] = -1;
  fpLoc[2] = -1;

  iterh->Reset();
  while ( (hit=(HTofHit *)iterh->Next())!=NULL) {
    fLoc[0]=hit->getSector();
    fLoc[1]=hit->getModule();
    fLoc[2]=hit->getCell();
    tof=hit->getTof();
    xpos=hit->getXpos();

    absTd=TMath::Abs(tof-prevTof);
    absXd=TMath::Abs(xpos-prevXpos);

    if(((fLoc[0]==fpLoc[0])&&(((fLoc[1]*8)+fLoc[2])==((fpLoc[1]*8)+fpLoc[2]+1))) &&
      (absTd<1.0) && (absXd<200.0)){
      if(clsize==0) fCLoc=fpLoc;  // If cluster has been found its location is identical with 1st hit in cluster.
      cluster = (HTofCluster *)fClusterCat->getObject(fCLoc);
      tof1=cluster->getTof();
      xpos1=cluster->getXpos();
      cluster->getXYZLab(xlab1, ylab1, zlab1);
      edep1=cluster->getEdep();
      xposAdc1=cluster->getXposAdc();
      lAmp1=cluster->getLeftAmp();
      rAmp1=cluster->getRightAmp();
      flagAdc1=cluster->getAdcFlag();
      tof2=hit->getTof();
      xpos2=hit->getXpos();
      hit->getXYZLab(xlab2, ylab2, zlab2);
      edep2=hit->getEdep();
      xposAdc2=hit->getXposAdc();
      lAmp2=hit->getLeftAmp();
      rAmp2=hit->getRightAmp();
      flagAdc2=hit->getAdcFlag();
      if((edep1 != 0.0) && (edep2 != 0.0)) {
        w1=edep1;
        w2=edep2;
        edepm=edep1+edep2;
      } else {
        w1=1.0;
        w2=1.0;
        edepm=0.0;
      }

      // Here comes the recalculation of the cluster data.
      // It is a weighted mean of the old cluster data
      // and a data of hit that also belongs to cluster.
      // Weight is the energy deposited.

      tofm=((tof1*w1)+(tof2*w2))/(w1+w2);
      xposm=((xpos1*w1)+(xpos2*w2))/(w1+w2);
      xposAdcm=((xposAdc1*w1)+(xposAdc2*w2))/(w1+w2);
      lAmpm=((lAmp1*w1)+(lAmp2*w2))/(w1+w2);
      rAmpm=((rAmp1*w1)+(rAmp2*w2))/(w1+w2);
      xlabm=((xlab1*w1)+(xlab2*w2))/(w1+w2);
      ylabm=((ylab1*w1)+(ylab2*w2))/(w1+w2);
      zlabm=((zlab1*w1)+(zlab2*w2))/(w1+w2);
      if((flagAdc1==2)&&(flagAdc2==2)) flagAdc=2;
      else flagAdc=0;

      clsize++;

    } else {
      clsize=0;
      cluster = (HTofCluster *)fClusterCat->getSlot(fLoc);
      if(cluster){
        cluster= new(cluster) HTofCluster;
        cluster->setSector(hit->getSector());
        cluster->setModule(hit->getModule());
        cluster->setCell(hit->getCell());
        tofm= hit->getTof();
        xposm=hit->getXpos();
        xposAdcm=hit->getXposAdc();
        edepm=hit->getEdep();
        lAmpm=hit->getLeftAmp();
        rAmpm=hit->getRightAmp();
        hit->getXYZLab(xlabm, ylabm, zlabm);
        flagAdc=hit->getAdcFlag();
      } else {
        printf("\nHTofClusterF: Slot (HTofCluster) creation not succesful! \n");
        return 1;
      }
    }
    r.setX(xlabm);
    r.setY(ylabm);
    r.setZ(zlabm);
    d = r.length();
    theta = (d>0.) ? (rad2deg * TMath::ACos(r.getZ() / d)) : 0.;
    phi = rad2deg * TMath::ATan2( r.getY(), r.getX());
    if (phi < 0.) phi += 360.;

    cluster->setTof(tofm);
    cluster->setXpos(xposm);
    cluster->setXposAdc(xposAdcm);
    cluster->setEdep(edepm);
    cluster->setLeftAmp(lAmpm);
    cluster->setRightAmp(rAmpm);
    cluster->setXYZLab(xlabm, ylabm, zlabm);
    cluster->setDistance(d);
    cluster->setTheta(theta);
    cluster->setPhi(phi);
    cluster->setClusterSize(clsize+1);
    cluster->setAdcFlag(flagAdc);

    // store previous hit data for the next cluster condition

    fpLoc=fLoc;
    prevTof=tof;
    prevXpos=xpos;
  }

#if DEBUG_LEVEL>2
  gDebuger->leaveFunc("HTofClusterF::execute");
#endif
  return 0;
}




ClassImp(HTofClusterF)














