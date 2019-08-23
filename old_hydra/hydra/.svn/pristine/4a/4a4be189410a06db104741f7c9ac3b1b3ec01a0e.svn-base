//*-- AUTHOR Bjoern Spruck
//*-- created : 24.03.06
//*-- modified : 22.03.07 BS

//_HADES_CLASS_DESCRIPTION
////////////////////////////////////////////////////////////////
//
//  HHodoCalibrater: perform global correction on the data
//
//  Calibrates HodoRaw data and writes to HodoCal.
//  Slope and Offset correction for Time and Adc (ToT; Signal Width) is done.
//  ATTENTION: ADC is not corrected yet... no need to do it
//  ... its not used anyway until now
//
//  An additional Cut on a coarse time windows is done, maybe this
//  is not needed in future, but it was important in former test beam times.
//
////////////////////////////////////////////////////////////////

using namespace std;
#include "hhododetector.h"
#include "hhodocalibrater.h"
#include "hododef.h"
#include "hhodoraw.h"
#include "hhodocal.h"
#include "hhodocalpar.h"
#include "hdebug.h"
#include "hades.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hevent.h"
#include "heventheader.h"
#include "hcategory.h"
#include <iostream>
#include <iomanip>


HHodoCalibrater::HHodoCalibrater(void)
{
   rawCat=0;
   calCat=0;
   iter=0;
   calpar=0;
   useNoTrailEvents=false;
   quietMode=false;
}


HHodoCalibrater::HHodoCalibrater(const Text_t *name,const Text_t *title) :
   HReconstructor(name,title)
{
   rawCat=0;
   calCat=0;
   iter=0;
   calpar=0;
   useNoTrailEvents=false;
   quietMode=false;
}

HHodoCalibrater::~HHodoCalibrater(void)
{
   if (iter) delete iter;
   iter=0;
}

Bool_t HHodoCalibrater::init(void)
{
   HHodoDetector *hodo;
   hodo=(HHodoDetector *)gHades->getSetup()->getDetector("Hodo");

   if(!hodo){
      Error("init","No Hodo Det. found.");
      return kFALSE;
   }

   // Categories
   rawCat=hodo->buildCategory(catHodoRaw);
   if (!rawCat){
      Error("init","No Hodo raw Cat");
      return kFALSE;
   }
   calCat=hodo->buildCategory(catHodoCal);
   if (!calCat){
      Error("init","No Hodo cal Cat");
      return kFALSE;
   }

   // Parameters, Slope and Offset
   calpar=(HHodoCalPar*)gHades->getRuntimeDb()->getContainer("HodoCalPar");
   if (!calpar){
      Error("init","No HodoCalPar Parameters");
      return kFALSE;
   }

   iter=(HIterator *)rawCat->MakeIterator();
   loc.set(2,0,0);
   fActive=kTRUE;

   return kTRUE;
}

Int_t HHodoCalibrater::execute(void)
{
   HHodoRaw *raw=0;
   HHodoCal *cal=0;
   Int_t mod=0, strip=0;

   Float_t rawTime;
   Float_t rawADC;

   // Do the calibration here

   // Multiplicity checks i would postphone to hitfinder...

   // But how do we handle cases with tdc channel mult>4?
   // same question applies to what to do if hits were rejected by TDC itself -> see unpacker
   // maybe this should go to the raw category...
   //Fill cal category
   iter->Reset();
   while ((raw=(HHodoRaw *)iter->Next())!=0) {
      Int_t m;// channel hit multiplicty

      m=raw->getNHits();
      if(m<=0) continue;// No Hits -> forget it

      raw->getAddress(mod,strip);
      loc[0] = mod; loc[1] = strip;

      cal=(HHodoCal*)calCat->getSlot(loc);
      if (cal) {
         cal=new (cal) HHodoCal;
         if ( !cal ) return EXIT_FAILURE;

         cal->setAddress(mod,strip);// What happens if no hit is filled in? At least mod and strip are !=-1

         HHodoCalParChan &pPar=(*calpar)[mod][strip];

         if(m>raw->getMaxMult()) m=raw->getMaxMult();

         for(Int_t i=0; i<m; i++){
            rawTime  = raw->getTime(i+1);
            rawADC  = raw->getADC(i+1);
            if(rawADC<0.0){
              if(useNoTrailEvents) rawADC=0.0;
              if(!quietMode) Warning("execute","Evt %i Mod: %i, Chn %i, Hit %i: No trailing time found", gHades->getCurrentEvent()->getHeader()->getEventSeqNumber(),mod,strip,i);
            }
            if(rawADC>=0.0){
               // This throws away hits without trailing data... (spikes?) EXCEPT useNoTrailEvents is set
               Float_t adc, time;
					// Attention: before 2007 slope/offset was done exactly the other way around.
					// To be consistent, it was changed to be like in Start/Tof/...
					// As data was not used in any analysis this should not be a problem

               time=pPar.getTDCSlope()*rawTime+pPar.getTDCOffset();// CHANGED
//               if(time>=lower && time<=upper){// Time window after calibration -> moved to hitfinder
//                  adc= pPar.getADCSlope()*(rawADC +pPar.getADCOffset());
                  adc= rawADC;// NO CORRECTION YET
                  cal->setTimeAdc(time,adc);
//               }
               // How to do "double-hit" time shift?
               // Is it really needed for the beamtime?
               // Think about it
            }
         }
         // dont fill multiplicity information here
         // maybe set an overflow flag if necessary...
         // idea behind it... high multiplicity might come from spikes AFTER the signal
         // -> not of interest
      }// else if no slot, ignore (is this the right behaviour???)
   }
   return EXIT_SUCCESS;
}

ClassImp(HHodoCalibrater)
