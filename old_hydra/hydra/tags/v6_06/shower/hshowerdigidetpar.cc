#pragma implementation
#include <TClass.h>
#include <TFile.h>
#include "hshowerdigidetpar.h"
#include "hparhadasciifileio.h"
#include <iostream.h>
#include <stdlib.h>
#include <TRandom.h>

ClassImp(HShowerDigiDetPar)

/////////////////////////////////////////////////////////
//
// HShowerDigiDetPar - parameters of digitalisation
//
/////////////////////////////////////////////////////////

//------------------------------------------------------------------------------

#define CHARGE_MATRIX_SIZE (2 * nMatrixRange + 1)
#define CALCULATE_FROM_FIT(FUN, X)  (((FUN[3]*X+FUN[2])*X+FUN[1])*X+FUN[0])

//------------------------------------------------------------------------------
 HShowerDigiDetPar::HShowerDigiDetPar(const char* name,const char* title,
                                      const char* context)
                                      : HShowerParSet(name,title,context)
{
    pfChargeMatrix = NULL;
    phEff          = NULL;
    ph2QvB         = NULL;
    pArrayQvB      = NULL;
    clear();
}

//------------------------------------------------------------------------------

HShowerDigiDetPar::~HShowerDigiDetPar()
{
    clear();
}

//------------------------------------------------------------------------------

void HShowerDigiDetPar::clear(void)
{
    fThickDet           = 1.0f;
    nMatrixRange        = 0;
    fBoxSize            = 1.0f;
    fChargeSlope        = 1.0f;
    fPlaneDist          = 1.0f;
    nPadRange           = 0;
    fPadThreshold       = 0.0f;
    fUpdatePadThreshold = 0.0f;
    fMasterOffset       = 0.0f;

    dQvSmin             = 0.0;
    dQvSmax             = 0.0;
    iQvSbins            = 0;

    if(pfChargeMatrix != NULL)
    {
        delete [] pfChargeMatrix;
        pfChargeMatrix = NULL;
    }

    memset(afMeanParams, 0, sizeof(afMeanParams));
    memset(afSigmaParams, 0, sizeof(afSigmaParams));
    memset(afEffParams, 0, sizeof(afEffParams));

    if(phEff != NULL)
    {
        delete phEff;
        phEff = NULL;
    }

    if(ph2QvB != NULL)
    {
        delete ph2QvB;
        ph2QvB = NULL;
    }

    if(pArrayQvB != NULL)
    {
        delete pArrayQvB;
        pArrayQvB = NULL;
    }
}

//------------------------------------------------------------------------------

void HShowerDigiDetPar::setChargeMatrix(Int_t nRange, const Float_t *pMatrix)
{
    if(pfChargeMatrix != NULL)
        delete [] pfChargeMatrix;

    nMatrixRange = nRange;

    if(nMatrixRange == 0)
        pfChargeMatrix = NULL;
    else
    {
        pfChargeMatrix = new Float_t[CHARGE_MATRIX_SIZE];
        if(pMatrix != NULL)
        {
            memcpy(pfChargeMatrix, pMatrix,
                            sizeof(Float_t) * CHARGE_MATRIX_SIZE);
        }
        else
            memset(pfChargeMatrix, 0, sizeof(Float_t) * CHARGE_MATRIX_SIZE);
    }
}

//------------------------------------------------------------------------------

void HShowerDigiDetPar::setMeanParams(const Float_t *pParams)
{
    if(pParams == NULL)
        memset(afMeanParams, 0, sizeof(afMeanParams));
    else
        memcpy(afMeanParams, pParams, sizeof(afMeanParams));
}

//------------------------------------------------------------------------------

void HShowerDigiDetPar::setSigmaParams(const Float_t *pParams)
{
    if(pParams == NULL)
        memset(afSigmaParams, 0, sizeof(afSigmaParams));
    else
        memcpy(afSigmaParams, pParams, sizeof(afSigmaParams));
}

//------------------------------------------------------------------------------

void HShowerDigiDetPar::setEffParams(const Float_t *pParams)
{
    if(pParams == NULL)
        memset(afEffParams, 0, sizeof(afEffParams));
    else
        memcpy(afEffParams, pParams, sizeof(afEffParams));
}

//------------------------------------------------------------------------------

