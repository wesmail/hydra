#include "hrichhit.h"
#include "hrichhitheader.h"
#include "hrichhitfinder.h"
#include "hevent.h"
#include "TArrayI.h"
#include "hratreeext.h"
#include "hcategory.h"
#include "hmatrixcategory.h"
#include "hlinearcatiter.h"
#include "hlocation.h"
#include "hrichcal.h"
//#include "hrichhitfpar.h"
#include "hdebug.h"
#include "hades.h"
#include "richdef.h"


ClassImp(HRichHitFinder)

HRichHitFinder::HRichHitFinder(Text_t *name,Text_t *title) :
  HReconstructor(name,title)
{
  init();
  fIter = NULL;
}

HRichHitFinder::HRichHitFinder()
{
  init();
  fIter = NULL;
}


HRichHitFinder::~HRichHitFinder(void) {
    if (m_pCellArr) {
      delete m_pCellArr;
    }

    if (fIter) delete fIter;
}

Bool_t HRichHitFinder::init() {
    TArrayI arr(4);
    arr[0] = 6;
    arr[1] = 3;
    arr[2] = 32;
    arr[3] = 32;

    m_pCalCat=gHades->getCurrentEvent()->getCategory(catRichCal);
    m_pHitCat=gHades->getCurrentEvent()->getCategory(catRichHit);
    m_pHitHdrCat=gHades->getCurrentEvent()->getCategory(catRichHitHdr);
    m_pCellArr = new HRaTreeExt(m_pCalCat, &arr);

    m_pHitFPar=NULL;
  
    fIter = (HLinearCatIter*)getCalCat()->MakeIterator(); 
    return kTRUE;
}

Bool_t HRichHitFinder::finalize(void) {
    return kTRUE;
}

HRichHitFinder& HRichHitFinder::operator=(HRichHitFinder &c) {

  return c;
}

Int_t HRichHitFinder::execute()
{
  HRichCal *pCal;
  HLocation loc;

  m_pCellArr->update();
 
  fIter->Reset(); 
  Int_t n = 0;
  Int_t hits = 0;

  while((pCal = (HRichCal *)fIter->Next()))
  {
     loc = fIter->getLocation();
     if (lookForHit(loc)) hits++;
     n++;
  }

  return n;
}

Bool_t HRichHitFinder::lookForHit(HLocation& fLoc) {
#if DEBUG_LEVEL>2
  gDebuger->enterFunc("HRichHitFinder::execute");
  gDebuger->message("Cal cat points to %p",fCalCat);
#endif
  HRichCal *cal=NULL;
  HRichHit *hit=NULL;
  Bool_t isHit = kFALSE;

  cal=(HRichCal *)m_pCalCat->getObject(fLoc);

  if (cal) {
    Float_t fCharge;

    hit=(HRichHit *)m_pHitCat->getNewSlot(fLoc);
    if (hit!=NULL) {
      hit=new(hit) HRichHit;

      hit->setSector(cal->getSector());
      hit->setEventNr(cal->getEventNr());

      isHit = kTRUE;
   }
  }
#if DEBUG_LEVEL>2
  gDebuger->leaveFunc("HRichHitFinder::execute");
#endif
  return isHit;
}



