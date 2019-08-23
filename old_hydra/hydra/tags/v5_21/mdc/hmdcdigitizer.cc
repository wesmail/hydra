#include "hgeantmdc.h"
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
//*-- Modified: 11/05/2000 by R. Holzmann
//*-- Modified: 30/07/99 by Ilse Koenig
//*-- Modified: 28/06/99 by Alexander Nekhaev

////////////////////////////////////////////////////////////////
//
//  HMdcDigitizer digitizes data and puts output values into 
//  CAL1 category for simulated data
//  
//
//  transform() - check either the particle go through 2
//                neigboring cells or not, at the present
//                only situation with two crossed neighboring
//                cells is taken into account
//  perp()      - define the shortest distance between sense wire
//                and trajectory of the particle
//  cellNum()   - define the number of crossed cell
//  storeCell() - put the data into GeantCell category 
////////////////////////////////////////////////////////////////


HMdcDigitizer::HMdcDigitizer(void) {
  fGeantCellCat = 0;
  fCalCat = 0;
  fDigitGeomPar = 0;
  fDigitPar = 0;
  iterin = 0;
  itercell = 0;
  fEventId = 0;
  pi = acos(-1.)/180;
  fUnit = 10.;
}

HMdcDigitizer::HMdcDigitizer(Text_t *name,Text_t *title, Float_t unit) : 
               HReconstructor(name,title) {
  fGeantCellCat = 0;
  fCalCat = 0;
  fDigitGeomPar = 0;
  fDigitPar = 0;
  iterin = 0;
  itercell = 0;
  fEventId = 0;
  pi = acos(-1.)/180;
  fUnit = unit;  // HGeant length unit (1. for cm, 10. for mm) 
}

HMdcDigitizer::~HMdcDigitizer(void) {
  if(iterin) delete iterin;
  if(itercell) delete itercell;
}

void HMdcDigitizer::setParContainers() {
  fDigitGeomPar = (HMdcLayerGeomPar*)gHades->getRuntimeDb()
                                           ->getContainer("MdcLayerGeomPar");
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

  fGeantMdcCat = (gHades->getCurrentEvent()->getCategory(catMdcGeantRaw));
  if(!fGeantMdcCat) {
     Error("HMdcDigitizer::init()","HGeant MDC input missing");
     return kFALSE;
  }
  iterin = (HIterator*)fGeantMdcCat->MakeIterator("native");

  fGeantCellCat = gHades->getCurrentEvent()->getCategory(catMdcGeantCell);
  if (!fGeantCellCat) {
     fGeantCellCat = gHades->getSetup()->getDetector("Mdc")
                                       ->buildCategory(catMdcGeantCell);
     if (!fGeantCellCat) return kFALSE;
     else gHades->getCurrentEvent()->
                addCategory(catMdcGeantCell,fGeantCellCat,"Mdc");
  }
  fGeantCellCat->setPersistency(kFALSE);   // We don't want to write this one
  itercell = (HIterator*)fGeantCellCat->MakeIterator("native");

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

  return kTRUE;
}


