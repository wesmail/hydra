//*-- Author : Dan Magestro
//*-- Created: 01/10/01
//*-- Last modified: 07/11/01

#ifndef HCUT_H
#define HCUT_H

class HCut : public TNamed {

public:
  HCut();
  HCut(const Text_t *name, const Text_t *title);
  ~HCut() {}

protected:
  Int_t fNCheck;   // number of checks
  Int_t fNGood;    // number of checks which pass cuts

ClassDef(HCut,1) // Base class for physics cuts

};

#endif
