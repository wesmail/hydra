// $Id: hrichcuttrackletsim.h,v 1.2 2003-04-04 13:13:47 eberl Exp $
// Last update by Thomas Eberl: 03/03/10 14:12:35
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
  void printCutList();
  void reset();
  void setStandardCuts();
    Bool_t switchTo(const char*,Bool_t clear=kTRUE);
    Bool_t check(HHitMatchSim*);
protected:

    Bool_t isConv(HHitMatchSim*);
    Bool_t isDalitz(HHitMatchSim*);

  // meta info on cut
    Bool_t isSim;//!

  // values

    

  // log whether active
    Int_t kisConv;
    Int_t kisDalitz;


    ClassDef(HRichCutTrackletSim,1) // Single particle cuts

};

#endif
