// @(#)$Id: hpidtrackfillerpar.h,v 1.1 2002-10-30 10:55:33 halo Exp $
//*-- Author : Marcin Jaskula 09/10/2002

#ifndef HPIDTRACKFILLERPAR_H
#define HPIDTRACKFILLERPAR_H

////////////////////////////////////////////////////////////////////////////////
//
// HPidTrackFillerPar
//
////////////////////////////////////////////////////////////////////////////////

#include "hpidparcont.h"

// -----------------------------------------------------------------------------

class HParamList;

// -----------------------------------------------------------------------------

#define HPIDTRACKFILLERPAR_NAME    "PidTrackFillerPar"
#define HPIDTRACKFILLERPAR_TITLE   "Parameters for HPidTrackFiller"
#define HPIDTRACKFILLERPAR_CONTEXT "PidTrackFillerPar"

// -----------------------------------------------------------------------------

class HPidTrackFillerPar : public HPidParCont
{
private:

    Float_t     fWindowKickRichTheta;
    Float_t     fWindowKickRichPhi;
    Float_t     fWindowMdcRichTheta;
    Float_t     fWindowMdcRichPhi;

    // -------------------------------------------------------------------------

public:

                    HPidTrackFillerPar(
                            const char *name    = HPIDTRACKFILLERPAR_NAME,
                            const char *title   = HPIDTRACKFILLERPAR_TITLE,
                            const char *context = HPIDTRACKFILLERPAR_CONTEXT);
    virtual        ~HPidTrackFillerPar(void);

    virtual void    clear(void);

    virtual void    putParams(HParamList *pl);
    virtual Bool_t  getParams(HParamList *pl);

    // -------------------------------------------------------------------------

    Float_t     getWindowKickRichTheta(void) const
                                            { return fWindowKickRichTheta;    }
    void        setWindowKickRichTheta(Float_t f) { fWindowKickRichTheta = f; }
    Float_t     getWindowKickRichPhi(void) const
                                            { return fWindowKickRichPhi;      }
    void        setWindowKickRichPhi(Float_t f) { fWindowKickRichPhi = f;     }

    Float_t     getWindowMdcRichTheta(void) const
                                            { return fWindowMdcRichTheta;     }
    void        setWindowMdcRichTheta(Float_t f) { fWindowMdcRichTheta = f;   }
    Float_t     getWindowMdcRichPhi(void) const
                                            { return fWindowMdcRichPhi;       }
    void        setWindowMdcRichPhi(Float_t f) { fWindowMdcRichPhi = f;       }

    // -------------------------------------------------------------------------

    virtual void Print(Option_t *p = "")    { printParams();    }
    virtual void print(void)                { printParams();    }

    // -------------------------------------------------------------------------

    ClassDef(HPidTrackFillerPar, 1) // Container with params for HPidTrackFiller
};

// -----------------------------------------------------------------------------

#endif  /* !HPIDTRACKFILLERPAR_H */
