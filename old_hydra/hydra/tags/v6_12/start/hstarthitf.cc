using namespace std;
#include "hstartcalibrater.h"
#include "hstartdef.h"
#include "hstartcal.h"
#include "hstartselfcopar.h"
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
#include "TMath.h"
#include <iostream> 
#include <iomanip>
#include "TH1.h"

//*--Author : D.Zovinec

///////////////////////////////////////////////////////////////////////////////
//  HStartHitF
//
//  This class implements the hit finder for the Start detector.
//  It iterates over the CAL level data objects and finds one that is supposed
//  to be the true Start hit.
//  Selection conditions:   - Start detector multiplicity  == 1
//                          - Start cal time is found in the interval
//                            defined around the peak of the selfcoincidence.
//
///////////////////////////////////////////////////////////////////////////////


HStartHitF::HStartHitF(void) {
  calCat=0;
  hitCat=0;
  iter=0;
  selfcopar=0;
  fHistograms=new TList;
  fControlHistograms=kFALSE;
}

HStartHitF::HStartHitF(Text_t *name,Text_t *title) :
               HReconstructor(name,title) {
  calCat=0;
  hitCat=0;
  iter=0;
  selfcopar=0;
  fHistograms=new TList;
  fControlHistograms=kFALSE;
}

HStartHitF::~HStartHitF(void) {
  if (iter) delete iter;
}

Bool_t HStartHitF::init(void) {

  selfcopar=(HStartSelfCoPar*)gHades->getRuntimeDb()->getContainer("StartSelfCoPar");
  if (!selfcopar) return kFALSE;

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

Int_t HStartHitF::execute(void) {
 
  HStartCal *cal=0;
  HStartHit *hit=0;
  Float_t time= -1000;
  Int_t strip= -1;
  Int_t multStart=0;
  Float_t  acp, acw;

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
    }
  }
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
    hit->setFlag(kFALSE);
    if(multStart == 1){
      hit->setModule(0);
      hit->setStrip(strip);
      hit->setTime(time);
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









