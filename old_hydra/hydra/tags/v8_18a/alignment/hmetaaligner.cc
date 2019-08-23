// -----------------------------------------------------------------------------

#include "hmetaaligner.h"

#include <TFitter.h>

#include "hades.h"
#include "hspectrometer.h"
#include "hparrootfileio.h"

#include "hshowerdetector.h"
#include "hshowerhistcell.h"
#include "hshowercalpar.h"
#include "hshowergeometry.h"
#include "hshowerpad.h"

#include "htofdetector.h"
#include "htofgeompar.h"

//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////////////
// HMetaAligner
//
// A class for aligning the META detectors respective to the straight lines
// defined from the target and MDC positions or the Santiago tracking.
//
// The input is a Ntuples file which defines:
//   - lines parameters
//   - position of the corresponding hits on a META detector
//   - resolutions of the positions
//
// The alignment procedure tries to minimize chi^2 defined as the sum of
// squares distances between the line and the hit on the detector normalized
// by the resolutions of the detectors.
//
// The class allows to fix/release translations and rotations of the detectors.
// The translations are defined as additional offsets in the lab system
// from the position defined in the parameters file (RunTime dB).
// The rotations are defined in the local co-ordinate system
// around X, Y, Z axes - in this order.
//
// Tukay's bi-squered weights may be use to improve the results, as well as
// an improved method which changes the weights on flight (it doesn't work well
// yet).
// The method calculates a weight of each chi^2 taken into the account.
// If the chi^2 > Tc then the point is not taken. Otherwise it is taken with
// a weight = (1 - (chi^2/Ct^2))^2
// When the improved method is used then the Ct changes from iteration
// to iteration (as it was written above this doesn't work well).
////////////////////////////////////////////////////////////////////////////////

//*-- Author : M. Jaskula (Sep 2001)
// $Id: hmetaaligner.cc,v 1.4 2008-05-09 16:06:15 halo Exp $

// -----------------------------------------------------------------------------

ClassImp(HMetaAligner)

// -----------------------------------------------------------------------------

HMetaAligner      *g_pCurrentMetaAligner = NULL;
const static Char_t *g_sapMetaAlignerParamsNames[kMETA_ALIGNER_PARAMS]
                        = { "dX", "dY", "dZ", "rotX", "rotY", "rotZ" };
const static Char_t *g_sapMinuitMethods[]
                        = { "MINImize", "MIGrad", "SIMplex" };

// -----------------------------------------------------------------------------

#define SAFE_DELETE(A)  { if(A != NULL) { delete A; A = NULL; }}

// -----------------------------------------------------------------------------

void hMetaAlignMinuitFunction(Int_t &npar, Double_t *gin, Double_t &f,
                            Double_t *par, Int_t iflag)
{
    g_pCurrentMetaAligner->minAlign(npar, gin, f, par, iflag);
}

// -----------------------------------------------------------------------------

