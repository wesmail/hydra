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
//*-- Modified : 30/11/99 by M. Sanchez
//*-- Modified : 15/06/98 by M. Sanchez
//*-- Modified : 02/11/99 by D. Vasiliev

////////////////////////
//HTofHitF
//
// adapted from /u/halo/packages/abase/new by Manuel Sanchez (17/06/98)
////////////////////////

Float_t xtof;
Int_t ytof;
//FILE *fpTof;

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
}

HTofHitF::HTofHitF(Text_t *name,Text_t *title) : HReconstructor (name,title) {
  fCalPar = NULL;
  fRawCat = fHitCat = NULL;
  initParContainer();
  fLoc.set(3,0,0,0);
}

Bool_t HTofHitF::init(void) {
  fRawCat=gHades->getCurrentEvent()->getCategory(catTofRaw);
  if (!fRawCat) {
    fRawCat=gHades->getSetup()->getDetector("Tof")->buildCategory(catTofRaw);
    if (!fRawCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catTofRaw,fRawCat,"Tof");
  }
  fHitCat=gHades->getCurrentEvent()->getCategory(catTofHit);
  if (!fHitCat) {
    fHitCat=gHades->getSetup()->getDetector("Tof")->buildCategory(catTofHit);
    if (!fHitCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catTofHit,fHitCat,"Tof");
  }

  //  fpTof = fopen("tofxy.txt","w");
  //  if(!fpTof) printf("Error while opening file tofxy.txt\n");
  return kTRUE;
}

Int_t HTofHitF::execute(void) {
#if DEBUG_LEVEL>2
  gDebuger->enterFunc("HTofHitF::execute");
#endif

  HIterator *iter=NULL;
  HTofRaw *raw=NULL;
  HTofHit *hit=NULL;
  HTofCalParCell *cell = NULL;

  //  Float_t velGr=15.4; // Esto deberia ser un data member.
  //  Float_t kL, kR;
  //  Float_t k1;
  //  kL = kR = 0.05;
  //  k1 = 40.;

  Float_t atof;
  Float_t axpos;

  Int_t multTof;

  Int_t evtID;
  evtID = gHades->getCurrentEvent()->getHeader()->getEventSeqNumber();

  multTof=0;
  xtof = 0.0;
  ytof = 0;

  iter=(HIterator *)fRawCat->MakeIterator();
  while ( (raw=(HTofRaw *)iter->Next())!=NULL) {

    fLoc[0]=raw->getSector();
    fLoc[1]=raw->getModule();
    fLoc[2]=raw->getCell();

    raw = (HTofRaw*) fRawCat->getObject(fLoc);
    if(raw) {
	
      hit=(HTofHit *)fHitCat->getSlot(fLoc);
      if (hit) {
	hit=new(hit) HTofHit;
	cell=fCalPar->getCell(fLoc);

       	if(raw->getLeftTime() && raw->getRightTime()) {

	  atof = (raw->getLeftTime() * cell->getLeftK() +
		  raw->getRightTime()*cell->getRightK())/2.0 - cell->getTimK();
	
	  axpos = cell->getVGroup()*(raw->getLeftTime() * cell->getLeftK() -
		  raw->getRightTime()*cell->getRightK())/2.0 -cell->getPosK();

	  multTof++;

	//atof = (kL*raw->getLeftTime() + kR*raw->getRightTime())/2.0 - k1;
	//axpos = (velGr*(kL*raw->getLeftTime()-kR*raw->getRightTime()))/2.;

	  xtof = axpos;
	  ytof = 8*fLoc[1]+fLoc[2];

	  hit->setTof(atof);
	  hit->setXpos(axpos);
	  hit->setSector((Char_t) fLoc[0]);
	  hit->setModule((Char_t) fLoc[1]);
	  hit->setCell((Char_t) fLoc[2]);
      	}
      } else {
	gDebuger->message("No habia sitio");
	return -1; //REVISAR ESTO
      }
    }
  }

  if(multTof==1) {

    //    printf("evt = %d\t  %f\t  %d\n",evtID,xtof,ytof);
    //    if(fpTof) fprintf(fpTof,"%d\t%f\t%d\n",evtID,xtof,ytof);

  }

  delete iter;
#if DEBUG_LEVEL>2
  gDebuger->leaveFunc("HTofHitF::execute");
#endif
  return 0;
}
  

ClassImp(HTofHitF)







