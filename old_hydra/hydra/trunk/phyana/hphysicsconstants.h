//*-- Author : Dan Magestro
//*-- Created: 03/09/01
//*-- Last modified: 06/05/05

#ifndef HPHYSICSCONSTANTS_H
#define HPHYSICSCONSTANTS_H

#include "TObject.h"

class HPhysicsConstants : public TObject {

private:
  static Double_t *PMass;
  static Int_t *PCharge;
  static Char_t **PName;

public:
  static Int_t pid(const Char_t *pidName);
  static const Char_t *pid(const Int_t &id);

  static Int_t charge(const Int_t &id);
  static Int_t charge(const Char_t *id) { return PCharge[pid(id)]; }

  static Double_t mass(const Int_t &id);
  static Double_t mass(const Char_t *id) { return PMass[pid(id)]; }

  ClassDef(HPhysicsConstants,1) // Class to store physics constants
};

#endif