Bool_t HShowerDigiDetPar::initAscii(HParHadAsciiFileIo* pHadAsciiFile)
{
Int_t   status = kTRUE;
TFile  *pFile;
Char_t  sFileName[1000];
Char_t  sHistName[1000];

    if(pHadAsciiFile == NULL)
        return kFALSE;

    try
    {
    HAsciiKey *pHAscii = pHadAsciiFile->GetKeyAscii();

        pHAscii->SetActiveSection("Shower Digitalization Parameters");

        fThickDet           = pHAscii->ReadFloat("thickDet");
        nMatrixRange        = pHAscii->ReadInt("matrixRange");
        fBoxSize            = pHAscii->ReadFloat("boxSize");
        fChargeSlope        = pHAscii->ReadFloat("chargeSlope");
        fPlaneDist          = pHAscii->ReadFloat("planeDist");
        nPadRange           = pHAscii->ReadInt("padRange");
        fPadThreshold       = pHAscii->ReadFloat("padThreshold");
        fUpdatePadThreshold = pHAscii->ReadFloat("updatePadThreshold");
        fMasterOffset       = pHAscii->ReadFloat("masterOffset");

//      pHAscii->ReadFloat(afMeanParams,  "chargeDigitMeanParams");
//      pHAscii->ReadFloat(afSigmaParams, "chargeDigitSigmaParams");
//      pHAscii->ReadFloat(afEffParams,   "efficiencyParams");

        setChargeMatrix(nMatrixRange);
        initChargeMatrix();

            // reads and get the histograms
        strcpy(sFileName, pHAscii->ReadString("sumVerBetaFile"));
        strcpy(sHistName, pHAscii->ReadString("sumVerBetaHist"));
        printf("sFileName: %s\nsHistName: %s\n", sFileName, sHistName);
        if(((pFile = new TFile(sFileName)) == NULL)
                || (pFile->IsOpen() == kFALSE))
        {
            Error("Cannot open the input histogram file: %s\n", sFileName);
            status = kFALSE;
        }
        else
        {
            if((ph2QvB = (TH2F *)pFile->Get(sHistName)) == NULL)
            {
                Error("No histogram %s in file: %s\n", sHistName, sFileName);
                status = kFALSE;
            }
            else
                ph2QvB->SetDirectory(0);
        }

        if(pFile != NULL)
            pFile->Close();

        if(status == kFALSE)
            return status;

            // reads and get the histograms
        strcpy(sFileName, pHAscii->ReadString("efficiencyFile"));
        strcpy(sHistName, pHAscii->ReadString("efficiencyHist"));
        if(((pFile = new TFile(sFileName)) == NULL)
                || (pFile->IsOpen() == kFALSE))
        {
            Error("Cannot open the input histogram file: %s\n", sFileName);
            status = kFALSE;
        }
        else
        {
            if((phEff = (TH1F *)pFile->Get(sHistName)) == NULL)
            {
                Error("No histogram %s in file: %s\n", sHistName, sFileName);
                status = kFALSE;
            }
            else
                phEff->SetDirectory(0);
        }

        if(pFile != NULL)
            pFile->Close();

        status = initSumVerBetaHistograms();
    }
    catch(Bool_t ret)
    {
        status = ret;
    }

    return status;
}

//------------------------------------------------------------------------------

Bool_t HShowerDigiDetPar::writeAscii(HParHadAsciiFileIo* pHadAsciiFile)
{
Bool_t status=kTRUE;

    if(pHadAsciiFile == NULL)
        return kFALSE;

    try
    {
    HAsciiKey* pHAscii = pHadAsciiFile->GetKeyAscii();

        pHAscii->WriteSection("Shower Digitalization Parameters");

        pHAscii->WriteFloat("thickDet", fThickDet);
        pHAscii->WriteInt("matrixRange", nMatrixRange);
        pHAscii->WriteFloat("boxSize", fBoxSize);
        pHAscii->WriteFloat("chargeSlope", fChargeSlope);
        pHAscii->WriteFloat("planeDist", fPlaneDist);
        pHAscii->WriteInt("padRange", nPadRange);
        pHAscii->WriteFloat("padThreshold", fPadThreshold);
        pHAscii->WriteFloat("updatePadThreshold", fUpdatePadThreshold);
        pHAscii->WriteFloat("masterOffset",fMasterOffset);

//      pHAscii->WriteFloat("chargeDigitMeanParams", POLY_DEGREE, afMeanParams);
//      pHAscii->WriteFloat("chargeDigitSigmaParams", POLY_DEGREE,
//                                                  afSigmaParams);
//      pHAscii->WriteFloat("efficiencyParams", POLY_DEGREE, afEffParams);
    }
    catch(Bool_t ret)
    {
        ret = status;
    }

    return status;
}

