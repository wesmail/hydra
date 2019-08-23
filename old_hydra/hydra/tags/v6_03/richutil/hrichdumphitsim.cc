// File: hrichdumphitsim.cc
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 01/08/20 20:16:44
//
#include "hrichdumphitsim.h"
#include "hrichdumphit.h"
#include "hrichhitsim.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hrichdetector.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hmatrixcatiter.h"
#include "hlocation.h"
#include "hrichraw.h"
#include "hrichcalpar.h"
#include "hrichcal.h"
#include "hdebug.h"
#include "hades.h"
#include "richdef.h"
#include <iostream.h>

ClassImp(HRichDumpHitSim)

HRichDumpHitSim::HRichDumpHitSim(Text_t *name,Text_t *title) :
  HRichDumpHit(name,title)
{
    fIter = NULL;
    nCounter = 0;
    nEvtCounter = 0;
}

HRichDumpHitSim::HRichDumpHitSim()
{
    fIter = NULL;
    nCounter = 0;
    nEvtCounter = 0;
}


HRichDumpHitSim::~HRichDumpHitSim(void) {
   if (fIter) delete fIter;
}


Bool_t HRichDumpHitSim::init() {

    HRichDetector *pRichDet = (HRichDetector*)gHades->getSetup()
                                                  ->getDetector("Rich");

    fHitCat=gHades->getCurrentEvent()->getCategory(catRichHit);
    cout<<"address of cat rich hit "<<fHitCat<<endl;
    if (!fHitCat) {
	cout<<"HRichDumpHitSim::init <> Message: building new lin cat"<<endl;
      fHitCat=pRichDet->buildMatrixCat("HRichHitSim",1);
      cout<<"address of HRichHitSim category: "<<fHitCat<<endl;
      if (!fHitCat) return kFALSE;
      else gHades->getCurrentEvent()
                         ->addCategory(catRichHit, fHitCat, "Rich");
    }

    fIter = (HIterator*)getHitCat()->MakeIterator(); 
    return kTRUE;
}

Bool_t HRichDumpHitSim::finalize() {

    return kTRUE;
}


Int_t HRichDumpHitSim::execute()
{
  HRichHitSim *pHit;
  fIter->Reset(); 
  while((pHit = (HRichHitSim *)fIter->Next()))
  {
      nCounter++;
      cout<<"evt no:"<<nEvtCounter<<" theta:"<<pHit->getTheta()<<" phi:"<<pHit->getPhi()<<" sec:"<<pHit->getSector()<<" ring nb:"<<nCounter<<endl;
      cout<<"sim test: "<<pHit->track1<<endl;
  }
  
  nEvtCounter++;
  return 0;
}


