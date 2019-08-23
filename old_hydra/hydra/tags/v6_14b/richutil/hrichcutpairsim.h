// $Id: hrichcutpairsim.h,v 1.3 2003-09-09 15:43:15 eberl Exp $
// Last update by Thomas Eberl: 03/07/18 17:24:28
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
    void getDalitzPairTrkNb(HDiHitMatchSim*,Int_t&,Int_t&);
    void getConvPairTrkNb(HDiHitMatchSim*,Int_t&,Int_t&);
    Bool_t switchTo(const char*,Bool_t clear=kTRUE);
    Bool_t check(HDiHitMatchSim*);
protected:
    Bool_t isConvPair(HDiHitMatchSim*);
    Bool_t isDalitzPair(HDiHitMatchSim*);
    Bool_t isEtaDalitzPair(HDiHitMatchSim*);
    Bool_t isAllCombiPair(HDiHitMatchSim*);
    Bool_t isLepCombiPair(HDiHitMatchSim*);

  // meta info on cut
    Bool_t isSim; //!

  // values

    

  // log whether active
    Int_t kisConvPair;
    Int_t kisDalitzPair;
    Int_t kisEtaDalitzPair;
    Int_t kisAllCombiPair;
    Int_t kisLepCombiPair;

    ClassDef(HRichCutPairSim,1) // Single particle cuts

};

#endif
