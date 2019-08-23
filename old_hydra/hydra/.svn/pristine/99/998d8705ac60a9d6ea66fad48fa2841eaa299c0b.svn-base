// $Id: hrichcuttrackletsim.h,v 1.1 2003-01-24 16:19:37 eberl Exp $
// Last update by Thomas Eberl: 03/01/17 19:45:21
//
#ifndef HRICHCUTTRACKLETSIM_H
#define HRICHCUTTRACKLETSIM_H

#include "hrichcuttracklet.h"

class HHitMatchSim;

class HRichCutTrackletSim : public HRichCutTracklet {

public:
  HRichCutTrackletSim();
  HRichCutTrackletSim(Text_t *name, Text_t *title);
  ~HRichCutTrackletSim() {}

  void printCutList(char *);
  void reset();
  void setStandardCuts();
    void switchTo(char*);
    Bool_t check(HHitMatchSim*);
    void setState(Int_t*,Float_t*);
protected:

  // meta info on cut
  

  // values

    // log whether active



    ClassDef(HRichCutTrackletSim,1) // Single particle cuts

};

#endif
