// $Id: hrichcutpairsim.h,v 1.4 2004-10-15 08:25:23 eberl Exp $
// Last update by Thomas Eberl: 04/07/12 18:20:32
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
    Bool_t isPhysCBPair(HDiHitMatchSim*);
    Bool_t isFakePair(HDiHitMatchSim*);
    Bool_t isConvPair(HDiHitMatchSim*);
    Bool_t isDalitzPair(HDiHitMatchSim*);
    Bool_t isEtaDalitzPair(HDiHitMatchSim*);
    Bool_t isAllCombiPair(HDiHitMatchSim*);
    Bool_t isLepCombiPair(HDiHitMatchSim*);

  // meta info on cut
    Bool_t isSim; //!

  // values

    

  // log whether active
    Int_t kisPhysCBPair;
    Int_t kisFakePair;
    Int_t kisConvPair;
    Int_t kisDalitzPair;
    Int_t kisEtaDalitzPair;
    Int_t kisAllCombiPair;
    Int_t kisLepCombiPair;

    ClassDef(HRichCutPairSim,1) // Single particle cuts

};

#endif
