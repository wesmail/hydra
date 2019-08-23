using namespace std;
#include "hstartcalibrater.h"
#include "hstartdef.h"
#include "hstartcal.h"
#include "hstartselfcopar.h"
#include "htofcalpar.h"
#include "hdebug.h"
#include "hades.h"
#include "heventheader.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hruntimedb.h"
#include "hiterator.h"
#include "hlinearcategory.h"
#include "hstarthitf.h"
#include "hstarthit.h"
#include "tofdef.h"
#include "htofraw.h"
#include "htofinocalibrater.h"
#include "tofinodef.h"
#include "htofinoraw.h"
#include "htofinocalpar.h"
#include "hlocation.h"
#include "TMath.h"
#include <iostream> 
#include <iomanip>
#include "TH1.h"

//*--Author : D.Zovinec
//*--Mod.  J. Pietraszko
///////////////////////////////////////////////////////////////////////////////
//  HStartHitF
//
//  This class implements the hit finder for the Start detector.
//  It iterates over the CAL level data objects and finds one that is supposed
//  to be the true Start hit.
//  Selection conditions:
//    for Start Mult=1      - Start detector multiplicity  == 1
//                          - Start cal time is found in the interval
//                            defined around the peak of the selfcoincidence.
//    
//    for Start Mult>1      - the procedure is based on checking the 
//							  Time of Flight for the fastest particles in 
//                            Tof and Tofino detectors (about 7ns). 
//                            The cross-talk is removed.   
//
///////////////////////////////////////////////////////////////////////////////


HStartHitF::HStartHitF(void) {
  calCat=0;
  hitCat=0;
  iter=0;
  fTofRawCat=0;
  fTofCalPar=0;
  iterTof=0;

  fTofinoRawCat=0;
  pTofinoCalPar = NULL;
  iterTofino=0;

  selfcopar=0;
  fHistograms=new TList;
  fControlHistograms=kFALSE;
}

HStartHitF::HStartHitF(Text_t *name,Text_t *title) :
               HReconstructor(name,title) {
  calCat=0;
  hitCat=0;
  iter=0;
  fTofRawCat=0;
  fTofCalPar=0;
  iterTof=0;

  fTofinoRawCat=0;
  pTofinoCalPar = NULL;
  iterTofino=0;
  selfcopar=0;
  fHistograms=new TList;
  fControlHistograms=kFALSE;
}

HStartHitF::~HStartHitF(void) {
  if (iter) delete iter;
  if (iterTof) delete iterTof;
  if (iterTofino) delete iterTofino;
}

Bool_t HStartHitF::init(void) {
                                                      
  tofdet = (HDetector *)gHades->getSetup()->getDetector("Tof");        
  tofinodet = (HDetector *)gHades->getSetup()->getDetector("Tofino");   

  initParContainer();

  calCat=gHades->getCurrentEvent()->getCategory(catStartCal);
  if (!calCat) {
    calCat=gHades->getSetup()->getDetector("Start")->buildCategory(catStartCal);
    if (!calCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catStartCal,calCat,"Start");
  }
  iter=(HIterator *)calCat->MakeIterator();

  hitCat=(HLinearCategory *)(gHades->getCurrentEvent()->getCategory(catStartHit));
  if (!hitCat) {
    hitCat = new HLinearCategory("HStartHit");
    if (!hitCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catStartHit,hitCat,"Start");
  }
  loc.set(1,0);
  fActive=kTRUE;

  // get Tof Raw and Tofino Raw cat. 
  if((tofdet && tofinodet)){
    // TOFINO 
    fTofinoRawCat = gHades->getCurrentEvent()->getCategory(catTofinoRaw);
      if (!fTofinoRawCat) {
        Warning("init","Tofino Raw level does not exist !!! ");
		tofinodet=0;
      }
		else{
    		iterTofino = (HIterator *)fTofinoRawCat->MakeIterator("native");
    	}
    // TOF
    fTofRawCat=gHades->getCurrentEvent()->getCategory(catTofRaw);
      if (!fTofRawCat) {
        Warning("init","Tof Raw level does not exist !!!");
		tofdet=0;
      }
		else{

		    iterTof=(HIterator *)fTofRawCat->MakeIterator();
		}
	
  } else {
      Warning("init"," Either Tof or Tofino or both are not in the setup. Only events with Start Mult=1 will be analysed ");
      fTofinoRawCat=0;
      fTofRawCat=0;
  }  
 

  if(fControlHistograms==kTRUE){
    Text_t  buffer[255];
    for(Int_t det=0;det<8;det++) {
      sprintf(buffer,"START_%i_%s",det+1,"cal");
      histStartC[det]=new TH1F(buffer,buffer,1000, -50, 50);
      fHistograms->Add(histStartC[det]);
      sprintf(buffer,"START_%i_%s",det+1,"hit");
      histStartH[det]=new TH1F(buffer,buffer,1000, -50, 50);
      fHistograms->Add(histStartH[det]);
    }
    histMultStart = new TH1F("MultStart","Start multiplicity",9,-0.5,8.5);
    fHistograms->Add(histMultStart);
    histPatternStart = new TH1F("PatternStart","PatternStart",8,0.5,8.5);
    fHistograms->Add(histPatternStart);
    histStartStat = new TH1F("StartStat","Start Statistics",9,-0.5,8.5);
    fHistograms->Add(histStartStat);
  }
  return kTRUE;
}

