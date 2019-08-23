//*-- Authors : Marcin Jaskula, Jerzy Pietraszko 11/10/2002
//            : code taken from HShowerHitFilnder and some old implemetation
//              of this class

#include "hshowerhitfinderIPU.h"
#include "hshowerhitIPU.h"

#include "TArrayI.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hshowerdetector.h"

#include "hratreeext.h"
#include "hcategory.h"
#include "hmatrixcategory.h"
#include "hlinearcatiter.h"
#include "hlocation.h"
#include "hshowercalIPU.h"
#include "hshowerhitfpar.h"
#include "hshowergeometry.h"
#include "hshowerpad.h"
#include "hiterator.h"
#include "hdebug.h"
#include "hades.h"
#include "hgeomvector.h"
#include "hgeomvector2.h"
#include "showerdef.h"
#include "triggerinfodef.h"
#include "htriggerdetector.h"

//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////////////
//
// HShowerHitFinderIPU
//
// Reconstructor for searching local maxima in the same way as
// the PreSHOWER IPU does it.
//
// Parameters:
// thr1,  thr2 - used in the same way like in the hardware
// m_iAlgFlag - defines the mask for the conditions:
//      0x01 - look for charges above thr1
//      0x02 - check if the pad is a local maximum from 5 pads
//      0x04 - check if (sum1>sum0+thr2 || sum2>sum0+thr2) (sumX from 9 pads)
//      0x08 - check if (sum1>thr2 || sum2>thr2)
//      WARNING: In hardware 0x08 works only if 0x04 is set.
//      In this algorithm they work independently.
//
// If m_iAlgFlag is 0 then every hit which fulfills ANY condition is stored.
// Otherwise the hits for which ALL conditions defined in the m_iAlgFlag
// are fulfilled are written out.
//
///////////////////////////////////////////////////////////////////////////////

ClassImp(HShowerHitFinderIPU)

// -----------------------------------------------------------------------------

#define CHECK_WEAK(ROW, COL)                                    \
    m_Loc[2] = iRow + ROW; m_Loc[3] = iCol + COL;               \
    if((m_Loc[2] >= kiMinRange) && (m_Loc[2] <= kiMaxRange)     \
        && (m_Loc[3] >= kiMinRange) && (m_Loc[3] <= kiMaxRange))\
    {                                                           \
        pCell = (HShowerCalIPU *) m_pCellArr->getObject(m_Loc); \
        if((pCell != NULL)                                      \
                && (pRef->getCharge() <= pCell->getCharge()))   \
            return 0;                                           \
    }

#define CHECK_STRONG(ROW, COL)                                  \
    m_Loc[2] = iRow + ROW; m_Loc[3] = iCol + COL;               \
    if((m_Loc[2] >= kiMinRange) && (m_Loc[2] <= kiMaxRange)     \
        && (m_Loc[3] >= kiMinRange) && (m_Loc[3] <= kiMaxRange))\
    {                                                           \
        pCell = (HShowerCalIPU * )m_pCellArr->getObject(m_Loc); \
        if((pCell != NULL)                                      \
                && (pRef->getCharge() < pCell->getCharge()))    \
            return 0;                                           \
    }

// -----------------------------------------------------------------------------

HShowerHitFinderIPU::HShowerHitFinderIPU(const Text_t *name,const Text_t *title,
                Int_t iAlgFlag, Int_t iThr1, Int_t iThr2)
                : HReconstructor(name, title)
{
    setDefault();

    m_iAlgFlag = iAlgFlag;
    m_iThr1 = iThr1;
    m_iThr2 = iThr2;
}

// -----------------------------------------------------------------------------

HShowerHitFinderIPU::HShowerHitFinderIPU(Int_t iAlgFlag,
                Int_t iThr1, Int_t iThr2)
                : HReconstructor("HShowerHitFinderIPU", "HShowerHitFinderIPU")
{
    setDefault();

    m_iAlgFlag = iAlgFlag;
    m_iThr1 = iThr1;
    m_iThr2 = iThr2;
}

// -----------------------------------------------------------------------------

HShowerHitFinderIPU::HShowerHitFinderIPU(void)
{
    setDefault();
}

// -----------------------------------------------------------------------------

