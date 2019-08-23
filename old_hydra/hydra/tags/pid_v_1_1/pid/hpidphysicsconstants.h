// @(#)$Id: hpidphysicsconstants.h,v 1.4 2003-03-06 15:24:32 jaskula Exp $
//*-- Author : Dan Magestro
//*-- Created: 03/09/01
//*-- Copy into Pid lib and changing the class name by Marcin Jaskula
//    15/10/02
//*-- Modified     : 24/02/03 by Marcin Jaskula
//                   new ids for fakes, get rid of TObject

#ifndef HPIDPHYSICSCONSTANTS_H
#define HPIDPHYSICSCONSTANTS_H

// -----------------------------------------------------------------------------

#include <TObject.h>

// -----------------------------------------------------------------------------

class HPidCommonTrack;
class HGeantKine;

// -----------------------------------------------------------------------------

class HPidPhysicsConstants
{
public:
    static Int_t       pid(const Char_t *pidName);
    static const char* pid(Short_t pid);

    static Int_t       charge(const int id);
    static Int_t       charge(const char *id) { return charge(pid(id)); }

    static Float_t     mass(const int id);
    static Float_t     mass(const char *id)   { return mass(pid(id));   }

    static Bool_t      isFake(Int_t iId);
    static Bool_t      isArtificial(Int_t iId);

    // -------------------------------------------------------------------------

    static Short_t     artificialPos(void)   { return -4; }
    static Short_t     artificialNeg(void)   { return -3; }
    static Short_t     fakePos(void)        { return -2; }
    static Short_t     fakeNeg(void)        { return -1; }

    // -------------------------------------------------------------------------

    static UInt_t      getProcess(HGeantKine *pKine, UInt_t iMask);

    // -------------------------------------------------------------------------
};

// -----------------------------------------------------------------------------

#endif
