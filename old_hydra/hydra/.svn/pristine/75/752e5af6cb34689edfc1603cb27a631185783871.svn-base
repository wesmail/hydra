// @(#)$Id: hshowercalibraterIPU.cc,v 1.3 2002-10-28 14:42:38 jaskula Exp $
//*-- Authors : Marcin Jaskula, Jerzy Pietraszko 11/10/2002
//            : code taken from HShowerCalibrator

#include "hshowercalibraterIPU.h"

#include "hruntimedb.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hshowerdetector.h"
#include "hcategory.h"
#include "hmatrixcatiter.h"
#include "hlocation.h"
#include "hshowerparset.h"
#include "hshowergeometry.h"
#include "hshowerraw.h"
#include "hshowercalIPU.h"
#include "hshowercalpar.h"
#include "hdebug.h"
#include "hades.h"
#include "hiterator.h"
#include "showerdef.h"

////////////////////////////////////////////////////////////////////////////////
//
// HShowerCalibraterIPU
//
// HShowerCalibraterIPU class calculates new value of measured charge from RAW
// level as IPU it does using calibrations parameters from the parameter i
// containers HShowerCalPar. Calculated charge is saved to CAL level.
//
////////////////////////////////////////////////////////////////////////////////

ClassImp(HShowerCalibraterIPU)

// -----------------------------------------------------------------------------

HShowerCalibraterIPU::HShowerCalibraterIPU(Text_t *name,Text_t *title)
                                    : HReconstructor(name, title)
{
    m_pCalPar = NULL;
    fIter = NULL;
    m_loc.set(4,0,0,0,0);
    m_zeroLoc.set(0);
}

// -----------------------------------------------------------------------------

HShowerCalibraterIPU::HShowerCalibraterIPU()
{
    m_pCalPar = NULL;
    m_loc.set(4,0,0,0,0);
    m_zeroLoc.set(0);
    fIter = NULL;
}

// -----------------------------------------------------------------------------

void HShowerCalibraterIPU::initCalPar()
{
//initializations of calibration parameters

    HShowerDetector *pShowerDet = (HShowerDetector*)gHades->getSetup()
                                                  ->getDetector("Shower");
    HRuntimeDb* rtdb=gHades->getRuntimeDb();

    m_pCalPar = rtdb->getContainer("ShowerCalPar");

    if(m_pCalPar)
    {
        ((HShowerCalPar*)m_pCalPar)->
             setSetup(pShowerDet->getShowerSectors(),
                      pShowerDet->getShowerModules(),
                      pShowerDet->getRows(),
                      pShowerDet->getColumns());

        rtdb->addContainer(m_pCalPar);
    }
}

// -----------------------------------------------------------------------------

HShowerCalibraterIPU::~HShowerCalibraterIPU(void)
{
// destructor

//   if (m_pCalPar) delete m_pCalPar;
     if (fIter) delete fIter;
}

// -----------------------------------------------------------------------------

Bool_t HShowerCalibraterIPU::init(void)
{
// Creates the ShowerRaw and ShowerCalIPU categories
// and adds them to the current event.
// Creates an iterator which loops over all fired cells

    HShowerDetector *pShowerDet = (HShowerDetector*)gHades->getSetup()
                                                  ->getDetector("Shower");

    m_pRawCat=gHades->getCurrentEvent()->getCategory(catShowerRaw);
    if(!m_pRawCat)
    {
        m_pRawCat = pShowerDet->buildCategory(catShowerRaw);

        if( ! m_pRawCat)
        {
            Error("init", "Cannot build category: catShowerRaw");
            return kFALSE;
        }
        else
        {
            gHades->getCurrentEvent()
                         ->addCategory(catShowerRaw, m_pRawCat, "Shower");
        }
    }

    m_pCalCat=gHades->getCurrentEvent()->getCategory(catShowerCalIPU);
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

    initCalPar();
    fIter = (HIterator *) m_pRawCat->MakeIterator("native");

    return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HShowerCalibraterIPU::reinit(void)
{
    return kTRUE;
}

// -----------------------------------------------------------------------------

void HShowerCalibraterIPU::setCalPar(HParSet *pCalPar)
{
    if(m_pCalPar)
        delete m_pCalPar;

    m_pCalPar = pCalPar;
}

// -----------------------------------------------------------------------------

Bool_t HShowerCalibraterIPU::finalize(void)
{
   return kTRUE;
}

// -----------------------------------------------------------------------------

HShowerCalibraterIPU& HShowerCalibraterIPU::operator=(HShowerCalibraterIPU &c)
{
    return c;
}

// -----------------------------------------------------------------------------

Int_t HShowerCalibraterIPU::execute(void)
{
// calibrates all fired cells

HShowerRaw *pRaw;

    fIter->Reset();
    while((pRaw = (HShowerRaw *)fIter->Next()))
    {
    //loop on all fired pads

        m_loc[0] = pRaw->getSector();
        m_loc[1] = pRaw->getModule();
        m_loc[2] = pRaw->getRow();
        m_loc[3] = pRaw->getCol();

        calibrate(pRaw);
    }

    return 0;
}

// -----------------------------------------------------------------------------

Bool_t HShowerCalibraterIPU::calibrate(HShowerRaw *raw)
{
//calibrates the pad using slope and offset from parameter container

#if DEBUG_LEVEL>2
  gDebuger->enterFunc("HShowerCalibraterIPU::execute");
  gDebuger->message("Raw cat points to %p",raw);
#endif

HShowerCalIPU     *cal        = NULL;
HShowerCalParCell *calparcell = NULL;
Int_t              fCharge;

    if(raw == NULL)
    {
        Warning("calibrate", "NULL input");
        return kFALSE;
    }

    //pointer for calibration parameters container for pad in m_loc position
    calparcell = (HShowerCalParCell*)
                        ((HShowerCalPar*)getCalPar())->getObject(m_loc);

    //*************************************************
    // fCharge calculation in the same way like in IPU
    //*************************************************
#if 1  // with Slope !!!

    if((int)raw->getCharge() < (int)calparcell->getThreshold())
        fCharge = 0;
    else
    {
        fCharge = (int)(calparcell->getSlope()
              * (((int)raw->getCharge()) + ((int)calparcell->getThreshold())));
    }

#else  // withaut Slope !!!!

    fCharge = (((int)raw->getCharge()) + ((int)calparcell->getThreshold()));

#endif // end slope switch

    if(fCharge > 0.0)
    {
        // charge in IPU in range [0, 255]
        if(fCharge > 255)
            fCharge = 255;

        //cutoff not interested data
        cal = (HShowerCalIPU *)m_pCalCat->getNewSlot(m_zeroLoc);

        if(cal != NULL)
        {
            cal = new(cal) HShowerCalIPU;//new element of ShowerCalIPU category
            cal->setCharge(fCharge);
            cal->setSector(raw->getSector());
            cal->setModule(raw->getModule());
            cal->setRow(raw->getRow());
            cal->setCol(raw->getCol());
        }
    }

#if DEBUG_LEVEL>2
    gDebuger->leaveFunc("HShowerCalibraterIPU::execute");
#endif

    return kTRUE;
}
