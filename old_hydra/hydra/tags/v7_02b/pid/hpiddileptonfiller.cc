// @(#)$Id: hpiddileptonfiller.cc,v 1.2 2003-04-03 13:33:48 jaskula Exp $
//*-- Author : Marcin Jaskula 01/12/2002

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidDileptonFiller                                                         //
//                                                                            //
// Filler of HPidDilepton category from the data in HPidParticle              //
// Every combination of two electrons (e+/e-, e+/e+, e-/e-) is filled.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "hcategory.h"
#include "hpiddileptonfiller.h"

#include "hpiddilepton.h"
#include "hpidparticle.h"
#include "piddef.h"

#include "hades.h"
#include "hevent.h"

// -----------------------------------------------------------------------------

ClassImp(HPidDileptonFiller)

// -----------------------------------------------------------------------------

#define SAFE_DELETE(A) { if(A) { delete (A); A = NULL; }}

// -----------------------------------------------------------------------------

const char sDileptonMarks[] = "-0+";

// -----------------------------------------------------------------------------

HPidDileptonFiller::HPidDileptonFiller(Option_t par[])
                    : HReconstructor("PidDileptonFiller",
                                     "Filler of HPidDilepton category")
{
// Default constructor.
// par parameter is used in setParameters() method

    setDefault();
    setParameters(par);
}

// -----------------------------------------------------------------------------

HPidDileptonFiller::HPidDileptonFiller(Text_t name[], Text_t title[],
                                        Option_t par[])
                    : HReconstructor(name, title)
{
// Constructor with names
// par parameter is used in setParameters() method

    setDefault();
    setParameters(par);
}

// -----------------------------------------------------------------------------

void HPidDileptonFiller::setDefault(void)
{
// Called in constructors to set default valuest to the members

    pInputCat = NULL;
    pOutCat   = NULL;
    iDebug    = 0;
    memset(aiDileptons, 0, sizeof(aiDileptons));
}

// -----------------------------------------------------------------------------

Bool_t HPidDileptonFiller::init(void)
{
// Build all categories

    memset(aiDileptons, 0, sizeof(aiDileptons));

    // output category
    if(buildOutputCategory() == kFALSE)
        return kFALSE;

    // HPidParticle category and iterator
    if((pInputCat = gHades->getCurrentEvent()->getCategory(catPidPart)) == NULL)
    {
        Error("init", "No category catPidPart");
        return kFALSE;
    }

    return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HPidDileptonFiller::buildOutputCategory(void)
{
// Build output category

    if((pOutCat = HPidDilepton::buildPidDileptonCategory()) == NULL)
    {
        Error("buildOutputCategory", "Cannot build HPidDilepton category");
        return kFALSE;
    }

    return kTRUE;
}

// -----------------------------------------------------------------------------

void HPidDileptonFiller::setParameters(Option_t par[])
{
// Set parameters by names. Options (may be separated by comma or blank chars):
// DEBUGX  - debug info, if X set then it define the debug info level

TString s = par;
char   *p;

    s.ToUpper();

    iDebug = 0;
    if((p = strstr(s.Data(), "DEBUG")) != NULL)
    {
        if(sscanf(p + strlen("DEBUG"), "%d", &iDebug) <= 0)
            iDebug = 1;
    }
}

// -----------------------------------------------------------------------------

Bool_t HPidDileptonFiller::finalize(void)
{
    if(iDebug)
        print();

    return kTRUE;
}

// -----------------------------------------------------------------------------

Int_t HPidDileptonFiller::execute(void)
{
// Make the dileptons

const HPidParticle *pPart1;
const HPidParticle *pPart2;
Int_t               i1, i2, iMax;
Int_t               iCount = 0;
HPidDilepton       *pDilepton;

    // clear the data from the old file
    pOutCat->Clear();

    iMax = pInputCat->getEntries();
    for(i1 = 0; i1 < iMax - 1; i1++)
    {
        if((pPart1 = (const HPidParticle *) pInputCat->getObject(i1)) == NULL)
            continue;

        if( ! isElectron(pPart1->getPid()))
            continue;

        for(i2 = i1 + 1; i2 < iMax; i2++)
        {
            if((pPart2 = (const HPidParticle *) pInputCat->getObject(i2))
                        == NULL)
            {
                continue;
            }

            if( ! isElectron(pPart2->getPid()))
                continue;

            pDilepton = createDilepton(pPart1, pPart2);

            if(iDebug)
            {
                if(iCount == 0)
                    printf("Dileptons: ");

                if(pDilepton->isOk())
                {
                    printf("%c ",
                            sDileptonMarks[1 + pDilepton->getCharge() / 2]);
                }
                else
                    printf("* ");
            }

            if(pDilepton->isOk())
                aiDileptons[1 + (pDilepton->getCharge() / 2)]++;

            iCount++;
        }
    }

    if((iDebug) && (iCount > 0))
        printf("(%d)\n", iCount);

    return 0;
}

// -----------------------------------------------------------------------------

HPidDilepton* HPidDileptonFiller::createDilepton(
                        const HPidParticle *p1, const HPidParticle *p2)
{
// Get next slot from the output category and fill it with data

HPidDilepton     *pOut = NULL;
static HLocation  locDummy;

    if(pOutCat == NULL)
    {
        Error("getNextSlot", "Output category not set: use init/reinit");
        return NULL;
    }

    if((pOut = (HPidDilepton *) pOutCat->getNewSlot(locDummy)) == NULL)
    {
        Error("getNextSlot", "No new slot");
        return NULL;
    }

    pOut = new(pOut) HPidDilepton(p1, p2);

    return pOut;
}

// -----------------------------------------------------------------------------

void HPidDileptonFiller::print(void) const
{
// Print the info

    printf("HPidDileptonFiller: found dileptons:\ne-/e-: %5d\n"
                "e+/e-: %5d\ne+/e+: %5d\n",
                aiDileptons[0], aiDileptons[1], aiDileptons[2]);
}
