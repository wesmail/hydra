//////////////////////////////////////////////////////////////////////////////
//
// HShowerHitDigitizer
//
// This class digitizes  Pre-Shower wires. The value of the charge and
// its position
// on the sense wire are calculated. Due to do it there are needed geometry
// and digitisation parameters.
// These data are stored in HShowerGeantWire category.
//
///////////////////////////////////////////////////////////////////////////
#include "hshowerhitdigitizer.h"

#include "hruntimedb.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hshowerdetector.h"
#include "hcategory.h"
#include "hmatrixcategory.h"
#include "hlinearcategory.h"
#include "hlocation.h"
#include "hshowergeantwire.h"
#include "hshowerdigidetpar.h"
#include "hshowergeometry.h"
#include "hdebug.h"
#include "hades.h"
#include "showerdef.h"
#include <math.h>
#include <stdlib.h>
#include "TRandom.h"


//*-- Author : Leszek Kidon & Jacek Otwinowski
//*-- Modified: 01/06/2000 L.Kidon
//*-- Modified: 22/04/2000 J.Otwinowski
//*-- Modified : 26/8/99 R. Holzmann
//*-- Modified : July 99 Leszek Kidon
//*-- Modified : 04-09/07/2001 M.Jaskula:
//*--            - Gauss->Landau charge distribution
//*--            - New weights for distributsion on the wires
//*-- Modified : 02/08/2001 M.Jaskula:
//*--            - New parameters
//*--            - Efficiency (beta)

ClassImp(HShowerHitDigitizer)

//------------------------------------------------------------------------------

//#define CALCULATE_FROM_FIT(FUN, X)  (((FUN[3]*X+FUN[2])*X+FUN[1])*X+FUN[0])

//------------------------------------------------------------------------------

HShowerHitDigitizer::HShowerHitDigitizer(Text_t *name,Text_t *title,
                            Float_t unit) : HShowerDigitizer(name,title)
{
    piconst = 2 * acos(0.);
    fUnit = unit;  // HGeant length unit (1. for cm, 10. for mm)
}

HShowerHitDigitizer::HShowerHitDigitizer()
{
    piconst = 2*acos(0.);
    fUnit = 10.;
}

HShowerHitDigitizer::~HShowerHitDigitizer(void)
{
}

Bool_t HShowerHitDigitizer::init() {
// creates GeantShower(input) and ShowerGeantWire(output) categories
// and adds them to the current event

    printf("initialization of shower hit digitizer \n");

HCategory *pCat;
HShowerDetector *pShowerDet = (HShowerDetector*)gHades->getSetup()
                                           ->getDetector("Shower");

    pCat=gHades->getCurrentEvent()->getCategory(catShowerGeantRaw);
    if( ! pCat)
    {
        pCat=pShowerDet->buildCategory(catShowerGeantRaw);

        if( ! pCat)
            return kFALSE;
        else
            gHades->getCurrentEvent()
                         ->addCategory(catShowerGeantRaw, pCat, "Simul");
    }

    setInCat(pCat);

    pCat=gHades->getCurrentEvent()->getCategory(catShowerGeantWire);
    if( ! pCat)
    {
        pCat=pShowerDet->buildCategory(catShowerGeantWire);

        if( ! pCat)
            return kFALSE;
        else
            gHades->getCurrentEvent()
                         ->addCategory(catShowerGeantWire, pCat, "Shower");
    }

    setOutCat(pCat);

    return HShowerDigitizer::init();
}

HShowerHitDigitizer& HShowerHitDigitizer::operator=(HShowerHitDigitizer &c)
{
// It should have been done
    return c;
}

Bool_t HShowerHitDigitizer::digitize(TObject *pHit)
{
HLocation fLoc;
Bool_t nResult = kFALSE;

#if DEBUG_LEVEL>2
    gDebuger->enterFunc("HShowerHitDigitizer::execute");
    gDebuger->message("category points to %p", pHit);
#endif

HGeantShower      *pGeantHit = (HGeantShower*)pHit;
HShowerDigiDetPar *pDetPar   = (HShowerDigiDetPar*) getDigiParSet();

    if( ! getGeometry())
    {
        Error("HShowerHitDigitizer::digitize",
                            "Shower geometry not initialized");
        return kFALSE;
    }

    if( ! pDetPar)
    {
        Error("HShowerHitDigitizer::digitize",
                    "Digitization parameters not initialized");
        return kFALSE;
    }

    if (pGeantHit)
    {
        nResult = digiHits(pGeantHit);
    }

#if DEBUG_LEVEL>2
    gDebuger->leaveFunc("HShowerHitDigitizer::execute");
#endif

    return nResult;
}

