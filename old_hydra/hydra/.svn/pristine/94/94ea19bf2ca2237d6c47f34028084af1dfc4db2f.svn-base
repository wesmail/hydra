// @(#)$Id: hpidmomimpbeta.h,v 1.1 2003-06-05 14:41:03 jaskula Exp $
//*-- Author : Marcin Jaskula 27/06/2003

#ifndef HPidMomImpBeta_H
#define HPidMomImpBeta_H

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidMomImpBeta                                                             //
//                                                                            //
// Improve the paritlce momentum from its ideal mass and the exp. beta        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "hreconstructor.h"
#include "TArrayI.h"

// -----------------------------------------------------------------------------

class HIterator;

// -----------------------------------------------------------------------------

class HPidMomImpBeta : public HReconstructor
{
public:
                        HPidMomImpBeta(void);
                        HPidMomImpBeta(Text_t name[], Text_t title[]);

                       ~HPidMomImpBeta(void);

    // -------------------------------------------------------------------------

    virtual Int_t       execute(void);
    virtual Bool_t      reinit(void)                    { return kTRUE;        }
    virtual Bool_t      init(void);
    virtual Bool_t      finalize(void)                  { return kTRUE;        }

    // -------------------------------------------------------------------------

    void                addParticle(Int_t iPid);
    void                removeParticle(Int_t iPid);

    // -------------------------------------------------------------------------

    virtual void        Print(Option_t* option) const   { print();             }
    virtual void        print(void) const;

    // -------------------------------------------------------------------------

protected:

    HIterator          *pitInput;       // iterator on the pInputCat category

    // -------------------------------------------------------------------------

    TArrayI             aParticles;     // list of particle ids

    // -------------------------------------------------------------------------

    void                setDefault(void);

    // -------------------------------------------------------------------------

    ClassDef(HPidMomImpBeta, 0)         // Recalc. HPidParticle mom from beta
};

// -----------------------------------------------------------------------------

#endif //HPidMomImpBeta_H

