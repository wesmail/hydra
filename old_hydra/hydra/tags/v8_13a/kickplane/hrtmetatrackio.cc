#include "hrtmetatrackio.h"
#include "kickdef.h"
#include "hcategory.h"
#include "hevent.h"

//***************** I/O from MDC-Meta
HRtMetaTrackIo::HRtMetaTrackIo(void) {
}

HRtMetaTrackIo::~HRtMetaTrackIo(void) {
  fMetaIter = 0;
}

Bool_t HRtMetaTrackIo::init(HEvent *ev,HRuntimeDb *rtdb) {
  HCategory *catMetaSeg = ev->getCategory(catKickMetaSeg);
  if (!catMetaSeg) return kFALSE;
  fMetaIter = (HIterator *)catMetaSeg->MakeIterator();

  fLocSector.set(1,0);
  fLocOuter.set(2,0,0);

  return kTRUE;
}

Int_t HRtMetaTrackIo::readOuterSegments(TClonesArray &segArray) {
  HRtMetaSeg *seg;
  Int_t nSegments = 0;
  HRtSegmentRef *ref = 0;
  
  segArray.Clear();
  fMetaIter->Reset();
  fMetaIter->gotoLocation(fLocSector);
  while ( (seg = (HRtMetaSeg *)fMetaIter->Next()) != 0) {
    ref = new (segArray[nSegments]) HRtSegmentRef;
    if (!ref) Error ("readOuterSegments", "Memory exhausted");
    nSegments++;
    
    HGeomVector &r = ref->getR ();
    HGeomVector &alpha = ref->getAlpha ();
    
    r = seg->getPosition();
    alpha = seg->getDirection();
    ref->setSegment (seg);
  }

  return nSegments;
}

void HRtMetaTrackIo::acceptTrack(HRtVector &p,HRtMatrix &cov,Float_t chi2,
				 HMdcSeg *innerSeg,HRtSegmentRef *outerSeg,
				 HRtTrackEvaluator *evaluator,
				 HRtFitter::EFitResult fitted) {
}

/*******************************************************************/

ClassImp(HRtMetaTrackIo)
