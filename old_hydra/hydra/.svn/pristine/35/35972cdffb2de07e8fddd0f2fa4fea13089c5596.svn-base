#include "hmdcdigitizer.h"
#include "hmdcdef.h"
#include "hdebug.h"
#include "hades.h"
#include "hmdcgeantraw.h"
#include "hmdccal.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hevent.h"
#include "hcategory.h"
#include "hlocation.h"
#include "hmdclayergeompar.h"
#include "hmdcdigitpar.h"
#include <iostream.h>

//*-- Author : A.Nekhaev
//*-- Modified: 19/05/99 by Ilse Koenig

////////////////////////////////////////////////////////////////
//
//  HMdcDigitizer digitizes data, puts output values to 
//  Cal1 category
//  
////////////////////////////////////////////////////////////////


HMdcDigitizer::HMdcDigitizer(void) {
  fGeantRawCat=0;
  fCalCat=0;
  fDigitGeomPar=0;
  fDigitPar=0;
  setParContainers();
  iter=0;    
}

HMdcDigitizer::HMdcDigitizer(Text_t *name,Text_t *title) : 
               HReconstructor(name,title) {
  fGeantRawCat=0;
  fCalCat=0;
  fDigitGeomPar=0;
  fDigitPar=0;
  setParContainers();
  iter=0;    
}

HMdcDigitizer::~HMdcDigitizer(void) {
  if (fDigitPar) gHades->getRuntimeDb()->removeContainer("MdcDigitPar");
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
  fGeantRawCat=gHades->getCurrentEvent()->getCategory(catMdcGeantRaw);
  if (!fGeantRawCat) {
    fGeantRawCat=gHades->getSetup()->getDetector("Mdc")->buildCategory(catMdcGeantRaw);
   if (!fGeantRawCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catMdcGeantRaw,fGeantRawCat,"Mdc");
  }

  fCalCat=gHades->getCurrentEvent()->getCategory(catMdcCal);
  if (!fCalCat) {
    fCalCat=gHades->getSetup()->getDetector("Mdc")->buildCategory(catMdcCal);
   if (!fCalCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catMdcCal,fCalCat,"Mdc");
  }
  iter=(HIterator *)fGeantRawCat->MakeIterator();
  loc.set(4,0,0,0,0);
  return kTRUE;
}

