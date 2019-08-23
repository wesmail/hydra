// @(#)$Id: hpidsimtrigger.h,v 1.1 2003-03-04 17:22:35 jaskula Exp $
//*-- Author : Marcin Jaskula 13/01/2003

#ifndef HPidSimTrigger_H
#define HPidSimTrigger_H

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidSimTrigger                                                             //
//                                                                            //
// Checks whether the TOF + TOFino mult. is high enought in sim data          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "hreconstructor.h"

// -----------------------------------------------------------------------------

class HIterator;

// -----------------------------------------------------------------------------

class HPidSimTrigger : public HReconstructor
{
public:
                        HPidSimTrigger(Int_t i = 4);
                        HPidSimTrigger(Text_t name[], Text_t title[],
                                        Int_t i = 4);

                       ~HPidSimTrigger(void);

    // -------------------------------------------------------------------------

    virtual Int_t       execute(void);
    virtual Bool_t      init(void);
    virtual Bool_t      reinit(void);
    virtual Bool_t      finalize(void);

    // -------------------------------------------------------------------------

    Int_t               getMinMult(void) const  { return iMinMult;   }
    void                setMinMult(Int_t i)     { iMinMult = i ;     }

    UInt_t              getEvents(void) const   { return uiEvents;   }
    UInt_t              getEventsOk(void) const { return uiEventsOk; }

    Double_t            getRatio(void) const;
    Double_t            getRatioError(void) const;

    // -------------------------------------------------------------------------

    virtual void        Print(Option_t* option) const   { print();             }
    virtual void        print(void) const;

    // -------------------------------------------------------------------------

protected:

    Int_t               iMinMult;
    HCategory          *pTofCat;
    HCategory          *pTofinoCat;

    HIterator          *pItTofino;

    UInt_t              uiEvents;
    UInt_t              uiEventsOk;

    // -------------------------------------------------------------------------

    void                setDefault(Int_t k);

    // -------------------------------------------------------------------------

    ClassDef(HPidSimTrigger, 0)  // TOF+TOFino multiplicity checker
};

// -----------------------------------------------------------------------------

#undef DEFAULT_OPTIONS

// -----------------------------------------------------------------------------

#endif //HPidSimTrigger_H
