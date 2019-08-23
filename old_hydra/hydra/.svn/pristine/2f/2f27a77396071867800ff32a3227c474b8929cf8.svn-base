#include "htofhitf.h"
#include "hades.h"
#include "htofraw.h"
#include "htofhit.h"
#include "htofcalpar.h"
#include "hruntimedb.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hdebug.h"
#include "tofdef.h"
#include "hevent.h"
#include "heventheader.h"
#include "hspectrometer.h"
#include "hdetector.h"

//*-- Author : 
//*-- Modified : 17/03/2000 by R. Holzmann
//*-- Modified : 30/11/99 by M. Sanchez
//*-- Modified : 02/11/99 by D. Vasiliev
//*-- Modified : 15/06/98 by M. Sanchez

/////////////////////////////////////////////////////////////////////////
// HTofHitF Tof hit finder
//
// adapted from /u/halo/packages/abase/new by Manuel Sanchez (17/06/98)
/////////////////////////////////////////////////////////////////////////

void HTofHitF::initParContainer() {
  fCalPar=(HTofCalPar *)gHades->getRuntimeDb()->getContainer("TofCalPar");
  if (!fCalPar) {
    fCalPar=new HTofCalPar;
    gHades->getRuntimeDb()->addContainer(fCalPar);
  }
}

HTofHitF::HTofHitF(void) {
  fCalPar = NULL;
  fRawCat = fHitCat = NULL;
  initParContainer();
  fLoc.set(3,0,0,0);
  iter = NULL;
}

HTofHitF::HTofHitF(Text_t *name,Text_t *title) : HReconstructor (name,title) {
  fCalPar = NULL;
  fRawCat = fHitCat = NULL;
  initParContainer();
  fLoc.set(3,0,0,0);
  iter = NULL;
}

HTofHitF::~HTofHitF(void) {
  if(iter) delete iter;
}

Bool_t HTofHitF::init(void) {
  fRawCat=gHades->getCurrentEvent()->getCategory(catTofRaw);
  if (!fRawCat) {
    fRawCat=gHades->getSetup()->getDetector("Tof")->buildCategory(catTofRaw);
    if (!fRawCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catTofRaw,fRawCat,"Tof");
  }
  iter=(HIterator*)fRawCat->MakeIterator("native");

  fHitCat=gHades->getCurrentEvent()->getCategory(catTofHit);
  if (!fHitCat) {
    fHitCat=gHades->getSetup()->getDetector("Tof")->buildCategory(catTofHit);
    if (!fHitCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catTofHit,fHitCat,"Tof");
  }
  
  return kTRUE;
}

Int_t HTofHitF::execute(void) {
#if DEBUG_LEVEL>2
  gDebuger->enterFunc("HTofHitF::execute");
#endif

  HTofRaw *raw=NULL;
  HTofHit *hit=NULL;
  HTofCalParCell *cell = NULL;

  Float_t atof;
  Float_t axpos;
  Float_t xtof;
  Int_t ytof;
  Int_t multTof;

  multTof=0;
  xtof = 0.0;
  ytof = 0;

  iter->Reset();

  while ( (raw=(HTofRaw *)iter->Next())!=NULL) {

    fLoc[0]=raw->getSector();
    fLoc[1]=raw->getModule();
    fLoc[2]=raw->getCell();

    hit = (HTofHit *)fHitCat->getSlot(fLoc);

    if (hit) {
      hit=new(hit) HTofHit;
      cell=fCalPar->getCell(fLoc);

      if(raw->getLeftTime() && raw->getRightTime()) {

	atof = (raw->getLeftTime() * cell->getLeftK() +
      	        raw->getRightTime()*cell->getRightK())/2.0 - cell->getTimK();
	
	axpos = cell->getVGroup()*(raw->getLeftTime() * cell->getLeftK() -
		raw->getRightTime()*cell->getRightK())/2.0 -cell->getPosK();

	multTof++;
	xtof = -axpos;   // x axis is inverted
	ytof = 8*fLoc[1]+fLoc[2];

	hit->setTof(atof);
	hit->setXpos(xtof);
	hit->setSector((Char_t) fLoc[0]);
	hit->setModule((Char_t) fLoc[1]);
	hit->setCell((Char_t) fLoc[2]);
      } else {
	hit->setTof(-1.0);
	hit->setXpos(-200.0);
	hit->setSector((Char_t) fLoc[0]);
	hit->setModule((Char_t) fLoc[1]);
	hit->setCell((Char_t) fLoc[2]);
//	gDebuger->message("Tof hit is incomplete!");
      }
    }
  }

#if DEBUG_LEVEL>2
  gDebuger->leaveFunc("HTofHitF::execute");
#endif
  return 0;
}
  

ClassImp(HTofHitF)














