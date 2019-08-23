//*-- Author : Dan Magestro
//*-- Created: 03/09/01
//*-- Last modified: 06/05/05

#ifndef HPHYSICSCONSTANTS_H
#define HPHYSICSCONSTANTS_H

#include "TObject.h"

class HPhysicsConstants : public TObject {

private:
  static double *PMass;
  static int *PCharge;
  static char **PName;

public:
  static Int_t pid(const Char_t *pidName);
  static const char *pid(const Int_t &id);

  static Int_t charge(const int &id);
  static Int_t charge(const char *id) { return PCharge[pid(id)]; }

  static double mass(const int &id);
  static double mass(const char *id) { return PMass[pid(id)]; }

  ClassDef(HPhysicsConstants,1) // Class to store physics constants
};

#endif
