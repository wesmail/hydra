#include "hmdcdigitizer.h"
#include "hmdcdef.h"
#include "hdebug.h"
#include "hades.h"
#include "hmdcgeantcell.h"
#include "hmdccal1sim.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hmdcdetector.h"
#include "hevent.h"
#include "hcategory.h"
#include "hlocation.h"
#include "hmdclayergeompar.h"
#include "hmdcdigitpar.h"
#include <iostream.h>

//*-- Author : A.Nekhaev
//*-- Modified: 24/08/99 by R. Holzmann
//*-- Modified: 30/07/99 by Ilse Koenig
//*-- Modified: 28/06/99 by Alexander Nekhaev

////////////////////////////////////////////////////////////////
//
//  HMdcDigitizer digitizes data, puts output values to 
//  Cal1 category for simulated data
//  
////////////////////////////////////////////////////////////////


HMdcDigitizer::HMdcDigitizer(void) {
  fGeantCellCat=0;
  fCalCat=0;
  fDigitGeomPar=0;
  fDigitPar=0;
  iter=0;    
}

HMdcDigitizer::HMdcDigitizer(Text_t *name,Text_t *title) : 
               HReconstructor(name,title) {
  fGeantCellCat=0;
  fCalCat=0;
  fDigitGeomPar=0;
  fDigitPar=0;
  //setParContainers();
  iter=0;    
}

HMdcDigitizer::~HMdcDigitizer(void) {
}

void HMdcDigitizer::setParContainers() {
  fDigitGeomPar=(HMdcLayerGeomPar*)gHades->getRuntimeDb()->getContainer("MdcLayerGeomPar");
  if (!fDigitGeomPar) {
    fDigitGeomPar=new HMdcLayerGeomPar;
    gHades->getRuntimeDb()->addContainer(fDigitGeomPar);
  }
  fDigitPar=(HMdcDigitPar*)gHades->getRuntimeDb()->getContainer("MdcDigitPar");
  if (!fDigitPar) {
    fDigitPar=new HMdcDigitPar;
    gHades->getRuntimeDb()->addContainer(fDigitPar);
  }
}

Bool_t HMdcDigitizer::init(void) {
  setParContainers();
  fGeantCellCat=gHades->getCurrentEvent()->getCategory(catMdcGeantCell);
  if (!fGeantCellCat) {
    fGeantCellCat=gHades->getSetup()->getDetector("Mdc")->buildCategory(catMdcGeantCell);
    if (!fGeantCellCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catMdcGeantCell,fGeantCellCat,"Mdc");
  }

  fCalCat=gHades->getCurrentEvent()->getCategory(catMdcCal1);
  if (!fCalCat) {
    HMdcDetector* mdc=(HMdcDetector*)(gHades->getSetup()->getDetector("Mdc"));
    fCalCat=mdc->buildMatrixCategory("HMdcCal1Sim",0.5F);
    if (!fCalCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catMdcCal1,fCalCat,"Mdc");
  } else {
    if (fCalCat->getClass()!=HMdcCal1Sim::Class()) {
      Error("HMdcDigitizer::init()","Misconfigured output category");
      return kFALSE;
    }
  }
  iter=(HIterator *)fGeantCellCat->MakeIterator();
  loc.set(4,0,0,0,0);
  return kTRUE;
}


Int_t HMdcDigitizer::execute(void) {
  HMdcGeantCell* geant=0;
  HMdcCal1Sim* cal=0;
  iter->Reset();
  Float_t spaceResol; // space resolution for MDC
  Float_t dTime[15];  // drift times
  Int_t track[15];    // track numbers
  // Position resolution, here one can define the type of this function.
  // Now the function has 5 parameters fSpacepar0,fSpacepar1,fSpacepar2,
  // min, max
  while ((geant=(HMdcGeantCell *)iter->Next())!=0) {
    loc[0]=geant->getSector();
    loc[1]=geant->getModule();
    loc[2]=geant->getLayer();
    loc[3]=geant->getCell();
    HMdcLayerGeomParLay& geomlayer = (*fDigitGeomPar)[loc[0]][loc[1]][loc[2]];
    HMdcDigitParLayer& layer = (*fDigitPar)[loc[0]][loc[1]][loc[2]];
    const Float_t min = layer.getSpacePar4();
    const Float_t max = layer.getSpacePar5();
    cal=(HMdcCal1Sim*)fCalCat->getSlot(loc);
    if (cal) {
      cal=new(cal) HMdcCal1Sim;
      cal->setAddress(loc[0],loc[1],loc[2],loc[3]);

// Digitisation procedure:

      Float_t scale=0.6F/geomlayer.getPitch();
//First TDC signal
      Int_t nHits=geant->getNumHits();
      for(Int_t ihit = 0;ihit<nHits;ihit++){
// Efficiency of MDC can be less then 100%...
        (gRandom->Rndm()>
            layer.getMdcEfficiency()) ? cal->setStatus(-1) : cal->setStatus(1);
// Position resolution
        Float_t minDist = geant->getMinDist(ihit);
        Float_t di=minDist*scale;
        if (di<=min) spaceResol=layer.getSpacePar1()*(di-min)*
                       (di-min)+layer.getSpacePar2();
        else {
          if (di>=max) spaceResol=layer.getSpacePar3()*(di-max)*
                       (di-max)+layer.getSpacePar2();
          else spaceResol=layer.getSpacePar2();
        }
        spaceResol/=10000.F; // mkm->cm

        // Drift Time
        Float_t gauss = gRandom->Gaus(0,1);  // Gauss distribution
        dTime[ihit]=(minDist + spaceResol*gauss)/(layer.getDriftVelocity()/
          10000.F)+geant->getTimeFlight(ihit);
        track[ihit]=geant->getNTrack(ihit);
      }

// TDC can distinguish two tracks if the distance between these tracks
// more than 50 mkm.

      if (nHits>1) select(dTime,track,nHits);

      cal->setTime1(dTime[0]);
      cal->setNTrack1(track[0]);

      if (nHits==1) cal->setNHits(1);
      else {
        const Float_t fac=50.F/layer.getDriftVelocity();
        Int_t second=0;
        Int_t i=1; 
        while (second==0 && i<nHits) {
          if ((dTime[i]-dTime[i-1])>=fac) second=i;
          i++;
        }
        if (!second) cal->setNHits(1);
        else {
          cal->setNHits(2);
          cal->setTime2(dTime[second]);
          cal->setNTrack2(track[second]);
        } 
      }
    }
  }
  return 0;
}


void HMdcDigitizer::select(Float_t* tof,Int_t* track,Int_t count)
// Puts the drift time values into increasing order
// Orders the corresponding track number accordingly
{
  register Int_t a,b,c;
  Int_t exchange;
  Float_t t;
  Int_t n;
  for(a=0;a<count-1;++a) {
    exchange=0;
    c=a;
    t=tof[a];
    n=track[a];
    for(b=a+1;b<count;++b) {
      if(tof[b]<t) {
	c=b;
	t=tof[b];
        n=track[b];
	exchange=1;
      }
    }
    if(exchange) {
      tof[c]=tof[a];
      tof[a]=t;
      track[c]=track[a];
      track[a]=n;
    }
  }
}


ClassImp(HMdcDigitizer)