void HStartHitF::initParContainer() {
  selfcopar=(HStartSelfCoPar*)gHades->getRuntimeDb()->getContainer("StartSelfCoPar");

  if((tofdet && tofinodet)){
    fTofCalPar=(HTofCalPar*)gHades->getRuntimeDb()->getContainer("TofCalPar");
    pTofinoCalPar = (HTofinoCalPar *)gHades->getRuntimeDb()->getContainer("TofinoCalPar");
  }
}


Int_t HStartHitF::execute(void) {

  HLocation ffLoc; 
  HStartCal *cal=0;
  HStartHit *hit=0;
  HTofRaw *tofraw=NULL;
  HTofinoRaw *tofinoraw=NULL;

  Float_t time= -1000;
  Float_t timeTmp= -1000;
  Float_t timeDiff= -1000;
  Float_t timeDiffCalHit=-1000;
  Int_t strip= -1;
  Int_t stripTmp= -1;
  Int_t multStart=0;
  Float_t acp=-1000;
  Float_t acw=-1000;
  // Tof data
  Float_t atofCorr = 0.000000276; // empirical correction of atof dependence on axpos
  Float_t atof;
  Float_t axpos;
  Int_t nWasHitTof=0;
  Int_t nWasHitTofino=0;
  Int_t nWasCrossTalk=0;

  Float_t fTofinocalTime=-1000;
  ffLoc.set(2,-1,-1);

  //  Diagnostic histograms filling  &  start module multiplicity checking.
  iter->Reset();
  while ((cal=(HStartCal *)iter->Next())!=0) {
    if((cal->getModule()) == 0) {
      strip = cal->getStrip();
      time = cal->getTime();
      if(fControlHistograms == kTRUE){
        histStartC[strip]->Fill(time);
        histPatternStart->Fill(strip+1);
      }
      multStart++;
    }//end of Module 0 condition.
  }// end of StartCal iteration


  if((multStart>1) && tofdet && tofinodet){
    iter->Reset();
    while ((cal=(HStartCal *)iter->Next())!=0 ) {
	  if((cal->getModule()) == 0) {
        stripTmp = cal->getStrip();
        timeTmp = cal->getTime();

        // Tof Raw data,    
        iterTof->Reset();
        while ( (tofraw=(HTofRaw *)iterTof->Next())!=0) {
          if(tofraw->getLeftTime() && tofraw->getRightTime()){
            HTofCalParCell& cell=(*fTofCalPar)[tofraw->getSector()][tofraw->getModule()][tofraw->getCell()];
            atof = (tofraw->getLeftTime() * cell.getLeftK() +
                    tofraw->getRightTime()*cell.getRightK())/2.0 - cell.getTimK();
            axpos = cell.getVGroup()*(tofraw->getRightTime() * cell.getRightK() -
                  tofraw->getLeftTime()*cell.getLeftK())/2.0 +cell.getPosK();
            atof = atof + (axpos*axpos*atofCorr);
            Float_t timeDiffTmp1 = TMath::Abs(atof - 7.0 - timeTmp); 
            if((atof>0.0)&&((nWasHitTof==0) ||(timeDiffTmp1 < timeDiff))){  
                time = timeTmp;
                strip = stripTmp; 
                timeDiff = timeDiffTmp1; 
                nWasHitTof=1; 
            }
          }
        }

        // Tofino Raw data,
        iterTofino->Reset();
        while ((tofinoraw=(HTofinoRaw *)iterTofino->Next())!=0) {
          ffLoc[0] = tofinoraw->getSector();
          ffLoc[1] = tofinoraw->getCell();
          fTofinocalTime = ((HTofinoCalPar*)pTofinoCalPar)->
                         channelToTime((tofinoraw->getSector()),(tofinoraw->getCell()),(tofinoraw->getTime()));
          fTofinocalTime -= ((HTofinoCalPar*)pTofinoCalPar)->getOffset(ffLoc);
          Float_t timeDiffTmp1 = TMath::Abs(fTofinocalTime - 7.0 - timeTmp); 
          if((fTofinocalTime<0.0) || ((nWasHitTof==1)&&(timeDiffTmp1 > timeDiff))) continue; 
          if((nWasHitTofino==0) ||(timeDiffTmp1 < timeDiff)){
              time = timeTmp;
              strip = stripTmp;
              timeDiff = timeDiffTmp1;
              nWasHitTofino=1;
          }
        } 
      } //end of Module 0 condition.
    } // end of StartHit iteration

      /////////////////////////////////////////////////////////
      // Now we have to reject the cross-talk effect !!!     //
      /////////////////////////////////////////////////////////

    iter->Reset();
    while ((cal=(HStartCal *)iter->Next())!=0) {
      if((cal->getModule()) == 0) {
        timeDiffCalHit = (cal->getTime()) - time;
        if(strip != (cal->getStrip())){
          if(timeDiffCalHit>-10.0 && timeDiffCalHit < 5.0 ) nWasCrossTalk=1;
        }
      }//end of Module 0 condition.
    }// end of StartCal iteration
  } //end If StartM>1 

  if(fControlHistograms == kTRUE){
    histMultStart->Fill(multStart);
    histStartStat->Fill(0.);
  }
  //  Hit finding & storing.
  hit=(HStartHit *)hitCat->getNewSlot(loc);
  if (hit) {
    hit = new(hit) HStartHit;
    hit->setModule(-1);
    hit->setStrip(-1);
    hit->setTime(-1000);
    hit->setStartMult(-1);
    hit->setFlag(kFALSE);   
    //
    // Events accepted: 
    //      all with StartM=1,
    //      with StartM>1 if: tof and tofino are available and  no cross-talk effect occurred.
    //                     
    if((multStart==1) || ((multStart>1) && tofdet && tofinodet && (nWasCrossTalk !=1))){ 
      hit->setModule(0);
      hit->setStrip(strip);
      hit->setTime(time);
      hit->setStartMult(multStart);
      if(fControlHistograms == kTRUE) histStartStat->Fill(1.);
      HStartSelfCoParChan &pPar=(*selfcopar)[0][strip];
      acp = pPar.getTime();
      acw = pPar.getHalfWidth();
      if((TMath::Abs(time-acp)) < acw){
	   hit->setFlag(kTRUE);
	   if(fControlHistograms == kTRUE){
          histStartH[strip]->Fill(time);
	      histStartStat->Fill(2.);
        }
      }
    }
  }
  Int_t trigbits=gHades->getCurrentEvent()->getHeader()->getTBit();
  if((trigbits >> 8) & 0x1) return 0;

  return (hit->getFlag())?0:kSkipEvent;
}

Bool_t HStartHitF::finalize(void) {
  return kTRUE;
}

ClassImp(HStartHitF)
