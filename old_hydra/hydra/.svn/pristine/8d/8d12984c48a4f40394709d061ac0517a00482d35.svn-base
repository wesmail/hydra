// $Id: hrichcutpairsim.h,v 1.1 2003-01-24 16:18:58 eberl Exp $
// Last update by Thomas Eberl: 03/01/17 19:36:27
//
#ifndef HRICHCUTPAIRSIM_H
#define HRICHCUTPAIRSIM_H

#include "hrichcutpair.h"

class HDiHitMatchSim;

class HRichCutPairSim : public HRichCutPair {

public:
    HRichCutPairSim();
    HRichCutPairSim(Text_t *name, Text_t *title);
    ~HRichCutPairSim() {}
    
    Bool_t check(HDiHitMatchSim*);
    void printCutList(char *);
    void reset();
    void switchTo(char *);
    void setStandardCuts();
    void setState(Int_t*,Float_t*);


protected:

  // meta info on cut
  

  // values
    
    // log whether active

    ClassDef(HRichCutPairSim,1) // Single particle cuts

};

#endif
