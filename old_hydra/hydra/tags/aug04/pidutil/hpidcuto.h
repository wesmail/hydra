// $Id: hpidcuto.h,v 1.1 2003-02-26 08:51:19 eberl Exp $
// Last update by Thomas Eberl: 03/02/26 09:25:55
// inspired by Dan's phyana class
#ifndef HPIDCUTO_H
#define HPIDCUTO_H

class TNamed;
class HPidCutO : public TNamed {

public:
  HPidCutO();
  HPidCutO(const Text_t *name, const Text_t *title);
  ~HPidCutO() {}
  virtual Bool_t check(TObject*){return kFALSE;};
protected:
  Int_t fNCheck;   // number of checks
  Int_t fNGood;    // number of checks which pass cuts

ClassDef(HPidCutO,1) // Base class for cuts

};

#endif
