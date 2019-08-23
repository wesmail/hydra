#ifndef HRTMETATRACKIO_H
#define HRTMETATRACKIO_H 
   
#include "hrttrackio.h"
#include "hiterator.h"

class HRtMetaTrackIo : public HRtTrackIo {
 public:
  HRtMetaTrackIo(void);
  ~HRtMetaTrackIo(void);
  virtual Bool_t init(HEvent *ev,HRuntimeDb *rtdb);
  Int_t readOuterSegments(TClonesArray &segArray);
  void acceptTrack(HRtVector &p,HRtMatrix &cov,Float_t chi2,
		   HMdcSeg *innerSeg,HRtSegmentRef *outerSeg,
		   HRtTrackEvaluator *evaluator);
  
 protected:
  HIterator *fMetaIter; //! public:
  ClassDef(HRtMetaTrackIo,1)
};

#endif