//------------------------------------------------------------------------------

Float_t HShowerDigiDetPar::calcCharge(Float_t fCharge, Float_t fDist,
                        Float_t fXd, Float_t fYd, Float_t fXu, Float_t fYu)
{
const float twoPI = 6.28318530718;

return ((fCharge / twoPI) *
        (atan(fXd * fYd / (fDist * sqrt(fDist * fDist + fXd * fXd + fYd * fYd)))
       - atan(fXd * fYu / (fDist * sqrt(fDist * fDist + fXd * fXd + fYu * fYu)))
       + atan(fXu * fYu / (fDist * sqrt(fDist * fDist + fXu * fXu + fYu * fYu)))
       - atan(fXu * fYd / (fDist * sqrt(fDist * fDist + fXu * fXu + fYd * fYd)))
       ));
}

//------------------------------------------------------------------------------

void  HShowerDigiDetPar::initChargeMatrix()
{
int   i;
float cornerX = -0.5 * fBoxSize;
float cornerY = cornerX;
float fM;

    setChargeMatrix(nMatrixRange);

    for(i = 0; i <= nMatrixRange; i++)
    {
        pfChargeMatrix[nMatrixRange - i] = calcCharge(1., fPlaneDist,
                                 cornerX + i * fBoxSize,
                                 cornerY,
                                 cornerX + (i + 1) * fBoxSize,
                                 cornerY + fBoxSize);

        if((fM = (float)(1.0 - fChargeSlope * i / nMatrixRange)) < 0.0f)
            fM = 0.0f;

        pfChargeMatrix[nMatrixRange - i] *= fM;
/*
        pfChargeMatrix[nMatrixRange - i] *= fChargeSlope
                        * ((float)(nMatrixRange - i + 1)) / (nMatrixRange + 1);
*/
        //printf("%d: %f\n", i, fM);

        pfChargeMatrix[nMatrixRange + i] = pfChargeMatrix[nMatrixRange - i];
    }

    for(i = 1; i < CHARGE_MATRIX_SIZE; i++)
        pfChargeMatrix[i] += pfChargeMatrix[i - 1];
}

//------------------------------------------------------------------------------

void HShowerDigiDetPar::Streamer(TBuffer &b)
{
    if(b.IsReading())
    {
    Version_t v = b.ReadVersion();

        if((HShowerDigiDetPar::IsA())->GetClassVersion() < 2)
        {
            Error("Streamer",
                "Bad version of HShowerDigiDetPar %d / %d !!!\n",
                v, (HShowerDigiDetPar::IsA())->GetClassVersion());

            exit(-1);
        }

        HShowerParSet::Streamer(b);

        b >> fThickDet;
        b >> nMatrixRange;
        b >> fBoxSize;
        b >> fChargeSlope;
        b >> fPlaneDist;
        b >> nPadRange;
        b >> fPadThreshold;
        b >> fUpdatePadThreshold;
        b >> fMasterOffset;

        setChargeMatrix(nMatrixRange);
        b.ReadFastArray(pfChargeMatrix, CHARGE_MATRIX_SIZE);

        if(v == 2)
        {
            b.ReadFastArray(afMeanParams, POLY_DEGREE);
            b.ReadFastArray(afSigmaParams, POLY_DEGREE);
            b.ReadFastArray(afEffParams, POLY_DEGREE);
            pArrayQvB = NULL;
            phEff     = NULL;
            ph2QvB    = NULL;
        }
        else
        {
            b >> phEff;
            b >> ph2QvB;
            pArrayQvB = NULL;

            phEff->SetDirectory(0);
            ph2QvB->SetDirectory(0);

            if( ! initSumVerBetaHistograms())
                Error("%s", "HShowerDigiDetPar::Streamer");
        }
    }
    else
    {
        b.WriteVersion(HShowerDigiDetPar::IsA());

        HShowerParSet::Streamer(b);

        b << fThickDet;
        b << nMatrixRange;
        b << fBoxSize;
        b << fChargeSlope;
        b << fPlaneDist;
        b << nPadRange;
        b << fPadThreshold;
        b << fUpdatePadThreshold;
        b << fMasterOffset;

        if(pfChargeMatrix != NULL)
            b.WriteFastArray(pfChargeMatrix, CHARGE_MATRIX_SIZE);

        /*
        b.WriteFastArray(afMeanParams, POLY_DEGREE);
        b.WriteFastArray(afSigmaParams, POLY_DEGREE);
        b.WriteFastArray(afEffParams, POLY_DEGREE);
        */

        b << (TObject*)phEff;
        b << (TObject*)ph2QvB;
    }
}

