#include "htofhitf.h"
#include "hades.h"
#include "htofcal.h"
#include "htofhit.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hdebug.h"
#include "tofdef.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"

//*-- Author : 
//*-- Modified : 15/06/98 by M. Sanchez

////////////////////////
//HTofHitF
//
// adapted from /u/halo/packages/abase/new by Manuel Sanchez (17/06/98)
////////////////////////

HTofHitF::HTofHitF(void) {
  fLoc.set(3,0,0,0);
}

HTofHitF::HTofHitF(Text_t *name,Text_t *title) : HReconstructor (name,title) {
  fLoc.set(3,0,0,0);
}

Bool_t HTofHitF::init(void) {
  fCalCat=gHades->getCurrentEvent()->getCategory(catTofCal);
  if (!fCalCat) {
    fCalCat=gHades->getSetup()->getDetector("Tof")->buildCategory(catTofCal);
    if (!fCalCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catTofCal,fCalCat,"Tof");
  }
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
  HIterator *iter=NULL;
  HTofCal *cal=NULL;
  HTofHit *hit=NULL;
  Float_t velGr=15.4; // Esto deberia ser un data member.
  iter=(HIterator *)fCalCat->MakeIterator();
  while ( (cal=(HTofCal *)iter->Next())!=NULL) {
    if (cal->getSize()>0) {
      fLoc[0]=cal->getSector();
      fLoc[1]=cal->getModule();
      fLoc[2]=cal->getCell();
      hit=(HTofHit *)fHitCat->getSlot(fLoc);
      if (hit) {
	hit=new(hit) HTofHit;
	hit->setX((cal->getTimeLeft()+cal->getTimeRight())/2.0);
	hit->setY((velGr*(cal->getTimeLeft()-cal->getTimeRight()))/2.0);
      } else {
	gDebuger->message("No habia sitio");
	return -1; //REVISAR ESTO
      }
    }
  }
  //delete iter;
#if DEBUG_LEVEL>2
  gDebuger->leaveFunc("HTofHitF::execute");
#endif
  return 0;
}
  

ClassImp(HTofHitF)