Int_t HMdcDigitizer::execute(void) {
  Float_t xcoord, ycoord, tof, theta, phi;
  Int_t trkNum;
  HGeantMdc* fGeant;
  loc.set(4,0,0,0,0);   // location used to fill the HMdcGeantCell category

  iterin->Reset();
  while((fGeant=(HGeantMdc*)iterin->Next()) != NULL) {// loop over HGeant input
     loc[0] = (Int_t)(fGeant->getSector());
     loc[1] = (Int_t)(fGeant->getModule());
     loc[2] = (Int_t)(fGeant->getLayer());
//   loc[3] is filled in transform(...) with the cell number
     fGeant->getHit(xcoord, ycoord, tof);
     xcoord /= fUnit;
     ycoord /= fUnit;
     fGeant->getIncidence(theta, phi);
     trkNum = fGeant->getTrack();
     if(loc[2]<6) transform(xcoord,ycoord,theta,phi,tof,trkNum);
  } 

  HMdcGeantCell* fCell = 0;
  HMdcCal1Sim* fCal = 0;
  Float_t spaceResol; // space resolution for MDC
  Float_t dTime[15];  // drift times
  Int_t track[15];    // track numbers
  // Position resolution, here one can define the type of this function.
  // Now the function has 5 parameters:
  // fSpacepar0, fSpacepar1, fSpacepar2, min, max

  itercell->Reset();
  while ((fCell=(HMdcGeantCell *)itercell->Next()) != NULL) {
    loc[0] = fCell->getSector();
    loc[1] = fCell->getModule();
    loc[2] = fCell->getLayer();
    loc[3] = fCell->getCell();
    HMdcLayerGeomParLay& geomlayer = (*fDigitGeomPar)[loc[0]][loc[1]][loc[2]];
    HMdcDigitParLayer& layer = (*fDigitPar)[loc[0]][loc[1]][loc[2]];
    const Float_t min = layer.getSpacePar4();
    const Float_t max = layer.getSpacePar5();
    fCal = (HMdcCal1Sim*)fCalCat->getSlot(loc);
    if (fCal) {
      fCal = new(fCal) HMdcCal1Sim;
      fCal->setAddress(loc[0],loc[1],loc[2],loc[3]);

// Digitisation procedure starts here:

      Float_t scale=0.6F*10./geomlayer.getPitch();   // mm -> cm

// First TDC signal
      Int_t nHits = fCell->getNumHits();
      for(Int_t ihit = 0;ihit<nHits;ihit++){

// Efficiency of MDC can be less then 100%...
        (gRandom->Rndm()> layer.getMdcEfficiency()) ?
              fCal->setStatus(-1) : fCal->setStatus(1);

// Position resolution
        Float_t minDist = fCell->getMinDist(ihit);
        Float_t di = minDist*scale;
        if (di<=min) spaceResol = layer.getSpacePar1()*(di-min)*
                       (di-min)+layer.getSpacePar2();
        else {
          if (di>=max) spaceResol = layer.getSpacePar3()*(di-max)*
                       (di-max)+layer.getSpacePar2();
          else spaceResol = layer.getSpacePar2();
        }
        spaceResol/=10000.F; // mkm->cm

        // Drift Time
        Float_t gauss = gRandom->Gaus(0,1);  // Gauss distribution
        dTime[ihit] = (minDist + spaceResol*gauss)/(layer.getDriftVelocity()/
                       10000.F) + fCell->getTimeFlight(ihit);
        track[ihit] = fCell->getNTrack(ihit);
      }

// TDC can distinguish two tracks if the distance between these tracks
// more than 50 mkm.

      if (nHits>1) select(dTime,track,nHits);

      fCal->setTime1(dTime[0]);
      fCal->setNTrack1(track[0]);

      if (nHits==1) fCal->setNHits(1);
      else {
        const Float_t fac = 50.F/layer.getDriftVelocity();
        Int_t second = 0;
        Int_t i = 1; 
        while (second==0 && i<nHits) {
          if ((dTime[i]-dTime[i-1])>=fac) second = i;
          i++;
        }
        if (!second) fCal->setNHits(1);
        else {
          fCal->setNHits(2);
          fCal->setTime2(dTime[second]);
          fCal->setNTrack2(track[second]);
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
     exchange = 0;
     c = a;
     t = tof[a];
     n = track[a];
     for(b=a+1;b<count;++b) {
        if(tof[b]<t) {
	   c = b;
  	   t = tof[b];
           n = track[b];
	   exchange = 1;
        }
     }
     if(exchange) {
        tof[c] = tof[a];
        tof[a] = t;
        track[c] = track[a];
        track[a] = n;
     }
  }
}

Bool_t HMdcDigitizer::transform(Float_t xcoor,Float_t ycoor,Float_t theta,
                                  Float_t phi,Float_t tof,Int_t trkNum) {
  HMdcLayerGeomParLay& layer = (*fDigitGeomPar)[loc[0]][loc[1]][loc[2]];
  Float_t pitch = 0.1*layer.getPitch();           // mm -> cm
  Float_t halfPitch = 0.5*pitch;
  Float_t halfCatDist = 0.05*layer.getCatDist();  // mm -> cm
  Int_t   nWires = layer.getNumWires();
  Float_t wOrient = layer.getWireOrient()*pi;
  Float_t centWire = layer.getCentralWireNr()-1.;

// Transformation from MDC's coordinate system to layer's coordinate system:
  Float_t y_wire = ycoor*cos(wOrient)-xcoor*sin(wOrient);

  Float_t ctanalpha = tan(theta*pi)*sin(phi*pi-wOrient);
  Float_t dY = halfCatDist*ctanalpha;
  dY<0.0 ? dY = -dY : dY;
  Float_t tmp = (centWire*pitch + y_wire+dY+halfPitch)/pitch;
  if(tmp < 0.0) return kFALSE;
  Int_t nCmax = (Int_t)tmp;
  tmp = (centWire*pitch + y_wire-dY+halfPitch)/pitch;
  Int_t nCmin;
  tmp < 0.0 ? nCmin=0 : nCmin=(Int_t)tmp;
  if(nCmin >= nWires) return kFALSE;
  nCmax>=nWires ? nCmax=nWires-1 : nCmax;
//printf("nTr=%i, %i-%i",trkNum,nCmin,nCmax);
  for (loc[3]=nCmin; loc[3]<=nCmax; loc[3]++) {
     Float_t yDist = y_wire-(loc[3]*pitch-centWire*pitch);
     Float_t sinAlpha = sqrt(1./(1.+ctanalpha*ctanalpha));
     Float_t cosAlpha = sqrt(1.-sinAlpha*sinAlpha);
     Float_t per = yDist*sinAlpha;
     per<0.0 ? per=-per : per;
     if(per*sinAlpha > halfPitch) {
        Float_t z1 = (yDist+halfPitch)/ctanalpha;
        Float_t z2 = (yDist-halfPitch)/ctanalpha;
        (z1*=z1)<(z2*=z2) ? per = sqrt(halfPitch*halfPitch+z1) :
                            per = sqrt(halfPitch*halfPitch+z2);
     } else if(per*cosAlpha > halfCatDist) {
        Float_t y1 = halfCatDist*ctanalpha+yDist;
        Float_t y2 = halfCatDist*ctanalpha-yDist;
        (y1*=y1)<(y2*=y2) ? per = sqrt(y1+halfCatDist*halfCatDist) :
                            per = sqrt(y2+halfCatDist*halfCatDist);
     }
     storeCell(per,tof,trkNum);
  }
  return kTRUE;
}

void HMdcDigitizer::storeCell(Float_t per,Float_t tof,Int_t trkNum) {
// Puts data to HMdcGeantCell Category
  hit = (HMdcGeantCell*)fGeantCellCat->getObject(loc);
  if (!hit) {
     hit = (HMdcGeantCell*)fGeantCellCat->getSlot(loc);
     hit = new(hit) HMdcGeantCell;
  }
  Int_t nHit;
  nHit = hit->getNumHits();
  hit->setSector(loc[0]);
  hit->setModule(loc[1]);
  hit->setLayer(loc[2]);
  hit->setCell(loc[3]);
  hit->setNumHits(nHit+1);
  hit->setMinDist(per,nHit);
  hit->setTimeFlight(tof,nHit);
  hit->setNTrack(trkNum,nHit);
}  

ClassImp(HMdcDigitizer)