void HMetaAligner::init(void)
{
// Called in the constructors to clear the variables

    m_bInitOK        = kFALSE;

    m_iSector        = 0;
    m_iSystem        = 0;
    m_pTupleFile     = NULL;
    m_pTuple         = NULL;
    m_iEntries       = 0;
    m_iMaxEntries    = 0;
    m_iVerbose       = 2;

    m_bNormalize     = kTRUE;
    m_bEqualSigmas   = kFALSE;

    // set default parameters
    for(Int_t i = 0; i < kMETA_ALIGNER_PARAMS; i++)
    {
        m_dStartParams[i] = 0.0;
        m_dParams[i]      = 0.0;
        m_dStartErrors[i] = (i < 3) ? 10 : 1;
        m_dErrors[i]      = 0.0;
        m_dSteps[i]       = kMATA_ALIGN_DEFAULT_STEP;
        m_bFixed[i]       = (i > 2);
    }

    m_dError         = kMATA_ALIGN_DEFAULT_ERROR;
    m_dMigSteps      = kMATA_ALIGN_DEFAULT_STEPS;

    m_bUseTukay      = kTRUE;
    m_dTukayConst    = kMATA_ALIGN_DEFAULT_TUKAY_CONST;
    m_dTukayConst_2  = m_dTukayConst * m_dTukayConst;

    m_dLastChi_2     = 0.0;
    m_iStatus        = 0;
    m_iLastThrown    = 0;
    m_iHits          = 0;

    m_pTupleFile     = NULL;
    m_pTuple         = NULL;

    m_pRTDB          = NULL;

    m_fXc            = 0.0;
    m_fYc            = 0.0;
    m_fZc            = 0.0;
    m_fAlphaX        = 0.0;
    m_fAlphaY        = 0.0;
    m_fAlphaZ        = 0.0;
    m_fXo            = 0.0;
    m_fYo            = 0.0;
    m_fSigXo         = 0.0;
    m_fSigYo         = 0.0;
    m_fSec           = 0.0;
    m_fMod           = 0.0;
    m_fSys           = 0.0;

    m_dChi2X         = 0.0;
    m_dChi2Y         = 0.0;

    m_dWeightChi2X   = 0.0;
    m_dWeightChi2Y   = 0.0;

    m_bImprove       = kFALSE;

    m_bChangeWeights = kFALSE;
}

// -----------------------------------------------------------------------------

HMetaAligner::HMetaAligner(const Char_t *pFileName, Int_t iSector, Int_t iSystem,
                            const Char_t *pParamFile, Int_t iRunId)
                        : TObject()
{
// Initialize the class.
// pFileName - is a name of an input file which consists of Ntuples
// iSector and iSystem define the detector. iSystem == 0: Shower, 1: Tof
// pParamFile and iRunId defines setup with parameters

    init();

    m_iSector = iSector;
    m_iSystem = iSystem;

    if( ! loadParameters(pParamFile, iRunId))
        return;

    if(openNTuple(pFileName))
        m_bInitOK = kTRUE;
}

// -----------------------------------------------------------------------------

HMetaAligner::~HMetaAligner(void)
{
    SAFE_DELETE(m_pTuple);
    SAFE_DELETE(m_pTupleFile);
}

// -----------------------------------------------------------------------------

Bool_t HMetaAligner::loadParameters(const Char_t *pParamFileName, Int_t iRunId)
{
// Initialize HADES & Runtime dB and load parameters for the specified detector

    if(gHades == NULL)
        new Hades;

    m_pRTDB = gHades->getRuntimeDb();

    if(m_iVerbose)
        printf("Init for System: %d   Sector: %d\n", m_iSystem, m_iSector);

    // initailize the detectors
    if(m_iSystem == 0)
        return getShowerGeom(pParamFileName, iRunId);
    else
        return getTofGeom(pParamFileName, iRunId);
}

// -----------------------------------------------------------------------------