void HShowerHitFinderIPU::setDefault(void)
{
    m_Loc.set(4, 0, 0, 0, 0);

    m_iAlgFlag = 0;
    m_iThr1    = 0;
    m_iThr2    = 0;

    m_pCellArr = NULL;
    m_pCalCat  = NULL;
    m_pHitCat  = NULL;
    fIter      = NULL;
}

// -----------------------------------------------------------------------------

HShowerHitFinderIPU::~HShowerHitFinderIPU(void)
{
    if(m_pCellArr != NULL)
    {
        delete m_pCellArr;
        m_pCellArr = NULL;
    }

    if(fIter != NULL)
    {
        delete fIter;
        fIter = NULL;
    }
}

// -----------------------------------------------------------------------------

Bool_t HShowerHitFinderIPU::init(void)
{
HShowerDetector *pShowerDet = (HShowerDetector*)gHades->getSetup()
                            ->getDetector("Shower");
  HTriggerDetector * pTriggerDet = (HTriggerDetector*)gHades->getSetup()->getDetector("Trigger");

    m_pCalCat = gHades->getCurrentEvent()->getCategory(catShowerCalIPU);
    if( ! m_pCalCat)
    {
        m_pCalCat = pShowerDet->buildCategory(catShowerCalIPU);

        if( ! m_pCalCat)
        {
            Error("init", "Cannot build category: catShowerCalIPU");
            return kFALSE;
        }
        else
        {
            gHades->getCurrentEvent()
                    ->addCategory(catShowerCalIPU, m_pCalCat, "Shower");
        }
    }

    m_pHitCat = gHades->getCurrentEvent()->getCategory(catShowerHitIPU);
    if( ! m_pHitCat)
    {
        m_pHitCat = pTriggerDet->buildCategory(catShowerHitIPU);

        if( ! m_pHitCat)
        {
            Error("init", "Cannot build category: catShowerHitIPU");
            return kFALSE;
        }
        else
        {
            gHades->getCurrentEvent()
                    ->addCategory(catShowerHitIPU, m_pHitCat, "Trigger");
        }
    }

TArrayI arr(4);

    arr[0]  = pShowerDet->getMaxSectors();
    arr[1]  = pShowerDet->getMaxModules();
    arr[2]  = pShowerDet->getRows();
    arr[3]  = pShowerDet->getColumns();

    m_pCellArr = new HRaTreeExt(m_pCalCat, &arr);
    fIter      = (HIterator *) m_pCalCat->MakeIterator("native");

    return initParameters();
}

// -----------------------------------------------------------------------------