Bool_t HShowerHitDigitizer::digiHits(HGeantShower *simhit)
{
HLocation         fLoc;
HShowerGeantWire *pGeantWire = NULL;
float             x_exit, y_exit;
Int_t             nDet, nSector;
Int_t             nFiredWire, nFiredWireIn, nFiredWireOut;
Float_t           fE, fTheta, fPhi, fBeta;
Float_t           fX, fY;
Float_t           fNewX, fNewY;
Float_t           fQ, fThick;
Float_t           fTan;
Int_t             i;
Float_t           fY0, fY1;
const HShowerDigiDetPar *pDigiDetPar = (HShowerDigiDetPar*)getDigiParSet();

    nDet = simhit->getModule();
    nSector = simhit->getSector();
    simhit->getHit(fE, fX, fY, fBeta);
    simhit->getIncidence(fTheta, fPhi);

        // check efficiency
    if( ! pDigiDetPar->checkEfficiency(fBeta))
        return kFALSE;

    fX /= fUnit;   // go from mm to cm again  (HGeant 5.0 gives mm!)
    fY /= fUnit;

        // degree to radian conv.
    fTheta = (TMath::Pi()/180.0) * fTheta;
    fPhi   = (TMath::Pi()/180.0) * fPhi;

    HShowerWireTab *phWires = ((HShowerGeometry*)getGeometry())->
                                  getWireTab(nDet);

    fThick        = pDigiDetPar->getThickDet();

    fTan = tan(fTheta);
    x_exit = fX + fThick* fTan * cos(fPhi);
    y_exit = fY + fThick* fTan * sin(fPhi);

    nFiredWireIn = phWires->lookupWire(fY);
    if(nFiredWireIn == -1)
        return kFALSE;

    nFiredWireOut = phWires->lookupWire(y_exit);
    if(nFiredWireOut == -1)
        return kFALSE;

    if(nFiredWireIn > nFiredWireOut) {   // swap
        i = nFiredWireIn;
        nFiredWireIn  = nFiredWireOut;
        nFiredWireOut = i;
    }

    for(nFiredWire = nFiredWireIn; nFiredWire <= nFiredWireOut; nFiredWire++)
    {
        fNewY = phWires->getWire(nFiredWire)->getYWire();

        if(nFiredWireOut == nFiredWireIn)
        {
                // track inside one cell
            fNewX = 0.5 * (fX + x_exit); // average (IN + OUT)
        }
        else
        {
                // find out Y track's corrd. inside the cell
            if(nFiredWire == nFiredWireIn)
                fY0 = fY;
            else
            {
                    // average of two sense wires (a field wire position)
                fY0 = 0.5 * (phWires->getWire(nFiredWire - 1)->getYWire()
                            + fNewY);
            }

            if(nFiredWire == nFiredWireOut)
                fY1 = y_exit;
            else
            {
                fY1 = 0.5 * (fNewY
                            + phWires->getWire(nFiredWire + 1)->getYWire());
            }

                // X corrd. depends on the average X value inside the cell
            fNewX = fX + 0.5 * (x_exit - fX) * (fY1 + fY0 - 2 * fY)
                            / (y_exit - fY);
        }

        //if((fQ = calcHit(fBeta, pfParams, pfSigmaParams)) > 0.0f)
        if((fQ = pDigiDetPar->getCharge(fBeta)) > 0.0f)
        {
            pGeantWire = (HShowerGeantWire*)((HLinearCategory*)getOutCat())
                                                          ->getNewSlot(fLoc);
            if(pGeantWire != NULL)
            {
                pGeantWire = new(pGeantWire) HShowerGeantWire;
                pGeantWire->setWireNr(nFiredWire);
                pGeantWire->setXY(fNewX, fNewY);
                pGeantWire->setCharge(fQ);
                pGeantWire->setModule(nDet);
                pGeantWire->setSector(nSector);
                pGeantWire->setTrack(simhit->getTrack());
            }
        }
    }

    return kTRUE;
}
/*
Float_t HShowerHitDigitizer::calcHit(Float_t fBeta,
                const Float_t *pfMeanParams, const Float_t *pfSigmaParams)
{
Float_t  fQ;
int      iTry;
Double_t dMean;
Double_t dSigma;

        // variables for Landau distribution
    dMean  = CALCULATE_FROM_FIT(pfMeanParams, fBeta);
    dSigma = CALCULATE_FROM_FIT(pfSigmaParams, fBeta);

        // do not make an infinitive loop for a negative distribution
    for(iTry = 0; iTry < 10; iTry++)
    {
        if((fQ = gRandom->Landau(dMean, dSigma)) > 0.0)
        {
            if(fQ > 250.0f)
                continue;

            return fQ;
        }
    }

        // for an infinite loop
    return 0.0f;
}
*/
