// @(#)$Id: hpidparcont.h,v 1.2 2002-11-04 02:44:28 jaskula Exp $
//*-- Author : Marcin Jaskula 09/10/2002
// Modified  : Marcin Jaskula 31/10/2002
//           - virtual Print() and print() calling printParams() added

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

    virtual void    Print(Option_t *p = "")    { printParams();    }
    virtual void    print(void)                { printParams();    }

    // -------------------------------------------------------------------------

    static Bool_t   makePidParIo(void);

    // -------------------------------------------------------------------------

    ClassDef(HPidParCont, 1)  // Base class for all containers which do not belong to any detector
};

// -----------------------------------------------------------------------------

#endif  /* !HPIDPARCONT_H */
