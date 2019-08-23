// @(#)$Id: hpidmomimp.cc,v 1.1 2003-06-05 14:41:03 jaskula Exp $
//*-- Author : Marcin Jaskula 02/06/2003

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidMomImp                                                                 //
//                                                                            //
// Improve the particle momentum by using a user defined formula              //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#include "hades.h"
#include "hevent.h"

#include "hcategory.h"
#include "hiterator.h"

#include "hpidmomimp.h"
#include "piddef.h"

#include "hpidparticle.h"

#include <TF1.h>

// -----------------------------------------------------------------------------

ClassImp(HPidMomImp)

// -----------------------------------------------------------------------------

#define SAFE_DELETE(A) { if(A) { delete (A); A = NULL; }}

// -----------------------------------------------------------------------------

HPidMomImp::HPidMomImp(void)
                    : HReconstructor("PidMomImp",
                                     "Recalc. mom using user function")
{
// Default constructor.

    setDefault();
}

// -----------------------------------------------------------------------------

HPidMomImp::HPidMomImp(Text_t name[], Text_t title[])
                    : HReconstructor(name, title)
{
// Constructor with names

    setDefault();
}

// -----------------------------------------------------------------------------

HPidMomImp::~HPidMomImp(void)
{
// Destructor

    SAFE_DELETE(pitInput);
    aFun.Clear();
}

// -----------------------------------------------------------------------------

void HPidMomImp::setDefault(void)
{
// Called in constructors to set default valuest to the members

    pitInput  = NULL;
    aFun.SetOwner(kTRUE);
}

// -----------------------------------------------------------------------------

Bool_t HPidMomImp::init(void)
{
// Build all categories

HCategory *pCat;

    // HPidParticle category and iterator
    if((pCat = gHades->getCurrentEvent()->getCategory(catPidPart)) == NULL)
    {
        Error("init", "No category catPidPart");
        return kFALSE;
    }

    if((pitInput = (HIterator *)pCat->MakeIterator()) == NULL)
    {
        Error("init", "Cannot make an iterator for HPidParticle category");

        return kFALSE;
    }

    return kTRUE;
}

// -----------------------------------------------------------------------------

void HPidMomImp::setFormula(Int_t iPid, const TF1 *pFun)
{
// A clone of the pFun function is added to the array of objects

    if(iPid < 0)
    {
        Error("setFormula", "Negative iPid: %d\n", iPid);
        return;
    }

    if(aFun.GetLast() > iPid)
    {
    TF1 *pOld;

        if((pOld = (TF1 *)aFun.RemoveAt(iPid)) != NULL)
            delete pOld;
    }

    aFun.AddAtAndExpand((pFun == NULL) ? NULL : pFun->Clone(), iPid);
}

// -----------------------------------------------------------------------------

Int_t HPidMomImp::execute(void)
{
// Recalculate the particle momentum

HPidParticle   *pPart;
Int_t           iPid;
Float_t         fMomOrg, fMomId;
TF1            *pFun;

    pitInput->Reset();
    while((pPart = (HPidParticle *)pitInput->Next()) != NULL)
    {
        iPid = pPart->getPid();

        if((iPid < 0) || (aFun.GetLast() < iPid))
            continue;

        if((pFun = (TF1 *) aFun.At(iPid)) == NULL)
            continue;

        fMomOrg = pPart->getRescaledVector().P();

        if((fMomOrg < pFun->GetXmin()) || (fMomOrg > pFun->GetXmax()))
            continue;

        fMomId = pFun->Eval(fMomOrg);

        pPart->setMomRescal(pPart->getMomRescal() * fMomId);
    }

    return 0;
}

// -----------------------------------------------------------------------------

void HPidMomImp::print(void) const
{
// Print the list of all algorithms

    printf("HPidMomImp:\n");

TF1 *pFun;

    for(Int_t i = 0; i < aFun.GetSize(); i++)
    {
        if((pFun = (TF1 *)aFun.At(i)) == NULL)
            continue;

        printf("%3d: \"%s\" %.2f %.2f\n", i, pFun->GetTitle(),
                pFun->GetXmin(), pFun->GetXmax());
    }
}
