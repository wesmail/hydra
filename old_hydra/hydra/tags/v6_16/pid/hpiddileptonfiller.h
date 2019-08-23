// @(#)$Id: hpiddileptonfiller.h,v 1.2 2003-10-23 18:56:32 jaskula Exp $
//*-- Author : Marcin Jaskula 01/12/2002

#ifndef HPIDDILEPTONFILLER_H
#define HPIDDILEPTONFILLER_H

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidDileptonFiller                                                         //
//                                                                            //
// Filler of HPidDilepton category from the data in HPidParticle              //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "hreconstructor.h"

// -----------------------------------------------------------------------------

class HCategory;
class HPidDilepton;
class HPidParticle;

// -----------------------------------------------------------------------------

class HPidDileptonFiller : public HReconstructor
{
public:
                        HPidDileptonFiller(Option_t par[] = "");

                        HPidDileptonFiller(Text_t name[], Text_t title[],
                                            Option_t par[] = "");

                       ~HPidDileptonFiller(void) {}

    // -------------------------------------------------------------------------

    virtual void        setParameters(Option_t par[]);

    // -------------------------------------------------------------------------

    virtual Int_t       execute(void);
    virtual Bool_t      init(void);
    virtual Bool_t      finalize(void);

    // -------------------------------------------------------------------------

    virtual void        Print(Option_t* option = "") const   { print();             }
    virtual void        print(void) const;

    Int_t               getDileptons(Int_t i)       { return aiDileptons[i];   }

    // -------------------------------------------------------------------------

    virtual Bool_t          buildOutputCategory(void);
    virtual HPidDilepton*   createDilepton(const HPidParticle *p1,
                                        const HPidParticle *p2);

    // -------------------------------------------------------------------------

    Int_t               getDebug(void) const            { return iDebug;       }
    void                setDebug(Int_t i)               { iDebug = i;          }

    // -------------------------------------------------------------------------

protected:

    HCategory          *pInputCat;      // category of HPidParticle
    HCategory          *pOutCat;        // category of HPidDilepton

    // -------------------------------------------------------------------------

    Int_t               iDebug;         // debug level
    Int_t               aiDileptons[3]; // counter of dileptons

    // -------------------------------------------------------------------------

    void                setDefault(void);
    Bool_t              isElectron(Int_t iPid)
                                        { return (iPid == 2) || (iPid == 3);   }

    // -------------------------------------------------------------------------

    ClassDef(HPidDileptonFiller, 0)     // Filler of HPidDilepton
};

// -----------------------------------------------------------------------------

#endif //HPIDDILEPTONFILLER_H
