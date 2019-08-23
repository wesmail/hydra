// @(#)$Id: hpidparcont.h,v 1.1 2002-10-30 10:55:33 halo Exp $
//*-- Author : Marcin Jaskula 09/10/2002

#ifndef HPIDPARCONT_H
#define HPIDPARCONT_H

////////////////////////////////////////////////////////////////////////////////
//
// HPidParCont
//
// Base class for all containers which do not belong to any detector
//
////////////////////////////////////////////////////////////////////////////////

#include "hparcond.h"

// -----------------------------------------------------------------------------

#define PID_PAR_IO_NAME "PidParIo"

// -----------------------------------------------------------------------------

class HPidParCont : public HParCond
{
public:

                    HPidParCont(const char *name, const char *title,
                                const char *context);
    virtual        ~HPidParCont(void);

    virtual Bool_t  init(void);
    virtual Bool_t  init(HParIo *inp);
    virtual Bool_t  init(HParIo *inp, Int_t *)
                            { return (inp != NULL) ? init(inp) : init(); }

    virtual Int_t   write(HParIo *io);

    // -------------------------------------------------------------------------

    virtual void    detachHistograms(void);

    // -------------------------------------------------------------------------

    static Bool_t   makePidParIo(void);

    // -------------------------------------------------------------------------

    ClassDef(HPidParCont, 1)  // Base class for all containers which do not belong to any detector
};

// -----------------------------------------------------------------------------

#endif  /* !HPIDPARCONT_H */
