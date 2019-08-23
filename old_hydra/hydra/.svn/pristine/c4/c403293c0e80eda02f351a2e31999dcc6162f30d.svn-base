// $Id: hrichcutpairsim.h,v 1.2 2003-04-04 13:13:28 eberl Exp $
// Last update by Thomas Eberl: 03/03/10 14:42:55
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

    void printCutList(char *);
    void printCutList();
    void reset();
    void setStandardCuts();
    Bool_t switchTo(const char*,Bool_t clear=kTRUE);
    Bool_t check(HDiHitMatchSim*);
protected:
    Bool_t isConvPair(HDiHitMatchSim*);
    Bool_t isDalitzPair(HDiHitMatchSim*);

  // meta info on cut
    Bool_t isSim; //!

  // values

    

  // log whether active
    Int_t kisConvPair;
    Int_t kisDalitzPair;

    ClassDef(HRichCutPairSim,1) // Single particle cuts

};

#endif
