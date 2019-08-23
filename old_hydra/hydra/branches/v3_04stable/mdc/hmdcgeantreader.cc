#include "hmdcgeantreader.h"
#include "hmdcdef.h"
#include "hdebug.h"
#include "hades.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hevent.h"
#include "hcategory.h"
#include "hmdclayergeompar.h"
#include "hdatasource.h"
#include <iostream.h>

//*-- Author : Alexander Nekhaev
//*-- Modified : 02/09/99 by V. Pechenov
//*-- Modified : 05/08/99 by R. Holzmann
//*-- Modified : 29/06/99 by Alexander Nekhaev

////////////////////////////////////////////////////////////////
//
//  HMdcGeantReader base class for HMdcGeantNtupleReader and
//  HMdcGeantRootReader
//
//  transform() - check either the particle go through 2 
//                neigboring cells or not, at the present 
//                only situation with two crossed neighboring 
//                cells is taken into account 
//  perp()      - define the shortest distance between sense wire 
//                and trajectory of the particle
//  cellNum()   - define the number of crossed cell 
//  storeCell() - put the data into GeantCell category
// 
////////////////////////////////////////////////////////////////

ClassImp(HMdcGeantReader)


HMdcGeantReader::HMdcGeantReader(void) {
  fGeantCellCat=0;
  fDigitGeomPar=0;
  pi=acos(-1.)/180;
}


void HMdcGeantReader::setParContainers() {
  fDigitGeomPar=(HMdcLayerGeomPar*)gHades->getRuntimeDb()->getContainer("MdcLayerGeomPar");
  if (!fDigitGeomPar) {
    fDigitGeomPar=new HMdcLayerGeomPar;
    gHades->getRuntimeDb()->addContainer(fDigitGeomPar);
  }
}

Bool_t HMdcGeantReader::transform(Float_t xcoor,Float_t ycoor,Float_t theta,
				  Float_t phi,Float_t tof,Int_t trkNum) {
  HMdcLayerGeomParLay& layer = (*fDigitGeomPar)[loc[0]][loc[1]][loc[2]];
  Float_t pitch = layer.getPitch();
  Float_t halfPitch = 0.5*pitch;
  Float_t halfCatDist = 0.5*layer.getCatDist();
  Int_t   nWires = layer.getNumWires();
  Float_t wOrient = layer.getWireOrient()*pi;
  Float_t centWire = layer.getCentralWireNr()-1.;   
    
// Transformation from MDC's coordinate system to layer's coord. system:
  Float_t y_wire = ycoor*cos(wOrient)-xcoor*sin(wOrient);
  
  Float_t ctanalpha=tan(theta*pi)*sin(phi*pi-wOrient);
  Float_t dY=halfCatDist*ctanalpha;
  dY<0.0 ? dY=-dY : dY;
  Float_t tmp=(centWire*pitch + y_wire+dY+halfPitch)/pitch; 
  if(tmp < 0.0) return kFALSE;
  Int_t nCmax=(Int_t)tmp;
  tmp=(centWire*pitch + y_wire-dY+halfPitch)/pitch;
  Int_t nCmin;
  tmp < 0.0 ? nCmin=0 : nCmin=(Int_t)tmp;
  if(nCmin >= nWires) return kFALSE;
  nCmax>=nWires ? nCmax=nWires-1 :nCmax;
//printf("nTr=%i, %i-%i",trkNum,nCmin,nCmax);
  for (loc[3]=nCmin; loc[3]<=nCmax; loc[3]++) {
   Float_t yDist=y_wire-(loc[3]*pitch-centWire*pitch);
   Float_t sinAlpha=sqrt(1./(1.+ctanalpha*ctanalpha));
   Float_t cosAlpha=sqrt(1.-sinAlpha*sinAlpha);
   Float_t per = yDist*sinAlpha;
   per<0.0 ? per=-per : per;
   if(per*sinAlpha > halfPitch) {
    Float_t z1=(yDist+halfPitch)/ctanalpha;
    Float_t z2=(yDist-halfPitch)/ctanalpha;
    (z1*=z1)<(z2*=z2) ? per=sqrt(halfPitch*halfPitch+z1) : per=sqrt(halfPitch*halfPitch+z2);
   } else if(per*cosAlpha > halfCatDist) {
    Float_t y1=halfCatDist*ctanalpha+yDist;
    Float_t y2=halfCatDist*ctanalpha-yDist;
    (y1*=y1)<(y2*=y2) ? per=sqrt(y1+halfCatDist*halfCatDist) : per=sqrt(y2+halfCatDist*halfCatDist);    
   }
   storeCell(per,tof,trkNum);
  }
  return kTRUE;
}



void HMdcGeantReader::storeCell(Float_t per,Float_t tof,Int_t trkNum) {
// Puts data to HMdcGeantCell Category
  hit=(HMdcGeantCell *)fGeantCellCat->getObject(loc);
  if (!hit) {
    hit=(HMdcGeantCell *)fGeantCellCat->getSlot(loc);
    hit = new(hit) HMdcGeantCell;
  }
  Int_t nHit;
  nHit=hit->getNumHits();
  hit->setSector(loc[0]);
  hit->setModule(loc[1]);
  hit->setLayer(loc[2]);
  hit->setCell(loc[3]);
  hit->setNumHits(nHit+1);
  hit->setMinDist(per, nHit);
  hit->setTimeFlight(tof, nHit);
  hit->setNTrack(trkNum,nHit);
}


