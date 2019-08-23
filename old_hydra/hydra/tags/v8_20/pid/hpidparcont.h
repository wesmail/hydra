// @(#)$Id: hpidparcont.h,v 1.6 2008-05-09 16:15:06 halo Exp $
//*-- Author : Marcin Jaskula 09/10/2002
// Modified  : Marcin Jaskula 31/10/2002
//           - virtual Print() and print() calling printParams() added
// Modified  : Marcin Jaskula & Tomasz Wojcik 26/06/2002
//             Oracle interface added

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

#define PID_PAR_IO_NAME     "PidParIo"
#define PID_PAR_ORA_IO_NAME "HCondParIo"

// -----------------------------------------------------------------------------

class HPidParCont : public HParCond
{
public:

                    HPidParCont(const Char_t *name, const Char_t *title,
                                const Char_t *context);
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

    void    putParams(HParamList *) {}
    Bool_t  getParams(HParamList *) { return kTRUE; }

    ClassDef(HPidParCont, 1)  // Base class for all containers which do not belong to any detector
};

// -----------------------------------------------------------------------------

#endif  /* !HPIDPARCONT_H */




