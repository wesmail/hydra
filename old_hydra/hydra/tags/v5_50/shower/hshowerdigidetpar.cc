#pragma implementation
#include <stdlib.h>
#include <TClass.h>
#include "hshowerdigidetpar.h"
#include "hparhadasciifileio.h"
#include <iostream.h>

ClassImp(HShowerDigiDetPar)

/////////////////////////////////////////////////////////
//
// HShowerDigiDetPar - parameters of digitalisation
//
/////////////////////////////////////////////////////////

//------------------------------------------------------------------------------

#define CHARGE_MATRIX_SIZE (2 * nMatrixRange + 1)

//------------------------------------------------------------------------------

HShowerDigiDetPar::HShowerDigiDetPar(const char* name,const char* title,
                                     const char* context)
                  : HShowerParSet(name,title,context) {
    pfChargeMatrix = NULL;
    clear();
}

//------------------------------------------------------------------------------

HShowerDigiDetPar::~HShowerDigiDetPar()
{
    pfChargeMatrix = NULL;
    clear();
}

//------------------------------------------------------------------------------

void HShowerDigiDetPar::clear()
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

    if(pfChargeMatrix != NULL)
    {
        delete [] pfChargeMatrix;
        pfChargeMatrix = NULL;
    }

    memset(afMeanParams, 0, sizeof(afMeanParams));
    memset(afSigmaParams, 0, sizeof(afSigmaParams));
    memset(afEffParams, 0, sizeof(afEffParams));
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
Int_t status = kTRUE;

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

        pHAscii->ReadFloat(afMeanParams,  "chargeDigitMeanParams");
        pHAscii->ReadFloat(afSigmaParams, "chargeDigitSigmaParams");
        pHAscii->ReadFloat(afEffParams,   "efficiencyParams");

        setChargeMatrix(nMatrixRange);
        initChargeMatrix();
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

        pHAscii->WriteFloat("chargeDigitMeanParams", POLY_DEGREE, afMeanParams);
        pHAscii->WriteFloat("chargeDigitSigmaParams", POLY_DEGREE,
                                                    afSigmaParams);
        pHAscii->WriteFloat("efficiencyParams", POLY_DEGREE, afEffParams);
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

    setChargeMatrix(nMatrixRange);

    for(i = 0; i <= nMatrixRange; i++)
    {
        pfChargeMatrix[nMatrixRange - i] = calcCharge(1., fPlaneDist,
                                 cornerX + i * fBoxSize,
                                 cornerY,
                                 cornerX + (i + 1) * fBoxSize,
                                 cornerY + fBoxSize);

        pfChargeMatrix[nMatrixRange - i] *= fChargeSlope
                        * ((float)(nMatrixRange - i + 1)) / (nMatrixRange + 1);

        pfChargeMatrix[nMatrixRange + i] = pfChargeMatrix[nMatrixRange - i];
    }

   
// only for testing 
/*
   fstream file("charge_matrix.txt",ios::out);

    for(i = 1; i < CHARGE_MATRIX_SIZE; i++)
      file << pfChargeMatrix[i] << endl; 
*/


    for(i = 1; i < CHARGE_MATRIX_SIZE; i++)
        pfChargeMatrix[i] += pfChargeMatrix[i - 1];
/*
    for(i = 0; i < CHARGE_MATRIX_SIZE; i++)
        printf("Par: %2d: %10.6f\n", i, pfChargeMatrix[i]);
*/
}

//------------------------------------------------------------------------------

void HShowerDigiDetPar::Streamer(TBuffer &b)
{
    if(b.IsReading())
    {
    Version_t v = b.ReadVersion();

        if((HShowerDigiDetPar::IsA())->GetClassVersion() != v)
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

        b.ReadFastArray(afMeanParams, POLY_DEGREE);
        b.ReadFastArray(afSigmaParams, POLY_DEGREE);
        b.ReadFastArray(afEffParams, POLY_DEGREE);
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

        b.WriteFastArray(afMeanParams, POLY_DEGREE);
        b.WriteFastArray(afSigmaParams, POLY_DEGREE);
        b.WriteFastArray(afEffParams, POLY_DEGREE);
    }
}