Bool_t HShowerHitFinderIPU::initParameters(void)
{
// Now it is a dummy function

    return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HShowerHitFinderIPU::finalize(void)
{
    return kTRUE;
}

// -----------------------------------------------------------------------------

Int_t HShowerHitFinderIPU::execute(void)
{
HShowerCalIPU *pCal;
Int_t          n    = 0;
Int_t          hits = 0;

    m_pCellArr->update();

    fIter->Reset();
    while((pCal = (HShowerCalIPU *)fIter->Next()))
    {
        if(pCal->getModule() > 0)
            continue;

        if(lookForHit(pCal))
            hits++;

        n++;
    }

    return 0;
}

// -----------------------------------------------------------------------------

Bool_t HShowerHitFinderIPU::lookForHit(HShowerCalIPU *pCal)
{
// checks the conditions and makes new entry in HShowerHitIPU cat
#if DEBUG_LEVEL>2
    gDebuger->enterFunc("HShowerHitFinderIPU::lookForHit");
    gDebuger->message("Cal cat points to %p", pCal);
#endif

    if(pCal == NULL)
    {
        Error("lookForHit", "NULL input *pCal");
        return kFALSE;
    }

    if(pCal->getModule() > 0)
    {
        Warning("lookForHit", "Works only for module = 0");
        return kFALSE;
    }

Byte_t   bCharge = pCal->getCharge();
UShort_t nSum[3];
Bool_t   bConds[4];
Int_t    iRes = 0, i;

enum eConds {
    eCnd_Thr = 0,
    eCnd_LM,
    eCnd_Cluster,
    eCnd_Cluster0
};

    memset(nSum, 0, sizeof(nSum));

    fillSums(pCal, nSum);

    bConds[eCnd_Thr]      = ((Int_t) bCharge > m_iThr1);
    bConds[eCnd_LM]       = isLocalMax(pCal);
    bConds[eCnd_Cluster]  = ((nSum[1] > nSum[0] + m_iThr2)
                            || (nSum[2] > nSum[0] + m_iThr2));
    bConds[eCnd_Cluster0] = ((nSum[1] > m_iThr2) || (nSum[2] > m_iThr2));

    // calculate the flag
    for(i = 0; i <= eCnd_Cluster0; i++)
    {
        if(bConds[i])
            iRes |= 1 << i;
    }

    // mask the algorithm flag
    if(iRes == 0)
        return kFALSE;

    if((m_iAlgFlag != 0) && ((iRes & m_iAlgFlag) != m_iAlgFlag))
        return kFALSE;

HShowerHitIPU *hit = NULL;

    m_Loc[0] = pCal->getSector();
    m_Loc[1] = pCal->getModule();
    m_Loc[2] = pCal->getRow();
    m_Loc[3] = pCal->getCol();

    if((hit= (HShowerHitIPU *) m_pHitCat->getNewSlot(m_Loc)) == NULL)
    {
        Error("lookForHit", "m_pHitCat->getNewSlot(m_Loc) == NULL");
        return kFALSE;
    }

    hit = new (hit) HShowerHitIPU;

    hit->setSector(pCal->getSector());
    hit->setRow(pCal->getRow());
    hit->setCol(pCal->getCol());
    hit->setCharge(bCharge);
    hit->setSum0(nSum[0]);
    hit->setSum1(nSum[1]);
    hit->setSum2(nSum[2]);
    hit->setFlag((Byte_t) iRes);

#if DEBUG_LEVEL>2
    gDebuger->leaveFunc("HShowerHitFinderIPU::lookForHit");
#endif

    return kTRUE;
}

// -----------------------------------------------------------------------------

void HShowerHitFinderIPU::fillSums(HShowerCalIPU *pCal, UShort_t nSum[3])
{
// Fills sums

Int_t          iR, iC, iM, iRMin, iRMax, iCMin, iCMax;
HShowerCalIPU *pCell;
const Int_t kiMinRange = 0;
const Int_t kiMaxRange = 31;

    iR = pCal->getRow();
    iRMin = (iR <= kiMinRange) ? kiMinRange : (iR - 1);
    iRMax = (iR >= kiMaxRange) ? kiMaxRange : (iR + 1);

    iC = pCal->getCol();
    iCMin = (iC <= kiMinRange) ? kiMinRange : (iC - 1);
    iCMax = (iC >= kiMaxRange) ? kiMaxRange : (iC + 1);

    m_Loc[0] = pCal->getSector();

    // calculate all sums
    for(iM = 0; iM < 3; iM++)
    {
        m_Loc[1] = iM;

        for(iR = iRMin; iR <= iRMax; iR++)
        {
            m_Loc[2] = iR;

            for(iC = iCMin; iC <= iCMax; iC++)
            {
                m_Loc[3] = iC;

                if((pCell = (HShowerCalIPU *) m_pCellArr->getObject(m_Loc))
                        == NULL)
                {
                    continue;
                }

                nSum[(Int_t)pCell->getModule()] += (UShort_t) pCell->getCharge();
            }
        }
    }
}

// -----------------------------------------------------------------------------

Bool_t HShowerHitFinderIPU::isLocalMax(HShowerCalIPU *pRef)
{
// Check if the pRef is a local maximum from 4 pads (up/down/left/right).
// The checking is the same like the one done in the IPU

HShowerCalIPU *pCell;
const Int_t kiMinRange = 0;
const Int_t kiMaxRange = 31;

    if(pRef == NULL)
    {
        Error("isLocalMax", "pRef == NULL");
        return kFALSE;
    }

    if(pRef->getModule() != 0)
    {
        Error("isLocalMax", "Module != 0");
        return kFALSE;
    }

Int_t iRow = pRef->getRow();
Int_t iCol = pRef->getCol();

    m_Loc[0] = pRef->getSector();
    m_Loc[1] = 0;

    CHECK_WEAK(-1, 0);
    CHECK_STRONG(1, 0);
    CHECK_STRONG(0, -1);
    CHECK_WEAK(0, 1);

    return kTRUE;
}
