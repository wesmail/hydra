// $Id: hrichcutpairsim.h,v 1.6 2009-07-15 11:39:21 halo Exp $
// Last update by Thomas Eberl: 04/07/12 18:20:32
//
#ifndef HRICHCUTPAIRSIM_H
#define HRICHCUTPAIRSIM_H

#include "hrichcutpair.h"

class HDiHitMatchSim;

class HRichCutPairSim : public HRichCutPair {

public:
    HRichCutPairSim();
    HRichCutPairSim(const Text_t *name,const Text_t *title);
    ~HRichCutPairSim() {}

    void printCutList(const Char_t *);
    void printCutList();
    void reset();
    void setStandardCuts();
    void getDalitzPairTrkNb(HDiHitMatchSim*,Int_t&,Int_t&);
    void getConvPairTrkNb(HDiHitMatchSim*,Int_t&,Int_t&);
    Bool_t switchTo(const Char_t*,Bool_t clear=kTRUE);
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
