// @(#)$Id: hbpidphysicsconstants.h,v 1.2 2008-05-09 16:52:44 halo Exp $
//*-- Author : Dan Magestro
//*-- Created: 03/09/01
//*-- Copy into Pid lib and changing the class name by Marcin Jaskula
//    15/10/02
//*-- Modified     : 24/02/03 by Marcin Jaskula
//                   new ids for fakes, get rid of TObject
//*-- Modified     : 03/11/06 by Alexander Belyaev
//                   leptonCharge, baryonCharge and strangeness are added

#ifndef HBPIDPHYSICSCONSTANTS_H
#define HBPIDPHYSICSCONSTANTS_H

// -----------------------------------------------------------------------------

#include <TObject.h>

// -----------------------------------------------------------------------------

class HGeantKine;

// -----------------------------------------------------------------------------

class HBPidPhysicsConstants : public TObject
{
public:
    static Int_t       pid(const Char_t *pidName);
    static const Char_t* pid(Short_t pid);

    static Int_t       charge(const Int_t id);
    static Int_t       charge(const Char_t *id) { return charge(pid(id)); }

    static Int_t       leptonCharge(const Int_t id);
    static Int_t       leptonCharge(const Char_t *id) { return leptonCharge(pid(id)); }

    static Int_t       baryonCharge(const Int_t id);
    static Int_t       baryonCharge(const Char_t *id) { return baryonCharge(pid(id)); }

    static Int_t       strangeness(const Int_t id);
    static Int_t       strangeness(const Char_t *id) { return strangeness(pid(id)); }

    static Float_t     mass(const Int_t id);
    static Float_t     mass(const Char_t *id)   { return mass(pid(id));   }

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
    ClassDef(HBPidPhysicsConstants,0)
};

// -----------------------------------------------------------------------------

#endif
