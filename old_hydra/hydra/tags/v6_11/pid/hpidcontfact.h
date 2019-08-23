// @(#)$Id: hpidcontfact.h,v 1.1 2002-10-30 10:55:33 halo Exp $
//*-- Author : Marcin Jaskula 09/10/2002

#ifndef HPIDCONTFACT_H
#define HPIDCONTFACT_H

////////////////////////////////////////////////////////////////////////////////
//
// HPidContFact
//
////////////////////////////////////////////////////////////////////////////////

#include "hcontfact.h"

// -----------------------------------------------------------------------------

class HParSet;
class HContainer;

// -----------------------------------------------------------------------------

class HPidContFact : public HContFact
{
private:
    void     setAllContainers(void);

public:
             HPidContFact(void);
            ~HPidContFact() {}
    HParSet* createContainer(HContainer *);

    // -------------------------------------------------------------------------

    ClassDef(HPidContFact,0) // Factory for parameter containers in libPid
};

#endif  /* !HPIDCONTFACT_H */