Int_t HMdcDigitizer::execute(void) {
 
  HMdcGeantRaw* geant=0;
  HMdcCal* cal=0;
  iter->Reset();
  while ((geant=(HMdcGeantRaw *)iter->Next())!=0) {
    loc[0]=geant->getSector();
    loc[1]=geant->getModule();
    loc[2]=geant->getLayer();
    loc[3]=geant->getCell();
    HMdcLayerGeomParLay& geomlayer = (*fDigitGeomPar)[loc[0]][loc[1]][loc[2]];
    HMdcDigitParLayer& layer = (*fDigitPar)[loc[0]][loc[1]][loc[2]];

    cal=(HMdcCal *)fCalCat->getSlot(loc);
    if (cal) {
      cal=new(cal) HMdcCal;
      cal->setSector(loc[0]);
      cal->setModule(loc[1]);
      cal->setLayer(loc[2]);
      cal->setCell(loc[3]);


// Digitisation procedure:

//First TDC signal

      for(Int_t ihit = 0;ihit<geant->getNumHits();ihit++){

// Efficiency of MDC can be less then 100%...
	(gRandom->Rndm()>layer.getMdcEfficiency()) ? cal->setStatus(-1) : cal->setStatus(1);

	 Float_t minDist = geant->getMinDist(ihit);

// Gauss distribution
       	Float_t gauss = gRandom->Gaus(0,1);
// Position resolution, here one can define the type of this function.
//  Now the function has 5 parameters fSpacepar0,fSpacepar1,fSpacepar2,
// min, max

        Float_t min=0.4;
        Float_t max=2.2;
        float scale=0.0;
        if(geomlayer.getPitch()<0.55)                         scale = 0.6/0.5;
        if(geomlayer.getPitch()>0.55&&geomlayer.getPitch()<0.7)   scale = 0.6/0.6;
        if(geomlayer.getPitch()>1.1&&geomlayer.getPitch()<1.3)    scale = 0.6/1.2;
        if(geomlayer.getPitch()>1.39)                         scale = 0.6/1.4;
        if((minDist*scale)<=min) spaceResol=layer.getSpacePar0()*(minDist*scale-min)*(minDist*scale-min)+layer.getSpacePar1();
        if(minDist*scale>min||minDist*scale<max) spaceResol=layer.getSpacePar1();
        if((minDist*scale)>=max) spaceResol=layer.getSpacePar2()*(minDist*scale-max)*(minDist*scale-max)+layer.getSpacePar1();
        spaceResol/=10000.; // mkm->cm

// Drift Time
        dTime[ihit]=(minDist + spaceResol*gauss)/(layer.getDriftVelocity()/10000.)+geant->getTimeFlight(ihit);
      }

      if(geant->getNumHits()>1)   select(dTime,geant->getNumHits());

// TDC can distinguish two tracks if the destance between these tracks more than 50 mkm.


      if(geant->getNumHits()==1) {
	cal->setTime(dTime[0]);
      }
      
      if(geant->getNumHits()==2){
	if((dTime[1]-dTime[0])<(50./layer.getDriftVelocity())) {
	  cal->setTime(dTime[0]);
	}
	else {
	  cal->setTime(dTime[0]);
	  cal->setTime(dTime[1]);
	  
	}
      }
      
      if(geant->getNumHits()==3) {
	if((dTime[1]-dTime[0])>=(50./layer.getDriftVelocity())) {
	  cal->setTime(dTime[0]);
	  cal->setTime(dTime[1]);
	}
	if(((dTime[1]-dTime[0])<(50./layer.getDriftVelocity()))&&((dTime[2]-dTime[0])>=(50./layer.getDriftVelocity()))){
	  cal->setTime(dTime[0]);
	  cal->setTime(dTime[2]);
	}
	if(((dTime[1]-dTime[0])<(50./layer.getDriftVelocity()))&&((dTime[2]-dTime[0])<(50./layer.getDriftVelocity()))){
	  cal->setTime(dTime[0]);
	}
      }
      
      
      if(geant->getNumHits()==4) {
	if((dTime[1]-dTime[0])>=(50./layer.getDriftVelocity())) {
	  cal->setTime(dTime[0]);
	  cal->setTime(dTime[1]);
	}
	if(((dTime[1]-dTime[0])<(50./layer.getDriftVelocity()))&&
	   ((dTime[2]-dTime[0])>=(50./layer.getDriftVelocity()))) {
	  cal->setTime(dTime[0]);
	  cal->setTime(dTime[2]);
	}
	if(((dTime[1]-dTime[0])<(50./layer.getDriftVelocity()))&&
	   ((dTime[2]-dTime[0])<(50./layer.getDriftVelocity()))&&
	   ((dTime[3]-dTime[0])<(50./layer.getDriftVelocity()))) {
	  cal->setTime(dTime[0]);	  
	}
	
      }
            
    }
  }
 return 1;
}

float HMdcDigitizer::select(Float_t *item,Int_t count)
{
  register Int_t a,b,c;
  Int_t exchange;
  Float_t t;
  for(a=0;a<count-1;++a)
    {
      exchange=0;
      c=a;
      t=item[a];
      for(b=a+1;b<count;++b)
	{
	  if(item[b]<t)
	    {
	      c=b;
	      t=item[b];
	      exchange=1;
	    }
	}
      if(exchange)
	{
	  item[c]=item[a];
	  item[a]=t;
	}
    }
  return kTRUE;
}


ClassImp(HMdcDigitizer)