Bool_t HMetaAligner::getShowerGeom(const Char_t *pParamFile, Int_t iRunId)
{
// Initailize PreShower geom parameters
HGeomTransform   tTransform;
Int_t            i;
HShowerGeometry *pShowerGeom;
HParRootFileIo  *pInput;

    if(gHades->getSetup()->getDetector("Shower") == NULL)
    {
    Int_t            mods[3] = {1, 1, 1};
    HShowerDetector *pShower = new HShowerDetector;

        for(Int_t iSec = 0; iSec < 6; iSec++)
            pShower->setModules(iSec, mods);

        gHades->getSetup()->addDetector(pShower);
    }

    pInput = new HParRootFileIo;
    pInput->open((Text_t *)pParamFile, "READ");
    m_pRTDB->setFirstInput(pInput);

    if((pShowerGeom = (HShowerGeometry*)m_pRTDB
                                ->getContainer("ShowerGeometry")) == NULL)
    {
        pShowerGeom = new HShowerGeometry;
        m_pRTDB->addContainer(pShowerGeom);
    }

    if(m_pRTDB->initContainers(iRunId) == kFALSE)
    {
        Error("getShowerGeom", "m_pRTDB->initContainers error");
        return kFALSE;
    }

    tTransform = pShowerGeom->getTransform(m_iSector, 0);

    m_arRI[0] = tTransform.getRotMatrix().inverse();
    m_avT[0]  = tTransform.getTransVector();

    for(i = 1; i < kMETA_ALIGNER_MATRICS; i++)
    {
        m_arRI[i] = m_arRI[0];
        m_avT[i]  = m_avT[0];
    }

    return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HMetaAligner::getTofGeom(const Char_t *pParamFile, Int_t iRunId)
{
// Initailize Tof geom parameters
HGeomTransform   tTransform;
Int_t            i;
HParRootFileIo  *pInput;
HTofGeomPar     *pTofGeom;

    if(gHades->getSetup()->getDetector("Tof") == NULL)
    {
    Int_t         mods[22] = {1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    HTofDetector *pTof     = new HTofDetector;

        for(Int_t iSec = 0; iSec < 6; iSec++)
            pTof->setModules(iSec, mods);

        gHades->getSetup()->addDetector(pTof);
    }

    pInput = new HParRootFileIo;
    pInput->open((Text_t *)pParamFile, "READ");
    m_pRTDB->setFirstInput(pInput);

    pTofGeom = (HTofGeomPar *)m_pRTDB->getContainer("TofGeomPar");
                       
    if(m_pRTDB->initContainers(iRunId) == kFALSE)
    {
        Error("getTofGeom", "m_pRTDB->initContainers error");
        return kFALSE;
    }

    for(i = 0; i < kMETA_ALIGNER_MATRICS; i++)
    {
        tTransform = pTofGeom->getModule(m_iSector, i)->getLabTransform();
        m_arRI[i]  = tTransform.getRotMatrix().inverse();
        m_avT[i]   = tTransform.getTransVector();
    }

    return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HMetaAligner::openNTuple(const Char_t *pFileName)
{
// Open the file with the NTuple

    m_pTupleFile = new TFile(pFileName);
    if((m_pTuple = (TNtuple *) m_pTupleFile->Get("metaAlign")) == NULL)
    {
        Error("openNTuple", "No 'metaAlign' Tuple in file: %s", pFileName);
        return kFALSE;
    }

    m_pTuple->SetBranchAddress("xc",     &m_fXc);
    m_pTuple->SetBranchAddress("yc",     &m_fYc);
    m_pTuple->SetBranchAddress("zc",     &m_fZc);
    m_pTuple->SetBranchAddress("alphax", &m_fAlphaX);
    m_pTuple->SetBranchAddress("alphay", &m_fAlphaY);
    m_pTuple->SetBranchAddress("alphaz", &m_fAlphaZ);
    m_pTuple->SetBranchAddress("xo",     &m_fXo);
    m_pTuple->SetBranchAddress("yo",     &m_fYo);
    m_pTuple->SetBranchAddress("sigxo",  &m_fSigXo);
    m_pTuple->SetBranchAddress("sigyo",  &m_fSigYo);
    m_pTuple->SetBranchAddress("s",      &m_fSec);
    m_pTuple->SetBranchAddress("mod",    &m_fMod);
    m_pTuple->SetBranchAddress("sys",    &m_fSys);

    if((m_iEntries = (Int_t)m_pTuple->GetEntries()) <= 0)
    {
        Error("openNTuple", "No entires in NTuple");
        return kFALSE;
    }

    m_iMaxEntries = m_iEntries;

    if(m_iVerbose)
        printf("%d events in file: %s\n", m_iMaxEntries, pFileName);

    return kTRUE;
}

// -----------------------------------------------------------------------------

void HMetaAligner::setValue(Int_t iParam, Double_t dVal)
{
// Set the start value for a parameter.
// iParam:
// 0: dX
// 1: dY
// 2: dZ
// 3: dRotX
// 4: dRotY
// 5: dRotZ

    if((iParam < 0) || (iParam >= kMETA_ALIGNER_PARAMS))
        Error("setValue", "Wrong param id: %d", iParam);
    else
        m_dStartParams[iParam] = dVal;
}

// -----------------------------------------------------------------------------

void HMetaAligner::setStep(Int_t iParam, Double_t dStep)
{
// Set start steps for the parameters

    if((iParam < 0) || (iParam >= kMETA_ALIGNER_PARAMS))
        Error("setStep", "Wrong param id: %d", iParam);
    else
        m_dSteps[iParam] = dStep;
}

// -----------------------------------------------------------------------------

void HMetaAligner::fixParam(Int_t iParam, Bool_t bFix)
{
// Fix the parameter

    if((iParam < 0) || (iParam >= kMETA_ALIGNER_PARAMS))
        Error("fixParam", "Wrong param id: %d", iParam);
    else
        m_bFixed[iParam] = bFix;
}

// -----------------------------------------------------------------------------

void HMetaAligner::releaseParam(Int_t iParam)
{
// Release the parameters

    fixParam(iParam, kFALSE);
}

// -----------------------------------------------------------------------------

void HMetaAligner::releaseAll(void)
{
// Release all the parameters

    for(Int_t i = 0; i < kMETA_ALIGNER_PARAMS; i++)
        m_bFixed[i] = kFALSE;
}

// -----------------------------------------------------------------------------

void HMetaAligner::fixAll(void)
{
// Fix all the parameters

    for(Int_t i = 0; i < kMETA_ALIGNER_PARAMS; i++)
        m_bFixed[i] = kTRUE;
}

// -----------------------------------------------------------------------------

void HMetaAligner::reversFixed(void)
{
// Revers the fix flag for all parameters

    for(Int_t i = 0; i < kMETA_ALIGNER_PARAMS; i++)
        m_bFixed[i] = ! m_bFixed[i];
}

// ---------------------------------------------------------------------------

void HMetaAligner::setError(Double_t d)
{
// Set the MINUIT error value

    m_dError = d;
}

// -----------------------------------------------------------------------------

void HMetaAligner::setMigRad(Double_t d)
{
// Set the maximum number of iteration for MINUIT

    m_dMigSteps = d;
}

// -----------------------------------------------------------------------------

void HMetaAligner::setTukayConst(Double_t d)
{
// Set the Tukay's constans.

    m_dTukayConst   = d;
    m_dTukayConst_2 = m_dTukayConst * m_dTukayConst;
}

// -----------------------------------------------------------------------------

void HMetaAligner::setNormalize(Bool_t bSet)
{
// Normalize the chi^2 to the number of events taken into the account

    m_bNormalize = bSet;
}

// -----------------------------------------------------------------------------

void HMetaAligner::setImprove(Bool_t bSet)
{
// Should MINUIT try to improve the results ? The calculations are twice longer.
    m_bImprove = bSet;
}

// -----------------------------------------------------------------------------

void HMetaAligner::setEntries(Int_t i)
{
// Set the number of entries taken into the account (Not all from the input
// file). If i <= 0 then all events are taken.

    if((i <= 0) || (i > m_iMaxEntries))
        m_iEntries = m_iMaxEntries;
    else
        m_iEntries = i;
}

// -----------------------------------------------------------------------------

void HMetaAligner::copyResultsToStart(void)
{
// Copy the results from the previous iteration to the input parameters.

    for(Int_t i = 0; i < kMETA_ALIGNER_PARAMS; i++)
        m_dStartParams[i] = m_dParams[i];
}

// -----------------------------------------------------------------------------

Bool_t HMetaAligner::runFit(Bool_t bUseTukay, const Char_t *pOut, Int_t iMethod)
{
// Run the alignment procedure.
// bUseTukay defines if the Tukay's weights should be used.
// pOut defines a file with the output NTuples. pOut == NULL - no output file.
// iMethod: MINUIT minimize method:
// 0 - MINImize
// 1 - MIGrad
// 2 - SIMplex
Double_t        args[3];
Int_t           i;
TVirtualFitter *pFitter;
Char_t            s[100];
Double_t        dL, dH;
Int_t           iVP, iNP;

    if( ! m_bInitOK)
    {
        Error("runFit", "Class not initialized !");
        return kFALSE;
    }

    if(m_iEntries <= 0)
    {
        Error("runFit", "Entries <= 0");
        return kFALSE;
    }

    m_bUseTukay    = bUseTukay;
    m_dLastChi_2   = 0.0;
    m_iLastThrown  = 0;
    m_iHits        = 0;
    m_iCalls       = 0;
    m_dPrevSigma   = 0.0;

    m_iMethod     = iMethod;

    if((m_iMethod < 0)
        || (m_iMethod >= (Int_t)(sizeof(g_sapMinuitMethods) / sizeof(Char_t *))))
    {
        Error("runFit", "Unknown method");
        return kFALSE;
    }

    g_pCurrentMetaAligner = this;

    pFitter = TVirtualFitter::Fitter(this);
    pFitter->Clear();
    pFitter->SetFCN(::hMetaAlignMinuitFunction);

    args[0] = m_dError;
    pFitter->ExecuteCommand("SET ERR", args, 1);

    for(i = 0; i < kMETA_ALIGNER_PARAMS; i++)
    {
        if(pFitter->SetParameter(i, g_sapMetaAlignerParamsNames[i],
                    m_dStartParams[i], m_dStartErrors[i], 0, 0) != 0)
        {
            Error("runFit", "MNPARM unable to implement definition: %s\n",
                                g_sapMetaAlignerParamsNames[i]);

            g_pCurrentMetaAligner = NULL;

            return kFALSE;
        }

        if(m_bFixed[i])
            pFitter->FixParameter(i);
        else
            pFitter->ReleaseParameter(i);
    }

    args[0] = m_dMigSteps;
    args[1] = 1.0; // 0.1 - tolerance
    pFitter->ExecuteCommand(g_sapMinuitMethods[m_iMethod], args, 2);

    if(m_bImprove)
        pFitter->ExecuteCommand("IMPROVE", args, 0);

    for(i = 0; i < kMETA_ALIGNER_PARAMS; i++)
        pFitter->GetParameter(i, s, m_dParams[i], m_dErrors[i], dL, dH);

    m_iStatus = pFitter->GetStats(m_dLastChi_2, dL, dH, iVP, iNP);

    if(pOut != NULL)
        writeResults(pOut);

    if(m_iVerbose >= 2)
        print();

    g_pCurrentMetaAligner = NULL;

    return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HMetaAligner::contFit(Bool_t bUseTukay, const Char_t *pOut, Int_t iMethod)
{
// Next iteration of the alignment procedure.
// Parameters as for runFit method.

    copyResultsToStart();
    return runFit(bUseTukay, pOut, iMethod);
}

// -----------------------------------------------------------------------------

void HMetaAligner::print(const Char_t *pFileName) const
{
// Print a table with all the results of the alignment procedure.
// If pFileName != NULL the output is redirected to the pFileName file,
// otherwise it is send to stdout.
// When (I) appears after the method name the minimization was improved.
// (Chw) after the Tukay's const. value means that the weight was changing.
// (N) after FCN means that FCN was normalized and (eS) means that
// all detectors resolutions was equal 11.
HGeomRotation r;
Char_t          sCond[20] = "OFF";
Char_t          sPar[10]  = "";
FILE         *pFile     = stdout;

    if((pFileName != NULL) && ((pFile = fopen(pFileName, "a")) == NULL))
    {
        Error("print", "Cannot open file: %s for writing", pFileName);
        return;
    }

    if(m_bUseTukay)
    {
        sprintf(sCond, "%6.2f", m_dTukayConst);

        if(m_bChangeWeights)
            strcat(sCond, "(Chw)");
    }

    if((m_bNormalize) || (m_bEqualSigmas))
    {
        sprintf(sPar, "(%s%s)", (m_bNormalize)   ? "N" : "",
                                (m_bEqualSigmas) ? "eS" : "");
    }

    fprintf(pFile, "\nSector: %d %s  Entries: %d  Method: %s%s  Error: %.4f\n"
            "Tukay const: %s  Thrown: %d / %d  FCN: %.7f%s(%d)"
            "  In: %d steps\n",
            m_iSector, (m_iSystem == 0) ? "Shower": "Tof", m_iEntries,
            g_sapMinuitMethods[m_iMethod], (m_bImprove) ? "(I)" : "",
            m_dError, sCond, m_iLastThrown,
            m_iHits, m_dLastChi_2, sPar, m_iStatus, m_iCalls);
    fprintf(pFile, "+-----------+-----------+-----------"
            "+-----------+-----------+-----------+\n"
            "|   Name    |   Fixed   |   Result  "
            "|   Error   |    Step   |   Start   |\n"
            "+-----------+-----------+-----------+"
            "-----------+-----------+-----------+\n"
           );

    for(Int_t i = 0; i < kMETA_ALIGNER_PARAMS; i++)
    {
        fprintf(pFile, "| %7s   |   %4s    | %9.4f | %9.4f | %9.4f | %9.4f |\n",
                        g_sapMetaAlignerParamsNames[i],
                        (m_bFixed[i]) ? "Yes" : "No",
                        m_dParams[i],
                        m_dErrors[i], m_dSteps[i], m_dStartParams[i]);
    }

    fprintf(pFile, "+-----------+-----------+-----------+"
            "-----------+-----------+-----------+\n");
/*
    fprintf(pFile, "Rotation matrix:\n");
    r.setEulerAngles(m_dParams[3], m_dParams[4], m_dParams[5]);
    printf("%8.4f %8.4f %8.4f\n%8.4f %8.4f %8.4f\n%8.4f %8.4f %8.4f\n\n",
            r(0), r(1), r(2), r(3), r(4), r(5), r(6), r(7), r(8));
*/

    if(pFile != stdout)
        fclose(pFile);
}

// -----------------------------------------------------------------------------

void HMetaAligner::printLine(const Char_t *pFileName) const
{
// Print the alignment results in a line.
// pFileName as for print method.
FILE *pFile = stdout;

    if((pFileName != NULL) && ((pFile = fopen(pFileName, "a")) == NULL))
    {
        Error("printLine", "Cannot open file: %s for writing", pFileName);
        return;
    }

    fprintf(pFile, "| %s %d |%9.4f |%9.4f |%9.4f |%9.4f |%9.4f |%9.4f |\n",
            (m_iSystem == 0) ? "Shower": " Tof  ", m_iSector,
            m_dParams[0], m_dParams[1], m_dParams[2],
            m_dParams[3], m_dParams[4], m_dParams[5]);

    if(pFile != stdout)
        fclose(pFile);
}

// -----------------------------------------------------------------------------

void HMetaAligner::printHtmlRow(const Char_t *pFileName) const
{
// Print the results as a HTML table's row.
// The order of the output: detector name, dX, dY, dZ, rotX, rotY, rotZ,
// number of hits not taken into the account / total number of hits, FCN.
// pFileName as for print method.
Int_t   i;
FILE *pFile = stdout;

    if((pFileName != NULL) && ((pFile = fopen(pFileName, "a")) == NULL))
    {
        Error("printHtmlRow", "Cannot open file: %s for writing", pFileName);
        return;
    }

    fprintf(pFile, "<tr align=right><td><tt>%s %d</tt></td>\n",
                    (m_iSystem) ? "Tof" : "Shower", m_iSector);

    for(i = 0; i < 6; i++)
        fprintf(pFile, "\t<td><tt>%.4f</tt></td>\n", m_dParams[i]);

    fprintf(pFile, "\t<td><tt>%d / %d</tt></td>\n\t<td><tt>%.7f</tt></td>\n"
                    "</tr>\n", m_iLastThrown, m_iHits, m_dLastChi_2);

    if(pFile != stdout)
        fclose(pFile);
}

// -----------------------------------------------------------------------------

Double_t HMetaAligner::funcResiduals(HGeomVector *pvDT, HGeomRotation *prDRot)
{
// Calculates the chi^2 from a hit
static HGeomRotation   rRotI;
static HGeomVector     vRc;
static HGeomVector     vAlpha;
HGeomVector           *pvT;
Double_t               dT;
Double_t               dChisq;
Double_t               dC;
Double_t               dChi2X;
Double_t               dChi2Y;
Double_t               dW;

    rRotI = m_arRI[(Int_t)m_fMod];
    pvT   = &m_avT[(Int_t)m_fMod];

    // rotate the matrix if necessary
    if(prDRot != NULL)
        rRotI = *prDRot * rRotI;

    // .... calculations
    // gdT the vectors of the line
    vRc.setXYZ(m_fXc, m_fYc, m_fZc);
    vAlpha.setXYZ(m_fAlphaX, m_fAlphaY, m_fAlphaZ);

    // transform the line's vectors to the local coord. of the detector
    vRc   -= *pvT;
    vRc   -= *pvDT;
    vRc    = rRotI * vRc;
    vAlpha = rRotI * vAlpha;

    // find the intersection of the line with the detector plaine
    dT = -vRc.Z() / vAlpha.Z();

    // find the distance between the hit and the intersection point
    vRc.setX(vRc.X() + vAlpha.X() * dT - m_fXo);
    vRc.setY(vRc.Y() + vAlpha.Y() * dT - m_fYo);
    vRc.setZ(0.0f);

    m_dDX = vRc.X();
    m_dDY = vRc.Y();

    // calculate chi^2
    if(m_bEqualSigmas)
    {
        m_fSigXo = m_fSigYo = 11.0;
    }

    dChi2X = vRc.X() * vRc.X() / (m_fSigXo * m_fSigXo);
    dChi2Y = vRc.Y() * vRc.Y() / (m_fSigYo * m_fSigYo);

    if(m_dWeightChi2X != 0.0)
        dChi2X *= m_dWeightChi2X;

    if(m_dWeightChi2Y != 0.0)
        dChi2Y *= m_dWeightChi2Y;

    m_dChi2X = vRc.X() / m_fSigXo;
    m_dChi2Y = vRc.Y() / m_fSigYo;

    dChisq = 0.5 * (dChi2X + dChi2Y);

    dW = dChisq;
    if((m_bChangeWeights) && (m_dPrevSigma > 0.0))
        dW = dChisq / m_dPrevSigma;

    m_dChi2 = dChisq;

    // check Tukay condition
    if( ! m_bUseTukay)
    {
        m_dChi2W = -2.0f;

        m_dChi2XSum += dChi2X;
        m_dChi2YSum += dChi2Y;

        return dChisq;
    }

    if(dW >= m_dTukayConst_2)
    {
        m_dChi2W = -1.0f;

        m_iLastThrown++;
        return 0.0f;
    }

    m_dChi2XSum += dChi2X;
    m_dChi2YSum += dChi2Y;

    //dC = 1.0 - dChisq / m_dTukayConst_2;
    dC = 1.0 - dW / m_dTukayConst_2;

    m_dWeightsSum0 += dC * dChisq;
    m_dWeightsSum1 += dC;

    dC = dC * dC * dChisq;

    m_dChi2W = dC;

    return dC;
}

// -----------------------------------------------------------------------------

void HMetaAligner::minAlign(Int_t &npar, Double_t *gin, Double_t &f,
                            Double_t *par, Int_t iflag)
{
// Function called by MINUIT.
Double_t              dChisq = 0.0;
Int_t                 i;
HGeomRotation        *pM = NULL;
static HGeomVector    vDT;
static HGeomRotation  rDRotI;
static HGeomRotation  rDRotX;

    vDT.setXYZ(par[0], par[1], par[2]);

    // calculate the matrix if necessary
    if((par[3] != 0.0f) || (par[4] != 0.0f) || (par[5] != 0.0f))
    {
        rDRotX.setEulerAngles(90.0, par[3], -90.0);
        rDRotI.setEulerAngles(0.0, par[4], par[5]);
        rDRotI = rDRotI * rDRotX;
        rDRotI.invert();
        pM = &rDRotI;
    }

    // loop over entries
    m_iHits = 0;
    m_iLastThrown = 0;
    m_dChi2XSum = m_dChi2YSum = 0.0;
    m_dWeightsSum0 = m_dWeightsSum1 = 0.0;
    for(i = 0; i < m_iEntries; i++)
    {
        m_pTuple->GetEntry(i);

        if(((Int_t)m_fSys != m_iSystem) || ((Int_t)m_fSec != m_iSector))
            continue;

        m_iHits++;

        dChisq += funcResiduals(&vDT, pM);
    }

    // normalize chi^2 to the number of entries
    if((m_bNormalize) & (m_iLastThrown != m_iHits))
    {
        dChisq /= (m_iHits - m_iLastThrown);
        m_dChi2XSum /= (m_iHits - m_iLastThrown);
        m_dChi2YSum /= (m_iHits - m_iLastThrown);

        m_dPrevSigma = m_dWeightsSum0 / m_dWeightsSum1;
    }

    // at least half of the points in the range
    if(m_iLastThrown > m_iHits / 2)
        dChisq = 1e+20;

    m_iCalls++;
    if(m_iVerbose)
    {
        printf("%5d:   dT: %8.3f %8.3f %8.3f   dR: %6.3f %6.3f %6.3f\n"
                "\tchi^2 : %10.7f   Thr: %5d / %d\n"
                "\tXchi^2: %10.7f   Ychi^2: %10.7f",
                m_iCalls, par[0], par[1], par[2], par[3], par[4], par[5],
                dChisq, m_iLastThrown, m_iHits,
                m_dChi2XSum, m_dChi2YSum);

                if(m_bChangeWeights)
                    printf("   Sigma: %10.7f\n\n", m_dPrevSigma);
                else
                    printf("\n\n");
    }

    m_dLastChi_2 = dChisq;
    f = dChisq;
}

// -----------------------------------------------------------------------------

void HMetaAligner::writeResults(const Char_t *pFileName)
{
// Function called to write results of the fit to an output file: pFileName
Int_t          i;
HGeomVector    vDT(m_dParams[0], m_dParams[1], m_dParams[2]);
HGeomRotation *pM;
TNtuple       *pTuple;
TFile         *pOut;
HGeomRotation  rDRotI;
HGeomRotation  rDRotX;

    // calculate the matrix if necessary
    if((m_dParams[3] != 0.0f) || (m_dParams[4] != 0.0f)
                || (m_dParams[5] != 0.0f))
    {
        rDRotX.setEulerAngles(90.0, m_dParams[3], -90.0);
        rDRotI.setEulerAngles(0.0, m_dParams[4], m_dParams[5]);
        rDRotI = rDRotI * rDRotX;
        rDRotI.invert();
        pM = &rDRotI;
    }
    else
        pM = NULL;

    // open an output file and create a Ntuple
    pOut = new TFile(pFileName, "RECREATE");
    pTuple = new TNtuple("res", "Results", "xm:ym:dx:dy:sys:c:cw:vx:vy");

    // loop over entries
    m_iHits = 0;
    m_iLastThrown = 0;
    for(i = 0; i < m_iEntries; i++)
    {
        m_pTuple->GetEntry(i);

        if((m_iSystem != (Int_t)m_fSys) || ((Int_t)m_fSec != m_iSector))
            continue;

        m_iHits++;

        funcResiduals(&vDT, pM);
        pTuple->Fill(m_fXo, m_fYo, m_dDX, m_dDY, m_iSystem,
                            m_dChi2, m_dChi2W, m_dChi2X, m_dChi2Y);
    }

    pOut->Write();

    delete pOut;
}
