// $Id: hpidcutparticle.h,v 1.1 2003-02-26 08:50:48 eberl Exp $
// Last update by Thomas Eberl: 03/02/26 08:31:56
//
#ifndef HPIDCUTPARTICLE_H
#define HPIDCUTPARTICLE_H

#include "hpidcuto.h"

class HPidParticle;

class HPidCutParticle : public HPidCutO {

public:
  HPidCutParticle();
  HPidCutParticle(Text_t *name, Text_t *title);
  ~HPidCutParticle() {}

  void printCutList(char *);
  void reset();
  void setStandardCuts();
    void switchTo(char*);
    Bool_t check(HPidParticle*);
    void setState(Int_t*,Float_t*);
protected:
    
    Bool_t isBetaExpMin(HPidParticle *);
    Bool_t isLepton(HPidParticle*);
    // meta info on cut
    
    
    // values
    Float_t nPartBetaExpMin;
    // log whether active
    Bool_t kPartBetaExpMin;
    Bool_t kPartLepton;
    

    ClassDef(HPidCutParticle,1) // Single particle cuts

};

#endif
