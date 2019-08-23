// @(#)$Id: hpidmomimpbeta.cc,v 1.2 2003-10-23 18:55:28 jaskula Exp $
//*-- Author : Marcin Jaskula 27/06/2003

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidMomImpBeta                                                             //
//                                                                            //
// Improve the paritlce momentum from its ideal mass and the exp. beta        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "hades.h"
#include "hevent.h"

#include "hcategory.h"
#include "hiterator.h"

#include "hpidmomimpbeta.h"
#include "piddef.h"

#include "hpidphysicsconstants.h"
#include "hpidparticle.h"

// -----------------------------------------------------------------------------

ClassImp(HPidMomImpBeta)

// -----------------------------------------------------------------------------

#define SAFE_DELETE(A) { if(A) { delete (A); A = NULL; }}

// -----------------------------------------------------------------------------

HPidMomImpBeta::HPidMomImpBeta(void)
                    : HReconstructor("PidMomImpBeta",
                                     "Recalc. mom from beta and mass")
{
// Default constructor.

    setDefault();
}

// -----------------------------------------------------------------------------

HPidMomImpBeta::HPidMomImpBeta(Text_t name[], Text_t title[])
                    : HReconstructor(name, title)
{
// Constructor with names

    setDefault();
}

// -----------------------------------------------------------------------------

HPidMomImpBeta::~HPidMomImpBeta(void)
{
// Destructor

    SAFE_DELETE(pitInput);
}

// -----------------------------------------------------------------------------

void HPidMomImpBeta::setDefault(void)
{
// Called in constructors to set default valuest to the members

    pitInput  = NULL;
    aParticles.Set(0);
}

// -----------------------------------------------------------------------------

Bool_t HPidMomImpBeta::init(void)
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

void HPidMomImpBeta::addParticle(Int_t iPid)
{
// Add another particle id to the list

    aParticles.Set(aParticles.GetSize() + 1);
    aParticles[aParticles.GetSize() - 1] = iPid;
}

// -----------------------------------------------------------------------------

void HPidMomImpBeta::removeParticle(Int_t iPid)
{
// Remove particle id from the list

Int_t iIn, iOut;

    for(iIn = 0, iOut = 0; iIn < aParticles.GetSize(); iIn++)
    {
        if(aParticles[iIn] != iPid)
        {
            if(iOut != iIn)
                aParticles[iOut] = aParticles[iIn];

            iOut++;
        }
    }

    aParticles.Set(iOut);
}

// -----------------------------------------------------------------------------

Int_t HPidMomImpBeta::execute(void)
{
// Recalculate the particle momentum

HPidParticle   *pPart;
Int_t           i;
Float_t         fMomOrg, fMomId, fBeta;

    pitInput->Reset();
    while((pPart = (HPidParticle *)pitInput->Next()) != NULL)
    {
        if(((fBeta = pPart->getBetaExp()) >= 1.0f) || (fBeta <= 0.0f))
            continue;

        for(i = 0; i < aParticles.GetSize(); i++)
        {
            if(aParticles[i] == pPart->getPid())
            {
                fMomOrg = pPart->P();
                fMomId  = pPart->getMassIdeal()
                                * fBeta / TMath::Sqrt(1 - fBeta * fBeta);

                pPart->setMomRescal(fMomId / fMomOrg);

                break;
            }
        }
    }

    return 0;
}

// -----------------------------------------------------------------------------

void HPidMomImpBeta::print(void) const
{
// Print the list of all algorithms

    printf("HPidMomImpBeta\n");

    printf("Particles list (%d) :", aParticles.GetSize());
    for(Int_t i = 0; i < aParticles.GetSize(); i++)
        printf(" %d", aParticles[i]);
}
