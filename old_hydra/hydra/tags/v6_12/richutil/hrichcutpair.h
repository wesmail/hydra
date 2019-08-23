// $Id: hrichcutpair.h,v 1.2 2003-04-04 13:13:12 eberl Exp $
// Last update by Thomas Eberl: 03/03/12 18:59:10
//
#ifndef HRICHCUTPAIR_H
#define HRICHCUTPAIR_H
#define MAXPAIRCUTS 10
#include "hrichcuto.h"

class HDiHitMatch;

class HRichCutPair : public HRichCutO {

public:
    HRichCutPair();
    HRichCutPair(Text_t *name, Text_t *title);
    ~HRichCutPair() {}
    
    Bool_t check(HDiHitMatch*);
    void printCutList(char *);
    void printCutList();
    void reset();
    Bool_t switchTo(const char *,Bool_t clear=kTRUE);
    void setStandardCuts();

  //   Float_t getPairOpangKICKCut(){return nPairOpangKICK;};
//     Int_t getPairOpangKICKStatus(){return kPairOpangKICK;};

//     Float_t getPairOpangMDCCut(){return nPairOpangMDC;};
//     Int_t getPairOpangMDCStatus(){return kPairOpangMDC;};

//     Float_t getPairMinMom1Cut(){return nPairMinMom1;};
//     Int_t getPairMinMom1Status(){return kPairMinMom1;};

//     Float_t getPairMinMom2Cut(){return nPairMinMom2;};
//     Int_t getPairMinMom2Status(){return kPairMinMom2;};

//     void setPairOpangKICKCut(Float_t n){nPairOpangKICK=n;};
//     void setPairOpangKICKStatus(Int_t n){ kPairOpangKICK=n;};

//     void setPairOpangMDCCut(Float_t n){ nPairOpangMDC=n;};
//     void setPairOpangMDCStatus(Int_t n){ kPairOpangMDC=n;};

//     void setPairMinMom1Cut(Float_t n){ nPairMinMom1=n;};
//     void setPairMinMom1Status(Int_t n){ kPairMinMom1=n;};

//     void setPairMinMom2Cut(Float_t n){ nPairMinMom2=n;};
//     void setPairMinMom2Status(Int_t n){ kPairMinMom2=n;};

    
    Bool_t isOpangKICKGreaterThan(HDiHitMatch*);
    Bool_t isOpangKICKLowerThan(HDiHitMatch*);
    Bool_t isOpangMDCGreaterThan(HDiHitMatch*);
    Bool_t isOpangMDCLowerThan(HDiHitMatch*);
    Bool_t isLikePPair(HDiHitMatch*);
    Bool_t isLikeMPair(HDiHitMatch*);
    Bool_t isUnLikePair(HDiHitMatch*);
    Bool_t isMom1GreaterThan(HDiHitMatch*);
    Bool_t isMom1LowerThan(HDiHitMatch*);
    Bool_t isMom2GreaterThan(HDiHitMatch*);
    Bool_t isMom2LowerThan(HDiHitMatch*);
    Bool_t is1MDCPair(HDiHitMatch*);
    Bool_t is1RICHPair(HDiHitMatch*);
    Bool_t is2HITPair(HDiHitMatch*);
    Bool_t isNoSameMETA(HDiHitMatch*);
    Bool_t is2MatchedGoodRing(HDiHitMatch*);
    Bool_t is2Fast(HDiHitMatch*);
protected:

  // meta info on cut
    Bool_t isExp;//!
    Bool_t listCut;//!

  // values
    Float_t nPairOpangKICK;
    Float_t nPairOpangMDC;
    Float_t nPairMinMom1;
    Float_t nPairMinMom2;
    
    // log whether active

    Int_t kPairOpangKICKGreaterThan;
    Int_t kPairOpangKICKLowerThan;
    Int_t kPairOpangMDCGreaterThan;
    Int_t kPairOpangMDCLowerThan;
    Int_t kPairMinMom1GreaterThan;
    Int_t kPairMinMom1LowerThan;
    Int_t kPairMinMom2GreaterThan;
    Int_t kPairMinMom2LowerThan;
    Int_t kUnlikePair;
    Int_t kLikePPair;
    Int_t kLikeMPair;
    Int_t kPair1MDC;
    Int_t kPair1RICH;
    Int_t kPair2HIT;
    Int_t kNoSameMETA;
    Int_t k2MatchedGoodRing;
    Int_t k2Fast;
    ClassDef(HRichCutPair,1) // Single particle cuts

};

#endif
