// @(#)$Id: hpidmomimp.h,v 1.2 2003-10-23 18:54:20 jaskula Exp $
//*-- Author : Marcin Jaskula 02/06/2003

#ifndef HPidMomImp_H
#define HPidMomImp_H

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidMomImp                                                                 //
//                                                                            //
// Improve the particle momentum by using a user defined formula              //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "hreconstructor.h"
#include <TObjArray.h>

// -----------------------------------------------------------------------------

class HIterator;
class TF1;

// -----------------------------------------------------------------------------

class HPidMomImp : public HReconstructor
{
public:
                        HPidMomImp(void);
                        HPidMomImp(Text_t name[], Text_t title[]);

                       ~HPidMomImp(void);

    // -------------------------------------------------------------------------

    virtual Int_t       execute(void);
    virtual Bool_t      reinit(void)                    { return kTRUE;        }
    virtual Bool_t      init(void);
    virtual Bool_t      finalize(void)                  { return kTRUE;        }

    // -------------------------------------------------------------------------

    void                setFormula(Int_t iPid, const TF1 *pFun);

    // -------------------------------------------------------------------------

    virtual void        Print(Option_t* option = "") const     { print();     }
    virtual void        print(void) const;

    // -------------------------------------------------------------------------

protected:

    TObjArray           aFun;       // array of functions
    HIterator          *pitInput;   // iterator over the category

    // -------------------------------------------------------------------------

    void                setDefault(void);

    // -------------------------------------------------------------------------

    ClassDef(HPidMomImp, 0)         // Recalc. HPidParticle by any function
};

// -----------------------------------------------------------------------------

#endif //HPidMomImp_H

