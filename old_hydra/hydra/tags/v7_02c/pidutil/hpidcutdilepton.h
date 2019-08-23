// $Id: hpidcutdilepton.h,v 1.1 2003-02-26 08:50:20 eberl Exp $
// Last update by Thomas Eberl: 03/02/25 16:11:41
//
#ifndef HPIDCUTDILEPTON_H
#define HPIDCUTDILEPTON_H

#include "hpidcuto.h"

class HPidDilepton;

class HPidCutDilepton : public HPidCutO {

public:
    HPidCutDilepton();
    HPidCutDilepton(Text_t *name, Text_t *title);
    ~HPidCutDilepton() {}
    
    Bool_t check(HPidDilepton*);
    void printCutList(char *);
    void reset();
    void switchTo(char *);
    void setStandardCuts();
    void setState(Int_t*,Float_t*);


    

protected:
    Bool_t isOpangGreaterThan(HPidDilepton*);
    Bool_t isOpangLowerThan(HPidDilepton*);
    Bool_t isLikePPair(HPidDilepton*);
    Bool_t isLikeMPair(HPidDilepton*);
    Bool_t isUnLikePair(HPidDilepton*);
    Bool_t isMom1GreaterThan(HPidDilepton*);
    Bool_t isMom1LowerThan(HPidDilepton*);
    Bool_t isMom2GreaterThan(HPidDilepton*);
    Bool_t isMom2LowerThan(HPidDilepton*);
    Bool_t is1MDCPair(HPidDilepton*);
    Bool_t is1RICHPair(HPidDilepton*);
    Bool_t is2HITPair(HPidDilepton*);

  // meta info on cut
  

  // values
    Float_t nPairOpang;
    Float_t nPairMinMom1;
    Float_t nPairMinMom2;
    
    // log whether active

    Int_t kPairOpangGreaterThan;
    Int_t kPairOpangLowerThan;
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
    ClassDef(HPidCutDilepton,1) // Dilepton cuts

};

#endif
