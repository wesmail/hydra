//*-- Author : Dan Magestro
//*-- Created: 03/09/01
//*-- Copy into Pid lib and changing the class name by Marcin Jaskula
//    15/10/02
//*-- Last modified: 15/10/02

#ifndef HPIDPHYSICSCONSTANTS_H
#define HPIDPHYSICSCONSTANTS_H

// -----------------------------------------------------------------------------

#include <TObject.h>

// -----------------------------------------------------------------------------

#define ARTIFICAL_PID_OFFSET 500

// -----------------------------------------------------------------------------

class HPidPhysicsConstants : public TObject
{
public:
    static Int_t pid(const Char_t *pidName);
    static const char *pid(const Int_t &pid);

    static Int_t charge(const int &id);
    static Int_t charge(const char * id) { return charge(pid(id)); }

    static Float_t mass(const int & id);
    static Float_t mass(const char * id) { return mass(pid(id));   }

    ClassDef(HPidPhysicsConstants, 0) // Class to store physics constants
};

// -----------------------------------------------------------------------------

#endif