//------------------------------------------------------------------------------

Bool_t HShowerDigiDetPar::initSumVerBetaHistograms(void)
{
Int_t    iSums;
Double_t dSumMin, dSumMax;
Int_t    iBeta;
TH1D    *pH;
char     s1[100];
char     s2[100];

    if(pArrayQvB != NULL)
    {
        delete pArrayQvB;
        pArrayQvB = NULL;
    }

    if(ph2QvB == NULL)
    {
        Error("%s", "No QvB histogram");
        return kFALSE;
    }

    iQvSbins = ph2QvB->GetNbinsX();
    dQvSmin  = ph2QvB->GetXaxis()->GetXmin();
    dQvSmax  = ph2QvB->GetXaxis()->GetXmax();

    d1_QvSDiff = 1.0 / (dQvSmax - dQvSmin);
    dQvSOneBin = (dQvSmax - dQvSmin) / iQvSbins;

    iSums    = ph2QvB->GetNbinsY();
    dSumMin  = ph2QvB->GetYaxis()->GetXmin();
    dSumMax  = ph2QvB->GetYaxis()->GetXmax();

    pArrayQvB = new TObjArray(iQvSbins);
    pArrayQvB->SetOwner();

    for(iBeta = 0; iBeta < iQvSbins; iBeta++)
    {
        sprintf(s1, "sumVerBeta_%02d", iBeta);
        sprintf(s2, "sum(%f)",
                    dQvSmin + (dQvSmax - dQvSmin) * (iBeta + 0.5) / iQvSbins);

        pH = ph2QvB->ProjectionY(s1, iBeta,  iBeta + 1);
        pH->SetDirectory(NULL);
        pH->SetTitle(s2);
        pArrayQvB->AddAt(pH, iBeta);
    }

    return kTRUE;
}

//------------------------------------------------------------------------------

Float_t HShowerDigiDetPar::getEfficiency(Float_t fBeta) const
{
    if(phEff == NULL)
        return CALCULATE_FROM_FIT(afEffParams, fBeta);

    return phEff->GetBinContent(phEff->GetXaxis()->FindFixBin(fBeta));
}

//------------------------------------------------------------------------------

Bool_t HShowerDigiDetPar::checkEfficiency(Float_t fBeta) const
{
Float_t f = getEfficiency(fBeta);

    return ((f >= 1.0f) || ((f > 0.0f) && (f > gRandom->Rndm())));
}

//------------------------------------------------------------------------------

Float_t HShowerDigiDetPar::getCharge(Float_t fBeta) const
{
Float_t  fQ;

    if(ph2QvB == NULL)
    {
    Double_t dMean;
    Double_t dSigma;
    Int_t    iTry;

        dMean  = CALCULATE_FROM_FIT(afMeanParams, fBeta);
        dSigma = CALCULATE_FROM_FIT(afSigmaParams, fBeta);

        for(iTry = 0; iTry < 10; iTry++)
        {
            if((fQ = gRandom->Landau(dMean, dSigma)) > 0.0)
            {
                if(fQ > 250.0f)
                    continue;

                return fQ;
            }
        }

        return 0.0f;
    }

Int_t iBin;
TH1D *pH;

    iBin = (Int_t)((fBeta - dQvSmin) * iQvSbins * d1_QvSDiff);
    if(iBin < 0)
        iBin = 0;

    if(iBin >= iQvSbins)
        iBin = iQvSbins - 1;

    if((pH = (TH1D *)pArrayQvB->At(iBin)) == NULL)
        return 0.0f;

    fQ = (Float_t)(pH->GetRandom() + (2 * gRandom->Rndm() - 1) * dQvSOneBin);
    return ((fQ < 0.0f) ? 0.0f : fQ);
}
