//*-- Author : Dan Magestro
//*-- Created: 03/09/01
//*-- Last modified: 07/11/01

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
  static const char *pid(const Int_t &pid) {return PName[pid]; }

  static Int_t charge(const int &id) {return PCharge[id]; }
  static Int_t charge(const char * id) { return PCharge[pid(id)]; }

  static double mass(const int & id) { return PMass[id]; }
  static double mass(const char * id) { return PMass[pid(id)]; }

  ClassDef(HPhysicsConstants,1) // Class to store physics constants
};

#endif
